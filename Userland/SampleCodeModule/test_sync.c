// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"
#include "user_syscalls.h"
#include "functions.h"
#include "syscall.h"

#define SEM_ID "test_sync_sem"
#define TOTAL_PAIR_PROCESSES 4
#define NULL (void *) 0


int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc)
{
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  print(".");
  aux += inc;
  *p = aux;
}

void my_process_inc(unsigned int argc, char *argv[])
{
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
    if ((sharedSem = sys_semOpen(SEM_ID, 1)) == -1)
    {
      print("test_sync: ERROR opening semaphore\n");
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++)
  {
    if (use_sem)
    sys_semWait(sharedSem);
    slowInc(&global, inc);
    if (use_sem)
    sys_semPost(sharedSem);
  }

  if (use_sem)
    sys_semPost(sharedSem);
  return;
}

// Los parametros a recibir son arg[1] = # de inc, arg[2] = 0 o 1 si se usa semaforo o no
void test_sync(unsigned int argc, char *argv[])
{ //{n, use_sem, 0}
  int pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3)
  {
    print("# de args equivocados.");
    return;
  }
  if ((satoi(argv[1])) <= 0)
  {
    print("arg0 no es > 0");
    return;
  }

  char *argvDec[] = {"dec_process", argv[1], "-1", argv[2]};
  char *argvInc[] = {"inc_process", argv[1], "1", argv[2]};

  global = 0;

  int i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    pids[i] = sys_create_process(&my_process_inc, 4, argvDec, 0, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process(&my_process_inc, 4, argvInc, 0, NULL);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    sys_waitpid(pids[i]);
    sys_waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("Final value");
  printInt(global);

  return;
}