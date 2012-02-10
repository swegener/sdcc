/*
   pr34070-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

int f(unsigned int x)
{
    return ((int)x) % 4;
}

void
testTortureExecute (void)
{
  if (f(-1) != -1)
    ASSERT (0);
  return;
}

