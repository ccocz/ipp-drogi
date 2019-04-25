#ifndef DROGI_QUEUE_H
#define DROGI_QUEUE_H

#include "heap.h"

typedef struct HeapNode HeapNode;

struct QueueNode{
  HeapNode *node;
  struct QueueNode *next;
  struct QueueNode *prev;
};

typedef struct QueueNode QueueNode;

struct Queue{
  QueueNode *head;
  QueueNode *tail;
};

typedef struct Queue Queue;

Queue *newQueue(void);
void pushQueue(Queue *queue, HeapNode *node);
void popQueue(Queue *queue);
HeapNode *topQueue(Queue *queue);
bool isEmpty(Queue *queue);
HeapNode *popEndQueue(Queue *queue);
void freeQueue(Queue *queue);

#endif //DROGI_QUEUE_H
