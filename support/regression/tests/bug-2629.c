/*
   bug-3240.c. A problem in the interaction between pointers to __code (implicit in this case) and plain pointers resulted in a false error.
 */

#include <testfwk.h>

const char *const data[] = {"a", "b", "c"};

#if 0 // TODO: Enable when bug #2629 is fixed.
char ptrs(const char *const * ptr);

void ptrs1()
{
   ptrs(data); // False error on about incompatible types this line
}
#endif

void testBug(void)
{
#if 0 // TODO: Enable when bug #2629 is fixed.
  ptrs1();
  ASSERT (ptrs(data) == 'b');
#endif
}

char ptrs(const char *const * ptr)
{
	return ptr[1][0];
}

