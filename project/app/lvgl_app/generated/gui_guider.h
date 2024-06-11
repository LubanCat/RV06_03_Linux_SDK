/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	lv_obj_t *screen_tileview_main;
	lv_obj_t *screen_tileview_main_tileview;
	lv_obj_t *screen_digital_clock;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_imgbtn_5;
	lv_obj_t *screen_imgbtn_5_label;
	lv_obj_t *screen_imgbtn_play;
	lv_obj_t *screen_imgbtn_play_label;
	lv_obj_t *screen_imgbtn_pre;
	lv_obj_t *screen_imgbtn_pre_label;
	lv_obj_t *screen_imgbtn_2;
	lv_obj_t *screen_imgbtn_2_label;
	lv_obj_t *screen_imgbtn_1;
	lv_obj_t *screen_imgbtn_1_label;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_slider_main;
	lv_obj_t *screen_imgbtn_weather;
	lv_obj_t *screen_imgbtn_weather_label;
	lv_obj_t *screen_tileview_main_NetWork;
	lv_obj_t *screen_imgbtn_6;
	lv_obj_t *screen_imgbtn_6_label;
	lv_obj_t *screen_label_kbs;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_sw_vpn;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_cpu;
	lv_obj_t *screen_chart_load;
	lv_obj_t *screen_arc_mem;
	lv_obj_t *screen_arc_cpu;
	lv_obj_t *screen_imgbtn_8;
	lv_obj_t *screen_imgbtn_8_label;
	lv_obj_t *screen_imgbtn_7;
	lv_obj_t *screen_imgbtn_7_label;
	lv_obj_t *screen_line_div;
	lv_obj_t *screen_tileview_main_Title;
	lv_obj_t *screen_animimg_1;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
void clock_count_12(int *hour, int *min, int *sec, char *meridiem);
void clock_count_24(int *hour, int *min, int *sec);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_animimg_1taikongren_00)
LV_IMG_DECLARE(screen_animimg_1taikongren_01)
LV_IMG_DECLARE(screen_animimg_1taikongren_02)
LV_IMG_DECLARE(screen_animimg_1taikongren_03)
LV_IMG_DECLARE(screen_animimg_1taikongren_04)
LV_IMG_DECLARE(screen_animimg_1taikongren_05)
LV_IMG_DECLARE(screen_animimg_1taikongren_06)
LV_IMG_DECLARE(screen_animimg_1taikongren_07)
LV_IMG_DECLARE(screen_animimg_1taikongren_08)
LV_IMG_DECLARE(screen_animimg_1taikongren_09)
LV_IMG_DECLARE(screen_animimg_1taikongren_10)
LV_IMG_DECLARE(screen_animimg_1taikongren_11)
LV_IMG_DECLARE(screen_animimg_1taikongren_12)
LV_IMG_DECLARE(screen_animimg_1taikongren_13)
LV_IMG_DECLARE(screen_animimg_1taikongren_14)
LV_IMG_DECLARE(screen_animimg_1taikongren_15)
LV_IMG_DECLARE(screen_animimg_1taikongren_16)
LV_IMG_DECLARE(screen_animimg_1taikongren_17)
LV_IMG_DECLARE(screen_animimg_1taikongren_18)
LV_IMG_DECLARE(screen_animimg_1taikongren_19)
LV_IMG_DECLARE(screen_animimg_1taikongren_20)
LV_IMG_DECLARE(screen_animimg_1taikongren_21)
LV_IMG_DECLARE(screen_animimg_1taikongren_22)
LV_IMG_DECLARE(screen_animimg_1taikongren_23)
LV_IMG_DECLARE(screen_animimg_1taikongren_24)
LV_IMG_DECLARE(screen_animimg_1taikongren_25)
LV_IMG_DECLARE(screen_animimg_1taikongren_26)
LV_IMG_DECLARE(screen_animimg_1taikongren_27)
LV_IMG_DECLARE(screen_animimg_1taikongren_28)
LV_IMG_DECLARE(screen_animimg_1taikongren_29)
LV_IMG_DECLARE(screen_animimg_1taikongren_30)
LV_IMG_DECLARE(screen_animimg_1taikongren_31)
LV_IMG_DECLARE(screen_animimg_1taikongren_32)
LV_IMG_DECLARE(screen_animimg_1taikongren_33)
LV_IMG_DECLARE(screen_animimg_1taikongren_34)
LV_IMG_DECLARE(screen_animimg_1taikongren_35)
LV_IMG_DECLARE(screen_animimg_1taikongren_36)
LV_IMG_DECLARE(screen_animimg_1taikongren_37)
LV_IMG_DECLARE(screen_animimg_1taikongren_38)
LV_IMG_DECLARE(screen_animimg_1taikongren_39)
LV_IMG_DECLARE(screen_animimg_1taikongren_40)
LV_IMG_DECLARE(screen_animimg_1taikongren_41)
LV_IMG_DECLARE(screen_animimg_1taikongren_42)
LV_IMG_DECLARE(screen_animimg_1taikongren_43)
LV_IMG_DECLARE(screen_animimg_1taikongren_44)
LV_IMG_DECLARE(screen_animimg_1taikongren_45)
LV_IMG_DECLARE(screen_animimg_1taikongren_46)
LV_IMG_DECLARE(screen_animimg_1taikongren_47)
LV_IMG_DECLARE(_btn_play_alpha_100x100);
LV_IMG_DECLARE(_btn_next_alpha_60x60);
LV_IMG_DECLARE(_electric_outlet_alpha_110x110);
LV_IMG_DECLARE(_weather_alpha_140x140);
LV_IMG_DECLARE(_virtual_alpha_100x100);
LV_IMG_DECLARE(_webcam_alpha_100x100);
LV_IMG_DECLARE(_wifi_router_alpha_100x100);
LV_IMG_DECLARE(_btn_prev_alpha_60x60);
LV_IMG_DECLARE(_global_network_alpha_100x100);

#ifdef __cplusplus
}
#endif
#endif