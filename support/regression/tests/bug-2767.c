/*
   bug1-2195.c

   Assertion failure in z80 code generation.
*/

#include <testfwk.h>

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) /* mcs51, hc08 and s08 have restrictions on function pointers wrt. reentrancy */
void f(void)
{
	((void (*)(int)) 0)(0);
}
#endif

void testBug(void)
{
}

