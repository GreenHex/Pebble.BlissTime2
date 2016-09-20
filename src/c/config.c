#include <pebble.h>
#include "config.h"
#include "base.h"

void handle_config_message(DictionaryIterator *iterator) {
  if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Configuration data received.");
  Tuple *temp_units = dict_find(iterator, MESSAGE_KEY_TEMPERATURE_UNITS);
  if (temp_units) {
    
  }
  Tuple *chime_interval = dict_find(iterator, MESSAGE_KEY_CHIME_INTERVAL);
  if (chime_interval) {
    
  }
  Tuple *start_time = dict_find(iterator, MESSAGE_KEY_START_TIME);
  if (start_time) {
    
  }
  Tuple *end_time = dict_find(iterator, MESSAGE_KEY_END_TIME);
  if (end_time) {
    
  }
  Tuple *on_days = dict_find(iterator, MESSAGE_KEY_ON_DAYS);
  if (on_days) {
    
  }
}
