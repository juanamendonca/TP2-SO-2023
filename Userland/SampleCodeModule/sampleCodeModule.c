/* sampleCodeModule.c */
#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "pong.h"
#include "test_sync.h"
#include "test_util.h"
#include "time.h"
#include "user_syscalls.h"
#include <stdint.h>

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
  sys_write("INFO PROCESSES", GREEN);
  print(": to get all the processes info");
  enter();
  sys_write("TEST PROCESSES", GREEN);
  print(": to test the scheduler");
  enter();
  sys_write("TEST PRIO", GREEN);
  print(": to test the scheduler priorities");
  enter();
  sys_write("TEST MM", GREEN);
  print(": to test the memory manager");
  enter();
  sys_write("MEMORY STATE", GREEN);
  print(": to print the current usage of memory");
  enter();
  sys_write("TESTSYNCH", GREEN);
  print(": to test semaphores and synchronization");
  enter();
  sys_write("INFO SEM", GREEN);
  print(": to get all sempahores info");
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
    enter();

  } else if (strcmp(buffer, "TESTSYNCH") == 0) {
    char *argv[] = {"test_sync", "10", "1"}; // Por ejemplo, 10 iteraciones
    test_sync(3, argv);
    enter();
    print("3");
    char buffer[400];
    sys_get_info_processes(buffer);
    print(buffer);

  } else if (strcmp(buffer, "INFO PROCESSES") == 0) {
    char buffer[400];
    sys_get_info_processes(buffer);
    print(buffer);
    enter();

  } else if (strcmp(buffer, "TEST PROCESSES") == 0) {
    // por ahora falla por que como el free no hace nada en algun momento el
    // malloc ya me da null;
    char *argv2[] = {"5"};
    test_processes(1, argv2);
    enter();

  } else if (strcmp(buffer, "TEST PRIO") == 0) {
    test_prio();
    enter();

  } else if (strcmp(buffer, "MEMORY STATE") == 0) {
    sys_printBitmap();
    enter();

  } else if (strcmp(buffer, "TEST MM") == 0) {
    char *argv[] = {
        "310000"}; // faltaria pedirle este valor a traves de una syscall
    test_mm(1, argv);
    enter();

  } else if (strcmp(buffer, "INFO SEM") == 0) {
    sys_sem();
    enter();

  } else if (strcmp(buffer, "RETURN") == 0) {
    return;
  } else {
    print("Invalid command, write HELP for more available commands");
    enter();
  }
}

void shell(unsigned int argc, char *argv[]) {
  sys_write("Welcome!", PURPLE);
  enter();
  help();

  while (1) {
    sys_write(">", BLUE);
    scanf(buffer, BUFFER_SIZE);
    if (strcmp(buffer, "RETURN") == 0) {
      return;
    }
    command(buffer);
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
