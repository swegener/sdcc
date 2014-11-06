/*
   bug-2304.c a literal address bug for operands bigger than 2 bytes.
 */

#include <testfwk.h>

void testBug(void)
{
#ifdef __SDCC_pic16
	signed long *l = (signed long *) 0x02b0;
	float *f = (float *) 0x02b0;
#else
	signed long *l = (signed long *) 0xcab0;
	float *f = (float *) 0xcab0;
#endif

	*l++ = -2;	
	*l = 2;
	ASSERT(*(l - 1) == -2);
	ASSERT(*l == 2);

	*f++ = 10;
	*f = 20;
	ASSERT(*(f - 1) == 10);
	ASSERT(*f == 20);
}

