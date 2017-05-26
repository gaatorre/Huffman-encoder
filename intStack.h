# ifndef _INTSTACK_H
# define _INTSTACK_H
# include <stdint.h>
# include <stdbool.h>

typedef uint32_t item;

typedef struct intStack
{
  uint32_t size; // How big?
  uint32_t top; // Where's the top?
  item *entries; // Array to hold it (via calloc)
} intStack;

intStack *newIntStack(); // Constructor
void delIntStack(intStack *); // Destructor

item popInt(intStack *); // Returns the top item
void pushInt(intStack *, item); // Adds an item to the top

bool emptyInt(intStack *); // Is it empty?
bool fullInt(intStack *); // Is it full?

# endif
