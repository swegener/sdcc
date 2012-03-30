/*
   20011126-1.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* Produced a overflow in ifcvt.c, causing S to contain 0xffffffff7fffffff.  */

int a = 1;

void
testTortureExecute (void)
{
// TODO: Enable when sdcc supports long long constants!
#if 0
  long long s;

  s = a;
  if (s < 0)
    s = -2147483648LL;
  else
    s = 2147483647LL;

  if (s < 0)
    ASSERT (0);

  return;
#endif
}

