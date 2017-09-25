/*
   pr66940.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

long long
foo (long long ival)
{
 if (ival <= 0)
    return -0x7fffffffffffffffL - 1;

 return 0x7fffffffffffffffL;
}

void
testTortureExecute (void)
{
  if (foo (-1) != (-0x7fffffffffffffffL - 1))
    ASSERT (0);

  if (foo (1) != 0x7fffffffffffffffL)
    ASSERT (0);

  return;
}
