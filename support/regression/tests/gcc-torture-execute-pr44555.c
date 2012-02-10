/*
   pr44555.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

struct a {
    char b[100];
};
int foo(struct a *a)
{
  if (&a->b)
    return 1;
  return 0;
}

void
testTortureExecute (void)
{
  if (foo((struct a *)0) != 0)
    ASSERT (0);
  return;
}

