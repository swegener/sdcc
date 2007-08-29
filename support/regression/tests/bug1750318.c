/*
    bug 1750318
*/

#include <testfwk.h>

xdata at(0x1234) char bar;

// no need to call this, it generates compiler error:
//   Internal error: validateOpType failed in
//   OP_SYMBOL(IC_RESULT (ic)) @ SDCCcse.c:2172:
//   expected symbol, got value
void foo(void) {
	*(char volatile xdata *) &bar = 0x80;
}

void
testBug(void)
{
	ASSERT(1);
}
