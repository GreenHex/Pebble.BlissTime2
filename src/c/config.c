#include <pebble.h>
#include "config.h"
#include "base.h"
#include "clock.h"
#include "weather.h"
#include "app_messaging.h"

int stringToInt(char *str);

void handle_config_message( DictionaryIterator *iterator ) {
  
  if ( !iterator ) return;
  
  int display_type = 0; // 0 - Nothing, 1 - Weather, 2 - Stocks
  int update_interval = 30; // mins
  int updates_start_at = 9;
  int updates_end_at = 17;
  int clock_type = 0; // 0 - Digital 12h, 1 - Digital 24h
  int chime_interval = 2; // 0 - Never, 1 - 1/2 hourly, 2 - hourly
  uint32_t chime_offset = 0;
  int chime_start_time = 0;
  int chime_end_time = 23;
  uint8_t on_days[7] = {1,1,1,1,1,1,1};
  time_t timeInSecs;
  struct tm *localTime;
  
  Tuple *t_display_type = dict_find(iterator, MESSAGE_KEY_DISPLAY_TYPE);
  if ( t_display_type ) {
    display_type = stringToInt( (char*) t_display_type->value->data );
  }
  Tuple *t_update_interval = dict_find( iterator, MESSAGE_KEY_UPDATE_INTERVAL ); // KEY RETURNING uint32
  if ( t_update_interval) {
    update_interval = t_update_interval->value->int32;
  }
  Tuple *t_update_start_time = dict_find( iterator, MESSAGE_KEY_UPDATE_START_TIME );
  if ( t_update_start_time) {
    updates_start_at = stringToInt( (char*) t_update_start_time->value->data );
  }
  Tuple *t_update_end_time = dict_find( iterator, MESSAGE_KEY_UPDATE_END_TIME );
  if ( t_update_end_time) {
    updates_end_at = stringToInt( (char*) t_update_end_time->value->data );
  }
  
  clear_weather();
  timeInSecs = time( NULL );
  localTime = localtime( &timeInSecs );
  
  if ( is_X_in_range( updates_start_at, updates_end_at, localTime->tm_hour ) ) {
    if ( display_type == 1 ) {
      request_weather();
    } else if ( display_type == 2 ) {
      request_stock();
    }
  }
  
  Tuple *t_clock_type = dict_find( iterator, MESSAGE_KEY_CLOCK_TYPE ); 
  if ( t_clock_type ) {
    clock_type = stringToInt( (char*) t_clock_type->value->data );
    configure_clock( clock_type );
  }
  Tuple *t_chime_interval = dict_find( iterator, MESSAGE_KEY_CHIME_INTERVAL ); 
  if ( t_chime_interval ) {
    chime_interval = stringToInt( (char*) t_chime_interval->value->data );
  }
  Tuple *t_chime_offset = dict_find( iterator, MESSAGE_KEY_CHIME_OFFSET ); // KEY RETURNING uint32
  if ( t_chime_offset ) {
    chime_offset = t_chime_offset->value->int32;
  }
  Tuple *t_start_time = dict_find( iterator, MESSAGE_KEY_CHIME_START_TIME );
  if ( t_start_time ) {
    chime_start_time = stringToInt( (char*) t_start_time->value->data );
  }
  Tuple *t_end_time = dict_find( iterator, MESSAGE_KEY_CHIME_END_TIME );
  if ( t_end_time ) {
    chime_end_time = stringToInt( (char*) t_end_time->value->data );
  }
  Tuple *t_on_days = NULL;
  for (int i = 0 ; i < 7 ; i++) {
    t_on_days = dict_find( iterator, MESSAGE_KEY_ON_DAYS + i );
    if ( t_on_days ) {
      on_days[i] = t_on_days->value->uint8;
    }
  }  
  configure_buzz( display_type, update_interval, updates_start_at, updates_end_at, chime_interval, (int) chime_offset, chime_start_time, chime_end_time, on_days[0], on_days[1], on_days[2], on_days[3], on_days[4], on_days[5], on_days[6] );
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
