
/*
 * pic18f442.h - 18F442 Device Library Header
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

#ifndef __PIC18F442_H__
#define __PIC18F442_H__

extern sfr at 0xfff TOSU;
extern sfr at 0xffe TOSH;
extern sfr at 0xffd TOSL;
extern sfr at 0xffc STKPTR;
extern sfr at 0xffb PCLATU;
extern sfr at 0xffa PCLATH;
extern sfr at 0xff9 PCL;
extern sfr at 0xff8 TBLPTRU;
extern sfr at 0xff7 TBLPTRH;
extern sfr at 0xff6 TBLPTRL;
extern sfr at 0xff5 TABLAT;
extern sfr at 0xff4 PRODH;
extern sfr at 0xff3 PRODL;
extern sfr at 0xff2 INTCON;
extern sfr at 0xfef INDF0;
extern sfr at 0xfee POSTINC0;
extern sfr at 0xfed POSTDEC0;
extern sfr at 0xfec PREINC0;
extern sfr at 0xfeb PLUSW0;
extern sfr at 0xfea FSR0H;
extern sfr at 0xfe9 FSR0L;
extern sfr at 0xfe8 WREG;
extern sfr at 0xfe7 INDF1;
extern sfr at 0xfe6 POSTINC1;
extern sfr at 0xfe5 POSTDEC1;
extern sfr at 0xfe4 PREINC1;
extern sfr at 0xfe3 PLUSW1;
extern sfr at 0xfe2 FSR1H;
extern sfr at 0xfe1 FSR1L;
extern sfr at 0xfe0 BSR;
extern sfr at 0xfdf INDF2;
extern sfr at 0xfde POSTINC2;
extern sfr at 0xfdd POSTDEC2;
extern sfr at 0xfdc PREINC2;
extern sfr at 0xfdb PLUSW2;
extern sfr at 0xfda FSR2H;
extern sfr at 0xfd9 FSR2L;
extern sfr at 0xfd8 STATUS;
extern sfr at 0xfd3 OSCCON;
extern sfr at 0xfd2 LVDCON;
extern sfr at 0xfd1 WDTCON;
extern sfr at 0xfd0 RCON;
extern sfr at 0xf9f IPR1;
extern sfr at 0xf9e PIR1;
extern sfr at 0xf9d PIE1;
extern sfr at 0xff1 INTCON2;
extern sfr at 0xff0 INTCON3;
extern sfr at 0xfa2 IPR2;
extern sfr at 0xfa1 PIR2;
extern sfr at 0xfa0 PIE2;
extern sfr at 0xf80 PORTA;
extern sfr at 0xf89 LATA;
extern sfr at 0xf92 TRISA;
extern sfr at 0xf81 PORTB;
extern sfr at 0xf93 TRISB;
extern sfr at 0xf8a LATB;
extern sfr at 0xf82 PORTC;
extern sfr at 0xf8b LATC;
extern sfr at 0xf94 TRISC;
extern sfr at 0xf83 PORTD;
extern sfr at 0xf8c LATD;
extern sfr at 0xf95 TRISD;
extern sfr at 0xf84 PORTE;
extern sfr at 0xf8d LATE;
extern sfr at 0xf96 TRISE;
extern sfr at 0xfc4 ADRESH;
extern sfr at 0xfc3 ADRESL;
extern sfr at 0xfc2 ADCON0;
extern sfr at 0xfc1 ADCON1;
extern sfr at 0xfbf CCPR1H;
extern sfr at 0xfbe CCPR1L;
extern sfr at 0xfbd CCP1CON;
extern sfr at 0xfbc CCPR2H;
extern sfr at 0xfbb CCPR2L;
extern sfr at 0xfba CCP2CON;
extern sfr at 0xfa9 EEADR;
extern sfr at 0xfa8 EEDATA;
extern sfr at 0xfa7 EECON2;
extern sfr at 0xfa6 EECON1;
extern sfr at 0xfc9 SSPBUF;
extern sfr at 0xfc8 SSPADD;
extern sfr at 0xfc7 SSPSTAT;
extern sfr at 0xfc6 SSPCON1;
extern sfr at 0xfc5 SSPCON2;
extern sfr at 0xfd7 TMR0H;
extern sfr at 0xfd6 TMR0L;
extern sfr at 0xfd5 T0CON;
extern sfr at 0xfcf TMR1H;
extern sfr at 0xfce TMR1L;
extern sfr at 0xfcd T1CON;
extern sfr at 0xfcc TMR2;
extern sfr at 0xfcb PR2;
extern sfr at 0xfca T2CON;
extern sfr at 0xfb3 TMR3H;
extern sfr at 0xfb2 TMR3L;
extern sfr at 0xfb1 T3CON;
extern sfr at 0xfaf SPBRG;
extern sfr at 0xfae RCREG;
extern sfr at 0xfad TXREG;
extern sfr at 0xfac TXSTA;
extern sfr at 0xfab RCSTA;


#endif

