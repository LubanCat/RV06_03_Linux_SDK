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



void setup_scr_screen_cleaned(lv_ui *ui)
{
	//Write codes screen_cleaned
	ui->screen_cleaned = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen_cleaned, 600, 1024);
	lv_obj_set_scrollbar_mode(ui->screen_cleaned, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cleaned, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_cleaned, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cleaned, lv_color_hex(0xf5f5f5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cleaned, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_img_2
	ui->screen_cleaned_img_2 = lv_img_create(ui->screen_cleaned);
	lv_obj_add_flag(ui->screen_cleaned_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_cleaned_img_2, &_cycle_alpha_200x200);
	lv_img_set_pivot(ui->screen_cleaned_img_2, 50,50);
	lv_img_set_angle(ui->screen_cleaned_img_2, 0);
	lv_obj_set_pos(ui->screen_cleaned_img_2, 200, 358);
	lv_obj_set_size(ui->screen_cleaned_img_2, 200, 200);

	//Write style for screen_cleaned_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_cleaned_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_time
	ui->screen_cleaned_time = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_time, "10");
	lv_label_set_long_mode(ui->screen_cleaned_time, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_time, 210, 408);
	lv_obj_set_size(ui->screen_cleaned_time, 180, 100);

	//Write style for screen_cleaned_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_time, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_time, &lv_font_SourceHanSerifCN2_96, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_label_2
	ui->screen_cleaned_label_2 = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_label_2, "清运前重量");
	lv_label_set_long_mode(ui->screen_cleaned_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_label_2, 42, 647);
	lv_obj_set_size(ui->screen_cleaned_label_2, 243, 58);

	//Write style for screen_cleaned_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_label_2, &lv_font_SourceHanSerifCN2_42, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_before_clean_weigth
	ui->screen_cleaned_before_clean_weigth = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_before_clean_weigth, "000.00KG");
	lv_label_set_long_mode(ui->screen_cleaned_before_clean_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_before_clean_weigth, 284, 640);
	lv_obj_set_size(ui->screen_cleaned_before_clean_weigth, 308, 49);

	//Write style for screen_cleaned_before_clean_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_before_clean_weigth, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_before_clean_weigth, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_before_clean_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_before_clean_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_before_clean_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_before_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_label_3
	ui->screen_cleaned_label_3 = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_label_3, "清运完成");
	lv_label_set_long_mode(ui->screen_cleaned_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_label_3, 193, 27);
	lv_obj_set_size(ui->screen_cleaned_label_3, 215, 58);

	//Write style for screen_cleaned_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_label_3, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_after_clean_weigth
	ui->screen_cleaned_after_clean_weigth = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_after_clean_weigth, "000.00KG");
	lv_label_set_long_mode(ui->screen_cleaned_after_clean_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_after_clean_weigth, 313, 701);
	lv_obj_set_size(ui->screen_cleaned_after_clean_weigth, 255, 50);

	//Write style for screen_cleaned_after_clean_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_after_clean_weigth, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_after_clean_weigth, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_after_clean_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_after_clean_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_after_clean_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_after_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_label_4
	ui->screen_cleaned_label_4 = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_label_4, "清运后重量");
	lv_label_set_long_mode(ui->screen_cleaned_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_label_4, 42, 701);
	lv_obj_set_size(ui->screen_cleaned_label_4, 243, 58);

	//Write style for screen_cleaned_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_label_4, &lv_font_SourceHanSerifCN2_42, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_clean_weigth
	ui->screen_cleaned_clean_weigth = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_clean_weigth, "000.00KG");
	lv_label_set_long_mode(ui->screen_cleaned_clean_weigth, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_clean_weigth, 310, 759);
	lv_obj_set_size(ui->screen_cleaned_clean_weigth, 262, 49);

	//Write style for screen_cleaned_clean_weigth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_clean_weigth, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_clean_weigth, &lv_font_SourceHanSerifCN2_48, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_clean_weigth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_clean_weigth, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_clean_weigth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_clean_weigth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cleaned_label_6
	ui->screen_cleaned_label_6 = lv_label_create(ui->screen_cleaned);
	lv_label_set_text(ui->screen_cleaned_label_6, "清运重量");
	lv_label_set_long_mode(ui->screen_cleaned_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_cleaned_label_6, 67, 759);
	lv_obj_set_size(ui->screen_cleaned_label_6, 243, 58);

	//Write style for screen_cleaned_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cleaned_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cleaned_label_6, &lv_font_SourceHanSerifCN2_42, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_cleaned_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_cleaned_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cleaned_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cleaned_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_cleaned.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen_cleaned);

}
