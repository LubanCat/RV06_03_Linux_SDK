#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cjson/cJSON.h>
#include "hardware.h"
#include <sys/stat.h>

#define SERIAL_PORT "/dev/ttyS0"
#define BUFFER_SIZE 1024
int serial_fd;

#define CLIENT_VERSION "1.0"

int calculate_md5(const char *filename, char *md5_str) {
    printf("Calculating MD5 for %s\n", filename);
    char command[128];
    sprintf(command, "md5sum %s | awk '{ print $1 }'", filename);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error opening pipe for MD5 calculation.\n");
        return 1;
    }
    fgets(md5_str, 64, fp);
    pclose(fp);
    // Remove the trailing newline character
    md5_str[strcspn(md5_str, "\n")] = 0;
    printf("Calculated MD5: %s\n", md5_str);
    return 0;
}


void delete_file(const char *filename) {
    if (remove(filename) != 0) {
        perror("Error deleting file");
    } else {
        printf("File %s successfully deleted\n", filename);
    }
}


void set_file_executable(const char *filename) {
    if (chmod(filename, S_IRWXU) != 0) {
        perror("Error setting file executable");
    } else {
        printf("File %s is now executable\n", filename);
    }
}

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
    cJSON_AddNumberToObject(root, "reCode", num);
    char *json_str = cJSON_Print(root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(root);
    free(json_str);
    return 0;
}


void handle_post_door_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int rod_value;
    int ret_code=0;

    cJSON *rod_value_item = cJSON_GetObjectItem(root, "rod_value");
    if (cJSON_IsNumber(rod_value_item)) {
        rod_value = rod_value_item->valueint;
    }else {
        printf("Error: rod_value is not a number\n");
    }

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "open") == 0) {
            set_post_door_open(rod_value);
        } else if (strcmp(str, "close") == 0) {
            set_post_door_close(rod_value);
        } else {
            printf("Error: unknown state: %s\n", str);
            ret_code=1;
        }
    }
    send_json_number("InLightCtrl", ret_code);
    cJSON_Delete(root);
}

void handle_out_light_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int ret_code=0;

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "open") == 0) {
            outside_light(1);
        } else if (strcmp(str, "close") == 0) {
            outside_light(0);
        } else {
            printf("Error: unknown state: %s\n", str);
            ret_code=1;
            return;
        }
    }
    send_json_number("InLightCtrl", ret_code);
    cJSON_Delete(root);
}

void handle_in_light_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int ret_code=0;
    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "open") == 0) {
            inside_light(1);
        } else if (strcmp(str, "close") == 0) {
            inside_light(0);
        } else {
            printf("Error: unknown state: %s\n", str);
            ret_code=1;
        }
    }
    send_json_number("InLightCtrl", ret_code);
    cJSON_Delete(root);
}

void handle_ir_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int state=0;
    int ret_code=0;

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "get_state") == 0) {
            state=get_overflow_status();
        }else{
            printf("Error: unknown state: %s\n", str);
            ret_code=1;
        }
    }

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "IRCtrl");
    cJSON_AddNumberToObject(send_root, "state", state);
    cJSON_AddNumberToObject(send_root, "reCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    cJSON_Delete(root);
    return ;
}

void handle_weight_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int adc0=0;
    int adc1=0;
    int ret_code=0;

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "get_raw_data") == 0) {
            adc0=read_weight_adc(0);
            adc1=read_weight_adc(1);
        }else{
            printf("Error: unknown ctrl: %s\n", str);
            ret_code=1;
        }
    }

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "WeightCtrl");
    cJSON_AddNumberToObject(send_root, "adc0", adc0);
    cJSON_AddNumberToObject(send_root, "adc1", adc1);
    cJSON_AddNumberToObject(send_root, "reCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return ;
}

void handle_clean_door_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int ret_code=0;
    int state=0;

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "CleanDoorCtrl");

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "open") == 0) {
            set_clear_door_open();
        }else if (strcmp(str, "get_state") == 0) {
            state=get_clear_door_status();
            cJSON_AddNumberToObject(send_root, "state", ret_code);
        }else{
            printf("Error: unknown ctrl: %s\n", str);
            ret_code=1;
        }
    }

    cJSON_AddNumberToObject(send_root, "reCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return;
}

void handle_update_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    int ret_code=0;
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "UpdateCtrl");

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "get_version") == 0) {
            cJSON_AddStringToObject(send_root, "version", CLIENT_VERSION);
        }else if (strcmp(str, "update") == 0) {
            printf("update\n");
        }else {
            printf("Error: unknown state: %s\n", str);
        }
    }
    cJSON_AddNumberToObject(send_root, "reCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return;
}

void handle_speaker_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    int ret_code=0;
    cJSON *ctrl_item = cJSON_GetObjectItem(root, "volume");
    if (cJSON_IsNumber(ctrl_item)) {
        printf("MUSIC SET %d\n",ctrl_item->valueint);
    }else{
        printf("Error: ctrl is not a number\n");
        ret_code=1;
    }
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "SpeakerCtrl");
    cJSON_AddNumberToObject(send_root, "reCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return;
}

void handle_file_transfer(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    } 
}

void handle_machine_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
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
        if (strcmp(str, "PostDoorCtrl") == 0) {
            handle_post_door_ctrl(json_str);
        }else if (strcmp(str, "OutLightCtrl") == 0) {
            handle_out_light_ctrl(json_str);
        }else if (strcmp(str, "InLightCtrl") == 0) {
            handle_in_light_ctrl(json_str);
        }else if (strcmp(str, "IRCtrl") == 0) {
            handle_ir_ctrl(json_str);
        }else if (strcmp(str, "WeightCtrl") == 0) {
            handle_weight_ctrl(json_str);
        }else if (strcmp(str, "CleanDoorCtrl") == 0) {
            handle_clean_door_ctrl(json_str);
        }else if (strcmp(str, "UpdateCtrl") == 0) {
            handle_update_ctrl(json_str);
        }else if (strcmp(str, "SpeakerCtrl") == 0) {
            handle_speaker_ctrl(json_str);
        }else if (strcmp(str, "FileTransfer") == 0) {
            handle_file_transfer(json_str);
        }else if (strcmp(str, "MachineCtrl") == 0) {
            handle_machine_ctrl(json_str);
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


int test_json() {
//读取json文件
    FILE *fp = fopen("test.json", "r");
    if (fp == NULL) {
        printf("open file failed\n");
        return 1;
    }
    char json_str[1024];
    fread(json_str, 1, 1024, fp);
    fclose(fp);
    printf("json_str:%s\n", json_str);
    process_json(json_str);
    return 0;
}

