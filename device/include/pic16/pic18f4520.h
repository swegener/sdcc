/* 
 * pic18f4520.h - PIC18F4520 Device Library Header
 * 
 * This file is part of the GNU PIC Library.
 * 
 * May, 2005
 * The GNU PIC Library is maintained by
 *     Raphael Neider <rneider AT web.de>
 * 
 * originally designed by
 *     Vangelis Rokas <vrokas@otenet.gr>
 * 
 * $Id$
 * 
 */

#ifndef __PIC18F4520_H__
#define __PIC18F4520_H__ 1

extern __sfr __at (0xF80) PORTA;
typedef union {
	struct {
		unsigned RA0       	: 1;
		unsigned RA1       	: 1;
		unsigned RA2       	: 1;
		unsigned RA3       	: 1;
		unsigned RA4       	: 1;
		unsigned RA5       	: 1;
		unsigned RA6       	: 1;
		unsigned RA7       	: 1;
	};
	struct {
		unsigned RA        	: 8;
	};
	/* aliases */
	struct {
		unsigned C1INN     	:1;
		unsigned C2INN     	:1;
		unsigned C2INP     	:1;
		unsigned C1INP     	:1;
		unsigned C1OUT     	:1;
		unsigned C2OUT     	:1;
		unsigned OSC2      	:1;
		unsigned OSC1      	:1;
	};
	struct {
		unsigned AN0       	:1;
		unsigned AN1       	:1;
		unsigned AN2       	:1;
		unsigned AN3       	:1;
		unsigned T0CKI     	:1;
		unsigned AN4       	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTA_t;
extern volatile __PORTA_t __at (0xF80) PORTAbits;

extern __sfr __at (0xF81) PORTB;
typedef union {
	struct {
		unsigned RB0       	: 1;
		unsigned RB1       	: 1;
		unsigned RB2       	: 1;
		unsigned RB3       	: 1;
		unsigned RB4       	: 1;
		unsigned RB5       	: 1;
		unsigned RB6       	: 1;
		unsigned RB7       	: 1;
	};
	struct {
		unsigned RB        	: 8;
	};
	/* aliases */
	struct {
		unsigned AN12      	:1;
		unsigned AN10      	:1;
		unsigned AN8       	:1;
		unsigned AN9       	:1;
		unsigned AN11      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTB_t;
extern volatile __PORTB_t __at (0xF81) PORTBbits;

extern __sfr __at (0xF82) PORTC;
typedef union {
	struct {
		unsigned RC0       	: 1;
		unsigned RC1       	: 1;
		unsigned RC2       	: 1;
		unsigned RC3       	: 1;
		unsigned RC4       	: 1;
		unsigned RC5       	: 1;
		unsigned RC6       	: 1;
		unsigned RC7       	: 1;
	};
	struct {
		unsigned RC        	: 8;
	};
	/* aliases */
	struct {
		unsigned T1CKI     	:1;
		unsigned T1OSCI    	:1;
		unsigned ECCPA     	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned U1TX      	:1;
		unsigned U1RX      	:1;
	};
	struct {
		unsigned T3CKI     	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
	struct {
		unsigned T1OSCO    	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTC_t;
extern volatile __PORTC_t __at (0xF82) PORTCbits;

extern __sfr __at (0xF83) PORTD;
typedef union {
	struct {
		unsigned RD0       	: 1;
		unsigned RD1       	: 1;
		unsigned RD2       	: 1;
		unsigned RD3       	: 1;
		unsigned RD4       	: 1;
		unsigned RD5       	: 1;
		unsigned RD6       	: 1;
		unsigned RD7       	: 1;
	};
	struct {
		unsigned RD        	: 8;
	};
	/* aliases */
	struct {
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned ECCPB     	:1;
		unsigned ECCPC     	:1;
		unsigned ECCPD     	:1;
	};
} __PORTD_t;
extern volatile __PORTD_t __at (0xF83) PORTDbits;

extern __sfr __at (0xF84) PORTE;
typedef union {
	struct {
		unsigned RE0       	: 1;
		unsigned RE1       	: 1;
		unsigned RE2       	: 1;
		unsigned RE3       	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
	struct {
		unsigned RE        	: 8;
	};
	/* aliases */
	struct {
		unsigned AN5       	:1;
		unsigned AN6       	:1;
		unsigned AN7       	:1;
		unsigned MCLR      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTE_t;
extern volatile __PORTE_t __at (0xF84) PORTEbits;

extern __sfr __at (0xF89) LATA;
typedef union {
	struct {
		unsigned LATA0     	: 1;
		unsigned LATA1     	: 1;
		unsigned LATA2     	: 1;
		unsigned LATA3     	: 1;
		unsigned LATA4     	: 1;
		unsigned LATA5     	: 1;
		unsigned LATA6     	: 1;
		unsigned LATA7     	: 1;
	};
} __LATA_t;
extern volatile __LATA_t __at (0xF89) LATAbits;

extern __sfr __at (0xF8A) LATB;
typedef union {
	struct {
		unsigned LATB0     	: 1;
		unsigned LATB1     	: 1;
		unsigned LATB2     	: 1;
		unsigned LATB3     	: 1;
		unsigned LATB4     	: 1;
		unsigned LATB5     	: 1;
		unsigned LATB6     	: 1;
		unsigned LATB7     	: 1;
	};
} __LATB_t;
extern volatile __LATB_t __at (0xF8A) LATBbits;

extern __sfr __at (0xF8B) LATC;
typedef union {
	struct {
		unsigned LATC0     	: 1;
		unsigned LATC1     	: 1;
		unsigned LATC2     	: 1;
		unsigned LATC3     	: 1;
		unsigned LATC4     	: 1;
		unsigned LATC5     	: 1;
		unsigned LATC6     	: 1;
		unsigned LATC7     	: 1;
	};
} __LATC_t;
extern volatile __LATC_t __at (0xF8B) LATCbits;

extern __sfr __at (0xF8C) LATD;
typedef union {
	struct {
		unsigned LATD0     	: 1;
		unsigned LATD1     	: 1;
		unsigned LATD2     	: 1;
		unsigned LATD3     	: 1;
		unsigned LATD4     	: 1;
		unsigned LATD5     	: 1;
		unsigned LATD6     	: 1;
		unsigned LATD7     	: 1;
	};
} __LATD_t;
extern volatile __LATD_t __at (0xF8C) LATDbits;

extern __sfr __at (0xF8D) LATE;
typedef union {
	struct {
		unsigned LATE0     	: 1;
		unsigned LATE1     	: 1;
		unsigned LATE2     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __LATE_t;
extern volatile __LATE_t __at (0xF8D) LATEbits;

extern __sfr __at (0xF92) TRISA;
typedef union {
	struct {
		unsigned TRISA0    	: 1;
		unsigned TRISA1    	: 1;
		unsigned TRISA2    	: 1;
		unsigned TRISA3    	: 1;
		unsigned TRISA4    	: 1;
		unsigned TRISA5    	: 1;
		unsigned TRISA6    	: 1;
		unsigned TRISA7    	: 1;
	};
} __TRISA_t;
extern volatile __TRISA_t __at (0xF92) TRISAbits;

extern __sfr __at (0xF93) TRISB;
typedef union {
	struct {
		unsigned TRISB0    	: 1;
		unsigned TRISB1    	: 1;
		unsigned TRISB2    	: 1;
		unsigned TRISB3    	: 1;
		unsigned TRISB4    	: 1;
		unsigned TRISB5    	: 1;
		unsigned TRISB6    	: 1;
		unsigned TRISB7    	: 1;
	};
} __TRISB_t;
extern volatile __TRISB_t __at (0xF93) TRISBbits;

extern __sfr __at (0xF94) TRISC;
typedef union {
	struct {
		unsigned TRISC0    	: 1;
		unsigned TRISC1    	: 1;
		unsigned TRISC2    	: 1;
		unsigned TRISC3    	: 1;
		unsigned TRISC4    	: 1;
		unsigned TRISC5    	: 1;
		unsigned TRISC6    	: 1;
		unsigned TRISC7    	: 1;
	};
} __TRISC_t;
extern volatile __TRISC_t __at (0xF94) TRISCbits;

extern __sfr __at (0xF95) TRISD;
typedef union {
	struct {
		unsigned TRISD0    	: 1;
		unsigned TRISD1    	: 1;
		unsigned TRISD2    	: 1;
		unsigned TRISD3    	: 1;
		unsigned TRISD4    	: 1;
		unsigned TRISD5    	: 1;
		unsigned TRISD6    	: 1;
		unsigned TRISD7    	: 1;
	};
} __TRISD_t;
extern volatile __TRISD_t __at (0xF95) TRISDbits;

extern __sfr __at (0xF96) TRISE;
typedef union {
	struct {
		unsigned TRISE0    	: 1;
		unsigned TRISE1    	: 1;
		unsigned TRISE2    	: 1;
		unsigned           	: 1;
		unsigned PSPMODE   	: 1;
		unsigned IBOV      	: 1;
		unsigned OBF       	: 1;
		unsigned IBF       	: 1;
	};
} __TRISE_t;
extern volatile __TRISE_t __at (0xF96) TRISEbits;

extern __sfr __at (0xF9B) OSCTUNE;
typedef union {
	struct {
		unsigned TUN       	: 5;
		unsigned           	: 1;
		unsigned PLLEN     	: 1;
		unsigned HF256DIV  	: 1;
	};
} __OSCTUNE_t;
extern volatile __OSCTUNE_t __at (0xF9B) OSCTUNEbits;

extern __sfr __at (0xF9D) PIE1;
typedef union {
	struct {
		unsigned TMR1IE    	: 1;
		unsigned TMR2IE    	: 1;
		unsigned CCP1IE    	: 1;
		unsigned SSPIE     	: 1;
		unsigned TXIE      	: 1;
		unsigned RCIE      	: 1;
		unsigned ADIE      	: 1;
		unsigned PSPIE     	: 1;
	};
} __PIE1_t;
extern volatile __PIE1_t __at (0xF9D) PIE1bits;

extern __sfr __at (0xF9E) PIR1;
typedef union {
	struct {
		unsigned TMR1IF    	: 1;
		unsigned TMR2IF    	: 1;
		unsigned CCP1IF    	: 1;
		unsigned SSPIF     	: 1;
		unsigned TXIF      	: 1;
		unsigned RCIF      	: 1;
		unsigned ADIF      	: 1;
		unsigned PSPIF     	: 1;
	};
} __PIR1_t;
extern volatile __PIR1_t __at (0xF9E) PIR1bits;

extern __sfr __at (0xF9F) IPR1;
typedef union {
	struct {
		unsigned TMR1IP    	: 1;
		unsigned TMR2IP    	: 1;
		unsigned CCP1IP    	: 1;
		unsigned SSPIP     	: 1;
		unsigned TXIP      	: 1;
		unsigned RCIP      	: 1;
		unsigned ADIP      	: 1;
		unsigned PSPIP     	: 1;
	};
} __IPR1_t;
extern volatile __IPR1_t __at (0xF9F) IPR1bits;

extern __sfr __at (0xFA0) PIE2;
typedef union {
	struct {
		unsigned CCP2IE    	: 1;
		unsigned TMR3IE    	: 1;
		unsigned LVDIE     	: 1;
		unsigned BCLIE     	: 1;
		unsigned EEIE      	: 1;
		unsigned           	: 1;
		unsigned CMIE      	: 1;
		unsigned OSCFIE    	: 1;
	};
} __PIE2_t;
extern volatile __PIE2_t __at (0xFA0) PIE2bits;

extern __sfr __at (0xFA1) PIR2;
typedef union {
	struct {
		unsigned CCP2IF    	: 1;
		unsigned TMR3IF    	: 1;
		unsigned LVDIF     	: 1;
		unsigned BCLIF     	: 1;
		unsigned EEIF      	: 1;
		unsigned           	: 1;
		unsigned CMIF      	: 1;
		unsigned OSCFIF    	: 1;
	};
} __PIR2_t;
extern volatile __PIR2_t __at (0xFA1) PIR2bits;

extern __sfr __at (0xFA2) IPR2;
typedef union {
	struct {
		unsigned CCP2IP    	: 1;
		unsigned TMR3IP    	: 1;
		unsigned LVDIP     	: 1;
		unsigned BCLIP     	: 1;
		unsigned EEIP      	: 1;
		unsigned           	: 1;
		unsigned CMIP      	: 1;
		unsigned OSCFIP    	: 1;
	};
} __IPR2_t;
extern volatile __IPR2_t __at (0xFA2) IPR2bits;

extern __sfr __at (0xFA6) EECON1;
typedef union {
	struct {
		unsigned RD        	: 1;
		unsigned WR        	: 1;
		unsigned WREN      	: 1;
		unsigned WRERR     	: 1;
		unsigned FREE      	: 1;
		unsigned           	: 1;
		unsigned CFGS      	: 1;
		unsigned EEPGD     	: 1;
	};
} __EECON1_t;
extern volatile __EECON1_t __at (0xFA6) EECON1bits;

extern __sfr __at (0xFA7) EECON2;

extern __sfr __at (0xFA8) EEDATA;

extern __sfr __at (0xFA9) EEADR;

extern __sfr __at (0xFAB) RCSTA;
typedef union {
	struct {
		unsigned RX9D      	: 1;
		unsigned OERR      	: 1;
		unsigned FERR      	: 1;
		unsigned ADDEN     	: 1;
		unsigned CREN      	: 1;
		unsigned SREN      	: 1;
		unsigned RX9       	: 1;
		unsigned SPEN      	: 1;
	};
} __RCSTA_t;
extern volatile __RCSTA_t __at (0xFAB) RCSTAbits;

extern __sfr __at (0xFAC) TXSTA;
typedef union {
	struct {
		unsigned TX9D      	: 1;
		unsigned TRMT      	: 1;
		unsigned BRGH      	: 1;
		unsigned SENDB     	: 1;
		unsigned SYNC      	: 1;
		unsigned TXEN      	: 1;
		unsigned TX9       	: 1;
		unsigned CSRC      	: 1;
	};
} __TXSTA_t;
extern volatile __TXSTA_t __at (0xFAC) TXSTAbits;

extern __sfr __at (0xFAD) TXREG;

extern __sfr __at (0xFAE) RCREG;

extern __sfr __at (0xFAF) SPBRG;

extern __sfr __at (0xFB0) SPBRGH;

extern __sfr __at (0xFB1) T3CON;
typedef union {
	struct {
		unsigned TMR3ON    	: 1;
		unsigned TMR3CS    	: 1;
		unsigned NOT_T3SYNC	: 1;
		unsigned T3CCP1    	: 1;
		unsigned T3CKPS0    	: 1;
		unsigned T3CKPS1    	: 1;
		unsigned T3CCP2    	: 1;
		unsigned RD16      	: 1;
	};
} __T3CON_t;
extern volatile __T3CON_t __at (0xFB1) T3CONbits;

extern __sfr __at (0xFB2) TMR3L;

extern __sfr __at (0xFB3) TMR3H;

extern __sfr __at (0xFB4) CMCON;
typedef union {
	struct {
		unsigned CM0       	: 1;
		unsigned CM1       	: 1;
		unsigned CM2       	: 1;
		unsigned CIS       	: 1;
		unsigned C1INV     	: 1;
		unsigned C2INV     	: 1;
		unsigned C1OUT     	: 1;
		unsigned C2OUT     	: 1;
	};
} __CMCON_t;
extern volatile __CMCON_t __at (0xFB4) CMCONbits;

extern __sfr __at (0xFB5) CVRCON;
typedef union {
	struct {
		unsigned CVR0      	: 1;
		unsigned CVR1      	: 1;
		unsigned CVR2      	: 1;
		unsigned CVR3      	: 1;
		unsigned CVRSS     	: 1;
		unsigned CVRR      	: 1;
		unsigned CVROE     	: 1;
		unsigned CVREN     	: 1;
	};
} __CVRCON_t;
extern volatile __CVRCON_t __at (0xFB5) CVRCONbits;

extern __sfr __at (0xFB6) ECCPAS1;
typedef union {
	struct {
		unsigned PSSBD0    	: 1;
		unsigned PSSBD1    	: 1;
		unsigned PSSAC0    	: 1;
		unsigned PSSAC1    	: 1;
		unsigned ECCPAS0   	: 1;
		unsigned ECCPAS1   	: 1;
		unsigned ECCPAS2   	: 1;
		unsigned ECCPASE   	: 1;
	};
} __ECCPAS1_t;
extern volatile __ECCPAS1_t __at (0xFB6) ECCPAS1bits;

extern __sfr __at (0xFB7) PWM1CON;
typedef union {
	struct {
		unsigned PDC       	: 7;
		unsigned PRSEN     	: 1;
	};
} __PWM1CON_t;
extern volatile __PWM1CON_t __at (0xFB7) PWM1CONbits;

extern __sfr __at (0xFB8) BAUDCON;
typedef union {
	struct {
		unsigned ABDEN     	: 1;
		unsigned WUE       	: 1;
		unsigned           	: 1;
		unsigned BRG16     	: 1;
		unsigned SCKP      	: 1;
		unsigned           	: 1;
		unsigned RCMT      	: 1;
		unsigned ABDOVF    	: 1;
	};
} __BAUDCON_t;
extern volatile __BAUDCON_t __at (0xFB8) BAUDCONbits;

extern __sfr __at (0xFBA) CCP2CON;
typedef union {
	struct {
		unsigned CCP2M0    	: 1;
		unsigned CCP2M1    	: 1;
		unsigned CCP2M2    	: 1;
		unsigned CCP2M3    	: 1;
		unsigned DC2B0     	: 1;
		unsigned DC2B1     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __CCP2CON_t;
extern volatile __CCP2CON_t __at (0xFBA) CCP2CONbits;

extern __sfr __at (0xFBB) CCPR2L;

extern __sfr __at (0xFBC) CCPR2H;

extern __sfr __at (0xFBD) ECCP1CON;
typedef union {
	struct {
		unsigned CCP1M0    	: 1;
		unsigned CCP1M1    	: 1;
		unsigned CCP1M2    	: 1;
		unsigned CCP1M3    	: 1;
		unsigned DC1B0     	: 1;
		unsigned DC1B1     	: 1;
		unsigned P1M0      	: 1;
		unsigned P1M1      	: 1;
	};
} __ECCP1CON_t;
extern volatile __ECCP1CON_t __at (0xFBD) ECCP1CONbits;

extern __sfr __at (0xFBE) CCPR1L;

extern __sfr __at (0xFBF) CCPR1H;

extern __sfr __at (0xFC0) ADCON2;
typedef union {
	struct {
		unsigned ADCS0     	: 1;
		unsigned ADCS1     	: 1;
		unsigned ADCS2     	: 1;
		unsigned ACQT0     	: 1;
		unsigned ACQT1     	: 1;
		unsigned ACQT2     	: 1;
		unsigned           	: 1;
		unsigned ADFM      	: 1;
	};
} __ADCON2_t;
extern volatile __ADCON2_t __at (0xFC0) ADCON2bits;

extern __sfr __at (0xFC1) ADCON1;
typedef union {
	struct {
		unsigned PCFG0     	: 1;
		unsigned PCFG1     	: 1;
		unsigned PCFG2     	: 1;
		unsigned PCFG3     	: 1;
		unsigned VCFG0     	: 1;
		unsigned VCFG1     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __ADCON1_t;
extern volatile __ADCON1_t __at (0xFC1) ADCON1bits;

extern __sfr __at (0xFC2) ADCON0;
typedef union {
	struct {
		unsigned ADON      	: 1;
		unsigned GO       	: 1;
		unsigned CHS0       	: 1;
		unsigned CHS1       	: 1;
		unsigned CHS2       	: 1;
		unsigned CHS3       	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __ADCON0_t;
extern volatile __ADCON0_t __at (0xFC2) ADCON0bits;

extern __sfr __at (0xFC3) ADRESL;

extern __sfr __at (0xFC4) ADRESH;

extern __sfr __at (0xFC5) SSPCON2;
typedef union {
	struct {
		unsigned SEN       	: 1;
		unsigned RSEN      	: 1;
		unsigned PEN       	: 1;
		unsigned RCEN      	: 1;
		unsigned ACKEN     	: 1;
		unsigned ACKDT     	: 1;
		unsigned ACKSTAT   	: 1;
		unsigned GCEN      	: 1;
	};
} __SSPCON2_t;
extern volatile __SSPCON2_t __at (0xFC5) SSPCON2bits;

extern __sfr __at (0xFC6) SSPCON1;
typedef union {
	struct {
		unsigned SSPM0      	: 1;
		unsigned SSPM1      	: 1;
		unsigned SSPM2      	: 1;
		unsigned SSPM3      	: 1;
		unsigned CKP       	: 1;
		unsigned SSPEN     	: 1;
		unsigned SSPOV     	: 1;
		unsigned WCOL      	: 1;
	};
} __SSPCON1_t;
extern volatile __SSPCON1_t __at (0xFC6) SSPCON1bits;

extern __sfr __at (0xFC7) SSPSTAT;
typedef union {
	struct {
		unsigned BF        	: 1;
		unsigned UA        	: 1;
		unsigned R_W      	: 1;
		unsigned S         	: 1;
		unsigned P         	: 1;
		unsigned D_A      	: 1;
		unsigned CKE       	: 1;
		unsigned SMP       	: 1;
	};
} __SSPSTAT_t;
extern volatile __SSPSTAT_t __at (0xFC7) SSPSTATbits;

extern __sfr __at (0xFC8) SSPADD;

extern __sfr __at (0xFC9) SSPBUF;

extern __sfr __at (0xFCA) T2CON;
typedef union {
	struct {
		unsigned T2CKPS0   	: 1;
		unsigned T2CKPS1   	: 1;
		unsigned TMR2ON    	: 1;
		unsigned TOUTPS0   	: 1;
		unsigned TOUTPS1   	: 1;
		unsigned TOUTPS2   	: 1;
		unsigned TOUTPS3   	: 1;
		unsigned           	: 1;
	};
} __T2CON_t;
extern volatile __T2CON_t __at (0xFCA) T2CONbits;

extern __sfr __at (0xFCB) PR2;

extern __sfr __at (0xFCC) TMR2;

extern __sfr __at (0xFCD) T1CON;
typedef union {
	struct {
		unsigned TMR1ON    	: 1;
		unsigned TMR1CS    	: 1;
		unsigned NOT_T1SYNC	: 1;
		unsigned T1OSCEN   	: 1;
		unsigned T1CKPS0   	: 1;
		unsigned T1CKPS1   	: 1;
		unsigned T1RUN     	: 1;
		unsigned RD16      	: 1;
	};
} __T1CON_t;
extern volatile __T1CON_t __at (0xFCD) T1CONbits;

extern __sfr __at (0xFCE) TMR1L;

extern __sfr __at (0xFCF) TMR1H;

extern __sfr __at (0xFD0) RCON;
typedef union {
	struct {
		unsigned BOR      	: 1;
		unsigned POR      	: 1;
		unsigned PD       	: 1;
		unsigned TO       	: 1;
		unsigned RI       	: 1;
		unsigned           	: 1;
		unsigned SBOREN    	: 1;
		unsigned IPEN      	: 1;
	};
} __RCON_t;
extern volatile __RCON_t __at (0xFD0) RCONbits;

extern __sfr __at (0xFD1) WDTCON;
typedef union {
	struct {
		unsigned SWDTEN    	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __WDTCON_t;
extern volatile __WDTCON_t __at (0xFD1) WDTCONbits;

extern __sfr __at (0xFD2) LVDCON;
typedef union {
	struct {
		unsigned LVDL0     	: 1;
		unsigned LVDL1     	: 1;
		unsigned LVDL2     	: 1;
		unsigned LVDL3     	: 1;
		unsigned LVDEN     	: 1;
		unsigned IRVST     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __LVDCON_t;
extern volatile __LVDCON_t __at (0xFD2) LVDCONbits;

extern __sfr __at (0xFD3) OSCCON;
typedef union {
	struct {
		unsigned SCS       	: 2;
		unsigned FLTS      	: 1;
		unsigned OSTS      	: 1;
		unsigned IRCF      	: 3;
		unsigned IDLEN     	: 1;
	};
} __OSCCON_t;
extern volatile __OSCCON_t __at (0xFD3) OSCCONbits;

extern __sfr __at (0xFD5) T0CON;
typedef union {
	struct {
		unsigned T0PS0     	: 1;
		unsigned T0PS1     	: 1;
		unsigned T0PS2     	: 1;
		unsigned PSA       	: 1;
		unsigned T0SE      	: 1;
		unsigned T0CS      	: 1;
		unsigned T08BIT    	: 1;
		unsigned TMR0ON    	: 1;
	};
} __T0CON_t;
extern volatile __T0CON_t __at (0xFD5) T0CONbits;

extern __sfr __at (0xFD6) TMR0L;

extern __sfr __at (0xFD7) TMR0H;

extern __sfr __at (0xFD8) STATUS;
typedef union {
	struct {
		unsigned C         	: 1;
		unsigned DC        	: 1;
		unsigned Z         	: 1;
		unsigned OV        	: 1;
		unsigned N         	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __STATUS_t;
extern volatile __STATUS_t __at (0xFD8) STATUSbits;

extern __sfr __at (0xFD9) FSR2L;

extern __sfr __at (0xFDA) FSR2H;
typedef union {
	struct {
		unsigned FSR2H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR2H_t;
extern volatile __FSR2H_t __at (0xFDA) FSR2Hbits;

extern __sfr __at (0xFDB) PLUSW2;

extern __sfr __at (0xFDC) PREINC2;

extern __sfr __at (0xFDD) POSTDEC2;

extern __sfr __at (0xFDE) POSTINC2;

extern __sfr __at (0xFDF) INDF2;

extern __sfr __at (0xFE0) BSR;
typedef union {
	struct {
		unsigned BSR       	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __BSR_t;
extern volatile __BSR_t __at (0xFE0) BSRbits;

extern __sfr __at (0xFE1) FSR1L;

extern __sfr __at (0xFE2) FSR1H;
typedef union {
	struct {
		unsigned FSR1H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR1H_t;
extern volatile __FSR1H_t __at (0xFE2) FSR1Hbits;

extern __sfr __at (0xFE3) PLUSW1;

extern __sfr __at (0xFE4) PREINC1;

extern __sfr __at (0xFE5) POSTDEC1;

extern __sfr __at (0xFE6) POSTINC1;

extern __sfr __at (0xFE7) INDF1;

extern __sfr __at (0xFE8) WREG;

extern __sfr __at (0xFE9) FSR0L;

extern __sfr __at (0xFEA) FSR0H;
typedef union {
	struct {
		unsigned FSR0H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR0H_t;
extern volatile __FSR0H_t __at (0xFEA) FSR0Hbits;

extern __sfr __at (0xFEB) PLUSW0;

extern __sfr __at (0xFEC) PREINC0;

extern __sfr __at (0xFED) POSTDEC0;

extern __sfr __at (0xFEE) POSTINC0;

extern __sfr __at (0xFEF) INDF0;

extern __sfr __at (0xFF0) INTCON3;
typedef union {
	struct {
		unsigned INT1IF    	: 1;
		unsigned INT2IF    	: 1;
		unsigned           	: 1;
		unsigned INT1IE    	: 1;
		unsigned INT2IE    	: 1;
		unsigned           	: 1;
		unsigned INT1IP    	: 1;
		unsigned INT2IP    	: 1;
	};
} __INTCON3_t;
extern volatile __INTCON3_t __at (0xFF0) INTCON3bits;

extern __sfr __at (0xFF1) INTCON2;
typedef union {
	struct {
		unsigned RBIP      	: 1;
		unsigned           	: 1;
		unsigned TMR0IP    	: 1;
		unsigned           	: 1;
		unsigned INTEDG2   	: 1;
		unsigned INTEDG1   	: 1;
		unsigned INTEDG0   	: 1;
		unsigned           	: 1;
	};
} __INTCON2_t;
extern volatile __INTCON2_t __at (0xFF1) INTCON2bits;

extern __sfr __at (0xFF2) INTCON;
typedef union {
	struct {
		unsigned RBIF      	: 1;
		unsigned INT0IF    	: 1;
		unsigned TMR0IF    	: 1;
		unsigned RBIE      	: 1;
		unsigned INT0IE    	: 1;
		unsigned TMR0IE    	: 1;
		unsigned PEIE      	: 1;
		unsigned GIE       	: 1;
	};
	struct {
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned GIEL      	: 1;
		unsigned GIEH      	: 1;
	};
} __INTCON_t;
extern volatile __INTCON_t __at (0xFF2) INTCONbits;

extern __sfr __at (0xFF3) PRODL;

extern __sfr __at (0xFF4) PRODH;

extern __sfr __at (0xFF5) TABLAT;

extern __sfr __at (0xFF6) TBLPTRL;

extern __sfr __at (0xFF7) TBLPTRH;

extern __sfr __at (0xFF8) TBLPTRU;
typedef union {
	struct {
		unsigned TBLPTRU   	: 5;
		unsigned ACSS      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __TBLPTRU_t;
extern volatile __TBLPTRU_t __at (0xFF8) TBLPTRUbits;

extern __sfr __at (0xFF9) PCL;

extern __sfr __at (0xFFA) PCLATH;
typedef union {
	struct {
		unsigned PCH       	: 8;
	};
} __PCLATH_t;
extern volatile __PCLATH_t __at (0xFFA) PCLATHbits;

extern __sfr __at (0xFFB) PCLATU;
typedef union {
	struct {
		unsigned PCU       	: 5;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PCLATU_t;
extern volatile __PCLATU_t __at (0xFFB) PCLATUbits;

extern __sfr __at (0xFFC) STKPTR;
typedef union {
	struct {
		unsigned STKPTR    	: 5;
		unsigned           	: 1;
		unsigned STKUNF    	: 1;
		unsigned STKFUL    	: 1;
	};
} __STKPTR_t;
extern volatile __STKPTR_t __at (0xFFC) STKPTRbits;

extern __sfr __at (0xFFD) TOSL;

extern __sfr __at (0xFFE) TOSH;

extern __sfr __at (0xFFF) TOSU;
typedef union {
	struct {
		unsigned TOSU      	: 5;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __TOSU_t;
extern volatile __TOSU_t __at (0xFFF) TOSUbits;

/* Configuration register locations */
#define	__CONFIG1H	0x300001
#define	__CONFIG2L	0x300002
#define	__CONFIG2H	0x300003
#define	__CONFIG3H	0x300005
#define	__CONFIG4L	0x300006
#define	__CONFIG5L	0x300008
#define	__CONFIG5H	0x300009
#define	__CONFIG6L	0x30000A
#define	__CONFIG6H	0x30000B
#define	__CONFIG7L	0x30000C
#define	__CONFIG7H	0x30000D


/* Oscillator 1H options */
#define	_OSC_11XX_EXT_RC_CLKOUT_ON_RA6_1H       	0xFC	/* 11XX EXT RC-CLKOUT on RA6 */
#define	_OSC_101X_EXT_RC_CLKOUT_ON_RA6_1H       	0xFA	/* 101X EXT RC-CLKOUT on RA6 */
#define	_OSC_INT_RC_CLKOUT_ON_RA6_PORT_ON_RA7_1H	0xF9	/* INT RC-CLKOUT on RA6,Port on RA7 */
#define	_OSC_INT_RC_PORT_ON_RA6_PORT_ON_RA7_1H  	0xF8	/* INT RC-Port on RA6,Port on RA7 */
#define	_OSC_EXT_RC_PORT_ON_RA6_1H              	0xF7	/* EXT RC-Port on RA6 */
#define	_OSC_HS_PLL_ON_FREQ_4XFOSC1_1H          	0xF6	/* HS-PLL enabled freq=4xFosc1 */
#define	_OSC_EC_PORT_ON_RA6_1H                  	0xF5	/* EC-Port on RA6 */
#define	_OSC_EC_CLKOUT_ON_RA6_1H                	0xF4	/* EC-CLKOUT on RA6 */
#define	_OSC_0011_EXT_RC_CLKOUT_ON_RA6_1H       	0xF3	/* 0011 EXT RC-CLKOUT on RA6 */
#define	_OSC_HS_1H                              	0xF2	/* HS */
#define	_OSC_XT_1H                              	0xF1	/* XT */
#define	_OSC_LP_1H                              	0xF0	/* LP */

/* Fail-Safe Clock Monitor Enable 1H options */
#define	_FCMEN_OFF_1H                           	0xBF	/* Disabled */
#define	_FCMEN_ON_1H                            	0xFF	/* Enabled */

/* Internal External Switch Over Mode 1H options */
#define	_IESO_OFF_1H                            	0x7F	/* Disabled */
#define	_IESO_ON_1H                             	0xFF	/* Enabled */


/* Power Up Timer 2L options */
#define	_PUT_OFF_2L                             	0xFF	/* Disabled */
#define	_PUT_ON_2L                              	0xFE	/* Enabled */

/* Brown Out Detect 2L options */
#define	_BODEN_ON_2L                            	0xFF	/* Enabled in hardware, SBOREN disabled */
#define	_BODEN_ON_WHILE_ACTIVE_2L               	0xFD	/* Enabled while active,disabled in SLEEP,SBOREN disabled */
#define	_BODEN_CONTROLLED_WITH_SBOREN_BIT_2L    	0xFB	/* Controlled with SBOREN bit */
#define	_BODEN_OFF_2L                           	0xF9	/* Disabled in hardware, SBOREN disabled */

/* Brown Out Voltage 2L options */
#define	_BODENV_2_0V_2L                         	0xFF	/* 2.0V */
#define	_BODENV_2_7V_2L                         	0xF7	/* 2.7V */
#define	_BODENV_4_2V_2L                         	0xEF	/* 4.2V */
#define	_BODENV_4_5V_2L                         	0xE7	/* 4.5V */


/* Watchdog Timer 2H options */
#define	_WDT_ON_2H                              	0xFF	/* Enabled */
#define	_WDT_DISABLED_CONTROLLED_2H             	0xFE	/* Disabled-Controlled by SWDTEN bit */

/* Watchdog Postscaler 2H options */
#define	_WDTPS_1_32768_2H                       	0xFF	/* 1:32768 */
#define	_WDTPS_1_16384_2H                       	0xFD	/* 1:16384 */
#define	_WDTPS_1_8192_2H                        	0xFB	/* 1:8192 */
#define	_WDTPS_1_4096_2H                        	0xF9	/* 1:4096 */
#define	_WDTPS_1_2048_2H                        	0xF7	/* 1:2048 */
#define	_WDTPS_1_1024_2H                        	0xF5	/* 1:1024 */
#define	_WDTPS_1_512_2H                         	0xF3	/* 1:512 */
#define	_WDTPS_1_256_2H                         	0xF1	/* 1:256 */
#define	_WDTPS_1_128_2H                         	0xEF	/* 1:128 */
#define	_WDTPS_1_64_2H                          	0xED	/* 1:64 */
#define	_WDTPS_1_32_2H                          	0xEB	/* 1:32 */
#define	_WDTPS_1_16_2H                          	0xE9	/* 1:16 */
#define	_WDTPS_1_8_2H                           	0xE7	/* 1:8 */
#define	_WDTPS_1_4_2H                           	0xE5	/* 1:4 */
#define	_WDTPS_1_2_2H                           	0xE3	/* 1:2 */
#define	_WDTPS_1_1_2H                           	0xE1	/* 1:1 */


/* CCP2 Mux 3H options */
#define	_CCP2MUX_RC1_3H                         	0xFF	/* RC1 */
#define	_CCP2MUX_RB3_3H                         	0xFE	/* RB3 */

/* PortB A/D Enable 3H options */
#define	_PBADEN_PORTB_4_0__CONFIGURED_AS_ANALOG_INPUTS_ON_RESET_3H	0xFF	/* PORTB<4:0> configured as analog inputs on RESET */
#define	_PBADEN_PORTB_4_0__CONFIGURED_AS_DIGITAL_I_O_ON_RESET_3H	0xFD	/* PORTB<4:0> configured as digital I/O on RESET */

/* Low Power Timer1 Osc enable 3H options */
#define	_LPT1OSC_ON_3H                          	0xFF	/* Enabled */
#define	_LPT1OSC_OFF_3H                         	0xFB	/* Disabled */

/* Master Clear Enable 3H options */
#define	_MCLRE_MCLR_ON_RE3_OFF_3H               	0xFF	/* MCLR Enabled,RE3 Disabled */
#define	_MCLRE_MCLR_OFF_RE3_ON_3H               	0x7F	/* MCLR Disabled,RE3 Enabled */


/* Stack Overflow Reset 4L options */
#define	_STVR_ON_4L                             	0xFF	/* Enabled */
#define	_STVR_OFF_4L                            	0xFE	/* Disabled */

/* Low Voltage Program 4L options */
#define	_LVP_ON_4L                              	0xFF	/* Enabled */
#define	_LVP_OFF_4L                             	0xFB	/* Disabled */

/* Extended CPU Enable 4L options */
#define	_ENHCPU_ON_4L                           	0xFF	/* Enabled */
#define	_ENHCPU_OFF_4L                          	0xBF	/* Disabled */

/* Background Debug 4L options */
#define	_BACKBUG_OFF_4L                         	0xFF	/* Disabled */
#define	_BACKBUG_ON_4L                          	0x7F	/* Enabled */


/* Code Protect 00800-01FFF 5L options */
#define	_CP_0_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_0_ON_5L                             	0xFE	/* Enabled */

/* Code Protect 02000-03FFF 5L options */
#define	_CP_1_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_1_ON_5L                             	0xFD	/* Enabled */

/* Code Protect 04000-05FFF 5L options */
#define	_CP_2_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_2_ON_5L                             	0xFB	/* Enabled */

/* Code Protect 06000-07FFF 5L options */
#define	_CP_3_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_3_ON_5L                             	0xF7	/* Enabled */


/* Data EEPROM Code Protect 5H options */
#define	_CPD_OFF_5H                             	0xFF	/* Disabled */
#define	_CPD_ON_5H                              	0x7F	/* Enabled */

/* Code Protect Boot 5H options */
#define	_CPB_OFF_5H                             	0xFF	/* Disabled */
#define	_CPB_ON_5H                              	0xBF	/* Enabled */


/* Table Write Protect 00800-01FFF 6L options */
#define	_WRT_0_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_0_ON_6L                            	0xFE	/* Enabled */

/* Table Write Protect 02000-03FFF 6L options */
#define	_WRT_1_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_1_ON_6L                            	0xFD	/* Enabled */

/* Table Write Protect 04000-05FFF 6L options */
#define	_WRT_2_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_2_ON_6L                            	0xFB	/* Enabled */

/* Table Write Protect 06000-07FFF 6L options */
#define	_WRT_3_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_3_ON_6L                            	0xF7	/* Enabled */


/* Data EEPROM Write Protect 6H options */
#define	_WRTD_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTD_ON_6H                             	0x7F	/* Enabled */

/* Table Write Protect Boot 6H options */
#define	_WRTB_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTB_ON_6H                             	0xBF	/* Enabled */

/* Config. Write Protect 6H options */
#define	_WRTC_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTC_ON_6H                             	0xDF	/* Enabled */


/* Table Read Protect 00800-01FFF 7L options */
#define	_EBTR_0_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_0_ON_7L                           	0xFE	/* Enabled */

/* Table Read Protect 02000-03FFF 7L options */
#define	_EBTR_1_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_1_ON_7L                           	0xFD	/* Enabled */

/* Table Read Protect 04000-05FFF 7L options */
#define	_EBTR_2_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_2_ON_7L                           	0xFB	/* Enabled */

/* Table Read Protect 06000-07FFF 7L options */
#define	_EBTR_3_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_3_ON_7L                           	0xF7	/* Enabled */


/* Table Read Protect Boot 7H options */
#define	_EBTRB_OFF_7H                           	0xFF	/* Disabled */
#define	_EBTRB_ON_7H                            	0xBF	/* Enabled */



/* Location of User ID words */
#define	__IDLOC0	0x200000
#define	__IDLOC1	0x200001
#define	__IDLOC2	0x200002
#define	__IDLOC3	0x200003
#define	__IDLOC4	0x200004
#define	__IDLOC5	0x200005
#define	__IDLOC6	0x200006
#define	__IDLOC7	0x200007

#endif // __PIC18F4520__
