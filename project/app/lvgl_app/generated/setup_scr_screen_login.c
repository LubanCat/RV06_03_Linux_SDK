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

	//Write codes screen_login_img_1
	ui->screen_login_img_1 = lv_img_create(ui->screen_login);
	lv_obj_add_flag(ui->screen_login_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_login_img_1, &_main_600x1024_alpha_600x1024);
	lv_img_set_pivot(ui->screen_login_img_1, 50,50);
	lv_img_set_angle(ui->screen_login_img_1, 0);
	lv_obj_set_pos(ui->screen_login_img_1, 0, 0);
	lv_obj_set_size(ui->screen_login_img_1, 600, 1024);

	//Write style for screen_login_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_login_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_price
	ui->screen_login_price = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_price, "0.70");
	lv_label_set_long_mode(ui->screen_login_price, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_price, 274, 434);
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
	lv_obj_set_pos(ui->screen_login_label_1, 143, 608);
	lv_obj_set_size(ui->screen_login_label_1, 131, 31);

	//Write style for screen_login_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_1, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
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

	//Write codes screen_login_label_2
	ui->screen_login_label_2 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_2, "最近清运：");
	lv_label_set_long_mode(ui->screen_login_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_2, 104, 643);
	lv_obj_set_size(ui->screen_login_label_2, 131, 31);

	//Write style for screen_login_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_2, lv_color_hex(0x26B08C), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_2, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_weigth
	ui->screen_login_weigth = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_weigth, "8.35KG");
	lv_label_set_long_mode(ui->screen_login_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_weigth, 261, 608);
	lv_obj_set_size(ui->screen_login_weigth, 131, 31);

	//Write style for screen_login_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_weigth, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_weigth, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_clean_time
	ui->screen_login_clean_time = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_clean_time, "2024-05-21 15:26:21");
	lv_label_set_long_mode(ui->screen_login_clean_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_clean_time, 206, 643);
	lv_obj_set_size(ui->screen_login_clean_time, 361, 31);

	//Write style for screen_login_clean_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_clean_time, lv_color_hex(0x26B08C), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_clean_time, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_clean_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_clean_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_clean_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_clean_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_canvas_1
	ui->screen_login_canvas_1 = lv_canvas_create(ui->screen_login);
	static lv_color_t buf_screen_login_canvas_1[220*220*4];
	lv_canvas_set_buffer(ui->screen_login_canvas_1, buf_screen_login_canvas_1, 220, 220, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_login_canvas_1, lv_color_hex(0xffffff), 255);
	lv_obj_set_pos(ui->screen_login_canvas_1, 190, 681);
	lv_obj_set_size(ui->screen_login_canvas_1, 220, 220);
	lv_obj_set_scrollbar_mode(ui->screen_login_canvas_1, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_login_qrcode
	ui->screen_login_qrcode = lv_qrcode_create(ui->screen_login, 170, lv_color_hex(0x2C3224), lv_color_hex(0xffffff));
	const char * screen_login_qrcode_data = "https://api.recycling.maizucity.com/index.html?sn=DEVICE123";
	lv_qrcode_update(ui->screen_login_qrcode, screen_login_qrcode_data, strlen(screen_login_qrcode_data));
	lv_obj_set_pos(ui->screen_login_qrcode, 215, 706);
	lv_obj_set_size(ui->screen_login_qrcode, 170, 170);

	//Write codes screen_login_label_3
	ui->screen_login_label_3 = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_label_3, "微信扫一扫登录");
	lv_label_set_long_mode(ui->screen_login_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_3, 161, 909);
	lv_obj_set_size(ui->screen_login_label_3, 278, 36);

	//Write style for screen_login_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_label_3, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
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

	//Write codes screen_login_net_status
	ui->screen_login_net_status = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_net_status, "已连网");
	lv_label_set_long_mode(ui->screen_login_net_status, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_net_status, 130, 981);
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

	//Write codes screen_login_singal
	ui->screen_login_singal = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_singal, "99");
	lv_label_set_long_mode(ui->screen_login_singal, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_singal, 218, 981);
	lv_obj_set_size(ui->screen_login_singal, 39, 31);

	//Write style for screen_login_singal, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_singal, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_singal, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_singal, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_singal, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_singal, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_singal, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_login_full
	ui->screen_login_full = lv_label_create(ui->screen_login);
	lv_label_set_text(ui->screen_login_full, "回收柜已满");
	lv_label_set_long_mode(ui->screen_login_full, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_full, 389, 608);
	lv_obj_set_size(ui->screen_login_full, 131, 31);
	lv_obj_add_flag(ui->screen_login_full, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_login_full, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_login_full, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_login_full, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_login_full, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_login_full, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_login_full, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_login_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

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
	lv_label_set_text(ui->screen_login_label_6, "无连网");
	lv_label_set_long_mode(ui->screen_login_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_login_label_6, 484, 976);
	lv_obj_set_size(ui->screen_login_label_6, 88, 31);
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

	//The custom code of screen_login.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_login);

	//Init events for screen.
	events_init_screen_login(ui);
}
