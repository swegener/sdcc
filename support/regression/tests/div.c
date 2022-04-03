/*  Test return of div functions

 */
#include <testfwk.h>

#include <stdlib.h>

void testDiv(void)
{
#if !(defined(__SDCC_mcs51) && defined(__SDCC_STACK_AUTO)) // bug ?
#if !defined(__SDCC_ds390) && !defined(__SDCC_ds390) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(__SDCC_mos6502) // struct return not yet supported
	ASSERT (div(4223, 23).quot == 4223 / 23);
	ASSERT (div(4223, 23).rem == 4223 % 23);
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) && !(defined(__SDCC_mcs51) && defined(__SDCC_MODEL_SMALL))// Lack of memory
	ASSERT (ldiv(4223, 23).quot == 4223l / 23);
	ASSERT (ldiv(4223, 23).rem == 4223l % 23);
	ASSERT (lldiv(4223, 23).quot == 4223ll / 23);
	ASSERT (lldiv(4223, 23).rem == 4223ll % 23);
#endif
#endif
#endif
}

