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

__sbit __at (0xF7) B_7;
__sbit __at (0xF6) B_6;
__sbit __at (0xF5) B_5;

/* the  return value is expected to be in acc, and not in the standard
 * location dpl. Therefore we choose return type void here: */
#if defined DSDCC_MODEL_HUGE
void
_gptrget (char *gptr) _naked
{
/* This is the banked version with pointers up to 23 bits.
   B cannot be trashed */

    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
    	jb		_B_7,codeptr$        ; >0x80 code		; 3
    	jnb		_B_6,xdataptr$       ; <0x40 far		; 3

        mov     dph,r0 ; save r0 independant of regbank	; 2
        mov     r0,dpl ; use only low order address		; 2

    	jb		_B_5,pdataptr$       ; >0x60 pdata		; 3
    ;
    ;   Pointer to data space
    ;
        mov     a,@r0									; 1
        mov     r0,dph ; restore r0						; 2
        mov     dph,#0 ; restore dph					; 2
        ret												; 1
    ;
    ;   pointer to external stack or pdata
    ;
 pdataptr$:
        movx    a,@r0									; 1
        mov     r0,dph ; restore r0						; 2
        mov     dph,#0 ; restore dph					; 2
        ret												; 1
	;
	;   pointer to code area
	;
 codeptr$:
 	; implementation for SiLabs C8051F12x
		mov		a,b										; 2
		anl		a,0x03									; 2
		swap	a										; 1
		anl		_PSBANK,#0x0F							; 3
		orl		_PSBANK,a								; 2

        clr     a										; 1
        movc    a,@a+dptr								; 1
        ret												; 1
    ;
    ;   pointer to xternal data
    ;
 xdataptr$:
 	; implementation for xram a16-a21 tied to P3
 		mov		_P3,b									; 3

        movx    a,@dptr									; 1
        ret												; 1
        												;===
        												;43 bytes
     _endasm ;
}

#elif defined DSDCC_MODEL_MEDIUM

void
_gptrget (char *gptr) _naked
{
/* This is the non-banked version with pointers up to 15 bits.
   Assumes B is free to be used */

    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
    	mov		b,dph									; 3
    	jb		_B_7,codeptr$        ; >0x80 code		; 3
    	jnb		_B_6,xdataptr$       ; <0x40 far		; 3

        mov     b,r0   ; save r0 independant of regbank	; 2
        mov     r0,dpl ; use only low order address		; 2

    	jb		_B_5,pdataptr$       ; >0x60 pdata		; 3
    ;
    ;   Pointer to data space
    ;
        mov     a,@r0									; 1
        mov     r0,b   ; restore r0						; 2
        ret												; 1
    ;
    ;   pointer to xternal stack or pdata
    ;
 pdataptr$:
        movx    a,@r0									; 1
        mov     r0,b   ; restore r0						; 2
        ret												; 1
	;
	;   pointer to code area, max 15 bits
	;
 codeptr$:
 	; 0x8000 <= dptr <= 0xFFFF
 	; no need to AND dph and restore from B if hardware wraps code memory
        anl     dph,#0x7F								; 3
        clr     a										; 1
        movc    a,@a+dptr								; 1
        mov     dph,b									; 3
        ret												; 1
    ;
    ;   pointer to xternal data, max 14 bits
    ;
 xdataptr$:
 	; 0 <= dptr <= 0x3FFF
        movx    a,@dptr									; 1
        ret												; 1
        												;===
        												;35 bytes
     _endasm ;
}

#elif 1

void
_gptrget (char *gptr) _naked
{
/* This is the new version with pointers up to 16 bits.
   B cannot be trashed */

    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
    	jb		_B_7,codeptr$        ; >0x80 code		; 3
    	jnb		_B_6,xdataptr$       ; <0x40 far		; 3

        mov     dph,r0 ; save r0 independant of regbank	; 2
        mov     r0,dpl ; use only low order address		; 2

    	jb		_B_5,pdataptr$       ; >0x60 pdata		; 3
    ;
    ;   Pointer to data space
    ;
        mov     a,@r0									; 1
        mov     r0,dph ; restore r0						; 2
        mov     dph,#0 ; restore dph					; 2
        ret												; 1
    ;
    ;   pointer to xternal stack or pdata
    ;
 pdataptr$:
        movx    a,@r0									; 1
        mov     r0,dph ; restore r0						; 2
        mov     dph,#0 ; restore dph					; 2
        ret												; 1
	;
	;   pointer to code area, max 16 bits
	;
 codeptr$:
        clr     a										; 1
        movc    a,@a+dptr								; 1
        ret												; 1
    ;
    ;   pointer to xternal data, max 16 bits
    ;
 xdataptr$:
        movx    a,@dptr									; 1
        ret												; 1
        												;===
        												;30 bytes
     _endasm ;
}

#else

void
_gptrget (char *gptr) _naked
{
/* This is the old version with pointers up to 16 bits. */

    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
        mov     a,b										; 2
        jz      00001$  ; 0 near						; 2
        dec     a										; 1
        jz      00002$  ; 1 far							; 2
        dec     a										; 1
        jz      00003$  ; 2 code						; 2
        dec     a										; 1
        jz      00004$  ; 3 pdata						; 2
        dec     a       ; 4 skip generic pointer		; 1
        dec     a										; 1
        jz      00001$  ; 5 idata						; 2
    ;
    ;   any other value for type
    ;   return xFF
        mov     a,#0xff									; 2
        ret												; 1
    ;
    ;   Pointer to data space
    ;
 00001$:
        push    ar0										; 2
        ;
        mov     r0,dpl     ; use only low order address	; 2
        mov     a,@r0									; 1
        ;
        pop     ar0										; 2
        ;
        ret												; 1
    ;
    ;   pointer to xternal data
    ;
 00002$:
        movx    a,@dptr									; 1
        ret												; 1
;
;   pointer to code area
;
 00003$:
        ; clr     a  is already 0
        movc    a,@a+dptr								; 1
        ret												; 1
;
;   pointer to xternal stack or pdata
;
 00004$:
#if USE_PDATA_PAGING_REGISTER
        mov     dph,__XPAGE     ; __XPAGE (usually p2) holds high byte for pdata access
        movx    a,@dptr
#else
        push    ar0										; 2
        mov     r0,dpl									; 2
        movx    a,@r0									; 1
        pop     ar0										; 2
#endif
        ret												; 1
        												;===
        												;40 bytes
     _endasm ;
}
#endif

#ifdef SDCC_ds390
/* the  return value is expected to be in acc/_ap, and not in the standard
 * location dpl/dph. Therefore we choose return type void here: */

#if 1

void
_gptrgetWord (unsigned *gptr)
{
/* This is the new version */
    gptr; /* hush the compiler */

    _asm
    ;
    ;   depending on the pointer type acc. to SDCCsymt.h
    ;
    	jb		_B_7,00003$           ; >0x80 code
    	jnb		_B_6,00002$           ; <0x40 far

        mov     dph,r0 ; save r0 independant of regbank
        mov     r0,dpl ; use only low order address

    	jb		_B_5,00004$           ; >0x60 pdata
    ;
    ;   Pointer to data space
    ;
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
        clr     a
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
        movx    a,@r0
        mov     _ap,a
        inc     r0
        movx    a,@r0
        inc     dpl
;
;   restore and return
;
 00005$:
        mov     r0,dph ; restore r0
        mov     dph,#0 ; restore dph
 00006$:
    _endasm ;

}

#else

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

#endif
