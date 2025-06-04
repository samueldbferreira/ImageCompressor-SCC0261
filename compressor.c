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
  /*
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

  Pixel *Image = (Pixel *) malloc((infoHeader.biWidth * infoHeader.biHeight) * sizeof(Pixel));
  loadBMPImage(inputBmpFile, infoHeader, Image);

  fclose(inputBmpFile); 

  if (compressionType == 1) {
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

    readBinary(outputFilePath);

    unsigned int originalFileSize = fileHeader.bfSize;
    unsigned int compressedFileSize = (unsigned int)getFileSize(outputFilePath);

    printf("\nOriginal file size: %u bytes\n", originalFileSize);
    printf("Compressed file size: %u bytes\n", compressedFileSize);

    printf("\nFile %s processed successfully with lossless compression\n", inputFilePath);
    printf("Output written to %s\n", outputFilePath);

    float reduction = getCompressionRatio(originalFileSize, compressedFileSize);
    printf("Compression ratio: %.2f%%\n", reduction);
  }

  if (compressionType == 2) {
    Channels_t* channels = createChannels(Image, infoHeader.biWidth, infoHeader.biHeight);

    Blocks_t* YBlocks = createBlocks(getY(channels), infoHeader.biWidth, infoHeader.biHeight);
    Blocks_t* CbBlocks = createBlocks(getCb(channels), infoHeader.biWidth, infoHeader.biHeight);
    Blocks_t* CrBlocks = createBlocks(getCr(channels), infoHeader.biWidth, infoHeader.biHeight);


    destroyBlocks(CrBlocks, infoHeader.biWidth, infoHeader.biHeight);
    destroyBlocks(CbBlocks, infoHeader.biWidth, infoHeader.biHeight);
    destroyBlocks(YBlocks, infoHeader.biWidth, infoHeader.biHeight);

    destroyChannels(channels);
  }
  */

  for (int i = 0; i < (infoHeader.biHeight * infoHeader.biWidth); i++) {
    fputc(getY(channels)[i], bmpOutputFile);
    fputc(getY(channels)[i], bmpOutputFile);
    fputc(getY(channels)[i], bmpOutputFile);
  }

  fclose(inputBmpFile);
  fclose(bmpOutputFile);

  double** block = YBlocks->data[0];

  double** output_dct = getDctBlock(block);

  int quant_luminancia[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68,109,103, 77},
    {24, 35, 55, 64, 81,104,113, 92},
    {79, 64, 78, 87,103,121,120,101},
    {72, 92, 95, 98,112,100,103, 99}
  };

  int quant_crominancia[8][8] = {
    {17, 18, 24, 47, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
  };

  double** output_quantized = getQuantizedBlock(output_dct, quant_luminancia);

  double input[8][8] = {
    { 272,   0,  0, -1,  0,  0,  0,  0 },
    {  92,   0,  0,  0,  0,  0,  0,  0 },
    {  -9,   2,  0,  0,  0,  0,  0,  0 },
    {   8,   1,  0,  0, -1,  0,  0,  0 },
    {  -1,  -1,  0,  1, -1,  0,  0,  0 },
    {   0,   0,  0,  0,  0,  0,  0,  0 },
    {   3,   1,  0,  0,  0,  0,  0,  0 },
    {  -1,   0,  0,  1,  0,  0,  0,  0 }
  };

  double **zizZagInput = (double**)malloc(8 * sizeof(double*));
  for (int i = 0; i < 8; i++) {
    zizZagInput[i] = (double*)malloc(8 * sizeof(double));
    for (int j = 0; j < 8; j++) {
      zizZagInput[i][j] = input[i][j];
    }
  }

  HashTable_t* table = createTable();

  int* zigzag = getZigZagArray(zizZagInput);

  for (int i = 0; i < 64; i++) {
    tableInsert(table, zigzag[i]);
  }

  List_t* tableItems = getItems(table);

  Tree_t* tree = createTreeFromList(tableItems);

  printTree(tree);

  destroyZigZagArray(zigzag);
  destroyTree(tree);
  destroyTable(table);
  destroyBlock(zizZagInput);
  
  destroyBlocks(YBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyBlocks(CbBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyBlocks(CrBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyChannels(channels);

  return 0;
}
