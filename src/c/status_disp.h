#pragma once
#include <pebble.h>
#include "clock.h"

void handle_message( Tuple *tupple_ptr );
void clear_status( void );
void status_init( Window *window );
void status_deinit( void );
void get_status( struct tm *time, bool ignoreUpdateInterval );