/*-------------------------------------------------------------------------

  _gptrput.c :- put value for a generic pointer

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

void
_gptrput (char *gptr, char c) _naked
{
    gptr; c; /* hush the compiler */

    _asm
	ar0 = 0x00
        push     acc
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

 00003$:
        pop     acc    ; do nothing
        sjmp    00005$
;
;       store into near space
;
 00001$:
        pop     acc
        push    ar0
        mov     r0,dpl
        mov     @r0,a
        pop     ar0
        sjmp    00005$

 00002$:
        pop     acc
        movx    @dptr,a
        sjmp    00005$

 00004$:
#if USE_PDATA_PAGING_REGISTER
        pop     acc
        mov     dph,__XPAGE     ; __XPAGE (usually p2) holds high byte for pdata access
        movx    @dptr,a
#else
        pop     acc
        push    ar0
        mov     r0,dpl
        movx    @r0,a
        pop     ar0
#endif

 00005$:
        ret
_endasm;
}

#ifdef SDCC_ds390
void
_gptrputWord ()
{
    _asm
        push     acc
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
        mov     a,b
        jz      00011$  ; 0 near
        dec     a
        jz      00012$  ; 1 far
        dec     a
        jz      00013$  ; 2 code
        dec     a
        jz      00014$  ; 3 pdata
        dec     a       ; 4 skip generic pointer
        dec     a
        jz      00011$  ; 5 idata
        pop     acc
        sjmp    00016$
;
;       store into near space
;
 00011$:
        pop     acc
        push    ar0
        mov     r0,dpl
        mov     @r0,_ap
        inc     r0
        mov     @r0,a
        sjmp    00015$

 00012$:
        mov     a, _ap
        movx    @dptr,a
        inc     dptr
        pop     acc
        movx    @dptr,a
        sjmp    00016$

 00013$:
        pop     acc    ; do nothing
        sjmp    00016$

 00014$:
        pop     acc
        push    ar0
        mov     r0,dpl
        xch     a,_ap
        movx    @r0,a
        inc     r0
        xch     a,_ap
        movx    @r0, a
 00015$:
        inc     dptr
        pop     ar0
 00016$:
    _endasm;
}

#endif
