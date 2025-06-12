#ifndef FILE_WRITER_H 
#define FILE_WRITER_H

#include <stdio.h>
#include "tree.h"
#include "image.h"
#include "block.h"

/**
 * Estrutura usada para escrita de bits em um arquivo binário.
 * Utilizada durante a codificação Huffman.
 */
typedef struct BitWriter {
  FILE *file;              ///< Ponteiro para o arquivo de saída
  unsigned char buffer;    ///< Buffer para acumular os bits
  int bitCount;            ///< Quantidade atual de bits no buffer
} BitWriter;

/**
 * Inicializa o BitWriter com o arquivo fornecido.
 * 
 * @param writer Estrutura BitWriter.
 * @param file Arquivo binário aberto para escrita.
 */
void initBitWriter(BitWriter *writer, FILE *file);

/**
 * Escreve um único bit no arquivo. Armazena no buffer até formar 1 byte.
 * 
 * @param writer Ponteiro para o BitWriter.
 * @param bit Bit a ser escrito (0 ou 1).
 */
void writeBit(BitWriter *writer, int bit);

/**
 * Escreve os bits restantes do buffer no arquivo, completando com zeros.
 * Deve ser chamado ao final da escrita.
 * 
 * @param writer Ponteiro para o BitWriter.
 */
void flushBits(BitWriter *writer);

/**
 * Escreve a árvore de Huffman em forma binária no arquivo.
 * 
 * @param writer Ponteiro para o BitWriter.
 * @param node Raiz da árvore a ser escrita.
 */
void writeHuffmanTree(BitWriter *writer, TreeNode_t *node);

/**
 * Codifica e escreve os dados de imagem usando compressão sem perdas.
 * 
 * @param width Largura da imagem.
 * @param height Altura da imagem.
 * @param tree Árvore de Huffman.
 * @param differences Vetor de diferenças de pixels (RGB).
 * @param codesTable Tabela de códigos Huffman.
 * @param outputFilePath Caminho do arquivo binário de saída.
 */
void writeBinaryFile(int width, int height, Tree_t *tree, Pixel *differences, CodesTable_t* codesTable, char* outputFilePath);

/**
 * Codifica e escreve os dados da imagem usando compressão com perdas (DCT + quantização).
 * 
 * @param width Largura com padding.
 * @param height Altura com padding.
 * @param originalWidth Largura original da imagem.
 * @param originalHeigth Altura original da imagem.
 * @param YTree Árvore de Huffman para canal Y.
 * @param CbTree Árvore de Huffman para canal Cb.
 * @param CrTree Árvore de Huffman para canal Cr.
 * @param YCodesTable Tabela de códigos para canal Y.
 * @param CbCodesTable Tabela de códigos para canal Cb.
 * @param CrCodesTable Tabela de códigos para canal Cr.
 * @param YBlocksQuant Blocos quantizados do canal Y.
 * @param CbBlocksQuant Blocos quantizados do canal Cb.
 * @param CrBlocksQuant Blocos quantizados do canal Cr.
 * @param outputFilePath Caminho do arquivo binário de saída.
 */
void writeLossyBinaryFile(
  int width,
  int height,
  int originalWidth,
  int originalHeigth,
  Tree_t *YTree,
  Tree_t *CbTree,
  Tree_t *CrTree,
  CodesTable_t *YCodesTable,
  CodesTable_t *CbCodesTable,
  CodesTable_t *CrCodesTable,
  IntBlocks_t *YBlocksQuant,
  IntBlocks_t *CbBlocksQuant,
  IntBlocks_t *CrBlocksQuant,
  char* outputFilePath
);

/**
 * Retorna o tamanho (em bytes) de um arquivo.
 * 
 * @param filename Caminho do arquivo.
 * @return Tamanho do arquivo em bytes, ou -1 se erro.
 */
long getFileSize(const char *filename);

/**
 * Calcula a taxa de compressão como porcentagem de redução.
 * 
 * @param originalFileSize Tamanho original (em bytes).
 * @param compressedFileSize Tamanho comprimido (em bytes).
 * @return Percentual de compressão.
 */
float getCompressionRatio(unsigned int originalFileSize, unsigned int compressedFileSize);

#endif
