/*
   pr78791.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* PR target/78791 */
#ifndef __SDCC_ds390
#ifndef __SDCC_mcs51
unsigned long long
foo (unsigned long long x, unsigned long long y, unsigned long long z)
{
  unsigned long long a = x / y;
  unsigned long long b = x % y;
  a |= z;
  b ^= z;
  return a + b;
}
#endif
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_s08
#ifndef __SDCC_hc08
#ifndef __SDCC_ds390
#ifndef __SDCC_mcs51
  if (foo (64, 7, 0) != 10 || foo (28, 3, 2) != 14)
    ASSERT (0);
  return;
#endif
#endif
#endif
#endif
}
