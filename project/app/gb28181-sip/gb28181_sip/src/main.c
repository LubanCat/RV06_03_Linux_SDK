#include "device.h"
#include "eXosip2/eXosip.h"
#include "nalu.h"

// char *server_id = "41010500002000000001";
// char *server_ip = "192.168.103.8";
// int server_port = 5060;
// int device_port = 5064;
// char *device_id = "41010500002000000005";
// char *username = "41010500002000000005";
// char *password = "12345678";
// char *manufacture = "lbc-ipc";
// char *filepath = "samples/bigbuckbunny_1280x720_24fps_annexb_012.h264";


// char *server_id = "41010500002000000001";
// char *server_ip = "8.137.12.122";
// int server_port = 8116;
// int device_port = 5064;
// char *device_id = "41010500002000000005";
// char *username = "41010500002000000005";
// char *password = "newings2024";
// char *manufacture = "lbc-ipc";
// char *filepath = "samples/bigbuckbunny_1280x720_24fps_annexb_012.h264";

char *server_id = "41010500002000000001";
char *server_ip = "192.168.103.37";
int server_port = 5060;
int device_port = 5064;
char *device_id = "41010500002000000002";
char *username = "lbc-ipc";
char *password = "12345678";
char *manufacture = "lbc-ipc";
char *filepath = "/oem/usr/share/0.h264";





int main(int argc, const char* argv[]) {
    device_info();
    load(filepath);
    device_start();
}
