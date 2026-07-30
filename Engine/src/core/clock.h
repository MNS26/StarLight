#pragma once
#include "defines.h"

typedef struct clock {
  f64 start_time;
  f64 elapsed;
} clock;

// Update provided clock. Needs to be called before checking elapsed time

// Has no effect on stopped clocks
void clock_update(clock* clock);

// Starts provided clock, reset elapsed time
void clock_start(clock* clock);

// Stops provided clock
void clock_stop(clock* clock);