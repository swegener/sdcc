#define SDCC_FLOAT_LIB
#include <float.h>


static void dummy(void) _naked
{
	// input passed in a,b,dph,dpl
	_asm
	.globl	fsgetarg
fsgetarg:
	// extract the input, placing it into:
	//      sign     exponent   mantiassa
	//      ----     --------   ---------
	//  a:  sign_a   exp_a     r4/r3/r2
	//
	mov	r2, dpl
	mov	r3, dph
	mov	c, b.7
	rlc	a
	mov	sign_a, c
	jz	00001$
	setb	b.7
00001$:
	mov	exp_a, a
	mov	r4, b
	ret
	_endasm;
}




