;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.3.4 Sun Mar 30 09:16:00 2003

;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.module and2
	list	p=16f877
	__CONFIG 0x3fff
	radix dec
include "p16f877.inc"
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
;	.globl _main
;	.globl _and_compound2
;	.globl _and_compound1
;	.globl _done
;	.globl _RD
;	.globl _WR
;	.globl _WREN
;	.globl _WRERR
;	.globl _EEPGD
;	.globl _PCFG0
;	.globl _PCFG1
;	.globl _PCFG2
;	.globl _PCFG3
;	.globl _ADFM
;	.globl _TXD8
;	.globl _TX9D
;	.globl _TRMT
;	.globl _BRGH
;	.globl _SYNC
;	.globl _TXEN
;	.globl _TX8_9
;	.globl _NOT_TX8
;	.globl _TX9
;	.globl _CSRC
;	.globl _BF
;	.globl _UA
;	.globl _READ_WRITE
;	.globl _R_W
;	.globl _NOT_WRITE
;	.globl _NOT_W
;	.globl _I2C_READ
;	.globl _R
;	.globl _I2C_START
;	.globl _S
;	.globl _I2C_STOP
;	.globl _P
;	.globl _DATA_ADDRESS
;	.globl _D_A
;	.globl _NOT_ADDRESS
;	.globl _NOT_A
;	.globl _I2C_DATA
;	.globl _D
;	.globl _CKE
;	.globl _SMP
;	.globl _SEN
;	.globl _RSEN
;	.globl _PEN
;	.globl _RCEN
;	.globl _ACKEN
;	.globl _ACKDT
;	.globl _ACKSTAT
;	.globl _GCEN
;	.globl _NOT_BOR
;	.globl _NOT_BO
;	.globl _NOT_POR
;	.globl _CCP2IE
;	.globl _BCLIE
;	.globl _EEIE
;	.globl _TMR1IE
;	.globl _TMR2IE
;	.globl _CCP1IE
;	.globl _SSPIE
;	.globl _TXIE
;	.globl _RCIE
;	.globl _ADIE
;	.globl _PS0
;	.globl _PS1
;	.globl _PS2
;	.globl _PSA
;	.globl _T0SE
;	.globl _T0CS
;	.globl _INTEDG
;	.globl _NOT_RBPU
;	.globl _ADON
;	.globl _GO_DONE
;	.globl _NOT_DONE
;	.globl _GO
;	.globl _CHS0
;	.globl _CHS1
;	.globl _CHS2
;	.globl _ADCS0
;	.globl _ADCS1
;	.globl _CCP2M0
;	.globl _CCP2M1
;	.globl _CCP2M2
;	.globl _CCP2M3
;	.globl _CCP2Y
;	.globl _CCP2X
;	.globl _RCD8
;	.globl _RX9D
;	.globl _OERR
;	.globl _FERR
;	.globl _ADDEN
;	.globl _CREN
;	.globl _SREN
;	.globl _RC8_9
;	.globl _NOT_RC8
;	.globl _RC9
;	.globl _RX9
;	.globl _SPEN
;	.globl _CCP1M0
;	.globl _CCP1M1
;	.globl _CCP1M2
;	.globl _CCP1M3
;	.globl _CCP1Y
;	.globl _CCP1X
;	.globl _SSPM0
;	.globl _SSPM1
;	.globl _SSPM2
;	.globl _SSPM3
;	.globl _CKP
;	.globl _SSPEN
;	.globl _SSPOV
;	.globl _WCOL
;	.globl _T2CKPS0
;	.globl _T2CKPS1
;	.globl _TMR2ON
;	.globl _TOUTPS0
;	.globl _TOUTPS1
;	.globl _TOUTPS2
;	.globl _TOUTPS3
;	.globl _TMR1ON
;	.globl _TMR1CS
;	.globl _T1SYNC
;	.globl _T1INSYNC
;	.globl _NOT_T1SYNC
;	.globl _T1OSCEN
;	.globl _T1CKPS0
;	.globl _T1CKPS1
;	.globl _CCP2IF
;	.globl _BCLIF
;	.globl _EEIF
;	.globl _CMIF
;	.globl _TMR1IF
;	.globl _TMR2IF
;	.globl _CCP1IF
;	.globl _SSPIF
;	.globl _TXIF
;	.globl _RCIF
;	.globl _ADIF
;	.globl _RBIF
;	.globl _INTF
;	.globl _T0IF
;	.globl _RBIE
;	.globl _INTE
;	.globl _T0IE
;	.globl _PEIE
;	.globl _GIE
;	.globl _C
;	.globl _DC
;	.globl _Z
;	.globl _NOT_PD
;	.globl _NOT_TO
;	.globl _RP0
;	.globl _RP1
;	.globl _IRP
;	.globl _ulong0
;	.globl _uchar1
;	.globl _uchar0
;	.globl _uint1
;	.globl _uint0
;	.globl _dummy
;	.globl _failures
;	.globl _success
;	.globl _PCL
;	.globl _INDF
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
_TMR0	EQU	0x0001
_STATUS	EQU	0x0003
_FSR	EQU	0x0004
_PORTA	EQU	0x0005
_PORTB	EQU	0x0006
_PORTC	EQU	0x0007
_PCLATH	EQU	0x000a
_INTCON	EQU	0x000b
_PIR1	EQU	0x000c
_PIR2	EQU	0x000d
_TMR1L	EQU	0x000e
_TMR1H	EQU	0x000f
_T1CON	EQU	0x0010
_TMR2	EQU	0x0011
_T2CON	EQU	0x0012
_SSPBUF	EQU	0x0013
_SSPCON	EQU	0x0014
_CCPR1L	EQU	0x0015
_CCPR1H	EQU	0x0016
_CCP1CON	EQU	0x0017
_RCSTA	EQU	0x0018
_TXREG	EQU	0x0019
_RCREG	EQU	0x001a
_CCPR2L	EQU	0x001b
_CCPR2H	EQU	0x001c
_CCP2CON	EQU	0x001d
_ADRESH	EQU	0x001e
_ADCON0	EQU	0x001f
_OPTION_REG	EQU	0x0081
_TRISA	EQU	0x0085
_TRISB	EQU	0x0086
_TRISC	EQU	0x0087
_PIE1	EQU	0x008c
_PIE2	EQU	0x008d
_PCON	EQU	0x008e
_SSPCON2	EQU	0x0091
_PR2	EQU	0x0092
_SSPADD	EQU	0x0093
_SSPSTAT	EQU	0x0094
_TXSTA	EQU	0x0098
_SPBRG	EQU	0x0099
_ADRESL	EQU	0x009e
_ADCON1	EQU	0x009f
_EEDATA	EQU	0x010c
_EEADR	EQU	0x010d
_EEDATH	EQU	0x010e
_EEADRH	EQU	0x010f
_EECON1	EQU	0x018c
_EECON2	EQU	0x018d
  cblock  0X0020	; Bank 0
	_dummy
	_uchar0
	_failures
	_uchar1
  endc
  cblock  0X0120	; Bank 0
	_dummy_120
	_uchar0_120
	_failures_120
	_uchar1_120
  endc
_dummy	EQU	0x020
_uchar0	EQU	0x021
_failures	EQU	0x022
_uchar1	EQU	0x023
_STATUS	EQU	0x003
PCL	EQU	0x002
_PCLATH	EQU	0x00a
FSR	EQU	0x004
INDF	EQU	0x000
INTCON	EQU	0x00b
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
;	.area	SSEG	(DATA)
;__start__stack:
;	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
;	.area	ISEG    (DATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
;	.area	XSEG    (XDATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
;	.area	BSEG    (BIT)
_IRP	EQU	0x001f
_RP1	EQU	0x001e
_RP0	EQU	0x001d
_NOT_TO	EQU	0x001c
_NOT_PD	EQU	0x001b
_Z	EQU	0x001a
_DC	EQU	0x0019
_C	EQU	0x0018
_GIE	EQU	0x005f
_PEIE	EQU	0x005e
_T0IE	EQU	0x005d
_INTE	EQU	0x005c
_RBIE	EQU	0x005b
_T0IF	EQU	0x005a
_INTF	EQU	0x0059
_RBIF	EQU	0x0058
_ADIF	EQU	0x0066
_RCIF	EQU	0x0065
_TXIF	EQU	0x0064
_SSPIF	EQU	0x0063
_CCP1IF	EQU	0x0062
_TMR2IF	EQU	0x0061
_TMR1IF	EQU	0x0060
_CMIF	EQU	0x006e
_EEIF	EQU	0x006c
_BCLIF	EQU	0x006b
_CCP2IF	EQU	0x0068
_T1CKPS1	EQU	0x0085
_T1CKPS0	EQU	0x0084
_T1OSCEN	EQU	0x0083
_NOT_T1SYNC	EQU	0x0082
_T1INSYNC	EQU	0x0082
_T1SYNC	EQU	0x0082
_TMR1CS	EQU	0x0081
_TMR1ON	EQU	0x0080
_TOUTPS3	EQU	0x0096
_TOUTPS2	EQU	0x0095
_TOUTPS1	EQU	0x0094
_TOUTPS0	EQU	0x0093
_TMR2ON	EQU	0x0092
_T2CKPS1	EQU	0x0091
_T2CKPS0	EQU	0x0090
_WCOL	EQU	0x00a7
_SSPOV	EQU	0x00a6
_SSPEN	EQU	0x00a5
_CKP	EQU	0x00a4
_SSPM3	EQU	0x00a3
_SSPM2	EQU	0x00a2
_SSPM1	EQU	0x00a1
_SSPM0	EQU	0x00a0
_CCP1X	EQU	0x00bd
_CCP1Y	EQU	0x00bc
_CCP1M3	EQU	0x00bb
_CCP1M2	EQU	0x00ba
_CCP1M1	EQU	0x00b9
_CCP1M0	EQU	0x00b8
_SPEN	EQU	0x00c7
_RX9	EQU	0x00c6
_RC9	EQU	0x00c6
_NOT_RC8	EQU	0x00c6
_RC8_9	EQU	0x00c6
_SREN	EQU	0x00c5
_CREN	EQU	0x00c4
_ADDEN	EQU	0x00c3
_FERR	EQU	0x00c2
_OERR	EQU	0x00c1
_RX9D	EQU	0x00c0
_RCD8	EQU	0x00c0
_CCP2X	EQU	0x00ed
_CCP2Y	EQU	0x00ec
_CCP2M3	EQU	0x00eb
_CCP2M2	EQU	0x00ea
_CCP2M1	EQU	0x00e9
_CCP2M0	EQU	0x00e8
_ADCS1	EQU	0x00ff
_ADCS0	EQU	0x00fe
_CHS2	EQU	0x00fd
_CHS1	EQU	0x00fc
_CHS0	EQU	0x00fb
_GO	EQU	0x00fa
_NOT_DONE	EQU	0x00fa
_GO_DONE	EQU	0x00fa
_ADON	EQU	0x00f8
_NOT_RBPU	EQU	0x040f
_INTEDG	EQU	0x040e
_T0CS	EQU	0x040d
_T0SE	EQU	0x040c
_PSA	EQU	0x040b
_PS2	EQU	0x040a
_PS1	EQU	0x0409
_PS0	EQU	0x0408
_ADIE	EQU	0x0466
_RCIE	EQU	0x0465
_TXIE	EQU	0x0464
_SSPIE	EQU	0x0463
_CCP1IE	EQU	0x0462
_TMR2IE	EQU	0x0461
_TMR1IE	EQU	0x0460
_EEIE	EQU	0x046c
_BCLIE	EQU	0x046b
_CCP2IE	EQU	0x0468
_NOT_POR	EQU	0x0471
_NOT_BO	EQU	0x0470
_NOT_BOR	EQU	0x0470
_GCEN	EQU	0x048f
_ACKSTAT	EQU	0x048e
_ACKDT	EQU	0x048d
_ACKEN	EQU	0x048c
_RCEN	EQU	0x048b
_PEN	EQU	0x048a
_RSEN	EQU	0x0489
_SEN	EQU	0x0488
_SMP	EQU	0x04a7
_CKE	EQU	0x04a6
_D	EQU	0x04a5
_I2C_DATA	EQU	0x04a5
_NOT_A	EQU	0x04a5
_NOT_ADDRESS	EQU	0x04a5
_D_A	EQU	0x04a5
_DATA_ADDRESS	EQU	0x04a5
_P	EQU	0x04a4
_I2C_STOP	EQU	0x04a4
_S	EQU	0x04a3
_I2C_START	EQU	0x04a3
_R	EQU	0x04a2
_I2C_READ	EQU	0x04a2
_NOT_W	EQU	0x04a2
_NOT_WRITE	EQU	0x04a2
_R_W	EQU	0x04a2
_READ_WRITE	EQU	0x04a2
_UA	EQU	0x04a1
_BF	EQU	0x04a0
_CSRC	EQU	0x04c7
_TX9	EQU	0x04c6
_NOT_TX8	EQU	0x04c6
_TX8_9	EQU	0x04c6
_TXEN	EQU	0x04c5
_SYNC	EQU	0x04c4
_BRGH	EQU	0x04c2
_TRMT	EQU	0x04c1
_TX9D	EQU	0x04c0
_TXD8	EQU	0x04c0
_ADFM	EQU	0x04ff
_PCFG3	EQU	0x04fb
_PCFG2	EQU	0x04fa
_PCFG1	EQU	0x04f9
_PCFG0	EQU	0x04f8
_EEPGD	EQU	0x0c67
_WRERR	EQU	0x0c63
_WREN	EQU	0x0c62
_WR	EQU	0x0c61
_RD	EQU	0x0c60
	ORG 0
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
;	.area	CSEG    (CODE)
;__interrupt_vect:
;	ljmp	__sdcc_gsinit_startup
;	reti
;	.ds	7
;	reti
;	.ds	7
;	reti
;	.ds	7
;	reti
;	.ds	7
;	reti
;	.ds	7
;	reti
;	.ds	7
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
;	.area GSINIT  (CODE)
;	.area GSFINAL (CODE)
;	.area GSINIT  (CODE)
__sdcc_gsinit_startup:
;	.area GSFINAL (CODE)
;	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
;	.area CSEG    (CODE)
;***
;  pBlock Stats: dbName = D
;***
;; Starting pCode block
;<>Start of new flow, seq=0x9 ancestor = 0x9
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _success, size = 1
;; 	line = 9247 result AOP_DIR=_success, left -=-, right AOP_LIT=0x00, size = 1
;; ***	genAssign  9321
;;	1241
;;	1257  _success   offset=0
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _failures, size = 1
;; 	line = 9247 result AOP_DIR=_failures, left -=-, right AOP_LIT=0x00, size = 1
;; ***	genAssign  9321
;;	1241
;;	1257  _failures   offset=0
;#CSRC	and2.c 54
;  }

	CLRF	_failures	;key=000,flow seq=009
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _dummy, size = 1
;; 	line = 9247 result AOP_DIR=_dummy, left -=-, right AOP_LIT=0x00, size = 1
;; ***	genAssign  9321
;;	1241
;;	1257  _dummy   offset=0
;#CSRC	and2.c 54
;  }

	CLRF	_dummy	;key=001,flow seq=009
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uint0, size = 2
;; 	line = 9247 result AOP_DIR=_uint0, left -=-, right AOP_LIT=0x00, size = 2
;; ***	genAssign  9321
;;	1241
;;	1257  _uint0   offset=0
;; ***	genAssign  9321
;;	1241
;;	1257  _uint0   offset=1
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uint1, size = 2
;; 	line = 9247 result AOP_DIR=_uint1, left -=-, right AOP_LIT=0x00, size = 2
;; ***	genAssign  9321
;;	1241
;;	1257  _uint1   offset=0
;; ***	genAssign  9321
;;	1241
;;	1257  _uint1   offset=1
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uchar0, size = 1
;; 	line = 9247 result AOP_DIR=_uchar0, left -=-, right AOP_LIT=0x00, size = 1
;; ***	genAssign  9321
;;	1241
;;	1257  _uchar0   offset=0
;#CSRC	and2.c 54
;  }

	CLRF	_uchar0	;key=002,flow seq=009
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uchar1, size = 1
;; 	line = 9247 result AOP_DIR=_uchar1, left -=-, right AOP_LIT=0x00, size = 1
;; ***	genAssign  9321
;;	1241
;;	1257  _uchar1   offset=0
;#CSRC	and2.c 54
;  }

	CLRF	_uchar1	;key=003,flow seq=009
;; Starting pCode block
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _ulong0, size = 4
;; 	line = 9247 result AOP_DIR=_ulong0, left -=-, right AOP_LIT=0x00, size = 4
;; ***	genAssign  9321
;;	1241
;;	1257  _ulong0   offset=0
;; ***	genAssign  9321
;;	1241
;;	1257  _ulong0   offset=1
;; ***	genAssign  9321
;;	1241
;;	1257  _ulong0   offset=2
;; ***	genAssign  9321
;;	1241
;;	1257  _ulong0   offset=3
;***
;  pBlock Stats: dbName = M
;***
;entry:  _main	;Function start
; 2 exit points
;has an exit
;functions called:
;   _and_compound1
;   _and_compound2
;   _done
;; Starting pCode block
;<>Start of new flow, seq=0x1 ancestor = 0x1
;;ic
;; ***	genLabel  3059
;;ic
;; ***	genFunction  2646 curr label offset=15previous max_key=5 
;;	-----------------------------------------
;;	 function main
;;	-----------------------------------------
;_main:
_main	;Function start
; 2 exit points
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uchar0, size = 1
;; 	line = 9247 result AOP_DIR=_uchar0, left -=-, right AOP_LIT=0x13, size = 1
;; ***	genAssign  9321
;#CSRC	and2.c 45
;  uchar0 = 0x13;
	MOVLW	0x13	;key=000,flow seq=001
;;	1241
;;	1257  _uchar0   offset=0
	MOVWF	_uchar0	;key=001,flow seq=001
;;ic
;; ***	genCall  2355
;; ***	saveRegisters  2026
;;ic
;; ***	genPlus  781
;; ***	aopForSym 434
;;	520 sym->rname = _uchar0, size = 1
;;	794
;;	811 size=1
;;Warning -pic port ignoring get(AOP_ACC)	1088
;; 	line = 787 result AOP_ACC=AOP_accumulator_bug, left AOP_DIR=_uchar0, right AOP_LIT=0x01, size = 1
;; ***	pic14_getDataSize  1751
;; ***	genPlusIncr  161
;; 	result AOP_ACC, left AOP_DIR, right AOP_LIT
;; 	genPlusIncr  173
;; 	genPlusIncr  197
;; 	couldn't increment 
;; ***	pic14_getDataSize  1751
;;	adding lit to something. size 1
;; ***	genAddLit  455
;; ***	pic14_getDataSize  1751
;;  left and result aren't same	genAddLit  680
;;	1241
;;	1257  _uchar0   offset=0
_and_compound1
;#CSRC	and2.c 25
;  uchar0 = (uchar0 + 1) & 0x0f;
	INCF	_uchar0,W	;key=002,flow seq=001
;; ***	emitMOVWF  438 ignoring mov into W
;;ic
;; ***	ifxForOp  5035
;; ***	genAnd  5242
;;	794
;;	811 size=1
;; ***	aopForSym 434
;;	520 sym->rname = _uchar0, size = 1
;;Warning -pic port ignoring get(AOP_ACC)	1088
;; 	line = 5276 result AOP_DIR=_uchar0, left AOP_ACC=AOP_accumulator_bug, right AOP_LIT=0x0f, size = 1
	ANDLW	0x0f	;key=003,flow seq=001
;;	1241
;;	1257  _uchar0   offset=0
;;     peep 2 - Removed redundant move
	MOVWF	_uchar0	;key=004,flow seq=001
;;     peep 1 - test/jump to test/skip
	XORLW	0x04	;key=005,flow seq=001
	BTFSS	_STATUS,2	;key=006,flow seq=001
;<>Start of new flow, seq=0x2 ancestor = 0x1
	INCF	_failures,F	;key=000,flow seq=002
;<>Start of new flow, seq=0x3 ancestor = 0x1
;; ***	unsaveRegisters  2088
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _uchar1, size = 1
;; 	line = 9247 result AOP_DIR=_uchar1, left -=-, right AOP_LIT=0x42, size = 1
;; ***	genAssign  9321
;#CSRC	and2.c 48
;  uchar1 = 0x42;
	MOVLW	0x42	;key=000,flow seq=003
;;	1241
;;	1257  _uchar1   offset=0
	MOVWF	_uchar1	;key=001,flow seq=003
;;ic
;; ***	genCall  2355
;; ***	saveRegisters  2026
;;ic
;; ***	genPlus  781
;; ***	aopForSym 434
;;	520 sym->rname = _uchar1, size = 1
;;	794
;;	811 size=1
;;Warning -pic port ignoring get(AOP_ACC)	1088
;; 	line = 787 result AOP_ACC=AOP_accumulator_bug, left AOP_DIR=_uchar1, right AOP_LIT=0x01, size = 1
;; ***	pic14_getDataSize  1751
;; ***	genPlusIncr  161
;; 	result AOP_ACC, left AOP_DIR, right AOP_LIT
;; 	genPlusIncr  173
;; 	genPlusIncr  197
;; 	couldn't increment 
;; ***	pic14_getDataSize  1751
;;	adding lit to something. size 1
;; ***	genAddLit  455
;; ***	pic14_getDataSize  1751
;;  left and result aren't same	genAddLit  680
;;	1241
;;	1255  _uchar1   offset=0 - had to alloc by reg name
_and_compound2
;#CSRC	and2.c 34
;  uchar0 = (uchar1 + 1) & 0x0f;
	INCF	_uchar1,W	;key=002,flow seq=003
;; ***	emitMOVWF  438 ignoring mov into W
;;ic
;; ***	ifxForOp  5035
;; ***	genAnd  5242
;;	794
;;	811 size=1
;; ***	aopForSym 434
;;	520 sym->rname = _uchar0, size = 1
;;Warning -pic port ignoring get(AOP_ACC)	1088
;; 	line = 5276 result AOP_DIR=_uchar0, left AOP_ACC=AOP_accumulator_bug, right AOP_LIT=0x0f, size = 1
	ANDLW	0x0f	;key=003,flow seq=003
;;	1241
;;	1257  _uchar0   offset=0
;;     peep 2 - Removed redundant move
	MOVWF	_uchar0	;key=004,flow seq=003
;;     peep 1 - test/jump to test/skip
	XORLW	0x03	;key=005,flow seq=003
	BTFSS	_STATUS,2	;key=006,flow seq=003
;<>Start of new flow, seq=0x4 ancestor = 0x3
	INCF	_failures,F	;key=000,flow seq=004
;<>Start of new flow, seq=0x5 ancestor = 0x3
	MOVF	_uchar1,W	;key=000,flow seq=005
;;     peep 1 - test/jump to test/skip
	XORLW	0x42	;key=001,flow seq=005
	BTFSS	_STATUS,2	;key=002,flow seq=005
;<>Start of new flow, seq=0x6 ancestor = 0x5
	INCF	_failures,F	;key=000,flow seq=006
;<>Start of new flow, seq=0x7 ancestor = 0x5
;; ***	unsaveRegisters  2088
;;ic
;; ***	genAssign  9238
;; ***	aopForSym 434
;;	520 sym->rname = _failures, size = 1
;; ***	aopForSym 434
;;	520 sym->rname = _success, size = 1
;; 	line = 9247 result AOP_DIR=_success, left -=-, right AOP_DIR=_failures, size = 1
;; ***	genAssign  9321
;; ***	genAssign  9340
;;	1241
;;	1257  _failures   offset=0
;#CSRC	and2.c 51
;  success = failures;
	MOVF	_failures,W	;key=000,flow seq=007
;;	1241
;;	1257  _success   offset=0
;;ic
;; ***	genCall  2355
;; ***	saveRegisters  2026
;;ic
;; ***	genPlus  781
;; ***	aopForSym 434
;;	520 sym->rname = _dummy, size = 1
;; 	line = 787 result AOP_DIR=_dummy, left AOP_DIR=_dummy, right AOP_LIT=0x01, size = 1
;; ***	pic14_getDataSize  1751
;; ***	genPlusIncr  161
;; 	result AOP_DIR, left AOP_DIR, right AOP_LIT
;; 	genPlusIncr  173
;;	1241
;;	1257  _dummy   offset=0
_done
;#CSRC	and2.c 18
;  dummy++;
	INCF	_dummy,F	;key=001,flow seq=007
;;ic
;; ***	genLabel  3059
;_00105_DS_:
;;ic
;; ***	genEndFunction  2829
;;	C$and2.c$20$1$1 ==.
;;	XG$done$0$0 ==.
;return
;; ***	unsaveRegisters  2088
;;ic
;; ***	genLabel  3059
;_00125_DS_:
;;ic
;; ***	genEndFunction  2829
;;	C$and2.c$53$1$1 ==.
;;	XG$main$0$0 ==.
;return
	RETURN		;key=002,flow seq=007
;<>Start of new flow, seq=0x8
; exit point of _main
	end
