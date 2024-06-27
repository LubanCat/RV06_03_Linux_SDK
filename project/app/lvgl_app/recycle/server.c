#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <pthread.h>
#include "module.h"
#include <sys/msg.h>

#define MAX_BUF_SIZE 512

// 定义服务器地址和端口
// #define CONFIG_SERVER_IP "192.168.103.142"
// #define CONFIG_SERVER_PORT 12345

#define CONFIG_SERVER_IP "47.101.195.22"
#define CONFIG_SERVER_PORT 9096

int msgid;
typedef enum {
    TYPE_HEARTBEAT  = 1,
    TYPE_OPENDOOR  = 2,
    TYPE_CLOSEDOOR  = 3,
    TYPE_OVERWEIGHT  = 4,
    TYPE_OVERFLOWER  = 5,
} receive_type;

typedef struct {
    receive_type type; // 消息类型
    int retcode;
    char trans_id[64];
    int open_type;
} message_queue_t;

// 设备信息
char *sn = "DEVICE123";
char *key = "device_key";
char *version = "1.0";

// 文件路径
char *photo_file = "/root/photo.jpg";
char *video_file = "/root/video.mp4";

int sock;
char trans_Id[64];  // 事务ID

// 声明互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

LoginConfig loginConfig;
LoginRes loginRes;
OpenDoor openDoor;
CloseDoor closeDoor;
HeartBeat heartBeat;

// 获取当前重量
double get_current_weight() {
    // 从硬件或其他数据源获取当前重量
    return 12.5; // 示例值
}

int send_tcp_packet(char *buf,int size){
    pthread_mutex_lock(&mutex);
    send(sock, buf, size, 0);
    pthread_mutex_unlock(&mutex);
    return 1;
}

// 获取登录配置
int get_login_config() {
    struct sockaddr_in server_addr;
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONFIG_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(CONFIG_SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "loginConfig");
    cJSON_AddStringToObject(root, "iccid", moduleInfo.iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddStringToObject(root, "key", key);
    char *json_str = cJSON_PrintUnformatted(root);

    // 锁定互斥锁

    // 发送登录配置请求
    printf("Sending loginConfig request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);

    if (recv_len < 0) {
        perror("Receive failed");
        close(sock);
        return -1;
    } else {
        recv_buf[recv_len] = '\0';
        printf("Server response: %s\n", recv_buf);

        // 解析 JSON 响应
        cJSON *response_json = cJSON_Parse(recv_buf);
        if (response_json == NULL) {
            printf("JSON parse error\n");
            close(sock);
            return -1;
        }

        cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
        if (retCode == NULL || !cJSON_IsNumber(retCode)) {
            printf("Invalid retCode\n");
            cJSON_Delete(response_json);
            close(sock);
            return -1;
        }

        switch (retCode->valueint) {
            case 0:
                // 成功，获取额外数据
                {
                    cJSON *ip = cJSON_GetObjectItem(response_json, "ip");
                    cJSON *tcpPort = cJSON_GetObjectItem(response_json, "tcp");

                    if (ip && tcpPort) {
                        strncpy(loginConfig.ip, ip->valuestring, sizeof(loginConfig.ip)-1);
                        loginConfig.tcpPort = tcpPort->valueint;
                    } else {
                        printf("Missing ip or tcpPort in response\n");
                        cJSON_Delete(response_json);
                        close(sock);
                        return -1;
                    }
                }
                break;
            case 1:
                printf("被服务商禁用\n");
                break;
            case 2:
                printf("被管理员禁用\n");
                break;
            case 3:
                printf("SN或KEY错误\n");
                break;
            case 4:
                printf("请绑定设备\n");
                break;
            default:
                printf("Unknown retCode\n");
                cJSON_Delete(response_json);
                close(sock);
                return -1;
        }

        cJSON_Delete(response_json);
    }

    // 关闭套接字
    close(sock);

    return 0;
}

// 登录
int server_login() {

    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "login");
    cJSON_AddStringToObject(root, "imsi", moduleInfo.imsi);
    cJSON_AddStringToObject(root, "imei", moduleInfo.imei);
    cJSON_AddStringToObject(root, "version", version);
    cJSON_AddStringToObject(root, "iccid", moduleInfo.iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录请求
    printf("Sending login request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);

    if (recv_len < 0) {
        perror("Receive failed");
        return -1;
    } else {
        recv_buf[recv_len] = '\0';
        printf("Server response: %s\n", recv_buf);

        // 解析 JSON 响应
        cJSON *response_json = cJSON_Parse(recv_buf);
        if (response_json == NULL) {
            printf("JSON parse error\n");
            return -1;
        }

        cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
        if (retCode == NULL || !cJSON_IsNumber(retCode)) {
            printf("Invalid retCode\n");
            cJSON_Delete(response_json);
            return -1;
        }

        switch (retCode->valueint) {
            case 0: {
                cJSON *qrCode = cJSON_GetObjectItem(response_json, "qrCode");
                cJSON *recyclePrice = cJSON_GetObjectItem(response_json, "recyclePrice");
                cJSON *heartBeatInterval = cJSON_GetObjectItem(response_json, "heartBeatInterval");
                cJSON *turnOnLight = cJSON_GetObjectItem(response_json, "turnOnLight");
                cJSON *turnOffLight = cJSON_GetObjectItem(response_json, "turnOffLight");

                if (qrCode && recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
                // if (recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
                    strncpy(loginRes.qrCode, qrCode->valuestring, sizeof(loginRes.qrCode) - 1);
                    loginRes.qrCode[sizeof(loginRes.qrCode) - 1] = '\0';
                    loginRes.recyclePrice = atof(recyclePrice->valuestring);
                    loginRes.heartBeatInterval = atoi(heartBeatInterval->valuestring);

                    // 解析 turnOnLight 和 turnOffLight 时间字符串
                    memset(&loginRes.turnOnLight, 0, sizeof(struct tm));
                    memset(&loginRes.turnOffLight, 0, sizeof(struct tm));

                    if (sscanf(turnOnLight->valuestring, "%2d%2d", &loginRes.turnOnLight.tm_hour, &loginRes.turnOnLight.tm_min) != 2) {
                        printf("Failed to parse turnOnLight time\n");
                        cJSON_Delete(response_json);
                        return -1;
                    }

                    if (sscanf(turnOffLight->valuestring, "%2d%2d", &loginRes.turnOffLight.tm_hour, &loginRes.turnOffLight.tm_min) != 2) {
                        printf("Failed to parse turnOffLight time\n");
                        cJSON_Delete(response_json);
                        return -1;
                    }

                    // 打印获取的时间
                    char turnOnLightBuf[6], turnOffLightBuf[6];
                    strftime(turnOnLightBuf, sizeof(turnOnLightBuf), "%H:%M", &loginRes.turnOnLight);
                    strftime(turnOffLightBuf, sizeof(turnOffLightBuf), "%H:%M", &loginRes.turnOffLight);

                    printf("Turn On Light Time: %s\n", turnOnLightBuf);
                    printf("Turn Off Light Time: %s\n", turnOffLightBuf);
                } else {
                    printf("Invalid response: missing expected fields\n");
                    cJSON_Delete(response_json);
                    return -1;
                }
                break;
            }
            case 1:
                printf("Login failed: 被服务商禁用\n");
                break;
            case 2:
                printf("Login failed: 被管理员禁用\n");
                break;
            case 3:
                printf("Login failed: SN或KEY错误\n");
                break;
            default:
                printf("Unknown retCode\n");
                cJSON_Delete(response_json);
                return -1;
        }
        cJSON_Delete(response_json);
    }
    return 0;
}

void *tcp_receiver(void *arg) {
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    printf("tcp_receiver\n");
    while (1) {
        recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);
        printf("sock = %d ,recv_len=%d\n ",sock,recv_len);
        if (recv_len < 0) {
            printf("wrong\n");
        } else {
            recv_buf[recv_len] = '\0';
            printf("Server response: %s\n", recv_buf);

            // 解析 JSON 响应
            cJSON *response_json = cJSON_Parse(recv_buf);
            if (response_json == NULL) {
                printf("JSON parse error\n");
            }

            cJSON *cmd = cJSON_GetObjectItem(response_json, "cmd");
            if (cmd == NULL) {
                printf("Invalid retCode\n");
                cJSON_Delete(response_json);
            }
            char str[20];
            strncpy(str, cmd->valuestring, sizeof(str)-1);
            message_queue_t msg;
            printf("get %s\n",str);
            if(strcmp(str,"openDoor")==0){
                msg.type=TYPE_OPENDOOR;
                cJSON *transId = cJSON_GetObjectItem(response_json, "transId");
                cJSON *openType = cJSON_GetObjectItem(response_json, "openType");

                strncpy(trans_Id, transId->valuestring, sizeof(trans_Id) - 1);
                trans_Id[sizeof(trans_Id) - 1] = '\0';
                openDoor.openType = openType->valueint;

                openDoor.curWeight = get_current_weight();  // Example function to get current weight
                cJSON_Delete(response_json);

                if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
                    perror("msgsnd failed");
                    exit(EXIT_FAILURE);
                }

                int retCode = 0;

                cJSON *root = cJSON_CreateObject();
                cJSON_AddStringToObject(root, "cmd", "openDoor");
                cJSON_AddNumberToObject(root, "retCode", retCode);  // Assuming retCode is set elsewhere
                cJSON_AddStringToObject(root, "transId", trans_Id);
                cJSON_AddNumberToObject(root, "curWeight", openDoor.curWeight);
                char *json_str = cJSON_PrintUnformatted(root);

                // Send response
                printf("Sending openDoor response: %s\n", json_str);
                send_tcp_packet(json_str, strlen(json_str));
                free(json_str);
                cJSON_Delete(root);
            }else{
                if(strcmp(str,"closeDoor")==0){
                    msg.type=TYPE_CLOSEDOOR;
                }else if(strcmp(str,"heartBeat")==0){
                    msg.type=TYPE_HEARTBEAT;
                }else if(strcmp(str,"overweight")==0){
                    msg.type=TYPE_OVERWEIGHT;
                }else if(strcmp(str,"overflow")==0){
                    msg.type=TYPE_OVERFLOWER;
                }
                cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
                if (retCode == NULL || !cJSON_IsNumber(retCode)) {
                    printf("Invalid retCode\n");
                    cJSON_Delete(response_json);
                }

                msg.retcode  = retCode->valueint;
                cJSON_Delete(response_json);

                if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
                    perror("msgsnd failed");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}


// 心跳上传函数
// int upload_heartbeat() {
//     // 创建 JSON 请求
//     cJSON *root = cJSON_CreateObject();
//     cJSON_AddStringToObject(root, "cmd", "heartBeat");
//     cJSON_AddNumberToObject(root, "signal", heartBeat->signal);
//     cJSON_AddNumberToObject(root, "curWeight", heartBeat->curWeight);
//     char *json_str = cJSON_PrintUnformatted(root);

//     // 发送心跳上传请求
//     printf("Sending heartBeat request: %s\n", json_str);
//     send_tcp_packet(json_str, strlen(json_str));
//     free(json_str);
//     cJSON_Delete(root);
    
//     // while(1);
//     message_queue_t msg;
//     if (msgrcv(msgid, &msg, sizeof(msg), 1, 0) == -1) {
//         perror("msgrcv failed");
//         exit(EXIT_FAILURE);
//     }
//     return msg.retcode;
// }

// 心跳线程函数
void *heartbeat_thread(void *arg) {

    while (1) {
        heartBeat.signal = module_signal();
        heartBeat.curWeight = get_current_weight();

        // 发送心跳包
        // ret_heartbeat = upload_heartbeat();

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "heartBeat");
        cJSON_AddNumberToObject(root, "signal", heartBeat.signal);
        cJSON_AddNumberToObject(root, "curWeight", heartBeat.curWeight);
        char *json_str = cJSON_PrintUnformatted(root);

        // 发送心跳上传请求
        printf("Sending heartBeat request: %s\n", json_str);
        send_tcp_packet(json_str, strlen(json_str));
        free(json_str);
        cJSON_Delete(root);

        // while(1);
        message_queue_t msg;
        if (msgrcv(msgid, &msg, sizeof(msg), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        if (msg.retcode == 0) {
            printf("Heartbeat sent successfully.\n");
        } else {
            printf("Failed to send heartbeat.\n");
        }
        printf("\n");

        // 休眠
        // sleep(loginRes.heartBeatInterval);
        sleep(5);
    }

    return NULL;
}

// 开门
int open_door() {
    message_queue_t msg;
    if (msgrcv(msgid, &msg, sizeof(msg), TYPE_OPENDOOR, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int close_door() {
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    snprintf(trans_Id, sizeof(trans_Id), "1");  // 替换为实际生成的事务ID
    closeDoor.curWeight = get_current_weight();
    closeDoor.changeWeight = 0.5;  // 替换为实际改变的重量

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "closeDoor");
    cJSON_AddStringToObject(root, "transId", trans_Id);
    cJSON_AddNumberToObject(root, "curWeight", closeDoor.curWeight);
    cJSON_AddNumberToObject(root, "changeWeight", closeDoor.changeWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送关门请求
    printf("Sending closeDoor request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    message_queue_t msg;
    if (msgrcv(msgid, &msg, sizeof(msg), TYPE_CLOSEDOOR, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    // printf("Received:%d %d\n", msg.type,msg.retcode);
    return msg.retcode;
}

// 上传超重
int upload_overweight(void) {
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    double curWeight = get_current_weight();

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overweight");
    cJSON_AddNumberToObject(root, "curWeight", curWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送超重上传请求
    printf("Sending overweight request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    message_queue_t msg;
    if (msgrcv(msgid, &msg, sizeof(msg), TYPE_OVERWEIGHT, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    // printf("Received:%d %d\n", msg.type,msg.retcode);
    return msg.retcode;
}

// 上传满载
int upload_overflow(int curCapacity) {
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overflow");
    cJSON_AddNumberToObject(root, "curCapacity", curCapacity);
    char *json_str = cJSON_PrintUnformatted(root);


    // 发送满载上传请求
    printf("Sending overflow request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    message_queue_t msg;
    if (msgrcv(msgid, &msg, sizeof(msg), TYPE_OVERFLOWER, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    // printf("Received:%d %d\n", msg.type,msg.retcode);
    return msg.retcode;
}

// 回调函数来捕获返回的数据
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *recv_buf = (char *)userp;
    if (recv_buf != NULL) {
        strncat(recv_buf, contents, realsize);
    }
    return realsize;
}

// 上传文件的通用函数
int upload_file(const char *url, const char *file_path, const char *file_type, int photoType) {
    int res_code;
    char res_msg[256];

    if (!url || !sn || !moduleInfo.iccid || !trans_Id || !file_path || !file_type) {
        fprintf(stderr, "Invalid input parameters\n");
        return -1;
    }

    CURL *curl;
    CURLcode res;
    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    char recv_buf[256] = {0};

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        printf("Initializing curl...\n");

        form = curl_mime_init(curl);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "sn");
        curl_mime_data(field, sn, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "iccid");
        curl_mime_data(field, moduleInfo.iccid, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "transId");
        curl_mime_data(field, trans_Id, CURL_ZERO_TERMINATED);
        printf("Added form part: transId = %s\n", trans_Id);

        if (strcmp(file_type, "photo") == 0) {
            field = curl_mime_addpart(form);
            curl_mime_name(field, "photoType");
            curl_mime_data(field, (photoType == 1 ? "1" : "0"), CURL_ZERO_TERMINATED); // 1: 开门照片, 0: 其他照片
        }

        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, file_path);
        printf("Added form part: file = %s\n", file_path);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        printf("Set URL: %s\n", url);

        // 捕获返回数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)recv_buf);
        printf("Set write function and buffer\n");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_mime_free(form);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        } else {
            printf("File uploaded successfully. Server response: %s\n", recv_buf);

            // 使用 cJSON 解析返回消息
            cJSON *json = cJSON_Parse(recv_buf);
            if (json == NULL) {
                fprintf(stderr, "JSON parse error\n");
                curl_mime_free(form);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return -1;
            }
            cJSON *code = cJSON_GetObjectItem(json, "code");
            cJSON *msg = cJSON_GetObjectItem(json, "msg");
            if (code && cJSON_IsNumber(code) && msg && cJSON_IsString(msg)) {
                res_code = code->valueint;
                strncpy(res_msg, msg->valuestring, sizeof(res_msg) - 1);
                res_msg[sizeof(res_msg) - 1] = '\0';
                printf("Parsed response: code = %d, msg = %s\n", res_code, res_msg);
            } else {
                fprintf(stderr, "Invalid JSON response\n");
                cJSON_Delete(json);
                curl_mime_free(form);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return -1;
            }
            cJSON_Delete(json);
        }

        curl_mime_free(form);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

// 上传照片
int upload_photo(const char *url, int photoType, const char *file_path) {
    return upload_file(url, file_path, "photo", photoType);
}

// 上传视频
int upload_video(const char *url, const char *file_path) {
    return upload_file(url, file_path, "video", 0);
}

int server() {
    int ret;

    // 获取登录配置
    ret = get_login_config(&loginConfig);
    if (ret == 0) {
        printf("Login configuration:\n");
        printf("IP: %s\n", loginConfig.ip);
        printf("TCP Port: %d\n", loginConfig.tcpPort);
        printf("\n");
    } else {
        printf("Failed to get login configuration.\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    struct timeval timeout;
    timeout.tv_sec = 2;  // 超时时间 秒
    timeout.tv_usec = 0; // 微秒设置

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // // 设置接收超时时间
    // if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
    //     perror("Set socket receive timeout failed");
    //     close(sock);
    //     return -1;
    // }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(loginConfig.tcpPort);
    server_addr.sin_addr.s_addr = inet_addr(loginConfig.ip);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 登录
    ret = server_login();
    if (ret == 0) {
        printf("Login successful:\n");
        printf("QR Code: %s\n", loginRes.qrCode);
        printf("Recycle Price: %.2f\n", loginRes.recyclePrice);
        printf("Heartbeat Interval: %d seconds\n", loginRes.heartBeatInterval);
        printf("\n");
    } else {
        printf("Login failed.\n");
        close(sock);
        return -1;
    }

    // 创建心跳线程
    pthread_t heartbeat_thread_id;
    if (pthread_create(&heartbeat_thread_id, NULL, heartbeat_thread, NULL) != 0) {
        perror("Failed to create heartbeat thread");
        return -1;
    }

    // 创建心跳线程
    pthread_t tcp_receiver_thread_id;
    if (pthread_create(&tcp_receiver_thread_id, NULL, tcp_receiver, NULL) != 0) {
        perror("Failed to create heartbeat thread");
        return -1;
    }

    // while (1){
    //     sleep(1);
    // }

    // // 下发开门
    // if (open_door() == 0) {
    //     printf("Open door process completed\n");
    //     printf("transId: %s\n", trans_Id);
    //     printf("openType: %d\n", openDoor.openType);
    // } else {
    //     printf("Open door process failed\n");
    // }

    // 上传关门
    ret = close_door(); // 假设箱子编号为1
    if (ret == 0) {
        printf("Close door request sent successfully.\n");
        printf("\n");
    } else {
        printf("Failed to send close door request.\n");
        printf("\n");
    }

    // 上传超重
    ret = upload_overweight();
    if (ret == 0) {
        printf("Overweight uploaded successfully.\n");
        printf("\n");
    } else {
        printf("Failed to upload overweight.\n");
        printf("\n");
    }

    // 上传溢满
    ret = upload_overflow(3); // 当前容量为 3: 高
    if (ret == 0) {
        printf("Overflow uploaded successfully.\n");
        printf("\n");
    } else {
        printf("Failed to upload overflow.\n");
        printf("\n");
    }

    // 上传照片
    ret = upload_photo("http://api.recycling.maizucity.com/api/device/upload/photo", 1, photo_file);
    if (ret != 0) {
        printf("Failed to upload photo.\n");
        return 1;
    }

    // 上传视频
    // ret = upload_video("http://serverIp:port/api/device/upload/video", video_file, &videoUploadResponse);
    ret = upload_video("http://api.recycling.maizucity.com/api/device/upload/video", video_file);
    if (ret != 0) {
        printf("Failed to upload video.\n");
        return 1;
    }

    // pthread_join(heartbeat_thread_id, NULL);

    // close(sock);
    return 0;
}
