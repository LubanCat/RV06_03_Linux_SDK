#ifndef HARDWARE_H
#define HARDWARE_H

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
