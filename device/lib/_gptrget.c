/*-------------------------------------------------------------------------

  _gptrget.c :- get value for a generic pointer

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

/* not all devices use P2 to page pdata memory, therefore __XPAGE was
   introduced. On some targets __XPAGE itself is a paged SFR so it is
   not safe for all platforms to set this. Furthermore some targets
   can be configured to behave differently for movx @dptr vs. movx @Ri
   (don't drive high byte of address bus for movx @Ri only) */
#define USE_PDATA_PAGING_REGISTER 0

/* the  return value is expected to be in acc, and not in the standard
 * location dpl. Therefore we choose return type void here: */
void
_gptrget (char *gptr) _naked
{
    gptr; /* hush the compiler */

    _asm
	ar0 = 0x00
    ;   save values passed
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
        mov     a,b
        jz      00001$  ; 0 near
        dec     a
        jz      00002$  ; 1 far
        dec     a
        jz      00003$  ; 2 code
        dec     a
        jz      00004$  ; 3 pdata
        dec     a       ; 4 skip generic pointer
        dec     a
        jz      00001$  ; 5 idata
    ;
    ;   any other value for type
    ;   return xFF
        mov     a,#0xff
        sjmp    00005$
    ;
    ;   Pointer to data space
    ;
 00001$:
        push    ar0
        ;
        mov     r0,dpl     ; use only low order address
        mov     a,@r0
        ;
        pop     ar0
        ;
        sjmp    00005$
    ;
    ;   pointer to xternal data
    ;
 00002$:
        movx    a,@dptr
        sjmp    00005$
;
;   pointer to code area
;
 00003$:
        ; clr     a  is already 0
        movc    a,@a+dptr
        sjmp    00005$
;
;   pointer to xternal stack or pdata
;
 00004$:
#if USE_PDATA_PAGING_REGISTER
        mov     dph,__XPAGE     ; __XPAGE (usually p2) holds high byte for pdata access
        movx    a,@dptr
#else
        push    ar0
        mov     r0,dpl
        movx    a,@r0
        pop     ar0
#endif

;
;   return
;
 00005$:
        ret
     _endasm ;
}

#ifdef SDCC_ds390
/* the  return value is expected to be in acc/_ap, and not in the standard
 * location dpl/dph. Therefore we choose return type void here: */
void
_gptrgetWord (unsigned *gptr)
{
    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
        mov     a,b
        jz      00001$  ; 0 near
        dec     a
        jz      00002$  ; 1 far
        dec     a
        jz      00003$  ; 2 code
        dec     a
        jz      00004$  ; 3 pdata
        dec     a       ; 4 skip generic pointer
        dec     a
        jz      00001$  ; 5 idata
    ;
    ;   any other value for type
    ;   return xFF
        mov     a,#0xff
        sjmp    00006$
    ;
    ;   Pointer to data space
    ;
 00001$:
        push    ar0
        mov     r0,dpl     ; use only low order address
        mov     _ap,@r0
        inc     r0
        mov     a,@r0
        inc     dpl
        sjmp    00005$
    ;
    ;   pointer to xternal data
    ;
 00002$:
        movx    a,@dptr
        mov     _ap,a
        inc     dptr
        movx    a,@dptr
        sjmp    00006$
;
;   pointer to code area
;
 00003$:
        ; clr     a  is already 0
        movc    a,@a+dptr
        mov     _ap,a
        clr     a
        inc     dptr
        movc    a,@a+dptr
        sjmp    00006$
;
;   pointer to xternal stack
;
 00004$:
        push    ar0
        mov     r0,dpl
        movx    a,@r0
        mov     _ap,a
        inc     r0
        movx    a,@r0
        inc     dpl
;
;   restore and return
;
00005$:
    pop ar0
00006$:
     _endasm ;

}
#endif
