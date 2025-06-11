#ifndef FILE_READER_H 
#define FILE_READER_H

#include <stdio.h>
#include "tree.h"

typedef struct {
  FILE* file;
  unsigned char buffer;
  int bitCount;
  int eof;
} BitReader;

int readBit(BitReader *reader);

void initBitReader(BitReader *reader, FILE *file);

TreeNode_t *readHuffmanTree(BitReader *reader);

void decompressLossless(FILE* inputFile, char* outputFilePath);

TreeNode_t* readHuffmanTreeLossy(BitReader *reader);

void decompressLossy(FILE* inputFile, char* outputFilePath);

#endif
