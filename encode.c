# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include "intStack.h"
# include "queue.h"
# include "huffman.h"


int main(void)
{
  queue *q = newQueue(10);

  for(int i = 1; enqueue(q,newNode('$', true, i)); i = rand() % 45646)
  {
    ;
  }

  treeNode *t;
  while(dequeue(q, &t))
  {
    printf("\tCount is %lu\n", t->count);
    delTree(t);
  }

  delQueue(q);
  return 0;
}
