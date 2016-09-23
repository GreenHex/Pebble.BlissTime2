#include <pebble.h>
#include "config.h"
#include "base.h"
#include "clock.h"
#include "weather.h"
#include "app_messaging.h"

int stringToInt(char *str);

void handle_config_message(DictionaryIterator *iterator) {
  
  int display_type = 0; // 0 - Weather, 1 - Stocks
  int temp_units = 0; // 0 - Celsius, 1 - Fahrenheit, 2 - Kelvin
  // char* stock_code;
  int clock_type = 0; // 0 - Digital 12h, 1 - Digital 24h
  int chime_interval = 2; // 0 - Never, 0 - 1/2 hourly, 2 - hourly
  uint32_t chime_offset = 0;
  int chime_start_time = 0;
  int chime_end_time = 23;
  uint8_t on_days[7] = {1,1,1,1,1,1,1};

  Tuple *t_display_type = dict_find(iterator, MESSAGE_KEY_DISPLAY_TYPE);
  if (t_display_type) {
    display_type = stringToInt( (char*) t_display_type->value->data );
  }
  Tuple *t_temp_units = dict_find(iterator, MESSAGE_KEY_TEMPERATURE_UNITS); 
  if (t_temp_units) {
    temp_units = stringToInt( (char*) t_temp_units->value->data );
    setTempUnits( temp_units ); 
  }
  /*
  Tuple *t_stock_code = dict_find(iterator, MESSAGE_KEY_STOCK_CODE);
  if (t_stock_code) {
    // DO NOTHING HERE, its all done in PEBBLEKIT JS
  }
  */
  if (display_type) {
    request_stock();
  } else {
    request_weather();
  }
  Tuple *t_clock_type = dict_find(iterator, MESSAGE_KEY_CLOCK_TYPE); 
  if (t_clock_type) {
    clock_type = stringToInt( (char*) t_clock_type->value->data );
    configure_clock( clock_type );
  }
  Tuple *t_chime_interval = dict_find(iterator, MESSAGE_KEY_CHIME_INTERVAL); 
  if (t_chime_interval) {
    chime_interval = stringToInt( (char*) t_chime_interval->value->data );
  }
  Tuple *t_chime_offset = dict_find(iterator, MESSAGE_KEY_CHIME_OFFSET); // ONLY KEY RETURNING uint32
  if (t_chime_offset) {
    chime_offset = t_chime_offset->value->int32;
  }
  Tuple *t_start_time = dict_find(iterator, MESSAGE_KEY_START_TIME);
  if (t_start_time) {
    chime_start_time = stringToInt( (char*) t_start_time->value->data );
  }
  Tuple *t_end_time = dict_find(iterator, MESSAGE_KEY_END_TIME);
  if (t_end_time) {
    chime_end_time = stringToInt( (char*) t_end_time->value->data );
  }
  Tuple *t_on_days = NULL;
  for (int i = 0 ; i < 7 ; i++) {
    t_on_days = dict_find(iterator, MESSAGE_KEY_ON_DAYS + i);
    if (t_on_days) {
      on_days[i] = t_on_days->value->uint8;
    }
  }  
  configure_buzz( display_type, chime_interval, (int) chime_offset, chime_start_time, chime_end_time, on_days[0], on_days[1], on_days[2], on_days[3], on_days[4], on_days[5], on_days[6] );
}

int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
             if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to convert it into integer.");
             return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}
