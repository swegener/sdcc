/*
   bug-3256.c
   A bug in the stm8 peephole optimizer for functions calls through function pointers where the caller has no non-pointer calls, and the callee has register arguments.
 */

#include <testfwk.h>

int (*volatile twocharargptr)(char, char) __raisonance;

int f(void)
{
	(*twocharargptr)(23, 42);
}

int twochararg(char a, char b) __raisonance
{
	return a + b;
}

void
testBug(void)
{
	twocharargptr = &twochararg;
	ASSERT (f() == 23 + 42);
}

