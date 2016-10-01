#include <pebble.h>
#include "clock.h"
#include "base.h"
#include "calendar.h"
#include "chime.h"
#include "status_disp.h"
#include "app_messaging.h"

#define CLOCK_POS 52

static void display_time_digital( struct tm *tick_time, struct CONFIG_PARAMS config_params );
static void display_time_analog( struct tm *tick_time );

static struct CONFIG_PARAMS config_params;
static TextLayer *clock_layer_digital;

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void get_config( struct CONFIG_PARAMS params ) {
  time_t timeInSecs = time( NULL );
  struct tm *localTime = localtime( &timeInSecs );
  
  memset( &config_params, 0, sizeof( struct CONFIG_PARAMS ) );
  config_params = params; // global
  get_status( localTime, config_params, true );
}

static void display_time_analog( struct tm *tick_time ) { // config_params not required

  if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: display_time_analog()" );
}

static void display_time_digital( struct tm *tick_time, struct CONFIG_PARAMS params ) {
  static char hour_text[] = "xx:xx";

  strftime( hour_text, sizeof( hour_text ), params.digital_clock_type_12_24 == 1 ? "%H:%M" : "%I:%M", tick_time );
  
  // This is a hack to get rid of the leading zero.
  if(hour_text[0] == '0') memmove( &hour_text[0], &hour_text[1], sizeof( hour_text ) - 1 );

  text_layer_set_text( clock_layer_digital, hour_text );
  
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: display_time_digital() %s", hour_text );
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tick_time->tm_hour, tick_time->tm_min, tick_time->tm_sec );
  
  if ( tick_time->tm_min == 0 ) {
    show_weeks( tick_time );
  }
  
  if ( config_params.clock_type_digital_analog == 1 ) {
    display_time_analog( tick_time );
  } else {
    display_time_digital( tick_time, config_params );
  }
  
  get_status( tick_time, config_params, false );
  
  do_chime( tick_time, config_params );
}

void clock_init( void ) {

  clock_layer_digital = text_layer_create( GRect( 0, CLOCK_POS + 23, 144, 60 ) );
  setup_text_layer( clock_layer_digital, RESOURCE_ID_FONT_EXO_50);
  text_layer_set_text_alignment( clock_layer_digital, GTextAlignmentCenter );
  text_layer_set_text_color( clock_layer_digital, GColorWhite );

  // subscription
  time_t now = time( NULL );
  struct tm *tick_time;
  tick_time = localtime( &now );
  handle_clock_tick( tick_time, MINUTE_UNIT );
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

void clock_deinit( void ) {
  tick_timer_service_unsubscribe();
  text_layer_destroy( clock_layer_digital );
}


