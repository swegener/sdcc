/** Bitfield tests.

*/
#include <testfwk.h>

struct {
  char c0_3 : 3;
  char c3_5 : 5;
} c_bitfield;

struct {
  int i0_7 : 7;
  int i7_9 : 9;
} i_bitfield;

struct {
  long l0_7 : 7;
  long l7_10 : 10;
  long l17_15 : 15;
} l_bitfield;

void
testBitfields(void)
{
  c_bitfield.c0_3 = 2;
  c_bitfield.c3_5 = 3;
  ASSERT(*(char *)(&c_bitfield) == (2 + (3<<3)) );

  i_bitfield.i0_7 = 23;
  i_bitfield.i7_9 = 234;
  ASSERT(*(int *)(&i_bitfield) == (23 + (234<<7)) );

  l_bitfield.l0_7 = 23;
  l_bitfield.l7_10 = 234;
  l_bitfield.l17_15 = 2345;
  ASSERT(*(long *)(&l_bitfield) == (23 + (234<<7) + (2345<<17)) );
}
