
;
; fps16x162uchar.S - convert fixed16x16 to signed char
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
	global ___fps16x162uchar

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


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1


S_fps16x162uchar____fps16x162uchar	code
___fps16x162uchar:

	; normally would read from 0x04 but since we haven't setup
	; a stack frame, then read from 0x04 - 1 (VR)

	movlw	0x03
	movf	PLUSW1, w

	return

	end
