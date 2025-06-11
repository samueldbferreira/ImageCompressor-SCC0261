#ifndef SYMBOL_CODES_LIST_H 
#define SYMBOL_CODES_LIST_H

typedef struct SymbolCode {
  int value;
  char *code;
  int codeSize;
  struct SymbolCode *next;
} SymbolCode_t;

typedef struct SymbolCodesList {
  int size;
  SymbolCode_t* first;
  SymbolCode_t* last;
} SymbolCodesList_t;

SymbolCodesList_t* createSymbolCodesList();

void symbolCodesListInsert(SymbolCodesList_t* list, int value);

SymbolCode_t *symbolCodesListSearch(SymbolCodesList_t *list, int value);

int getSymbolCodesListSize(SymbolCodesList_t *list);

void concatenateSymbolCodesLists(SymbolCodesList_t *a, SymbolCodesList_t *b);

void printSymbolCodeItem(SymbolCode_t *item);

void printSymbolCodesList(SymbolCodesList_t* list);

void destroySymbolCodesList(SymbolCodesList_t* list);

#endif
