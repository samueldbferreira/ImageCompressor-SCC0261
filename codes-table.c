#include <stdio.h>
#include <stdlib.h>
#include "codes-table.h"
#include "symbol-codes-list.h"

// Cria uma nova tabela de códigos com listas encadeadas (hash table)
CodesTable_t *createCodesTable() {
  CodesTable_t *table = (CodesTable_t *) malloc(sizeof(CodesTable_t));

  table->items = (SymbolCodesList_t **) malloc(CODES_TABLE_SIZE * sizeof(SymbolCodesList_t*));
  for (int i = 0; i < CODES_TABLE_SIZE; i++) {
    table->items[i] = createSymbolCodesList();
  }

  return table;
}

// Função de hash para mapear valores no intervalo da tabela
int codesTableHash(int value) {
  int normalizedIndex = value + 255;
  int index = normalizedIndex % CODES_TABLE_SIZE;
  return index;
}

// Insere um valor na tabela, se ainda não existir
void tableCodesInsert(CodesTable_t *table, int value) {
  if (table == NULL) {
    printf("Invalid table (null) at tableInsert.\n");
    return;
  }

  if (value < -255 || value > 255) {
    printf("Invalid value at tableInsert.\n");
    return;
  }

  int index = codesTableHash(value);

  SymbolCodesList_t *list = table->items[index];

  SymbolCode_t *item = symbolCodesListSearch(list, value);

  if (item == NULL) {
    symbolCodesListInsert(list, value);
  }
}

// Busca um código binário correspondente ao valor fornecido
SymbolCode_t *tableCodesSearch(CodesTable_t *table, int value) {
  int index = codesTableHash(value);
  SymbolCodesList_t *list = table->items[index];
  return symbolCodesListSearch(list, value);
}

// Retorna todos os itens da tabela em uma lista única
SymbolCodesList_t *getCodesTableItems(CodesTable_t *table) {
  if (table == NULL) {
    printf("Invalid table (null) at getCodesTableItems.\n");
    return NULL;
  }

  SymbolCodesList_t *items = createSymbolCodesList();

  for (int i = 0; i < CODES_TABLE_SIZE; i++) {
    int listSize = getSymbolCodesListSize(table->items[i]);
    if (listSize > 0) {
      concatenateSymbolCodesLists(items, table->items[i]);
    }
  }

  return items;
}

// Libera memória da tabela e todas as listas internas
void destroyCodesTable(CodesTable_t *table) {
  if (table == NULL) {
    printf("Invalid table (null) at destroyTable.\n");
    return;
  }

  for (int i = 0; i < CODES_TABLE_SIZE; i++) {
    destroySymbolCodesList(table->items[i]);
  }

  free(table->items);
  free(table);
}
