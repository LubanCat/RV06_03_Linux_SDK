#ifndef CAPTURE_H
#define CAPTURE_H
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

typedef struct {
    unsigned int length;
    char *start;
    int width;
    int height;
    char device[15];
    int fd;
} video_frame_t;

typedef struct {
    video_frame_t video_frame;
    int live;
    int err;
    int avc_init_flag;
    int record_flag;
    int rtmp_flag;
    int test_flag;
    int photo_flag;
    int preview_flag;
    int photo_opendoor;
} video_device_t;

int capture_h264_compass(char *device,int width,int height,char *output_path);
int capture_rtmp_init(video_frame_t *video_frame,char *rtmp_url);
int capture_rtmp_deinit();
int capture_rtmp_write_video_frame(char *buffer,int buffer_size,int64_t present_time, int key_frame);
int capture_start(video_frame_t *video_frame);
int video_linux_skip_frame(video_frame_t *video_frame,int time);
int video_linux_get_frame_rgb(video_frame_t *video_frame,char *data,int *length);
int capture_stop(video_frame_t *video_frame);
int capture_jpg_compass(char *device,int time,int width,int height,char *output_path);
int capture_jpg_compass_osd(char *device,int capture_time,int width,int height,char *output_path,int type);
int get_cam_video(int ch,char *video_path);
int reset_usb(int bus_num,int dev_num);
int reset_cam(int ch);

#endif // HARDWARE_H
