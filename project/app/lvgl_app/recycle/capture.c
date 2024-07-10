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

#define COUNT 1

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

int video_linux_get_frame(video_frame_t *video_frame ,char *data,int* length)
{
    //出队
    struct v4l2_buffer readbuf;
    readbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(video_frame->fd, VIDIOC_DQBUF, &readbuf) < 0){
        perror("dqbuf fail");
        return 0;
    }
    memcpy(data, video_frame->start, video_frame->length);
    length[0] = readbuf.length;
    //入队
    if(ioctl(video_frame->fd, VIDIOC_QBUF, &readbuf) < 0){
        perror("qbuf fail");
        return 0;
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
        return 0;
    }
    // memcpy(data, video_frame->start, video_frame->length);
    //入队
    if(ioctl(video_frame->fd, VIDIOC_QBUF, &readbuf) < 0){
        perror("qbuf fail");
        return 0;
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
        return 1;
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

    return 1;
}

int capture_stop(video_frame_t *video_frame)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //采集结束
    if(ioctl(video_frame->fd, VIDIOC_STREAMOFF, &type)<0){
        perror("stop fail");
        return 0;
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
    capture_start(&video_frame);

    char *data;
    data=malloc(video_frame.height*video_frame.width*2);
    int length;

    int afd = open(output_path, O_WRONLY | O_CREAT, 0644);
    if (afd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    video_linux_get_frame(&video_frame,data,&length);
    // 写入数组内容到文件
    if (write(afd, data, length) != length) {
        perror("write");
        close(afd);
        exit(EXIT_FAILURE);
    }
    close(afd);
    free(data);
    capture_stop(&video_frame);
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
        exit(EXIT_FAILURE);
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
