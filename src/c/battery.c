#include <pebble.h>
#include "global.h"
#include "battery.h"

#define COLOUR_BATT_5           PBL_IF_COLOR_ELSE( GColorRed, GColorBlack )
#define COLOUR_BATT_10          PBL_IF_COLOR_ELSE( GColorOrange, GColorBlack )
#define COLOUR_BATT_20          PBL_IF_COLOR_ELSE( GColorChromeYellow, GColorBlack )
#define COLOUR_BATT_40          PBL_IF_COLOR_ELSE( GColorLimerick, GColorBlack )
#define COLOUR_BATT_60          PBL_IF_COLOR_ELSE( GColorBrass, GColorBlack )
#define COLOUR_BATT_80          PBL_IF_COLOR_ELSE( GColorKellyGreen, GColorBlack )
#define COLOUR_BATT_100         PBL_IF_COLOR_ELSE( GColorIslamicGreen, GColorBlack )

static Layer *window_layer = 0;
static TextLayer *batt_layer = 0;
static BitmapLayer *charging_icon_bitmap_layer = 0;
static GBitmap *charging_icon_bitmap = 0;

static void handle_battery( BatteryChargeState charge_state ) {
  static char battery_text[] = "xxxx";

  if ( charge_state.is_charging ) {
    layer_set_hidden( bitmap_layer_get_layer( charging_icon_bitmap_layer ), false );
  } else {
    layer_set_hidden( bitmap_layer_get_layer( charging_icon_bitmap_layer ), true );
  }
  #if defined( PBL_COLOR )
    GColor batt_txt_colour = GColorClear;
    if ( charge_state.charge_percent < 6 ) {
      batt_txt_colour = COLOUR_BATT_5;
    } else if ( charge_state.charge_percent < 11 ) {
      batt_txt_colour = COLOUR_BATT_10;
    } else if ( charge_state.charge_percent < 21 ) {
      batt_txt_colour = COLOUR_BATT_20;
    } else if ( charge_state.charge_percent < 41 ) {
      batt_txt_colour = COLOUR_BATT_40;
    } else if ( charge_state.charge_percent < 61 ) {
      batt_txt_colour = COLOUR_BATT_60;
    } else if ( charge_state.charge_percent < 81 ) {
      batt_txt_colour = COLOUR_BATT_80;
    } else {
      batt_txt_colour = COLOUR_BATT_100;
    }
    text_layer_set_text_color( batt_layer, batt_txt_colour );   
  #else
    text_layer_set_text_color( batt_layer, GColorBlack );
  #endif
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
  
  charging_icon_bitmap  = gbitmap_create_with_resource( RESOURCE_ID_BATTERY_CHARGING_ICON );
  charging_icon_bitmap_layer = bitmap_layer_create( GRect( 0, 0, 8, 15 ) );
  bitmap_layer_set_compositing_mode( charging_icon_bitmap_layer, GCompOpSet );
  bitmap_layer_set_background_color( charging_icon_bitmap_layer, GColorClear );
  bitmap_layer_set_bitmap( charging_icon_bitmap_layer, charging_icon_bitmap );
  layer_add_child( text_layer_get_layer( batt_layer ), bitmap_layer_get_layer( charging_icon_bitmap_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( charging_icon_bitmap_layer ), true );
  

  handle_battery( battery_state_service_peek() );
  battery_state_service_subscribe( handle_battery );
}

void battery_deinit() {
  battery_state_service_unsubscribe();
  text_layer_destroy( batt_layer );
}