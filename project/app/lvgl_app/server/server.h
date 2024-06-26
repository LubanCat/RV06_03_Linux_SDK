#ifndef SERVER_H
#define SERVER_H

#include <time.h>


// 登录配置服务器响应
typedef struct {
    char ip[64];
    int tcpPort;
} LoginConfigResponse;

// 登录服务器响应
typedef struct {
    char qrCode[128];
    double recyclePrice; // 价格，包含小数
    int heartBeatInterval; // 时间，单位为秒
    struct tm turnOnLight;
    struct tm turnOffLight;
} LoginResponse;

// 心跳发送参数
typedef struct {
    int signal;
    double curWeight;
} HeartBeatParams;

// 服务器开门请求
typedef struct {
    char transId[64];  // 事务ID
    int openType;      // 开门类型 1：投递回收物 2：清理回收物
} OpenDoorRequest;

// 终端开门回复
typedef struct {
    int retCode;       // 0:成功，1：失败
    char transId[64];  // 事务ID
    double curWeight;  // 当前重量（千克）
} OpenDoorResponse;

// 终端到服务器关门请求
typedef struct {
    char transId[32];   // 事务ID
    double curWeight;   // 当前重量，单位千克
    double changeWeight; // 改变重量，单位千克
} CloseDoorRequest;

// 定义上传响应结构体
typedef struct {
    int code;
    char msg[256];
} UploadResponse;

// 获取登录配置
int get_login_config(const char *iccid, const char *sn, const char *key, LoginConfigResponse *response);

// 登录
int login(const char *imsi, const char *imei, const char *version, const char *iccid, const char *sn, LoginResponse *response);

// 上传心跳
int upload_heartbeat(const HeartBeatParams *params);

// 开门
int open_door(OpenDoorResponse *response);

// 关门
int close_door(CloseDoorRequest *request);

// 上传超重
int upload_overweight(int overweight);

// 上传满载
int upload_overflow(int overflow);

// 上传文件通用函数
int upload_file(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, const char *file_type, int photoType, UploadResponse *response);

// 上传照片
int upload_photo(const char *url, const char *sn, const char *iccid, const char *transId, int photoType, const char *file_path, UploadResponse *response);

// 上传视频
int upload_video(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, UploadResponse *response);

int server();

#endif // SERVER_H
