;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:18 2000

;--------------------------------------------------------
	.module ser_ir
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _ser_handler
	.globl _ser_can_xmt
	.globl _ser_can_rcv
	.globl _ser_getc
	.globl _ser_gets
	.globl _ser_puts
	.globl _ser_putc
	.globl _ser_init
	.globl _ser_gets_PARM_2
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
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
G$P1$0$0 == 0x0090
_P1	=	0x0090
G$SCON$0$0 == 0x0098
_SCON	=	0x0098
G$SBUF$0$0 == 0x0099
_SBUF	=	0x0099
G$IE$0$0 == 0x00a8
_IE	=	0x00a8
G$P3$0$0 == 0x00b0
_P3	=	0x00b0
G$IP$0$0 == 0x00b8
_IP	=	0x00b8
G$PSW$0$0 == 0x00d0
_PSW	=	0x00d0
G$ACC$0$0 == 0x00e0
_ACC	=	0x00e0
G$A$0$0 == 0x00e0
_A	=	0x00e0
G$B$0$0 == 0x00f0
_B	=	0x00f0
;--------------------------------------------------------
; special function bits 
;--------------------------------------------------------
G$IT0$0$0 == 0x0088
_IT0	=	0x0088
G$IE0$0$0 == 0x0089
_IE0	=	0x0089
G$IT1$0$0 == 0x008a
_IT1	=	0x008a
G$IE1$0$0 == 0x008b
_IE1	=	0x008b
G$TR0$0$0 == 0x008c
_TR0	=	0x008c
G$TF0$0$0 == 0x008d
_TF0	=	0x008d
G$TR1$0$0 == 0x008e
_TR1	=	0x008e
G$TF1$0$0 == 0x008f
_TF1	=	0x008f
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
G$AIN0$0$0 == 0x0090
_AIN0	=	0x0090
G$AIN1$0$0 == 0x0091
_AIN1	=	0x0091
G$RI$0$0 == 0x0098
_RI	=	0x0098
G$TI$0$0 == 0x0099
_TI	=	0x0099
G$RB8$0$0 == 0x009a
_RB8	=	0x009a
G$TB8$0$0 == 0x009b
_TB8	=	0x009b
G$REN$0$0 == 0x009c
_REN	=	0x009c
G$SM2$0$0 == 0x009d
_SM2	=	0x009d
G$SM1$0$0 == 0x009e
_SM1	=	0x009e
G$SM0$0$0 == 0x009f
_SM0	=	0x009f
G$EX0$0$0 == 0x00a8
_EX0	=	0x00a8
G$ET0$0$0 == 0x00a9
_ET0	=	0x00a9
G$EX1$0$0 == 0x00aa
_EX1	=	0x00aa
G$ET1$0$0 == 0x00ab
_ET1	=	0x00ab
G$ES$0$0 == 0x00ac
_ES	=	0x00ac
G$ET2$0$0 == 0x00ad
_ET2	=	0x00ad
G$EA$0$0 == 0x00af
_EA	=	0x00af
G$P3_0$0$0 == 0x00b0
_P3_0	=	0x00b0
G$P3_1$0$0 == 0x00b1
_P3_1	=	0x00b1
G$P3_2$0$0 == 0x00b2
_P3_2	=	0x00b2
G$P3_3$0$0 == 0x00b3
_P3_3	=	0x00b3
G$P3_4$0$0 == 0x00b4
_P3_4	=	0x00b4
G$P3_5$0$0 == 0x00b5
_P3_5	=	0x00b5
G$P3_7$0$0 == 0x00b7
_P3_7	=	0x00b7
G$RXD$0$0 == 0x00b0
_RXD	=	0x00b0
G$TXD$0$0 == 0x00b1
_TXD	=	0x00b1
G$INT0$0$0 == 0x00b2
_INT0	=	0x00b2
G$INT1$0$0 == 0x00b3
_INT1	=	0x00b3
G$T0$0$0 == 0x00b4
_T0	=	0x00b4
G$T1$0$0 == 0x00b5
_T1	=	0x00b5
G$AOUT$0$0 == 0x00b6
_AOUT	=	0x00b6
G$PX0$0$0 == 0x00b8
_PX0	=	0x00b8
G$PT0$0$0 == 0x00b9
_PT0	=	0x00b9
G$PX1$0$0 == 0x00ba
_PX1	=	0x00ba
G$PT1$0$0 == 0x00bb
_PT1	=	0x00bb
G$PS$0$0 == 0x00bc
_PS	=	0x00bc
G$P$0$0 == 0x00d0
_P	=	0x00d0
G$FL$0$0 == 0x00d1
_FL	=	0x00d1
G$OV$0$0 == 0x00d2
_OV	=	0x00d2
G$RS0$0$0 == 0x00d3
_RS0	=	0x00d3
G$RS1$0$0 == 0x00d4
_RS1	=	0x00d4
G$F0$0$0 == 0x00d5
_F0	=	0x00d5
G$AC$0$0 == 0x00d6
_AC	=	0x00d6
G$CY$0$0 == 0x00d7
_CY	=	0x00d7
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area	DSEG    (DATA)
Fser_ir$rbuf$0$0==.
_rbuf:
	.ds	0x000a
Fser_ir$xbuf$0$0==.
_xbuf:
	.ds	0x000a
Fser_ir$rcnt$0$0==.
_rcnt:
	.ds	0x0001
Fser_ir$xcnt$0$0==.
_xcnt:
	.ds	0x0001
Fser_ir$rpos$0$0==.
_rpos:
	.ds	0x0001
Fser_ir$xpos$0$0==.
_xpos:
	.ds	0x0001
Fser_ir$busy$0$0==.
_busy:
	.ds	0x0001
_ser_gets_PARM_2:
	.ds	0x0001
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
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
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$ser_init$0$0 ==.
;	ser_ir.c 49
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
;	ser_ir.c 51
	clr	_ES
;	ser_ir.c 52
	mov	_xpos,#0x00
	mov	_rpos,#0x00
	mov	_xcnt,#0x00
	mov	_rcnt,#0x00
;	ser_ir.c 53
	mov	_busy,#0x00
;	ser_ir.c 54
	mov	_SCON,#0x50
;	ser_ir.c 55
	orl	_PCON,#0x80
;	ser_ir.c 56
	anl	_TMOD,#0x0f
;	ser_ir.c 57
	orl	_TMOD,#0x20
;	ser_ir.c 58
	mov	_TL1,#0xfd
	mov	_TH1,#0xfd
	setb	_TR1
;	ser_ir.c 59
	setb	_ES
00101$:
	C$ser_ir.c$60$1$1 ==.
	XG$ser_init$0$0 ==.
	ret
	G$ser_handler$0$0 ==.
;	ser_ir.c 62
;	-----------------------------------------
;	 function ser_handler
;	-----------------------------------------
_ser_handler:
	push	acc
	push	b
	push	dpl
	push	dph
	push	ar2
	push	ar0
	push	psw
	mov	psw,#0x00
;	ser_ir.c 64
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RI,00104$
00118$:
;	ser_ir.c 65
	clr	_RI
;	ser_ir.c 67
	clr	c
	mov	a,_rcnt
	subb	a,#0x0a
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00104$
00119$:
;	ser_ir.c 68
	mov	r2,_rcnt
	inc	_rcnt
	mov	a,r2
	add	a,_rpos
	mov	b,#0x0a
	div	ab
	mov	r2,b
	mov	a,r2
	add	a,#_rbuf
	mov	r0,acc
	mov	@r0,_SBUF
00104$:
;	ser_ir.c 70
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _TI,00111$
00120$:
;	ser_ir.c 71
	clr	_TI
;	ser_ir.c 72
	mov	_busy,_xcnt
	mov	a,_xcnt
; Peephole 110   removed ljmp by inverse jump logic
	jz  00111$
00121$:
;	ser_ir.c 73
	dec	_xcnt
;	ser_ir.c 74
	mov	r2,_xpos
	inc	_xpos
	mov	a,r2
	add	a,#_xbuf
	mov	r0,a
	mov	_SBUF,@r0
;	ser_ir.c 75
	clr	c
	mov	a,_xpos
	subb	a,#0x0a
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00111$
00122$:
;	ser_ir.c 76
	mov	_xpos,#0x00
00111$:
	pop	psw
	pop	ar0
	pop	ar2
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	C$ser_ir.c$79$1$1 ==.
	XG$ser_handler$0$0 ==.
	reti
	G$ser_putc$0$0 ==.
;	ser_ir.c 81
;	-----------------------------------------
;	 function ser_putc
;	-----------------------------------------
_ser_putc:
;	ser_ir.c 92
	mov	r2,dpl
;	ser_ir.c 83
00101$:
	clr	c
	mov	a,_xcnt
	subb	a,#0x0a
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00101$
00112$:
;	ser_ir.c 85
	clr	_ES
;	ser_ir.c 86
	mov	a,_busy
; Peephole 110   removed ljmp by inverse jump logic
	jz  00105$
00113$:
;	ser_ir.c 87
	mov	r3,_xcnt
	inc	_xcnt
	mov	a,r3
	add	a,_xpos
	mov	b,#0x0a
	div	ab
	mov	r3,b
	mov	a,r3
	add	a,#_xbuf
	mov	r0,acc
	mov	@r0,ar2
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	ser_ir.c 89
	mov	_SBUF,r2
;	ser_ir.c 90
	mov	_busy,#0x01
00106$:
;	ser_ir.c 92
	setb	_ES
00107$:
	C$ser_ir.c$93$1$1 ==.
	XG$ser_putc$0$0 ==.
	ret
	G$ser_getc$0$0 ==.
;	ser_ir.c 95
;	-----------------------------------------
;	 function ser_getc
;	-----------------------------------------
_ser_getc:
;	ser_ir.c 98
00101$:
	mov	a,_rcnt
; Peephole 110   removed ljmp by inverse jump logic
	jz  00101$
00111$:
;	ser_ir.c 100
	clr	_ES
;	ser_ir.c 101
	dec	_rcnt
;	ser_ir.c 102
	mov	r2,_rpos
	inc	_rpos
	mov	a,r2
	add	a,#_rbuf
	mov	r0,a
	mov	ar2,@r0
;	ser_ir.c 103
	clr	c
	mov	a,_rpos
	subb	a,#0x0a
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00105$
00112$:
;	ser_ir.c 104
	mov	_rpos,#0x00
00105$:
;	ser_ir.c 105
	setb	_ES
;	ser_ir.c 106
	mov	dpl,r2
00106$:
	C$ser_ir.c$107$1$1 ==.
	XG$ser_getc$0$0 ==.
	ret
	G$ser_puts$0$0 ==.
;	ser_ir.c 110
;	-----------------------------------------
;	 function ser_puts
;	-----------------------------------------
_ser_puts:
;	ser_ir.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	ser_ir.c 113
00103$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	inc	r2
	cjne	r2,#0x00,00111$
	inc	r3
00111$:
	lcall	__gptrget
; Peephole 166   removed redundant mov
	mov  r5,a
	mov  ar6,r5 
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00112$:
;	ser_ir.c 114
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r6,#0x0a,00102$
; Peephole 201   removed redundant sjmp
00113$:
00114$:
	push	ar2
	push	ar3
	push	ar4
	push	ar6
	mov	dpl,#0x0d
	lcall	_ser_putc
	pop	ar6
	pop	ar4
	pop	ar3
	pop	ar2
00102$:
;	ser_ir.c 115
	push	ar2
	push	ar3
	push	ar4
	mov	dpl,r6
	lcall	_ser_putc
	pop	ar4
	pop	ar3
	pop	ar2
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00106$:
	C$ser_ir.c$117$1$1 ==.
	XG$ser_puts$0$0 ==.
	ret
	G$ser_gets$0$0 ==.
;	ser_ir.c 119
;	-----------------------------------------
;	 function ser_gets
;	-----------------------------------------
_ser_gets:
;	ser_ir.c 130
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	ser_ir.c 124
	mov	r5,#0x00
00105$:
	clr	c
	mov	a,_ser_gets_PARM_2
	subb	a,r5
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00113$:
;	ser_ir.c 125
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_ser_getc
	mov	r6,dpl
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	ser_ir.c 126
	cjne	r6,#0x0d,00114$
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00114$:
;	ser_ir.c 127
	mov	ar7,r5
	inc	r5
	mov	a,r7
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
	mov	b,r4
	mov	a,r6
	lcall	__gptrput
;	ser_ir.c 128
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r6,#0x0a,00105$
; Peephole 201   removed redundant sjmp
00115$:
00116$:
00107$:
;	ser_ir.c 130
	mov	a,r5
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
	mov	b,r4
; Peephole 180   changed mov to clr
	clr  a
	lcall	__gptrput
00108$:
	C$ser_ir.c$131$1$1 ==.
	XG$ser_gets$0$0 ==.
	ret
	G$ser_can_xmt$0$0 ==.
;	ser_ir.c 133
;	-----------------------------------------
;	 function ser_can_xmt
;	-----------------------------------------
_ser_can_xmt:
;	ser_ir.c 135
	clr	c
	mov	a,#0x0a
	subb	a,_xcnt
	mov	dpl,a
00101$:
	C$ser_ir.c$136$1$1 ==.
	XG$ser_can_xmt$0$0 ==.
	ret
	G$ser_can_rcv$0$0 ==.
;	ser_ir.c 138
;	-----------------------------------------
;	 function ser_can_rcv
;	-----------------------------------------
_ser_can_rcv:
;	ser_ir.c 140
	mov	dpl,_rcnt
00101$:
	C$ser_ir.c$141$1$1 ==.
	XG$ser_can_rcv$0$0 ==.
	ret
	.area	CSEG    (CODE)
