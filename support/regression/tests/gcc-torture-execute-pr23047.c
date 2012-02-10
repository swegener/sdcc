/*
   pr15262.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#include <limits.h>

void f(int i)
{
  i = i > 0 ? i : -i;
  if (i<0)
    return;
  ASSERT (0);
}

void
testTortureExecute (void)
{
  f(INT_MIN);
  return;
}

