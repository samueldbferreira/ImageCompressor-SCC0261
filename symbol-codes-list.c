#include <stdio.h>
#include <stdlib.h>
#include "symbol-codes-list.h"

SymbolCodesList_t* createSymbolCodesList() {
  SymbolCodesList_t* list = (SymbolCodesList_t*) malloc(sizeof(SymbolCodesList_t));

  if (list == NULL) {
    printf("Invalid list (NULL) on createList.\n");
    return NULL;
  }

  list->size = 0;
  list->first = NULL;
  list->last = NULL;

  return list;
}

void symbolCodesListInsert(SymbolCodesList_t* list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.\n");
    return;
  }

  SymbolCode_t* newNode = (SymbolCode_t*) malloc(sizeof(SymbolCode_t));
  newNode->difference = difference;
  newNode->code = NULL;
  newNode->codeSize = 0;
  newNode->next = NULL;

  if (list->first == NULL) {
    list->size++;
    list->first = newNode;
    list->last = newNode;
    return;
  }

  list->size++;
  list->last->next = newNode;
  list->last = newNode;
}

SymbolCode_t *symbolCodesListSearch(SymbolCodesList_t *list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listSearch.\n");
    return NULL;
  }

  SymbolCode_t* aux = list->first;
  while (aux != NULL) {
    if (aux->difference == difference)
    {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

int getSymbolCodesListSize(SymbolCodesList_t *list) {
  return list->size;
}

void concatenateSymbolCodesLists(SymbolCodesList_t *a, SymbolCodesList_t *b) {
  if (a == NULL || b == NULL) {
    printf("Invalid lists (NULL) for concatenateLists.");
    return;
  }

  if (a->size == 0) {
    a->first = b->first;
    a->last = b->last;
    a->size = b->size;
  } else {
    a->last->next = b->first;
    a->last = b->last;
    a->size += b->size;
  }

  b->first = NULL;
  b->last = NULL;
}

void printSymbolCodeItem(SymbolCode_t *item) {
  if (item == NULL) {
    printf("Invalid item (null) at printItem.\n");
    return;
  }

  printf("(diff > %i; codeSize > %i; ", item->difference, item->codeSize);
  if (item->codeSize > 0) {
    printf("code > %s)\n", item->code);
  }
}

void printSymbolCodesList(SymbolCodesList_t* list) {
    if (list == NULL) {
    printf("Invalid list (NULL) for printList.\n");
    return;
  }

  int i = 1;
  SymbolCode_t* aux = list->first;
  while (aux != NULL) {
    printf("(diff > %i; codeSize > %i; ", aux->difference, aux->codeSize);
    if (aux->codeSize > 0) {
      printf("code > %s)\n", aux->code);
    }

    aux = aux->next;

    i++;
  }
  printf("\n");
}

void destroySymbolCodesList(SymbolCodesList_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for destroyList.\n");
    return;
  }

  SymbolCode_t* aux = list->first;
  while (aux != NULL) {
    list->first = aux->next;
    free(aux);
    aux = list->first;
  }

  free(list);
}
