#ifndef DROGI_QUEUE_H
#define DROGI_QUEUE_H

#include "heap.h"

typedef struct HeapNode HeapNode;
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;
/**
 * @brief Structure for certain queue element
 */
struct QueueNode{
  HeapNode *node;           /**<The node in the heap element represents*/
  struct QueueNode *next;   /**<Next queue element*/
  struct QueueNode *prev;   /**<Previous queue element*/
};
/**
 * @brief Structure for whole of the queue
 */
struct Queue{
  QueueNode *head;          /**<The head of the queue*/
  QueueNode *tail;          /**<The tail of the queue*/
};

Queue *newQueue(void);
void pushQueue(Queue *queue, HeapNode *node);
void popQueue(Queue *queue);
QueueNode *topQueue(Queue *queue);
bool isEmpty(Queue *queue);
HeapNode *popEndQueue(Queue *queue);
void freeQueue(Queue *queue);

#endif
