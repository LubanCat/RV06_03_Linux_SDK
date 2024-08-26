#ifndef SERVER_H
#define SERVER_H

#include <time.h>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"
#include "rkmuxer.h"


// 登录配置服务器响应
typedef struct {
    char ip[64];
    int tcpPort;
} LoginConfig;

// 登录服务器响应
typedef struct {
    char qrCode[128];
    double recyclePrice; // 价格，包含小数
    int heartBeatInterval; // 时间，单位为秒
    struct tm turnOnLight;
    struct tm turnOffLight;
} LoginRes;

// 心跳发送参数
typedef struct {
    int signal;
    double curWeight;
} HeartBeat;

// 开门参数
typedef struct {
    int openType;      // 开门类型 1：投递回收物 2：清理回收物
    double curWeight;  // 当前重量（千克）
} OpenDoor;

// 关门参数
typedef struct {
    double curWeight;   // 当前重量，单位千克
    double changeWeight; // 改变重量，单位千克
} CloseDoor;

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

extern LoginConfig loginConfig;
extern LoginRes loginRes;
extern OpenDoor openDoor;
extern CloseDoor closeDoor;
extern HeartBeat heartBeat;

// 获取登录配置
int get_login_config();

// 登录
int server_login();

// 上传心跳
int upload_heartbeat();

// 开门
int open_door();

// 关门
int close_door(double cur_weight, double diff_weight);

// 上传超重
int upload_overweight(double curWeight);

// 上传满载
int upload_overflow(int curCapacity);

// 上传异常
int upload_fault(FaultType type);

// 上传文件通用函数
int upload_file(const char *url, const char *file_path, const char *file_type, int photoType);

// 上传照片
int upload_photo(const char *url, int photoType, const char *file_path);

// 上传视频
int upload_video(const char *url, const char *file_path);

// 服务器注册和通信
void* socket_task_thread(void* arg);

// 读取配置文件
int read_device_info();

// 写配置文件
int write_device_info(const char *key_name, const char *value);

// 系统读配置文件接口
int read_info();
int allow_phone_login(char *phone_number);
int get_overflow_status();
int send_tcp_packet(char *buf,int size);
void *tcp_receiver(void *arg);
void *heartbeat_thread(void *arg);
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void camara_test_start(int num);
void camara_test_stop();

#endif // SERVER_H
