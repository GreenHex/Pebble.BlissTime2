#pragma once
#include <pebble.h>

void handle_message( Tuple *tupple_ptr );
void clear_status( void );
void status_init( void );
void status_deinit( void );
void request_weather( void );
void request_stock( void );