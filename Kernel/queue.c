#include <memoryManager.h>
#include <queue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Queue *createQueue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (!queue) {
    return NULL;
  }
  queue->front = queue->rear = queue->iterator = NULL; // Inicializa el iterador
  return queue;
}

int isEmpty(Queue *queue) { return queue->front == NULL; }

void enqueue(Queue *queue, pcb *data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    return;
  }
  newNode->data = data;
  newNode->next = NULL;

  if (isEmpty(queue)) {
    queue->front = queue->rear = newNode;
    queue->iterator = newNode; // Inicializa el iterador si la cola está vacía
    return;
  }

  queue->rear->next = newNode;
  queue->rear = newNode;
}

pcb *dequeue(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }

  Node *temp = queue->front;
  pcb *data = temp->data;

  queue->front = queue->front->next;

  // Actualiza el iterador
  if (queue->front == NULL) {
    queue->rear = NULL;
    queue->iterator = NULL;
  } else {
    queue->iterator = queue->front;
  }

  free(temp);

  return data;
}

pcb *front(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return queue->front->data;
}

pcb *iterate(Queue *queue) {
  if (queue->iterator == NULL) {
    return NULL;
  }
  pcb *data = queue->iterator->data;
  queue->iterator = queue->iterator->next;
  if (queue->iterator == NULL) {
    queue->iterator = queue->front; // Vuelve al principio al alcanzar el final
  }
  return data;
}

void destroyQueue(Queue *queue) {
  while (!isEmpty(queue)) {
    dequeue(queue);
  }
  free(queue);
}