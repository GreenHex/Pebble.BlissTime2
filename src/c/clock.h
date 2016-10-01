#pragma once
#include <pebble.h>
#include "config.h"

bool is_X_in_range( int a, int b, int x );
void get_config( struct CONFIG_PARAMS params );
void clock_init( void );
void clock_deinit( void );
