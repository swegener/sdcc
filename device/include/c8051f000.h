/*-------------------------------------------------------------------------
   Register Declarations for the Cygnal C8051F000-C8051F017 Processor Range

   Copyright (C) 2003 - Maarten Brock, sourceforge.brock@dse.nl

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
-------------------------------------------------------------------------*/

#ifndef C8051F000_H
#define C8051F000_H


/*  BYTE Registers  */
sfr at 0x80 P0       ;  /* PORT 0                                                  */
sfr at 0x81 SP       ;  /* STACK POINTER                                           */
sfr at 0x82 DPL      ;  /* DATA POINTER - LOW BYTE                                 */
sfr at 0x83 DPH      ;  /* DATA POINTER - HIGH BYTE                                */
sfr at 0x87 PCON     ;  /* POWER CONTROL                                           */
sfr at 0x88 TCON     ;  /* TIMER CONTROL                                           */
sfr at 0x89 TMOD     ;  /* TIMER MODE                                              */
sfr at 0x8A TL0      ;  /* TIMER 0 - LOW BYTE                                      */
sfr at 0x8B TL1      ;  /* TIMER 1 - LOW BYTE                                      */
sfr at 0x8C TH0      ;  /* TIMER 0 - HIGH BYTE                                     */
sfr at 0x8D TH1      ;  /* TIMER 1 - HIGH BYTE                                     */
sfr at 0x8E CKCON    ;  /* CLOCK CONTROL                                           */
sfr at 0x8F PSCTL    ;  /* PROGRAM STORE R/W CONTROL                               */
sfr at 0x90 P1       ;  /* PORT 1                                                  */
sfr at 0x91 TMR3CN   ;  /* TIMER 3 CONTROL                                         */
sfr at 0x92 TMR3RLL  ;  /* TIMER 3 RELOAD REGISTER - LOW BYTE                      */
sfr at 0x93 TMR3RLH  ;  /* TIMER 3 RELOAD REGISTER - HIGH BYTE                     */
sfr at 0x94 TMR3L    ;  /* TIMER 3 - LOW BYTE                                      */
sfr at 0x95 TMR3H    ;  /* TIMER 3 - HIGH BYTE                                     */
sfr at 0x98 SCON     ;  /* SERIAL PORT CONTROL                                     */
sfr at 0x99 SBUF     ;  /* SERIAL PORT BUFFER                                      */
sfr at 0x9A SPI0CFG  ;  /* SERIAL PERIPHERAL INTERFACE 0 CONFIGURATION             */
sfr at 0x9B SPI0DAT  ;  /* SERIAL PERIPHERAL INTERFACE 0 DATA                      */
sfr at 0x9D SPI0CKR  ;  /* SERIAL PERIPHERAL INTERFACE 0 CLOCK RATE CONTROL        */
sfr at 0x9E CPT0CN   ;  /* COMPARATOR 0 CONTROL                                    */
sfr at 0x9F CPT1CN   ;  /* COMPARATOR 1 CONTROL                                    */
sfr at 0xA0 P2       ;  /* PORT 2                                                  */
sfr at 0xA4 PRT0CF   ;  /* PORT 0 CONFIGURATION                                    */
sfr at 0xA5 PRT1CF   ;  /* PORT 1 CONFIGURATION                                    */
sfr at 0xA6 PRT2CF   ;  /* PORT 2 CONFIGURATION                                    */
sfr at 0xA7 PRT3CF   ;  /* PORT 3 CONFIGURATION                                    */
sfr at 0xA8 IE       ;  /* INTERRUPT ENABLE                                        */
sfr at 0xAD PRT1IF   ;  /* PORT 1 EXTERNAL INTERRUPT FLAGS                         */
sfr at 0xAF EMI0CN   ;  /* EXTERNAL MEMORY INTERFACE CONTROL                       */
sfr at 0xAF _XPAGE ;    /* XDATA/PDATA PAGE                                        */
sfr at 0xB0 P3       ;  /* PORT 3                                                  */
sfr at 0xB1 OSCXCN   ;  /* EXTERNAL OSCILLATOR CONTROL                             */
sfr at 0xB2 OSCICN   ;  /* INTERNAL OSCILLATOR CONTROL                             */
sfr at 0xB6 FLSCL    ;  /* FLASH MEMORY TIMING PRESCALER                           */
sfr at 0xB7 FLACL    ;  /* FLASH ACESS LIMIT                                       */
sfr at 0xB8 IP       ;  /* INTERRUPT PRIORITY                                      */
sfr at 0xBA AMX0CF   ;  /* ADC 0 MUX CONFIGURATION                                 */
sfr at 0xBB AMX0SL   ;  /* ADC 0 MUX CHANNEL SELECTION                             */
sfr at 0xBC ADC0CF   ;  /* ADC 0 CONFIGURATION                                     */
sfr at 0xBE ADC0L    ;  /* ADC 0 DATA - LOW BYTE                                   */
sfr at 0xBF ADC0H    ;  /* ADC 0 DATA - HIGH BYTE                                  */
sfr at 0xC0 SMB0CN   ;  /* SMBUS 0 CONTROL                                         */
sfr at 0xC1 SMB0STA  ;  /* SMBUS 0 STATUS                                          */
sfr at 0xC2 SMB0DAT  ;  /* SMBUS 0 DATA                                            */
sfr at 0xC3 SMB0ADR  ;  /* SMBUS 0 SLAVE ADDRESS                                   */
sfr at 0xC4 ADC0GTL  ;  /* ADC 0 GREATER-THAN REGISTER - LOW BYTE                  */
sfr at 0xC5 ADC0GTH  ;  /* ADC 0 GREATER-THAN REGISTER - HIGH BYTE                 */
sfr at 0xC6 ADC0LTL  ;  /* ADC 0 LESS-THAN REGISTER - LOW BYTE                     */
sfr at 0xC7 ADC0LTH  ;  /* ADC 0 LESS-THAN REGISTER - HIGH BYTE                    */
sfr at 0xC8 T2CON    ;  /* TIMER 2 CONTROL                                         */
sfr at 0xCA RCAP2L   ;  /* TIMER 2 CAPTURE REGISTER - LOW BYTE                     */
sfr at 0xCB RCAP2H   ;  /* TIMER 2 CAPTURE REGISTER - HIGH BYTE                    */
sfr at 0xCC TL2      ;  /* TIMER 2 - LOW BYTE                                      */
sfr at 0xCD TH2      ;  /* TIMER 2 - HIGH BYTE                                     */
sfr at 0xCF SMB0CR   ;  /* SMBUS 0 CLOCK RATE                                      */
sfr at 0xD0 PSW      ;  /* PROGRAM STATUS WORD                                     */
sfr at 0xD1 REF0CN   ;  /* VOLTAGE REFERENCE 0 CONTROL                             */
sfr at 0xD2 DAC0L    ;  /* DAC 0 REGISTER - LOW BYTE                               */
sfr at 0xD3 DAC0H    ;  /* DAC 0 REGISTER - HIGH BYTE                              */
sfr at 0xD4 DAC0CN   ;  /* DAC 0 CONTROL                                           */
sfr at 0xD5 DAC1L    ;  /* DAC 1 REGISTER - LOW BYTE                               */
sfr at 0xD6 DAC1H    ;  /* DAC 1 REGISTER - HIGH BYTE                              */
sfr at 0xD7 DAC1CN   ;  /* DAC 1 CONTROL                                           */
sfr at 0xD8 PCA0CN   ;  /* PCA 0 COUNTER CONTROL                                   */
sfr at 0xD9 PCA0MD   ;  /* PCA 0 COUNTER MODE                                      */
sfr at 0xDA PCA0CPM0 ;  /* CONTROL REGISTER FOR PCA 0 MODULE 0                     */
sfr at 0xDB PCA0CPM1 ;  /* CONTROL REGISTER FOR PCA 0 MODULE 1                     */
sfr at 0xDC PCA0CPM2 ;  /* CONTROL REGISTER FOR PCA 0 MODULE 2                     */
sfr at 0xDD PCA0CPM3 ;  /* CONTROL REGISTER FOR PCA 0 MODULE 3                     */
sfr at 0xDE PCA0CPM4 ;  /* CONTROL REGISTER FOR PCA 0 MODULE 4                     */
sfr at 0xE0 ACC      ;  /* ACCUMULATOR                                             */
sfr at 0xE1 XBR0     ;  /* DIGITAL CROSSBAR CONFIGURATION REGISTER 0               */
sfr at 0xE2 XBR1     ;  /* DIGITAL CROSSBAR CONFIGURATION REGISTER 1               */
sfr at 0xE3 XBR2     ;  /* DIGITAL CROSSBAR CONFIGURATION REGISTER 2               */
sfr at 0xE6 EIE1     ;  /* EXTERNAL INTERRUPT ENABLE 1                             */
sfr at 0xE7 EIE2     ;  /* EXTERNAL INTERRUPT ENABLE 2                             */
sfr at 0xE8 ADC0CN   ;  /* ADC 0 CONTROL                                           */
sfr at 0xE9 PCA0L    ;  /* PCA 0 TIMER - LOW BYTE                                  */
sfr at 0xEA PCA0CPL0 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 0 - LOW BYTE  */
sfr at 0xEB PCA0CPL1 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 1 - LOW BYTE  */
sfr at 0xEC PCA0CPL2 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 2 - LOW BYTE  */
sfr at 0xED PCA0CPL3 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 3 - LOW BYTE  */
sfr at 0xEE PCA0CPL4 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 4 - LOW BYTE  */
sfr at 0xEF RSTSRC   ;  /* RESET SOURCE                                            */
sfr at 0xF0 B        ;  /* B REGISTER                                              */
sfr at 0xF6 EIP1     ;  /* EXTERNAL INTERRUPT PRIORITY REGISTER 1                  */
sfr at 0xF7 EIP2     ;  /* EXTERNAL INTERRUPT PRIORITY REGISTER 2                  */
sfr at 0xF8 SPI0CN   ;  /* SERIAL PERIPHERAL INTERFACE 0 CONTROL                   */
sfr at 0xF9 PCA0H    ;  /* PCA 0 TIMER - HIGH BYTE                                 */
sfr at 0xFA PCA0CPH0 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 0 - HIGH BYTE */
sfr at 0xFB PCA0CPH1 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 1 - HIGH BYTE */
sfr at 0xFC PCA0CPH2 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 2 - HIGH BYTE */
sfr at 0xFD PCA0CPH3 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 3 - HIGH BYTE */
sfr at 0xFE PCA0CPH4 ;  /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 4 - HIGH BYTE */
sfr at 0xFF WDTCN    ;  /* WATCHDOG TIMER CONTROL                                  */


/*  BIT Registers  */

/*  P0  0x80 */
sbit at 0x87 P0_7    ;
sbit at 0x86 P0_6    ;
sbit at 0x85 P0_5    ;
sbit at 0x84 P0_4    ;
sbit at 0x83 P0_3    ;
sbit at 0x82 P0_2    ;
sbit at 0x81 P0_1    ;
sbit at 0x80 P0_0    ;

/*  TCON  0x88 */
sbit at 0x8F TF1     ;  /* TIMER 1 OVERFLOW FLAG                     */
sbit at 0x8E TR1     ;  /* TIMER 1 ON/OFF CONTROL                    */
sbit at 0x8D TF0     ;  /* TIMER 0 OVERFLOW FLAG                     */
sbit at 0x8C TR0     ;  /* TIMER 0 ON/OFF CONTROL                    */
sbit at 0x8B IE1     ;  /* EXT. INTERRUPT 1 EDGE FLAG                */
sbit at 0x8A IT1     ;  /* EXT. INTERRUPT 1 TYPE                     */
sbit at 0x89 IE0     ;  /* EXT. INTERRUPT 0 EDGE FLAG                */
sbit at 0x88 IT0     ;  /* EXT. INTERRUPT 0 TYPE                     */

/*  P1  0x90 */
sbit at 0x97 P1_7    ;
sbit at 0x96 P1_6    ;
sbit at 0x95 P1_5    ;
sbit at 0x94 P1_4    ;
sbit at 0x93 P1_3    ;
sbit at 0x92 P1_2    ;
sbit at 0x91 P1_1    ;
sbit at 0x90 P1_0    ;

/*  SCON  0x98 */
sbit at 0x9F SM0     ;  /* SERIAL MODE CONTROL BIT 0                 */
sbit at 0x9E SM1     ;  /* SERIAL MODE CONTROL BIT 1                 */
sbit at 0x9D SM2     ;  /* MULTIPROCESSOR COMMUNICATION ENABLE       */
sbit at 0x9C REN     ;  /* RECEIVE ENABLE                            */
sbit at 0x9B TB8     ;  /* TRANSMIT BIT 8                            */
sbit at 0x9A RB8     ;  /* RECEIVE BIT 8                             */
sbit at 0x99 TI      ;  /* TRANSMIT INTERRUPT FLAG                   */
sbit at 0x98 RI      ;  /* RECEIVE INTERRUPT FLAG                    */

/*  P2  0xA0 */
sbit at 0xA7 P2_7    ;
sbit at 0xA6 P2_6    ;
sbit at 0xA5 P2_5    ;
sbit at 0xA4 P2_4    ;
sbit at 0xA3 P2_3    ;
sbit at 0xA2 P2_2    ;
sbit at 0xA1 P2_1    ;
sbit at 0xA0 P2_0    ;

/*  IE  0xA8 */
sbit at 0xAF EA      ;  /* GLOBAL INTERRUPT ENABLE                   */
sbit at 0xAD ET2     ;  /* TIMER 2 INTERRUPT ENABLE                  */
sbit at 0xAC ES      ;  /* SERIAL PORT INTERRUPT ENABLE              */
sbit at 0xAB ET1     ;  /* TIMER 1 INTERRUPT ENABLE                  */
sbit at 0xAA EX1     ;  /* EXTERNAL INTERRUPT 1 ENABLE               */
sbit at 0xA9 ET0     ;  /* TIMER 0 INTERRUPT ENABLE                  */
sbit at 0xA8 EX0     ;  /* EXTERNAL INTERRUPT 0 ENABLE               */

/*  P3  0xB0 */
sbit at 0xB7 P3_7    ;
sbit at 0xB6 P3_6    ;
sbit at 0xB5 P3_5    ;
sbit at 0xB4 P3_4    ;
sbit at 0xB3 P3_3    ;
sbit at 0xB2 P3_2    ;
sbit at 0xB1 P3_1    ;
sbit at 0xB0 P3_0    ;

/*  IP  0xB8 */
sbit at 0xBD PT2     ;  /* TIMER 2 PRIORITY                          */
sbit at 0xBC PS      ;  /* SERIAL PORT PRIORITY                      */
sbit at 0xBB PT1     ;  /* TIMER 1 PRIORITY                          */
sbit at 0xBA PX1     ;  /* EXTERNAL INTERRUPT 1 PRIORITY             */
sbit at 0xB9 PT0     ;  /* TIMER 0 PRIORITY                          */
sbit at 0xB8 PX0     ;  /* EXTERNAL INTERRUPT 0 PRIORITY             */

/*  SMB0CN  0xC0 */
sbit at 0xC7 BUSY    ;  /* SMBUS 0 BUSY                              */
sbit at 0xC6 ENSMB   ;  /* SMBUS 0 ENABLE                            */
sbit at 0xC5 STA     ;  /* SMBUS 0 START FLAG                        */
sbit at 0xC4 STO     ;  /* SMBUS 0 STOP FLAG                         */
sbit at 0xC3 SI      ;  /* SMBUS 0 INTERRUPT PENDING FLAG            */
sbit at 0xC2 AA      ;  /* SMBUS 0 ASSERT/ACKNOWLEDGE FLAG           */
sbit at 0xC1 SMBFTE  ;  /* SMBUS 0 FREE TIMER ENABLE                 */
sbit at 0xC0 SMBTOE  ;  /* SMBUS 0 TIMEOUT ENABLE                    */

/*  T2CON  0xC8 */
sbit at 0xCF TF2     ;  /* TIMER 2 OVERFLOW FLAG                     */
sbit at 0xCE EXF2    ;  /* EXTERNAL FLAG                             */
sbit at 0xCD RCLK    ;  /* RECEIVE CLOCK FLAG                        */
sbit at 0xCC TCLK    ;  /* TRANSMIT CLOCK FLAG                       */
sbit at 0xCB EXEN2   ;  /* TIMER 2 EXTERNAL ENABLE FLAG              */
sbit at 0xCA TR2     ;  /* TIMER 2 ON/OFF CONTROL                    */
sbit at 0xC9 CT2     ;  /* TIMER OR COUNTER SELECT                   */
sbit at 0xC8 CPRL2   ;  /* CAPTURE OR RELOAD SELECT                  */

/*  PSW  0xD0 */
sbit at 0xD7 CY      ;  /* CARRY FLAG                                */
sbit at 0xD6 AC      ;  /* AUXILIARY CARRY FLAG                      */
sbit at 0xD5 F0      ;  /* USER FLAG 0                               */
sbit at 0xD4 RS1     ;  /* REGISTER BANK SELECT 1                    */
sbit at 0xD3 RS0     ;  /* REGISTER BANK SELECT 0                    */
sbit at 0xD2 OV      ;  /* OVERFLOW FLAG                             */
sbit at 0xD1 F1      ;  /* USER FLAG 1                               */
sbit at 0xD0 P       ;  /* ACCUMULATOR PARITY FLAG                   */

/*  PCA0CN  0xD8H */
sbit at 0xDF CF      ;  /* PCA 0 COUNTER OVERFLOW FLAG               */
sbit at 0xDE CR      ;  /* PCA 0 COUNTER RUN CONTROL BIT             */
sbit at 0xDC CCF4    ;  /* PCA 0 MODULE 4 INTERRUPT FLAG             */
sbit at 0xDB CCF3    ;  /* PCA 0 MODULE 3 INTERRUPT FLAG             */
sbit at 0xDA CCF2    ;  /* PCA 0 MODULE 2 INTERRUPT FLAG             */
sbit at 0xD9 CCF1    ;  /* PCA 0 MODULE 1 INTERRUPT FLAG             */
sbit at 0xD8 CCF0    ;  /* PCA 0 MODULE 0 INTERRUPT FLAG             */

/*  ADC0CN  0xE8H */
sbit at 0xEF AD0EN   ;  /* ADC 0 ENABLE                              */
sbit at 0xEE AD0TM   ;  /* ADC 0 TRACK MODE                          */
sbit at 0xED AD0INT  ;  /* ADC 0 CONVERISION COMPLETE INTERRUPT FLAG */
sbit at 0xEC AD0BUSY ;  /* ADC 0 BUSY FLAG                           */
sbit at 0xEB ADSTM1  ;  /* ADC 0 START OF CONVERSION MODE BIT 1      */
sbit at 0xEA ADSTM0  ;  /* ADC 0 START OF CONVERSION MODE BIT 0      */
sbit at 0xE9 AD0WINT ;  /* ADC 0 WINDOW COMPARE INTERRUPT FLAG       */
sbit at 0xE8 ADLJST  ;  /* ADC 0 RIGHT JUSTIFY DATA BIT              */

/*  SPI0CN  0xF8H */
sbit at 0xFF SPIF    ;  /* SPI 0 INTERRUPT FLAG                      */
sbit at 0xFE WCOL    ;  /* SPI 0 WRITE COLLISION FLAG                */
sbit at 0xFD MODF    ;  /* SPI 0 MODE FAULT FLAG                     */
sbit at 0xFC RXOVRN  ;  /* SPI 0 RX OVERRUN FLAG                     */
sbit at 0xFB TXBSY   ;  /* SPI 0 TX BUSY FLAG                        */
sbit at 0xFA SLVSEL  ;  /* SPI 0 SLAVE SELECT                        */
sbit at 0xF9 MSTEN   ;  /* SPI 0 MASTER ENABLE                       */
sbit at 0xF8 SPIEN   ;  /* SPI 0 SPI ENABLE                          */


/* Predefined SFR Bit Masks */

#define IDLE    0x01    /* PCON */
#define STOP    0x02    /* PCON */
#define TF3     0x80    /* TMR3CN */
#define CPFIF   0x10    /* CPTnCN */
#define CPRIF   0x20    /* CPTnCN */
#define CPOUT   0x40    /* CPTnCN */
#define ECCF    0x01    /* PCA0CPMn */
#define PWM     0x02    /* PCA0CPMn */
#define TOG     0x04    /* PCA0CPMn */
#define MAT     0x08    /* PCA0CPMn */
#define CAPN    0x10    /* PCA0CPMn */
#define CAPP    0x20    /* PCA0CPMn */
#define ECOM    0x40    /* PCA0CPMn */

#endif
