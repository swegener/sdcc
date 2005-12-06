/*-------------------------------------------------------------------------
  setjmp.c - source file for ANSI routines setjmp & longjmp

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)

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
#include <8051.h>
#include <setjmp.h>

#if defined(SDCC_USE_XSTACK)

static void dummy (void) _naked
{
	_asm

;------------------------------------------------------------
;Allocation info for local variables in function 'setjmp'
;------------------------------------------------------------
;buf                       Allocated to registers dptr b
;------------------------------------------------------------
;../../device/lib/_setjmp.c:180:int setjmp (jmp_buf buf)
;	-----------------------------------------
;	 function setjmp
;	-----------------------------------------
	.globl _setjmp
_setjmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;../../device/lib/_setjmp.c:183:*buf++ = bpx;
;     genPointerSet
;     genGenPointerSet
	mov	a,_bpx
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:184:*buf++ = spx;
;     genPointerSet
;     genGenPointerSet
	mov	a,_spx
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:185:*buf++ = bp;
;     genPointerSet
;     genGenPointerSet
	mov	a,_bp
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:186:*buf++ = SP;
;     genPointerSet
;     genGenPointerSet
	mov	a,sp
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:187:*buf++ = *((unsigned char data *) SP  );
;     genCast
;     genPointerGet
;     genNearPointerGet
;     genPointerSet
;     genGenPointerSet
	mov	r0,sp
	mov	a,@r0
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:188:*buf   = *((unsigned char data *)SP - 1);
;     genCast
;     genMinus
;     genMinusDec
;	peephole 177.g	optimized mov sequence
	dec	r0
;     genPointerGet
;     genNearPointerGet
	mov	a,@r0
;     genPointerSet
;     genGenPointerSet
	lcall	__gptrput
;../../device/lib/_setjmp.c:189:return 0;
;     genRet
	mov	dptr,#0x0000
	ret

;------------------------------------------------------------
;Allocation info for local variables in function 'longjmp'
;------------------------------------------------------------
;rv                        Allocated to stack - offset -2
;buf                       Allocated to registers dptr b
;lsp                       Allocated to registers r5
;------------------------------------------------------------
;../../device/lib/_setjmp.c:192:int longjmp (jmp_buf buf, int rv)
;	-----------------------------------------
;	 function longjmp
;	-----------------------------------------
	.globl _longjmp
_longjmp:
;     genReceive
	mov	r0,_spx
	dec	r0
	movx	a,@r0
	mov	r2,a
	dec	r0
	movx	a,@r0
	mov	r3,a
;../../device/lib/_setjmp.c:193:bpx = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	mov	_bpx,a
	inc	dptr
;../../device/lib/_setjmp.c:194:spx = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	mov	_spx,a
	inc	dptr
;../../device/lib/_setjmp.c:195:bp = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	mov	_bp,a
	inc	dptr
;../../device/lib/_setjmp.c:196:lsp = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	inc	dptr
;     genAssign
	mov	r5,a
;../../device/lib/_setjmp.c:197:*((unsigned char data *) lsp) = *buf++;
;     genCast
	mov	r0,a
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	inc	dptr
;     genPointerSet
;     genNearPointerSet
	mov	@r0,a
;../../device/lib/_setjmp.c:198:*((unsigned char data *) lsp - 1) = *buf;
;     genMinus
;     genMinusDec
	dec	r0
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
;     genPointerSet
;     genNearPointerSet
	mov	@r0,a
;../../device/lib/_setjmp.c:199:SP = lsp;
;     genAssign
	mov	sp,r5
;../../device/lib/_setjmp.c:200:return rv;
;     genAssign
	mov	dph,r2
	mov	dpl,r3
;     genRet
	ret

	_endasm;
}

#elif defined(SDCC_STACK_AUTO)

static void dummy (void) _naked
{
	_asm

;------------------------------------------------------------
;Allocation info for local variables in function 'setjmp'
;------------------------------------------------------------
;buf                       Allocated to registers dptr b
;------------------------------------------------------------
;../../device/lib/_setjmp.c:122:int setjmp (unsigned char *buf)
;	-----------------------------------------
;	 function setjmp
;	-----------------------------------------
	.globl _setjmp
_setjmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;     genReceive
;../../device/lib/_setjmp.c:125:*buf   = BP;
;     genPointerSet
;     genGenPointerSet
	mov	a,_bp
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:126:*buf   = SP;
;     genPointerSet
;     genGenPointerSet
	mov	a,sp
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:127:*buf++ = *((unsigned char data *) SP  );
;     genCast
	mov	r0,sp
;     genPointerGet
;     genNearPointerGet
	mov	a,@r0
;     genPointerSet
;     genGenPointerSet
	lcall	__gptrput
	inc	dptr
;../../device/lib/_setjmp.c:128:*buf++ = *((unsigned char data *)SP - 1);
;     genCast
;     genMinus
;     genMinusDec
	dec	r0
;     genPointerGet
;     genNearPointerGet
	mov	a,@r0
;     genPointerSet
;     genGenPointerSet
	lcall	__gptrput
;../../device/lib/_setjmp.c:129:return 0;
;     genRet
	mov	dptr,#0x0000
	ret

;------------------------------------------------------------
;Allocation info for local variables in function 'longjmp'
;------------------------------------------------------------
;rv                        Allocated to stack - offset -3
;buf                       Allocated to registers dptr b
;lsp                       Allocated to registers r5
;------------------------------------------------------------
;../../device/lib/_setjmp.c:28:int longjmp (jmp_buf buf, int rv)
;	-----------------------------------------
;	 function longjmp
;	-----------------------------------------
	.globl _longjmp
_longjmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;     genReceive
	mov	r0,sp
	dec	r0
	dec	r0
	mov	ar2,@r0
	dec	r0
	mov	ar3,@r0
;../../device/lib/_setjmp.c:30:bp = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	inc	dptr
;     genAssign
	mov	_bp,a
;../../device/lib/_setjmp.c:31:lsp = *buf++;
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	inc	dptr
;     genAssign
	mov	r5,a
;../../device/lib/_setjmp.c:32:*((unsigned char data *) lsp) = *buf++;
;     genCast
	mov	r0,a
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
	inc	dptr
;     genPointerSet
;     genNearPointerSet
	mov	@r0,a
;../../device/lib/_setjmp.c:33:*((unsigned char data *) lsp - 1) = *buf;
;     genCast
;     genMinus
;     genMinusDec
	dec	r0
;     genPointerGet
;     genGenPointerGet
	lcall	__gptrget
;     genPointerSet
;     genNearPointerSet
	mov	@r0,a
;../../device/lib/_setjmp.c:34:SP = lsp;
;     genAssign
	mov	sp,r5
;../../device/lib/_setjmp.c:35:return rv;
;     genRet
	mov	dph,r2
	mov	dpl,r3
	ret

	_endasm;
}

#else

//extern unsigned char data bp;

int setjmp (jmp_buf buf)
{
    /* registers would have been saved on the
       stack anyway so we need to save SP
       and the return address */
//    *buf++ = bp;
    *buf++ = SP;
    *buf++ = *((unsigned char data *) SP  );
    *buf   = *((unsigned char data *)SP - 1);
    return 0;
}

int longjmp (jmp_buf buf, int rv)
{
    unsigned char lsp;
//    bp = *buf++;
    lsp = *buf++;
    *((unsigned char data *) lsp) = *buf++;
    *((unsigned char data *) lsp - 1) = *buf;
    SP = lsp;
    return rv;
}

#endif
