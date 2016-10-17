#pragma once
#include <pebble.h>
#include "config.h"

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 52 + 1
#define SEC_HAND_LENGTH 45
#define SEC_HAND_TAIL_LENGTH 20
#define SEC_HAND_TIP_LENGTH 10
#define MIN_HAND_LENGTH 40
#define HOUR_HAND_LENGTH 26
#define SEC_HAND_WIDTH 1
#define MIN_HAND_WIDTH 5
#define HOUR_HAND_WIDTH 7
#define CENTER_DOT_RADIUS 9
#define DIGITAL_CLOCK_TEXT_Y_POS 23

#define COLOUR_DOT              PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_DOT_OUTLINE      PBL_IF_COLOR_ELSE( GColorDarkGray, GColorBlack )
#define COLOUR_HANDS_OUTLINE    PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite )
#define COLOUR_DIGITAL_FG       PBL_IF_COLOR_ELSE( GColorChromeYellow, GColorWhite )
#define COLOUR_DIGITAL_BG       PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )

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
  uint8_t hand_width;
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
