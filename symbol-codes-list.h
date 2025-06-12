#ifndef SYMBOL_CODES_LIST_H 
#define SYMBOL_CODES_LIST_H

/**
 * Representa um nó da lista de códigos Huffman.
 * Cada nó guarda um valor original e seu código binário correspondente.
 */
typedef struct SymbolCode {
  int value;               ///< Valor original (ex: diferença de pixel ou valor quantizado)
  char *code;              ///< Código binário correspondente (como string)
  int codeSize;            ///< Tamanho do código (em bits)
  struct SymbolCode *next; ///< Próximo elemento na lista
} SymbolCode_t;

/**
 * Lista encadeada de códigos, usada para armazenar os mapeamentos
 * valor → código após a construção da árvore de Huffman.
 */
typedef struct SymbolCodesList {
  int size;                ///< Número de elementos na lista
  SymbolCode_t* first;     ///< Primeiro elemento
  SymbolCode_t* last;      ///< Último elemento
} SymbolCodesList_t;

/**
 * Cria uma nova lista de códigos vazia.
 * 
 * @return Ponteiro para a lista criada.
 */
SymbolCodesList_t* createSymbolCodesList();

/**
 * Insere um novo valor na lista, com código ainda não definido.
 * 
 * @param list Ponteiro para a lista.
 * @param value Valor a ser inserido.
 */
void symbolCodesListInsert(SymbolCodesList_t* list, int value);

/**
 * Busca um valor na lista.
 * 
 * @param list Ponteiro para a lista.
 * @param value Valor a ser buscado.
 * @return Ponteiro para o nó correspondente ou NULL.
 */
SymbolCode_t *symbolCodesListSearch(SymbolCodesList_t *list, int value);

/**
 * Retorna o tamanho da lista.
 * 
 * @param list Ponteiro para a lista.
 * @return Número de elementos.
 */
int getSymbolCodesListSize(SymbolCodesList_t *list);

/**
 * Concatena a lista b no final da lista a.
 * 
 * @param a Lista de destino.
 * @param b Lista a ser anexada.
 */
void concatenateSymbolCodesLists(SymbolCodesList_t *a, SymbolCodesList_t *b);

/**
 * Imprime os dados de um único símbolo.
 * 
 * @param item Ponteiro para o nó.
 */
void printSymbolCodeItem(SymbolCode_t *item);

/**
 * Imprime todos os símbolos da lista.
 * 
 * @param list Ponteiro para a lista.
 */
void printSymbolCodesList(SymbolCodesList_t* list);

/**
 * Libera toda a memória alocada pela lista.
 * 
 * @param list Ponteiro para a lista.
 */
void destroySymbolCodesList(SymbolCodesList_t* list);

#endif
