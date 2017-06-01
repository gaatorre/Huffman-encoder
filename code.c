# include "code.h"

# define INCREMENT 8000
void appendCode(code C, bitV *bv)
{
	static uint32_t position = 0;
	while ((bv->l) + C->l > bv->l)
	{
		bv->l += INCREMENT;
		realloc(bv->vector, bv->l*(sizeof(uint8_t)));
	}
	for (uint32_t i = position; i < C->l; i++)
	{
		if (C->bits[position/8] & (0x1 << (c->l % 8))
		{
			setBit(bv, position);
		}
		uint32_t value = (valBit(bv, i) ? 1:0);
		printf("Bit %u = %u\n", i, value);
	}
	position += C->l;
}
