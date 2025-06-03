#ifndef FILE_WRITER_H 
#define FILE_WRITER_H

#include <stdio.h>
#include "tree.h"

typedef struct BitWriter {
  FILE *file;
  unsigned char buffer;
  int bitCount;
} BitWriter;

void initBitWriter(BitWriter *writer, FILE *file);

void writeBit(BitWriter *writer, int bit);

void flushBits(BitWriter *writer);

void writeHuffmanTree(BitWriter *writer, TreeNode_t *node);

void writeBinaryFile(int width, int height, int numSymbols, Tree_t *tree);

#endif
