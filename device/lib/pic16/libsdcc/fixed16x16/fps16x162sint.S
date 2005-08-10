
;
; fps16x162sint.S - convert fixed16x16 to signed char
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
	global ___fps16x162sint

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
PLUSW2	equ	0xfdb
PLUSW1	equ	0xfe3
PRODL	equ	0xff3
PRODH	equ	0xff4


S_fps16x162sint____fps16x162sint	code
___fps16x162sint:

	movlw	0x04
	movff	PLUSW1, PRODL

	movlw	0x03
	movf	PLUSW1, w

	btfss	PRODL, 7
	bra	@1

	bcf	PRODL, 7

	comf	WREG, f
	comf	PRODL, f
	infsnz	WREG, f
	incf	PRODL, f

@1:
	return

	end
