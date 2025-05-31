#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "min-heap.h"
#include "table.h"
#include "channels.h"
#include "block.h"

int main() {
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
                  
  Channels_t* channels = createChannels(Image, infoHeader.biWidth, infoHeader.biHeight);

  Blocks_t* YBlocks = createBlocks(getY(channels), infoHeader.biWidth, infoHeader.biHeight);
  Blocks_t* CbBlocks = createBlocks(getCb(channels), infoHeader.biWidth, infoHeader.biHeight);
  Blocks_t* CrBlocks = createBlocks(getCr(channels), infoHeader.biWidth, infoHeader.biHeight);

  char* outputFilePath = "./output.bmp";
  FILE* bmpOutputFile = fopen(outputFilePath, "wb");
  if(!bmpOutputFile) {
    printf("Error opening output file %s\n", outputFilePath);
    return 1;
  }

  // Reescrevendo o cabeçalho do arquivo BMP
  fseek(inputBmpFile, 0, SEEK_SET);
  for (int i = 0; i < 54; i++) {
    fputc(fgetc(inputBmpFile), bmpOutputFile);
  }

  for (int i = 0; i < (infoHeader.biHeight * infoHeader.biWidth); i++) {
    fputc(getY(channels)[i], bmpOutputFile);
    fputc(getY(channels)[i], bmpOutputFile);
    fputc(getY(channels)[i], bmpOutputFile);
  }

  fclose(inputBmpFile);
  fclose(bmpOutputFile);

  destroyBlocks(YBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyBlocks(CbBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyBlocks(CrBlocks, infoHeader.biWidth, infoHeader.biHeight);
  destroyChannels(channels);

  return 0;
}
