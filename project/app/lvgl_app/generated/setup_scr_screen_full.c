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



void setup_scr_screen_full(lv_ui *ui)
{
	//Write codes screen_full
	ui->screen_full = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_full, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_full, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_full, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_full, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_img_1
	ui->screen_full_img_1 = lv_img_create(ui->screen_full);
	lv_obj_add_flag(ui->screen_full_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_full_img_1, &_main_600x1024_alpha_600x1024);
	lv_img_set_pivot(ui->screen_full_img_1, 50,50);
	lv_img_set_angle(ui->screen_full_img_1, 0);
	lv_obj_set_pos(ui->screen_full_img_1, 0, 0);
	lv_obj_set_size(ui->screen_full_img_1, 600, 1024);

	//Write style for screen_full_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_full_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_price
	ui->screen_full_price = lv_label_create(ui->screen_full);
	lv_label_set_text(ui->screen_full_price, "0.70");
	lv_label_set_long_mode(ui->screen_full_price, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_full_price, 274, 434);
	lv_obj_set_size(ui->screen_full_price, 154, 94);

	//Write style for screen_full_price, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_full_price, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_full_price, &lv_font_SourceHanSerifCN2_56, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_full_price, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_full_price, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_full_price, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_full_price, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_img_2
	ui->screen_full_img_2 = lv_img_create(ui->screen_full);
	lv_obj_add_flag(ui->screen_full_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_full_img_2, &_cycle_alpha_200x200);
	lv_img_set_pivot(ui->screen_full_img_2, 50,50);
	lv_img_set_angle(ui->screen_full_img_2, 0);
	lv_obj_set_pos(ui->screen_full_img_2, 200, 624);
	lv_obj_set_size(ui->screen_full_img_2, 200, 200);

	//Write style for screen_full_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_full_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_time
	ui->screen_full_time = lv_label_create(ui->screen_full);
	lv_label_set_text(ui->screen_full_time, "10");
	lv_label_set_long_mode(ui->screen_full_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_full_time, 220, 674);
	lv_obj_set_size(ui->screen_full_time, 161, 101);

	//Write style for screen_full_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_full_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_full_time, &lv_font_SourceHanSerifCN2_96, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_full_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_full_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_full_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_full_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_label_2
	ui->screen_full_label_2 = lv_label_create(ui->screen_full);
	lv_label_set_text(ui->screen_full_label_2, "请清运后再投递");
	lv_label_set_long_mode(ui->screen_full_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_full_label_2, 47, 936);
	lv_obj_set_size(ui->screen_full_label_2, 506, 61);

	//Write style for screen_full_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_full_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_full_label_2, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_full_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_full_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_full_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_full_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_full_spangroup_1
	ui->screen_full_spangroup_1 = lv_spangroup_create(ui->screen_full);
	lv_spangroup_set_align(ui->screen_full_spangroup_1, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_full_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_full_spangroup_1, LV_SPAN_MODE_BREAK);
	//create spans
	lv_span_t *screen_full_spangroup_1_span;
	screen_full_spangroup_1_span = lv_spangroup_new_span(ui->screen_full_spangroup_1);
	lv_span_set_text(screen_full_spangroup_1_span, "当前回收柜");
	lv_style_set_text_color(&screen_full_spangroup_1_span->style, lv_color_hex(0x000000));
	lv_style_set_text_decor(&screen_full_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_full_spangroup_1_span->style, &lv_font_SourceHanSerifCN2_48);
	screen_full_spangroup_1_span = lv_spangroup_new_span(ui->screen_full_spangroup_1);
	lv_span_set_text(screen_full_spangroup_1_span, "已满");
	lv_style_set_text_color(&screen_full_spangroup_1_span->style, lv_color_hex(0xf00000));
	lv_style_set_text_decor(&screen_full_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_full_spangroup_1_span->style, &lv_font_SourceHanSerifCN2_48);
	lv_obj_set_pos(ui->screen_full_spangroup_1, 127, 874);
	lv_obj_set_size(ui->screen_full_spangroup_1, 346, 62);

	//Write style state: LV_STATE_DEFAULT for &style_screen_full_spangroup_1_main_main_default
	static lv_style_t style_screen_full_spangroup_1_main_main_default;
	ui_init_style(&style_screen_full_spangroup_1_main_main_default);
	
	lv_style_set_border_width(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_radius(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_bg_opa(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_style_set_shadow_width(&style_screen_full_spangroup_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_full_spangroup_1, &style_screen_full_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_spangroup_refr_mode(ui->screen_full_spangroup_1);

	//The custom code of screen_full.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_full);

}
