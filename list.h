#ifndef LIST_H 
#define LIST_H

typedef struct ListNode {
  int difference;
  int frequence;
  struct ListNode* next;
} ListNode_t;

typedef struct List {
  int size;
  ListNode_t* first;
  ListNode_t* last;
} List_t;

List_t* createList();

void listInsert(List_t* list, int difference);

ListNode_t *listSearch(List_t *list, int difference);

void incrementFrequence(ListNode_t *item);

int getListSize(List_t *list);

void concatenateLists(List_t *a, List_t *b);

void printItem(ListNode_t *item);

void printList(List_t* list);

void destroyList(List_t* list);

#endif
