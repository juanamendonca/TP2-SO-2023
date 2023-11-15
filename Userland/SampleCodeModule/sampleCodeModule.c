/* sampleCodeModule.c */
#include "functions.h"
#include "shell.h"
#include "test_mm.h"
#include "user_syscalls.h"
#include <stdint.h>

char buffer[BUFFER_SIZE];

void shell() {
  sys_write("Welcome!", PURPLE);
  enter();
  help();
  char *args[MAX_ARGS];

  while (1) {
    sys_write(">", BLUE);
    entry(buffer, args);
  }
}

int main() {
  shell();
  print("back in userland");
  return 0;
}
