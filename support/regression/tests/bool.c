/* bool.c
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#include <stdbool.h>
#include <stdint.h>

#if !defined __SDCC_WEIRD_BOOL

struct s
{
	bool b;
};

bool ret_true(void)
{
	return(true);
}

bool ret_false(void)
{
	return(false);
}

bool (* const pa[])(void) = {&ret_true, &ret_false};

volatile bool E;

void
testBool(void)
{
	ASSERT(true);
	ASSERT((*(pa[0]))() == true);
	ASSERT((*(pa[1]))() == false);

	E = true;
	ASSERT((E ? 1 : 0) == (!(!E)));
	ASSERT((E += 2) == 1);
	ASSERT((--E, --E, E) == E);

	E = false;
	ASSERT((E ? 1 : 0) == (!(!E)));
	ASSERT((E += 2) == 1);
	ASSERT((--E, --E, E) == E);
}

#else

void testBool(void)
{
}

#endif

