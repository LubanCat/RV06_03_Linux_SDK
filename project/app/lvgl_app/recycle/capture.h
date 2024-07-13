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


int capture_start(video_frame_t *video_frame);
int video_linux_get_frame(video_frame_t *video_frames);
int video_linux_get_frame_rgb(video_frame_t *video_frame,char **data,int *length);
int capture_stop(video_frame_t *video_frame);
int capture_jpg(char *device,int width,int height,char* output_path);
int get_video_in_out(int ch);
#endif // HARDWARE_H
