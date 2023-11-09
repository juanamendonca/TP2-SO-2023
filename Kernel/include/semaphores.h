#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "stdio.h"
#include "defs.h"
#include "scheduler.h"
#include "lib.h"
#include "memoryManager.h"

#define NAME_LIMIT 15
#define SEM_LIMIT 30

typedef struct pNode
{
    struct pNode *next;
    uint64_t pid;
} process;

typedef struct
{
    char name[NAME_LIMIT];
    process *firstProcess; // First process Waiting
    process *lastProcess;  // Las process Waiting
    uint64_t lock;
    int value;
    uint64_t size;     // Process quantity that use the semaphore
    uint64_t sizeList; // Blocked process quantity
} semaphore;

extern uint64_t _xchg(uint64_t *lock, int value);  // Defined at libasm.c

void start_semaphores();

// Open a semaphore.
uint64_t sem_open(char *name, uint64_t initValue);

// Close a sempahore
uint64_t sem_close(char *name);

// Locks a semaphore. If successful (the lock was acquired), sem_wait() and sem_trywait() will return 0.  Otherwise, -1 is returned and errno is set, and the state of the semaphore is unchanged.
uint64_t sem_wait(uint64_t semIndex);

// the value of the semaphore is incremented, and all threads which are waiting on the semaphore are awakened. If successful, sem_post() will return 0.  Otherwise, -1 is returned.
uint64_t sem_post(uint64_t semIndex);

void sem();

char *getSemName(uint64_t semIndex);

void printProcessesSem(uint64_t semIndex);

void printSem(semaphore sem);



#endif