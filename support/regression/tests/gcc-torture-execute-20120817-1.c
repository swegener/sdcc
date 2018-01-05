/*
   20120817-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

typedef unsigned long long u64;
unsigned long foo = 0;
u64 f();

u64 f() {
  return ((u64)40) + ((u64) 24) * (int)(foo - 1);
}

void
testTortureExecute (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08)
  if (f () != 16)
    ASSERT (0);
#endif
}

