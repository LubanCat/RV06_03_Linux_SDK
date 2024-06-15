#ifndef PTZCMD_H
#define PTZCMD_H

#include <stdint.h>

typedef struct {
    int leftRight; // 镜头左移右移 0:停止 1:左移 2:右移
    int upDown;    // 镜头上移下移 0:停止 1:上移 2:下移
    int zoomInOut; // 镜头放大缩小 0:停止 1:缩小 2:放大
    int moveSpeedH;// 水平镜头移动速度 (0-255)
    int moveSpeedV;// 竖直镜头移动速度 (0-255)
    int zoomSpeed; // 镜头缩放速度 (0-255)
} PtzCommand;

int parseCmdString(const char *cmdStr, PtzCommand *cmd);

int motor_run_relative(int x_steps, int y_steps);

int motor_init(void);

#endif  // PTZCMD_H
