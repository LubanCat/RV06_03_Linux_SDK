#include "ptzcmd.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

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

    motor_run_absolute(HORIZONTAL_MAX_STEPS, VERTICAL_MAX_STEPS);  // 归零

    if (read_success) {
        // 如果读取成功，则移动到保存的位置
        printf("移动到保存的位置 x=%d, y=%d\n", x_current_steps, y_current_steps);
        // 读取到绝对路径
        motor_run_absolute(x_current_steps, y_current_steps);
    } else {
        // 如果文件不存在或读取失败，则归零并居中
        printf("文件不存在或读取失败，移动到居中位置\n");
        motor_run_absolute(HORIZONTAL_MAX_STEPS / 2, 0);    // 居中
    }

    // 初始化时加载预置点列表
    load_presets();
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
        for (int i = 0; i < MAX_PRESETS; ++i) {  // 遍历所有预置位
            if (presets[i].is_set) {
                // printf("保存预置位: 索引=%d, 名称=%s, x坐标=%d, y坐标=%d\n", i, presets[i].name, presets[i].x_steps, presets[i].y_steps);
                fprintf(file, "预置位: 索引=%d, 名称=%s, x坐标=%d, y坐标=%d\n", i, presets[i].name, presets[i].x_steps, presets[i].y_steps);
            }
        }
        fclose(file);
    } else {
        perror("打开文件写入失败");
    }
}

// 从文件加载预置位
int load_presets() {
    int num_presets = 0;
    FILE *file = fopen(PRESETS_FILE, "r");
    if (file != NULL) {
        char line[100];  // 假设每行不超过 100 个字符
        while (fgets(line, sizeof(line), file) != NULL) {
            int index, x_steps, y_steps;
            char name[20];

            // 使用 sscanf 解析每行的预置位信息
            if (sscanf(line, "预置位: 索引=%d, 名称=%19[^,], x坐标=%d, y坐标=%d", &index, name, &x_steps, &y_steps) == 4) {
                // 将解析得到的信息存储到预置位数组中
                if (index >= 0 && index < MAX_PRESETS) {
                    strncpy(presets[index].name, name, sizeof(presets[index].name) - 1);
                    presets[index].name[sizeof(presets[index].name) - 1] = '\0';  // 确保字符串结尾为 null
                    presets[index].x_steps = x_steps;
                    presets[index].y_steps = y_steps;
                    presets[index].is_set = 1;
                    num_presets++;
                } else {
                    printf("警告：预置位索引 %d 超出范围\n", index);
                }
            } else {
                printf("警告：无法解析预置位信息：%s\n", line);
            }
        }
        fclose(file);
        printf("加载 %d 个预置位信息\n", num_presets);
    } else {
        perror("打开文件读取失败");
    }
    return num_presets;
}

// 删除指定索引位置的预置位
void delete_preset(int index) {
    if (index < 0 || index >= MAX_PRESETS) {
        printf("无效的预置位索引\n");
        return;
    }

    // 标记要删除的预置位为未设置状态
    presets[index].is_set = 0;

    // 输出删除信息
    printf("正在删除预置位: 索引=%d\n", index);

    // 重新保存更新后的预置位信息
    save_presets();
}

// 打印所有已设置内容的预置位信息
void print_presets() {
    printf("当前已设置内容的预置位:\n");
    int printed = 0;
    for (int i = 0; i < MAX_PRESETS; ++i) {
        if (presets[i].is_set && presets[i].name[0] != '\0') {
            printf("预置位 %d: 名称=%s, x=%d, y=%d\n", i , presets[i].name, presets[i].x_steps, presets[i].y_steps);
            printed++;
        }
    }
    if (printed == 0) {
        printf("没有已设置的预置位\n");
    }
}

// 将当前的云台位置设置为指定索引的预置位
void set_preset(int index) {
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
    snprintf(presets[index].name, sizeof(presets[index].name), "Point%d", index);
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

/************************************ 巡航点 ***********************************/
// 将巡航点保存到文件
void save_cruise(CruiseData *data) {
    FILE *file = fopen(CRUISE_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_CRUISE; ++i) {
            if (data[i].is_set) {
                fprintf(file, "Group=%d, Speed=%d, Time=%u, Points=", data[i].Group, data[i].Speed, data[i].Time);
                int num_points = 0;
                for (int j = 0; j < MAX_POINTS; ++j) {
                    if (data[i].Points[j] != 0) {
                        fprintf(file, "%d,", data[i].Points[j]);
                        num_points++;
                    }
                }
                if (num_points > 0) {
                    fseek(file, -1, SEEK_CUR);  // 去掉最后一个逗号
                }
                fprintf(file, "\n");
            }
        }
        fclose(file);
    } else {
        perror("打开文件写入失败");
    }
}

// 从文件加载巡航点
int load_cruise(CruiseData *data) {
    int num_cruise = 0;
    FILE *file = fopen(CRUISE_FILE, "r");
    if (file != NULL) {
        char line[100];  // 假设每行不超过 100 个字符
        while (fgets(line, sizeof(line), file) != NULL) {
            int Group, Speed;
            unsigned int Time;
            int Points[MAX_POINTS];
            // 使用 sscanf 解析每行的巡航点信息
            if (sscanf(line, "Group=%d, Speed=%d, Time=%u, Points=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                       &Group, &Speed, &Time,
                       &Points[0], &Points[1], &Points[2], &Points[3],
                       &Points[4], &Points[5], &Points[6], &Points[7],
                       &Points[8], &Points[9], &Points[10], &Points[11],
                       &Points[12], &Points[13], &Points[14], &Points[15]) == 18) {
                // 将解析得到的信息存储到巡航点数组中
                if (Group >= 0 && Group < MAX_CRUISE) {
                    data[Group].Group = Group;
                    data[Group].Speed = Speed;
                    data[Group].Time = Time;
                    for (int j = 0; j < MAX_POINTS; ++j) {
                        data[Group].Points[j] = Points[j];
                    }
                    num_cruise++;
                } else {
                    printf("警告：巡航点索引 %d 超出范围\n", Group);
                }
            } else {
                printf("警告：无法解析巡航点信息：%s\n", line);
            }
        }
        fclose(file);
        printf("加载 %d 个巡航点信息\n", num_cruise);
    } else {
        perror("打开文件读取失败");
    }
    return num_cruise;
}

// 打印所有已设置内容的巡航数据信息
// 打印巡航点数据
void print_cruise() {
    for (int i = 0; i < MAX_CRUISE; ++i) {
        if (cruise[i].is_set) {
            printf("Group=%d, Speed=%d, Time=%u, Points=", cruise[i].Group, cruise[i].Speed, cruise[i].Time);
            for (int j = 0; j < MAX_POINTS; ++j) {
                if (cruise[i].Points[j] != 0) {
                    printf("%d,", cruise[i].Points[j]);
                }
            }
            printf("\n");
        }
    }
}

// 设置或更新巡航点
void set_cruise_point(int Group, int Point) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        printf("无效的巡航数据Group\n");
        return;
    }

    printf("设置巡航数据组: 巡航组=%d, 添加点=%d\n", Group, Point);

    // 查找空的点位置
    int i;
    for (i = 0; i < MAX_POINTS; ++i) {
        if (cruise[Group].Points[i] == 0) {
            cruise[Group].Points[i] = Point;
            break;
        }
    }
    if (i == MAX_POINTS) {
        printf("巡航数据组已满: 巡航组=%d\n", Group);
        return;
    }

    // 如果是新建的 Group，则设置默认值
    if (!cruise[Group].is_set) {
        cruise[Group].Speed = DEFAULT_SPEED;
        cruise[Group].Time = DEFAULT_TIME;
        cruise[Group].is_set = 1;
    }

    // 保存更新后的巡航数据到文件
    save_cruise(cruise);
}

// 删除巡航点或整组数据
void delete_cruise(int Group, int Point) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        printf("无效的巡航数据Group\n");
        return;
    }

    if (Point == 0) {
        // 删除整组数据
        memset(&cruise[Group], 0, sizeof(CruiseData));
        cruise[Group].is_set = 0;
        printf("删除整组巡航点: 巡航组=%d\n", Group);
    } else {
        // 删除对应点
        int i;
        for (i = 0; i < MAX_POINTS; ++i) {
            if (cruise[Group].Points[i] == Point) {
                cruise[Group].Points[i] = 0;
                printf("删除巡航点: 巡航组=%d，位置点=%d\n", Group, Point);
                break;
            }
        }
        if (i == MAX_POINTS) {
            printf("点未找到: 索引=%d, 点=%d\n", Group, Point);
            return;
        }
    }

    // 保存更新后的巡航数据到文件
    save_cruise(cruise);
}

// 设置或更新巡航速度
void set_cruise_speed(int Group, int Speed) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        printf("无效的巡航数据Group\n");
        return;
    }
    
    printf("设置巡航速度: 巡航组=%d，速度=%d\n", Group, Speed);

    if (cruise[Group].is_set) {
        cruise[Group].Speed = Speed;
        // 保存更新后的巡航数据到文件
        save_cruise(cruise);
    }
}

// 设置或更新巡航点停留时间
void set_cruise_time(int Group, int Time) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        printf("无效的巡航数据:Group\n");
        return;
    }

    printf("设置巡航停留: 巡航组=%d，停留时间=%ds\n", Group, Time);

    if (cruise[Group].is_set) {
        cruise[Group].Time = Time;
        // 保存更新后的巡航数据到文件
        save_cruise(cruise);
    }
}