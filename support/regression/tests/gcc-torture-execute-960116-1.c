/*
   960116-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#if 0
static inline int
p (int *p)
{
  return !((long) p & 1);
}

int
f (int *q)
{
  if (p (q) && *q)
    return 1;
  return 0;
}
#endif

void
testTortureExecute (void)
{
#if 0
  if (f ((int*) 0xffffffff) != 0)
    ASSERT (0);
  return;
#endif
}

