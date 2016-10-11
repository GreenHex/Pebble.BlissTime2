#include <pebble.h>
#include "global.h"
#include "battery.h"

static Layer *window_layer = 0;
static TextLayer *batt_layer = 0;

static void handle_battery( BatteryChargeState charge_state ) {
  static char battery_text[] = "xxxx";

  if (charge_state.is_charging) {
    text_layer_set_text_color( batt_layer, GColorWhite );
    text_layer_set_background_color( batt_layer, GColorBlack );
  } else {
    text_layer_set_text_color( batt_layer, GColorBlack );
    text_layer_set_background_color( batt_layer, GColorWhite );
  }
  snprintf( battery_text, sizeof( battery_text ), "%d%%", charge_state.charge_percent );
  text_layer_set_text( batt_layer, battery_text );
}

void battery_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  batt_layer = text_layer_create( GRect( 111, 1, 32, 15 ) );
  text_layer_set_font( batt_layer, fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DROIDSANS_12 ) ) );
  text_layer_set_background_color( batt_layer, GColorClear );
  text_layer_set_text_alignment( batt_layer, GTextAlignmentRight );
  layer_add_child( window_layer, text_layer_get_layer( batt_layer ) );

  handle_battery( battery_state_service_peek() );
  battery_state_service_subscribe( handle_battery );
}

void battery_deinit() {
  battery_state_service_unsubscribe();
  text_layer_destroy( batt_layer );
}