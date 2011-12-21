/*
   doloop-2.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#include <limits.h>

volatile unsigned int i;

void
testTortureExecute (void)
{
  unsigned short z = 0;

  do ++i;
  while (--z > 0);
  if (i != USHRT_MAX + 1U)
    ASSERT (0);
  return;
}

