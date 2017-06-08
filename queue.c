# include <stdlib.h>
# include "queue.h"
# include "code.h"
// taken from Piazza examples. Credit to Prof. Long

// Constructor
queue *newQueue(uint32_t size)
{
  queue *q = (queue *) calloc(1, sizeof(queue));
  // size is one more than specified for a check on fullness
  q->size = size + 1;
  q->head = 0;
  q->tail = 0;
  q->Q = (treeNode **) calloc(size +1, sizeof(treeNode *));
  return q;
}

// Destructor
void delQueue(queue *q)
{
  // Deletes the last node that was pushed which is always the root
  delTree(q->Q[q->head - 1]);
  free(q->Q);
  free(q);
}

// Is it empty?
bool emptyQ(queue *q)
{
  // If the head and tail point at the same index then it is empty
  return q->head % q->size == q->tail % q->size;
}

// Is it full?
bool fullQ(queue *q)
{
  // If the head is pointing to the only empty slot available then it returns true
  return (q->head + 1) % q->size == (q->tail) % q->size;
}

// Add an treeNode and uses insertion sort to move everything over
bool enqueue(queue *q, treeNode *i)
{
  // if the queue is full then nothing can be inserted so it returns false
  if(fullQ(q))
  {
    return false;
  }
  // else the queue is not full so it inserts the node
  else
  {
    uint32_t index = q->head;
    uint32_t size = q->size;

    // moves the values over to make room for the node
    // starting at the end, nodes get moved over if they are greater than it
    // and the node is inserted in its proper location -- insertion sort
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
  // you can't remove from the rear if its empty
  if (emptyQ(q))
	{
		return false;
	}
  // if it's not empty then update the tail by one
	else
	{
		*i = q->Q[q->tail];
		q->tail = (q->tail + 1) % q->size;
		return true;
	}
}
