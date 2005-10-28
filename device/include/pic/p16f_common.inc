; -----------------------------------------------
; p16f_common.inc - definitions common to all 14 bit PIC devices
;
; Copyright (C) 2005 by Raphael Neider
;
; This file is part of the SDCC/pic14 library.
; This file is licensed under the terms of the LGPL, see LGPL
; for details.
;
; $Id$

		extern	STK00
		extern	STK01
		extern	STK02
		extern	STK03
		extern	STK04
		extern	STK05
		extern	STK06
		extern	STK07
		extern	STK08
		extern	STK09
		extern	STK10
		extern	STK11
		extern	STK12
		extern	STK13
		extern	STK14

; -----------------------------------------------
; --- PIC14 special function registers
; -----------------------------------------------

INDF		EQU	0x0000
TMR0		EQU	0x0001
PCL		EQU	0x0002
STATUS		EQU	0x0003
FSR		EQU	0x0004
PORTA		EQU	0x0005
PORTB		EQU	0x0006
; unimplemented EQU	0x0007
EEDATA		EQU	0x0008
EEADR		EQU	0x0009
PCLATH		EQU	0x000a
INTCON		EQU	0x000b

; INDF		EQU 	0x0080
OPTION_REG	EQU	0x0081
; PCL		EQU	0x0082
; STATUS	EQU	0x0083
; FSR		EQU	0x0084
TRISA		EQU	0x0085
TRISB		EQU	0x0086
; unimplemented EQU	0x0087
EECON1		EQU	0x0088
EECON2		EQU	0x0089
; PCLATH	EQU	0x008a
; INTCON	EQU	0x008b

; -----------------------------------------------
; --- useful symbolic constants
; -----------------------------------------------

C		EQU	0
DC		EQU	1
Z		EQU	2
nPD		EQU	3
nTO		EQU	4
RP0		EQU	5
RP1		EQU	6	; possibly unimplemented
IRP		EQU	7	; possibly unimplemented

