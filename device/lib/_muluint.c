/*-------------------------------------------------------------------------

  _muluint.c :- routine for unsigned int (16 bit) multiplication               

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

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
