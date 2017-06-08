# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include "treeStack.h"

// Stack of tree nodes

treeStack *newTreeStack()
{
  treeStack *s = (treeStack *) calloc(1, sizeof(treeStack));
  s->size = 10;
  s->top = 0;
  s->entries = (treeNode **) calloc(s->size, sizeof(treeNode *));
  return s;
}

// deletes a treeStack and the array of entries
//but not the nodes themselves because that will be deleted
//once we delete the tree
void delTreeStack(treeStack *s)
{
    free(s->entries);
    free(s);
    return;
}

// pops the value at the top of the treeStack
// returns NIL if the treeStackis empty
treeNode *popTree(treeStack *s)
{
  // if the treeStackis not empty, return the element at the top
  //and decreases the value of top
  if(!emptyTree(s))
  {
    treeNode *i = s->entries[--s->top];
    return i;
  }

  return NIL;
}

// pushes entries onto the treeStack
// resizes if the treeStackis full
void pushTree(treeStack *s, treeNode *i)
{
  // if full then the treeStackneeds to be resized
  if(fullTree(s))
  {
    s->size *= 2;
    s->entries = (treeNode **) realloc( (void **)s->entries, sizeof(treeNode *) * (s->size) );
  }
  // pushes the treeNode to the top of the treeStackwhile incrementing top
  s->entries[s->top++] = i;
  return;
}

// returns true if treeStackis empty
// false otherwise
bool emptyTree(treeStack *s)
{
  return s->top == 0;
}

// returns true if treeStackis full
// false otherwise
bool fullTree(treeStack *s)
{
  return s->top == s->size;
}
