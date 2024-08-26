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
	lv_obj_t *screen_login;
	bool screen_login_del;
	lv_obj_t *screen_login_price;
	lv_obj_t *screen_login_label_1;
	lv_obj_t *screen_login_weigth;
	lv_obj_t *screen_login_canvas_1;
	lv_obj_t *screen_login_qrcode;
	lv_obj_t *screen_login_label_3;
	lv_obj_t *screen_login_label_4;
	lv_obj_t *screen_login_full_2;
	lv_obj_t *screen_login_password_1;
	lv_obj_t *screen_login_password_1_label;
	lv_obj_t *screen_login_password_2;
	lv_obj_t *screen_login_password_2_label;
	lv_obj_t *screen_login_password_3;
	lv_obj_t *screen_login_password_3_label;
	lv_obj_t *screen_login_label_6;
	lv_obj_t *screen_login_label_7;
	lv_obj_t *screen_login_server_status;
	lv_obj_t *screen_login_version;
	lv_obj_t *screen_login_net_0;
	lv_obj_t *screen_login_net_1;
	lv_obj_t *screen_login_net_2;
	lv_obj_t *screen_login_net_3;
	lv_obj_t *screen_login_net_4;
	lv_obj_t *screen_login_net_5;
	lv_obj_t *screen_login_net_status;
	lv_obj_t *screen_login_canvas_2;
	lv_obj_t *screen_login_img_1;
	lv_obj_t *screen_login_full;
	lv_obj_t *screen_login_updating;
	lv_obj_t *screen_login_keyboard;
	lv_obj_t *screen_login_label_8;
	lv_obj_t *screen_login_phone_number;
	lv_obj_t *screen_login_key_1;
	lv_obj_t *screen_login_key_2;
	lv_obj_t *screen_login_key_7;
	lv_obj_t *screen_login_key_4;
	lv_obj_t *screen_login_key_exit;
	lv_obj_t *screen_login_key_8;
	lv_obj_t *screen_login_key_5;
	lv_obj_t *screen_login_key_0;
	lv_obj_t *screen_login_key_del;
	lv_obj_t *screen_login_key_9;
	lv_obj_t *screen_login_key_6;
	lv_obj_t *screen_login_key_3;
	lv_obj_t *screen_login_label_25;
	lv_obj_t *screen_login_label_26;
	lv_obj_t *screen_login_label_27;
	lv_obj_t *screen_login_label_28;
	lv_obj_t *screen_login_label_29;
	lv_obj_t *screen_login_label_30;
	lv_obj_t *screen_login_label_31;
	lv_obj_t *screen_login_label_32;
	lv_obj_t *screen_login_label_33;
	lv_obj_t *screen_login_label_34;
	lv_obj_t *screen_login_label_35;
	lv_obj_t *screen_login_label_36;
	lv_obj_t *screen_login_key_enter;
	lv_obj_t *screen_login_label_24;
	lv_obj_t *screen_login_sn_num;
	lv_obj_t *screen_login_phone_notice;
	lv_obj_t *screen_login_phone_bad;
	lv_obj_t *screen_full;
	bool screen_full_del;
	lv_obj_t *screen_full_price;
	lv_obj_t *screen_full_img_2;
	lv_obj_t *screen_full_time;
	lv_obj_t *screen_full_label_2;
	lv_obj_t *screen_full_spangroup_1;
	lv_obj_t *screen_submit;
	bool screen_submit_del;
	lv_obj_t *screen_submit_line_1;
	lv_obj_t *screen_submit_img_2;
	lv_obj_t *screen_submit_time;
	lv_obj_t *screen_submit_label_1;
	lv_obj_t *screen_submit_label_2;
	lv_obj_t *screen_submit_label_3;
	lv_obj_t *screen_submit_label_4;
	lv_obj_t *screen_submit_label_5;
	lv_obj_t *screen_submit_weigth;
	lv_obj_t *screen_submit_money;
	lv_obj_t *screen_submit_post;
	lv_obj_t *screen_submit_post_label;
	lv_obj_t *screen_submit_close_door;
	lv_obj_t *screen_submit_close_door_label;
	lv_obj_t *screen_submit_label_6;
	lv_obj_t *screen_submit_label_7;
	lv_obj_t *screen_submit_cam_wrong;
	lv_obj_t *screen_clean;
	bool screen_clean_del;
	lv_obj_t *screen_clean_img_2;
	lv_obj_t *screen_clean_time;
	lv_obj_t *screen_clean_label_2;
	lv_obj_t *screen_clean_weigth;
	lv_obj_t *screen_clean_open_clean;
	lv_obj_t *screen_clean_open_clean_label;
	lv_obj_t *screen_clean_img_3;
	lv_obj_t *screen_clean_quit;
	lv_obj_t *screen_clean_quit_label;
	lv_obj_t *screen_cleaning;
	bool screen_cleaning_del;
	lv_obj_t *screen_cleaning_img_2;
	lv_obj_t *screen_cleaning_time;
	lv_obj_t *screen_cleaning_label_2;
	lv_obj_t *screen_cleaning_weigth;
	lv_obj_t *screen_cleaning_label_3;
	lv_obj_t *screen_cleaned;
	bool screen_cleaned_del;
	lv_obj_t *screen_cleaned_img_2;
	lv_obj_t *screen_cleaned_time;
	lv_obj_t *screen_cleaned_label_2;
	lv_obj_t *screen_cleaned_before_clean_weigth;
	lv_obj_t *screen_cleaned_label_3;
	lv_obj_t *screen_cleaned_after_clean_weigth;
	lv_obj_t *screen_cleaned_label_4;
	lv_obj_t *screen_cleaned_clean_weigth;
	lv_obj_t *screen_cleaned_label_6;
	lv_obj_t *screen_test;
	bool screen_test_del;
	lv_obj_t *screen_test_label_3;
	lv_obj_t *screen_test_label_4;
	lv_obj_t *screen_test_ir1_status;
	lv_obj_t *screen_test_ir2_status;
	lv_obj_t *screen_test_label_11;
	lv_obj_t *screen_test_rod_push_key;
	lv_obj_t *screen_test_rod_push_key_label;
	lv_obj_t *screen_test_rod_pull_key;
	lv_obj_t *screen_test_rod_pull_key_label;
	lv_obj_t *screen_test_inside_light_key;
	lv_obj_t *screen_test_inside_light_key_label;
	lv_obj_t *screen_test_outside_light_key;
	lv_obj_t *screen_test_outside_light_key_label;
	lv_obj_t *screen_test_inside_cam_key;
	lv_obj_t *screen_test_inside_cam_key_label;
	lv_obj_t *screen_test_outside_cam_key;
	lv_obj_t *screen_test_outside_cam_key_label;
	lv_obj_t *screen_test_btn_18;
	lv_obj_t *screen_test_btn_18_label;
	lv_obj_t *screen_test_rod_hinder_value;
	lv_obj_t *screen_test_press_up_key;
	lv_obj_t *screen_test_press_up_key_label;
	lv_obj_t *screen_test_press_down_key;
	lv_obj_t *screen_test_press_down_key_label;
	lv_obj_t *screen_test_canvas_1;
	lv_obj_t *screen_test_cal_100_key;
	lv_obj_t *screen_test_cal_100_key_label;
	lv_obj_t *screen_test_get_weight_key;
	lv_obj_t *screen_test_get_weight_key_label;
	lv_obj_t *screen_test_quit;
	lv_obj_t *screen_test_quit_label;
	lv_obj_t *screen_test_open_lock_key;
	lv_obj_t *screen_test_open_lock_key_label;
	lv_obj_t *screen_test_ir_status_key;
	lv_obj_t *screen_test_ir_status_key_label;
	lv_obj_t *screen_test_peels_key;
	lv_obj_t *screen_test_peels_key_label;
	lv_obj_t *screen_test_cal_350_key;
	lv_obj_t *screen_test_cal_350_key_label;
	lv_obj_t *screen_test_cal_200_key;
	lv_obj_t *screen_test_cal_200_key_label;
	lv_obj_t *screen_test_weight;
	lv_obj_t *screen_test_label_12;
	lv_obj_t *screen_test_cam;
	bool screen_test_cam_del;
	lv_obj_t *screen_test_cam_btn_1;
	lv_obj_t *screen_test_cam_btn_1_label;
	lv_obj_t *screen_test_cam_cam_wrong;
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
void setup_scr_screen_test(lv_ui *ui);
void setup_scr_screen_test_cam(lv_ui *ui);

LV_IMG_DECLARE(_main_600x1024_600x1024);

LV_IMG_DECLARE(_main_600x1024_600x1024);
LV_IMG_DECLARE(_0_alpha_30x29);
LV_IMG_DECLARE(_1_alpha_30x29);
LV_IMG_DECLARE(_2_alpha_30x29);
LV_IMG_DECLARE(_3_alpha_30x29);
LV_IMG_DECLARE(_4_alpha_30x29);
LV_IMG_DECLARE(_5_alpha_30x29);
LV_IMG_DECLARE(_phone_login_alpha_118x167);
LV_IMG_DECLARE(_9_alpha_555x350);
LV_IMG_DECLARE(_111_alpha_555x350);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_1_alpha_120x60);
LV_IMG_DECLARE(_key_2_alpha_400x50);
LV_IMG_DECLARE(_phone_notice_alpha_555x350);
LV_IMG_DECLARE(_phone_num_not_use_alpha_550x110);

LV_IMG_DECLARE(_main_600x1024_600x1024);
LV_IMG_DECLARE(_cycle_alpha_200x200);
LV_IMG_DECLARE(_cycle_alpha_200x200);
LV_IMG_DECLARE(_cam_wrong_2_alpha_480x320);
LV_IMG_DECLARE(_cycle_alpha_200x200);
LV_IMG_DECLARE(_re_alpha_222x216);
LV_IMG_DECLARE(_cycle_alpha_200x200);
LV_IMG_DECLARE(_cycle_alpha_200x200);
LV_IMG_DECLARE(_cam_wrong_1_alpha_480x640);

LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_48)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_56)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_28)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_24)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_32)
LV_FONT_DECLARE(lv_font_Amiko_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_38)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_96)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_36)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_54)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_42)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_34)
LV_FONT_DECLARE(lv_font_SourceHanSerifCN2_64)


#ifdef __cplusplus
}
#endif
#endif
