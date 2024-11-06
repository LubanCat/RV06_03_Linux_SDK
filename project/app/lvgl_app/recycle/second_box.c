#include "second_box.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cjson/cJSON.h>

#define SERIAL_PORT "/dev/ttyS5"
#define BUFFER_SIZE 256
int serial_fd;

int send_message(char *str ,int len) {
    write(serial_fd, str, len);
}
void receive_message(char *json_buffer) {
    char buffer[BUFFER_SIZE];
    int json_len = 0;
    int brace_count = 0;
    int an=0;
    int count=0;
    while(an==0){
        ssize_t bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);
        // printf("bytes_read = %d\n",bytes_read);
        if (bytes_read > 0) {
            for (int i = 0; i < bytes_read; i++) {
                if (buffer[i] == '\n'){
                    count++;
                    if(count>1){
                        count=0;
                        continue;
                    }
                }else
                    count=0;
                    
                json_buffer[json_len++] = buffer[i];

                if (buffer[i] == '{') {
                    brace_count++;
                } else if (buffer[i] == '}') {
                    brace_count--;
                }

                if (brace_count == 0 && json_len > 0) {
                    json_buffer[json_len] = '\0';
                    json_len = 0;
                    an=1;
                    break;
                }
            }
            // 
        } else 
            printf("read revice %ld\n", bytes_read);
    }
    // printf("received signal: %s\n", json_buffer);
}

int setup_serial_port(const char *port) {
    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 5;

    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    return fd;
}

int process_json(const char *json_str) {
    int ret;
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    printf("Received JSON: %s\n", json_str);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "PostDoorCtrl") == 0 || 
            strcmp(str, "CleanDoorCtrl") == 0 ||
            strcmp(str, "InLightCtrl") == 0 ||
            strcmp(str, "OutLightCtrl") == 0 ||
            strcmp(str, "SpeakerCtrl") == 0 
            ) {
            cJSON *ctrl_item = cJSON_GetObjectItem(root, "retCode");
            if (cJSON_IsNumber(ctrl_item)) {
                printf("control result: %d\n", ctrl_item->valueint);
                ret=ctrl_item->valueint;
            }else{
                printf("control result: unknown\n");
                ret= -1;
            }
            
        }else{
            printf("Unknown command: %s\n", str);
            ret= -1;
        }
    }
    cJSON_Delete(root);
    return ret;
}
int set_second_box_post_door(int state ,int rod_value){
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "PostDoorCtrl");
    if(state)
        cJSON_AddStringToObject(send_root, "ctrl", "open");
    else
        cJSON_AddStringToObject(send_root, "ctrl", "close");

    cJSON_AddNumberToObject(send_root, "rod_value", rod_value);

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);


    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    int ret = process_json(json_buffer);
    return ret;
}

int set_second_box_out_light(int state){

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "OutLightCtrl");
    if(state)
        cJSON_AddStringToObject(send_root, "ctrl", "open");
    else
        cJSON_AddStringToObject(send_root, "ctrl", "close");
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    return process_json(json_buffer);
}

int set_second_box_in_light(int state){
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "inLightCtrl");
    if(state)
        cJSON_AddStringToObject(send_root, "ctrl", "open");
    else
        cJSON_AddStringToObject(send_root, "ctrl", "close");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    return process_json(json_buffer);
}

int set_second_box_clean_door(){
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "CleanDoorCtrl");
    cJSON_AddStringToObject(send_root, "ctrl", "open");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    return process_json(json_buffer);
}

int set_second_box_spk_volume(int volume){
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "SpeakerCtrl");

    cJSON_AddNumberToObject(send_root, "volume", volume);

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    return process_json(json_buffer);
}

int get_second_box_ir_status(int *state){
    int ret;
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "CleanDoorCtrl");
    cJSON_AddStringToObject(send_root, "ctrl", "get_state");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);


    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    printf("Received JSON: %s\n", json_buffer);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "IRCtrl") == 0) {
            cJSON *ctrl_item = cJSON_GetObjectItem(root, "retCode");
            if (cJSON_IsNumber(ctrl_item)) {
                cJSON *state_item = cJSON_GetObjectItem(root, "state");
                if (cJSON_IsNumber(state_item)) {
                    state[0]=state_item->valueint;
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }
                ret=ctrl_item->valueint;
            }else{
                printf("control result: unknown\n");
                ret= -1;
            }
            
        }else{
            printf("Unknown command: %s\n", str);
            ret= -1;
        }
    }
    cJSON_Delete(root);
    return ret;
}

int get_second_box_raw_weight(int *adc){
    int ret;

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "WeightCtrl");
    cJSON_AddStringToObject(send_root, "ctrl", "get_raw_data");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
    cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    printf("Received JSON: %s\n", json_buffer);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "WeightCtrl") == 0) {
            cJSON *ctrl_item = cJSON_GetObjectItem(root, "retCode");
            if (cJSON_IsNumber(ctrl_item)) {
                cJSON *adc0_item = cJSON_GetObjectItem(root, "adc0");
                if (cJSON_IsNumber(adc0_item)) {
                    adc[0]=adc0_item->valueint;
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }

                cJSON *adc1_item = cJSON_GetObjectItem(root, "adc1");
                if (cJSON_IsNumber(adc1_item)) {
                    adc[1]=adc1_item->valueint;
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }
    
            }else{
                printf("control result: unknown\n");
                ret= -1;
            }
            ret=ctrl_item->valueint;
            
        }else{
            printf("Unknown command: %s\n", str);
            ret= -1;
        }
    }
    cJSON_Delete(root);
    return ret;
}

int get_second_box_cleandoor_status(int *state){
    int ret;

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "GetCleanDoor");
    cJSON_AddStringToObject(send_root, "ctrl", "get_state");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
        cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    printf("Received JSON: %s\n", json_buffer);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "GetCleanDoor") == 0) {
            cJSON *ctrl_item = cJSON_GetObjectItem(root, "retCode");
            if (cJSON_IsNumber(ctrl_item)) {
                cJSON *state_item = cJSON_GetObjectItem(root, "state");
                if (cJSON_IsNumber(state_item)) {
                    state[0]=state_item->valueint;
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }
    
            }else{
                printf("control result: unknown\n");
                ret= -1;
            }
            ret=ctrl_item->valueint;
            
        }else{
            printf("Unknown command: %s\n", str);
            ret= -1;
        }
    }
    cJSON_Delete(root);
    return ret;
}

int get_second_box_version(char *version){
    int ret;

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "UpdateCtrl");
    cJSON_AddStringToObject(send_root, "ctrl", "get_state");

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    char json_buffer[BUFFER_SIZE];
    receive_message(json_buffer);
        cJSON *root = cJSON_Parse(json_buffer);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    printf("Received JSON: %s\n", json_buffer);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "UpdateCtrl") == 0) {
            cJSON *ctrl_item = cJSON_GetObjectItem(root, "retCode");
            if (cJSON_IsNumber(ctrl_item)) {
                cJSON *state_item = cJSON_GetObjectItem(root, "get_version");
                if (cJSON_IsString(state_item)) {
                    strcpy(version,state_item->valuestring);
                    ret=ctrl_item->valueint;
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }
            }else{
                printf("control result: unknown\n");
                ret= -1;
            }
            
        }else{
            printf("Unknown command: %s\n", str);
            ret= -1;
        }
    }
    cJSON_Delete(root);
    return ret;
}


int second_box_file_transfer(char *filename, int filesize,char *filepath, char* md5){
    
    int ret;

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "FileCtrl");
    cJSON_AddStringToObject(send_root, "ctrl", "upload");
    cJSON_AddStringToObject(send_root, "filename", filename);
    cJSON_AddNumberToObject(send_root, "filesize", filesize);
    cJSON_AddStringToObject(send_root, "filepath", filepath);
    cJSON_AddStringToObject(send_root, "md5", md5);

    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    send_message(json_str,len);
    cJSON_Delete(send_root);
    free(json_str);
    // 等待，收到信号后执行下一步
    int an=0;

    while(an==0){
        char json_buffer[BUFFER_SIZE];
        receive_message(json_buffer);
        cJSON *root = cJSON_Parse(json_buffer);
        if (root == NULL) {
            fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
            return -1;
        }

        printf("Received JSON: %s\n", json_buffer);

        cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
        if (cJSON_IsString(cmd_item)) {
            char str[20];
            strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
            str[sizeof(str) - 1] = '\0';
            if (strcmp(str, "FileCtrl") == 0) {
                cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
                if (cJSON_IsString(ctrl_item)) {
                    if(strcmp(ctrl_item->valuestring, "ready")==0){
                        printf("ready to send file\n");
                    }else if(strcmp(ctrl_item->valuestring, "resend")==0){
                        printf("resend file\n");
                    }else if(strcmp(ctrl_item->valuestring, "success")==0){
                        printf("file upload success\n");
                        an=1;
                        ret=1;
                    }
                }else{
                    printf("control result: unknown\n");
                    ret= -1;
                }
                
            }else{
                printf("Unknown command: %s\n", str);
                ret= -1;
            }
        }
        cJSON_Delete(root);
    }

    return ret;
}


// int main()  
// {
//     serial_fd = setup_serial_port(SERIAL_PORT);
//     if (serial_fd < 0) {
//         return -1;
//     }
//     int state=0;
//     char version[20];
//     int adc[2];

    
//     // 设置第二个盒子的后门状态打开
//     printf("set post door state open\n");
//     set_second_box_post_door(1,200);
//     // 设置第二个盒子的后门状态关闭
//     printf("set post door state close\n");
//     set_second_box_post_door(0,200);

//     // 设置第二个盒子的外部灯光状态，参数1表示激活状态
//     printf("set out light state on\n");
//     set_second_box_out_light(1);
//     // 设置第二个盒子的外部灯光状态，参数1表示非激活状态
//     printf("set out light state off\n");
//     set_second_box_out_light(0);

//     // 设置第二个盒子的内部灯光状态，参数1表示激活状态
//     printf("set in light state on\n");
//     set_second_box_in_light(1);
//     // 设置第二个盒子的内部灯光状态，参数1表示非激活状态
//     printf("set in light state off\n");
//     set_second_box_in_light(0);

//     // 执行清洁第二个盒子的操作，打开
//     printf("clean second box\n");
//     set_second_box_clean_door();

//     // 设置第二个盒子的扬声器音量，参数1表示音量级别
//     printf("set speaker volume 20\n");
//     set_second_box_spk_volume(20);
//     // 设置第二个盒子的扬声器音量，参数1表示音量级别
//     printf("set speaker volume 50\n");
//     set_second_box_spk_volume(50);;


//     // 获取第二个盒子的IR状态
//     get_second_box_ir_status(&state);
//     printf("IR state: %d\n", state);

//     // 获取第二个盒子的ADC数据
//     get_second_box_raw_weight(adc);
//     printf("ADC0: %d, ADC1: %d\n", adc[0], adc[1]);
//     // 获取第二个盒子的清洁门状态
//     get_second_box_cleandoor_status(&state);
//     printf("Clean door state: %d\n", state);
//     // 获取第二个盒子的版本号
//     get_second_box_version(version);
//     printf("Version: %s\n", version);
//     char filename[20] = "test.txt";
//     char filepath[20] = "/root/test.txt";
//     char md5[33] = "d41d8cd98f00b204e9800998ecf8427e";
//     int filesize = 1023;
//     second_box_file_transfer(filename, filesize, filepath, md5);

//     close(serial_fd);
//     return 0;
// }




