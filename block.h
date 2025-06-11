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

int ceilDiv(int a, int b);

Blocks_t* createBlocks(int* channel, int width, int height);

IntBlocks_t* createIntBlocks(int totalBlocks);

double** getDctBlock(double** input);

Blocks_t* getDctBlocks(Blocks_t* blocks);

double** getIdctBlock(double** input);

Blocks_t* getIdctBlocks(Blocks_t* blocks);

int** getQuantizedBlock(double** input, int quantizationTable[8][8]);

IntBlocks_t* getQuantizedBlocks(Blocks_t* blocks, int quantizationTable[8][8]);

double** getDequantizedBlock(int** input, int quantizationTable[8][8]);

Blocks_t* getDequantizedBlocks(IntBlocks_t* blocks, int quantizationTable[8][8]);

int* getZigZagArray(int** block);

void destroyZigZagArray(int* arr);

void destroyBlock(double** block);

void destroyBlocks(Blocks_t* blocks);

void destroyIntBlock(int** block);

void destroyIntBlocks(IntBlocks_t* blocks);

#endif
