#pragma once
#include <pebble.h>
#include "clock.h"

void handle_message( Tuple *tupple_ptr );
void clear_status( void );
void status_init( void );
void status_deinit( void );
void get_status( struct tm *time, struct CONFIG_PARAMS config_params, bool ignoreUpdateInterval );