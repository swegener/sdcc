/*
   921016-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

void
testTortureExecute (void)
{
#if 0
  int j=1081;
  struct
    {
      signed int m:11;
   } l;
  if((l.m = j) == j)
    ASSERT(0);
  return;
#endif
}

