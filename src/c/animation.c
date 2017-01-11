//
// Copyright (c) 2016 Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"
#include "animation.h"

extern tm tm_time;

extern void implementation_teardown( Animation *animation );
static uint32_t start_time_secs = 0;
static uint32_t end_time_secs = 0;

static void uint32_setter( void *subject, uint32_t uint32 ) {
  time_t now = (time_t) uint32;
  tm_time = *localtime( &now );
  layer_mark_dirty( (Layer *) subject );
}

static uint32_t uint32_getter( void *subject ) {
  return (uint32_t) time( NULL );
}
  
static const PropertyAnimationImplementation uint32_animation_implementation = {
  .base = {
    .update = (AnimationUpdateImplementation) property_animation_update_uint32,
    .teardown = implementation_teardown,
  },
    .accessors = {
      .setter = { .uint32 = (const UInt32Setter) uint32_setter },
      .getter = { .uint32 = (const UInt32Getter) uint32_getter },
  },
};

void start_animation( int delay_ms, int duration_ms, AnimationCurve anim_curve, void *subject ) {
  static PropertyAnimation *uint32_prop_animation = 0;
  static Animation *uint32_animation = 0;
  
  end_time_secs = (uint32_t) time( NULL );
  start_time_secs = end_time_secs - ( 60 * 45 );
  
  uint32_prop_animation = property_animation_create( &uint32_animation_implementation, NULL, NULL, NULL );
  property_animation_subject( uint32_prop_animation, (void *) &subject, true );
  property_animation_from( uint32_prop_animation, (void *) &( start_time_secs ), sizeof( start_time_secs ), true );
  property_animation_to( uint32_prop_animation, (void *) &( end_time_secs ), sizeof( end_time_secs ), true );
  uint32_animation = property_animation_get_animation( uint32_prop_animation );
  
  animation_set_curve( uint32_animation, anim_curve );
  animation_set_delay( uint32_animation, delay_ms );
  animation_set_duration( uint32_animation, duration_ms );
  
  animation_schedule( uint32_animation );
}