#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// 定义服务器地址和端口
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

// 获取登录配置
int get_login_config(const char *iccid, const char *sn, const char *key, LoginConfigResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "loginConfig");
    cJSON_AddStringToObject(root, "iccid", iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddStringToObject(root, "key", key);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录配置请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        cJSON *ip = cJSON_GetObjectItem(response_json, "ip");
        cJSON *tcpPort = cJSON_GetObjectItem(response_json, "tcpPort");

        if (retCode && ip && tcpPort) {
            response->retCode = retCode->valueint;
            strncpy(response->ip, ip->valuestring, sizeof(response->ip));
            response->tcpPort = tcpPort->valueint;
        } else {
            printf("Invalid response\n");
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
int login(const char *imsi, const char *imei, const char *version, const char *iccid, const char *sn, LoginResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[512];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "login");
    cJSON_AddStringToObject(root, "imsi", imsi);
    cJSON_AddStringToObject(root, "imei", imei);
    cJSON_AddStringToObject(root, "version", version);
    cJSON_AddStringToObject(root, "iccid", iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        cJSON *qrCode = cJSON_GetObjectItem(response_json, "qrCode");
        cJSON *recyclePrice = cJSON_GetObjectItem(response_json, "recyclePrice");
        cJSON *heartBeatInterval = cJSON_GetObjectItem(response_json, "heartBeatInterval");
        cJSON *turnOnLight = cJSON_GetObjectItem(response_json, "turnOnLight");
        cJSON *turnOffLight = cJSON_GetObjectItem(response_json, "turnOffLight");

        if (retCode && qrCode && recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
            response->retCode = retCode->valueint;
            strncpy(response->qrCode, qrCode->valuestring, sizeof(response->qrCode));
            response->recyclePrice = recyclePrice->valuedouble;
            response->heartBeatInterval = heartBeatInterval->valueint;
            strncpy(response->turnOnLight, turnOnLight->valuestring, sizeof(response->turnOnLight));
            strncpy(response->turnOffLight, turnOffLight->valuestring, sizeof(response->turnOffLight));
        } else {
            printf("Invalid response\n");
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

// 上传心跳
int upload_heartbeat(const char *sn, int signal, double curWeight, HeartBeatResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "heartBeat");
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddNumberToObject(root, "signal", signal);
    cJSON_AddNumberToObject(root, "curWeight", curWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送心跳上传请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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

        if (retCode) {
            response->retCode = retCode->valueint;
        } else {
            printf("Invalid response\n");
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

// 开门
int open_door(const char *sn, int box, OpenDoorResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "openDoor");
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddNumberToObject(root, "box", box);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送开门请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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

        if (retCode) {
            response->retCode = retCode->valueint;
        } else {
            printf("Invalid response\n");
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

// 关门
int close_door(const char *sn, int box, CloseDoorResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "closeDoor");
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddNumberToObject(root, "box", box);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送关门请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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

        if (retCode) {
            response->retCode = retCode->valueint;
        } else {
            printf("Invalid response\n");
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

// 上传超重
int upload_overweight(const char *sn, int box, int overweight) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "uploadOverweight");
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddNumberToObject(root, "box", box);
    cJSON_AddNumberToObject(root, "overweight", overweight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送超重上传请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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

        cJSON_Delete(response_json);
    }

    // 关闭套接字
    close(sock);

    return 0;
}

// 上传满载
int upload_overflow(const char *sn, int box, int overflow) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[256], recv_buf[256];
    int recv_len;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "uploadOverflow");
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddNumberToObject(root, "box", box);
    cJSON_AddNumberToObject(root, "overflow", overflow);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送满载上传请求
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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

        cJSON_Delete(response_json);
    }

    // 关闭套接字
    close(sock);

    return 0;
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
int upload_file(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, const char *file_type, int photoType, UploadResponse *response) {
    if (!url || !sn || !iccid || !transId || !file_path || !file_type || !response) {
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
        form = curl_mime_init(curl);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "sn");
        curl_mime_data(field, sn, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "iccid");
        curl_mime_data(field, iccid, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "transId");
        curl_mime_data(field, transId, CURL_ZERO_TERMINATED);

        if (strcmp(file_type, "photo") == 0) {
            field = curl_mime_addpart(form);
            curl_mime_name(field, "photoType");
            curl_mime_data(field, (photoType == 1 ? "1" : "0"), CURL_ZERO_TERMINATED); // 1: 开门照片, 0: 其他照片
        }

        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, file_path);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

        // 捕获返回数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)recv_buf);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_mime_free(form);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        } else {
            printf("%s uploaded successfully.\n", file_type);

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
                response->code = code->valueint;
                strncpy(response->msg, msg->valuestring, sizeof(response->msg) - 1);
                response->msg[sizeof(response->msg) - 1] = '\0';
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
int upload_photo(const char *url, const char *sn, const char *iccid, const char *transId, int photoType, const char *file_path, UploadResponse *response) {
    return upload_file(url, sn, iccid, transId, file_path, "photo", photoType, response);
}

// 上传视频
int upload_video(const char *url, const char *sn, const char *iccid, const char *transId, const char *file_path, UploadResponse *response) {
    return upload_file(url, sn, iccid, transId, file_path, "video", 0, response);
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "server.h"

// 心跳线程函数
void *heartbeat_thread(void *arg) {
    const char *sn = (const char *)arg;
    HeartBeatResponse heartbeatResponse;
    int signal_strength = 80;
    double current_weight = 15.5;

    while (1) {
        // 发送心跳包
        int ret = upload_heartbeat(sn, signal_strength, current_weight, &heartbeatResponse);
        if (ret == 0) {
            printf("Heartbeat sent successfully.\n");
        } else {
            printf("Failed to send heartbeat.\n");
        }

        // 休眠 30 秒
        sleep(30);
    }

    return NULL;
}

int server() {
    // 设备信息
    const char *iccid = "1234567890";
    const char *sn = "DEVICE123";
    const char *key = "device_key";
    const char *imsi = "IMSI123456";
    const char *imei = "IMEI789012";
    const char *version = "1.0";

    // 文件路径
    const char *photo_file = "/path/to/photo.jpg";
    const char *video_file = "/path/to/video.mp4";

    // 获取登录配置
    LoginConfigResponse loginConfigResponse;
    int ret = get_login_config(iccid, sn, key, &loginConfigResponse);
    if (ret == 0) {
        printf("Login configuration:\n");
        printf("IP: %s\n", loginConfigResponse.ip);
        printf("TCP Port: %d\n", loginConfigResponse.tcpPort);
    } else {
        printf("Failed to get login configuration.\n");
        return 1;
    }

    // 创建心跳线程
    pthread_t tid;
    int thread_err = pthread_create(&tid, NULL, heartbeat_thread, (void *)sn);
    if (thread_err != 0) {
        fprintf(stderr, "Failed to create heartbeat thread: %s\n", strerror(thread_err));
        return 1;
    }

    // 登录
    LoginResponse loginResponse;
    ret = login(imsi, imei, version, iccid, sn, &loginResponse);
    if (ret == 0) {
        printf("Login successful:\n");
        printf("QR Code: %s\n", loginResponse.qrCode);
        printf("Recycle Price: %.2f\n", loginResponse.recyclePrice);
        printf("Heartbeat Interval: %d seconds\n", loginResponse.heartBeatInterval);
        printf("Turn on Light Time: %s\n", loginResponse.turnOnLight);
        printf("Turn off Light Time: %s\n", loginResponse.turnOffLight);
    } else {
        printf("Login failed.\n");
        return 1;
    }

    // 下发开门
    OpenDoorResponse openDoorResponse;
    ret = open_door(sn, 1, &openDoorResponse); // 开门类型为 1: 投递回收物
    if (ret == 0) {
        printf("Open door request sent successfully.\n");
        printf("Transaction ID: %s\n", openDoorResponse.transId);
        printf("Current Weight: %d kg\n", openDoorResponse.curWeight);
    } else {
        printf("Failed to send open door request.\n");
    }
    
    // 上传关门
    CloseDoorResponse closeDoorResponse;
    ret = close_door(sn, 1, &closeDoorResponse); // 假设箱子编号为1
    if (ret == 0) {
        printf("Close door request sent successfully.\n");
    } else {
        printf("Failed to send close door request.\n");
    }

    // 上传超重
    OverweightResponse overweightResponse;
    ret = upload_overweight(sn, 25.3, &overweightResponse);
    if (ret == 0) {
        printf("Overweight uploaded successfully.\n");
    } else {
        printf("Failed to upload overweight.\n");
        return 1;
    }

    // 上传溢满
    OverflowResponse overflowResponse;
    ret = upload_overflow(sn, 3, &overflowResponse); // 当前容量为 3: 高
    if (ret == 0) {
        printf("Overflow uploaded successfully.\n");
    } else {
        printf("Failed to upload overflow.\n");
        return 1;
    }

    // 上传照片
    UploadResponse photoUploadResponse;
    ret = upload_photo("http://serverIp:port/api/device/upload/photo", sn, iccid, "trans123", 1, photo_file, &photoUploadResponse);
    if (ret == 0) {
        printf("Photo uploaded successfully. Server response: %s\n", photoUploadResponse.msg);
    } else {
        printf("Failed to upload photo.\n");
        return 1;
    }

    // 上传视频
    UploadResponse videoUploadResponse;
    ret = upload_video("http://serverIp:port/api/device/upload/video", sn, iccid, "trans123", video_file, &videoUploadResponse);
    if (ret == 0) {
        printf("Video uploaded successfully. Server response: %s\n", videoUploadResponse.msg);
    } else {
        printf("Failed to upload video.\n");
        return 1;
    }

    return 0;
}
