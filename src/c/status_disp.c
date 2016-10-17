#include <pebble.h>
#include "global.h"
#include "status_disp.h"
#include "app_messaging.h"

static Layer *window_layer = 0;
static TextLayer *s_status_layer = 0;
static GFont status_font = 0;

void show_status( Tuple *tuple_ptr, DictionaryIterator *iterator ) {
  static char buff[32] = "";
  Tuple* colour_tuple = ( iterator ) ? dict_find( iterator, MESSAGE_KEY_STATUS_COLOUR ) : 0;
  GColor status_colour = GColorBlack;

  if ( tuple_ptr ) {
    #if defined( PBL_COLOR )
    if ( colour_tuple ) {
      status_colour = GColorFromHEX( colour_tuple->value->int32 );
    }
    #endif
    snprintf( buff, sizeof( buff ), "%s", tuple_ptr->value->cstring );
    text_layer_set_text_color( s_status_layer, status_colour);    
    text_layer_set_text( s_status_layer, buff );
  }
}

void clear_status( void ) {
  if ( s_status_layer ) text_layer_set_text( s_status_layer, "" );
}

void get_status( struct tm *tick_time, bool ignoreUpdateInterval ) {
  if ( ! ( persist_read_int( MESSAGE_KEY_STATUS_DISPLAY_TYPE ) ) ) { // CMD_UNDEFINED
    clear_status();
    return;
  }

  if ( ! is_X_in_range( persist_read_int( MESSAGE_KEY_STATUS_UPDATE_START_TIME ), 
                       persist_read_int( MESSAGE_KEY_STATUS_UPDATE_END_TIME ), tick_time->tm_hour ) ) {
    clear_status();
    return;
  }

  if ( ! persist_read_bool( MESSAGE_KEY_STATUS_UPDATE_ON_DAYS + tick_time->tm_wday ) ) {
    clear_status();
    return;
  }

  if ( ( ! ignoreUpdateInterval ) && ( tick_time->tm_min % ( persist_read_int( MESSAGE_KEY_STATUS_UPDATE_INTERVAL ) ) ) ) return;

  send_request( persist_read_int( MESSAGE_KEY_STATUS_DISPLAY_TYPE ) ); // Remember: CMD_WEATHER = 1, CMD_STOCKS = 2
}

void status_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  s_status_layer = text_layer_create( GRect( 2, 1, 130, 15 ) );
  status_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DROIDSANS_12 ) );
  text_layer_set_font( s_status_layer, status_font );
  text_layer_set_background_color( s_status_layer, GColorClear );
  layer_add_child( window_layer, text_layer_get_layer( s_status_layer ) );
}

void status_deinit() {
  text_layer_destroy( s_status_layer );
  fonts_unload_custom_font( status_font );
}