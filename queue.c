// # include <stdint.h>
// # include <stdbool.h>
# include <stdlib.h>
# include "queue.h"

// taken from Piazza examples. Credit to Prof. Long

// Constructor
queue *newQueue(uint32_t size)
{
  queue *q = (queue *) calloc(1, sizeof(queue));
  q->size = size + 1;
  q->head = 0;
  q->tail = 0;
  q->Q = (item *) calloc(size +1, sizeof(item));
  return q;
  // p->Q[size] = NULL;
}
void delQueue(queue *q) // Destructor
{
  free(q->Q);
  free(q);
}

// Is it empty?
bool empty(queue *q)
{
  return q->head % q->size == q->tail % q->size;
}

// Is it full?
bool full(queue *q)
{
  return (q->head + 1) % q->size == (q->tail) % q->size;
}

// uses insertion sort to move everything over
// Add an item
bool enqueue(queue *q, item i)
{
  if(full(q))
  {
    return false;
  }
  else
  {
    uint32_t index = q->head;
    uint32_t size = q->size;
    while(index > q-> tail && q->Q[(index -1) % size] > i)
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
bool dequeue(queue *q, item *i)
{
  if (empty(q))
	{
		return false;
	}
	else
	{
		*i = q->Q[q->tail];
		q->tail = (q->tail + 1) % q->size;
		return true;
	}
}
