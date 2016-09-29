#include <pebble.h>
#include "clock.h"
#include "base.h"
#include "calendar.h"
#include "status_disp.h"
#include "app_messaging.h"

#define CLOCK_POS 52

void display_clock( void );
static void display_time_digital( void );

static struct CONFIG_PARAMS config_params;
static bool date_shown = false; //
static TextLayer *clock_layer4; // Digital only
static struct tm *clock_time = 0;

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };
  
// Buzz patterns
static uint32_t const one_segment[] = { 200, 200 };
VibePattern single_vibe_pattern = {
  .durations = one_segment,
  .num_segments = ARRAY_LENGTH( one_segment ),
};
static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH( two_segments ),
};

static void display_time_digital( void ) {
  static char hour_text[] = "xx:xx";

  strftime( hour_text, sizeof( hour_text ), config_params.digital_clock_type_12_24 == 1 ? "%H:%M" : "%I:%M", clock_time );
  
  // This is a hack to get rid of the leading zero.
  if(hour_text[0] == '0') memmove( &hour_text[0], &hour_text[1], sizeof( hour_text ) - 1 );

  text_layer_set_text( clock_layer4, hour_text );
}

static void show_time( struct tm *tick_time ) {
  clock_time = tick_time;
  display_time_digital();

  if ( tick_time->tm_min == 0 || !date_shown ) {
    show_weeks( tick_time->tm_mon, tick_time->tm_mday, tick_time->tm_wday, tick_time->tm_year );
    date_shown = true;
  }
}

static void do_buzz( struct tm *time ) {

  int mins_from_zero = time->tm_hour * 60 + time->tm_min + config_params.chime_offset;

  // Stop if buzzing is off
  if ( config_params.chime_interval == 0 ) return;
  
  // Stop if not on for the day
  if ( config_params.chime_on_days[time->tm_wday] == 0 ) return;

  // Stop if not within time range.
  // Add one minute to buzz_end to buzz on the last hour.
  if ( !is_X_in_range( config_params.chime_start_time * 60, config_params.chime_end_time * 60 + 1, mins_from_zero ) ) return;
  
  // Stop if not at offset
  if ( mins_from_zero % ( ( config_params.chime_interval == 1 ) ? 30 : 60 ) ) return;

  // is this half hour or full hour?
  if ( time->tm_min + config_params.chime_offset == 0 ) {
    vibes_enqueue_custom_pattern( double_vibe_pattern );
  } else {
    vibes_enqueue_custom_pattern( single_vibe_pattern );
  }
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  show_time( tick_time );
  
  if ( config_params.status_display_type ) {
    if ( is_X_in_range( config_params.status_update_start_time, config_params.status_update_end_time, tick_time->tm_hour ) ) {
      if ( tick_time->tm_min % config_params.status_update_interval == 0 ) {
        if ( config_params.status_display_type == 1 ) {
          request_weather();
        } else if ( config_params.status_display_type == 2 ) {
          request_stock();
        } 
      }
    } else {
      clear_status();
    }
  } else {
    clear_status();
  }
  do_buzz(tick_time);
}

void display_clock( void ) {
  return;
  if ( config_params.clock_type_digital_analog ) {
    display_time_digital(); // display_time_analog();
  } else {
    display_time_digital();
  }
}

void display_status( void ) {
  time_t timeInSecs = time( NULL );
  struct tm *localTime = localtime( &timeInSecs );
  
  if ( is_X_in_range( config_params.status_update_start_time, config_params.status_update_end_time, localTime->tm_hour ) ) {
    if ( config_params.status_display_type == 1 ) {
      request_weather();
    } else if ( config_params.status_display_type == 2 ) {
      request_stock();
    }
  }
}

void reconfigure_display( void ) {
  display_clock();
  display_status();
}

void get_config( struct CONFIG_PARAMS params ) {
  
  memset( &config_params, 0, sizeof( struct CONFIG_PARAMS ) );
  config_params = params;
  
  reconfigure_display();
}

void configure_clock( int mode ) {
  config_params.clock_type_digital_analog = mode;
  // display_time_digital();
}

void clock_init( void ) {

  clock_layer4 = text_layer_create( GRect( 0, CLOCK_POS+23, 144, 60 ) );
  setup_text_layer( clock_layer4, RESOURCE_ID_FONT_EXO_50);
  text_layer_set_text_alignment( clock_layer4, GTextAlignmentCenter );
  text_layer_set_text_color( clock_layer4, GColorWhite );

  // subscription
  time_t now = time( NULL );
  struct tm *tick_time;
  tick_time = localtime( &now );
  handle_clock_tick( tick_time, MINUTE_UNIT );
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

void clock_deinit( void ) {
  tick_timer_service_unsubscribe();
  text_layer_destroy( clock_layer4 );
}


