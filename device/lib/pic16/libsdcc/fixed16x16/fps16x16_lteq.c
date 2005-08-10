/*-------------------------------------------------------------------------
   fps16x16_lteq.c - compare two __fixed16x16 values

             written by - Raphael Neider, rneider@web.de (2005)
   
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

   $Id$
-------------------------------------------------------------------------*/

union u_t {
  long value;
  __fixed16x16 fix;
};

char
__fps16x16_lteq (__fixed16x16 a, __fixed16x16 b)
{
  union u_t u1, u2;

  u1.fix = a;
  u2.fix = b;
  
  return (u1.value <= u2.value);
}

