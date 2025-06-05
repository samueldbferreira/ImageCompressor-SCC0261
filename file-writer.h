#ifndef FILE_WRITER_H 
#define FILE_WRITER_H

#include <stdio.h>
#include "tree.h"
#include "image.h"

typedef struct BitWriter {
  FILE *file;
  unsigned char buffer;
  int bitCount;
} BitWriter;

void initBitWriter(BitWriter *writer, FILE *file);

void writeBit(BitWriter *writer, int bit);

void flushBits(BitWriter *writer);

void writeHuffmanTree(BitWriter *writer, TreeNode_t *node);

void writeBinaryFile(int width, int height, Tree_t *tree, Pixel *differences, CodesTable_t* codesTable, char* outputFilePath);

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
  char* outputFilePath
);

long getFileSize(const char *filename);

float getCompressionRatio(unsigned int originalFileSize, unsigned int compressedFileSize);

#endif
