/*-------------------------------------------------------------------------

  _muluint.c :- routine for unsigned int (16 bit) multiplication               

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

#ifdef SDCC_MODEL_FLAT24

unsigned int _muluint (unsigned int a, unsigned int b) 
{
  a*b; // hush the compiler

  /* muluint=
      (int)(lsb_a*lsb_b) +
      (char)(msb_a*lsb_b)<<8 +
      (char)(lsb_a*msb_b)<<8
  */

  _asm 
    mov r2,dph ; msb_a
    mov r3,dpl ; lsb_a

    mov b,r3 ; lsb_a
    mov dptr,#__muluint_PARM_2
    movx a,@dptr ; lsb_b
    mul ab ; lsb_a*lsb_b
    mov r0,a
    mov r1,b

    mov b,r2 ; msb_a
    movx a,@dptr ; lsb_b
    mul ab ; msb_a*lsb_b
    add a,r1
    mov r1,a

    mov b,r3 ; lsb_a
    inc dptr
    movx a,@dptr ; msb_b
    mul ab ; lsb_a*msb_b
    add a,r1

    mov dph,a
    mov dpl,r0
    ret
  _endasm;
}

#else

union uu {
	struct { unsigned short lo,hi ;} s;
        unsigned int t;
} ;

unsigned int _muluint (unsigned int a, unsigned int b) 
{
#ifdef SDCC_MODEL_LARGE
	union uu _xdata *x;
	union uu _xdata *y; 
	union uu t;
        x = (union uu _xdata *)&a;
        y = (union uu _xdata *)&b;
#else
	register union uu _near *x;
	register union uu _near *y; 
	union uu t;
        x = (union uu _near *)&a;
        y = (union uu _near *)&b;
#endif

        t.t = x->s.lo * y->s.lo;
        t.s.hi += (x->s.lo * y->s.hi) + (x->s.hi * y->s.lo);

       return t.t;
} 

#endif
