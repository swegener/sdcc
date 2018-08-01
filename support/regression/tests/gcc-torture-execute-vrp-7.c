/*
vrp-7.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

struct T
{
  int b : 1;
} t;

void foo (int f)
{
  t.b = (f & 0x10) ? 1 : 0;
}

void
testTortureExecute (void)
{
#ifndef __SDCC_z80 // Bug #2786
#ifndef __SDCC_z180 // Bug #2786
#ifndef __SDCC_gbz80 // Bug #2786
#ifndef __SDCC_r2k // Bug #2786
#ifndef __SDCC_r3ka // Bug #2786
#ifndef __SDCC_tlcs90 // Bug #2786
#ifndef __SDCC_ds390 // Bug #2785
  foo (0x10);
  if (!t.b)
    ASSERT (0);
  return;
#endif
#endif
#endif
#endif
#endif
#endif
#endif
}
