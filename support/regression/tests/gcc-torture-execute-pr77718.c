/*
   pr77718.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#include <string.h>

/* PR middle-end/77718 */

char a[64];

int
foo (void)
{
  return memcmp ("bbbbbb", a, 6);
}

int
bar (void)
{
  return memcmp (a, "bbbbbb", 6);
}

void
testTortureExecute (void)
{
  memset (a, 'a', sizeof (a));
  if (((foo () < 0) ^ ('a' > 'b'))
      || ((bar () < 0) ^ ('a' < 'b')))
    ASSERT (0);
  return;
}
