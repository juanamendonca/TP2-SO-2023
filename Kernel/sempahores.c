#include "lib.h" // Asumiendo que esta contiene _xchg y funciones relacionadas
#include "scheduler.h"
#include "semaphores.h"
#include "strings.h"
#include "sys_calls.h"
#include "video.h"

static semaphore semSpaces[SEM_LIMIT];
static uint64_t lock_semaphore = 0;

static void enqueue(semaphore *sem, uint64_t pid);
static uint64_t dequeue(semaphore *sem);
void print_procceses_blocked(process *processNode);

// Encuentra un semáforo por nombre
static int find_semaphore(char *name) {
  for (int i = 0; i < SEM_LIMIT; i++) {
    if (semSpaces[i].name[0] != '\0' && strcmp(name, semSpaces[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

// Inicializa el sistema de semáforos
void start_semaphores() {
  for (int i = 0; i < SEM_LIMIT; i++) {
    semSpaces[i].name[0] = '\0'; // Indica semáforo no utilizado
    semSpaces[i].firstProcess = NULL;
    semSpaces[i].lastProcess = NULL;
    semSpaces[i].lock = 0;
    semSpaces[i].value = 0;
    semSpaces[i].size = 0;
    semSpaces[i].sizeList = 0;
  }
}

// Abre o crea un semáforo
uint64_t sem_open(char *name, uint64_t initValue) {
  _xchg(&lock_semaphore, 1); // Adquiere el bloqueo global

  int semIndex = find_semaphore(name);
  if (semIndex == -1) {
    // Encuentra un espacio disponible y crea el semáforo
    for (semIndex = 0;
         semIndex < SEM_LIMIT && semSpaces[semIndex].name[0] != '\0';
         semIndex++)
      ;
    if (semIndex == SEM_LIMIT) {
      _xchg(&lock_semaphore, 0); // Libera el bloqueo global
      return -1;                 // No hay espacio disponible
    }

    strncpy(semSpaces[semIndex].name, name, NAME_LIMIT - 1);
    semSpaces[semIndex].name[NAME_LIMIT - 1] = '\0';
    semSpaces[semIndex].value = initValue;
  }

  _xchg(&lock_semaphore, 0); // Libera el bloqueo global
  return semIndex;
}

// Cierra un semáforo
uint64_t sem_close(char *name) {
  _xchg(&lock_semaphore, 1); // Adquiere el bloqueo global

  int semIndex = find_semaphore(name);
  if (semIndex == -1) {
    _xchg(&lock_semaphore, 0); // Libera el bloqueo global
    return -1;                 // Semáforo no encontrado
  }

  // Limpieza del semáforo
  semSpaces[semIndex].name[0] = '\0';

  _xchg(&lock_semaphore, 0); // Libera el bloqueo global
  return 0;
}

// Añade un proceso a la cola del semáforo
static void enqueue(semaphore *sem, uint64_t pid) {
  process *newProcess = malloc(sizeof(process));
  if (newProcess == NULL) {
    return; // Manejo de error: memoria insuficiente
  }
  newProcess->pid = pid;
  newProcess->blocked = 0; // Inicializa como no bloqueado
  newProcess->next = NULL;

  if (sem->lastProcess == NULL) {
    sem->firstProcess = sem->lastProcess = newProcess;
  } else {
    sem->lastProcess->next = newProcess;
    sem->lastProcess = newProcess;
  }
  sem->sizeList++;
}

// Quita un proceso de la cola del semáforo
static uint64_t dequeue(semaphore *sem) {
  if (sem->firstProcess == NULL) {
    return -1; // La cola está vacía
  }

  process *temp = sem->firstProcess;
  uint64_t pid = temp->pid;

  sem->firstProcess = temp->next;
  if (sem->firstProcess == NULL) {
    sem->lastProcess = NULL;
  }

  free(temp);
  sem->sizeList--;

  return pid;
}
// Función auxiliar para bloquear el proceso y luego liberar el bloqueo del
// semáforo de manera atómica
static void block_process_and_release_lock(semaphore *sem, uint64_t pid) {
  enqueue(sem, pid);    // Encola el proceso antes de bloquearlo
  _xchg(&sem->lock, 0); // Libera el bloqueo del semáforo
  block(pid);           // Bloquea el proceso
}

uint64_t sem_wait(uint64_t semIndex) {
  if (semIndex >= SEM_LIMIT)
    return -1;

  acquire(&semSpaces[semIndex].lock); // Adquiere el bloqueo del semáforo

  if (semSpaces[semIndex].value > 0) {
    semSpaces[semIndex].value--;
    release(&semSpaces[semIndex].lock); // Libera el bloqueo del semáforo
  } else {
    uint64_t pid = getPid();
    block_process_and_release_lock(
        &semSpaces[semIndex],
        pid); // Bloquea el proceso y libera el bloqueo atómicamente
  }

  return 0;
}

uint64_t sem_post(uint64_t semIndex) {
  if (semIndex >= SEM_LIMIT)
    return -1;

  acquire(&semSpaces[semIndex].lock); // Adquiere el bloqueo del semáforo

  if (semSpaces[semIndex].sizeList > 0) {
    uint64_t pid = dequeue(&semSpaces[semIndex]);
    unblock(pid);
  } else {
    semSpaces[semIndex].value++;
  }

  release(&semSpaces[semIndex].lock); // Libera el bloqueo del semáforo
  return 0;
}

// Adquiere un bloqueo
void acquire(uint64_t *lock) {
  while (_xchg(lock, 1) != 0) {
    // Loop activo hasta que el bloqueo se adquiera
  }
}

// Libera un bloqueo
void release(uint64_t *lock) { _xchg(lock, 0); }

void sem() {
  print("SEM'S NAME       STATE       BLOCKED PROCESSES\n");
  for (int i = 0; i < SEM_LIMIT; i++) {
    if (semSpaces[i].name[0] != '\0') { // Chequea si el semáforo está en uso
      printSem(semSpaces[i]);
    }
  }
}

void printSem(semaphore sem) {
  print(sem.name);
  if (strlen(sem.name) > 10) {
    putTab();
    putTab();
  } else {
    putTab();
    putTab();
    putTab();
    putTab();
  }

  printInt(sem.value);
  putTab();
  putTab();
  putTab();
  print_procceses_blocked(sem.firstProcess);
  putLine();
}

void print_procceses_blocked(process *processNode) {
  while (processNode != NULL) {
    putLine();
    printInt(processNode->pid);
    print(" ");
    processNode = processNode->next;
  }
  print("-\n");
}

char *getSemName(uint64_t semIndex) {
  if (semIndex >= SEM_LIMIT) {
    print("Wrong Index in getSemName\n");
    return NULL;
  }
  return semSpaces[semIndex].name;
}
void printProcessesSem(uint64_t semIndex) {
  //     if (semIndex >= SEM_LIMIT)
  //     {
  //         print("Wrong Index in printProcessesSem\n");
  //         return;
  //     }
  //     semaphore sem = semSpaces[semIndex].sem;
  //     print_procceses_blocked(sem.firstProcess);
}