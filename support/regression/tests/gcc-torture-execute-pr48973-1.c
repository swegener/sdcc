/*
   pr48973-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

/* PR middle-end/48973 */

struct S { signed int f : 1; } s;
int v = -1;

void
foo (unsigned int x)
{
  if (x != -1U)
    ASSERT (0);
}

void
testTortureExecute (void)
{
  s.f = (v & 1) > 0;
  foo (s.f);
  return;
}

