#include "functions.h"
#include "syscall.h"
#include "test_util.h"
#include "user_syscalls.h"
#include <stddef.h>

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void test_processes(unsigned int argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {"endless_loop"};
  // char *buffer[400];

  if (argc != 1)
    return;

  if ((max_processes = satoi(argv[0])) <= 0)
    return;

  p_rq p_rqs[max_processes];

  while (1) {
    // sys_printBitmap();
    // enter();
    // sys_get_info_processes(buffer);
    // print(buffer);

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = sys_create_process(&endless_loop, 1, argvAux, -1, NULL);

      if (p_rqs[rq].pid == -1) {
        print("-");
        print("test_processes: ERROR creating process\n");
        return;
      } else {
        // print("creating ");
        printInt(p_rqs[rq].pid);
        enter();
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // enter();
    // sys_get_info_processes(buffer);
    // print(buffer);

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
            if (sys_kill_process(p_rqs[rq].pid) == -1) {
              print("test_processes: ERROR killing process\n");
              return;
            }
            // print("killing ");
            printInt(p_rqs[rq].pid);
            enter();
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (sys_block_process(p_rqs[rq].pid) == -1) {
              print("test_processes: ERROR blocking process\n");
              return;
            }
            // print("blocking ");
            printInt(p_rqs[rq].pid);
            enter();
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sys_unblock_process(p_rqs[rq].pid) == -1) {
            print("test_processes: ERROR unblocking process\n");
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
