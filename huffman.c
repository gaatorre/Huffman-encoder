// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include "huffman.h"
# include <stdio.h>

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
  treeNode *t = (treeNode *) calloc(1, sizeof(treeNode));
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

void printTree(treeNode *t, int depth)
{
  if (t && t->leaf)
  {
    if (isalnum(t->symbol))
    {
            spaces(4 * depth); printf("%c (%lu)\n", t->symbol, t->count);
    }
    else
    {
            spaces(4 * depth); printf("%X (%lu)\n", t->symbol, t->count);
    }
  }
  else if (t)
  {
    spaces(4 * depth); printf("$ (%lu)\n", t->count);
    printTree(t->left, depth + 1);
    printTree(t->right, depth + 1);
  }
  return;
}

// Join two subtrees
treeNode *join(treeNode *l, treeNode *r)
{
  treeNode *t = newNode('$', false, l->count + r->count);
  t->left = l;
  t->right = r;
  return t;
}
