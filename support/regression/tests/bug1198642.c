/*
   bug1198642.c
*/

#include <testfwk.h>

void
test_cse_generic_ptr(void)
{
#if defined(SDCC_mcs51)
	volatile void *p1;
	volatile void *p2;

	p1 = (data char *)1;
	p2 = (idata char *)1;
	ASSERT (p1 == p2);

	p1 = (data char *)1;
	p2 = (xdata char *)1;
	ASSERT (p1 != p2);

	p1 = (data char *)1;
	p2 = (idata char *)2;
	ASSERT (p1 != p2);
#endif
}
