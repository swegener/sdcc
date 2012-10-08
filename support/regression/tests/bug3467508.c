/*
   bug1665511.c

   Cast of integer literals to pointer behaved differently than cast from integer.
*/

#include <testfwk.h>

void testBug(void)
{
	volatile int i = -1;
	ASSERT((void *)(i) == (void *)(-1));
}

