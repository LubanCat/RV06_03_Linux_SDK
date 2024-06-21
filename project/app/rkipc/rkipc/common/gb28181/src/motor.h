#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define HORIZONTAL_MAX_STEPS 8000
#define VERTICAL_MAX_STEPS 1000

#define MAX_PRESETS 0xFF
#define PRESETS_FILE "/oem/motor_preset.txt"

#define MAX_POINTS 16
#define MAX_CRUISE 0xF
#define DEFAULT_SPEED 100
#define DEFAULT_TIME 1
#define CRUISE_FILE "/oem/motor_cruise.txt"

#define BUFFER_SIZE 1024

#define MOTOR_DEV "/dev/motor"

/* ioctl cmd */
#define MOTOR_STOP          _IOW('M',0,long)
#define MOTOR_RESET         _IOW('M',1,long)
#define MOTOR_MOVE          _IOW('M',2,long)
#define MOTOR_GET_STATUS    _IOW('M',3,long)
#define MOTOR_SPEED         _IOW('M',4,long)
#define MOTOR_GOBACK        _IOW('M',5,long)
#define MOTOR_CRUISE        _IOW('M',6,long)
#define MOTOR_SET_STATUS    _IOW('M',7,long)

enum motor_status {
    MOTOR_IS_STOP,
    MOTOR_IS_RUNNING,
};

struct motors_input_steps {
    int input_x_steps;
    int input_y_steps;
};

struct motor_message {
    int x_cur_steps;
    int y_cur_steps;
    enum motor_status x_cur_status;
    enum motor_status y_cur_status;
    int x_cur_speed;
    int y_cur_speed;
};

typedef struct {
    char cmd[BUFFER_SIZE];
    char sn[BUFFER_SIZE];
} Tuple;

typedef struct {
    char name[20];  // 预置位名称
    int x_steps;    // x 方向步数
    int y_steps;    // y 方向步数
    int is_set;     // 标记预置位是否设置的标志
} MotorPreset;

MotorPreset presets[MAX_PRESETS];  // 预置位数组

// 定义数据结构
typedef struct {
    int Group;
    int Points[MAX_POINTS];
    int Speed;
    unsigned int Time; // 使用unsigned int来表示停留时间
    int is_set;
} CruiseData;

CruiseData cruise[MAX_CRUISE];  // 预置位数组



/* 云台 相对坐标移动 */
int motor_run_relative(int x_steps, int y_steps);

/* 云台 绝对坐标移动 */
int motor_run_absolute(int x_steps, int y_steps);

/* 云台初始化 */
int motor_init(void);

/* 获取云台状态 */
int get_motor_status();


// 将预置位保存到文件
void save_presets();

// 从文件加载预置位
int load_presets();

// 删除指定索引位置的预置位
void delete_preset(int index);

// 打印所有预置位
void print_presets();

// 将当前的云台位置设置为指定索引的预置位
void set_preset(int index);

// 将云台移动到指定索引的预置位
void move_to_preset(int index);


// 打印所有已设置内容的巡航数据信息
void print_cruise();

// 设置或更新巡航点
void set_cruise_point(int Group, int Point);

// 设置或更新巡航速度
void set_cruise_speed(int Group, int Speed);

// 设置或更新巡航点停留时间
void set_cruise_time(int Group, int Time);

// 加载巡航点数据
void load_cruise();

// 获取巡航数据组并控制步进电机
void cruise_run(int Group);

#endif  // MOTOR_H
