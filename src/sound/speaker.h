#pragma once

#include <pc.h>
#include <stdint.h>
#include <unistd.h>

#define SPEAKER_PORT 0x61

void speaker_beep(uint32_t frequency, uint32_t duration);
