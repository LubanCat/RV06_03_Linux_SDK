#include "ptzcmd.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

extern int cruise_group;

int parseCmdString(const char *cmdStr, PtzCommand *cmd) {
    if (strlen(cmdStr) != 16) {
        printf("Invalid command string length: %zu\n", strlen(cmdStr));
        return -1;  // Invalid command string length
    }

    int cmdCode;
    int data_byte5, data_byte6, data_byte7;

    // 读取指令位
    sscanf(cmdStr + 6, "%02X", &cmdCode);
    sscanf(cmdStr + 8, "%02X", &data_byte5);
    sscanf(cmdStr + 10, "%02X", &data_byte6);
    sscanf(cmdStr + 12, "%02X", &data_byte7);

    printf("Parsed values - cmdCode:%02X, data_byte5:%02X, data_byte6:%02X, data_byte7:%02X,\n", cmdCode, data_byte5, data_byte6, data_byte7);

    // 0b00000000 PTZ的所有操作均停止
    if (cmdCode == 0){
        // 停止巡航
        cruise_group=-1;
    }
    // 0b00xxxxxx PTZ指令
    else if ((cmdCode >> 6) == 0){
        int x_steps=0, y_steps=0;

        cmd->moveSpeedH = data_byte5;
        cmd->moveSpeedV = data_byte6;
        cmd->zoomSpeed = data_byte7;
        cmd->leftRight = cmdCode & 0x03;       // 最低的两位比特 (00000011)
        cmd->upDown = (cmdCode >> 2) & 0x03; // 第二组两位比特 (00001100)
        cmd->zoomInOut = (cmdCode >> 4) & 0x03; // 第三组两位比特 (00110000)
        // int bitGroup4 = (cmdCode >> 6) & 0x03; // 最高的两位比特 (11000000)

        if (cmd->leftRight == 1) {
            x_steps = 1000; // 右
        } else if (cmd->leftRight == 2) {
            x_steps = -1000; // 左
        }

        if (cmd->upDown == 1) {
            y_steps = 200;  // 下
        } else if (cmd->upDown == 2) {
            y_steps = -200; // 上
        }

        if (cmd->zoomInOut == 1) {
            // 放大
        } else if (cmd->zoomInOut == 2) {
            // 缩小
        }

        motor_run_relative(x_steps, y_steps);
    }
    // 0b0100xxxx FI指令
    else if ((cmdCode >> 4) == 4) {
        // FI
    }
    // 预置位指令
    else if (cmdCode >= 0x81 && cmdCode <= 0x83) {
        int index = data_byte6;
        if (cmdCode == 0x81 ){
            // 设置新的预置位示例
            set_preset(index);
            // print_presets();  // 设置后打印预置位
        }
        if (cmdCode == 0x82 ){
            // 调用
            move_to_preset(index);
        }
        if (cmdCode == 0x83 ){
            // 删除
            delete_preset(index);
            // print_presets();  // 设置后打印预置位
        }
    }
    // 巡航指令
    else if (cmdCode >= 0x84 && cmdCode <= 0x88) {
        int group = data_byte5;
        int point = data_byte6;
        unsigned int data = ((data_byte7 >> 4) << 8) | data_byte6;

        if (cmdCode == 0x84 ){
            // 加入巡航点
            set_cruise_point(group, point);
            print_cruise();
        }
        if (cmdCode == 0x85 ){
            // 删除巡航点
            delete_cruise(group, point);
            print_cruise();
        }
        if (cmdCode == 0x86 ){
            // 设置巡航速度
            set_cruise_speed(group, data);
            print_cruise();
        }
        if (cmdCode == 0x87 ){
            // 设置巡航停留时间
            set_cruise_time(group, data);
            print_cruise();
        }
        if (cmdCode == 0x88 ){
            // 开始巡航
            printf("开始巡航: 巡航组=%d",group);
            cruise_group=group; //传递给motor线程
        }
    }
    // 扫描指令
    else if (cmdCode >= 0x89 && cmdCode <= 0x8a) {

    }
    else if (cmdCode == 0x8c && cmdCode == 0x8d) {

    }

    return 0;
}
