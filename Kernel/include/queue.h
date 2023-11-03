#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *front;
    Node *rear;
    Node *iterator;
} Queue;

Queue *createQueue();
int isEmpty(Queue *queue);
int isFull(Queue *queue);
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
int front(Queue *queue);
int iterate(Queue *queue);
void destroyQueue(Queue *queue);

#endif