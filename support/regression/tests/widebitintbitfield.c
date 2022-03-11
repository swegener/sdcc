/** bit-precise integer types allow wider bit-fields than int.

    width: 7, 17, 24, 32, 33, 40, 48, 63, 64, 65
    sign: unsigned, signed
*/

#include <testfwk.h>

#include <stdbool.h>

#if __SDCC_BITINT_MAXWIDTH >= {width} // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!
typedef {sign} _BitInt({width}) bitinttype;

struct {
  bitinttype bw : {width};
  unsigned _BitInt(4) b4 : 4;
  bool b : 1;
} bf = {5, 6, false};

struct {
  bitinttype bw : {width};
  unsigned _BitInt(4) b4 : 4;
  bitinttype bw_1 : {width} - 1;
} bf2 = {2, 2, 5};
#endif

void testWideBitIntBitField(void)
{
#if __SDCC_BITINT_MAXWIDTH >= {width} // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!
  ASSERT (bf.bw == 5);
  ASSERT (bf.b4 == 6);
  ASSERT (bf.b == false);

  ASSERT (bf2.bw == 2);
  ASSERT (bf2.b4 == 2);
  ASSERT (bf2.bw_1 == 5);
#endif
}

