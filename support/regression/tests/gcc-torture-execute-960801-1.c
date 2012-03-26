/*
   960801-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

// TODO: Enable when sdcc can return long long!
#if 0
unsigned
f ()
{
  long long l2;
  unsigned short us;
  unsigned long long ul;
  short s2;

  ul = us = l2 = s2 = -1;
  return ul;
}

unsigned long long
g ()
{
  long long l2;
  unsigned short us;
  unsigned long long ul;
  short s2;

  ul = us = l2 = s2 = -1;
  return ul;
}
#endif

void
testTortureExecute (void)
{
#if 0
  if (f () != (unsigned short) -1)
    ASSERT (0);
  if (g () != (unsigned short) -1)
    ASSERT (0);
  return;
#endif
}

