/*
   pr67929_1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

int
foo (float a)
{
  return a * 4.9f;
}


void
testTortureExecute (void)
{
  if (foo (10.0f) != 49)
    ASSERT (0);
}
