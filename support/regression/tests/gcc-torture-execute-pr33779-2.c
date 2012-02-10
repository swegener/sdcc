/*
   pr33779-2.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

int foo(long int i)
{
  return ((int)((unsigned long)(i + 1) * 4)) / 4;
}

void
testTortureExecute (void)
{
  if (foo(0x3fffffff) != 0)
    ASSERT (0);
  return;
}

