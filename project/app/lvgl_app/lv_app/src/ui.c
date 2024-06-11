// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Home;
lv_obj_t * ui_Charttest;
lv_obj_t * ui_WuQiang;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 32
    #error "LV_COLOR_DEPTH should be 32bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Home_screen_init(void)
{

    // ui_Home

    ui_Home = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Charttest

    ui_Charttest = lv_chart_create(ui_Home);

    lv_obj_set_width(ui_Charttest, 1404);
    lv_obj_set_height(ui_Charttest, 728);

    lv_obj_set_x(ui_Charttest, -106);
    lv_obj_set_y(ui_Charttest, 3);

    lv_obj_set_align(ui_Charttest, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_Charttest, lv_color_hex(0xB29999), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Charttest, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Charttest, lv_color_hex(0xC0ABAB), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Charttest, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Charttest, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Charttest, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_line_color(ui_Charttest, lv_color_hex(0xB85656), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Charttest, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_Charttest, lv_color_hex(0xB85656), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Charttest, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);

    // ui_WuQiang

    ui_WuQiang = lv_bar_create(ui_Home);
    lv_bar_set_range(ui_WuQiang, 0, 100);
    lv_bar_set_value(ui_WuQiang, 25, LV_ANIM_OFF);

    lv_obj_set_width(ui_WuQiang, 1200);
    lv_obj_set_height(ui_WuQiang, 40);

    lv_obj_set_x(ui_WuQiang, -58);
    lv_obj_set_y(ui_WuQiang, 439);

    lv_obj_set_align(ui_WuQiang, LV_ALIGN_CENTER);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Home_screen_init();
    lv_disp_load_scr(ui_Home);
}

