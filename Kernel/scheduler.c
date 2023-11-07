#include "scheduler.h"
#include "interrupts.h"
#include "priorityQueue.h"
#include "strings.h"
#include "video.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZE (1024 * 4)
#define QUANTUM 1

int pid = 0;
PriorityQueue *queue = NULL;
pcb *dummyPcb = NULL;
pcb *currentPcb = NULL;

void dummy(int argc, char **argv);
void initalizeScheduler();
int getNewPid();
int initalizePcb(pcb *newProcess, int argc, char **argv, int foreground,
                 int *fd, void *stack);
static void processStart(int argc, char *argv[], void *function(int, char **));
static void finishProcess();
int changeState(int pid, state state);
int killProcess(int pid);
void initalizeStackFrame(void (*fn)(int, char **), int argc, char **argv,
                         void *rbp);
int initalizeProcess(void (*process)(int argc, char **argv), int argc,
                     char **argv, int foreground, int *fd);
int block(int pid);
int unblock(int pid);
void freePcb(pcb *process);

void dummy(int argc, char **argv) {
  putArrayNext("en dummy", WHITE);
  while (1) {
    _hlt();
  }
}

void hola(int argc, char **argv) {
  int i = 0;
  while (i < 200) {
    putDecNext(i, WHITE);
    putLine();
    i++;
  }
}

void chau(int argc, char **argv) { putArrayNext("chau", WHITE); }

void process1(int argc, char **argv) {
  putLine();
  putArrayNext("proceso 1", WHITE);
  putLine();
}

void process2(int argc, char **argv) {
  putLine();
  putArrayNext("proceso 2", WHITE);
  putLine();
}

void initalizeScheduler() {
  queue = createPriorityQueue();
  if (queue == NULL) {
    return;
  }

  char *argv[] = {"dummy"};
  int fd[] = {0, 0};
  initalizeProcess((void *)&dummy, 1, argv, 1, fd);
  dummyPcb = dequeueP(queue);

  char *argv2[] = {"hola"};
  initalizeProcess((void *)&hola, 1, argv2, 1, fd);

  char *argv3[] = {"chau"};
  initalizeProcess((void *)&chau, 1, argv3, 1, fd);

  char *argv4[] = {"process1"};
  initalizeProcess((void *)&process1, 1, argv4, 1, fd);

  char *argv5[] = {"process2"};
  initalizeProcess((void *)&process2, 1, argv5, 1, fd);
}

void *scheduler(void *rsp) {
  if (queue == NULL) {
    putArrayNext("queue is null", WHITE);
    return rsp;
  }
  if (currentPcb == NULL) {
    if (isEmptyPReady(queue)) {
      currentPcb = dummyPcb;
    } else {
      currentPcb = dequeuePReady(queue);
    }
  } else {
    if (currentPcb->quantum > 0 && currentPcb->state == READY) {
      currentPcb->quantum--;
      return rsp;
    }
    if (currentPcb->priority < 4) {
      currentPcb->priority++;
    }
    currentPcb->rsp = rsp;
    currentPcb->quantum = currentPcb->priority * QUANTUM;
    enqueueP(queue, currentPcb, currentPcb->priority);
    if (isEmptyPReady(queue)) {
      currentPcb = dummyPcb;
    } else {
      currentPcb = dequeuePReady(queue);
    }
  }
  return currentPcb->rsp;
}

int getNewPid() {
  int toReturn = pid;
  pid++;
  return toReturn;
}

int initializePcb(pcb *newProcess, int argc, char **argv, int foreground,
                  int *fd, void *stack) {
  newProcess->pid = getNewPid();
  newProcess->ppid = 0;
  newProcess->foreground = foreground;
  newProcess->state = READY;
  newProcess->priority = 1;
  newProcess->quantum = 1 * QUANTUM;
  strcpy(newProcess->name, argv[0]);
  newProcess->fd[0] = fd[0];
  newProcess->fd[1] = fd[1];
  newProcess->rbp = (void *)((char *)stack + STACK_SIZE - 1);
  newProcess->rsp = (void *)(newProcess->rbp - sizeof(stackFrame));
  newProcess->argc = argc;
  char **arguments = malloc(sizeof(char *) * argc);
  if (arguments == NULL) {
    return -1;
  }
  for (int i = 0; i < argc; i += 1) {
    arguments[i] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
    if (arguments[i] == NULL) {
      i--;
      // Libero los que guarde hasta ahora
      while (i >= 0) {
        free(arguments[i]);
        i -= 1;
      }
      free(arguments);
      return -1;
    }
    strcpy(arguments[i], argv[i]);
  }
  newProcess->argv = arguments;
  return 0;
}

static void processStart(int argc, char *argv[], void *process(int, char **)) {
  process(argc, argv);
  finishProcess();
}

static void finishProcess() { killProcess(currentPcb->pid); }

int killProcess(int pid) {
  if (changeState(pid, KILLED) < 0) {
    return -1;
  }
  if (pid == currentPcb->pid) {
    callTimer();
  }
  return pid;
}

int changeState(int pid, state state) {
  if (pid == currentPcb->pid) {
    currentPcb->state = state;
    return pid;
  } else {
    pcb *processPcb = getProcessP(queue, pid);
    if (processPcb == NULL) {
      return -1;
    }
    processPcb->state = state;
    return pid;
  }
}

void initalizeStackFrame(void (*fn)(int, char **), int argc, char **argv,
                         void *rbp) {
  stackFrame *sf = (stackFrame *)rbp - 1;
  sf->r15 = 0x001;
  sf->r14 = 0x002;
  sf->r13 = 0x003;
  sf->r12 = 0x004;
  sf->r11 = 0x005;
  sf->r10 = 0x006;
  sf->r9 = 0x007;
  sf->r8 = 0x008;
  sf->rsi = (uint64_t)argv;
  sf->rdi = (uint64_t)argc;
  sf->rbp = 0x00B;
  sf->rdx = (uint64_t)fn;
  sf->rcx = 0x00C;
  sf->rbx = 0x00D;
  sf->rax = 0x00E;
  sf->rip = (uint64_t)processStart;
  sf->cs = 0x008;
  sf->flags = 0x202;
  sf->rsp = (uint64_t)(&sf->base);
  sf->ss = 0x000;
  sf->base = 0x000;
}

int initalizeProcess(void (*process)(int argc, char **argv), int argc,
                     char **argv, int foreground, int *fd) {
  if (process == NULL) {
    return -1;
  }

  pcb *newProcess = malloc(sizeof(pcb));

  if (newProcess == NULL) {
    return -1;
  }

  void *stack = malloc(sizeof(char) * STACK_SIZE);

  if (stack == NULL) {
    free(newProcess);
    return -1;
  }

  if (initializePcb(newProcess, argc, argv, foreground, fd, stack) == -1) {
    return -1;
  }

  initalizeStackFrame(process, argc, newProcess->argv, newProcess->rbp);

  enqueueP(queue, newProcess, newProcess->priority);

  return 0;
}

int block(int pid) {
  int toReturn = changeState(pid, BLOCKED);
  if (currentPcb != NULL && currentPcb->pid == pid) {
    callTimer();
  }
  return toReturn;
}

int unblock(int pid) { return changeState(pid, READY); }

void freePcb(pcb *process) {
  if (process == NULL) {
    return;
  }

  // Libera la memoria asignada para los argumentos del proceso
  if (process->argv != NULL) {
    for (int i = 0; i < process->argc; i++) {
      free(process->argv[i]);
    }
    free(process->argv);
  }
  free((void *)((char *)process->rbp - STACK_SIZE + 1));
  free(process);
}
