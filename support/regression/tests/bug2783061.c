/*
    bug 2783061
*/

#include <stdarg.h>
#include <testfwk.h>

#define CP	(void code*)0x1234
#define XP	(void xdata*)0x5678

void varargs_fn(char k, ...)
{
	va_list arg;
	void code * cp;
	void xdata * xp;
	void * gp;

	va_start (arg, k);

	cp = va_arg(arg, void code *);
	ASSERT(cp == CP);
	xp = va_arg(arg, void xdata *);
	ASSERT(xp == XP);
	gp = va_arg(arg, void *);
	ASSERT(gp == (void *)CP);
	gp = va_arg(arg, void *);
	ASSERT(gp == (void *)XP);

	va_end (arg);
}

void
testBug(void)
{
	void code * cp = CP;
	void xdata * xp = XP;

	varargs_fn('k', (void code *)cp, (void xdata *)xp, cp, xp);
}
