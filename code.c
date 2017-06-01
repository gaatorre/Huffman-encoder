# include "code.h"
# include <stdlib.h>

# define INCREMENT 8000
void appendCode(code C, bitV *bv)
{
	static uint32_t position = 0;
	while ((bv->length) + C.l > bv->length)
	{
		bv->length += INCREMENT;
		bv->vector = realloc(bv->vector, bv->length*(sizeof(uint8_t)));
	}
	for (uint32_t i = position; (i - position) < C.l; i++)
	{
		if (C.bits[i/8] & (0x1 << (C.l % 8)))
		{
			setBit(bv, i);
		}
		uint32_t value = (valBit(bv, i) ? 1:0);
		printf("Bit %u = %u\n", i, value);
	}
	position += C.l;
}
