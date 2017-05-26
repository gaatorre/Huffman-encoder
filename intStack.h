# ifndef _INTSTACK_H
# define _INTSTACK_H
# include <stdint.h>
# include <stdbool.h>


typedef struct intStack
{
  uint32_t size; // How big?
  uint32_t top; // Where's the top?
  uint32_t *entries; // Array to hold it (via calloc)
} intStack;

intStack *newIntStack(); // Constructor
void delIntStack(intStack *); // Destructor

uint32_t popInt(intStack *); // Returns the top uint32_t
void pushInt(intStack *, uint32_t); // Adds an uint32_t to the top

bool emptyInt(intStack *); // Is it empty?
bool fullInt(intStack *); // Is it full?

# endif
