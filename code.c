# include "code.h"

void appendCode(code C, bitV *bv)
{
	static uint32_t position = 0;
	while ((bv->l)+C->l > bv->l)
	{
		realloc(bv, (bv->l)+1000);
	}
	bv->v[position] |= code->bits[0];
}
