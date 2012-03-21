/*
   930513-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#include <stdio.h>
char buf[2];

void f (int (*fp)(char *, const char *, ...))
{
  (*fp)(buf, "%.0f", 5.0);
}

void
testTortureExecute (void)
{
#if 0
  f (&sprintf);
  if (buf[0] != '5' || buf[1] != 0)
    ASSERT (0);
  return;
#endif
}

