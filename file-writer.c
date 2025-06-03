#include <stdio.h>
#include <stdlib.h>
#include "file-writer.h"

void initBitWriter(BitWriter *writer, FILE *file) {
  writer->file = file;
  writer->buffer = 0;
  writer->bitCount = 0;
}

void writeBit(BitWriter *writer, int bit) {
  writer->buffer = (writer->buffer << 1) | (bit & 1);
  writer->bitCount++;
    
  if (writer->bitCount == 8) {
    fwrite(&writer->buffer, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

void flushBits(BitWriter *writer) {
  if (writer->bitCount > 0) {
    writer->buffer <<= (8 - writer->bitCount);
    fwrite(&writer->buffer, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

void writeHuffmanTree(BitWriter *writer, TreeNode_t *node) {
  if (node == NULL) {
    writeBit(writer, 0); // Marca nó nulo
    return;
  }
  
  writeBit(writer, 1); // Marca que existe nó
  
  if (node->childLeft == NULL && node->childRight == NULL) {
    writeBit(writer, 1); // Marca como folha
    // Escreve a diferença (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->difference >> i) & 1);
    }

    // Escreve a frequência (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->frequence >> i) & 1);
    }
  } else {
    writeBit(writer, 0); // Marca como nó interno
    // Escreve a diferença (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->difference >> i) & 1);
    }

    // Escreve a frequência (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->frequence >> i) & 1);
    }
    writeHuffmanTree(writer, node->childLeft);
    writeHuffmanTree(writer, node->childRight);
  }
}

void writeBinaryFile(int width, int height, int numSymbols, Tree_t *tree) { 
  FILE* file = fopen("./output.bin", "wb");
  if (!file) {
    printf("Error opening output file.\n");
    return;
  }

  fwrite(&height, sizeof(int), 1, file);
  fwrite(&width, sizeof(int), 1, file);
  fwrite(&numSymbols, sizeof(int), 1, file);
  fflush(file);

  BitWriter writer;
  initBitWriter(&writer, file);
  writeHuffmanTree(&writer, tree->root);
  flushBits(&writer);

  fclose(file);
}
