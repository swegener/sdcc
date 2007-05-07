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

__sbit __at (0xF7) B_7;
__sbit __at (0xF6) B_6;
__sbit __at (0xF5) B_5;

#if defined DSDCC_MODEL_HUGE
void
_gptrput (char *gptr, char c) __naked
{
/* This is the banked version with pointers up to 22 bits.
   B cannot be trashed */

    gptr; c; /* hush the compiler */

    __asm
    ;
    ;   depending on the pointer type according to SDCCsymt.h
    ;
        jb      _B_7,codeptr$        ; >0x80 code       ; 3
        jnb     _B_6,xdataptr$       ; <0x40 far        ; 3

        mov     dph,r0 ; save r0 independant of regbank ; 2
        mov     r0,dpl ; use only low order address     ; 2

        jb      _B_5,pdataptr$       ; >0x60 pdata      ; 3
    ;
    ;   store into near/idata space
    ;
        mov     @r0,a                                   ; 1
 dataptrrestore$:
        mov     r0,dph ; restore r0                     ; 2
        mov     dph,#0 ; restore dph                    ; 2

 codeptr$:
        ret                                             ; 1
    ;
    ;   store into external stack/pdata space
    ;
 pdataptr$:
        movx    @r0,a                                   ; 1
        sjmp    dataptrrestore$                         ; 2
    ;
    ;   store into far space
    ;
 xdataptr$:
        mov     _P3,b                                   ; 3

        movx    @dptr,a                                 ; 1
        ret                                             ; 1

                                                        ;===
                                                        ;27 bytes
    __endasm;
}

#elif defined DSDCC_MODEL_MEDIUM

void
_gptrput (char *gptr, char c) __naked
{
/* This is the non-banked version with pointers up to 14 bits.
   Assumes B is free to be used */

    gptr; c; /* hush the compiler */

    __asm
    ;
    ;   depending on the pointer type according to SDCCsymt.h
    ;
        mov     b,dph                                   ; 3
        jb      _B_7,codeptr$        ; >0x80 code       ; 3
        jnb     _B_6,xdataptr$       ; <0x40 far        ; 3

        mov     b,r0   ; save r0 independant of regbank ; 2
        mov     r0,dpl ; use only low order address     ; 2

        jb      _B_5,pdataptr$       ; >0x60 pdata      ; 3
    ;
    ;   store into near/idata space
    ;
        mov     @r0,a                                   ; 1
 dataptrrestore$:
        mov     r0,b   ; restore r0                     ; 2

 codeptr$:
        ret                                             ; 1
    ;
    ;   store into external stack/pdata space
    ;
 pdataptr$:
        movx    @r0,a                                   ; 1
        sjmp    dataptrrestore$                         ; 2
    ;
    ;   store into far space, max 14 bits
    ;
 xdataptr$:
    ; 0 <= dptr <= 0x3FFF
        movx    @dptr,a                                 ; 1
        ret                                             ; 1
                                                        ;===
                                                        ;25 bytes
    __endasm;
}

#else

void
_gptrput (char *gptr, char c) __naked
{
/* This is the new version with pointers up to 16 bits.
   B cannot be trashed */

    gptr; c; /* hush the compiler */

    __asm
    ;
    ;   depending on the pointer type according to SDCCsymt.h
    ;
        jb      _B_7,codeptr$        ; >0x80 code       ; 3
        jnb     _B_6,xdataptr$       ; <0x40 far        ; 3

        mov     dph,r0 ; save r0 independant of regbank ; 2
        mov     r0,dpl ; use only low order address     ; 2

        jb      _B_5,pdataptr$       ; >0x60 pdata      ; 3
    ;
    ;   store into near/idata space
    ;
        mov     @r0,a                                   ; 1
 dataptrrestore$:
        mov     r0,dph ; restore r0                     ; 2
        mov     dph,#0 ; restore dph                    ; 2

 codeptr$:
        ret                                             ; 1
    ;
    ;   store into external stack/pdata space
    ;
 pdataptr$:
        movx    @r0,a                                   ; 1
        sjmp    dataptrrestore$                         ; 2
    ;
    ;   store into far space
    ;
 xdataptr$:
        movx    @dptr,a                                 ; 1
        ret                                             ; 1

                                                        ;===
                                                        ;24 bytes
    __endasm;
}

#endif

#ifdef SDCC_ds390

void
_gptrputWord ()
{
    __asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
        jb      _B_7,00013$           ; >0x80 code
        jnb     _B_6,00012$           ; <0x40 far

        mov     dph,r0 ; save r0 independant of regbank
        mov     r0,dpl ; use only low order address

        jb      _B_5,00014$           ; >0x60 pdata
;
;       store into near space
;
        mov     @r0,_ap
        inc     r0
        mov     @r0,a
        sjmp    00015$
;
;       store into far space
;
 00012$:
        xch     a,_ap
        movx    @dptr,a
        inc     dptr
        xch     a,_ap
        movx    @dptr,a
        sjmp    00016$
;
;       store into code space
;
 00013$:
        inc     dptr   ; do nothing
        sjmp    00016$
;
;       store into xstack space
;
 00014$:
        xch     a,_ap
        movx    @r0,a
        inc     r0
        xch     a,_ap
        movx    @r0, a
 00015$:
        mov     dpl,r0
        mov     r0,dph ; restore r0
        mov     dph,#0 ; restore dph
 00016$:
    __endasm;
}

#endif
