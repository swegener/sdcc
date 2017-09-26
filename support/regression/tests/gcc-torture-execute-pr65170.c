/*
   pr65170.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* PR tree-optimization/65170 */

#ifndef __SDCC_ds390 // Enable when ds390 supports long long
typedef unsigned long long int V;
typedef unsigned int H;

void
foo (V b, V c)
{
#if !defined(__SDCC_mcs51)
  V a;
  b &= (H) -1;
  c &= (H) -1;
  a = b * c;
  if (a != 1)
    ASSERT (0);
#endif
}
#endif

void
testTortureExecute (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(__SDCC_gbz80)
  foo (1, 1);
  return;
#endif
}

