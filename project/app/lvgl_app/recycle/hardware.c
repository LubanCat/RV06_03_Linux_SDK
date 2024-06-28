#include <stdio.h>
#include <stdbool.h>
#include "module.h"

// 随机数生成
double generate_random_number() {
    // 初始化随机数生成器
    srand(time(NULL));
    // 生成两位整数部分，范围 10 到 99
    int integer_part = rand() % 90 + 10;
    // 生成小数部分，范围 0 到 99
    int fractional_part = rand() % 100;
    // 组合整数部分和小数部分
    double random_number = integer_part + fractional_part / 100.0;
    return random_number;
}


// 获取当前重量
double get_current_weight() {
    // 需要修改成从硬件获取重量
    double weight = generate_random_number();
    return weight; 
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
    return 0;
}

// 外部灯光操作
int outside_light(int status) {
    if (status == 1){
        printf("Outside Light is ON\n");
    } else {
        printf("Outside Light is OFF\n");
    }
}

// 内部灯光操作
int inside_light(int status) {
    if (status == 1){
        printf("Inside Light is ON\n");
    } else {
        printf("Inside Light is OFF\n");
    }
}
