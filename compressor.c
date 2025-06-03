#include <stdio.h>
#include <stdlib.h>
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

    writeBinaryFile(infoHeader.biWidth, infoHeader.biHeight, 5, treeFrequencies, differences, codesTable);

    destroyCodesTable(codesTable);

    destroyTree(treeFrequencies);

    destroyList(itemsFrequencies);

    destroyTable(frequencesTable);

    readBinary();
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

  return 0;
}
