/*-------------------------------------------------------------------------
   Register Declarations for the mcs51 compatible microcontrollers

   Written By -  Bela Torok / bela.torok@kssg.ch (November 2000)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!


   History:
   --------
   Version 1.0 Nov 2, 2000 - B. Torok  / bela.torok@kssg.ch
   Initial release, supported microcontrollers:
   8051, 8052, Atmel AT89C1051, AT89C2051, AT89C4051,
   Siemens SAB80515, SAB80535, SAB80515A

   Version 1.0.1
   SAB80515A definitions revised by Michael Schmitt / michael.schmitt@t-online.de

   Adding support for additional microcontrollers:
   -----------------------------------------------

   1. Make an entry with the inventory of the register set of
      the microcontroller in the  "Describe microcontrollers" section.

   2. If necessary add/modify entry(s) in the "Register definitions" section

   3. If necessary add/modify entry(s) in the "Interrupt vectors" section

   4. Make a step-by-step protocol of your modifications

   5. Send the protocol and the modified file to me ( bela.torok@kssg.ch ).
      I'm going to compile/verify changes made by different authors.


   Microcontroller support:

   Use one of the following options:

   1. use #include <mcs51reg.h> in your program & define REG_XXXXXX in your makefile.

   2. use following definitions prior the
      #include <mcs51reg.h> line in your program:
      e.g.:
      #define REG_8052_H       -> 8052 type microcontroller
      or
      #define REG_AT89CX051_H  -> Atmel AT89C1051, AT89C2051 and AT89C4051 microcontrollers


   Use only one of the following definitions!!!

   Supported Microcontrollers:

   No definition      8051
   REG_8051_H         8051
   REG_8052_H         8052
   REG_AT89CX051_H    Atmel AT89C1051, AT89C2051 and AT89C4051
   REG_SAB80515_H     Siemens SAB80515 & SAB80535
   REG_SAB80515A_H    Siemens SAB80515A

-----------------------------------------------------------------------*/


#ifndef HEADER_MCS51REG
#define HEADER_MCS51REG

//////////////////////////////////
///  Describe microcontrollers ///
///  (inventory of registers)  ///
//////////////////////////////////

// definitions for the 8051
#ifdef REG_8051_H
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: 8051

#define P0
#define SP
#define DPL
#define DPH
#define PCON
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define P2
#define IE
#define P3
#define IP
#define PSW
#define ACC
#define B

#endif
// end of definitions for the 8051


// definitions for the 8052 microcontroller
#ifdef REG_8052_H
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: 8052

// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define P2
#define IE
#define P3
#define IP
#define PSW
#define ACC
#define B

// 8052 specific registers
#define T2CON
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2

#endif
// end of definitions for the 8052 microcontroller

// definitionsons for the Atmel
// AT89C1051, AT89C2051 and AT89C4051 microcontrollers
#ifdef REG_AT89CX051_H
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Atmel AT89Cx051

// 8051 register set without P0 & P2
#define SP
#define DPL
#define DPH
#define PCON
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define IE
#define P3
#define IP
#define PSW
#define ACC
#define B

#endif
// end of definitionsons for the Atmel
// AT89C1051, AT89C2051 and AT89C4051 microcontrollers

// definitions for the Siemens SAB80515 & SAB80535
#ifdef REG_SAB80515_H
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Siemens SAB80515 & SAB80535

// 8051 register set without IE, IP & PCON
#define P0
#define SP
#define DPL
#define DPH
#define PCON
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define P2
#define P3
#define PSW
#define ACC
#define B

// SAB80515 specific registers
#define IE_SAB80515_FAMILY
#define IP0         ; // interrupt priority register - SAB80515 specific
#define IEN1        ; // interrupt enable register - SAB80515 specific
#define IRCON       ; // interrupt control register - SAB80515 specific
#define CCEN        ; // compare/capture enable register
#define CCL1        ; // compare/capture register 1, low byte
#define CCH1        ; // compare/capture register 1, high byte
#define CCL2        ; // compare/capture register 2, low byte
#define CCH2        ; // compare/capture register 2, high byte
#define CCL3        ; // compare/capture register 3, low byte
#define CCH3        ; // compare/capture register 3, high byte
#define T2CON       ;
#define CRCL        ; // compare/reload/capture register, low byte
#define CRCH        ; // compare/reload/capture register, high byte
#define TL2         ;
#define TH2         ;
#define ADCON       ; // A/D-converter control register
#define ADDAT       ; // A/D-converter data register
#define DAPR        ; // D/A-converter program register
#define P4          ; // Port 4 - SAB80515 specific
#define P5          ; // Port 5 - SAB80515 specific
#define PCON_SAB80515 ; // PCON of the SAB80515

#endif
// end of definitions for the Siemens SAB80515

// definitions for the Siemens SAB80515A
#ifdef REG_SAB80515A_H
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Siemens SAB80515A

// 8051 register set without IE, IP & PCON
#define P0
#define SP
#define DPL
#define DPH
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define P2
#define P3
#define PSW
#define ACC
#define B

// SAB80515A specific registers
#define IE_SAB80515_FAMILY
#define IP0         ; // interrupt priority register - SAB80515 specific
#define IP1         ; // interrupt priority register - SAB80515 specific
#define IEN1        ; // interrupt enable register - SAB80515 specific
#define IRCON       ; // interrupt control register - SAB80515 specific
#define CCEN        ; // compare/capture enable register
#define CCL1        ; // compare/capture register 1, low byte
#define CCH1        ; // compare/capture register 1, high byte
#define CCL2        ; // compare/capture register 2, low byte
#define CCH2        ; // compare/capture register 2, high byte
#define CCL3        ; // compare/capture register 3, low byte
#define CCH3        ; // compare/capture register 3, high byte
#define T2CON       ;
#define CRCL        ; // compare/reload/capture register, low byte
#define CRCH        ; // compare/reload/capture register, high byte
#define TL2         ;
#define TH2
#define ADCON0      ; // A/D-converter control register 0
#define ADDATH      ; // A/D data high byte
#define ADDATL      ; // A/D data low byte
#define ADCON1      ; // A/D-converter control register 1
#define SRELL       ; // Baudrate generator reload low
#define SYSCON      ; // XRAM Controller Access Control
#define SRELH       ; // Baudrate generator reload high
#define P4          ; // Port 4 - SAB80515 specific
#define P5          ; // Port 5 - SAB80515 specific
#define P6          ; // Port 6 - SAB80515 specific
#define PCON_SAB80515A ; // PCON of the SAB80515A
#define XPAGE       ; // Page Address Register for Extended On-Chip RAM

#endif
// end of definitions for the Siemens SAB80515A

/////////////////////////////////////////////////////////
///  don't specify microcontrollers below this line!  ///
/////////////////////////////////////////////////////////

// default microcontroller -> 8051
// use default if no microcontroller specified
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#warning //////////////////////////////////
#warning // No microcontroller defined!  //
#warning //////////////////////////////////
#warning Code will be generated for the
#warning 8051 (default) microcontroller.
#warning If you have another microcontroller
#warning define it in the makefile, or in the
#warning "C" source prior
#warning the #include <mcs51reg.h> statement.
#warning If you use a non supported
#warning microcontroller, mcs51reg.h can be
#warning easily extended to support your HW.

// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define P2
#define IE
#define P3
#define IP
#define PSW
#define ACC
#define B

#endif
// end of definitions for the default microcontroller


#ifdef MCS51REG_ERROR
#error Two or more microcontrollers defined!
#endif

////////////////////////////////
///  Register definitions    ///
///  (In alphabetical order) ///
////////////////////////////////

#ifdef ACC
#undef ACC
sfr at 0xE0 ACC  ;
#endif

#ifdef ADCON
#undef ADCON
sfr at 0xD8 ADCON   ; // A/D-converter control register SAB80515 specific
// Bit registers
sbit at 0xD8 MX0        ;
sbit at 0xD9 MX1        ;
sbit at 0xDA MX2        ;
sbit at 0xDB ADM        ;
sbit at 0xDC BSY        ;
sbit at 0xDE CLK        ;
sbit at 0xDF BD         ;
#endif

/* ADCON0 ... Siemens also called this register ADCON in the User Manual */
#ifdef ADCON0
#undef ADCON0
sfr at 0xD8 ADCON0      ; // A/D-converter control register 0 SAB80515A specific
// Bit registers
sbit at 0xD8 MX0        ;
sbit at 0xD9 MX1        ;
sbit at 0xDA MX2        ;
sbit at 0xDB ADM        ;
sbit at 0xDC BSY        ;
sbit at 0xDD ADEX       ;
sbit at 0xDE CLK        ;
sbit at 0xDF BD         ;
// Not directly accessible ADCON0
#define ADCON0_MX0		0x01
#define ADCON0_MX1		0x02
#define ADCON0_MX2		0x04
#define ADCON0_ADM		0x08
#define ADCON0_BSY		0x10
#define ADCON0_ADEX		0x20
#define ADCON0_CLK		0x40
#define ADCON0_BD		0x80
#endif

#ifdef ADDAT
#undef ADDAT
sfr at 0xD9 ADDAT   ; // A/D-converter data register SAB80515 specific
#endif

#ifdef ADDATH
#undef ADDATH
sfr at 0xD9 ADDATH      ; // A/D data high byte SAB80515A specific
#endif

#ifdef ADDATL
#undef ADDATL
sfr at 0xDA ADDATL      ; // A/D data low byte SAB80515A specific
#endif

#ifdef ADCON1
#undef ADCON1
sfr at 0xDC ADCON1      ; // A/D-converter control register 1 SAB80515A specific
// Not directly accessible ADCON1
#define ADCON1_MX0		0x01
#define ADCON1_MX1		0x02
#define ADCON1_MX2		0x04
#define ADCON1_ADCL		0x80
#endif

#ifdef B
#undef B
sfr at 0xF0 B    ;
// Bit registers
sbit at 0xF0 BREG_F0        ;
sbit at 0xF1 BREG_F1        ;
sbit at 0xF2 BREG_F2        ;
sbit at 0xF3 BREG_F3        ;
sbit at 0xF4 BREG_F4        ;
sbit at 0xF5 BREG_F5        ;
sbit at 0xF6 BREG_F6        ;
sbit at 0xF7 BREG_F7        ;
#endif

#ifdef CCEN
#undef CCEN
sfr at 0xC1 CCEN        ; // compare/capture enable register SAB80515 specific
#endif

#ifdef CCH1
#undef CCH1
sfr at 0xC3 CCH1        ; // compare/capture register 1, high byte SAB80515 specific
#endif

#ifdef CCH2
#undef CCH2
sfr at 0xC5 CCH2        ; // compare/capture register 2, high byte SAB80515 specific
#endif

#ifdef CCH3
#undef CCH3
sfr at 0xC7 CCH3        ; // compare/capture register 3, high byte SAB80515 specific
#endif

#ifdef CCL1
#undef CCL1
sfr at 0xC2 CCL1        ; // compare/capture register 1, low byte SAB80515 specific
#endif

#ifdef CCL2
#undef CCL2
sfr at 0xC4 CCL2        ; // compare/capture register 2, low byte SAB80515 specific
#endif

#ifdef CCL3
#undef CCL3
sfr at 0xC6 CCL3        ; // compare/capture register 3, low byte SAB80515 specific
#endif

#ifdef CRCH
#undef CRCH
sfr at 0xCB CRCH    ; // compare/reload/capture register, high byte SAB80515 specific
#endif

#ifdef CRCL
#undef CRCL
sfr at 0xCA CRCL    ; // compare/reload/capture register, low byte SAB80515 specific
#endif

#ifdef DAPR
#undef DAPR
sfr at 0xD8 DAPR    ; // D/A-converter program register SAB80515 specific
#endif

#ifdef DPH
#undef DPH
sfr at 0x83 DPH  ;
#endif

#ifdef DPL
#undef DPL
sfr at 0x82 DPL  ;
#endif

#ifdef IE
#undef IE
sfr at 0xA8 IE   ;
// Bit registers
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAF EA   ;
#ifdef REG_8052_H
sbit at 0xAD ET2  ; // Enable timer2 interrupt
#endif
#endif // IE

#ifdef IE_SAB80515_FAMILY
#undef IE_SAB80515_FAMILY
sfr at 0xA8 IE   ;
sfr at 0xA8 IEN0        ; // IE as called by Siemens
// Bit registers
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAD ET2        ; // Enable timer 2 overflow SAB80515 specific
sbit at 0xAE WDT        ; // watchdog timer reset - SAB80515 specific
sbit at 0xAF EA   ;
sbit at 0xAF EAL        ; // EA as called by Siemens
#endif // IE_SAB80515_FAMILY

#ifdef IEN1
#undef IEN1
sfr at 0xB8 IEN1        ; // interrupt enable register - SAB80515 specific
// Bit registers
sbit at 0xB8 EADC       ; // A/D converter interrupt enable
sbit at 0xB9 EX2        ;
sbit at 0xBA EX3        ;
sbit at 0xBB EX4        ;
sbit at 0xBC EX5        ;
sbit at 0xBD EX6        ;
sbit at 0xBE SWDT       ; // watchdog timer start/reset
sbit at 0xBF EXEN2      ; // timer2 external reload interrupt enable
#endif

#ifdef IP
#undef IP
sfr at 0xB8 IP   ;
// Bit registers
sbit at 0xB8 PX0  ;
sbit at 0xB9 PT0  ;
sbit at 0xBA PX1  ;
sbit at 0xBB PT1  ;
sbit at 0xBC PS   ;
#endif

#ifdef IP0
#undef IP0
sfr at 0xA9 IP0         ; // interrupt priority register SAB80515 specific
// Not directly accessible IP1 bits
#define IP0_0    0x01
#define IP0_1    0x02
#define IP0_2    0x04
#define IP0_3    0x08
#define IP0_4    0x10
#define IP0_5    0x20
#define WDTS     0x40
#endif

#ifdef IP1
#undef IP1
sfr at 0xB9 IP1         ; // interrupt priority register SAB80515 specific
// Not directly accessible IP1 bits
#define IP1_0    0x01
#define IP1_1    0x02
#define IP1_2    0x04
#define IP1_3    0x08
#define IP1_4    0x10
#define IP1_5    0x20
#endif

#ifdef IRCON
#undef IRCON
sfr at 0xC0 IRCON       ; // interrupt control register - SAB80515 specific
// Bit registers
sbit at 0xC0 IADC       ; // A/D converter irq flag
sbit at 0xC1 IEX2       ; // external interrupt edge detect flag
sbit at 0xC2 IEX3       ;
sbit at 0xC3 IEX4       ;
sbit at 0xC4 IEX5       ;
sbit at 0xC5 IEX6       ;
sbit at 0xC6 TF2        ; // timer 2 owerflow flag
sbit at 0xC7 EXF2       ; // timer2 reload flag
#endif

#ifdef P0
#undef P0
sfr at 0x80 P0   ;
//  Bit Registers
sbit at 0x80 P0_0 ;
sbit at 0x81 P0_1 ;
sbit at 0x82 P0_2 ;
sbit at 0x83 P0_3 ;
sbit at 0x84 P0_4 ;
sbit at 0x85 P0_5 ;
sbit at 0x86 P0_6 ;
sbit at 0x87 P0_7 ;
#endif

#ifdef P1
#undef P1
sfr at 0x90 P1   ;
// Bit registers
sbit at 0x90 P1_0 ;
sbit at 0x91 P1_1 ;
sbit at 0x92 P1_2 ;
sbit at 0x93 P1_3 ;
sbit at 0x94 P1_4 ;
sbit at 0x95 P1_5 ;
sbit at 0x96 P1_6 ;
sbit at 0x97 P1_7 ;
#ifdef REG_SAB80515_H
sbit at 0x90 INT3_CC0   ; // P1 alternate functions - SAB80515 specific
sbit at 0x91 INT4_CC1   ;
sbit at 0x92 INT5_CC2   ;
sbit at 0x93 INT6_CC3   ;
sbit at 0x94 INT2       ;
sbit at 0x95 T2EX       ;
sbit at 0x96 CLKOUT     ;
sbit at 0x97 T2         ;
#endif
#ifdef REG_SAB80515A_H
sbit at 0x90 INT3_CC0   ; // P1 alternate functions - SAB80515 specific
sbit at 0x91 INT4_CC1   ;
sbit at 0x92 INT5_CC2   ;
sbit at 0x93 INT6_CC3   ;
sbit at 0x94 INT2       ;
sbit at 0x95 T2EX       ;
sbit at 0x96 CLKOUT     ;
sbit at 0x97 T2         ;
#endif
#endif

#ifdef P2
#undef P2
sfr at 0xA0 P2   ;
// Bit registers
sbit at 0xA0 P2_0 ;
sbit at 0xA1 P2_1 ;
sbit at 0xA2 P2_2 ;
sbit at 0xA3 P2_3 ;
sbit at 0xA4 P2_4 ;
sbit at 0xA5 P2_5 ;
sbit at 0xA6 P2_6 ;
sbit at 0xA7 P2_7 ;
#endif

#ifdef P3
#undef P3
sfr at 0xB0 P3   ;
// Bit registers
sbit at 0xB0 P3_0 ;
sbit at 0xB1 P3_1 ;
sbit at 0xB2 P3_2 ;
sbit at 0xB3 P3_3 ;
sbit at 0xB4 P3_4 ;
sbit at 0xB5 P3_5 ;
sbit at 0xB6 P3_6 ;
sbit at 0xB7 P3_7 ;
// alternate names
sbit at 0xB0 RXD  ;
sbit at 0xB1 TXD  ;
sbit at 0xB2 INT0 ;
sbit at 0xB3 INT1 ;
sbit at 0xB4 T0   ;
sbit at 0xB5 T1   ;
sbit at 0xB6 WR   ;
sbit at 0xB7 RD   ;
#endif

#ifdef P4
#undef P4
sfr at 0xE8 P4          ; // Port 4 - SAB80515 specific
// Bit registers
sbit at 0xE8 P4_0       ;
sbit at 0xE9 P4_1       ;
sbit at 0xEA P4_2       ;
sbit at 0xEB P4_3       ;
sbit at 0xEC P4_4       ;
sbit at 0xED P4_5       ;
sbit at 0xEE P4_6       ;
sbit at 0xEF P4_7       ;
#endif

#ifdef P5
#undef P5
sfr at 0xF8 P5          ; // Port 5 - SAB80515 specific
// Bit registers
sbit at 0xF8 P5_0       ;
sbit at 0xF9 P5_1       ;
sbit at 0xFA P5_2       ;
sbit at 0xFB P5_3       ;
sbit at 0xFC P5_4       ;
sbit at 0xFD P5_5       ;
sbit at 0xFE P5_6       ;
sbit at 0xFF P5_7       ;
#endif

#ifdef P6
#undef P6
sfr at 0xDB P6          ; // Port 6 - SAB80515 specific
#endif

#ifdef PCON
#undef PCON
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define PD              0x02
#define GF0             0x04
#define GF1             0x08
#define SMOD            0x80
#endif

#ifdef PCON_SAB80515
#undef PCON_SAB80515
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define SMOD            0x80
#endif

#ifdef PCON_SAB80515A
#undef PCON_SAB80515A
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define IDLE            0x01  ; same as IDL
#define PD              0x02
#define PDE             0x02  ; same as PD
#define GF0             0x04
#define GF1             0x08
#define IDLS            0x20
#define PDS             0x40
#define SMOD            0x80
// alternate names
#define PCON_IDLE       0x01
#define PCON_PDE        0x02
#define PCON_GF0        0x04
#define PCON_GF1        0x08
#define PCON_IDLS       0x20
#define PCON_PDS        0x40
#define PCON_SMOD       0x80
#endif

#ifdef PSW
#undef PSW
sfr at 0xD0 PSW  ;
// Bit registers
sbit at 0xD0 P    ;
sbit at 0xD1 F1   ;
sbit at 0xD2 OV   ;
sbit at 0xD3 RS0  ;
sbit at 0xD4 RS1  ;
sbit at 0xD5 F0   ;
sbit at 0xD6 AC   ;
sbit at 0xD7 CY   ;
#endif

#ifdef RCAP2H
#undef RCAP2H
sfr at 0xCB RCAP2H  ;
#endif

#ifdef RCAP2L
#undef RCAP2L
sfr at 0xCA RCAP2L  ;
#endif

#ifdef SBUF
#undef SBUF
sfr at 0x99 SBUF ;
#endif

#ifdef SCON
#undef SCON
sfr at 0x98 SCON ;
// Bit registers
sbit at 0x98 RI   ;
sbit at 0x99 TI   ;
sbit at 0x9A RB8  ;
sbit at 0x9B TB8  ;
sbit at 0x9C REN  ;
sbit at 0x9D SM2  ;
sbit at 0x9E SM1  ;
sbit at 0x9F SM0  ;
#endif

#ifdef SP
#undef SP
sfr at 0x81 SP   ;
#endif

#ifdef SRELH
#undef SRELH
sfr at 0xBA SRELH       ; // Baudrate generator reload high
#endif

#ifdef SRELL
#undef SRELL
sfr at 0xAA SRELL       ; // Baudrate generator reload low
#endif

#ifdef SYSCON
#undef SYSCON
sfr at 0xB1 SYSCON      ; // XRAM Controller Access Control
// SYSCON bits
#define SYSCON_XMAP0	0x01
#define SYSCON_XMAP1	0x02
#define SYSCON_RMAP		0x10
#define SYSCON_EALE		0x20
#endif

#ifdef T2CON
#undef T2CON
sfr at 0xC8 T2CON ;
// Bit registers
sbit at 0xC8 T2CON_0 ;
sbit at 0xC9 T2CON_1 ;
sbit at 0xCA T2CON_2 ;
sbit at 0xCB T2CON_3 ;
sbit at 0xCC T2CON_4 ;
sbit at 0xCD T2CON_5 ;
sbit at 0xCE T2CON_6 ;
sbit at 0xCF T2CON_7 ;
// alternate names
sbit at 0xC8 T2I0 ;
sbit at 0xC9 T2I1 ;
sbit at 0xCA T2CM ;
sbit at 0xCB T2R0 ;
sbit at 0xCC T2R1 ;
sbit at 0xCD I2FR ;
sbit at 0xCE I3FR ;
sbit at 0xCF T2PS ;
#endif

#ifdef TCON
#undef TCON
sfr at 0x88 TCON ;
//  Bit registers
sbit at 0x88 IT0  ;
sbit at 0x89 IE0  ;
sbit at 0x8A IT1  ;
sbit at 0x8B IE1  ;
sbit at 0x8C TR0  ;
sbit at 0x8D TF0  ;
sbit at 0x8E TR1  ;
sbit at 0x8F TF1  ;
#endif

#ifdef TH0
#undef TH0
sfr at 0x8C TH0  ;
#endif

#ifdef TH1
#undef TH1
sfr at 0x8D TH1  ;
#endif

#ifdef TH2
#undef TH2
sfr at 0xCD TH2     ;
#endif

#ifdef TL0
#undef TL0
sfr at 0x8A TL0  ;
#endif

#ifdef TL1
#undef TL1
sfr at 0x8B TL1  ;
#endif

#ifdef TL2
#undef TL2
sfr at 0xCC TL2     ;
#endif

#ifdef TMOD
#undef TMOD
sfr at 0x89 TMOD ;
// Not directly accessible TMOD bits
#define T0_M0           0x01
#define T0_M1           0x02
#define T0_CT           0x04
#define T0_GATE         0x08
#define T1_M0           0x10
#define T1_M1           0x20
#define T1_CT           0x40
#define T1_GATE         0x80

#define T0_MASK         0x0F
#define T1_MASK         0xF0
#endif

#ifdef XPAGE
#undef XPAGE
sfr at 0x91 XPAGE     ; // Page Address Register for Extended On-Chip Ram - Siemens SAB80515A specific
#endif


/////////////////////////
/// Interrupt vectors ///
/////////////////////////

// Interrupt numbers: address = (number * 8) + 3
#define IE0_VECTOR      0       // 0x03 external interrupt 0
#define TF0_VECTOR      1       // 0x0b timer 0
#define IE1_VECTOR      2       // 0x13 external interrupt 1
#define TF1_VECTOR      3       // 0x1b timer 1
#define SI0_VECTOR      4       // 0x23 serial port 0


#ifdef REG_SAB80515_H
// Interrupt numbers: address = (number * 8) + 3
#define TF2_VECTOR      5       // 0x2B timer 2
#define EX2_VECTOR      5       // 0x2B external interrupt 2

#define IADC_VECTOR     8       // 0x43 A/D converter interrupt
#define IEX2_VECTOR     9       // 0x4B external interrupt 2
#define IEX3_VECTOR    10       // 0x53 external interrupt 3
#define IEX4_VECTOR    11       // 0x5B external interrupt 4
#define IEX5_VECTOR    12       // 0x63 external interrupt 5
#define IEX6_VECTOR    13       // 0x6B external interrupt 6
#endif

#ifdef REG_SAB80515A_H
// Interrupt numbers: address = (number * 8) + 3
#define TF2_VECTOR      5       // 0x2B timer 2
#define EX2_VECTOR      5       // 0x2B external interrupt 2

#define IADC_VECTOR     8       // 0x43 A/D converter interrupt
#define IEX2_VECTOR     9       // 0x4B external interrupt 2
#define IEX3_VECTOR    10       // 0x53 external interrupt 3
#define IEX4_VECTOR    11       // 0x5B external interrupt 4
#define IEX5_VECTOR    12       // 0x63 external interrupt 5
#define IEX6_VECTOR    13       // 0x6B external interrupt 6
#endif

#endif	// End of the header -> #ifndef MCS51REG_H