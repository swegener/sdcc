/*
   bug-2885.c
   INT_MIN was not of type int.
 */

#include <testfwk.h>

#include <limits.h>

void testBug (void)
{
#if 0 // Bug
  ASSERT(sizeof(INT_MIN) == sizeof(int));
#endif
  ASSERT(sizeof(INT_MAX) == sizeof(int));
  ASSERT(sizeof(UINT_MAX) == sizeof(unsigned int));
  ASSERT(sizeof(LONG_MIN) == sizeof(long));
  ASSERT(sizeof(LONG_MAX) == sizeof(long));
  ASSERT(sizeof(ULONG_MAX) == sizeof(unsigned long));
}

