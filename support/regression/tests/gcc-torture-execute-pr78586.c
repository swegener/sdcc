/*
   pr78586.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#include <stdio.h>

/* PR tree-optimization/78586 */

#ifndef __SDCC_pdk14 // Lackof memory
void
foo (unsigned long x)
{
  char a[30];
  unsigned long b = sprintf (a, "%lu", x);
  if (b != 4)
    ASSERT (0);
}
#endif

void
testTortureExecute (void)
{
#ifndef __SDCC_pdk14 // Lackof memory
  foo (1000);
  return;
#endif
}
