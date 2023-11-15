#include "scheduler.h"
#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
  pcb *data;
  struct Node *next;
} Node;

typedef struct Queue {
  Node *front;
  Node *rear;
  Node *iterator;
} Queue;

Queue *createQueue();

int isEmpty(Queue *queue);

int isEmptyReady(Queue *queue);

int isFull(Queue *queue);

void enqueue(Queue *queue, pcb *data);

pcb *dequeue(Queue *queue);

pcb *dequeueReady(Queue *queue);

pcb *front(Queue *queue);

void destroyQueue(Queue *queue);

pcb *getProcess(Queue *queue, int pid);

void startIterator(Queue *queue);

int hasNext(Queue *queue);

pcb *next(Queue *queue);

pcb *getAndDeleteProcess(Queue *queue, int pid);

pcb *getAndDeleteFirstProcess(Queue *queue);

void deletedForegroundProcesses(Queue *queue);

#endif