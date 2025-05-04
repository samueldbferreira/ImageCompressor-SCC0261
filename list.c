
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

List_t* createList() {
  List_t* list = (List_t*) malloc(sizeof(List_t));

  if (list == NULL) {
    printf("Invalid list (NULL) on createList.");
    return NULL;
  }

  list->first = NULL;
  list->last = NULL;

  return list;
};

void listInsert(List_t* list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.");
    return;
  }

  ListNode_t* newNode = (ListNode_t*) malloc(sizeof(ListNode_t));
  newNode->difference = difference;
  newNode->frequence = 0;
  newNode->next = NULL;

  if (list->first == NULL) {
    list->first = newNode;
    list->last = newNode;
    return;
  }

  list->last->next = newNode;
  list->last = newNode;
}

void printList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for printList.");
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
    printf("Invalid list (NULL) for destroyList.");
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
