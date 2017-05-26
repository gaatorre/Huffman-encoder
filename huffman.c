# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
  treeNode *t = (treeNode *) malloc(sizeof(treeNode));
  t->symbol = s;
  t->count = c;
  t->leaf = l;
  t->left = NIL;
  t->right = NIL;
}

// void *delTree(treeNode *s)
// {
//
// }
