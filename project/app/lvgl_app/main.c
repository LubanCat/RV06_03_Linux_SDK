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

#define DISP_BUF_SIZE (600 * 1024 * 4)
#define MAX_WEIGHT 200
#define MSGSZ 128

lv_ui guider_ui;
extern int sock;
lv_disp_drv_t disp_drv;
volatile sig_atomic_t g_main_run_ = 1; // 使用 volatile sig_atomic_t 类型，确保在信号处理程序中正确更新
int msqid;  // 消息队列ID
int password_1 = 0, password_2 = 0, password_3 = 0;
pthread_mutex_t lv_task_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_connect;
extern char cleantime_str[100];

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

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
        usleep(5000);
    }
    printf("exit lv_task_thread");
    return NULL;
}

// 辅助线程
void* aux_task_thread(void* arg) {
    int signal_count = 0;
    int light_count = 0;
    int overflow_count = 0;
    int current_light_state = 0;
    int curCapacity,curCapacityBefore;
    loginRes.turnOnLight.tm_hour = 19;
    loginRes.turnOnLight.tm_min = 0;
    loginRes.turnOffLight.tm_hour = 7;
    loginRes.turnOffLight.tm_min = 0;

    while (g_main_run_) {
        // LVGL 更新4G信号强度
        if (signal_count == 10) {
            heartBeat.signal = module_signal();
            if(screen_login_flag == 1){
                // 在登录页面才更新
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_login_net_status(heartBeat.signal);
                pthread_mutex_unlock(&lv_task_mutex);
            }
            signal_count = 0;
        }

        // 灯光更新
        if (light_count == 5) {
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

        if (overflow_count == 1) {
            // 获取超重、满溢
            int curCapacity = get_overflow_status();
            if (curCapacity == 3){
                // LVGL_UI 回收柜已满
                if(screen_login_flag == 1){
                    // 在登录页面才更新
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_login_full(1);
                    pthread_mutex_unlock(&lv_task_mutex);
                }
            }else{
                if(screen_login_flag == 1){
                    // 在登录页面才更新
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_login_full(0);
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

        sleep(1);

        // 触屏 进入配置模式
        // if (password_1 && password_2 && password_3){
        //     screen_login_flag=0;
        //     set_screen_test(&guider_ui.screen_login_del);
        //     password_1 = 0;
        //     password_2 = 0;
        //     password_3 = 0;
        // }

        overflow_count++;
        signal_count++;
        light_count++;
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

    // 同步时间
    system("ntpdate ntp1.aliyun.com");

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

    set_screen_login(&guider_ui.blank_del, loginRes.recyclePrice, get_current_weight(), cleantime_str, loginRes.qrCode, heartBeat.signal,server_connect);

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
        // 在循环中处理消息队列，避免被信号中断导致的错误
        message_buf rbuf;
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, IPC_NOWAIT) < 0) {
            if (errno != ENOMSG && errno != EINTR) {
                perror("msgrcv");
                break;
            }
        } else {
            printf("Received message: %s\n", rbuf.mtext);
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
