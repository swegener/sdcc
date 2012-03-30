/*
   20000717-2.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

// TODO: Enable when sdcc supports long long constants!
#if 0
static void
compare (long long foo)
{
  if (foo < 4294967297LL)
    ASSERT (0);
}
#endif

void
testTortureExecute (void)
{
#if 0
  compare (8589934591LL);
  return;
#endif
}

