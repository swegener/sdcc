/** Simple long long tests.

 */
#include <testfwk.h>

#ifdef __SDCC
#pragma std_sdcc99
#endif

#if !defined(__SDCC_mcs51) && !defined(__SDCC_hc08) && !defined(__SDCC_ds390) && !defined(__SDCC_pic14) && !defined(__SDCC_pic16) && !defined(__SDCC_gbz80) && !defined(__SDCC_r2k)
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

long long (*gp)(void) = &g;
#endif

void
testLongLong (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_hc08) && !defined(__SDCC_ds390) && !defined(__SDCC_pic14) && !defined(__SDCC_pic16) && !defined(__SDCC_gbz80) && !defined(__SDCC_r2k)
  i = 42;
  ASSERT (g() == 43);
  i = 23;
  ASSERT ((*gp)() == 24);
  ASSERT (c() == 12);
  x = 42;
  ASSERT (h() == x);
#endif
}

