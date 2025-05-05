#include <stdio.h>
#include <stdlib.h>
#include "list.h"

List_t* createList() {
  List_t* list = (List_t*) malloc(sizeof(List_t));

  if (list == NULL) {
    printf("Invalid list (NULL) on createList.\n");
    return NULL;
  }

  list->size = 0;
  list->first = NULL;
  list->last = NULL;

  return list;
};

void listInsert(List_t* list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.\n");
    return;
  }

  ListNode_t* newNode = (ListNode_t*) malloc(sizeof(ListNode_t));
  newNode->difference = difference;
  newNode->frequence = 1;
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

ListNode_t* listSearch(List_t* list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listSearch.\n");
    return NULL;
  }

  ListNode_t* aux = list->first;
  while (aux != NULL) {
    if (aux->difference == difference)
    {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

void incrementFrequence(ListNode_t* item) {
  if (item == NULL) {
    printf("Invalid item (null) at incrementFrequence.\n");
    return;
  }

  item->frequence++;
}

int getListSize(List_t* list) {
  return list->size;
}

void concatenateLists(List_t* a, List_t* b) {
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

void printItem(ListNode_t* item) {
  if (item == NULL) {
    printf("Invalid item (null) at printItem.\n");
    return;
  }

  printf("(diff > %i; freq > %i)\n", item->difference, item->frequence);
}

void printList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for printList.\n");
    return;
  }

  int i = 1;
  ListNode_t* aux = list->first;
  while (aux != NULL) {
    printf("NODE %d: (diff > %i; freq > %i)\n", i, aux->difference, aux->frequence);

    aux = aux->next;

    i++;
  }
  printf("\n");
}

void destroyList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for destroyList.\n");
    return;
  }

  ListNode_t* aux = list->first;
  while (aux != NULL) {
    list->first = aux->next;
    free(aux);
    aux = list->first;
  }

  free(list);
}
