/** Bit vars test.

*/
#include <testfwk.h>

#if defined (SDCC_STACK_AUTO) || defined (SDCC_hc08) || defined (SDCC_z80) || defined (PORT_HOST)
#define NO_BITS
#endif

#ifndef NO_BITS
char foo(bit a, bit b, char c)
{
  return a + b + c;
}
#endif

void
testBits(void)
{
#ifndef NO_BITS
  bit x = 2;
  ASSERT (foo(x,3,4) == 6);
#endif
}
