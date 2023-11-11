/* sampleCodeModule.c */
#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "pong.h"
#include "test_util.h"
#include "time.h"
#include "user_syscalls.h"
#include <stdint.h>
#include "test_sync.h"

#define BUFFER_SIZE 100

char buffer[BUFFER_SIZE];

void help() {

  print("The available commands are:");
  enter();
  sys_write("HELP", GREEN);
  print(": to get the information on the available commands");
  enter();
  sys_write("TIME", GREEN);
  print(": to get the current time");
  enter();
  sys_write("CLEAR", GREEN);
  print(": to clear the terminal");
  enter();
  sys_write("PONG", GREEN);
  print(": to play game");
  enter();
  sys_write("REGISTERS TEST", GREEN);
  print(": to check the correct loading of registers ");
  enter();
  sys_write("REGISTERS", GREEN);
  print(": to print register status");
  enter();
  sys_write("DIVIDE BY ZERO", GREEN);
  print(": to trigger divide by zero exception");
  enter();
  sys_write("INVALID OPERATION", GREEN);
  print(": to trigger invalid operation exception ");
  enter();
  sys_write("ALLOC", GREEN);
  print(": to allocate 0x1000 memory ");
  enter();
  sys_write("INFO PROCESSES", GREEN);
  print(": to get all the processes info");
  enter();
  sys_write("TEST PROCESSES", GREEN);
  print(": to test the scheduler");
  enter();
  sys_write("INFO SEMAPHORES", GREEN);
  print(": to get all the sempahores actives info");
  enter();
  sys_write("TEST SEMPAHORES", GREEN);
  print(": to test the sempahores");
  enter();
}

void command(char *entry) {
  deleteExtraSpaces(buffer);
  if (strcmp(buffer, "TIME") == 0) {
    getTime(WHITE);
    enter();
  } else if (strcmp(buffer, "CLEAR") == 0) {
    sys_clear_screen();
  } else if (strcmp(buffer, "HELP") == 0) {
    help();
  } else if (strcmp(buffer, "PONG") == 0) {
    pong();
  } else if (strcmp(buffer, "REGISTERS") == 0) {
    getRegInfo();
  } else if (strcmp(buffer, "REGISTERS TEST") == 0) {
    regsTester();
  } else if (strcmp(buffer, "DIVIDE BY ZERO") == 0) {
    divisionTester();
  } else if (strcmp(buffer, "INVALID OPERATION") == 0) {
    invalidOpTester();
  } else if (strcmp(buffer, "ALLOC") == 0) {
    // por ahora nada
  } else if (strcmp(buffer, "INFO PROCESSES") == 0) {
    char buffer[400];
    sys_get_info_processes(buffer);
    print(buffer);
  } else if (strcmp(buffer, "TEST PROCESSES") == 0) {
    // por ahora falla por que como el free no hace nada en algun momento el
    // malloc ya me da null;
    char *argv2[] = {"5"};
    test_processes(1, argv2);
  } else if (strcmp(buffer, "INFO SEMAPHORES") == 0) {

  
  } else if (strcmp(buffer, "TEST SEMAPHORES") == 0) {
    test_sync();
  }
   else {
    print("Invalid command");
    enter();
    help();
  }
}

void shell(unsigned int argc, char *argv[]) {
  sys_write("Welcome!", PURPLE);
  enter();
  help();
  while (1) {
    sys_write(">", BLUE);
    scanf(buffer, BUFFER_SIZE);
    command(buffer);
  }
}

int main() {

  // MALLOC testing
  // char *memory;
  // char *memory2;
  // sys_alloc(&memory, 10);
  // *memory = 'b';
  // *(memory + 1) = 'o';
  // *(memory + 2) = 'c';
  // *(memory + 3) = '\n';
  // *(memory + 4) = 'a';
  // *(memory + 5) = 'a';
  // *(memory + 6) = '\0';
  // sys_write(memory, PURPLE);
  // enter();

  // sys_alloc(&memory2, 10);
  // *memory2 = 'a';
  // *(memory2 + 1) = 's';
  // *(memory2 + 2) = 'd';
  // *(memory2 + 3) = 'f';
  // *(memory2 + 4) = '\0';
  // sys_write(memory2, PURPLE);
  // enter();
  char *argv[] = {"shell"};
  int fd[] = {0, 0};
  int shellPid = sys_create_process(&shell, 1, argv, 1, fd);
  sys_waitpid(shellPid);
  return 0;
}
