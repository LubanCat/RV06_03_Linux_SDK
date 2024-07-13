// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "custom.h"

// 关闭仓门
int close_door_flag;
// 继续投递
int post_flag;
//退出按钮
int quit_flag;
//打开清理门
int open_clean_door_flag;
//手动模拟关闭清理门
int hand_close_door_flag;
// 在登录页面
int screen_login_flag;
extern int rod_hinder_value;

int peels_en=0;
int cal_en=0;

void device_close_door(){
    printf("close door\n");
}

void device_open_clean_door(){
  printf("open clean door\n");
}

void set_lv_label_text_float(lv_obj_t * name,double value){
    char str[10];
    sprintf(str,"%.2f",value);
    lv_label_set_text(name,str);
}

void set_lv_label_text_int(lv_obj_t * name,int value){
    lv_label_set_text_fmt(name, "%d", value);
}
 
//screen_login
void set_screen_login_price(double price){
    set_lv_label_text_float(guider_ui.screen_login_price,price);
}

void set_screen_login_weigth(double weigth){
    set_lv_label_text_float(guider_ui.screen_login_weigth,weigth);
}

void set_screen_login_clean_time(char *time){
    lv_label_set_text_fmt(guider_ui.screen_login_clean_time, "%s", time);
}

void set_screen_login_net_status(int singal){
    int net_status;

    if (singal == 99) {
        net_status = 0;
    } else {
        net_status = 1;
    }

    if(net_status){
        lv_label_set_text(guider_ui.screen_login_net_status, "已连网");
        set_lv_label_text_int(guider_ui.screen_login_singal,singal);
    }else{
        lv_label_set_text(guider_ui.screen_login_net_status, "无网络");
        set_lv_label_text_int(guider_ui.screen_login_singal,singal);
    }
}


void set_screen_login_server_status(int server_status){
    if(server_status){
        lv_label_set_text(guider_ui.screen_login_server_status, "已连接");
    }else{
        lv_label_set_text(guider_ui.screen_login_server_status, "未连接");
    }
}

void set_screen_login_qrcode(char *qrcode){
    lv_qrcode_update(guider_ui.screen_login_qrcode, qrcode, strlen(qrcode));
}

//screen_full
void set_screen_full_price(double price){
    set_lv_label_text_float(guider_ui.screen_full_price,price);
}

void set_screen_full_time(int time){
    set_lv_label_text_int(guider_ui.screen_full_time,time);
}

//screen_submit
void set_screen_submit_money(double money){
    set_lv_label_text_float(guider_ui.screen_submit_money,money);
}

void set_screen_submit_time(int time){
    set_lv_label_text_int(guider_ui.screen_submit_time,time);
}

void set_screen_submit_weigth(double weigth){
    set_lv_label_text_float(guider_ui.screen_submit_weigth,weigth);
}

void set_screen_submit_video(char *video){
}

//screen_clean
void set_screen_clean_time(int time){
    set_lv_label_text_int(guider_ui.screen_clean_time,time);
}

void set_screen_clean_weigth(double weigth){
    set_lv_label_text_float(guider_ui.screen_clean_weigth,weigth);
}

//screen_cleaning
void set_screen_cleaning_weigth(double weigth){
    set_lv_label_text_float(guider_ui.screen_cleaning_weigth,weigth);
}

//screen_cleaned
void set_screen_cleaned_weigth(double before_clean_weigth,double after_clean_weigth,double clean_weigth){
    set_lv_label_text_float(guider_ui.screen_cleaned_before_clean_weigth,before_clean_weigth);
    set_lv_label_text_float(guider_ui.screen_cleaned_after_clean_weigth,after_clean_weigth);
    set_lv_label_text_float(guider_ui.screen_cleaned_clean_weigth,clean_weigth);
}

void set_screen_cleaned_time(int time){
    set_lv_label_text_int(guider_ui.screen_cleaned_time,time);
}

void set_screen_test_rod_hinder_value(int rod_hinder_value){
    set_lv_label_text_int(guider_ui.screen_test_rod_hinder_value,rod_hinder_value);
}

void set_screen_login(bool *screen_del,double price,double weigth,char *time,char *qrcode,int singal,int server_status){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_login, 
                        guider_ui.screen_login_del, screen_del, 
                        setup_scr_screen_login, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_login_price(price);
    set_screen_login_weigth(weigth);
    set_screen_login_clean_time(time);
    set_screen_login_net_status(singal);
    set_screen_login_qrcode(qrcode);
    set_screen_login_server_status(server_status);
    // 当在登录页面时，开启其他线程在登录页面的更新
    screen_login_flag = 1;
}

void set_screen_full(bool *screen_del,double price,int time){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_full, 
                        guider_ui.screen_full_del, screen_del, 
                        setup_scr_screen_full, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_full_price(price);
    set_screen_full_time(time);
}

void set_screen_submit(bool *screen_del,double money,int time,double weigth){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_submit, 
                        guider_ui.screen_submit_del, screen_del, 
                        setup_scr_screen_submit, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_submit_money(money);
    set_screen_submit_time(time);
    set_screen_submit_weigth(weigth);
}

void set_screen_clean(bool *screen_del,int time,double weigth){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_clean, 
                        guider_ui.screen_clean_del, screen_del, 
                        setup_scr_screen_clean, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_clean_time(time);
    set_screen_clean_weigth(weigth);
}

void set_screen_cleaning(bool *screen_del,double weigth){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_cleaning, 
                        guider_ui.screen_cleaning_del, screen_del, 
                        setup_scr_screen_cleaning, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_cleaning_weigth(weigth);
}

void set_screen_cleaned(bool *screen_del,int time,double before_clean_weigth,double after_clean_weigth,double clean_weigth){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_cleaned, 
                        guider_ui.screen_cleaned_del, screen_del, 
                        setup_scr_screen_cleaned, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);

    set_screen_cleaned_weigth(before_clean_weigth,after_clean_weigth,clean_weigth);
    set_screen_cleaned_time(time);
}

void set_screen_test(bool *screen_del){
    ui_load_scr_animation(&guider_ui, &guider_ui.screen_test, 
                        guider_ui.screen_test_del, screen_del, 
                        setup_scr_screen_test, LV_SCR_LOAD_ANIM_NONE, 
                        200, 200, true, true);
    char str[20];
    sprintf(str,"%d",rod_hinder_value);
    lv_label_set_text(guider_ui.screen_test_rod_hinder_value,str);
}

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

