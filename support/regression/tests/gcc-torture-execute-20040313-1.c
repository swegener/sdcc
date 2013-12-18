/*
   20040313-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* PR middle-end/14470 */
/* Origin: Lodewijk Voge <lvoge@cs.vu.nl> */

void
testTortureExecute (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_stm8)
  int t[1025] = { 1024 }, d;

  d = 0;
  d = t[d]++;
  if (t[0] != 1025)
    ASSERT (0);
  if (d != 1024)
    ASSERT (0);
  return;
#endif
}

