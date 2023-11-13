#include <lib.h>
#include <time.h>


static unsigned long ticks = 0;

void timer_handler() { ticks++; }

int ticks_elapsed() { return ticks; }

int seconds_elapsed() { return ticks / 18; }

void sleepms(int mseconds) {
  int startTime = seconds_elapsed();
  while (mseconds > seconds_elapsed() - startTime) {
    _hlt();
  }
};
