#define SDCC_FLOAT_LIB
#include <float.h>


static void dummy(void) _naked
{
	_asm
	.globl	fs_rshift_a
fs_rshift_a:
	jz	00020$
	mov	r0, a
	add	a, exp_a	// adjust exponent
	jnc	00001$
	mov	a, #255		// don't roll over
00001$:
	mov	exp_a, a
#ifdef FLOAT_SHIFT_SPEEDUP
	mov	a, r0
	add	a, #248
	jnc	00003$
	xch	a, r4
	xch	a, r3
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r4
	//mov	r1, ar2		// avoid dependence on register bank
	//mov	r2, ar3
	//mov	r3, ar4
	//mov	r4, #0
	add	a, #248
	jnc	00003$
	xch	a, r3
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r3
	//mov	r1, ar2
	//mov	r2, ar3
	//mov	r3, #0
	add	a, #248
	jnc	00003$
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r2
	//mov	r1, ar2
	//mov	r2, #0
	add	a, #248
	jnc	00003$
	mov	r1, #0
	ret
00003$:
	add	a, #8
	jz	00020$
	mov	r0, a
#endif
00005$:
	clr	c
	mov	a, r4
	rrc	a
	mov	r4, a
	mov	a, r3
	rrc	a
	mov	r3, a
	mov	a, r2
	rrc	a
	mov	r2, a
	mov	a, r1
	rrc	a
	mov	r1, a
	djnz	r0, 00005$
00020$:
	ret
	_endasm;
}

