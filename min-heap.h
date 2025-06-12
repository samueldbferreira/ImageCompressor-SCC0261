#ifndef MIN_HEAP_H 
#define MIN_HEAP_H

#include "tree.h"

#define MAX_HEAP_SIZE 512  ///< Capacidade máxima da heap mínima

/**
 * Estrutura de heap mínima utilizada para construir a árvore de Huffman.
 * Armazena ponteiros para árvores ordenadas pela frequência.
 */
typedef struct MinHeap {
  Tree_t** arr;   ///< Vetor de ponteiros para árvores
  int size;       ///< Número atual de elementos na heap
  int capacity;   ///< Capacidade máxima (MAX_HEAP_SIZE)
} MinHeap_t;

/**
 * Cria uma heap mínima vazia.
 * 
 * @return Ponteiro para a heap criada.
 */
MinHeap_t *createHeap();

/**
 * Insere uma árvore na heap, mantendo a propriedade de ordenação.
 * 
 * @param heap Ponteiro para a heap.
 * @param tree Ponteiro para a árvore a ser inserida.
 */
void heapInsert(MinHeap_t *heap, Tree_t *tree);

/**
 * Remove e retorna a árvore com menor frequência da heap.
 * 
 * @param heap Ponteiro para a heap.
 * @return Ponteiro para a árvore removida.
 */
Tree_t *extractMin(MinHeap_t *heap);

/**
 * Imprime o conteúdo da heap (debug).
 * 
 * @param heap Ponteiro para a heap.
 */
void printHeap(MinHeap_t *heap);

/**
 * Retorna o número de elementos na heap.
 * 
 * @param heap Ponteiro para a heap.
 * @return Quantidade atual de elementos.
 */
int heapSize(MinHeap_t *heap);

/**
 * Libera toda a memória alocada pela heap e destrói as árvores contidas nela.
 * 
 * @param heap Ponteiro para a heap.
 */
void destroyHeap(MinHeap_t *heap);

#endif
