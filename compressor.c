#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "list.h"

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

  return 0;
}
