#include <pebble.h>
#include "global.h"
#include "status_disp.h"
#include "app_messaging.h"

static Layer *window_layer = 0;
static TextLayer *s_status_layer = 0;
static GFont status_font = 0;

void show_status( Tuple *tupple_ptr ) {
  static char buff[32];
  if ( tupple_ptr ) {
    snprintf( buff, sizeof( buff ), "%s", tupple_ptr->value->cstring );
    text_layer_set_text( s_status_layer, buff );
  }
}

void clear_status( void ) {
  if ( s_status_layer ) text_layer_set_text( s_status_layer, "" );
}

void request_weather( void ) {
  send_request( CMD_WEATHER );
}

void request_stock( void ) {
  send_request( CMD_STOCKS );
}

void get_status( struct tm *tick_time, bool ignoreUpdateInterval ) {
  
  if ( ! ( (int) persist_read_int( MESSAGE_KEY_STATUS_DISPLAY_TYPE ) ) ) {
    clear_status();
    return;
  }
    
  if ( ! is_X_in_range( (int) persist_read_int( MESSAGE_KEY_STATUS_UPDATE_START_TIME ), 
                         (int) persist_read_int( MESSAGE_KEY_STATUS_UPDATE_END_TIME), tick_time->tm_hour ) ) {
    clear_status();
    return;
  }
  
  if ( ! persist_read_bool( MESSAGE_KEY_STATUS_UPDATE_ON_DAYS + tick_time->tm_wday ) ) {
    clear_status();
    return;
  }
    
  if ( ( ! ignoreUpdateInterval ) && ( tick_time->tm_min % ( (int) persist_read_int( MESSAGE_KEY_STATUS_UPDATE_INTERVAL ) ) ) ) return;
      
  if ( ( (int) persist_read_int( MESSAGE_KEY_STATUS_DISPLAY_TYPE ) ) == 1 ) {

    request_weather();

  } else if ( ( (int) persist_read_int( MESSAGE_KEY_STATUS_DISPLAY_TYPE ) ) == 2 ) {

    request_stock();
  }

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





