;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:22 2000

;--------------------------------------------------------
	.module _autobaud
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _autobaud
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
	G$autobaud$0$0 ==.
;	_autobaud.c 52
;	-----------------------------------------
;	 function autobaud
;	-----------------------------------------
_autobaud:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_autobaud.c 56
	mov	_TMOD,#0x11
;	_autobaud.c 57
	mov	_TCON,#0x00
;	_autobaud.c 58
	mov	_TL1,#0x00
	mov	_TH1,#0x00
;	_autobaud.c 62
00101$:
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00101$
00142$:
;	_autobaud.c 66
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00101$
00143$:
;	_autobaud.c 67
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00101$
00144$:
;	_autobaud.c 68
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00101$
00145$:
;	_autobaud.c 69
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00101$
00146$:
;	_autobaud.c 72
00113$:
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RXD,00113$
00147$:
;	_autobaud.c 73
	setb	_TR1
;	_autobaud.c 74
00116$:
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00116$
00148$:
;	_autobaud.c 75
00119$:
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RXD,00119$
00149$:
;	_autobaud.c 76
00122$:
; Peephole 112   removed ljmp by inverse jump logic
	jb   _RXD,00122$
00150$:
;	_autobaud.c 77
00125$:
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _RXD,00125$
00151$:
;	_autobaud.c 78
	clr	_TR1
;	_autobaud.c 81
	mov	a,_TH1
	add	a,acc
	mov	r2,a
	mov	a,_TL1
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r3,a
	orl	a,r2
;	_autobaud.c 84
; Peephole 105   removed redundant mov
	mov  _TH1,a
	add	a,acc
	mov	r2,a
	mov	a,_TL1
	rl	a
	rl	a
; Peephole 139   removed redundant mov
; Peephole 183   avoided anl during execution
	anl  a,#(0x03 & 0x01)
	mov  r3,a
	orl	a,r2
;	_autobaud.c 87
; Peephole 105   removed redundant mov
; Peephole 184   removed redundant mov
	cpl  a
;	_autobaud.c 90
; Peephole 185   changed order of increment (acc incremented also!)
	inc  a
	mov  _TH1,a
;	_autobaud.c 92
	mov	_TL1,_TH1
;	_autobaud.c 93
	mov	_TMOD,#0x21
;	_autobaud.c 94
	mov	_PCON,#0x80
;	_autobaud.c 95
	mov	_SCON,#0x52
00128$:
	C$_autobaud.c$96$1$1 ==.
	XG$autobaud$0$0 ==.
	ret
	.area	CSEG    (CODE)
