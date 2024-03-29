#include "fade.h"

fade_t fade_direction = FADE_IN;
bool fade_in_progress = false;
uint16_t fade_progress = 0;
uint32_t fade_duration_in_tics = 0;
timer_t fade_timer = { .start = 0 };
uint8_t fade_from[PALETTE_BYTES];
uint8_t fade_to[PALETTE_BYTES];
uint8_t fade_palette[PALETTE_BYTES];

/**
 * Inicia el efecto de fade.
 */
bool fade_start(uint32_t duration_in_tics)
{
  if (fade_in_progress)
    return false;

  fade_in_progress = true;
  fade_duration_in_tics = duration_in_tics;
  timer_reset(&fade_timer);
  return true;
}

/**
 * Realiza un efecto de fade in.
 */
void fade_in(uint32_t duration_in_tics)
{
  if (fade_start(duration_in_tics) == false)
    return;

  fade_direction = FADE_IN;
  memcpy(fade_to, palette_current, PALETTE_BYTES);
  memset(fade_from, 0, PALETTE_BYTES);
  memset(fade_palette, 0, PALETTE_BYTES);
}

/**
 * Realiza un efecto de fade out.
 */
void fade_out(uint32_t duration_in_tics)
{
  if (fade_start(duration_in_tics) == false)
    return;

  fade_direction = FADE_OUT;
  memcpy(fade_from, palette_current, PALETTE_BYTES);
  memset(fade_to, 0, PALETTE_BYTES);
  memcpy(fade_palette, fade_from, PALETTE_BYTES);
}

/**
 * Realiza un efecto de fade de color.
 */
void fade_color(uint32_t duration_in_tics, uint8_t r, uint8_t g, uint8_t b)
{
  if (fade_start(duration_in_tics) == false)
    return;

  fade_direction = FADE_OUT;
  memcpy(fade_from, palette_current, PALETTE_BYTES);
  for (uint16_t i = 0; i < PALETTE_SIZE; i++) {
    uint16_t offset = i * 3;
    fade_to[offset + 0] = r;
    fade_to[offset + 1] = g;
    fade_to[offset + 2] = b;
  }
  memcpy(fade_palette, fade_from, PALETTE_BYTES);
}

int32_t linear(uint16_t progress, int32_t from, int32_t to)
{
  return (from + (to - from) * progress) / 0xFFFF;
}

int32_t clamp(int32_t value, int32_t min, int32_t max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

/**
 * Actualiza la paleta actual utilizada para realizar
 * el efecto de fade.
 */
void fade_update()
{
  if (!fade_in_progress)
    return;

  uint32_t elapsed = timer_elapsed(&fade_timer);
  if (elapsed >= fade_duration_in_tics) {
    elapsed = fade_duration_in_tics;
    fade_in_progress = false;
  }

  fade_progress = 0xFFFF * elapsed / fade_duration_in_tics;
  for (uint16_t i = 0; i < PALETTE_BYTES; i++) {
    fade_palette[i] = linear(fade_progress, fade_from[i], fade_to[i]);
  }
  palette_set_all(fade_palette);
}
