;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:15 2000

;--------------------------------------------------------
	.module _ser
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _ser_charAvail
	.globl _ser_printString
	.globl _ser_getc
	.globl _ser_putc
	.globl _ser_interrupt_handler
	.globl _ser_init
	.globl _ser_rxBuffer
	.globl _ser_txBuffer
	.globl _ser_rxIndexOut
	.globl _ser_rxIndexIn
	.globl _ser_txIndexOut
	.globl _ser_txIndexIn
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
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area	ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area	BSEG    (BIT)
F_ser$ser_txBusy$0$0==.
_ser_txBusy:
	.ds	0x0001
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
G$ser_txIndexIn$0$0==.
_ser_txIndexIn:
	.ds	0x0001
G$ser_txIndexOut$0$0==.
_ser_txIndexOut:
	.ds	0x0001
G$ser_rxIndexIn$0$0==.
_ser_rxIndexIn:
	.ds	0x0001
G$ser_rxIndexOut$0$0==.
_ser_rxIndexOut:
	.ds	0x0001
G$ser_txBuffer$0$0==.
_ser_txBuffer:
	.ds	0x0100
G$ser_rxBuffer$0$0==.
_ser_rxBuffer:
	.ds	0x0100
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$ser_init$0$0 ==.
;	_ser.c 49
;	-----------------------------------------
;	 function ser_init
;	-----------------------------------------
_ser_init:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_ser.c 51
	clr	_ES
;	_ser.c 53
	clr	_ser_txBusy
;	_ser.c 55
	mov	dptr,#_ser_txIndexIn
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_ser.c 56
	mov	dptr,#_ser_txIndexOut
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_ser.c 57
	mov	dptr,#_ser_rxIndexIn
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_ser.c 58
	mov	dptr,#_ser_rxIndexOut
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_ser.c 60
	mov	_T2CON,#0x30
;	_ser.c 63
	mov	_RCAP2H,#0xff
;	_ser.c 64
	mov	_RCAP2L,#0xdd
;	_ser.c 67
	mov	_T2CON,#0x34
;	_ser.c 69
	mov	_SCON,#0x50
;	_ser.c 71
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _TI,00102$
00109$:
;	_ser.c 72
	clr	_TI
00102$:
;	_ser.c 74
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RI,00104$
00110$:
;	_ser.c 75
	clr	_RI
00104$:
;	_ser.c 78
	setb	_ES
00105$:
	C$_ser.c$79$1$1 ==.
	XG$ser_init$0$0 ==.
	ret
	G$ser_interrupt_handler$0$0 ==.
;	_ser.c 82
;	-----------------------------------------
;	 function ser_interrupt_handler
;	-----------------------------------------
_ser_interrupt_handler:
	ar2 = 0x0a
	ar3 = 0x0b
	ar4 = 0x0c
	ar5 = 0x0d
	ar6 = 0x0e
	ar7 = 0x0f
	ar0 = 0x08
	ar1 = 0x09
	push	acc
	push	b
	push	dpl
	push	dph
	push	psw
	mov	psw,#0x08
;	_ser.c 84
	clr	_ES
;	_ser.c 86
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _TI,00105$
00113$:
;	_ser.c 87
	clr	_TI
;	_ser.c 88
	mov	dptr,#_ser_txIndexOut
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_ser_txIndexIn
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r3,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar2,00102$
; Peephole 201   removed redundant sjmp
00114$:
00115$:
;	_ser.c 89
	clr	_ser_txBusy
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00102$:
;	_ser.c 92
	mov	dptr,#_ser_txIndexOut
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	mov	a,r2
	add	a,#_ser_txBuffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_ser_txBuffer >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_SBUF,a
00105$:
;	_ser.c 96
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RI,00107$
00116$:
;	_ser.c 97
	clr	_RI
;	_ser.c 98
	mov	dptr,#_ser_rxIndexIn
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_ser_rxIndexIn
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	mov	a,r2
	add	a,#_ser_rxBuffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_ser_rxBuffer >> 8)
	mov	dph,a
	mov	a,_SBUF
	movx	@dptr,a
00107$:
;	_ser.c 101
	setb	_ES
00108$:
	pop	psw
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	C$_ser.c$102$1$1 ==.
	XG$ser_interrupt_handler$0$0 ==.
	reti
	G$ser_putc$0$0 ==.
;	_ser.c 105
;	-----------------------------------------
;	 function ser_putc
;	-----------------------------------------
_ser_putc:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_ser.c 117
	mov	r2,dpl
;	_ser.c 107
	clr	_ES
;	_ser.c 109
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _ser_txBusy,00102$
00107$:
;	_ser.c 110
	mov	dptr,#_ser_txIndexIn
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_ser_txIndexIn
	mov	a,#0x01
	add	a,r3
	movx	@dptr,a
	mov	a,r3
	add	a,#_ser_txBuffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_ser_txBuffer >> 8)
	mov	dph,a
	mov	a,r2
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
;	_ser.c 113
	setb	_ser_txBusy
;	_ser.c 114
	mov	_SBUF,r2
00103$:
;	_ser.c 117
	setb	_ES
00104$:
	C$_ser.c$118$1$1 ==.
	XG$ser_putc$0$0 ==.
	ret
	G$ser_getc$0$0 ==.
;	_ser.c 121
;	-----------------------------------------
;	 function ser_getc
;	-----------------------------------------
_ser_getc:
;	_ser.c 126
	mov	dptr,#_ser_rxIndexOut
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_ser_rxIndexIn
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r3,a
	cjne	a,ar2,00107$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00107$:
;	_ser.c 127
	mov	dptr,#_ser_rxIndexOut
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	mov	a,r2
	add	a,#_ser_rxBuffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_ser_rxBuffer >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r2,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
;	_ser.c 130
	mov	r2,#0x00
00103$:
;	_ser.c 134
	mov	dpl,r2
00104$:
	C$_ser.c$135$1$1 ==.
	XG$ser_getc$0$0 ==.
	ret
	G$ser_printString$0$0 ==.
;	_ser.c 138
;	-----------------------------------------
;	 function ser_printString
;	-----------------------------------------
_ser_printString:
;	_ser.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_ser.c 140
00101$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00108$:
;	_ser.c 141
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	inc	r2
	cjne	r2,#0x00,00109$
	inc	r3
00109$:
	lcall	__gptrget
	mov	r5,a
	push	ar2
	push	ar3
	push	ar4
	mov	dpl,r5
	lcall	_ser_putc
	pop	ar4
	pop	ar3
	pop	ar2
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00104$:
	C$_ser.c$143$1$1 ==.
	XG$ser_printString$0$0 ==.
	ret
	G$ser_charAvail$0$0 ==.
;	_ser.c 146
;	-----------------------------------------
;	 function ser_charAvail
;	-----------------------------------------
_ser_charAvail:
;	_ser.c 148
	mov	r2,#0x00
;	_ser.c 150
	mov	dptr,#_ser_rxIndexOut
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_ser_rxIndexIn
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r4,a
	cjne	a,ar3,00106$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00106$:
;	_ser.c 151
	mov	r2,#0x01
00102$:
;	_ser.c 154
	mov	dpl,r2
00103$:
	C$_ser.c$155$1$1 ==.
	XG$ser_charAvail$0$0 ==.
	ret
	.area	CSEG    (CODE)
