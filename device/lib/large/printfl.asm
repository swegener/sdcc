;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:37 2000

;--------------------------------------------------------
	.module printfl
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _printf_small
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
Fprintfl$ch$0$0==.
_ch:
	.ds	0x0001
Fprintfl$radix$0$0==.
_radix:
	.ds	0x0001
Fprintfl$str$0$0==.
_str:
	.ds	0x0003
Fprintfl$val$0$0==.
_val:
	.ds	0x0004
_pvaloct_sloc0_1_0:
	.ds	0x0004
_printf_small_sloc0_1_0:
	.ds	0x0001
_printf_small_sloc1_1_0:
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
Fprintfl$long_flag$0$0==.
_long_flag:
	.ds	0x0001
Fprintfl$string_flag$0$0==.
_string_flag:
	.ds	0x0001
Fprintfl$short_flag$0$0==.
_short_flag:
	.ds	0x0001
Fprintfl$sign$0$0==.
_sign:
	.ds	0x0001
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
_pvalhex_val_1_1:
	.ds	0x0004
_pvalhex_sp_1_1:
	.ds	0x0001
_pvalhex_lval_1_1:
	.ds	0x0004
_pvaloct_val_1_1:
	.ds	0x0004
_pvaloct_lval_1_1:
	.ds	0x0004
_pvaldec_val_1_1:
	.ds	0x0004
_pvaldec_lval_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
	G$printf_small$0$0 ==.
;	printfl.c 215
	clr	_long_flag
	G$printf_small$0$0 ==.
;	printfl.c 215
	clr	_string_flag
	G$printf_small$0$0 ==.
;	printfl.c 215
	clr	_short_flag
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	Fprintfl$pvalhex$0$0 ==.
;	printfl.c 53
;	-----------------------------------------
;	 function pvalhex
;	-----------------------------------------
_pvalhex:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	printfl.c 0
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#_pvalhex_val_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	printfl.c 55
	mov	dptr,#_pvalhex_sp_1_1
	mov	a,_SP
	movx	@dptr,a
;	printfl.c 56
	mov	dptr,#_pvalhex_val_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_pvalhex_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	printfl.c 58
; Peephole 112   removed ljmp by inverse jump logic
	jb   _long_flag,00102$
00119$:
;	printfl.c 59
	mov	dptr,#_pvalhex_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
00102$:
;	printfl.c 61
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _short_flag,00116$
00120$:
;	printfl.c 62
	mov	dptr,#_pvalhex_lval_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_pvalhex_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	printfl.c 64
00116$:
00105$:
	mov	dptr,#_pvalhex_lval_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	mov	a,r2
	orl	a,r6
	orl	a,r7
	orl	a,r0
	jnz	00121$
	ljmp	00108$
00121$:
;	printfl.c 65
	mov	ar1,r2
	anl	ar1,#0x0f
	mov	a,r1
; Peephole 180   changed mov to clr
; Peephole 186   optimized movc sequence
	mov  dptr,#__str_0
	movc a,@a+dptr
	mov	_ch,a
;	printfl.c 66
	 push _ch 
;	printfl.c 67
	mov	a,r6
	swap	a
	xch	a,r2
	swap	a
	anl	a,#0x0f
	xrl	a,r2
	xch	a,r2
	anl	a,#0x0f
	xch	a,r2
	xrl	a,r2
	xch	a,r2
	mov	r6,a
	mov	a,r7
	swap	a
	anl	a,#0xf0
	orl	a,r6
	mov	r6,a
	mov	a,r0
	swap	a
	xch	a,r7
	swap	a
	anl	a,#0x0f
	xrl	a,r7
	xch	a,r7
	anl	a,#0x0f
	xch	a,r7
	xrl	a,r7
	xch	a,r7
	mov	r0,a
	mov	dptr,#_pvalhex_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	ljmp	00105$
;	printfl.c 70
00108$:
	mov	dptr,#_pvalhex_sp_1_1
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r2,a
	cjne	a,_SP,00122$
; Peephole 132   changed ljmp to sjmp
	sjmp 00111$
00122$:
;	printfl.c 71
	 pop _ch 
;	printfl.c 72
	mov	dpl,_ch
	lcall	_putchar
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00111$:
	C$printfl.c$75$1$1 ==.
	XFprintfl$pvalhex$0$0 ==.
	ret
	Fprintfl$pvaloct$0$0 ==.
;	printfl.c 77
;	-----------------------------------------
;	 function pvaloct
;	-----------------------------------------
_pvaloct:
;	printfl.c 0
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#_pvaloct_val_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	printfl.c 81
;	printfl.c 82
	mov	dptr,#_pvaloct_val_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00102$
00126$:
;	printfl.c 83
	mov	dptr,#_pvaloct_lval_1_1
	clr	c
	clr	a
	subb	a,r3
	movx	@dptr,a
	clr	a
	subb	a,r4
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,r5
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,r6
	inc	dptr
	movx	@dptr,a
;	printfl.c 84
	setb	_sign
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
;	printfl.c 86
	mov	dptr,#_pvaloct_lval_1_1
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
;	printfl.c 87
	clr	_sign
00103$:
;	printfl.c 90
; Peephole 112   removed ljmp by inverse jump logic
	jb   _long_flag,00105$
00127$:
;	printfl.c 91
	mov	dptr,#_pvaloct_lval_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#_pvaloct_lval_1_1
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
00105$:
;	printfl.c 92
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _short_flag,00108$
00128$:
;	printfl.c 93
	mov	dptr,#_pvaloct_lval_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#_pvaloct_lval_1_1
	mov	a,r3
	movx	@dptr,a
	inc	dptr
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	printfl.c 95
00108$:
	mov	dptr,#_pvaloct_lval_1_1
	movx	a,@dptr
	mov	_pvaloct_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_pvaloct_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_pvaloct_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(_pvaloct_sloc0_1_0 + 3),a
	mov	a,_pvaloct_sloc0_1_0
	orl	a,(_pvaloct_sloc0_1_0 + 1)
	orl	a,(_pvaloct_sloc0_1_0 + 2)
	orl	a,(_pvaloct_sloc0_1_0 + 3)
	jnz	00129$
	ljmp	00110$
00129$:
;	printfl.c 96
	mov	dptr,#__modulong_PARM_2
	clr	a
	inc	dptr
	inc	dptr
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	mov	a,#0x08
	movx	@dptr,a
	mov	dpl,_pvaloct_sloc0_1_0
	mov	dph,(_pvaloct_sloc0_1_0 + 1)
	mov	b,(_pvaloct_sloc0_1_0 + 2)
	mov	a,(_pvaloct_sloc0_1_0 + 3)
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	r7,b
	mov	r0,a
	mov	a,#0x30
	add	a,r2
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r3,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r7
	mov	r7,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r0
	mov	r0,a
	mov	_ch,r2
;	printfl.c 97
	 push _ch 
;	printfl.c 98
	mov	r2,_pvaloct_sloc0_1_0
	mov	a,(_pvaloct_sloc0_1_0 + 1)
	swap	a
	rl	a
	xch	a,r2
	swap	a
	rl	a
	anl	a,#0x1f
	xrl	a,r2
	xch	a,r2
	anl	a,#0x1f
	xch	a,r2
	xrl	a,r2
	xch	a,r2
	mov	r3,a
	mov	a,(_pvaloct_sloc0_1_0 + 2)
	swap	a
	rl	a
	anl	a,#0xe0
	orl	a,r3
	mov	r3,a
	mov	r7,(_pvaloct_sloc0_1_0 + 2)
	mov	a,(_pvaloct_sloc0_1_0 + 3)
	swap	a
	rl	a
	xch	a,r7
	swap	a
	rl	a
	anl	a,#0x1f
	xrl	a,r7
	xch	a,r7
	anl	a,#0x1f
	xch	a,r7
	xrl	a,r7
	xch	a,r7
	mov	r0,a
	mov	dptr,#_pvaloct_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	ljmp	00108$
00110$:
;	printfl.c 101
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _sign,00113$
00130$:
;	printfl.c 102
	mov	_ch,#0x2d
;	printfl.c 103
	 push _ch 
;	printfl.c 107
00113$:
	mov	a,_SP
	cjne	a,_SP,00131$
; Peephole 132   changed ljmp to sjmp
	sjmp 00116$
00131$:
;	printfl.c 108
	 pop _ch 
;	printfl.c 109
	mov	dpl,_ch
	lcall	_putchar
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00116$:
	C$printfl.c$111$1$1 ==.
	XFprintfl$pvaloct$0$0 ==.
	ret
	Fprintfl$pvaldec$0$0 ==.
;	printfl.c 113
;	-----------------------------------------
;	 function pvaldec
;	-----------------------------------------
_pvaldec:
;	printfl.c 0
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#_pvaldec_val_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	printfl.c 117
;	printfl.c 118
	mov	dptr,#_pvaldec_val_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00102$
00120$:
;	printfl.c 119
	mov	dptr,#_pvaldec_lval_1_1
	clr	c
	clr	a
	subb	a,r3
	movx	@dptr,a
	clr	a
	subb	a,r4
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,r5
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,r6
	inc	dptr
	movx	@dptr,a
;	printfl.c 120
	setb	_sign
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	printfl.c 122
	mov	dptr,#_pvaldec_lval_1_1
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
;	printfl.c 123
	clr	_sign
;	printfl.c 126
00104$:
	mov	dptr,#_pvaldec_lval_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	a,r3
	orl	a,r4
	orl	a,r5
	orl	a,r6
	jnz	00121$
	ljmp	00106$
00121$:
;	printfl.c 127
	mov	dptr,#__modulong_PARM_2
	clr	a
	inc	dptr
	inc	dptr
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	mov	a,#0x0a
	movx	@dptr,a
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,r6
	lcall	__modulong
	mov	r7,dpl
	mov	r0,dph
	mov	r1,b
	mov	r2,a
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	mov	a,#0x30
	add	a,r7
	mov	r7,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r0
	mov	r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r1
	mov	r1,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r2
	mov	r2,a
	mov	_ch,r7
;	printfl.c 128
	 push _ch 
;	printfl.c 129
	mov	dptr,#__divulong_PARM_2
	clr	a
	inc	dptr
	inc	dptr
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	mov	a,#0x0a
	movx	@dptr,a
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,r6
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	dptr,#_pvaldec_lval_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	ljmp	00104$
00106$:
;	printfl.c 132
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _sign,00109$
00122$:
;	printfl.c 133
	mov	_ch,#0x2d
;	printfl.c 134
	 push _ch 
;	printfl.c 138
00109$:
	mov	a,_SP
	cjne	a,_SP,00123$
; Peephole 132   changed ljmp to sjmp
	sjmp 00112$
00123$:
;	printfl.c 139
	 pop _ch 
;	printfl.c 140
	mov	dpl,_ch
	lcall	_putchar
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00112$:
	C$printfl.c$142$1$1 ==.
	XFprintfl$pvaldec$0$0 ==.
	ret
	G$printf_small$0$0 ==.
;	printfl.c 144
;	-----------------------------------------
;	 function printf_small
;	-----------------------------------------
_printf_small:
	push	_bp
	mov	_bp,sp
	inc	sp
;	printfl.c 148
	mov	a,_bp
	add	a,#0xfb
	mov	r2,a
; Peephole 3.c   changed mov to clr
	clr  a
	mov  r3,a
	mov  r4,a
; Peephole 212  reduced add sequence to inc
	mov  r0,_bp
	inc  r0
	mov	@r0,ar2
00129$:
;	printfl.c 150
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	mov	ar3,@r0
	inc	r0
	mov	ar4,@r0
	inc	r0
	mov	ar5,@r0
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
	jnz	00154$
	ljmp	00133$
00154$:
;	printfl.c 151
	cjne	r6,#0x25,00155$
	sjmp	00156$
00155$:
	ljmp	00127$
00156$:
;	printfl.c 152
	clr	_short_flag
	clr	_string_flag
	clr	_long_flag
;	printfl.c 153
	inc	r3
	cjne	r3,#0x00,00157$
	inc	r4
00157$:
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	mov	@r0,ar3
	inc	r0
	mov	@r0,ar4
	inc	r0
	mov	@r0,ar5
;	printfl.c 154
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	r7,a
	cjne	r7,#0x68,00158$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00158$:
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r7,#0x6c,00103$
; Peephole 201   removed redundant sjmp
00159$:
00160$:
;	printfl.c 156
	setb	_long_flag
;	printfl.c 157
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	mov	a,#0x01
	add	a,r3
	mov	@r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r4
	inc	r0
	mov	@r0,a
	inc	r0
	mov	@r0,ar5
;	printfl.c 158
;	printfl.c 160
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
	setb	_short_flag
;	printfl.c 161
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	mov	a,#0x01
	add	a,r3
	mov	@r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r4
	inc	r0
	mov	@r0,a
	inc	r0
	mov	@r0,ar5
;	printfl.c 162
00103$:
;	printfl.c 164
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	mov	ar3,@r0
	inc	r0
	mov	ar4,@r0
	inc	r0
	mov	ar5,@r0
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	r3,a
	cjne	r3,#0x63,00161$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00161$:
	cjne	r3,#0x64,00162$
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00162$:
	cjne	r3,#0x6f,00163$
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00163$:
	cjne	r3,#0x73,00164$
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00164$:
; Peephole 132   changed ljmp to sjmp
;	printfl.c 166
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r3,#0x78,00109$
	sjmp 00106$
00165$:
00104$:
	setb	_string_flag
;	printfl.c 167
;	printfl.c 169
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00105$:
	mov	_radix,#0x0a
;	printfl.c 170
;	printfl.c 172
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00106$:
	mov	_radix,#0x10
;	printfl.c 173
;	printfl.c 175
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00107$:
	mov	_radix,#0x00
;	printfl.c 176
;	printfl.c 178
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
	mov	_radix,#0x08
;	printfl.c 180
00109$:
;	printfl.c 182
	jb	_string_flag,00166$
	ljmp	00114$
00166$:
;	printfl.c 183
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	a,@r1
	add	a,#0xfd
	mov	r0,a
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	@r1,ar0
	mov	_str,@r0
	inc	r0
	mov	(_str + 1),@r0
	inc	r0
	mov	(_str + 2),@r0
	dec	r0
	dec	r0
;	printfl.c 184
00110$:
	mov	_printf_small_sloc1_1_0,_str
	mov	(_printf_small_sloc1_1_0 + 1),(_str + 1)
	mov	(_printf_small_sloc1_1_0 + 2),(_str + 2)
	mov	dpl,_printf_small_sloc1_1_0
	mov	dph,(_printf_small_sloc1_1_0 + 1)
	mov	b,(_printf_small_sloc1_1_0 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  _printf_small_sloc0_1_0,a
	jnz	00167$
	ljmp	00131$
00167$:
	mov	dpl,_str
	mov	dph,(_str + 1)
	mov	b,(_str + 2)
	mov	a,#0x01
	add	a,_str
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(_str + 1)
	mov	r7,a
	mov	r3,(_str + 2)
	mov	_str,r2
	mov	(_str + 1),r7
	mov	(_str + 2),r3
	lcall	__gptrget
	mov	r4,a
	push	ar2
	push	ar3
	push	ar4
	push	ar6
	push	ar7
	mov	dpl,r4
	lcall	_putchar
	pop	ar7
	pop	ar6
	pop	ar4
	pop	ar3
	pop	ar2
;	printfl.c 185
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00114$:
;	printfl.c 188
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _long_flag,00119$
00168$:
;	printfl.c 189
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	a,@r1
	add	a,#0xfc
	mov	r0,a
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	@r1,ar0
	mov	_val,@r0
	inc	r0
	mov	(_val + 1),@r0
	inc	r0
	mov	(_val + 2),@r0
	inc	r0
	mov	(_val + 3),@r0
	dec	r0
	dec	r0
	dec	r0
; Peephole 132   changed ljmp to sjmp
	sjmp 00120$
00119$:
;	printfl.c 191
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _short_flag,00116$
00169$:
;	printfl.c 192
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	a,@r1
	add	a,#0xff
	mov	r0,a
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	@r1,ar0
	mov	ar2,@r0
	mov	_val,r2
	mov	a,r2
	rlc	a
	subb	a,acc
	mov	(_val + 1),a
	mov	(_val + 2),a
	mov	(_val + 3),a
; Peephole 132   changed ljmp to sjmp
	sjmp 00120$
00116$:
;	printfl.c 194
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	a,@r1
	add	a,#0xfe
	mov	r0,a
; Peephole 212  reduced add sequence to inc
	mov  r1,_bp
	inc  r1
	mov	@r1,ar0
	mov	ar2,@r0
	inc	r0
	mov	ar3,@r0
	dec	r0
	mov	_val,r2
	mov	(_val + 1),r3
	mov	a,r3
	rlc	a
	subb	a,acc
	mov	(_val + 2),a
	mov	(_val + 3),a
00120$:
;	printfl.c 196
	mov	a,_radix
; Peephole 110   removed ljmp by inverse jump logic
	jz  00123$
00170$:
	mov	a,_radix
	cjne	a,#0x08,00171$
; Peephole 132   changed ljmp to sjmp
	sjmp 00124$
00171$:
	mov	a,_radix
	cjne	a,#0x0a,00172$
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00172$:
	mov	a,_radix
; Peephole 132   changed ljmp to sjmp
;	printfl.c 198
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,#0x10,00131$
	sjmp 00122$
00173$:
00121$:
	push	ar6
	mov	dpl,_val
	mov	dph,(_val + 1)
	mov	b,(_val + 2)
	mov	a,(_val + 3)
	lcall	_pvaldec
	pop	ar6
;	printfl.c 199
;	printfl.c 201
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00122$:
	push	ar6
	mov	dpl,_val
	mov	dph,(_val + 1)
	mov	b,(_val + 2)
	mov	a,(_val + 3)
	lcall	_pvalhex
	pop	ar6
;	printfl.c 202
;	printfl.c 204
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00123$:
	mov	r2,_val
	push	ar6
	mov	dpl,r2
	lcall	_putchar
	pop	ar6
;	printfl.c 205
;	printfl.c 207
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00124$:
	push	ar6
	mov	dpl,_val
	mov	dph,(_val + 1)
	mov	b,(_val + 2)
	mov	a,(_val + 3)
	lcall	_pvaloct
	pop	ar6
;	printfl.c 209
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00127$:
;	printfl.c 212
	mov	dpl,r6
	lcall	_putchar
00131$:
;	printfl.c 150
	mov	a,_bp
	add	a,#0xfb
	mov	r0,a
	inc	@r0
	cjne	@r0,#0x00,00174$
	inc	r0
	inc	@r0
00174$:
	ljmp	00129$
00133$:
	mov	sp,_bp
	pop	_bp
	C$printfl.c$214$1$1 ==.
	XG$printf_small$0$0 ==.
	ret
	.area	CSEG    (CODE)
Fprintfl$_str_0$0$0 == .
__str_0:
	.ascii /0123456789ABCDEF/
	.byte	0
