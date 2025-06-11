/*
  NOME: Samuel de Assunção Ferreira
  Nº USP: 12543565
  DISCIPLINA: SSC226 - Multimídia
  PROFESSOR: Rudinei Goularte
  PROJETO: Compressão de Imagens
*/

#include <stdio.h>
#include "file-reader.h"

int main() {
  printf("\nEnter the binary file path: ");
  char inputFilePath[256];
  scanf("%255s", inputFilePath); 
  FILE* inputFile = fopen(inputFilePath, "rb");
  if (!inputFile) {
    printf("Error opening binary input file.\n");
    return 1;
  }

  printf("\nEnter the output BMP file path (SHOULD END WITH .bmp): ");
  char outputFilePath[256];
  scanf("%255s", outputFilePath);

  int compressionType;
  fread(&compressionType, sizeof(int), 1, inputFile);

  if (compressionType == 0) {
    decompressLossless(inputFile, outputFilePath);
  } else if (compressionType == 1) {
    decompressLossy(inputFile, outputFilePath);
  } else {
    printf("Invalid compression type in input file.\n");
    fclose(inputFile);
    return 1;
  }

  printf("\nDecompression completed successfully.\n");
  printf("Output BMP file created at: %s\n", outputFilePath);

  fclose(inputFile);

  return 0;
}
