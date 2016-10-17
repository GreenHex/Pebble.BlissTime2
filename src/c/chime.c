#include <pebble.h>
#include "global.h"
#include "chime.h"

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

void do_chime( struct tm *tick_time ) {
  if ( tick_time->tm_sec ) return;

  int mins_from_zero = tick_time->tm_hour * 60 + tick_time->tm_min + ( persist_read_int( MESSAGE_KEY_CHIME_OFFSET ) );

  // Stop if chime is off
  if ( persist_read_int( MESSAGE_KEY_CHIME_INTERVAL ) == 0 ) return;

  // Stop if not ON for the day
  if ( ! persist_read_bool( MESSAGE_KEY_CHIME_ON_DAYS + tick_time->tm_wday ) ) return;

  // Stop if not within time range.
  // Add one minute to buzz_end to buzz on the last hour.
  if ( !is_X_in_range( ( persist_read_int( MESSAGE_KEY_CHIME_START_TIME ) ) * 60,
                      ( persist_read_int( MESSAGE_KEY_CHIME_END_TIME ) ) * 60 + 1, mins_from_zero ) ) return;

  // Stop if not at offset
  if ( mins_from_zero % ( ( persist_read_int( MESSAGE_KEY_CHIME_INTERVAL ) == EVERY_HALF_HOUR ) ? 30 : 60 ) ) return;

  // is this half hour or full hour?
  if ( tick_time->tm_min + persist_read_int( MESSAGE_KEY_CHIME_OFFSET ) == 0 ) {
    vibes_enqueue_custom_pattern( double_vibe_pattern );
    if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: do_chime() hour" );
  } else {
    vibes_enqueue_custom_pattern( single_vibe_pattern );
    if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: do_chime() half-hour" );
  }
}
