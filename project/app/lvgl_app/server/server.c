#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <pthread.h>

#define MAX_BUF_SIZE 512
// 定义服务器地址和端口
// #define CONFIG_SERVER_IP "192.168.103.142"
// #define CONFIG_SERVER_PORT 12345

#define CONFIG_SERVER_IP "47.101.195.22"
#define CONFIG_SERVER_PORT 9096

int sock;

LoginConfigResponse loginConfigResponse;
LoginResponse loginResponse;
OpenDoorRequest openDoorRequest;
OpenDoorResponse openDoorResponse;
CloseDoorRequest closeDoorRequest;


// 获取登录配置
int get_login_config(const char *iccid, const char *sn, const char *key, LoginConfigResponse *response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
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
    cJSON_AddStringToObject(root, "iccid", iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddStringToObject(root, "key", key);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录配置请求
    printf("Sending loginConfig request: %s\n", json_str);
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
                        strncpy(response->ip, ip->valuestring, sizeof(response->ip));
                        response->tcpPort = tcpPort->valueint;
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
int login(const char *imsi, const char *imei, const char *version, const char *iccid, const char *sn, LoginResponse *response) {

    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

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
    printf("Sending login request: %s\n", json_str);
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
                // cJSON *qrCode = cJSON_GetObjectItem(response_json, "qrCode");
                cJSON *recyclePrice = cJSON_GetObjectItem(response_json, "recyclePrice");
                cJSON *heartBeatInterval = cJSON_GetObjectItem(response_json, "heartBeatInterval");
                cJSON *turnOnLight = cJSON_GetObjectItem(response_json, "turnOnLight");
                cJSON *turnOffLight = cJSON_GetObjectItem(response_json, "turnOffLight");

                // if (qrCode && recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
                if (recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
                    // strncpy(response->qrCode, qrCode->valuestring, sizeof(response->qrCode) - 1);
                    // response->qrCode[sizeof(response->qrCode) - 1] = '\0';
                    response->recyclePrice = atof(recyclePrice->valuestring);
                    response->heartBeatInterval = atoi(heartBeatInterval->valuestring);
                    strncpy(response->turnOnLight, turnOnLight->valuestring, sizeof(response->turnOnLight) - 1);
                    response->turnOnLight[sizeof(response->turnOnLight) - 1] = '\0';
                    strncpy(response->turnOffLight, turnOffLight->valuestring, sizeof(response->turnOffLight) - 1);
                    response->turnOffLight[sizeof(response->turnOffLight) - 1] = '\0';
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

// 上传心跳
int upload_heartbeat(const HeartBeatParams *params) {
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "heartBeat");
    cJSON_AddNumberToObject(root, "signal", params->signal);
    cJSON_AddNumberToObject(root, "curWeight", params->curWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送心跳上传请求
    printf("Sending heartBeat request: %s\n", json_str);
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    sleep(1);
    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        
        if (retCode->valueint == 0 ){
            cJSON_Delete(response_json);
            return 0;
        }
        cJSON_Delete(response_json);
    }
    return -1;
}

// 心跳线程函数
void *heartbeat_thread(void *arg) {
    int signal_strength = 80;
    double current_weight = 15.5;

    HeartBeatParams params;
    params.signal = signal_strength;
    params.curWeight = current_weight;

    while (1) {
        // 发送心跳包
        int ret = upload_heartbeat(&params);
        if (ret == 0) {
            printf("Heartbeat sent successfully.\n");
        } else {
            printf("Failed to send heartbeat.\n");
        }
        printf("\n");

        // 休眠
        // sleep(loginResponse.heartBeatInterval);
        sleep(30);
    }

    return NULL;
}

// 假设的获取当前重量的函数
double get_current_weight() {
    // 实际实现中，应该从硬件或其他数据源获取当前重量
    return 15.5; // 示例值
}

// 开门
int open_door(OpenDoorResponse *response) {
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 接收服务器开门请求
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
        perror("Receive failed");
        return -1;
    } else {
        recv_buf[recv_len] = '\0';
        printf("Server request: %s\n", recv_buf);

        // 解析 JSON 请求
        cJSON *request_json = cJSON_Parse(recv_buf);
        if (request_json == NULL) {
            printf("JSON parse error\n");
            return -1;
        }

        // Check if cmd is "openDoor" and extract necessary fields
        cJSON *cmd = cJSON_GetObjectItem(request_json, "cmd");
        cJSON *transId = cJSON_GetObjectItem(request_json, "transId");
        cJSON *openType = cJSON_GetObjectItem(request_json, "openType");

        if (cmd && transId && openType && strcmp(cmd->valuestring, "openDoor") == 0) {
            strncpy(openDoorRequest.transId, transId->valuestring, sizeof(openDoorRequest.transId) - 1);
            openDoorRequest.transId[sizeof(openDoorRequest.transId) - 1] = '\0';
            openDoorRequest.openType = openType->valueint;

            // Assuming sn and response are available, fill response details
            strncpy(response->transId, transId->valuestring, sizeof(response->transId) - 1);
            response->transId[sizeof(response->transId) - 1] = '\0';
            response->curWeight = get_current_weight();  // Example function to get current weight

            response->retCode = 0;

            // Create JSON response
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "cmd", "openDoor");
            cJSON_AddNumberToObject(root, "retCode", response->retCode);  // Assuming retCode is set elsewhere
            cJSON_AddStringToObject(root, "transId", response->transId);
            cJSON_AddNumberToObject(root, "curWeight", response->curWeight);
            char *json_str = cJSON_PrintUnformatted(root);

            // Send response
            printf("Sending openDoor response: %s\n", json_str);
            send(sock, json_str, strlen(json_str), 0);
            free(json_str);
            cJSON_Delete(root);

            cJSON_Delete(request_json);
            return 0;
        } else {
            printf("Invalid or unexpected request\n");
            cJSON_Delete(request_json);
            return -1;
        }
    }
}

int close_door(CloseDoorRequest *request) {
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    snprintf(request->transId, sizeof(request->transId), "1");  // 替换为实际生成的事务ID
    request->curWeight = 10.5;  // 替换为实际当前重量
    request->changeWeight = 0.5;  // 替换为实际改变的重量

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "closeDoor");
    cJSON_AddStringToObject(root, "transId", request->transId);
    cJSON_AddNumberToObject(root, "curWeight", request->curWeight);
    cJSON_AddNumberToObject(root, "changeWeight", request->changeWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送关门请求
    printf("Sending closeDoor request: %s\n", json_str);
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        
        if (retCode->valueint == 0 ){
            cJSON_Delete(response_json);
            return 0;
        }
        cJSON_Delete(response_json);
    }
    return -1;
}

// 上传超重
int upload_overweight(int curWeight) {
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overweight");
    cJSON_AddNumberToObject(root, "curWeight", curWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送超重上传请求
    printf("Sending overweight request: %s\n", json_str);
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        
        if (retCode->valueint == 0 ){
            cJSON_Delete(response_json);
            return 0;
        }
        cJSON_Delete(response_json);
    }
    return -1;
}

// 上传满载
int upload_overflow(int curCapacity) {
    char send_buf[MAX_BUF_SIZE], recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overflow");
    cJSON_AddNumberToObject(root, "curCapacity", curCapacity);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送满载上传请求
    printf("Sending overflow request: %s\n", json_str);
    send(sock, json_str, strlen(json_str), 0);
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    if ((recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0)) < 0) {
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
        
        if (retCode->valueint == 0 ){
            cJSON_Delete(response_json);
            return 0;
        }
        cJSON_Delete(response_json);
    }
    return -1;
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
    int ret = get_login_config(iccid, sn, key, &loginConfigResponse);
    if (ret == 0) {
        printf("Login configuration:\n");
        printf("IP: %s\n", loginConfigResponse.ip);
        printf("TCP Port: %d\n", loginConfigResponse.tcpPort);
        printf("\n");
    } else {
        printf("Failed to get login configuration.\n");
        return 1;
    }

    struct sockaddr_in server_addr;

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(loginConfigResponse.tcpPort);
    server_addr.sin_addr.s_addr = inet_addr(loginConfigResponse.ip);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 登录
    ret = login(imsi, imei, version, iccid, sn, &loginResponse);
    if (ret == 0) {
        printf("Login successful:\n");
        // printf("QR Code: %s\n", loginResponse.qrCode);
        printf("Recycle Price: %.2f\n", loginResponse.recyclePrice);
        printf("Heartbeat Interval: %d seconds\n", loginResponse.heartBeatInterval);
        printf("Turn on Light Time: %s\n", loginResponse.turnOnLight);
        printf("Turn off Light Time: %s\n", loginResponse.turnOffLight);
        printf("\n");
    } else {
        printf("Login failed.\n");
        close(sock);
        return -1;
    }

    // // 创建心跳线程
    // pthread_t heartbeat_thread_id;
    // if (pthread_create(&heartbeat_thread_id, NULL, heartbeat_thread, NULL) != 0) {
    //     perror("Failed to create heartbeat thread");
    //     return -1;
    // }

    // // 下发开门
    // if (open_door(&OpenDoorRequest) == 0) {
    //     printf("Open door process completed\n");
    //     printf("transId: %s\n", OpenDoorRequest.transId);
    //     printf("openType: %d\n", OpenDoorRequest.openType);
    // } else {
    //     printf("Open door process failed\n");
    // }
    
    // 上传关门
    ret = close_door(&closeDoorRequest); // 假设箱子编号为1
    if (ret == 0) {
        printf("Close door request sent successfully.\n");
        printf("\n");
    } else {
        printf("Failed to send close door request.\n");
        printf("\n");
    }

    // 上传超重
    ret = upload_overweight(25.3);
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

    // while (1);
    // // 上传照片
    // UploadResponse photoUploadResponse;
    // ret = upload_photo("http://serverIp:port/api/device/upload/photo", sn, iccid, "trans123", 1, photo_file, &photoUploadResponse);
    // if (ret == 0) {
    //     printf("Photo uploaded successfully. Server response: %s\n", photoUploadResponse.msg);
    // } else {
    //     printf("Failed to upload photo.\n");
    //     return 1;
    // }

    // // 上传视频
    // UploadResponse videoUploadResponse;
    // ret = upload_video("http://serverIp:port/api/device/upload/video", sn, iccid, "trans123", video_file, &videoUploadResponse);
    // if (ret == 0) {
    //     printf("Video uploaded successfully. Server response: %s\n", videoUploadResponse.msg);
    // } else {
    //     printf("Failed to upload video.\n");
    //     return 1;
    // }

    // pthread_join(heartbeat_thread_id, NULL);

    // close(sock);
    return 0;
}
