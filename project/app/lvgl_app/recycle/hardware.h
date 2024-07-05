#ifndef HARDWARE_H
#define HARDWARE_H

#include "rk_gpio.h"

#define msleep(x) usleep(x * 1000)

#define GPIO_LOCK_CEN           5
#define GPIO_PUSH_ROD0          2
#define GPIO_PUSH_ROD1          3
#define GPIO_LIGHT_INSIDE       1
#define GPIO_LIGHT_OUTSIDE      0
#define GPIO_FAN                118

#define GPIO_LOCK_INT           4
#define GPIO_IR1_STATUS         135
#define GPIO_IR2_STATUS         133
#define GPIO_PEOPLE_CHECK       123


// 获取当前重量
double get_current_weight();

// 获取超重状态
int get_overflow_status();

// 设置清运门打开
int set_clear_door_open();

// 获取清运门状态
int get_clear_door_status();

// 设置推杆开启投递门
int set_post_door_open();

// 设置推杆关闭投递门
int set_post_door_close();

// 内部灯光操作
int inside_light(int status);

// 外部灯光操作
int outside_light(int status);

#endif // HARDWARE_H
