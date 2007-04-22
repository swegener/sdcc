/*-------------------------------------------------------------------------
  _memset.c - part of string library functions

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
#include "string.h" 

#if defined (_SDCC_NO_ASM_LIB_FUNCS) || !defined (SDCC_mcs51) || \
    (!defined (SDCC_MODEL_SMALL) && !defined (SDCC_MODEL_LARGE)) || \
     (defined (SDCC_STACK_AUTO) || defined (SDCC_PARMS_IN_BANK1) )

  void * memset (
          void * buf,
          unsigned char ch ,
          size_t count) 
  {
          register unsigned char * ret = buf;

          while (count--) {
                  *(unsigned char *) ret = ch;
                  ret = ((unsigned char *) ret) + 1;
          }

          return buf ;
  }

#else

  /* assembler implementation for mcs51 */
  static void dummy(void) __naked
  {
    __asm

  /* assigning function parameters to registers.
     SDCC_PARMS_IN_BANK1 or SDCC_STACK_AUTO not yet implemented. */
  #if defined (SDCC_MODEL_SMALL)

    #if defined(SDCC_NOOVERLAY)
        .area DSEG    (DATA)
    #else
        .area OSEG    (OVR,DATA)
    #endif
        _memset_PARM_2::
              .ds 1
        _memset_PARM_3::
              .ds 2

        .area CSEG    (CODE)

        _memset::

        ;   Assign buf (b holds memspace, no need to touch)
                mov     r4,dpl
                mov     r5,dph
                ;
        ;   Assign count
                mov     r6,_memset_PARM_3
                mov     r7,(_memset_PARM_3 + 1)
                ;
        ;   if (!count) return buf;
        ;   check for count != 0 intermangled with gymnastic
        ;   preparing djnz instructions
                cjne    r6,#0x00,COUNT_LSB_NOT_ZERO
                mov     a,r7
                jz      MEMSET_END
                dec     r7
        COUNT_LSB_NOT_ZERO:
                inc     r7
                ;
                ; This was 8 byte overhead for preparing
                ; the count argument for an integer loop with two
                ; djnz instructions - it might make sense to
                ; let SDCC automatically generate this when
                ; it encounters a loop like:
                ; for(i=0;i<j;i++){...}
                ; (at least for option --opt-code-speed)
                ;

        ;   Assign ch
                mov     a,_memset_PARM_2

  #else

        .area XSEG    (XDATA)

        _memset_PARM_2::
                .ds 1
        _memset_PARM_3::
                .ds 2

        .area CSEG    (CODE)

        _memset::

        ;   Assign buf (b holds memspace, no need to touch)
                mov     r4,dpl
                mov     r5,dph
                ;
        ;   Assign count
                mov     dptr,#_memset_PARM_3
                movx    a,@dptr
                mov     r6,a
                inc     dptr
                movx    a,@dptr
                mov     r7,a
                ;
        ;   if (!count) return buf;
        ;   check for count != 0 intermangled with gymnastic
        ;   preparing djnz instructions
                cjne    r6,#0x00,COUNT_LSB_NOT_ZERO
        ;   acc holds r7
                jz      MEMSET_END
                dec     r7
        COUNT_LSB_NOT_ZERO:
                inc     r7
                ;
        ;   Assign ch
                mov     dptr,#_memset_PARM_2
                movx    a,@dptr
        ;   acc is precious now
                ;
        ;   Restore dptr
                mov     dpl,r4
                mov     dph,r5

  #endif

        /* now independent of the parameter passing everything
           should be in registers by now and the loop may start */
        ;   _memset.c do {

        MEMSET_LOOP:
        ;   _memset.c *p = ch;
                lcall   __gptrput

        ;   _memset.c p++;
                inc     dptr

        ;   _memset.c } while(--count) ;
                djnz    r6,MEMSET_LOOP
                djnz    r7,MEMSET_LOOP
                ;

        MEMSET_END:
        ;   _memset.c return buf ;
                ; b was unchanged
                mov     dpl,r4
                mov     dph,r5
                ;
                ret

    __endasm;
  }

#endif
