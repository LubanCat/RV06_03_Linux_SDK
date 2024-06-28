#include <stdio.h>
#include "module.h"

// 获取当前重量
double get_current_weight() {
    // 从硬件或其他数据源获取当前重量
    return 12.5; // 示例值
}

// 获取超重状态
int get_overflow_status() {
    return 2; // 1-3
}

// 设置清运门打开
int set_clear_door_open(){
    return 0;
}

// 获取清运门状态
int get_clear_door_status(){
    return 0;
}

// 设置推杆开启投递门
int set_post_door_open(){
    return 0;
}

// 设置推杆关闭投递门
int set_post_door_close(){
    
    close_door();
    return 0;
}
