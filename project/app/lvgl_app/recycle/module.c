#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "module.h"

int tty_fd;

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

int module_init(void) {
    int ret;
    struct termios options;

    tty_fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY | O_NDELAY);
    if (tty_fd < 0) {
        perror("Cannot open serial port device /dev/ttyUSB2");
        return -1;
    }

    // 串口配置
    tcgetattr(tty_fd, &options);
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
    tcsetattr(tty_fd, TCSANOW, &options);

    // 设置tty_fd为非阻塞模式
    if (set_nonblocking(tty_fd) < 0) {
        close(tty_fd);
        return 99;
    }

    // 发送ATE0命令关闭回显
    ret = write(tty_fd, "ATE0\r\n", 6);
    if (ret < 0) {
        perror("Failed to send ATE0 command");
        close(tty_fd);
        return -1;
    }

    usleep(100000); // 等待响应

    // 读取ATE0命令的响应
    char buf[256];
    ret = read(tty_fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read ATE0 response");
        close(tty_fd);
        return -1;
    }

    return 0;
}

int module_signal() {
    int ret;
    char buf[256];
    int rssi = 99, ber;

    // 发送AT+CSQ命令查询信号质量
    ret = write(tty_fd, "AT+CSQ\r\n", 8);
    if (ret < 0) {
        perror("Failed to send AT+CSQ command");
        close(tty_fd);
        return 99;
    }

    usleep(100000); // 等待响应

    // 初始化读取缓冲区
    memset(buf, 0, sizeof(buf));

    // 读取AT+CSQ命令的响应，非阻塞读取一次
    ret = read(tty_fd, buf, sizeof(buf));
    if (ret < 0) {
        if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("No data available to read\n");
        } else {
            perror("Failed to read AT+CSQ response");
        }
        close(tty_fd);
        return 99;
    }

    // 提取信号质量数据
    char *start = strstr(buf, "+CSQ: ");
    if (start) {
        sscanf(start, "+CSQ: %d,%d", &rssi, &ber);
        printf("Signal Quality: RSSI=%d, BER=%d\n", rssi, ber);
    } else {
        printf("No +CSQ response found\n");
    }

    return rssi;
}

int module_iccid(void) {
    int ret;
    char buf[256];

    ret = write(tty_fd, "AT+MCCID\r\n", 10);
    if (ret < 0) {
        perror("Failed to send AT+MCCID command");
        close(tty_fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(tty_fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+MCCID response");
        close(tty_fd);
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

int module_imsi(void) {
    int ret;
    char buf[256];

    ret = write(tty_fd, "AT+CIMI\r\n", 9);
    if (ret < 0) {
        perror("Failed to send AT+CIMI command");
        close(tty_fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(tty_fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+CIMI response");
        close(tty_fd);
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

int module_imei(void) {
    int ret;
    char buf[256];

    ret = write(tty_fd, "AT+CGSN\r\n", 9);
    if (ret < 0) {
        perror("Failed to send AT+CGSN command");
        close(tty_fd);
        return -1;
    }

    usleep(100000); // 等待响应

    ret = read(tty_fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read AT+CGSN response");
        close(tty_fd);
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

int module_deinit(void) {
    close(tty_fd);
    return 0;
}

int module_info(void) {
    module_iccid();
    module_imsi();
    module_imei();
    return 0;
}
