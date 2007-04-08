/*-------------------------------------------------------------------------
   _divslong.c - routine for division of 32 bit long

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


#include <sdcc-lib.h>

#if _SDCC_MANGLES_SUPPORT_FUNS
unsigned long _divulong (unsigned long x, unsigned long y);
#endif

/*   Assembler-functions are provided for:
     mcs51 small
     mcs51 small stack-auto
*/

#if !defined(SDCC_USE_XSTACK) && !defined(_SDCC_NO_ASM_LIB_FUNCS)
#  if defined(SDCC_mcs51)
#    if defined(SDCC_MODEL_SMALL)
#      if defined(SDCC_STACK_AUTO) && !defined(SDCC_PARMS_IN_BANK1)
#        define _DIVSLONG_ASM_SMALL_AUTO
#      else
#        define _DIVSLONG_ASM_SMALL
#      endif
#    endif
#  endif
#endif

#if defined _DIVSLONG_ASM_SMALL

static void
_divslong_dummy (void) __naked
{
	__asm

	#define x0	dpl
	#define x1	dph
	#define x2	b
	#define x3	r3

	.globl __divslong

	// _divslong_PARM_2 shares the same memory with _divulong_PARM_2
	// and is defined in _divulong.c
#if defined(SDCC_PARMS_IN_BANK1)
	#define y0      (b1_0)
	#define y1      (b1_1)
	#define y2      (b1_2)
	#define y3      (b1_3)
#else
	#define y0      (__divslong_PARM_2)
	#define y1      (__divslong_PARM_2 + 1)
	#define y2      (__divslong_PARM_2 + 2)
	#define y3      (__divslong_PARM_2 + 3)
#endif
__divslong:
				; x3 in acc
				; y3 in (__divslong_PARM_2 + 3)
	mov	x3,a		; save x3

	clr	F0 		; Flag 0 in PSW
				; available to user for general purpose
	jnb	acc.7,a_not_negative

	setb	F0

	clr	a
	clr	c
	subb	a,x0
	mov	x0,a
	clr	a
	subb	a,x1
	mov	x1,a
	clr	a
	subb	a,x2
	mov	x2,a
	clr	a
	subb	a,x3
	mov	x3,a

a_not_negative:

	mov	a,y3
	jnb	acc.7,b_not_negative

	cpl	F0

	clr	a
	clr	c
	subb	a,y0
	mov	y0,a
	clr	a
	subb	a,y1
	mov	y1,a
	clr	a
	subb	a,y2
	mov	y2,a
	clr	a
	subb	a,y3
	mov	y3,a

b_not_negative:

	mov	a,x3		; restore x3 in acc

	lcall	__divulong

	jnb	F0,not_negative

	mov	x3,a		; save x3

	clr	a
	clr	c
	subb	a,x0
	mov	x0,a
	clr	a
	subb	a,x1
	mov	x1,a
	clr	a
	subb	a,x2
	mov	x2,a
	clr	a
	subb	a,x3	; x3 ends in acc

not_negative:
	ret

	__endasm;
}

#elif defined _DIVSLONG_ASM_SMALL_AUTO

static void
_divslong_dummy (void) __naked
{
	__asm

	#define x0	dpl
	#define x1	dph
	#define x2	b
	#define x3	r3

	.globl __divslong

__divslong:

				; x3 in acc
	mov	x3,a		; save x3

	clr	F0 		; Flag 0 in PSW
				; available to user for general purpose
	jnb	acc.7,a_not_negative

	setb	F0

	clr	a
	clr	c
	subb	a,x0
	mov	x0,a
	clr	a
	subb	a,x1
	mov	x1,a
	clr	a
	subb	a,x2
	mov	x2,a
	clr	a
	subb	a,x3
	mov	x3,a

a_not_negative:

	mov	a,sp
	add	a,#-2		; 2 bytes return address
	mov	r0,a		; r0 points to y3
	mov	a,@r0		; y3

	jnb	acc.7,b_not_negative

	cpl	F0

	dec	r0
	dec	r0
	dec	r0

	clr	a
	clr	c
	subb	a,@r0		; y0
	mov	@r0,a
	clr	a
	inc	r0
	subb	a,@r0		; y1
	mov	@r0,a
	clr	a
	inc	r0
	subb	a,@r0		; y2
	mov	@r0,a
	clr	a
	inc	r0
	subb	a,@r0		; y3
	mov	@r0,a

b_not_negative:
	dec	r0
	dec	r0
	dec	r0		; r0 points to y0

	lcall	__divlong

	jnb	F0,not_negative

	mov	x3,a		; save x3

	clr	a
	clr	c
	subb	a,x0
	mov	x0,a
	clr	a
	subb	a,x1
	mov	x1,a
	clr	a
	subb	a,x2
	mov	x2,a
	clr	a
	subb	a,x3		; x3 ends in acc

not_negative:
	ret

	__endasm;
}

#else // _DIVSLONG_ASM

long
_divslong (long x, long y)
{
  long r;

  r = _divulong((x < 0 ? -x : x),
                (y < 0 ? -y : y));
  if ( (x < 0) ^ (y < 0))
    return -r;
  else
    return r;
}

#endif // _DIVSLONG_ASM
