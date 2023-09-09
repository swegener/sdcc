/* bug-3641.c
   The hc08/s08 backend omitted the least-significant byte in an assignment of a literal to a local 24-bit variable, depending on which registers it resided in.
*/

#include <testfwk.h>

// In both assignments to i, the least-significant byte gets omitted.
void loopm2(unsigned char *a)
{
    for (unsigned _BitInt(24) i = (1ul << 20); i < (1ul << 20) + 1; i = (1ul << 20) + 1)
        a[i - (1ul << 20)] = 1;
}

void
testBug(void)
{
	char c = 0;
	loopm2(&c);
#ifndef __SDCC_pdk15 // Another bug.
	ASSERT(c == 1);
#endif
}

