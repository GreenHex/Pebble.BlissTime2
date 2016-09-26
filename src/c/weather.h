#pragma once
#include <pebble.h>

void handle_message( Tuple *tupple_ptr );
void clear_weather( void );
void weather_init( void );
void weather_deinit( void );
void request_weather( void );
void request_stock( void );