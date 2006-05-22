/* promoting bit to char */

#include <testfwk.h>

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80) || defined(SDCC_hc08) || defined(SDCC_pic16)
#  define NO_BIT_TYPE
#endif

#if defined(NO_BIT_TYPE)
volatile int a = 1, b = 1;
#else
volatile bit a = 1, b = 1;
#endif

char
foo (void)
{
  return (a << 1) | b;
}

static void
testBitToCharPromotion(void)
{
  ASSERT(foo() == 3);
}
