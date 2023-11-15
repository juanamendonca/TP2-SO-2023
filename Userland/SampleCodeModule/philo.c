#include "functions.h"
#include "user_syscalls.h"
#include <philo.h>
#include <stddef.h>

#define N 5
#define MAX_PHYL 5
#define SEM_MAIN "semMain"
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

enum state { THINKING, EATING, WAITING };

typedef struct {
  uint64_t pid;
  int state;
  uint64_t semId;
  char *semName;
} philo_t;

char *semNames[] = {"p0", "p1", "p2", "p3", "p4"};
static philo_t philos[MAX_PHYL];
uint64_t semMain;
int semPhil[N];

static void printState() {
  for (int i = 0; i < N; i++) {
    (philos[i].state == EATING) ? print(" E ") : print(" . ");
  }
  print("\n");
}

void test(int index) {
  int left = (index + N - 1) % N;
  int right = (index + 1) % N;

  if (philos[index].state == WAITING && philos[left].state != EATING &&
      philos[right].state != EATING) {
    philos[index].state = EATING;
    printState();
    sys_semPost(philos[index].semId);
  }
}

// take up chopsticks, first left then right
void takeChopstick(int index) {
  sys_semWait(semMain);
  philos[index].state = WAITING;
  test(index);
  sys_semPost(semMain);
  sys_semWait(philos[index].semId);
}

// put down chopsticks
void putChopstick(int index) {
  int left = (index + N - 1) % N;
  int right = (index + 1) % N;

  sys_semWait(semMain);
  philos[index].state = THINKING;
  test(left);
  test(right);
  sys_semPost(semMain);
}

void phyloProcess(unsigned int argc, char **argv) {
  int index = argc - 1;
  while (1) {
    sys_sleep(1);
    // search chopsticks, is hungry
    takeChopstick(index);

    sys_sleep(1);
    // Finish eating, left their chopsticks.
    putChopstick(index);
  }
}

int addPhilo(int index) {
  sys_semWait(semMain);
  philos[index].semName = semNames[index];

  if ((philos[index].semId = sys_semOpen(philos[index].semName, 1)) == -1) {
    print("ERROR opening sem in addPhil\n");
    return 1;
  }

  philos[index].state = THINKING;

  char *argv[] = {"philosopher", ""};
  unsigned int argc = index + 1;

  if ((philos[index].pid =
           sys_create_process(&phyloProcess, argc, argv, -1, NULL)) == 0) {
    print("ERROR creating process\n");
    return 1;
  }

  sys_semPost(semMain);
  return 0;
}

void philosophersApp() {
  print("Bienvenido a los Filosofos comensales\n");

  if ((semMain = sys_semOpen(SEM_MAIN, 1)) == -1) {
    print("ERROR opening main sem\n");
    return;
  }

  for (int i = 0; i < N; i++) {
    if (addPhilo(i) == 1) {
      print("ERROR adding philosophers\n");
      return;
    }
  }

  for (int i = 0; i < N; i++) {
    sys_waitpid(philos[i].pid);
  }
}
