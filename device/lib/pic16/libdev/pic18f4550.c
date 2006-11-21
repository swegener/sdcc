/* 
 * pic18f4550.h - PIC18F4550 Device Library Sources
 * 
 * This file is part of the GNU PIC Library.
 * 
 * July, 2006
 * The GNU PIC Library is maintained by
 *     Raphael Neider <rneider AT web.de>
 * 
 * originally designed by
 *     Vangelis Rokas <vrokas@otenet.gr>
 * 
 * $Id$
 * 
 */

#include <pic18f4550.h>

__sfr __at (0xF62) SPPDATA;
volatile __SPPDATA_t __at (0xF62) SPPDATAbits;

__sfr __at (0xF63) SPPCFG;
volatile __SPPCFG_t __at (0xF63) SPPCFGbits;

__sfr __at (0xF64) SPPEPS;
volatile __SPPEPS_t __at (0xF64) SPPEPSbits;

__sfr __at (0xF65) SPPCON;
volatile __SPPCON_t __at (0xF65) SPPCONbits;

__sfr __at (0xF66) UFRML;
volatile __UFRML_t __at (0xF66) UFRMLbits;

__sfr __at (0xF67) UFRMH;
volatile __UFRMH_t __at (0xF67) UFRMHbits;

__sfr __at (0xF68) UIR;
volatile __UIR_t __at (0xF68) UIRbits;

__sfr __at (0xF69) UIE;
volatile __UIE_t __at (0xF69) UIEbits;

__sfr __at (0xF6A) UEIR;
volatile __UEIR_t __at (0xF6A) UEIRbits;

__sfr __at (0xF6B) UEIE;
volatile __UEIE_t __at (0xF6B) UEIEbits;

__sfr __at (0xF6C) USTAT;
volatile __USTAT_t __at (0xF6C) USTATbits;

__sfr __at (0xF6D) UCON;
volatile __UCON_t __at (0xF6D) UCONbits;

__sfr __at (0xF6E) UADDR;
volatile __UADDR_t __at (0xF6E) UADDRbits;

__sfr __at (0xF6F) UCFG;
volatile __UCFG_t __at (0xF6F) UCFGbits;

__sfr __at (0xF70) UEP0;
volatile __UEP0_t __at (0xF70) UEP0bits;

__sfr __at (0xF71) UEP1;
volatile __UEP1_t __at (0xF71) UEP1bits;

__sfr __at (0xF72) UEP2;
volatile __UEP2_t __at (0xF72) UEP2bits;

__sfr __at (0xF73) UEP3;
volatile __UEP3_t __at (0xF73) UEP3bits;

__sfr __at (0xF74) UEP4;
volatile __UEP4_t __at (0xF74) UEP4bits;

__sfr __at (0xF75) UEP5;
volatile __UEP5_t __at (0xF75) UEP5bits;

__sfr __at (0xF76) UEP6;
volatile __UEP6_t __at (0xF76) UEP6bits;

__sfr __at (0xF77) UEP7;
volatile __UEP7_t __at (0xF77) UEP7bits;

__sfr __at (0xF78) UEP8;
volatile __UEP8_t __at (0xF78) UEP8bits;

__sfr __at (0xF79) UEP9;
volatile __UEP9_t __at (0xF79) UEP9bits;

__sfr __at (0xF7A) UEP10;
volatile __UEP10_t __at (0xF7A) UEP10bits;

__sfr __at (0xF7B) UEP11;
volatile __UEP11_t __at (0xF7B) UEP11bits;

__sfr __at (0xF7C) UEP12;
volatile __UEP12_t __at (0xF7C) UEP12bits;

__sfr __at (0xF7D) UEP13;
volatile __UEP13_t __at (0xF7D) UEP13bits;

__sfr __at (0xF7E) UEP14;
volatile __UEP14_t __at (0xF7E) UEP14bits;

__sfr __at (0xF7F) UEP15;
volatile __UEP15_t __at (0xF7F) UEP15bits;

__sfr __at (0xF80) PORTA;
volatile __PORTA_t __at (0xF80) PORTAbits;

__sfr __at (0xF81) PORTB;
volatile __PORTB_t __at (0xF81) PORTBbits;

__sfr __at (0xF82) PORTC;
volatile __PORTC_t __at (0xF82) PORTCbits;

__sfr __at (0xF83) PORTD;
volatile __PORTD_t __at (0xF83) PORTDbits;

__sfr __at (0xF84) PORTE;
volatile __PORTE_t __at (0xF84) PORTEbits;

__sfr __at (0xF89) LATA;
volatile __LATA_t __at (0xF89) LATAbits;

__sfr __at (0xF8A) LATB;
volatile __LATB_t __at (0xF8A) LATBbits;

__sfr __at (0xF8B) LATC;
volatile __LATC_t __at (0xF8B) LATCbits;

__sfr __at (0xF8C) LATD;
volatile __LATD_t __at (0xF8C) LATDbits;

__sfr __at (0xF8D) LATE;
volatile __LATE_t __at (0xF8D) LATEbits;

__sfr __at (0xF92) TRISA;
volatile __TRISA_t __at (0xF92) TRISAbits;

__sfr __at (0xF93) TRISB;
volatile __TRISB_t __at (0xF93) TRISBbits;

__sfr __at (0xF94) TRISC;
volatile __TRISC_t __at (0xF94) TRISCbits;

__sfr __at (0xF95) TRISD;
volatile __TRISD_t __at (0xF95) TRISDbits;

__sfr __at (0xF96) TRISE;
volatile __TRISE_t __at (0xF96) TRISEbits;

__sfr __at (0xF9B) OSCTUNE;
volatile __OSCTUNE_t __at (0xF9B) OSCTUNEbits;

__sfr __at (0xF9D) PIE1;
volatile __PIE1_t __at (0xF9D) PIE1bits;

__sfr __at (0xF9E) PIR1;
volatile __PIR1_t __at (0xF9E) PIR1bits;

__sfr __at (0xF9F) IPR1;
volatile __IPR1_t __at (0xF9F) IPR1bits;

__sfr __at (0xFA0) PIE2;
volatile __PIE2_t __at (0xFA0) PIE2bits;

__sfr __at (0xFA1) PIR2;
volatile __PIR2_t __at (0xFA1) PIR2bits;

__sfr __at (0xFA2) IPR2;
volatile __IPR2_t __at (0xFA2) IPR2bits;

__sfr __at (0xFA6) EECON1;
volatile __EECON1_t __at (0xFA6) EECON1bits;

__sfr __at (0xFA7) EECON2;

__sfr __at (0xFA8) EEDATA;

__sfr __at (0xFA9) EEADR;

__sfr __at (0xFAB) RCSTA;
volatile __RCSTA_t __at (0xFAB) RCSTAbits;

__sfr __at (0xFAC) TXSTA;
volatile __TXSTA_t __at (0xFAC) TXSTAbits;

__sfr __at (0xFAD) TXREG;

__sfr __at (0xFAE) RCREG;

__sfr __at (0xFAF) SPBRG;

__sfr __at (0xFB0) SPBRGH;

__sfr __at (0xFB1) T3CON;
volatile __T3CON_t __at (0xFB1) T3CONbits;

__sfr __at (0xFB2) TMR3L;

__sfr __at (0xFB3) TMR3H;

__sfr __at (0xFB4) CMCON;
volatile __CMCON_t __at (0xFB4) CMCONbits;

__sfr __at (0xFB5) CVRCON;
volatile __CVRCON_t __at (0xFB5) CVRCONbits;

__sfr __at (0xFB6) ECCP1AS;
volatile __ECCP1AS_t __at (0xFB6) ECCP1ASbits;

__sfr __at (0xFB7) ECCP1DEL;
volatile __ECCP1DEL_t __at (0xFB7) ECCP1DELbits;

__sfr __at (0xFB8) BAUDCON;
volatile __BAUDCON_t __at (0xFB8) BAUDCONbits;

__sfr __at (0xFBA) CCP2CON;
volatile __CCP2CON_t __at (0xFBA) CCP2CONbits;

__sfr __at (0xFBB) CCPR2L;

__sfr __at (0xFBC) CCPR2H;

__sfr __at (0xFBD) CCP1CON;
volatile __CCP1CON_t __at (0xFBD) CCP1CONbits;

__sfr __at (0xFBE) CCPR1L;

__sfr __at (0xFBF) CCPR1H;

__sfr __at (0xFC0) ADCON2;
volatile __ADCON2_t __at (0xFC0) ADCON2bits;

__sfr __at (0xFC1) ADCON1;
volatile __ADCON1_t __at (0xFC1) ADCON1bits;

__sfr __at (0xFC2) ADCON0;
volatile __ADCON0_t __at (0xFC2) ADCON0bits;

__sfr __at (0xFC3) ADRESL;

__sfr __at (0xFC4) ADRESH;

__sfr __at (0xFC5) SSPCON2;
volatile __SSPCON2_t __at (0xFC5) SSPCON2bits;

__sfr __at (0xFC6) SSPCON1;
volatile __SSPCON1_t __at (0xFC6) SSPCON1bits;

__sfr __at (0xFC7) SSPSTAT;
volatile __SSPSTAT_t __at (0xFC7) SSPSTATbits;

__sfr __at (0xFC8) SSPADD;

__sfr __at (0xFC9) SSPBUF;

__sfr __at (0xFCA) T2CON;
volatile __T2CON_t __at (0xFCA) T2CONbits;

__sfr __at (0xFCB) PR2;

__sfr __at (0xFCC) TMR2;

__sfr __at (0xFCD) T1CON;
volatile __T1CON_t __at (0xFCD) T1CONbits;

__sfr __at (0xFCE) TMR1L;

__sfr __at (0xFCF) TMR1H;

__sfr __at (0xFD0) RCON;
volatile __RCON_t __at (0xFD0) RCONbits;

__sfr __at (0xFD1) WDTCON;
volatile __WDTCON_t __at (0xFD1) WDTCONbits;

__sfr __at (0xFD2) HLVDCON;
volatile __HLVDCON_t __at (0xFD2) HLVDCONbits;

__sfr __at (0xFD3) OSCCON;
volatile __OSCCON_t __at (0xFD3) OSCCONbits;

__sfr __at (0xFD5) T0CON;
volatile __T0CON_t __at (0xFD5) T0CONbits;

__sfr __at (0xFD6) TMR0L;

__sfr __at (0xFD7) TMR0H;

__sfr __at (0xFD8) STATUS;
volatile __STATUS_t __at (0xFD8) STATUSbits;

__sfr __at (0xFD9) FSR2L;

__sfr __at (0xFDA) FSR2H;
volatile __FSR2H_t __at (0xFDA) FSR2Hbits;

__sfr __at (0xFDB) PLUSW2;

__sfr __at (0xFDC) PREINC2;

__sfr __at (0xFDD) POSTDEC2;

__sfr __at (0xFDE) POSTINC2;

__sfr __at (0xFDF) INDF2;

__sfr __at (0xFE0) BSR;
volatile __BSR_t __at (0xFE0) BSRbits;

__sfr __at (0xFE1) FSR1L;

__sfr __at (0xFE2) FSR1H;
volatile __FSR1H_t __at (0xFE2) FSR1Hbits;

__sfr __at (0xFE3) PLUSW1;

__sfr __at (0xFE4) PREINC1;

__sfr __at (0xFE5) POSTDEC1;

__sfr __at (0xFE6) POSTINC1;

__sfr __at (0xFE7) INDF1;

__sfr __at (0xFE8) WREG;

__sfr __at (0xFE9) FSR0L;

__sfr __at (0xFEA) FSR0H;
volatile __FSR0H_t __at (0xFEA) FSR0Hbits;

__sfr __at (0xFEB) PLUSW0;

__sfr __at (0xFEC) PREINC0;

__sfr __at (0xFED) POSTDEC0;

__sfr __at (0xFEE) POSTINC0;

__sfr __at (0xFEF) INDF0;

__sfr __at (0xFF0) INTCON3;
volatile __INTCON3_t __at (0xFF0) INTCON3bits;

__sfr __at (0xFF1) INTCON2;
volatile __INTCON2_t __at (0xFF1) INTCON2bits;

__sfr __at (0xFF2) INTCON;
volatile __INTCON_t __at (0xFF2) INTCONbits;

__sfr __at (0xFF3) PRODL;

__sfr __at (0xFF4) PRODH;

__sfr __at (0xFF5) TABLAT;

__sfr __at (0xFF6) TBLPTRL;

__sfr __at (0xFF7) TBLPTRH;

__sfr __at (0xFF8) TBLPTRU;
volatile __TBLPTRU_t __at (0xFF8) TBLPTRUbits;

__sfr __at (0xFF9) PCL;

__sfr __at (0xFFA) PCLATH;
volatile __PCLATH_t __at (0xFFA) PCLATHbits;

__sfr __at (0xFFB) PCLATU;
volatile __PCLATU_t __at (0xFFB) PCLATUbits;

__sfr __at (0xFFC) STKPTR;
volatile __STKPTR_t __at (0xFFC) STKPTRbits;

__sfr __at (0xFFD) TOSL;

__sfr __at (0xFFE) TOSH;

__sfr __at (0xFFF) TOSU;
volatile __TOSU_t __at (0xFFF) TOSUbits;

