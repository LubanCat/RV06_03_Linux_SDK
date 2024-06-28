#ifndef SERVER_H
#define SERVER_H

#include <time.h>


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
int close_door();

// 上传超重
int upload_overweight();

// 上传满载
int upload_overflow(int curCapacity);

// 上传文件通用函数
int upload_file(const char *url, const char *file_path, const char *file_type, int photoType);

// 上传照片
int upload_photo(const char *url, int photoType, const char *file_path);

// 上传视频
int upload_video(const char *url, const char *file_path);

// 服务器注册和通信
int server_start();

// 读取配置文件
int read_info();

int get_overflow_status();
double get_current_weight();
int send_tcp_packet(char *buf,int size);
void *tcp_receiver(void *arg);
void *heartbeat_thread(void *arg);
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // SERVER_H
