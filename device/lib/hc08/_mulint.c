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
     hc08
     hc08 stack-auto
*/


#if !defined(_SDCC_NO_ASM_LIB_FUNCS)

#pragma save
#pragma less_pedantic
int
_mulint (int a, int b)
{
  a,b;	/* reference to make compiler happy */

#if !defined(SDCC_STACK_AUTO)
    __asm
   	ais #-2
	psha
	pshx

	ldx __mulint_PARM_2+1
	mul
	sta 4,s
	stx 3,s

	lda 1,s
	ldx __mulint_PARM_2+1
	mul
	add 3,s
	sta 3,s

	lda 2,s
	ldx __mulint_PARM_2
	mul
	add 3,s
	sta 3,s

	ais #2
	pulx
	pula
    __endasm;
#else
    __asm
   	ais #-2
	psha
	pshx

	ldx 8,s
	mul
	sta 4,s
	stx 3,s

	lda 1,s
	ldx 8,s
	mul
	add 3,s
	sta 3,s

	lda 2,s
	ldx 7,s
	mul
	add 3,s
	sta 3,s

	ais #2
	pulx
	pula
    __endasm;
#endif
}
#pragma restore

#else

union uu {
	struct { unsigned char hi,lo ;} s;
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
