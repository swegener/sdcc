/*
   bug-2357.c
 */

#include <testfwk.h>
#include <string.h>

#ifdef __SDCC
#pragma std_c99
#endif

#if !defined (__SDCC_mcs51) || (!defined (__SDCC_MODEL_SMALL) && !defined (__SDCC_MODEL_MEDIUM) && !defined (__SDCC_STACK_AUTO))

short i = -1;
short ww[127];
short qq[128];

short
foo2(void)
{
  return ww[117 + i];
}

short
foo3(void)
{
  return qq[117 + i];
}

#endif

void
testBug (void)
{
#if !defined (__SDCC_mcs51) || (!defined (__SDCC_MODEL_SMALL) && !defined (__SDCC_MODEL_MEDIUM) && !defined (__SDCC_STACK_AUTO))

  char *p0 = (char *) ww;
  char *p1 = (char *) qq;

  memset(p0, 0x33, 128);
  memset(p0 + 128, 0x55, 124);

  memset(p1, 0x33, 128);
  memset(p1 + 128, 0x55, 124);

  ASSERT (foo2() == 0x5555);
  ASSERT (foo3() == 0x5555);

#endif
}