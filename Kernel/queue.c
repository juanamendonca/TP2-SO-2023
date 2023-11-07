#include "queue.h"
#include "memoryManager.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Queue *createQueue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (!queue) {
    return NULL;
  }
  queue->front = queue->rear = NULL; // Inicializa el iterador
  return queue;
}

int isEmpty(Queue *queue) { return queue->front == NULL; }

int isEmptyReady(Queue *queue) {
  Node *current = queue->front;

  while (current != NULL) {
    if (current->data->state == READY) {
      return 0; // Se encontró al menos un proceso en estado READY
    }
    current = current->next;
  }

  return 1; // No se encontraron procesos en estado READY
}

void enqueue(Queue *queue, pcb *data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    return;
  }
  newNode->data = data;
  newNode->next = NULL;

  if (isEmpty(queue)) {
    queue->front = queue->rear = newNode;
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

  if (queue->front == NULL) {
    queue->rear = NULL;
  }

  free(temp);

  return data;
}

pcb *dequeueReady(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }

  Node *current = queue->front;
  Node *prev = NULL;

  while (current != NULL && current->data->state != READY) {
    prev = current;
    current = current->next;
  }

  if (current == NULL) {
    return NULL; // No se encontró un PCB en estado READY
  }

  // El PCB en estado READY se encuentra en 'current'
  if (prev == NULL) {
    // Si el primer elemento de la cola está en estado READY
    queue->front = current->next;
    if (queue->front == NULL) {
      queue->rear = NULL;
    }
  } else {
    // Si el PCB en estado READY está en una posición distinta al principio
    prev->next = current->next;
    if (prev->next == NULL) {
      queue->rear = prev;
    }
  }

  pcb *data = current->data;
  free(current);

  return data;
}

pcb *front(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return queue->front->data;
}

void destroyQueue(Queue *queue) {
  while (!isEmpty(queue)) {
    dequeue(queue);
  }
  free(queue);
}

pcb *getProcess(Queue *queue, int pid) {
  Node *current = queue->front;
  while (current != NULL) {
    if (current->data->pid == pid) {
      return current->data;
    }
    current = current->next;
  }
  return NULL;
}