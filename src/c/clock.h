#pragma once
#include <pebble.h>
#include "config.h"

struct ANALOG_LAYER_DATA {
  bool show_seconds;
};

bool is_X_in_range( int a, int b, int x );
void get_config( struct CONFIG_PARAMS params );
void clock_init( Window* window );
void clock_deinit( void );
