// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

// 关闭仓门
extern int close_door_flag;
// 继续投递
extern int post_flag;
//退出按钮
extern int quit_flag;
//打开清理门
extern int open_clean_door_flag;
//手动模拟关闭清理门
extern int hand_close_door_flag;
// 在登录页面
extern int screen_login_flag;

void device_close_door();
void device_open_clean_door();
void set_lv_label_text_float(lv_obj_t * name,double value);
void set_lv_label_text_int(lv_obj_t * name,int value);
void set_screen_login_price(double price);
void set_screen_login_weigth(double weigth);
void set_screen_login_clean_time(char *time);
void set_screen_login_net_status(int singal);
void set_screen_login_qrcode(char *qrcode);
void set_screen_login_server_status(int server_status);
void set_screen_login_updating(int updating);
void set_screen_full_price(double price);
void set_screen_full_time(int time);
void set_screen_submit_money(double money);
void set_screen_submit_time(int time);
void set_screen_submit_weigth(double weigth);
void set_screen_submit_video(char *video);
void set_screen_clean_time(int time);
void set_screen_clean_weigth(double weigth);
void set_screen_cleaning_weigth(double weigth);
void set_screen_cleaned_weigth(double before_clean_weigth,double after_clean_weigth,double clean_weigth);
void set_screen_cleaned_time(int time);
void set_screen_test_rod_hinder_value(int rod_hinder_value);

void set_screen_login(bool *screen_del,double price,double weigth,char *time,char *qrcode,int singal,int server_status);
void set_screen_full(bool *screen_del,double price,int time);
void set_screen_submit(bool *screen_del,double money,int time,double weigth);
void set_screen_clean(bool *screen_del,int time,double weigth);
void set_screen_cleaning(bool *screen_del,double weigth);
void set_screen_cleaned(bool *screen_del,int time,double before_clean_weigth,double after_clean_weigth,double clean_weigth);
void set_screen_test(bool *screen_del);
void custom_init(lv_ui *ui);




#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
