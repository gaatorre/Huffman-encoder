// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "huffman.h"
# include "code.h"
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
void delTree(treeNode *t)
{
  // Has reached the end of the sublist
  if(t == NIL)
  {
    return;
  }

  delTree(t->left); // delete the left side
  delTree(t->right); // delete the right side
  delNode(t); // free the node
  return;
}

// Dump a Huffman tree onto a file
void dumpTree(treeNode *t, FILE *file)
{
  if(t == NIL)
  {
    return;
  }
  dumpTree(t->left, file);
  dumpTree(t->right, file);

  if(t->leaf)
  {
    printf("L%c", t->symbol);
    fputc('L', file);
    fputc(t->symbol, file);
  }
  else
  {
    printf("I");
    fputc('I', file);
  }
  return;
}

void buildCode(treeNode *t, code s, code table[256])
{
  uint32_t g;
  if(t->left)
  {
    pushCode(&s, 0);
    buildCode(t->left, s, table);
    popCode(&s, &g);
  }
  if(t->right)
  {
    pushCode(&s, 1);
    buildCode(t->right, s, table);
    popCode(&s, &g);
  }
  if(t->leaf)
  {
    printf("I am a LEAF! %c: ", t->symbol);
    printCode(&s);
    memcpy((void *)&(table[t->symbol]), &s, sizeof(code));
    popCode(&s, &g);
    printf("\n");
  }

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
