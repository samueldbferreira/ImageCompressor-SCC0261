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

int hash(int difference) {
  int normalizedIndex = difference + 255;

  int index = normalizedIndex % TABLE_SIZE;

  return index;
}

void tableInsert(HashTable_t *table, int difference) {
  if (table == NULL) {
    printf("Invalid table (null) at tableInsert.\n");
    return;
  }

  if (difference < -255 || difference > 255) {
    printf("Invalid difference at tableInsert.\n");
    return;
  }

  int index = hash(difference);

  List_t *list = table->items[index];

  ListNode_t *item = listSearch(list, difference);

  if (item == NULL) {
    listInsert(list, difference);
  } else {
    incrementFrequence(item);
  }
}

ListNode_t* tableSearch(HashTable_t* table, int difference) {
  int index = hash(difference);

  List_t *list = table->items[index];

  return listSearch(list, difference);
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
