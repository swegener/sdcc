/*
   20030209-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#pragma disable_warning 93
#endif

#ifndef __SDCC_mcs51
#ifndef __SDCC_gbz80
#ifndef __SDCC_hc08
#ifndef __SDCC_s08
double x[100][100]; /* Too big for mcs51 and gbz80. */
#endif
#endif
#endif
#endif
void
testTortureExecute (void)
{
#ifndef __SDCC_mcs51
#ifndef __SDCC_gbz80
#ifndef __SDCC_hc08
#ifndef __SDCC_s08
  int i;

  i = 99;
  x[i][0] = 42;
  if (x[99][0] != 42)
    ASSERT (0);
  return;
#endif
#endif
#endif
#endif
}

