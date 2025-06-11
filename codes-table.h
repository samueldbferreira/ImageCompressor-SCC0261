#ifndef CODES_TABLE_H 
#define CODES_TABLE_H

#define CODES_TABLE_SIZE 1031

#include "symbol-codes-list.h"

typedef struct CodesTable {
  SymbolCodesList_t** items;
} CodesTable_t;

CodesTable_t *createCodesTable();

void tableCodesInsert(CodesTable_t *table, int value);

SymbolCode_t *tableCodesSearch(CodesTable_t *table, int value);

SymbolCodesList_t *getCodesTableItems(CodesTable_t *table);

void destroyCodesTable(CodesTable_t *table);

#endif
