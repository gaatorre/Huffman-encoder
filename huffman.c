// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include "huffman.h"
# include <stdio.h>

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
  treeNode *t = (treeNode *) malloc(sizeof(treeNode));
  t->symbol = s;
  t->count = c;
  t->leaf = l;
  t->left = NIL;
  t->right = NIL;
  return t;
}

// deletes the complete tree
void delTree(treeNode *s)
{
  // Has reached the end of the sublist
  if(s == NIL)
    return;

  delTree(s->left); // delete the left side
  delTree(s->right); // delete the right side
  delNode(s); // free the node
  return;
}
