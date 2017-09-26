/*
   pr66940.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifndef __SDCC_ds390 // Enable when ds390 supports long long
long long
foo (long long ival)
{
 if (ival <= 0)
    return -0x7fffffffffffffffL - 1;

 return 0x7fffffffffffffffL;
}
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_ds390 // Enable when ds390 supports long long
  if (foo (-1) != (-0x7fffffffffffffffL - 1))
    ASSERT (0);

  if (foo (1) != 0x7fffffffffffffffL)
    ASSERT (0);
#endif
  return;
}
