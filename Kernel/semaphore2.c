#include "memoryManager.h"
#include "scheduler.h"
#include "semaphores2.h"
#include "strings.h"
#include "video.h"

semaphore *sems = NULL;
semaphore *semsL = NULL;
int semCant = 0;

int semId = 0;

int totalLock = 0;

void acquire(int *lock);
void release(int *lock);
semaphore *findSem(char *name);
int semOpen(char *name, int value);
semaphore *create_sem(char *name, int value);
int deleteFromQueueS(int id);

void acquire(int *lock) {
  while (_xchg(lock, 1) != 0)
    ;
}

void release(int *lock) { _xchg(lock, 0); }

void start_semaphores() {
  acquire(&totalLock);
  release(&totalLock);
}

int sem_open(char *name, int value) {
  semaphore *sem = findSem(name);
  acquire(&totalLock);
  if (sem == NULL) {
    sem = create_sem(name, value);
    if (sem == NULL) {
      release(&totalLock);
      return -1;
    }
    semCant++;
  }
  sem->cantP++;
  release(&totalLock);
  return sem->id;
}

semaphore *findSem(char *name) {
  semaphore *current = sems;
  for (int i = 0; i < semCant; i++) {
    if (strcmp(current->name, name) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

semaphore *findSemId(int id) {
  semaphore *current = sems;
  for (int i = 0; i < semCant; i++) {
    if (current->id == id) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

semaphore *create_sem(char *name, int value) {
  semaphore *sem = malloc(sizeof(semaphore));
  if (sem == NULL) {
    return NULL;
  } else {
    sem->id = semId++;
    strcpy(sem->name, name);
    sem->firstP = NULL;
    sem->lastP = NULL;
    sem->lock = 0;
    sem->value = value;
    sem->sizeP = 0;
    sem->cantP = 0;
  }
  if (sems == NULL) {
    sems = sem;
    semsL = sem;
  } else {
    semsL->next = sem;
    semsL = sem;
  }
  return sem;
}

int sem_close(int id) {
  semaphore *sem = findSemId(id);
  if (sem == NULL) {
    return -1;
  }
  acquire(&totalLock);
  ;
  if (sem->cantP == 1) {
    free(sem->firstP);
    deleteFromQueueS(sem->id);
    semCant--;
  } else {
    sem->cantP--;
  }
  release(&totalLock);
  return 0;
}

int deleteFromQueueS(int id) {
  semaphore *current = sems;
  semaphore *prev = NULL;
  while (current != NULL) {
    if (current->id == id) {
      if (prev == NULL) {
        // El proceso está al principio de la lista
        sems = current->next;
        if (sems == NULL) {
          // Si la lista queda vacía, actualiza lastProcess también
          semsL = NULL;
        }
      } else {
        // El proceso está en alguna posición intermedia o al final
        prev->next = current->next;
        if (prev->next == NULL) {
          // Si el proceso estaba al final, actualiza lastProcess
          semsL = prev;
        }
      }

      // Libera la memoria del nodo eliminado
      free(current);
      return 0;
    }

    prev = current;
    current = current->next;
  }
  return -1;
}

int sem_wait(int id) {
  semaphore *sem = findSemId(id);
  if (sem == NULL) {
    return -1;
  }
  acquire(&(sem->lock));
  if (sem->value > 0) {
    sem->value--;
    release(&(sem->lock));
    return 0;
  } else {
    PNode *node = malloc(sizeof(PNode));
    node->pid = getPid();
    if (sem->firstP != NULL) {
      sem->lastP->next = node;
      sem->lastP = node;
    } else {
      sem->firstP = node;
      sem->lastP = node;
    }
    sem->sizeP++;
    release(&(sem->lock));
    block(node->pid);
  }
  return 0;
}

int sem_post(int id) {
  semaphore *sem = findSemId(id);
  if (sem == NULL) {
    return -1;
  }
  acquire(&(sem->lock));
  int unblocked = -1;
  while (unblocked == -1 && sem->firstP != NULL) {
    unblocked = unblock(sem->firstP->pid);
    PNode *node = sem->firstP;
    sem->firstP = node->next;
    free(node);
    sem->sizeP--;
    if (unblocked != -1) {
      release(&(sem->lock));
      return 0;
    }
  }
  sem->value++;
  release(&(sem->lock));
  return 0;
}

void sem() {
  semaphore *sem = sems;
  while (sem != NULL) {
    putArrayNext(sem->name, WHITE);
    putArrayNext("\t", WHITE);
    putDecNext(sem->value, WHITE);
    putArrayNext("\t", WHITE);
    sem = sem->next;
  }
}
