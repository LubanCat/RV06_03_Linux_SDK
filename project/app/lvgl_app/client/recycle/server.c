#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cjson/cJSON.h>
#include "hardware.h"

#define SERIAL_PORT "/dev/ttyS0"
#define BUFFER_SIZE 1024
int serial_fd;

int setup_serial_port(const char *port) {
    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
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
//将str和num组成json格式的字符串，并发送给客户端
int send_json_number(char *str,int num)        //发送json数据
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", str);
    cJSON_AddNumberToObject(root, "num", num);
    char *json_str = cJSON_Print(root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(root);
    free(json_str);
    return 0;
}


void handle_door_ctrl(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    cJSON *state_item = cJSON_GetObjectItem(root, "state");
    if (cJSON_IsNumber(state_item)) {
        int state = state_item->valueint;
        printf("Door state: %d\n", state);
        if(state){
            // set_post_door_open();   
            printf("opendoor\n");
        }else{
            // set_post_door_close(); 
            printf("closedoor\n");
        }
        
        send_json_number("DoorCtrl", 0);
    }
}

void handle_out_light_ctrl(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON *state_item = cJSON_GetObjectItem(root, "state");
    if (cJSON_IsNumber(state_item)) {
        int state = state_item->valueint;
        printf("Out light state: %d\n", state);
        if(state){
            // outside_light(1);
            printf("turnonlight\n");
        }else{
            // outside_light(0);
            printf("turnofflight\n");
        }
        
        send_json_number("OutLightCtrl", 0);
    }
}

void handle_in_light_ctrl(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON *state_item = cJSON_GetObjectItem(root, "state");
    if (cJSON_IsNumber(state_item)) {
        int state = state_item->valueint;
        printf("In light state: %d\n", state);
        if(state){
            // inside_light(1);
            printf("turnonlight\n");
        }else{
            // inside_light(0);
            printf("turnofflight\n");
        }
        
        send_json_number("InLightCtrl", 0);
    }
}

void handle_get_overflow(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // int overflow = get_overflow_status();
    int overflow =1;
    printf("Overflow: %d\n", overflow);
    send_json_number("get_overflow", overflow);
}
void handle_get_weight(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    double weight =0.99;
    // double weight = get_current_weight();
    printf("Weight: %f\n", weight);
    // send_json_number("get_weight", weight);
}
void process_json(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    printf("Received JSON: %s\n", json_str);

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (cJSON_IsString(cmd_item)) {
        char str[20];
        strncpy(str, cmd_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "DoorCtrl") == 0) {
            handle_door_ctrl(json_str);
        }else if (strcmp(str, "OutLightCtrl") == 0) {
            handle_out_light_ctrl(json_str);
        }else if (strcmp(str, "InLightCtrl") == 0) {
            handle_in_light_ctrl(json_str);
        }else if (strcmp(str, "get_overflow") == 0) {
            handle_get_overflow(json_str);
        }else if (strcmp(str, "get_weight") == 0) {
            handle_get_weight(json_str);
        }else{
            printf("Error: unknown command: %s\n", str);
        }
    }else { 
        printf("Error: cmd is not a string\n");/*打印解析cmd失败*/
    }
    cJSON_Delete(root);
}

int server_start() {
    serial_fd = setup_serial_port(SERIAL_PORT);
    if (serial_fd < 0) {
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char json_buffer[BUFFER_SIZE];
    int json_len = 0;
    int brace_count = 0;

    while (1) {
        ssize_t bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';

            for (int i = 0; i < bytes_read; i++) {
                json_buffer[json_len++] = buffer[i];

                if (buffer[i] == '{') {
                    brace_count++;
                } else if (buffer[i] == '}') {
                    brace_count--;
                }

                if (brace_count == 0 && json_len > 0) {
                    json_buffer[json_len] = '\0';
                    process_json(json_buffer);
                    json_len = 0;
                }
            }
        } else if (bytes_read == 0) {
            printf("Connection closed\n");
            break;
        } else {
            perror("read");
            break;
        }
    }

    close(serial_fd);
    return 0;
}