#include "functions.h"
#include "syscall.h"
#include "test_util.h"
#include "user_syscalls.h"
#include <stddef.h>
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

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = sys_create_process(&printWrapper, 1, argv, -1, NULL);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_block_process(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblock_process(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_kill_process(pids[i]);
  return;
}
