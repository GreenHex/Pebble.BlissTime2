#pragma once
#include <pebble.h>
#include "config.h"

enum CLOCK_TYPE {
  CLK_DIGITAL = 0,
  CLK_ANALOG = 1
};

enum DIGITAL_TYPE {
  DIGITAL_WATCH_PREFS = 0,
  DIGITAL_12_HOUR = 1,
  DIGITAL_24_HOUR = 2
};

struct ANALOG_LAYER_DATA {
  bool show_seconds;
};

struct HAND_DRAW_PARAMS {
	GContext *ctx;
  GPoint center_pt;
  GPoint from_pt;
	GPoint to_pt;
	uint8_t	hand_width;
	GColor hand_color;
	GColor hand_outline_color;
	uint16_t dot_radius;
	GColor dot_color;
	GColor dot_outline_color;
};

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
