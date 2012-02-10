/*
   pr36691.c from the execute part of the gcc torture tests.
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

unsigned char g_5;

void func_1 (void)
{
  for (g_5 = 9; g_5 >= 4; g_5 -= 5)
    ;
}

void
testTortureExecute (void)
{
  func_1 ();
  if (g_5 != 0)
    ASSERT (0);
  return;
}

