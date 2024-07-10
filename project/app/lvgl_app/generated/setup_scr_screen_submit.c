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



void setup_scr_screen_submit(lv_ui *ui)
{
	//Write codes screen_submit
	ui->screen_submit = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_submit, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_submit, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_submit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_img_1
	ui->screen_submit_img_1 = lv_img_create(ui->screen_submit);
	lv_obj_add_flag(ui->screen_submit_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_submit_img_1, &_backup_600x1024_alpha_600x1024);
	lv_img_set_pivot(ui->screen_submit_img_1, 50,50);
	lv_img_set_angle(ui->screen_submit_img_1, 0);
	lv_obj_set_pos(ui->screen_submit_img_1, 0, 0);
	lv_obj_set_size(ui->screen_submit_img_1, 600, 1024);

	//Write style for screen_submit_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_submit_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_line_1
	ui->screen_submit_line_1 = lv_line_create(ui->screen_submit);
	static lv_point_t screen_submit_line_1[] ={{0, 30},{600, 30},};
	lv_line_set_points(ui->screen_submit_line_1, screen_submit_line_1, 2);
	lv_obj_set_pos(ui->screen_submit_line_1, 0, 420);
	lv_obj_set_size(ui->screen_submit_line_1, 600, 60);

	//Write style for screen_submit_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->screen_submit_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->screen_submit_line_1, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->screen_submit_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->screen_submit_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_img_2
	ui->screen_submit_img_2 = lv_img_create(ui->screen_submit);
	lv_obj_add_flag(ui->screen_submit_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_submit_img_2, &_cycle_alpha_200x200);
	lv_img_set_pivot(ui->screen_submit_img_2, 50,50);
	lv_img_set_angle(ui->screen_submit_img_2, 0);
	lv_obj_set_pos(ui->screen_submit_img_2, 200, 34);
	lv_obj_set_size(ui->screen_submit_img_2, 200, 200);

	//Write style for screen_submit_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_submit_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_time
	ui->screen_submit_time = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_time, "99");
	lv_label_set_long_mode(ui->screen_submit_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_time, 220, 84);
	lv_obj_set_size(ui->screen_submit_time, 161, 101);

	//Write style for screen_submit_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_time, &lv_font_SourceHanSerifCN2_96, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_1
	ui->screen_submit_label_1 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_1, "可回收物仓门已打开");
	lv_label_set_long_mode(ui->screen_submit_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_1, 32, 299);
	lv_obj_set_size(ui->screen_submit_label_1, 537, 58);

	//Write style for screen_submit_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_1, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_2
	ui->screen_submit_label_2 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_2, "请尽快投递");
	lv_label_set_long_mode(ui->screen_submit_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_2, 32, 366);
	lv_obj_set_size(ui->screen_submit_label_2, 537, 58);

	//Write style for screen_submit_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_2, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_3
	ui->screen_submit_label_3 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_3, "本次投递重量");
	lv_label_set_long_mode(ui->screen_submit_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_3, 43, 476);
	lv_obj_set_size(ui->screen_submit_label_3, 213, 32);

	//Write style for screen_submit_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_3, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_4
	ui->screen_submit_label_4 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_4, "本次预计获得金额");
	lv_label_set_long_mode(ui->screen_submit_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_4, 344, 473);
	lv_obj_set_size(ui->screen_submit_label_4, 213, 32);

	//Write style for screen_submit_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_4, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_5
	ui->screen_submit_label_5 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_5, "> > >");
	lv_label_set_long_mode(ui->screen_submit_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_5, 205, 534);
	lv_obj_set_size(ui->screen_submit_label_5, 213, 32);

	//Write style for screen_submit_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_5, &lv_font_SourceHanSerifCN2_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_weigth
	ui->screen_submit_weigth = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_weigth, "000.00");
	lv_label_set_long_mode(ui->screen_submit_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_weigth, 25, 527);
	lv_obj_set_size(ui->screen_submit_weigth, 138, 46);

	//Write style for screen_submit_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_weigth, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_weigth, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_weigth, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_money
	ui->screen_submit_money = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_money, "000.00");
	lv_label_set_long_mode(ui->screen_submit_money, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_money, 368, 527);
	lv_obj_set_size(ui->screen_submit_money, 144, 46);

	//Write style for screen_submit_money, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_money, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_money, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_money, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_money, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_money, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_canvas_1
	ui->screen_submit_canvas_1 = lv_canvas_create(ui->screen_submit);
	static lv_color_t buf_screen_submit_canvas_1[320*240*4];
	lv_canvas_set_buffer(ui->screen_submit_canvas_1, buf_screen_submit_canvas_1, 320, 240, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_submit_canvas_1, lv_color_hex(0x000000), 255);
	lv_obj_set_pos(ui->screen_submit_canvas_1, 129, 618);
	lv_obj_set_size(ui->screen_submit_canvas_1, 320, 240);
	lv_obj_set_scrollbar_mode(ui->screen_submit_canvas_1, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_submit_post
	ui->screen_submit_post = lv_btn_create(ui->screen_submit);
	ui->screen_submit_post_label = lv_label_create(ui->screen_submit_post);
	lv_label_set_text(ui->screen_submit_post_label, "继续投递");
	lv_label_set_long_mode(ui->screen_submit_post_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_submit_post_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_submit_post, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_submit_post_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_submit_post, 166, 921);
	lv_obj_set_size(ui->screen_submit_post, 246, 60);
	lv_obj_add_flag(ui->screen_submit_post, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_submit_post, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit_post, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_submit_post, lv_color_hex(0x3ea500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_submit_post, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_submit_post, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_post, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_post, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_post, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_post, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_post, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_post, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_close_door
	ui->screen_submit_close_door = lv_btn_create(ui->screen_submit);
	ui->screen_submit_close_door_label = lv_label_create(ui->screen_submit_close_door);
	lv_label_set_text(ui->screen_submit_close_door_label, "关闭仓门");
	lv_label_set_long_mode(ui->screen_submit_close_door_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_submit_close_door_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_submit_close_door, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_submit_close_door_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_submit_close_door, 166, 921);
	lv_obj_set_size(ui->screen_submit_close_door, 246, 60);

	//Write style for screen_submit_close_door, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit_close_door, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_submit_close_door, lv_color_hex(0x3ea500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_submit_close_door, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_submit_close_door, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_close_door, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_close_door, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_close_door, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_close_door, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_close_door, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_close_door, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_6
	ui->screen_submit_label_6 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_6, "公斤");
	lv_label_set_long_mode(ui->screen_submit_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_6, 168, 527);
	lv_obj_set_size(ui->screen_submit_label_6, 90, 46);

	//Write style for screen_submit_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_6, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_7
	ui->screen_submit_label_7 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_7, "元");
	lv_label_set_long_mode(ui->screen_submit_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_7, 516, 527);
	lv_obj_set_size(ui->screen_submit_label_7, 43, 46);

	//Write style for screen_submit_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_7, &lv_font_SourceHanSerifCN2_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_submit.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_submit);

	//Init events for screen.
	events_init_screen_submit(ui);
}
