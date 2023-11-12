/* sampleCodeModule.c */
#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "pong.h"
#include "shell.h"
#include "test_mm.h"
#include "test_sync.h"
#include "test_util.h"
#include "time.h"
#include "user_syscalls.h"
#include <stdint.h>

#define BUFFER_SIZE 100
#define MAX_ARGS 5

char buffer[BUFFER_SIZE];

void shell(unsigned int argc, char *argv[]) {
  sys_write("Welcome!", PURPLE);
  enter();
  help();
  char *args[MAX_ARGS];

  while (1) {
    sys_write(">", BLUE);
    scanf(buffer, BUFFER_SIZE);
    deleteExtraSpaces(buffer);
    parseArgs(buffer, args);
    runCommand(args[0]);
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
