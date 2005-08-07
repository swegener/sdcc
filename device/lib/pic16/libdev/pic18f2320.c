
/*
 * pic18f2320.c - PIC18F2320 Device Library Source
 *
 * This file is part of the GNU PIC Library.
 *
 * January, 2004
 * The GNU PIC Library is maintained by,
 * 	Vangelis Rokas <vrokas@otenet.gr>
 *
 * $Id$
 *
 */

#include <pic18f2220.h>

__sfr __at (0xf80) PORTA;
volatile __PORTAbits_t __at (0xf80) PORTAbits;

__sfr __at (0xf81) PORTB;
volatile __PORTBbits_t __at (0xf81) PORTBbits;

__sfr __at (0xf82) PORTC;
volatile __PORTCbits_t __at (0xf82) PORTCbits;

__sfr __at (0xf89) LATA;
volatile __LATAbits_t __at (0xf89) LATAbits;

__sfr __at (0xf8a) LATB;
volatile __LATBbits_t __at (0xf8a) LATBbits;

__sfr __at (0xf8b) LATC;
volatile __LATCbits_t __at (0xf8b) LATCbits;

__sfr __at (0xf92) TRISA;
volatile __TRISAbits_t __at (0xf92) TRISAbits;

__sfr __at (0xf93) TRISB;
volatile __TRISBbits_t __at (0xf93) TRISBbits;

__sfr __at (0xf94) TRISC;
volatile __TRISCbits_t __at (0xf94) TRISCbits;

__sfr __at (0xf9b) OSCTUNE;
volatile __OSCTUNEbits_t __at (0xf9b) OSCTUNEbits;

__sfr __at (0xf9d) PIE1;
volatile __PIE1bits_t __at (0xf9d) PIE1bits;

__sfr __at (0xf9e) PIR1;
volatile __PIR1bits_t __at (0xf9e) PIR1bits;

__sfr __at (0xf9f) IPR1;
volatile __IPR1bits_t __at (0xf9f) IPR1bits;

__sfr __at (0xfa0) PIE2;
volatile __PIE2bits_t __at (0xfa0) PIE2bits;

__sfr __at (0xfa1) PIR2;
volatile __PIR2bits_t __at (0xfa1) PIR2bits;

__sfr __at (0xfa2) IPR2;
volatile __IPR2bits_t __at (0xfa2) IPR2bits;

__sfr __at (0xfa6) EECON1;
volatile __EECON1bits_t __at (0xfa6) EECON1bits;

__sfr __at (0xfa7) EECON2;
__sfr __at (0xfa8) EEDATA;
__sfr __at (0xfa9) EEADR;
__sfr __at (0xfab) RCSTA;
volatile __RCSTAbits_t __at (0xfab) RCSTAbits;

__sfr __at (0xfac) TXSTA;
volatile __TXSTAbits_t __at (0xfac) TXSTAbits;

__sfr __at (0xfad) TXREG;
__sfr __at (0xfae) RCREG;
__sfr __at (0xfaf) SPBRG;
__sfr __at (0xfb1) T3CON;
volatile __T3CONbits_t __at (0xfb1) T3CONbits;

__sfr __at (0xfb2) TMR3L;
__sfr __at (0xfb3) TMR3H;
__sfr __at (0xfb4) CMCON;
volatile __CMCONbits_t __at (0xfb4) CMCONbits;

__sfr __at (0xfb5) CVRCON;
volatile __CVRCONbits_t __at (0xfb5) CVRCONbits;

__sfr __at (0xfba) CCP2CON;
volatile __CCP2CONbits_t __at (0xfba) CCP2CONbits;

__sfr __at (0xfbb) CCPR2L;
__sfr __at (0xfbc) CCPR2H;
__sfr __at (0xfbd) CCP1CON;
volatile __CCP1CONbits_t __at (0xfbd) CCP1CONbits;

__sfr __at (0xfbe) CCPR1L;
__sfr __at (0xfbf) CCPR1H;
__sfr __at (0xfc0) ADCON2;
volatile __ADCON2bits_t __at (0xfc0) ADCON2bits;

__sfr __at (0xfc1) ADCON1;
volatile __ADCON1bits_t __at (0xfc1) ADCON1bits;

__sfr __at (0xfc2) ADCON0;
volatile __ADCON0bits_t __at (0xfc2) ADCON0bits;

__sfr __at (0xfc3) ADRESL;
__sfr __at (0xfc4) ADRESH;
__sfr __at (0xfc5) SSPCON2;
volatile __SSPCON2bits_t __at (0xfc5) SSPCON2bits;

__sfr __at (0xfc6) SSPCON1;
volatile __SSPCON1bits_t __at (0xfc6) SSPCON1bits;

__sfr __at (0xfc7) SSPSTAT;
volatile __SSPSTATbits_t __at (0xfc7) SSPSTATbits;

__sfr __at (0xfc8) SSPADD;
__sfr __at (0xfc9) SSPBUF;
__sfr __at (0xfca) T2CON;
volatile __T2CONbits_t __at (0xfca) T2CONbits;

__sfr __at (0xfcb) PR2;
__sfr __at (0xfcc) TMR2;
__sfr __at (0xfcd) T1CON;
volatile __T1CONbits_t __at (0xfcd) T1CONbits;

__sfr __at (0xfce) TMR1L;
__sfr __at (0xfcf) TMR1H;
__sfr __at (0xfd0) RCON;
volatile __RCONbits_t __at (0xfd0) RCONbits;

__sfr __at (0xfd1) WDTCON;
volatile __WDTCONbits_t __at (0xfd1) WDTCONbits;

__sfr __at (0xfd2) LVDCON;
volatile __LVDCONbits_t __at (0xfd2) LVDCONbits;

__sfr __at (0xfd3) OSCCON;
volatile __OSCCONbits_t __at (0xfd3) OSCCONbits;

__sfr __at (0xfd5) T0CON;
volatile __T0CONbits_t __at (0xfd5) T0CONbits;

__sfr __at (0xfd6) TMR0L;
__sfr __at (0xfd7) TMR0H;
__sfr __at (0xfd8) STATUS;
volatile __STATUSbits_t __at (0xfd8) STATUSbits;

__sfr __at (0xfd9) FSR2L;
__sfr __at (0xfda) FSR2H;
__sfr __at (0xfdb) PLUSW2;
__sfr __at (0xfdc) PREINC2;
__sfr __at (0xfdd) POSTDEC2;
__sfr __at (0xfde) POSTINC2;
__sfr __at (0xfdf) INDF2;
__sfr __at (0xfe0) BSR;
__sfr __at (0xfe1) FSR1L;
__sfr __at (0xfe2) FSR1H;
__sfr __at (0xfe3) PLUSW1;
__sfr __at (0xfe4) PREINC1;
__sfr __at (0xfe5) POSTDEC1;
__sfr __at (0xfe6) POSTINC1;
__sfr __at (0xfe7) INDF1;
__sfr __at (0xfe8) WREG;
__sfr __at (0xfe9) FSR0L;
__sfr __at (0xfea) FSR0H;
__sfr __at (0xfeb) PLUSW0;
__sfr __at (0xfec) PREINC0;
__sfr __at (0xfed) POSTDEC0;
__sfr __at (0xfee) POSTINC0;
__sfr __at (0xfef) INDF0;
__sfr __at (0xff0) INTCON3;
volatile __INTCON3bits_t __at (0xff0) INTCON3bits;

__sfr __at (0xff1) INTCON2;
volatile __INTCON2bits_t __at (0xff1) INTCON2bits;

__sfr __at (0xff2) INTCON;
volatile __INTCONbits_t __at (0xff2) INTCONbits;

__sfr __at (0xff3) PRODL;
__sfr __at (0xff4) PRODH;
__sfr __at (0xff5) TABLAT;
__sfr __at (0xff6) TBLPTRL;
__sfr __at (0xff7) TBLPTRH;
__sfr __at (0xff8) TBLPTRU;
__sfr __at (0xff9) PCL;
__sfr __at (0xffa) PCLATH;
__sfr __at (0xffb) PCLATU;
__sfr __at (0xffc) STKPTR;
volatile __STKPTRbits_t __at (0xffc) STKPTRbits;

__sfr __at (0xffd) TOSL;
__sfr __at (0xffe) TOSH;
__sfr __at (0xfff) TOSU;


