/*  Test struct / union parameters

    type: int, long, long long

 */

#include <testfwk.h>

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Todo: enable when struct parameters are supported!
struct s
{
	{type} a;
	{type} b;
};

// Callee
{type} f(struct s s)
{
	return s.a + s.b;
}

// Caller
{type} g({type} i, {type} j)
{
	struct s s = {i, j};
	return f(s);
}
#endif

void testParam (void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Todo: enable when struct parameters are supported!
	ASSERT (g(23, 42) == 23 + 42);
#endif
}

