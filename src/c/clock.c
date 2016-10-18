#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "calendar.h"
#include "chime.h"
#include "status_disp.h"
#include "app_messaging.h"

static Layer *window_layer = 0;
static BitmapLayer *analog_clock_bitmap_layer = 0;
static Layer *analog_clock_layer = 0;
static TextLayer *digital_clock_text_layer = 0;
static BitmapLayer *top_black_out_layer = 0;
static GBitmap *analog_clock_bitmap = 0;
static GFont large_digital_font = 0;
static AppTimer* secs_display_apptimer = 0; 
static tm tm_time;

static void start_seconds_display( AccelAxisType axis, int32_t direction );

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void draw_clock( void ) {
  time_t now = time( NULL );
  tm_time = *localtime( &now ); // copy to global

  if ( ( persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == CLK_ANALOG ) { // analog
    layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), true );
    layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), false );
    layer_set_hidden( analog_clock_layer, false );
    if ( persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) accel_tap_service_subscribe( start_seconds_display );
  } else { // digital
    layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), true );
    layer_set_hidden( analog_clock_layer, true );
    layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), false );
    accel_tap_service_unsubscribe();
  }
  get_status( &tm_time, true );
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global

  // if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );

  if ( ( units_changed & DAY_UNIT ) == DAY_UNIT ) show_weeks( &tm_time );

  if ( ( units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) get_status( &tm_time, false );

  if ( ( persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == CLK_ANALOG ) {
    layer_mark_dirty( analog_clock_layer );
  } else {
    layer_mark_dirty( text_layer_get_layer( digital_clock_text_layer ) );
  }

  if ( ( units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) do_chime( &tm_time );
}

static void digital_clock_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  // uses global tm_time

  static char str_time[] = "xx:xx";
  int digital_type = (int) persist_read_int( MESSAGE_KEY_DIGITAL_CLOCK_TYPE_12_OR_24 );

  strftime( str_time, sizeof( str_time ), digital_type ? ( digital_type == DIGITAL_24_HOUR ?  "%H:%M" : "%I:%M" ) : ( clock_is_24h_style() ?  "%H:%M" : "%I:%M" ), &tm_time );

  // This is a hack to get rid of the leading zero.
  if( str_time[0] == '0' ) memmove( &str_time[0], &str_time[1], sizeof( str_time ) - 1 );

  GRect layer_bounds = layer_get_bounds( layer );
  GRect text_bounds = layer_bounds;
  text_bounds.origin.y += DIGITAL_CLOCK_TEXT_Y_POS;
  graphics_context_set_fill_color( ctx, COLOUR_DIGITAL_BG );
  graphics_fill_rect( ctx, layer_bounds, 0, GCornersAll );
  graphics_context_set_text_color( ctx, COLOUR_DIGITAL_FG );
  graphics_draw_text( ctx, str_time, large_digital_font, text_bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, 0 );
}

static void draw_clock_hand( struct HAND_DRAW_PARAMS *pDP ) {
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->dot_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius );
  // hand outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width + 2);
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // hand
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width );
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // dot
  graphics_context_set_fill_color( pDP->ctx, pDP->dot_color );
  graphics_fill_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 1 );	
}

static void analog_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  // uses global tm_time

  static struct HAND_DRAW_PARAMS hand_params;
  GPoint hour_hand = GPoint( 0, 0 );
  GPoint min_hand = GPoint( 0, 0 );
  GRect layer_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &layer_bounds );

  int32_t hour_angle = ( ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 ) );
  hour_hand.y = ( -cos_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
  hour_hand.x = ( sin_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;

  int32_t min_angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60;
  min_hand.y = ( -cos_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
  min_hand.x = ( sin_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;

  // hour hand
  hand_params = (struct HAND_DRAW_PARAMS) {
    .ctx = ctx,
    .center_pt = center_pt,
    .from_pt = center_pt,
    .to_pt = hour_hand,
    .hand_width = HOUR_HAND_WIDTH,
    .hand_color = COLOUR_HOUR_HAND,
    .hand_outline_color = COLOUR_HANDS_OUTLINE,
    .dot_radius = CENTER_DOT_RADIUS,
    .dot_color = COLOUR_DOT,
    .dot_outline_color = COLOUR_DOT_OUTLINE
  };
  draw_clock_hand( &hand_params );

  // minute hand
  hand_params = (struct HAND_DRAW_PARAMS) {
    .ctx = ctx,
    .center_pt = center_pt,
    .from_pt = center_pt,
    .to_pt = min_hand,
    .hand_width = MIN_HAND_WIDTH,
    .hand_color = COLOUR_MIN_HAND,
    .hand_outline_color = COLOUR_HANDS_OUTLINE,
    .dot_radius = CENTER_DOT_RADIUS - 2,
    .dot_color = COLOUR_DOT,
    .dot_outline_color = COLOUR_DOT_OUTLINE
  };
  draw_clock_hand( &hand_params );

  if ( ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds ) {
    GPoint sec_hand = GPoint( 0, 0 );
    GPoint sec_hand_tail = GPoint( 0, 0 );
    int32_t sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60;
    int32_t sec_tail_angle = sec_angle + ( TRIG_MAX_ANGLE / 2 );
    sec_hand.y = ( -cos_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
    sec_hand.x = ( sin_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;
    sec_hand_tail.y = ( -cos_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.y;
    sec_hand_tail.x = ( sin_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.x;

   // second hand
    hand_params = (struct HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = sec_hand,
      .to_pt = sec_hand_tail,
      .hand_width = SEC_HAND_WIDTH,
      .hand_color = COLOUR_SEC_HAND,
      .hand_outline_color = COLOUR_HANDS_OUTLINE,
      .dot_radius = CENTER_DOT_RADIUS - 4,
      .dot_color = COLOUR_DOT,
      .dot_outline_color = COLOUR_DOT_OUTLINE
    };
    draw_clock_hand( &hand_params );

    #if defined( PBL_COLOR ) // second hand tip
    GPoint sec_hand_tip = GPoint( 0, 0 );
    sec_hand_tip.y = ( -cos_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.y;
    sec_hand_tip.x = ( sin_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.x;
    graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH );
    graphics_context_set_stroke_color( ctx, COLOUR_SEC_HAND_TIP );
    graphics_draw_line( ctx, sec_hand, sec_hand_tip );
    #endif
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
  if ( secs_display_apptimer) app_timer_cancel( secs_display_apptimer ); // Just for fun.
  secs_display_apptimer = 0; // if we are here, we know for sure that timer has expired. 

  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  if ( ( persist_read_int( MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG ) ) == CLK_DIGITAL ) return;

  if ( ! persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) return;

  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );

  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  //
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000 );
  } else {
    secs_display_apptimer = app_timer_register( (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000, stop_seconds_display, 0 );
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
  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, true );
  //
  digital_clock_text_layer = text_layer_create( clock_layer_bounds );
  layer_add_child( window_layer, text_layer_get_layer( digital_clock_text_layer ) );
  layer_set_update_proc( text_layer_get_layer( digital_clock_text_layer ), digital_clock_text_layer_update_proc );
  layer_set_hidden( text_layer_get_layer( digital_clock_text_layer ), true );
  large_digital_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_EXO_50 ) );

  // required for blacking out unsighty white line at top when quick view is on
  top_black_out_layer = bitmap_layer_create( GRect( 0, 0, 144, 3 ) );
  bitmap_layer_set_background_color( top_black_out_layer, GColorBlack );
  layer_add_child( window_layer, bitmap_layer_get_layer( top_black_out_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( top_black_out_layer ), true );

  // subscriptions
  UnobstructedAreaHandlers handler = {
    .change = prv_unobstructed_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe( handler, window_layer );

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );

  // show current time
  draw_clock();
}

void clock_deinit( void ) {
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer );
  accel_tap_service_unsubscribe(); // are we over-unsubscribing?
  tick_timer_service_unsubscribe();
  bitmap_layer_destroy( top_black_out_layer );
  text_layer_destroy( digital_clock_text_layer );
  bitmap_layer_destroy( analog_clock_bitmap_layer );
  layer_destroy( analog_clock_layer );
  gbitmap_destroy( analog_clock_bitmap );
  fonts_unload_custom_font( large_digital_font );
}
