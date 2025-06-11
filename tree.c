#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "list.h"
#include "tree.h"
#include "min-heap.h"
#include "codes-table.h"

Tree_t* createTree(int value, int frequence) {
  Tree_t* tree = (Tree_t*) malloc(sizeof(Tree_t));

  TreeNode_t *root = (TreeNode_t*) malloc(sizeof(TreeNode_t));
  root->value = value;
  root->frequence = frequence;
  root->childLeft = NULL;
  root->childRight = NULL;

  tree->root = root;

  return tree;
}

Tree_t* createTreeFromList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) at createTreeFromList.\n");
    return NULL;
  }

  MinHeap_t* heap = createHeap();

  ListNode_t *aux = list->first;
  while (aux != NULL) {
    Tree_t *treeAux = createTree(aux->value, aux->frequence);
    heapInsert(heap, treeAux);
    aux = aux->next;
  }

  while (heapSize(heap) > 1) {
    Tree_t* smallest = extractMin(heap);
    Tree_t* secondSmallest = extractMin(heap);
    Tree_t* merged = mergeTrees(smallest, secondSmallest);
    heapInsert(heap, merged);
  }

  Tree_t *tree = extractMin(heap);

  return tree;
}

void printTreeUtil(TreeNode_t* root) {
  if (root == NULL) {
    return;
  }

  printf("(%d, %d) -> ", root->value, root->frequence);
  if (root->childLeft == NULL) printf("NULL, ");
  else printf("(%d, %d), ", root->childLeft->value, root->childLeft->frequence);
  if (root->childRight == NULL) printf("NULL\n");
  else printf("(%d, %d)\n", root->childRight->value, root->childRight->frequence);

  printTreeUtil(root->childLeft);
  printTreeUtil(root->childRight);
}

void writeTreeToFileUtil(TreeNode_t *root, FILE *file) {
  if (root == NULL) {
    unsigned char nullMarker = 0xFF;
    fwrite(&nullMarker, sizeof(unsigned char), 1, file);
    return;
  }

  unsigned char isLeaf = (root->childLeft == NULL && root->childRight == NULL) ? 1 : 0;
  fwrite(&isLeaf, sizeof(unsigned char), 1, file);

  if (isLeaf) {
    fwrite(&root->value, sizeof(int), 1, file);
  }

  writeTreeToFileUtil(root->childLeft, file);
  writeTreeToFileUtil(root->childRight, file);
}

void writeTreeToFile(Tree_t *root, FILE *file) {
  if (root == NULL || file == NULL) {
    printf("Invalid tree or file pointer at writeTreeToFile.\n");
    return;
  }

  writeTreeToFileUtil(root->root, file);
}

void printTree(Tree_t* tree) {
  if (tree == NULL) {
    return;
  }

  printTreeUtil(tree->root);
}

void generateCodesTableUtil(TreeNode_t* root, char* code, int index, CodesTable_t* table) {
  if (root == NULL) {
    return;
  }

  if (root->childLeft == NULL && root->childRight == NULL) {
    code[index] = '\0';
    SymbolCode_t *item = tableCodesSearch(table, root->value);
    if (item == NULL) {
      printf("Error: SymbolCode not found for value %d\n", root->value);
      return;
    }
    
    item->code = (char*)malloc(index * sizeof(char));
    if (item->code == NULL) {
      printf("Memory allocation failed for code.");
      return;
    }
    strncpy(item->code, code, index);

    item->code = strdup(code);
    item->codeSize = index;

    return;
  }

  code[index] = '0';
  generateCodesTableUtil(root->childLeft, code, index + 1, table);

  code[index] = '1';
  generateCodesTableUtil(root->childRight, code, index + 1, table);
}

CodesTable_t* generateCodesTable(HashTable_t* frequencesTable, List_t* items, Tree_t* tree) {
  CodesTable_t *codesTable = createCodesTable();
  
  ListNode_t *aux = items->first;
  while (aux != NULL) {
    tableCodesInsert(codesTable, aux->value);
    aux = aux->next;
  }

  generateCodesTableUtil(tree->root, (char*) malloc(100 * sizeof(char)), 0, codesTable);

  return codesTable;
}

void destroyTreeUtil(TreeNode_t* root) {
  if (root == NULL) {
    return;
  }

  destroyTreeUtil(root->childLeft);
  destroyTreeUtil(root->childRight);
  free(root);
}

void destroyTree(Tree_t* tree) {
  if (tree == NULL) {
    return;
  }

  destroyTreeUtil(tree->root);
  free(tree);
}

Tree_t* mergeTrees(Tree_t* a, Tree_t* b) {
  if (a == NULL || b == NULL) {
    printf("Invalid trees (NULL) for mergeTrees.");
    return NULL;
  }

  if (a->root == NULL || b->root == NULL) {
    printf("Invalid roots (NULL) for mergeTrees.");
    return NULL;
  }

  int value = EMPTY_DIFFERENCE;
  int frequence = a->root->frequence + b->root->frequence;

  Tree_t* newTree = createTree(value, frequence);
  newTree->root->childLeft = a->root;
  newTree->root->childRight = b->root;

  a->root = NULL;
  b->root = NULL;

  a = NULL;
  b = NULL;

  return newTree;
}
