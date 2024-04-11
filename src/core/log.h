#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <conio.h>
#include <string.h>

void log_header(const char *fmt, ...);
void log_line(const char *fmt, ...);
void log_answer(const char *fmt, ...);
