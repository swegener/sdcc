
/*
 * pic18f448.c - PIC18F448 Device Library Source
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

#include <pic18f448.h>

sfr at 0xf00 RXF0SIDH;
volatile __RXF0SIDHbits_t at 0xf00 RXF0SIDHbits;

sfr at 0xf01 RXF0SIDL;
volatile __RXF0SIDLbits_t at 0xf01 RXF0SIDLbits;

sfr at 0xf02 RXF0EIDH;
volatile __RXF0EIDHbits_t at 0xf02 RXF0EIDHbits;

sfr at 0xf03 RXF0EIDL;
volatile __RXF0EIDLbits_t at 0xf03 RXF0EIDLbits;

sfr at 0xf04 RXF1SIDH;
volatile __RXF1SIDHbits_t at 0xf04 RXF1SIDHbits;

sfr at 0xf05 RXF1SIDL;
volatile __RXF1SIDLbits_t at 0xf05 RXF1SIDLbits;

sfr at 0xf06 RXF1EIDH;
volatile __RXF1EIDHbits_t at 0xf06 RXF1EIDHbits;

sfr at 0xf07 RXF1EIDL;
volatile __RXF1EIDLbits_t at 0xf07 RXF1EIDLbits;

sfr at 0xf08 RXF2SIDH;
volatile __RXF2SIDHbits_t at 0xf08 RXF2SIDHbits;

sfr at 0xf09 RXF2SIDL;
volatile __RXF2SIDLbits_t at 0xf09 RXF2SIDLbits;

sfr at 0xf0a RXF2EIDH;
volatile __RXF2EIDHbits_t at 0xf0a RXF2EIDHbits;

sfr at 0xf0b RXF2EIDL;
volatile __RXF2EIDLbits_t at 0xf0b RXF2EIDLbits;

sfr at 0xf0c RXF3SIDH;
volatile __RXF3SIDHbits_t at 0xf0c RXF3SIDHbits;

sfr at 0xf0d RXF3SIDL;
volatile __RXF3SIDLbits_t at 0xf0d RXF3SIDLbits;

sfr at 0xf0e RXF3EIDH;
volatile __RXF3EIDHbits_t at 0xf0e RXF3EIDHbits;

sfr at 0xf0f RXF3EIDL;
volatile __RXF3EIDLbits_t at 0xf0f RXF3EIDLbits;

sfr at 0xf10 RXF4SIDH;
volatile __RXF4SIDHbits_t at 0xf10 RXF4SIDHbits;

sfr at 0xf11 RXF4SIDL;
volatile __RXF4SIDLbits_t at 0xf11 RXF4SIDLbits;

sfr at 0xf12 RXF4EIDH;
volatile __RXF4EIDHbits_t at 0xf12 RXF4EIDHbits;

sfr at 0xf13 RXF4EIDL;
volatile __RXF4EIDLbits_t at 0xf13 RXF4EIDLbits;

sfr at 0xf14 RXF5SIDH;
volatile __RXF5SIDHbits_t at 0xf14 RXF5SIDHbits;

sfr at 0xf15 RXF5SIDL;
volatile __RXF5SIDLbits_t at 0xf15 RXF5SIDLbits;

sfr at 0xf16 RXF5EIDH;
volatile __RXF5EIDHbits_t at 0xf16 RXF5EIDHbits;

sfr at 0xf17 RXF5EIDL;
volatile __RXF5EIDLbits_t at 0xf17 RXF5EIDLbits;

sfr at 0xf18 RXM0SIDH;
volatile __RXM0SIDHbits_t at 0xf18 RXM0SIDHbits;

sfr at 0xf19 RXM0SIDL;
volatile __RXM0SIDLbits_t at 0xf19 RXM0SIDLbits;

sfr at 0xf1a RXM0EIDH;
volatile __RXM0EIDHbits_t at 0xf1a RXM0EIDHbits;

sfr at 0xf1b RXM0EIDL;
volatile __RXM0EIDLbits_t at 0xf1b RXM0EIDLbits;

sfr at 0xf1c RXM1SIDH;
volatile __RXM1SIDHbits_t at 0xf1c RXM1SIDHbits;

sfr at 0xf1d RXM1SIDL;
volatile __RXM1SIDLbits_t at 0xf1d RXM1SIDLbits;

sfr at 0xf1e RXM1EIDH;
volatile __RXM1EIDHbits_t at 0xf1e RXM1EIDHbits;

sfr at 0xf1f RXM1EIDL;
volatile __RXM1EIDLbits_t at 0xf1f RXM1EIDLbits;

sfr at 0xf20 TXB2CON;
volatile __TXB2CONbits_t at 0xf20 TXB2CONbits;

sfr at 0xf21 TXB2SIDH;
volatile __TXB2SIDHbits_t at 0xf21 TXB2SIDHbits;

sfr at 0xf22 TXB2SIDL;
volatile __TXB2SIDLbits_t at 0xf22 TXB2SIDLbits;

sfr at 0xf23 TXB2EIDH;
volatile __TXB2EIDHbits_t at 0xf23 TXB2EIDHbits;

sfr at 0xf24 TXB2EIDL;
volatile __TXB2EIDLbits_t at 0xf24 TXB2EIDLbits;

sfr at 0xf25 TXB2DLC;
volatile __TXB2DLCbits_t at 0xf25 TXB2DLCbits;

sfr at 0xf26 TXB2D0;
volatile __TXB2D0bits_t at 0xf26 TXB2D0bits;

sfr at 0xf27 TXB2D1;
volatile __TXB2D1bits_t at 0xf27 TXB2D1bits;

sfr at 0xf28 TXB2D2;
volatile __TXB2D2bits_t at 0xf28 TXB2D2bits;

sfr at 0xf29 TXB2D3;
volatile __TXB2D3bits_t at 0xf29 TXB2D3bits;

sfr at 0xf2a TXB2D4;
volatile __TXB2D4bits_t at 0xf2a TXB2D4bits;

sfr at 0xf2b TXB2D5;
volatile __TXB2D5bits_t at 0xf2b TXB2D5bits;

sfr at 0xf2c TXB2D6;
volatile __TXB2D6bits_t at 0xf2c TXB2D6bits;

sfr at 0xf2d TXB2D7;
volatile __TXB2D7bits_t at 0xf2d TXB2D7bits;

sfr at 0xf2e CANSTATRO4;
volatile __CANSTATRO4bits_t at 0xf2e CANSTATRO4bits;

sfr at 0xf30 TXB1CON;
volatile __TXB1CONbits_t at 0xf30 TXB1CONbits;

sfr at 0xf31 TXB1SIDH;
volatile __TXB1SIDHbits_t at 0xf31 TXB1SIDHbits;

sfr at 0xf32 TXB1SIDL;
volatile __TXB1SIDLbits_t at 0xf32 TXB1SIDLbits;

sfr at 0xf33 TXB1EIDH;
volatile __TXB1EIDHbits_t at 0xf33 TXB1EIDHbits;

sfr at 0xf34 TXB1EIDL;
volatile __TXB1EIDLbits_t at 0xf34 TXB1EIDLbits;

sfr at 0xf35 TXB1DLC;
volatile __TXB1DLCbits_t at 0xf35 TXB1DLCbits;

sfr at 0xf36 TXB1D0;
volatile __TXB1D0bits_t at 0xf36 TXB1D0bits;

sfr at 0xf37 TXB1D1;
volatile __TXB1D1bits_t at 0xf37 TXB1D1bits;

sfr at 0xf38 TXB1D2;
volatile __TXB1D2bits_t at 0xf38 TXB1D2bits;

sfr at 0xf39 TXB1D3;
volatile __TXB1D3bits_t at 0xf39 TXB1D3bits;

sfr at 0xf3a TXB1D4;
volatile __TXB1D4bits_t at 0xf3a TXB1D4bits;

sfr at 0xf3b TXB1D5;
volatile __TXB1D5bits_t at 0xf3b TXB1D5bits;

sfr at 0xf3c TXB1D6;
volatile __TXB1D6bits_t at 0xf3c TXB1D6bits;

sfr at 0xf3d TXB1D7;
volatile __TXB1D7bits_t at 0xf3d TXB1D7bits;

sfr at 0xf3e CANSTATRO3;
volatile __CANSTATRO3bits_t at 0xf3e CANSTATRO3bits;

sfr at 0xf40 TXB0CON;
volatile __TXB0CONbits_t at 0xf40 TXB0CONbits;

sfr at 0xf41 TXB0SIDH;
volatile __TXB0SIDHbits_t at 0xf41 TXB0SIDHbits;

sfr at 0xf42 TXB0SIDL;
volatile __TXB0SIDLbits_t at 0xf42 TXB0SIDLbits;

sfr at 0xf43 TXB0EIDH;
volatile __TXB0EIDHbits_t at 0xf43 TXB0EIDHbits;

sfr at 0xf44 TXB0EIDL;
volatile __TXB0EIDLbits_t at 0xf44 TXB0EIDLbits;

sfr at 0xf45 TXB0DLC;
volatile __TXB0DLCbits_t at 0xf45 TXB0DLCbits;

sfr at 0xf46 TXB0D0;
volatile __TXB0D0bits_t at 0xf46 TXB0D0bits;

sfr at 0xf47 TXB0D1;
volatile __TXB0D1bits_t at 0xf47 TXB0D1bits;

sfr at 0xf48 TXB0D2;
volatile __TXB0D2bits_t at 0xf48 TXB0D2bits;

sfr at 0xf49 TXB0D3;
volatile __TXB0D3bits_t at 0xf49 TXB0D3bits;

sfr at 0xf4a TXB0D4;
volatile __TXB0D4bits_t at 0xf4a TXB0D4bits;

sfr at 0xf4b TXB0D5;
volatile __TXB0D5bits_t at 0xf4b TXB0D5bits;

sfr at 0xf4c TXB0D6;
volatile __TXB0D6bits_t at 0xf4c TXB0D6bits;

sfr at 0xf4d TXB0D7;
volatile __TXB0D7bits_t at 0xf4d TXB0D7bits;

sfr at 0xf4e CANSTATRO2;
volatile __CANSTATRO2bits_t at 0xf4e CANSTATRO2bits;

sfr at 0xf50 RXB1CON;
volatile __RXB1CONbits_t at 0xf50 RXB1CONbits;

sfr at 0xf51 RXB1SIDH;
volatile __RXB1SIDHbits_t at 0xf51 RXB1SIDHbits;

sfr at 0xf52 RXB1SIDL;
volatile __RXB1SIDLbits_t at 0xf52 RXB1SIDLbits;

sfr at 0xf53 RXB1EIDH;
volatile __RXB1EIDHbits_t at 0xf53 RXB1EIDHbits;

sfr at 0xf54 RXB1EIDL;
volatile __RXB1EIDLbits_t at 0xf54 RXB1EIDLbits;

sfr at 0xf55 RXB1DLC;
volatile __RXB1DLCbits_t at 0xf55 RXB1DLCbits;

sfr at 0xf56 RXB1D0;
volatile __RXB1D0bits_t at 0xf56 RXB1D0bits;

sfr at 0xf57 RXB1D1;
volatile __RXB1D1bits_t at 0xf57 RXB1D1bits;

sfr at 0xf58 RXB1D2;
volatile __RXB1D2bits_t at 0xf58 RXB1D2bits;

sfr at 0xf59 RXB1D3;
volatile __RXB1D3bits_t at 0xf59 RXB1D3bits;

sfr at 0xf5a RXB1D4;
volatile __RXB1D4bits_t at 0xf5a RXB1D4bits;

sfr at 0xf5b RXB1D5;
volatile __RXB1D5bits_t at 0xf5b RXB1D5bits;

sfr at 0xf5c RXB1D6;
volatile __RXB1D6bits_t at 0xf5c RXB1D6bits;

sfr at 0xf5d RXB1D7;
volatile __RXB1D7bits_t at 0xf5d RXB1D7bits;

sfr at 0xf5e CANSTATRO1;
volatile __CANSTATRO1bits_t at 0xf5e CANSTATRO1bits;

sfr at 0xf60 RXB0CON;
volatile __RXB0CONbits_t at 0xf60 RXB0CONbits;

sfr at 0xf61 RXB0SIDH;
volatile __RXB0SIDHbits_t at 0xf61 RXB0SIDHbits;

sfr at 0xf62 RXB0SIDL;
volatile __RXB0SIDLbits_t at 0xf62 RXB0SIDLbits;

sfr at 0xf63 RXB0EIDH;
volatile __RXB0EIDHbits_t at 0xf63 RXB0EIDHbits;

sfr at 0xf64 RXB0EIDL;
volatile __RXB0EIDLbits_t at 0xf64 RXB0EIDLbits;

sfr at 0xf65 RXB0DLC;
volatile __RXB0DLCbits_t at 0xf65 RXB0DLCbits;

sfr at 0xf66 RXB0D0;
sfr at 0xf67 RXB0D1;
sfr at 0xf68 RXB0D2;
sfr at 0xf69 RXB0D3;
sfr at 0xf6a RXB0D4;
sfr at 0xf6b RXB0D5;
sfr at 0xf6c RXB0D6;
sfr at 0xf6d RXB0D7;
sfr at 0xf6e CANSTAT;
volatile __CANSTATbits_t at 0xf6e CANSTATbits;

sfr at 0xf6f CANCON;
volatile __CANCONbits_t at 0xf6f CANCONbits;

sfr at 0xf70 BRGCON1;
volatile __BRGCON1bits_t at 0xf70 BRGCON1bits;

sfr at 0xf71 BRGCON2;
volatile __BRGCON2bits_t at 0xf71 BRGCON2bits;

sfr at 0xf72 BRGCON3;
volatile __BRGCON3bits_t at 0xf72 BRGCON3bits;

sfr at 0xf73 CIOCON;
volatile __CIOCONbits_t at 0xf73 CIOCONbits;

sfr at 0xf74 COMSTAT;
volatile __COMSTATbits_t at 0xf74 COMSTATbits;

sfr at 0xf75 RXERRCNT;
volatile __RXERRCNTbits_t at 0xf75 RXERRCNTbits;

sfr at 0xf76 TXERRCNT;
volatile __TXERRCNTbits_t at 0xf76 TXERRCNTbits;

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
sfr at 0xfab RCSTA;
volatile __RCSTAbits_t at 0xfab RCSTAbits;

sfr at 0xfac TXSTA;
volatile __TXSTAbits_t at 0xfac TXSTAbits;

sfr at 0xfad TXREG;
sfr at 0xfae RCREG;
sfr at 0xfaf SPBRG;
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

sfr at 0xfb6 ECCPAS;
volatile __ECCPASbits_t at 0xfb6 ECCPASbits;

sfr at 0xfb7 ECCP1DEL;
volatile __ECCP1DELbits_t at 0xfb7 ECCP1DELbits;

sfr at 0xfba ECCP1CON;
volatile __ECCP1CONbits_t at 0xfba ECCP1CONbits;

sfr at 0xfbb ECCPR1L;
sfr at 0xfbc ECCPR1H;
sfr at 0xfbd CCP1CON;
volatile __CCP1CONbits_t at 0xfbd CCP1CONbits;

sfr at 0xfbe CCPR1L;
sfr at 0xfbf CCPR1H;
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


