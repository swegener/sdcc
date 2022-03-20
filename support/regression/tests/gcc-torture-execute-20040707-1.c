/*
   20040707-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parameters are supported!
struct s { char c1, c2; };
void foo (struct s s)
{
  static struct s s1;
  s1 = s;
}
#endif

void
testTortureExecute (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parameters are supported!
  static struct s s2;
  foo (s2);
  return;
#endif
}
