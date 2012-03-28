/** Simple long long tests.

 */
#include <testfwk.h>

#ifdef __SDCC
#pragma std_sdcc99
#pragma disable_warning 85
#endif

#if !defined(__SDCC_mcs51) && !defined(__SDCC_hc08) && !defined(__SDCC_ds390) && !defined(__SDCC_pic14) && !defined(__SDCC_pic16)
long long x;
int i;

long long g(void)
{
  int y = i + 1;
  return (y);
}

long long h(void)
{
  return (x);
}

long long c(void)
{
  return (12ll);
}

long long d(int i)
{
  return (i);
}

long long (*gp)(void) = &g;
#endif

void
testLongLong (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_hc08) && !defined(__SDCC_ds390) && !defined(__SDCC_pic14) && !defined(__SDCC_pic16)
  i = 42;
  ASSERT (g() == 43);
  i = 23;
#ifndef __SDCC_gbz80
  ASSERT ((*gp)() == 24);
#endif
  ASSERT (c() == 12);
  x = 42;
  ASSERT (h() == x);
  ASSERT (d(12) == 12ll);
#endif
}

