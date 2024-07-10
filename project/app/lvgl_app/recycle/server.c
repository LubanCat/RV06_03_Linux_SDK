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
#include "../custom/custom.h"
#include "capture.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_BUF_SIZE 512
#define COUNT_DOWN99 99
#define COUNT_DOWN10 10
#define REFRESH_RATE 10

// 定义服务器地址和端口
// #define CONFIG_SERVER_IP "192.168.103.142"
// #define CONFIG_SERVER_PORT 12345

#define CONFIG_SERVER_IP "47.101.195.22"
#define CONFIG_SERVER_PORT 9096
extern lv_disp_drv_t disp_drv;
char *preview_data;
char in_video[12];
char out_video[12];

void* capture_thread(void* arg) {
    sleep(1);
    video_frame_t *video_frame = (video_frame_t *)arg;
    video_frame->width=320;
    video_frame->height=240;
    sprintf(video_frame->device,"%s",out_video);
    int ret = capture_start(video_frame);
    if(ret != -1){
        lv_color_t *color_p;
        int size=320*240;
        color_p=malloc(size*sizeof(lv_color_t));
        int length;

        lv_area_t area;
        area.x1=618;
        area.x2=857;
        area.y1=129;
        area.y2=448;
        
        while(1){
            video_linux_get_frame_rgb(video_frame,&preview_data,&length);
            memset(color_p,0x00,size*sizeof(lv_color_t));

            for(int j=0;j<240;j++){
                for(int i=0;i<320;i++){
                    color_p[j+(319-i)*240].full |= 0xff<<24;
                    color_p[j+(319-i)*240].full |= preview_data[(j*320+i)*3+0]<<16;
                    color_p[j+(319-i)*240].full |= preview_data[(j*320+i)*3+1]<<8;
                    color_p[j+(319-i)*240].full |= preview_data[(j*320+i)*3+2];
                }
            }
            
            drm_flush(&disp_drv,&area,color_p);
            free(preview_data);
        }

    }
}

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
    int open_type;
} message_queue_t;

// 设备信息
char sn[32];
char key[32];
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

int send_tcp_packet(char *buf,int size){
    pthread_mutex_lock(&mutex);
    send(sock, buf, size, 0);
    pthread_mutex_unlock(&mutex);
    return 1;
}

int read_info() {
    // 打开文件
    FILE *file = fopen("/root/device_info.json", "r");
    if (file == NULL) {
        perror("无法打开设备配置文件");
        return -1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存来存储文件内容
    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        printf("无法分配内存\n");
        fclose(file);
        return -1;
    }

    // 读取文件内容
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0'; // 确保文件内容以空字符结尾

    // 关闭文件
    fclose(file);

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse(file_content);
    if (root == NULL) {
        printf("无法解析 JSON 数据\n");
        free(file_content);
        return -1;
    }

    // 获取 sn 和 key 字符串
    cJSON *sn_item = cJSON_GetObjectItem(root, "sn");
    cJSON *key_item = cJSON_GetObjectItem(root, "key");

    if (cJSON_IsString(sn_item) && cJSON_IsString(key_item)) {
        strncpy(sn, sn_item->valuestring, 32 - 1); // 复制 sn 字符串到 sn 数组
        sn[32 - 1] = '\0'; // 确保字符串以空字符结尾
        strncpy(key, key_item->valuestring, 32 - 1); // 复制 key 字符串到 key 数组
        key[32 - 1] = '\0'; // 确保字符串以空字符结尾

        printf("SN: %s\n", sn);
        printf("Key: %s\n", key);
    } else {
        printf("无法获取 sn 或 key\n");
        // 释放内存
        cJSON_Delete(root);
        free(file_content);

        return -1;
    }

    // 释放内存
    cJSON_Delete(root);
    free(file_content);

    return 0;
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

    if (recv_len <= 0) {
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
        printf("sock = %d ,recv_len=%d\n",sock,recv_len);
        if (recv_len <= 0) {
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

// 心跳线程函数
void *heartbeat_thread(void *arg) {

    while (1) {
        // heartBeat.signal = module_signal();
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
        sleep(loginRes.heartBeatInterval);
    }

    return NULL;
}

// 开门
int open_door() {
    close_door_flag=0;
    post_flag=0;
    quit_flag=0;
    open_clean_door_flag=0;
    hand_close_door_flag=0;
    message_queue_t msg;

    if (msgrcv(msgid, &msg, sizeof(msg), TYPE_OPENDOOR, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    
    // 关闭登录页面元素更新 
    screen_login_flag=0;

    double sub_weight =  openDoor.curWeight; //开门命令上传给服务器的重量

    printf("开门: openType=%d\n", openDoor.openType);

    if (openDoor.openType == 1) {
        // 满溢时进入满溢界面，不开投递门
        if (get_overflow_status() == 3) {
            printf("当前回收柜已满，请清运后再投递\n");
            set_screen_full(&guider_ui.screen_login_del,loginRes.recyclePrice,COUNT_DOWN10);
            for (int i = COUNT_DOWN10; i > 0; i--) {
                printf("倒计时: %d秒 后返回登录界面\n", i);
                set_screen_full_time(i);
                sleep(1);
            }
            // 返回到登录页面
            set_screen_login(&guider_ui.screen_full_del, loginRes.recyclePrice, sub_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal);
        } else {
            int i = COUNT_DOWN99 * REFRESH_RATE;
            double cur_weight = 0, diff_weight, money;
            pthread_t capture_thread_id;

            //判断/dev/video0是什么类型
            int ch = get_video_in_out(0);
            if(ch == 2){
                strcpy(out_video,"/dev/video2");
                strcpy(in_video,"/dev/video0");
            }else if(ch == 1){
                strcpy(out_video,"/dev/video0");
                strcpy(in_video,"/dev/video2");
            }
            else{
                printf("skip\n");
            }
            //获取当前时间
            char currentTime[20]; // 足够存储 "YYYYMMDD_HHMMSS" 和结束符
            time_t now;
            struct tm *localNow;
            time(&now);
            localNow = localtime(&now);
            strftime(currentTime, sizeof(currentTime), "%Y%m%d_%H%M%S", localNow);
            
            struct stat statbuf;
            //创建文件夹
            char *media_path="/root/media";
            if (stat(media_path, &statbuf) == -1) {
                // 文件夹不存在，尝试创建
                if (mkdir(media_path, 0755) == -1) {
                    perror("mkdir error");
                }
                printf("文件夹 '%s' 已创建\n", media_path);
            } else if (S_ISDIR(statbuf.st_mode))
                printf("文件夹 '%s' 已存在\n", media_path);
            else 
                printf("路径 '%s' 已存在，但不是文件夹\n", media_path);

            char time_path[32];
            sprintf(time_path,"%s/%s",media_path,currentTime);

            int result = mkdir(time_path, 0755);
            if (result == 0) {
                printf("Folder created successfully.\n");
            } else {
                perror("mkdir failed"); // 打印错误信息
            }
            //柜内拍照
            char in_video_open_path[64];
            sprintf(in_video_open_path,"%s/in_video_open.jpg",time_path);
            capture_jpg(in_video,1920,1080,in_video_open_path);

            //柜外拍照
            char out_video_open_path[64];
            sprintf(out_video_open_path,"%s/out_video_open.jpg",time_path);
            capture_jpg(out_video,1920,1080,out_video_open_path);
            // 投递界面
            set_screen_submit(&guider_ui.screen_login_del,0,COUNT_DOWN99,0);
            video_frame_t *video_frame;
            video_frame = malloc(sizeof(video_frame_t));
            int ret = pthread_create(&capture_thread_id, NULL, capture_thread, (void *)video_frame);
            if (ret != 0) {
                perror("pthread_create");
            }

            inside_light(1);
            set_post_door_open();
            printf("可回收物仓门已打开，请尽快投递\n");
            for (; i > 0; i--) {
                // 获取重量，循环更新页面
                if ((i % REFRESH_RATE) == 0) {
                    cur_weight= get_current_weight();
                    diff_weight = round((cur_weight - sub_weight) * 100) / 100; //手动保留两位小数
                    money = round((diff_weight * loginRes.recyclePrice) * 100) / 100; //手动保留两位小数
                    // printf("倒计时: 总重量%fKG，当前重量%fKG，计费重量%fKG，计费%f元\n", sub_weight, cur_weight, diff_weight, money);
                    printf("倒计时: %d秒 后关闭可回收物仓门\n", i/REFRESH_RATE);
                    set_screen_submit_time(i/REFRESH_RATE);
                    set_screen_submit_money(money);
                    set_screen_submit_weigth(diff_weight);
                }

                // 获取关闭仓门按键状态
                if (close_door_flag) {
                    printf("按键：关闭仓门");
                    close_door_flag=0;
                    break;
                }

                if (i <= COUNT_DOWN10 * REFRESH_RATE) {
                    // 投递重量、金额，10s倒计时
                    lv_obj_add_flag(guider_ui.screen_submit_close_door, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(guider_ui.screen_submit_post, LV_OBJ_FLAG_HIDDEN);

                    // 获取继续投递按键状态，按下倒计时重新从99开始
                    if (post_flag) {
                        printf("按键：继续投递\n");
                        i = COUNT_DOWN99 * REFRESH_RATE;
                        post_flag=0;
                    }
                }else{
                    lv_obj_add_flag(guider_ui.screen_submit_post, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(guider_ui.screen_submit_close_door, LV_OBJ_FLAG_HIDDEN);
                }
                // usleep(1000000 / REFRESH_RATE); 
                usleep(600000 / REFRESH_RATE); 
            }
            set_post_door_close();
            close_door(cur_weight, diff_weight);
            inside_light(0);
            pthread_cancel(capture_thread_id);
            capture_stop(video_frame);
            free(video_frame);
            if(preview_data != NULL)
                free(preview_data);

            //柜内拍照
            char in_video_close_path[64];
            sprintf(in_video_close_path,"%s/in_video_close.jpg",time_path);
            capture_jpg(in_video,1920,1080,in_video_close_path);

            //柜外拍照
            char out_video_close_path[64];
            sprintf(out_video_close_path,"%s/out_video_close.jpg",time_path);
            capture_jpg(out_video,1920,1080,out_video_close_path);

            set_screen_login(&guider_ui.screen_submit_del, loginRes.recyclePrice, cur_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal);
            openDoor.openType = 0;
        }
    } else if (openDoor.openType == 2) {
        int i = COUNT_DOWN99 * REFRESH_RATE;
        int clearing = 0;
        double cur_weight = 0;
        double diff_weight = 0;
        // 清运操作页面1
        set_screen_clean(&guider_ui.screen_login_del,COUNT_DOWN99,sub_weight);

        for (; i > 0; i--) {
            if ((i % REFRESH_RATE) == 0) {
                // 更新清运界面倒计时
                set_screen_clean_time(i/REFRESH_RATE);
                printf("倒计时: %d秒后返回登录界面，可清运重量%.2fKG\n", i/REFRESH_RATE, sub_weight);
            }
            // 获取开启清运门按键状态，按下开启清运门
            if (open_clean_door_flag) {
                inside_light(1);
                printf("按键：开启清运门\n");
                set_clear_door_open();
                open_clean_door_flag=0;
                clearing = 1;
                break;
            }

            // 获取退出按键状态，按下返回登录界面
            if (quit_flag) {
                printf("按键：退出该模式\n");
                set_screen_login(&guider_ui.screen_clean_del, loginRes.recyclePrice, sub_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal);
                quit_flag=0;
                break;
            }
            usleep(1000000 / REFRESH_RATE); 
        }

        // 判断是否进入清运状态，显示正在清运
        if (clearing == 1) {
            i = 0;
            // 清运操作页面2
            set_screen_cleaning(&guider_ui.screen_login_del,sub_weight);
            while (1) {
                if ((i % REFRESH_RATE) == 0) {                
                    cur_weight = get_current_weight();
                    diff_weight = sub_weight - cur_weight;
                    // diff_weight = round((sub_weight - cur_weight) * 100) / 100; //手动保留两位小数
                    // 清运操作页面2 更新当前重量
                    set_screen_cleaning_weigth(cur_weight);
                    printf("清运中: 当前重量%.2fKG\n", cur_weight);
                }

                // 检测清理门关闭状态
                if (get_clear_door_status() == 0) {
                    printf("电磁锁检测：手动关闭清理门\n");
                    close_door(cur_weight, diff_weight); // 发送关门信息
                    inside_light(0);
                    hand_close_door_flag=0;
                    clearing = 2;
                    break;
                }
                usleep(1000000 / REFRESH_RATE); 
                i++;
            }
        }

        // 判断进入清运完成状态 
        if (clearing == 2) {
            // 更新清运完成页面 
            set_screen_cleaned(&guider_ui.screen_login_del,COUNT_DOWN10,sub_weight, cur_weight, diff_weight);
            printf("清运前重量%.2fKG,清运后重量%.2fKG,清运重量%.2fKG\n", sub_weight, cur_weight, diff_weight);
            for (i = COUNT_DOWN10; i > 0; i--) {
                printf("清运完成: %d秒后返回登录界面\n", i);
                set_screen_cleaned_time(i);
                sleep(1);
            }
        }
        set_screen_login(&guider_ui.screen_cleaned_del, loginRes.recyclePrice, cur_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal);
        // 关门,发关门消息
        openDoor.openType = 0;
    }
    return 0;
}

int close_door(double cur_weight, double diff_weight) {
    char recv_buf[MAX_BUF_SIZE];

    // 创建 JSON 请求
    closeDoor.curWeight = cur_weight;
    closeDoor.changeWeight = diff_weight;

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

    return 0;
}

// 上传超重
int upload_overweight(double curWeight) {
    char recv_buf[MAX_BUF_SIZE];

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
    return msg.retcode;
}

// 上传满载
int upload_overflow(int curCapacity) {
    char recv_buf[MAX_BUF_SIZE];

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

int server_start() {
    int ret;
    // 默认参数
    strcpy(sn, "00016000000001");
    strcpy(key, "device_key");

    ret = read_info();
    if (ret != 0){
        // {"sn":"00016000000002","key":"device_key"}
        printf("无法获取 sn 或 key，设置默认参数\n");
        // LVGL提示 未设置SN和Key
        // 默认参数
        strcpy(sn, "00016000000001");
        strcpy(key, "device_key");
    }

    // 获取登录配置
    while (1) {
        // 获取登录配置
        ret = get_login_config(&loginConfig);
        if (ret == 0) {
            printf("Login configuration:\n");
            printf("IP: %s\n", loginConfig.ip);
            printf("TCP Port: %d\n", loginConfig.tcpPort);
            printf("\n");
            break; // 成功获取配置后退出循环
        } else {
            printf("Failed to get login configuration. Retrying in 60 seconds...\n");
            sleep(60); // 获取失败时，等待60秒后重试
        }
    }

    struct sockaddr_in server_addr;

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

    // 登录服务器
    while (1) {
        ret = server_login();
        if (ret == 0) {
            printf("Login successful:\n");
            printf("QR Code: %s\n", loginRes.qrCode);
            printf("Recycle Price: %.2f\n", loginRes.recyclePrice);
            printf("Heartbeat Interval: %d seconds\n", loginRes.heartBeatInterval);
            printf("\n");
            break;
        } else {
            printf("Failed to get login configuration. Retrying in 60 seconds...\n");
            sleep(60); // 获取失败时，等待60秒后重试
        }
    }

    // 创建心跳线程
    pthread_t heartbeat_thread_id;
    if (pthread_create(&heartbeat_thread_id, NULL, heartbeat_thread, NULL) != 0) {
        perror("Failed to create heartbeat thread");
        return -1;
    }

    // 创建接收线程
    pthread_t tcp_receiver_thread_id;
    if (pthread_create(&tcp_receiver_thread_id, NULL, tcp_receiver, NULL) != 0) {
        perror("Failed to create heartbeat thread");
        return -1;
    }

    return 0;
}

int test() {
    int ret;
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

    return 0;
}
