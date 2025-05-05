#ifndef TREE_H 
#define TREE_H

#include "list.h"

#define EMPTY_DIFFERENCE 999;

typedef struct TreeNode {
  int difference;
  int frequence;
  struct TreeNode* childLeft;
  struct TreeNode* childRight;
} TreeNode_t;

typedef struct Tree {
  TreeNode_t* root;
} Tree_t;

Tree_t* createTree(int difference, int frequence);

Tree_t* createTreeFromList(List_t* list);

Tree_t* mergeTrees(Tree_t* a, Tree_t* b);

void printTree(Tree_t* tree);

void destroyTree(Tree_t* tree);

#endif
