/** Bit vars test.

    type: bool, char, unsigned char, unsigned short, unsigned long
*/

#include <testfwk.h>
#include <stdbool.h>

#ifndef PORT_HOST
#pragma disable_warning 180 //no warning about using complement on bit/unsigned char
#endif

#ifdef __bool_true_false_are_defined

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

#endif //__bool_true_false_are_defined

void
testBits(void)
{
#ifdef __bool_true_false_are_defined
  bool x = 2;
  ASSERT (foo(x,3,4) == 6);

  ASSERT (complement (~_0, 1));
  ASSERT (complement (~_1, 1));

#if defined TYPE_char && !defined SDCC_CHAR_UNSIGNED
  ASSERT (complement (~_ff, 0));
#else
  ASSERT (complement (~_ff, 1));
#endif

#if defined TYPE_bool
  ASSERT (complement (~_ffff, 1));
#elif defined TYPE_char && !defined SDCC_CHAR_UNSIGNED
  ASSERT (complement (~_ffff, 0));
#else
  if (sizeof({type}) < sizeof(int))
    ASSERT (complement (~_ffff, 1));
  else
    ASSERT (complement (~_ffff, 0));
#endif

#endif //__bool_true_false_are_defined
}
