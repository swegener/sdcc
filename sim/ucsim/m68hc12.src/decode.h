#include "gen.h"

DEC(0, 89,	adc	, _A_i8  	, "ADCA %b"	, ' ', 2	, 1)
DEC(0, 99,	adc	, _A_dop	, "ADCA %d"	, ' ', 2	, 3)
DEC(0, a9,	adc	, _A_xbop8	, "ADCA %p"	, ' ', -2	, 0x66433)
DEC(0, b9,	adc	, _A_eop 	, "ADCA %e"	, ' ', 3	, 3)

DEC(0, c9,	adc	, _B_i8  	, "ADCB %b"	, ' ', 2	, 1)
DEC(0, d9,	adc	, _B_dop	, "ADCB %d"	, ' ', 2	, 3)
DEC(0, e9,	adc	, _B_xbop8	, "ADCB %p"	, ' ', -2	, 0x66433)
DEC(0, f9,	adc	, _B_eop 	, "ADCB %e"	, ' ', 3	, 3)

DEC(0, 8b,	add	, _A_i8  	, "ADDA %b"	, ' ', 2	, 1)
DEC(0, 9b,	add	, _A_dop	, "ADDA %d"	, ' ', 2	, 3)
DEC(0, ab,	add	, _A_xbop8	, "ADDA %p"	, ' ', -2	, 0x66433)
DEC(0, bb,	add	, _A_eop 	, "ADDA %e"	, ' ', 3	, 3)

DEC(0, cb,	add	, _B_i8  	, "ADDB %b"	, ' ', 2	, 1)
DEC(0, db,	add	, _B_dop	, "ADDB %d"	, ' ', 2	, 3)
DEC(0, eb,	add	, _B_xbop8	, "ADDB %p"	, ' ', -2	, 0x66433)
DEC(0, fb,	add	, _B_eop 	, "ADDB %e"	, ' ', 3	, 3)

DEC(0, 84,	And	, _A_i8  	, "ANDA %b"	, ' ', 2	, 1)
DEC(0, 94,	And	, _A_dop	, "ANDA %d"	, ' ', 2	, 3)
DEC(0, a4,	And	, _A_xbop8	, "ANDA %p"	, ' ', -2	, 0x66433)
DEC(0, b4,	And	, _A_eop 	, "ANDA %e"	, ' ', 3	, 3)

DEC(0, c4,	And	, _B_i8  	, "ANDB %b"	, ' ', 2	, 1)
DEC(0, d4,	And	, _B_dop	, "ANDB %d"	, ' ', 2	, 3)
DEC(0, e4,	And	, _B_xbop8	, "ANDB %p"	, ' ', -2	, 0x66433)
DEC(0, f4,	And	, _B_eop 	, "ANDB %e"	, ' ', 3	, 3)

DEC(0, 88,	eor	, _A_i8  	, "EORA %b"	, ' ', 2	, 1)
DEC(0, 98,	eor	, _A_dop	, "EORA %d"	, ' ', 2	, 3)
DEC(0, a8,	eor	, _A_xbop8	, "EORA %p"	, ' ', -2	, 0x66433)
DEC(0, b8,	eor	, _A_eop 	, "EORA %e"	, ' ', 3	, 3)

DEC(0, c8,	eor	, _B_i8  	, "EORB %b"	, ' ', 2	, 1)
DEC(0, d8,	eor	, _B_dop	, "EORB %d"	, ' ', 2	, 3)
DEC(0, e8,	eor	, _B_xbop8	, "EORB %p"	, ' ', -2	, 0x66433)
DEC(0, f8,	eor 	, _B_eop 	, "EORB %e"	, ' ', 3	, 3)

DEC(0, 8a,	Or	, _A_i8  	, "ORAA %b"	, ' ', 2	, 1)
DEC(0, 9a,	Or	, _A_dop	, "ORAA %d"	, ' ', 2	, 3)
DEC(0, aa,	Or	, _A_xbop8	, "ORAA %p"	, ' ', -2	, 0x66433)
DEC(0, ba,	Or	, _A_eop 	, "ORAA %e"	, ' ', 3	, 3)

DEC(0, ca,	Or	, _B_i8  	, "ORAB %b"	, ' ', 2	, 1)
DEC(0, da,	Or	, _B_dop	, "ORAB %d"	, ' ', 2	, 3)
DEC(0, ea,	Or	, _B_xbop8	, "ORAB %p"	, ' ', -2	, 0x66433)
DEC(0, fa,	Or 	, _B_eop 	, "ORAB %e"	, ' ', 3	, 3)

DEC(0, 85,	bit	, _Aop_i8  	, "BITA %b"	, ' ', 2	, 1)
DEC(0, 95,	bit	, _Aop_dop	, "BITA %d"	, ' ', 2	, 3)
DEC(0, a5,	bit	, _Aop_xbop8	, "BITA %p"	, ' ', -2	, 0x66433)
DEC(0, b5,	bit	, _Aop_eop 	, "BITA %e"	, ' ', 3	, 3)

DEC(0, c5,	bit	, _Bop_i8  	, "BITB %b"	, ' ', 2	, 1)
DEC(0, d5,	bit	, _Bop_dop	, "BITB %d"	, ' ', 2	, 3)
DEC(0, e5,	bit	, _Bop_xbop8	, "BITB %p"	, ' ', -2	, 0x66433)
DEC(0, f5,	bit 	, _Bop_eop 	, "BITB %e"	, ' ', 3	, 3)

DEC(0, 81,	cmp	, _Aop_i8  	, "CMPA %b"	, ' ', 2	, 1)
DEC(0, 91,	cmp	, _Aop_dop	, "CMPA %d"	, ' ', 2	, 3)
DEC(0, a1,	cmp	, _Aop_xbop8	, "CMPA %p"	, ' ', -2	, 0x66433)
DEC(0, b1,	cmp	, _Aop_eop 	, "CMPA %e"	, ' ', 3	, 3)

DEC(0, c1,	cmp	, _Bop_i8  	, "CMPB %b"	, ' ', 2	, 1)
DEC(0, d1,	cmp	, _Bop_dop	, "CMPB %d"	, ' ', 2	, 3)
DEC(0, e1,	cmp	, _Bop_xbop8	, "CMPB %p"	, ' ', -2	, 0x66433)
DEC(0, f1,	cmp 	, _Bop_eop 	, "CMPB %e"	, ' ', 3	, 3)

DEC(0, 80,	sub	, _A_i8  	, "SUBA %b"	, ' ', 2	, 1)
DEC(0, 90,	sub	, _A_dop	, "SUBA %d"	, ' ', 2	, 3)
DEC(0, a0,	sub	, _A_xbop8	, "SUBA %p"	, ' ', -2	, 0x66433)
DEC(0, b0,	sub	, _A_eop 	, "SUBA %e"	, ' ', 3	, 3)

DEC(0, c0,	sub	, _B_i8  	, "SUBB %b"	, ' ', 2	, 1)
DEC(0, d0,	sub	, _B_dop	, "SUBB %d"	, ' ', 2	, 3)
DEC(0, e0,	sub	, _B_xbop8	, "SUBB %p"	, ' ', -2	, 0x66433)
DEC(0, f0,	sub	, _B_eop 	, "SUBB %e"	, ' ', 3	, 3)

DEC(0, 82,	sbc	, _A_i8  	, "SBCA %b"	, ' ', 2	, 1)
DEC(0, 92,	sbc	, _A_dop	, "SBCA %d"	, ' ', 2	, 3)
DEC(0, a2,	sbc	, _A_xbop8	, "SBCA %p"	, ' ', -2	, 0x66433)
DEC(0, b2,	sbc	, _A_eop 	, "SBCA %e"	, ' ', 3	, 3)

DEC(0, c2,	sbc	, _B_i8  	, "SBCB %b"	, ' ', 2	, 1)
DEC(0, d2,	sbc	, _B_dop	, "SBCB %d"	, ' ', 2	, 3)
DEC(0, e2,	sbc	, _B_xbop8	, "SBCB %p"	, ' ', -2	, 0x66433)
DEC(0, f2,	sbc	, _B_eop 	, "SBCB %e"	, ' ', 3	, 3)

DEC(0, 86,	lda	, _A_i8  	, "LDAA %b"	, ' ', 2	, 1)
DEC(0, 96,	lda	, _A_dop	, "LDAA %d"	, ' ', 2	, 3)
DEC(0, a6,	lda	, _A_xbop8	, "LDAA %p"	, ' ', -2	, 0x66433)
DEC(0, b6,	lda	, _A_eop 	, "LDAA %e"	, ' ', 3	, 3)

DEC(0, c6,	lda	, _B_i8  	, "LDAB %b"	, ' ', 2	, 1)
DEC(0, d6,	lda	, _B_dop	, "LDAB %d"	, ' ', 2	, 3)
DEC(0, e6,	lda	, _B_xbop8	, "LDAB %p"	, ' ', -2	, 0x66433)
DEC(0, f6,	lda	, _B_eop 	, "LDAB %e"	, ' ', 3	, 3)

DEC(0, 83,	sub16	, _D_i16  	, "SUBD %B"	, ' ', 3	, 2)
DEC(0, 93,	sub16	, _D_dop16	, "SUBD %D"	, ' ', 2	, 3)
DEC(0, a3,	sub16	, _D_xbop16	, "SUBD %p"	, ' ', -2	, 0x66433)
DEC(0, b3,	sub16	, _D_eop16 	, "SUBD %E"	, ' ', 3	, 3)

DEC(0, 8c,	cp16	, _Dop_i16  	, "CPD %B"	, ' ', 3	, 2)
DEC(0, 9c,	cp16	, _Dop_dop16	, "CPD %D"	, ' ', 2	, 3)
DEC(0, ac,	cp16	, _Dop_xbop16	, "CPD %p"	, ' ', -2	, 0x66433)
DEC(0, bc,	cp16	, _Dop_eop16 	, "CPD %E"	, ' ', 3	, 3)

DEC(0, 8d,	cp16	, _Yop_i16  	, "CPY %B"	, ' ', 3	, 2)
DEC(0, 9d,	cp16	, _Yop_dop16	, "CPY %D"	, ' ', 2	, 3)
DEC(0, ad,	cp16	, _Yop_xbop16	, "CPY %p"	, ' ', -2	, 0x66433)
DEC(0, bd,	cp16	, _Yop_eop16 	, "CPY %E"	, ' ', 3	, 3)

DEC(0, 8e,	cp16	, _Xop_i16  	, "CPX %B"	, ' ', 3	, 2)
DEC(0, 9e,	cp16	, _Xop_dop16	, "CPX %D"	, ' ', 2	, 3)
DEC(0, ae,	cp16	, _Xop_xbop16	, "CPX %p"	, ' ', -2	, 0x66433)
DEC(0, be,	cp16	, _Xop_eop16 	, "CPX %E"	, ' ', 3	, 3)

DEC(0, 8f,	cp16	, _Sop_i16  	, "CPS %B"	, ' ', 3	, 2)
DEC(0, 9f,	cp16	, _Sop_dop16	, "CPS %D"	, ' ', 2	, 3)
DEC(0, af,	cp16	, _Sop_xbop16	, "CPS %p"	, ' ', -2	, 0x66433)
DEC(0, bf,	cp16	, _Sop_eop16 	, "CPS %E"	, ' ', 3	, 3)

DEC(0, c3,	add16	, _D_i16  	, "ADDD %B"	, ' ', 3	, 2)
DEC(0, d3,	add16	, _D_dop16	, "ADDD %D"	, ' ', 2	, 3)
DEC(0, e3,	add16	, _D_xbop16	, "ADDD %p"	, ' ', -2	, 0x66433)
DEC(0, f3,	add16	, _D_eop16 	, "ADDD %E"	, ' ', 3	, 3)

DEC(0, cc,	ld16	, _D_i16  	, "LDD %B"	, ' ', 3	, 2)
DEC(0, dc,	ld16	, _D_dop16	, "LDD %D"	, ' ', 2	, 3)
DEC(0, ec,	ld16	, _D_xbop16	, "LDD %p"	, ' ', -2	, 0x66433)
DEC(0, fc,	ld16	, _D_eop16 	, "LDD %E"	, ' ', 3	, 3)

DEC(0, ce,	ld16	, _X_i16  	, "LDX %B"	, ' ', 3	, 2)
DEC(0, de,	ld16	, _X_dop16	, "LDX %D"	, ' ', 2	, 3)
DEC(0, ee,	ld16	, _X_xbop16	, "LDX %p"	, ' ', -2	, 0x66433)
DEC(0, fe,	ld16	, _X_eop16 	, "LDX %E"	, ' ', 3	, 3)

DEC(0, cd,	ld16	, _Y_i16  	, "LDY %B"	, ' ', 3	, 2)
DEC(0, dd,	ld16	, _Y_dop16	, "LDY %D"	, ' ', 2	, 3)
DEC(0, ed,	ld16	, _Y_xbop16	, "LDY %p"	, ' ', -2	, 0x66433)
DEC(0, fd,	ld16	, _Y_eop16 	, "LDY %E"	, ' ', 3	, 3)

DEC(0, cf,	ld16	, _S_i16  	, "LDS %B"	, ' ', 3	, 2)
DEC(0, df,	ld16	, _S_dop16	, "LDS %D"	, ' ', 2	, 3)
DEC(0, ef,	ld16	, _S_xbop16	, "LDS %p"	, ' ', -2	, 0x66433)
DEC(0, ff,	ld16	, _S_eop16 	, "LDS %E"	, ' ', 3	, 3)

DEC(0, 87,	clr	, _A	 	, "CLRA"	, ' ', 1	, 1)
DEC(0, c7,	clr	, _B	 	, "CLRB"	, ' ', 1	, 1)
DEC(0, 69,	clr	, _xb	 	, "CLR %p"	, ' ', -2	, 0x55322)
DEC(0, 79,	clr	, _e	 	, "CLR %e"	, ' ', 3	, 3)

DEC(0, 97,	tst	, _Aop	 	, "TSTA"	, ' ', 1	, 1)
DEC(0, d7,	tst	, _Bop	 	, "TSTB"	, ' ', 1	, 1)
DEC(0, e7,	tst	, _xbop8 	, "TST %p"	, ' ', -2	, 0x55322)
DEC(0, f7,	tst	, _eop	 	, "TST %e"	, ' ', 3	, 3)

DEC(0, 40,	neg	, _A	 	, "NEGA"	, ' ', 1	, 1)
DEC(0, 50,	neg	, _B	 	, "NEGB"	, ' ', 1	, 1)
DEC(0, 60,	neg	, _xb	 	, "NEG %p"	, ' ', -2	, 0x66433)
DEC(0, 70,	neg	, _e	 	, "NEG %e"	, ' ', 3	, 3)

DEC(0, 41,	com	, _A	 	, "COMA"	, ' ', 1	, 1)
DEC(0, 51,	com	, _B	 	, "COMB"	, ' ', 1	, 1)
DEC(0, 61,	com	, _xb	 	, "COM %p"	, ' ', -2	, 0x66433)
DEC(0, 71,	com	, _e	 	, "COM %e"	, ' ', 3	, 3)

DEC(0, 42,	inc	, _A	 	, "INCA"	, ' ', 1	, 1)
DEC(0, 52,	inc	, _B	 	, "INCB"	, ' ', 1	, 1)
DEC(0, 62,	inc	, _xb	 	, "INC %p"	, ' ', -2	, 0x66433)
DEC(0, 72,	inc	, _e	 	, "INC %e"	, ' ', 3	, 3)

DEC(0, 43,	dec	, _A	 	, "DECA"	, ' ', 1	, 1)
DEC(0, 53,	dec	, _B	 	, "DECB"	, ' ', 1	, 1)
DEC(0, 63,	dec	, _xb	 	, "DEC %p"	, ' ', -2	, 0x66433)
DEC(0, 73,	dec	, _e	 	, "DEC %e"	, ' ', 3	, 3)

DEC(0, 44,	lsr	, _A	 	, "LSRA"	, ' ', 1	, 1)
DEC(0, 54,	lsr	, _B	 	, "LSRB"	, ' ', 1	, 1)
DEC(0, 64,	lsr	, _xb	 	, "LSR %p"	, ' ', -2	, 0x66433)
DEC(0, 74,	lsr	, _e	 	, "LSR %e"	, ' ', 3	, 3)

DEC(0, 45,	rol	, _A	 	, "ROLA"	, ' ', 1	, 1)
DEC(0, 55,	rol	, _B	 	, "ROLB"	, ' ', 1	, 1)
DEC(0, 65,	rol	, _xb	 	, "ROL %p"	, ' ', -2	, 0x66433)
DEC(0, 75,	rol	, _e	 	, "ROL %e"	, ' ', 3	, 3)

DEC(0, 46,	ror	, _A	 	, "RORA"	, ' ', 1	, 1)
DEC(0, 56,	ror	, _B	 	, "RORB"	, ' ', 1	, 1)
DEC(0, 66,	ror	, _xb	 	, "ROR %p"	, ' ', -2	, 0x66433)
DEC(0, 76,	ror	, _e	 	, "ROR %e"	, ' ', 3	, 3)

DEC(0, 47,	asr	, _A	 	, "ASRA"	, ' ', 1	, 1)
DEC(0, 57,	asr	, _B	 	, "ASRB"	, ' ', 1	, 1)
DEC(0, 67,	asr	, _xb	 	, "ASR %p"	, ' ', -2	, 0x66433)
DEC(0, 77,	asr	, _e	 	, "ASR %e"	, ' ', 3	, 3)

DEC(0, 48,	asl	, _A	 	, "ASLA"	, ' ', 1	, 1)
DEC(0, 58,	asl	, _B	 	, "ASLB"	, ' ', 1	, 1)
DEC(0, 68,	asl	, _xb	 	, "ASL %p"	, ' ', -2	, 0x66433)
DEC(0, 78,	asl	, _e	 	, "ASL %e"	, ' ', 3	, 3)

DEC(0, 49,	lsr16	, _D		, "LSRD"	, ' ', 1	, 1)
DEC(0, 59,	asl16	, _D		, "LSLD"	, ' ', 1	, 1)

DEC(0, 4a,	call_e	, _NONE		, "CALL $ep$"	, ' ', 4	, 7)
DEC(0, 4b,	call_id	, _NONE		, "CALL $ip$"	, ' ', -2	, 0xaa988)

DEC(0, 5a,	sta	, _d_Aop 	, "STAA %d"	, ' ', 2	, 2)
DEC(0, 6a,	sta	, _xb_Aop	, "STAA %p"	, ' ', -2	, 0x55332)
DEC(0, 7a,	sta	, _e_Aop 	, "STAA %e"	, ' ', 3	, 3)

DEC(0, 5b,	sta	, _d_Bop 	, "STAB %d"	, ' ', 2	, 2)
DEC(0, 6b,	sta	, _xb_Bop	, "STAB %p"	, ' ', -2	, 0x55332)
DEC(0, 7b,	sta	, _e_Bop 	, "STAB %e"	, ' ', 3	, 3)

DEC(0, 5c,	stsx	, _da_Dop 	, "STD %d"	, ' ', 2	, 2)
DEC(0, 6c,	stsx	, _xba_Dop	, "STD %p"	, ' ', -2	, 0x55332)
DEC(0, 7c,	stsx	, _ea_Dop 	, "STD %e"	, ' ', 3	, 3)

DEC(0, 5e,	stsx	, _da_Xop 	, "STX %d"	, ' ', 2	, 2)
DEC(0, 6e,	stsx	, _xba_Xop	, "STX %p"	, ' ', -2	, 0x55332)
DEC(0, 7e,	stsx	, _ea_Xop 	, "STX %e"	, ' ', 3	, 3)

DEC(0, 5d,	stsx	, _da_Yop 	, "STY %d"	, ' ', 2	, 2)
DEC(0, 6d,	stsx	, _xba_Yop	, "STY %p"	, ' ', -2	, 0x55332)
DEC(0, 7d,	stsx	, _ea_Yop 	, "STY %e"	, ' ', 3	, 3)

DEC(0, 5f,	stsx	, _da_Sop 	, "STS %d"	, ' ', 2	, 2)
DEC(0, 6f,	stsx	, _xba_Sop	, "STS %p"	, ' ', -2	, 0x55332)
DEC(0, 7f,	stsx	, _ea_Sop 	, "STS %e"	, ' ', 3	, 3)

DEC(0, 4c,	Or	, _d_i8		, "BSET %d,%b"	, ' ', 3	, 4)
DEC(0, 0c,	Or	, _xb_i8	, "BSET %p,%b"	, ' ', -4	, 4)
DEC(0, 1c,	Or	, _e_i8		, "BSET %e,%b"	, ' ', 4	, 4)

DEC(0, 4d,	And	, _d_i8n	, "BCLR %d,%b"	, ' ', 3	, 4)
DEC(0, 0d,	And	, _xb_i8n	, "BCLR %p,%b"	, ' ', -4	, 4)
DEC(0, 1d,	And	, _e_i8n	, "BCLR %e,%b"	, ' ', 4	, 4)

DEC(0, 4e,	brset_d	, _NONE		, "BRSET %d,%b,%r", ' ', 4	, 5)
DEC(0, 0e,	brset_id, _NONE		, "BRSET %p,%b,%r", ' ', -4	, 5)
DEC(0, 1e,	brset_e	, _NONE		, "BRSET %e,%b,%r", ' ', 5	, 5)

DEC(0, 4f,	brclr_d	, _NONE		, "BRCLR %d,%b,%r", ' ', 4	, 5)
DEC(0, 0f,	brclr_id, _NONE		, "BRCLR %p,%b,%r", ' ', -4	, 5)
DEC(0, 1f,	brclr_e	, _NONE		, "BRCLR %e,%b,%r", ' ', 5	, 5)

DEC(0, a7,	NOP	, 0xa7		, "NOP"		, ' ', 1	, 1)

DEC(0, b7,	exec_b7	, _NONE		, "%T"		, ' ', 2	, 0)

DEC(0x18, 06,	add	, _A_B		, "ABA"		, ' ', 2	, 2)
