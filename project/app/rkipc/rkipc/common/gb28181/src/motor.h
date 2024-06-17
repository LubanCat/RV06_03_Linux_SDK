#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define HORIZONTAL_MAX_STEPS 8000
#define VERTICAL_MAX_STEPS 1000

#define MAX_PRESETS 0xFF
#define PRESETS_FILE "/oem/motor_preset.txt"

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

/* 云台 相对坐标移动 */
int motor_run_relative(int x_steps, int y_steps);

/* 云台 绝对坐标移动 */
int motor_run_absolute(int x_steps, int y_steps);

/* 云台初始化 */
int motor_init(void);


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

#endif  // MOTOR_H
