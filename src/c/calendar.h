#pragma once
#include <pebble.h>

#define COLOUR_LINE_LAYER              PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_CAL_WEEKDAYS_LABEL      PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_CAL_WEEKENDS_LABEL      PBL_IF_COLOR_ELSE( GColorDarkGray, GColorBlack )
#define COLOUR_CAL_WEEKDAYS_TXT        PBL_IF_COLOR_ELSE( GColorOxfordBlue, GColorBlack )
#define COLOUR_CAL_WEEKENDS_TXT        PBL_IF_COLOR_ELSE( GColorOrange /* GColorDarkCandyAppleRed */, GColorBlack )
#define COLOUR_CAL_TODAY_WEEKDAYS_BG   COLOUR_CAL_WEEKDAYS_TXT
#define COLOUR_CAL_TODAY_WEEKENDS_BG   COLOUR_CAL_WEEKENDS_TXT
#define COLOUR_CAL_TODAY_FG            PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )

void show_weeks( struct tm *tick_time );
void calendar_init( Window *window );
void calendar_deinit( void );
