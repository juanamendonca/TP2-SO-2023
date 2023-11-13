// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_sync.h"
#include "functions.h"
#include "syscall.h"
#include "test_util.h"
#include "user_syscalls.h"

#define SEM_ID "test_sync_sem"
#define TOTAL_PAIR_PROCESSES 3

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
  return;
}

void my_process_inc(unsigned int argc, char *argv[]) {
  print("Iniciando my_process_inc\n");

  int n;
  int inc;
  int use_sem;
  int semIndex = -1;

  if (argc != 4) {
    print("my_process_inc: Número incorrecto de argumentos\n");
    goto cleanup;
  }

  n = satoi(argv[1]);
  print(argv[1]);
  printInt(n);
  enter();
  inc = satoi(argv[2]);
  print(argv[2]);
  printInt(inc);
  enter();
  use_sem = satoi(argv[3]);
  print(argv[3]);
  printInt(use_sem);
  enter();

  if (n <= 0 || inc == 0 || use_sem < 0) {
    print("my_process_inc: Argumentos inválidos\n");
    goto cleanup;
  }

  if (use_sem) {
    print("Intentando abrir semaforo\n");
    semIndex = sys_semOpen(SEM_ID, 1);
    if (semIndex == -1) {
      print("test_sync: ERROR opening semaphore\n");
      goto cleanup;
    }
  }

  for (int i = 0; i < n; i++) {
    if (use_sem) {
      print("Esperando semaforo\n");
      sys_semWait(semIndex);
      print("semaforo adquirido\n");
    }

    slowInc(&global, inc);

    if (use_sem) {
      print("Liberando semaforo\n");
      sys_semPost(semIndex);
      print("semaforo liberado\n");
    }
  }

cleanup:
  if (use_sem && semIndex != -1) {
    print("Cerrando semaforo\n");
    sys_semClose(SEM_ID);
  }
  print("Terminando my_process_inc\n");
  return;
}

void test_sync(unsigned int argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3)
    return;

  char *argvDec[] = {"dec_process", argv[1], "1", argv[2]};
  char *argvInc[] = {"inc_process", argv[1], "1", argv[2]};

  global = 0;

  int vec[2] = {0, 0};
  int i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process(&my_process_inc, 4, argvInc, 0, vec);
    if (pids[i] < 0) {
      print("cound not initialize process");
      return;
    }
    pids[i + TOTAL_PAIR_PROCESSES] =
        sys_create_process(&my_process_inc, 4, argvInc, 0, vec);
    if (pids[i] < 0) {
      print("cound not initialize process");
      return;
    }
  }

  // print("Sale del primer for");
  char buffer[400];

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_waitpid(pids[i]);
    // print("Yes enter here");
    // printInt(pids[i]);
    // sys_get_info_processes(buffer);
    // print(buffer);
    //   enter();
    // sys_kill_process(pids[i+ TOTAL_PAIR_PROCESSES]);
    sys_waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
    // sys_get_info_processes(buffer);
    // print(buffer);
    //  print("Salió acá");
  }
  // print("2");
  //
  enter();
  sys_get_info_processes(buffer);
  print(buffer);
  print("Final Value Global: ");
  enter();
  printInt(global);
  enter();
  return;
}