#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Cria uma lista encadeada vazia
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
}

// Insere novo nó com valor e frequência = 1 ao final da lista
void listInsert(List_t* list, int value) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.\n");
    return;
  }

  ListNode_t* newNode = (ListNode_t*) malloc(sizeof(ListNode_t));
  newNode->value = value;
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

// Busca um nó com valor específico dentro da lista
ListNode_t* listSearch(List_t* list, int value) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listSearch.\n");
    return NULL;
  }

  ListNode_t* aux = list->first;
  while (aux != NULL) {
    if (aux->value == value) {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

// Incrementa a frequência de um nó (já existente)
void incrementFrequence(ListNode_t* item) {
  if (item == NULL) {
    printf("Invalid item (null) at incrementFrequence.\n");
    return;
  }

  item->frequence++;
}

// Retorna o tamanho da lista
int getListSize(List_t* list) {
  return list->size;
}

// Junta a lista 'b' no final da lista 'a'
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

  // Esvazia b (usado para juntar buckets da tabela hash)
  b->first = NULL;
  b->last = NULL;
}

// Imprime um único item (valor e frequência)
void printItem(ListNode_t* item) {
  if (item == NULL) {
    printf("Invalid item (null) at printItem.\n");
    return;
  }

  printf("(diff > %i; freq > %i)\n", item->value, item->frequence);
}

// Imprime toda a lista com índice dos nós
void printList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for printList.\n");
    return;
  }

  int i = 1;
  ListNode_t* aux = list->first;
  while (aux != NULL) {
    printf("NODE %d: (diff > %i; freq > %i)\n", i, aux->value, aux->frequence);
    aux = aux->next;
    i++;
  }
  printf("\n");
}

// Libera toda a memória da lista
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
