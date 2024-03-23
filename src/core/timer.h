#pragma once

// @see http://qzx.com/pc-gpe/pit.txt

#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TICS_PER_SEC 100

#define TIMER_CHANNEL0 0x40
#define TIMER_CHANNEL1 0x41
#define TIMER_CHANNEL2 0x42
#define TIMER_CONTROL 0x43

#define TIMER_MODE 0x34

#define TIMER_INTERRUPT 0x08
#define TIMER_FREQUENCY 0x1234DD / TICS_PER_SEC

extern volatile int32_t tics;

void timer_start();
void timer_stop();
