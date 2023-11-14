#include "priorityQueue.h"
#include "memoryManager.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

PriorityQueue *createPriorityQueue() {
  PriorityQueue *queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (!queue) {
    return NULL;
  }
  queue->q1 = createQueue();
  queue->q2 = createQueue();
  queue->q3 = createQueue();
  queue->q4 = createQueue();

  if (queue->q1 == NULL || queue->q2 == NULL || queue->q3 == NULL ||
      queue->q4 == NULL) {
    return NULL;
  }

  return queue;
}

int isEmptyP(PriorityQueue *queue) {
  return isEmpty(queue->q1) && isEmpty(queue->q2) && isEmpty(queue->q3) &&
         isEmpty(queue->q4);
}

int isEmptyPReady(PriorityQueue *queue) {
  return isEmptyReady(queue->q1) && isEmptyReady(queue->q2) &&
         isEmptyReady(queue->q3) && isEmptyReady(queue->q4);
}

void enqueueP(PriorityQueue *queue, pcb *data, int priority) {
  if (priority == 1) {
    enqueue(queue->q1, data);
  }
  if (priority == 2) {
    enqueue(queue->q2, data);
  }
  if (priority == 3) {
    enqueue(queue->q3, data);
  }
  if (priority == 4) {
    enqueue(queue->q4, data);
  }
}

pcb *dequeueP(PriorityQueue *queue) {
  pcb *data = dequeue(queue->q1);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q1, se devuelve
  }

  data = dequeue(queue->q2);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q2, se devuelve
  }

  data = dequeue(queue->q3);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q3, se devuelve
  }

  data = dequeue(queue->q4);
  return data; // Se devuelva el proceso en q4 o NULL si no se encuentra ninguno
}

pcb *dequeuePReady(PriorityQueue *queue) {
  pcb *data = dequeueReady(queue->q1);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q1 en estado READY, se
                 // devuelve
  }

  data = dequeueReady(queue->q2);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q2 en estado READY, se
                 // devuelve
  }

  data = dequeueReady(queue->q3);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q3 en estado READY, se
                 // devuelve
  }

  data = dequeueReady(queue->q4);
  if (data != NULL) {
    return data; // Si se encuentra un proceso en q4 en estado READY, se
                 // devuelve
  }

  return NULL; // Si no se encuentra ningún proceso en estado READY, se devuelve
               // NULL
}

void destroyP(PriorityQueue *queue) {
  destroyQueue(queue->q1);
  destroyQueue(queue->q2);
  destroyQueue(queue->q3);
  destroyQueue(queue->q4);
  free(queue);
}

pcb *getProcessP(PriorityQueue *queue, int pid) {
  pcb *process = getProcess(queue->q1, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q1, se devuelve
  }

  process = getProcess(queue->q2, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q2, se devuelve
  }

  process = getProcess(queue->q3, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q3, se devuelve
  }

  process = getProcess(queue->q4, pid);
  return process; // Si se encuentra el proceso en q4 o no se encuentra en
                  // ninguna cola, se devuelve
}

void startIteratorP(PriorityQueue *queue) {
  startIterator(queue->q1);
  startIterator(queue->q2);
  startIterator(queue->q3);
  startIterator(queue->q4);
}

int hasNextP(PriorityQueue *queue) {
  return hasNext(queue->q1) || hasNext(queue->q2) || hasNext(queue->q3) ||
         hasNext(queue->q4);
}

pcb *nextP(PriorityQueue *queue) {
  if (hasNext(queue->q1)) {
    return next(queue->q1);
  }
  if (hasNext(queue->q2)) {
    return next(queue->q2);
  }
  if (hasNext(queue->q3)) {
    return next(queue->q3);
  }
  if (hasNext(queue->q4)) {
    return next(queue->q4);
  }
  return NULL;
}

pcb *getAndDeleteProcessP(PriorityQueue *queue, int pid) {
  pcb *process = getAndDeleteProcess(queue->q1, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q1, se devuelve
  }

  process = getAndDeleteProcess(queue->q2, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q2, se devuelve
  }

  process = getAndDeleteProcess(queue->q3, pid);
  if (process != NULL) {
    return process; // Si se encuentra el proceso en q3, se devuelve
  }

  process = getAndDeleteProcess(queue->q4, pid);
  return process; // Si se encuentra el proceso en q4 o no se encuentra en
                  // ninguna cola, se devuelve
}

pcb *getAndDeleteFirstProcessP(PriorityQueue *queue) {
    pcb *process = getAndDeleteFirstProcess(queue->q1);
    if (process != NULL) {
        return process; // Si se encuentra el proceso en q1, se devuelve
    }

    process = getAndDeleteFirstProcess(queue->q2);
    if (process != NULL) {
        return process; // Si se encuentra el proceso en q2, se devuelve
    }

    process = getAndDeleteFirstProcess(queue->q3);
    if (process != NULL) {
        return process; // Si se encuentra el proceso en q3, se devuelve
    }

    process = getAndDeleteFirstProcess(queue->q4);
    return process; // Si se encuentra el proceso en q4 o no se encuentra en
    // ninguna cola, se devuelve
}