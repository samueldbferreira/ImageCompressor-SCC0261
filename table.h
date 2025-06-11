#ifndef TABLE_H 
#define TABLE_H

#include "list.h"

#define TABLE_SIZE 1031

typedef struct HashTable {
  List_t** items;
} HashTable_t;

HashTable_t *createTable();

void tableInsert(HashTable_t *table, int value);

ListNode_t *tableSearch(HashTable_t *table, int value);

List_t *getItems(HashTable_t *table);

void destroyTable(HashTable_t *table);

#endif
