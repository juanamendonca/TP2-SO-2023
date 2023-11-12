/* sampleCodeModule.c */
#include "functions.h"
#include "shell.h"
#include "user_syscalls.h"
#include <stdint.h>

char buffer[BUFFER_SIZE];

void shell(unsigned int argc, char *argv[]) {
  sys_write("Welcome!", PURPLE);
  enter();
  // help();
  char *args[MAX_ARGS];

  while (1) {
    sys_write(">", BLUE);
    entry(buffer, args);
  }
}

int main() {
  char *argv[] = {"shell"};
  int fd[] = {0, 0};
  int shellPid = sys_create_process(&shell, 1, argv, 1, fd);
  sys_waitpid(shellPid);
  print("back in userland");
  return 0;
}
