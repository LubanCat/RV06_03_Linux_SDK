#include "device.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern rtp_net_t rtp_net;
extern char local_ip[24];
int sockfd;

int device_bind(int listen_port) {

    if (rtp_net.rtp_protocol == "TCP/RTP/AVP") {
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    printf("sockfd = %d\n",sockfd);
    struct sockaddr_in bind_address;
    bind_address.sin_family = AF_INET;
    bind_address.sin_port = htons(listen_port);
    bind_address.sin_addr.s_addr = inet_addr(local_ip);

    int bind_status = bind(sockfd, (struct sockaddr *)&bind_address, sizeof(bind_address));
    if (bind_status) {
        printf("socket bind failed: %d\n", bind_status);
        return bind_status;
    }
    printf("local_port %d\n",rtp_net.rtp_port);
    printf("port %d\n",bind_address.sin_port);
    printf("local_ip %s\n",local_ip);
    printf("s_addr %d\n",bind_address.sin_addr.s_addr);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(rtp_net.rtp_port);
    inet_pton(AF_INET, rtp_net.rtp_ip, &server_address.sin_addr);

    int connect_status = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if ( connect_status < 0) {
        printf("socket connect failed: %d\n", connect_status);
        return connect_status;
    }

    return 0;
}

void device_send_network_packet(const char * data, int length) {
    if (rtp_net.rtp_protocol == "TCP/RTP/AVP") {
        send(sockfd, data, length, 0);
    } else {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(rtp_net.rtp_port);
        inet_pton(AF_INET, rtp_net.rtp_ip, &server_address.sin_addr);
        sendto(sockfd, data, length, 0, (struct sockaddr *)&server_address, sizeof(server_address));
    }
}