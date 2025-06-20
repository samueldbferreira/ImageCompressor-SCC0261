#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "block.h"

#define PI 3.14159265358979323846

// Cria e aloca dinamicamente um bloco 8x8 de doubles
double** createBlock() {
  double** block = (double**) malloc(BLOCK_SIZE * sizeof(double*));
  for (int i = 0; i < BLOCK_SIZE; i++) {
    block[i] = (double*) malloc(BLOCK_SIZE * sizeof(double));
  }

  return block;
}

// Cria e aloca dinamicamente um bloco 8x8 de inteiros
int** createIntBlock() {
  int** block = (int**) malloc(BLOCK_SIZE * sizeof(int*));
  for (int i = 0; i < BLOCK_SIZE; i++) {
    block[i] = (int*) malloc(BLOCK_SIZE * sizeof(int));
  }

  return block;
}

// Divisão com arredondamento para cima
int ceilDiv(int a, int b) {
  return (a + b - 1) / b;
}

// Divide um canal de pixels em blocos 8x8 armazenados como matrizes de double
Blocks_t* createBlocks(int* channel, int width, int height) {
  Blocks_t* blocks = (Blocks_t*) malloc(sizeof(Blocks_t));
  if (blocks == NULL) {
    printf("Memory allocation failed for blocks\n");
    return NULL;
  }

  int blocksPerRow = width / BLOCK_SIZE;
  int blocksPerCol = height / BLOCK_SIZE;
  int totalBlocks = blocksPerRow * blocksPerCol;

  blocks->data = (double***) malloc(totalBlocks * sizeof(double**));
  if (blocks->data == NULL) {
    printf("Memory allocation failed for blocks->data\n");
    return NULL;
  }

  // Aloca os blocos
  for (int i = 0; i < totalBlocks; i++) {
    blocks->data[i] = createBlock();
  }

  // Copia os valores do canal para os blocos 8x8 correspondentes
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

// Cria estrutura para blocos inteiros a partir de um total conhecido
IntBlocks_t* createIntBlocks(int totalBlocks) {
  IntBlocks_t* blocks = (IntBlocks_t*) malloc(sizeof(IntBlocks_t));
  if (blocks == NULL) {
    printf("Memory allocation failed for int blocks\n");
    return NULL;
  }

  blocks->totalBlocks = totalBlocks;
  blocks->data = (int***) malloc(totalBlocks * sizeof(int**));
  for (int i = 0; i < totalBlocks; i++) {
    blocks->data[i] = createIntBlock();
  }

  return blocks;
}

// Aplica DCT em um único bloco 8x8
double** getDctBlock(double** input) {
  double sum;
  static double cosCache[BLOCK_SIZE][BLOCK_SIZE];
  static int first_run = 1;

  // Constantes de normalização
  double c[BLOCK_SIZE];
  for (int i = 0; i < BLOCK_SIZE; i++) {
    c[i] = (i == 0) ? sqrt(1.0 / 2.0) : 1.0;
  }
  
  // Cache de cossenos para acelerar a computação
  if (first_run) {
    first_run = 0;
    for (int x = 0; x < BLOCK_SIZE; x++) {
      for (int i = 0; i < BLOCK_SIZE; i++) {
        cosCache[x][i] = cos((2 * x + 1) * i * PI / 16.0);
      }
    }
  }
  
  double** output = createBlock();

  // Cálculo da DCT 2D
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      sum = 0.0;
      
      for (int x = 0; x < BLOCK_SIZE; x++) {
        double cos_i = cosCache[x][i];
        for (int y = 0; y < BLOCK_SIZE; y++) {
          sum += input[x][y] * cos_i * cosCache[y][j];
        }
      }
      
      output[i][j] = 0.25 * c[i] * c[j] * sum;
    }
  }

  return output;
}

// Aplica DCT em todos os blocos de uma imagem
Blocks_t* getDctBlocks(Blocks_t* blocks) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) on getDctBlocks.");
    return NULL;
  }

  Blocks_t* dctBlocks = (Blocks_t*) malloc(sizeof(Blocks_t));
  if (dctBlocks == NULL) {
    printf("Memory allocation failed for dctBlocks\n");
    return NULL;
  }

  dctBlocks->data = (double***) malloc(blocks->totalBlocks * sizeof(double**));
  if (dctBlocks->data == NULL) {
    printf("Memory allocation failed for dctBlocks->data\n");
    free(dctBlocks);
    return NULL;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    dctBlocks->data[i] = getDctBlock(blocks->data[i]);
  }

  dctBlocks->totalBlocks = blocks->totalBlocks;

  return dctBlocks;
}

// Aplica IDCT em um único bloco 8x8
double** getIdctBlock(double** input) {
  double sum;
  static double cosCache[BLOCK_SIZE][BLOCK_SIZE];
  static int first_run = 1;

  double c[BLOCK_SIZE];
  for (int i = 0; i < BLOCK_SIZE; i++) {
    c[i] = (i == 0) ? sqrt(1.0 / 2.0) : 1.0;
  }

  if (first_run) {
    first_run = 0;
    for (int x = 0; x < BLOCK_SIZE; x++) {
      for (int i = 0; i < BLOCK_SIZE; i++) {
        cosCache[x][i] = cos((2 * x + 1) * i * PI / 16.0);
      }
    }
  }

  double** output = createBlock();

  // Cálculo da IDCT 2D
  for (int x = 0; x < BLOCK_SIZE; x++) {
    for (int y = 0; y < BLOCK_SIZE; y++) {
      sum = 0.0;
      
      for (int i = 0; i < BLOCK_SIZE; i++) {
        double ci = c[i];
        double cos_xi = cosCache[x][i];
        for (int j = 0; j < BLOCK_SIZE; j++) {
          sum += ci * c[j] * input[i][j] * cos_xi * cosCache[y][j];
        }
      }

      output[x][y] = 0.25 * sum;
    }
  }

  return output;
}

// Aplica IDCT em todos os blocos da imagem
Blocks_t* getIdctBlocks(Blocks_t* blocks) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) on getIdctBlocks.");
    return NULL;
  }

  Blocks_t* idctBlocks = (Blocks_t*) malloc(sizeof(Blocks_t));
  if (idctBlocks == NULL) {
    printf("Memory allocation failed for idctBlocks\n");
    return NULL;
  }

  idctBlocks->data = (double***) malloc(blocks->totalBlocks * sizeof(double**));
  if (idctBlocks->data == NULL) {
    printf("Memory allocation failed for idctBlocks->data\n");
    free(idctBlocks);
    return NULL;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    idctBlocks->data[i] = getIdctBlock(blocks->data[i]);
  }

  idctBlocks->totalBlocks = blocks->totalBlocks;

  return idctBlocks;
}

// Quantiza um bloco aplicando a matriz de quantização
int** getQuantizedBlock(double** input, int quantizationTable[8][8]) {
  int** output = createIntBlock();

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      output[i][j] = round(input[i][j] / quantizationTable[i][j]);
    }
  }

  return output;
}

// Quantiza todos os blocos de uma imagem
IntBlocks_t* getQuantizedBlocks(Blocks_t* blocks, int quantizationTable[8][8]) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) on getQuantizedBlocks.");
    return NULL;
  }

  IntBlocks_t* quantizedBlocks = (IntBlocks_t*) malloc(sizeof(IntBlocks_t));
  if (quantizedBlocks == NULL) {
    printf("Memory allocation failed for quantizedBlocks\n");
    return NULL;
  }

  quantizedBlocks->data = (int***) malloc(blocks->totalBlocks * sizeof(int**));
  if (quantizedBlocks->data == NULL) {
    printf("Memory allocation failed for quantizedBlocks->data\n");
    free(quantizedBlocks);
    return NULL;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    quantizedBlocks->data[i] = getQuantizedBlock(blocks->data[i], quantizationTable);
  }

  quantizedBlocks->totalBlocks = blocks->totalBlocks;

  return quantizedBlocks;
}

// Desfaz a quantização multiplicando pela tabela
double** getDequantizedBlock(int** input, int quantizationTable[8][8]) {
  double** output = createBlock();

  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      output[i][j] = input[i][j] * quantizationTable[i][j];
    }
  }

  return output;
}

// Aplica desquantização em todos os blocos
Blocks_t* getDequantizedBlocks(IntBlocks_t* blocks, int quantizationTable[8][8]) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) on getDequantizedBlocks.");
    return NULL;
  }

  Blocks_t* dequantizedBlocks = (Blocks_t*) malloc(sizeof(Blocks_t));
  if (dequantizedBlocks == NULL) {
    printf("Memory allocation failed for dequantizedBlocks\n");
    return NULL;
  }

  dequantizedBlocks->data = (double***) malloc(blocks->totalBlocks * sizeof(double**));
  if (dequantizedBlocks->data == NULL) {
    printf("Memory allocation failed for dequantizedBlocks->data\n");
    free(dequantizedBlocks);
    return NULL;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    dequantizedBlocks->data[i] = getDequantizedBlock(blocks->data[i], quantizationTable);
  }

  dequantizedBlocks->totalBlocks = blocks->totalBlocks;

  return dequantizedBlocks;
}

// Ordem ZigZag para leitura de blocos
int indexes[64] = {
  0, 1, 8, 16, 9, 2, 3, 10,
  17, 24, 32, 25, 18, 11, 4, 5,
  12, 19, 26, 33, 40, 48, 41, 34,
  27, 20, 13, 6, 7, 14, 21, 28,
  35, 42, 49, 56, 57, 50, 43, 36,
  29, 22, 15, 23, 30, 37, 44, 51,
  58, 59, 52, 45, 38, 31, 39, 46,
  53, 60, 61, 54, 47, 55, 62, 63
};

// Libera memória de um bloco 8x8 de double
void destroyBlock(double** block) {
  if (block == NULL) {
    printf("Invalid block (null) on destroyBlock.");
    return;
  }

  for (int i = 0; i < BLOCK_SIZE; i++) {
    free(block[i]);
  }
  free(block);
}

// Libera todos os blocos (tipo double)
void destroyBlocks(Blocks_t* blocks) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) for destroyBlocks.");
    return;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    destroyBlock(blocks->data[i]);
  }
  free(blocks->data);
  free(blocks);
}

// Libera memória de um bloco 8x8 de inteiros
void destroyIntBlock(int** block) {
  if (block == NULL) {
    printf("Invalid int block (null) on destroyIntBlock.");
    return;
  }

  for (int i = 0; i < BLOCK_SIZE; i++) {
    free(block[i]);
  }
  free(block);
}

// Libera todos os blocos inteiros
void destroyIntBlocks(IntBlocks_t* blocks) {
  if (blocks == NULL) {
    printf("Invalid blocks (null) for destroyIntBlocks.");
    return;
  }

  for (int i = 0; i < blocks->totalBlocks; i++) {
    destroyIntBlock(blocks->data[i]);
  }
  free(blocks->data);  
}
