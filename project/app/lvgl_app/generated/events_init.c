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
#include "capture.h"
#include "custom.h"

extern int settlement,password_2,password_3,password_1,rod_hinder_value,peels_en,cal_en,server_connect,full,key_press,key_press_end;
extern video_device_t out_video_device;
extern video_device_t in_video_device;
#include "server.h"
extern int phone_login_flag;
extern char key_input[5];
extern int key_input_count;

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
static void screen_login_img_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		phone_login_flag=1;
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='1';
	  key_input_count++;
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_2_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		printf("boom\n");
	key_input[key_input_count]='2';
	key_input_count++;
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_7_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='7';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_4_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='4';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_exit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]=0x12;
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_8_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='8';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_5_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='5';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_0_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='0';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_del_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]=0x13;
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_9_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='9';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_6_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='6';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_3_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]='3';
		key_input_count++;
	
		break;
	}
	default:
		break;
	}
}
static void screen_login_key_enter_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_input[key_input_count]=0x11;
		key_input_count++;
	
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
	lv_obj_add_event_cb(ui->screen_login_img_1, screen_login_img_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_1, screen_login_key_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_2, screen_login_key_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_7, screen_login_key_7_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_4, screen_login_key_4_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_exit, screen_login_key_exit_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_8, screen_login_key_8_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_5, screen_login_key_5_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_0, screen_login_key_0_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_del, screen_login_key_del_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_9, screen_login_key_9_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_6, screen_login_key_6_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_3, screen_login_key_3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_login_key_enter, screen_login_key_enter_event_handler, LV_EVENT_ALL, ui);
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
static void screen_cleaning_settlement_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		settlement=1;
		break;
	}
	default:
		break;
	}
}
void events_init_screen_cleaning(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_cleaning_settlement, screen_cleaning_settlement_event_handler, LV_EVENT_ALL, ui);
}
static void screen_test_rod_push_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_press=1;
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
		key_press=1;
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
		key_press=1;
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
		key_press=1;
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
		key_press=1;
		inside_light(1);
		in_video_device.test_flag=1;
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test_cam, guider_ui.screen_test_cam_del, &guider_ui.screen_test_del, setup_scr_screen_test_cam, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, true);
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
		key_press=1;
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test_cam, guider_ui.screen_test_cam_del, &guider_ui.screen_test_del, setup_scr_screen_test_cam, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
	out_video_device.test_flag=1;
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
		key_press=1;
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
		key_press=1;
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
static void screen_test_cal_2_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_press=1;
	if(peels_en==1){
	  if(cal_en==0){
	    lv_obj_set_style_bg_color(guider_ui.screen_test_cal_2_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	    set_rat_weight(2.00);
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
		key_press=1;
	if(peels_en==1){
	  if(cal_en==1){
	    double weight=get_current_weight();
	    char str[10];
	    if (isinf(weight) || isnan(weight)) {
	    	sprintf(str,"校准失败");
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
		key_press_end=1;
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
		key_press=1;
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
		key_press=1;
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
		key_press=1;
	if(peels_en==0){
	  peels_en=1;
	  lv_obj_set_style_bg_color(guider_ui.screen_test_peels_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_2_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_25_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_100_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  get_self_weight();
	}else{
	  lv_obj_set_style_bg_color(guider_ui.screen_test_peels_key, lv_color_hex(0x65b763), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_2_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_25_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
	  lv_obj_set_style_bg_color(guider_ui.screen_test_cal_100_key, lv_color_hex(0x969696), LV_PART_MAIN|LV_STATE_DEFAULT);
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
static void screen_test_cal_100_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_press=1;
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
static void screen_test_cal_25_key_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		key_press=1;
	if(peels_en==1){
	  if(cal_en==0){
	    lv_obj_set_style_bg_color(guider_ui.screen_test_cal_25_key, lv_color_hex(0x345e33), LV_PART_MAIN|LV_STATE_DEFAULT);
	    set_rat_weight(25.00);
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
	lv_obj_add_event_cb(ui->screen_test_cal_2_key, screen_test_cal_2_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_get_weight_key, screen_test_get_weight_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_quit, screen_test_quit_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_open_lock_key, screen_test_open_lock_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_ir_status_key, screen_test_ir_status_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_peels_key, screen_test_peels_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_cal_100_key, screen_test_cal_100_key_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_test_cal_25_key, screen_test_cal_25_key_event_handler, LV_EVENT_ALL, ui);
}
static void screen_test_cam_btn_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		out_video_device.test_flag=0;
	in_video_device.test_flag=0;
		inside_light(0);
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_test, guider_ui.screen_test_del, &guider_ui.screen_test_cam_del, setup_scr_screen_test, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, true);
		set_screen_test_rod_hinder_value(rod_hinder_value);
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
