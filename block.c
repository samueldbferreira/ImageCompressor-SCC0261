#include <stdio.h>
#include <stdlib.h>
#include "block.h"

Blocks_t* createBlocks(int* channel, int width, int height) {
  Blocks_t* blocks = (Blocks_t*) malloc(sizeof(Blocks_t));
  if (blocks == NULL) {
    printf("Memory allocation failed for blocks\n");
    return NULL;
  }

  int blocksPerRow = width / BLOCK_SIZE;
  int blocksPerCol = height / BLOCK_SIZE;
  int totalBlocks = blocksPerRow * blocksPerCol;

  blocks->data = (float***) malloc(totalBlocks * sizeof(float**));
  if (blocks->data == NULL) {
    printf("Memory allocation failed for blocks->data\n");
    return NULL;
  }

  for (int i = 0; i < totalBlocks; i++) {
    blocks->data[i] = (float**) malloc(BLOCK_SIZE * sizeof(float*));
    for (int j = 0; j < BLOCK_SIZE; j++) {
      blocks->data[i][j] = (float*) malloc(BLOCK_SIZE * sizeof(float));
      if (blocks->data[i][j] == NULL) {
        printf("Memory allocation failed for blocks->data[%d][%d]\n", i, j);
        return NULL;
      }
    }
  }

  for (int i = 0; i <= height - BLOCK_SIZE; i += BLOCK_SIZE) {
    for (int j = 0; j <= width - BLOCK_SIZE; j += BLOCK_SIZE) {
      int blockIndex = (i / BLOCK_SIZE) * blocksPerRow + (j / BLOCK_SIZE);

      for (int bi = 0; bi < BLOCK_SIZE; bi++) {
        for (int bj = 0; bj < BLOCK_SIZE; bj++) {
          int pixelIndex = (i + bi) * width + (j + bj);
          blocks->data[blockIndex][bi][bj] = (float)channel[pixelIndex];
        }
      }
    }
  }

  blocks->totalBlocks = totalBlocks;

  return blocks;
}

void destroyBlocks(Blocks_t* blocks, int width, int height) {
  int blocksPerRow = width / BLOCK_SIZE;
  int blocksPerCol = height / BLOCK_SIZE;
  int totalBlocks = blocksPerRow * blocksPerCol;

  for (int i = 0; i < totalBlocks; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      free(blocks->data[i][j]);
    }
    free(blocks->data[i]);
  }
  free(blocks->data);
  free(blocks);
}