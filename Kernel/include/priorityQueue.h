#include "queue.h"
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

typedef struct PriorityQueue {
  Queue *q1;
  Queue *q2;
  Queue *q3;
  Queue *q4;
} PriorityQueue;

PriorityQueue *createPriorityQueue();

int isEmptyP(PriorityQueue *queue);

int isEmptyPReady(PriorityQueue *queue);

void enqueueP(PriorityQueue *queue, pcb *data, int priority);

pcb *dequeueP(PriorityQueue *queue);

pcb *dequeuePReady(PriorityQueue *queue);

pcb *getProcessP(PriorityQueue *queue, int pid);

void startIteratorP(PriorityQueue *queue);

int hasNextP(PriorityQueue *queue);

pcb *nextP(PriorityQueue *queue);

pcb *getAndDeleteProcessP(PriorityQueue *queue, int pid);

void deletedForegroundProcessesP(PriorityQueue *queue);

#endif