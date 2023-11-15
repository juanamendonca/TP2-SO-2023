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
    {&testProcessesP, "TEST-PROCESSES $1", "to test the scheduler, $1: qty of procceses to create"},
    {&testPrioP, "TEST-PRIO", "to test the scheduler priorities"},
    {&memoryP, "MEMORY", "to print the current usage of memory"},
    {&memoryTestP, "TEST-MEMORY", "to test the memory manager"},
    {&semP, "SEM", "to get all sempahores info"},
    {&testSyncP, "TEST-SYNC $1 $2", "to test semaphores, $1: qty of processes to create, $2: 1 with sem | 0 without"},
    {&loopP, "LOOP", "prints process periodically"},
    {&killP, "KILL $1", "kills the process with given pid, $1 pid to kill"},
    {&niceP, "NICE $1 $2", "changes the process given the pid and new priority, $1: pid, $2: priority 1-4"},
    {&blockP, "BLOCK", "blocks the process"},
    {&unblockP, "UNBLOCK", "unblocks the process"},
    {&catP, "CAT", "prints stdin"},
    {&wcP, "WC", "counts the lines o the input"},
    {&filterP, "FILTER", "filters vowels of the input"},
    {&phyloP, "PHYLO", "shows the philosopher problem with only 5 philosophers"}};

void entry(char *buffer, char **args);
int parseArgs(char *argString, char **args);
int findPipe(char **args, int argc);
int getCommand(char *name);
void runCommand(char **args, int argc, int fd[], int com, int cont);
int runCommandPipes(int c1, int c2, char **argv, int argc, int pipe);
void help();

void help() {
  print("The available commands are:\n");
  print("Press Left Shift send EOF\nPress Right Shift to kill all foreground processes");
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
    if (runCommandPipes(c1, c2, args, argc, pipe) == -1) {
      print("Error in creating pipe");
    }

  } else {
    int fore = 1;

    if (strcmp(args[argc - 1], "B") == 0) {
      argc--;
      fore = 0;
    }
    runCommand(args, argc, NULL, c1, fore);
  }
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
    }
    if ((argString[i] == ' ' || argString[i + 1] == '\0') && argStart != -1) {
      // Termina el argumento actual
      if (argString[i] == ' ') {
        argString[i] = '\0';
      }
      args[count] = &argString[argStart];
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

void runCommand(char **args, int argc, int fd[], int com, int fore) {
  int pid = sys_create_process(commandsInfo[com].process, argc, args, fore, fd);
  if (fore) {
    sys_waitpid(pid);
  }
}

int runCommandPipes(int c1, int c2, char **argv, int argc, int pipe) {
  int pipeId = sys_pipe_open("/");
  int fd1[] = {0, pipeId};
  int fd2[] = {pipeId, 0};
  int pid1 = sys_create_process(commandsInfo[c1].process, pipe, argv, 1, fd1);
  // runCommand(args, pipe, fd1, c1, 1);
  if (pid1 == -1) {
    sys_pipe_close(pipeId);
    return -1;
  }
  int pid2 = sys_create_process(commandsInfo[c2].process, argc - pipe - 1,
                                &argv[pipe + 1], 1, fd2);
  // runCommand(&args[pipe + 1], argc - pipe - 1, fd2, c2, fore);
  if (pid2 == -1) {
    sys_kill_process(pid1);
    sys_pipe_close(pipeId);
    return -1;
  }

  sys_waitpid(pid1);
  sys_waitpid(pid2);
  sys_pipe_close(pipeId);
  return 0;
}
