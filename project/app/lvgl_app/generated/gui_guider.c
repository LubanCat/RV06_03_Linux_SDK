/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void setup_ui(lv_ui *ui){
	setup_scr_screen(ui);
	lv_scr_load(ui->screen);
}

void clock_count_24(int *hour, int *min, int *sec)
{
    (*sec)++;
    if(*sec == 60)
    {
        *sec = 0;
        (*min)++;
    }
    if(*min == 60)
    {
        *min = 0;
        (*hour)++;
    }
    if(*hour == 24)
    {
        *hour = 0;
    }
}

void clock_count_12(int *hour, int *min, int *sec, char *meridiem)
{
    (*sec)++;
    if(*sec == 60)
    {
        *sec = 0;
        (*min)++;
    }
    if(*min == 60)
    {
        *min = 0;
        if(*hour < 12)
        {
            (*hour)++;
        } else {
            (*hour)++;
            *hour = *hour %12;
        }
    }
    if(*hour == 12 && *sec == 0 && *min ==0)
    {
        if((strcmp(meridiem, "PM") == 0))
        {
            strcpy(meridiem, "AM");
        } else {
            strcpy(meridiem, "PM");
        }
    }
}