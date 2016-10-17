#pragma once

#define COLOUR_BATT_5           GColorRed
#define COLOUR_BATT_10          GColorOrange
#define COLOUR_BATT_20          GColorChromeYellow
#define COLOUR_BATT_40          GColorLimerick
#define COLOUR_BATT_60          GColorBrass
#define COLOUR_BATT_80          GColorKellyGreen
#define COLOUR_BATT_100         GColorIslamicGreen

void battery_init( Window *window );
void battery_deinit( void );