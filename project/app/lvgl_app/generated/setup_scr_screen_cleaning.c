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



void setup_scr_screen_cleaning(lv_ui *ui)
{
	//Write codes screen_cleaning
	ui->screen_cleaning = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_cleaning, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_cleaning, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cleaning, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_cleaning, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cleaning, lv_color_hex(0xf5f5f5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cleaning, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_img_2
	ui->screen_cleaning_img_2 = lv_img_create(ui->screen_cleaning);
	lv_obj_add_flag(ui->screen_cleaning_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_cleaning_img_2, &_cycle_alpha_200x200);
	lv_img_set_pivot(ui->screen_cleaning_img_2, 50,50);
	lv_img_set_angle(ui->screen_cleaning_img_2, 0);
	lv_obj_set_pos(ui->screen_cleaning_img_2, 200, 358);
	lv_obj_set_size(ui->screen_cleaning_img_2, 200, 200);

	//Write style for screen_cleaning_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_cleaning_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_time
	ui->screen_cleaning_time = lv_label_create(ui->screen_cleaning);
	lv_label_set_text(ui->screen_cleaning_time, "清运中");
	lv_label_set_long_mode(ui->screen_cleaning_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaning_time, 210, 429);
	lv_obj_set_size(ui->screen_cleaning_time, 180, 59);

	//Write style for screen_cleaning_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaning_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaning_time, &lv_font_SourceHanSerifCN2_54, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaning_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaning_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaning_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaning_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_label_2
	ui->screen_cleaning_label_2 = lv_label_create(ui->screen_cleaning);
	lv_label_set_text(ui->screen_cleaning_label_2, "当前重量");
	lv_label_set_long_mode(ui->screen_cleaning_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaning_label_2, 33, 599);
	lv_obj_set_size(ui->screen_cleaning_label_2, 243, 58);

	//Write style for screen_cleaning_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaning_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaning_label_2, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaning_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaning_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaning_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaning_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_weigth
	ui->screen_cleaning_weigth = lv_label_create(ui->screen_cleaning);
	lv_label_set_text(ui->screen_cleaning_weigth, "000.00KG");
	lv_label_set_long_mode(ui->screen_cleaning_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaning_weigth, 258, 601);
	lv_obj_set_size(ui->screen_cleaning_weigth, 319, 49);

	//Write style for screen_cleaning_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaning_weigth, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaning_weigth, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaning_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaning_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaning_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaning_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_label_3
	ui->screen_cleaning_label_3 = lv_label_create(ui->screen_cleaning);
	lv_label_set_text(ui->screen_cleaning_label_3, "清运操作");
	lv_label_set_long_mode(ui->screen_cleaning_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaning_label_3, 193, 27);
	lv_obj_set_size(ui->screen_cleaning_label_3, 215, 58);

	//Write style for screen_cleaning_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaning_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaning_label_3, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaning_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaning_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaning_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaning_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaning_settlement
	ui->screen_cleaning_settlement = lv_btn_create(ui->screen_cleaning);
	ui->screen_cleaning_settlement_label = lv_label_create(ui->screen_cleaning_settlement);
	lv_label_set_text(ui->screen_cleaning_settlement_label, "去结算并上传数据");
	lv_label_set_long_mode(ui->screen_cleaning_settlement_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_cleaning_settlement_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_cleaning_settlement, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_cleaning_settlement_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_cleaning_settlement, 145, 865);
	lv_obj_set_size(ui->screen_cleaning_settlement, 310, 60);

	//Write style for screen_cleaning_settlement, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_cleaning_settlement, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cleaning_settlement, lv_color_hex(0x3ea500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cleaning_settlement, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cleaning_settlement, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaning_settlement, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaning_settlement, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaning_settlement, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaning_settlement, &lv_font_SourceHanSerifCN2_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaning_settlement, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaning_settlement, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_cleaning.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_cleaning);

	//Init events for screen.
	events_init_screen_cleaning(ui);
}
