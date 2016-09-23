#pragma once

void setTempUnits( int units );
void handle_weather_message( DictionaryIterator *iterator );
void weather_init( void );
void weather_deinit( void );
void request_weather( void );
void handle_stocks_message( DictionaryIterator *iterator );
void request_stock( void );