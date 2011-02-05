/*
   bug3166208.c
 */

#include <testfwk.h>

// no need to call this, it generates compiler error:
//   SDCCval.c:1073: expected SPECIFIER, got null-link
volatile char a;

void bug(void)
{
	if ((* (char __xdata *)0xDF53))
	{
		a = 2;
	}
}

void
testBug (void)
{
	ASSERT (1);
}
