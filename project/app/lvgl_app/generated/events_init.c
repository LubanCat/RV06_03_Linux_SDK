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


#include "custom.h"
static void screen_login_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		
		break;
	}
	default:
		break;
	}
}
void events_init_screen_login(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_login, screen_login_event_handler, LV_EVENT_ALL, ui);
}
static void screen_submit_close_door_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		device_close_door();
		break;
	}
	default:
		break;
	}
}
void events_init_screen_submit(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_submit_close_door, screen_submit_close_door_event_handler, LV_EVENT_ALL, ui);
}
static void screen_clean_quit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_login, guider_ui.screen_login_del, &guider_ui.screen_clean_del, setup_scr_screen_login, LV_SCR_LOAD_ANIM_NONE, 200, 200, true, true);
		break;
	}
	default:
		break;
	}
}
void events_init_screen_clean(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_clean_quit, screen_clean_quit_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{

}
