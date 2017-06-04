// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include "queue.h"
# include "code.h"
// taken from Piazza examples. Credit to Prof. Long

// Constructor
queue *newQueue(uint32_t size)
{
  queue *q = (queue *) calloc(1, sizeof(queue));
  q->size = size + 1;
  q->head = 0;
  q->tail = 0;
  q->Q = (treeNode **) calloc(size +1, sizeof(treeNode *));
  return q;
  // p->Q[size] = NULL;
}

void delQueue(queue *q) // Destructor
{
  delTree(q->Q[q->head - 1]);
  free(q->Q);
  free(q);
}

// Is it empty?
bool emptyQ(queue *q)
{
  return q->head % q->size == q->tail % q->size;
}

// Is it full?
bool fullQ(queue *q)
{
  return (q->head + 1) % q->size == (q->tail) % q->size;
}

// uses insertion sort to move everything over
// Add an treeNode
bool enqueue(queue *q, treeNode *i)
{
  if(fullQ(q))
  {
    return false;
  }
  else
  {
    uint32_t index = q->head;
    uint32_t size = q->size;

    while(index > q->tail && compare((q->Q[(index -1) % size]), i) > 0)
    {
      q->Q[index % size] = q->Q[(index -1) % size];
      index--;
    }
    q->Q[index % size] = i;
		q->head = q->head + 1; // updates the pointer by one

    return true;
  }
}

// Remove from the rear
bool dequeue(queue *q, treeNode **i)
{
  if (emptyQ(q))
	{
		return false;
	}
	else
	{
		*i = q->Q[q->tail];
    // q->Q[q->tail] = NIL;
		q->tail = (q->tail + 1) % q->size;
		return true;
	}
}
