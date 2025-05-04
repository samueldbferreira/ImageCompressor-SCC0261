#ifndef MIN_HEAP_H 
#define MIN_HEAP_H

#include "tree.h"

typedef struct MinHeap {
  Tree_t** arr;
  int size;
  int capacity;
} MinHeap_t;

#define MAX_HEAP_SIZE 512

MinHeap_t *createHeap();

void heapInsert(MinHeap_t *heap, Tree_t *tree);

Tree_t *extractMin(MinHeap_t *heap);

void printHeap(MinHeap_t *heap);

int heapSize(MinHeap_t *heap);

void destroyHeap(MinHeap_t *heap);

#endif
