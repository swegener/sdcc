/* promoting bit to char */

#include <testfwk.h>

#if defined(PORT_HOST)
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
