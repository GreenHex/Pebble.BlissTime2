#include <pebble.h>
#include "weather.h"
#include "base.h"
#include "app_messaging.h"

static TextLayer *s_weather_layer = 0;
static int temp_units = 0;

void setTempUnits( int units ) {
  temp_units = units;
}

void handle_weather_message( DictionaryIterator *iterator ) {

  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];

  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

  if(temp_tuple && conditions_tuple) {
  
    snprintf( temperature_buffer, sizeof(temperature_buffer), "%s", temp_tuple->value->cstring );
    snprintf( conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring );

    // Assemble full string and display
    snprintf( weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer );
    text_layer_set_text( s_weather_layer, weather_layer_buffer );
  }
}

void clear_weather() {
  if (s_weather_layer)  text_layer_set_text(s_weather_layer, "");
}

void weather_init() {
  s_weather_layer = text_layer_create(GRect(2, 1, 130, 16)); // GRect(110,1,34,16));
  setup_text_layer(s_weather_layer, RESOURCE_ID_FONT_DROIDSANS_12);
  clear_weather();
}

void weather_deinit() {
  text_layer_destroy(s_weather_layer);
}

void request_weather( void ) {
  send_request( CMD_WEATHER );
}


/////// STOCKS STUFF
void handle_stocks_message( DictionaryIterator *iterator ) {
  
  static char stocks_layer_buffer[32];

  /*
  Tuple *t_stock_code = dict_find(iterator, MESSAGE_KEY_STOCK_CODE );
  if (t_stock_code) {
    if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Got STOCK CODE");
  }
  */
  Tuple *t_cmp = dict_find(iterator, MESSAGE_KEY_CMP );
  if(t_cmp) {
    snprintf( stocks_layer_buffer, sizeof(stocks_layer_buffer), "%s", t_cmp->value->cstring );
    text_layer_set_text( s_weather_layer, stocks_layer_buffer );
  }
  
}

void request_stock( void ) {
  send_request( CMD_STOCKS );
}

