; /*-------------------------------------------------------------------------
;
;   crtbank.asm :- C run-time: bank switching
;
;    This library is free software; you can redistribute it and/or modify it
;    under the terms of the GNU Library General Public License as published by the
;    Free Software Foundation; either version 2, or (at your option) any
;    later version.
;
;    This library is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU Library General Public License for more details.
;
;    You should have received a copy of the GNU Library General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;
;    In other words, you are welcome to use, share and improve this program.
;    You are forbidden to forbid anyone else to use, share and improve
;    what you give them.   Help stamp out software-hoarding!
; -------------------------------------------------------------------------*/

	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)

; /*-------------------------------------------------------------------------
;   Example for SiLabs C8051F12x / C8051F13x with 128kB code memory
;   divided into 4 banks of 32kB
;   These devices have an sfr called PSBANK containing two parts:
;   COBANK (bit 5:4): Constant Operations Bank
;   IFBANK (bit 1:0): Instruction Fetch Bank
;   From 0x0000 to 0x7FFF bank 0 is always accessed
;   From 0x8000 to 0xFFFF bank COBANK is accessed for reading with movc
;   From 0x8000 to 0xFFFF bank IFBANK is accessed for executing instructions
;
;   Make your own copy of this file,
;   adapt it to your situation,
;   and compile/link it in your project
; -------------------------------------------------------------------------*/

	.globl _PSBANK

	.area HOME    (CODE)

__sdcc_banked_call::
	push	_PSBANK		;save return bank
	xch	a,r0		;save Acc in r0, do not assume any register bank
	push	acc		;push LSB address
	mov	a,r1
	push	acc		;push MSB address
	mov	a,r2		;get new bank
	anl	a,0x0F		;remove storage class indicator
	anl	_PSBANK,#0xF0
	orl	_PSBANK,a	;select bank
	xch	a,r0		;restore Acc
	ret			;make the call

__sdcc_banked_ret::
	pop	_PSBANK		;restore bank
	ret			;return to caller
