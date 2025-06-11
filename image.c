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
  printf("bfType: 0x%X\n", header->bfType);
  printf("bfSize: %u bytes\n", header->bfSize);
  printf("bfReserved1: %u\n", header->bfReserved1);
  printf("bfReserved2: %u\n", header->bfReserved2);
  printf("bfOffBits: %u bytes\n", header->bfOffBits);
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

// Carrega a imagem BMP e aplica padding se necessário (compressão com perdas)
void loadBMPImage(FILE *input, BMPINFOHEADER InfoHeader, Pixel *Image, int compressionType) {
  fseek(input, 54, SEEK_SET); // Pula cabeçalhos BMP

  if (compressionType == 1) {
    // Compressão sem perdas: lê imagem como está, sem padding
    int tam = InfoHeader.biHeight * InfoHeader.biWidth;

    for (int i = 0; i < tam; i++){
      Image[i].B = fgetc(input);
      Image[i].G = fgetc(input);
      Image[i].R = fgetc(input);
    }
  } else {
    // Compressão com perdas: garante múltiplos de 8 (necessário para DCT 8x8)
    int newWidth = ceilDiv(InfoHeader.biWidth, BLOCK_SIZE) * BLOCK_SIZE;
    int newHeight = ceilDiv(InfoHeader.biHeight, BLOCK_SIZE) * BLOCK_SIZE;

    int originalRowSize = InfoHeader.biWidth * 3;
    int paddedRowSize = ((originalRowSize + 3) / 4) * 4; // Alinha para múltiplo de 4 bytes
    int paddingSize = paddedRowSize - originalRowSize;

    for (int i = 0; i < newWidth * newHeight; i++) {
      int x = i % newWidth;
      int y = i / newWidth;

      // Se ainda estamos dentro da imagem original, lemos os valores reais
      if (x < InfoHeader.biWidth && y < InfoHeader.biHeight) {
        Image[i].B = fgetc(input);
        Image[i].G = fgetc(input);
        Image[i].R = fgetc(input);

        // Pula o padding do fim da linha, apenas na última coluna original
        if (x == InfoHeader.biWidth - 1) {
          fseek(input, paddingSize, SEEK_CUR);
        }
      } else {
        // Pixels adicionados por padding (fora da imagem original): preenche com preto (0,0,0)
        Image[i].B = 0;
        Image[i].G = 0;
        Image[i].R = 0;
      }
    }
  }
}
