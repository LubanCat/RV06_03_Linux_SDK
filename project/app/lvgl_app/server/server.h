#ifndef SERVER_H
#define SERVER_H

// 定义登录配置响应结构体
typedef struct {
    int retCode;
    char ip[64];
    int tcpPort;
} LoginConfigResponse;

// 定义登录响应结构体
typedef struct {
    int retCode;
    char qrCode[256];
    double recyclePrice;
    int heartBeatInterval;
    char turnOnLight[5];
    char turnOffLight[5];
} LoginResponse;

// 定义心跳响应结构体
typedef struct {
    int retCode;
} HeartBeatResponse;

// 定义开门响应结构体
typedef struct {
    int retCode;
    char transId[64];
    int curWeight;
} OpenDoorResponse;

// 定义关门响应结构体
typedef struct {
    int retCode;
} CloseDoorResponse;

// 定义超重响应结构体
typedef struct {
    int retCode;
} OverweightResponse;

// 定义溢满响应结构体
typedef struct {
    int retCode;
} OverflowResponse;

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
int upload_heartbeat(const char *sn, int signal, double curWeight, HeartBeatResponse *response);

// 开门
int open_door(const char *sn, int box, OpenDoorResponse *response);

// 关门
int close_door(const char *sn, int box, CloseDoorResponse *response);

// 上传超重
int upload_overweight(const char *sn, int box, int overweight);

// 上传满载
int upload_overflow(const char *sn, int box, int overflow);

// 上传文件通用函数
int upload_file(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, const char *file_type, int photoType, UploadResponse *response);

// 上传照片
int upload_photo(const char *url, const char *sn, const char *iccid, const char *transId, int photoType, const char *file_path, UploadResponse *response);

// 上传视频
int upload_video(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, UploadResponse *response);

int server();

#endif // SERVER_H
