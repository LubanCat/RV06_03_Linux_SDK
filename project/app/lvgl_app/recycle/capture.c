#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <sys/times.h>
#include <time.h> 
#include <jpeglib.h>
#include "capture.h"

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"

#define COUNT 1
#define FORMATE RK_FMT_RGB888

int capture_start(video_frame_t *video_frame)
{
    video_frame->fd = open(video_frame->device, O_RDWR);
    if (video_frame->fd == -1) {
        printf("打开摄像头失败: %s\n",video_frame->device);
        return -1;
    }
    struct v4l2_streamparm streamparam;
    // 设置摄像头格式为MJPEG
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = video_frame->width; // 设置所需的宽度
    fmt.fmt.pix.height = video_frame->height; // 设置所需的高度
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; // 设置为MJPEG格式
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(video_frame->fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("设置摄像头格式失败");
        close(video_frame->fd);
        return -1;
    }

    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = ioctl(video_frame->fd, VIDIOC_G_FMT, &vfmt);
    if(ret < 0){
        perror("Get FMT fail!");
        return -1;
    }

    // 获取帧率参数
    streamparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(video_frame->fd, VIDIOC_G_PARM, &streamparam) < 0) {
        perror("获取帧率参数失败");
        close(video_frame->fd);
        return -1;
    }

    struct v4l2_requestbuffers reqbuf;
    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.count = COUNT;//申请缓存区个数
    reqbuf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(video_frame->fd, VIDIOC_REQBUFS, &reqbuf);
    if(ret < 0){
        perror("request fail");
        return -1;    
    }

    //把内核空间映射到用户空间
    struct v4l2_buffer kbuf;
    memset(&kbuf, 0, sizeof(kbuf));
    //把申请的缓存区空间一个一个取出来与用户空间映射
    kbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    kbuf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(video_frame->fd, VIDIOC_QUERYBUF, &kbuf)<0){
        perror("query buf fail");
    }    
    video_frame->length = kbuf.length;
    video_frame->start = (char *)mmap(NULL, kbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, video_frame->fd, kbuf.m.offset);
    if(video_frame->start == MAP_FAILED){
        perror("mmap fail");
        return -1;
    }

    //缓存入队
    if(ioctl(video_frame->fd, VIDIOC_QBUF, &kbuf) < 0){
        perror("qbuf fail");
        return -1;
    }

    //启动摄像头数据采集
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(video_frame->fd, VIDIOC_STREAMON, &type)<0){
        perror("start fail");
        return -1;
    }
    return 1;
}

int video_linux_get_frame(video_frame_t *video_frame)
{
    //出队
    struct v4l2_buffer readbuf;
    readbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(video_frame->fd, VIDIOC_DQBUF, &readbuf) < 0){
        perror("dqbuf fail");
    }
    // memcpy(data, video_frame->start, video_frame->length);
    // length[0] = readbuf.length;
    //入队
    if(ioctl(video_frame->fd, VIDIOC_QBUF, &readbuf) < 0){
        perror("qbuf fail");
    }

    return 1;
}


int video_linux_get_frame_rgb(video_frame_t *video_frame,char **data,int *length)
{
    //出队
    struct v4l2_buffer readbuf;
    readbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(video_frame->fd, VIDIOC_DQBUF, &readbuf) < 0){
        perror("dqbuf fail");
    }
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPARRAY buffer; // Output row buffer
    unsigned long row_stride; // Physical row width in output buffer

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, video_frame->start, video_frame->length);

    jpeg_read_header(&cinfo, TRUE);
    // 开始解码
    jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width *cinfo.output_components;
    length[0]=row_stride*cinfo.output_height;

    buffer = (*cinfo.mem->alloc_sarray)
              ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    *data = (unsigned char *) malloc(cinfo.output_height * row_stride);
    if (*data == NULL) {
        printf("内存分配失败\n");
        jpeg_destroy_decompress(&cinfo);
    }
    int row = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(*data + row * row_stride, buffer[0], row_stride);
        row++;
    }

    jpeg_finish_decompress(&cinfo);

    // 释放 JPEG 解码器
    jpeg_destroy_decompress(&cinfo);

    if(ioctl(video_frame->fd, VIDIOC_QBUF, &readbuf) < 0){
        perror("qbuf fail");
    }

    return 1;
}

int capture_stop(video_frame_t *video_frame)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //采集结束
    if(ioctl(video_frame->fd, VIDIOC_STREAMOFF, &type)<0){
        perror("stop fail");
        return -1;
    }
    //释放映射空间
    int i=0;
    munmap(video_frame->start, video_frame->length);
    close(video_frame->fd); 
    return 1;
}


int capture_jpg(char *device,int width,int height,char* output_path){
    video_frame_t video_frame;
    video_frame.width=width;
    video_frame.height=height;
    sprintf(video_frame.device,"%s",device);
    int ret = capture_start(&video_frame);
    if(ret < 0 ){
        return -1;
    }

    int afd = open(output_path, O_WRONLY | O_CREAT, 0644);
    if (afd == -1) {
        perror("open");
        return -1;
    }
    video_linux_get_frame(&video_frame);
    // 写入数组内容到文件
    if (write(afd, video_frame.start, video_frame.length) != video_frame.length) {
        perror("write");
        close(afd);
        return -1;
    }
    close(afd);
    capture_stop(&video_frame);
}


int capture_jpg_compass(char *device,int width,int height,char *output_path){
    video_frame_t video_frame;
    video_frame.width=width;
    video_frame.height=height;
    sprintf(video_frame.device,"%s",device);
    int ret = capture_start(&video_frame);
    if(ret < 0 ){
        return -1;
    }

    char *data;
    int length;
    video_linux_get_frame_rgb(&video_frame,&data,&length);
    capture_stop(&video_frame);

    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;
    void *pVirAddr = RK_NULL;
    VIDEO_FRAME_INFO_S stFrame;
    VENC_STREAM_S rcv;
    int buffersize;

    char *src_buf;
    //初始化mpi
    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = width;
    stPicBufAttr.u32Height = height;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)FORMATE;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        printf("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    
    buffersize= stMbPicCalResult.u32MBSize;
    printf("calc picture size: %d\n", buffersize);
    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&pstRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));

    //h264
    stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
    stAttr.stVencAttr.u32PicWidth = width;
    stAttr.stVencAttr.u32PicHeight = height;
    stAttr.stVencAttr.u32VirWidth = width;
    stAttr.stVencAttr.u32VirHeight = height;    
    stAttr.stVencAttr.u32MaxPicWidth = width;
    stAttr.stVencAttr.u32MaxPicHeight = height;
    stAttr.stVencAttr.u32StreamBufCnt = 2;
    stAttr.stVencAttr.u32Profile = 0;
    stAttr.stVencAttr.u32BufSize = width * height;
    stAttr.stVencAttr.enPixelFormat = FORMATE;
    stAttr.stVencAttr.bByFrame = RK_TRUE;
    stAttr.stVencAttr.enMirror = MIRROR_NONE;
    stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
    stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
    stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
    
    //初始化通道
    ret = RK_MPI_VENC_CreateChn(0,&stAttr);
    if(ret < 0)
        printf("CreateChn ret = 0x%X\n",ret);
    pstRecvParam.s32RecvPicNum=1;
    ret = RK_MPI_VENC_StartRecvFrame(0, &pstRecvParam);
    if(ret < 0)
        printf("StartRecvFrame ret = 0x%X\n",ret);

    memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
    MB_BLK blk = RK_NULL;
    ret = RK_MPI_SYS_MmzAlloc(&blk, RK_NULL, RK_NULL,buffersize);
    if (RK_NULL == blk) {
        printf("RK_MPI_MB_GetMB fail\n");
        usleep(2000llu);
    }

    pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);

    memcpy(pVirAddr,data,width * height * 3);

    RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);

    stFrame.stVFrame.pMbBlk = blk;
    stFrame.stVFrame.u32Width = width;
    stFrame.stVFrame.u32Height = height;
    stFrame.stVFrame.u32VirWidth = width;
    stFrame.stVFrame.u32VirHeight = height;
    stFrame.stVFrame.enPixelFormat = FORMATE;
    stFrame.stVFrame.u32FrameFlag |= FRAME_FLAG_SNAP_END;

    ret = RK_MPI_VENC_SendFrame(0, &stFrame, -1);
    src_buf=(char *)malloc(width*height*3);

    FILE *fp = fopen(output_path, "wb+");
    rcv.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
    ret = RK_MPI_VENC_GetStream(0, &rcv, 3000);
    if (ret == RK_SUCCESS) {
		void *data = RK_MPI_MB_Handle2VirAddr(rcv.pstPack->pMbBlk);
        memcpy(src_buf,data,rcv.pstPack->u32Len);
		fwrite(data, 1, rcv.pstPack->u32Len, fp);
        printf("pic size = %d\n",rcv.pstPack->u32Len);
        fflush(fp);
        fclose(fp);
    }
    free(data);
    RK_MPI_SYS_Free(blk);
    RK_MPI_VENC_ReleaseStream(0, &rcv);
    RK_MPI_VENC_StopRecvFrame(0);
    RK_MPI_VENC_DestroyChn(0);
    return -1;
}

int get_video_in_out(int ch){
    char path[64];
    sprintf(path,"/sys/class/video4linux/video%d/device/uevent",ch);
    char str[384];

    // 打开文件
    FILE *file = fopen(path, "r");

    // 检查文件是否成功打开
    if (file == NULL) {
        perror("无法打开文件"); // perror 用于打印错误信息
        return -1;
    }

    size_t  bytesRead = fread(str, 1, sizeof(str), file);

    // 分割字符串的标记
    const char *delimiter = "\n";
    char *str_copy = strdup(str); // 复制字符串，因为strtok会修改原始字符串
    char *token = strtok(str_copy, delimiter);

    char product[32]; // 存储PRODUCT值的数组

    // 遍历udev信息的每个字段
    while (token != NULL) {
        // 检查是否是PRODUCT行
        if (strncmp(token, "PRODUCT=", 8) == 0) {
            // 提取PRODUCT值
            strcpy(product, token + 8);
            break;
        }
        token = strtok(NULL, delimiter);
    }

    free(str_copy); // 释放复制的字符串

    char *last_slash = strrchr(product, '/'); // 查找最后一个斜杠

    if (last_slash != NULL) {
        *last_slash = '\0'; 
    } else {
        printf("No slash found in the string.\n");
    }

    if(strcmp(product,"c45/6366")==0)
        return 2;
    else if(strcmp(product,"5a3/9230")==0)
        return 1;
    else
        return 0;
}
