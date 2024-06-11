/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl/lvgl.h"

void events_init(lv_ui *ui)
{
}

static void screen_imgbtn_weather_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_bg_opa(guider_ui.screen_imgbtn_weather, 101, LV_PART_MAIN);
		lv_obj_set_style_bg_opa(guider_ui.screen_imgbtn_weather, 255, LV_PART_MAIN);
	}
		break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_imgbtn_weather, screen_imgbtn_weather_event_handler, LV_EVENT_ALL, NULL);
}
