/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "custom.h"
#include "events_init.h"
#include "gui_guider.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include <time.h>

static int screen_digital_clock_hour_value = 11;
static int screen_digital_clock_min_value = 25;
static int screen_digital_clock_sec_value = 0;

int update_digital_clock() {
  time_t current_time = time(NULL);
  struct tm *local_time = localtime(&current_time);

  screen_digital_clock_hour_value = local_time->tm_hour;
  screen_digital_clock_min_value = local_time->tm_min;
  screen_digital_clock_sec_value = local_time->tm_sec;

  return 0;
}

int update_date() {
  time_t current_time = time(NULL);
  struct tm *local_time = localtime(&current_time);

  char year[5];
  char month[3];
  char day[3];
  strftime(year, sizeof(year), "%Y", local_time);
  strftime(month, sizeof(month), "%m", local_time);
  strftime(day, sizeof(day), "%d", local_time);

  lv_label_set_text_fmt(guider_ui.screen_label_1, "%s年%s月%s日", year, month,
                        day);

  return 0;
}

void screen_digital_clock_timer(lv_timer_t *timer) {
  update_digital_clock();
  if (lv_obj_is_valid(guider_ui.screen_digital_clock)) {
    lv_label_set_text_fmt(guider_ui.screen_digital_clock, "%02d:%02d",
                          screen_digital_clock_hour_value,
                          screen_digital_clock_min_value);
    update_date();
  }
}
static const lv_img_dsc_t *screen_animimg_1_imgs[48] = {
    &screen_animimg_1taikongren_00, &screen_animimg_1taikongren_01,
    &screen_animimg_1taikongren_02, &screen_animimg_1taikongren_03,
    &screen_animimg_1taikongren_04, &screen_animimg_1taikongren_05,
    &screen_animimg_1taikongren_06, &screen_animimg_1taikongren_07,
    &screen_animimg_1taikongren_08, &screen_animimg_1taikongren_09,
    &screen_animimg_1taikongren_10, &screen_animimg_1taikongren_11,
    &screen_animimg_1taikongren_12, &screen_animimg_1taikongren_13,
    &screen_animimg_1taikongren_14, &screen_animimg_1taikongren_15,
    &screen_animimg_1taikongren_16, &screen_animimg_1taikongren_17,
    &screen_animimg_1taikongren_18, &screen_animimg_1taikongren_19,
    &screen_animimg_1taikongren_20, &screen_animimg_1taikongren_21,
    &screen_animimg_1taikongren_22, &screen_animimg_1taikongren_23,
    &screen_animimg_1taikongren_24, &screen_animimg_1taikongren_25,
    &screen_animimg_1taikongren_26, &screen_animimg_1taikongren_27,
    &screen_animimg_1taikongren_28, &screen_animimg_1taikongren_29,
    &screen_animimg_1taikongren_30, &screen_animimg_1taikongren_31,
    &screen_animimg_1taikongren_32, &screen_animimg_1taikongren_33,
    &screen_animimg_1taikongren_34, &screen_animimg_1taikongren_35,
    &screen_animimg_1taikongren_36, &screen_animimg_1taikongren_37,
    &screen_animimg_1taikongren_38, &screen_animimg_1taikongren_39,
    &screen_animimg_1taikongren_40, &screen_animimg_1taikongren_41,
    &screen_animimg_1taikongren_42, &screen_animimg_1taikongren_43,
    &screen_animimg_1taikongren_44, &screen_animimg_1taikongren_45,
    &screen_animimg_1taikongren_46, &screen_animimg_1taikongren_47};

void setup_scr_screen(lv_ui *ui) {

  // Write codes screen
  ui->screen = lv_obj_create(NULL);

  // Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
  static lv_style_t style_screen_main_main_default;
  if (style_screen_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_main_main_default);
  else
    lv_style_init(&style_screen_main_main_default);
  lv_style_set_bg_color(&style_screen_main_main_default,
                        lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_opa(&style_screen_main_main_default, 0);
  lv_obj_add_style(ui->screen, &style_screen_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_tileview_main
  ui->screen_tileview_main = lv_tileview_create(ui->screen);
  lv_obj_set_pos(ui->screen_tileview_main, 0, 0);
  lv_obj_set_size(ui->screen_tileview_main, 480, 480);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_tileview_main_main_main_default
  static lv_style_t style_screen_tileview_main_main_main_default;
  if (style_screen_tileview_main_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_tileview_main_main_main_default);
  else
    lv_style_init(&style_screen_tileview_main_main_main_default);
  lv_style_set_radius(&style_screen_tileview_main_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_tileview_main_main_main_default,
                        lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_color(&style_screen_tileview_main_main_main_default,
                             lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_dir(&style_screen_tileview_main_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_tileview_main_main_main_default, 255);
  lv_obj_add_style(ui->screen_tileview_main,
                   &style_screen_tileview_main_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_tileview_main_main_scrollbar_default
  static lv_style_t style_screen_tileview_main_main_scrollbar_default;
  if (style_screen_tileview_main_main_scrollbar_default.prop_cnt > 1)
    lv_style_reset(&style_screen_tileview_main_main_scrollbar_default);
  else
    lv_style_init(&style_screen_tileview_main_main_scrollbar_default);
  lv_style_set_radius(&style_screen_tileview_main_main_scrollbar_default, 0);
  lv_style_set_bg_color(&style_screen_tileview_main_main_scrollbar_default,
                        lv_color_make(0xea, 0xef, 0xf3));
  lv_style_set_bg_opa(&style_screen_tileview_main_main_scrollbar_default, 255);
  lv_obj_add_style(ui->screen_tileview_main,
                   &style_screen_tileview_main_main_scrollbar_default,
                   LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

  // add new tile screen_tileview_main_tileview
  ui->screen_tileview_main_tileview =
      lv_tileview_add_tile(ui->screen_tileview_main, 0, 0, LV_DIR_RIGHT);
  static bool screen_digital_clock_timer_enabled = false;

  // Write codes screen_digital_clock
  ui->screen_digital_clock = lv_label_create(ui->screen_tileview_main_tileview);
  lv_obj_set_style_text_align(ui->screen_digital_clock, LV_TEXT_ALIGN_CENTER,
                              0);
  lv_obj_set_pos(ui->screen_digital_clock, 40, 60);
  lv_obj_set_size(ui->screen_digital_clock, 220, 120);
  lv_label_set_text_fmt(guider_ui.screen_digital_clock, "%02d:%02d",
                        screen_digital_clock_hour_value,
                        screen_digital_clock_min_value);

  // create timer
  if (!screen_digital_clock_timer_enabled) {
    lv_timer_create(screen_digital_clock_timer, 1000, NULL);
    screen_digital_clock_timer_enabled = true;
  }
  // Write style state: LV_STATE_DEFAULT for
  // style_screen_digital_clock_main_main_default
  static lv_style_t style_screen_digital_clock_main_main_default;
  if (style_screen_digital_clock_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_digital_clock_main_main_default);
  else
    lv_style_init(&style_screen_digital_clock_main_main_default);
  lv_style_set_radius(&style_screen_digital_clock_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_digital_clock_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_digital_clock_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_digital_clock_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_digital_clock_main_main_default, 0);
  lv_style_set_text_color(&style_screen_digital_clock_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_digital_clock_main_main_default,
                         &lv_font_Alatsi_Regular_80);
  lv_style_set_text_letter_space(&style_screen_digital_clock_main_main_default,
                                 0);
  lv_style_set_pad_left(&style_screen_digital_clock_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_digital_clock_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_digital_clock_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_digital_clock_main_main_default, 0);
  lv_obj_add_style(ui->screen_digital_clock,
                   &style_screen_digital_clock_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_label_3
  ui->screen_label_3 = lv_label_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_label_3, 5, 5);
  lv_obj_set_size(ui->screen_label_3, 100, 32);
  lv_label_set_text(ui->screen_label_3, "HinLink");
  lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_3_main_main_default
  static lv_style_t style_screen_label_3_main_main_default;
  if (style_screen_label_3_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_3_main_main_default);
  else
    lv_style_init(&style_screen_label_3_main_main_default);
  lv_style_set_radius(&style_screen_label_3_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_3_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_3_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_3_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_3_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_3_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_3_main_main_default,
                         &lv_font_Alatsi_Regular_14);
  lv_style_set_text_letter_space(&style_screen_label_3_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_3_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_3_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_3_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_3_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_3, &style_screen_label_3_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_label_5
  ui->screen_label_5 = lv_label_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_label_5, 390, 5);
  lv_obj_set_size(ui->screen_label_5, 85, 32);
  lv_label_set_text(ui->screen_label_5, "SoloLinker");
  lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_5_main_main_default
  static lv_style_t style_screen_label_5_main_main_default;
  if (style_screen_label_5_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_5_main_main_default);
  else
    lv_style_init(&style_screen_label_5_main_main_default);
  lv_style_set_radius(&style_screen_label_5_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_5_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_5_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_5_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_5_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_5_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_5_main_main_default,
                         &lv_font_Alatsi_Regular_14);
  lv_style_set_text_letter_space(&style_screen_label_5_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_5_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_5_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_5_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_5_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_5, &style_screen_label_5_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_imgbtn_5
  ui->screen_imgbtn_5 = lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_5, 290, 226);
  lv_obj_set_size(ui->screen_imgbtn_5, 60, 60);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_5_main_main_default
  static lv_style_t style_screen_imgbtn_5_main_main_default;
  if (style_screen_imgbtn_5_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_5_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_5_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_5_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_5_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_5_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_5_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_5,
                   &style_screen_imgbtn_5_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_5_main_main_pressed
  static lv_style_t style_screen_imgbtn_5_main_main_pressed;
  if (style_screen_imgbtn_5_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_5_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_5_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_5_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_5_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_5_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_5,
                   &style_screen_imgbtn_5_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_5_main_main_checked
  static lv_style_t style_screen_imgbtn_5_main_main_checked;
  if (style_screen_imgbtn_5_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_5_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_5_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_5_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_5_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_5_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_5,
                   &style_screen_imgbtn_5_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_5, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_btn_next_alpha_60x60, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_5, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_imgbtn_play
  ui->screen_imgbtn_play = lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_play, 190, 206);
  lv_obj_set_size(ui->screen_imgbtn_play, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_play_main_main_default
  static lv_style_t style_screen_imgbtn_play_main_main_default;
  if (style_screen_imgbtn_play_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_play_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_play_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_play_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_play_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_play_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_play_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_play,
                   &style_screen_imgbtn_play_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_play_main_main_pressed
  static lv_style_t style_screen_imgbtn_play_main_main_pressed;
  if (style_screen_imgbtn_play_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_play_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_play_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_play_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_play_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_play_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_play,
                   &style_screen_imgbtn_play_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_play_main_main_checked
  static lv_style_t style_screen_imgbtn_play_main_main_checked;
  if (style_screen_imgbtn_play_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_play_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_play_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_play_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_play_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_play_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_play,
                   &style_screen_imgbtn_play_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_play, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_btn_play_alpha_100x100, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_play, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_imgbtn_pre
  ui->screen_imgbtn_pre = lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_pre, 130, 228);
  lv_obj_set_size(ui->screen_imgbtn_pre, 60, 60);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_pre_main_main_default
  static lv_style_t style_screen_imgbtn_pre_main_main_default;
  if (style_screen_imgbtn_pre_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_pre_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_pre_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_pre_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_pre_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_pre_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_pre_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_pre,
                   &style_screen_imgbtn_pre_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_pre_main_main_pressed
  static lv_style_t style_screen_imgbtn_pre_main_main_pressed;
  if (style_screen_imgbtn_pre_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_pre_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_pre_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_pre_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_pre_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_pre_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_pre,
                   &style_screen_imgbtn_pre_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_pre_main_main_checked
  static lv_style_t style_screen_imgbtn_pre_main_main_checked;
  if (style_screen_imgbtn_pre_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_pre_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_pre_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_pre_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_pre_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_pre_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_pre,
                   &style_screen_imgbtn_pre_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_pre, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_btn_prev_alpha_60x60, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_pre, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_imgbtn_2
  ui->screen_imgbtn_2 = lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_2, 350, 349);
  lv_obj_set_size(ui->screen_imgbtn_2, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_2_main_main_default
  static lv_style_t style_screen_imgbtn_2_main_main_default;
  if (style_screen_imgbtn_2_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_2_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_2_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_2_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_2_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_2_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_2_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_2,
                   &style_screen_imgbtn_2_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_2_main_main_pressed
  static lv_style_t style_screen_imgbtn_2_main_main_pressed;
  if (style_screen_imgbtn_2_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_2_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_2_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_2_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_2_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_2_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_2,
                   &style_screen_imgbtn_2_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_2_main_main_checked
  static lv_style_t style_screen_imgbtn_2_main_main_checked;
  if (style_screen_imgbtn_2_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_2_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_2_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_2_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_2_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_2_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_2,
                   &style_screen_imgbtn_2_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_webcam_alpha_100x100, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_imgbtn_1
  ui->screen_imgbtn_1 = lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_1, 57, 354);
  lv_obj_set_size(ui->screen_imgbtn_1, 110, 110);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_1_main_main_default
  static lv_style_t style_screen_imgbtn_1_main_main_default;
  if (style_screen_imgbtn_1_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_1_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_1_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_1_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_1_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_1_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_1_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_1,
                   &style_screen_imgbtn_1_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_1_main_main_pressed
  static lv_style_t style_screen_imgbtn_1_main_main_pressed;
  if (style_screen_imgbtn_1_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_1_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_1_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_1_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_1_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_1_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_1,
                   &style_screen_imgbtn_1_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_1_main_main_checked
  static lv_style_t style_screen_imgbtn_1_main_main_checked;
  if (style_screen_imgbtn_1_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_1_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_1_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_1_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_1_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_1_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_1,
                   &style_screen_imgbtn_1_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_electric_outlet_alpha_110x110, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_label_1
  ui->screen_label_1 = lv_label_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_label_1, 70, 170);
  lv_obj_set_size(ui->screen_label_1, 140, 19);
  lv_label_set_text(ui->screen_label_1, "2023年10月30日");
  lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_1_main_main_default
  static lv_style_t style_screen_label_1_main_main_default;
  if (style_screen_label_1_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_1_main_main_default);
  else
    lv_style_init(&style_screen_label_1_main_main_default);
  lv_style_set_radius(&style_screen_label_1_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_1_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_1_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_1_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_1_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_1_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_1_main_main_default,
                         &lv_font_AlimamaShuHeiTi_Bold_15);
  lv_style_set_text_letter_space(&style_screen_label_1_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_1_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_1_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_1_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_1_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_1, &style_screen_label_1_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_slider_main
  ui->screen_slider_main = lv_slider_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_slider_main, 40, 300);
  lv_obj_set_size(ui->screen_slider_main, 400, 10);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_slider_main_main_main_default
  static lv_style_t style_screen_slider_main_main_main_default;
  if (style_screen_slider_main_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_slider_main_main_main_default);
  else
    lv_style_init(&style_screen_slider_main_main_main_default);
  lv_style_set_radius(&style_screen_slider_main_main_main_default, 50);
  lv_style_set_bg_color(&style_screen_slider_main_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_slider_main_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_slider_main_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_slider_main_main_main_default, 100);
  lv_style_set_outline_color(&style_screen_slider_main_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_outline_width(&style_screen_slider_main_main_main_default, 0);
  lv_style_set_outline_opa(&style_screen_slider_main_main_main_default, 255);
  lv_style_set_pad_left(&style_screen_slider_main_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_slider_main_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_slider_main_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_slider_main_main_main_default, 0);
  lv_obj_add_style(ui->screen_slider_main,
                   &style_screen_slider_main_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_slider_main_main_indicator_default
  static lv_style_t style_screen_slider_main_main_indicator_default;
  if (style_screen_slider_main_main_indicator_default.prop_cnt > 1)
    lv_style_reset(&style_screen_slider_main_main_indicator_default);
  else
    lv_style_init(&style_screen_slider_main_main_indicator_default);
  lv_style_set_radius(&style_screen_slider_main_main_indicator_default, 50);
  lv_style_set_bg_color(&style_screen_slider_main_main_indicator_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_slider_main_main_indicator_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_slider_main_main_indicator_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_slider_main_main_indicator_default, 255);
  lv_obj_add_style(ui->screen_slider_main,
                   &style_screen_slider_main_main_indicator_default,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_slider_main_main_knob_default
  static lv_style_t style_screen_slider_main_main_knob_default;
  if (style_screen_slider_main_main_knob_default.prop_cnt > 1)
    lv_style_reset(&style_screen_slider_main_main_knob_default);
  else
    lv_style_init(&style_screen_slider_main_main_knob_default);
  lv_style_set_radius(&style_screen_slider_main_main_knob_default, 50);
  lv_style_set_bg_color(&style_screen_slider_main_main_knob_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_slider_main_main_knob_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_slider_main_main_knob_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_slider_main_main_knob_default, 255);
  lv_obj_add_style(ui->screen_slider_main,
                   &style_screen_slider_main_main_knob_default,
                   LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_slider_set_range(ui->screen_slider_main, 0, 100);
  lv_slider_set_value(ui->screen_slider_main, 20, false);

  // Write codes screen_imgbtn_weather
  ui->screen_imgbtn_weather =
      lv_imgbtn_create(ui->screen_tileview_main_tileview);
  lv_obj_set_pos(ui->screen_imgbtn_weather, 280, 40);
  lv_obj_set_size(ui->screen_imgbtn_weather, 140, 140);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_weather_main_main_default
  static lv_style_t style_screen_imgbtn_weather_main_main_default;
  if (style_screen_imgbtn_weather_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_weather_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_weather_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_weather_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_weather_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_weather_main_main_default,
                               0);
  lv_style_set_img_opa(&style_screen_imgbtn_weather_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_weather,
                   &style_screen_imgbtn_weather_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_weather_main_main_pressed
  static lv_style_t style_screen_imgbtn_weather_main_main_pressed;
  if (style_screen_imgbtn_weather_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_weather_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_weather_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_weather_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_weather_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_weather_main_main_pressed,
                               0);
  lv_obj_add_style(ui->screen_imgbtn_weather,
                   &style_screen_imgbtn_weather_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_weather_main_main_checked
  static lv_style_t style_screen_imgbtn_weather_main_main_checked;
  if (style_screen_imgbtn_weather_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_weather_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_weather_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_weather_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_weather_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_weather_main_main_checked,
                               0);
  lv_obj_add_style(ui->screen_imgbtn_weather,
                   &style_screen_imgbtn_weather_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_weather, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_weather_alpha_140x140, NULL);
  lv_imgbtn_set_src(ui->screen_imgbtn_weather, LV_IMGBTN_STATE_CHECKED_RELEASED,
                    NULL, &_weather_alpha_140x140, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_weather, LV_OBJ_FLAG_CHECKABLE);

  // add new tile screen_tileview_main_NetWork
  ui->screen_tileview_main_NetWork = lv_tileview_add_tile(
      ui->screen_tileview_main, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);

  // Write codes screen_imgbtn_6
  ui->screen_imgbtn_6 = lv_imgbtn_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_imgbtn_6, 30, 26);
  lv_obj_set_size(ui->screen_imgbtn_6, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_6_main_main_default
  static lv_style_t style_screen_imgbtn_6_main_main_default;
  if (style_screen_imgbtn_6_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_6_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_6_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_6_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_6_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_6_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_6_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_6,
                   &style_screen_imgbtn_6_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_6_main_main_pressed
  static lv_style_t style_screen_imgbtn_6_main_main_pressed;
  if (style_screen_imgbtn_6_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_6_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_6_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_6_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_6_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_6_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_6,
                   &style_screen_imgbtn_6_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_6_main_main_checked
  static lv_style_t style_screen_imgbtn_6_main_main_checked;
  if (style_screen_imgbtn_6_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_6_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_6_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_6_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_6_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_6_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_6,
                   &style_screen_imgbtn_6_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_6, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_wifi_router_alpha_100x100, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_6, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_label_kbs
  ui->screen_label_kbs = lv_label_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_label_kbs, 417, 328);
  lv_obj_set_size(ui->screen_label_kbs, 50, 20);
  lv_label_set_text(ui->screen_label_kbs, "KB/S");
  lv_label_set_long_mode(ui->screen_label_kbs, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_kbs, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_kbs_main_main_default
  static lv_style_t style_screen_label_kbs_main_main_default;
  if (style_screen_label_kbs_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_kbs_main_main_default);
  else
    lv_style_init(&style_screen_label_kbs_main_main_default);
  lv_style_set_radius(&style_screen_label_kbs_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_kbs_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_kbs_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_kbs_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_kbs_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_kbs_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_kbs_main_main_default,
                         &lv_font_Alatsi_Regular_14);
  lv_style_set_text_letter_space(&style_screen_label_kbs_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_kbs_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_kbs_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_kbs_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_kbs_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_kbs,
                   &style_screen_label_kbs_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_label_4
  ui->screen_label_4 = lv_label_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_label_4, 192, 328);
  lv_obj_set_size(ui->screen_label_4, 80, 20);
  lv_label_set_text(ui->screen_label_4, "DOWN/UP");
  lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_4_main_main_default
  static lv_style_t style_screen_label_4_main_main_default;
  if (style_screen_label_4_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_4_main_main_default);
  else
    lv_style_init(&style_screen_label_4_main_main_default);
  lv_style_set_radius(&style_screen_label_4_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_4_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_4_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_4_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_4_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_4_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_4_main_main_default,
                         &lv_font_Alatsi_Regular_14);
  lv_style_set_text_letter_space(&style_screen_label_4_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_4_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_4_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_4_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_4_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_4, &style_screen_label_4_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_sw_vpn
  ui->screen_sw_vpn = lv_switch_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_sw_vpn, 190, 370);
  lv_obj_set_size(ui->screen_sw_vpn, 270, 60);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_sw_vpn_main_main_default
  static lv_style_t style_screen_sw_vpn_main_main_default;
  if (style_screen_sw_vpn_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_sw_vpn_main_main_default);
  else
    lv_style_init(&style_screen_sw_vpn_main_main_default);
  lv_style_set_radius(&style_screen_sw_vpn_main_main_default, 100);
  lv_style_set_bg_color(&style_screen_sw_vpn_main_main_default,
                        lv_color_make(0xe6, 0xe2, 0xe6));
  lv_style_set_bg_grad_color(&style_screen_sw_vpn_main_main_default,
                             lv_color_make(0xe6, 0xe2, 0xe6));
  lv_style_set_bg_grad_dir(&style_screen_sw_vpn_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_sw_vpn_main_main_default, 255);
  lv_obj_add_style(ui->screen_sw_vpn, &style_screen_sw_vpn_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_sw_vpn_main_indicator_checked
  static lv_style_t style_screen_sw_vpn_main_indicator_checked;
  if (style_screen_sw_vpn_main_indicator_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_sw_vpn_main_indicator_checked);
  else
    lv_style_init(&style_screen_sw_vpn_main_indicator_checked);
  lv_style_set_radius(&style_screen_sw_vpn_main_indicator_checked, 100);
  lv_style_set_bg_color(&style_screen_sw_vpn_main_indicator_checked,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_sw_vpn_main_indicator_checked,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_sw_vpn_main_indicator_checked,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_sw_vpn_main_indicator_checked, 255);
  lv_obj_add_style(ui->screen_sw_vpn,
                   &style_screen_sw_vpn_main_indicator_checked,
                   LV_PART_INDICATOR | LV_STATE_CHECKED);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_sw_vpn_main_knob_default
  static lv_style_t style_screen_sw_vpn_main_knob_default;
  if (style_screen_sw_vpn_main_knob_default.prop_cnt > 1)
    lv_style_reset(&style_screen_sw_vpn_main_knob_default);
  else
    lv_style_init(&style_screen_sw_vpn_main_knob_default);
  lv_style_set_radius(&style_screen_sw_vpn_main_knob_default, 100);
  lv_style_set_bg_color(&style_screen_sw_vpn_main_knob_default,
                        lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_color(&style_screen_sw_vpn_main_knob_default,
                             lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_dir(&style_screen_sw_vpn_main_knob_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_sw_vpn_main_knob_default, 255);
  lv_obj_add_style(ui->screen_sw_vpn, &style_screen_sw_vpn_main_knob_default,
                   LV_PART_KNOB | LV_STATE_DEFAULT);

  // Write codes screen_label_2
  ui->screen_label_2 = lv_label_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_label_2, 370, 120);
  lv_obj_set_size(ui->screen_label_2, 40, 32);
  lv_label_set_text(ui->screen_label_2, "MEM");
  lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_2_main_main_default
  static lv_style_t style_screen_label_2_main_main_default;
  if (style_screen_label_2_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_2_main_main_default);
  else
    lv_style_init(&style_screen_label_2_main_main_default);
  lv_style_set_radius(&style_screen_label_2_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_2_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_2_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_2_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_2_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_2_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_2_main_main_default,
                         &lv_font_Alatsi_Regular_16);
  lv_style_set_text_letter_space(&style_screen_label_2_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_2_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_2_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_2_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_2_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_2, &style_screen_label_2_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_label_cpu
  ui->screen_label_cpu = lv_label_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_label_cpu, 220, 120);
  lv_obj_set_size(ui->screen_label_cpu, 35, 32);
  lv_label_set_text(ui->screen_label_cpu, "CPU");
  lv_label_set_long_mode(ui->screen_label_cpu, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->screen_label_cpu, LV_TEXT_ALIGN_LEFT, 0);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_label_cpu_main_main_default
  static lv_style_t style_screen_label_cpu_main_main_default;
  if (style_screen_label_cpu_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_label_cpu_main_main_default);
  else
    lv_style_init(&style_screen_label_cpu_main_main_default);
  lv_style_set_radius(&style_screen_label_cpu_main_main_default, 0);
  lv_style_set_bg_color(&style_screen_label_cpu_main_main_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_label_cpu_main_main_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_label_cpu_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_label_cpu_main_main_default, 0);
  lv_style_set_text_color(&style_screen_label_cpu_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_text_font(&style_screen_label_cpu_main_main_default,
                         &lv_font_Alatsi_Regular_16);
  lv_style_set_text_letter_space(&style_screen_label_cpu_main_main_default, 2);
  lv_style_set_pad_left(&style_screen_label_cpu_main_main_default, 0);
  lv_style_set_pad_right(&style_screen_label_cpu_main_main_default, 0);
  lv_style_set_pad_top(&style_screen_label_cpu_main_main_default, 0);
  lv_style_set_pad_bottom(&style_screen_label_cpu_main_main_default, 0);
  lv_obj_add_style(ui->screen_label_cpu,
                   &style_screen_label_cpu_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write codes screen_chart_load
  ui->screen_chart_load = lv_chart_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_chart_load, 180, 159);
  lv_obj_set_size(ui->screen_chart_load, 280, 160);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_chart_load_main_main_default
  static lv_style_t style_screen_chart_load_main_main_default;
  if (style_screen_chart_load_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_chart_load_main_main_default);
  else
    lv_style_init(&style_screen_chart_load_main_main_default);
  lv_style_set_bg_color(&style_screen_chart_load_main_main_default,
                        lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_color(&style_screen_chart_load_main_main_default,
                             lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_bg_grad_dir(&style_screen_chart_load_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_chart_load_main_main_default, 255);
  lv_style_set_pad_left(&style_screen_chart_load_main_main_default, 5);
  lv_style_set_pad_right(&style_screen_chart_load_main_main_default, 5);
  lv_style_set_pad_top(&style_screen_chart_load_main_main_default, 5);
  lv_style_set_pad_bottom(&style_screen_chart_load_main_main_default, 5);
  lv_style_set_line_color(&style_screen_chart_load_main_main_default,
                          lv_color_make(0xe8, 0xe8, 0xe8));
  lv_style_set_line_width(&style_screen_chart_load_main_main_default, 2);
  lv_style_set_line_opa(&style_screen_chart_load_main_main_default, 255);
  lv_obj_add_style(ui->screen_chart_load,
                   &style_screen_chart_load_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_chart_set_type(ui->screen_chart_load, LV_CHART_TYPE_LINE);
  lv_chart_set_range(ui->screen_chart_load, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
  lv_chart_set_div_line_count(ui->screen_chart_load, 9, 10);
  lv_chart_set_point_count(ui->screen_chart_load, 10);
  lv_chart_series_t *screen_chart_load_0 = lv_chart_add_series(
      ui->screen_chart_load, lv_color_make(0x5f, 0xa6, 0xdd),
      LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_0, 1);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_0, 20);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_0, 30);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_0, 40);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_0, 5);
  lv_chart_series_t *screen_chart_load_1 = lv_chart_add_series(
      ui->screen_chart_load, lv_color_make(0x00, 0x00, 0x00),
      LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_1, 10);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_1, 30);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_1, 70);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_1, 30);
  lv_chart_set_next_value(ui->screen_chart_load, screen_chart_load_1, 80);

  // Write codes screen_arc_mem
  ui->screen_arc_mem = lv_arc_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_arc_mem, 336, 17);
  lv_obj_set_size(ui->screen_arc_mem, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_mem_main_main_default
  static lv_style_t style_screen_arc_mem_main_main_default;
  if (style_screen_arc_mem_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_mem_main_main_default);
  else
    lv_style_init(&style_screen_arc_mem_main_main_default);
  lv_style_set_bg_color(&style_screen_arc_mem_main_main_default,
                        lv_color_make(0xf6, 0xf6, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_arc_mem_main_main_default,
                             lv_color_make(0xf6, 0xf6, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_arc_mem_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_arc_mem_main_main_default, 255);
  lv_style_set_border_width(&style_screen_arc_mem_main_main_default, 0);
  lv_style_set_arc_color(&style_screen_arc_mem_main_main_default,
                         lv_color_make(0xe6, 0xe6, 0xe6));
  lv_style_set_arc_width(&style_screen_arc_mem_main_main_default, 5);
  lv_obj_add_style(ui->screen_arc_mem, &style_screen_arc_mem_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_mem_main_indicator_default
  static lv_style_t style_screen_arc_mem_main_indicator_default;
  if (style_screen_arc_mem_main_indicator_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_mem_main_indicator_default);
  else
    lv_style_init(&style_screen_arc_mem_main_indicator_default);
  lv_style_set_arc_color(&style_screen_arc_mem_main_indicator_default,
                         lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_arc_width(&style_screen_arc_mem_main_indicator_default, 5);
  lv_obj_add_style(ui->screen_arc_mem,
                   &style_screen_arc_mem_main_indicator_default,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_mem_main_knob_default
  static lv_style_t style_screen_arc_mem_main_knob_default;
  if (style_screen_arc_mem_main_knob_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_mem_main_knob_default);
  else
    lv_style_init(&style_screen_arc_mem_main_knob_default);
  lv_style_set_bg_color(&style_screen_arc_mem_main_knob_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_arc_mem_main_knob_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_arc_mem_main_knob_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_arc_mem_main_knob_default, 255);
  lv_style_set_pad_all(&style_screen_arc_mem_main_knob_default, 5);
  lv_obj_add_style(ui->screen_arc_mem, &style_screen_arc_mem_main_knob_default,
                   LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_arc_set_bg_angles(ui->screen_arc_mem, 0, 360);
  lv_arc_set_angles(ui->screen_arc_mem, 90, 180);
  lv_arc_set_rotation(ui->screen_arc_mem, 0);
  lv_obj_set_style_pad_top(ui->screen_arc_mem, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui->screen_arc_mem, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui->screen_arc_mem, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui->screen_arc_mem, 20, LV_STATE_DEFAULT);

  // Write codes screen_arc_cpu
  ui->screen_arc_cpu = lv_arc_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_arc_cpu, 193, 16);
  lv_obj_set_size(ui->screen_arc_cpu, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_cpu_main_main_default
  static lv_style_t style_screen_arc_cpu_main_main_default;
  if (style_screen_arc_cpu_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_cpu_main_main_default);
  else
    lv_style_init(&style_screen_arc_cpu_main_main_default);
  lv_style_set_bg_color(&style_screen_arc_cpu_main_main_default,
                        lv_color_make(0xf6, 0xf6, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_arc_cpu_main_main_default,
                             lv_color_make(0xf6, 0xf6, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_arc_cpu_main_main_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_arc_cpu_main_main_default, 255);
  lv_style_set_border_width(&style_screen_arc_cpu_main_main_default, 0);
  lv_style_set_arc_color(&style_screen_arc_cpu_main_main_default,
                         lv_color_make(0xe6, 0xe6, 0xe6));
  lv_style_set_arc_width(&style_screen_arc_cpu_main_main_default, 5);
  lv_obj_add_style(ui->screen_arc_cpu, &style_screen_arc_cpu_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_cpu_main_indicator_default
  static lv_style_t style_screen_arc_cpu_main_indicator_default;
  if (style_screen_arc_cpu_main_indicator_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_cpu_main_indicator_default);
  else
    lv_style_init(&style_screen_arc_cpu_main_indicator_default);
  lv_style_set_arc_color(&style_screen_arc_cpu_main_indicator_default,
                         lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_arc_width(&style_screen_arc_cpu_main_indicator_default, 5);
  lv_obj_add_style(ui->screen_arc_cpu,
                   &style_screen_arc_cpu_main_indicator_default,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_arc_cpu_main_knob_default
  static lv_style_t style_screen_arc_cpu_main_knob_default;
  if (style_screen_arc_cpu_main_knob_default.prop_cnt > 1)
    lv_style_reset(&style_screen_arc_cpu_main_knob_default);
  else
    lv_style_init(&style_screen_arc_cpu_main_knob_default);
  lv_style_set_bg_color(&style_screen_arc_cpu_main_knob_default,
                        lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_color(&style_screen_arc_cpu_main_knob_default,
                             lv_color_make(0x21, 0x95, 0xf6));
  lv_style_set_bg_grad_dir(&style_screen_arc_cpu_main_knob_default,
                           LV_GRAD_DIR_VER);
  lv_style_set_bg_opa(&style_screen_arc_cpu_main_knob_default, 255);
  lv_style_set_pad_all(&style_screen_arc_cpu_main_knob_default, 5);
  lv_obj_add_style(ui->screen_arc_cpu, &style_screen_arc_cpu_main_knob_default,
                   LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_arc_set_bg_angles(ui->screen_arc_cpu, 0, 360);
  lv_arc_set_angles(ui->screen_arc_cpu, 90, 180);
  lv_arc_set_rotation(ui->screen_arc_cpu, 0);
  lv_obj_set_style_pad_top(ui->screen_arc_cpu, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui->screen_arc_cpu, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui->screen_arc_cpu, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui->screen_arc_cpu, 20, LV_STATE_DEFAULT);

  // Write codes screen_imgbtn_8
  ui->screen_imgbtn_8 = lv_imgbtn_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_imgbtn_8, 30, 337);
  lv_obj_set_size(ui->screen_imgbtn_8, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_8_main_main_default
  static lv_style_t style_screen_imgbtn_8_main_main_default;
  if (style_screen_imgbtn_8_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_8_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_8_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_8_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_8_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_8_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_8_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_8,
                   &style_screen_imgbtn_8_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_8_main_main_pressed
  static lv_style_t style_screen_imgbtn_8_main_main_pressed;
  if (style_screen_imgbtn_8_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_8_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_8_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_8_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_8_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_8_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_8,
                   &style_screen_imgbtn_8_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_8_main_main_checked
  static lv_style_t style_screen_imgbtn_8_main_main_checked;
  if (style_screen_imgbtn_8_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_8_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_8_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_8_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_8_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_8_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_8,
                   &style_screen_imgbtn_8_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_8, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_virtual_alpha_100x100, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_8, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_imgbtn_7
  ui->screen_imgbtn_7 = lv_imgbtn_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_imgbtn_7, 30, 191);
  lv_obj_set_size(ui->screen_imgbtn_7, 100, 100);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_imgbtn_7_main_main_default
  static lv_style_t style_screen_imgbtn_7_main_main_default;
  if (style_screen_imgbtn_7_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_7_main_main_default);
  else
    lv_style_init(&style_screen_imgbtn_7_main_main_default);
  lv_style_set_text_color(&style_screen_imgbtn_7_main_main_default,
                          lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor(&style_screen_imgbtn_7_main_main_default,
                           lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_7_main_main_default, 0);
  lv_style_set_img_opa(&style_screen_imgbtn_7_main_main_default, 255);
  lv_obj_add_style(ui->screen_imgbtn_7,
                   &style_screen_imgbtn_7_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  // Write style state: LV_STATE_PRESSED for
  // style_screen_imgbtn_7_main_main_pressed
  static lv_style_t style_screen_imgbtn_7_main_main_pressed;
  if (style_screen_imgbtn_7_main_main_pressed.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_7_main_main_pressed);
  else
    lv_style_init(&style_screen_imgbtn_7_main_main_pressed);
  lv_style_set_text_color(&style_screen_imgbtn_7_main_main_pressed,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_7_main_main_pressed,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_7_main_main_pressed, 0);
  lv_obj_add_style(ui->screen_imgbtn_7,
                   &style_screen_imgbtn_7_main_main_pressed,
                   LV_PART_MAIN | LV_STATE_PRESSED);

  // Write style state: LV_STATE_CHECKED for
  // style_screen_imgbtn_7_main_main_checked
  static lv_style_t style_screen_imgbtn_7_main_main_checked;
  if (style_screen_imgbtn_7_main_main_checked.prop_cnt > 1)
    lv_style_reset(&style_screen_imgbtn_7_main_main_checked);
  else
    lv_style_init(&style_screen_imgbtn_7_main_main_checked);
  lv_style_set_text_color(&style_screen_imgbtn_7_main_main_checked,
                          lv_color_make(0xFF, 0x33, 0xFF));
  lv_style_set_img_recolor(&style_screen_imgbtn_7_main_main_checked,
                           lv_color_make(0x00, 0x00, 0x00));
  lv_style_set_img_recolor_opa(&style_screen_imgbtn_7_main_main_checked, 0);
  lv_obj_add_style(ui->screen_imgbtn_7,
                   &style_screen_imgbtn_7_main_main_checked,
                   LV_PART_MAIN | LV_STATE_CHECKED);
  lv_imgbtn_set_src(ui->screen_imgbtn_7, LV_IMGBTN_STATE_RELEASED, NULL,
                    &_global_network_alpha_100x100, NULL);
  lv_obj_add_flag(ui->screen_imgbtn_7, LV_OBJ_FLAG_CHECKABLE);

  // Write codes screen_line_div
  ui->screen_line_div = lv_line_create(ui->screen_tileview_main_NetWork);
  lv_obj_set_pos(ui->screen_line_div, 160, 30);
  lv_obj_set_size(ui->screen_line_div, 8, 420);

  // Write style state: LV_STATE_DEFAULT for
  // style_screen_line_div_main_main_default
  static lv_style_t style_screen_line_div_main_main_default;
  if (style_screen_line_div_main_main_default.prop_cnt > 1)
    lv_style_reset(&style_screen_line_div_main_main_default);
  else
    lv_style_init(&style_screen_line_div_main_main_default);
  lv_style_set_line_color(&style_screen_line_div_main_main_default,
                          lv_color_make(0xcf, 0xc9, 0xc9));
  lv_style_set_line_width(&style_screen_line_div_main_main_default, 2);
  lv_style_set_line_rounded(&style_screen_line_div_main_main_default, true);
  lv_obj_add_style(ui->screen_line_div,
                   &style_screen_line_div_main_main_default,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  static lv_point_t screen_line_div[] = {
      {0, 0},
      {0, 400},
  };
  lv_line_set_points(ui->screen_line_div, screen_line_div, 2);

  // add new tile screen_tileview_main_Title
  ui->screen_tileview_main_Title =
      lv_tileview_add_tile(ui->screen_tileview_main, 2, 0, LV_DIR_LEFT);

  // Write codes screen_animimg_1
  ui->screen_animimg_1 = lv_animimg_create(ui->screen_tileview_main_Title);
  lv_obj_set_pos(ui->screen_animimg_1, 78, 114);
  lv_obj_set_size(ui->screen_animimg_1, 320, 240);
  lv_animimg_set_src(ui->screen_animimg_1,
                     (lv_img_dsc_t **)screen_animimg_1_imgs, 48);
  lv_animimg_set_duration(ui->screen_animimg_1, 1440);
  lv_animimg_set_repeat_count(ui->screen_animimg_1, 3000);
  lv_animimg_start(ui->screen_animimg_1);

  // Init events for screen
  events_init_screen(ui);
}
