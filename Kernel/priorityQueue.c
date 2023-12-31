#include "priorityQueue.h"
#include "memoryManager.h"
#include "queue.h"
#include "video.h"
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
    return data; // If a proccess is found in q1 then is returned
  }

  data = dequeue(queue->q2);
  if (data != NULL) {
    return data; // If a proccess is found in q2 then is returned
  }

  data = dequeue(queue->q3);
  if (data != NULL) {
    return data; // If a proccess is found in q3 then is returned
  }

  data = dequeue(queue->q4);
  return data; // A process is returned in q4 or NULL if nothing is found.
}

pcb *dequeuePReady(PriorityQueue *queue) {
  pcb *data = dequeueReady(queue->q1);
  if (data != NULL) {
    return data; // If a proccess is found in q1 in a READY STATE then is
                 // returned
  }

  data = dequeueReady(queue->q2);
  if (data != NULL) {
    return data; // If a proccess is found in q2 in a READY STATE then is
                 // returned
  }

  data = dequeueReady(queue->q3);
  if (data != NULL) {
    return data; // If a proccess is found in q3 in a READY STATE then is
                 // returned
  }

  data = dequeueReady(queue->q4);
  if (data != NULL) {
    return data; // If a proccess is found in q4 in a READY STATE then is
                 // returned
  }

  return NULL; // NULL is returned if no proccess is found
}

pcb *getProcessP(PriorityQueue *queue, int pid) {
  pcb *process = getProcess(queue->q1, pid);
  if (process != NULL) {
    return process; // If a proccess is found in q1 then is returned
  }

  process = getProcess(queue->q2, pid);
  if (process != NULL) {
    return process; // If a proccess is found in q2 then is returned
  }

  process = getProcess(queue->q3, pid);
  if (process != NULL) {
    return process; // If a proccess is found in q3 then is returned
  }

  process = getProcess(queue->q4, pid);
  return process; // A process is returned in q4 or NULL if nothing is found.
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

    return process; // If a proccess is found in q1 then is returned
  }

  process = getAndDeleteProcess(queue->q2, pid);
  if (process != NULL) {

    return process; // If a proccess is found in q2 then is returned
  }

  process = getAndDeleteProcess(queue->q3, pid);
  if (process != NULL) {

    return process; // If a proccess is found in q3 then is returned
  }

  process = getAndDeleteProcess(queue->q4, pid);

  return process; // If a proccess is found in q4 then is returned
}

void deletedForegroundProcessesP(PriorityQueue *queue) {
  deletedForegroundProcesses(queue->q1);
  deletedForegroundProcesses(queue->q2);
  deletedForegroundProcesses(queue->q3);
  deletedForegroundProcesses(queue->q4);
}