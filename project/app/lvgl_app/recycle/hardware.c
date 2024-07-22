#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "module.h"
#include "hardware.h"
#include "server.h"
#include "rk_gpio.h"
#include <math.h>

Recycle_Box_Cnf g_sRecycle_Box_Cnf;
int rod_hinder_value = 200;
int stop_music = 0;

// 初始化GPIO
gpio_init(int gpio, int dir, int value) 
{
    rk_gpio_export(gpio);
    rk_gpio_set_direction(gpio, dir);
    rk_gpio_set_value(gpio, value);
}

int recycle_gpio_init() 
{
    // 电磁锁，默认输出低电平上锁
    gpio_init(GPIO_LOCK_CEN, GPIO_DIRECTION_OUTPUT, 0);
    // 电磁锁状态，关锁输入低电平
    gpio_init(GPIO_LOCK_INT, GPIO_DIRECTION_INPUT, 1);

    // 推杆，默认输出低电平
    gpio_init(GPIO_PUSH_ROD0, GPIO_DIRECTION_OUTPUT, 0);
    gpio_init(GPIO_PUSH_ROD1, GPIO_DIRECTION_OUTPUT, 0);
    set_post_door_close();

    // 柜内灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_INSIDE, GPIO_DIRECTION_OUTPUT, 0);
    // 柜外灯，默认输出低电平关灯
    gpio_init(GPIO_LIGHT_OUTSIDE, GPIO_DIRECTION_OUTPUT, 0);

    // 风扇，默认输出低电平不工作
    //gpio_init(GPIO_FAN, GPIO_DIRECTION_OUTPUT, 0);

    // 红外对射状态，输入低电平有效
    gpio_init(GPIO_IR1_STATUS, GPIO_DIRECTION_INPUT, 1);
    gpio_init(GPIO_IR2_STATUS, GPIO_DIRECTION_INPUT, 1);

    // 接近检测 输入低电平有效
    //gpio_init(GPIO_PEOPLE_CHECK, GPIO_DIRECTION_INPUT, 1);
}

int read_weight_adc()
{
    char path[64] = {0};
    char buffer[16] = {0};

    sprintf(path, "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw", 0);
    
    int fd = open(path, O_RDONLY);
    if (fd < 0) 
    {
        perror("无法打开文件");
        return -1;
    }

    if (read(fd, buffer, sizeof(buffer) - 1) < 0) 
    {
        perror("读取文件失败");
        close(fd);
        return -1;
    }

    close(fd);

    return atoi(buffer) / 100; //最后两位无精度，舍去
}

// 去皮
void get_self_weight()
{
    char str[32] = {0};
    
    // 从adc读取数据
    g_sRecycle_Box_Cnf.self_adc_var = read_weight_adc();
    
    printf("称重去皮值：%d\n", g_sRecycle_Box_Cnf.self_adc_var);

    sprintf(str, "%d", g_sRecycle_Box_Cnf.self_adc_var);
    write_device_info("self_weight", str);
}

// 校准
void set_rat_weight(double rat_weight) 
{
    char str[32] = {0};
    int sum = 0.0;

    printf("准备校准\n");
    printf("rat_weight = %f\n", rat_weight);

    for (int i = 0; i < 5; i++) 
    {
        sum += (read_weight_adc() - g_sRecycle_Box_Cnf.self_adc_var);//去皮

        msleep(50);
    }
    
    double average = sum / 5;
    g_sRecycle_Box_Cnf.rat = rat_weight / average;

    printf("称重计算系数：%.16f\n", g_sRecycle_Box_Cnf.rat);
    
    sprintf(str, "%.16f", g_sRecycle_Box_Cnf.rat);  // 称重系数保留16位小数
    
    write_device_info("rat", str);
}

double get_current_weight() 
{
    // 从adc读取数据
    int adc_read = read_weight_adc();
    int diff_adc_value = adc_read - g_sRecycle_Box_Cnf.self_adc_var;
    
    // 重量转换
    double weight = diff_adc_value * 1.0 * g_sRecycle_Box_Cnf.rat;
    if (weight < 0)	weight = 0.00;
    weight = round(weight * 100) / 100; // 保留两位小数
    
    printf("称重 ADC=%d diff_adc_value=%d\n", adc_read, diff_adc_value);

    return weight;
}

// 获取溢出状态
int get_overflow_status() 
{
    int overflow_status = 1;

    // 有阻挡，输入0
    if (rk_gpio_get_value(GPIO_IR1_STATUS) == 0)
    {
        overflow_status = 3;
    }

    if (rk_gpio_get_value(GPIO_IR2_STATUS) == 0)
    {
        overflow_status = 3;
    }

    return overflow_status; // 1-3
}

// 设置清运门打开
void set_clear_door_open() 
{
    rk_gpio_set_value(GPIO_LOCK_CEN, 1);
    
    msleep(20);
    
    rk_gpio_set_value(GPIO_LOCK_CEN, 0);
}

// 获取清运门状态
int get_clear_door_status() 
{
    // 关锁状态低电平，开锁状态高电平
    return rk_gpio_get_value(GPIO_LOCK_INT);
}

int get_rod_status() 
{
    FILE *fd = NULL;
    int voltage = 0;
    int nRet = 0;

    fd = fopen("/sys/bus/iio/devices/iio:device1/in_voltage1_raw", "r");
    if (fd == NULL) 
    {
        return -1;
    }

    if (fscanf(fd, "%d", &voltage) != 1) 
    {
        fclose(fd);
        return -1;
    }

    // 关闭文件
    fclose(fd);

    // 判断 rod 的状态
    if (voltage < 5)
    {
        nRet = 0;
    }
    else if (voltage < rod_hinder_value) 
    {
        nRet = 1;
    }
    else
    {
        nRet = 2;
    }

    return nRet;
}

void rod_push() 
{
    rod_stop();
    
    msleep(20);
    
    rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 1);
}

void rod_pull() 
{
    rod_stop();
    
    msleep(20);
    
    rk_gpio_set_value(GPIO_PUSH_ROD0, 1);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 0);
}

void rod_stop() 
{
    rk_gpio_set_value(GPIO_PUSH_ROD0, 0);
    rk_gpio_set_value(GPIO_PUSH_ROD1, 0);
}


// 设置推杆开启投递门
void set_post_door_open() 
{
    int status;
    int rod_retry = 0;

    rod_push();
    
    msleep(1000);

    while (1)
    {
        status = get_rod_status();
        if (status == 0) 
        {
            rod_stop();
            break; // 跳出循环
        }
        
        msleep(100);
    }
}

// 音乐播放线程函数
void *play_music(void *arg) 
{
    while (1) 
    {
        system("/usr/bin/aplay -Dhw:0 /oem/usr/share/closedoor.wav");

        // 检查是否需要停止播放音乐
        if (stop_music) 
        {
            stop_music = 0;
            break;
        }

        sleep(1); // 避免音乐播放完成后过于频繁地重新启动播放
    }
    
    return NULL;
}

// 设置推杆关闭投递门
void set_post_door_close()
{
    int status;
    int rod_retry = 0;

    // 创建播放音乐的线程
    pthread_t music_thread;
    if (pthread_create(&music_thread, NULL, play_music, NULL) != 0) 
    {
        perror("Failed to create music thread");
        return 1;
    }

    rod_pull();

    msleep(1000);
    
    while (1) 
    {
        status = get_rod_status();
        if (status == 2) 
        {
            rod_push();
            
            msleep(3000);

            rod_retry++;
            if (rod_retry == 5)
            {
                rod_stop();
                // 向服务器发送维修信息
                upload_fault(FAULT_DOOR_CLOSE);
                break;
            }
            
            rod_pull();
        }

        if (status == 0) 
        {
            rod_stop();
            break; // 跳出循环
        }
        
        msleep(100);
    }

    // 设置停止播放音乐标志
    stop_music = 1;
}

// 外部灯光操作
void outside_light(int status) 
{
    if (status == 1)
    {
        rk_gpio_set_value(GPIO_LIGHT_OUTSIDE, 1);
        printf("Outside Light is ON\n");
    } 
    else 
    {
        rk_gpio_set_value(GPIO_LIGHT_OUTSIDE, 0);
        printf("Outside Light is OFF\n");
    }
}

// 内部灯光操作
void inside_light(int status) 
{
    if (status == 1)
    {
        rk_gpio_set_value(GPIO_LIGHT_INSIDE, 1);
        printf("Inside Light is ON\n");
    } 
    else 
    {
        rk_gpio_set_value(GPIO_LIGHT_INSIDE, 0);
        printf("Inside Light is OFF\n");
    }
}
