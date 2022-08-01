/*
   C2X unreachable
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c2x

#include <stddef.h>

// todo: drop dis extra macro once we have c2x preprocessor support
#ifndef unreachable
#define unreachable __builtin_unreachable
#endif

volatile int i;

void f(void)
{
	if(i)
		unreachable();
}
#endif

void
testUnreachable(void)
{
#ifdef __SDCC
	if(0)
		unreachable();
	f();
#endif
}

