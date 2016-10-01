#include <pebble.h>
#include "base.h"
#include "chime.h"
#include "clock.h"

// Buzz patterns
static uint32_t const one_segment[] = { 200, 200 };
VibePattern single_vibe_pattern = {
  .durations = one_segment,
  .num_segments = ARRAY_LENGTH( one_segment ),
};

static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH( two_segments ),
};

void do_chime( struct tm *time, struct CONFIG_PARAMS config_params ) {
  
  if ( time->tm_sec ) return;

  int mins_from_zero = time->tm_hour * 60 + time->tm_min + config_params.chime_offset;

  // Stop if buzzing is off
  if ( config_params.chime_interval == 0 ) return;
  
  // Stop if not on for the day
  if ( ! config_params.chime_on_days[time->tm_wday] ) return;
  
  // Stop if not within time range.
  // Add one minute to buzz_end to buzz on the last hour.
  if ( !is_X_in_range( config_params.chime_start_time * 60, config_params.chime_end_time * 60 + 1, mins_from_zero ) ) return;
  
  // Stop if not at offset
  if ( mins_from_zero % ( ( config_params.chime_interval == 1 ) ? 30 : 60 ) ) return;
  
  // is this half hour or full hour?
  if ( time->tm_min + config_params.chime_offset == 0 ) {
    vibes_enqueue_custom_pattern( double_vibe_pattern );
    if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: do_chime() hour" );
  } else {
    vibes_enqueue_custom_pattern( single_vibe_pattern );
    if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: do_chime() half-hour" );
  }
}
