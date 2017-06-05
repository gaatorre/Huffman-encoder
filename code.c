# include "code.h"
# include <stdlib.h>

# define INCREMENT 8000

uint32_t appendCode(code C, bitV *bv)
{
	static uint32_t position = 0;
	// printf("Before Appending code\n");
	// printVec(bv);
	while(position + C.l > bv->length)
	{
		bv->length += INCREMENT;
		uint8_t *temp = (uint8_t *) realloc(bv->vector, bv->length * (sizeof(uint8_t)));
		if(temp == NULL)
		{
			printf("ERROR IN REALLOC\n");
			exit(1);
		}
		else
		{
			bv->vector = temp;
		}
		for (uint32_t i = position; i < bv->length; i++)
	  {
	    clrBit(bv, i);
	  }
		// printf("\nafter realloc\n");
		// printVec(bv);
	}
	for (uint32_t i = position; (i - position) < C.l; i++)
	{
		if (C.bits[(i - position)/8] & (0x1 << ((i - position) % 8)))
		{
			setBit(bv, i);
		}
	}
	// printf("\nafter appendCode\n");
	// printVec(bv);
	position += C.l;
	return position;
}
