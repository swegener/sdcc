
/*
 * pic18f8720.c - PIC18F8720 Device Library Source
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

#include <pic18f8720.h>

sfr at 0xf6b RCSTA2;
volatile __RCSTA2bits_t at 0xf6b RCSTA2bits;

sfr at 0xf6c TXSTA2;
volatile __TXSTA2bits_t at 0xf6c TXSTA2bits;

sfr at 0xf6d TXREG2;
sfr at 0xf6e RCREG2;
sfr at 0xf6f SPBRG2;
sfr at 0xf70 CCP5CON;
volatile __CCP5CONbits_t at 0xf70 CCP5CONbits;

sfr at 0xf71 CCPR5L;
sfr at 0xf72 CCPR5H;
sfr at 0xf73 CCP4CON;
volatile __CCP4CONbits_t at 0xf73 CCP4CONbits;

sfr at 0xf74 CCPR4L;
sfr at 0xf75 CCPR4H;
sfr at 0xf76 T4CON;
volatile __T4CONbits_t at 0xf76 T4CONbits;

sfr at 0xf77 PR4;
sfr at 0xf78 TMR4;
sfr at 0xf80 PORTA;
volatile __PORTAbits_t at 0xf80 PORTAbits;

sfr at 0xf81 PORTB;
volatile __PORTBbits_t at 0xf81 PORTBbits;

sfr at 0xf82 PORTC;
volatile __PORTCbits_t at 0xf82 PORTCbits;

sfr at 0xf83 PORTD;
volatile __PORTDbits_t at 0xf83 PORTDbits;

sfr at 0xf84 PORTE;
volatile __PORTEbits_t at 0xf84 PORTEbits;

sfr at 0xf85 PORTF;
volatile __PORTFbits_t at 0xf85 PORTFbits;

sfr at 0xf86 PORTG;
volatile __PORTGbits_t at 0xf86 PORTGbits;

sfr at 0xf87 PORTH;
volatile __PORTHbits_t at 0xf87 PORTHbits;

sfr at 0xf88 PORTJ;
volatile __PORTJbits_t at 0xf88 PORTJbits;

sfr at 0xf89 LATA;
volatile __LATAbits_t at 0xf89 LATAbits;

sfr at 0xf8a LATB;
volatile __LATBbits_t at 0xf8a LATBbits;

sfr at 0xf8b LATC;
volatile __LATCbits_t at 0xf8b LATCbits;

sfr at 0xf8c LATD;
volatile __LATDbits_t at 0xf8c LATDbits;

sfr at 0xf8d LATE;
volatile __LATEbits_t at 0xf8d LATEbits;

sfr at 0xf8e LATF;
volatile __LATFbits_t at 0xf8e LATFbits;

sfr at 0xf8f LATG;
volatile __LATGbits_t at 0xf8f LATGbits;

sfr at 0xf90 LATH;
volatile __LATHbits_t at 0xf90 LATHbits;

sfr at 0xf91 LATJ;
volatile __LATJbits_t at 0xf91 LATJbits;

sfr at 0xf92 TRISA;
volatile __TRISAbits_t at 0xf92 TRISAbits;

sfr at 0xf93 TRISB;
volatile __TRISBbits_t at 0xf93 TRISBbits;

sfr at 0xf94 TRISC;
volatile __TRISCbits_t at 0xf94 TRISCbits;

sfr at 0xf95 TRISD;
volatile __TRISDbits_t at 0xf95 TRISDbits;

sfr at 0xf96 TRISE;
volatile __TRISEbits_t at 0xf96 TRISEbits;

sfr at 0xf97 TRISF;
volatile __TRISFbits_t at 0xf97 TRISFbits;

sfr at 0xf98 TRISG;
volatile __TRISGbits_t at 0xf98 TRISGbits;

sfr at 0xf99 TRISH;
volatile __TRISHbits_t at 0xf99 TRISHbits;

sfr at 0xf9a TRISJ;
volatile __TRISJbits_t at 0xf9a TRISJbits;

sfr at 0xf9c MEMCON;
volatile __MEMCONbits_t at 0xf9c MEMCONbits;

sfr at 0xf9d PIE1;
volatile __PIE1bits_t at 0xf9d PIE1bits;

sfr at 0xf9e PIR1;
volatile __PIR1bits_t at 0xf9e PIR1bits;

sfr at 0xf9f IPR1;
volatile __IPR1bits_t at 0xf9f IPR1bits;

sfr at 0xfa0 PIE2;
volatile __PIE2bits_t at 0xfa0 PIE2bits;

sfr at 0xfa1 PIR2;
volatile __PIR2bits_t at 0xfa1 PIR2bits;

sfr at 0xfa2 IPR2;
volatile __IPR2bits_t at 0xfa2 IPR2bits;

sfr at 0xfa3 PIE3;
volatile __PIE3bits_t at 0xfa3 PIE3bits;

sfr at 0xfa4 PIR3;
volatile __PIR3bits_t at 0xfa4 PIR3bits;

sfr at 0xfa5 IPR3;
volatile __IPR3bits_t at 0xfa5 IPR3bits;

sfr at 0xfa6 EECON1;
volatile __EECON1bits_t at 0xfa6 EECON1bits;

sfr at 0xfa7 EECON2;
sfr at 0xfa8 EEDATA;
sfr at 0xfa9 EEADR;
sfr at 0xfaa EEADRH;
sfr at 0xfab RCSTA1;
volatile __RCSTA1bits_t at 0xfab RCSTA1bits;

sfr at 0xfac TXSTA1;
volatile __TXSTA1bits_t at 0xfac TXSTA1bits;

sfr at 0xfad TXREG1;
sfr at 0xfae RCREG1;
sfr at 0xfaf SPBRG1;
sfr at 0xfb0 PSPCON;
volatile __PSPCONbits_t at 0xfb0 PSPCONbits;

sfr at 0xfb1 T3CON;
volatile __T3CONbits_t at 0xfb1 T3CONbits;

sfr at 0xfb2 TMR3L;
sfr at 0xfb3 TMR3H;
sfr at 0xfb4 CMCON;
volatile __CMCONbits_t at 0xfb4 CMCONbits;

sfr at 0xfb5 CVRCON;
volatile __CVRCONbits_t at 0xfb5 CVRCONbits;

sfr at 0xfb7 CCP3CON;
volatile __CCP3CONbits_t at 0xfb7 CCP3CONbits;

sfr at 0xfb8 CCPR3L;
sfr at 0xfb9 CCPR3H;
sfr at 0xfba CCP2CON;
volatile __CCP2CONbits_t at 0xfba CCP2CONbits;

sfr at 0xfbb CCPR2L;
sfr at 0xfbc CCPR2H;
sfr at 0xfbd CCP1CON;
volatile __CCP1CONbits_t at 0xfbd CCP1CONbits;

sfr at 0xfbe CCPR1L;
sfr at 0xfbf CCPR1H;
sfr at 0xfc0 ADCON2;
volatile __ADCON2bits_t at 0xfc0 ADCON2bits;

sfr at 0xfc1 ADCON1;
volatile __ADCON1bits_t at 0xfc1 ADCON1bits;

sfr at 0xfc2 ADCON0;
volatile __ADCON0bits_t at 0xfc2 ADCON0bits;

sfr at 0xfc3 ADRESL;
sfr at 0xfc4 ADRESH;
sfr at 0xfc5 SSPCON2;
volatile __SSPCON2bits_t at 0xfc5 SSPCON2bits;

sfr at 0xfc6 SSPCON1;
volatile __SSPCON1bits_t at 0xfc6 SSPCON1bits;

sfr at 0xfc7 SSPSTAT;
volatile __SSPSTATbits_t at 0xfc7 SSPSTATbits;

sfr at 0xfc8 SSPADD;
sfr at 0xfc9 SSPBUF;
sfr at 0xfca T2CON;
volatile __T2CONbits_t at 0xfca T2CONbits;

sfr at 0xfcb PR2;
sfr at 0xfcc TMR2;
sfr at 0xfcd T1CON;
volatile __T1CONbits_t at 0xfcd T1CONbits;

sfr at 0xfce TMR1L;
sfr at 0xfcf TMR1H;
sfr at 0xfd0 RCON;
volatile __RCONbits_t at 0xfd0 RCONbits;

sfr at 0xfd1 WDTCON;
volatile __WDTCONbits_t at 0xfd1 WDTCONbits;

sfr at 0xfd2 LVDCON;
volatile __LVDCONbits_t at 0xfd2 LVDCONbits;

sfr at 0xfd3 OSCCON;
volatile __OSCCONbits_t at 0xfd3 OSCCONbits;

sfr at 0xfd5 T0CON;
sfr at 0xfd6 TMR0L;
sfr at 0xfd7 TMR0H;
sfr at 0xfd8 STATUS;
volatile __STATUSbits_t at 0xfd8 STATUSbits;

sfr at 0xfd9 FSR2L;
sfr at 0xfda FSR2H;
sfr at 0xfdb PLUSW2;
sfr at 0xfdc PREINC2;
sfr at 0xfdd POSTDEC2;
sfr at 0xfde POSTINC2;
sfr at 0xfdf INDF2;
sfr at 0xfe0 BSR;
sfr at 0xfe1 FSR1L;
sfr at 0xfe2 FSR1H;
sfr at 0xfe3 PLUSW1;
sfr at 0xfe4 PREINC1;
sfr at 0xfe5 POSTDEC1;
sfr at 0xfe6 POSTINC1;
sfr at 0xfe7 INDF1;
sfr at 0xfe8 WREG;
sfr at 0xfe9 FSR0L;
sfr at 0xfea FSR0H;
sfr at 0xfeb PLUSW0;
sfr at 0xfec PREINC0;
sfr at 0xfed POSTDEC0;
sfr at 0xfee POSTINC0;
sfr at 0xfef INDF0;
sfr at 0xff0 INTCON3;
volatile __INTCON3bits_t at 0xff0 INTCON3bits;

sfr at 0xff1 INTCON2;
volatile __INTCON2bits_t at 0xff1 INTCON2bits;

sfr at 0xff2 INTCON;
volatile __INTCONbits_t at 0xff2 INTCONbits;

sfr at 0xff3 PRODL;
sfr at 0xff4 PRODH;
sfr at 0xff5 TABLAT;
sfr at 0xff6 TBLPTRL;
sfr at 0xff7 TBLPTRH;
sfr at 0xff8 TBLPTRU;
sfr at 0xff9 PCL;
sfr at 0xffa PCLATH;
sfr at 0xffb PCLATU;
sfr at 0xffc STKPTR;
volatile __STKPTRbits_t at 0xffc STKPTRbits;

sfr at 0xffd TOSL;
sfr at 0xffe TOSH;
sfr at 0xfff TOSU;


