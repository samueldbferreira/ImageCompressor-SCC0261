#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "file-reader.h"

void initBitReader(BitReader *reader, FILE *file) {
  reader->file = file;
  reader->buffer = 0;
  reader->bitCount = 0;
}

int readBit(BitReader *reader) {
  if (reader->bitCount == 0) {
    if (fread(&reader->buffer, 1, 1, reader->file) != 1) {
      return -1; // Erro ou EOF
    }
    reader->bitCount = 8;
  }
  
  int bit = (reader->buffer >> (reader->bitCount - 1)) & 1;
  reader->bitCount--;
  return bit;
}

TreeNode_t* readHuffmanTree(BitReader *reader) {
  int bit = readBit(reader);
  if (bit != 1) return NULL; // Nó nulo ou erro
  
  bit = readBit(reader);
  if (bit == -1) return NULL;
  
  TreeNode_t *node = (TreeNode_t*)malloc(sizeof(TreeNode_t));

  if (bit == 1) { // É folha
    node->difference = 0;
    for (int i = 0; i < 32; i++) {
      bit = readBit(reader);
      if (bit == -1) {
        free(node);
        return NULL;
      };
      node->difference = (node->difference << 1) | bit;
    }
    node->frequence = 0;

    node->childLeft = NULL;
    node->childRight = NULL;
  } else { // É nó interno
    node->difference = 0;
    for (int i = 0; i < 32; i++) {
      bit = readBit(reader);
      if (bit == -1) {
        free(node);
        return NULL;
      };
      node->difference = (node->difference << 1) | bit;
    }
    node->frequence = 0;

    node->childLeft = readHuffmanTree(reader);
    node->childRight = readHuffmanTree(reader);
    if (!node->childLeft || !node->childRight) {
      destroyTreeUtil(node); // Implemente esta função
      return NULL;
    }
  }
  return node;
}

void readBinary() {
  FILE* file = fopen("output.bin", "rb");
  if (!file) {
    printf("Error opening binary file.\n");
  }

  int readedHeight;
  fread(&readedHeight, sizeof(int), 1, file);
  int readedWidth;
  fread(&readedWidth, sizeof(int), 1, file);
  printf("\n\nReaded Height: %d, Width: %d\n", readedHeight, readedWidth);

  int numberOfSymbols;
  fread(&numberOfSymbols, sizeof(int), 1, file);
  printf("Number of symbols in the Huffman tree: %d\n", numberOfSymbols);

  BitReader reader;
  initBitReader(&reader, file);
  TreeNode_t *readedTreeRoot = readHuffmanTree(&reader);

  Tree_t *arvoreLida = (Tree_t*)malloc(sizeof(Tree_t));
  arvoreLida->root = readedTreeRoot;
  printTree(arvoreLida);

  fclose(file);

  destroyTree(arvoreLida);
}
