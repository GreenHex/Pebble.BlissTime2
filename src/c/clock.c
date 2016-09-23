#include <pebble.h>
#include "clock.h"
#include "base.h"
#include "calendar.h"
#include "weather.h"
#include "app_messaging.h"

#define CLOCK_POS 52

static int display_type=0;
static bool date_shown = false;
static TextLayer *clock_layer4; // Digital only
static int clock_mode = 0; // 0 Digital 12h, 1 Digital 24h
static int buzz_freq = 0;
static int buzz_offset = 0;
static int buzz_start = 5;
static int buzz_end = 23;
static int buzz_on_days[7] = {1,1,1,1,1,1,1};
static struct tm *clock_time;
  
// Buzz patterns
static uint32_t const one_segment[] = { 200 };
VibePattern single_vibe_pattern = {
  .durations = one_segment,
  .num_segments = ARRAY_LENGTH(one_segment),
};
static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH(two_segments),
};


static void display_time_digital() {
  static char hour_text[] = "xx:xx";

  strftime(hour_text, sizeof(hour_text), clock_mode == 1 ? "%H:%M" : "%I:%M", clock_time);
  
  // This is a hack to get rid of the leading zero.
  if(hour_text[0] == '0') memmove(&hour_text[0], &hour_text[1], sizeof(hour_text) - 1);

  text_layer_set_text(clock_layer4, hour_text);
}

static void show_time(struct tm *tick_time) {
  clock_time = tick_time;
  display_time_digital();

  if (tick_time->tm_min == 0 || !date_shown) {
    show_weeks(tick_time->tm_mon,tick_time->tm_mday,tick_time->tm_wday,tick_time->tm_year);
    date_shown = true;
  }
}

static void do_buzz(struct tm *time) {
  // Stop if buzzing is off
  if (buzz_freq == 0) {
    return;
  }
  int hour = time->tm_hour;
  int min = time->tm_min;
  int day = time->tm_wday;
  if (min == 0) min = 60;

  // Stop if not on for the day
  if (buzz_on_days[day] == 0) return;

  // Stop if not within time range
  if ((hour == (buzz_start-1) && (min+buzz_offset != 60)) || hour < (buzz_start-1)) return;
  if ((hour == buzz_end && buzz_offset != 0) || hour > buzz_end) return;

  // Stop if not at offset
  int buzz_min = 60;
  if (buzz_freq == 1) buzz_min = 30;
  if ((min+buzz_offset) % buzz_min != 0) return;

  if (min+buzz_offset == 60) {
    vibes_enqueue_custom_pattern(double_vibe_pattern);
  } else {
    vibes_enqueue_custom_pattern(single_vibe_pattern);
  }
}

static void handle_clock_tick(struct tm *tick_time, TimeUnits units_changed) {
  show_time(tick_time);
  
  // update weather
  if(tick_time->tm_min % 30 == 0) {
    if (display_type) {
      request_stock();
    } else {
      request_weather();
    }
  }
  
  do_buzz(tick_time);
}

void configure_buzz(int disp_type, int freq, int lead_time, int start, int end, int sun, int mon, int tue, int wed, int thu, int fri, int sat) {
  display_type = disp_type;
  buzz_freq = freq;
  buzz_offset = lead_time;
  buzz_start = start;
  buzz_end = end;
  buzz_on_days[0] = sun;
  buzz_on_days[1] = mon;
  buzz_on_days[2] = tue;
  buzz_on_days[3] = wed;
  buzz_on_days[4] = thu;
  buzz_on_days[5] = fri;
  buzz_on_days[6] = sat;
}

void configure_clock(int mode) {
  clock_mode = mode;
  // display_time_digital();
}

void clock_init() {

  clock_layer4 = text_layer_create(GRect(0,CLOCK_POS+23,144,60));
  setup_text_layer(clock_layer4, RESOURCE_ID_FONT_EXO_50);
  text_layer_set_text_alignment(clock_layer4, GTextAlignmentCenter);
  text_layer_set_text_color(clock_layer4, GColorWhite);

  // subscription
  time_t now = time(NULL);
  struct tm *tick_time;
  tick_time = localtime(&now);
  handle_clock_tick(tick_time, MINUTE_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_clock_tick);
}

void clock_deinit() {
  tick_timer_service_unsubscribe();
  text_layer_destroy(clock_layer4);
}
