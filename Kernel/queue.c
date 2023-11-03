#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <queue.h>
#include <memoryManager.h>

Queue *createQueue()
{
    Queue *queue = (Queue *)allocMemory(sizeof(Queue));
    if (!queue)
    {
        return NULL;
    }
    queue->front = queue->rear = queue->iterator = NULL; // Inicializa el iterador
    return queue;
}

int isEmpty(Queue *queue)
{
    return queue->front == NULL;
}

void enqueue(Queue *queue, int data)
{
    Node *newNode = (Node *)allocMemory(sizeof(Node));
    if (!newNode)
    {
        return;
    }
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue))
    {
        queue->front = queue->rear = newNode;
        queue->iterator = newNode; // Inicializa el iterador si la cola está vacía
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

int dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        return -1; // Puedes manejar esta condición de error de manera diferente
    }

    Node *temp = queue->front;
    int data = temp->data;

    queue->front = queue->front->next;

    // Actualiza el iterador
    if (queue->front == NULL)
    {
        queue->rear = NULL;
        queue->iterator = NULL;
    }
    else
    {
        queue->iterator = queue->front;
    }

    //free(temp);

    return data;
}

int front(Queue *queue)
{
    if (isEmpty(queue))
    {
        return -1; // Puedes manejar esta condición de error de manera diferente
    }
    return queue->front->data;
}

int iterate(Queue *queue)
{
    if (queue->iterator == NULL)
    {
        return -1; // Puedes manejar esta condición de error de manera diferente
    }
    int data = queue->iterator->data;
    queue->iterator = queue->iterator->next;
    if (queue->iterator == NULL)
    {
        queue->iterator = queue->front; // Vuelve al principio al alcanzar el final
    }
    return data;
}

void destroyQueue(Queue *queue)
{
    while (!isEmpty(queue))
    {
        dequeue(queue);
    }
    //free(queue);
}