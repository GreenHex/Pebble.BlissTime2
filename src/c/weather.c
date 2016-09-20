#include <pebble.h>
#include "base.h"
#include "weather.h"

static TextLayer *s_weather_layer;

void handle_weather_message(DictionaryIterator *iterator) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];

  if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Message received.");
  
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
  }
}

void weather_init() {
  s_weather_layer = text_layer_create(GRect(2, 1, 130, 16)); // GRect(110,1,34,16));
  setup_text_layer(s_weather_layer, RESOURCE_ID_FONT_DROIDSANS_12);
  text_layer_set_text(s_weather_layer, "Loading...");
}

void weather_deinit() {
  text_layer_destroy(s_weather_layer);
}
