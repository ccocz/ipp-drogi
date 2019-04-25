#ifndef DROGI_HEAP_H
#define DROGI_HEAP_H

#include "map.h"
#include "queue.h"

typedef struct City City;
typedef struct HeapNode HeapNode;
typedef struct Heap Heap;
typedef struct Queue Queue;

struct HeapNode{
  City *city;
  City *from;
  bool visited;
  int lastYear;
  int distance;
  int year;
  struct HeapNode *parent;
  struct HeapNode *left;
  struct HeapNode *right;
};

struct Heap{
  HeapNode *root;
  Queue *last;
};

Heap *newHeap(City *root);
void insertHeap(Heap *heap, struct City *city);
HeapNode *minHeap(Heap *heap);
void decreaseValue(HeapNode *node, unsigned dist, int year);
void freeMe(Heap *heap, HeapNode *root);
void heapifyMin(HeapNode *root);
void freeHeap(Heap *heap);

#endif //DROGI_HEAP_H
