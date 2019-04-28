#include "heap.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct HeapNode HeapNode;

Heap *newHeap(City *root) {
  Heap *aux = malloc(sizeof(Heap));
  HeapNode *start = malloc(sizeof(HeapNode));
  start->city = root;
  start->distance = 0;
  start->visited = false;
  start->year = INT32_MAX;
  start->left = NULL;
  start->right = NULL;
  start->parent = NULL;
  start->from = NULL;
  root->heapNode = start;
  Queue *new = newQueue();
  pushQueue(new, start);
  aux->last = new;
  aux->root = start;
  return aux;
}

void insertHeap(Heap *heap, City *city) {
  QueueNode *aux = topQueue(heap->last);
  if (aux->node->left) {
    aux->node->right = malloc(sizeof(HeapNode));
    aux->node->right->parent = aux->node;
    aux->node->right->left = NULL;
    aux->node->right->right = NULL;
    aux->node->right->visited = false;
    aux->node->right->city = city;
    aux->node->right->distance = UINT64_MAX;
    aux->node->right->year = 0;
    aux->node->right->from = NULL;
    city->heapNode = aux->node->right;
    pushQueue(heap->last, aux->node->right);
    popQueue(heap->last);
  } else {
    aux->node->left = malloc(sizeof(HeapNode));
    aux->node->left->parent = aux->node;
    aux->node->left->left = NULL;
    aux->node->left->right = NULL;
    aux->node->left->visited = false;
    aux->node->left->city = city;
    aux->node->left->distance = UINT64_MAX;
    aux->node->left->year = 0;
    aux->node->left->from = NULL;
    city->heapNode = aux->node->left;
    pushQueue(heap->last, aux->node->left);
  }
}

void swapInfo(HeapNode *a, HeapNode *b) {
  uint64_t auxDistance = a->distance;
  int auxYear = a->year;
  int auxLastYear = a->lastYear;
  Road *auxFrom = a->from;
  City *auxCity = a->city;
  a->distance = b->distance;
  a->year = b->year;
  a->city = b->city;
  a->from = b->from;
  a->lastYear = b->lastYear;
  b->distance = auxDistance;
  b->year = auxYear;
  b->city = auxCity;
  b->from = auxFrom;
  b->lastYear = auxLastYear;
  a->city->heapNode = a;
  b->city->heapNode = b;
}

inline bool maxi(int x, int y) {
  if (x < 0 && y < 0) {
    return x <= y;
  }
  else {
    return x >= y;
  }
}

void heapifyMin(HeapNode *root) {
  if (!root->left && root->right) {
    if (root->right->distance == root->distance) {
      if(maxi(root->right->year, root->year)) {
        swapInfo(root->right, root);
        heapifyMin(root->right);
      }
    } else if (root->right->distance < root->distance) {
      swapInfo(root->right, root);
      heapifyMin(root->right);
    }
  }
  else if (root->left && !root->right) {
    if (root->left->distance == root->distance) {
      if(maxi(root->left->year, root->year)) {
        swapInfo(root->left, root);
        heapifyMin(root->left);
      }
    } else if (root->left->distance < root->distance) {
      swapInfo(root->left, root);
      heapifyMin(root->left);
    }
  } else if (root->left && root->right) {
    if (root->left->distance < root->right->distance ||
        (root->left->distance == root->right->distance &&
         maxi(root->left->year, root->right->year))) {
      if (root->left->distance < root->distance ||
          (root->left->distance == root->distance &&
           maxi(root->left->year, root->year))) {
        swapInfo(root->left, root);
        heapifyMin(root->left);
      }
    }
    else if (root->right->distance < root->left->distance ||
        (root->right->distance == root->left->distance &&
         maxi(root->right->year, root->left->year))) {
      if (root->right->distance < root->distance ||
          (root->right->distance == root->distance &&
           maxi(root->right->year, root->year))) {
        swapInfo(root->right, root);
        heapifyMin(root->right);
      }
    }
  }
}

void freeMe(Heap *heap, HeapNode *root) {
  if (root->parent) {
    if (root->parent->left == root) {
      root->parent->left = NULL;
    } else {
      root->parent->right = NULL;
    }
  } else {
    heap->root = NULL;
  }
  root->visited = true;
}

HeapNode *minHeap(Heap *heap) {
  HeapNode *last = popEndQueue(heap->last);
  swapInfo(heap->root, last);
  freeMe(heap, last);
  if (heap->root) {
    heapifyMin(heap->root);
  }
  return last;
}

void goUp(HeapNode *root) {
  if (root->parent) {
    if (root->parent->distance > root->distance) {
      swapInfo(root->parent, root);
      goUp(root->parent);
    } else if (root->parent->distance == root->distance &&
               maxi(root->year, root->parent->year)) {
      swapInfo(root->parent, root);
      goUp(root->parent);
    }
  }
}

void decreaseValue(HeapNode *root, uint64_t dist, int year) {
  root->distance = dist;
  root->year = year;
  goUp(root);
}

void freeHeap(Heap *heap) {
  freeQueue(heap->last);
  free(heap);
}