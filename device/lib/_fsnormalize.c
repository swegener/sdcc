#define SDCC_FLOAT_LIB
#include <float.h>

static void dummy(void) _naked
{
	_asm
	.globl	fs_normalize_a
fs_normalize_a:
#ifdef FLOAT_SHIFT_SPEEDUP
	mov	r0, #4
00001$:
	mov	a, r4
	jnz	00003$
	xch	a, r1
	xch	a, r2
	xch	a, r3
	mov	r4, a
	//mov	r4, ar3
	//mov	r3, ar2
	//mov	r2, ar1
	//mov	r1, #0
	mov	a, exp_a
	add	a, #248
	mov	exp_a, a
	djnz	r0, 00001$
	ret
#else
	mov	a, r4
#endif
00003$:
	mov	r0, #32
00005$:
	jb	acc.7, 00006$
	dec	exp_a
	clr	c
	mov	a, r1
	rlc	a
	mov	r1, a
	mov	a, r2
	rlc	a
	mov	r2, a
	mov	a, r3
	rlc	a
	mov	r3, a
	mov	a, r4
	rlc	a
	mov	r4, a
	djnz	r0, 00005$
00006$:
	ret
	_endasm;
}
