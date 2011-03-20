/*
 * bug3223041.c
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_sdcc99
#endif

#include <stdbool.h>

#ifndef BOOL
#define BOOL bool
#endif

BOOL and(BOOL a, BOOL b, BOOL c, BOOL d)
{
	return a & b & c & d;
}

BOOL or(BOOL a, BOOL b, BOOL c, BOOL d)
{
	return a | b | c | d;
}

BOOL xor(BOOL a, BOOL b, BOOL c, BOOL d)
{
	return a ^ b ^ c ^ d;
}

void
testBug(void)
{
	unsigned char i, x;
	for (i=0; i<16; i++)
	{
		ASSERT( and(i&0x01, i&0x02, i&0x04, i&0x08) == (i==15) );
		ASSERT( or (i&0x01, i&0x02, i&0x04, i&0x08) == (i!= 0) );
		x = i ^ (i>>1);
		x = x ^ (x>>2);
		ASSERT( xor(i&0x01, i&0x02, i&0x04, i&0x08) == (x & 0x01) );
	}
}
