# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include "treeStack.h"

// Stack of unisigned ints

treeStack *newTreeStack()
{
  treeStack *s = (treeStack *) calloc(1, sizeof(treeStack));
  s->size = 10;
  s->top = 0;
  s->entries = (item *) calloc(s->size, sizeof(item));
  return s;
}

// deletes a treeStackand and its contents
void delTreeStack(treeStack *s)
{
  free(s->entries);
  free(s);
}

// pops the value at the top of the treeStack
// returns NIL if the treeStackis empty
item popTree(treeStack *s)
{
  // if the treeStackis not empty, return the element at the top
  if(!emptyInt(s))
  {
    item i = s->entries[--s->top];
    return i;
  }

  return NIL;
}

// pushes entries onto the treeStack
// resizes if the treeStackis full
void pushTree(treeStack *s, item i)
{
  // if full then the treeStackneeds to be resized
  if(fullTree(s))
  {
    s->size *= 2;
    s->entries = (item *) realloc( (void *)s->entries, sizeof(item) * (s->size) );
  }
  // pushes the item to the top of the treeStackwhile incrementing top
  s->entries[s->top++] = i;
  return;
}

// returns 1 if treeStackis empty
// 0 otherwise
bool emptyTree(treeStack *s)
{
  return s->top == 0;
}

// returns 1 if treeStackis full
// 0 otherwise
bool fullTree(treeStack *s)
{
  return s->top == s->size;
}
