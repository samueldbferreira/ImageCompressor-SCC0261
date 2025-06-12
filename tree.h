#ifndef TREE_H 
#define TREE_H

#include "list.h"
#include "table.h"
#include "codes-table.h"

#define EMPTY_DIFFERENCE 999;  ///< Valor fictício usado em nós internos da árvore de Huffman

/**
 * Nó da árvore de Huffman, contendo valor, frequência e filhos esquerdo e direito.
 */
typedef struct TreeNode {
  int value;                    ///< Valor original (ou EMPTY_DIFFERENCE se for nó interno)
  int frequence;                ///< Frequência de ocorrência do valor
  struct TreeNode* childLeft;  ///< Filho esquerdo
  struct TreeNode* childRight; ///< Filho direito
} TreeNode_t;

/**
 * Estrutura que representa uma árvore de Huffman, contendo ponteiro para o nó raiz.
 */
typedef struct Tree {
  TreeNode_t* root;
} Tree_t;

/**
 * Cria uma árvore com um único nó (folha).
 * 
 * @param value Valor armazenado.
 * @param frequence Frequência associada.
 * @return Ponteiro para a árvore criada.
 */
Tree_t* createTree(int value, int frequence);

/**
 * Cria a árvore de Huffman a partir de uma lista de frequências.
 * 
 * @param list Lista contendo valores e frequências.
 * @return Ponteiro para a árvore construída.
 */
Tree_t* createTreeFromList(List_t* list);

/**
 * Mescla duas árvores em uma nova árvore com nó raiz contendo a soma das frequências.
 * 
 * @param a Primeira árvore.
 * @param b Segunda árvore.
 * @return Nova árvore resultante da fusão.
 */
Tree_t* mergeTrees(Tree_t* a, Tree_t* b);

/**
 * Gera a tabela de códigos Huffman (valor → código binário) a partir da árvore.
 * 
 * @param frequencesTable Tabela de frequências usada para gerar os códigos.
 * @param items Lista com os valores únicos.
 * @param tree Árvore de Huffman construída.
 * @return Ponteiro para a tabela de códigos criada.
 */
CodesTable_t* generateCodesTable(HashTable_t* frequencesTable, List_t* items, Tree_t* tree);

/**
 * Escreve a árvore de Huffman em um arquivo, de forma serializada (pré-ordem).
 * 
 * @param root Ponteiro para a árvore.
 * @param file Ponteiro para o arquivo aberto em modo binário.
 */
void writeTreeToFile(Tree_t *root, FILE *file);

/**
 * Imprime a árvore de Huffman no terminal (formato de depuração).
 * 
 * @param tree Ponteiro para a árvore.
 */
void printTree(Tree_t* tree);

/**
 * Libera recursivamente todos os nós da árvore (função auxiliar).
 * 
 * @param root Ponteiro para o nó raiz.
 */
void destroyTreeUtil(TreeNode_t *root);

/**
 * Libera toda a memória da árvore.
 * 
 * @param tree Ponteiro para a árvore.
 */
void destroyTree(Tree_t* tree);

#endif
