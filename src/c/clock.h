#pragma once
#include <pebble.h>
#include "config.h"

struct ANALOG_LAYER_DATA {
  bool show_seconds;
  struct tm tm_show;
};

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
