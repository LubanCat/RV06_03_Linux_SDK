#ifndef HARDWARE_H
#define HARDWARE_H

#include "rk_gpio.h"
#include <fcntl.h>

#define msleep(x) 				usleep(x * 1000)

#define GPIO_LOCK_CEN           283
#define GPIO_PUSH_ROD0          5
#define GPIO_PUSH_ROD1          4
#define GPIO_LIGHT_INSIDE       2
#define GPIO_LIGHT_OUTSIDE      40
//#define GPIO_FAN                118

#define GPIO_LOCK_INT           287
// 红外1
#define GPIO_IR1_STATUS         284
// 红外2
#define GPIO_IR2_STATUS         285

//#define GPIO_PEOPLE_CHECK     123

typedef struct
{
    int self_adc_var;
    double rat;
    double com_weight;
} Recycle_Box_Cnf;

typedef enum {
    FAULT_DOOR_OPEN = 1,       // 投递门开门异常
    FAULT_DOOR_CLOSE,          // 投递门关门异常
    FAULT_CLEAN_DOOR_OPEN,     // 清运门开门异常
    FAULT_CLEAN_DOOR_CLOSE,    // 清运门关门异常
    FAULT_CAMERA,              // 摄像头异常
    FAULT_ELECTROMAGNETIC_LOCK,// 电磁锁异常
    FAULT_INNER_LIGHT,         // 内灯异常
    FAULT_OUTER_LIGHT,         // 外灯异常
    FAULT_PUSH_ROD             // 推杆异常
} FaultType;


extern Recycle_Box_Cnf g_sRecycle_Box_Cnf;
extern int rod_hinder_value;
// 获取当前重量
double get_current_weight();
double set_com_weight() ;

// 获取超重状态
int get_overflow_status();

// 设置清运门打开
void set_clear_door_open();

// 获取清运门状态
int get_clear_door_status();

// 设置推杆开启投递门
void set_post_door_open();

// 设置推杆关闭投递门
void set_post_door_close();

// 内部灯光操作
void inside_light(int status);

// 外部灯光操作
void outside_light(int status);
void *play_music(void *arg);

#endif // HARDWARE_H
