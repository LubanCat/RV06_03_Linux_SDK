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
#include "lv_conf.h"
#include "server.h"
#include "module.h"

#include "generated/gui_guider.h"
#include "generated/events_init.h"

#define DISP_BUF_SIZE (480 * 480)

lv_ui guider_ui;

int main(void)
{
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

    /*Create a Demo*/
    // lv_demo_benchmark_set_max_speed(true);
    // lv_demo_benchmark();
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_keypad_encoder();
    // lv_demo_music();

    /* ui_init(); */
    setup_ui(&guider_ui);
    events_init(&guider_ui);

    module_init();
    module_info();

    server();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5000);
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
