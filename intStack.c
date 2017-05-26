# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include "stack.h"

// Stack of unisigned ints

intStack *newIntStack()
{
  intStack *s = (intStack *) calloc(1, sizeof(stack));
  s->size = 10;
  s->top = 0;
  s->entries = (item *) calloc(s->size, sizeof(item));
  return s;
}

// deletes a intStack and its contents
void delIntStack(intStack *s)
{
  free(s->entries);
  free(s);
}

// pops the value at the top of the stack
// returns -1 if the intStack is empty
item popInt(intStack *s)
{
  // if the intStack is not empty, return the element at the top
  if(!emptyInt(s))
  {
    item i = s->entries[--s->top];
    s->entries[s->top] = 0; // set the value to zero
    return i;
  }

  return -1;
}

// pushes entries onto the stack
// resizes if the intStack is full
void pushInt(intStack *s, item i)
{
  // if full then the intStack needs to be resized
  if(fullInt(s))
  {
    s->size *= 2;
    s->entries = (item *) realloc((void *)s->entries, sizeof(item) * (s->size));
  }
  // pushes the item to the top of the intStack while incrementing top
  s->entries[s->top++] = i;
  return;
}

// returns 1 if intStack is empty
// 0 otherwise
bool emptyInt(intStack *s)
{
  return s->top == 0;
}

// returns 1 if intStack is full
// 0 otherwise
bool fullInt(intStack *s)
{
  return s->top == s->size;
}
