#pragma once
#include <pebble.h>

bool is_X_in_range( int a, int b, int x );
void configure_buzz( int, int, int, int, int, int, int, int, int, int, int, int, int, int, int );
void configure_clock( int );
void clock_init( void );
void clock_deinit( void );
