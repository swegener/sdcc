/*
   func-ptr-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#pragma disable_warning 93 // Using float for double.

static double f (float a);
static double (*fp) (float a);

void
testTortureExecute (void)
{
#ifndef SDCC_hc08
  fp = f;
  if (fp ((float) 1) != 1.0)
    ASSERT (0);
  return;
#endif
}

static double
f (float a)
{
  return a;
}
