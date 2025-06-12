/*
  NOME: Samuel de Assunção Ferreira
  Nº USP: 12543565
  DISCIPLINA: SSC226 - Multimídia
  PROFESSOR: Rudinei Goularte
  PROJETO: Compressão de Imagens
  REPOSITÓRIO: https://github.com/samueldbferreira/ImageCompressor-SCC0261
*/

#include <stdio.h>
#include "file-reader.h"

/*
  Função principal do descompressor de imagens.

  - Lê arquivo binário gerado pela compressão
  - Identifica o tipo de compressão utilizada:
    - Modo 0 → Sem perdas: diferenças + Huffman
    - Modo 1 → Com perdas: YCbCr, DCT, quantização, Huffman

  Etapas principais:
  1. Leitura do cabeçalho e árvore de Huffman;
  2. Decodificação dos dados comprimidos;
  3. Reconstrução dos blocos e conversão para RGB (modo 1);
  4. Escrita da imagem BMP final.

  Ao final, a imagem descomprimida é salva no caminho indicado.
*/

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
