#pragma once
#include <pebble.h>

struct CONFIG_PARAMS {
  // status settings
  int status_display_type; // 0 - Nothing, 1 - Weather, 2 - Stocks
  int status_update_interval; // mins
  int status_update_start_time;
  int status_update_end_time;
  bool status_update_on_days[7]; // = { 0, 1, 1, 1, 1, 1, 0 };
  // clock settings
  int clock_type_digital_or_analog; // 0 - Digital, 1 - Analog
  int digital_clock_type_12_or_24; // 0 - Digital 12h, 1 - Digital 24h
  // chime settings
  int chime_interval; // 0 - Never, 1 - 1/2 hourly, 2 - hourly
  int chime_start_time;
  int chime_end_time;
  bool chime_on_days[7]; // = { 0, 1, 1, 1, 1, 1, 0 };
  int chime_offset; // mins
};

void handle_config_message( DictionaryIterator *iterator );