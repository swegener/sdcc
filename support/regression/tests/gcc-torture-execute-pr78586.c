/*
   pr78586.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#include <stdio.h>

/* PR tree-optimization/78586 */

void
foo (unsigned long x)
{
  char a[30];
  unsigned long b = sprintf (a, "%lu", x);
  if (b != 4)
    ASSERT (0);
}

void
testTortureExecute (void)
{
  foo (1000);
  return;
}
