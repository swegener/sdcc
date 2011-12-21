/*
   loop-11.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#ifndef SDCC_mcs51
static int a[199];

static void
foo ()
{
  int i;
  for (i = 198; i >= 0; i--)
    a[i] = i;
}
#endif

void
testTortureExecute (void)
{
#ifndef SDCC_mcs51
  int i;
  foo ();
  for (i = 0; i < 199; i++)
    if (a[i] != i)
      ASSERT (0);
#endif
  return;
}
