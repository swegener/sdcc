#define SDCC_FLOAT_LIB
#include <float.h>

static void dummy(void) _naked
{
	_asm
	.globl	fs_swap_a_b
fs_swap_a_b:
	mov	a, exp_a
	xch	a, exp_b
	mov	exp_a, a
	// is there a faster way to swap these 2 bits???
	// this trick with psw can play havoc with the resiter bank setting
	// mov	a, psw
	// swap	a		// depends on sign bits in psw.1 & psw.5
	// mov	psw, a
	 mov	c, sign_a
	 rlc	a
	 mov	c, sign_b
	 mov	sign_a, c
	 rrc	a
	 mov	sign_b, c
	mov	a, r4
	xch	a, r7
	mov	r4, a
	mov	a, r3
	xch	a, r6
	mov	r3, a
	mov	a, r2
	xch	a, r5
	mov	r2, a
	ret
	_endasm;
}

