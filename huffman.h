# ifndef _HUFFMAN_H
# define _HUFFMAN_H
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <ctype.h>
# include "code.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
  uint8_t symbol;
  uint64_t count;
  bool leaf;
  treeNode *left, *right;
};

// New node, with symbols, leaf or not, a count associated with it
treeNode *newNode(uint8_t s, bool l, uint64_t c);

// Delete a tree
void delTree(treeNode *t);

// // Dump a Huffman tree onto a file
void dumpTree(treeNode *t, int file);

// // Build a tree from a saved tree
// treeNode *loadTree(uint8_t savedTree[], uint16_t treeBytes);
//
// // Step through a tree following the code
// int32_t *stepTree(treeNode *root, treeNode **t, uint32_t code);

//Parase a Huffman tree to build codes
void buildCode(treeNode *t, code s, code table[256]);

static inline void delNode(treeNode *h) { free(h); return; }

static inline int32_t compare(treeNode *l, treeNode *r)
{
  return l->count - r->count; // l < r if negative, 1 = r if 0, ...
}

static inline void spaces(int c) { for (int i = 0; i < c; i += 1) { putchar(' '); } return; }

void printTree(treeNode *t, int depth);

// Join two subtrees
treeNode *join(treeNode *l, treeNode *r);

# endif
