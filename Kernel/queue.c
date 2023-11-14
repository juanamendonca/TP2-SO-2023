#include "queue.h"
#include "memoryManager.h"
#include "video.h"
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

  while (current != NULL) {
    if (current->data->state == READY) {

      // Se encontró un PCB en estado READY, devolverlo y eliminarlo
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
    } else if (current->data->state == KILLED) {
      // Se encontró un PCB en estado KILLED, eliminarlo
      if (prev == NULL) {
        // Si el primer elemento de la cola está en estado KILLED
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // Si el PCB en estado KILLED está en una posición distinta al principio
        prev->next = current->next;
        if (prev->next == NULL) {
          queue->rear = prev;
        }
      }
      freePcb(current->data);
      free(current);
    } else {
      prev = current;
    }

    current = current->next;
  }

  return NULL; // No se encontró un PCB en estado READY
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

void startIterator(Queue *queue) { queue->iterator = queue->front; }

int hasNext(Queue *queue) { return queue->iterator != NULL; }

pcb *next(Queue *queue) {
  pcb *toReturn = queue->iterator->data;
  queue->iterator = queue->iterator->next;
  return toReturn;
}

pcb *getAndDeleteProcess(Queue *queue, int pid) {
  Node *current = queue->front;
  Node *prev = NULL;

  while (current != NULL) {
    if (current->data->pid == pid) {
      if (prev == NULL) {
        // Si el proceso a eliminar es el primer elemento de la cola
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // Si el proceso a eliminar está en una posición distinta al principio
        prev->next = current->next;
        if (prev->next == NULL) {
          queue->rear = prev;
        }
      }

      pcb *data = current->data;
      free(current);
      return data;
    } else {
      prev = current;
    }

    current = current->next;
  }

  return NULL; // No se encontró un proceso con el PID especificado
}

pcb *getAndDeleteFirstProcess(Queue *queue) {
    Node *current = queue->front;
    Node *prev = NULL;

    while (current != NULL) {
        if (current->data->foreground && current->data->pid > 1) {
            if (prev == NULL) {
                // Si el proceso a eliminar es el primer elemento de la cola
                queue->front = current->next;
                if (queue->front == NULL) {
                    queue->rear = NULL;
                }
            } else {
                // Si el proceso a eliminar está en una posición distinta al principio

                prev->next = current->next;
                if (prev->next == NULL) {
                    queue->rear = prev;
                }
            }

            pcb *data = current->data;
            free(current);
            return data;
        } else {
            prev = current;
        }

        current = current->next;
    }


    return NULL; // No se encontró un proceso con el PID especificado
}
