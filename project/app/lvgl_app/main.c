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
#include <cjson/cJSON.h>
#include "lv_conf.h"
#include "server.h"
#include "module.h"
#include "hardware.h"
#include "custom.h"
#include "generated/gui_guider.h"
#include "generated/events_init.h"

#define DISP_BUF_SIZE (480 * 480)
#define MAX_WEIGHT 20

lv_ui guider_ui;
extern int sock;

void* lv_task_thread(void* arg) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 5 * 1000000; // 5毫秒

    while (1) {
        lv_task_handler();
        nanosleep(&ts, NULL);
    }
    return NULL;
}

// 辅助线程
void* aux_task_thread(void* arg) {
    int signal_count = 0;
    int light_count = 0;
    int overflow_count = 0;
    loginRes.turnOnLight.tm_hour = 19;
    loginRes.turnOnLight.tm_min = 0;
    loginRes.turnOffLight.tm_hour = 7;
    loginRes.turnOffLight.tm_min = 0;

    while (1) {
        // LVGL 更新4G信号强度
        if (signal_count == 5) {
            int net_status;
            heartBeat.signal = module_signal();
            if (heartBeat.signal == 99) {
                net_status = 0;
            } else {
                net_status = 1;
            }

            set_screen_login_net_status(net_status,heartBeat.signal);
            signal_count = 0;
        }

        // 灯光更新
        if (light_count == 60) {
            time_t t = time(NULL);
            struct tm* now = localtime(&t);

            int now_minutes = now->tm_hour * 60 + now->tm_min;
            int start_minutes = loginRes.turnOnLight.tm_hour * 60 + loginRes.turnOnLight.tm_min;
            int end_minutes = loginRes.turnOffLight.tm_hour * 60 + loginRes.turnOffLight.tm_min;

            if (start_minutes < end_minutes) {
                // 不跨午夜
                if (now_minutes >= start_minutes && now_minutes < end_minutes) {
                    outside_light(1);
                } else {
                    outside_light(0);
                }
            } else {
                // 跨午夜
                if (now_minutes >= start_minutes || now_minutes < end_minutes) {
                    outside_light(1);
                } else {
                    outside_light(0);
                }
            }
            light_count = 0;
        }

        if (overflow_count == 90) {
            // 获取超重、满溢 
            int curCapacity = get_overflow_status();
            if (curCapacity == 3){
                upload_overflow(curCapacity);
            }

            int cur_weight = get_current_weight();
            if (cur_weight >= MAX_WEIGHT){
                upload_overweight();
            }
            overflow_count = 0;
        }

        sleep(1);

        overflow_count++;
        signal_count++;
        light_count++;
    }
    return NULL;
}

int main(void)
{
    pthread_t lv_task_thread_id;
    pthread_t aux_task_thread_id;
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
    static lv_disp_drv_t disp_drv;
    uint32_t dpi = 0;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = drm_flush;
    disp_drv.wait_cb    = drm_wait_vsync;
    /* disp_drv.sw_rotate = 1; */
    /* disp_drv.rotated = LV_DISP_ROT_90; */
    drm_get_sizes(&disp_drv.hor_res, &disp_drv.ver_res, &dpi);
    lv_disp_drv_register(&disp_drv);

    /* Linux input device init */
    evdev_init();

    /* Initialize and register a display input driver */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = evdev_read;   //lv_gesture_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev)
    lv_indev_drv_register(&indev_drv);

    // 获取4G模块信息
    module_init();
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

    ret = pthread_create(&aux_task_thread_id, NULL, aux_task_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    server_start();

    set_screen_login(&guider_ui.blank_del, loginRes.recyclePrice, get_current_weight(), "2024/06/27 15:35:99", loginRes.qrCode, 1, heartBeat.signal);

    int curCapacity = get_overflow_status();
    if (curCapacity == 3){
        upload_overflow(curCapacity);
        // LVGL_UI 回收柜已满
    }

    while(1){
        open_door(curCapacity);
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
