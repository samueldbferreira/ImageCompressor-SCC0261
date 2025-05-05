#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "min-heap.h"
#include "table.h"

int main() {
  char* inputFilePath = "./256x256.bmp";
  FILE* inputBmpFile = fopen(inputFilePath, "rb");
  if (!inputBmpFile) {
    printf("Error opening file %s\n", inputFilePath);
    return 1;
  }

  BMPFILEHEADER fileHeader;
  leituraHeader(inputBmpFile, &fileHeader);
  printFileHeader(&fileHeader);

  BMPINFOHEADER infoHeader;
  leituraInfoHeader(inputBmpFile, &infoHeader);
  printInfoHeader(&infoHeader);

  Pixel *Image;
  Image = (Pixel *) malloc((infoHeader.biWidth * infoHeader.biHeight) * sizeof(Pixel));
  loadBMPImage(inputBmpFile, infoHeader, Image);

  char* outputFilePath = "./output.bmp";
  FILE* bmpOutputFile = fopen(outputFilePath, "wb");
  if(!bmpOutputFile) {
    printf("Error opening output file %s\n", outputFilePath);
    return 1;
  }
                  
  fseek(inputBmpFile, 0, SEEK_SET);
  
  // Reescrevendo o cabeçalho do arquivo BMP
  for (int i = 0; i < 54; i++) {
    fputc(fgetc(inputBmpFile), bmpOutputFile);
  }

  // Compressão simples: armazenando a diferença entre os pixels
  // O primeiro pixel é armazenado diretamente
  fputc(Image[0].B, bmpOutputFile);
  fputc(Image[0].G, bmpOutputFile);
  fputc(Image[0].R, bmpOutputFile);

  // Armazenando a diferença entre os pixels consecutivos
  // O primeiro pixel já foi armazenado, então começamos do segundo
  for (int i = 1; i < (infoHeader.biHeight * infoHeader.biWidth); i++) {
    fputc(Image[i].B - Image[i - 1].B, bmpOutputFile);
    fputc(Image[i].G - Image[i - 1].G, bmpOutputFile);
    fputc(Image[i].R - Image[i - 1].R, bmpOutputFile);
  }

  fclose(inputBmpFile);
  fclose(bmpOutputFile);

  HashTable_t *table = createTable();

  for (int i = 0; i < 7; i ++) {
    tableInsert(table, 0);
  }
  for (int i = 0; i < 12; i ++) {
    tableInsert(table, 1);
  }
  for (int i = 0; i < 15; i ++) {
    tableInsert(table, 2);
  }
  for (int i = 0; i < 18; i ++) {
    tableInsert(table, 3);
  }
  for (int i = 0; i < 20; i ++) {
    tableInsert(table, 4);
  }

  List_t *tableItems = getItems(table);

  Tree_t *finalTree = createTreeFromList(tableItems);
  printTree(finalTree);

  destroyTable(table);
  destroyList(tableItems);
  destroyTree(finalTree);

  return 0;
}
