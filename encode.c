# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include "intStack.h"
# include "treeStack.h"
# include "queue.h"
# include "huffman.h"


int main(void)
{
  treeStack *t = newTreeStack();
  for(int x = 1; x <= 30; x++)
  {
    pushTree(t, newNode('$',true, x));
  }
  treeNode *test;
  while((test = popTree(t)) != NIL)
  {
    printf("%lu\n", test->count);
  }
  delTreeStack(t);
  return 0;
}
