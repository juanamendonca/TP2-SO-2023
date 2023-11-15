#include "exceptionTester.h"
#include "functions.h"
#include "getInforegs.h"
#include "philo.h"
#include "pong.h"
#include "shell.h"
#include "test_mm.h"
#include "test_sync.h"
#include "test_util.h"
#include "time.h"
#include "user_syscalls.h"
#include <stdint.h>

#define ESC 27

void helpP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  help();
}

void getTimeP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  getTime(WHITE);
}

void clearP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  sys_clear_screen();
}

// void pongP(unsigned int argc, char *argv[]) { pong(); }

void getRegInfoP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  getRegInfo();
}

void regsTesterP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  regsTester();
}

void divisionTesterP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  divisionTester();
}

void invalidOpTesterP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  invalidOpTester();
}

void infoProcessesP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  char buffer[400];
  sys_get_info_processes(buffer);
  print(buffer);
}

void testProcessesP(unsigned int argc, char *argv[]) {
  if (argc != 2) {
    print("Wrong arguments");
    return;
  }
  char *argv2[] = {argv[1]};
  test_processes(1, argv2);
}

void testPrioP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  test_prio();
}

void memoryP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  sys_printBitmap();
}

void memoryTestP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  char *argv2[] = {
      "310000"}; // faltaria pedirle este valor a traves de una syscall
  test_mm(1, argv2);
}

void semP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  sys_sem();
}

void testSyncP(unsigned int argc, char *argv[]) {
  if (argc != 3) {
    print("Wrong arguments");
    return;
  }
  test_sync(argc, argv);
}

void loopP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  int pid = sys_get_pid();
  while (1) {
    sys_sleep(5);
    printInt(pid);
  }
}

void killP(unsigned int argc, char *argv[]) {
  if (argc != 2) {
    print("Wrong arguments");
    return;
  }
  sys_kill_process(atoi2(argv[1]));
}

void niceP(unsigned int argc, char *argv[]) {
  if (argc != 3) {
    print("Wrong arguments");
    return;
  }
  int prio = atoi2(argv[2]);
  if (prio < 1 || prio > 4) {
    print("Wrong priority, between 1 and 4.");
    return;
  }
  if (sys_nice(atoi2(argv[1]), prio) == -1) {
    print("No process with that pid");
  }
}

void blockP(unsigned int argc, char *argv[]) {
  if (argc != 2) {
    print("Wrong arguments");
    return;
  }
  if (sys_block_process(atoi2(argv[1])) == -1) {
    print("No process with that pid");
  }
}

void unblockP(unsigned int argc, char *argv[]) {
  if (argc != 2) {
    print("Wrong arguments");
    return;
  }
  if (sys_unblock_process(atoi2(argv[1])) == -1) {
    print("No process with that pid");
  }
}

void catP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  // sys_get_clean_buffer();

  char c[2];
  c[1] = '\0'; // Corrected array index and assigned '\0' to the second element
  while ((c[0] = getChar()) != '\0') {
    print(c);
  }
  print(c);
  print("terminanado cat");
}

void wcP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  int count = 0;
  char c[2];
  c[1] = '\0';
  while ((c[0] = getChar()) != '\0') {
    if (c[0] == '\n') {
      count++;
    }
  }
  printInt(count);
  if (count == 1) {
    print(" line");
  } else {
    print(" lines");
  }
  return;
}

void filterP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }
  char c[2];
  c[1] = '\0';
  sys_read(c);
  while (c[0] != '\0') {
    if (c[0] == 'a' || c[0] == 'e' || c[0] == 'i' || c[0] == 'o' ||
        c[0] == 'u' || c[0] == 'A' || c[0] == 'E' || c[0] == 'I' ||
        c[0] == 'O' || c[0] == 'U') {
    } else {
      print(c);
    }
    sys_read(c);
  }
}

void phyloP(unsigned int argc, char *argv[]) {
  if (argc != 1) {
    print("Wrong arguments");
    return;
  }

  philosophersApp();
}
