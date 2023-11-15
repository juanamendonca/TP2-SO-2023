#include "test_sync.h"
#include "functions.h"
#include "syscall.h"
#include "test_util.h"
#include "user_syscalls.h"

#define SEM_ID "test_sync_sem"
#define TOTAL_PAIR_PROCESSES 4

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  print(".");
  sys_giveup_cpu(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(unsigned int argc, char *argv[]) {
  int64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4)
    return;

  if ((n = satoi(argv[1])) <= 0)
    return;
  if ((inc = satoi(argv[2])) == 0)
    return;
  if ((use_sem = satoi(argv[3])) < 0)
    return;

  int sharedSem;
  if (use_sem)
    if ((sharedSem = sys_semOpen(SEM_ID, 1)) == -1) {
      print("test_sync: ERROR opening semaphore\n");
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) {
      sys_semWait(sharedSem);
    }
    slowInc(&global, inc);

    if (use_sem) {
      sys_semPost(sharedSem);
    }
  }

  if (use_sem) {
    sys_semClose(sharedSem);
  }

  return;
}

// The parameters to be received are arg[1] = number of increments, arg[2] = 0
// or 1 if a semaphore is used or not

void test_sync(unsigned int argc, char *argv[]) { //{n, use_sem, 0}
  int pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3) {
    print("#  args wrong.");
    return;
  }
  if ((satoi(argv[1])) <= 0) {
    print("arg0 is not > 0");
    return;
  }

  char *argvDec[] = {"dec_process", argv[1], "-1", argv[2]};
  char *argvInc[] = {"inc_process", argv[1], "1", argv[2]};

  global = 0;

  int fd[] = {0, 0};

  int i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process(&my_process_inc, 4, argvDec, -1, fd);
    pids[i + TOTAL_PAIR_PROCESSES] =
        sys_create_process(&my_process_inc, 4, argvInc, -1, fd);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_waitpid(pids[i]);
    sys_waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("\nFinal value: ");
  printInt(global);

  return;
}