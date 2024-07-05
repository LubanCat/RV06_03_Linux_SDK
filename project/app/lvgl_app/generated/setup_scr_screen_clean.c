/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_clean(lv_ui *ui)
{
	//Write codes screen_clean
	ui->screen_clean = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_clean, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_clean, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_clean, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_clean, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_img_1
	ui->screen_clean_img_1 = lv_img_create(ui->screen_clean);
	lv_obj_add_flag(ui->screen_clean_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_clean_img_1, &_backup_600x1024_alpha_600x1024);
	lv_img_set_pivot(ui->screen_clean_img_1, 50,50);
	lv_img_set_angle(ui->screen_clean_img_1, 0);
	lv_obj_set_pos(ui->screen_clean_img_1, 0, 0);
	lv_obj_set_size(ui->screen_clean_img_1, 600, 1024);

	//Write style for screen_clean_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_clean_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_img_2
	ui->screen_clean_img_2 = lv_img_create(ui->screen_clean);
	lv_obj_add_flag(ui->screen_clean_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_clean_img_2, &_cycle_alpha_200x200);
	lv_img_set_pivot(ui->screen_clean_img_2, 50,50);
	lv_img_set_angle(ui->screen_clean_img_2, 0);
	lv_obj_set_pos(ui->screen_clean_img_2, 200, 162);
	lv_obj_set_size(ui->screen_clean_img_2, 200, 200);

	//Write style for screen_clean_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_clean_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_time
	ui->screen_clean_time = lv_label_create(ui->screen_clean);
	lv_label_set_text(ui->screen_clean_time, "99");
	lv_label_set_long_mode(ui->screen_clean_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_clean_time, 220, 212);
	lv_obj_set_size(ui->screen_clean_time, 161, 101);

	//Write style for screen_clean_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clean_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clean_time, &lv_font_SourceHanSerifCN2_96, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_clean_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_clean_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clean_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_label_2
	ui->screen_clean_label_2 = lv_label_create(ui->screen_clean);
	lv_label_set_text(ui->screen_clean_label_2, "可清运物重量");
	lv_label_set_long_mode(ui->screen_clean_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_clean_label_2, 21, 703);
	lv_obj_set_size(ui->screen_clean_label_2, 537, 58);

	//Write style for screen_clean_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clean_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clean_label_2, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_clean_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_clean_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clean_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clean_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_weigth
	ui->screen_clean_weigth = lv_label_create(ui->screen_clean);
	lv_label_set_text(ui->screen_clean_weigth, "4.53KG");
	lv_label_set_long_mode(ui->screen_clean_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_clean_weigth, 190, 803);
	lv_obj_set_size(ui->screen_clean_weigth, 220, 49);

	//Write style for screen_clean_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clean_weigth, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clean_weigth, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_clean_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_clean_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clean_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_open_clean
	ui->screen_clean_open_clean = lv_btn_create(ui->screen_clean);
	ui->screen_clean_open_clean_label = lv_label_create(ui->screen_clean_open_clean);
	lv_label_set_text(ui->screen_clean_open_clean_label, "开启清运门");
	lv_label_set_long_mode(ui->screen_clean_open_clean_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_clean_open_clean_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_clean_open_clean, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_clean_open_clean_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_clean_open_clean, 177, 906);
	lv_obj_set_size(ui->screen_clean_open_clean, 246, 60);

	//Write style for screen_clean_open_clean, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_clean_open_clean, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_clean_open_clean, lv_color_hex(0x3ea500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_clean_open_clean, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_clean_open_clean, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clean_open_clean, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clean_open_clean, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clean_open_clean, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clean_open_clean, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_clean_open_clean, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clean_open_clean, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_img_3
	ui->screen_clean_img_3 = lv_img_create(ui->screen_clean);
	lv_obj_add_flag(ui->screen_clean_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_clean_img_3, &_3_alpha_222x216);
	lv_img_set_pivot(ui->screen_clean_img_3, 50,50);
	lv_img_set_angle(ui->screen_clean_img_3, 0);
	lv_obj_set_pos(ui->screen_clean_img_3, 189, 450);
	lv_obj_set_size(ui->screen_clean_img_3, 222, 216);

	//Write style for screen_clean_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_clean_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clean_quit
	ui->screen_clean_quit = lv_btn_create(ui->screen_clean);
	ui->screen_clean_quit_label = lv_label_create(ui->screen_clean_quit);
	lv_label_set_text(ui->screen_clean_quit_label, "退出");
	lv_label_set_long_mode(ui->screen_clean_quit_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_clean_quit_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_clean_quit, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_clean_quit_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_clean_quit, 16, 25);
	lv_obj_set_size(ui->screen_clean_quit, 130, 60);

	//Write style for screen_clean_quit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_clean_quit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_clean_quit, lv_color_hex(0x3ea500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_clean_quit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_clean_quit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clean_quit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clean_quit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clean_quit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clean_quit, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_clean_quit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clean_quit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_clean.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_clean);

	//Init events for screen.
	events_init_screen_clean(ui);
}
