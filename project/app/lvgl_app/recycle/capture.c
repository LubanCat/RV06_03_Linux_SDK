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
#include <dirent.h>
#include <pthread.h>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"
#include "rkmuxer.h"
#define RTMP_URL "rtmp://10.255.200.135/live/"
int rtmp_start;

#define COUNT 1
#define FORMATE RK_FMT_RGB888

int capture_rtmp_init(video_frame_t *video_frame,char *rtmp_url){
    VideoParam g_video_param;
    memset(&g_video_param, 0, sizeof(g_video_param));
    g_video_param.level = 52;
    g_video_param.width=video_frame->width;
    g_video_param.height=video_frame->height;
    g_video_param.bit_rate=24*1024;
    g_video_param.frame_rate_den=1;
    g_video_param.frame_rate_num = 14;
    g_video_param.profile=77;
    memcpy(g_video_param.codec, "H.264", strlen("H.264"));
    memcpy(g_video_param.format, "NV12", strlen("NV12"));

	rkmuxer_init(3, "flv", rtmp_url, &g_video_param, NULL);

    return 0;
}

int capture_rtmp_deinit(){
    rkmuxer_deinit(3);
}

int capture_rtmp_write_video_frame(char *buffer,int buffer_size,int64_t present_time, int key_frame){
    rkmuxer_write_video_frame(3, buffer, buffer_size, present_time, key_frame);
}

int capture_storage_init(video_frame_t *video_frame){


    char *file_name="/root/media/record.mp4";
    FILE *file = fopen(file_name, "w");
    fclose(file);
    VideoParam g_video_param;
    AudioParam g_audio_param;
    g_video_param.level = 52;
    g_video_param.width=video_frame->width;
    g_video_param.height=video_frame->height;
    g_video_param.bit_rate=24*1024;
    g_video_param.frame_rate_den=1;
    g_video_param.frame_rate_num = 14;
    g_video_param.profile=77;
    memcpy(g_video_param.codec, "H.264", strlen("H.264"));
    memcpy(g_video_param.format, "NV12", strlen("NV12"));

    g_audio_param.sample_rate=16000;
    g_audio_param.frame_size=1024;
	rkmuxer_init(0, NULL,file_name,&g_video_param,&g_audio_param);
    return 0;
}

int capture_storage_deinit(){
    rkmuxer_deinit(0);
}

int capture_storage_write_video_frame(char *buffer,int buffer_size,int64_t present_time, int key_frame){
    rkmuxer_write_video_frame(0, buffer, buffer_size, present_time, key_frame);
}

int capture_start(video_frame_t *video_frame)
{
    int ret = 0;
    video_frame->fd = open(video_frame->device, O_RDWR);
    if (video_frame->fd == -1) {
        printf("打开摄像头失败: %s\n",video_frame->device);
        return -1;
    }
    struct v4l2_streamparm streamparam;
    // 设置摄像头格式为MJPEG
    struct v4l2_format fmt = {0};
    memset(&fmt, 0, sizeof(fmt));
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
        return -1;
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

int capture_stop(video_frame_t *video_frame)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //采集结束
    if(ioctl(video_frame->fd, VIDIOC_STREAMOFF, &type)<0){
        perror("stop fail");
        return -1;
    }
    //释放映射空间
    munmap(video_frame->start, video_frame->length);
    close(video_frame->fd); 
    return 1;
}

int video_linux_get_frame_rgb(video_frame_t *video_frame,char *data,int *length)
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

    int row = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(data + row * row_stride, buffer[0], row_stride);
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

int capture_jpg_compass(char *device,int width,int height,char *output_path){
    video_frame_t video_frame;
    video_frame.width=width;
    video_frame.height=height;
    sprintf(video_frame.device,"%s",device);
    int ret = capture_start(&video_frame);
    if(ret < 0){
        capture_stop(&video_frame);
        return -1;
    }

    char *data = malloc(width*height*3);
    int length;

    video_linux_get_frame_rgb(&video_frame,data,&length);
    capture_stop(&video_frame);

    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;
    void *pVirAddr = RK_NULL;
    VIDEO_FRAME_INFO_S stFrame;
    VENC_STREAM_S rcv;
    int buffersize;

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

    FILE *fp = fopen(output_path, "wb+");
    rcv.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
    ret = RK_MPI_VENC_GetStream(0, &rcv, 3000);
    if (ret == RK_SUCCESS) {
		void *data = RK_MPI_MB_Handle2VirAddr(rcv.pstPack->pMbBlk);
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
    return 0;
}

int capture_venc_init(video_frame_t *video_frame){
    int width=video_frame->width;
    int height=video_frame->height;
    RK_S32 is_end = 0;
    int ret;

    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;

    //初始化mpi
    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&pstRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));

    //h264
    stAttr.stVencAttr.enType = RK_VIDEO_ID_AVC;
    stAttr.stVencAttr.u32PicWidth = width;
    stAttr.stVencAttr.u32PicHeight = height;
    stAttr.stVencAttr.u32VirWidth = width;
    stAttr.stVencAttr.u32VirHeight = height;    
    stAttr.stVencAttr.u32MaxPicWidth = width;
    stAttr.stVencAttr.u32MaxPicHeight = height;
    stAttr.stVencAttr.u32StreamBufCnt = 3;
    stAttr.stVencAttr.u32Profile = H264E_PROFILE_MAIN;
    stAttr.stVencAttr.u32BufSize = width * height *3 /2;
    stAttr.stVencAttr.enPixelFormat = FORMATE;
    stAttr.stVencAttr.enMirror = MIRROR_NONE;

    stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
    stAttr.stRcAttr.stH264Cbr.u32Gop = 14;
    stAttr.stRcAttr.stH264Cbr.u32SrcFrameRateDen=1;
    stAttr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 14;
    stAttr.stRcAttr.stH264Cbr.fr32DstFrameRateDen=1;
    stAttr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 14;    
    stAttr.stRcAttr.stH264Cbr.u32BitRate = 24*1024;
    stAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;

    //初始化通道
    ret = RK_MPI_VENC_CreateChn(0,&stAttr);
    if(ret < 0)
        printf("CreateChn ret = 0x%X\n",ret);
    pstRecvParam.s32RecvPicNum=-1;
    ret = RK_MPI_VENC_StartRecvFrame(0, &pstRecvParam);
    if(ret < 0)
        printf("StartRecvFrame ret = 0x%X\n",ret);
}

int capture_rgb_to_avc(video_frame_t *video_frame,char *data){
    int width=video_frame->width;
    int height=video_frame->height;
    RK_S32 is_end = 0;
    int ret;

    RK_S32 s32Ret = RK_SUCCESS;
    void *pVirAddr = RK_NULL;
    VIDEO_FRAME_INFO_S stFrame;

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
    
    int buffersize= stMbPicCalResult.u32MBSize;

    memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
    MB_BLK blk = RK_NULL;
    ret = RK_MPI_SYS_MmzAlloc(&blk, RK_NULL, RK_NULL,buffersize);
    if (RK_NULL == blk) {
        printf("RK_MPI_MB_GetMB fail\n");
        usleep(2000llu);
    }

    pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);

    RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);
    stFrame.stVFrame.pMbBlk = blk;
    stFrame.stVFrame.u32Width = width;
    stFrame.stVFrame.u32Height = height;
    stFrame.stVFrame.u32VirWidth = width;
    stFrame.stVFrame.u32VirHeight = height;
    stFrame.stVFrame.enPixelFormat = FORMATE;
    stFrame.stVFrame.u32FrameFlag |= is_end ? FRAME_FLAG_SNAP_END : 0;
    
    memcpy(pVirAddr,data,width * height * 3);
    ret = RK_MPI_VENC_SendFrame(0, &stFrame, -1);

    RK_MPI_SYS_Free(blk);
    return 0;
}

int capture_venc_deinit(){
    RK_MPI_VENC_StopRecvFrame(0);
    RK_MPI_VENC_DestroyChn(0);
}

int capture_rgb_to_jpg(video_frame_t *video_frame,char *data,char *output_path){
    int width=video_frame->width;
    int height=video_frame->height;
    int ret;

    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;
    void *pVirAddr = RK_NULL;
    VIDEO_FRAME_INFO_S stFrame;
    VENC_STREAM_S rcv;
    int buffersize;

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

    //jpg
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
    ret = RK_MPI_VENC_CreateChn(1,&stAttr);
    if(ret < 0)
        printf("CreateChn ret = 0x%X\n",ret);
    pstRecvParam.s32RecvPicNum=1;
    ret = RK_MPI_VENC_StartRecvFrame(1, &pstRecvParam);
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

    ret = RK_MPI_VENC_SendFrame(1, &stFrame, -1);

    FILE *fp = fopen(output_path, "wb+");
    rcv.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
    ret = RK_MPI_VENC_GetStream(1, &rcv, 3000);
    if (ret == RK_SUCCESS) {
		void *data = RK_MPI_MB_Handle2VirAddr(rcv.pstPack->pMbBlk);
		fwrite(data, 1, rcv.pstPack->u32Len, fp);
        printf("pic size = %d\n",rcv.pstPack->u32Len);
        fflush(fp);
        fclose(fp);
    }
    RK_MPI_SYS_Free(blk);
    RK_MPI_VENC_ReleaseStream(1, &rcv);
    RK_MPI_VENC_StopRecvFrame(1);
    RK_MPI_VENC_DestroyChn(1);
    return 0;
}

int get_cam_video(int ch,char *video_path){
    DIR *d;
    struct dirent *dir;
    char path[64];
    if(ch)
        //外摄像头
        sprintf(path,"%s","/sys/bus/usb/devices/1-1/1-1.4/1-1.4:1.0/video4linux");
    else
        //内摄像头
        sprintf(path,"%s","/sys/bus/usb/devices/1-1/1-1.2/1-1.2:1.0/video4linux");

    d = opendir(path); // 请替换为你要列出的目录路径
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(strcmp(dir->d_name,"video0")==0){
                strcpy(video_path,"/dev/video0");
                closedir(d);
                return 0;
            }else if(strcmp(dir->d_name,"video2")==0){
                strcpy(video_path,"/dev/video2");
                closedir(d);
                return 0;
            }
        }
        video_path=NULL;
        closedir(d);
    } else {
        perror("无法打开目录");
        video_path=NULL;
        return -2;
    }
    video_path=NULL;
    return -1;
}
