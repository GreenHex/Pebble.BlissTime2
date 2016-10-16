#include <pebble.h>
#include "global.h"
#include "calendar.h"
#include "clock.h"
#include "status_disp.h"
#include "battery.h"
#include "config.h"
#include "chime.h"
#include "app_messaging.h"

static Window *window;

static void window_load( Window *window ) { // order is important as layers are drawn over each other.
  calendar_init( window );
  status_init( window );
  battery_init( window );
  clock_init( window );
  messaging_init();
}

static void window_unload( Window *window ) {
  messaging_deinit();
  clock_deinit();
  battery_deinit();
  status_deinit();
  calendar_deinit();
}

static void init( void ) {
  window = window_create();
  window_set_window_handlers( window, ( WindowHandlers ) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push( window, false );
}

static void destroy( void ) {
  window_destroy( window );
}

int main( void ) {
  init();
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_DEBUG, "After init(): Heap: %d bytes used, %d bytes free", (int) heap_bytes_used(), (int) heap_bytes_free() );  
  app_event_loop();
  destroy();
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_DEBUG, "After window_destroy() Heap: %d bytes used, %d bytes free", (int) heap_bytes_used(), (int) heap_bytes_free() );

  return 0;
}
