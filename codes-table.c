#include <stdio.h>
#include <stdlib.h>
#include "codes-table.h"
#include "symbol-codes-list.h"

CodesTable_t *createCodesTable() {
  CodesTable_t *table = (CodesTable_t *) malloc(sizeof(CodesTable_t));

  table->items = (SymbolCodesList_t **) malloc(CODES_TABLE_SIZE * sizeof(SymbolCodesList_t*));
  for (int i = 0; i < CODES_TABLE_SIZE; i++) {
    table->items[i] = createSymbolCodesList();
  }

  return table;
}

int codesTableHash(int value) {
  int normalizedIndex = value + 255;

  int index = normalizedIndex % CODES_TABLE_SIZE;

  return index;
}

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

SymbolCode_t *tableCodesSearch(CodesTable_t *table, int value) {
  int index = codesTableHash(value);

  SymbolCodesList_t *list = table->items[index];

  return symbolCodesListSearch(list, value);
}

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