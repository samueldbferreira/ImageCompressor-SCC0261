#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "min-heap.h"
#include "table.h"
#include "codes-table.h"
#include "file-writer.h"
#include "file-reader.h"

int main() {
  /*
  HashTable_t *tableExample = createTable();

  // W
  for (int i = 0; i < 7; i++) {
    tableInsert(tableExample, 0);
  }

  // U
  for (int i = 0; i < 12; i ++) {
    tableInsert(tableExample, 1);
  }

  // X
  for (int i = 0; i < 15; i ++) {
    tableInsert(tableExample, 2);
  }

  // V
  for (int i = 0; i < 18; i ++) {
    tableInsert(tableExample, 3);
  }

  // Y
  for (int i = 0; i < 20; i ++) {
    tableInsert(tableExample, 4);
  }

  List_t *itemsExample = getItems(tableExample);

  
  Tree_t *treeExample = createTreeFromList(itemsExample);
  printTree(treeExample);

  writeBinaryFile(infoHeader.biWidth, infoHeader.biHeight, 5, treeExample)

  destroyTree(treeExample);

  destroyList(itemsExample);
  */

  char* inputFilePath = "./256x256.bmp";
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

  for (int i = 0; i < 10; i++) {
    printf("Pixel %d: B: %d, G: %d, R: %d\n", i, Image[i].B, Image[i].G, Image[i].R);
  }
  
  Pixel* differences = (Pixel *) malloc((10) * sizeof(Pixel));
  differences[0].B = Image[0].B;
  differences[0].G = Image[0].G;
  differences[0].R = Image[0].R;
  for (int i = 1; i < 10; i++) {
    differences[i].B = (Image[i].B - Image[i - 1].B);
    differences[i].G = (Image[i].G - Image[i - 1].G);
    differences[i].R = (Image[i].R - Image[i - 1].R);
  }

  HashTable_t* frequencesTable = createTable();

  for (int i = 1; i < 10; i++) {
    tableInsert(frequencesTable, differences[i].B);
    tableInsert(frequencesTable, differences[i].G);
    tableInsert(frequencesTable, differences[i].R);
  }

  List_t* itemsFrequencies = getItems(frequencesTable);

  Tree_t* treeFrequencies = createTreeFromList(itemsFrequencies);
  // printTree(treeFrequencies);

  CodesTable_t* codesTable = generateCodesTable(frequencesTable, itemsFrequencies, treeFrequencies);

  writeBinaryFile(infoHeader.biWidth, infoHeader.biHeight, 5, treeFrequencies, differences, codesTable);

  destroyCodesTable(codesTable);

  destroyTree(treeFrequencies);

  destroyList(itemsFrequencies);

  destroyTable(frequencesTable);

  readBinary();

  FILE* outputBmpFile = fopen("./output.bmp", "wb");
  if (!outputBmpFile) {
    printf("Error opening output file.\n");
    return 1;
  }

  fwrite(&fileHeader, sizeof(BMPFILEHEADER), 1, outputBmpFile);
  fwrite(&infoHeader, sizeof(BMPINFOHEADER), 1, outputBmpFile);

  for (int i = 0; i < infoHeader.biWidth * infoHeader.biHeight; i++) {
    printf("Writing pixel %d: B: %d, G: %d, R: %d\n", i, Image[i].B, Image[i].G, Image[i].R);
    fputc((unsigned char)Image[i].B, outputBmpFile);
    fputc((unsigned char)Image[i].G, outputBmpFile);
    fputc((unsigned char)Image[i].R, outputBmpFile);
  }

  fclose(outputBmpFile);

  return 0;
}
