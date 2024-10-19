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



void setup_scr_screen_login(lv_ui *ui)
{
	//Write codes screen_login
	ui->screen_login = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_login, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_login, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_login, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_login, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(ui->screen_login, &_main_600x1024_600x1024, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_opa(ui->screen_login, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_price
	ui->screen_login_price = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_price, "***");
	lv_label_set_long_mode(ui->screen_login_price, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_price, 274, 433);
	lv_obj_set_size(ui->screen_login_price, 154, 94);

	//Write style for screen_login_price, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_price, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_price, &lv_font_SourceHanSerifCN2_56, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_price, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_price, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_price, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_1
	ui->screen_login_label_1 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_1, "当前重量：");
	lv_label_set_long_mode(ui->screen_login_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_1, 132, 620);
	lv_obj_set_size(ui->screen_login_label_1, 181, 31);

	//Write style for screen_login_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_1, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_weigth
	ui->screen_login_weigth = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_weigth, "***");
	lv_label_set_long_mode(ui->screen_login_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_weigth, 309, 620);
	lv_obj_set_size(ui->screen_login_weigth, 191, 31);

	//Write style for screen_login_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_weigth, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_weigth, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_weigth, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_canvas_1
	ui->screen_login_canvas_1 = lv_canvas_create(ui->screen_login);
	static lv_color_t buf_screen_login_canvas_1[210*210*4];
	lv_canvas_set_buffer(ui->screen_login_canvas_1, buf_screen_login_canvas_1, 210, 210, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_login_canvas_1, lv_color_hex(0xffffff), 255);
	lv_obj_set_pos(ui->screen_login_canvas_1, 68, 687);
	lv_obj_set_size(ui->screen_login_canvas_1, 210, 210);
	lv_obj_set_scrollbar_mode(ui->screen_login_canvas_1, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_login_label_3
	ui->screen_login_label_3 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_3, "微信扫一扫登录");
	lv_label_set_long_mode(ui->screen_login_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_3, 34, 905);
	lv_obj_set_size(ui->screen_login_label_3, 278, 36);

	//Write style for screen_login_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_3, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_4
	ui->screen_login_label_4 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_4, "网络状态：");
	lv_label_set_long_mode(ui->screen_login_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_4, 12, 981);
	lv_obj_set_size(ui->screen_login_label_4, 131, 31);

	//Write style for screen_login_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_4, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_full_2
	ui->screen_login_full_2 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_full_2, "回收柜已满");
	lv_label_set_long_mode(ui->screen_login_full_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_full_2, 398, 622);
	lv_obj_set_size(ui->screen_login_full_2, 131, 31);
	lv_obj_add_flag(ui->screen_login_full_2, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_full_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_full_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_full_2, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_full_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_full_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_full_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_full_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_password_1
	ui->screen_login_password_1 = lv_btn_create(ui->screen_login);
	ui->screen_login_password_1_label = lv_label_create(ui->screen_login_password_1);
	lv_label_set_text(ui->screen_login_password_1_label, "\n");
	lv_label_set_long_mode(ui->screen_login_password_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_login_password_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_login_password_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_login_password_1_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_login_password_1, 301, 49);
	lv_obj_set_size(ui->screen_login_password_1, 100, 90);

	//Write style for screen_login_password_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_login_password_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_login_password_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_password_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_password_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_password_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_password_1, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_password_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_password_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_password_2
	ui->screen_login_password_2 = lv_btn_create(ui->screen_login);
	ui->screen_login_password_2_label = lv_label_create(ui->screen_login_password_2);
	lv_label_set_text(ui->screen_login_password_2_label, "\n");
	lv_label_set_long_mode(ui->screen_login_password_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_login_password_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_login_password_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_login_password_2_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_login_password_2, 354, 313);
	lv_obj_set_size(ui->screen_login_password_2, 28, 28);

	//Write style for screen_login_password_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_login_password_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_login_password_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_password_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_password_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_password_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_password_2, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_password_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_password_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_password_3
	ui->screen_login_password_3 = lv_btn_create(ui->screen_login);
	ui->screen_login_password_3_label = lv_label_create(ui->screen_login_password_3);
	lv_label_set_text(ui->screen_login_password_3_label, "\n");
	lv_label_set_long_mode(ui->screen_login_password_3_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_login_password_3_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_login_password_3, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_login_password_3_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_login_password_3, 38, 433);
	lv_obj_set_size(ui->screen_login_password_3, 56, 61);

	//Write style for screen_login_password_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_login_password_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_login_password_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_password_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_password_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_password_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_password_3, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_password_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_password_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_6
	ui->screen_login_label_6 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_6, "无连网已接");
	lv_label_set_long_mode(ui->screen_login_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_6, 488, 913);
	lv_obj_set_size(ui->screen_login_label_6, 88, 53);
	lv_obj_add_flag(ui->screen_login_label_6, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_6, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_7
	ui->screen_login_label_7 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_7, "服务器状态：");
	lv_label_set_long_mode(ui->screen_login_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_7, 340, 981);
	lv_obj_set_size(ui->screen_login_label_7, 173, 31);

	//Write style for screen_login_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_7, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_server_status
	ui->screen_login_server_status = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_server_status, "***");
	lv_label_set_long_mode(ui->screen_login_server_status, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_server_status, 488, 981);
	lv_obj_set_size(ui->screen_login_server_status, 88, 31);

	//Write style for screen_login_server_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_server_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_server_status, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_server_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_server_status, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_server_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_server_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_qrcode
	ui->screen_login_qrcode = lv_qrcode_create(ui->screen_login, 170, lv_color_hex(0x2C3224), lv_color_hex(0xffffff));
	const char * screen_login_qrcode_data = "https://api.recycling.maizucity.com/index.html?sn=DEVICE123";
	lv_qrcode_update(ui->screen_login_qrcode, screen_login_qrcode_data, strlen(screen_login_qrcode_data));
	lv_obj_set_pos(ui->screen_login_qrcode, 88, 706);
	lv_obj_set_size(ui->screen_login_qrcode, 170, 170);

	//Write codes screen_login_version
	ui->screen_login_version = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_version, "v1.xxx");
	lv_label_set_long_mode(ui->screen_login_version, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_version, 508, 12);
	lv_obj_set_size(ui->screen_login_version, 83, 19);

	//Write style for screen_login_version, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_version, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_version, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_version, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_version, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_version, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_0
	ui->screen_login_net_0 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_0, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_0, &_0_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_0, 50,50);
	lv_img_set_angle(ui->screen_login_net_0, 0);
	lv_obj_set_pos(ui->screen_login_net_0, 220, 975);
	lv_obj_set_size(ui->screen_login_net_0, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_0, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_1
	ui->screen_login_net_1 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_1, &_1_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_1, 50,50);
	lv_img_set_angle(ui->screen_login_net_1, 0);
	lv_obj_set_pos(ui->screen_login_net_1, 220, 975);
	lv_obj_set_size(ui->screen_login_net_1, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_1, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_2
	ui->screen_login_net_2 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_2, &_2_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_2, 50,50);
	lv_img_set_angle(ui->screen_login_net_2, 0);
	lv_obj_set_pos(ui->screen_login_net_2, 220, 975);
	lv_obj_set_size(ui->screen_login_net_2, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_2, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_3
	ui->screen_login_net_3 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_3, &_3_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_3, 50,50);
	lv_img_set_angle(ui->screen_login_net_3, 0);
	lv_obj_set_pos(ui->screen_login_net_3, 220, 975);
	lv_obj_set_size(ui->screen_login_net_3, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_3, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_4
	ui->screen_login_net_4 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_4, &_4_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_4, 50,50);
	lv_img_set_angle(ui->screen_login_net_4, 0);
	lv_obj_set_pos(ui->screen_login_net_4, 220, 975);
	lv_obj_set_size(ui->screen_login_net_4, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_4, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_5
	ui->screen_login_net_5 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_net_5, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_net_5, &_5_alpha_30x29);
	lv_img_set_pivot(ui->screen_login_net_5, 50,50);
	lv_img_set_angle(ui->screen_login_net_5, 0);
	lv_obj_set_pos(ui->screen_login_net_5, 220, 975);
	lv_obj_set_size(ui->screen_login_net_5, 30, 29);
	lv_obj_add_flag(ui->screen_login_net_5, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_net_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_net_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_net_status
	ui->screen_login_net_status = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_net_status, "***\n");
	lv_label_set_long_mode(ui->screen_login_net_status, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_net_status, 129, 981);
	lv_obj_set_size(ui->screen_login_net_status, 88, 31);

	//Write style for screen_login_net_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_net_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_net_status, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_net_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_net_status, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_net_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_net_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_canvas_2
	ui->screen_login_canvas_2 = lv_canvas_create(ui->screen_login);
	static lv_color_t buf_screen_login_canvas_2[210*210*4];
	lv_canvas_set_buffer(ui->screen_login_canvas_2, buf_screen_login_canvas_2, 210, 210, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_login_canvas_2, lv_color_hex(0xffffff), 255);
	lv_obj_set_pos(ui->screen_login_canvas_2, 330, 687);
	lv_obj_set_size(ui->screen_login_canvas_2, 210, 210);
	lv_obj_set_scrollbar_mode(ui->screen_login_canvas_2, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_login_label_37
	ui->screen_login_label_37 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_37, "请输入手机号登录");
	lv_label_set_long_mode(ui->screen_login_label_37, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_37, 296, 903);
	lv_obj_set_size(ui->screen_login_label_37, 278, 36);

	//Write style for screen_login_label_37, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_37, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_37, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_37, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_37, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_37, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_37, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_img_1
	ui->screen_login_img_1 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_1, &_phone_login2_alpha_181x181);
	lv_img_set_pivot(ui->screen_login_img_1, 50,50);
	lv_img_set_angle(ui->screen_login_img_1, 0);
	lv_obj_set_pos(ui->screen_login_img_1, 345, 702);
	lv_obj_set_size(ui->screen_login_img_1, 181, 181);

	//Write style for screen_login_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_full
	ui->screen_login_full = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_full, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_full, &_91_alpha_300x252);
	lv_img_set_pivot(ui->screen_login_full, 50,50);
	lv_img_set_angle(ui->screen_login_full, 0);
	lv_obj_set_pos(ui->screen_login_full, 291, 680);
	lv_obj_set_size(ui->screen_login_full, 300, 252);
	lv_obj_add_flag(ui->screen_login_full, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_full, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_full, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_updating
	ui->screen_login_updating = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_updating, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_updating, &_111_alpha_555x350);
	lv_img_set_pivot(ui->screen_login_updating, 50,50);
	lv_img_set_angle(ui->screen_login_updating, 0);
	lv_obj_set_pos(ui->screen_login_updating, 23, 606);
	lv_obj_set_size(ui->screen_login_updating, 555, 350);
	lv_obj_add_flag(ui->screen_login_updating, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_updating, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_updating, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_24
	ui->screen_login_label_24 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_24, "SN:");
	lv_label_set_long_mode(ui->screen_login_label_24, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_24, 12, 12);
	lv_obj_set_size(ui->screen_login_label_24, 45, 16);

	//Write style for screen_login_label_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_24, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_24, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_24, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_24, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_24, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_sn_num
	ui->screen_login_sn_num = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_sn_num, "xxxxxxxxxxxxxx");
	lv_label_set_long_mode(ui->screen_login_sn_num, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_sn_num, 57, 12);
	lv_obj_set_size(ui->screen_login_sn_num, 232, 16);

	//Write style for screen_login_sn_num, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_sn_num, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_sn_num, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_sn_num, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_sn_num, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_sn_num, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_sn_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_keyboard
	ui->screen_login_keyboard = lv_obj_create(ui->screen_login);
	lv_obj_set_pos(ui->screen_login_keyboard, 0, 0);
	lv_obj_set_size(ui->screen_login_keyboard, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_login_keyboard, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_login_keyboard, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_keyboard, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_keyboard, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_keyboard, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_login_keyboard, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_login_keyboard, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_keyboard, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_img_2
	ui->screen_login_img_2 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_2, &_key_backup_alpha_438x369);
	lv_img_set_pivot(ui->screen_login_img_2, 50,50);
	lv_img_set_angle(ui->screen_login_img_2, 0);
	lv_obj_set_pos(ui->screen_login_img_2, 79, 410);
	lv_obj_set_size(ui->screen_login_img_2, 438, 369);

	//Write style for screen_login_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_phone_number
	ui->screen_login_phone_number = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_phone_number, "请输入手机号");
	lv_label_set_long_mode(ui->screen_login_phone_number, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_phone_number, 101, 325);
	lv_obj_set_size(ui->screen_login_phone_number, 400, 50);

	//Write style for screen_login_phone_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_phone_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_phone_number, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_phone_number, lv_color_hex(0x626262), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_phone_number, &lv_font_SourceHanSerifCN2_28, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_phone_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_phone_number, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_phone_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_phone_number, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_phone_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_login_phone_number, lv_color_hex(0xe3e3e3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_login_phone_number, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_phone_number, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_phone_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_phone_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_phone_number, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_phone_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_1
	ui->screen_login_key_1 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_1, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_1, 50,50);
	lv_img_set_angle(ui->screen_login_key_1, 0);
	lv_obj_set_pos(ui->screen_login_key_1, 100, 445);
	lv_obj_set_size(ui->screen_login_key_1, 120, 60);

	//Write style for screen_login_key_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_2
	ui->screen_login_key_2 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_2, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_2, 50,50);
	lv_img_set_angle(ui->screen_login_key_2, 0);
	lv_obj_set_pos(ui->screen_login_key_2, 240, 445);
	lv_obj_set_size(ui->screen_login_key_2, 120, 60);

	//Write style for screen_login_key_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_7
	ui->screen_login_key_7 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_7, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_7, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_7, 50,50);
	lv_img_set_angle(ui->screen_login_key_7, 0);
	lv_obj_set_pos(ui->screen_login_key_7, 100, 605);
	lv_obj_set_size(ui->screen_login_key_7, 120, 60);

	//Write style for screen_login_key_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_4
	ui->screen_login_key_4 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_4, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_4, 50,50);
	lv_img_set_angle(ui->screen_login_key_4, 0);
	lv_obj_set_pos(ui->screen_login_key_4, 100, 525);
	lv_obj_set_size(ui->screen_login_key_4, 120, 60);

	//Write style for screen_login_key_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_exit
	ui->screen_login_key_exit = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_exit, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_exit, &_key_6_alpha_400x60);
	lv_img_set_pivot(ui->screen_login_key_exit, 50,50);
	lv_img_set_angle(ui->screen_login_key_exit, 0);
	lv_obj_set_pos(ui->screen_login_key_exit, 100, 910);
	lv_obj_set_size(ui->screen_login_key_exit, 400, 60);

	//Write style for screen_login_key_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_8
	ui->screen_login_key_8 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_8, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_8, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_8, 50,50);
	lv_img_set_angle(ui->screen_login_key_8, 0);
	lv_obj_set_pos(ui->screen_login_key_8, 240, 605);
	lv_obj_set_size(ui->screen_login_key_8, 120, 60);

	//Write style for screen_login_key_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_5
	ui->screen_login_key_5 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_5, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_5, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_5, 50,50);
	lv_img_set_angle(ui->screen_login_key_5, 0);
	lv_obj_set_pos(ui->screen_login_key_5, 240, 525);
	lv_obj_set_size(ui->screen_login_key_5, 120, 60);

	//Write style for screen_login_key_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_0
	ui->screen_login_key_0 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_0, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_0, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_0, 50,50);
	lv_img_set_angle(ui->screen_login_key_0, 0);
	lv_obj_set_pos(ui->screen_login_key_0, 100, 685);
	lv_obj_set_size(ui->screen_login_key_0, 120, 60);

	//Write style for screen_login_key_0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_del
	ui->screen_login_key_del = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_del, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_del, &_key_4_alpha_255x60);
	lv_img_set_pivot(ui->screen_login_key_del, 50,50);
	lv_img_set_angle(ui->screen_login_key_del, 0);
	lv_obj_set_pos(ui->screen_login_key_del, 240, 685);
	lv_obj_set_size(ui->screen_login_key_del, 255, 60);

	//Write style for screen_login_key_del, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_del, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_9
	ui->screen_login_key_9 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_9, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_9, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_9, 50,50);
	lv_img_set_angle(ui->screen_login_key_9, 0);
	lv_obj_set_pos(ui->screen_login_key_9, 380, 605);
	lv_obj_set_size(ui->screen_login_key_9, 120, 60);

	//Write style for screen_login_key_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_6
	ui->screen_login_key_6 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_6, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_6, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_6, 50,50);
	lv_img_set_angle(ui->screen_login_key_6, 0);
	lv_obj_set_pos(ui->screen_login_key_6, 380, 525);
	lv_obj_set_size(ui->screen_login_key_6, 120, 60);

	//Write style for screen_login_key_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_3
	ui->screen_login_key_3 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_3, &_key_3_alpha_120x60);
	lv_img_set_pivot(ui->screen_login_key_3, 50,50);
	lv_img_set_angle(ui->screen_login_key_3, 0);
	lv_obj_set_pos(ui->screen_login_key_3, 380, 445);
	lv_obj_set_size(ui->screen_login_key_3, 120, 60);

	//Write style for screen_login_key_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_25
	ui->screen_login_label_25 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_25, "1");
	lv_label_set_long_mode(ui->screen_login_label_25, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_25, 129, 449);
	lv_obj_set_size(ui->screen_login_label_25, 60, 53);

	//Write style for screen_login_label_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_25, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_25, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_25, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_25, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_25, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_25, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_login_label_25, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_login_label_25, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_25, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_26
	ui->screen_login_label_26 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_26, "2");
	lv_label_set_long_mode(ui->screen_login_label_26, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_26, 271, 449);
	lv_obj_set_size(ui->screen_login_label_26, 60, 53);

	//Write style for screen_login_label_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_26, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_26, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_26, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_26, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_26, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_27
	ui->screen_login_label_27 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_27, "3");
	lv_label_set_long_mode(ui->screen_login_label_27, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_27, 410, 452);
	lv_obj_set_size(ui->screen_login_label_27, 60, 53);

	//Write style for screen_login_label_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_27, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_27, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_27, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_27, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_27, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_27, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_28
	ui->screen_login_label_28 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_28, "4");
	lv_label_set_long_mode(ui->screen_login_label_28, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_28, 129, 529);
	lv_obj_set_size(ui->screen_login_label_28, 60, 53);

	//Write style for screen_login_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_28, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_28, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_28, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_28, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_28, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_29
	ui->screen_login_label_29 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_29, "5");
	lv_label_set_long_mode(ui->screen_login_label_29, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_29, 270, 528);
	lv_obj_set_size(ui->screen_login_label_29, 60, 53);

	//Write style for screen_login_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_29, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_29, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_29, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_29, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_30
	ui->screen_login_label_30 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_30, "6");
	lv_label_set_long_mode(ui->screen_login_label_30, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_30, 411, 528);
	lv_obj_set_size(ui->screen_login_label_30, 60, 53);

	//Write style for screen_login_label_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_30, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_30, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_30, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_30, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_30, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_31
	ui->screen_login_label_31 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_31, "9");
	lv_label_set_long_mode(ui->screen_login_label_31, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_31, 410, 609);
	lv_obj_set_size(ui->screen_login_label_31, 60, 53);

	//Write style for screen_login_label_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_31, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_31, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_31, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_31, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_31, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_31, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_32
	ui->screen_login_label_32 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_32, "8");
	lv_label_set_long_mode(ui->screen_login_label_32, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_32, 270, 607);
	lv_obj_set_size(ui->screen_login_label_32, 60, 53);

	//Write style for screen_login_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_32, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_32, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_32, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_32, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_32, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_33
	ui->screen_login_label_33 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_33, "7");
	lv_label_set_long_mode(ui->screen_login_label_33, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_33, 130, 606);
	lv_obj_set_size(ui->screen_login_label_33, 60, 53);

	//Write style for screen_login_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_33, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_33, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_33, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_33, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_33, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_35
	ui->screen_login_label_35 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_35, "0");
	lv_label_set_long_mode(ui->screen_login_label_35, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_35, 130, 688);
	lv_obj_set_size(ui->screen_login_label_35, 60, 53);

	//Write style for screen_login_label_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_35, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_35, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_35, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_35, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_35, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_35, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_36
	ui->screen_login_label_36 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_36, "删除");
	lv_label_set_long_mode(ui->screen_login_label_36, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_36, 337, 688);
	lv_obj_set_size(ui->screen_login_label_36, 60, 53);

	//Write style for screen_login_label_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_36, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_36, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_36, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_36, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_36, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_34
	ui->screen_login_label_34 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_34, "退出");
	lv_label_set_long_mode(ui->screen_login_label_34, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_34, 250, 910);
	lv_obj_set_size(ui->screen_login_label_34, 100, 60);

	//Write style for screen_login_label_34, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_34, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_34, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_34, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_34, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_34, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_34, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_enter
	ui->screen_login_key_enter = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_key_enter, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_key_enter, &_key_5_alpha_400x60);
	lv_img_set_pivot(ui->screen_login_key_enter, 50,50);
	lv_img_set_angle(ui->screen_login_key_enter, 0);
	lv_obj_set_pos(ui->screen_login_key_enter, 100, 810);
	lv_obj_set_size(ui->screen_login_key_enter, 400, 60);

	//Write style for screen_login_key_enter, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_key_enter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_38
	ui->screen_login_label_38 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_38, "登录");
	lv_label_set_long_mode(ui->screen_login_label_38, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_38, 250, 810);
	lv_obj_set_size(ui->screen_login_label_38, 100, 60);

	//Write style for screen_login_label_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_38, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_38, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_38, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_38, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_38, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_img_3
	ui->screen_login_img_3 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_3, &_logo_alpha_100x100);
	lv_img_set_pivot(ui->screen_login_img_3, 50,50);
	lv_img_set_angle(ui->screen_login_img_3, 0);
	lv_obj_set_pos(ui->screen_login_img_3, 258, 135);
	lv_obj_set_size(ui->screen_login_img_3, 100, 100);

	//Write style for screen_login_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_img_4
	ui->screen_login_img_4 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_img_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_4, &_key_6_alpha_154x56);
	lv_img_set_pivot(ui->screen_login_img_4, 50,50);
	lv_img_set_angle(ui->screen_login_img_4, 0);
	lv_obj_set_pos(ui->screen_login_img_4, 0, 53);
	lv_obj_set_size(ui->screen_login_img_4, 154, 56);

	//Write style for screen_login_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_img_5
	ui->screen_login_img_5 = lv_img_create(ui->screen_login_keyboard);
	lv_obj_add_flag(ui->screen_login_img_5, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_5, &_count_alpha_45x37);
	lv_img_set_pivot(ui->screen_login_img_5, 50,50);
	lv_img_set_angle(ui->screen_login_img_5, 0);
	lv_obj_set_pos(ui->screen_login_img_5, 101, 60);
	lv_obj_set_size(ui->screen_login_img_5, 45, 37);

	//Write style for screen_login_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_key_time
	ui->screen_login_key_time = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_key_time, "60");
	lv_label_set_long_mode(ui->screen_login_key_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_key_time, 10, 53);
	lv_obj_set_size(ui->screen_login_key_time, 54, 53);

	//Write style for screen_login_key_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_key_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_key_time, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_key_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_key_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_key_time, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_key_time, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_key_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_label_40
	ui->screen_login_label_40 = lv_label_create(ui->screen_login_keyboard);
	lv_label_set_text(ui->screen_login_label_40, "S");
	lv_label_set_long_mode(ui->screen_login_label_40, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_40, 63, 53);
	lv_obj_set_size(ui->screen_login_label_40, 29, 53);

	//Write style for screen_login_label_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_40, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_40, &lv_font_SourceHanSerifCN2_38, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_40, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_40, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_40, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_40, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_phone_notice
	ui->screen_login_phone_notice = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_phone_notice, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_phone_notice, &_phone_notice_alpha_555x350);
	lv_img_set_pivot(ui->screen_login_phone_notice, 50,50);
	lv_img_set_angle(ui->screen_login_phone_notice, 0);
	lv_obj_set_pos(ui->screen_login_phone_notice, 23, 609);
	lv_obj_set_size(ui->screen_login_phone_notice, 555, 350);
	lv_obj_add_flag(ui->screen_login_phone_notice, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_phone_notice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_phone_notice, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_phone_bad
	ui->screen_login_phone_bad = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_phone_bad, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_phone_bad, &_phone_num_not_use_alpha_550x110);
	lv_img_set_pivot(ui->screen_login_phone_bad, 50,50);
	lv_img_set_angle(ui->screen_login_phone_bad, 0);
	lv_obj_set_pos(ui->screen_login_phone_bad, 32, 130);
	lv_obj_set_size(ui->screen_login_phone_bad, 550, 110);
	lv_obj_add_flag(ui->screen_login_phone_bad, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_phone_bad, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_phone_bad, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_login.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_login);

	//Init events for screen.
	events_init_screen_login(ui);
}
