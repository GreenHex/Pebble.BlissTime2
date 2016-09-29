#include <pebble.h>
#include "config.h"
#include "base.h"
#include "clock.h"
#include "weather.h"
#include "app_messaging.h"

int stringToInt(char *str);

void handle_config_message( DictionaryIterator *iterator ) {
  
  if ( !iterator ) return;

  // status display
  int status_display_type = 0; // 0 - Nothing, 1 - Weather, 2 - Stocks
  int status_update_interval = 30; // mins
  int status_update_start_time = 9;
  int status_update_end_time = 17;
  // clock settings
  int clock_type_digital_analog = 0; // 0 - Digital, 1 - Analog
  int digital_clock_type_12_24 = 0; // 0 - Digital 12h, 1 - Digital 24h
  // chime settings
  int chime_interval = 2; // 0 - Never, 1 - 1/2 hourly, 2 - hourly
  int chime_start_time = 0;
  int chime_end_time = 23;
  uint8_t chime_on_days[7] = { 1, 1, 1, 1, 1, 1, 1 };
  uint32_t chime_offset = 0;
  
  // status display
  Tuple *t_status_display_type = dict_find( iterator, MESSAGE_KEY_STATUS_DISPLAY_TYPE );
  if ( t_status_display_type ) {
    status_display_type = stringToInt( (char*) t_status_display_type->value->data );
  }
  Tuple *t_status_update_interval = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_INTERVAL ); // KEY RETURNING uint32
  if ( t_status_update_interval) {
    status_update_interval = (int) t_status_update_interval->value->int32;
  }
  Tuple *t_status_update_start_time = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_START_TIME );
  if ( t_status_update_start_time ) {
    status_update_start_time = stringToInt( (char*) t_status_update_start_time->value->data );
  }
  Tuple *t_status_update_end_time = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_END_TIME );
  if ( t_status_update_end_time ) {
    status_update_end_time = stringToInt( (char*) t_status_update_end_time->value->data );
  }
  // clock settings
  Tuple *t_clock_type_digital_analog = dict_find( iterator, MESSAGE_KEY_CLOCK_TYPE_DIGITAL_ANALOG );
  if ( t_clock_type_digital_analog ) {
    clock_type_digital_analog = stringToInt( (char*) t_clock_type_digital_analog->value->data );
  }  
  Tuple *t_digital_clock_type_12_24 = dict_find( iterator, MESSAGE_KEY_DIGITAL_CLOCK_TYPE_12_24 ); 
  if ( t_digital_clock_type_12_24 ) {
    digital_clock_type_12_24 = stringToInt( (char*) t_digital_clock_type_12_24->value->data );
  }
  // chime settings
  Tuple *t_chime_interval = dict_find( iterator, MESSAGE_KEY_CHIME_INTERVAL ); 
  if ( t_chime_interval ) {
    chime_interval = stringToInt( (char*) t_chime_interval->value->data );
  }
  Tuple *t_start_time = dict_find( iterator, MESSAGE_KEY_CHIME_START_TIME );
  if ( t_start_time ) {
    chime_start_time = stringToInt( (char*) t_start_time->value->data );
  }
  Tuple *t_end_time = dict_find( iterator, MESSAGE_KEY_CHIME_END_TIME );
  if ( t_end_time ) {
    chime_end_time = stringToInt( (char*) t_end_time->value->data );
  }
  Tuple *t_chime_on_days = NULL;
  for (int i = 0 ; i < 7 ; i++) {
    t_chime_on_days = dict_find( iterator, MESSAGE_KEY_CHIME_ON_DAYS + i );
    if ( t_chime_on_days ) {
      chime_on_days[i] = t_chime_on_days->value->uint8;
    }
  }
  Tuple *t_chime_offset = dict_find( iterator, MESSAGE_KEY_CHIME_OFFSET ); // KEY RETURNING uint32
  if ( t_chime_offset ) {
    chime_offset = t_chime_offset->value->int32;
  }

  configure_buzz( status_display_type,
                   status_update_interval, 
                   status_update_start_time,
                   status_update_end_time,
                   clock_type_digital_analog,
                   digital_clock_type_12_24,  
                   chime_interval, 
                   chime_start_time,
                   chime_end_time,
                   chime_on_days[0],
                   chime_on_days[1],
                   chime_on_days[2],
                   chime_on_days[3],
                   chime_on_days[4],
                   chime_on_days[5],
                   chime_on_days[6],
                   (int) chime_offset
                 );

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
