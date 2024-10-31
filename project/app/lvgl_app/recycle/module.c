#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "module.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h> // 添加这行以

ModuleInfo moduleInfo;

// 将fd设置为非阻塞模式
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Failed to get file status flags");
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("Failed to set file status flags");
        return -1;
    }

    return 0;
}

int module_signal() {
    int ret;
    char buf[256];
    int rssi = 99, ber;

    int fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Cannot open serial port device /dev/ttyUSB2");
        return -1;
    }
    // // 设置fd为非阻塞模式
    // if (set_nonblocking(fd) < 0) {
    //     close(fd);
    //     return 99;
    // }

    // 发送AT+CSQ命令查询信号质量
    ret = write(fd, "AT+CSQ\r\n", 8);
    if (ret < 0) {
        perror("Failed to send AT+CSQ command");
        close(fd);
        return 99;
    }

    for (int i=0;i<5;i++){
        // 初始化读取缓冲区
        memset(buf, 0, sizeof(buf));

        // 读取AT+CSQ命令的响应
        ret = read(fd, buf, sizeof(buf));
        if (ret < 0) {
            if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                printf("No data available to read\n");
            } else {
                perror("Failed to read AT+CSQ response");
            }
            close(fd);
            return 99;
        }
        // 读到换行符就跳过继续读
        if(ret > 1)
            break;
    }
    // 提取信号质量数据
    char *start = strstr(buf, "+CSQ: ");
    if (start) {
        sscanf(start, "+CSQ: %d,%d", &rssi, &ber);
        printf("Signal Quality: RSSI=%d, BER=%d\n", rssi, ber);
    } else {
        printf("No +CSQ response found\n");
    }

    close(fd);

    return rssi;
}

int module_iccid(int fd) {
    int ret;
    char buf[256];

    ret = write(fd, "AT+MCCID\r\n", 10);
    if (ret < 0) {
        perror("Failed to send AT+MCCID command");
        close(fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+MCCID response");
        close(fd);
        return -1;
    }

    // 提取ICCID数据
    char *start = strstr(buf, "+MCCID: ");
    if (start) {
        sscanf(start, "+MCCID: %31s", moduleInfo.iccid);
        printf("ICCID=%s\n", moduleInfo.iccid);
    } else {
        printf("No +MCCID response found\n");
    }

    return 0;
}

int module_imsi(int fd) {
    int ret;
    char buf[256];

    ret = write(fd, "AT+CIMI\r\n", 9);
    if (ret < 0) {
        perror("Failed to send AT+CIMI command");
        close(fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+CIMI response");
        close(fd);
        return -1;
    }

    // 提取IMSI数据
    if (sscanf(buf, "%31s", moduleInfo.imsi) == 1) {
        printf("IMSI=%s\n", moduleInfo.imsi);
    } else {
        printf("No valid IMSI response found\n");
    }

    return 0;
}

int module_imei(int fd) {
    int ret;
    char buf[256];

    ret = write(fd, "AT+CGSN\r\n", 9);
    if (ret < 0) {
        perror("Failed to send AT+CGSN command");
        close(fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+CGSN response");
        close(fd);
        return -1;
    }

    // 提取IMEI数据
    if (sscanf(buf, "%31s", moduleInfo.imei) == 1) {
        printf("IMEI=%s\n", moduleInfo.imei);
    } else {
        printf("No valid IMEI response found\n");
    }

    return 0;
}

int module_restart() {
    int ret;
    int fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Cannot open serial port device /dev/ttyUSB2");
        return -1;
    }

    // 发送重置网卡命令
    ret = write(fd, "AT+MREBOOT=0\r\n", 8);
    if (ret < 0) {
        perror("Failed to send AT+CSQ command");
        close(fd);
        return -1;
    }
    close(fd);

    sleep(5);

    system("ifconfig eth1 up");
    system("udhcpc -i eth1");
    return 0;
}

int module_info(void) {
    int ret;
    struct termios options;

    int fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Cannot open serial port device /dev/ttyUSB2");
        return -1;
    }

    // 串口配置
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &options);

    // 发送ATE0命令关闭回显
    ret = write(fd, "ATE0\r\n", 6);
    if (ret < 0) {
        perror("Failed to send ATE0 command\n");
        close(fd);
        return -1;
    }

    usleep(100000); // 等待响应

    // 读取ATE0命令的响应
    char buf[256];
    ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read ATE0 response\n");
        close(fd);
        return -1;
    }

    module_iccid(fd);
    module_imsi(fd);
    module_imei(fd);

    close(fd);

    return 0;
}

int module_get_time() {
    int ret;
    int fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY );
    if (fd < 0) {
        perror("Cannot open serial port device /dev/ttyUSB2");
        return -1;
    }

    // 发送AT+CCLK?命令更新时间
    ret = write(fd, "AT+CCLK?\r\n", 11);
    if (ret < 0) {
        perror("Failed to send AT+CCLK? command");
    }
    char buf[256];
    for (int i=0;i<5;i++){
        // 初始化读取缓冲区
        memset(buf, 0, sizeof(buf));

        // 读取AT+CSQ命令的响应
        ret = read(fd, buf, sizeof(buf));
        if (ret < 0) {
            if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                printf("No data available to read\n");
            } else {
                perror("Failed to read AT+CCLK? response");
            }
            close(fd);
            return 99;
        }
        // 读到换行符就跳过继续读
        if(ret > 1){
            close(fd);
            break;
        }
            
    }

    //将时间str转换成时间，str的格式是+CCLK: "24/10/30,08:24:31+32"
    int year, month, day, hour, minute, second, timezone;
    char *start = strstr(buf, "+CCLK: ");
    if (start) {
        printf("start: %s\n", start+8);
        //将"24/10/30,08:57:57+32"转化为时间
        sscanf(start+8, "%d/%d/%d,%d:%d:%d+%d", &year, &month, &day, &hour, &minute, &second, &timezone);
        printf("get UTC time: %04d-%02d-%02d %02d:%02d:%02d\n", year += 2000, month, day, hour, minute, second);
    } else {
        printf("No +CCLK response found\n");
    }

    struct tm new_time;
    struct timeval tv;

    // 将年份转换为从1900年开始的年数
    new_time.tm_year = year - 1900;
    new_time.tm_mon = month - 1;  // tm_mon是从0开始的
    new_time.tm_mday = day;
    new_time.tm_hour = hour;
    new_time.tm_min = minute;
    new_time.tm_sec = second;

    // 将tm结构转换为time_t，并设置时区为本地时区
    new_time.tm_isdst = -1;  // 让库函数决定是否使用夏令时
    tv.tv_sec = mktime(&new_time);
    tv.tv_usec = 0;  // 微秒部分设置为0
    tv.tv_sec += timezone/4*3600;  // 东八区，UTC+8:00

    // 设置系统时间
    if (settimeofday(&tv, NULL) == -1) {
        perror("settimeofday");
        return -1;
    }
    return 0;
}