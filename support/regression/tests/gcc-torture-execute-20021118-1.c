/*
   20021118-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && \
  (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parmeters are supported!
struct s { int f[4]; };

int foo (struct s s, int x1, int x2, int x3, int x4, int x5, int x6, int x7)
{
  return s.f[3] + x7;
}
#endif

void
testTortureExecute (void)
{
#if 0 // Todo: enable when brace omission in struct initialization is supported!
  struct s s = { 1, 2, 3, 4 };

  if (foo (s, 100, 200, 300, 400, 500, 600, 700) != 704)
    ASSERT (0);
  return;
#endif
}

