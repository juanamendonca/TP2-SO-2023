#include "shell.h"
#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "pong.h"
#include "shellProcesses.h"
#include "test_mm.h"
#include "test_sync.h"
#include "test_util.h"
#include "time.h"
#include "user_syscalls.h"
#include <stdint.h>

static command commandsInfo[] = {
    {&helpP, "HELP", "to get the information on the available commands"},
    {&getTimeP, "TIME", "to get the current time"},
    {&clearP, "CLEAR", ": to clear the terminal"},
    //{&pongP, "PONG", "to play game"},
    {&getRegInfoP, "REGISTERS", "to print register status"},
    {&regsTesterP, "REGISTER-TESTER",
     "to check the correct loading of registers "},
    {&divisionTesterP, "ZERO", ": to trigger divide by zero exception"},
    {&invalidOpTesterP, "INVALIDOP", "to trigger invalid operation exception "},
    {&infoProcessesP, "PS", ": to get all the processes info"},
    {&testProcessesP, "TEST-PROCESSES", "to test the scheduler"},
    {&testPrioP, "TEST-PRIO", "to test the scheduler priorities"},
    {&loopP, "LOOP", "prints process pid every 2 seconds"},
    {&killP, "KILL", "kills the process with given pid"},
    {&niceP, "NICE", "changes the process given the pid and new priority"},
    {&blockP, "BLOCK", "blocks the process"},
    {&unblockP, "UNBLOCK", "unblocks the process"},
    {&catP, "CAT", "prints stdin"},
    {&wcP, "WC", "counts the lines o the input"},
    {&filterP, "FILTER", "filters vowels of the input"},
    {&phyloP, "PHYLO", "shows the philosopher problem"}};

void entry(char *buffer, char **args);
int parseArgs(char *argString, char **args);
int findPipe(char **args, int argc);
int getCommand(char *name);
void runCommand(char **args, int argc, int fd[], int com);
void help();

void help() {
  print("The available commands are:");
  enter();
  int numCommands = sizeof(commandsInfo) / sizeof(commandsInfo[0]);
  for (int i = 0; i < numCommands; i++) {
    sys_write(commandsInfo[i].name, GREEN);
    print(": ");
    print(commandsInfo[i].description);
    enter();
  }
}

void entry(char *buffer, char **args) {
  scanf(buffer, BUFFER_SIZE);
  deleteExtraSpaces(buffer);
  int argc = parseArgs(buffer, args);
  int pipe = findPipe(args, argc);
  if (pipe == -2) {
    print("Invalid command. Too many pipes");
    enter();
    return;
  }
  if (pipe == 0) {
    print("Pipe does not have inicial function");
    enter();
    return;
  }
  sys_write_dec(pipe, WHITE);
  sys_write_dec(argc, WHITE);
  if (pipe == (argc - 1)) {
    print("Pipe does not have destination function");
    enter();
    return;
  }
  int c1 = getCommand(args[0]);
  if (c1 == -1) {
    print("Invalid command");
    enter();
    return;
  }
  if (pipe > 0) {
    int c2 = getCommand(args[pipe + 1]);
    if (c2 == -1) {
      print("Invalid second command");
      enter();
      return;
    }
  }
  int fd[] = {0, 0};
  runCommand(args, argc, fd, c1);
  enter();
}

int getCommand(char *name) {
  int numCommands = sizeof(commandsInfo) / sizeof(commandsInfo[0]);
  for (int i = 0; i < numCommands; i++) {
    if (strcmp(commandsInfo[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

int parseArgs(char *argString, char **args) {
  int count = 0;
  int argStart = -1;

  for (int i = 0; argString[i] != '\0'; i++) {
    if (argString[i] != ' ' && argStart == -1) {
      // Comienza un nuevo argumento
      argStart = i;
    } else if ((argString[i] == ' ' || argString[i + 1] == '\0') &&
               argStart != -1) {
      // Termina el argumento actual
      if (argString[i] == ' ') {
        argString[i] = '\0';
      }
      args[count] = (char *)&argString[argStart];
      count++;
      argStart = -1;
    }
  }

  return count;
}

// -1 if there are no pipes, -2 if there are more than 1, if not the position of
// the pipe
int findPipe(char **args, int argc) {
  int pipe = -1;
  int cant = 0;
  for (int i = 0; i < argc; i++) {
    if (strcmp(args[i], "/") == 0) {
      pipe = i;
      cant++;
    }
  }
  if (cant > 1) {
    return -2;
  }
  return pipe;
}

void runCommand(char **args, int argc, int fd[], int com) {
  int pid = sys_create_process(commandsInfo[com].process, argc, args, 1, fd);
  sys_waitpid(pid);
}
