#include "device.h"
#include "ptzcmd.h"
#include "motor.h"
#include "common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> // 用于sleep函数的
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>


static int SN_MAX = 99999999;
static int sn;
char from_sip[100];
char contact[100];
char to_sip[100];

char server_id[21] = "41010500002000000001";
char server_ip[21] = "192.168.103.37";
int server_port = 5060;
int device_port = 5064;
char device_id[21] = "41010500002000000002";
char username[32] = "41010500002000000002";
char password[32] = "12345678";
char manufacture[32] = "lbc-4G-ipc";

// char *server_id = "34020000002000000001";
// char *server_ip = "192.168.103.8";
// int server_port = 6668;
// int device_port = 5064;
// char device_id[21];
// char username[21];
// char *password = "12345678";
// char *manufacture = "lbc-4G-ipc";

// char *server_id = "41010500002000000001";
// char *server_ip = "47.108.227.19";
// int server_port = 8116;
// int device_port = 5064;
// char device_id[21];
// char username[21];
// char *password = "newings2024";
// char *manufacture = "lbc-4G-ipc";

extern Nalu_t *nalu_vector;
extern int nalu_vector_size;
extern int rtp_sockfd;

AlarmInstance alarm_Instance;
struct eXosip_t* sip_context;
bool is_running=false;
bool is_register=false;
bool is_pushing=false;
int is_guard = 0;
char local_ip[24];
pthread_t heartbeat_thread_id;
pthread_t push_thread_id;
rtp_net_t rtp_net;
char ssrc[20];
extern int g_video_run_;

static int get_sn() {
	if (sn >= SN_MAX) {
		sn = 0;
	}
	sn++;
	return sn;
}

int device_info() {
    char chipid[17];  // 增加一个字节用于存放字符串结束符
    char chip_device_id[21];  // 21个字符 + 1个字节的字符串结束符
    char chip_username[17];  // 16个字符 + 1个字节的字符串结束符

    // 获取chipid并转换为设备ID
    rk_chip_id_get(chipid);
    printf("chipid : %s\n", chipid);

    // 将16进制字符串转换为无符号长长整数
    unsigned long long chipid_decimal = strtoull(chipid, NULL, 16);

    // 将10进制整数格式化为21位长度的字符串，高位用0补齐
    snprintf(chip_device_id, sizeof(chip_device_id), "%020llu", chipid_decimal);

    // 将10进制整数格式化为16位长度的16进制字符串，高位用0补齐
    snprintf(chip_username, sizeof(chip_username), "%016llx", chipid_decimal);

    printf("chip_device_id: %s\n", chip_device_id);
    printf("chip_username: %s\n", chip_username);

    strncpy(username, chip_username, sizeof(username) - 1);
    strncpy(device_id, chip_device_id, sizeof(device_id) - 1);

    // 打印更新前的设备信息
    printf("default device info: \n");
    printf("server_id: %s\n", server_id);
    printf("server_ip: %s\n", server_ip);
    printf("server_port: %d\n", server_port);
    printf("device_id: %s\n", device_id);
    printf("device_port: %d\n", device_port);
    printf("username: %s\n", username);
    printf("password: %s\n", password);
    printf("manufacture: %s\n", manufacture);

    // 获取配置并设置设备信息
    if (rk_param_get_int("gb28181.setting:enable", 0)) {
        // 使用临时变量存储获取的值
        const char *temp_server_id = rk_param_get_string("gb28181.setting:server_id", "default");
        const char *temp_server_ip = rk_param_get_string("gb28181.setting:server_ip", "default");
        int temp_server_port = rk_param_get_int("gb28181.setting:server_port", 0);
        const char *temp_device_id = rk_param_get_string("gb28181.setting:device_id", "default");
        int temp_device_port = rk_param_get_int("gb28181.setting:device_port", 0);
        const char *temp_username = rk_param_get_string("gb28181.setting:username", "default");
        const char *temp_password = rk_param_get_string("gb28181.setting:password", "default");
        const char *temp_manufacture = rk_param_get_string("gb28181.setting:manufacture", "default");

        // 打印临时变量值
        printf("ini file device info: \n");
        printf("temp_server_id: %s\n", temp_server_id);
        printf("temp_server_ip: %s\n", temp_server_ip);
        printf("temp_server_port: %d\n", temp_server_port);
        printf("temp_device_id: %s\n", temp_device_id);
        printf("temp_device_port: %d\n", temp_device_port);
        printf("temp_username: %s\n", temp_username);
        printf("temp_password: %s\n", temp_password);
        printf("temp_manufacture: %s\n", temp_manufacture);

        // 更新全局变量
        if (temp_server_port)
            server_port = temp_server_port;
        if (temp_device_port)
            device_port = temp_device_port;

        if (temp_server_id && strcmp(temp_server_id, "default") != 0) {
            strncpy(server_id, temp_server_id, sizeof(server_id) - 1);
        }
        if (temp_server_ip && strcmp(temp_server_ip, "default") != 0) {
            strncpy(server_ip, temp_server_ip, sizeof(server_ip) - 1);
        }
        if (temp_device_id && strcmp(temp_device_id, "default") != 0) {
            strncpy(device_id, temp_device_id, sizeof(device_id) - 1);
        }
        if (temp_username && strcmp(temp_username, "default") != 0) {
            strncpy(username, temp_username, sizeof(username) - 1);
        }
        if (temp_password && strcmp(temp_password, "default") != 0) {
            strncpy(password, temp_password, sizeof(password) - 1);
        }
        if (temp_manufacture && strcmp(temp_manufacture, "default") != 0) {
            strncpy(manufacture, temp_manufacture, sizeof(manufacture) - 1);
        }
    }

    // 打印更新后的设备信息
    printf("finally device info: \n");
    printf("server_id: %s\n", server_id);
    printf("server_ip: %s\n", server_ip);
    printf("server_port: %d\n", server_port);
    printf("device_id: %s\n", device_id);
    printf("device_port: %d\n", device_port);
    printf("username: %s\n", username);
    printf("password: %s\n", password);
    printf("manufacture: %s\n", manufacture);

    return 0;
}

void *device_heartbeat_task() {
    char *buffer = NULL;
    size_t buflen = 256;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
    }

	while (true) {
        if (is_register) {
            snprintf(buffer, buflen,
                "<?xml version=\"1.0\"?>\r\n"
                "<Notify>\r\n"
                "<CmdType>Keepalive</CmdType>\r\n"
                "<SN>%d</SN>\r\n"
                "<DeviceID>%s</DeviceID>\r\n"
                "<Status>OK</Status>\r\n"
                "</Notify>\r\n",
            get_sn(),device_id);

            osip_message_t* request = device_create_msg();
            if (request != NULL) {
                osip_message_set_content_type(request, "Application/MANSCDP+xml");
                osip_message_set_body(request, buffer, strlen(buffer));
                device_send_request(request);
                printf("sent heartbeat\n");
            }
        }

        sleep(60);
	}
}

osip_message_t * device_create_msg() {

    osip_message_t * request = NULL;
    int status = eXosip_message_build_request(sip_context, &request, "MESSAGE", to_sip, from_sip, NULL);
    if (OSIP_SUCCESS != status) {
        printf("build request failed:%d\n", status);
    }

    return request;
}

void *device_start(){

    const char *hostname = "8.8.8.8";
    const int port = 53; // DNS服务端口
    struct sockaddr_in saddr;
    int sd, rv;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &saddr.sin_addr);

    int network=0;
    while(network == 0){
        sd = socket(AF_INET, SOCK_STREAM, 0);
        if (sd < 0) {
            perror("socket");
            printf("sd = %d \n",sd);
        }else{
            rv = connect(sd, (struct sockaddr *)&saddr, sizeof(saddr));
            if (rv < 0) {
                perror("connect");
                printf("rv = %d \n",rv);
                close(sd);
            }else{
                printf("rv = %d \n",rv);
                close(sd);
                network=1;
            }
        }
        printf("connect again\n");
        sleep(1);
    }
    printf("connect succes\n");

    printf("sip init begin\n");
    sip_context = eXosip_malloc();

    if (OSIP_SUCCESS != eXosip_init(sip_context)) {
        printf("sip init failed.\n");
        return NULL;
    }

    if (OSIP_SUCCESS != eXosip_listen_addr(sip_context, IPPROTO_UDP, NULL, device_port, AF_INET, 0)) {
        printf("sip port bind failed.\n");
        eXosip_quit(sip_context);
        sip_context = NULL ;
        return NULL;
    }

    // 打开文件读取保存的位置
    FILE *file = fopen("/oem/guard_status.txt", "r");
    if (file != NULL) {
        // 文件存在，读取报警订阅状态
        fscanf(file, "guard_status=%d", &is_guard);
        LOG_INFO("读取本地报警订阅状态：%d\n", is_guard);
        fclose(file);
    } else {
        LOG_ERROR("本地报警订阅状态读取失败\n");
    }

    is_running = true;

    eXosip_guess_localip(sip_context, AF_INET, local_ip,sizeof(local_ip));
    printf("local_ip is %s\n",local_ip);

    snprintf(from_sip, sizeof(from_sip), "sip:%s@%s:%d", device_id,local_ip,device_port);
    snprintf(contact, sizeof(contact), "sip:%s@%s:%d", device_id,local_ip,device_port);
    snprintf(to_sip, sizeof(to_sip), "sip:%s@%s:%d", server_id,server_ip,server_port);

    printf("from_sip is %s\n",from_sip);
    printf("contact is %s\n",contact);
    printf("to_sip is %s\n",to_sip);

    // clear auth
    eXosip_clear_authentication_info(sip_context);

    osip_message_t * register_message = NULL;
    int register_id = eXosip_register_build_initial_register(sip_context, from_sip,
                    to_sip,contact , 3600, &register_message);

    printf("register_id is %d\n",register_id);

    if (NULL == register_message) {
        printf("eXosip_register_build_initial_register failed\n");
        return NULL;
    }

    eXosip_lock(sip_context);
	eXosip_register_send_register(sip_context, register_id, register_message);
	eXosip_unlock(sip_context);

    int status = pthread_create(&heartbeat_thread_id, NULL, device_heartbeat_task, NULL);
    if (status != 0) {
        fprintf(stderr, "Error creating thread: %s\n", strerror(status));
        return NULL;
    }

    device_process_request();
}

void device_send_response(eXosip_event_t *evt, osip_message_t * msg) {
    eXosip_lock(sip_context);
    eXosip_message_send_answer(sip_context, evt->tid, 200, msg);
    eXosip_unlock(sip_context);
}

void device_send_request(osip_message_t * request) {
    eXosip_lock(sip_context);
    eXosip_message_send_request(sip_context, request);
    eXosip_unlock(sip_context);
}

void device_send_response_ok(eXosip_event_t *evt) {
    osip_message_t *msg = evt->request;
    eXosip_message_build_answer(sip_context, evt->tid, 200, &msg);
    device_send_response(evt, msg);
}

cmd_sn_t device_get_cmd(const char * body) {
    mxml_node_t *tree, *node, *main_node;
    const char *tags[] = {"Query", "Control", "Notify", "Response"};
    int tags_num = 4;
    cmd_sn_t result;

    // 初始化结果
    result.cmd_type = NULL;
    result.sn = NULL;
    result.cmd_ptz = NULL;

    tree = mxmlLoadString(NULL,NULL,body);
    if (tree == NULL) {
        fprintf(stderr, "cannot parse the xml\n");
        return (cmd_sn_t){0, NULL, NULL};
    }

    for (int i = 0; i < tags_num; i++)
    {
        if (i == tags_num) {
            mxmlDelete(tree);
            return result;
        }

        main_node = mxmlFindElement(tree, tree, tags[i], NULL, NULL, MXML_DESCEND_ALL);
        if (main_node != NULL) {
            strcpy(result.cmd_name, tags[i]);
            break; // 找到标签后退出循环
        }
    }

    // fprintf(stderr, "%s xml with root\n",result.cmd_name);

    // 获取CmdType节点
    node = mxmlFindElement(tree, tree, "CmdType", NULL, NULL, MXML_DESCEND_ALL);
    if (node == NULL) {
        fprintf(stderr, "cannot get the cmd type\n");
        mxmlDelete(tree);
        return result;
    }

    result.cmd_type = strdup(mxmlGetText(node, NULL));
    // printf("get the cmd type %s\n",result.cmd_type);

    if(strcmp(result.cmd_type, "DeviceControl") == 0){
        node = mxmlFindElement(tree, tree, "PTZCmd", NULL, NULL, MXML_DESCEND_ALL);
        if (node != NULL) {
            result.cmd_ptz = strdup(mxmlGetText(node, NULL));
            printf("get the cmd_ptz %s\n", result.cmd_ptz);
        }
        node = mxmlFindElement(tree, tree, "GuardCmd", NULL, NULL, MXML_DESCEND_ALL);
        if (node != NULL) {
            result.cmd_guard = strdup(mxmlGetText(node, NULL));
            printf("get the cmd_guard %s\n", result.cmd_guard);
        }

    }else if(strcmp(result.cmd_type, "RecordInfo") == 0){
        node = mxmlFindElement(tree, tree, "StartTime", NULL, NULL, MXML_DESCEND_ALL);
        if (node == NULL) {
            fprintf(stderr, "cannot get the StartTime\n");
            mxmlDelete(tree);
            return result;
        }
        result.starttime = strdup(mxmlGetText(node, NULL));
        printf("starttime: %s\n", result.starttime);

        node = mxmlFindElement(tree, tree, "EndTime", NULL, NULL, MXML_DESCEND_ALL);
        if (node == NULL) {
            fprintf(stderr, "cannot get the EndTime\n");
            mxmlDelete(tree);
            return result;
        }
        result.endtime = strdup(mxmlGetText(node, NULL));
        printf("endtime: %s\n", result.endtime);
    }

    // 获取SN节点
    node = mxmlFindElement(tree, tree, "SN", NULL, NULL, MXML_DESCEND_ALL);
    if (node == NULL) {
        fprintf(stderr, "cannot get the SN\n");
        mxmlDelete(tree);
        return result;
    }

    result.sn = strdup(mxmlGetText(node, NULL));
    // printf("get the SN %s\n",result.sn);
    mxmlDelete(tree);
    return result;
}

void device_process_request() {
    uint64_t i=0;
    while (g_video_run_) {
        eXosip_event_t *evt = eXosip_event_wait(sip_context, 0, 100);
        eXosip_lock(sip_context);
        eXosip_automatic_action(sip_context);
        eXosip_unlock(sip_context);

        if (evt == NULL) {
            continue;
        }

        switch (evt->type){
        case EXOSIP_REGISTRATION_SUCCESS: {
            printf("got REGISTRATION_SUCCESS\n");
            is_register = true;
            break;
        }
        case EXOSIP_REGISTRATION_FAILURE: {
            printf("got REGISTRATION_FAILURE\n");
            if (evt->response == NULL) {
                printf("register 401 has no response !!!\n");
                break;
            }

            if (401 == evt->response->status_code) {
                osip_www_authenticate_t * www_authenticate_header;

                osip_message_get_www_authenticate(evt->response, 0, &www_authenticate_header);

                if (eXosip_add_authentication_info(sip_context, device_id, username, password,
                                    "MD5", www_authenticate_header->realm)) {
                    printf("register add auth failed\n");
                    break;
                };
            };
            break;
        }
        case EXOSIP_MESSAGE_NEW: {
            printf("got MESSAGE_NEW\n");
            // printf("ss_status=%d,ss_reason=%d\n",evt->ss_status,evt->ss_reason);
            if (MSG_IS_MESSAGE(evt->request)) {
                osip_body_t * body = NULL;
                osip_message_get_body(evt->request, 0, &body);
                device_send_response_ok(evt);
                printf("%s\n",body->body);
                cmd_sn_t cmd_sn = device_get_cmd(body->body);
                printf("got new cmd: %s sn: %s name: %s\n", cmd_sn.cmd_type, cmd_sn.sn, cmd_sn.cmd_name);
                if (strcmp(cmd_sn.cmd_name, "Query") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "Catalog") == 0)
                        device_process_catalog_query(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "DeviceStatus") == 0)
                        device_process_devicestatus_query(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "DeviceInfo") == 0)
                        device_process_deviceinfo_query(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "PresetQuery") == 0)
                        device_process_presetquery_query(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "RecordInfo") == 0)
                        device_process_record_info_query(cmd_sn);
                    else if (strcmp(cmd_sn.cmd_type, "Alarm") == 0)
                        device_process_undo(cmd_sn.sn);
                    else
                        printf("unhandled cmd: %s\n", cmd_sn.cmd_type);
                }
                else if (strcmp(cmd_sn.cmd_name, "Control") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "DeviceControl") == 0)
                        if (cmd_sn.cmd_ptz != NULL)
                            device_process_devicecontrol_ptz_control(cmd_sn.cmd_ptz);
                        else if (cmd_sn.cmd_guard != NULL)
                            device_process_devicecontrol_guard_control(cmd_sn, cmd_sn.cmd_guard);
                    else
                        printf("unhandled control cmd: %s\n", cmd_sn.cmd_type);
                }else if (strcmp(cmd_sn.cmd_name, "Notify") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "Broadcast") == 0)
                        device_process_broadcast_notify(evt,cmd_sn);
                    else if (strcmp(cmd_sn.cmd_type, "Alarm") == 0)
                        device_process_undo(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "MediaStatus") == 0)
                        device_process_undo(cmd_sn.sn);
                    else if (strcmp(cmd_sn.cmd_type, "Catalog") == 0)
                        device_process_undo(cmd_sn.sn);
                    else
                        printf("unhandled control cmd: %s\n", cmd_sn.cmd_type);
                }
                else
                    printf("not get cmd_name\n");
            }
            else if (MSG_IS_BYE(evt->request)) {
                printf("got BYE message\n");
                device_send_response_ok(evt);
                break;
            }
            break;
        }
        case EXOSIP_CALL_INVITE: {
            printf("got CALL_INVITE\n");

            sdp_message_t *sdp_msg = eXosip_get_remote_sdp(sip_context, evt->did);
            if (!sdp_msg) {
                printf("eXosip_get_remote_sdp failed\n");
                break;
            }

            sdp_connection_t *connection = eXosip_get_video_connection(sdp_msg);
            if (!connection) {
                printf("eXosip_get_video_connection failed\n");
                break;
            }

            rtp_net.rtp_ip = connection->c_addr;

            sdp_media_t *video_sdp = eXosip_get_video_media(sdp_msg);
            if (!video_sdp) {
                printf("eXosip_get_video_media failed\n");
                break;
            }

            rtp_net.rtp_port = atoi(video_sdp->m_port);
            printf("rtp server: %s:%d\n", rtp_net.rtp_ip, rtp_net.rtp_port);

            rtp_net.rtp_protocol = video_sdp->m_proto;

            printf("rtp protocol %s \n", rtp_net.rtp_protocol);

            osip_body_t *sdp_body = NULL;
			osip_message_get_body(evt->request, 0, &sdp_body);
            if (NULL == sdp_body) {
                printf("osip_message_get_body failed\n");
                break;
            }

            rtp_net.listen_port = rtp_net.rtp_port;
            printf("rtp listen_port %d\n", rtp_net.listen_port);

            char *y_sdp_first_index = strstr(sdp_body->body, "y=");
            if (y_sdp_first_index != NULL) {
                char *y_sdp_last_index = strstr(y_sdp_first_index, "\r\n");
                if (y_sdp_last_index != NULL) {
                    int ssrc_length = y_sdp_last_index - (y_sdp_first_index + 2);
                    if (ssrc_length > 0 && ssrc_length < sizeof(ssrc) - 1) {
                        strncpy(ssrc, y_sdp_first_index + 2, ssrc_length);
                        ssrc[ssrc_length] = '\0'; // 确保字符串以null结尾
                        printf("The extracted SSRC is: %s\n", ssrc);
                    } else
                        printf("Invalid SSRC length.\n");
                } else
                    printf("Could not find the end of the SDP line.\n");
            } else
                printf("Could not find the 'y=' in the SDP body.\n");

            char *buffer = NULL;
            size_t buflen = 1024;
            size_t len = 0;

            buffer = (char *)malloc(buflen);
            if (buffer == NULL) {
                fprintf(stderr, "Failed to allocate memory\n");
                return;
            }

            if (strcmp(rtp_net.rtp_protocol, "TCP/RTP/AVP") == 0) {
                snprintf(buffer, buflen,
                    "v=0\r\n"
                    "o=%s 0 0 IN IP4 %s\r\n"
                    "s=Play\r\n"
                    "c=IN IP4 %s\r\n"
                    "t=0 0\r\n"
                    "m=video %d TCP/RTP/AVP 96\r\n"
                    "a=sendonly\r\n"
                    "a=rtpmap:96 PS/90000\r\n"
                    "y=%s\r\n",
                device_id,local_ip,local_ip,rtp_net.listen_port,ssrc);
            }
            else if (strcmp(rtp_net.rtp_protocol, "RTP/AVP") == 0) {
                snprintf(buffer, buflen,
                    "v=0\r\n"
                    "o=%s 0 0 IN IP4 %s\r\n"
                    "s=Play\r\n"
                    "c=IN IP4 %s\r\n"
                    "t=0 0\r\n"
                    "m=video %d RTP/AVP 96\r\n"
                    "a=sendonly\r\n"
                    "a=rtpmap:96 PS/90000\r\n"
                    "y=%s\r\n",
                device_id,local_ip,local_ip,rtp_net.listen_port,ssrc);
            }
            else
                printf("unsopport rtp_protocol:%s\n",rtp_net.rtp_protocol);


            printf("buffer is :\n%s\n",buffer);

            osip_message_t * message = evt->request;
            int status = eXosip_call_build_answer(sip_context, evt->tid, 200, &message);

            if (status != 0) {
                printf("call invite build answer failed");
                break;
            }

            osip_message_set_content_type(message, "APPLICATION/SDP");
            osip_message_set_body(message, buffer, strlen(buffer));

            eXosip_call_send_answer(sip_context, evt->tid, 200, message);
            break;
        }
        case EXOSIP_CALL_ACK: {
            // if(is_pushing == 1){
            //     printf("Already pushing\n");
            // }else {
            //     printf("got CALL_ACK: begin pushing rtp stream...\n");
            //     int status = pthread_create(&push_thread_id, NULL, device_push_rtp_stream, NULL);
            //     if (status != 0) {
            //         fprintf(stderr, "Error creating thread: %s\n", strerror(status));
            //         return ;
            //     }
            // }
            is_pushing = true;
            printf("got CALL_ACK: begin pushing rtp stream...\n");
            printf("is_pushing = %d\n",is_pushing);
            int status=0;
            status = device_bind(rtp_net.listen_port);
            if (status != 0) {
                printf("device bind socket address failed: %d\n", status);
                return 0;
            }

			printf("device bind socket address success\n");
            break;
        }
        case EXOSIP_CALL_CLOSED: {
            printf("got CALL_CLOSED: stop pushing rtp stream...\n");
            // pthread_cancel(push_thread_id);
            is_pushing=false;
            close(rtp_sockfd);
            break;
        }
        case EXOSIP_MESSAGE_ANSWERED: {
            printf("got MESSAGE_ANSWERED: unhandled\n");
            break;
        }
        case EXOSIP_IN_SUBSCRIPTION_NEW:{
            printf("get EXOSIP_IN_SUBSCRIPTION_NEW \n");
            if (MSG_IS_SUBSCRIBE(evt->request)) {
                osip_body_t * body = NULL;
                osip_message_get_body(evt->request, 0, &body);
                device_send_response_ok(evt);
                printf("%s\n",body->body);
                cmd_sn_t cmd_sn = device_get_cmd(body->body);
                printf("got new name: %s cmd: %s sn: %s\n", cmd_sn.cmd_name, cmd_sn.cmd_type, cmd_sn.sn);
                if (strcmp(cmd_sn.cmd_name, "Query") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "Alarm") == 0)
                        device_subscribe_alarm(evt,cmd_sn);
                    else
                        printf("unhandled cmd: %s\n", cmd_sn.cmd_type);
                }
            }
            break;
        }
        default: {
            printf("unhandled sip evt type%d\n", evt->type);
            break;
        }
        }
        eXosip_event_free(evt);
    }
}

void device_process_catalog_query(char *sn) {
    char *buffer = NULL;
    size_t buflen = 1024;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    snprintf(buffer, buflen,
         "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
         "<Response>\r\n"
         "<CmdType>Catalog</CmdType>\r\n"
         "<SN>%s</SN>\r\n"
         "<DeviceID>%s</DeviceID>\r\n"
         "<SumNum>1</SumNum>\r\n"
         "<DeviceList Num=\"1\">\r\n"
         "<Item>\r\n"
         "<DeviceID>%s</DeviceID>\r\n"
         "<Manufacturer>%s</Manufacturer>\r\n"
         "<Status>ON</Status>\r\n"
         "<Name>IPC</Name>\r\n"
         "<ParentID>%s</ParentID>\r\n"
         "</Item>\r\n"
         "</DeviceList>\r\n"
         "</Response>\r\n",
         sn,device_id, device_id, manufacture, server_id);

    printf("catalog response: %s\n", buffer);


    osip_message_t *request = device_create_msg();
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }
    free(buffer); // 释放内存
}

void device_process_devicestatus_query(char *sn) {
    char *buffer = NULL;
    size_t buflen = 1024;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char curtime[72] = {0};
    sprintf(curtime, "%d-%d-%dT%02d:%02d:%02d", (timeinfo->tm_year + 1900), (timeinfo->tm_mon + 1),
                        timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    snprintf(buffer, buflen,
        "<?xml version=\"1.0\"?>\r\n"
        "<Response>\r\n"
        "<CmdType>DeviceStatus</CmdType>\r\n"
        "<SN>%d</SN>\r\n"
        "<DeviceID>%s</DeviceID>\r\n"
        "<Result>OK</Result>\r\n"
        "<Online>ONLINE</Online>\r\n"
        "<Status>OK</Status>\r\n"
        "<DeviceTime>%s</DeviceTime>\r\n"
        "<Alarmstatus Num=\"0\">\r\n"
        "</Alarmstatus>\r\n"
        "<Encode>ON</Encode>\r\n"
        "<Record>OFF</Record>\r\n"
        "</Response>\r\n",
     get_sn(),device_id, curtime);

    printf("devicestatus response: %s\n", buffer);
    osip_message_t *request = device_create_msg();
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }
    free(buffer); // 释放内存
}

void device_process_deviceinfo_query(char *sn) {
    char *buffer = NULL;
    size_t buflen = 1024;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    snprintf(buffer, buflen,
        "<?xml version=\"1.0\"?>\r\n"
        "<Response>\r\n"
        "<CmdType>DeviceInfo</CmdType>\r\n"
        "<SN>%d</SN>\r\n"
        "<DeviceID>%s</DeviceID>\r\n"
        "<Result>OK</Result>\r\n"
        "<DeviceType>simulate client</DeviceType>\r\n"
        "<Manufacturer>ZHD</Manufacturer>\r\n"
        "<Model>28181</Model>\r\n"
        "<Firmware>fireware</Firmware>\r\n"
        "<MaxCamera>1</MaxCamera>\r\n"
        "<MaxAlarm>0</MaxAlarm>\r\n"
        "</Response>\r\n",
        get_sn(),device_id);

    printf("deviceinfo response: \n%s\n", buffer);
    osip_message_t *request = device_create_msg();
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }
    free(buffer); // 释放内存
}

void device_process_devicecontrol_query(char *sn) {

}

void device_process_presetquery_query(char *sn) {
    size_t buflen = 0x6000;
    char *buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    // 加载预置点信息
    int num_presets = load_presets();

    // 初始化 XML 内容
    snprintf(buffer, buflen,
             "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
             "<Response>\r\n"
             "<CmdType>PresetQuery</CmdType>\r\n"
             "<SN>%s</SN>\r\n"
             "<DeviceID>%s</DeviceID>\r\n"
             "<Result>OK</Result>\r\n"
             "<PresetList Num=\"%d\">\r\n",
             sn, device_id, num_presets);

    // 计算当前 XML 数据长度
    size_t len = strlen(buffer);

    // 动态生成预置点 XML，并追加到 buffer 中
    for (int i = 0; i < MAX_PRESETS; ++i) {
        if (presets[i].is_set) {
            char itemXML[256];  // 根据实际情况调整大小
            snprintf(itemXML, sizeof(itemXML),
                     "<Item>\r\n"
                     "<PresetID>%d</PresetID>\r\n"
                     "<PresetName>%s</PresetName>\r\n"
                     "</Item>\r\n",
                     i, presets[i].name);

            size_t itemXML_len = strlen(itemXML);


            // 追加预置点 XML 数据到 buffer
            strcat(buffer, itemXML);
            len += itemXML_len;
        }
    }

    // 在 buffer 末尾添加 XML 尾部信息
    strcat(buffer, "</PresetList>\r\n"
                   "</Response>\r\n");

    printf("deviceinfo response: \n%s\n", buffer);

    osip_message_t *request = device_create_msg();
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }

    free(buffer);
}

void device_process_devicecontrol_ptz_control(char *ptz) {
    PtzCommand cmd;
    parseCmdString(ptz, &cmd);
}

void device_process_devicecontrol_guard_control(cmd_sn_t cmd_sn, char *guard_status) {
    char *buffer = NULL;
    size_t buflen = 384;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    snprintf(buffer, buflen,
        "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
        "<Response>\r\n"
        "<CmdType>DeviceControl</CmdType>\r\n"
        "<SN>%s</SN>\r\n"
        "<DeviceID>%s</DeviceID>\r\n"
        "<Result>OK</Result>\r\n"
        "</Response>\r\n",
        cmd_sn.sn, device_id);

    printf("deviceinfo response: \n%s\n", buffer);

    osip_message_t *request = device_create_msg("MESSAGE");
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }
    free(buffer); // 释放内存

    if (strcmp(guard_status, "SetGuard") == 0)
        is_guard=1;
    else
        is_guard=0;

    // 打开或创建文件以写入报警订阅状态
    FILE *file = fopen("/oem/guard_status.txt", "w");
    if (file == NULL) {
        LOG_ERROR("open /oem/guard_status.txt failed\n");
        return -1;
    }

    // 将报警订阅状态写入文件
    fprintf(file, "guard_status=%d", is_guard);
    LOG_INFO("本地报警订阅状态写入:\n", is_guard);
    fclose(file);

}

void device_subscribe_alarm(eXosip_event_t *evt,cmd_sn_t cmd_sn){
    char *buffer = NULL;
    size_t buflen = 512;
    size_t len = 0;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    snprintf(buffer, buflen,
        "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
        "<Response>\r\n"
        "<CmdType>Alarm</CmdType>\r\n"
        "<SN>%s</SN>\r\n"
        "<DeviceID>%s</DeviceID>\r\n"
        "<Result>OK</Result>\r\n"
        "</Response>\r\n",
        cmd_sn.sn, device_id);

    printf("deviceinfo response: \n%s\n", buffer);

    osip_message_t * request = NULL;
    int status = eXosip_subscribe_build_initial_request(sip_context, &request, to_sip, from_sip, NULL,"presence",3600);
    if (OSIP_SUCCESS != status) {
        printf("build request failed:%d\n", status);
    }
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        eXosip_subscribe_send_initial_request(sip_context,request);
    }
    free(buffer); // 释放内存
}

void device_process_broadcast_notify(eXosip_event_t *evt , cmd_sn_t cmd_sn) {

}

void device_process_undo(char *sn) {
    printf("func undo!!!\n");
}

void device_process_record_info_query(cmd_sn_t cmd_sn) {

}

void device_send_alarm(alarmEvent event) {
    if (!is_running || !is_guard) {
        return;
    }

    if (event == EVENT_MD && get_motor_status()) {
        return;
    }

    time_t current_time;
    time(&current_time);

    // 检查事件是否在设定时间内重复
    if (difftime(current_time, alarm_Instance.last_event_time[event]) < ALARM_TIME_THRESHOLD) {
        printf("Event %d ignored due to time threshold\n", event);
        return;
    }

    struct tm* timeinfo = localtime(&current_time);

    snprintf(alarm_Instance.time[event], sizeof(alarm_Instance.time[event]),
             "%d-%02d-%02dT%02d:%02d:%02d",
             (timeinfo->tm_year + 1900), (timeinfo->tm_mon + 1),
             timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    printf("alarm_Instance.time[%d]=%s\n", event, alarm_Instance.time[event]);

    switch (event) {
        case EVENT_MD:
            printf("Event: Movement Detection\n");
            snprintf(alarm_Instance.description[event], sizeof(alarm_Instance.description[event]), "移动侦测报警");
            alarm_Instance.method = METHOD_VIDEO;
            alarm_Instance.priority = 4;
            break;
        case EVENT_OD:
            printf("Event: Obstruction Detection\n");
            snprintf(alarm_Instance.description[event], sizeof(alarm_Instance.description[event]), "摄像头遮挡报警");
            alarm_Instance.method = METHOD_DEVICE;
            alarm_Instance.priority = 1;
            break;
        case EVENT_PERSON:
            printf("Event: Person Detection\n");
            snprintf(alarm_Instance.description[event], sizeof(alarm_Instance.description[event]), "人体检测报警");
            alarm_Instance.method = METHOD_VIDEO;
            alarm_Instance.priority = 3;
            break;
        default:
            printf("Event: None or Unknown\n");
            return;
    }

    char *buffer = NULL;
    size_t buflen = 1024;

    buffer = (char *)malloc(buflen);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    snprintf(buffer, buflen,
        "<?xml version=\"1.0\"?>\r\n"
        "<Notify>\r\n"
        "<CmdType>Alarm</CmdType>\r\n"
        "<SN>%d</SN>\r\n"                                     /*命令序列号*/
        "<DeviceID>%s</DeviceID>\r\n"                         /*设备编码*/
        "<AlarmPriority>%d</AlarmPriority>\r\n"               /*报警等级*/
        "<AlarmTime>%s</AlarmTime>\r\n"                       /*报警时间*/
        "<AlarmMethod>%d</AlarmMethod>\r\n"                   /*报警方式*/
        "<AlarmDescription>%s</AlarmDescription>\r\n"         /*警情描述*/
        "</Notify>\r\n",
        get_sn(), device_id, alarm_Instance.priority, alarm_Instance.time[event],
        alarm_Instance.method, alarm_Instance.description[event]);

    printf("buffer=%s\n", buffer);

    osip_message_t *request = device_create_msg("MESSAGE");
    if (request != NULL) {
        osip_message_set_content_type(request, "Application/MANSCDP+xml");
        osip_message_set_body(request, buffer, strlen(buffer));
        device_send_request(request);
    }

    // 更新上次事件发生时间
    alarm_Instance.last_event_time[event] = current_time;
    alarm_Instance.event = EVENT_NONE;

    free(buffer);
}
