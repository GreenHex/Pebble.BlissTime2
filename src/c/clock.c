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
#define SEC_HAND_LENGTH 45
#define SEC_HAND_TAIL_LENGTH 20
#define MIN_HAND_LENGTH 40
#define HOUR_HAND_LENGTH 26
#define SEC_HAND_WIDTH 1
#define MIN_HAND_WIDTH 4
#define HOUR_HAND_WIDTH 6
#define CENTER_DOT_SIZE 8
#define CLOCK_TEXT_Y_POS 22
#define SECONDS_DISPLAY_TIMEOUT_MS 30000

static Layer *window_layer = 0;
static BitmapLayer *analog_clock_bitmap_layer = 0;
static Layer *analog_clock_layer = 0;
static TextLayer *digital_clock_text_layer = 0;
static BitmapLayer *top_black_out_layer = 0;
static GBitmap *analog_clock_bitmap;
static AppTimer* secs_display_apptimer = 0; 

static void start_seconds_display( AccelAxisType axis, int32_t direction );

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void draw_clock( void ) {
  time_t timeInSecs = time( NULL );
  struct tm *localTime = localtime( &timeInSecs );
  
  if ( ( (int) persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == 1 ) { // analog
    layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), true );
    layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), false );
    layer_set_hidden( analog_clock_layer, false );
    accel_tap_service_subscribe( start_seconds_display );
  } else { // digital
    layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), true );
    layer_set_hidden( analog_clock_layer, true );
    layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), false );
    accel_tap_service_unsubscribe();
  }
  get_status( localTime, true );
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  
  // if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tick_time->tm_hour, tick_time->tm_min, tick_time->tm_sec );
  
  if ( units_changed & DAY_UNIT ) show_weeks( tick_time );
  
  if ( ( (int) persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == 1 ) { // global
    layer_mark_dirty( analog_clock_layer );
  } else {
    layer_mark_dirty( text_layer_get_layer( digital_clock_text_layer ) );
  }
  
  get_status( tick_time, false );
  
  do_chime( tick_time );
}

static void digital_clock_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  time_t now = time( NULL );
  struct tm *t = localtime( &now );
  static char str_time[] = "xx:xx";
  
  strftime( str_time, sizeof( str_time ), ( (int) persist_read_int( MESSAGE_KEY_DIGITAL_CLOCK_TYPE_12_OR_24 ) ) == 1 ? "%H:%M" : "%I:%M", t );
  
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
  GPoint sec_hand = GPoint( 0, 0 );
  GPoint sec_hand_tail = GPoint( 0, 0 );
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
  
  // graphics_context_set_fill_color( ctx, GColorBlack );
  // graphics_fill_rect( ctx, layer_bounds, 0, GCornersAll );
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_circle( ctx, center_pt, CENTER_DOT_SIZE );
  //
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_context_set_stroke_width( ctx, HOUR_HAND_WIDTH );
  graphics_draw_line( ctx, hour_hand, center_pt );  
  //
  graphics_context_set_stroke_color( ctx, GColorBlack );
  graphics_context_set_stroke_width( ctx, 1 );
  graphics_draw_circle( ctx, center_pt, CENTER_DOT_SIZE - 1 );
  //
  graphics_context_set_stroke_width( ctx, MIN_HAND_WIDTH + 2);
  graphics_draw_line( ctx, min_hand, center_pt );
  graphics_fill_circle( ctx, center_pt, CENTER_DOT_SIZE - 2);
  //
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_context_set_stroke_width( ctx, MIN_HAND_WIDTH );
  graphics_draw_line( ctx, min_hand, center_pt );
  //
  struct ANALOG_LAYER_DATA *layer_data = layer_get_data( layer );
  if ( layer_data->show_seconds ) {
    int32_t sec_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
    int32_t sec_tail_angle = sec_angle + ( TRIG_MAX_ANGLE / 2 );
    sec_hand.y = ( -cos_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
    sec_hand.x = ( sin_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;
    sec_hand_tail.y = ( -cos_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
    sec_hand_tail.x = ( sin_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;
    
    graphics_context_set_stroke_color( ctx, GColorBlack );
    graphics_context_set_stroke_width( ctx, 1 );
    graphics_draw_circle( ctx, center_pt, CENTER_DOT_SIZE - 3 );
    //
    graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH + 2);
    graphics_draw_line( ctx, sec_hand, sec_hand_tail );
    graphics_fill_circle( ctx, center_pt, CENTER_DOT_SIZE - 4);
    //
    graphics_context_set_stroke_color( ctx, GColorWhite );
    graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH );
    graphics_draw_line( ctx, sec_hand, sec_hand_tail );
  }
}

static void prv_unobstructed_change( AnimationProgress progress, void *window_root_layer ) {
  
  GRect unobstructed_bounds = layer_get_unobstructed_bounds( window_root_layer );
  GRect full_bounds = layer_get_bounds( window_root_layer );
  
  GRect analog_clock_unobstructed_frame = layer_get_frame( analog_clock_layer );
  GRect digital_clock_unobstructed_frame = layer_get_frame( text_layer_get_layer( digital_clock_text_layer ) );
  
  int16_t obstruction_height = full_bounds.size.h - unobstructed_bounds.size.h;
  
  analog_clock_unobstructed_frame.origin.y = CLOCK_POS_Y - obstruction_height;
  digital_clock_unobstructed_frame.origin.y = CLOCK_POS_Y - obstruction_height;

  layer_set_frame( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_unobstructed_frame );
  layer_set_frame( text_layer_get_layer( digital_clock_text_layer ), digital_clock_unobstructed_frame );

}

static void prv_unobstructed_did_change( void *context ) {
  
  GRect unobstructed_bounds = layer_get_unobstructed_bounds( window_layer );
  GRect full_bounds = layer_get_bounds( window_layer );
  
  layer_set_hidden( bitmap_layer_get_layer( top_black_out_layer ), grect_equal( &full_bounds, &unobstructed_bounds) );  
}

static void stop_seconds_display( void* data ) { // after timer elapses
  
  // app_timer_cancel( secs_display_apptimer ); // is this required at all? NO!
  secs_display_apptimer = 0; // if we are here, we know for sure that timer has expired. 
  
  struct ANALOG_LAYER_DATA *layer_data = layer_get_data( analog_clock_layer );
  layer_data->show_seconds = false;
  
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  
  if ( ( (int) persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == 0 ) return;
  
  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );
  
  struct ANALOG_LAYER_DATA *layer_data = layer_get_data( analog_clock_layer );
  layer_data->show_seconds = true;
  //
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, SECONDS_DISPLAY_TIMEOUT_MS );
  } else {
    secs_display_apptimer = app_timer_register( SECONDS_DISPLAY_TIMEOUT_MS, stop_seconds_display, 0 );
  }
}


void clock_init( Window *window ) {
  
  window_layer = window_get_root_layer( window );
  GRect window_bounds = layer_get_bounds( window_layer );
  GRect clock_layer_bounds = GRect( window_bounds.origin.x + CLOCK_POS_X, window_bounds.origin.y + CLOCK_POS_Y, 
                                       window_bounds.size.w - CLOCK_POS_X, window_bounds.size.h - CLOCK_POS_Y );
  //
  analog_clock_bitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_ANALOG_CLOCKFACE );
  analog_clock_bitmap_layer = bitmap_layer_create( clock_layer_bounds );
  bitmap_layer_set_bitmap( analog_clock_bitmap_layer, analog_clock_bitmap );
  layer_add_child( window_layer, bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), true );
  //
  analog_clock_layer = layer_create_with_data( layer_get_bounds( bitmap_layer_get_layer( analog_clock_bitmap_layer ) ),
                                                 sizeof( struct ANALOG_LAYER_DATA ) );
  struct ANALOG_LAYER_DATA *layer_data = layer_get_data( analog_clock_layer );
  layer_data->show_seconds = false;  
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, true );
  //
  digital_clock_text_layer = text_layer_create( clock_layer_bounds );
  layer_add_child( window_layer, text_layer_get_layer( digital_clock_text_layer ) );
  layer_set_update_proc( text_layer_get_layer( digital_clock_text_layer ), digital_clock_text_layer_update_proc );
  layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), true );
  
  UnobstructedAreaHandlers handler = {
    .change = prv_unobstructed_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe( handler, window_layer );
  
  // required for blacking out unsighty while line at top when quick view is on
  top_black_out_layer = bitmap_layer_create( GRect( 0, 0, 144, 2 ) );
  bitmap_layer_set_background_color( top_black_out_layer, GColorBlack );
  layer_add_child( window_layer, bitmap_layer_get_layer( top_black_out_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( top_black_out_layer ), true );
  
  // subscriptions
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  
  // show current time
  draw_clock();
}

void clock_deinit( void ) {
  accel_tap_service_unsubscribe(); // are we over-unsubscribing?
  tick_timer_service_unsubscribe();
  bitmap_layer_destroy( top_black_out_layer );
  text_layer_destroy( digital_clock_text_layer );
  bitmap_layer_destroy( analog_clock_bitmap_layer );
  layer_destroy( analog_clock_layer );
}

