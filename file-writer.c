#include <stdio.h>
#include <stdlib.h>
#include "file-writer.h"
#include "image.h"
#include "block.h"
#include <stdint.h>

// Inicializa a estrutura do gravador de bits (BitWriter)
void initBitWriter(BitWriter *writer, FILE *file) {
  writer->file = file;
  writer->buffer = 0;
  writer->bitCount = 0;
}

// Escreve um único bit no buffer. Quando acumula 8 bits, grava 1 byte no arquivo
void writeBit(BitWriter *writer, int bit) {
  writer->buffer = (writer->buffer << 1) | (bit & 1);
  writer->bitCount++;
    
  if (writer->bitCount == 8) {
    fwrite(&writer->buffer, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

// Escreve um inteiro como 32 bits
void writeIntAsBits(BitWriter* writer, int value) {
  for (int i = 0; i < 32; i++) {
    writeBit(writer, (value >> (31 - i)) & 1);
  }
}

// Esvazia o buffer final, preenchendo com zeros à direita
void flushBits(BitWriter *writer) {
  if (writer->bitCount > 0) {
    uint8_t aligned = writer->buffer << (8 - writer->bitCount);
    fwrite(&aligned, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

// Escreve a árvore de Huffman binária no arquivo de forma recursiva
void writeHuffmanTree(BitWriter *writer, TreeNode_t *node) {
  if (node == NULL) {
    writeBit(writer, 0); // Marca nó nulo
    return;
  }
  
  writeBit(writer, 1); // Marca existência do nó
  
  if (node->childLeft == NULL && node->childRight == NULL) {
    writeBit(writer, 1); // É folha
    // Escreve valor (32 bits)
    writeIntAsBits(writer, node->value);
  } else {
    writeBit(writer, 0); // Nó interno
    // Escreve valor (32 bits)
    writeIntAsBits(writer, node->value);

    writeHuffmanTree(writer, node->childLeft);
    writeHuffmanTree(writer, node->childRight);
  }
}

// Codifica e grava imagem comprimida sem perdas
void writeBinaryFile(int width, int height, Tree_t *tree, Pixel *differences, CodesTable_t* codesTable, char* outputFilePath) { 
  FILE* file = fopen(outputFilePath, "wb");
  if (!file) {
    printf("Error opening output file.\n");
    return;
  }

  // Grava header
  int compressionType = 0;
  fwrite(&compressionType, sizeof(int), 1, file);
  fwrite(&height, sizeof(int), 1, file);
  fwrite(&width, sizeof(int), 1, file);

  // Grava árvore de Huffman
  BitWriter writer;
  initBitWriter(&writer, file);
  writeHuffmanTree(&writer, tree->root);

  // Grava primeiro pixel bruto (B, G, R)
  writeIntAsBits(&writer, differences[0].B);
  writeIntAsBits(&writer, differences[0].G);
  writeIntAsBits(&writer, differences[0].R);

  // Grava todos os pixels subsequentes como diferenças codificadas
  for (int i = 1; i < (width * height); i++) {
    SymbolCode_t* B = tableCodesSearch(codesTable, differences[i].B);
    for (int j = 0; j < B->codeSize; j++) {
      writeBit(&writer, B->code[j] - '0');
    }

    SymbolCode_t* G = tableCodesSearch(codesTable, differences[i].G);
    for (int j = 0; j < G->codeSize; j++) {
      writeBit(&writer, G->code[j] - '0');
    }

    SymbolCode_t* R = tableCodesSearch(codesTable, differences[i].R);
    for (int j = 0; j < R->codeSize; j++) {
      writeBit(&writer, R->code[j] - '0');
    }
  }

  flushBits(&writer);
  fclose(file);
}

// Grava imagem comprimida com perdas, com quantização e árvore por canal
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
) {
  FILE* file = fopen(outputFilePath, "wb");
  if (!file) {
    printf("Error opening output file.\n");
    return;
  }

  // Grava header com informações da imagem original e com padding
  int compressionType = 1;
  fwrite(&compressionType, sizeof(int), 1, file);
  fwrite(&width, sizeof(int), 1, file);
  fwrite(&height, sizeof(int), 1, file);
  fwrite(&originalWidth, sizeof(int), 1, file);
  fwrite(&originalHeigth, sizeof(int), 1, file);

  BitWriter writer;
  initBitWriter(&writer, file);

  // Para cada canal (Y, Cb, Cr):
  for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
    Tree_t* tree;
    IntBlocks_t* blocks;
    CodesTable_t* codesTable;

    if (channelIndex == 0) {
      tree = YTree;
      blocks = YBlocksQuant;
      codesTable = YCodesTable;
    } else if (channelIndex == 1) {
      tree = CbTree;
      blocks = CbBlocksQuant;
      codesTable = CbCodesTable;
    } else {
      tree = CrTree;
      blocks = CrBlocksQuant;
      codesTable = CrCodesTable;
    }

    //   Grava árvore de Huffman
    writeHuffmanTree(&writer, tree->root);

    //   Codifica e grava coeficientes quantizados dos blocos
    //   Código binarios sao vindo da tabela de códigos
    for (int blockIndex = 0; blockIndex < blocks->totalBlocks; blockIndex++) {
      int** block = blocks->data[blockIndex];
      for (int x = 0; x < BLOCK_SIZE; x++) {
        for (int y = 0; y < BLOCK_SIZE; y++) {
          SymbolCode_t* symbolCode = tableCodesSearch(codesTable, block[x][y]);
          for (int symbolIndex = 0; symbolIndex < symbolCode->codeSize; symbolIndex++) {
            int bit = symbolCode->code[symbolIndex] - '0';
            writeBit(&writer, bit);
          }
        }
      }
    }
  }

  flushBits(&writer);
  
  fclose(file);
}

// Retorna o tamanho de um arquivo (em bytes)
long getFileSize(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    perror("Erro ao abrir arquivo");
    return -1;
  }

  fseek(fp, 0, SEEK_END);    // Vai para o fim do arquivo
  long size = ftell(fp);     // Pega a posição atual (tamanho)
  fclose(fp);
  return size;
}

// Calcula a razão de compressão em porcentagem
float getCompressionRatio(unsigned int originalFileSize, unsigned int compressedFileSize) {
  if (originalFileSize == 0) {
    return -1.0f;
  }

  float ratio = (1.0f - ((float)compressedFileSize / originalFileSize)) * 100.0f;
  return ratio;
}
