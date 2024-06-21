#include "ptzcmd.h"
#include "motor.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

extern int motor_fd;

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
        LOG_INFO("移动到保存的位置 x=%d, y=%d\n", x_current_steps, y_current_steps);
        // 读取到绝对路径
        motor_run_absolute(x_current_steps, y_current_steps);
    } else {
        // 如果文件不存在或读取失败，则归零并居中
        LOG_ERROR("文件不存在或读取失败，移动到居中位置\n");
        motor_run_absolute(HORIZONTAL_MAX_STEPS / 2, 0);    // 居中
    }

    // 初始化时加载预置点列表
    load_presets();

    // 初始化时加载巡航点数据
    load_cruise();

    return 0;
}

/* 云台 相对坐标移动 */
int motor_run_relative(int x_steps, int y_steps)
{
    if (motor_fd == -1) {
        LOG_ERROR("open motor failed\n");
        return 0;
    }

    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;
    int x_current_steps = 0;
    int y_current_steps = 0;

    // 设置新的步数
    motors_steps.input_x_steps = x_steps;
    motors_steps.input_y_steps = y_steps;
    LOG_DEBUG("移动步数设置 x=%d, y=%d\n", motors_steps.input_x_steps, motors_steps.input_y_steps);
    ioctl(motor_fd, MOTOR_MOVE, (unsigned long)&motors_steps);

    usleep(10000); // 等待10毫秒

    // 等待电机完成移动
    do {
        ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        // 检查电机是否完成移动
        if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
            LOG_DEBUG("电机移动完成,当前位置：x=%d, y=%d\n", get_motor_message.x_cur_steps, get_motor_message.y_cur_steps);
            break;
        }
        // 如果电机还在移动，则等待一段时间后重试
        usleep(10000); // 等待10毫秒
    } while (1);

    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 打开或创建文件以写入当前步数
    FILE *file = fopen("/oem/motor_steps.txt", "w");
    if (file == NULL) {
        LOG_ERROR("open /oem/motor_steps.txt failed\n");
        return -1;
    }

    // 将步数写入文件
    fprintf(file, "x_current_steps=%d\ny_current_steps=%d\n", x_current_steps, y_current_steps);
    fclose(file);

    return 0;
}

/* 云台 绝对坐标移动 */
int motor_run_absolute(int x_steps, int y_steps)
{
    if (motor_fd == -1) {
        LOG_ERROR("open motor failed\n");
        return 0;
    }

    struct motors_input_steps motors_steps;
    struct motor_message get_motor_message;
    int x_current_steps = 0;
    int y_current_steps = 0;
    int x_actual_steps = 0;
    int y_actual_steps = 0;

    ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    x_actual_steps = x_steps - x_current_steps;
    y_actual_steps = y_steps - y_current_steps;

    // 设置新的步数
    motors_steps.input_x_steps = x_actual_steps;
    motors_steps.input_y_steps = y_actual_steps;
    LOG_DEBUG("移动步数设置 x=%d, y=%d\n", motors_steps.input_x_steps, motors_steps.input_y_steps);
    ioctl(motor_fd, MOTOR_MOVE, (unsigned long)&motors_steps);

    usleep(10000); // 等待100毫秒

    // 等待电机完成移动
    do {
        ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        // 检查电机是否完成移动
        if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
            LOG_DEBUG("电机移动完成,当前位置：x=%d, y=%d\n", get_motor_message.x_cur_steps, get_motor_message.y_cur_steps);
            break;
        }
        // 如果电机还在移动，则等待一段时间后重试
        usleep(10000); // 等待100毫秒
    } while (1);

    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 打开或创建文件以写入当前步数
    FILE *file = fopen("/oem/motor_steps.txt", "w");
    if (file == NULL) {
        LOG_ERROR("open file failed\n");
        return -1;
    }

    // 将步数写入文件
    fprintf(file, "x_current_steps=%d\ny_current_steps=%d\n", x_current_steps, y_current_steps);
    fclose(file);

    return 0;
}

// return run:1 stop:0
int get_motor_status()
{
    struct motor_message get_motor_message;

    if (motor_fd == -1) {
        LOG_ERROR("open motor failed\n");
        return -1;
    }

    ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);

    if (get_motor_message.x_cur_status == 0 && get_motor_message.y_cur_status == 0) {
        return 0;
    }
    return 1;
}

/************************************ 预置位置 ***********************************/

// 将预置位保存到文件
void save_presets() {
    FILE *file = fopen(PRESETS_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_PRESETS; ++i) {  // 遍历所有预置位
            if (presets[i].is_set) {
                fprintf(file, "预置位: 索引=%d, 名称=%s, x坐标=%d, y坐标=%d\n", i, presets[i].name, presets[i].x_steps, presets[i].y_steps);
            }
        }
        fclose(file);
    } else {
        LOG_ERROR("打开文件写入失败\n");
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
                    LOG_ERROR("警告：预置位索引 %d 超出范围\n", index);
                }
            } else {
                LOG_ERROR("警告：无法解析预置位信息：%s\n", line);
            }
        }
        fclose(file);
        LOG_DEBUG("加载 %d 个预置位信息\n", num_presets);
    } else {
        LOG_ERROR("预置位文件读取失败\n");
    }
    return num_presets;
}

// 删除指定索引位置的预置位
void delete_preset(int index) {
    if (index < 0 || index >= MAX_PRESETS) {
        LOG_ERROR("无效的预置位索引\n");
        return;
    }

    // 标记要删除的预置位为未设置状态
    presets[index].is_set = 0;

    // 输出删除信息
    LOG_INFO("正在删除预置位: 索引=%d\n", index);

    // 重新保存更新后的预置位信息
    save_presets();
}

// 打印所有已设置内容的预置位信息
void print_presets() {
    LOG_INFO("当前已设置内容的预置位:\n");
    int printed = 0;
    for (int i = 0; i < MAX_PRESETS; ++i) {
        if (presets[i].is_set && presets[i].name[0] != '\0') {
            LOG_INFO("预置位 %d: 名称=%s, x=%d, y=%d\n", i , presets[i].name, presets[i].x_steps, presets[i].y_steps);
            printed++;
        }
    }
    if (printed == 0) {
        LOG_INFO("没有已设置的预置位\n");
    }
}

// 将当前的云台位置设置为指定索引的预置位
void set_preset(int index) {
    int x_current_steps = 0;
    int y_current_steps = 0;
    struct motor_message get_motor_message;

    // 获取电机当前位置信息
    if (ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message) == -1) {
        LOG_ERROR("ioctl 获取电机状态失败\n");
        return;
    }
    x_current_steps = get_motor_message.x_cur_steps;
    y_current_steps = get_motor_message.y_cur_steps;

    // 检查索引是否有效
    if (index < 0 || index >= MAX_PRESETS) {
        LOG_ERROR("无效的预置位索引\n");
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
        LOG_ERROR("无效的预置位索引\n");
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
        LOG_ERROR("打开文件写入失败\n");
    }
}

// 从文件加载巡航点
void load_cruise() {
    FILE *file = fopen(CRUISE_FILE, "r");
    if (file != NULL) {
        char line[256];  // 假设每行不超过 256 个字符
        int index = 0;

        while (fgets(line, sizeof(line), file)) {
            if (index >= MAX_CRUISE) {
                LOG_ERROR("警告: 文件中存在超出最大巡航组数的数据，已忽略\n");
                break;
            }

            // 解析行数据
            int Group, Speed, Time;
            if (sscanf(line, "Group=%d, Speed=%d, Time=%u, Points=", &Group, &Speed, &Time) == 3) {
                cruise[index].Group = Group;
                cruise[index].Speed = Speed;
                cruise[index].Time = Time;
                cruise[index].is_set = 1;

                // 解析 Points
                char *token;
                char *rest = line;
                int point_index = 0;
                while ((token = strtok_r(rest, ",", &rest))) {
                    if (strncmp(token, "Points=", 7) == 0) {
                        continue;  // 跳过 Points= 部分
                    }
                    int point;
                    if (sscanf(token, "%d", &point) == 1) {
                        cruise[index].Points[point_index++] = point;
                        if (point_index >= MAX_POINTS) {
                            break;  // 已达到最大点数
                        }
                    }
                }

                index++;
            }
        }

        fclose(file);
    } else {
        LOG_ERROR("巡航点文件读取失败\n");
    }
}

// 打印所有已设置内容的巡航数据信息
void print_cruise() {
    for (int i = 0; i < MAX_CRUISE; ++i) {
        if (cruise[i].is_set) {
            LOG_INFO("Group=%d, Speed=%d, Time=%u, Points=", cruise[i].Group, cruise[i].Speed, cruise[i].Time);
            for (int j = 0; j < MAX_POINTS; ++j) {
                if (cruise[i].Points[j] != 0) {
                    LOG_INFO("%d,", cruise[i].Points[j]);
                }
            }
            LOG_INFO("\n");
        }
    }
}

// 设置或更新巡航点
void set_cruise_point(int Group, int Point) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        LOG_ERROR("无效的巡航数据Group\n");
        return;
    }

    LOG_INFO("设置巡航数据组: 巡航组=%d, 添加点=%d\n", Group, Point);

    // 查找空的点位置
    int i;
    for (i = 0; i < MAX_POINTS; ++i) {
        if (cruise[Group].Points[i] == 0) {
            cruise[Group].Points[i] = Point;
            break;
        }
    }
    if (i == MAX_POINTS) {
        LOG_ERROR("巡航数据组已满: 巡航组=%d\n", Group);
        return;
    }

    // 如果是新建的 Group，则设置默认值
    if (!cruise[Group].is_set) {
        cruise[Group].Group = Group;
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
        LOG_ERROR("无效的巡航数据Group\n");
        return;
    }

    if (Point == 0) {
        // 删除整组数据
        memset(&cruise[Group], 0, sizeof(CruiseData));
        cruise[Group].is_set = 0;
        LOG_INFO("删除整组巡航点: 巡航组=%d\n", Group);
    } else {
        // 删除对应点
        int i;
        for (i = 0; i < MAX_POINTS; ++i) {
            if (cruise[Group].Points[i] == Point) {
                cruise[Group].Points[i] = 0;
                LOG_INFO("删除巡航点: 巡航组=%d，位置点=%d\n", Group, Point);
                break;
            }
        }
        if (i == MAX_POINTS) {
            LOG_ERROR("点未找到: 索引=%d, 点=%d\n", Group, Point);
            return;
        }
    }

    // 保存更新后的巡航数据到文件
    save_cruise(cruise);
}

// 设置或更新巡航速度
void set_cruise_speed(int Group, int Speed) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        LOG_ERROR("无效的巡航数据Group\n");
        return;
    }

    LOG_INFO("设置巡航速度: 巡航组=%d，速度=%d\n", Group, Speed);

    if (cruise[Group].is_set) {
        cruise[Group].Speed = Speed;
        // 保存更新后的巡航数据到文件
        save_cruise(cruise);
    }
}

// 设置或更新巡航点停留时间
void set_cruise_time(int Group, int Time) {
    if (Group < 0 || Group >= MAX_CRUISE) {
        LOG_ERROR("无效的巡航数据:Group\n");
        return;
    }

    LOG_INFO("设置巡航停留: 巡航组=%d，停留时间=%ds\n", Group, Time);

    if (cruise[Group].is_set) {
        cruise[Group].Time = Time;
        // 保存更新后的巡航数据到文件
        save_cruise(cruise);
    }
}

// 获取巡航数据组并控制步进电机
void cruise_run(int Group) {
    if (Group < 0 || Group >= MAX_CRUISE || !cruise[Group].is_set) {
        LOG_ERROR("无效的巡航数据组索引或者数据组未设置\n");
        sleep(1);
        return;
    }

    // 读取速度和时间
    int speed = cruise[Group].Speed;
    unsigned int time = cruise[Group].Time;

    LOG_INFO("开始执行巡航数据组: 巡航组=%d, 速度=%d, 时间=%u\n", Group, speed, time);

    // 按照 Points 的顺序控制步进电机
    for (int i = 0; i < MAX_POINTS; ++i) {
        if (cruise[Group].Points[i] != 0) {
            int point = cruise[Group].Points[i];

            // 查找当前点在 presets 中的索引
            if (presets[point].is_set ) {
                // 找到了对应的预置位，使用其 x_steps 和 y_steps 控制步进电机
                LOG_DEBUG("找到预置位点 %d，x_steps=%d, y_steps=%d\n",
                       point, presets[point].x_steps, presets[point].y_steps);
                motor_run_absolute(presets[point].x_steps, presets[point].y_steps);

                // 停留一段时间，模拟时间控制
                sleep(time);
            } else {
                // 未找到对应的预置位，跳过当前点
                LOG_INFO("未找到预置位点 %d，跳过\n", point);
            }
        }
    }
}
