#include <stdio.h>
#include <stdlib.h>
#include "file-writer.h"
#include "image.h"

void initBitWriter(BitWriter *writer, FILE *file) {
  writer->file = file;
  writer->buffer = 0;
  writer->bitCount = 0;
}

void writeBit(BitWriter *writer, int bit) {
  writer->buffer = (writer->buffer << 1) | (bit & 1);
  writer->bitCount++;
    
  if (writer->bitCount == 8) {
    fwrite(&writer->buffer, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

void flushBits(BitWriter *writer) {
  if (writer->bitCount > 0) {
    writer->buffer <<= (8 - writer->bitCount);
    fwrite(&writer->buffer, 1, 1, writer->file);
    writer->buffer = 0;
    writer->bitCount = 0;
  }
}

void writeHuffmanTree(BitWriter *writer, TreeNode_t *node) {
  if (node == NULL) {
    writeBit(writer, 0); // Marca nó nulo
    return;
  }
  
  writeBit(writer, 1); // Marca que existe nó
  
  if (node->childLeft == NULL && node->childRight == NULL) {
    writeBit(writer, 1); // Marca como folha
    // Escreve a diferença (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->difference >> i) & 1);
    }
  } else {
    writeBit(writer, 0); // Marca como nó interno
    // Escreve a diferença (32 bits)
    for (int i = 31; i >= 0; i--) {
      writeBit(writer, (node->difference >> i) & 1);
    }

    writeHuffmanTree(writer, node->childLeft);
    writeHuffmanTree(writer, node->childRight);
  }
}

void writeIntAsBits(BitWriter* writer, int value) {
  for (int i = 0; i < 32; i++) {
    writeBit(writer, (value >> (31 - i)) & 1);
  }
}

void writeBinaryFile(int width, int height, int numSymbols, Tree_t *tree, Pixel *differences, CodesTable_t* codesTable) { 
  FILE* file = fopen("./output.bin", "wb");
  if (!file) {
    printf("Error opening output file.\n");
    return;
  }

  fwrite(&height, sizeof(int), 1, file);
  fwrite(&width, sizeof(int), 1, file);
  fwrite(&numSymbols, sizeof(int), 1, file);

  BitWriter writer;
  initBitWriter(&writer, file);
  writeHuffmanTree(&writer, tree->root);

  writeIntAsBits(&writer, differences[0].B);
  writeIntAsBits(&writer, differences[0].G);
  writeIntAsBits(&writer, differences[0].R);

  for (int i = 1; i < (width * height); i++) {
    SymbolCode_t* B = tableCodesSearch(codesTable, differences[i].B);
    for (int j = 0; j < B->codeSize; j++) {
      writeBit(&writer, B->code[j] - '0');
    }

    SymbolCode_t* G = tableCodesSearch(codesTable, differences[i].G);
    for (int j = 0; j < G->codeSize; j++) {
      writeBit(&writer, G->code[j] - '0');
    }

    SymbolCode_t* R = tableCodesSearch(codesTable, differences[i].R);
    for (int j = 0; j < R->codeSize; j++) {
      writeBit(&writer, R->code[j] - '0');
    }
  }

  flushBits(&writer);
  fclose(file);
}
