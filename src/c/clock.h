#pragma once
#include <pebble.h>

struct CONFIG_PARAMS {
  // status settings
  int status_display_type; // = 0; // 0 - Nothing, 1 - Weather, 2 - Stocks
  int status_update_interval; // = 30; // mins
  int status_update_start_time; // = 9;
  int status_update_end_time; // = 17;
  // clock settings
  int clock_type_digital_analog; // = 0; // 0 - Digital, 1 - Analog
  int digital_clock_type_12_24; // = 0; // 0 - Digital 12h, 1 - Digital 24h
  // chime settings
  int chime_interval; // = 2; // 0 - Never, 1 - 1/2 hourly, 2 - hourly
  int chime_start_time; // = 0;
  int chime_end_time; // = 23;
  uint8_t chime_on_days[7]; // = { 1, 1, 1, 1, 1, 1, 1 };
  uint32_t chime_offset; // = 0;
};

void get_config( struct CONFIG_PARAMS params );
void clock_init( void );
void clock_deinit( void );
