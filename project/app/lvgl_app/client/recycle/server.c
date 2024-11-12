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
#include <pthread.h>

#define SERIAL_PORT "/dev/ttyS3"
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
//将str和num组成json格式的字符串，并发送给客户端
int send_json_number(char *str,int num)        //发送json数据
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", str);
    cJSON_AddNumberToObject(root, "retCode", num);
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
    send_json_number("PostDoorCtrl", ret_code);
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
    send_json_number("OutLightCtrl", ret_code);
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
    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
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
    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
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
        }else{
            printf("Error: unknown ctrl: %s\n", str);
            ret_code=1;
        }
    }

    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return;
}


void handle_get_clean_door_status(const char *main_json_str) {
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
            state=get_clear_door_status();
        }else{
            printf("Error: unknown state: %s\n", str);
            ret_code=1;
        }
    }

    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "GetCleanDoor");
    cJSON_AddNumberToObject(send_root, "state", state);
    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    cJSON_Delete(root);
    return ;
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
        }else {
            printf("Error: unknown state: %s\n", str);
        }
    }
    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
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
        char str[64];
        sprintf(str,"/oem/usr/bin/rk_mpi_amix_test --control=23 --value=%d",ctrl_item->valueint*6);
        system(str);
        system("/usr/bin/aplay -Dhw:0 /oem/usr/share/closedoor.wav");
    }
    cJSON *send_root = cJSON_CreateObject();
    cJSON_AddStringToObject(send_root, "cmd", "SpeakerCtrl");
    cJSON_AddNumberToObject(send_root, "retCode", ret_code);
    char *json_str = cJSON_Print(send_root);
    int len = strlen(json_str);
    write(serial_fd, json_str, len);
    cJSON_Delete(send_root);
    free(json_str);
    return;
}

void handle_file_transfer(const char *main_json_str) {
    char filename[64];
    char filepath[64];
    int filesize;
    char md5[64];

    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "upload") == 0) {
            cJSON *size_item = cJSON_GetObjectItem(root, "filesize");
            filesize = size_item->valueint;

            cJSON *md5_item = cJSON_GetObjectItem(root, "md5");
            strncpy(md5, md5_item->valuestring, sizeof(md5) - 1);
            str[sizeof(md5) - 1] = '\0';

            cJSON *filepath_item = cJSON_GetObjectItem(root, "filepath");
            strncpy(filepath, filepath_item->valuestring, sizeof(filepath) - 1);
            str[sizeof(filepath) - 1] = '\0';

            printf("Uploading file: %s, size: %d, md5: %s\n", filepath, filesize, md5);

            cJSON *send_root = cJSON_CreateObject();
            cJSON_AddStringToObject(send_root, "cmd", "FileTransfer");
            cJSON_AddStringToObject(send_root, "ctrl", "ready");
            char *json_str = cJSON_Print(send_root);
            int len = strlen(json_str);
            write(serial_fd, json_str, len);
            cJSON_Delete(send_root);
            free(json_str);
        }else{
            printf("Error: unknown state: %s\n", str);
        }

        int a=0;
        while (a<1){
            a++;
            //读串口512个字节内容
            char buffer[512];
            int file_len = 0;

            //创建文件 unix write()
            int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                printf("open file failed\n");
                return;
            }

            while (file_len < filesize) {
                int len = read(serial_fd, buffer, 512);
                if (len > 0) {
                    write(fd, buffer, len);
                    file_len += len;
                } else if (len == 0) {
                    printf("Connection closed\n");
                    close(fd);
                    remove(filepath);
                    break;
                } else {
                    perror("read");
                    close(fd);
                    remove(filepath);
                    break;
                }
            }
            close(fd);
        }
    
        //计算文件大小
        struct stat st;
        if (stat(filepath, &st) < 0) {
            printf("stat file failed\n");
            return;
        }
        printf("filesize: %ld\n", st.st_size);
        if (st.st_size != filesize) {
            printf("File size error\n");
        }

        //计算md5
        char md5_str[33];
        calculate_md5(filepath, md5_str);
        printf("md5: %s\n", md5_str);

        if (strcmp(md5_str, md5) == 0) {
            printf("File transfer success\n");
            cJSON *send_root = cJSON_CreateObject();
            cJSON_AddStringToObject(send_root, "cmd", "FileTransfer");
            cJSON_AddStringToObject(send_root, "ctrl", "success");
            char *json_str = cJSON_Print(send_root);
            int len = strlen(json_str);
            write(serial_fd, json_str, len);
            cJSON_Delete(send_root);
            free(json_str);

            // 使用base64解码文件内容
            char real_filepath[64]="/root/recycle_app_client";
            char base64_str[64];
            sprintf(base64_str, "base64 -d %s > %s", filepath,real_filepath);
            system(base64_str);
            printf("base64 decode success\n");
            //添加可执行权限
            char cmd[64];
            sprintf(cmd, "chmod +x %s",real_filepath);
            system(cmd);
        } else {
            printf("File transfer failed\n");
        }
    }
}

void handle_machine_ctrl(const char *main_json_str) {
    cJSON *root = cJSON_Parse(main_json_str);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON *ctrl_item = cJSON_GetObjectItem(root, "ctrl");
    if (cJSON_IsString(ctrl_item)) {
        char str[20];
        strncpy(str, ctrl_item->valuestring, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        if (strcmp(str, "reboot") == 0) {
            cJSON *send_root = cJSON_CreateObject();
            cJSON_AddStringToObject(send_root, "cmd", "MachineCtrl");
            cJSON_AddNumberToObject(send_root, "retCode", 0);
            char *json_str = cJSON_Print(send_root);
            int len = strlen(json_str);
            write(serial_fd, json_str, len);
            cJSON_Delete(send_root);
            free(json_str);

            system("reboot");
        }else if(strcmp(str, "update") == 0) {
            int ret=0;
            //判断 /root/recycle_app_client 是否存在
            if (access("/root/recycle_app_client", F_OK) == 0) {
                cJSON *send_root = cJSON_CreateObject();
                cJSON_AddStringToObject(send_root, "cmd", "MachineCtrl");
                cJSON_AddNumberToObject(send_root, "retCode", 0);
                char *json_str = cJSON_Print(send_root);
                int len = strlen(json_str);
                write(serial_fd, json_str, len);
                cJSON_Delete(send_root);
                free(json_str);
                exit(0);
            }else{
                cJSON *send_root = cJSON_CreateObject();
                cJSON_AddStringToObject(send_root, "cmd", "MachineCtrl");
                cJSON_AddNumberToObject(send_root, "retCode", 1);
                char *json_str = cJSON_Print(send_root);
                int len = strlen(json_str);
                write(serial_fd, json_str, len);
                cJSON_Delete(send_root);
                free(json_str);
            }
        }
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
        }else if (strcmp(str, "GetCleanDoor") == 0) {
            handle_get_clean_door_status(json_str);
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

    recycle_gpio_init();
    outside_light(1);
    sleep(1);
    outside_light(0);
    
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

