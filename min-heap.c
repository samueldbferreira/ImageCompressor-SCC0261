#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "min-heap.h"

void swap(Tree_t** a, Tree_t** b) {
  Tree_t* temp = *a;
  *a = *b;
  *b = temp;
}

int left_child(int i) {
  return 2 * i + 1;
}

int right_child(int i) {
  return 2 * i + 2;
}

int parent(int i) {
  return (i - 1) / 2;
}

MinHeap_t* createHeap() {
  MinHeap_t* heap = (MinHeap_t*) malloc(sizeof(MinHeap_t));

  heap->arr = (Tree_t**) malloc(MAX_HEAP_SIZE * sizeof(Tree_t*));
  heap->size = 0;
  heap->capacity = MAX_HEAP_SIZE;

  return heap;
}

void heapInsert(MinHeap_t* heap, Tree_t* tree) {
  if (heap->size == heap->capacity) {
    printf("Max heap capacity reached.\n");
    return;
  }

  heap->arr[heap->size] = tree;
  int current = heap->size;
  heap->size++;

  while (current != 0 && heap->arr[current]->root->frequence < heap->arr[parent(current)]->root->frequence) {
    swap(&heap->arr[current], &heap->arr[parent(current)]);
    current = parent(current);
  }
}

Tree_t* extractMin(MinHeap_t *heap) {
  if (heap->size == 0) {
    printf("The heap is already empty.\n");
    return NULL;
  }

  Tree_t* min = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size - 1];
  heap->size--;

  int current = 0;
  while (1) {
    int left = left_child(current);
    int right = right_child(current);
    int smallest = current;

    if (left < heap->size && heap->arr[left]->root->frequence < heap->arr[smallest]->root->frequence) {
      smallest = left;
    }
      
    if (right < heap->size && heap->arr[right]->root->frequence < heap->arr[smallest]->root->frequence) {
      smallest = right;
    }

    if (smallest == current) {
      break;
    }

    swap(&heap->arr[current], &heap->arr[smallest]);

    current = smallest;
  }

  return min;
}

void printHeap(MinHeap_t* heap) {
  for (int i = 0; i < heap->size; i++) {
    printf("Tree %d: ", i);
    printTree(heap->arr[i]);
  }
};

int heapSize(MinHeap_t* heap) {
  return heap->size;
}

void destroyHeap(MinHeap_t* heap) {
  for (int i = 0; i < heap->size; i++) {
    destroyTree(heap->arr[i]);
  }
  free(heap->arr);
  free(heap);
}
