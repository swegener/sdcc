/*
 * bug2719592.c
 */

#include <testfwk.h>
#include <stdbool.h>

#ifndef BOOL
#define BOOL bool
#endif

BOOL foo(char i, BOOL bv)
{
	bv &= (i == 1);
	return bv;
}

void
testBug(void)
{
	ASSERT( !foo(0,0) );
	ASSERT( !foo(0,1) );
	ASSERT( !foo(1,0) );
	ASSERT( foo(1,1) );
}
