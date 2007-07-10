/*
   bug1738367.c
*/

#include <testfwk.h>
#include <stdbool.h>

#ifdef __bool_true_false_are_defined

bool ternary(unsigned char status)
{
	return (status == 0) ? 0 : 1;
}

#endif //__bool_true_false_are_defined


void
testBug(void)
{
#ifdef __bool_true_false_are_defined
	ASSERT(!ternary(0x00));
	ASSERT( ternary(0x10));
#endif //__bool_true_false_are_defined
}
