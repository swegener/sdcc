/** bit-precise bit-fields

*/

#include <testfwk.h>

#include <stdbool.h>

#if __SDCC_BITINT_MAXWIDTH >= 4 // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!
struct {
  unsigned _BitInt(3) b3 : 3;
  unsigned _BitInt(4) b4 : 4;
  bool b : 1;
} ubf = {5, 6, false};

struct {
  signed _BitInt(3) b3 : 3;
  signed _BitInt(4) b4 : 4;
  signed int b : 1;
} sbf = {-2, 2, -1};

struct {
  _BitInt(3) b3 : 3;
  _BitInt(4) b4 : 4;
  unsigned int b : 1;
} bf = {-2, 2, 1};

struct {
  unsigned _BitInt(3) b2 : 2;
  unsigned _BitInt(4) b3 : 3;
  unsigned _BitInt(4) b4 : 4;
} ubf2 = {2, 2, 5};
#endif

void testBitIntBitField(void)
{
#if __SDCC_BITINT_MAXWIDTH >= 4 // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!
	ASSERT (ubf.b3 == 5);
	ASSERT (ubf.b4 == 6);
	ASSERT (ubf.b == false);

	ASSERT (sbf.b3 == -2);
	ASSERT (sbf.b4 == 2);
	ASSERT (sbf.b == -1);

	ASSERT (bf.b3 == -2);
	ASSERT (bf.b4 == 2);
	ASSERT (bf.b == 1);

	ASSERT (ubf2.b2 == 2);
	ASSERT (ubf2.b3 == 2);
	ASSERT (ubf2.b4 == 5);

	ubf2.b3 = 7;
	ASSERT (ubf2.b2 == 2);
	ASSERT (ubf2.b3 == 7);
	ASSERT (ubf2.b4 == 5);

	sbf.b3 = -1;
	sbf.b4 = 0;
	sbf.b = -1;
	ASSERT (sbf.b3 == -1);
	ASSERT (sbf.b4 == 0);
	ASSERT (sbf.b == -1);
#endif
}

