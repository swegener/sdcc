/** Bit vars test.

    type: bool, char, unsigned char, unsigned short, unsigned long
*/

#include <testfwk.h>
#include <stdbool.h>

#ifndef PORT_HOST
#pragma disable_warning 180 //no warning about using complement on bit/unsigned char
#endif

#if defined (SDCC_hc08) || defined (SDCC_z80)
#define NO_BITS
#endif

#if defined (__GNUC__) && defined (__alpha__) && (__GNUC__ < 3)
/* since this fails on GCC 2.95.4 on alpha... */
#define NO_BITS
#endif

#ifndef NO_BITS

#define TYPE_{type}

char foo(bool a, bool b, char c)
{
  return a + b + c;
}

char complement(bool a, bool b)
{
  return (a == b);
}

{type} _0 = 0, _1 = 1, _ff = 0xFF, _ffff = -1;

#endif

void
testBits(void)
{
#ifndef NO_BITS
  bool x = 2;
  ASSERT (foo(x,3,4) == 6);

  ASSERT (complement (~_0, 1));
  ASSERT (complement (~_1, 1));

#if defined TYPE_char
  ASSERT (complement (~_ff, 0));
#else
  ASSERT (complement (~_ff, 1));
#endif

#if defined TYPE_bool
  ASSERT (complement (~_ffff, 1));
#elif defined TYPE_char
  ASSERT (complement (~_ffff, 0));
#else
  if (sizeof({type}) < sizeof(int))
    ASSERT (complement (~_ffff, 1));
  else
    ASSERT (complement (~_ffff, 0));
#endif

#endif
}
