
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "block.h"

void leituraHeader(FILE *F, BMPFILEHEADER *H) {
  fread(&H->bfType, sizeof(unsigned short), 1, F);
  fread(&H->bfSize, sizeof(unsigned int), 1, F);
  fread(&H->bfReserved1, sizeof(unsigned short), 1, F);
  fread(&H->bfReserved2, sizeof(unsigned short), 1, F);
  fread(&H->bfOffBits, sizeof(unsigned int), 1, F);
}

void leituraInfoHeader(FILE *F, BMPINFOHEADER *H) {
  fread(&H->biSize, sizeof(unsigned int), 1, F);
  fread(&H->biWidth, sizeof(int), 1, F);
  fread(&H->biHeight, sizeof(int), 1, F);
  fread(&H->biPlanes, sizeof(unsigned short), 1, F);
  fread(&H->biBitCount, sizeof(unsigned short), 1, F);
  fread(&H->biCompression, sizeof(unsigned int), 1, F);
  fread(&H->biSizeImage, sizeof(unsigned int), 1, F);
  fread(&H->biXPelsPerMeter, sizeof(int), 1, F);
  fread(&H->biYPelsPerMeter, sizeof(int), 1, F);
  fread(&H->biClrUsed, sizeof(unsigned int), 1, F);
  fread(&H->biClrImportant, sizeof(unsigned int), 1, F);
}

void printFileHeader(BMPFILEHEADER *header) {
  printf("File Header:\n");
  printf("bfType: 0x%X\n", header->bfType);  // Deve ser 'BM' (0x4D42 em little-endian)
  printf("bfSize: %u bytes\n", header->bfSize);
  printf("bfReserved1: %u\n", header->bfReserved1);  // Deve ser 0
  printf("bfReserved2: %u\n", header->bfReserved2);  // Deve ser 0
  printf("bfOffBits: %u bytes\n", header->bfOffBits);  // Offset para os dados de pixel
}

void printInfoHeader(BMPINFOHEADER *header) {
  printf("\nInfo Header:\n");
  printf("biSize: %u\n", header->biSize);
  printf("biWidth: %d\n", header->biWidth);
  printf("biHeight: %d\n", header->biHeight);
  printf("biPlanes: %u\n", header->biPlanes);
  printf("biBitCount: %u\n", header->biBitCount);
  printf("biCompression: %u\n", header->biCompression);
  printf("biSizeImage: %u\n", header->biSizeImage);
  printf("biXPelsPerMeter: %d\n", header->biXPelsPerMeter);
  printf("biYPelsPerMeter: %d\n", header->biYPelsPerMeter);
  printf("biClrUsed: %u\n", header->biClrUsed);
  printf("biClrImportant: %u\n", header->biClrImportant);
}

void loadBMPImage(FILE *input, BMPINFOHEADER InfoHeader, Pixel *Image, int compressionType) {
  fseek(input, 54, SEEK_SET);

  if (compressionType == 1) {
    int tam = InfoHeader.biHeight * InfoHeader.biWidth;

    for (int i = 0; i < tam; i++){
      Image[i].B = fgetc(input);
      Image[i].G = fgetc(input);
      Image[i].R = fgetc(input);
    }
  } else {
    int newWidth = ceilDiv(InfoHeader.biWidth, BLOCK_SIZE) * BLOCK_SIZE;
    int newHeight = ceilDiv(InfoHeader.biHeight, BLOCK_SIZE) * BLOCK_SIZE;

    int originalRowSize = InfoHeader.biWidth * 3;
    int paddedRowSize = ((originalRowSize + 3) / 4) * 4;
    int paddingSize = paddedRowSize - originalRowSize;

    for (int i = 0; i < newWidth * newHeight; i++) {
      int x = i % newWidth;
      int y = i / newWidth;

      if (x < InfoHeader.biWidth && y < InfoHeader.biHeight) {
        Image[i].B = fgetc(input);
        Image[i].G = fgetc(input);
        Image[i].R = fgetc(input);

        if (x == InfoHeader.biWidth - 1) {
          fseek(input, paddingSize, SEEK_CUR);
        }
      } else {
        Image[i].B = 0;
        Image[i].G = 0;
        Image[i].R = 0;
      }
    }
  }
}
