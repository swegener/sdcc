/*
    bug 3366711
*/

#include <testfwk.h>

void f(unsigned char c)
{
	unsigned char * p = (unsigned char*)8;
	p[3] = c ? 2 : 1; /* Old register allocator crashed in this assignment to a constant address. */
}

void
testBug (void)
{
  ASSERT (1);
}

