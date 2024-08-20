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
#include "generated/gui_guider.h"
#include "generated/events_init.h"

#define DISP_BUF_SIZE (600 * 1024 * 4)
#define MAX_WEIGHT 200
#define MSGSZ 128

// 登录界面的4G信号更新时间 单位：s
#define SIGNAL_UPDATE_TIME 10
// 登录界面的服务器连接更新时间 单位：s
#define SERVER_UPDATE_TIME 5
// 灯光更新时间 单位：s
#define LIGHT_UPDATE_TIME 5
// 溢出更新时间 单位：s
#define OVERFLOW_UPDATE_TIME 1
// 读4G信号错误的时间 单位：次数 时间=SIGNAL_UPDATE_TIME * 次数
#define SIGNAL_ERR_COUNT 5

lv_ui guider_ui;
extern int sock;
lv_disp_drv_t disp_drv;
volatile sig_atomic_t g_main_run_ = 1; // 使用 volatile sig_atomic_t 类型，确保在信号处理程序中正确更新
int msqid;  // 消息队列ID
int password_1 = 0, password_2 = 0, password_3 = 0;
pthread_mutex_t lv_task_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_connect;
extern char cleantime_str[100];

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


int main(void) {
    pthread_t lv_task_thread_id;
    pthread_t aux_task_thread_id;
    pthread_t socket_task_thread_id;
    int ret;

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
    // disp_drv.sw_rotate = 1;
    // disp_drv.rotated = LV_DISP_ROT_90;
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

    //用于自定义的lvgl程序
    // setup_ui(&guider_ui);
    // events_init(&guider_ui);

    //用于demo
    // lv_demo_benchmark_set_max_speed(true);
    // lv_demo_benchmark();
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_keypad_encoder();
    lv_demo_music();

    ret = pthread_create(&lv_task_thread_id, NULL, lv_task_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    while(1){
        usleep(500000);
    }

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
