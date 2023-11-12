#include "scheduler.h"
#include "interrupts.h"
#include "priorityQueue.h"
#include "strings.h"
#include "video.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE (1024 * 4)
#define QUANTUM 1
#define BUFFER_SIZE_PROCESS 100

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
void killCurrent();
int changeState(int pid, state state);
int killProcess(int pid);
void initalizeStackFrame(void (*fn)(int, char **), int argc, char **argv,
                         void *rbp);
int initalizeProcess(void (*process)(int argc, char **argv), int argc,
                     char **argv, int foreground, int *fd);
int block(int pid);
int unblock(int pid);
void freePcb(pcb *process);
int processInfo(pcb *pcb, char *buffer);
int processesInfo(char *buffer);
int getPid();
int nice(int pid, int priority);
void giveUpCPU();
void waitpid(int pid);

void dummy(int argc, char **argv) {
  while (1) {
    // putArrayNext("dummy ", WHITE);
    _hlt();
  }
}

void hola(int argc, char **argv) {
  int i = 0;
  while (1) {
    putArrayNext(" ", WHITE);
    i++;
  }
}

void chau(int argc, char **argv) { putArrayNext("chau", WHITE); }

void process1(int argc, char **argv) {
  putLine();
  putArrayNext("proceso 1", WHITE);
  putLine();
  while (1) {
    putArrayNext(".", WHITE);
  }
}

void process2(int argc, char **argv) {
  putLine();
  putArrayNext("proceso 2", WHITE);
  putLine();
  char *buffer = malloc(500);
  processesInfo(buffer);
  putArrayNext("info ya se hizo", WHITE);
  if (*buffer == '\0') {
    putArrayNext("buffer esta vacio", WHITE);
  }
  putArrayNext(buffer, WHITE);
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

  // char *argv2[] = {"hola"};
  // initalizeProcess((void *)&hola, 1, argv2, 1, fd);

  // char *argv3[] = {"chau"};
  // initalizeProcess((void *)&chau, 1, argv3, 1, fd);

  // char *argv4[] = {"process1"};
  // initalizeProcess((void *)&process1, 1, argv4, 1, fd);

  // char *argv5[] = {"process2"};
  // initalizeProcess((void *)&process2, 1, argv5, 1, fd);
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
  } else if (currentPcb->pid == dummyPcb->pid) {
    if (isEmptyPReady(queue)) {
      dummyPcb->rsp = rsp;
      return rsp;
    } else {
      currentPcb = dequeuePReady(queue);
    }
  } else {
    if (currentPcb->quantum > 0 && currentPcb->state == READY) {
      currentPcb->quantum--;
      return rsp;
    }
    if (currentPcb->priority < 4 && currentPcb->state == READY) {
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
  if (currentPcb == NULL) {
    newProcess->ppid = 0;
  } else {
    newProcess->ppid = currentPcb->pid;
  }
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
  newProcess->waitingPid = 0;
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
  killCurrent();
}

void killCurrent() { killProcess(currentPcb->pid); }

int killProcess(int pid) {
  pcb *process = NULL;
  if (pid == currentPcb->pid) {
    process = currentPcb;
  } else {
    process = getProcessP(queue, pid);
  }
  if (process == NULL) {
    return -1;
  }
  if (process->waitingPid > 0) {
    unblock(process->ppid);
  }
  changeState(pid, KILLED);
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
    putArrayNext(" process is null ", WHITE);
    return -1;
  }

  pcb *newProcess = malloc(sizeof(pcb));

  if (newProcess == NULL) {
    putArrayNext(" pcb is null ", WHITE);
    return -1;
  }

  void *stack = malloc(sizeof(char) * STACK_SIZE);

  if (stack == NULL) {
    free(newProcess);
    putArrayNext(" stack is null ", WHITE);
    return -1;
  }

  if (initializePcb(newProcess, argc, argv, foreground, fd, stack) == -1) {
    putArrayNext(" initialize pcb is null ", WHITE);
    return -1;
  }

  initalizeStackFrame(process, argc, newProcess->argv, newProcess->rbp);

  enqueueP(queue, newProcess, newProcess->priority);

  return newProcess->pid;
}

int block(int pid) {
  if (currentPcb != NULL && currentPcb->pid == pid) {
    if (currentPcb->priority > 1 &&
        (currentPcb->priority * QUANTUM - currentPcb->quantum) <
            ((currentPcb->priority - 1) * QUANTUM)) {
      currentPcb->priority--;
    }
    currentPcb->quantum = QUANTUM * currentPcb->priority;
  }
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

int processInfo(pcb *pcb, char *buffer) {
  if (pcb == NULL || buffer == NULL) {
    return -1;
  }
  intToString(pcb->pid, buffer);
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  int digits = uintToBase((uint64_t)pcb->rsp, buffer, 16);
  fillHexa(16 - digits, buffer);
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  digits = uintToBase((uint64_t)pcb->rbp, buffer, 16);
  fillHexa(16 - digits, buffer);
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  strcpy(buffer, pcb->name);
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  if (pcb->foreground == 1) {
    strcpy(buffer, "foreground");
  } else {
    strcpy(buffer, "background");
  }
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  if (pcb->state == READY) {
    strcpy(buffer, "ready");
  } else if (pcb->state == BLOCKED) {
    strcpy(buffer, "blocked");
  } else {
    strcpy(buffer, "killed");
  }
  buffer += strlen(buffer);
  *(buffer) = '\t';
  buffer++;
  intToString(pcb->priority, buffer);

  return 0;
}

int processesInfo(char *buffer) {
  *buffer = '\0';
  startIteratorP(queue);
  while (hasNextP(queue)) {
    processInfo(nextP(queue), buffer);
    buffer += strlen(buffer);
    *(buffer) = '\n';
    buffer++;
  }
  processInfo(currentPcb, buffer);
  buffer += strlen(buffer);
  *(buffer) = '\n';
  buffer++;
  *(buffer) = '\0';
  return 0;
}

int getPid() {
  if (currentPcb == NULL) {
    return -1;
  }
  return currentPcb->pid;
}

int nice(int pid, int priority) {
  if (priority < 1 || priority > 4) {
    return -1;
  }
  pcb *pcb = getAndDeleteProcessP(queue, pid);
  if (pcb == NULL) {
    return -1;
  }
  pcb->priority = priority;
  pcb->quantum = QUANTUM * pcb->quantum;
  enqueueP(queue, pcb, pcb->priority);
  return 0;
}

void giveUpCPU() {
  if (currentPcb == NULL) {
    return;
  }
  currentPcb->quantum = QUANTUM * currentPcb->quantum;
  enqueueP(queue, currentPcb, currentPcb->priority);
  callTimer();
}

void waitpid(int pid) {
  pcb *process = getProcessP(queue, pid);
  if (process == NULL) {
    return;
  }
  if (process->state == KILLED) {
    return;
  }
  process->waitingPid = 1;
  block(process->ppid);
}