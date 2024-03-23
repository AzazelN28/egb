#pragma once

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../video/palette.h"
#include "../video/video.h"
#include "../sound/speaker.h"
#include "../sound/sb.h"
#include "./timer.h"
#include <stdbool.h>
#include <stdio.h>

extern bool game_is_running;

void game_init();
void game_loop();
void game_quit();
