#include "device.h"
#include "ptzcmd.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> // 用于sleep函数的

static int SN_MAX = 99999999;
static int sn;
char from_sip[100];
char contact[100];
char to_sip[100];

extern char *server_id;
extern char *server_ip;
extern int server_port;
extern int device_port;
extern char *device_id;
extern char *username;
extern char *password;
extern char *manufacture;
extern char *filepath;
extern Nalu_t *nalu_vector;
extern int nalu_vector_size;
extern int sockfd;

struct eXosip_t* sip_context;
bool is_running=false;
bool is_register=false;
bool is_pushing=false;
char local_ip[24];
pthread_t heartbeat_thread_id;
pthread_t push_thread_id;
rtp_net_t rtp_net;
char ssrc[20];


static int get_sn() {
	if (sn >= SN_MAX) {
		sn = 0;
	}
	sn++;
	return sn;
}

int device_info(){
    printf("device info: \n");
    printf("server sip sid: %s\n", server_id);
    printf("server ip address: %s\n", server_ip);
    printf("server port: %d\n", server_port);
    printf("device sip id: %s\n", device_id);
    printf("device port id: %d\n", device_port);
    printf("username: %s\n", username);
    printf("password: %s\n", password);
    printf("manufacture: %s\n", manufacture);
    printf("sample file path: %s\n", filepath);
};

void *device_push_rtp_stream() {
    is_pushing = true;

    int status = device_bind(rtp_net.listen_port);
    if (status != 0) {
        printf("device bind socket address failed: %d\n", status);
        return 0;
    }

    printf("device bind socket address success\n");

	char ps_header[PS_HDR_LEN];
	char ps_system_header[SYS_HDR_LEN];
	char ps_map_header[PSM_HDR_LEN];
	char pes_header[PES_HDR_LEN];
	char rtp_header[RTP_HDR_LEN];

	int time_base = 90000;
	int fps = 24;
	int send_packet_interval = 1000 / fps;

	int interval = time_base / fps;
	long pts = 0;

	char frame[1024 * 128];

	int single_packet_max_length = 1400;

	char rtp_packet[RTP_HDR_LEN+1400];

	// int ssrc = 0xffffffff;
	int rtp_seq = 0;

    // Nalu *nalu = new Nalu();
	// nalu->packet = (char *)malloc(1024*128);
	// nalu->length = 1024 * 128;

    printf("device pushing steam\n");

    while (is_pushing) {
        for (int i = 0; i < nalu_vector_size; i++) {
            Nalu_t nalu = nalu_vector[i];

            NaluType type = nalu.type;
            int length = nalu.length;
            char * packet = nalu.packet;

            int index = 0;
            if (NALU_TYPE_IDR == type) {
                gb28181_make_ps_header(ps_header, pts);

                memcpy(frame,ps_header,PS_HDR_LEN);
                index += PS_HDR_LEN;

                gb28181_make_sys_header(ps_system_header, 0x3f);

                memcpy(frame+ index, ps_system_header, SYS_HDR_LEN);
                index += SYS_HDR_LEN;

                gb28181_make_psm_header(ps_map_header);

                memcpy(frame + index, ps_map_header, PSM_HDR_LEN);
                index += PSM_HDR_LEN;

            } else {
                gb28181_make_ps_header(ps_header, pts);

                memcpy(frame, ps_header, PS_HDR_LEN);
                index += PS_HDR_LEN;
            }

            //封装pes
            gb28181_make_pes_header(pes_header, 0xe0, length, pts, pts);

            memcpy(frame+index, pes_header, PES_HDR_LEN);
            index += PES_HDR_LEN;

            memcpy(frame + index, packet, length);
            index += length;

            //组包rtp
            int rtp_packet_count = ((index - 1) / single_packet_max_length) + 1;
            for (int i = 0; i < rtp_packet_count; i++) {
                gb28181_make_rtp_header(rtp_header, rtp_seq, pts, atoi(ssrc), i == (rtp_packet_count - 1));
                int writed_count = single_packet_max_length;

                if ((i + 1)*single_packet_max_length > index)
                    writed_count = index - (i* single_packet_max_length);
                //添加包长字节
                int rtp_start_index=0;

                unsigned short rtp_packet_length = RTP_HDR_LEN + writed_count;
                if (rtp_net.rtp_protocol == "TCP/RTP/AVP") {
                    unsigned char packt_length_ary[2];
                    packt_length_ary[0] = (rtp_packet_length >> 8) & 0xff;
                    packt_length_ary[1] = rtp_packet_length & 0xff;
                    memcpy(rtp_packet, packt_length_ary, 2);
                    rtp_start_index = 2;
                }

                memcpy(rtp_packet+ rtp_start_index, rtp_header, RTP_HDR_LEN);
                memcpy(rtp_packet+ +rtp_start_index + RTP_HDR_LEN, frame+ (i* single_packet_max_length), writed_count);
                rtp_seq++;

                if (is_pushing)
                    device_send_network_packet(rtp_packet, rtp_start_index + rtp_packet_length);
                else
                    printf("nothing to do\n");
            }
            pts += interval;
            usleep(send_packet_interval*1000);
        }
    }
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

int device_start(){
    printf("sip init begin\n");
    sip_context = eXosip_malloc();

    if (OSIP_SUCCESS != eXosip_init(sip_context)) {
        printf("sip init failed.\n");
        return -1;
    }

    if (OSIP_SUCCESS != eXosip_listen_addr(sip_context, IPPROTO_UDP, NULL, device_port, AF_INET, 0)) {
        printf("sip port bind failed.\n");
        eXosip_quit(sip_context);
        sip_context = NULL ;
        return -1;
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
        return -1;
    }

    eXosip_lock(sip_context);
	eXosip_register_send_register(sip_context, register_id, register_message);
	eXosip_unlock(sip_context);

    int status = pthread_create(&heartbeat_thread_id, NULL, device_heartbeat_task, NULL);
    if (status != 0) {
        fprintf(stderr, "Error creating thread: %s\n", strerror(status));
        return 1;
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
    mxml_node_t *tree, *node;
    cmd_sn_t result;

    // 初始化结果
    result.cmd_type = NULL;
    result.sn = NULL;
    result.cmd_ptz = NULL;

    tree = mxmlLoadString(NULL,NULL,body);
    if (tree == NULL) {
        fprintf(stderr, "cannot parse the xml\n");
        return (cmd_sn_t){NULL, NULL};
    }

    // 获取根节点
    node = mxmlFindElement(tree, tree, "Query", NULL, NULL, MXML_DESCEND_ALL);
    if (node == NULL) {
        node = mxmlFindElement(tree, tree, "Control", NULL, NULL, MXML_DESCEND_ALL);
        if (node == NULL) {
            fprintf(stderr, "cannot get root node of xml\n");
            mxmlDelete(tree);
            return (cmd_sn_t){NULL, NULL};
        }
    }

    // 检查根节点名称
    const char *root_name = mxmlGetElement(node);
    strcpy(result.cmd_name, root_name);
    if (strcmp(root_name, "Query") == 0) {
        fprintf(stderr, "Query xml with root\n");



    } else if (strcmp(root_name, "Control") == 0) {
        fprintf(stderr, "Control xml with root\n");
        // 获取CmdType节点
        node = mxmlFindElement(node, tree, "PTZCmd", NULL, NULL, MXML_DESCEND_ALL);
        if (node == NULL) {
            fprintf(stderr, "cannot get the PTZCmd\n");
            mxmlDelete(tree);
            return result;
        }
        result.cmd_ptz = strdup(mxmlGetText(node, NULL));
        printf("get the cmd_ptz %s\n", result.cmd_ptz  );
    } else {
        fprintf(stderr, "invalid xml with root\n");
        mxmlDelete(tree);
        return (cmd_sn_t){NULL, NULL};
    }

    // 获取CmdType节点
    node = mxmlFindElement(tree, tree, "CmdType", NULL, NULL, MXML_DESCEND_ALL);
    if (node == NULL) {
        fprintf(stderr, "cannot get the cmd type\n");
        mxmlDelete(tree);
        return result;
    }
    result.cmd_type = strdup(mxmlGetText(node, NULL));
    printf("get the cmd type %s\n",result.cmd_type);

    // 获取SN节点
    node = mxmlFindElement(tree, tree, "SN", NULL, NULL, MXML_DESCEND_ALL);
    if (node == NULL) {
        fprintf(stderr, "cannot get the SN\n");
        mxmlDelete(tree);
        return result;
    }
    result.sn = strdup(mxmlGetText(node, NULL));
    printf("get the SN %s\n",result.sn);

    mxmlDelete(tree);
    return result;
}

void device_process_request() {
    uint64_t i=0;
    while (is_running) {
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
            if (MSG_IS_MESSAGE(evt->request)) {
                osip_body_t * body = NULL;
                osip_message_get_body(evt->request, 0, &body);
                if (body != NULL) {
                    printf("new message request:\n%s\n", body->body);
                }

                device_send_response_ok(evt);

                cmd_sn_t cmd_sn = device_get_cmd(body->body);
                printf("got new cmd: %s sn: %s\n", cmd_sn.cmd_type, cmd_sn.sn);
                printf("got new name: %s ptz: %s\n", cmd_sn.cmd_name, cmd_sn.cmd_ptz);
                if (strcmp(cmd_sn.cmd_name, "Query") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "Catalog") == 0) {
                        device_process_catalog_query(cmd_sn.sn);
                    }
                    else if (strcmp(cmd_sn.cmd_type, "DeviceStatus") == 0) {
                        device_process_devicestatus_query(cmd_sn.sn);
                    }
                    else if (strcmp(cmd_sn.cmd_type, "DeviceInfo") == 0) {
                        device_process_deviceinfo_query(cmd_sn.sn);
                    }
                    else if (strcmp(cmd_sn.cmd_type, "DeviceControl") == 0) {
                        device_process_devicecontrol_query(cmd_sn.sn);
                    }
                    else {
                        printf("unhandled cmd: %s\n", cmd_sn.cmd_type);
                    }
                }
                else if (strcmp(cmd_sn.cmd_name, "Control") == 0) {
                    if (strcmp(cmd_sn.cmd_type, "DeviceControl") == 0) {
                        device_process_devicecontrol_control(cmd_sn.cmd_ptz);
                    } else {
                        printf("unhandled control cmd: %s\n", cmd_sn.cmd_type);
                    }
                }
                else {
                    printf("not get cmd_name\n");
                }
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
            if(is_pushing == 1){
                printf("Already pushing\n");
            }else {
                printf("got CALL_ACK: begin pushing rtp stream...\n");
                int status = pthread_create(&push_thread_id, NULL, device_push_rtp_stream, NULL);
                if (status != 0) {
                    fprintf(stderr, "Error creating thread: %s\n", strerror(status));
                    return ;
                }
            }
            printf("is_pushing = %d\n",is_pushing);
            break;
        }
        case EXOSIP_CALL_CLOSED: {
            printf("got CALL_CLOSED: stop pushing rtp stream...\n");
            pthread_cancel(push_thread_id);
            close(sockfd);
            is_pushing=false;
            break;
        }
        case EXOSIP_MESSAGE_ANSWERED: {
            printf("got MESSAGE_ANSWERED: unhandled\n");
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
}

void device_process_devicecontrol_query(char *sn) {

}

void device_process_devicecontrol_control(char *ptz) {
    PtzCommand cmd;
    parseCmdString(ptz, &cmd);
}
