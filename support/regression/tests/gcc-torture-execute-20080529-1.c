/*
   20080529-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

// TODO. Enable when long long support is more complete!

/* PR target/36362 */

#if 0
int
test (float c)
{
  return !!c * 7LL == 0;
}
#endif

void
testTortureExecute (void)
{
#if 0
  if (test (1.0f) != 0)
    ASSERT (0);
#endif
  return;
}

