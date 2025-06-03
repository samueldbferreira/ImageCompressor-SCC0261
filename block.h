#ifndef BLOCK_H 
#define BLOCK_H

#define BLOCK_SIZE 8

typedef struct Blocks {
  float*** data;
  int totalBlocks;
} Blocks_t;

Blocks_t* createBlocks(int* channel, int width, int height);

void destroyBlocks(Blocks_t* blocks, int width, int height);

#endif
