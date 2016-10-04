#include <pebble.h>
#include "global.h"
#include "status_disp.h"
#include "base.h"
#include "app_messaging.h"

static Layer *window_layer = 0;
static TextLayer *s_status_layer = 0;

void handle_message( Tuple *tupple_ptr ) {
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

void get_status( struct tm *time, struct CONFIG_PARAMS *p_config_params, bool ignoreUpdateInterval ) {
  
  if ( ! p_config_params ) return;
  
  if ( ! p_config_params->status_display_type ) {
    clear_status();
    return;
  }
    
  if ( ! is_X_in_range( p_config_params->status_update_start_time, p_config_params->status_update_end_time, time->tm_hour ) ) {
    clear_status();
    return;
  }
  
  if ( ! p_config_params->status_update_on_days[ time->tm_wday ] ) {
    clear_status();
    return;
  }
    
  if ( ( ! ignoreUpdateInterval ) && ( time->tm_min % p_config_params->status_update_interval ) ) return;
      
  if ( p_config_params->status_display_type == 1 ) {

    request_weather();

  } else if ( p_config_params->status_display_type == 2 ) {

    request_stock();
  }

}

void status_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  s_status_layer = text_layer_create( GRect( 2, 1, 130, 16 ) );
  setup_text_layer( s_status_layer, RESOURCE_ID_FONT_DROIDSANS_12 );
}

void status_deinit() {
  text_layer_destroy( s_status_layer );
}





