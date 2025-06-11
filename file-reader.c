#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree.h"
#include "file-reader.h"
#include "image.h"
#include "block.h"

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
  FILE* file = fopen(filePath, "rb");
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

  int horizontalBlocks = ceilDiv(width, BLOCK_SIZE);
  int verticalBlocks = ceilDiv(height, BLOCK_SIZE);
  int totalBlocks = horizontalBlocks * verticalBlocks;

  IntBlocks_t* YQuantizedBlocks;
  IntBlocks_t* CbQuantizedBlocks;
  IntBlocks_t* CrQuantizedBlocks;

  Tree_t *YTree;
  Tree_t *CbTree;
  Tree_t *CrTree;

  BitReader reader;
  initBitReader(&reader, file);

  for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
    IntBlocks_t* quantizedBlocks = createIntBlocks(totalBlocks);
    
    Tree_t *tree = (Tree_t*)malloc(sizeof(Tree_t));
    TreeNode_t *treeRoot = readHuffmanTreeLossy(&reader);
    tree->root = treeRoot;

    TreeNode_t* currentNode = treeRoot;
    for (int i = 0; i < totalBlocks; i++) {
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
          quantizedBlocks->data[i] = block;
          currentNode = treeRoot;
          coeficientsCount++;
        }
      }
    }

    if (channelIndex == 0) {
      YQuantizedBlocks = quantizedBlocks;
      YTree = tree;
    } else if (channelIndex == 1) {
      CbQuantizedBlocks = quantizedBlocks;
      CbTree = tree;
    } else {
      CrQuantizedBlocks = quantizedBlocks;
      CrTree = tree;
    }
  }

  Blocks_t* YDctBlocks = getDequantizedBlocks(YQuantizedBlocks, YQUANTIZATION);
  Blocks_t* CbDctBlocks = getDequantizedBlocks(CbQuantizedBlocks, CBCRQUANTIZATION);
  Blocks_t* CrDctBlocks = getDequantizedBlocks(CrQuantizedBlocks, CBCRQUANTIZATION);

  Blocks_t* YIdctBlocks = getIdctBlocks(YDctBlocks);
  Blocks_t* CbIdctBlocks = getIdctBlocks(CbDctBlocks);
  Blocks_t* CrIdctBlocks = getIdctBlocks(CrDctBlocks);

  int rowSize = (originalWidth * 3 + 3) & (~3);
  int padding = rowSize - (originalWidth * 3);
  int imageSize = rowSize * originalHeight;

  BMPFILEHEADER fileHeader = {
    .bfType = 0x4D42,
    .bfSize = 54 + imageSize,
    .bfReserved1 = 0,
    .bfReserved2 = 0,
    .bfOffBits = 54
  };

  BMPINFOHEADER infoHeader = {
    .biSize = 40,
    .biWidth = originalWidth,
    .biHeight = originalHeight,
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

  for (int y = originalHeight - 1; y >= 0; y--) {
    for (int x = 0; x < originalWidth; x++) {
      int blockX = x / BLOCK_SIZE;
      int blockY = (originalHeight - 1 - y) / BLOCK_SIZE;
      int blockIndex = blockY * horizontalBlocks + blockX;

      int localX = x % BLOCK_SIZE;
      int localY = (originalHeight - 1 - y) % BLOCK_SIZE;

      double Y = YIdctBlocks->data[blockIndex][localY][localX];
      double Cb = CbIdctBlocks->data[blockIndex][localY][localX];
      double Cr = CrIdctBlocks->data[blockIndex][localY][localX];

      int BValue = round(Y + 1.772 * (Cb - 128));
      int GValue = round(Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128));
      int RValue = round(Y + 1.402 * (Cr - 128));

      if (BValue < 0) BValue = 0;
      if (BValue > 255) BValue = 255;
      if (GValue < 0) GValue = 0;
      if (GValue > 255) GValue = 255;
      if (RValue < 0) RValue = 0;
      if (RValue > 255) RValue = 255;

      unsigned char B = BValue;
      unsigned char G = GValue;
      unsigned char R = RValue;

      fputc(B, outputBmpFile);
      fputc(G, outputBmpFile);
      fputc(R, outputBmpFile);
    }
    fwrite(paddingBytes, 1, padding, outputBmpFile); // escreve padding
  }

  fclose(outputBmpFile);

  destroyBlocks(YIdctBlocks);
  destroyBlocks(CbIdctBlocks);
  destroyBlocks(CrIdctBlocks);

  destroyBlocks(YDctBlocks);
  destroyBlocks(CbDctBlocks);
  destroyBlocks(CrDctBlocks);

  destroyIntBlocks(YQuantizedBlocks);
  destroyIntBlocks(CbQuantizedBlocks);
  destroyIntBlocks(CrQuantizedBlocks);

  destroyTree(YTree);
  destroyTree(CbTree);
  destroyTree(CrTree);

  fclose(file);
}
