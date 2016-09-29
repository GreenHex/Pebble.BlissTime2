#include <pebble.h>
#include "weather.h"
#include "base.h"
#include "app_messaging.h"

static TextLayer *s_weather_layer = 0;

void handle_message( Tuple *tupple_ptr ) {
  static char buff[32];
  if ( tupple_ptr ) {
    snprintf( buff, sizeof( buff ), "%s", tupple_ptr->value->cstring );
    text_layer_set_text( s_weather_layer, buff );
  }
}

void clear_weather( void ) {
  if (s_weather_layer) text_layer_set_text( s_weather_layer, "" );
}

void weather_init() {
  s_weather_layer = text_layer_create( GRect( 2, 1, 130, 16 ) );
  setup_text_layer( s_weather_layer, RESOURCE_ID_FONT_DROIDSANS_12 );
}

void weather_deinit() {
  text_layer_destroy( s_weather_layer );
}

void request_weather( void ) {
  send_request( CMD_WEATHER );
}

/////// STOCKS STUFF
void request_stock( void ) {
  send_request( CMD_STOCKS );
}

