/*****************************************************************************
|*
|*  MODULE:            regc515c.h
|*
 ****************************************************************************/

#ifndef _REGC515C_H
#define _REGC515C_H

/* Byte registers in numerical order */

sfr at 0x80 P0;
sfr at 0x81 SP;
sfr at 0x82 DPL;
sfr at 0x83 DPH;
sfr at 0x86 WDTREL;
sfr at 0x87 PCON;
sfr at 0x88 TCON;
sfr at 0x88 PCON1;
sfr at 0x89 TMOD;
sfr at 0x8A TL0;
sfr at 0x8B TL1;
sfr at 0x8C TH0;
sfr at 0x8D TH1;
sfr at 0x90 P1;
sfr at 0x91 XPAGE;
sfr at 0x92 DPSEL;
sfr at 0x93 SSCCON;
sfr at 0x94 STB;
sfr at 0x95 SRB;
sfr at 0x96 SSCMOD;
sfr at 0x98 SCON;
sfr at 0x99 SBUF;
sfr at 0x9A IEN2;
sfr at 0xA0 P2;
sfr at 0xA8 IEN0;
sfr at 0xA9 IP0;
sfr at 0xAA SRELL;
sfr at 0xAB SCF;
sfr at 0xAC SCIEN;
sfr at 0xB0 P3;
sfr at 0xB1 SYSCON;
sfr at 0xB8 IEN1;
sfr at 0xB9 IP1;
sfr at 0xBA SRELH;
sfr at 0xC0 IRCON;
sfr at 0xC1 CCEN;
sfr at 0xC2 CCL1;
sfr at 0xC3 CCH1;
sfr at 0xC4 CCL2;
sfr at 0xC5 CCH2;
sfr at 0xC6 CCL3;
sfr at 0xC7 CCH3;
sfr at 0xC8 T2CON;
sfr at 0xCA CRCL;
sfr at 0xCB CRCH;
sfr at 0xCC TL2;
sfr at 0xCD TH2;
sfr at 0xD0 PSW;
sfr at 0xD8 ADCON0;
sfr at 0xD9 ADDATH;
sfr at 0xDA ADDATL;
sfr at 0xDB P6;
sfr at 0xDC ADCON1;
sfr at 0xE0 ACC;
sfr at 0xE8 P4;
sfr at 0xF0 B;
sfr at 0xF8 P5;
sfr at 0xF8 DIR5;
sfr at 0xFA P7;


/* defining bits in SFR P0 */
sbit at 0x80  P0_0;
sbit at 0x81  P0_1;
sbit at 0x82  P0_2;
sbit at 0x83  P0_3;
sbit at 0x84  P0_4;
sbit at 0x85  P0_5;
sbit at 0x86  P0_6;
sbit at 0x87  P0_7;



/* defining bits in SFR PCON1 */
sbit at 0x88  IT0;
sbit at 0x89  IE0;
sbit at 0x8a  IT1;
sbit at 0x8b  IE1;
sbit at 0x8c  TR0;
sbit at 0x8d  TF0;
sbit at 0x8e  TR1;
sbit at 0x8f  TF1;
sbit at 0x8f  EWPD;



/* defining bits in SFR P1 */
sbit at 0x90  P1_0;
sbit at 0x90  INT3;
sbit at 0x91  P1_1;
sbit at 0x91  INT4;
sbit at 0x92  P1_2;
sbit at 0x92  INT5;
sbit at 0x93  P1_3;
sbit at 0x93  INT6;
sbit at 0x94  P1_4;
sbit at 0x94  INT2;
sbit at 0x95  P1_5;
sbit at 0x95  T2EX;
sbit at 0x96  P1_6;
sbit at 0x96  CLKOUT;
sbit at 0x97  P1_7;
sbit at 0x97  T2;



/* defining bits in SFR SCON */
sbit at 0x98  RI;
sbit at 0x99  TI;
sbit at 0x9a  RB8;
sbit at 0x9b  TB8;
sbit at 0x9c  REN;
sbit at 0x9d  SM2;
sbit at 0x9e  SM1;
sbit at 0x9f  SM0;



/* defining bits in SFR P2 */
sbit at 0xa0  P2_0;
sbit at 0xa1  P2_1;
sbit at 0xa2  P2_2;
sbit at 0xa3  P2_3;
sbit at 0xa4  P2_4;
sbit at 0xa5  P2_5;
sbit at 0xa6  P2_6;
sbit at 0xa7  P2_7;



/* defining bits in SFR IEN0 */
sbit at 0xa8  EX0;
sbit at 0xa9  ET0;
sbit at 0xaa  EX1;
sbit at 0xab  ET1;
sbit at 0xac  ES;
sbit at 0xad  ET2;
sbit at 0xae  WDT;
sbit at 0xaf  EA;



/* defining bits in SFR P3 */
sbit at 0xb0  P3_0;
sbit at 0xb0  RXD;
sbit at 0xb1  P3_1;
sbit at 0xb1  TXD;
sbit at 0xb2  P3_2;
sbit at 0xb2  INT0;
sbit at 0xb3  P3_3;
sbit at 0xb3  INT1;
sbit at 0xb4  P3_4;
sbit at 0xb4  T0;
sbit at 0xb5  P3_5;
sbit at 0xb5  T1;
sbit at 0xb6  P3_6;
sbit at 0xb6  WR;
sbit at 0xb7  P3_7;
sbit at 0xb7  RD;



/* defining bits in SFR IEN1 */
sbit at 0xb8  EADC;
sbit at 0xb9  EX2;
sbit at 0xba  EX3;
sbit at 0xbb  EX4;
sbit at 0xbc  EX5;
sbit at 0xbd  EX6;
sbit at 0xbe  SWDT;
sbit at 0xbf  EXEN2;



/* defining bits in SFR IRCON */
sbit at 0xc0  IADC;
sbit at 0xc1  IEX2;
sbit at 0xc2  IEX3;
sbit at 0xc3  IEX4;
sbit at 0xc4  IEX5;
sbit at 0xc5  IEX6;
sbit at 0xc6  TF2;
sbit at 0xc7  EXF2;



/* defining bits in SFR T2CON */
sbit at 0xc8  T2I0;
sbit at 0xc9  T2I1;
sbit at 0xca  T2CM;
sbit at 0xcb  T2R0;
sbit at 0xcc  T2R1;
sbit at 0xcd  I2FR;
sbit at 0xce  I3FR;
sbit at 0xcf  T2PS;



/* defining bits in SFR PSW */
sbit at 0xd0  P;
sbit at 0xd1  F1;
sbit at 0xd2  OV;
sbit at 0xd3  RS0;
sbit at 0xd4  RS1;
sbit at 0xd5  F0;
sbit at 0xd6  AC;
sbit at 0xd7  CY;



/* defining bits in SFR ADCON0 */
sbit at 0xd8  MX0;
sbit at 0xd9  MX1;
sbit at 0xda  MX2;
sbit at 0xdb  ADM;
sbit at 0xdc  BSY;
sbit at 0xdd  ADEX;
sbit at 0xde  CLK;
sbit at 0xdf  BD;



/* defining bits in SFR ACC */
sbit at 0xe0  ACC_0;
sbit at 0xe1  ACC_1;
sbit at 0xe2  ACC_2;
sbit at 0xe3  ACC_3;
sbit at 0xe4  ACC_4;
sbit at 0xe5  ACC_5;
sbit at 0xe6  ACC_6;
sbit at 0xe7  ACC_7;



/* defining bits in SFR P4 */
sbit at 0xe8  P4_0;
sbit at 0xe8  ADST;
sbit at 0xe9  P4_1;
sbit at 0xe9  SCLK;
sbit at 0xea  P4_2;
sbit at 0xea  SRI;
sbit at 0xeb  P4_3;
sbit at 0xeb  STO;
sbit at 0xec  P4_4;
sbit at 0xec  SLS;
sbit at 0xed  P4_5;
sbit at 0xed  INT8;
sbit at 0xee  P4_6;
sbit at 0xee  TXDC;
sbit at 0xef  P4_7;
sbit at 0xef  RXDC;



/* defining bits in SFR B */
sbit at 0xf0  B_0;
sbit at 0xf1  B_1;
sbit at 0xf2  B_2;
sbit at 0xf3  B_3;
sbit at 0xf4  B_4;
sbit at 0xf5  B_5;
sbit at 0xf6  B_6;
sbit at 0xf7  B_7;



/* defining bits in SFR DIR5 */
sbit at 0xf8  P5_0;
sbit at 0xf9  P5_1;
sbit at 0xfa  P5_2;
sbit at 0xfb  P5_3;
sbit at 0xfc  P5_4;
sbit at 0xfd  P5_5;
sbit at 0xfe  P5_6;
sbit at 0xff  P5_7;
sbit at 0xf8  DIR5_0;
sbit at 0xf9  DIR5_1;
sbit at 0xfa  DIR5_2;
sbit at 0xfb  DIR5_3;
sbit at 0xfc  DIR5_4;
sbit at 0xfd  DIR5_5;
sbit at 0xfe  DIR5_6;
sbit at 0xff  DIR5_7;

#endif /* _REGC515C_H */


