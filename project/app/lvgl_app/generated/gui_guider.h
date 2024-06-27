/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *blank;
	bool blank_del;
	lv_obj_t *blank_label_1;
	lv_obj_t *blank_label_2;
	lv_obj_t *blank_label_3;
	lv_obj_t *screen_login;
	bool screen_login_del;
	lv_obj_t *screen_login_recycle_name;
	lv_obj_t *screen_login_label_1;
	lv_obj_t *screen_login_price;
	lv_obj_t *screen_login_label_3;
	lv_obj_t *screen_login_clean_time;
	lv_obj_t *screen_login_qrcode;
	lv_obj_t *screen_login_label_5;
	lv_obj_t *screen_login_weigth;
	lv_obj_t *screen_login_full;
	lv_obj_t *screen_login_label_8;
	lv_obj_t *screen_login_label_9;
	lv_obj_t *screen_login_net_status;
	lv_obj_t *screen_login_singal;
	lv_obj_t *screen_full;
	bool screen_full_del;
	lv_obj_t *screen_full_recycle_name;
	lv_obj_t *screen_full_label_1;
	lv_obj_t *screen_full_price;
	lv_obj_t *screen_full_label_2;
	lv_obj_t *screen_full_label_3;
	lv_obj_t *screen_full_time;
	lv_obj_t *screen_submit;
	bool screen_submit_del;
	lv_obj_t *screen_submit_recycle_name;
	lv_obj_t *screen_submit_weigth;
	lv_obj_t *screen_submit_time;
	lv_obj_t *screen_submit_label_4;
	lv_obj_t *screen_submit_money;
	lv_obj_t *screen_submit_canvas_1;
	lv_obj_t *screen_submit_close_door;
	lv_obj_t *screen_submit_close_door_label;
	lv_obj_t *screen_submit_post;
	lv_obj_t *screen_submit_post_label;
	lv_obj_t *screen_submit_label_5;
	lv_obj_t *screen_clean;
	bool screen_clean_del;
	lv_obj_t *screen_clean_label_2;
	lv_obj_t *screen_clean_time;
	lv_obj_t *screen_clean_quit;
	lv_obj_t *screen_clean_quit_label;
	lv_obj_t *screen_clean_label_5;
	lv_obj_t *screen_clean_weigth;
	lv_obj_t *screen_clean_open_clean_door;
	lv_obj_t *screen_clean_open_clean_door_label;
	lv_obj_t *screen_cleaning;
	bool screen_cleaning_del;
	lv_obj_t *screen_cleaning_label_2;
	lv_obj_t *screen_cleaning_label_4;
	lv_obj_t *screen_cleaning_label_5;
	lv_obj_t *screen_cleaning_weigth;
	lv_obj_t *screen_cleaning_hand_close_door;
	lv_obj_t *screen_cleaning_hand_close_door_label;
	lv_obj_t *screen_cleaned;
	bool screen_cleaned_del;
	lv_obj_t *screen_cleaned_label_2;
	lv_obj_t *screen_cleaned_time;
	lv_obj_t *screen_cleaned_label_5;
	lv_obj_t *screen_cleaned_before_clean_weigth;
	lv_obj_t *screen_cleaned_label_6;
	lv_obj_t *screen_cleaned_label_9;
	lv_obj_t *screen_cleaned_after_clean_weigth;
	lv_obj_t *screen_cleaned_label_7;
	lv_obj_t *screen_cleaned_label_12;
	lv_obj_t *screen_cleaned_clean_weigth;
	lv_obj_t *screen_cleaned_label_10;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_blank(lv_ui *ui);
void setup_scr_screen_login(lv_ui *ui);
void setup_scr_screen_full(lv_ui *ui);
void setup_scr_screen_submit(lv_ui *ui);
void setup_scr_screen_clean(lv_ui *ui);
void setup_scr_screen_cleaning(lv_ui *ui);
void setup_scr_screen_cleaned(lv_ui *ui);

LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_48)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_24)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN_24)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN_84)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_56)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_64)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_32)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN_85)


#ifdef __cplusplus
}
#endif
#endif
