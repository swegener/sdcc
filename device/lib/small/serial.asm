;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:18 2000

;--------------------------------------------------------
	.module serial
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _serial_getc
	.globl _serial_putc
	.globl _serial_interrupt_handler
	.globl _serial_init
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
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area	ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area	BSEG    (BIT)
Fserial$work_flag_byte_arrived$0$0==.
_work_flag_byte_arrived:
	.ds	0x0001
Fserial$work_flag_buffer_transfered$0$0==.
_work_flag_buffer_transfered:
	.ds	0x0001
Fserial$tx_serial_buffer_empty$0$0==.
_tx_serial_buffer_empty:
	.ds	0x0001
Fserial$rx_serial_buffer_empty$0$0==.
_rx_serial_buffer_empty:
	.ds	0x0001
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
Fserial$stx_index_in$0$0==.
_stx_index_in:
	.ds	0x0001
Fserial$srx_index_in$0$0==.
_srx_index_in:
	.ds	0x0001
Fserial$stx_index_out$0$0==.
_stx_index_out:
	.ds	0x0001
Fserial$srx_index_out$0$0==.
_srx_index_out:
	.ds	0x0001
Fserial$stx_buffer$0$0==.
_stx_buffer:
	.ds	0x0100
Fserial$srx_buffer$0$0==.
_srx_buffer:
	.ds	0x0100
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$serial_init$0$0 ==.
;	serial.c 24
;	-----------------------------------------
;	 function serial_init
;	-----------------------------------------
_serial_init:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	serial.c 26
	mov	_SCON,#0x50
;	serial.c 27
	mov	_T2CON,#0x34
;	serial.c 28
	setb	_PS
;	serial.c 29
	mov	_T2CON,#0x34
;	serial.c 30
	mov	_RCAP2H,#0xff
;	serial.c 31
	mov	_RCAP2L,#0xda
;	serial.c 33
	clr	_RI
;	serial.c 34
	clr	_TI
;	serial.c 36
	mov	dptr,#_srx_index_out
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
	mov	dptr,#_stx_index_out
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
	mov	dptr,#_srx_index_in
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
	mov	dptr,#_stx_index_in
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	serial.c 37
	setb	_tx_serial_buffer_empty
	setb	_rx_serial_buffer_empty
;	serial.c 38
	clr	_work_flag_buffer_transfered
;	serial.c 39
	clr	_work_flag_byte_arrived
;	serial.c 40
	setb	_ES
00101$:
	C$serial.c$41$1$1 ==.
	XG$serial_init$0$0 ==.
	ret
	G$serial_interrupt_handler$0$0 ==.
;	serial.c 43
;	-----------------------------------------
;	 function serial_interrupt_handler
;	-----------------------------------------
_serial_interrupt_handler:
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
;	serial.c 45
	clr	_ES
;	serial.c 46
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _TI,00105$
00113$:
;	serial.c 48
	clr	_TI
;	serial.c 49
	mov	dptr,#_stx_index_in
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_stx_index_out
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r3,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar2,00102$
; Peephole 201   removed redundant sjmp
00114$:
00115$:
;	serial.c 51
	setb	_tx_serial_buffer_empty
;	serial.c 52
	setb	_work_flag_buffer_transfered
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00102$:
;	serial.c 54
	mov	dptr,#_stx_index_out
	mov	a,#0x01
	add	a,r3
	movx	@dptr,a
	mov	a,r3
	add	a,#_stx_buffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_stx_buffer >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_SBUF,a
00105$:
;	serial.c 56
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RI,00107$
00116$:
;	serial.c 58
	clr	_RI
;	serial.c 59
	mov	dptr,#_srx_index_in
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_srx_index_in
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	mov	a,r2
	add	a,#_srx_buffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_srx_buffer >> 8)
	mov	dph,a
	mov	a,_SBUF
	movx	@dptr,a
;	serial.c 60
	setb	_work_flag_byte_arrived
;	serial.c 61
	clr	_rx_serial_buffer_empty
00107$:
;	serial.c 63
	setb	_ES
00108$:
	pop	psw
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	C$serial.c$64$1$1 ==.
	XG$serial_interrupt_handler$0$0 ==.
	reti
	G$serial_putc$0$0 ==.
;	serial.c 68
;	-----------------------------------------
;	 function serial_putc
;	-----------------------------------------
_serial_putc:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	serial.c 77
	mov	r2,dpl
;	serial.c 70
	mov	dptr,#_stx_index_in
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_stx_index_in
	mov	a,#0x01
	add	a,r3
	movx	@dptr,a
	mov	a,r3
	add	a,#_stx_buffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_stx_buffer >> 8)
	mov	dph,a
	mov	a,r2
	movx	@dptr,a
;	serial.c 71
	clr	_ES
;	serial.c 72
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _tx_serial_buffer_empty,00102$
00106$:
;	serial.c 74
	clr	_tx_serial_buffer_empty
;	serial.c 75
	setb	_TI
00102$:
;	serial.c 77
	setb	_ES
00103$:
	C$serial.c$78$1$1 ==.
	XG$serial_putc$0$0 ==.
	ret
	G$serial_getc$0$0 ==.
;	serial.c 80
;	-----------------------------------------
;	 function serial_getc
;	-----------------------------------------
_serial_getc:
;	serial.c 82
	mov	dptr,#_srx_index_out
	movx	a,@dptr
; Peephole 214 reduced some extra movs
	mov  r2,a
	add  a,#0x01	
; Peephole 100   removed redundant mov
	mov  r3,a
	mov  dptr,#_srx_index_out
	movx @dptr,a
	mov	a,r2
	add	a,#_srx_buffer
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#(_srx_buffer >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r2,a
;	serial.c 83
	clr	_ES
;	serial.c 84
	mov	dptr,#_srx_index_in
	movx	a,@dptr
	mov	r4,a
	mov	a,r3
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar4,00102$
; Peephole 201   removed redundant sjmp
00106$:
00107$:
	setb	_rx_serial_buffer_empty
00102$:
;	serial.c 85
	setb	_ES
;	serial.c 86
	mov	dpl,r2
00103$:
	C$serial.c$87$1$1 ==.
	XG$serial_getc$0$0 ==.
	ret
	.area	CSEG    (CODE)
