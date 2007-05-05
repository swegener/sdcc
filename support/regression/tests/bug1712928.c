/*
   bug1712928.c
*/

#include <testfwk.h>

#if defined (SDCC_mcs51)
unsigned char NakedFunc(void) __naked
{
	__asm
	mov	dpl,#0x01
	ret
	__endasm;
}
#endif

void
testBug(void)
{
#if defined (SDCC_mcs51)
	unsigned char hsum, sum;

	hsum = NakedFunc();
	sum = NakedFunc();
	sum += hsum;

	ASSERT(sum == 2);
#endif
}
