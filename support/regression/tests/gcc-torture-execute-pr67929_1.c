/*
   pr67929_1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifndef __SDCC_pdk14 // Lack of memory
int
foo (float a)
{
  return a * 4.9f;
}
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_pdk14 // Lack of memory
  if (foo (10.0f) != 49)
    ASSERT (0);
#endif
}
