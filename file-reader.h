#ifndef FILE_READER_H 
#define FILE_READER_H

#include <stdio.h>
#include "tree.h"

/**
 * Estrutura usada para leitura de bits a partir de um arquivo binário.
 * Utilizada na decodificação dos dados compactados.
 */
typedef struct {
  FILE* file;              ///< Ponteiro para o arquivo de entrada
  unsigned char buffer;    ///< Buffer de leitura de 1 byte
  int bitCount;            ///< Número de bits restantes no buffer
  int eof;                 ///< Flag de fim de arquivo
} BitReader;

/**
 * Inicializa o BitReader com o arquivo fornecido.
 * 
 * @param reader Estrutura BitReader.
 * @param file Arquivo binário já aberto para leitura.
 */
void initBitReader(BitReader *reader, FILE *file);

/**
 * Lê um único bit do arquivo. Quando o buffer estiver vazio, um novo byte é lido.
 * 
 * @param reader Ponteiro para o BitReader.
 * @return Valor do bit lido (0 ou 1), ou -1 em caso de erro ou EOF.
 */
int readBit(BitReader *reader);

/**
 * Reconstrói a árvore de Huffman a partir do fluxo binário (compressão sem perdas).
 * 
 * @param reader Ponteiro para o BitReader.
 * @return Raiz da árvore reconstruída.
 */
TreeNode_t *readHuffmanTree(BitReader *reader);

/**
 * Realiza a descompressão de um arquivo sem perdas e gera uma imagem BMP.
 * 
 * @param inputFile Arquivo de entrada (compactado).
 * @param outputFilePath Caminho do arquivo BMP de saída.
 */
void decompressLossless(FILE* inputFile, char* outputFilePath);

/**
 * Realiza a descompressão de um arquivo com perdas (DCT + quantização) e gera BMP.
 * 
 * @param inputFile Arquivo de entrada (compactado).
 * @param outputFilePath Caminho do arquivo BMP de saída.
 */
void decompressLossy(FILE* inputFile, char* outputFilePath);

#endif
