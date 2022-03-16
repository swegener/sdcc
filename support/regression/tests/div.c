/*  Test return of div functions

 */
#include <testfwk.h>

#include <stdlib.h>

void testDiv(void)
{
#ifndef __SDCC_mcs51 // mcs51 does not yet support returning struct
	ASSERT (div(4223, 23).quot == 4223 / 23);
	ASSERT (div(4223, 23).rem == 4223 % 23);
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Lack of memory
	ASSERT (ldiv(4223, 23).quot == 4223l / 23);
	ASSERT (ldiv(4223, 23).rem == 4223l % 23);
#if !defined(__SDCC_ds390) && !defined (__SDCC_hc08) && !defined (__SDCC_s08) && !defined(__SDCC_mos6502) && !defined (__SDCC_mos65c02)
	// segfault on compiling lldiv
	ASSERT (lldiv(4223, 23).quot == 4223ll / 23);
	ASSERT (lldiv(4223, 23).rem == 4223ll % 23);
#endif
#endif
#endif
}

