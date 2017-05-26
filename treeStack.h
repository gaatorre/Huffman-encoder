# ifndef _TREESTACK_H
# define _TREESTACK_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct treeStack
{
  uint32_t size; // How big?
  uint32_t top; // Where's the top?
  treeNode *entries; // Array to hold it (via calloc)
} treeStack;

treeStack *newTreeStack(); // Constructor
void delTreeStack(treeStack *); // Destructor

treeNode popTree(treeStack *); // Returns the top item
void pushTree(treeStack *, treeNode); // Adds an item to the top

bool emptyTree(treeStack *); // Is it empty?
bool fullTree(treeStack *); // Is it full?

# endif
