#include <pebble.h>
#include "global.h"
#include "base.h"
#include "calendar.h"
#include "clock.h"
#include "status_disp.h"
#include "battery.h"
#include "config.h"
#include "chime.h"
#include "app_messaging.h"

extern Window *window;
extern Layer *window_layer;

static void window_load( Window *window ) {
  base_init();
  calendar_init();
  clock_init();
  status_init();
  battery_init();
  callback_init();
  
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_DEBUG, "Heap: %d bytes used, %d bytes free", (int) heap_bytes_used(), (int) heap_bytes_free() );
}

static void window_unload( Window *window ) {
  battery_deinit();
  status_deinit();
  clock_deinit();
  calendar_deinit();
  base_deinit();
  callback_deinit();
}

static void init( void ) {
  window = window_create();
  window_layer = window_get_root_layer( window );

  window_set_window_handlers( window, ( WindowHandlers ) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push( window, true );
}

static void destroy( void ) {
  window_destroy(window);
}

int main( void ) {
  init();
  app_event_loop();
  destroy();
  
  return 0;
}
