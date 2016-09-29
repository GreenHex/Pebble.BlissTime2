#include <pebble.h>
#include "app_messaging.h"
#include "base.h"
#include "weather.h"
#include "config.h"

static void inbox_received_callback( DictionaryIterator *iterator, void *context ) {
  Tuple *tupple_ptr = 0;

  APP_LOG( APP_LOG_LEVEL_ERROR, "app_messaging.c: inbox_received_callback(): Received a message from phone." );
  
  if ( ( tupple_ptr = dict_find( iterator, MESSAGE_KEY_TEMPERATURE ) ) || 
       ( tupple_ptr = dict_find( iterator, MESSAGE_KEY_CMP ) ) ) {
    handle_message( tupple_ptr );
  } else { // probably configuraton data, we don't check
    handle_config_message( iterator );
  }
}

static void inbox_dropped_callback( AppMessageResult reason, void *context ) {
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_ERROR, "Message dropped." );
}

static void outbox_failed_callback( DictionaryIterator *iterator, AppMessageResult reason, void *context ) {
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_ERROR, "Outbox send failed." );
}

static void outbox_sent_callback( DictionaryIterator *iterator, void *context ) {
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "Outbox send success." );
}

void callback_init( void ) {
// Register callbacks
  app_message_register_inbox_received( inbox_received_callback );
  app_message_register_inbox_dropped( inbox_dropped_callback );
  app_message_register_outbox_failed( outbox_failed_callback );
  app_message_register_outbox_sent( outbox_sent_callback );
  
  // app_message_open( app_message_inbox_size_maximum(), app_message_outbox_size_maximum() ); // this don't work
  app_message_open( 256, 64 ); // this worky
}

void callback_deinit( void ){
  app_message_deregister_callbacks();
}

void send_request( enum CMD_TYPE requestType ) {
  DictionaryIterator *out_iter;
  AppMessageResult result = app_message_outbox_begin( &out_iter );
  if( result == APP_MSG_OK ) {
    dict_write_int( out_iter, MESSAGE_KEY_REQUEST, &requestType, sizeof( enum CMD_TYPE ), true );
    dict_write_end( out_iter );
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      if (DEBUG) APP_LOG( APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int) result );
    }
  } else {
    if (DEBUG) APP_LOG( APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int) result );
  }
}
