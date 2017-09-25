/*
   20141107-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#include <string.h>

#define bool _Bool

bool f(int a, bool c);
bool f(int a, bool c)
{
  if (!a)
    c = !c;
  return c;
}

void checkf(int a, bool b)
{
  bool c = f(a, b);
  char d;
  memcpy (&d, &c, 1);
  if ( d != (a==0)^b)
    ASSERT(0);
}

void
testTortureExecute (void)
{
  checkf(0, 0);
  checkf(0, 1);
  checkf(1, 1);
  checkf(1, 0);
}
