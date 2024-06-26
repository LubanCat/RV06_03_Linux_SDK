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


// 初始化模块
int module_init(void);

// 获取信号强度
int module_signal(void);

// 获取ICCID
int module_iccid(void);

// 获取IMSI
int module_imsi(void);

// 获取IMEI
int module_imei(void);

// 反初始化模块
int module_deinit(void);

// 获取模块所有信息
int module_info(void);

#endif // MODULE_H
