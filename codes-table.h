#ifndef CODES_TABLE_H 
#define CODES_TABLE_H

#define CODES_TABLE_SIZE 1031  ///< Tamanho da tabela de códigos (número primo para melhor dispersão)

#include "symbol-codes-list.h"

/**
 * Tabela de códigos Huffman organizada como hash table.
 * Cada posição contém uma lista encadeada de códigos.
 * Usada para mapeamento valor → código de forma eficiente.
 */
typedef struct CodesTable {
  SymbolCodesList_t** items;
} CodesTable_t;

/**
 * Cria uma nova tabela de códigos vazia.
 * 
 * @return Ponteiro para a tabela criada.
 */
CodesTable_t *createCodesTable();

/**
 * Insere um valor na tabela (sem código inicial).
 * 
 * @param table Ponteiro para a tabela.
 * @param value Valor a ser inserido.
 */
void tableCodesInsert(CodesTable_t *table, int value);

/**
 * Busca um valor na tabela e retorna o código correspondente.
 * 
 * @param table Ponteiro para a tabela.
 * @param value Valor a ser buscado.
 * @return Ponteiro para o nó contendo o código ou NULL.
 */
SymbolCode_t *tableCodesSearch(CodesTable_t *table, int value);

/**
 * Concatena todas as listas da tabela em uma única lista.
 * 
 * @param table Ponteiro para a tabela.
 * @return Lista com todos os símbolos e códigos.
 */
SymbolCodesList_t *getCodesTableItems(CodesTable_t *table);

/**
 * Libera toda a memória da tabela de códigos.
 * 
 * @param table Ponteiro para a tabela.
 */
void destroyCodesTable(CodesTable_t *table);

#endif
