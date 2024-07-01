#include <stdio.h>
#include <stdbool.h>
#include "module.h"
#include "hardware.h"
#include "rk_gpio.h"

// 初始化GPIO
gpio_init(int gpio, int dir, int value) {
    rk_gpio_export(gpio);
    rk_gpio_set_direction(gpio, dir);
    rk_gpio_set_value(gpio, value);
}

int recycle_gpio_init() {
    // 电磁锁，默认输出低电平上锁
    gpio_init(GPIO_CLEAR_LOCK, GPIO_DIRECTION_OUTPUT, 0);

    // 推杆，默认输出 
    gpio_init(GPIO_PUSH_ROD0, GPIO_DIRECTION_OUTPUT, 0);
    gpio_init(GPIO_PUSH_ROD1, GPIO_DIRECTION_OUTPUT, 0);

    // 柜内灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_INSIDE, GPIO_DIRECTION_OUTPUT, 0);

    // 柜外灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_OUTSIDE, GPIO_DIRECTION_OUTPUT, 0);

    // 风扇，默认输出低电平不工作
    gpio_init(GPIO_FAN, GPIO_DIRECTION_OUTPUT, 0);

    // 电磁锁状态，关闭输入低电平
    gpio_init(GPIO_LOCK_STATUS, GPIO_DIRECTION_INPUT, 1);

    // 红外对射状态，输入低电平有效
    gpio_init(GPIO_IR1_STATUS, GPIO_DIRECTION_INPUT, 1);
    gpio_init(GPIO_IR2_STATUS, GPIO_DIRECTION_INPUT, 1);

    // 接近检测 输入低电平有效
    gpio_init(GPIO_IR2_STATUS, GPIO_DIRECTION_INPUT, 1);
}

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

// 获取溢出状态
int get_overflow_status() {
    int overflow_status = 1;

    if (rk_gpio_get_value(GPIO_IR1_STATUS)){
        overflow_status = 2;
    }

    if (rk_gpio_get_value(GPIO_IR2_STATUS)){
        overflow_status = 3;
    }

    return overflow_status; // 1-3
}

// 设置清运门打开
int set_clear_door_open(){
    rk_gpio_set_value(GPIO_CLEAR_LOCK, 1);
    return 0;
}

// 获取清运门状态
int get_clear_door_status(){
    rk_gpio_get_value(GPIO_LOCK_STATUS);
    return 0;
}

get_rod_status(){
    sleep(2);
    return 0;
}

// 设置推杆开启投递门
int set_post_door_open(){
    rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 1);

    if (get_rod_status()) {
        rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
        rk_gpio_set_value(GPIO_PUSH_ROD1, 0);
    }
    return 0;
}

// 设置推杆关闭投递门
int set_post_door_close(){
    rk_gpio_set_value(GPIO_PUSH_ROD0, 1);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 0);

    if (get_rod_status()) {
        rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
        rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    }
    return 0;
}

// 外部灯光操作
int outside_light(int status) {
    if (status == 1){
        rk_gpio_set_value(GPIO_LIGHT_OUTSIDE, 1);
        printf("Outside Light is ON\n");
    } else {
        rk_gpio_set_value(GPIO_LIGHT_OUTSIDE, 0);
        printf("Outside Light is OFF\n");
    }
}

// 内部灯光操作
int inside_light(int status) {
    if (status == 1){
        rk_gpio_set_value(GPIO_LIGHT_INSIDE, 1);
        printf("Inside Light is ON\n");
    } else {
        rk_gpio_set_value(GPIO_LIGHT_INSIDE, 0);
        printf("Inside Light is OFF\n");
    }
}
