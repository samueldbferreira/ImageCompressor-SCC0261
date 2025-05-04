#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

Tree_t* createTree(int difference, int frequence) {
  Tree_t* tree = (Tree_t*) malloc(sizeof(Tree_t));

  TreeNode_t *root = (TreeNode_t*) malloc(sizeof(TreeNode_t));
  root->difference = difference;
  root->frequence = frequence;
  root->childLeft = NULL;
  root->childRight = NULL;

  tree->root = root;

  return tree;
}

void printTreeUtil(TreeNode_t* root) {
  if (root == NULL) {
    return;
  }

  printf("(%d, %d) -> ", root->difference, root->frequence);
  if (root->childLeft == NULL) printf("NULL, ");
  else printf("(%d, %d), ", root->childLeft->difference, root->childLeft->frequence);
  if (root->childRight == NULL) printf("NULL\n");
  else printf("(%d, %d)\n", root->childRight->difference, root->childRight->frequence);

  printTreeUtil(root->childLeft);
  printTreeUtil(root->childRight);
}

void printTree(Tree_t* tree) {
  if (tree == NULL) {
    return;
  }

  printTreeUtil(tree->root);
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

  int difference = EMPTY_DIFFERENCE;
  int frequence = a->root->frequence + b->root->frequence;

  Tree_t* newTree = createTree(difference, frequence);
  newTree->root->childLeft = a->root;
  newTree->root->childRight = b->root;

  a->root = NULL;
  b->root = NULL;

  a = NULL;
  b = NULL;

  return newTree;
}
