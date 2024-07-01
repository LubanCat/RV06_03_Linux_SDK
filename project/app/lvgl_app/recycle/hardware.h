#ifndef HARDWARE_H
#define HARDWARE_H

#include "rk_gpio.h"

#define GPIO_CLEAR_LOCK         43
#define GPIO_PUSH_ROD0          49
#define GPIO_PUSH_ROD1          50
#define GPIO_LIGHT_INSIDE       52
#define GPIO_LIGHT_OUTSIDE      53
#define GPIO_FAN                58

#define GPIO_LOCK_STATUS        44
#define GPIO_IR1_STATUS         55
#define GPIO_IR2_STATUS         56
#define GPIO_NEAR_STATUS        102


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
