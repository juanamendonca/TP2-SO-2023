#include "queue.h"
#include "memoryManager.h"
#include "video.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Function to create a new Queue
Queue *createQueue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (!queue) {
    return NULL;
  }
  queue->front = queue->rear = queue->iterator =
      NULL; // Initialize the iterator
  return queue;
}

// Function to check if the Queue is empty
int isEmpty(Queue *queue) { return queue->front == NULL; }

// Function to check if there is at least one process in READY state in the
// Queue
int isEmptyReady(Queue *queue) {
  Node *current = queue->front;

  while (current != NULL) {
    if (current->data->state == READY) {
      return 0; // Found at least one process in READY state
    }
    current = current->next;
  }

  return 1; // No processes in READY state found
}

// Function to add a process to the Queue
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

// Function to remove and return the first process from the Queue
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

// Function to dequeue and return the first process in READY state
pcb *dequeueReady(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }

  Node *current = queue->front;
  Node *prev = NULL;

  while (current != NULL) {
    if (current->data->state == READY) {

      // Found a PCB in READY state, return and remove it
      if (prev == NULL) {
        // If the first element of the queue is in READY state
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // If the PCB in READY state is not at the beginning
        prev->next = current->next;
        if (prev->next == NULL) {
          queue->rear = prev;
        }
      }

      pcb *data = current->data;
      free(current);
      return data;
    } else if (current->data->state == KILLED) {
      // Found a PCB in KILLED state, remove it
      if (prev == NULL) {
        // If the first element of the queue is in KILLED state
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // If the PCB in KILLED state is not at the beginning
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

  return NULL; // No PCB in READY state found
}

// Function to get the first process in the Queue
pcb *front(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return queue->front->data;
}

// Function to find a process by its PID
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

// Function to initialize the Queue iterator
void startIterator(Queue *queue) { queue->iterator = queue->front; }

// Function to check if there is a next element in the Queue during iteration
int hasNext(Queue *queue) { return queue->iterator != NULL; }

// Function to return the next element in the Queue during iteration
pcb *next(Queue *queue) {
  pcb *toReturn = queue->iterator->data;
  queue->iterator = queue->iterator->next;
  return toReturn;
}

// Function to find and delete a process by its PID
pcb *getAndDeleteProcess(Queue *queue, int pid) {
  Node *current = queue->front;
  Node *prev = NULL;

  while (current != NULL) {
    if (current->data->pid == pid) {
      if (prev == NULL) {
        // If the process to delete is the first element of the queue
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // If the process to delete is not at the beginning
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

  return NULL; // No process found with the specified PID
}

// Function to delete all foreground processes with a PID greater than 1
void deletedForegroundProcesses(Queue *queue) {
  Node *current = queue->front;
  Node *prev = NULL;
  Node *next = NULL;

  while (current != NULL) {
    if (current->data->foreground && current->data->pid > 1) {
      if (prev == NULL) {
        // If the process to delete is the first element of the queue
        queue->front = current->next;
        if (queue->front == NULL) {
          queue->rear = NULL;
        }
      } else {
        // If the process to delete is not at the beginning
        prev->next = current->next;
        if (prev->next == NULL) {
          queue->rear = prev;
        }
      }
      next = current->next;
      if (current->data->waitingPid > 0) {
        unblock(current->data->ppid);
      }
      freePcb(current->data);
      free(current);
    } else {
      next = current->next;
      prev = current;
    }

    current = next;
  }
}
