/*
 * bug3290691.c
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_sdcc99
#endif

#include <stdbool.h>

#ifndef BOOL
#define BOOL bool
#endif

BOOL foo_b(BOOL b)
{
	return !b;
}

unsigned char bar_c(unsigned char c)
{
	return foo_b(c);
}

unsigned char foo_c(unsigned char c)
{
	return !c;
}

BOOL bar_b(BOOL c)
{
	return foo_c(c);
}

void
testBug(void)
{
	ASSERT( bar_c(1) == 0 );
	ASSERT( bar_c(0) == 1 );
	ASSERT( bar_b(1) == 0 );
	ASSERT( bar_b(0) == 1 );
}
