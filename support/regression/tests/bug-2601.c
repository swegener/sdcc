/*
   bug-2601.c
*/

#include <testfwk.h>

unsigned char *p;
unsigned char *a;
signed char b;

void f(void)
{
    p = a + b;
}

void g(void)
{
    p = b + a;
}

void testBug(void)
{
	unsigned char c[2] = {23, 42};

	a = c + 1;
	b = -1;

	p = 0;
	f();
	ASSERT(*p == 23);

	p = 0;
	g();
	ASSERT(*p == 23);
}

