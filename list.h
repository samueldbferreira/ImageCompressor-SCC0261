#ifndef LIST_H 
#define LIST_H

/**
 * Nó de uma lista encadeada que armazena:
 * - Um valor inteiro (ex: diferença de pixels ou valor quantizado)
 * - Sua frequência de ocorrência
 * - Ponteiro para o próximo nó
 */
typedef struct ListNode {
  int value;
  int frequence;
  struct ListNode* next;
} ListNode_t;

/**
 * Lista encadeada com ponteiros para o primeiro e último nó
 * e contador de tamanho. Usada como base para tabela de hash e códigos.
 */
typedef struct List {
  int size;
  ListNode_t* first;
  ListNode_t* last;
} List_t;

/**
 * Cria uma lista vazia.
 * 
 * @return Ponteiro para nova lista alocada dinamicamente.
 */
List_t* createList();

/**
 * Insere um novo nó com valor e frequência 1 no final da lista.
 * 
 * @param list Ponteiro para a lista.
 * @param value Valor a ser inserido.
 */
void listInsert(List_t* list, int value);

/**
 * Busca um nó com valor específico na lista.
 * 
 * @param list Lista onde será feita a busca.
 * @param value Valor procurado.
 * @return Ponteiro para o nó encontrado ou NULL.
 */
ListNode_t *listSearch(List_t *list, int value);

/**
 * Incrementa a frequência de um nó existente.
 * 
 * @param item Nó cuja frequência será incrementada.
 */
void incrementFrequence(ListNode_t *item);

/**
 * Retorna o número de elementos na lista.
 * 
 * @param list Ponteiro para a lista.
 * @return Tamanho da lista.
 */
int getListSize(List_t *list);

/**
 * Concatena a lista b no final da lista a.
 * 
 * @param a Lista destino (será modificada).
 * @param b Lista origem (será esvaziada).
 */
void concatenateLists(List_t *a, List_t *b);

/**
 * Imprime os dados de um único nó.
 * 
 * @param item Ponteiro para o nó a ser impresso.
 */
void printItem(ListNode_t *item);

/**
 * Imprime toda a lista (índice, valor e frequência).
 * 
 * @param list Ponteiro para a lista.
 */
void printList(List_t* list);

/**
 * Libera toda a memória alocada pela lista.
 * 
 * @param list Ponteiro para a lista a ser destruída.
 */
void destroyList(List_t* list);

#endif
