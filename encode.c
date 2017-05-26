# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
// # include "intStack.h"
# include "queue.h"

# define MAX 10

int main(void)
{
  uint32_t x = 0;
  queue *q = newQueue(10);
  for(int i = 1; enqueue(q, rand() % 10000); i++)
  {
    ;
  }
  dequeue(q, &x);
  dequeue(q, &x);
  dequeue(q, &x);
  enqueue(q, rand() % 10000);
  enqueue(q, rand() % 10000);
  dequeue(q, &x);
  dequeue(q, &x);
  dequeue(q, &x);
  dequeue(q, &x);
  enqueue(q, rand() % 10000);
  enqueue(q, rand() % 10000);
  dequeue(q, &x);


  while(dequeue(q, &x))
  {
    printf("%d\n", x);
  }

  return 0;
}
