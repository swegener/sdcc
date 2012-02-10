/*
   pr49768.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

/* PR tree-optimization/49768 */

void
testTortureExecute (void)
{
  static struct { unsigned int : 1; unsigned int s : 1; } s = { .s = 1 };
  if (s.s != 1)
    ASSERT (0);
  return;
}

