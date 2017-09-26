/*
   20120817-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#ifndef __SDCC_ds390 // Enable when ds390 supports long long
typedef unsigned long long u64;
unsigned long foo = 0;
u64 f();

#if !defined(__SDCC_mcs51)
u64 f() {
  return ((u64)40) + ((u64) 24) * (int)(foo - 1);
}
#endif
#endif

void
testTortureExecute (void)
{
#if !defined(__SDCC_gbz80) && !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_hc08) && !defined(__SDCC_s08)
  if (f () != 16)
    ASSERT (0);
#endif
}

