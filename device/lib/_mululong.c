/*-------------------------------------------------------------------------
   _mululong.c - routine for multiplication of 32 bit unsigned long

             Written By -  Jean Louis VERN jlvern@writeme.com (1999)

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

struct some_struct {
	int a ;
	char b;
	long c ;};
union bil {
        struct {unsigned char b0,b1,b2,b3 ;} b;
        struct {unsigned int lo,hi ;} i;
        unsigned long l;
        struct { unsigned char b0; unsigned int i12; unsigned char b3;} bi;
} ;
#if defined(SDCC_MODEL_LARGE) || defined (SDCC_MODEL_FLAT24)
#define bcast(x) ((union bil _xdata  *)&(x))
#else
#define bcast(x) ((union bil _near *)&(x))
#endif

/*
                     3   2   1   0
       X             3   2   1   0
       ----------------------------
                   0.3 0.2 0.1 0.0 
               1.3 1.2 1.1 1.0 
           2.3 2.2 2.1 2.0 
       3.3 3.2 3.1 3.0 
       ----------------------------
                  |3.3|1.3|0.2|0.0|   A
                    |2.3|0.3|0.1|     B
                    |3.2|1.2|1.0|     C
                      |2.2|1.1|       D
                      |3.1|2.0|       E
                        |2.1|         F
                        |3.0|         G
                          |-------> only this side 32 x 32 -> 32
*/
unsigned long _mululong (unsigned long a, unsigned long b) 
{
        union bil t;

        t.i.hi = (unsigned int)bcast(a)->b.b0 * bcast(b)->b.b2;       // A
        t.i.lo = (unsigned int)bcast(a)->b.b0 * bcast(b)->b.b0;       // A
        t.b.b3 += (unsigned char)(bcast(a)->b.b3 *
                                  bcast(b)->b.b0);      // G
        t.b.b3 += (unsigned char)(bcast(a)->b.b2 *
                                  bcast(b)->b.b1);      // F
        t.i.hi += (unsigned int)bcast(a)->b.b2 * bcast(b)->b.b0;      // E <- b lost in .lst
        // bcast(a)->i.hi is free !
        t.i.hi += (unsigned int)bcast(a)->b.b1 * bcast(b)->b.b1;      // D <- b lost in .lst

        bcast(a)->bi.b3 = (unsigned char)(bcast(a)->b.b1 *
                                          bcast(b)->b.b2);
        bcast(a)->bi.i12 = (unsigned int)bcast(a)->b.b1 *
                           bcast(b)->b.b0;              // C

        bcast(b)->bi.b3 = (unsigned char)(bcast(a)->b.b0 *
                                          bcast(b)->b.b3);
        bcast(b)->bi.i12 = (unsigned int)bcast(a)->b.b0 *
                           bcast(b)->b.b1;              // B
        bcast(b)->bi.b0 = 0;                            // B
        bcast(a)->bi.b0 = 0;                            // C
        t.l += a;

        return t.l + b;
}
