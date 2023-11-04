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
int isFull(Queue *queue);
void enqueue(Queue *queue, pcb *data);
pcb *dequeue(Queue *queue);
pcb *front(Queue *queue);
pcb *iterate(Queue *queue);
void destroyQueue(Queue *queue);

#endif