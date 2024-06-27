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
	lv_obj_set_size(ui->screen_submit, 480, 480);
	lv_obj_set_scrollbar_mode(ui->screen_submit, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_submit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_submit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_submit, LV_GRAD_DIR_HOR, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_submit, lv_color_hex(0x9bcffb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_main_stop(ui->screen_submit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_stop(ui->screen_submit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_recycle_name
	ui->screen_submit_recycle_name = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_recycle_name, "本次投递重量");
	lv_label_set_long_mode(ui->screen_submit_recycle_name, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_recycle_name, 23, 107);
	lv_obj_set_size(ui->screen_submit_recycle_name, 180, 29);

	//Write style for screen_submit_recycle_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_recycle_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_recycle_name, &lv_font_SourceHanSerifCN_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_recycle_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_recycle_name, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_recycle_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_recycle_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_weigth
	ui->screen_submit_weigth = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_weigth, "4.5公斤\n");
	lv_label_set_long_mode(ui->screen_submit_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_weigth, 8, 152);
	lv_obj_set_size(ui->screen_submit_weigth, 152, 25);

	//Write style for screen_submit_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_weigth, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_weigth, &lv_font_SourceHanSerifCN_24, LV_PART_MAIN|LV_STATE_DEFAULT);
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

	//Write codes screen_submit_time
	ui->screen_submit_time = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_time, "99");
	lv_label_set_long_mode(ui->screen_submit_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_time, 152, 6);
	lv_obj_set_size(ui->screen_submit_time, 140, 87);

	//Write style for screen_submit_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_time, &lv_font_SourceHanSerifCN_84, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_time, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_label_4
	ui->screen_submit_label_4 = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_label_4, "本次预计获得金额");
	lv_label_set_long_mode(ui->screen_submit_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_label_4, 256, 103);
	lv_obj_set_size(ui->screen_submit_label_4, 220, 29);

	//Write style for screen_submit_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_label_4, &lv_font_SourceHanSerifCN_24, LV_PART_MAIN|LV_STATE_DEFAULT);
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

	//Write codes screen_submit_money
	ui->screen_submit_money = lv_label_create(ui->screen_submit);
	lv_label_set_text(ui->screen_submit_money, "11元");
	lv_label_set_long_mode(ui->screen_submit_money, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_submit_money, 294, 152);
	lv_obj_set_size(ui->screen_submit_money, 138, 29);

	//Write style for screen_submit_money, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_money, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_money, &lv_font_SourceHanSerifCN_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_money, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_submit_money, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_money, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_money, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_canvas_1
	ui->screen_submit_canvas_1 = lv_canvas_create(ui->screen_submit);
	static lv_color_t buf_screen_submit_canvas_1[267*152*4];
	lv_canvas_set_buffer(ui->screen_submit_canvas_1, buf_screen_submit_canvas_1, 267, 152, LV_IMG_CF_TRUE_COLOR_ALPHA);
	lv_canvas_fill_bg(ui->screen_submit_canvas_1, lv_color_hex(0x000000), 255);
	lv_obj_set_pos(ui->screen_submit_canvas_1, 107, 205);
	lv_obj_set_size(ui->screen_submit_canvas_1, 267, 152);
	lv_obj_set_scrollbar_mode(ui->screen_submit_canvas_1, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_submit_close_door
	ui->screen_submit_close_door = lv_btn_create(ui->screen_submit);
	ui->screen_submit_close_door_label = lv_label_create(ui->screen_submit_close_door);
	lv_label_set_text(ui->screen_submit_close_door_label, "关闭仓门");
	lv_label_set_long_mode(ui->screen_submit_close_door_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_submit_close_door_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_submit_close_door, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_submit_close_door_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_submit_close_door, 190, 392);
	lv_obj_set_size(ui->screen_submit_close_door, 100, 50);

	//Write style for screen_submit_close_door, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit_close_door, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_submit_close_door, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_submit_close_door, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_submit_close_door, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_close_door, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_close_door, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_close_door, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_close_door, &lv_font_SourceHanSerifCN2_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_close_door, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_close_door, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_submit_continue_open
	ui->screen_submit_continue_open = lv_btn_create(ui->screen_submit);
	ui->screen_submit_continue_open_label = lv_label_create(ui->screen_submit_continue_open);
	lv_label_set_text(ui->screen_submit_continue_open_label, "继续投递");
	lv_label_set_long_mode(ui->screen_submit_continue_open_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_submit_continue_open_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_submit_continue_open, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_submit_continue_open_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_submit_continue_open, 190, 392);
	lv_obj_set_size(ui->screen_submit_continue_open, 100, 50);
	lv_obj_add_flag(ui->screen_submit_continue_open, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_submit_continue_open, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_submit_continue_open, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_submit_continue_open, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_submit_continue_open, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_submit_continue_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_submit_continue_open, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_submit_continue_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_submit_continue_open, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_submit_continue_open, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_submit_continue_open, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_submit_continue_open, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_submit.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_submit);

	//Init events for screen.
	events_init_screen_submit(ui);
}
