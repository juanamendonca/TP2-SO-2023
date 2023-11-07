#include "scheduler.h"
#include "interrupts.h"
#include "queue.h"
#include "strings.h"
#include "video.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZE (1024 * 4)

int pid = 0;
Queue *queue = NULL;
pcb *dummyPcb = NULL;
pcb *currentPcb = NULL;

void dummy(int argc, char **argv);
void initalizeScheduler();
int getNewPid();
int initalizePcb(pcb *newProcess, int argc, char **argv, int foreground,
                 int *fd, void *stack);
static void processStart(int argc, char *argv[], void *function(int, char **));
void initalizeStackFrame(void (*fn)(int, char **), int argc, char **argv,
                         void *rbp);
int initalizeProcess(void (*process)(int argc, char **argv), int argc,
                     char **argv, int foreground, int *fd);

void dummy(int argc, char **argv) {
  putArrayNext("en dummy", WHITE);
  while (1) {
    _hlt();
  }
}

// void hola(int argc, char **argv) {
//   int i = 0;
//   while (1) {
//     putDecNext(i, WHITE);
//     putLine();
//     i++;
//   }
// }

// void chau(int argc, char **argv) {
//   putArrayNext("chau", WHITE);
//   while (1) {
//     _hlt();
//   }
// }

void initalizeScheduler() {
  queue = createQueue();
  if (queue == NULL) {
    return;
  }

  char *argv[] = {"dummy"};
  int fd[] = {0, 0};
  initalizeProcess((void *)&dummy, 1, argv, 1, fd);
  dummyPcb = dequeue(queue);

  //   char *argv2[] = {"hola"};
  //   initalizeProcess((void *)&hola, 1, argv2, 1, fd);

  //   char *argv3[] = {"chau"};
  //   initalizeProcess((void *)&chau, 1, argv3, 1, fd);
}

void *scheduler(void *rsp) {
  if (queue == NULL) {
    putArrayNext("queue is null", WHITE);
    return rsp;
  }
  if (currentPcb == NULL) {
    if (isEmpty(queue)) {
      currentPcb = dummyPcb;
    } else {
      currentPcb = dequeue(queue);
      enqueue(queue, currentPcb);
    }
  } else {
    currentPcb->rsp = rsp;
    if (isEmpty(queue)) {
      currentPcb = dummyPcb;
    } else {
      currentPcb = dequeue(queue);
      enqueue(queue, currentPcb);
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

  enqueue(queue, newProcess);

  return 0;
}
