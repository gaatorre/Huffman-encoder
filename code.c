# include "code.h"
# include <stdlib.h>

# define INCREMENT 8000

//This code given to us by Darrell Long

uint32_t appendCode(code C, bitV *bv)
{
	static uint32_t position = 0; // keeps track of where to insert bits
	// reallocs if the bit vector is too small
	while(position + C.l > bv->length)
	{
		bv->length += INCREMENT;
		uint8_t *temp = (uint8_t *) realloc(bv->vector, bv->length * (sizeof(uint8_t)));
		// error handling
		if(temp == NULL)
		{
			printf("ERROR IN REALLOC\n");
			exit(1);
		}
		else
		{
			bv->vector = temp;
		}
		// realloc does not zero out the new memory allocated so we must do it ourselves
		for (uint32_t i = position; i < bv->length; i++)
	  {
	    clrBit(bv, i);
	  }
	}
	// appends the code to the bitVector
	for (uint32_t i = position; (i - position) < C.l; i++)
	{
		// if the bit is a 1, set the bit in the bit vector
		if (C.bits[(i - position)/8] & (0x1 << ((i - position) % 8)))
		{
			setBit(bv, i);
		}
	}
	// updates the position
	position += C.l;
	return position;
}
