#include <pebble.h>
#include "config.h"
#include "base.h"
#include "clock.h"
#include "status_disp.h"
#include "app_messaging.h"

int stringToInt( char *str );

void handle_config_message( DictionaryIterator *iterator ) {
  
  if ( !iterator ) return;
  
  struct CONFIG_PARAMS configParams;
  
  memset( &configParams, 0, sizeof( struct CONFIG_PARAMS ) );
  
  // status display
  Tuple *t_status_display_type = dict_find( iterator, MESSAGE_KEY_STATUS_DISPLAY_TYPE );
  if ( t_status_display_type ) {
    configParams.status_display_type = stringToInt( (char*) t_status_display_type->value->data );
  }
  Tuple *t_status_update_interval = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_INTERVAL );
  if ( t_status_update_interval) {
    configParams.status_update_interval = stringToInt( (char*) t_status_update_interval->value->data );
  }
  Tuple *t_status_update_start_time = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_START_TIME );
  if ( t_status_update_start_time ) {
    configParams.status_update_start_time = stringToInt( (char*) t_status_update_start_time->value->data );
  }
  Tuple *t_status_update_end_time = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_END_TIME );
  if ( t_status_update_end_time ) {
    configParams.status_update_end_time = stringToInt( (char*) t_status_update_end_time->value->data );
  }
  Tuple *t_status_update_on_days = NULL;
  for ( int i = 0 ; i < 7 ; i++ ) {
    t_status_update_on_days = dict_find( iterator, MESSAGE_KEY_STATUS_UPDATE_ON_DAYS + i );
    if ( t_status_update_on_days ) {
      configParams.status_update_on_days[i] = ( ( t_status_update_on_days->value->uint8 == 't' ) || 
                                               ( t_status_update_on_days->value->uint8 == 'T' ) );
     }
  }
  // clock settings
  Tuple *t_clock_type_digital_analog = dict_find( iterator, MESSAGE_KEY_CLOCK_TYPE_DIGITAL_ANALOG );
  if ( t_clock_type_digital_analog ) {
    configParams.clock_type_digital_analog = stringToInt( (char*) t_clock_type_digital_analog->value->data );
  }  
  Tuple *t_digital_clock_type_12_24 = dict_find( iterator, MESSAGE_KEY_DIGITAL_CLOCK_TYPE_12_24 ); 
  if ( t_digital_clock_type_12_24 ) {
    configParams.digital_clock_type_12_24 = stringToInt( (char*) t_digital_clock_type_12_24->value->data );
  }
  // chime settings
  Tuple *t_chime_interval = dict_find( iterator, MESSAGE_KEY_CHIME_INTERVAL ); 
  if ( t_chime_interval ) {
    configParams.chime_interval = stringToInt( (char*) t_chime_interval->value->data );
  }
  Tuple *t_chime_start_time = dict_find( iterator, MESSAGE_KEY_CHIME_START_TIME );
  if ( t_chime_start_time ) {
    configParams.chime_start_time = stringToInt( (char*) t_chime_start_time->value->data );
  }
  Tuple *t_chime_end_time = dict_find( iterator, MESSAGE_KEY_CHIME_END_TIME );
  if ( t_chime_end_time ) {
    configParams.chime_end_time = stringToInt( (char*) t_chime_end_time->value->data );
  }
  Tuple *t_chime_on_days = NULL;
  for ( int i = 0 ; i < 7 ; i++ ) {
    t_chime_on_days = dict_find( iterator, MESSAGE_KEY_CHIME_ON_DAYS + i );
    if ( t_chime_on_days ) {
      configParams.chime_on_days[i] = ( ( t_chime_on_days->value->uint8 == 't' ) ||
                                       ( t_chime_on_days->value->uint8 == 'T' ) );
     }
  }
  Tuple *t_chime_offset = dict_find( iterator, MESSAGE_KEY_CHIME_OFFSET );
  if ( t_chime_offset ) {
    configParams.chime_offset = stringToInt( (char*) t_chime_offset->value->data );
  }

  get_config( configParams );
}


int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
             if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "config.c: stringToInt(): Unable to convert it into integer.");
             return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}
