#include <stdio.h>
#include <stdbool.h>
#include "module.h"
#include "hardware.h"
#include "rk_gpio.h"

Recycle_Box_Cnf g_sRecycle_Box_Cnf;
int rod_hinder_value = 280;

// 初始化GPIO
gpio_init(int gpio, int dir, int value) {
    rk_gpio_export(gpio);
    rk_gpio_set_direction(gpio, dir);
    rk_gpio_set_value(gpio, value);
}

int recycle_gpio_init() {
    // 电磁锁，默认输出低电平上锁
    gpio_init(GPIO_LOCK_CEN, GPIO_DIRECTION_OUTPUT, 0);
    // 电磁锁状态，关锁输入低电平
    gpio_init(GPIO_LOCK_INT, GPIO_DIRECTION_INPUT, 1);

    // 推杆，默认输出低电平
    gpio_init(GPIO_PUSH_ROD0, GPIO_DIRECTION_OUTPUT, 1);
    gpio_init(GPIO_PUSH_ROD1, GPIO_DIRECTION_OUTPUT, 0);

    // 柜内灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_INSIDE, GPIO_DIRECTION_OUTPUT, 0);
    // 柜外灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_OUTSIDE, GPIO_DIRECTION_OUTPUT, 0);

    // 风扇，默认输出低电平不工作
    gpio_init(GPIO_FAN, GPIO_DIRECTION_OUTPUT, 0);

    // 红外对射状态，输入低电平有效
    gpio_init(GPIO_IR1_STATUS, GPIO_DIRECTION_INPUT, 1);
    gpio_init(GPIO_IR2_STATUS, GPIO_DIRECTION_INPUT, 1);

    // 接近检测 输入低电平有效
    gpio_init(GPIO_PEOPLE_CHECK, GPIO_DIRECTION_INPUT, 1);
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

int read_weight_adc(){
    char path[64];
    int value;
    char buffer[16];
    int ch = 0;

    sprintf(path,"/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",ch);
    
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("无法打开文件");
        return -1;
    }

    ssize_t num_read = read(fd, buffer, sizeof(buffer) - 1);
    if (num_read < 0) {
        perror("读取文件失败");
        close(fd);
        return -1;
    }

    buffer[num_read] = '\0'; // 确保字符串以null结尾
    // 将读取的内容转换为int型数据
    value = atoi(buffer) / 100; //最后两位无精度，舍去
    // 关闭文件
    close(fd);

    msleep(500);

    return value;
}

// 去皮
int get_self_weight()
{
    char str[32];
    // 从adc读取数据
    g_sRecycle_Box_Cnf.self_adc_var = read_weight_adc();
    printf("称重去皮值：%d\n", g_sRecycle_Box_Cnf.self_adc_var);

    sprintf(str, "%d", g_sRecycle_Box_Cnf.self_adc_var);
    write_device_info("self_weight",str);

    return 1;
}

// 校准
void set_rat_weight(double rat_weight) {
    char str[32];

    printf("准备校准\n");
    printf("rat_weight = %f\n",rat_weight);

    int sum = 0.0;
    for (int i = 0; i < 5; i++) {
        int value = (read_weight_adc() - g_sRecycle_Box_Cnf.self_adc_var);//去皮
        sum += value;
        msleep(500);
    }
    double average = sum / 5;
    g_sRecycle_Box_Cnf.rat = rat_weight / average;

    printf("称重计算系数：%.16f\n", g_sRecycle_Box_Cnf.rat);
    sprintf(str, "%.16f", g_sRecycle_Box_Cnf.rat);  // 称重系数保留16位小数
    write_device_info("rat",str);
    return 1;
}

double get_current_weight()
{
    // 从adc读取数据
    int adc_read = read_weight_adc();

    int diff_adc_value = adc_read - g_sRecycle_Box_Cnf.self_adc_var;
    // 重量转换
    double weight = diff_adc_value * 1.0 * g_sRecycle_Box_Cnf.rat;
    printf("称重 ADC=%d diff_adc_value=%d\n", adc_read, diff_adc_value);

    return weight;
}

// 获取溢出状态
int get_overflow_status() {
    int overflow_status = 1;

    // 有阻挡，输入0
    if (rk_gpio_get_value(GPIO_IR1_STATUS) == 0){
        overflow_status = 2;
    }

    if (rk_gpio_get_value(GPIO_IR2_STATUS) == 0){
        overflow_status = 3;
    }

    return overflow_status; // 1-3
}

// 设置清运门打开
int set_clear_door_open(){
    rk_gpio_set_value(GPIO_LOCK_CEN, 1);
    msleep(100);
    rk_gpio_set_value(GPIO_LOCK_CEN, 0);
    return 0;
}

// 获取清运门状态
int get_clear_door_status(){
    // 关锁状态低电平，开锁状态高电平
    int rev = rk_gpio_get_value(GPIO_LOCK_INT);
    // 关锁返回0，开锁返回1
    return rev;
}

get_rod_status() {
    FILE *file;
    int voltage;

    file = fopen("/sys/bus/iio/devices/iio:device1/in_voltage1_raw", "r");
    if (file == NULL) {
        return -1;
    }

    if (fscanf(file, "%d", &voltage) != 1) {
        fclose(file);
        return -1;
    }

    // 关闭文件
    fclose(file);

    // 判断 rod 的状态
    if (voltage < 5) {
        return 0;
    } else if (voltage < rod_hinder_value) {
        return 1;
    } else {
        return 2;
    }
}

rod_push(){
    rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 1);
}

rod_pull(){
    rk_gpio_set_value(GPIO_PUSH_ROD0, 1);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 0);
}

rod_stop(){
    rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 0);
}

// 设置推杆开启投递门
int set_post_door_open(){
    int status;
    int rod_retry = 0;
    rod_push();

    msleep(1000);

    while (1) {
        status = get_rod_status();

        if (status == 2) {
            rod_retry++;
            rod_pull();
            msleep(3000);
            if (rod_retry == 5){
                rod_stop();
                // 向服务器发送维修信息
                break;
            }
            rod_push();
        }

        if (status == 0) {
            rod_stop();
            break; // 跳出循环
        }

        msleep(100);
    }

    return 0;
}

// 设置推杆关闭投递门
int set_post_door_close(){
    int status;
    int rod_retry = 0;
    rod_pull();

    msleep(1000);

    while (1) {
        status = get_rod_status();

        if (status == 2) {
            rod_retry++;
            rod_push();
            msleep(3000);
            if (rod_retry == 5){
                rod_stop();
                // 向服务器发送维修信息
                break;
            }
            rod_pull();
        }

        if (status == 0) {
            rod_stop();
            break; // 跳出循环
        }
        msleep(100);
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
