#ifndef DEVICE_INCLUDE
#define DEVICE_INCLUDE
#include <stdio.h>
#include "eXosip2/eXosip.h"
#include <mxml.h>
#include "gb28181_header_maker.h"
#include "nalu.h"

typedef struct {
    char *cmd_type;
    char *sn;
    char *cmd_ptz;
    char cmd_name[20];
} cmd_sn_t;

typedef struct {
    char *rtp_ip;
    int rtp_port;
    int listen_port;
    char *rtp_protocol;
} rtp_net_t;


int device_info();
int device_start();
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