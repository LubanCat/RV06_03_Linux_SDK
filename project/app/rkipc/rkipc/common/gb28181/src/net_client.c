#include "device.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern rtp_net_t rtp_net;
extern char local_ip[24];
int rtp_sockfd;

int device_bind(int listen_port) {

    if (strcmp(rtp_net.rtp_protocol, "TCP/RTP/AVP") == 0) {
        rtp_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
        rtp_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    printf("rtp_sockfd = %d\n",rtp_sockfd);
    struct sockaddr_in bind_address;
    bind_address.sin_family = AF_INET;
    bind_address.sin_port = htons(listen_port);
    bind_address.sin_addr.s_addr = inet_addr(local_ip);

    int bind_status = bind(rtp_sockfd, (struct sockaddr *)&bind_address, sizeof(bind_address));
    printf("local_port %d\n",rtp_net.rtp_port);
    printf("port %d\n",bind_address.sin_port);
    printf("local_ip %s\n",local_ip);
    printf("s_addr %d\n",bind_address.sin_addr.s_addr);
    if (bind_status) {
        printf("socket bind failed: %d\n", bind_status);
        return bind_status;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(rtp_net.rtp_port);
    inet_pton(AF_INET, rtp_net.rtp_ip, &server_address.sin_addr);

    int connect_status = connect(rtp_sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if ( connect_status < 0) {
        printf("socket connect failed: %d\n", connect_status);
        return connect_status;
    }

    return 0;
}

void device_send_network_packet(const char * data, int length) {
    if (strcmp(rtp_net.rtp_protocol, "TCP/RTP/AVP") == 0) {
        send(rtp_sockfd, data, length, 0);
    } else {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(rtp_net.rtp_port);
        inet_pton(AF_INET, rtp_net.rtp_ip, &server_address.sin_addr);
        sendto(rtp_sockfd, data, length, 0, (struct sockaddr *)&server_address, sizeof(server_address));
    }
}