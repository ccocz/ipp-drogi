#include "queue.h"
#include "heap.h"
#include <stdlib.h>

Queue *newQueue(void) {
  Queue *aux = malloc(sizeof(Queue));
  aux->head = aux->tail = NULL;
  return aux;
}

bool isEmpty(Queue *queue) {
  return !queue->head;
}

void pushQueue(Queue *queue, HeapNode *node) {
  if (!queue->head) {
    QueueNode *aux = malloc(sizeof(QueueNode));
    aux->node = node;
    aux->next = NULL;
    aux->prev = NULL;
    queue->head = queue->tail = aux;
  } else {
    QueueNode *aux = malloc(sizeof(QueueNode));
    aux->node = node;
    aux->next = NULL;
    aux->prev = queue->tail;
    queue->tail->next = aux;
    queue->tail = aux;
  }
}

void popQueue(Queue *queue) {
  queue->head = queue->head->next;
}

HeapNode *topQueue(Queue *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return queue->head;
}

HeapNode *popEndQueue(Queue *queue) {
  HeapNode *aux = queue->tail->node;
  QueueNode *tbfreed = queue->tail;
  queue->tail = queue->tail->prev;
  free(tbfreed);
  return aux;
}

void freeQueue(Queue *queue) {
  QueueNode *fr;
  while (queue->tail) {
    fr = queue->tail;
    queue->tail = queue->tail->prev;
    free(fr);
  }
  free(queue);
}