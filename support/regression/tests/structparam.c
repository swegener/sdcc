/*  Test struct / union parameters

    type: int, long, long long

 */

#include <testfwk.h>

#if defined(__SDCC_stm8) || defined(__SDCC_z80) || defined(__SDCC_sm83) || defined(__SDCC_r3ka) // Many ports don't support struct parameters yet.
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
#if defined(__SDCC_stm8) || defined(__SDCC_z80) || defined(__SDCC_sm83) || defined(__SDCC_r3ka) // Many ports don't support struct parameters yet.
	ASSERT (g(23, 42) == 23 + 42);
#endif
}

