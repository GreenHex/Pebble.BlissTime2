#include <pebble.h>
#include "clock.h"
#include "base.h"
#include "calendar.h"
#include "chime.h"
#include "status_disp.h"
#include "app_messaging.h"

#define CLOCK_POS 52

static void display_time_digital( struct tm *tick_time );

static struct CONFIG_PARAMS config_params;
static bool date_shown = false; //
static TextLayer *clock_layer4; // Digital only

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void get_config( struct CONFIG_PARAMS params ) {
  time_t timeInSecs = time( NULL );
  struct tm *localTime = localtime( &timeInSecs );
  
  memset( &config_params, 0, sizeof( struct CONFIG_PARAMS ) );
  config_params = params;
  display_status( localTime, config_params, true );

}

static void display_time_digital( struct tm *tick_time ) {
  static char hour_text[] = "xx:xx";

  strftime( hour_text, sizeof( hour_text ), config_params.digital_clock_type_12_24 == 1 ? "%H:%M" : "%I:%M", tick_time );
  
  // This is a hack to get rid of the leading zero.
  if(hour_text[0] == '0') memmove( &hour_text[0], &hour_text[1], sizeof( hour_text ) - 1 );

  text_layer_set_text( clock_layer4, hour_text );
}

static void show_time( struct tm *tick_time ) {
  
  display_time_digital( tick_time );

  if ( tick_time->tm_min == 0 || !date_shown ) {
    show_weeks( tick_time->tm_mon, tick_time->tm_mday, tick_time->tm_wday, tick_time->tm_year );
    date_shown = true;
  }
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  
  show_time( tick_time );
  display_status( tick_time, config_params, false );
  do_chime( tick_time, config_params );
  
}

void clock_init( void ) {

  clock_layer4 = text_layer_create( GRect( 0, CLOCK_POS + 23, 144, 60 ) );
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


