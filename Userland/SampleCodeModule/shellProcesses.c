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

void helpP(unsigned int argc, char *argv[]) { help(); }

void getTimeP(unsigned int argc, char *argv[]) { getTime(WHITE); }

void clearP(unsigned int argc, char *argv[]) { sys_clear_screen(); }

// void pongP(unsigned int argc, char *argv[]) { pong(); }

void getRegInfoP(unsigned int argc, char *argv[]) { getRegInfo(); }

void regsTesterP(unsigned int argc, char *argv[]) { regsTester(); }

void divisionTesterP(unsigned int argc, char *argv[]) { divisionTester(); }

void invalidOpTesterP(unsigned int argc, char *argv[]) { invalidOpTester(); }

void infoProcessesP(unsigned int argc, char *argv[]) {
  char buffer[400];
  sys_get_info_processes(buffer);
  print(buffer);
}

void testProcessesP(unsigned int argc, char *argv[]) {
  char *argv2[] = {argv[1]};
  test_processes(1, argv2);
}

void testPrioP(unsigned int argc, char *argv[]) { test_prio(); }

void memoryP(unsigned int argc, char *argv[]) { sys_printBitmap(); }

void memoryTestP(unsigned int argc, char *argv[]) {
  char *argv2[] = {
      "310000"}; // faltaria pedirle este valor a traves de una syscall
  test_mm(1, argv2);
}

void semP(unsigned int argc, char *argv[]) { sys_sem(); }

void testSyncP(unsigned int argc, char *argv[]) { test_sync(argc, argv); }

void loopP(unsigned int argc, char *argv[]) {}

void killP(unsigned int argc, char *argv[]) {}

void niceP(unsigned int argc, char *argv[]) {}

void blockP(unsigned int argc, char *argv[]) {}

void unblockP(unsigned int argc, char *argv[]) {}

void catP(unsigned int argc, char *argv[]) {}

void wcP(unsigned int argc, char *argv[]) {}

void filterP(unsigned int argc, char *argv[]) {}

void phyloP(unsigned int argc, char *argv[]) {}
