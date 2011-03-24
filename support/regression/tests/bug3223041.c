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

#ifdef SDCC_hc08
#define CAST(x)	(x ? true : false)
#else
#define CAST(x)	(x)
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
		ASSERT( and(CAST(i&0x01), CAST(i&0x02), CAST(i&0x04), CAST(i&0x08)) == (i==15) );
		ASSERT( or (CAST(i&0x01), CAST(i&0x02), CAST(i&0x04), CAST(i&0x08)) == (i!= 0) );
		x = i ^ (i>>1);
		x = x ^ (x>>2);
		ASSERT( xor(CAST(i&0x01), CAST(i&0x02), CAST(i&0x04), CAST(i&0x08)) == (x & 0x01) );
	}
}
