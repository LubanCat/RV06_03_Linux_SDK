#ifndef MODULE_H
#define MODULE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

// 4G模块参数结构体
typedef struct {
    char iccid[32];  // ICCID
    char imei[32];   // IMEI
    char imsi[32];   // IMSI
    int signal;      // 信号强度
} ModuleInfo;

extern ModuleInfo moduleInfo;

// 获取信号强度
int module_signal(void);

// 获取ICCID
int module_iccid(int fd);

// 获取IMSI
int module_imsi(int fd);

// 获取IMEI
int module_imei(int fd);

// 获取模块所有信息
int module_info(void);

#endif // MODULE_H
