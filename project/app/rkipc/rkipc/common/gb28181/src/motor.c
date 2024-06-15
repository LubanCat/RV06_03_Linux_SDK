#include "ptzcmd.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

MotorPreset presets[MAX_PRESETS];  // 预置位数组
int num_presets = 0;  // 当前保存的预置位数量

/************************************ 电机 ***********************************/
/* 云台初始化 */
int motor_init(void)
{
    FILE *file;
    int x_current_steps = 0;
    int y_current_steps = 0;
    int read_success = 0;


    // 打开文件读取保存的位置
    file = fopen("/oem/motor_steps.txt", "r");
    if (file != NULL) {
        // 文件存在，尝试读取步数
        if (fscanf(file, "x_current_steps=%d\ny_current_steps=%d", &x_current_steps, &y_current_steps) == 2) {
            read_success = 1;
        }
        fclose(file);
    }

    motor_run_relative(HORIZONTAL_MAX_STEPS, VERTICAL_MAX_STEPS);  // 归零

    if (read_success) {
        // 如果读取成功，则移动到保存的位置
        printf("移动到保存的位置 x=%d, y=%d\n", x_current_steps, y_current_steps);
        // current_steps为绝对路径，需要转换
        motor_run_relative(x_current_steps - HORIZONTAL_MAX_STEPS, y_current_steps - VERTICAL_MAX_STEPS);
    } else {
        // 如果文件不存在或读取失败，则归零并居中
        printf("文件不存在或读取失败，移动到居中位置\n");
        motor_run_relative(-4000, 0);    // 居中
    }

    return 0;
}

/* 云台 相对坐标移动 */
int motor_run_relative(int x_steps, int y_steps)
{
    int fd;
    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;
    int x_current_steps = 0;
    int y_current_steps = 0;

    // 打开电机设备
    fd = open(MOTOR_DEV, O_RDWR);
    if (fd == -1) {
        perror("open motor");
        return -1;
    }

    // 设置新的步数
    motors_steps.input_x_steps = x_steps;
    motors_steps.input_y_steps = y_steps;
    printf("移动步数设置 x=%d, y=%d\n", motors_steps.input_x_steps, motors_steps.input_y_steps);
    ioctl(fd, MOTOR_MOVE, (unsigned long)&motors_steps);

    usleep(100000); // 等待100毫秒

    // 等待电机完成移动
    do {
        ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        // 检查电机是否完成移动
        if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
            printf("电机移动完成,当前位置：x=%d, y=%d\n", get_motor_message.x_cur_steps, get_motor_message.y_cur_steps);
            break;
        }
        // 如果电机还在移动，则等待一段时间后重试
        usleep(100000); // 等待100毫秒
    } while (1);

    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 打开或创建文件以写入当前步数
    FILE *file = fopen("/oem/motor_steps.txt", "w");
    if (file == NULL) {
        perror("open file");
        return -1;
    }

    // 将步数写入文件
    fprintf(file, "x_current_steps=%d\ny_current_steps=%d\n", x_current_steps, y_current_steps);
    fclose(file);

    // 关闭设备
    close(fd);
    return 0;
}

/* 云台 绝对坐标移动 */
int motor_run_absolute(int x_steps, int y_steps)
{
    int fd;
    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;
    int x_current_steps = 0;
    int y_current_steps = 0;
    int x_actual_steps = 0;
    int y_actual_steps = 0;

    // 打开电机设备
    fd = open(MOTOR_DEV, O_RDWR);
    if (fd == -1) {
        perror("open motor");
        return -1;
    }

    ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    x_actual_steps = x_steps - x_current_steps;
    y_actual_steps = y_steps - y_current_steps;

    // 设置新的步数
    motors_steps.input_x_steps = x_actual_steps;
    motors_steps.input_y_steps = y_actual_steps;
    printf("移动步数设置 x=%d, y=%d\n", motors_steps.input_x_steps, motors_steps.input_y_steps);
    ioctl(fd, MOTOR_MOVE, (unsigned long)&motors_steps);

    usleep(100000); // 等待100毫秒

    // 等待电机完成移动
    do {
        ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        // 检查电机是否完成移动
        if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
            printf("电机移动完成,当前位置：x=%d, y=%d\n", get_motor_message.x_cur_steps, get_motor_message.y_cur_steps);
            break;
        }
        // 如果电机还在移动，则等待一段时间后重试
        usleep(100000); // 等待100毫秒
    } while (1);

    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 打开或创建文件以写入当前步数
    FILE *file = fopen("/oem/motor_steps.txt", "w");
    if (file == NULL) {
        perror("open file");
        return -1;
    }

    // 将步数写入文件
    fprintf(file, "x_current_steps=%d\ny_current_steps=%d\n", x_current_steps, y_current_steps);
    fclose(file);

    // 关闭设备
    close(fd);
    return 0;
}

/************************************ 预置位置 ***********************************/
// 将预置位保存到文件
void save_presets() {
    FILE *file = fopen(PRESETS_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < num_presets; ++i) {
            fprintf(file, "预置位 %d: 名称=%s, x步数=%d, y步数=%d\n", i+1, presets[i].name, presets[i].x_steps, presets[i].y_steps);
        }
        fclose(file);
    } else {
        perror("打开文件写入失败");
    }
}

// 从文件加载预置位
void load_presets() {

    FILE *file = fopen(PRESETS_FILE, "r");
    if (file != NULL) {
        num_presets = 0;
        while (fscanf(file, "预置位 %*d: 名称=%19s, x步数=%d, y步数=%d\n", presets[num_presets].name, &presets[num_presets].x_steps, &presets[num_presets].y_steps) == 3) {
            presets[num_presets].is_set = 1;
            num_presets++;
            if (num_presets >= MAX_PRESETS) {
                break;  // 避免预置位数组溢出
            }
        }
        fclose(file);
    } else {
        perror("打开文件读取失败");
    }
}

// 删除指定索引位置的预置位
void delete_preset(int index) {
    if (index < 0 || index >= num_presets) {
        printf("无效的预置位索引\n");
        return;
    }
    // 移动预置位数组来删除指定位置的预置位
    for (int i = index; i < num_presets - 1; ++i) {
        presets[i] = presets[i + 1];
    }
    num_presets--;
    save_presets();
}

// 打印所有预置位
void print_presets() {
    printf("当前预置位:\n");
    for (int i = 0; i < num_presets; ++i) {
        printf("预置位 %d: 名称=%s, x=%d, y=%d\n", i+1, presets[i].name, presets[i].x_steps, presets[i].y_steps);
    }
}

// 将当前的云台位置设置为指定索引的预置位
void set_preset(int index, const char *name) {
    int x_current_steps = 0;
    int y_current_steps = 0;
    struct motor_message get_motor_message;

    // 打开电机设备
    int fd = open(MOTOR_DEV, O_RDWR);
    if (fd == -1) {
        perror("无法打开电机设备");
        return;
    }

    // 获取电机当前位置信息
    if (ioctl(fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message) == -1) {
        perror("ioctl 获取电机状态失败");
        close(fd);
        return;
    }
    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 关闭电机设备
    close(fd);

    // 检查索引是否有效
    if (index < 0 || index >= MAX_PRESETS) {
        printf("无效的预置位索引\n");
        return;
    }

    // 设置预置位信息
    strncpy(presets[index].name, name, sizeof(presets[index].name) - 1);
    presets[index].name[sizeof(presets[index].name) - 1] = '\0';  // 确保字符串结尾为 null
    presets[index].x_steps = x_current_steps;
    presets[index].y_steps = y_current_steps;
    presets[index].is_set = 1;

    // 保存预置位信息到文件或其他存储介质
    save_presets();
}

// 将云台移动到指定索引的预置位
void move_to_preset(int index) {
   
    // 检查索引是否有效
    if (index < 0 || index >= MAX_PRESETS || !presets[index].is_set) {
        printf("无效的预置位索引\n");
        return;
    }
    
    // 绝对位置移动
    motor_run_absolute(presets[index].x_steps, presets[index].y_steps);
}
