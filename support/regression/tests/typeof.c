/*
   while.c
*/

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c2x

typeof(7l) l7 = 7;

void g0(long l)
{
	typeof_unqual(const int) i = l + 1;
	i++;
	ASSERT(sizeof(i) == sizeof(int));
}
#endif

void
testTypeof(void)
{
#ifdef __SDCC
	g0(0);
	ASSERT(sizeof(l7) == sizeof(long));
#endif
}

