#ifndef BLOCK_H 
#define BLOCK_H

#define BLOCK_SIZE 8

typedef struct Blocks {
  double*** data;
  int totalBlocks;
} Blocks_t;

double** createBlock();

Blocks_t* createBlocks(int* channel, int width, int height);

double** getDctBlock(double** input);

double** getIdctBlock(double** input);

double** getQuantizedBlock(double** input, int quantizationTable[8][8]);

double** getDequantizedBlock(double** input, int quantizationTable[8][8]);

int* getZigZagArray(double** block);

void destroyZigZagArray(int* arr);

void destroyBlock(double** block);

void destroyBlocks(Blocks_t* blocks, int width, int height);

#endif
