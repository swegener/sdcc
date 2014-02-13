/*
   bug-2253.c was a bug in operand handling of operands that are outside the stack-pointer-offset range in code generation for wide division in the stm8 port.
 */

#include <testfwk.h>

char ad(char *p)
{
  p;
}

unsigned int ss(unsigned int c, unsigned int d)
{
  char s1[255];
  ad(s1);
  return c / d;
}

void testBug(void)
{
  ASSERT(ss(4, 2) == 2);
}

