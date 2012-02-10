/*
   pr34070-2.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

int f(unsigned int x, int n)
{
    return ((int)x) / (1 << n);
}

void
testTortureExecute (void)
{
  if (f(-1, 1) != 0)
    ASSERT (0);
  return;
}

