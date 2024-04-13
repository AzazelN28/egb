#include "log.h"

static uint8_t log_y = 1;

void log_header(const char *fmt, ...)
{
  log_y = 1;

  char buffer[100] = {0};

  textbackground(BLUE);
  textcolor(YELLOW);
  memset(buffer, 32, 80);
  gotoxy(1, 1);
  cputs(buffer);

  va_list args;
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  const uint8_t x = (40 - (strlen(buffer) >> 1));
  gotoxy(x, 1);
  cputs(buffer);
}

void log_line(const char *fmt, ...)
{
  char buffer[100] = {0};

  va_list args;
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  textbackground(BLACK);
  textcolor(LIGHTGRAY);
  gotoxy(1, ++log_y);
  cputs(buffer);
}

void log_answer(const char *fmt, ...)
{
  char buffer[100] = {0};

  va_list args;
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  textbackground(BLACK);
  textcolor(LIGHTGRAY);
  const uint8_t y = wherey();
  const uint8_t x = (80 - strlen(fmt));
  gotoxy(x, y);
  cputs(buffer);
}
