#ifndef PTZCMD_H
#define PTZCMD_H

#include <stdint.h>

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
    int leftRight; // 镜头左移右移 0:停止 1:左移 2:右移
    int upDown;    // 镜头上移下移 0:停止 1:上移 2:下移
    int inOut;     // 镜头放大缩小 0:停止 1:缩小 2:放大
    int moveSpeed; // 镜头移动速度 (0-255)
    int zoomSpeed; // 镜头缩放速度 (0-255)
} PtzCommand;


/**
 * 解析云台指令码
 *
 * @param cmdStr 云台指令码字符串
 * @param cmd 解析后的PTZ指令结构体
 * @return 0 表示成功，-1 表示失败
 */
int parseCmdString(const char *cmdStr, PtzCommand *cmd);

int motor_run(int x_steps, int y_steps);

#endif  // PTZCMD_H
