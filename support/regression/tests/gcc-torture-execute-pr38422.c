/*
   pr38411.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#include <limits.h>

/* PR middle-end/38422 */

struct S
{
  int s : (sizeof (int) * CHAR_BIT - 2);
} s;

void
foo (void)
{
  s.s *= 2;
}

void
testTortureExecute (void)
{
  s.s = 24;
  foo ();
  if (s.s != 48)
    ASSERT (0);
  return;
}
