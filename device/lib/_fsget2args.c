#define SDCC_FLOAT_LIB
#include <float.h>




static void dummy(void) _naked
{
	// arg1: passed in a,b,dph,dpl
	// arg2: passed on stack
	_asm
	.globl	fsgetargs
fsgetargs:
	// extract the two inputs, placing them into:
	//      sign     exponent   mantiassa
	//      ----     --------   ---------
	//  a:  sign_a   exp_a     r4/r3/r2
	//  b:  sign_b   exp_b     r7/r6/r5
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
	// now extract the 2nd parameter from the stack
	mov	a, sp
	add	a, #249
	mov	r0, a
	mov	a, @r0
	mov	r5, a
	inc	r0
	mov	a, @r0
	mov	r6, a
	inc	r0
	mov	b, @r0
	inc	r0
	mov	a, @r0
	mov	c, b.7
	rlc	a
	mov	sign_b, c
	jz	00002$
	setb	b.7
00002$:
	mov	exp_b, a
	mov	r7, b
	ret
	_endasm;
}





#if 0
// This old version was designed before the change to make all this
// code fully reentrant.  What a mess the 2nd parameter turns out to
// be.

void __fsgetargs (float a, float b)
{
	a;	// passed in a,b,dph,dpl
	b;	// ___fsadd_PARM_2

	_asm
	// extract the two inputs, placing them into:
	//      sign     exponent   mantiassa
	//      ----     --------   ---------
	//  a:  sign_a   exp_a     r4/r3/r2
	//  b:  sign_b   exp_b     r7/r6/r5
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
#ifdef SDCC_MODEL_SMALL
	mov	r5, (___fsadd_PARM_2 + 0)
	mov	r6, (___fsadd_PARM_2 + 1)
	mov	b, (___fsadd_PARM_2 + 2)
	mov	a, (___fsadd_PARM_2 + 3)
	mov	c, b.7
	rlc	a
	mov	sign_b, c
	jz	00002$
	setb	b.7
00002$:
	mov	exp_b, a
	mov	r7, b
#endif
	_endasm;
}


#ifdef SDCC_MODEL_LARGE
void __fsgetarglarge2 (void)
{
	_asm
	movx	a, @dptr
	mov	r5, a
	inc	dptr
	movx	a, @dptr
	mov	r6, a
	inc	dptr
	movx	a, @dptr
	mov	b, a
	inc	dptr
	movx	a, @dptr
	mov	c, b.7
	rlc	a
	mov	sign_b, c
	jz	00002$
	setb	b.7
00002$:
	mov	exp_b, a
	mov	r7, b
	_endasm;
}
#endif

#endif

