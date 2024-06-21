#ifndef DEVICE_INCLUDE
#define DEVICE_INCLUDE
#include <stdio.h>
#include "eXosip2/eXosip.h"
#include "libmxml4/mxml.h"
#include "gb28181_header_maker.h"
#include "nalu.h"

#define EVENT_MAX_NUM (4)
#define ALARM_TIME_THRESHOLD 60 // 时间阈值，以秒为单位

// 定义报警方式的枚举类型
typedef enum {
    METHOD_PHONE = 1,           /* 电话报警 */
    METHOD_DEVICE = 2,          /* 设备报警 */
    METHOD_SMS = 3,             /* 短信报警 */
    METHOD_GPS = 4,             /* GPS报警 */
    METHOD_VIDEO = 5,           /* 视频报警 */
    METHOD_FAULT = 6,           /* 故障报警 */
    METHOD_OTHERS = 7           /* 其他报警 */
} alarmMethod;

// 定义报警事件的枚举类型
typedef enum {
    EVENT_NONE = 0,
    EVENT_MD = 1,               /* 移动侦测 */
    EVENT_OD = 2,               /* 遮挡报警 */
    EVENT_PERSON = 3            /* 人体检测 */
} alarmEvent;

// 定义报警结构体
typedef struct {
    int priority;                           /* 报警等级 1-4 */
    char description[EVENT_MAX_NUM][64];    /* 警情描述 */
    char time[EVENT_MAX_NUM][32];           /* 报警发生时间 */
    alarmMethod method;                     /* 报警方式 */
    alarmEvent event;                       /* 报警事件 */
    time_t last_event_time[EVENT_MAX_NUM];  /* 上次事件发生时间 */
} AlarmInstance;

typedef struct {
    char *cmd_type;
    char *sn;
    char *cmd_ptz;
    char *cmd_guard;
    char cmd_name[20];
    char *starttime;
    char *endtime;
} cmd_sn_t;

typedef struct {
    char *rtp_ip;
    int rtp_port;
    int listen_port;
    char *rtp_protocol;
} rtp_net_t;


int device_info();
void *device_start();
void device_process_request();
osip_message_t * device_create_msg();
cmd_sn_t device_get_cmd(const char * body);
void device_send_response(eXosip_event_t *evt, osip_message_t * msg);
void device_send_response_ok(eXosip_event_t *evt);
void *device_heartbeat_task();
void device_process_deviceinfo_query(char *sn);
void device_process_devicestatus_query(char *sn);
void device_process_catalog_query(char *sn);
void device_send_request(osip_message_t * request);
void device_process_devicecontrol_query(char *sn);
void device_process_devicecontrol_control(char *ptz);
void device_send_network_packet(const char * data, int length);
int device_bind(int listen_port);

#endif