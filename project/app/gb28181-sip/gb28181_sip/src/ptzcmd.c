#include "ptzcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

int motor_run(int x_steps, int y_steps)
{
    int fd;
    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;

    // 打开电机设备
    fd = open(MOTOR_DEV, O_RDWR);
    if (fd == -1) {
        perror("open motor");
        return -1;
    }

    // 设置新的步数
    motors_steps.input_x_steps = x_steps;
    motors_steps.input_y_steps = y_steps;
    printf("移动步数设置 x=%d, y=%d\n", motors_steps.input_x_steps, motors_steps.input_y_steps);
    ioctl(fd, MOTOR_MOVE, (unsigned long)&motors_steps);

    usleep(100000); // 等待100毫秒
    // 等待电机完成移动
    do {
        ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        // 检查电机是否完成移动
        if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
            printf("电机移动完成,当前位置：x=%d, y=%d\n", get_motor_message.x_cur_steps, get_motor_message.y_cur_steps);
            break;
        }
        // 如果电机还在移动，则等待一段时间后重试
        usleep(100000); // 等待100毫秒
    } while (1);

    // 关闭设备
    close(fd);
    return 0;
}

int parseCmdString(const char *cmdStr, PtzCommand *cmd) {
    if (strlen(cmdStr) != 16) {
        printf("Invalid command string length: %zu\n", strlen(cmdStr));
        return -1;  // Invalid command string length
    }

    int cmdCode, moveSpeed, zoomSpeed;
    int x_steps=0, y_steps=0;

    sscanf(cmdStr + 6, "%02X", &cmdCode);
    sscanf(cmdStr + 8, "%02X", &moveSpeed);
    sscanf(cmdStr + 12, "%1X", &zoomSpeed);

    printf("Parsed values - cmdCode: %02X, moveSpeed: %02X, zoomSpeed: %1X\n", cmdCode, moveSpeed, zoomSpeed);

    cmd->moveSpeed = moveSpeed;
    cmd->zoomSpeed = zoomSpeed;

    cmd->leftRight = 0;
    if (cmdCode & 0x01) {
        cmd->leftRight = 2;  // 右移
        x_steps = 1000;
        motor_run(x_steps, y_steps);
    } else if (cmdCode & 0x02) {
        cmd->leftRight = 1;  // 左移
        x_steps = -1000;
        motor_run(x_steps, y_steps);
    }

    cmd->upDown = 0;
    if (cmdCode & 0x04) {
        cmd->upDown = 2;  // 下移
        y_steps = 200;
        motor_run(x_steps, y_steps);
    } else if (cmdCode & 0x08) {
        cmd->upDown = 1;  // 上移
        y_steps = -200;
        motor_run(x_steps, y_steps);
    }

    cmd->inOut = 0;
    if (cmdCode & 0x10) {
        cmd->inOut = 2;  // 放大
    } else if (cmdCode & 0x20) {
        cmd->inOut = 1;  // 缩小
    }

    printf("Parsed command - leftRight: %d, upDown: %d, inOut: %d, moveSpeed: %d, zoomSpeed: %d\n",
           cmd->leftRight, cmd->upDown, cmd->inOut, cmd->moveSpeed, cmd->zoomSpeed);

    return 0;
}
