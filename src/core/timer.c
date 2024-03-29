#include "timer.h"

volatile uint32_t timer_tics = 0;

static _go32_dpmi_seginfo prev_handler, new_handler;

/**
 * Maneja el timer.
 */
void timer_handler()
{
  timer_tics++;
}

/**
 * Arranca el timer.
 */
void timer_start()
{
  _go32_dpmi_get_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                 &prev_handler);

  new_handler.pm_offset = (uint32_t)timer_handler;
  new_handler.pm_selector = _go32_my_cs();
  _go32_dpmi_chain_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                   &new_handler);

  outportb(TIMER_CONTROL, TIMER_MODE);
  outportl(TIMER_CHANNEL0, TIMER_FREQUENCY);
  outportl(TIMER_CHANNEL0, TIMER_FREQUENCY >> 8);
}

/**
 * Detiene el timer.
 */
void timer_stop()
{
  _go32_dpmi_set_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                 &prev_handler);

  outportb(TIMER_CONTROL, TIMER_MODE);
  outportl(TIMER_CHANNEL0, 0x00);
  outportl(TIMER_CHANNEL0, 0x00);
}

void timer_reset(timer_t *timer)
{
  timer->start = timer_tics;
}

timer_t* timer_new()
{
  timer_t* timer = (timer_t*)calloc(1, sizeof(timer_t));
  timer_reset(timer);
  return timer;
}

void timer_free(timer_t* timer)
{
  free(timer);
}

uint32_t timer_elapsed(timer_t* timer)
{
  return timer_tics - timer->start;
}

bool timer_timeout(timer_t* timer, uint32_t duration_in_tics)
{
  return timer_elapsed(timer) >= duration_in_tics;
}
