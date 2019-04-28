#ifndef DROGI_QUEUE_H
#define DROGI_QUEUE_H

#include "heap.h"

typedef struct HeapNode HeapNode;
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;

struct QueueNode{
  HeapNode *node;
  struct QueueNode *next;
  struct QueueNode *prev;
};

struct Queue{
  QueueNode *head;
  QueueNode *tail;
};

Queue *newQueue(void);
void pushQueue(Queue *queue, HeapNode *node);
void popQueue(Queue *queue);
QueueNode *topQueue(Queue *queue);
bool isEmpty(Queue *queue);
HeapNode *popEndQueue(Queue *queue);
void freeQueue(Queue *queue);

#endif //DROGI_QUEUE_H
