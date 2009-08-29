/*-------------------------------------------------------------------------
  _strlen.c - part of string library functions

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)
                           mcs51 assembler by Frieder Ferlemann (2007)

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

#if (!defined (SDCC_mcs51))

  /* Generic routine first */
  int strlen ( const char * str )
  {
    register int i = 0 ;

    while (*str++)
      i++ ;

    return i;
  }

#else

  /* Assembler version for mcs51 */
  int strlen ( const char * str ) __naked
  {
    str;     /* hush the compiler */

    __asm
      ; dptr holds pointer
      ; b holds pointer memspace
      ;

      ; char *ptr = str:
      mov     r2,dpl
      mov     r3,dph
      ;

      ; while ( *ptr ) ptr++;
    L00101$:
      lcall   __gptrget
      jz      L00102$
      inc     dptr
      sjmp    L00101$
      ;

    L00102$:
      ; return ptr - str;
      clr     c
      mov     a,dpl
      subb    a,r2
      mov     dpl,a
      ;
      mov     a,dph
      subb    a,r3
      mov     dph,a
      ;
      ret
    __endasm;
  }

#endif
