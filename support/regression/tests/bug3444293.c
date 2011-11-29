/*
   bug3444293.c
*/

#include <testfwk.h>

void *s[3];

void f(void)
{
	s[0] = 0;
	s[1] = 0; // Access to s is off by one byte here.
}

void testBug(void)
{
	s[0] = (void *)(0xffff);
	s[1] = (void *)(0xffff);
	f();
	ASSERT(!s[0]);
	ASSERT(!s[1]);
}

