#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "min-heap.h"
#include "table.h"
#include "codes-table.h"
#include "file-writer.h"
#include "file-reader.h"

int main() {
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

  fclose(inputBmpFile);

  writeBinaryFile(infoHeader.biWidth, infoHeader.biHeight, 5, treeExample);

  destroyTree(treeExample);

  destroyList(itemsExample);

  readBinary();

  /*
  printf("\nDifferences\n");
  printf("[INFO] Pixel 1 values: B: %d, G: %d, R: %d\n", Image[0].B, Image[0].G, Image[0].R);
  for (int i = 1; i < 10; i++) {
    printf("[INFO] Pixel %d values: B: %d, G: %d, R: %d\n", i + 1, Image[i].B - Image[i - 1].B, Image[i].G - Image[i - 1].G, Image[i].R - Image[i - 1].R);
  }
  */

  return 0;
}
