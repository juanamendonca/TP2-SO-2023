#include <functions.h>
#include <stdint.h>
#include <time.h>
#include <user_syscalls.h>


int getFormat(int n) {
  int dec = n % 240;
  dec = dec >> 4;
  int units = n & 15;
  return dec * 10 + units;
}

void addCero(char *buffer) {
  if (buffer[1] == '\0') {
    buffer[2] = '\0';
    buffer[1] = buffer[0];
    buffer[0] = '0';
  }
}

void getTime(int color) {
  sys_write("TIME: ", color);
  int hours, minutes, seconds;
  sys_get_hours(&hours);
  sys_get_minutes(&minutes);
  sys_get_seconds(&seconds);
  char str_hours[3];
  char str_minutes[3];
  char str_seconds[3];

  intToString(getFormat(hours) - 3, str_hours);
  addCero(str_hours);
  sys_write(str_hours, color);
  sys_write(":", color);

  intToString(getFormat(minutes), str_minutes);
  addCero(str_minutes);
  sys_write(str_minutes, color);
  sys_write(":", color);

  intToString(getFormat(seconds), str_seconds);
  addCero(str_seconds);
  sys_write(str_seconds, color);
}
