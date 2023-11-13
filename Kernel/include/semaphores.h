#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "lib.h"
#include "memoryManager.h"
#include "scheduler.h"


#define NAME_LIMIT 15
#define SEM_LIMIT 30

typedef struct pNode {
  struct pNode *next;
  uint64_t pid;
  volatile int blocked;
} process;

typedef struct {
  char name[NAME_LIMIT];
  process *firstProcess; // First process Waiting
  process *lastProcess;  // Last process Waiting
  uint64_t lock;
  int value;
  uint64_t size;     // Process quantity that use the semaphore
  uint64_t sizeList; // Blocked process quantity
} semaphore;

void start_semaphores();

uint64_t sem_open(char *name, uint64_t initValue);
uint64_t sem_close(char *name);
uint64_t sem_wait(uint64_t semIndex);
uint64_t sem_post(uint64_t semIndex);
void sem();
void printSem(semaphore sem);
char *getSemName(uint64_t semIndex);
void printProcessesSem(uint64_t semIndex);
extern uint64_t _xchg(uint64_t *lock, int value); // Defined at libasm.c
void acquire(uint64_t *lock);
void release(uint64_t *lock);

#endif // SEMAPHORES_H