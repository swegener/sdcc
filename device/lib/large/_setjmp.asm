;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:32 2000

;--------------------------------------------------------
	.module _setjmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _setjmp
	.globl _longjmp
	.globl _longjmp_PARM_2
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
G$P0$0$0 == 0x0080
_P0	=	0x0080
G$P1$0$0 == 0x0090
_P1	=	0x0090
G$P2$0$0 == 0x00a0
_P2	=	0x00a0
G$P3$0$0 == 0x00b0
_P3	=	0x00b0
G$PSW$0$0 == 0x00d0
_PSW	=	0x00d0
G$ACC$0$0 == 0x00e0
_ACC	=	0x00e0
G$B$0$0 == 0x00f0
_B	=	0x00f0
G$SP$0$0 == 0x0081
_SP	=	0x0081
G$DPL$0$0 == 0x0082
_DPL	=	0x0082
G$DPH$0$0 == 0x0083
_DPH	=	0x0083
G$PCON$0$0 == 0x0087
_PCON	=	0x0087
G$TCON$0$0 == 0x0088
_TCON	=	0x0088
G$TMOD$0$0 == 0x0089
_TMOD	=	0x0089
G$TL0$0$0 == 0x008a
_TL0	=	0x008a
G$TL1$0$0 == 0x008b
_TL1	=	0x008b
G$TH0$0$0 == 0x008c
_TH0	=	0x008c
G$TH1$0$0 == 0x008d
_TH1	=	0x008d
G$AUXR$0$0 == 0x008e
_AUXR	=	0x008e
G$IE$0$0 == 0x00a8
_IE	=	0x00a8
G$IP$0$0 == 0x00b8
_IP	=	0x00b8
G$SCON$0$0 == 0x0098
_SCON	=	0x0098
G$SBUF$0$0 == 0x0099
_SBUF	=	0x0099
G$T2CON$0$0 == 0x00c8
_T2CON	=	0x00c8
G$RCAP2H$0$0 == 0x00cb
_RCAP2H	=	0x00cb
G$RCAP2L$0$0 == 0x00ca
_RCAP2L	=	0x00ca
;--------------------------------------------------------
; special function bits 
;--------------------------------------------------------
G$CY$0$0 == 0x00d7
_CY	=	0x00d7
G$AC$0$0 == 0x00d6
_AC	=	0x00d6
G$F0$0$0 == 0x00d5
_F0	=	0x00d5
G$RS1$0$0 == 0x00d4
_RS1	=	0x00d4
G$RS0$0$0 == 0x00d3
_RS0	=	0x00d3
G$OV$0$0 == 0x00d2
_OV	=	0x00d2
G$P$0$0 == 0x00d0
_P	=	0x00d0
G$TF1$0$0 == 0x008f
_TF1	=	0x008f
G$TR1$0$0 == 0x008e
_TR1	=	0x008e
G$TF0$0$0 == 0x008d
_TF0	=	0x008d
G$TR0$0$0 == 0x008c
_TR0	=	0x008c
G$IE1$0$0 == 0x008b
_IE1	=	0x008b
G$IT1$0$0 == 0x008a
_IT1	=	0x008a
G$IE0$0$0 == 0x0089
_IE0	=	0x0089
G$IT0$0$0 == 0x0088
_IT0	=	0x0088
G$EA$0$0 == 0x00af
_EA	=	0x00af
G$ES$0$0 == 0x00ac
_ES	=	0x00ac
G$ET1$0$0 == 0x00ab
_ET1	=	0x00ab
G$EX1$0$0 == 0x00aa
_EX1	=	0x00aa
G$ET0$0$0 == 0x00a9
_ET0	=	0x00a9
G$EX0$0$0 == 0x00a8
_EX0	=	0x00a8
G$PS$0$0 == 0x00bc
_PS	=	0x00bc
G$PT1$0$0 == 0x00bb
_PT1	=	0x00bb
G$PX1$0$0 == 0x00ba
_PX1	=	0x00ba
G$PT0$0$0 == 0x00b9
_PT0	=	0x00b9
G$PX0$0$0 == 0x00b8
_PX0	=	0x00b8
G$RD$0$0 == 0x00b7
_RD	=	0x00b7
G$WR$0$0 == 0x00b6
_WR	=	0x00b6
G$T1$0$0 == 0x00b5
_T1	=	0x00b5
G$T0$0$0 == 0x00b4
_T0	=	0x00b4
G$INT1$0$0 == 0x00b3
_INT1	=	0x00b3
G$INT0$0$0 == 0x00b2
_INT0	=	0x00b2
G$TXD$0$0 == 0x00b1
_TXD	=	0x00b1
G$RXD$0$0 == 0x00b0
_RXD	=	0x00b0
G$P1_0$0$0 == 0x0090
_P1_0	=	0x0090
G$P1_1$0$0 == 0x0091
_P1_1	=	0x0091
G$P1_2$0$0 == 0x0092
_P1_2	=	0x0092
G$P1_3$0$0 == 0x0093
_P1_3	=	0x0093
G$P1_4$0$0 == 0x0094
_P1_4	=	0x0094
G$P1_5$0$0 == 0x0095
_P1_5	=	0x0095
G$P1_6$0$0 == 0x0096
_P1_6	=	0x0096
G$P1_7$0$0 == 0x0097
_P1_7	=	0x0097
G$SM0$0$0 == 0x009f
_SM0	=	0x009f
G$SM1$0$0 == 0x009e
_SM1	=	0x009e
G$SM2$0$0 == 0x009d
_SM2	=	0x009d
G$REN$0$0 == 0x009c
_REN	=	0x009c
G$TB8$0$0 == 0x009b
_TB8	=	0x009b
G$RB8$0$0 == 0x009a
_RB8	=	0x009a
G$TI$0$0 == 0x0099
_TI	=	0x0099
G$RI$0$0 == 0x0098
_RI	=	0x0098
G$T2CON_0$0$0 == 0x00c8
_T2CON_0	=	0x00c8
G$T2CON_1$0$0 == 0x00c9
_T2CON_1	=	0x00c9
G$T2CON_2$0$0 == 0x00ca
_T2CON_2	=	0x00ca
G$T2CON_3$0$0 == 0x00cb
_T2CON_3	=	0x00cb
G$T2CON_4$0$0 == 0x00cc
_T2CON_4	=	0x00cc
G$T2CON_5$0$0 == 0x00cd
_T2CON_5	=	0x00cd
G$T2CON_6$0$0 == 0x00ce
_T2CON_6	=	0x00ce
G$T2CON_7$0$0 == 0x00cf
_T2CON_7	=	0x00cf
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area	DSEG    (DATA)
_setjmp_sloc0_1_0:
	.ds	0x0003
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG	(OVR,DATA)
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area	ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area	BSEG    (BIT)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
_longjmp_PARM_2:
	.ds	0x0002
_longjmp_bp_1_1:
	.ds	0x0003
_setjmp_bp_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$longjmp$0$0 ==.
;	_setjmp.c 28
;	-----------------------------------------
;	 function longjmp
;	-----------------------------------------
_longjmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_setjmp.c 35
	push	b
	push	dph
	push	dpl
	mov	dptr,#_longjmp_bp_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_setjmp.c 31
	mov	dptr,#_longjmp_bp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	a,#0x02
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
	mov	b,r4
	lcall	__gptrget
	mov	r5,a
;	_setjmp.c 32
	mov	ar0,r5
	mov	a,#0x01
	add	a,r2
	mov	r6,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r7,a
	mov	ar1,r4
	mov	dptr,#_longjmp_bp_1_1
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 192   used a instead of ar2 as source
	mov  r2,a
	mov  @r0,a
;	_setjmp.c 33
	dec	r0
	mov	dpl,r6
	mov	dph,r7
	mov	b,r1
	lcall	__gptrget
; Peephole 192   used a instead of ar6 as source
	mov  r6,a
	mov  @r0,a
;	_setjmp.c 34
	mov	_SP,r5
;	_setjmp.c 35
	mov	dptr,#_longjmp_PARM_2
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dpl,r2
	mov	dph,r5
00101$:
	C$_setjmp.c$36$1$1 ==.
	XG$longjmp$0$0 ==.
	ret
	G$setjmp$0$0 ==.
;	_setjmp.c 38
;	-----------------------------------------
;	 function setjmp
;	-----------------------------------------
_setjmp:
;	_setjmp.c 0
	push	b
	push	dph
	push	dpl
	mov	dptr,#_setjmp_bp_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_setjmp.c 43
	mov	dptr,#_setjmp_bp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	a,#0x01
	add	a,r2
	mov	_setjmp_sloc0_1_0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	(_setjmp_sloc0_1_0 + 1),a
	mov	(_setjmp_sloc0_1_0 + 2),r4
	mov	r0,_SP
	mov	ar5,@r0
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__gptrput
;	_setjmp.c 44
	mov	a,#0x01
	add	a,_setjmp_sloc0_1_0
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(_setjmp_sloc0_1_0 + 1)
	mov	r3,a
	mov	r4,(_setjmp_sloc0_1_0 + 2)
	mov	dptr,#_setjmp_bp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	mov	r5,_SP
	mov	a,r5
	add	a,#0xff
	mov	r0,a
	mov	ar5,@r0
	mov	dpl,_setjmp_sloc0_1_0
	mov	dph,(_setjmp_sloc0_1_0 + 1)
	mov	b,(_setjmp_sloc0_1_0 + 2)
	mov	a,r5
	lcall	__gptrput
;	_setjmp.c 45
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,_SP
	lcall	__gptrput
;	_setjmp.c 46
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
00101$:
	C$_setjmp.c$48$1$1 ==.
	XG$setjmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
