#include <pebble.h>
#include "app_messaging.h"
#include "base.h"
#include "weather.h"
#include "config.h"

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  // if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Message received. %d", (int) dict_size(iterator));
  
  if (dict_find(iterator, MESSAGE_KEY_TEMPERATURE)) {
    if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Weather data." );
    handle_weather_message(iterator);
  } else {
    if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Configuration data." );
    handle_config_message(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped.");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed.");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success.");
}

void callback_init() {
// Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  // app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum()); // this don't work
  app_message_open(256, 64); // this worky
}

void callback_deinit(){
  app_message_deregister_callbacks();
}