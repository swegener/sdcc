/*
   bug1738367.c
*/

#include <testfwk.h>
#include <stdbool.h>

bool Ternary(unsigned char status)
{
	return (status == 0) ? 0 : 1;
}

void
testBug(void)
{
	ASSERT(!Ternary(0x00));
	ASSERT( Ternary(0x10));
}
