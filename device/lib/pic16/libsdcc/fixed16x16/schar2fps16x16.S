
;
; schar2fps16x16.S - convert signed char to fixed16x16
;
; written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
;
;
; $Id$
;

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global ___schar2fps16x16

;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
WREG	equ	0xfe8
FSR0L	equ	0xfe9
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW1	equ	0xfe3
PRODL	equ	0xff3
PRODH	equ	0xff4


S_schar2fps16x16____schar2fps16x16	code
___schar2fps16x16:

	movlw	0x01
	movff	PLUSW1, PRODH

	clrf	WREG
	clrf	PRODL
	clrf	FSR0L

	btfss	PRODH, 7
	bra	@1

	comf	PRODH, f
	comf	FSR0L, f

	infsnz	PRODH, f
	incf	FSR0L, f

	bsf	FSR0L, 7

@1:
	return

	end
