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
	bv->vector[position] |= code->bits[0];
	position += C->l;
}
