/*
   bug3564649.c
*/

#include <testfwk.h>

#pragma disable_warning 85

void g(void *p)
{
}

_Noreturn void e(void)
{
}

void f(int i)
{
	void *p;

	if(i == 7)
		p = 0;
	else
		e();

	g(p); /* False "may be used before initialization" warning occoured here. */
}

void testBug(void)
{
}

