/*-------------------------------------------------------------------------
  _modschar.c :- routine for signed char (8 bit) modulus

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)
	     Adopted for char (8-bit) and pic16 port by
	     		- Vangelis Rokas, vrokas@otenet.gr (2004)

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

unsigned char _moduchar (unsigned char a, unsigned char b);

char _modschar (char a, char b)
{
  register char r;
  register char ta, tb;

	if(a<0)ta = -a; else ta = a;
	if(b<0)tb = -b; else tb = b;
	
//	r = _moduchar((a < 0 ? -a : a),
//	            (b < 0 ? -b : b));
	r = _moduchar(ta, tb);

       if (a < 0)
	    return -r;
	else
	    return r;
}        	

