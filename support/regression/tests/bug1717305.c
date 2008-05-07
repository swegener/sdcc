/*
    bug 1717305
*/

#include <testfwk.h>

#ifdef SDCC
#pragma std_sdcc99
#endif

static inline int f(const int a)
{
	return (a + 3);
}

int g(int b)
{
	return (f(b));
}

void
testBug(void)
{
    int x = 0;
    ASSERT (g(x) == 3);
}
