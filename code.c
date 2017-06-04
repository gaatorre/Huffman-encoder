# include "code.h"
# include <stdlib.h>

# define INCREMENT 8000

uint32_t appendCode(code C, bitV *bv)
{
	static uint32_t position = 0;
	while (position + C.l > bv->length)
	{
		bv->length += INCREMENT;
		bv->vector = (uint8_t *)realloc(bv->vector, bv->length * (sizeof(uint8_t)));
	}
	for (uint32_t i = position; (i - position) < C.l; i++)
	{
		if (C.bits[(i - position)/8] & (0x1 << ((i - position) % 8)))
		{
			setBit(bv, i);
		}
	}
	position += C.l;
	return position;
}
