; -----------------------------------------------
; --- generic pointer access helpers
; -----------------------------------------------

GPTRTAG_DATA	EQU	0x00
GPTRTAG_CODE	EQU	0x80

; setup FSR to point to (STK00:STK01)
setup_fsr	macro
	movf	STK01, W
	movwf	FSR
	; select indirectly addressed bank
	bcf	STATUS, IRP
	btfsc	STK00, 0
	bsf	STATUS, IRP
	endm

; increment FSR, update IRP on bank crossings
; FIXME: update IRP if FSR is reset to 0...
inc_fsr		macro
	incf	FSR, F
	endm

; dispatch according to gptr type
select_routine	macro	dataptr, codeptr
	; __data pointer tag: 0x00
	xorlw	GPTRTAG_DATA
	btfsc	STATUS, Z
	goto	dataptr
	; __code pointer tag: 0x80
	xorlw	(GPTRTAG_DATA ^ GPTRTAG_CODE)
	btfsc	STATUS, Z
	goto	codeptr
	endm

check_data	macro	dataptr
	; __data pointer tag: 0x00
	xorlw	GPTRTAG_DATA
	btfsc	STATUS, Z
	goto	dataptr
	endm

