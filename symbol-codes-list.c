#include <stdio.h>
#include <stdlib.h>
#include "symbol-codes-list.h"

// Cria uma lista encadeada para armazenar símbolos e seus códigos binários
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

// Insere um novo símbolo na lista com valor e código inicialmente vazio
void symbolCodesListInsert(SymbolCodesList_t* list, int value) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.\n");
    return;
  }

  SymbolCode_t* newNode = (SymbolCode_t*) malloc(sizeof(SymbolCode_t));
  newNode->value = value;
  newNode->code = NULL;       // O código será atribuído depois (ex: durante construção da árvore)
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

// Busca um símbolo específico pelo valor
SymbolCode_t *symbolCodesListSearch(SymbolCodesList_t *list, int value) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listSearch.\n");
    return NULL;
  }

  SymbolCode_t* aux = list->first;
  while (aux != NULL) {
    if (aux->value == value) {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

// Retorna o número de elementos na lista
int getSymbolCodesListSize(SymbolCodesList_t *list) {
  return list->size;
}

// Junta os elementos da lista 'b' no final da lista 'a'
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

  // Esvazia a lista 'b' após concatenação
  b->first = NULL;
  b->last = NULL;
}

// Imprime um símbolo individual e seu código, se disponível
void printSymbolCodeItem(SymbolCode_t *item) {
  if (item == NULL) {
    printf("Invalid item (null) at printItem.\n");
    return;
  }

  printf("(diff > %i; codeSize > %i; ", item->value, item->codeSize);
  if (item->codeSize > 0) {
    printf("code > %s)\n", item->code);
  }
}

// Imprime todos os símbolos da lista com seus respectivos códigos
void printSymbolCodesList(SymbolCodesList_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for printList.\n");
    return;
  }

  SymbolCode_t* aux = list->first;
  while (aux != NULL) {
    printf("(diff > %i; codeSize > %i; ", aux->value, aux->codeSize);
    if (aux->codeSize > 0) {
      printf("code > %s)\n", aux->code);
    }
    aux = aux->next;
  }
  printf("\n");
}

// Libera a memória alocada pela lista de códigos
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
