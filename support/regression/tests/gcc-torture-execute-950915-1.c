/*
   950915-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

// TODO: Enable when sdcc supports long long!
#if 0
long int a = 100000;
long int b = 21475;

long
f ()
{
  return ((long long) a * (long long) b) >> 16;
}
#endif

void
testTortureExecute (void)
{
// TODO: Enable when sdcc supports long long!
#if 0
  if (f () < 0)
    ASSERT (0);
  return;
#endif
}

