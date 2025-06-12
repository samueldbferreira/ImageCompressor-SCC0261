#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree.h"
#include "file-reader.h"
#include "image.h"
#include "block.h"

// Inicializa a estrutura do leitor de bits
void initBitReader(BitReader *reader, FILE *file) {
  reader->file = file;
  reader->buffer = 0;
  reader->bitCount = 0;
}

// Lê 1 bit do buffer (se estiver vazio, lê 1 byte do arquivo)
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

// Reconstrói um inteiro de 32 bits lido bit a bit
int readIntFromBits(BitReader* reader) {
  int value = 0;
  for (int i = 0; i < 32; i++) {
    int bit = readBit(reader);  // Lê um bit
    value = (value << 1) | bit; // Desloca o valor atual e adiciona o novo bit
  }
  return value;
}

// Reconstrói a árvore de Huffman recursivamente
TreeNode_t* readHuffmanTree(BitReader *reader) {
  int bit = readBit(reader);
  if (bit == -1) return NULL; // EOF real

  if (bit == 0) return NULL; // Nó nulo

  bit = readBit(reader); // 1 = folha, 0 = interno
  if (bit == -1) return NULL;

  // Aloca um novo nó da árvore
  TreeNode_t *node = malloc(sizeof(TreeNode_t));
  if (!node) return NULL;
  node->value = 0;
  for (int i = 0; i < 32; i++) {
    int b = readBit(reader);
    if (b == -1) {
      free(node);
      return NULL;
    }
    node->value = (node->value << 1) | b;
  }
  node->frequence = 0;

  // Se for uma folha, não tem filhos
  if (bit == 1) {
    node->childLeft = NULL;
    node->childRight = NULL;
  } else {
    // Se for um nó interno, lê os filhos recursivamente
    node->childLeft = readHuffmanTree(reader);
    node->childRight = readHuffmanTree(reader);
  }

  return node;
}

// Realiza a descompressão da imagem sem perdas e reconstrói o BMP
void decompressLossless(FILE* inputFile, char* outputFilePath) {
  // Lê dimensões
  int readedHeight;
  fread(&readedHeight, sizeof(int), 1, inputFile);
  int readedWidth;
  fread(&readedWidth, sizeof(int), 1, inputFile);

  BitReader reader;
  initBitReader(&reader, inputFile);

  // Lê arvore de Huffman
  TreeNode_t *readedTreeRoot = readHuffmanTree(&reader);
  Tree_t *arvoreLida = (Tree_t*)malloc(sizeof(Tree_t));
  arvoreLida->root = readedTreeRoot;

  // Aloca memória para os pixels recuperados
  Pixel* recoveredPixels = (Pixel*)malloc((readedHeight * readedWidth) * sizeof(Pixel));

  // Lê primeiro pixel diretamente
  recoveredPixels[0].B = readIntFromBits(&reader);
  recoveredPixels[0].G = readIntFromBits(&reader);
  recoveredPixels[0].R = readIntFromBits(&reader);

  // Decodifica diferenças dos pixels restantes com Huffman e recupera os valores RGB
  TreeNode_t* currentNode = arvoreLida->root;
  for (int i = 1; i < (readedHeight * readedWidth); i++) {
    for (int j = 0; j < 3; j++) {
      int foundLeaf = 0;
      while (foundLeaf == 0) {
        if (currentNode->childLeft == NULL && currentNode->childRight == NULL) {
          if (j == 0) {
            recoveredPixels[i].B = recoveredPixels[i - 1].B + currentNode->value;
          } else if (j == 1) {
            recoveredPixels[i].G = recoveredPixels[i - 1].G + currentNode->value;
          } else {
            recoveredPixels[i].R = recoveredPixels[i - 1].R + currentNode->value;
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

  // Prepara o cabeçalho do BMP
  BMPFILEHEADER fileHeader = {
    .bfType = 0x4D42,
    .bfSize = 54 + (readedWidth * readedHeight * 3),
    .bfReserved1 = 0,
    .bfReserved2 = 0,
    .bfOffBits = 54
  };

  BMPINFOHEADER infoHeader = {
    .biSize = 40,
    .biWidth = readedWidth,
    .biHeight = readedHeight,
    .biPlanes = 1,
    .biBitCount = 24,
    .biCompression = 0,
    .biSizeImage = 0,
    .biXPelsPerMeter = 0,
    .biYPelsPerMeter = 0,
    .biClrUsed = 0,
    .biClrImportant = 0
  };

  // Abre o arquivo BMP de saída
  FILE* outputBmpFile = fopen(outputFilePath, "wb");
  if (!outputBmpFile) {
    printf("Error opening output file.\n");
    return;
  }

  // Escreve o cabeçalho do BMP
  fwrite(&fileHeader, sizeof(BMPFILEHEADER), 1, outputBmpFile);
  fwrite(&infoHeader, sizeof(BMPINFOHEADER), 1, outputBmpFile);

  // Escreve os pixels recuperados no arquivo BMP
  for (int i = 0; i < infoHeader.biWidth * infoHeader.biHeight; i++) {
    fputc((unsigned char)recoveredPixels[i].B, outputBmpFile);
    fputc((unsigned char)recoveredPixels[i].G, outputBmpFile);
    fputc((unsigned char)recoveredPixels[i].R, outputBmpFile);
  }

  fclose(outputBmpFile);

  destroyTree(arvoreLida);
}

// Matrizes de quantização padrão JPEG (Y e CbCr)
int YQUANTIZATION[8][8] = {
  {16, 11, 10, 16, 24, 40, 51, 61},
  {12, 12, 14, 19, 26, 58, 60, 55},
  {14, 13, 16, 24, 40, 57, 69, 56},
  {14, 17, 22, 29, 51, 87, 80, 62},
  {18, 22, 37, 56, 68,109,103, 77},
  {24, 35, 55, 64, 81,104,113, 92},
  {49, 64, 78, 87,103,121,120,101},
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

// Descomprime imagem com perdas, reconstrói canais YCbCr e gera BMP
void decompressLossy(FILE* inputFile, char* outputFilePath) {
  // Lê header
  int width;
  fread(&width, sizeof(int), 1, inputFile);
  int height;
  fread(&height, sizeof(int), 1, inputFile);
  int originalWidth;
  fread(&originalWidth, sizeof(int), 1, inputFile);
  int originalHeight;
  fread(&originalHeight, sizeof(int), 1, inputFile);

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
  initBitReader(&reader, inputFile);
  
  // Para cada canal (Y, Cb, Cr)
  for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
    IntBlocks_t* quantizedBlocks = createIntBlocks(totalBlocks);
    
    // Lê arvore de Huffman
    Tree_t *tree = (Tree_t*)malloc(sizeof(Tree_t));
    TreeNode_t *treeRoot = readHuffmanTree(&reader);
    tree->root = treeRoot;

    // Lê a codificiação dos blocos e recupera os valores quantizados ao percorrer a árvore
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
        
        // Bit 0 vai para o filho esquerdo, bit 1 para o direito
        if (bit == 0) currentNode = currentNode->childLeft;
        else currentNode = currentNode->childRight;
        
        // Se for um nó folha, salva o valor quantizado, incrementa o contador e reseta para a raiz
        if (currentNode->childLeft == NULL && currentNode->childRight == NULL) {
          block[coeficientsCount / BLOCK_SIZE][coeficientsCount % BLOCK_SIZE] = currentNode->value;
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

  // Desfaz a quantização dos blocos de cada canal
  Blocks_t* YDctBlocks = getDequantizedBlocks(YQuantizedBlocks, YQUANTIZATION);
  Blocks_t* CbDctBlocks = getDequantizedBlocks(CbQuantizedBlocks, CBCRQUANTIZATION);
  Blocks_t* CrDctBlocks = getDequantizedBlocks(CrQuantizedBlocks, CBCRQUANTIZATION);

  // Desfaz o DCT, obtendo os valores originais de Y, Cb e Cr aproximados
  Blocks_t* YIdctBlocks = getIdctBlocks(YDctBlocks);
  Blocks_t* CbIdctBlocks = getIdctBlocks(CbDctBlocks);
  Blocks_t* CrIdctBlocks = getIdctBlocks(CrDctBlocks);

  int rowSize = (originalWidth * 3 + 3) & (~3);
  int padding = rowSize - (originalWidth * 3);
  int imageSize = rowSize * originalHeight;

  // Prepara o cabeçalho do BMP
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

  // Abre o arquivo BMP de saída
  FILE* outputBmpFile = fopen(outputFilePath, "wb");
  if (!outputBmpFile) {
    printf("Error opening output image.\n");
    return;
  }

  // Escreve o cabeçalho do BMP
  fwrite(&fileHeader, sizeof(BMPFILEHEADER), 1, outputBmpFile);
  fwrite(&infoHeader, sizeof(BMPINFOHEADER), 1, outputBmpFile);

  // Buffer de padding
  unsigned char paddingBytes[3] = {0, 0, 0};

  // Preenche a imagem com os valores RGB convertidos dos blocos YCbCr
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
}
