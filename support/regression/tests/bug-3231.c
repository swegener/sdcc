/*
   bug-3230.c
   A bug in code generation for __z88dk_callee __critical functions with parameters.
 */

#include <testfwk.h>

volatile int j;

void f0(int i) __z88dk_callee __critical
{
	j = i;
}

int f1(int i) __z88dk_callee __critical
{
	return (i + j);
}

void testBug(void)
{
	f0(23);
	ASSERT (j == 23);
	ASSERT (f1(23) == 46);
}

