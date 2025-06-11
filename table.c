#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "table.h"

HashTable_t *createTable() {
  HashTable_t *table = (HashTable_t *) malloc(sizeof(HashTable_t));

  table->items = (List_t **) malloc(TABLE_SIZE * sizeof(List_t*));
  for (int i = 0; i < TABLE_SIZE; i++) {
    table->items[i] = createList();
  }

  return table;
}

int hash(int value) {
  int normalizedIndex = value + 255;

  int index = normalizedIndex % TABLE_SIZE;

  return index;
}

void tableInsert(HashTable_t *table, int value) {
  if (table == NULL) {
    printf("Invalid table (null) at tableInsert.\n");
    return;
  }

  if (value < -255 || value > 255) {
    printf("Invalid value at tableInsert.\n");
    return;
  }

  int index = hash(value);

  List_t *list = table->items[index];

  ListNode_t *item = listSearch(list, value);

  if (item == NULL) {
    listInsert(list, value);
  } else {
    incrementFrequence(item);
  }
}

ListNode_t* tableSearch(HashTable_t* table, int value) {
  int index = hash(value);

  List_t *list = table->items[index];

  return listSearch(list, value);
}

List_t* getItems(HashTable_t* table) {
  if (table == NULL) {
    printf("Invalid table (null) at getItems.\n");
    return NULL;
  }

  List_t *items = createList();

  for (int i = 0; i < TABLE_SIZE; i++) {
    int listSize = getListSize(table->items[i]);
    if (listSize > 0) {
      concatenateLists(items, table->items[i]);
    }
  }

  return items;
}

void destroyTable(HashTable_t* table) {
  if (table == NULL) {
    printf("Invalid table (null) at destroyTable.\n");
    return;
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    destroyList(table->items[i]);
  }
  free(table->items);
  free(table);
}
