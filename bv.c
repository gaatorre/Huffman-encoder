#include <stdio.h>
#include "bv.h"
#include <stdlib.h>

//Implements functions for bit Vector

bitV *newVec(uint32_t length)
{
	bitV *bv = (bitV *)calloc(1, sizeof(bitV)); //Allocate space for bit Vec
	//allocate space for the vector itself
	bv->vector = (uint8_t *)calloc(length/8+1, sizeof(uint8_t));
	//Sets length of bit vector
	bv->length = length;
	return bv;
}

void delVec(bitV *bv)
{
	free(bv->vector); //Frees allocated space of the vector
	free(bv); //Frees allocated space of the bit Vector
	return;
}

void oneVec(bitV *bv)
{
	//Goes through each index of the bit Vector
	for(uint32_t i = 0; i <= (bv->length)/8;i++)
	{
		//bitwise or with the compliment of the vector to set all to 1
		*(bv->vector+i) |= ~(*(bv->vector+i));
	}
	return;
}

void setBit(bitV *bv, uint32_t index)
{
	//bitwise or with bitwise 1 shifted according to the bit needed
	*((bv->vector)+index/8) |= (0x1 << index%8);
	return;
}

void clrBit(bitV *bv, uint32_t index)
{
	//bitwise and with the compliment of bitwise 1 shifted according to bit needed
	*((bv->vector)+index/8) &= ~(0x1 << index%8);
	return;
}

uint8_t valBit(bitV *bv, uint32_t index)
{
	//bitwise and with bitwise 1 shifted according to the bit needed
	//returns a number if the bit is a 1 or a 0 if the bit is a 0
	return (*((bv->vector)+index/8) & (0x1 << index%8));
}

uint32_t lenVec(bitV *bv)
{
	return bv->length;
}

void printVec(bitV *bv)
{
	for(uint32_t i = 0; i < bv->length; i++)
	{
		printf("Bit #%u: %u\n", i, valBit(bv, i) != 0);
	}
}
