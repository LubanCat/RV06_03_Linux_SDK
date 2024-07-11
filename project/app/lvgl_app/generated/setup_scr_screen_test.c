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



void setup_scr_screen_test(lv_ui *ui)
{
	//Write codes screen_test
	ui->screen_test = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_test, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_test, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_test, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test, lv_color_hex(0xf5f5f5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_label_3
	ui->screen_test_label_3 = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_label_3, "红外1状态：\n");
	lv_label_set_long_mode(ui->screen_test_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_label_3, 69, 297);
	lv_obj_set_size(ui->screen_test_label_3, 208, 39);

	//Write style for screen_test_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_label_3, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_label_4
	ui->screen_test_label_4 = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_label_4, "红外2状态：\n");
	lv_label_set_long_mode(ui->screen_test_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_label_4, 291, 298);
	lv_obj_set_size(ui->screen_test_label_4, 238, 38);

	//Write style for screen_test_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_label_4, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_ir1_status
	ui->screen_test_ir1_status = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_ir1_status, "0\n\n");
	lv_label_set_long_mode(ui->screen_test_ir1_status, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_ir1_status, 204, 293);
	lv_obj_set_size(ui->screen_test_ir1_status, 115, 35);

	//Write style for screen_test_ir1_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_ir1_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_ir1_status, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_ir1_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_ir1_status, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_ir1_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_ir1_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_ir2_status
	ui->screen_test_ir2_status = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_ir2_status, "0\n\n");
	lv_label_set_long_mode(ui->screen_test_ir2_status, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_ir2_status, 443, 293);
	lv_obj_set_size(ui->screen_test_ir2_status, 115, 57);

	//Write style for screen_test_ir2_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_ir2_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_ir2_status, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_ir2_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_ir2_status, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_ir2_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_ir2_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_label_11
	ui->screen_test_label_11 = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_label_11, "未连网");
	lv_label_set_long_mode(ui->screen_test_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_label_11, 45, 48);
	lv_obj_set_size(ui->screen_test_label_11, 100, 32);
	lv_obj_add_flag(ui->screen_test_label_11, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_test_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_label_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_label_11, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_label_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_label_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_rod_push_key
	ui->screen_test_rod_push_key = lv_btn_create(ui->screen_test);
	ui->screen_test_rod_push_key_label = lv_label_create(ui->screen_test_rod_push_key);
	lv_label_set_text(ui->screen_test_rod_push_key_label, "推杆开");
	lv_label_set_long_mode(ui->screen_test_rod_push_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_rod_push_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_rod_push_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_rod_push_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_rod_push_key, 77, 40);
	lv_obj_set_size(ui->screen_test_rod_push_key, 200, 60);

	//Write style for screen_test_rod_push_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_rod_push_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_rod_push_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_rod_push_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_rod_push_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_rod_push_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_rod_push_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_rod_push_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_rod_push_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_rod_push_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_rod_push_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_rod_pull_key
	ui->screen_test_rod_pull_key = lv_btn_create(ui->screen_test);
	ui->screen_test_rod_pull_key_label = lv_label_create(ui->screen_test_rod_pull_key);
	lv_label_set_text(ui->screen_test_rod_pull_key_label, "推杆关");
	lv_label_set_long_mode(ui->screen_test_rod_pull_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_rod_pull_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_rod_pull_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_rod_pull_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_rod_pull_key, 329, 40);
	lv_obj_set_size(ui->screen_test_rod_pull_key, 200, 60);

	//Write style for screen_test_rod_pull_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_rod_pull_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_rod_pull_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_rod_pull_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_rod_pull_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_rod_pull_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_rod_pull_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_rod_pull_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_rod_pull_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_rod_pull_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_rod_pull_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_inside_light_key
	ui->screen_test_inside_light_key = lv_btn_create(ui->screen_test);
	ui->screen_test_inside_light_key_label = lv_label_create(ui->screen_test_inside_light_key);
	lv_label_set_text(ui->screen_test_inside_light_key_label, "内灯");
	lv_label_set_long_mode(ui->screen_test_inside_light_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_inside_light_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_inside_light_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_inside_light_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_inside_light_key, 77, 126);
	lv_obj_set_size(ui->screen_test_inside_light_key, 200, 60);

	//Write style for screen_test_inside_light_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_inside_light_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_inside_light_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_inside_light_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_inside_light_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_inside_light_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_inside_light_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_inside_light_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_inside_light_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_inside_light_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_inside_light_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_outside_light_key
	ui->screen_test_outside_light_key = lv_btn_create(ui->screen_test);
	ui->screen_test_outside_light_key_label = lv_label_create(ui->screen_test_outside_light_key);
	lv_label_set_text(ui->screen_test_outside_light_key_label, "外灯");
	lv_label_set_long_mode(ui->screen_test_outside_light_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_outside_light_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_outside_light_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_outside_light_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_outside_light_key, 325, 126);
	lv_obj_set_size(ui->screen_test_outside_light_key, 200, 60);

	//Write style for screen_test_outside_light_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_outside_light_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_outside_light_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_outside_light_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_outside_light_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_outside_light_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_outside_light_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_outside_light_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_outside_light_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_outside_light_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_outside_light_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_inside_cam_key
	ui->screen_test_inside_cam_key = lv_btn_create(ui->screen_test);
	ui->screen_test_inside_cam_key_label = lv_label_create(ui->screen_test_inside_cam_key);
	lv_label_set_text(ui->screen_test_inside_cam_key_label, "内摄像头预览");
	lv_label_set_long_mode(ui->screen_test_inside_cam_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_inside_cam_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_inside_cam_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_inside_cam_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_inside_cam_key, 75, 342);
	lv_obj_set_size(ui->screen_test_inside_cam_key, 200, 60);

	//Write style for screen_test_inside_cam_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_inside_cam_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_inside_cam_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_inside_cam_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_inside_cam_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_inside_cam_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_inside_cam_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_inside_cam_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_inside_cam_key, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_inside_cam_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_inside_cam_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_outside_cam_key
	ui->screen_test_outside_cam_key = lv_btn_create(ui->screen_test);
	ui->screen_test_outside_cam_key_label = lv_label_create(ui->screen_test_outside_cam_key);
	lv_label_set_text(ui->screen_test_outside_cam_key_label, "外摄像头预览");
	lv_label_set_long_mode(ui->screen_test_outside_cam_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_outside_cam_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_outside_cam_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_outside_cam_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_outside_cam_key, 325, 342);
	lv_obj_set_size(ui->screen_test_outside_cam_key, 200, 60);

	//Write style for screen_test_outside_cam_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_outside_cam_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_outside_cam_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_outside_cam_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_outside_cam_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_outside_cam_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_outside_cam_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_outside_cam_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_outside_cam_key, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_outside_cam_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_outside_cam_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_btn_18
	ui->screen_test_btn_18 = lv_btn_create(ui->screen_test);
	ui->screen_test_btn_18_label = lv_label_create(ui->screen_test_btn_18);
	lv_label_set_text(ui->screen_test_btn_18_label, "        阻力阀值");
	lv_label_set_long_mode(ui->screen_test_btn_18_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_btn_18_label, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_btn_18, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_btn_18_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_btn_18, 75, 425);
	lv_obj_set_size(ui->screen_test_btn_18, 450, 60);

	//Write style for screen_test_btn_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_btn_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_btn_18, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_btn_18, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_btn_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_btn_18, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_btn_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_btn_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_btn_18, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_btn_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_btn_18, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_rod_hinder_value
	ui->screen_test_rod_hinder_value = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_rod_hinder_value, "280");
	lv_label_set_long_mode(ui->screen_test_rod_hinder_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_rod_hinder_value, 365, 435);
	lv_obj_set_size(ui->screen_test_rod_hinder_value, 90, 40);

	//Write style for screen_test_rod_hinder_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_rod_hinder_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_rod_hinder_value, &lv_font_SourceHanSerifCN2_34, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_rod_hinder_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_rod_hinder_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_rod_hinder_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_rod_hinder_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_rod_hinder_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_rod_hinder_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_rod_hinder_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_press_up_key
	ui->screen_test_press_up_key = lv_btn_create(ui->screen_test);
	ui->screen_test_press_up_key_label = lv_label_create(ui->screen_test_press_up_key);
	lv_label_set_text(ui->screen_test_press_up_key_label, "+");
	lv_label_set_long_mode(ui->screen_test_press_up_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_press_up_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_press_up_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_press_up_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_press_up_key, 460, 435);
	lv_obj_set_size(ui->screen_test_press_up_key, 50, 40);

	//Write style for screen_test_press_up_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_press_up_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_press_up_key, lv_color_hex(0x4c924c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_press_up_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_press_up_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_press_up_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_press_up_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_press_up_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_press_up_key, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_press_up_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_press_up_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_press_down_key
	ui->screen_test_press_down_key = lv_btn_create(ui->screen_test);
	ui->screen_test_press_down_key_label = lv_label_create(ui->screen_test_press_down_key);
	lv_label_set_text(ui->screen_test_press_down_key_label, "-");
	lv_label_set_long_mode(ui->screen_test_press_down_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_press_down_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_press_down_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_press_down_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_press_down_key, 310, 435);
	lv_obj_set_size(ui->screen_test_press_down_key, 50, 40);

	//Write style for screen_test_press_down_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_press_down_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_press_down_key, lv_color_hex(0x4c924c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_press_down_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_press_down_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_press_down_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_press_down_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_press_down_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_press_down_key, &lv_font_SourceHanSerifCN2_64, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_press_down_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_press_down_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_canvas_1
	ui->screen_test_canvas_1 = lv_canvas_create(ui->screen_test);
	static lv_color_t buf_screen_test_canvas_1[500*378*4];
	lv_canvas_set_buffer(ui->screen_test_canvas_1, buf_screen_test_canvas_1, 500, 378, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_test_canvas_1, lv_color_hex(0xe3e3e3), 255);
	lv_obj_set_pos(ui->screen_test_canvas_1, 50, 514);
	lv_obj_set_size(ui->screen_test_canvas_1, 500, 378);
	lv_obj_set_scrollbar_mode(ui->screen_test_canvas_1, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_test_cal_100_key
	ui->screen_test_cal_100_key = lv_btn_create(ui->screen_test);
	ui->screen_test_cal_100_key_label = lv_label_create(ui->screen_test_cal_100_key);
	lv_label_set_text(ui->screen_test_cal_100_key_label, "100kg校准");
	lv_label_set_long_mode(ui->screen_test_cal_100_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_cal_100_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_cal_100_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_cal_100_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_cal_100_key, 75, 621);
	lv_obj_set_size(ui->screen_test_cal_100_key, 140, 80);

	//Write style for screen_test_cal_100_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_cal_100_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_cal_100_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_cal_100_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_cal_100_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_cal_100_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_cal_100_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_cal_100_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_cal_100_key, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_cal_100_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_cal_100_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_get_weight_key
	ui->screen_test_get_weight_key = lv_btn_create(ui->screen_test);
	ui->screen_test_get_weight_key_label = lv_label_create(ui->screen_test_get_weight_key);
	lv_label_set_text(ui->screen_test_get_weight_key_label, "称重");
	lv_label_set_long_mode(ui->screen_test_get_weight_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_get_weight_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_get_weight_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_get_weight_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_get_weight_key, 75, 724);
	lv_obj_set_size(ui->screen_test_get_weight_key, 450, 76);

	//Write style for screen_test_get_weight_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_get_weight_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_get_weight_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_get_weight_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_get_weight_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_get_weight_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_get_weight_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_get_weight_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_get_weight_key, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_get_weight_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_get_weight_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_quit
	ui->screen_test_quit = lv_btn_create(ui->screen_test);
	ui->screen_test_quit_label = lv_label_create(ui->screen_test_quit);
	lv_label_set_text(ui->screen_test_quit_label, "返回");
	lv_label_set_long_mode(ui->screen_test_quit_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_quit_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_quit, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_quit_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_quit, 450, 933);
	lv_obj_set_size(ui->screen_test_quit, 100, 50);

	//Write style for screen_test_quit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_quit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_quit, lv_color_hex(0xf26666), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_quit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_quit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_quit, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_quit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_quit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_quit, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_quit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_quit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_open_lock_key
	ui->screen_test_open_lock_key = lv_btn_create(ui->screen_test);
	ui->screen_test_open_lock_key_label = lv_label_create(ui->screen_test_open_lock_key);
	lv_label_set_text(ui->screen_test_open_lock_key_label, "电磁锁开");
	lv_label_set_long_mode(ui->screen_test_open_lock_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_open_lock_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_open_lock_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_open_lock_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_open_lock_key, 77, 211);
	lv_obj_set_size(ui->screen_test_open_lock_key, 200, 60);

	//Write style for screen_test_open_lock_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_open_lock_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_open_lock_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_open_lock_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_open_lock_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_open_lock_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_open_lock_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_open_lock_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_open_lock_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_open_lock_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_open_lock_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_ir_status_key
	ui->screen_test_ir_status_key = lv_btn_create(ui->screen_test);
	ui->screen_test_ir_status_key_label = lv_label_create(ui->screen_test_ir_status_key);
	lv_label_set_text(ui->screen_test_ir_status_key_label, "红外状态");
	lv_label_set_long_mode(ui->screen_test_ir_status_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_ir_status_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_ir_status_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_ir_status_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_ir_status_key, 325, 211);
	lv_obj_set_size(ui->screen_test_ir_status_key, 200, 60);

	//Write style for screen_test_ir_status_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_ir_status_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_ir_status_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_ir_status_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_ir_status_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_ir_status_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_ir_status_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_ir_status_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_ir_status_key, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_ir_status_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_ir_status_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_peels_key
	ui->screen_test_peels_key = lv_btn_create(ui->screen_test);
	ui->screen_test_peels_key_label = lv_label_create(ui->screen_test_peels_key);
	lv_label_set_text(ui->screen_test_peels_key_label, "清零去皮");
	lv_label_set_long_mode(ui->screen_test_peels_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_peels_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_peels_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_peels_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_peels_key, 75, 529);
	lv_obj_set_size(ui->screen_test_peels_key, 450, 73);

	//Write style for screen_test_peels_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_peels_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_peels_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_peels_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_peels_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_peels_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_peels_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_peels_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_peels_key, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_peels_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_peels_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_cal_350_key
	ui->screen_test_cal_350_key = lv_btn_create(ui->screen_test);
	ui->screen_test_cal_350_key_label = lv_label_create(ui->screen_test_cal_350_key);
	lv_label_set_text(ui->screen_test_cal_350_key_label, "350kg校准");
	lv_label_set_long_mode(ui->screen_test_cal_350_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_cal_350_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_cal_350_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_cal_350_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_cal_350_key, 385, 621);
	lv_obj_set_size(ui->screen_test_cal_350_key, 140, 80);

	//Write style for screen_test_cal_350_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_cal_350_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_cal_350_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_cal_350_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_cal_350_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_cal_350_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_cal_350_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_cal_350_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_cal_350_key, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_cal_350_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_cal_350_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_cal_200_key
	ui->screen_test_cal_200_key = lv_btn_create(ui->screen_test);
	ui->screen_test_cal_200_key_label = lv_label_create(ui->screen_test_cal_200_key);
	lv_label_set_text(ui->screen_test_cal_200_key_label, "200kg校准");
	lv_label_set_long_mode(ui->screen_test_cal_200_key_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_test_cal_200_key_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_test_cal_200_key, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_test_cal_200_key_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_test_cal_200_key, 230, 621);
	lv_obj_set_size(ui->screen_test_cal_200_key, 140, 80);

	//Write style for screen_test_cal_200_key, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_test_cal_200_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_test_cal_200_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_test_cal_200_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_test_cal_200_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_cal_200_key, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_cal_200_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_cal_200_key, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_cal_200_key, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_cal_200_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_cal_200_key, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_test_weight
	ui->screen_test_weight = lv_label_create(ui->screen_test);
	lv_label_set_text(ui->screen_test_weight, "000.00kg");
	lv_label_set_long_mode(ui->screen_test_weight, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_test_weight, 149, 813);
	lv_obj_set_size(ui->screen_test_weight, 303, 67);

	//Write style for screen_test_weight, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_test_weight, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_test_weight, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_test_weight, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_test_weight, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_test_weight, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_test_weight, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_test_weight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_test.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_test);

	//Init events for screen.
	events_init_screen_test(ui);
}
