#pragma once
#include <pebble.h>

#define DEBUG 0

void setup_text_layer(TextLayer*,uint32_t);
void setup_bitmap_layer(BitmapLayer*);
void setup_cal_layer(TextLayer*);
void base_init(void);
void base_deinit(void);
