#include "timer.h"

volatile int32_t tics = 0;

static _go32_dpmi_seginfo prev_handler, new_handler;

void timer_handler() { tics++; }

void timer_start() {
  _go32_dpmi_get_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                 &prev_handler);

  new_handler.pm_offset = (int32_t)timer_handler;
  new_handler.pm_selector = _go32_my_cs();
  _go32_dpmi_chain_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                   &new_handler);

  outportb(TIMER_CONTROL, TIMER_MODE);
  outportl(TIMER_CHANNEL0, TIMER_FREQUENCY);
  outportl(TIMER_CHANNEL0, TIMER_FREQUENCY >> 8);
}

void timer_stop() {
  _go32_dpmi_set_protected_mode_interrupt_vector(TIMER_INTERRUPT,
                                                 &prev_handler);

  outportb(TIMER_CONTROL, TIMER_MODE);
  outportl(TIMER_CHANNEL0, 0x00);
  outportl(TIMER_CHANNEL0, 0x00);
}
