#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/drm.h"
#include "lv_app/src/ui.h"
#include "lv_drivers/indev/evdev.h"
#include "lvgl/demos/stress/lv_demo_stress.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <cjson/cJSON.h>
#include "lv_conf.h"
#include "server.h"
#include "module.h"
#include "hardware.h"
#include "custom.h"
#include "generated/gui_guider.h"
#include "generated/events_init.h"
#include "capture.h"
#include <sys/stat.h>

#define DISP_BUF_SIZE (600 * 1024 * 4)
#define MAX_WEIGHT 200
#define MAX_BUF_SIZE 512

extern char trans_Id[64];

// 登录界面的4G信号更新时间 单位：s
#define SIGNAL_UPDATE_TIME 10
// 登录界面的服务器连接更新时间 单位：s
#define SERVER_UPDATE_TIME 5
// 灯光更新时间 单位：s
#define LIGHT_UPDATE_TIME 5
// 溢出更新时间 单位：s
#define OVERFLOW_UPDATE_TIME 2
// 读4G信号错误的时间 单位：次数 时间=SIGNAL_UPDATE_TIME * 次数
#define SIGNAL_ERR_COUNT 5

#define NTP_UPDATE_COUNT 86400
#define CHECK_OPENDOOR_COUNT 30

lv_ui guider_ui;
extern int sock;
lv_disp_drv_t disp_drv;
volatile sig_atomic_t g_main_run_ = 1; // 使用 volatile sig_atomic_t 类型，确保在信号处理程序中正确更新
int password_1 = 0, password_2 = 0, password_3 = 0;
pthread_mutex_t lv_task_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_connect;
extern char cleantime_str[100];
extern int updating_flag;
extern pthread_mutex_t camara_mutex;

extern video_device_t out_video_device;
extern video_device_t in_video_device;

char key_input[5];
int key_input_count;

int overflow_flag=0;

int phone_login_flag=0;
int phone_login_update=0;

char phone_number[12];
int phone_number_count;

int ntp_update(){
    system("ntpdate 114.118.7.161");
    system("ntpdate ntp1.aliyun.com");
}

extern pthread_t opendoor_thread_id;

void sig_proc(int signo) {
    if (signo == SIGINT || signo == SIGTERM) {
        printf("received signal %d\n", signo);
        g_main_run_ = 0; // 将运行标志设置为 0，以便退出 while 循环
    }
}

void* lv_task_thread(void* arg) {

    while (g_main_run_) {
        pthread_mutex_lock(&lv_task_mutex);
        lv_task_handler();
        pthread_mutex_unlock(&lv_task_mutex);
        usleep(1000);
    }
    printf("exit lv_task_thread");
    return NULL;
}


void* keyboard_thread(void* arg) {
    char input[5];
    int input_count;
    int show=0;
    while(g_main_run_){
        // 进入登录界面
        if(phone_login_flag && screen_login_flag && !overflow_flag){
            // 判断有没有加载键盘
            if(phone_login_update){
                if(key_input_count > 0){
                    memcpy(input,key_input,5);
                    input_count=key_input_count;
                    key_input_count=0;
                    for(int i=0;i<input_count;i++){
                        if(key_input[i] >= '0' && key_input[i] <= '9'){
                            if(phone_number_count<11){
                                phone_number[phone_number_count]=key_input[i];
                                phone_number_count++;
                                phone_number[phone_number_count]='\0';
                            }else
                                printf("phone number is full\n");
                        }//enter
                        else if(key_input[i] == 0x11){
                            if(phone_number_count!=11){
                                printf("phone_number_count is wrong:%d\n",phone_number_count);
                            }//通过
                            else{
                                printf("allow to login %s\n",phone_number);
                                allow_phone_login(phone_number);
                            }
                        }//del
                        else if(key_input[i] == 0x13){
                            if(phone_number_count>0){
                                phone_number_count--;
                                phone_number[phone_number_count]='\0';
                            }
                        }//exit
                        else if(key_input[i] == 0x12){
                            printf("press key_exit\n");
                            phone_login_flag=0;
                            phone_login_update=0;
                            phone_number_count=0;
                            memset(phone_number,0,sizeof(phone_number));
                            key_input_count=0;
                            memset(key_input,0,sizeof(key_input));
                            usleep(100*1000);

                            pthread_mutex_lock(&lv_task_mutex);
                            show=0;
                            lv_obj_add_flag(guider_ui.screen_login_keyboard, LV_OBJ_FLAG_HIDDEN);
                            pthread_mutex_unlock(&lv_task_mutex);
                            break;
                        }
                    }
                    // usleep(1000 * 1000);
                    // printf("phone_number:%s\n",phone_number);
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_login_phone_number(phone_number,phone_number_count);
                    pthread_mutex_unlock(&lv_task_mutex);
                }
 
                
                //更新显示数据
                // printf("phone_number:%s\n",phone_number);
            }else{
                phone_login_update=1;
                usleep(300*1000);
                phone_number_count=0;
                phone_number[0]='\0';
                printf("open key_board\n");
                pthread_mutex_lock(&lv_task_mutex);
                lv_obj_clear_flag(guider_ui.screen_login_keyboard, LV_OBJ_FLAG_HIDDEN);
                show=1;
                pthread_mutex_unlock(&lv_task_mutex);
            }
        }
        else{
            phone_login_flag=0;
            phone_login_update=0;
            phone_number_count=0;

            phone_number[0]='\0';
            if(show){
                printf("clear key_board\n");
                pthread_mutex_lock(&lv_task_mutex);
                lv_obj_add_flag(guider_ui.screen_login_keyboard, LV_OBJ_FLAG_HIDDEN);
                show=0;
                pthread_mutex_unlock(&lv_task_mutex);
            }
        }
        usleep(100 * 1000);
    }
}

int is_file_exist(char *filename){
    struct stat buffer;
    if (stat(filename, &buffer) == 0)
        return 0;
    else
        if (errno == ENOENT)
            return -1;
        else 
            return -1;
}

void upload_close_door(char *time){
    char in_video_open_path[64];
    char out_video_open_path[64];
    char in_video_close_path[64];
    char out_video_close_path[64];
    char time_path[32];
    char *media_path="/root/media";
    char device[64];
    sprintf(time_path,"%s/%s",media_path,time);
    sprintf(in_video_open_path,"%s/in_video_open.jpg",time_path);
    sprintf(out_video_open_path,"%s/out_video_open.jpg",time_path);
    sprintf(in_video_close_path,"%s/in_video_close.jpg",time_path);
    sprintf(out_video_close_path,"%s/out_video_close.jpg",time_path);

    if(is_file_exist(in_video_close_path) == -1){
        get_cam_video(0,device);
        capture_jpg_compass(device,10,640,480,in_video_close_path);
    }

    if(is_file_exist(out_video_close_path) == -1){
        get_cam_video(1,device);
        capture_jpg_compass(device,10,640,480,out_video_close_path);
    }

    upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_open_path);
    upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_open_path);
    upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_close_path);
    upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_close_path);
}   
// 辅助线程
void* aux_task_thread(void* arg) {

    int signal_err_count = 0;
    int signal_count = 0;
    int light_count = 0;
    int server_count = 0;
    int overflow_count = 0;
    int opendoor_count=0;
    int current_light_state = 0;
    int curCapacity,curCapacityBefore;
    long ntp_count=0;
    pthread_t module_restart_thread_id;
    loginRes.turnOnLight.tm_hour = 19;
    loginRes.turnOnLight.tm_min = 0;
    loginRes.turnOffLight.tm_hour = 7;
    loginRes.turnOffLight.tm_min = 0;

    while (g_main_run_) {
        // LVGL 更新4G信号强度
        if (signal_count >= SIGNAL_UPDATE_TIME) {
            heartBeat.signal = module_signal();
            if (heartBeat.signal == 99){
                signal_err_count++;
                // 连续读到SIGNAL_ERR_COUNT次后
                if(signal_err_count > SIGNAL_ERR_COUNT){
                    // 重启4G网卡
                    printf("need to reboot net card\n");
                    module_restart();
                    signal_err_count = 0;
                }
            }else{
                signal_err_count = 0;
            }

            if(screen_login_flag){
                // 在登录页面才更新
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_login_net_status(heartBeat.signal);
                pthread_mutex_unlock(&lv_task_mutex);
            }
            signal_count = 0;
        }
        // LVGL 更新服务器连接状态
        if (server_count >= SERVER_UPDATE_TIME) {
            if(screen_login_flag){
                // 在登录页面才更新
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_login_server_status(server_connect);
                pthread_mutex_unlock(&lv_task_mutex);
            }
            server_count=0;
        }

        // 灯光更新
        if (light_count >= LIGHT_UPDATE_TIME) {
            time_t t = time(NULL);
            struct tm* now = localtime(&t);

            int now_minutes = now->tm_hour * 60 + now->tm_min;
            int start_minutes = loginRes.turnOnLight.tm_hour * 60 + loginRes.turnOnLight.tm_min;
            int end_minutes = loginRes.turnOffLight.tm_hour * 60 + loginRes.turnOffLight.tm_min;

            int new_light_state = 0;

            if (start_minutes < end_minutes) {
                // 不跨午夜
                if (now_minutes >= start_minutes && now_minutes < end_minutes) {
                    new_light_state = 1;
                }
            } else {
                // 跨午夜
                if (now_minutes >= start_minutes || now_minutes < end_minutes) {
                    new_light_state = 1;
                }
            }

            // 只有当灯光状态发生变化时才更新灯光
            if (current_light_state != new_light_state) {
                outside_light(new_light_state);
                current_light_state = new_light_state;
            }

            // 重置灯光计数
            light_count = 0;
        }

        if(updating_flag){
            if(screen_login_flag){
                pthread_mutex_lock(&lv_task_mutex);
                lv_obj_clear_flag(guider_ui.screen_login_updating, LV_OBJ_FLAG_HIDDEN);
                pthread_mutex_unlock(&lv_task_mutex);
            }
        }else{
            if(screen_login_flag){
                pthread_mutex_lock(&lv_task_mutex);
                lv_obj_add_flag(guider_ui.screen_login_updating, LV_OBJ_FLAG_HIDDEN);
                pthread_mutex_unlock(&lv_task_mutex);
            }   
        }

        if (overflow_count >= OVERFLOW_UPDATE_TIME) {
            // 获取超重、满溢
            int curCapacity = get_overflow_status();
            if (curCapacity == 3){
                // LVGL_UI 回收柜已满
                if(screen_login_flag == 1){
                    // 在登录页面才更新
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_login_full(1);
                    overflow_flag=1;
                    pthread_mutex_unlock(&lv_task_mutex);
                }
            }else{
                if(screen_login_flag == 1){
                    // 在登录页面才更新
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_login_full(0);
                    overflow_flag=0;
                    pthread_mutex_unlock(&lv_task_mutex);
                }
            }
                  
            if (curCapacity != curCapacityBefore) {
                upload_overflow(curCapacity);
            }

            // int cur_weight = get_current_weight();
            // upload_overweight(cur_weight);

            curCapacityBefore = curCapacity;
            overflow_count = 0;
        }

        if(ntp_count > NTP_UPDATE_COUNT){
            ntp_update();
            ntp_count=0;
        }
        
        if(opendoor_count > CHECK_OPENDOOR_COUNT){
            opendoor_count=0;
            printf("time is ok\n");
            if(opendoor_thread_id == NULL){
                printf("Enter\n");
                opendoor_thread_id=1;
                char *filename = "/root/opendoor.txt"; // 你要读取的文件名
                FILE *file = fopen(filename, "r");
                char buffer[20];
                char time[24];
                int size;
                if (file != NULL) {
                    fgets(trans_Id, sizeof(trans_Id), file);
                    trans_Id[strlen(trans_Id)-1]='\0';
                    fgets(time, sizeof(time), file);
                    time[strlen(time)-1]='\0';
                    fgets(buffer, sizeof(buffer), file);
                    buffer[strlen(buffer)-1]='\0';
                    char *endptr;
                    double sub_weight = strtod(buffer, &endptr);
                    printf("time:%s,trans_Id:%s,weight=%f\n",time,trans_Id,sub_weight);
                    
                    double cur_weight= get_current_weight();
                    if(cur_weight < 0)
                        cur_weight=0.0;
                    double diff_weight = round((cur_weight - sub_weight) * 100) / 100;
                    close_door(cur_weight, diff_weight);
                    fclose(file);
                    if (remove(filename) == 0) {
                        printf("文件 '%s' 删除成功。\n", filename);
                    } else {
                        perror("删除文件失败");
                    }

                    upload_close_door(time);
                    opendoor_thread_id=NULL;
                }
                opendoor_thread_id=NULL;
                printf("Quit\n");
            }
        }

        sleep(1);

        // 触屏 进入配置模式
        // if (password_1 && password_2 && password_3){
        //     screen_login_flag=0;
        //     set_screen_test(&guider_ui.screen_login_del);
        //     password_1 = 0;
        //     password_2 = 0;
        //     password_3 = 0;
        // }
        // printf("-\n");
        overflow_count++;
        signal_count++;
        light_count++;
        server_count++;
        ntp_count++;
        opendoor_count++;
    }
    printf("exit aux_task_thread");
    return NULL;
}

int main(void) {
    pthread_t lv_task_thread_id;
    pthread_t aux_task_thread_id;
    pthread_t socket_task_thread_id;
    int ret;

    signal(SIGINT, sig_proc);
    signal(SIGTERM, sig_proc);

    /*LittlevGL init*/
    lv_init();

    /*Linux DRM device init*/
    drm_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    uint32_t dpi = 0;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = drm_flush;
    disp_drv.wait_cb = drm_wait_vsync;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    drm_get_sizes(&disp_drv.hor_res, &disp_drv.ver_res, &dpi);
    lv_disp_drv_register(&disp_drv);

    /* Linux input device init */
    evdev_init();

    /* Initialize and register a display input driver */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv);

    // 同步时间
    ntp_update();
    // 获取4G模块信息

    module_info();
    heartBeat.signal = module_signal();

    /* ui_init(); */
    setup_ui(&guider_ui);
    events_init(&guider_ui);

    ret = pthread_create(&lv_task_thread_id, NULL, lv_task_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // 读本地配置文件
    read_info();
     // 初始化引脚
    recycle_gpio_init();   

    //server_start();
    ret = pthread_create(&socket_task_thread_id, NULL, socket_task_thread, NULL);
    if (ret != 0)
        {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    while(!server_connect){
        sleep(1);
    }

    ret = pthread_create(&aux_task_thread_id, NULL, aux_task_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    pthread_t keyboard_thread_id;
    ret = pthread_create(&keyboard_thread_id, NULL, keyboard_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    cam_init();
    pthread_mutex_lock(&lv_task_mutex);
    set_screen_login(&guider_ui.blank_del, loginRes.recyclePrice, get_current_weight(), cleantime_str, loginRes.qrCode, heartBeat.signal,server_connect);
    pthread_mutex_unlock(&lv_task_mutex);
    
    int curCapacity = get_overflow_status();
    upload_overflow(curCapacity);
    if (curCapacity == 3) {
        // LVGL_UI 回收柜已满
        pthread_mutex_lock(&lv_task_mutex);
        set_screen_login_full(1);
        pthread_mutex_unlock(&lv_task_mutex);
    } else {
        pthread_mutex_lock(&lv_task_mutex);
        set_screen_login_full(0);
        pthread_mutex_unlock(&lv_task_mutex);
    }

    while (g_main_run_) {
        while (server_connect) 
        {
            char recv_buf[MAX_BUF_SIZE+1] = {0};
            int recv_len;
    
            recv_len = recv(sock, recv_buf, MAX_BUF_SIZE, 0);
            if (recv_len < 0)
                continue;

            printf("sock = %d, recv_len = %d\n", sock, recv_len);
            if (recv_len > 0){
                printf("Server response: %s\n", recv_buf);
    
                // 调用消息处理函数
                message_receiver_handler(recv_buf);
            }
            else if (recv_len == 0){
                // To do ...
                printf("----------close socket, reconnect--- \n");
                
                close(sock);
                sock = -1;
                server_connect = 0;
            }
        }
    }

    // deinit
    pthread_mutex_destroy(&lv_task_mutex);
    pthread_join(lv_task_thread_id, NULL);
    pthread_join(aux_task_thread_id, NULL);

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
