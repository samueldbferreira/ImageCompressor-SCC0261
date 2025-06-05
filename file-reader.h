#ifndef FILE_READER_H 
#define FILE_READER_H

#include <stdio.h>
#include "tree.h"

typedef struct BitReader {
  FILE *file;
  unsigned char buffer;
  int bitCount;
} BitReader;

int readBit(BitReader *reader);

void initBitReader(BitReader *reader, FILE *file);

TreeNode_t *readHuffmanTree(BitReader *reader);

void readBinary(char* filePath);

void readLossyBinary(char* filePath);

#endif
