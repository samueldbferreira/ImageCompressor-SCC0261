#ifndef TABLE_H 
#define TABLE_H

#include "list.h"

#define TABLE_SIZE 1031  ///< Tamanho da tabela de hash (número primo para melhor dispersão)

/**
 * Estrutura que representa uma tabela de hash.
 * Cada posição da tabela contém uma lista encadeada (tratamento de colisões).
 * Usada para armazenar frequências de valores (como diferenças de pixel ou valores quantizados).
 */
typedef struct HashTable {
  List_t** items;
} HashTable_t;

/**
 * Cria uma nova tabela de hash com listas vazias.
 * 
 * @return Ponteiro para a tabela criada.
 */
HashTable_t *createTable();

/**
 * Insere ou atualiza a frequência de um valor na tabela.
 * 
 * @param table Ponteiro para a tabela.
 * @param value Valor a ser inserido ou atualizado.
 */
void tableInsert(HashTable_t *table, int value);

/**
 * Busca um valor na tabela.
 * 
 * @param table Ponteiro para a tabela.
 * @param value Valor procurado.
 * @return Ponteiro para o nó correspondente ou NULL.
 */
ListNode_t *tableSearch(HashTable_t *table, int value);

/**
 * Concatena todas as listas da tabela em uma única lista.
 * 
 * @param table Ponteiro para a tabela.
 * @return Ponteiro para lista com todos os nós da tabela.
 */
List_t *getItems(HashTable_t *table);

/**
 * Libera toda a memória da tabela e de suas listas internas.
 * 
 * @param table Ponteiro para a tabela a ser destruída.
 */
void destroyTable(HashTable_t *table);

#endif
