#ifndef DROGI_HEAP_H
#define DROGI_HEAP_H

#include "map.h"
#include "queue.h"

typedef struct City City;
typedef struct HeapNode HeapNode;
typedef struct Heap Heap;
typedef struct Queue Queue;
typedef struct Road Road;

/**
 * @brief Structure for certain heap node
 */
struct HeapNode{
  City *city;               /**<The city in the node*/
  Road *from;               /**<Best way found to that node*/
  bool visited;             /**<Is the node visited before*/
  uint64_t distance;        /**<The best distance to node found so far*/
  int year;                 /**<The newest year to node found so far*/
  struct HeapNode *parent;  /**<Parent node in the heap*/
  struct HeapNode *left;    /**<Left node in the heap*/
  struct HeapNode *right;   /**<Right node in the heap*/
};
/**
 * @brief Structure for whole heap
 */
struct Heap{
  HeapNode *root;           /**<The root of the heap*/
  Queue *last;              /**<Structure queue used to build heap*/
};

Heap *newHeap(City *root);
void insertHeap(Heap *heap, struct City *city);
HeapNode *minHeap(Heap *heap);
void decreaseValue(HeapNode *node, uint64_t dist, int year);
void freeMe(Heap *heap, HeapNode *root);
void heapifyMin(HeapNode *root);
void freeHeap(Heap *heap);

#endif