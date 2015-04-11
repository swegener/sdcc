/** z88dk.c
*/
#include <testfwk.h>
#include <stdlib.h>

#if !defined(__SDCC_z80) && !defined(__SDCC_z180) && !defined(__SDCC_r2k) && !defined(__SDCC_r3ka) && !defined(__SDCC_tlcs90)
#define __z88dk_fastcall
#endif

unsigned char f1(unsigned char c) __z88dk_fastcall
{
	return c + 1;
}

unsigned int f2(unsigned int c) __z88dk_fastcall
{
	return c + 1;
}

unsigned long int f4(unsigned long int c) __z88dk_fastcall
{
	return c + 1;
}

void
testZ88dk(void)
{
#if 0 // Peephole optimizer bug #2371.
  ASSERT (f1 (23) == 24);
  ASSERT (f2 (23) == 24);
  ASSERT (f4 (23) == 24);
#endif
}

