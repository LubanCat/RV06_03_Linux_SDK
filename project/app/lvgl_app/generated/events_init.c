/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


#include "hardware.h"
#include "custom.h"

extern int password_2,password_3,password_1,rod_hinder_value,peels_en,cal_en,server_connect;

#include "server.h"
static void screen_login_password_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		password_1=1;
		break;
	}
	default:
		break;
	}
}
static void screen_login_password_2_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		password_2=1;
		break;
	}
	default:
		break;
	}
}
static void screen_login_password_3_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		password_3=1;
		break;
	}
	default:
		break;
	}
}
void events_init_screen_login(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_login_password_1, screen_login_password_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_password_2, screen_login_password_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_password_3, screen_login_password_3_event_handler, LV_EVENT_ALL, ui);
}
static void screen_submit_post_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		post_flag=1;
		break;
	}
	default:
		break;
	}
}
static void screen_submit_close_door_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		close_door_flag=1;
		break;
	}
	default:
		break;
	}
}
void events_init_screen_submit(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_submit_post, screen_submit_post_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_submit_close_door, screen_submit_close_door_event_handler, LV_EVENT_ALL, ui);
}
static void screen_clean_open_clean_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		open_clean_door_flag=1;
		break;
	}
	default:
		break;
	}
}
static void screen_clean_quit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		quit_flag = 1;
		break;
	}
	default:
		break;
	}
}
void events_init_screen_clean(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_clean_open_clean, screen_clean_open_clean_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_clean_quit, screen_clean_quit_event_handler, LV_EVENT_ALL, ui);
}
static void screen_test_rod_push_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_bg_color(guider_ui.screen_test_rod_push_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	set_post_door_open();
	lv_obj_set_style_bg_color(guider_ui.screen_test_rod_push_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	
	
	
		break;
	}
	default:
		break;
	}
}
static void screen_test_rod_pull_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_bg_color(guider_ui.screen_test_rod_pull_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	set_post_door_close();
	lv_obj_set_style_bg_color(guider_ui.screen_test_rod_pull_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	
		break;
	}
	default:
		break;
	}
}
static void screen_test_inside_light_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_bg_color(guider_ui.screen_test_inside_light_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	inside_light(1);
	sleep(5);
	inside_light(0);
	lv_obj_set_style_bg_color(guider_ui.screen_test_inside_light_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
		break;
	}
	default:
		break;
	}
}
static void screen_test_outside_light_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_bg_color(guider_ui.screen_test_outside_light_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	outside_light(1);
	sleep(5);
	outside_light(0);
	lv_obj_set_style_bg_color(guider_ui.screen_test_outside_light_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
		break;
	}
	default:
		break;
	}
}
static void screen_test_inside_cam_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		camara_test_start(1);
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test_cam, guider_ui.screen_test_cam_del, &guider_ui.screen_test_del, setup_scr_screen_test_cam, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
		break;
	}
	default:
		break;
	}
}
static void screen_test_outside_cam_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test_cam, guider_ui.screen_test_cam_del, &guider_ui.screen_test_del, setup_scr_screen_test_cam, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
		camara_test_start(2);
		break;
	}
	default:
		break;
	}
}
static void screen_test_press_up_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		rod_hinder_value+=5;
	char str[10];
	sprintf(str,"%d",rod_hinder_value);
	lv_label_set_text(guider_ui.screen_test_rod_hinder_value,str);
		break;
	}
	default:
		break;
	}
}
static void screen_test_press_down_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		rod_hinder_value-=5;
	char str[10];
	sprintf(str,"%d",rod_hinder_value);
	lv_label_set_text(guider_ui.screen_test_rod_hinder_value,str);
		break;
	}
	default:
		break;
	}
}
static void screen_test_cal_100_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if(peels_en==1){
	  if(cal_en==0){
	    lv_obj_set_style_bg_color(guider_ui.screen_test_cal_100_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	    set_rat_weight(100.00);
	    lv_obj_set_style_bg_color(guider_ui.screen_test_get_weight_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	    cal_en=1;
	  }
	
	}else{
	  cal_en=0;
	}
		break;
	}
	default:
		break;
	}
}
static void screen_test_get_weight_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if(peels_en==1){
	  if(cal_en==1){
	    double weight=get_current_weight();
	    char str[10];

		if (isinf(weight) || isnan(weight)) {
			sprintf(str,"校准错误");
		} else {
			sprintf(str,"%.2fkg",weight);
		}
	    lv_label_set_text(guider_ui.screen_test_weight,str);
	  }
	}
	
	
	
	
		break;
	}
	default:
		break;
	}
}
static void screen_test_quit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		double cur_weight= get_current_weight();
		set_screen_login(&guider_ui.screen_test_del, loginRes.recyclePrice, cur_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal,server_connect);
		char str[20];
		sprintf(str, "%d", rod_hinder_value);
		write_device_info("rod_hinder",str);
        set_screen_login_qrcode(loginRes.qrCode);
		screen_login_flag = 1;
		break;
	}
	default:
		break;
	}
}
static void screen_test_open_lock_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		set_clear_door_open();
		break;
	}
	default:
		break;
	}
}
static void screen_test_ir_status_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if (rk_gpio_get_value(GPIO_IR1_STATUS) == 0)
	  lv_label_set_text(guider_ui.screen_test_ir1_status,"1");
	else
	  lv_label_set_text(guider_ui.screen_test_ir1_status,"0");
	
	if (rk_gpio_get_value(GPIO_IR2_STATUS) == 0)
	  lv_label_set_text(guider_ui.screen_test_ir2_status,"1");
	else
	  lv_label_set_text(guider_ui.screen_test_ir2_status,"0");
		break;
	}
	default:
		break;
	}
}
static void screen_test_peels_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if(peels_en==0){
	  peels_en=1;
	  lv_obj_set_style_bg_color(guider_ui.screen_test_peels_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_100_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_200_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_350_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  get_self_weight();
	}else{
	  lv_obj_set_style_bg_color(guider_ui.screen_test_peels_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_100_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_200_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_350_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_get_weight_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  peels_en=0;
	  cal_en=0;
	}
	
	
		break;
	}
	default:
		break;
	}
}
static void screen_test_cal_350_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if(peels_en==1){
	  if(cal_en==0){
	    lv_obj_set_style_bg_color(guider_ui.screen_test_cal_350_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	    set_rat_weight(350.00);
	    lv_obj_set_style_bg_color(guider_ui.screen_test_get_weight_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	    cal_en=1;
	  }
	}else{
	  cal_en=0;
	}
		break;
	}
	default:
		break;
	}
}
static void screen_test_cal_200_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		if(peels_en==1){
	  if(cal_en==0){
	    lv_obj_set_style_bg_color(guider_ui.screen_test_cal_200_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	    set_rat_weight(200.00);
	    lv_obj_set_style_bg_color(guider_ui.screen_test_get_weight_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	    cal_en=1;
	  }
	}else{
	  cal_en=0;
	}
		break;
	}
	default:
		break;
	}
}
void events_init_screen_test(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_test_rod_push_key, screen_test_rod_push_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_rod_pull_key, screen_test_rod_pull_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_inside_light_key, screen_test_inside_light_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_outside_light_key, screen_test_outside_light_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_inside_cam_key, screen_test_inside_cam_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_outside_cam_key, screen_test_outside_cam_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_press_up_key, screen_test_press_up_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_press_down_key, screen_test_press_down_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_cal_100_key, screen_test_cal_100_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_get_weight_key, screen_test_get_weight_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_quit, screen_test_quit_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_open_lock_key, screen_test_open_lock_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_ir_status_key, screen_test_ir_status_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_peels_key, screen_test_peels_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_cal_350_key, screen_test_cal_350_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_cal_200_key, screen_test_cal_200_key_event_handler, LV_EVENT_ALL, ui);
}
static void screen_test_cam_btn_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		camara_test_stop();
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test, guider_ui.screen_test_del, &guider_ui.screen_test_cam_del, setup_scr_screen_test, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
		break;
	}
	default:
		break;
	}
}
void events_init_screen_test_cam(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_test_cam_btn_1, screen_test_cam_btn_1_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{

}
