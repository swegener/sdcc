/*
pr86231.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

/* PR tree-optimization/86231 */

#define ONE ((void *) 1)
#define TWO ((void *) 2)

int
foo (void *p, int x)
{
  if (p == ONE) return 0;
  if (!p)
    p = x ? TWO : ONE;
  return p == ONE ? 0 : 1;
}

int v[8];

void
testTortureExecute (void)
{
  if (foo ((void *) 0, 0) != 0
      || foo ((void *) 0, 1) != 1
      || foo (ONE, 0) != 0
      || foo (ONE, 1) != 0
      || foo (TWO, 0) != 1
      || foo (TWO, 1) != 1
      || foo (&v[7], 0) != 1
      || foo (&v[7], 1) != 1)
    ASSERT (0);
  return;
}
