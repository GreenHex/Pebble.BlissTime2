#pragma once
#include <pebble.h>

void show_weeks( struct tm *tick_time );
void calendar_init( Window *window );
void calendar_deinit( void );
