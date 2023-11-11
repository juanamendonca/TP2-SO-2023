#include "functions.h"
#include "syscall.h"
#include "test_util.h"
#include <stdint.h>

#define MINOR_WAIT                                                             \
  1000000 // TODO: Change this value to prevent a process from flooding the
          // screen
#define WAIT                                                                   \
  100000000 // TODO: Change this value to make the wait long enough to see
            // theese
            // processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 4  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 1 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void printWrapper() { endless_loop_print(MINOR_WAIT); }

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless_loop_print"};
  uint64_t i;
  int fd[] = {0, 0};

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process(&printWrapper, 1, argv, 0, fd);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}
