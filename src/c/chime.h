#pragma once
#include "pebble.h"
#include "clock.h"
#include "config.h"

enum CHIME_INTERVAL {
  NO_CHIME = 0,
  EVERY_HALF_HOUR = 1,
  EVERY_HOUR = 2
};

void do_chime( struct tm *time );