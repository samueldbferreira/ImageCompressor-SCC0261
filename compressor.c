/*
  NOME: Samuel de Assunção Ferreira
  Nº USP: 12543565
  DISCIPLINA: SSC226 - Multimídia
  PROFESSOR: Rudinei Goularte
  PROJETO: Compressão de Imagens
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "min-heap.h"
#include "table.h"
#include "codes-table.h"
#include "file-writer.h"
#include "file-reader.h"
#include "channels.h"
#include "block.h"

int main() {
  printf("Enter the compression type:\n");
  printf("1 - Lossless (Differences + Huffmann)\n");
  printf("2 - Lossy (JPEG pipeline)\n");
  printf("Enter your choice (1 or 2): ");
  int compressionType;
  scanf("%d", &compressionType);
  if (compressionType != 1 && compressionType != 2) {
    printf("Invalid compression type. Please enter 1 or 2.\n");
    return 1;
  }

  printf("\nEnter the input BMP file path: ");
  char inputFilePath[256];
  scanf("%255s", inputFilePath); 
  if (inputFilePath[0] == '\0') {
    printf("Invalid file path.\n");
    return 1;
  }

  printf("\nEnter the output binary file path: ");
  char outputFilePath[256];
  scanf("%255s", outputFilePath);
  if (outputFilePath[0] == '\0') {
    printf("Invalid output file path.\n");
    return 1;
  }
  printf("\nProcessing file %s...\n", inputFilePath);

  FILE* inputBmpFile = fopen(inputFilePath, "rb");
  if (!inputBmpFile) {
    printf("Error opening file %s\n", inputFilePath);
    return 1;
  }

  BMPFILEHEADER fileHeader;
  leituraHeader(inputBmpFile, &fileHeader);

  BMPINFOHEADER infoHeader;
  leituraInfoHeader(inputBmpFile, &infoHeader);

  if (compressionType == 1) {
    Pixel *Image = (Pixel *) malloc((infoHeader.biWidth * infoHeader.biHeight) * sizeof(Pixel));
    loadBMPImage(inputBmpFile, infoHeader, Image, 1);

    fclose(inputBmpFile);

    Pixel* differences = (Pixel *) malloc((infoHeader.biWidth * infoHeader.biHeight) * sizeof(Pixel));
    differences[0].B = Image[0].B;
    differences[0].G = Image[0].G;
    differences[0].R = Image[0].R;
    for (int i = 1; i < (infoHeader.biWidth * infoHeader.biHeight); i++) {
      differences[i].B = (Image[i].B - Image[i - 1].B);
      differences[i].G = (Image[i].G - Image[i - 1].G);
      differences[i].R = (Image[i].R - Image[i - 1].R);
    }

    HashTable_t* frequencesTable = createTable();

    for (int i = 1; i < (infoHeader.biWidth * infoHeader.biHeight); i++) {
      tableInsert(frequencesTable, differences[i].B);
      tableInsert(frequencesTable, differences[i].G);
      tableInsert(frequencesTable, differences[i].R);
    }

    List_t* itemsFrequencies = getItems(frequencesTable);

    Tree_t* treeFrequencies = createTreeFromList(itemsFrequencies);

    CodesTable_t* codesTable = generateCodesTable(frequencesTable, itemsFrequencies, treeFrequencies);

    writeBinaryFile(infoHeader.biWidth, infoHeader.biHeight, treeFrequencies, differences, codesTable, outputFilePath);

    destroyCodesTable(codesTable);

    destroyTree(treeFrequencies);

    destroyList(itemsFrequencies);

    destroyTable(frequencesTable);

    free(differences);

    free(Image);
  }

  if (compressionType == 2) {
    int newWidth = ceilDiv(infoHeader.biWidth, BLOCK_SIZE) * BLOCK_SIZE;
    int newHeight = ceilDiv(infoHeader.biHeight, BLOCK_SIZE) * BLOCK_SIZE;

    Pixel *Image = (Pixel *) malloc((newWidth * newHeight) * sizeof(Pixel));
    loadBMPImage(inputBmpFile, infoHeader, Image, 2);

    fclose(inputBmpFile);

    Channels_t* channels = createChannels(Image, newWidth, newHeight);
  
    Blocks_t* YBlocks = createBlocks(getY(channels), newWidth, newHeight);
    Blocks_t* CbBlocks = createBlocks(getCb(channels), newWidth, newHeight);
    Blocks_t* CrBlocks = createBlocks(getCr(channels), newWidth, newHeight);

    Blocks_t* YBlocksDct = getDctBlocks(YBlocks);
    Blocks_t* CbBlocksDct = getDctBlocks(CbBlocks);
    Blocks_t* CrBlocksDct = getDctBlocks(CrBlocks);

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

    IntBlocks_t* YBlocksQuant = getQuantizedBlocks(YBlocksDct, YQUANTIZATION);
    IntBlocks_t* CbBlocksQuant = getQuantizedBlocks(CbBlocksDct, CBCRQUANTIZATION);
    IntBlocks_t* CrBlocksQuant = getQuantizedBlocks(CrBlocksDct, CBCRQUANTIZATION);

    Tree_t* YTree;
    Tree_t* CbTree;
    Tree_t* CrTree;

    CodesTable_t* YCodesTable;
    CodesTable_t* CbCodesTable;
    CodesTable_t* CrCodesTable;  

    for (int i = 0; i < 3; i++) {
      IntBlocks_t* blocksQuant = NULL;
      
      if (i == 0) {
        blocksQuant = YBlocksQuant;
      } else if (i == 1) {
        blocksQuant = CbBlocksQuant;
      } else {
        blocksQuant = CrBlocksQuant;
      }

      HashTable_t* frequencesTable = createTable();

      for (int i = 0; i < blocksQuant->totalBlocks; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
          for (int k = 0; k < BLOCK_SIZE; k++) {
            int value = blocksQuant->data[i][j][k];
            tableInsert(frequencesTable, value);
          }
        }
      }

      List_t* items = getItems(frequencesTable);

      Tree_t* tree = createTreeFromList(items);
      if (i == 0) {
        YTree = tree;
      } else if (i == 1) {
        CbTree = tree;
      } else {
        CrTree = tree;
      }

      CodesTable_t* codesTable = generateCodesTable(frequencesTable, items, tree);
      if (i == 0) {
        YCodesTable = codesTable;
      } else if (i == 1) {
        CbCodesTable = codesTable;
      } else {
        CrCodesTable = codesTable;
      }

      destroyList(items);

      destroyTable(frequencesTable);
    }

    writeLossyBinaryFile(
      newWidth,
      newHeight,
      infoHeader.biWidth,
      infoHeader.biHeight,
      YTree,
      CbTree,
      CrTree,
      YCodesTable,
      CbCodesTable,
      CrCodesTable,
      YBlocksQuant,
      CbBlocksQuant,
      CrBlocksQuant,
      outputFilePath
    );

    destroyIntBlocks(CrBlocksQuant);
    destroyIntBlocks(CbBlocksQuant);
    destroyIntBlocks(YBlocksQuant);

    destroyBlocks(CrBlocksDct);
    destroyBlocks(CbBlocksDct);
    destroyBlocks(YBlocksDct);

    destroyBlocks(CrBlocks);
    destroyBlocks(CbBlocks);
    destroyBlocks(YBlocks);

    destroyChannels(channels);

    free(Image);
  }

  unsigned int originalFileSize = fileHeader.bfSize;
  unsigned int compressedFileSize = (unsigned int)getFileSize(outputFilePath);
  float reduction = getCompressionRatio(originalFileSize, compressedFileSize);

  if (compressionType == 1) {
    printf("\nFile %s processed successfully with lossless compression\n", inputFilePath);
  } else {
    printf("\nFile %s processed successfully with lossy compression\n", inputFilePath);
  }
  printf("Output written to %s\n", outputFilePath);
  printf("Compression ratio: %.2f%%\n", reduction);

  return 0;
}
