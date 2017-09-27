/*
   pr78675.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* PR tree-optimization/78675 */

long int a;

long int
foo (long int x)
{
  long int b;
  while (a < 1)
    {
      b = a && x;
      ++a;
    }
  return b;
}

void
testTortureExecute (void)
{
  if (foo (0) != 0)
    ASSERT (0);
  a = 0;
  if (foo (1) != 0)
    ASSERT (0);
  a = 0;
  if (foo (25) != 0)
    ASSERT (0);
  a = -64;
  if (foo (0) != 0)
    ASSERT (0);
  a = -64;
  if (foo (1) != 0)
    ASSERT (0);
  a = -64;
  if (foo (25) != 0)
    ASSERT (0);
  return;
}
