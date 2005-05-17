/* 
 * pic18f4331.h - PIC18F4331 Device Library Header
 * 
 * This file is part of the GNU PIC Library.
 * 
 * May, 2005
 * The GNU PIC Library is maintained by
 *     Raphael Neider <rneider@web.de>
 * 
 * originally designed by
 *     Vangelis Rokas <vrokas@otenet.gr>
 * 
 * $Id$
 * 
 */

#ifndef __PIC18F4331_H__
#define __PIC18F4331_H__ 1

extern __sfr __at 0xF60 DFLTCON;
typedef union {
	struct {
		unsigned FLTCK     	: 3;
		unsigned FLT1EN    	: 1;
		unsigned FLT2EN    	: 1;
		unsigned FLT3EN    	: 1;
		unsigned FLT4EN    	: 1;
		unsigned           	: 1;
	};
} __DFLTCON_t;
extern volatile __DFLTCON_t __at 0xF60 DFLTCONbits;

extern __sfr __at 0xF61 CAP3CON;
typedef union {
	struct {
		unsigned CAP3M     	: 4;
		unsigned           	: 1;
		unsigned CAP3TMR   	: 1;
		unsigned CAP3REN   	: 1;
		unsigned           	: 1;
	};
} __CAP3CON_t;
extern volatile __CAP3CON_t __at 0xF61 CAP3CONbits;

extern __sfr __at 0xF62 CAP2CON;
typedef union {
	struct {
		unsigned CAP2M     	: 4;
		unsigned           	: 1;
		unsigned CAP2TMR   	: 1;
		unsigned CAP2REN   	: 1;
		unsigned           	: 1;
	};
} __CAP2CON_t;
extern volatile __CAP2CON_t __at 0xF62 CAP2CONbits;

extern __sfr __at 0xF63 CAP1CON;
typedef union {
	struct {
		unsigned CAP1M     	: 4;
		unsigned           	: 1;
		unsigned CAP1TMR   	: 1;
		unsigned CAP1REN   	: 1;
		unsigned           	: 1;
	};
} __CAP1CON_t;
extern volatile __CAP1CON_t __at 0xF63 CAP1CONbits;

extern __sfr __at 0xF64 CAP3BUFL;

extern __sfr __at 0xF65 CAP3BUFH;

extern __sfr __at 0xF66 CAP2BUFL;

extern __sfr __at 0xF67 CAP2BUFH;

extern __sfr __at 0xF68 CAP1BUFL;

extern __sfr __at 0xF69 CAP1BUFH;

extern __sfr __at 0xF6A OVDCONS;
typedef union {
	struct {
		unsigned POUT      	: 8;
	};
} __OVDCONS_t;
extern volatile __OVDCONS_t __at 0xF6A OVDCONSbits;

extern __sfr __at 0xF6B OVDCOND;
typedef union {
	struct {
		unsigned POVD      	: 8;
	};
} __OVDCOND_t;
extern volatile __OVDCOND_t __at 0xF6B OVDCONDbits;

extern __sfr __at 0xF6C FLTCONFIG;
typedef union {
	struct {
		unsigned FLTAEN    	: 1;
		unsigned FLTAMOD   	: 1;
		unsigned FLTAS     	: 1;
		unsigned FLTCON    	: 1;
		unsigned FLTBEN    	: 1;
		unsigned FLTBMOD   	: 1;
		unsigned FLTBS     	: 1;
		unsigned           	: 1;
	};
} __FLTCONFIG_t;
extern volatile __FLTCONFIG_t __at 0xF6C FLTCONFIGbits;

extern __sfr __at 0xF6D DTCON;
typedef union {
	struct {
		unsigned DTA       	: 6;
		unsigned DTAPS     	: 2;
	};
} __DTCON_t;
extern volatile __DTCON_t __at 0xF6D DTCONbits;

extern __sfr __at 0xF6E PWMCON1;
typedef union {
	struct {
		unsigned OSYNC     	: 1;
		unsigned UDIS      	: 1;
		unsigned           	: 1;
		unsigned SEVTDIR   	: 1;
		unsigned SEVOPS    	: 4;
	};
} __PWMCON1_t;
extern volatile __PWMCON1_t __at 0xF6E PWMCON1bits;

extern __sfr __at 0xF6F PWMCON0;
typedef union {
	struct {
		unsigned PMOD      	: 4;
		unsigned PWMEN     	: 3;
		unsigned           	: 1;
	};
} __PWMCON0_t;
extern volatile __PWMCON0_t __at 0xF6F PWMCON0bits;

extern __sfr __at 0xF70 SEVTCMPH;
typedef union {
	struct {
		unsigned SEVTCMPH  	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __SEVTCMPH_t;
extern volatile __SEVTCMPH_t __at 0xF70 SEVTCMPHbits;

extern __sfr __at 0xF71 SEVTCMPL;

extern __sfr __at 0xF72 PDC3H;
typedef union {
	struct {
		unsigned PDC3H     	: 6;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PDC3H_t;
extern volatile __PDC3H_t __at 0xF72 PDC3Hbits;

extern __sfr __at 0xF73 PDC3L;

extern __sfr __at 0xF74 PDC2H;
typedef union {
	struct {
		unsigned PDC2H     	: 6;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PDC2H_t;
extern volatile __PDC2H_t __at 0xF74 PDC2Hbits;

extern __sfr __at 0xF75 PDC2L;

extern __sfr __at 0xF76 PDC1H;
typedef union {
	struct {
		unsigned PDC1H     	: 6;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PDC1H_t;
extern volatile __PDC1H_t __at 0xF76 PDC1Hbits;

extern __sfr __at 0xF77 PDC1L;

extern __sfr __at 0xF78 PDC0H;
typedef union {
	struct {
		unsigned PDC0H     	: 6;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PDC0H_t;
extern volatile __PDC0H_t __at 0xF78 PDC0Hbits;

extern __sfr __at 0xF79 PDC0L;

extern __sfr __at 0xF7A PTPERH;
typedef union {
	struct {
		unsigned PTPERH    	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PTPERH_t;
extern volatile __PTPERH_t __at 0xF7A PTPERHbits;

extern __sfr __at 0xF7B PTPERL;

extern __sfr __at 0xF7C PTMRH;
typedef union {
	struct {
		unsigned PTMRH     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PTMRH_t;
extern volatile __PTMRH_t __at 0xF7C PTMRHbits;

extern __sfr __at 0xF7D PTMRL;

extern __sfr __at 0xF7E PTCON1;
typedef union {
	struct {
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned PTDIR     	: 1;
		unsigned PTEN      	: 1;
	};
} __PTCON1_t;
extern volatile __PTCON1_t __at 0xF7E PTCON1bits;

extern __sfr __at 0xF7F PTCON0;
typedef union {
	struct {
		unsigned PTMOD     	: 2;
		unsigned PTCKPS    	: 2;
		unsigned PTOPS     	: 4;
	};
} __PTCON0_t;
extern volatile __PTCON0_t __at 0xF7F PTCON0bits;

extern __sfr __at 0xF80 PORTA;
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
		unsigned AN0       	:1;
		unsigned AN1       	:1;
		unsigned AN2       	:1;
		unsigned AN3       	:1;
		unsigned AN4       	:1;
		unsigned AN5       	:1;
		unsigned OSC2      	:1;
		unsigned OSC1      	:1;
	};
	struct {
		unsigned           	:1;
		unsigned           	:1;
		unsigned CAP1      	:1;
		unsigned CAP2      	:1;
		unsigned CAP3      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTA_t;
extern volatile __PORTA_t __at 0xF80 PORTAbits;

extern __sfr __at 0xF81 PORTB;
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
		unsigned PWM0      	:1;
		unsigned PWM1      	:1;
		unsigned PWM2      	:1;
		unsigned PWM3      	:1;
		unsigned PWM5      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTB_t;
extern volatile __PORTB_t __at 0xF81 PORTBbits;

extern __sfr __at 0xF82 PORTC;
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
		unsigned CCP2      	:1;
		unsigned CCP1      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned U1TX      	:1;
		unsigned U1RX      	:1;
	};
	struct {
		unsigned T1OSCO    	:1;
		unsigned T1OSCI    	:1;
		unsigned PWMFLTB   	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTC_t;
extern volatile __PORTC_t __at 0xF82 PORTCbits;

extern __sfr __at 0xF83 PORTD;
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
		unsigned           	:1;
		unsigned PWM6      	:1;
		unsigned PWM7      	:1;
	};
} __PORTD_t;
extern volatile __PORTD_t __at 0xF83 PORTDbits;

extern __sfr __at 0xF84 PORTE;
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
		unsigned AN6       	:1;
		unsigned AN7       	:1;
		unsigned AN8       	:1;
		unsigned MCLR      	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
		unsigned           	:1;
	};
} __PORTE_t;
extern volatile __PORTE_t __at 0xF84 PORTEbits;

extern __sfr __at 0xF87 TMR5L;

extern __sfr __at 0xF88 TMR5H;

extern __sfr __at 0xF89 LATA;
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
extern volatile __LATA_t __at 0xF89 LATAbits;

extern __sfr __at 0xF8A LATB;
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
extern volatile __LATB_t __at 0xF8A LATBbits;

extern __sfr __at 0xF8B LATC;
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
extern volatile __LATC_t __at 0xF8B LATCbits;

extern __sfr __at 0xF8C LATD;
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
extern volatile __LATD_t __at 0xF8C LATDbits;

extern __sfr __at 0xF8D LATE;
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
extern volatile __LATE_t __at 0xF8D LATEbits;

extern __sfr __at 0xF90 PR5L;

extern __sfr __at 0xF91 PR5H;

extern __sfr __at 0xF92 TRISA;
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
extern volatile __TRISA_t __at 0xF92 TRISAbits;

extern __sfr __at 0xF93 TRISB;
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
extern volatile __TRISB_t __at 0xF93 TRISBbits;

extern __sfr __at 0xF94 TRISC;
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
extern volatile __TRISC_t __at 0xF94 TRISCbits;

extern __sfr __at 0xF95 TRISD;
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
extern volatile __TRISD_t __at 0xF95 TRISDbits;

extern __sfr __at 0xF96 TRISE;
typedef union {
	struct {
		unsigned TRISE0    	: 1;
		unsigned TRISE1    	: 1;
		unsigned TRISE2    	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __TRISE_t;
extern volatile __TRISE_t __at 0xF96 TRISEbits;

extern __sfr __at 0xF99 ADCHS;
typedef union {
	struct {
		unsigned SASEL     	: 2;
		unsigned SCSEL     	: 2;
		unsigned SBSEL     	: 2;
		unsigned SDSEL     	: 2;
	};
} __ADCHS_t;
extern volatile __ADCHS_t __at 0xF99 ADCHSbits;

extern __sfr __at 0xF9A ADCON3;
typedef union {
	struct {
		unsigned SSRC      	: 5;
		unsigned           	: 1;
		unsigned ADRS      	: 2;
	};
} __ADCON3_t;
extern volatile __ADCON3_t __at 0xF9A ADCON3bits;

extern __sfr __at 0xF9B OSCTUNE;
typedef union {
	struct {
		unsigned TUN       	: 6;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __OSCTUNE_t;
extern volatile __OSCTUNE_t __at 0xF9B OSCTUNEbits;

extern __sfr __at 0xF9D PIE1;
typedef union {
	struct {
		unsigned TMR1IE    	: 1;
		unsigned TMR2IE    	: 1;
		unsigned CCP1IE    	: 1;
		unsigned SSPIE     	: 1;
		unsigned TXIE      	: 1;
		unsigned RCIE      	: 1;
		unsigned ADIE      	: 1;
		unsigned           	: 1;
	};
} __PIE1_t;
extern volatile __PIE1_t __at 0xF9D PIE1bits;

extern __sfr __at 0xF9E PIR1;
typedef union {
	struct {
		unsigned TMR1IF    	: 1;
		unsigned TMR2IF    	: 1;
		unsigned CCP1IF    	: 1;
		unsigned SSPIF     	: 1;
		unsigned TXIF      	: 1;
		unsigned RCIF      	: 1;
		unsigned ADIF      	: 1;
		unsigned           	: 1;
	};
} __PIR1_t;
extern volatile __PIR1_t __at 0xF9E PIR1bits;

extern __sfr __at 0xF9F IPR1;
typedef union {
	struct {
		unsigned TMR1IP    	: 1;
		unsigned TMR2IP    	: 1;
		unsigned CCP1IP    	: 1;
		unsigned SSPIP     	: 1;
		unsigned TXIP      	: 1;
		unsigned RCIP      	: 1;
		unsigned ADIP      	: 1;
		unsigned           	: 1;
	};
} __IPR1_t;
extern volatile __IPR1_t __at 0xF9F IPR1bits;

extern __sfr __at 0xFA0 PIE2;
typedef union {
	struct {
		unsigned CCP2IE    	: 1;
		unsigned           	: 1;
		unsigned LVDIE     	: 1;
		unsigned           	: 1;
		unsigned EEIE      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned OSFIE     	: 1;
	};
} __PIE2_t;
extern volatile __PIE2_t __at 0xFA0 PIE2bits;

extern __sfr __at 0xFA1 PIR2;
typedef union {
	struct {
		unsigned CCP2IF    	: 1;
		unsigned           	: 1;
		unsigned LVDIF     	: 1;
		unsigned           	: 1;
		unsigned EEIF      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned OSFIF     	: 1;
	};
} __PIR2_t;
extern volatile __PIR2_t __at 0xFA1 PIR2bits;

extern __sfr __at 0xFA2 IPR2;
typedef union {
	struct {
		unsigned CCP2IP    	: 1;
		unsigned           	: 1;
		unsigned LVDIP     	: 1;
		unsigned           	: 1;
		unsigned EEIP      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned OSFIP     	: 1;
	};
} __IPR2_t;
extern volatile __IPR2_t __at 0xFA2 IPR2bits;

extern __sfr __at 0xFA3 PIE3;
typedef union {
	struct {
		unsigned TMR5IE    	: 1;
		unsigned IC1IE     	: 1;
		unsigned IC2QEIE   	: 1;
		unsigned IC3DRIE   	: 1;
		unsigned PTIE      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PIE3_t;
extern volatile __PIE3_t __at 0xFA3 PIE3bits;

extern __sfr __at 0xFA4 PIR3;
typedef union {
	struct {
		unsigned TMR5IF    	: 1;
		unsigned IC1IF     	: 1;
		unsigned IC2QEIF   	: 1;
		unsigned IC3DRIF   	: 1;
		unsigned PTIF      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PIR3_t;
extern volatile __PIR3_t __at 0xFA4 PIR3bits;

extern __sfr __at 0xFA5 IPR3;
typedef union {
	struct {
		unsigned TMR5IP    	: 1;
		unsigned IC1IP     	: 1;
		unsigned IC2QEIP   	: 1;
		unsigned IC3DRIP   	: 1;
		unsigned PTIP      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __IPR3_t;
extern volatile __IPR3_t __at 0xFA5 IPR3bits;

extern __sfr __at 0xFA6 EECON1;
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
extern volatile __EECON1_t __at 0xFA6 EECON1bits;

extern __sfr __at 0xFA7 EECON2;

extern __sfr __at 0xFA8 EEDATA;

extern __sfr __at 0xFA9 EEADR;

extern __sfr __at 0xFAA BAUDCTL;
typedef union {
	struct {
		unsigned ABDEN     	: 1;
		unsigned WUE       	: 1;
		unsigned           	: 1;
		unsigned BRG16     	: 1;
		unsigned SCKP      	: 1;
		unsigned           	: 1;
		unsigned RCIDL     	: 1;
		unsigned           	: 1;
	};
} __BAUDCTL_t;
extern volatile __BAUDCTL_t __at 0xFAA BAUDCTLbits;

extern __sfr __at 0xFAB RCSTA;
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
extern volatile __RCSTA_t __at 0xFAB RCSTAbits;

extern __sfr __at 0xFAC TXSTA;
typedef union {
	struct {
		unsigned TX9D      	: 1;
		unsigned TRMT      	: 1;
		unsigned BRGH      	: 1;
		unsigned           	: 1;
		unsigned SYNC      	: 1;
		unsigned TXEN      	: 1;
		unsigned TX9       	: 1;
		unsigned CSRC      	: 1;
	};
} __TXSTA_t;
extern volatile __TXSTA_t __at 0xFAC TXSTAbits;

extern __sfr __at 0xFAD TXREG;

extern __sfr __at 0xFAE RCREG;

extern __sfr __at 0xFAF SPBRG;

extern __sfr __at 0xFB0 SPBRGH;

extern __sfr __at 0xFB6 QEICON;
typedef union {
	struct {
		unsigned PDEC      	: 2;
		unsigned QEIM      	: 3;
		unsigned UP_nDOWN  	: 1;
		unsigned           	: 1;
		unsigned nVELM     	: 1;
	};
} __QEICON_t;
extern volatile __QEICON_t __at 0xFB6 QEICONbits;

extern __sfr __at 0xFB7 T5CON;
typedef union {
	struct {
		unsigned TMR5ON    	: 1;
		unsigned TMR5CS    	: 1;
		unsigned nT5SYNC   	: 1;
		unsigned T5PS      	: 2;
		unsigned T5MOD     	: 1;
		unsigned nRESEN    	: 1;
		unsigned T5SEN     	: 1;
	};
} __T5CON_t;
extern volatile __T5CON_t __at 0xFB7 T5CONbits;

extern __sfr __at 0xFB8 ANSEL0;
typedef union {
	struct {
		unsigned AN0       	: 1;
		unsigned AN1       	: 1;
		unsigned AN2       	: 1;
		unsigned AN3       	: 1;
		unsigned AN4       	: 1;
		unsigned AN5       	: 1;
		unsigned AN6       	: 1;
		unsigned AN7       	: 1;
	};
} __ANSEL0_t;
extern volatile __ANSEL0_t __at 0xFB8 ANSEL0bits;

extern __sfr __at 0xFB9 ANSEL1;
typedef union {
	struct {
		unsigned ANS8      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __ANSEL1_t;
extern volatile __ANSEL1_t __at 0xFB9 ANSEL1bits;

extern __sfr __at 0xFBA CCP2CON;
typedef union {
	struct {
		unsigned CCP2M     	: 4;
		unsigned DC2B      	: 2;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __CCP2CON_t;
extern volatile __CCP2CON_t __at 0xFBA CCP2CONbits;

extern __sfr __at 0xFBB CCPR2L;

extern __sfr __at 0xFBC CCPR2H;

extern __sfr __at 0xFBD CCP1CON;
typedef union {
	struct {
		unsigned CCP1M     	: 4;
		unsigned DC1B      	: 2;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __CCP1CON_t;
extern volatile __CCP1CON_t __at 0xFBD CCP1CONbits;

extern __sfr __at 0xFBE CCPR1L;

extern __sfr __at 0xFBF CCPR1H;

extern __sfr __at 0xFC0 ADCON2;
typedef union {
	struct {
		unsigned ADCS      	: 3;
		unsigned ACQT      	: 4;
		unsigned ADFM      	: 1;
	};
} __ADCON2_t;
extern volatile __ADCON2_t __at 0xFC0 ADCON2bits;

extern __sfr __at 0xFC1 ADCON1;
typedef union {
	struct {
		unsigned ADPNT     	: 2;
		unsigned BFOVFL    	: 1;
		unsigned BFEMT     	: 1;
		unsigned FIFOEN    	: 1;
		unsigned           	: 1;
		unsigned VCFG      	: 2;
	};
} __ADCON1_t;
extern volatile __ADCON1_t __at 0xFC1 ADCON1bits;

extern __sfr __at 0xFC2 ADCON0;
typedef union {
	struct {
		unsigned ADON      	: 1;
		unsigned GO_nDONE  	: 1;
		unsigned ACMOD     	: 2;
		unsigned ACSCH     	: 1;
		unsigned ACONV     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __ADCON0_t;
extern volatile __ADCON0_t __at 0xFC2 ADCON0bits;

extern __sfr __at 0xFC3 ADRESL;

extern __sfr __at 0xFC4 ADRESH;

extern __sfr __at 0xFC6 SSPCON;
typedef union {
	struct {
		unsigned SSPM      	: 4;
		unsigned CKP       	: 1;
		unsigned SSPEN     	: 1;
		unsigned SSPOV     	: 1;
		unsigned WCOL      	: 1;
	};
} __SSPCON_t;
extern volatile __SSPCON_t __at 0xFC6 SSPCONbits;

extern __sfr __at 0xFC7 SSPSTAT;
typedef union {
	struct {
		unsigned BF        	: 1;
		unsigned UA        	: 1;
		unsigned R_nW      	: 1;
		unsigned S         	: 1;
		unsigned P         	: 1;
		unsigned D_nA      	: 1;
		unsigned CKE       	: 1;
		unsigned SMP       	: 1;
	};
} __SSPSTAT_t;
extern volatile __SSPSTAT_t __at 0xFC7 SSPSTATbits;

extern __sfr __at 0xFC8 SSPADD;

extern __sfr __at 0xFC9 SSPBUF;

extern __sfr __at 0xFCA T2CON;
typedef union {
	struct {
		unsigned T2CKPS    	: 2;
		unsigned TMR2ON    	: 1;
		unsigned TOUTPS    	: 4;
		unsigned           	: 1;
	};
} __T2CON_t;
extern volatile __T2CON_t __at 0xFCA T2CONbits;

extern __sfr __at 0xFCB PR2;

extern __sfr __at 0xFCC TMR2;

extern __sfr __at 0xFCD T1CON;
typedef union {
	struct {
		unsigned TMR1ON    	: 1;
		unsigned TMR1CS    	: 1;
		unsigned nT1SYNC   	: 1;
		unsigned T1OSCEN   	: 1;
		unsigned T1CKPS    	: 2;
		unsigned T1RUN     	: 1;
		unsigned RD16      	: 1;
	};
} __T1CON_t;
extern volatile __T1CON_t __at 0xFCD T1CONbits;

extern __sfr __at 0xFCE TMR1L;

extern __sfr __at 0xFCF TMR1H;

extern __sfr __at 0xFD0 RCON;
typedef union {
	struct {
		unsigned nBOR      	: 1;
		unsigned nPOR      	: 1;
		unsigned nPD       	: 1;
		unsigned nTO       	: 1;
		unsigned nRI       	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned IPEN      	: 1;
	};
} __RCON_t;
extern volatile __RCON_t __at 0xFD0 RCONbits;

extern __sfr __at 0xFD1 WDTCON;
typedef union {
	struct {
		unsigned SWDTEN    	: 1;
		unsigned WDT       	: 7;
	};
} __WDTCON_t;
extern volatile __WDTCON_t __at 0xFD1 WDTCONbits;

extern __sfr __at 0xFD2 LVDCON;
typedef union {
	struct {
		unsigned LVDL      	: 4;
		unsigned LVDEN     	: 1;
		unsigned IRVST     	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __LVDCON_t;
extern volatile __LVDCON_t __at 0xFD2 LVDCONbits;

extern __sfr __at 0xFD3 OSCCON;
typedef union {
	struct {
		unsigned SCS       	: 2;
		unsigned FLTS      	: 1;
		unsigned OSTS      	: 1;
		unsigned IRCF      	: 3;
		unsigned IDLEN     	: 1;
	};
} __OSCCON_t;
extern volatile __OSCCON_t __at 0xFD3 OSCCONbits;

extern __sfr __at 0xFD5 T0CON;
typedef union {
	struct {
		unsigned T0PS      	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned _16BIT    	: 1;
		unsigned TMR0ON    	: 1;
	};
} __T0CON_t;
extern volatile __T0CON_t __at 0xFD5 T0CONbits;

extern __sfr __at 0xFD6 TMR0L;

extern __sfr __at 0xFD7 TMR0H;

extern __sfr __at 0xFD8 STATUS;
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
extern volatile __STATUS_t __at 0xFD8 STATUSbits;

extern __sfr __at 0xFD9 FSR2L;

extern __sfr __at 0xFDA FSR2H;
typedef union {
	struct {
		unsigned FSR2H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR2H_t;
extern volatile __FSR2H_t __at 0xFDA FSR2Hbits;

extern __sfr __at 0xFDB PLUSW2;

extern __sfr __at 0xFDC PREINC2;

extern __sfr __at 0xFDD POSTDEC2;

extern __sfr __at 0xFDE POSTINC2;

extern __sfr __at 0xFDF INDF2;

extern __sfr __at 0xFE0 BSR;
typedef union {
	struct {
		unsigned BSR       	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __BSR_t;
extern volatile __BSR_t __at 0xFE0 BSRbits;

extern __sfr __at 0xFE1 FSR1L;

extern __sfr __at 0xFE2 FSR1H;
typedef union {
	struct {
		unsigned FSR1H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR1H_t;
extern volatile __FSR1H_t __at 0xFE2 FSR1Hbits;

extern __sfr __at 0xFE3 PLUSW1;

extern __sfr __at 0xFE4 PREINC1;

extern __sfr __at 0xFE5 POSTDEC1;

extern __sfr __at 0xFE6 POSTINC1;

extern __sfr __at 0xFE7 INDF1;

extern __sfr __at 0xFE8 WREG;

extern __sfr __at 0xFE9 FSR0L;

extern __sfr __at 0xFEA FSR0H;
typedef union {
	struct {
		unsigned FSR0H     	: 4;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __FSR0H_t;
extern volatile __FSR0H_t __at 0xFEA FSR0Hbits;

extern __sfr __at 0xFEB PLUSW0;

extern __sfr __at 0xFEC PREINC0;

extern __sfr __at 0xFED POSTDEC0;

extern __sfr __at 0xFEE POSTINC0;

extern __sfr __at 0xFEF INDF0;

extern __sfr __at 0xFF0 INTCON3;
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
extern volatile __INTCON3_t __at 0xFF0 INTCON3bits;

extern __sfr __at 0xFF1 INTCON2;
typedef union {
	struct {
		unsigned RBIP      	: 1;
		unsigned           	: 1;
		unsigned TMR0IP    	: 1;
		unsigned           	: 1;
		unsigned INTEDG2   	: 1;
		unsigned INTEDG1   	: 1;
		unsigned INTEDG0   	: 1;
		unsigned nRBPU     	: 1;
	};
} __INTCON2_t;
extern volatile __INTCON2_t __at 0xFF1 INTCON2bits;

extern __sfr __at 0xFF2 INTCON;
typedef union {
	struct {
		unsigned RBIF      	: 1;
		unsigned INT0IF    	: 1;
		unsigned TMR0IF    	: 1;
		unsigned RBIE      	: 1;
		unsigned INT0IE    	: 1;
		unsigned TMR0IE    	: 1;
		unsigned PEIE_GIEL 	: 1;
		unsigned GIE_GIEH  	: 1;
	};
	struct {
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
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
extern volatile __INTCON_t __at 0xFF2 INTCONbits;

extern __sfr __at 0xFF3 PRODL;

extern __sfr __at 0xFF4 PRODH;

extern __sfr __at 0xFF5 TABLAT;

extern __sfr __at 0xFF6 TBLPTRL;

extern __sfr __at 0xFF7 TBLPTRH;

extern __sfr __at 0xFF8 TBLPTRU;
typedef union {
	struct {
		unsigned TBLPTRU   	: 5;
		unsigned ACSS      	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __TBLPTRU_t;
extern volatile __TBLPTRU_t __at 0xFF8 TBLPTRUbits;

extern __sfr __at 0xFF9 PCL;

extern __sfr __at 0xFFA PCLATH;
typedef union {
	struct {
		unsigned PCH       	: 8;
	};
} __PCLATH_t;
extern volatile __PCLATH_t __at 0xFFA PCLATHbits;

extern __sfr __at 0xFFB PCLATU;
typedef union {
	struct {
		unsigned PCU       	: 5;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __PCLATU_t;
extern volatile __PCLATU_t __at 0xFFB PCLATUbits;

extern __sfr __at 0xFFC STKPTR;
typedef union {
	struct {
		unsigned STKPTR    	: 5;
		unsigned           	: 1;
		unsigned STKUNF    	: 1;
		unsigned STKFUL    	: 1;
	};
} __STKPTR_t;
extern volatile __STKPTR_t __at 0xFFC STKPTRbits;

extern __sfr __at 0xFFD TOSL;

extern __sfr __at 0xFFE TOSH;

extern __sfr __at 0xFFF TOSU;
typedef union {
	struct {
		unsigned TOSU      	: 5;
		unsigned           	: 1;
		unsigned           	: 1;
		unsigned           	: 1;
	};
} __TOSU_t;
extern volatile __TOSU_t __at 0xFFF TOSUbits;

/* Configuration register locations */
#define	CONFIG1H	0x300001
#define	CONFIG2L	0x300002
#define	CONFIG2H	0x300003
#define	CONFIG3L	0x300004
#define	CONFIG3H	0x300005
#define	CONFIG4L	0x300006
#define	CONFIG5L	0x300008
#define	CONFIG5H	0x300009
#define	CONFIG6L	0x30000A
#define	CONFIG6H	0x30000B
#define	CONFIG7L	0x30000C
#define	CONFIG7H	0x30000D


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
#define	_FCMEN_ON_1H                            	0xFF	/* Enabled */
#define	_FCMEN_OFF_1H                           	0xBF	/* Disabled */

/* Internal External Switch Over Mode 1H options */
#define	_IESO_OFF_1H                            	0x7F	/* Disabled */
#define	_IESO_ON_1H                             	0xFF	/* Enabled */


/* Power Up Timer 2L options */
#define	_PUT_OFF_2L                             	0xFF	/* Disabled */
#define	_PUT_ON_2L                              	0xFE	/* Enabled */

/* Brown Out Detect 2L options */
#define	_BODEN_ON_2L                            	0xFF	/* Enabled */
#define	_BODEN_OFF_2L                           	0xFD	/* Disabled */

/* Brown Out Voltage 2L options */
#define	_BODENV_UNDEFINED_2L                    	0xFF	/* Undefined */
#define	_BODENV_2_7V_2L                         	0xFB	/* 2.7V */
#define	_BODENV_4_2V_2L                         	0xF7	/* 4.2V */
#define	_BODENV_4_5V_2L                         	0xF3	/* 4.5V */


/* Watchdog Timer 2H options */
#define	_WDT_ON_2H                              	0xFF	/* Enabled */
#define	_WDT_OFF_2H                             	0xFE	/* Disabled */

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

/* Watchdog Timer Window 2H options */
#define	_WINEN_OFF_2H                           	0xFF	/* Disabled */
#define	_WINEN_ON_2H                            	0xDF	/* Enabled */


/* PWM Output Pin Reset 3L options */
#define	_PWMPIN_PWM_OUTPUTS_OFF_UPON_RESET_3L   	0xFF	/* PWM outputs disabled upon RESET */
#define	_PWMPIN_PWM_OUTPUTS_DRIVE_ACTIVE_STATES_UPON_RESET_3L	0xFB	/* PWM outputs drive active states upon RESET */

/* Low-Side Transistors Polarity 3L options */
#define	_LPOL_PWM_0__2__4_AND_6_ARE_ACTIVE_HIGH_3L	0xFF	/* PWM 0, 2, 4 and 6 are active high */
#define	_LPOL_PWM_0__2__4_AND_6_ARE_ACTIVE_LOW_3L	0xF7	/* PWM 0, 2, 4 and 6 are active low */

/* High-Side Transistors Polarity 3L options */
#define	_HPOL_PWM_1__3__5__AND_7_ARE_ACTIVE_HIGH_3L	0xFF	/* PWM 1, 3, 5, and 7 are active high */
#define	_HPOL_PWM_1__3__5__AND_7_ARE_ACTIVE_LOW_3L	0xEF	/* PWM 1, 3, 5, and 7 are active low */

/* Timer1 OSC 3L options */
#define	_T1OSCMX_LOW_POWER_3L                   	0xFF	/* Low Power */
#define	_T1OSCMX_LEGACY_3L                      	0xDF	/* Legacy */


/* FLTA Mux 3H options */
#define	_FLTAMX_FLTA_INPUT_MUXED_WITH_RC1_3H    	0xFF	/* FLTA input muxed with RC1 */
#define	_FLTAMX_FLTA_INPUT_MUXED_WITH_RD4_3H    	0xFE	/* FLTA input muxed with RD4 */

/* SSP I/O Mux 3H options */
#define	_SSPMX_SCK_SCL__SDA_SDI_AND_SDO_ARE_MUX_W__RD3__RD2_AND_RD1_RESPECTIVELY__3H	0xFB	/* SCK/SCL, SDA/SDI and SDO are mux w/ RD3, RD2 and RD1 respectively. */
#define	_SSPMX_SCK_SCL__SDA_SDI_AND_SDO_ARE_MUX_W__RC5__RC4_AND_RC7_RESPECTIVELY__3H	0xFF	/* SCK/SCL, SDA/SDI and SDO are mux w/ RC5, RC4 and RC7 respectively. */

/* PWM4 Mux 3H options */
#define	_PWM4MX_PWM4_OUTPUT_MUXED_W__RB5_3H     	0xFF	/* PWM4 output muxed w/ RB5 */
#define	_PWM4MX_PWM4_OUTPUT_MUXED_W__RD5_3H     	0xF7	/* PWM4 output muxed w/ RD5 */

/* TMR0/T5CKI EXT CLK Mux 3H options */
#define	_EXCLKMX_TMR0_T5CKI_EXTERNAL_CLOCK_INPUT_IS_MULTIPLEXED_WITH_RD0_3H	0xEF	/* TMR0/T5CKI external clock input is multiplexed with RD0 */
#define	_EXCLKMX_TMR0_T5CKI_EXTERNAL_CLOCK_INPUT_IS_MULTIPLEXED_WITH_RC3_3H	0xFF	/* TMR0/T5CKI external clock input is multiplexed with RC3 */

/* Master Clear Enable 3H options */
#define	_MCLRE_MCLR_ON__RE3_INPUT_OFF_3H        	0xFF	/* MCLR enabled, RE3 input disabled */
#define	_MCLRE_RE3_INPUT_ON__MCLR_OFF_3H        	0x7F	/* RE3 input enabled, MCLR disabled */


/* Background Debug 4L options */
#define	_BACKBUG_OFF_4L                         	0xFF	/* Disabled */
#define	_BACKBUG_ON_4L                          	0x7F	/* Enabled */

/* Low Voltage Program 4L options */
#define	_LVP_ON_4L                              	0xFF	/* Enabled */
#define	_LVP_OFF_4L                             	0xFB	/* Disabled */

/* Stack Overflow Reset 4L options */
#define	_STVR_ON_4L                             	0xFF	/* Enabled */
#define	_STVR_OFF_4L                            	0xFE	/* Disabled */


/* Code Protect 00200-00FFF 5L options */
#define	_CP_0_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_0_ON_5L                             	0xFE	/* Enabled */

/* Code Protect 01000-01FFF 5L options */
#define	_CP_1_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_1_ON_5L                             	0xFD	/* Enabled */

/* Code Protect 02000-02FFF 5L options */
#define	_CP_2_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_2_ON_5L                             	0xFB	/* Enabled */

/* Code Protect 03000-03FFF 5L options */
#define	_CP_3_OFF_5L                            	0xFF	/* Disabled */
#define	_CP_3_ON_5L                             	0xF7	/* Enabled */


/* Data EE Read Protect 5H options */
#define	_CPD_OFF_5H                             	0xFF	/* Disabled */
#define	_CPD_ON_5H                              	0x7F	/* Enabled */

/* Code Protect Boot 5H options */
#define	_CPB_OFF_5H                             	0xFF	/* Disabled */
#define	_CPB_ON_5H                              	0xBF	/* Enabled */


/* Table Write Protect 00200-00FFF 6L options */
#define	_WRT_0_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_0_ON_6L                            	0xFE	/* Enabled */

/* Table Write Protect 01000-01FFF 6L options */
#define	_WRT_1_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_1_ON_6L                            	0xFD	/* Enabled */

/* Table Write Protect 02000-02FFF 6L options */
#define	_WRT_2_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_2_ON_6L                            	0xFB	/* Enabled */

/* Table Write Protect 03000-03FFF 6L options */
#define	_WRT_3_OFF_6L                           	0xFF	/* Disabled */
#define	_WRT_3_ON_6L                            	0xF7	/* Enabled */


/* Data EE Write Protect 6H options */
#define	_WRTD_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTD_ON_6H                             	0x7F	/* Enabled */

/* Table Write Protect Boot 6H options */
#define	_WRTB_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTB_ON_6H                             	0xBF	/* Enabled */

/* Config. Write Protect 6H options */
#define	_WRTC_OFF_6H                            	0xFF	/* Disabled */
#define	_WRTC_ON_6H                             	0xDF	/* Enabled */


/* Table Read Protect 00200-00FFF 7L options */
#define	_EBTR_0_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_0_ON_7L                           	0xFE	/* Enabled */

/* Table Read Protect 01000-01FFF 7L options */
#define	_EBTR_1_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_1_ON_7L                           	0xFD	/* Enabled */

/* Table Read Protect 02000-02FFF 7L options */
#define	_EBTR_2_OFF_7L                          	0xFF	/* Disabled */
#define	_EBTR_2_ON_7L                           	0xFB	/* Enabled */

/* Table Read Protect 03000-03FFF 7L options */
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

#endif // __PIC18F4331__
