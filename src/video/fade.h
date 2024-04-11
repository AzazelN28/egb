#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "palette.h"
#include "../core/timer.h"
#include "../math/range.h"

typedef enum fade {
  FADE_IN,
  FADE_OUT
} fade_t;

extern fade_t fade_direction;
extern bool fade_in_progress;
extern uint32_t fade_progress;
extern uint32_t fade_duration_in_tics;
extern timer_t fade_timer;

void fade_in(uint32_t tics);
void fade_out(uint32_t tics);
void fade_color(uint32_t tics, uint8_t r, uint8_t g, uint8_t b);
void fade_update();
