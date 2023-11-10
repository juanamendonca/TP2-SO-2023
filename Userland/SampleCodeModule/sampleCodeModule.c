/* sampleCodeModule.c */
#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "pong.h"
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
  sys_write("ALLOC", GREEN);
  print(": to allocate 0x1000 memory ");
  enter();
  sys_write("SEM", GREEN);
  print(": To print current sempahore information");
  enter();
  sys_write("test_synch", GREEN);
  print(": to test sempahores");
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
  } else if (strcmp(buffer, "test_synch") == 0) {
    // por ahora nada
  } else if (strcmp(buffer, "SEM") == 0) {
    // 
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
  sys_create_process(&shell, 1, argv, 1, fd);
  return 0;
}
