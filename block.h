#ifndef BLOCK_H 
#define BLOCK_H

#define BLOCK_SIZE 8

typedef struct Blocks {
  double*** data;
  int totalBlocks;
} Blocks_t;

typedef struct IntBlocks {
  int*** data;
  int totalBlocks;
} IntBlocks_t;

double** createBlock();

int** createIntBlock();

Blocks_t* createBlocks(int* channel, int width, int height);

double** getDctBlock(double** input);

Blocks_t* getDctBlocks(Blocks_t* blocks);

double** getIdctBlock(double** input);

int** getQuantizedBlock(double** input, int quantizationTable[8][8]);

IntBlocks_t* getQuantizedBlocks(Blocks_t* blocks, int quantizationTable[8][8]);

double** getDequantizedBlock(double** input, int quantizationTable[8][8]);

int* getZigZagArray(int** block);

void destroyZigZagArray(int* arr);

void destroyBlock(double** block);

void destroyBlocks(Blocks_t* blocks, int width, int height);

void destroyIntBlock(int** block);

void destroyIntBlocks(IntBlocks_t* blocks, int width, int height);

#endif
