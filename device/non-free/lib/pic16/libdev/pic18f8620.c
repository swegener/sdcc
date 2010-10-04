
/*
 * pic18f8620.c - PIC18F8620 Device Library Source
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

#include <pic18f8620.h>

__sfr __at (0xf6b) RCSTA2;
volatile __RCSTA2bits_t __at (0xf6b) RCSTA2bits;

__sfr __at (0xf6c) TXSTA2;
volatile __TXSTA2bits_t __at (0xf6c) TXSTA2bits;

__sfr __at (0xf6d) TXREG2;
__sfr __at (0xf6e) RCREG2;
__sfr __at (0xf6f) SPBRG2;
__sfr __at (0xf70) CCP5CON;
volatile __CCP5CONbits_t __at (0xf70) CCP5CONbits;

__sfr __at (0xf71) CCPR5L;
__sfr __at (0xf72) CCPR5H;
__sfr __at (0xf73) CCP4CON;
volatile __CCP4CONbits_t __at (0xf73) CCP4CONbits;

__sfr __at (0xf74) CCPR4L;
__sfr __at (0xf75) CCPR4H;
__sfr __at (0xf76) T4CON;
volatile __T4CONbits_t __at (0xf76) T4CONbits;

__sfr __at (0xf77) PR4;
__sfr __at (0xf78) TMR4;
__sfr __at (0xf80) PORTA;
volatile __PORTAbits_t __at (0xf80) PORTAbits;

__sfr __at (0xf81) PORTB;
volatile __PORTBbits_t __at (0xf81) PORTBbits;

__sfr __at (0xf82) PORTC;
volatile __PORTCbits_t __at (0xf82) PORTCbits;

__sfr __at (0xf83) PORTD;
volatile __PORTDbits_t __at (0xf83) PORTDbits;

__sfr __at (0xf84) PORTE;
volatile __PORTEbits_t __at (0xf84) PORTEbits;

__sfr __at (0xf85) PORTF;
volatile __PORTFbits_t __at (0xf85) PORTFbits;

__sfr __at (0xf86) PORTG;
volatile __PORTGbits_t __at (0xf86) PORTGbits;

__sfr __at (0xf87) PORTH;
volatile __PORTHbits_t __at (0xf87) PORTHbits;

__sfr __at (0xf88) PORTJ;
volatile __PORTJbits_t __at (0xf88) PORTJbits;

__sfr __at (0xf89) LATA;
volatile __LATAbits_t __at (0xf89) LATAbits;

__sfr __at (0xf8a) LATB;
volatile __LATBbits_t __at (0xf8a) LATBbits;

__sfr __at (0xf8b) LATC;
volatile __LATCbits_t __at (0xf8b) LATCbits;

__sfr __at (0xf8c) LATD;
volatile __LATDbits_t __at (0xf8c) LATDbits;

__sfr __at (0xf8d) LATE;
volatile __LATEbits_t __at (0xf8d) LATEbits;

__sfr __at (0xf8e) LATF;
volatile __LATFbits_t __at (0xf8e) LATFbits;

__sfr __at (0xf8f) LATG;
volatile __LATGbits_t __at (0xf8f) LATGbits;

__sfr __at (0xf90) LATH;
volatile __LATHbits_t __at (0xf90) LATHbits;

__sfr __at (0xf91) LATJ;
volatile __LATJbits_t __at (0xf91) LATJbits;

__sfr __at (0xf92) TRISA;
volatile __TRISAbits_t __at (0xf92) TRISAbits;

__sfr __at (0xf93) TRISB;
volatile __TRISBbits_t __at (0xf93) TRISBbits;

__sfr __at (0xf94) TRISC;
volatile __TRISCbits_t __at (0xf94) TRISCbits;

__sfr __at (0xf95) TRISD;
volatile __TRISDbits_t __at (0xf95) TRISDbits;

__sfr __at (0xf96) TRISE;
volatile __TRISEbits_t __at (0xf96) TRISEbits;

__sfr __at (0xf97) TRISF;
volatile __TRISFbits_t __at (0xf97) TRISFbits;

__sfr __at (0xf98) TRISG;
volatile __TRISGbits_t __at (0xf98) TRISGbits;

__sfr __at (0xf99) TRISH;
volatile __TRISHbits_t __at (0xf99) TRISHbits;

__sfr __at (0xf9a) TRISJ;
volatile __TRISJbits_t __at (0xf9a) TRISJbits;

__sfr __at (0xf9c) MEMCON;
volatile __MEMCONbits_t __at (0xf9c) MEMCONbits;

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

__sfr __at (0xfa3) PIE3;
volatile __PIE3bits_t __at (0xfa3) PIE3bits;

__sfr __at (0xfa4) PIR3;
volatile __PIR3bits_t __at (0xfa4) PIR3bits;

__sfr __at (0xfa5) IPR3;
volatile __IPR3bits_t __at (0xfa5) IPR3bits;

__sfr __at (0xfa6) EECON1;
volatile __EECON1bits_t __at (0xfa6) EECON1bits;

__sfr __at (0xfa7) EECON2;
__sfr __at (0xfa8) EEDATA;
__sfr __at (0xfa9) EEADR;
__sfr __at (0xfaa) EEADRH;
__sfr __at (0xfab) RCSTA1;
volatile __RCSTA1bits_t __at (0xfab) RCSTA1bits;

__sfr __at (0xfac) TXSTA1;
volatile __TXSTA1bits_t __at (0xfac) TXSTA1bits;

__sfr __at (0xfad) TXREG1;
__sfr __at (0xfae) RCREG1;
__sfr __at (0xfaf) SPBRG1;
__sfr __at (0xfb0) PSPCON;
volatile __PSPCONbits_t __at (0xfb0) PSPCONbits;

__sfr __at (0xfb1) T3CON;
volatile __T3CONbits_t __at (0xfb1) T3CONbits;

__sfr __at (0xfb2) TMR3L;
__sfr __at (0xfb3) TMR3H;
__sfr __at (0xfb4) CMCON;
volatile __CMCONbits_t __at (0xfb4) CMCONbits;

__sfr __at (0xfb5) CVRCON;
volatile __CVRCONbits_t __at (0xfb5) CVRCONbits;

__sfr __at (0xfb7) CCP3CON;
volatile __CCP3CONbits_t __at (0xfb7) CCP3CONbits;

__sfr __at (0xfb8) CCPR3L;
__sfr __at (0xfb9) CCPR3H;
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


/* for compatibility reasons */
__sfr __at (0xfab) RCSTA;
volatile __RCSTA1bits_t __at (0xfab) RCSTAbits;

__sfr __at (0xfac) TXSTA;
volatile __TXSTA1bits_t __at (0xfac) TXSTAbits;

__sfr __at (0xfad) TXREG;
__sfr __at (0xfae) RCREG;
__sfr __at (0xfaf) SPBRG;
