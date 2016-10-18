#include <pebble.h>
#include "global.h"
#include "calendar.h"

static Layer *window_layer = 0;
static BitmapLayer *line_layer = 0;
static BitmapLayer *active_day_layers[ 7 ];
static TextLayer *layers[ 21 ];
static GFont cal_font;

static char days[ 32 ][ 3 ] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                               "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                               "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" };
static int days_in_month[ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char* days_of_week[ 7 ] = { "S", "M", "T", "W", "T", "F", "S" };

static void clear_active_bitmap_layers() {
  for ( int i = 0; i < 7; i++ ) {
    bitmap_layer_set_background_color( active_day_layers[ i ], GColorClear );
  }
}

void show_weeks( struct tm *tick_time ) {
  int current_month = tick_time->tm_mon;
  int current_day = tick_time->tm_mday;
  int day_of_week = tick_time->tm_wday;
  int current_year = tick_time->tm_year;

  if (((current_year % 4 == 0) && (current_year % 100 != 0)) || (current_year % 400 == 0)) {
    days_in_month[1] = 29;
  }
  
  clear_active_bitmap_layers();

  int prev_month = current_month - 1;
  if ( prev_month < 0 ) prev_month = 11;
  int start = current_day - day_of_week;

  for ( int i = 0; i < 14; i++ ) {
    int day = start + i;
    if ( day < 1 ) {
      day = days_in_month[ prev_month ] + i + start;
    }
    else if ( days_in_month[current_month] < day ) {
      day = day - days_in_month[ current_month ];
    }
    text_layer_set_text( layers[ 7 + i ], days[ day ] );

    #if defined( PBL_COLOR )
    if ( ( i == 0 ) || ( i == 6 ) || ( i == 7 ) || ( i == 13 ) ) {
      text_layer_set_text_color( layers[ 7 + i ], COLOUR_CAL_WEEKENDS_TXT );
    } else {
      text_layer_set_text_color( layers[ 7 + i ], COLOUR_CAL_WEEKDAYS_TXT );
    }
    # else 
    text_layer_set_text_color( layers[ 7 + i ], GColorBlack );
    #endif
  }

  // today
  GColor bg_highlight_colour = GColorBlack;
  if ( ( day_of_week == 0 ) || ( day_of_week == 6 ) ) {
    bg_highlight_colour = COLOUR_CAL_TODAY_WEEKENDS_BG;
  } else {
    bg_highlight_colour = COLOUR_CAL_TODAY_WEEKDAYS_BG;
  }
  bitmap_layer_set_background_color( active_day_layers[ day_of_week ], bg_highlight_colour );
  text_layer_set_text_color( layers[ 7 + day_of_week ], COLOUR_CAL_TODAY_FG );
}

void calendar_init( Window *window ) {
  window_layer = window_get_root_layer( window );

  // Black line at top
  line_layer = bitmap_layer_create( GRect( 5, 17, 133, 1 ) );
  bitmap_layer_set_background_color( line_layer, COLOUR_LINE_LAYER );
  layer_add_child( window_layer, bitmap_layer_get_layer( line_layer ) );

  // Active day bitmaps
  for ( int col = 0; col < 7; col++ ) {
    active_day_layers[ col ] = bitmap_layer_create( GRect( 2 + ( 20 * col ), 30, 20, 11 ) );
    bitmap_layer_set_background_color( active_day_layers[ col ], GColorClear );
    layer_add_child( window_layer, bitmap_layer_get_layer( active_day_layers[ col ] ) );
  }

  // Dates...
  cal_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DROIDSANS_13 ) );
  for ( int row = 0; row < 3 ; row++ ) {
    for ( int col = 0, layer_idx = 0; col < 7; col++ ) {
      layer_idx = col + row * 7;
      // setup layer
      layers[ layer_idx ] = text_layer_create(GRect( 2 + ( 20 * col ), 16 + ( 11 * row ), 20, 16 ) );
      text_layer_set_background_color( layers[ layer_idx ], GColorClear );
      text_layer_set_text_alignment( layers[ layer_idx ], GTextAlignmentCenter );
      // text_layer_set_text_color( layers[ layer_idx ], COLOUR_CAL_WEEKDAYS_TXT );
      text_layer_set_font( layers[ layer_idx ], cal_font );
      layer_add_child( window_layer, text_layer_get_layer( layers[ layer_idx ] ) );
    }
  }

  // Days-of-the-week...
  for ( int col = 0; col < 7; col++ ) {
    #if defined( PBL_COLOR )
    if ( ( col == 0 ) || ( col == 6 ) ) {
      text_layer_set_text_color( layers[ col ], COLOUR_CAL_WEEKENDS_LABEL );
    } else {
      text_layer_set_text_color( layers[ col ], COLOUR_CAL_WEEKDAYS_LABEL );
    }
    # else 
    text_layer_set_text_color( layers[ col ], GColorBlack );
    #endif
    text_layer_set_text( layers[ col ], days_of_week[ col ] );
  }

  time_t now = time( NULL );
  struct tm *tick_time = localtime( &now );

  show_weeks( tick_time );
}

void calendar_deinit() {
  for ( int i = 0; i < 7; i++ ) {
    bitmap_layer_destroy( active_day_layers[ i ] );
  }
  for ( int i = 0; i < 21; i++ ) {
    text_layer_destroy( layers[ i ] );
  }
  bitmap_layer_destroy( line_layer );
  fonts_unload_custom_font( cal_font );
}