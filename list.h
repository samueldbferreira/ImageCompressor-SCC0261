typedef struct ListNode {
  int difference;
  int frequence;
  struct ListNode* next;
} ListNode_t;


typedef struct List {
  ListNode_t* first;
  ListNode_t* last;
} List_t;

List_t *createList();

void listInsert(List_t *list, int difference);

void printList(List_t *list);

void destroyList(List_t *list);

