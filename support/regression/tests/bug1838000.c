/*
    bug 1838000
*/

#include <testfwk.h>

#ifdef SDCC
 #include <sdcc-lib.h>
#else
 #define _AUTOMEM
 #define _STATMEM
#endif

typedef struct { char b:1; } t;
t glbl = { 0 };
t _STATMEM * gp = &glbl;

void func(char _AUTOMEM *p)
{
	*p++ = gp->b ? 'A' : 'B';
	*p   = 'C';
}

void
testBug(void)
{
	char x[2];
	func(x);
	ASSERT(x[0]=='B');
}
