/*-------------------------------------------------------------------------

  _divuint.c :- routine for unsigned int (16 bit) division

             Ecrit par -  Jean-Louis Vern . jlvern@writeme.com (1999)

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

#define MSB_SET(x) ((x >> (8*sizeof(x)-1)) & 1)

unsigned int _divuint (unsigned int a, unsigned int b)
{
	unsigned int reste = 0	;
	unsigned short count = 16;
	bit c;

	do{
		// reste:a <- 0;
		c = MSB_SET(a);
		a <<= 1;
		reste <<= 1;
		if(c)
			reste |= 1;
		
		if(reste >= b){
			reste -= b;
			// a <- (result = 1)
			a |= 1;
		}
	} while(--count);
	
	return a;
}
