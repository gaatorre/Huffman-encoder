# ifndef _QUEUE_H
# define _QUEUE_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"


typedef struct queue
{
  uint32_t size; // How big is it?
  uint32_t head, tail; // Front and rear locations
  treeNode **Q; // Array to hold it (via calloc)
} queue;

queue *newQueue(uint32_t size); // Constructor
void delQueue(queue *); // Destructor

bool emptyQ(queue *); // Is it empty?
bool fullQ(queue *); // Is it full?

bool enqueue(queue *, treeNode *); // Add an treeNode
bool dequeue(queue *, treeNode **); // Remove from the rear

# endif
