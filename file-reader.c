#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "file-reader.h"
#include "image.h"

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

void readBinary() {
  FILE* file = fopen("output.bin", "rb");
  if (!file) {
    printf("Error opening binary file.\n");
  }

  int readedHeight;
  fread(&readedHeight, sizeof(int), 1, file);
  int readedWidth;
  fread(&readedWidth, sizeof(int), 1, file);
  int numberOfSymbols;
  fread(&numberOfSymbols, sizeof(int), 1, file);

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
