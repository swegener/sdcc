/*
   pr42269-2.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#if 0
SDCC cannot return long long yet. TODO: Enable later!
/* Make sure that language + abi extensions in passing S interoperate.  */

static long long
foo (unsigned short s)
{
  return (short) s;
}
#endif
unsigned short s = 0xFFFF;

void
testTortureExecute (void)
{
#if 0
  ASSERT (!(foo (s) + 1 != 0));
#endif
}

