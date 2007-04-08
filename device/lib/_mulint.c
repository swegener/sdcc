/*-------------------------------------------------------------------------
  _mulint.c :- routine for (unsigned) int (16 bit) multiplication

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

/* Signed and unsigned multiplication are the same - as long as the output
   has the same precision as the input.

   Assembler-functions are provided for:
     ds390
     mcs51 small
     mcs51 small stack-auto
     mcs51 large
*/

#if !defined(SDCC_USE_XSTACK) && !defined(_SDCC_NO_ASM_LIB_FUNCS)
#  if defined(SDCC_ds390)
#    if !defined(SDCC_STACK_AUTO)
#      define _MULINT_ASM_LARGE
#    endif
#  elif defined(SDCC_mcs51)
#    if defined(SDCC_MODEL_SMALL)
#      if defined(SDCC_STACK_AUTO) && !defined(SDCC_PARMS_IN_BANK1)
#        define _MULINT_ASM_SMALL_AUTO
#      else
#        define _MULINT_ASM_SMALL
#      endif
#    else // must be SDCC_MODEL_LARGE
#      if !defined(SDCC_STACK_AUTO)
#        define _MULINT_ASM_LARGE
#     endif
#   endif
#  endif
#endif

#if defined(_MULINT_ASM_LARGE)

#pragma save
#pragma less_pedantic
int
_mulint (int a, int b)
{
  a*b; // hush the compiler

  /* mulint=
      (int)(lsb_a*lsb_b) +
      (char)(msb_a*lsb_b)<<8 +
      (char)(lsb_a*msb_b)<<8
  */

  __asm
    mov r2,dph ; msb_a
    mov r3,dpl ; lsb_a

    mov b,r3 ; lsb_a
#if defined(SDCC_PARMS_IN_BANK1)
    mov a,b1_0
#else
    mov dptr,#__mulint_PARM_2
    movx a,@dptr ; lsb_b
#endif
    mul ab ; lsb_a*lsb_b
    mov r0,a
    mov r1,b

    mov b,r2 ; msb_a
#if defined(SDCC_PARMS_IN_BANK1)
    mov a,b1_0
#else
    movx a,@dptr ; lsb_b
#endif
    mul ab ; msb_a*lsb_b
    add a,r1
    mov r1,a

    mov b,r3 ; lsb_a
#if defined(SDCC_PARMS_IN_BANK1)
    mov a,b1_1
#else
    inc dptr
    movx a,@dptr ; msb_b
#endif
    mul ab ; lsb_a*msb_b
    add a,r1

    mov dph,a
    mov dpl,r0
    ret
  __endasm;
}
#pragma restore

#elif defined(_MULINT_ASM_SMALL) || defined(_MULINT_ASM_SMALL_AUTO)

#pragma save
#pragma less_pedantic
int
_mulint_dummy (void) __naked
{
	__asm

__mulint:

	.globl __mulint

#if !defined(SDCC_STACK_AUTO) || defined(SDCC_PARMS_IN_BANK1)

#if defined(SDCC_NOOVERLAY)
	.area DSEG    (DATA)
#else
	.area OSEG    (OVR,DATA)
#endif
#if defined(SDCC_PARMS_IN_BANK1)
	#define bl 	(b1_0)
	#define bh 	(b1_1)
#else
	#define bl 	(__mulint_PARM_2)
	#define bh 	(__mulint_PARM_2 + 1)
__mulint_PARM_2:

	.globl __mulint_PARM_2

	.ds	2
#endif

	.area CSEG    (CODE)

	; globbered registers none

	mov	a,dpl			;  1  al
	mov	b,bl			;  2  bl
	mul	ab			;  4  al * bl
	xch	a,dpl			;  1  store low-byte of return value, fetch al
	push	b			;  2

	mov	b,bh			;  2  bh
	mul	ab			;  4  al * bh
	pop	b			;  2
	add	a,b			;  1
	xch	a,dph			;  1  ah -> acc

	mov	b,bl			;  2  bl
	mul	ab			;  4  ah * bl
	add	a,dph			;  1
	mov	dph,a			;  1
	ret				;  2
					; 30

#else // SDCC_STACK_AUTO

	; globbered registers r0

	mov	a,#-2			;  1  return address 2 bytes
	add	a,sp			;  1
	mov	r0,a			;  1  r0 points to bh

	mov	a,@r0			;  1  bh
	mov	b,dpl			;  2  al
	mul	ab			;  4  al * bh
	push	acc			;  2

	mov	b,dpl			;  2  al
	dec	r0			;  1
	mov	a,@r0			;  1  bl
	mul	ab			;  4  al * bl

	mov	dpl,a			;  1  low-byte of return-value

	pop	acc			;  2
	add	a,b			;  1
	xch	a,dph			;  1  ah -> acc

	mov	b,@r0			;  2  bl
	mul	ab			;  4  ah * bl
	add	a,dph			;  1
	mov	dph,a			;  1

	ret

#endif // SDCC_STACK_AUTO

	__endasm;
}
#pragma restore

#else

union uu {
	struct { unsigned char lo,hi ;} s;
        unsigned int t;
} ;

int
_mulint (int a, int b)
{
#if !defined(SDCC_STACK_AUTO) && (defined(SDCC_MODEL_LARGE) || defined(SDCC_ds390))	// still needed for large
	union uu __xdata *x;
	union uu __xdata *y;
	union uu t;
        x = (union uu __xdata *)&a;
        y = (union uu __xdata *)&b;
#else
	register union uu *x;
	register union uu *y;
	union uu t;
        x = (union uu *)&a;
        y = (union uu *)&b;
#endif

        t.t = x->s.lo * y->s.lo;
        t.s.hi += (x->s.lo * y->s.hi) + (x->s.hi * y->s.lo);

       return t.t;
}

#endif

#undef _MULINT_ASM
