// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include "huffman.h"
# include "code.h"
# include "treeStack.h"


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
void dumpTree(treeNode *t, int file)
{
  static char interNode = 'I';
  static char leaf = 'L';
  if(t == NIL)
  {
    return;
  }
  dumpTree(t->left, file);
  dumpTree(t->right, file);

  if(t->leaf)
  {
    // printf("L%c", t->symbol);
    write(file, &leaf, sizeof(leaf));
    write(file, &t->symbol, sizeof(t->symbol));
  }
  else
  {
    // printf("I");
    write(file, &interNode, sizeof(interNode));
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
    // printf("I am a LEAF! %c: ", t->symbol);
    // printCode(&s);
    memcpy((void *)&(table[t->symbol]), &s, sizeof(code));
    popCode(&s, &g);
    // printf("\n");
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

//Load from a given tree
treeNode *loadTree(uint8_t savedTree[], uint16_t treeSize)
{
    treeStack *ts = newTreeStack();
    for (uint16_t i = 0; i < treeSize; i++)
    {
        if (savedTree[i] == 'L')
        {
            pushTree(ts, newNode(savedTree[++i], true, 0));
        }
        else if (savedTree[i] == 'I')
        {
            treeNode *right = popTree(ts);
            treeNode *left = popTree(ts);
            pushTree(ts, join(left, right));
        }
    }
    treeNode *output = popTree(ts);
    //delTreeStack(ts);
    return output;
}

// Step through a tree following the code
int32_t stepTree(treeNode *root, treeNode **t, uint32_t code)
{
    if (code == 0)
    {
        t = &((*t)->left);
    }
    else if (code == 1)
    {
        t = &((*t)->right);
    }
    if ((*t)->leaf)
    {
        int32_t out = (int32_t)((*t)->symbol);
        t = &root;
        return out;
    }
    return -1;
}
