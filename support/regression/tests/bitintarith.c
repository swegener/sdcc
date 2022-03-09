/** bit-precise integers

    width: 2, 4, 7, 8, 9, 15, 16, 17, 24, 32, 33, 40, 48, 63, 64, 65
*/

#include <testfwk.h>

#if __SDCC_BITINT_MAXWIDTH >= {width} // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!

typedef _BitInt({width}) bitinttype;

typedef unsigned _BitInt({width}) ubitinttype;

volatile bitinttype a, b;
volatile ubitinttype ua, ub;

#endif

void testBitIntArith(void)
{
#if __SDCC_BITINT_MAXWIDTH >= {width} // TODO: When we can regression-test in --std-c23 mode, use the standard macro from limits.h instead!

	a = -1, b = 1;
	ASSERT(a + b == 0);
	ua = 1, ub = 1;
	ASSERT(ua + ub == 2);

	// From here on, we do the computation on the left at runtime, the one on the right at compile time, to check both.

	a = 23, b = -42;
	ua = 23, ub = -42;

#if {width} >= 8 // Signed overflow is undefined behaviour	
	ASSERT(a + b == (bitinttype)(23) + (bitinttype)(-42));
	ASSERT(a - b == (bitinttype)(23) - (bitinttype)(-42));
#if {width} >= 11 // Signed overflow is undefined behaviour
	ASSERT(a * b == (bitinttype)(23) * (bitinttype)(-42));
#endif
	ASSERT(a % b == (bitinttype)(23) % (bitinttype)(-42));
	ASSERT(a / b == (bitinttype)(23) / (bitinttype)(-42));
#endif
	// Wrap on unsigned overflow
	ASSERT(ua + ub == (ubitinttype)(23) + (ubitinttype)(-42));
	ASSERT(ua - ub == (ubitinttype)(23) - (ubitinttype)(-42));
	ASSERT(ua * ub == (ubitinttype)(23) * (ubitinttype)(-42));
	ASSERT(ua % ub == (ubitinttype)(23) % (ubitinttype)(-42));
	ASSERT(ua / ub == (ubitinttype)(23) / (ubitinttype)(-42));

	ASSERT((a < b) == ((bitinttype)(23) < (bitinttype)(-42)));
	ASSERT((a <= b) == ((bitinttype)(23) <= (bitinttype)(-42)));
	ASSERT((ua < ub) == ((ubitinttype)(23) < (ubitinttype)(-42)));
	ASSERT((ua <= ub) == ((ubitinttype)(23) <= (ubitinttype)(-42)));

	ASSERT((ua & ub) == ((ubitinttype)(23) & (ubitinttype)(-42)));
	ASSERT((ua | ub) == ((ubitinttype)(23) | (ubitinttype)(-42)));
	ASSERT((ua ^ ub) == ((ubitinttype)(23) ^ (ubitinttype)(-42)));
	
#if {width} >= 4
	a = 3, ua = 3;
	ASSERT (a << 1 == (bitinttype)(3) << 1);
	ASSERT (ua << 1 == (ubitinttype)(3) << 1);
#endif
	ASSERT (a >> 1 == (bitinttype)(3) >> 1);
	ASSERT (ua >> 1 == (ubitinttype)(3) >> 1);

#endif
}

