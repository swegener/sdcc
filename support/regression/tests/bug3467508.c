/*
   bug1665511.c

   Cast of integer literals to pointer behaved differently than cast from integer.
*/

#include <testfwk.h>

void testBug(void)
{
#ifndef __SDCC_mcs51
#ifndef __SDCC_ds390
	volatile int i = -1;
	ASSERT((void *)(i) == (void *)(-1));
#endif
#endif
}

