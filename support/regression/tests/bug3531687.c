/*
  bug3531687.c

  The bug resulted in wrong comparisons of char to bool, casting the char operand to bool.
*/

#include <testfwk.h>

int f(char a, char flag)
{
  if (a == (flag ? 1 : 0))
    return 0;
  return 1;
}

void
testBug(void)
{
  ASSERT(f(2, 1));
}
