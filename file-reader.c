#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree.h"
#include "file-reader.h"
#include "image.h"
#include "block.h"

int ceil_div(int a, int b) {
  return (a + b - 1) / b;
}

void initBitReader(BitReader *reader, FILE *file) {
  reader->file = file;
  reader->buffer = 0;
  reader->bitCount = 0;
}

int readBit(BitReader *reader) {
  if (reader->bitCount == 0) {
    if (fread(&reader->buffer, 1, 1, reader->file) != 1) {
      return -1; // Erro ou EOF
    }
    reader->bitCount = 8;
  }
  
  int bit = (reader->buffer >> (reader->bitCount - 1)) & 1;
  reader->bitCount--;
  return bit;
}

int readIntFromBits(BitReader* reader) {
  int value = 0;
  for (int i = 0; i < 32; i++) {
    int bit = readBit(reader);  // Lê um bit
    value = (value << 1) | bit; // Desloca o valor atual e adiciona o novo bit
  }
  return value;
}

TreeNode_t* readHuffmanTree(BitReader *reader) {
  int bit = readBit(reader);
  if (bit != 1) return NULL; // Nó nulo ou erro
  
  bit = readBit(reader);
  if (bit == -1) return NULL;
  
  TreeNode_t *node = (TreeNode_t*)malloc(sizeof(TreeNode_t));

  if (bit == 1) { // É folha
    node->difference = 0;
    for (int i = 0; i < 32; i++) {
      bit = readBit(reader);
      if (bit == -1) {
        free(node);
        return NULL;
      };
      node->difference = (node->difference << 1) | bit;
    }
    node->frequence = 0;
  
    node->childLeft = NULL;
    node->childRight = NULL;
  } else { // É nó interno
    node->difference = 0;
    for (int i = 0; i < 32; i++) {
      bit = readBit(reader);
      if (bit == -1) {
        free(node);
        return NULL;
      };
      node->difference = (node->difference << 1) | bit;
    }
    node->frequence = 0;
  
    node->childLeft = readHuffmanTree(reader);
    node->childRight = readHuffmanTree(reader);
    if (!node->childLeft || !node->childRight) {
      destroyTreeUtil(node);
      return NULL;
    }
  }
  return node;
}

void readBinary(char* filePath) {
  FILE* file = fopen(filePath, "rb");
  if (!file) {
    printf("Error opening binary file.\n");
  }

  int readedHeight;
  fread(&readedHeight, sizeof(int), 1, file);
  int readedWidth;
  fread(&readedWidth, sizeof(int), 1, file);

  BitReader reader;
  initBitReader(&reader, file);
  TreeNode_t *readedTreeRoot = readHuffmanTree(&reader);

  Tree_t *arvoreLida = (Tree_t*)malloc(sizeof(Tree_t));
  arvoreLida->root = readedTreeRoot;

  Pixel* recoveredPixels = (Pixel*)malloc((readedHeight * readedWidth) * sizeof(Pixel));

  recoveredPixels[0].B = readIntFromBits(&reader);
  recoveredPixels[0].G = readIntFromBits(&reader);
  recoveredPixels[0].R = readIntFromBits(&reader);

  TreeNode_t* currentNode = arvoreLida->root;
  // The first pixel is skipped as it is not compressed
  for (int i = 1; i < (readedHeight * readedWidth); i++) {
    for (int j = 0; j < 3; j++) {
      int foundLeaf = 0;
      while (foundLeaf == 0) {
        if (currentNode->childLeft == NULL && currentNode->childRight == NULL) {
          if (j == 0) {
            recoveredPixels[i].B = recoveredPixels[i - 1].B + currentNode->difference;
          } else if (j == 1) {
            recoveredPixels[i].G = recoveredPixels[i - 1].G + currentNode->difference;
          } else {
            recoveredPixels[i].R = recoveredPixels[i - 1].R + currentNode->difference;
          }
          currentNode = arvoreLida->root;
          foundLeaf = 1;
        }

        int bit = readBit(&reader);
        if (bit == -1) {
          printf("Error reading bit.\n");
          break;
        }

        if (bit == 0) {
          currentNode = currentNode->childLeft;
        } 
        
        if (bit == 1) {
          currentNode = currentNode->childRight;
        }
      }
    }
  }

  fclose(file);

  BMPFILEHEADER fileHeader = {
    .bfType = 0x4D42,       // 'BM'
    .bfSize = 54 + (readedWidth * readedHeight * 3), // Tamanho total
    .bfReserved1 = 0,
    .bfReserved2 = 0,
    .bfOffBits = 54         // Offset para os pixels (14 + 40)
  };

  BMPINFOHEADER infoHeader = {
    .biSize = 40,
    .biWidth = readedWidth,
    .biHeight = readedHeight,
    .biPlanes = 1,
    .biBitCount = 24,       // 24 bits por pixel (RGB)
    .biCompression = 0,
    .biSizeImage = 0,       // Pode ser 0 para BI_RGB
    .biXPelsPerMeter = 0,
    .biYPelsPerMeter = 0,
    .biClrUsed = 0,
    .biClrImportant = 0
  };

  FILE* outputBmpFile = fopen("./output.bmp", "wb");
  if (!outputBmpFile) {
    printf("Error opening output file.\n");
    return;
  }

  fwrite(&fileHeader, sizeof(BMPFILEHEADER), 1, outputBmpFile);
  fwrite(&infoHeader, sizeof(BMPINFOHEADER), 1, outputBmpFile);

  for (int i = 0; i < infoHeader.biWidth * infoHeader.biHeight; i++) {
    fputc((unsigned char)recoveredPixels[i].B, outputBmpFile);
    fputc((unsigned char)recoveredPixels[i].G, outputBmpFile);
    fputc((unsigned char)recoveredPixels[i].R, outputBmpFile);
  }

  fclose(outputBmpFile);

  destroyTree(arvoreLida);
}

TreeNode_t* readHuffmanTreeLossy(BitReader *reader) {
  int bit = readBit(reader);
  if (bit == -1) return NULL; // EOF real

  if (bit == 0) return NULL; // Nó nulo

  bit = readBit(reader); // 1 = folha, 0 = interno
  if (bit == -1) return NULL;

  TreeNode_t *node = malloc(sizeof(TreeNode_t));
  if (!node) return NULL;

  node->difference = 0;
  for (int i = 0; i < 32; i++) {
    int b = readBit(reader);
    if (b == -1) {
      free(node);
      return NULL;
    }
    node->difference = (node->difference << 1) | b;
  }

  node->frequence = 0;

  if (bit == 1) {
    node->childLeft = NULL;
    node->childRight = NULL;
  } else {
    node->childLeft = readHuffmanTreeLossy(reader);
    node->childRight = readHuffmanTreeLossy(reader);
  }

  return node;
}

int YQUANTIZATION[8][8] = {
  {16, 11, 10, 16, 24, 40, 51, 61},
  {12, 12, 14, 19, 26, 58, 60, 55},
  {14, 13, 16, 24, 40, 57, 69, 56},
  {14, 17, 22, 29, 51, 87, 80, 62},
  {18, 22, 37, 56, 68,109,103, 77},
  {24, 35, 55, 64, 81,104,113, 92},
  {79, 64, 78, 87,103,121,120,101},
  {72, 92, 95, 98,112,100,103, 99}
};

int CBCRQUANTIZATION[8][8] = {
  {17, 18, 24, 47, 99, 99, 99, 99},
  {18, 21, 26, 66, 99, 99, 99, 99},
  {24, 26, 56, 99, 99, 99, 99, 99},
  {47, 66, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99}
};

void readLossyBinary(char* filePath) {
  FILE* file = fopen("lossy-output.bin", "rb");
  if (!file) {
    printf("Error opening binary file.\n");
    return;
  }

  int compressionType;
  fread(&compressionType, sizeof(int), 1, file);
  int width;
  fread(&width, sizeof(int), 1, file);
  int height;
  fread(&height, sizeof(int), 1, file);
  int originalWidth;
  fread(&originalWidth, sizeof(int), 1, file);
  int originalHeight;
  fread(&originalHeight, sizeof(int), 1, file);

  BitReader reader;
  initBitReader(&reader, file);
  
  TreeNode_t *YTreeRoot = readHuffmanTreeLossy(&reader);
  Tree_t *YTree = (Tree_t*)malloc(sizeof(Tree_t));
  YTree->root = YTreeRoot;

  int bitsRead = 0;
  TreeNode_t* currentNode = YTreeRoot;

  int blocks_horizontal = ceil_div(width, 8);
  int blocks_vertical = ceil_div(height, 8);
  int total_blocks = blocks_horizontal * blocks_vertical;

  IntBlocks_t* YQuantizedBlocks = createIntBlocks(total_blocks);

  for (int i = 0; i < total_blocks; i++) {
    int** block = createIntBlock();

    int coeficientsCount = 0;
    while (coeficientsCount < BLOCK_SIZE * BLOCK_SIZE) {
      int bit = readBit(&reader);
      if (bit == -1) {
        printf("\nPremature EOF at bit\n");
        break;
      }
      
      if (bit == 0) currentNode = currentNode->childLeft;
      else currentNode = currentNode->childRight;
      
      if (currentNode->childLeft == NULL && currentNode->childRight == NULL) {
        block[coeficientsCount / BLOCK_SIZE][coeficientsCount % BLOCK_SIZE] = currentNode->difference;
        YQuantizedBlocks->data[i] = block;
        currentNode = YTreeRoot;
        coeficientsCount++;
      }
    }
  }

  Blocks_t* YDctBlocks = getDequantizedBlocks(YQuantizedBlocks, YQUANTIZATION);

  Blocks_t* YIdctBlocks = getIdctBlocks(YDctBlocks);

  int rowSize = (width * 3 + 3) & (~3); // múltiplo de 4
  int padding = rowSize - (width * 3);
  int imageSize = rowSize * height;

  BMPFILEHEADER fileHeader = {
    .bfType = 0x4D42,       // 'BM'
    .bfSize = 54 + imageSize,
    .bfReserved1 = 0,
    .bfReserved2 = 0,
    .bfOffBits = 54
  };

  BMPINFOHEADER infoHeader = {
    .biSize = 40,
    .biWidth = width,
    .biHeight = height,
    .biPlanes = 1,
    .biBitCount = 24,
    .biCompression = 0,
    .biSizeImage = imageSize,
    .biXPelsPerMeter = 0,
    .biYPelsPerMeter = 0,
    .biClrUsed = 0,
    .biClrImportant = 0
  };

  FILE* outputBmpFile = fopen("./output-lossy.bmp", "wb");
  if (!outputBmpFile) {
    printf("Error opening output file.\n");
    return;
  }

  fwrite(&fileHeader, sizeof(BMPFILEHEADER), 1, outputBmpFile);
  fwrite(&infoHeader, sizeof(BMPINFOHEADER), 1, outputBmpFile);

  // Buffer de padding
  unsigned char paddingBytes[3] = {0, 0, 0};

  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      int blockX = x / BLOCK_SIZE;
      int blockY = (height - 1 - y) / BLOCK_SIZE;
      int blockIndex = blockY * blocks_horizontal + blockX;

      double** block = YIdctBlocks->data[blockIndex];
      int localX = x % BLOCK_SIZE;
      int localY = (height - 1 - y) % BLOCK_SIZE;

      int value = round(block[localY][localX]);
      if (value < 0) value = 0;
      if (value > 255) value = 255;

      unsigned char B = value;
      unsigned char G = value;
      unsigned char R = value;

      fputc(B, outputBmpFile);
      fputc(G, outputBmpFile);
      fputc(R, outputBmpFile);
    }
    fwrite(paddingBytes, 1, padding, outputBmpFile); // escreve padding
  }

  fclose(outputBmpFile);

  destroyBlocks(YIdctBlocks);

  destroyBlocks(YDctBlocks);

  destroyIntBlocks(YQuantizedBlocks);

  destroyTree(YTree);
  fclose(file);
}
