#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "base.h"
#include "calendar.h"
#include "chime.h"
#include "status_disp.h"
#include "app_messaging.h"

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 52 + 1
#define MIN_HAND_LENGTH 40
#define HOUR_HAND_LENGTH 26
#define MIN_HAND_WIDTH 4
#define HOUR_HAND_WIDTH 6
#define CENTER_DOT_SIZE 8
#define CLOCK_TEXT_Y_POS 22

static Layer *window_layer = 0;
static Layer *analog_clock_layer = 0;
static TextLayer *digital_clock_layer = 0;
static struct CONFIG_PARAMS config_params;

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void get_config( struct CONFIG_PARAMS params ) {
  time_t timeInSecs = time( NULL );
  struct tm *localTime = localtime( &timeInSecs );
  
  memset( &config_params, 0, sizeof( struct CONFIG_PARAMS ) );
  config_params = params; // copy to global
  
  if ( config_params.clock_type_digital_or_analog == 1 ) { // global
    layer_set_hidden( analog_clock_layer, false );
    layer_set_hidden( text_layer_get_layer( digital_clock_layer ), true );    
  } else {
    layer_set_hidden( text_layer_get_layer( digital_clock_layer ), false );
    layer_set_hidden( analog_clock_layer, true );
  }
  get_status( localTime, &config_params, true );
  
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  
  // if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tick_time->tm_hour, tick_time->tm_min, tick_time->tm_sec );
  
  if ( tick_time->tm_min == 0 ) {
    show_weeks( tick_time );
  }
  
  if ( config_params.clock_type_digital_or_analog == 1 ) { // global
    layer_mark_dirty( analog_clock_layer );
  } else {
    layer_mark_dirty( text_layer_get_layer( digital_clock_layer ) );
  }
  
  get_status( tick_time, &config_params, false );
  
  do_chime( tick_time, &config_params );
}

static void digital_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  time_t now = time( NULL );
  struct tm *t = localtime( &now );
  static char str_time[] = "xx:xx";
  
  strftime( str_time, sizeof( str_time ), config_params.digital_clock_type_12_or_24 == 1 ? "%H:%M" : "%I:%M", t );
  
  // This is a hack to get rid of the leading zero.
  if(str_time[0] == '0') memmove( &str_time[0], &str_time[1], sizeof( str_time ) - 1 );
  
  GRect layer_bounds = layer_get_bounds( layer );
  GRect text_bounds = layer_bounds;
  text_bounds.origin.y += CLOCK_TEXT_Y_POS;
  graphics_context_set_fill_color( ctx, GColorBlack );
  graphics_fill_rect( ctx, layer_bounds, 0, GCornersAll );
  graphics_context_set_text_color( ctx, GColorWhite );
  graphics_draw_text( ctx, str_time, fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_EXO_50 ) ), 
                       text_bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, 0 );
}

static void analog_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  time_t now = time( NULL );
  struct tm *t = localtime( &now );
  GPoint min_hand = GPoint( 0, 0 );
  GPoint hour_hand = GPoint( 0, 0 );
  
  GRect layer_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &layer_bounds );

  int32_t hour_angle = ((TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  hour_hand.y = ( -cos_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
  hour_hand.x = ( sin_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;
  
  int32_t min_angle = TRIG_MAX_ANGLE * t->tm_min / 60;
  min_hand.y = ( -cos_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
  min_hand.x = ( sin_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;
  
  graphics_context_set_fill_color( ctx, GColorBlack );
  graphics_fill_rect( ctx, layer_bounds, 0, GCornersAll );
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_circle( ctx, center_pt, CENTER_DOT_SIZE );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_context_set_stroke_width( ctx, HOUR_HAND_WIDTH );
  graphics_draw_line( ctx, hour_hand, center_pt );  
  graphics_context_set_stroke_color( ctx, GColorBlack );
  graphics_context_set_stroke_width( ctx, 1 );
  graphics_draw_circle( ctx, center_pt, CENTER_DOT_SIZE - 1 );
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_context_set_stroke_width( ctx, MIN_HAND_WIDTH );
  graphics_draw_line( ctx, min_hand, center_pt );
}

void clock_init( Window *window ) {
  
  window_layer = window_get_root_layer( window );
  GRect window_bounds = layer_get_bounds( window_layer );
  GRect clock_layer_bounds = GRect( window_bounds.origin.x + CLOCK_POS_X, window_bounds.origin.y + CLOCK_POS_Y, 
                                       window_bounds.size.w - CLOCK_POS_X, window_bounds.size.h - CLOCK_POS_Y );
  
  analog_clock_layer = layer_create( clock_layer_bounds );
  layer_add_child( window_layer, analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, true );
  
  digital_clock_layer = text_layer_create( clock_layer_bounds );
  layer_add_child( window_layer, text_layer_get_layer( digital_clock_layer ) );
  layer_set_update_proc( text_layer_get_layer( digital_clock_layer ), digital_clock_layer_update_proc );
  layer_set_hidden( text_layer_get_layer( digital_clock_layer ), true );
  
  // subscription
  time_t now = time( NULL );
  struct tm *t = localtime( &now );
  handle_clock_tick( t, MINUTE_UNIT );
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

void clock_deinit( void ) {
  tick_timer_service_unsubscribe();
  text_layer_destroy( digital_clock_layer );
  layer_destroy( analog_clock_layer );
}

