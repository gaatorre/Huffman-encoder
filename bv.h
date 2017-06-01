# ifndef _BVector
# define _BVector
# include <stdint.h>

/*Interface for bv.h provided by Darrell Long*/

typedef struct bitV
{
	uint8_t *vector;
	uint32_t length;
} bitV;

bitV *newVec(uint32_t length);

void delVec(bitV *bv);

void oneVec(bitV *bv);

void setBit(bitV *bv, uint32_t index);

void clrBit(bitV *bv, uint32_t index);

uint8_t valBit(bitV *bv, uint32_t index);

uint32_t lenVec(bitV *bv);

# endif
