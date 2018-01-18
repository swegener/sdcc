/*
   pr78791.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* PR target/78791 */
unsigned long long
foo (unsigned long long x, unsigned long long y, unsigned long long z)
{
  unsigned long long a = x / y;
  unsigned long long b = x % y;
  a |= z;
  b ^= z;
  return a + b;
}

void
testTortureExecute (void)
{
  if (foo (64, 7, 0) != 10 || foo (28, 3, 2) != 14)
    ASSERT (0);
  return;
}
