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
   Infineon / Siemens SAB80515, SAB80535, SAB80515A

   Version 1.0.1 (Nov 3, 2000)
   SAB80515A definitions revised by Michael Schmitt / michael.schmitt@t-online.de

   Version 1.0.2 (Nov 6, 2000)
   T2CON bug corrected 8052 and SABX microcontrollers have different T2CONs
   Support for for the Atmel AT89C52, AT80LV52, AT89C55, AT80LV55
   Support for the Dallas DS80C320 and DS80C323 microcontrollers
   microcontrollers - B. Torok / bela.torok@kssg.ch

   Version 1.0.3 (Nov 7, 2000)
   SAB80517 definitions added by Michael Schmitt / michael.schmitt@t-online.de
   Dallas AT89S53 definitions added by B. Torok / bela.torok@kssg.ch
   Dallas DS87C520 and DS83C520 definitions added by B. Torok / bela.torok@kssg.ch

   Version 1.0.4 (Nov 9, 2000)
   To simplify the identication of registers, a large number of definitios
   were renamed. Long register names now (hopefully) clearly define the
   function of the registers.
   Dallas DS89C420 definitions added by B. Torok / bela.torok@kssg.ch

   Adding support for additional microcontrollers:
   -----------------------------------------------

   1. Don't modify this file!!!

   2. Insert your code in a separate file e.g.: mcs51reg_update.h and include
      this after the #define HEADER_MCS51REG statement

   3. The mcs51reg_update.h file should contain following definitions:

          a. An entry with the inventory of the register set of the
             microcontroller in the  "Describe microcontrollers" section.

          b. If necessary add entry(s) in for registers not defined in this file

          c. Define the tnterrupt vectors

   4. Send me the file mcs51reg_update.h ( bela.torok@kssg.ch ).
      I'm going to verify/merge new definitions to this file.


   Microcontroller support:

   Use one of the following options:

   1. use #include <mcs51reg.h> in your program & define REG_XXXXXX in your makefile.

   2. use following definitions prior the
      #include <mcs51reg.h> line in your program:
      e.g.:
      #define MICROCONTROLLER_8052       -> 8052 type microcontroller
      or
      #define MICROCONTROLLER_AT89CX051  -> Atmel AT89C1051, AT89C2051 and AT89C4051 microcontrollers


   Use only one of the following definitions!!!

   Supported Microcontrollers:

   No definition                8051
   MICROCONTROLLER_8051         8051
   MICROCONTROLLER_8052         8052
   MICROCONTROLLER_AT89CX051    Atmel AT89C1051, AT89C2051 and AT89C4051
   MICROCONTROLLER_AT89S53      Atmel AT89S53 microcontroller
   MICROCONTROLLER_AT89X52      Atmel AT89C52 and AT80LV52 microcontrollers
   MICROCONTROLLER_AT89X55      Atmel AT89C55 and AT80LV55 microcontrollers
   MICROCONTROLLER_DS80C32X     Dallas DS80C320 and DS80C323 microcontrollers
   MICROCONTROLLER_DS89C420     Dallas DS89C420 microcontroller
   MICROCONTROLLER_DS8XC520     Dallas DS87C520 and DS83C520 microcontrollers
   MICROCONTROLLER_SAB80515     Infineon / Siemens SAB80515 & SAB80535
   MICROCONTROLLER_SAB80515A    Infineon / Siemens SAB80515A
   MICROCONTROLLER_SAB80517     Infineon / Siemens SAB80517

-----------------------------------------------------------------------*/


#ifndef HEADER_MCS51REG
#define HEADER_MCS51REG

///////////////////////////////////////////////////////
///  Insert header here (for developers only)       ///
///  remove "//" from the begining of the next line ///
//#include "mcs51reg_update.h"                      ///
///////////////////////////////////////////////////////

//////////////////////////////////
///  Describe microcontrollers ///
///  (inventory of registers)  ///
//////////////////////////////////

// definitions for the 8051
#ifdef MICROCONTROLLER_8051
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
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__x__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__x__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
#endif
// end of definitions for the 8051


// definitions for the 8052 microcontroller
#ifdef MICROCONTROLLER_8052
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
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
#endif
// end of definitions for the 8052 microcontroller


// definitionsons for the Atmel
// AT89C1051, AT89C2051 and AT89C4051 microcontrollers
#ifdef MICROCONTROLLER_AT89CX051
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
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
#define SCON
#define SBUF
#define IE__EA__x__x__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__x__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
#endif
// end of definitionsons for the Atmel
// AT89C1051, AT89C2051 and AT89C4051 microcontrollers


// definitions for the Atmel AT89S53
#ifdef MICROCONTROLLER_AT89S53
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: AT89S53
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// AT89S53 specific register
#define T2MOD
#define P1_EXT__x__x__x__x__x__x__T2EX__T2
#define SPCR
#define SPDR
#define SPSR
#define WCOM
#define DPL1
#define DPH1
#endif
// end of definitions for the Atmel AT89S53 microcontroller


// definitions for the Atmel AT89C52 and AT89LV52 microcontrollers
#ifdef MICROCONTROLLER_AT89X52
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: AT89C52 or AT89LV52
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// AT89X55 specific register
#define T2MOD
#define P1_EXT__x__x__x__x__x__x__T2EX__T2
#endif
// end of definitions for the Atmel AT89C52 and AT89LV52 microcontrollers


// definitions for the Atmel AT89C55 and AT89LV55 microcontrollers
#ifdef MICROCONTROLLER_AT89X55
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: AT89C55 or AT89LV55
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// AT89X55 specific register
#define T2MOD
#define P1_EXT__x__x__x__x__x__x__T2EX__T2
#endif
// end of definitions for the Atmel AT89C55 and AT89LV55 microcontrollers


// definitions for the Dallas DS80C320 and DS80C323 microcontrollers
#ifdef MICROCONTROLLER_DS80C32X
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Dallas DS80C320 or DS80C323
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__SMOD0__x__x__GF1__GF0__STOP__IDLE
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
#define IE__EA__ES1__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP__x__PS1__PT2__PS__PT1_PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// DS80C320 specific register
#define DPL1
#define DPH1
#define DPS__x__x__x__x__x__x__x__SEL
#define CKCON
#define EXIF__IE5__IE4__IE3__IE2__x__RGMD__RGSL__BGS
#define SADDR0
#define SADDR1
#define SADEN0
#define SADEN1
#define SCON1
#define SBUF1
#define STATUS__PIP__HIP__LIP__x__x__x__x__x
#define TA
#define T2MOD
#define P1_EXT__INT5__INT4__INT3__INT2__TXD1__RXD1__T2EX__T2
#define WDCON
#define EIE
#define EIP
#endif
// end of definitions for the Dallas DS80C320 and DS80C323 microcontrollers


// definitions for the Dallas DS89C420 microcontroller
#ifdef MICROCONTROLLER_DS89C420
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Dallas DS89C420
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__SMOD0__OFDF__OFDE__GF1__GF0__STOP__IDLE
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
#define IE__EA__ES1__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP__x__PS1__PT2__PS__PT1_PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// DS8XC520 specific registers
#define ACON
#define DPL1
#define DPH1
#define DPS__ID1__ID0__TSL__AID__x__x__x__SEL
#define CKCON
#define CKMOD
#define IP0__x__LPS1__LPT2__LPS0__LPT1__LPX1__LPT0__LPX0
#define IP1__x__MPS1__MPT2__MPS0__MPT1__MPX1__MPT0__MPX0
#define EXIF__IE5__IE4__IE3__IE2__CKRY__RGMD__RGSL__BGS
#define PMR
#define SADDR0
#define SADDR1
#define SADEN0
#define SADEN1
#define SCON1
#define SBUF1
#define STATUS__PIP__HIP__LIP__XTUP__SPTA2__SPTA1__SPTA0__SPRA0
#define TA
#define T2MOD
#define P1_EXT__INT5__INT4__INT3__INT2__TXD1__RXD1__T2EX__T2
#define WDCON
#define ROMSIZE__HBPF__BPF__TE__MOVCX__PRAME__RMS2__RMS1__RMS0
#define WDCON
#define EIE
#define EIP
#endif
// end of definitions for the Dallas DS89C420 microcontroller


// definitions for the Dallas DS87C520 and DS83C520 microcontrollers
#ifdef MICROCONTROLLER_DS8XC520
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Dallas DS87C520 or DS85C520
// 8051 register set
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__SMOD0__x__x__GF1__GF0__STOP__IDLE
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
#define IE__EA__ES1__ET2__ES__ET1__EX1__ET0__EX0
#define P3
#define IP__x__PS1__PT2__PS__PT1_PX1__PT0__PX0
#define PSW
#define ACC
#define B
// 8052 specific registers
#define T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#define RCAP2L
#define RCAP2H
#define TL2
#define TH2
// DS8XC520 specific registers
#define DPL1
#define DPH1
#define DPS__x__x__x__x__x__x__x__SEL
#define CKCON
#define EXIF__IE5__IE4__IE3__IE2__XT_RG__RGMD__RGSL__BGS
#define PMR
#define SADDR0
#define SADDR1
#define SADEN0
#define SADEN1
#define SCON1
#define SBUF1
#define STATUS__PIP__HIP__LIP__XTUP__SPTA2__SPTA1__SPTA0__SPRA0
#define TA
#define T2MOD
#define P1_EXT__INT5__INT4__INT3__INT2__TXD1__RXD1__T2EX__T2
#define WDCON
#define ROMSIZE__x__x__x__x__x__RMS2__RMS1__RMS0
#define BP2
#define WDCON
#define EIE
#define EIP
#endif
// end of definitions for the Dallas DS87C520 and DS83C520 microcontrollers


// definitions for the Infineon / Siemens SAB80515 & SAB80535
#ifdef MICROCONTROLLER_SAB80515
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Infineon / Siemens SAB80515 & SAB80535
// 8051 register set without IP
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__x__x__x__x__x__x__x
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
#define IE__EA_WDT_ET2_ES_ET1_EX1_ET0_EX0
#define P3
#define PSW
#define ACC
#define B
// SAB80515 specific registers
#define P1_EXT__T2__CLKOUT__T2EX__INT2__INT6_CC3__INT5_CC2__INT4_CC1__INT3_CC0
#define IP0__x__WDTS__IP0_5__IP0_4__IP0_3__IP0_2__IP0_1__IP0_0
#define IEN1
#define IRCON
#define CCEN
#define CCL1
#define CCH1
#define CCL2
#define CCH2
#define CCL3
#define CCH3
#define T2CON__T2PS__I3FR__I2FR__T2R1__T2R0__T2CM__T2I1__T2I0
#define CRCL
#define CRCH
#define TL2
#define TH2
#define ADCON
#define ADDAT
#define DAPR__SAB80515
#define P4_AT_0XE8
#define P5_AT_0XF8
#endif
// end of definitions for the Infineon / Siemens SAB80515


// definitions for the Infineon / Siemens SAB80515A
#ifdef MICROCONTROLLER_SAB80515A
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Infineon / Siemens SAB80515A
// 8051 register set without IP
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__PDS__IDLS__x__x__x__PD__IDL
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
#define IE__EA_WDT_ET2_ES_ET1_EX1_ET0_EX0
#define P3
#define PSW
#define ACC
#define B
// SAB80515A specific registers
#define P1_EXT__T2__CLKOUT__T2EX__INT2__INT6_CC3__INT5_CC2__INT4_CC1__INT3_CC0
#define IP0__x__WDTS__IP0_5__IP0_4__IP0_3__IP0_2__IP0_1__IP0_0
#define IP1__x__x__IP1_5__IP1_4__IP1_3__IP1_2__IP1_1__IP1_0
#define IEN1
#define IRCON
#define CCEN
#define CCL1
#define CCH1
#define CCL2
#define CCH2
#define CCL3
#define CCH3
#define T2CON__T2PS__I3FR__I2FR__T2R1__T2R0__T2CM__T2I1__T2I0
#define CRCL
#define CRCH
#define TL2
#define TH2
#define ADCON0
#define ADDATH
#define ADDATL
#define ADCON1
#define SRELL
#define SYSCON
#define SRELH
#define P4_AT_0XE8
#define P5_AT_0XF8
#define P6_AT_0XDB
#define XPAGE
#endif
// end of definitions for the Infineon / Siemens SAB80515A


// definitions for the Infineon / Siemens SAB80517
#ifdef MICROCONTROLLER_SAB80517
#ifdef MICROCONTROLLER_DEFINED
#define MCS51REG_ERROR
#endif
#ifndef MICROCONTROLLER_DEFINED
#define MICROCONTROLLER_DEFINED
#endif
#warning Selected HW: Infineon / Siemens SAB80517
// 8051 register set without IP, SCON & SBUF
#define P0
#define SP
#define DPL
#define DPH
#define PCON__SMOD__PDS__IDLS__x__x__x__PD__IDL
#define TCON
#define TMOD
#define TL0
#define TL1
#define TH0
#define TH1
#define P1
// #define SCON
// #define SBUF
#define P2
#define IE__EA_WDT_ET2_ES_ET1_EX1_ET0_EX0
#define P3
#define PSW
#define ACC
#define B
// SAB80517 specific registers
#define P1_EXT__T2__CLKOUT__T2EX__INT2__INT6_CC3__INT5_CC2__INT4_CC1__INT3_CC0
#define IP0__x__WDTS__IP0_5__IP0_4__IP0_3__IP0_2__IP0_1__IP0_0
#define IP1__x__x__IP1_5__IP1_4__IP1_3__IP1_2__IP1_1__IP1_0
#define IEN1
#define IEN2__SAB80517
#define IRCON
#define CCEN
#define CCL1
#define CCH1
#define CCL2
#define CCH2
#define CCL3
#define CCH3
#define CCL4
#define CCH4
#define CC4EN
#define CMEN
#define CMH0
#define CML0
#define CMH1
#define CML1
#define CMH2
#define CML2
#define CMH3
#define CML3
#define CMH4
#define CML4
#define CMH5
#define CML5
#define CMH6
#define CML6
#define CMH7
#define CML7
#define CMSEL
#define T2CON__T2PS__I3FR__I2FR__T2R1__T2R0__T2CM__T2I1__T2I0
#define CRCL
#define CRCH
#define CTCON
#define CTRELH
#define CTRELL
#define TL2
#define TH2
#define ADCON0
#define ADCON1
#define ADDAT
#define DAPR__SAB80517
#define P4_AT_0XE8
#define P5_AT_0XF8
#define P6_AT_0XFA
#define P7_AT_0XDB
#define P8_AT_0XDD
#define DPSEL
#define ARCON
#define MD0
#define MD1
#define MD2
#define MD3
#define MD4
#define MD5
#define S0BUF
#define S0CON
#define S0RELH
#define S0RELL
#define S1BUF
#define S1CON
#define S1RELH
#define S1RELL
#define WDTH
#define WDTL
#define WDTREL
#endif
// end of definitions for the Infineon / Siemens SAB80517


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
#define PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
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
#define IE__EA__x__x__ES__ET1__EX1__ET0__EX0
#define P3
#define IP___x__x__x__PS__PT1__PX1__PT0__PX0
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

#ifdef ACON
#undef ACON
sfr at 0x9D ACON   ; // DS89C420 specific
// Bit registers
#define PAGE0   0x20
#define PAGES   0x40
#define PAGEE   0x80
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

// ADCON0 ... Infineon / Siemens also called this register ADCON in the User Manual
#ifdef ADCON0
#undef ADCON0
sfr at 0xD8 ADCON0      ; // A/D-converter control register 0 SAB80515A &
// Bit registers          // SAB80517 specific
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

#ifdef ADCON1
#undef ADCON1
sfr at 0xDC ADCON1      ; // A/D-converter control register 1 SAB80515A & SAB80517 specific
// Not directly accessible ADCON1
#define ADCON1_MX0		0x01
#define ADCON1_MX1		0x02
#define ADCON1_MX2		0x04
#define ADCON1_ADCL		0x80
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

#ifdef ARCON
#undef ARCON
sfr at 0xEF ARCON       ; // arithmetic control register SAB80517
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

#ifdef BP2
#undef BP2
sfr at 0xC3 BP2    ;
// Not directly accessible bits
#define MS0   0x01
#define MS1   0x02
#define MS2   0x04
#define LB1   0x08
#define LB2   0x10
#define LB3   0x20
#endif

#ifdef CC4EN
#undef CC4EN
sfr at 0xC9 CC4EN       ; // compare/capture 4 enable register SAB80517 specific
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

#ifdef CCH4
#undef CCH4
sfr at 0xCF CCH4        ; // compare/capture register 4, high byte SAB80515 specific
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

#ifdef CCL4
#undef CCL4
sfr at 0xCE CCL4        ; // compare/capture register 4, low byte SAB80515 specific
#endif

#ifdef CKCON
#undef CKCON
sfr at 0x8E CKCON       ; // DS80C320 specific
// Not directly accessible Bits. DS80C320 specific
#define MD0    0x01
#define MD1    0x02
#define MD2    0x04
#define T0M    0x08
#define T1M    0x10
#define T2M    0x20
#define WD0    0x40
#define WD1    0x80
#endif

#ifdef CKMOD
#undef CKMOD
sfr at 0x96 CKMOD       ; // DS89C420 specific
// Not directly accessible Bits.
#define T0MH   0x08
#define T1MH   0x10
#define T2MH   0x20
#endif

#ifdef CMEN
#undef CMEN
sfr at 0xF6 CMEN    ; // compare enable register SAB80517 specific
#endif

#ifdef CMH0
#undef CMH0
sfr at 0xD3 CMH0    ; // compare register 0 high byte SAB80517 specific
#endif

#ifdef CMH1
#undef CMH1
sfr at 0xD5 CMH1    ; // compare register 1 high byte SAB80517 specific
#endif

#ifdef CMH2
#undef CMH2
sfr at 0xD7 CMH2    ; // compare register 2 high byte SAB80517 specific
#endif

#ifdef CMH3
#undef CMH3
sfr at 0xE3 CMH3    ; // compare register 3 high byte SAB80517 specific
#endif

#ifdef CMH4
#undef CMH4
sfr at 0xE5 CMH4    ; // compare register 4 high byte SAB80517 specific
#endif

#ifdef CMH5
#undef CMH5
sfr at 0xE7 CMH5    ; // compare register 5 high byte SAB80517 specific
#endif

#ifdef CMH6
#undef CMH6
sfr at 0xF3 CMH6    ; // compare register 6 high byte SAB80517 specific
#endif

#ifdef CMH7
#undef CMH7
sfr at 0xF5 CMH7    ; // compare register 7 high byte SAB80517 specific
#endif

#ifdef CML0
#undef CML0
sfr at 0xD2 CML0    ; // compare register 0 low byte SAB80517 specific
#endif

#ifdef CML1
#undef CML1
sfr at 0xD4 CML1    ; // compare register 1 low byte SAB80517 specific
#endif

#ifdef CML2
#undef CML2
sfr at 0xD6 CML2    ; // compare register 2 low byte SAB80517 specific
#endif

#ifdef CML3
#undef CML3
sfr at 0xE2 CML3    ; // compare register 3 low byte SAB80517 specific
#endif

#ifdef CML4
#undef CML4
sfr at 0xE4 CML4    ; // compare register 4 low byte SAB80517 specific
#endif

#ifdef CML5
#undef CML5
sfr at 0xE6 CML5    ; // compare register 5 low byte SAB80517 specific
#endif

#ifdef CML6
#undef CML6
sfr at 0xF2 CML6    ; // compare register 6 low byte SAB80517 specific
#endif

#ifdef CML7
#undef CML7
sfr at 0xF4 CML7    ; // compare register 7 low byte SAB80517 specific
#endif

#ifdef CMSEL
#undef CMSEL
sfr at 0xF7 CMSEL   ; // compare input select SAB80517
#endif

#ifdef CRCH
#undef CRCH
sfr at 0xCB CRCH    ; // compare/reload/capture register, high byte SAB80515 specific
#endif

#ifdef CRCL
#undef CRCL
sfr at 0xCA CRCL    ; // compare/reload/capture register, low byte SAB80515 specific
#endif

#ifdef CTCON
#undef CTCON
sfr at 0xE1 CTCON    ; // com.timer control register SAB80517
#endif

#ifdef CTRELH
#undef CTRELH
sfr at 0xDF CTRELH  ; // com.timer rel register high byte SAB80517
#endif

#ifdef CTRELL
#undef CTRELL
sfr at 0xDE CTRELL  ; // com.timer rel register low byte SAB80517
#endif

#ifdef DAPR__SAB80515
#undef DAPR__SAB80515
sfr at 0xD8 DAPR    ; // D/A-converter program register SAB80515 specific
#endif

#ifdef DAPR__SAB80517
#undef DAPR__SAB80517
sfr at 0xDA DAPR    ; // D/A-converter program register SAB80517 specific
#endif

#ifdef DPH
#undef DPH
sfr at 0x83 DPH  ;
sfr at 0x83 DP0H ;  // Alternate name for AT89S53
#endif

#ifdef DPH1
#undef DPH1
sfr at 0x85 DPH1  ; // DS80C320 specific
sfr at 0x85 DP1H  ; // Alternate name for AT89S53
#endif

#ifdef DPL
#undef DPL
sfr at 0x82 DPL  ;  // Alternate name for AT89S53
sfr at 0x82 DP0L ;
#endif

#ifdef DPL1
#undef DPL1
sfr at 0x84 DPL1  ; // DS80C320 specific
sfr at 0x84 DP1L  ; // Alternate name for AT89S53
#endif

#ifdef DPS__x__x__x__x__x__x__x__SEL
#undef DPS__x__x__x__x__x__x__x__SEL
sfr at 0x86 DPS  ;
// Not directly accessible DPS Bit. DS80C320 & DPS8XC520 specific
#define SEL    0x01
#endif

#ifdef DPS__ID1__ID0__TSL__AID__x__x__x__SEL
#undef DPS__ID1__ID0__TSL__AID__x__x__x__SEL
sfr at 0x86 DPS  ;
// Not directly accessible DPS Bit. DS89C420 specific
#define SEL    0x01
#define AID    0x10
#define TSL    0x20
#define ID0    0x40
#define ID1    0x80
#endif

#ifdef DPSEL
#undef DPSEL
sfr at 0x92 DPSEL   ; // data pointer select register SAB80517
#endif

#ifdef EIE
#undef EIE
sfr at 0xE8 EIE  ;
// Bit registers DS80C320 specific
sbit at 0xE8 EX2    ;
sbit at 0xE9 EX3    ;
sbit at 0xEA EX4    ;
sbit at 0xEB EX5    ;
sbit at 0xEC EWDI   ;
#endif

#ifdef EIP
#undef EIP
sfr at 0xE8 EIP  ;
// Bit registers DS80C320 specific
sbit at 0xF8 PX2    ;
sbit at 0xF9 PX3    ;
sbit at 0xFA PX4    ;
sbit at 0xFB PX5    ;
sbit at 0xFC PWDI   ;
#endif

#ifdef EXIF__IE5__IE4__IE3__IE2__x__RGMD__RGSL__BGS
#undef EXIF__IE5__IE4__IE3__IE2__x__RGMD__RGSL__BGS
sfr at 0x91 EXIF  ;
// Not directly accessible EXIF Bits DS80C320 specific
#define BGS    0x01
#define RGSL   0x02
#define RGMD   0x04
#define IE2    0x10
#define IE3    0x20
#define IE4    0x40
#define IE5    0x80
#endif

#ifdef EXIF__IE5__IE4__IE3__IE2__XT_RG__RGMD__RGSL__BGS
#undef EXIF__IE5__IE4__IE3__IE2__XT_RG__RGMD__RGSL__BGS
sfr at 0x91 EXIF  ;
// Not directly accessible EXIF Bits DS87C520 specific
#define BGS    0x01
#define RGSL   0x02
#define RGMD   0x04
#define XT_RG  0x08
#define IE2    0x10
#define IE3    0x20
#define IE4    0x40
#define IE5    0x80
#endif

#ifdef EXIF__IE5__IE4__IE3__IE2__CKRY__RGMD__RGSL__BGS
#undef EXIF__IE5__IE4__IE3__IE2__CKRY__RGMD__RGSL__BGS
sfr at 0x91 EXIF  ;
// Not directly accessible EXIF Bits DS89C420 specific
#define BGS    0x01
#define RGSL   0x02
#define RGMD   0x04
#define CKRY   0x08
#define IE2    0x10
#define IE3    0x20
#define IE4    0x40
#define IE5    0x80
#endif

#ifdef IE__EA__x__x__ES__ET1__EX1__ET0__EX0
#undef IE__EA__x__x__ES__ET1__EX1__ET0__EX0
sfr at 0xA8 IE   ;
// Bit registers
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAF EA   ;
#endif

#ifdef IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
#undef IE__EA__x__ET2__ES__ET1__EX1__ET0__EX0
sfr at 0xA8 IE   ;
// Bit registers
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAD ET2  ; // Enable timer2 interrupt
sbit at 0xAF EA   ;
#endif // IE

#ifdef IE__EA__ES1__ET2__ES__ET1__EX1__ET0__EX0
#undef IE__EA__ES1__ET2__ES__ET1__EX1__ET0__EX0
sfr at 0xA8 IE   ;
// Bit registers
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAC ES0  ; // Alternate name
sbit at 0xAD ET2  ; // Enable timer2 interrupt
sbit at 0xAE ES1  ;
sbit at 0xAF EA   ;
#endif // IE

#ifdef IE__EA_WDT_ET2_ES_ET1_EX1_ET0_EX0
#undef IE__EA_WDT_ET2_ES_ET1_EX1_ET0_EX0
sfr at 0xA8 IE    ;
sfr at 0xA8 IEN0  ; // Alternate name
// Bit registers for the SAB80515 and compatible IE
sbit at 0xA8 EX0  ;
sbit at 0xA9 ET0  ;
sbit at 0xAA EX1  ;
sbit at 0xAB ET1  ;
sbit at 0xAC ES   ;
sbit at 0xAC ES0  ;
sbit at 0xAD ET2  ; // Enable timer 2 overflow SAB80515 specific
sbit at 0xAE WDT  ; // watchdog timer reset - SAB80515 specific
sbit at 0xAF EA   ;
sbit at 0xAF EAL  ; // EA as called by Infineon / Siemens
#endif

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

#ifdef IEN2__SAB80517
#undef IEN2__SAB80517
sfr at 0x9A IEN2        ; // interrupt enable register 2 SAB80517
#endif

#ifdef IP___x__x__x__PS__PT1__PX1__PT0__PX0
#undef IP___x__x__x__PS__PT1__PX1__PT0__PX0
sfr at 0xB8 IP   ;
// Bit registers
sbit at 0xB8 PX0  ;
sbit at 0xB9 PT0  ;
sbit at 0xBA PX1  ;
sbit at 0xBB PT1  ;
sbit at 0xBC PS   ;
#endif

#ifdef IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
#undef IP___x__x__PT2__PS__PT1__PX1__PT0__PX0
sfr at 0xB8 IP    ;
// Bit registers
sbit at 0xB8 PX0  ;
sbit at 0xB9 PT0  ;
sbit at 0xBA PX1  ;
sbit at 0xBB PT1  ;
sbit at 0xBC PS   ;
sbit at 0xBD PT2  ;
#endif

#ifdef IP__x__PS1__PT2__PS__PT1_PX1__PT0__PX0
#undef IP__x__PS1__PT2__PS__PT1_PX1__PT0__PX0
sfr at 0xB8 IP   ;
// Bit registers
sbit at 0xB8 PX0  ;
sbit at 0xB9 PT0  ;
sbit at 0xBA PX1  ;
sbit at 0xBB PT1  ;
sbit at 0xBC PS   ;
sbit at 0xBD PT2  ;
sbit at 0xBE PS1  ;
#endif

#ifdef IP0__x__WDTS__IP0_5__IP0_4__IP0_3__IP0_2__IP0_1__IP0_0
#undef IP0__x__WDTS__IP0_5__IP0_4__IP0_3__IP0_2__IP0_1__IP0_0
sfr at 0xA9 IP0   ; // interrupt priority register SAB80515 specific
// Not directly accessible IP0 bits
#define IP0_0    0x01
#define IP0_1    0x02
#define IP0_2    0x04
#define IP0_3    0x08
#define IP0_4    0x10
#define IP0_5    0x20
#define WDTS     0x40
#endif

#ifdef IP0__x__LPS1__LPT2__LPS0__LPT1__LPX1__LPT0__LPX0
#undef IP0__x__LPS1__LPT2__LPS0__LPT1__LPX1__LPT0__LPX0
sfr at 0xB8 IP0   ; // interrupt priority register DS89C420 specific
// Not directly accessible IP0 bits
#define LPX0    0x01
#define LPT0    0x02
#define LPX1    0x04
#define LPT1    0x08
#define LPS0    0x10
#define LPT2    0x20
#define LPS1    0x40
#endif

#ifdef IP1__x__x__IP1_5__IP1_4__IP1_3__IP1_2__IP1_1__IP1_0
#undef IP1__x__x__IP1_5__IP1_4__IP1_3__IP1_2__IP1_1__IP1_0
sfr at 0xB9 IP1   ; // interrupt priority register SAB80515 specific
// Not directly accessible IP1 bits
#define IP1_0    0x01
#define IP1_1    0x02
#define IP1_2    0x04
#define IP1_3    0x08
#define IP1_4    0x10
#define IP1_5    0x20
#endif

#ifdef IP1__x__MPS1__MPT2__MPS0__MPT1__MPX1__MPT0__MPX0
#undef IP1__x__MPS1__MPT2__MPS0__MPT1__MPX1__MPT0__MPX0
sfr at 0xB1 IP1   ; // interrupt priority register DS89C420 specific
// Not directly accessible IP0 bits
#define LPX0    0x01
#define LPT0    0x02
#define LPX1    0x04
#define LPT1    0x08
#define LPS0    0x10
#define LPT2    0x20
#define LPS1    0x40
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

#ifdef IRCON0
#undef IRCON0
sfr at 0xC0 IRCON0       ; // interrupt control register - SAB80515 specific
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

#ifdef IRCON1
#undef IRCON1
sfr at 0xD1 IRCON1      ; // interrupt control register - SAB80515 specific
#endif

#ifdef MD0
#undef MD0
sfr at 0xE9 MD0         ; // MUL / DIV register 0 SAB80517
#endif

#ifdef MD1
#undef MD1
sfr at 0xEA MD1         ; // MUL / DIV register 1 SAB80517
#endif

#ifdef MD2
#undef MD2
sfr at 0xEB MD2         ; // MUL / DIV register 2 SAB80517
#endif

#ifdef MD3
#undef MD3
sfr at 0xEC MD3         ; // MUL / DIV register 3 SAB80517
#endif

#ifdef MD4
#undef MD4
sfr at 0xED MD4         ; // MUL / DIV register 4 SAB80517
#endif

#ifdef MD5
#undef MD5
sfr at 0xEE MD5         ; // MUL / DIV register 5 SAB80517
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
#endif

#ifdef P1_EXT__INT5__INT4__INT3__INT2__TXD1__RXD1__T2EX__T2
#undef P1_EXT__INT5__INT4__INT3__INT2__TXD1__RXD1__T2EX__T2
// P1 alternate functions
sbit at 0x90 T2   ;
sbit at 0x91 T2EX ;
sbit at 0x92 RXD1 ;
sbit at 0x93 TXD1 ;
sbit at 0x94 INT2 ;
sbit at 0x95 INT3 ;
sbit at 0x96 INT4 ;
sbit at 0x97 INT5 ;
#endif

#ifdef P1_EXT__T2__CLKOUT__T2EX__INT2__INT6_CC3__INT5_CC2__INT4_CC1__INT3_CC0
sbit at 0x90 INT3_CC0   ; // P1 alternate functions - SAB80515 specific
sbit at 0x91 INT4_CC1   ;
sbit at 0x92 INT5_CC2   ;
sbit at 0x93 INT6_CC3   ;
sbit at 0x94 INT2       ;
sbit at 0x95 T2EX       ;
sbit at 0x96 CLKOUT     ;
sbit at 0x97 T2         ;
#endif

#ifdef P1_EXT__x__x__x__x__x__x__T2EX__T2
#undef P1_EXT__x__x__x__x__x__x__T2EX__T2
// P1 alternate functions
sbit at 0x90 T2         ;
sbit at 0x91 T2EX       ;
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

#ifdef P4_AT_0XE8
#undef P4_AT_0XE8
sfr at 0xE8 P4          ; // Port 4 - SAB80515 & compatible microcontrollers
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

#ifdef P5_AT_0XF8
#undef P5_AT_0XF8
sfr at 0xF8 P5          ; // Port 5 - SAB80515 & compatible microcontrollers
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

#ifdef P6_AT_0XDB
#undef P6_AT_0XDB
sfr at 0xDB P6          ; // Port 6 - SAB80515 & compatible microcontrollers
#endif

#ifdef P6_AT_0XFA
#undef P6_AT_0XFA
sfr at 0xFA P6          ; // Port 6 - SAB80517 specific
#endif

#ifdef P7_AT_0XDB
#undef P7_AT_0XDB
sfr at 0xDB P7          ; // Port 7 - SAB80517 specific
#endif

#ifdef P8_AT_0XDD
#undef P8_AT_0XDD
sfr at 0xDD P8          ; // Port 6 - SAB80517 specific
#endif

#ifdef PCON__SMOD__x__x__x__x__x__x__x
#undef PCON__SMOD__x__x__x__x__x__x__x
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define SMOD    0x80
#endif

#ifdef PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
#undef PCON__SMOD__x__x__x__GF1__GF0__PD__IDL
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define PD              0x02 
#define GF0             0x04
#define GF1             0x08
#define SMOD            0x80
#endif

#ifdef PCON__SMOD__PDS__IDLS__x__x__x__PD__IDL
#undef PCON__SMOD__PDS__IDLS__x__x__x__PD__IDL
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define IDLE            0x01  ; same as IDL
#define PD              0x02  ;
#define PDE             0x02  ; same as PD
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

#ifdef PCON__SMOD__SMOD0__x__x__GF1__GF0__STOP__IDLE
#undef PCON__SMOD__SMOD0__x__x__GF1__GF0__STOP__IDLE
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define IDLE            0x01  ; same as IDL
#define STOP            0x02  ;
#define GF0             0x04
#define GF1             0x08
#define SMOD0           0x40
#define SMOD            0x80
#endif

#ifdef PCON__SMOD__SMOD0__OFDF__OFDE__GF1__GF0__STOP__IDLE
#undef PCON__SMOD__SMOD0__OFDF__OFDE__GF1__GF0__STOP__IDLE
sfr at 0x87 PCON ;
// Not directly accessible PCON bits
#define IDL             0x01
#define IDLE            0x01  ; same as IDL
#define STOP            0x02  ;
#define GF0             0x04
#define GF1             0x08
#define OFDE            0x10
#define OFDF            0x20
#define SMOD0           0x40
#define SMOD            0x80
#endif

#ifdef PMR
#undef PMR
sfr at 0xC4 PMR  ;   // DS87C520, DS83C520
// Not directly accessible bits
#define DME0    0x01
#define DME1    0x02
#define ALEOFF  0x04
#define XTOFF   0x08
#define SWB     0x20
#define CD0     0x40
#define CD1     0x80
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

#ifdef ROMSIZE__x__x__x__x__x__RMS2__RMS1__RMS0
#undef ROMSIZE__x__x__x__x__x__RMS2__RMS1__RMS0
sfr at 0xC2 ROMSIZE  ;   // DS87C520, DS83C520
// Not directly accessible bits
#define RSM0    0x01
#define RSM1    0x02
#define RSM2    0x04
#endif

#ifdef ROMSIZE__HBPF__BPF__TE__MOVCX__PRAME__RMS2__RMS1__RMS0
#undef ROMSIZE__HBPF__BPF__TE__MOVCX__PRAME__RMS2__RMS1__RMS0
sfr at 0xC2 ROMSIZE  ;   // DS87C520, DS83C520
// Not directly accessible bits
#define RSM0    0x01
#define RSM1    0x02
#define RSM2    0x04
#define PRAME   0x08
#define MOVCX   0x10
#define TE      0x20
#define BPF     0x40
#define HBPF    0x80
#endif

#ifdef S0BUF
#undef S0BUF
sfr at 0x99 S0BUF ; // serial channel 0 buffer register SAB80517 specific
#endif

#ifdef S0CON
#undef S0CON
sfr at 0x98 S0CON ; // serial channel 0 control register SAB80517 specific
// Bit registers
sbit at 0x98 RI0  ; // S0CON.0: receiver0 interrupt flag
sbit at 0x99 TI0  ; // S0CON.1: transmitter0 interrupt flag
sbit at 0x9A RB80 ; // S0CON.2: receiver0 bit8
sbit at 0x9B TB80 ; // S0CON.3: transmitter0 bit 8
sbit at 0x9C REN0 ; // S0CON.4: receiver0 enable
sbit at 0x9D SM20 ; // S0CON.5: multiprocessor feature
sbit at 0x9E SM1  ; // S0CON.6: | select serial mode
sbit at 0x9F SM0  ; // S0CON.7: |
#endif

#ifdef S0RELL
#undef S0RELL
sfr at 0xAA S0RELL ; // serial channel 0 reload register low byte SAB80517 specific
#endif

#ifdef S0RELH
#undef S0RELH
sfr at 0xBA S0RELH ; // serial channel 0 reload register high byte SAB80517 specific
#endif

#ifdef S1BUF
#undef S1BUF
sfr at 0x9C S1BUF ; // serial channel 1 buffer register SAB80517 specific
#endif

#ifdef S1CON
#undef S1CON
sfr at 0x9B S1CON ; // serial channel 1 control register SAB80517 specific
#endif

#ifdef S1RELL
#undef S1RELL
sfr at 0x9D S1RELL ; // serial channel 1 reload register low byte SAB80517 specific
#endif

#ifdef S1RELH
#undef S1RELH
sfr at 0xBB S1RELH ; // serial channel 1 reload register high byte SAB80517 specific
#endif

#ifdef SADDR0
#undef SADDR0
// DS80C320 specific
sfr at 0xA9 SADDR0  ;
#endif

#ifdef SADDR1
#undef SADDR1
// DS80C320 specific
sfr at 0xAA SADDR1  ;
#endif

#ifdef SADEN0
#undef SADEN0
// DS80C320 specific
sfr at 0xB9 SADEN0  ;
#endif

#ifdef SADEN1
#undef SADEN1
// DS80C320 specific
sfr at 0xBA SADEN1  ;
#endif

#ifdef SBUF
#undef SBUF
sfr at 0x99 SBUF ;
#endif

#ifdef SBUF1
#undef SBUF1
// DS80C320 specific
sfr at 0xC1 SBUF1 ;
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

#ifdef SCON1
#undef SCON1
// DS80C320 specific
sfr at 0xC0 SCON1  ;
// Bit registers
sbit at 0xC8 RI_1         ;
sbit at 0xC9 TI_1         ;
sbit at 0xCA RB8_1        ;
sbit at 0xCB TB8_1        ;
sbit at 0xCC REN_1        ;
sbit at 0xCD SM2_1        ;
sbit at 0xCE SM1_1        ;
sbit at 0xCF SM0_1        ;
sbit at 0xCF FE_1         ;
sbit at 0xCF SM0_FE_1     ;
#endif

#ifdef SP
#undef SP
sfr at 0x81 SP   ;
#endif

#ifdef SPCR
#undef SPCR
sfr at 0xD5 SPCR   ;   // AT89S53 specific
// Not directly accesible bits
#define SPR0 0x01
#define SPR1 0x02
#define CPHA 0x04
#define CPOL 0x08
#define MSTR 0x10
#define DORD 0x20
#define SPE  0x40
#define SPIE 0x80
#endif

#ifdef SPDR
#undef SPDR
sfr at 0x86 SPDR   ;   // AT89S53 specific
// Not directly accesible bits
#define SPD_0 0x01
#define SPD_1 0x02
#define SPD_2 0x04
#define SPD_3 0x08
#define SPD_4 0x10
#define SPD_5 0x20
#define SPD_6 0x40
#define SPD_7 0x80
#endif

#ifdef SPSR
#undef SPSR
sfr at 0xAA SPSR   ;   // AT89S53 specific
// Not directly accesible bits
#define SPIF 0x40
#define WCOL 0x80
#endif

#ifdef SRELH
#undef SRELH
sfr at 0xBA SRELH       ; // Baudrate generator reload high
#endif

#ifdef SRELL
#undef SRELL
sfr at 0xAA SRELL       ; // Baudrate generator reload low
#endif

#ifdef STATUS__PIP__HIP__LIP__x__x__x__x__x
#undef STATUS__PIP__HIP__LIP__x__x__x__x__x
// DS80C320 specific
sfr at 0xC5 STATUS ;
// Not directly accessible Bits. DS80C320 specific
#define LIP  0x20
#define HIP  0x40
#define PIP  0x80
#endif

#ifdef STATUS__PIP__HIP__LIP__XTUP__SPTA2__SPTA1__SPTA0__SPRA0
#undef STATUS__PIP__HIP__LIP__XTUP__SPTA2__SPTA1__SPTA0__SPRA0
sfr at 0xC5 STATUS ; // DS87C520 & DS83520specific
// Not directly accessible Bits.
#define SPRA0  0x01
#define SPTA0  0x02
#define SPTA1  0x04
#define SPTA2  0x08
#define XTUP   0x10
#define LIP    0x20
#define HIP    0x40
#define PIP    0x80
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

#ifdef T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
#undef T2CON__TF2__EXF2__RCLK__TCLK__EXEN2__TR2__C_T2__CP_RL2
sfr at 0xC8 T2CON ;
// Definitions for the 8052 compatible microcontrollers.
// Bit registers
sbit at 0xC8 CP_RL2  ;
sbit at 0xC9 C_T2    ;
sbit at 0xCA TR2     ;
sbit at 0xCB EXEN2   ;
sbit at 0xCC TCLK    ;
sbit at 0xCD RCLK    ;
sbit at 0xCE EXF2    ;
sbit at 0xCF TF2     ;
// alternate names
sbit at 0xC8 T2CON_0 ;
sbit at 0xC9 T2CON_1 ;
sbit at 0xCA T2CON_2 ;
sbit at 0xCB T2CON_3 ;
sbit at 0xCC T2CON_4 ;
sbit at 0xCD T2CON_5 ;
sbit at 0xCE T2CON_6 ;
sbit at 0xCF T2CON_7 ;
#endif

#ifdef T2CON__T2PS__I3FR__I2FR__T2R1__T2R0__T2CM__T2I1__T2I0
#undef T2CON__T2PS__I3FR__I2FR__T2R1__T2R0__T2CM__T2I1__T2I0
sfr at 0xC8 T2CON ;
// Definitions for the Infineon / Siemens SAB80515, SAB80515A, SAB80517
// Bit registers
sbit at 0xC8 T2I0 ;
sbit at 0xC9 T2I1 ;
sbit at 0xCA T2CM ;
sbit at 0xCB T2R0 ;
sbit at 0xCC T2R1 ;
sbit at 0xCD I2FR ;
sbit at 0xCE I3FR ;
sbit at 0xCF T2PS ;
// alternate names
sbit at 0xC8 T2CON_0 ;
sbit at 0xC9 T2CON_1 ;
sbit at 0xCA T2CON_2 ;
sbit at 0xCB T2CON_3 ;
sbit at 0xCC T2CON_4 ;
sbit at 0xCD T2CON_5 ;
sbit at 0xCE T2CON_6 ;
sbit at 0xCF T2CON_7 ;
#endif

#ifdef T2MOD
#undef T2MOD
// Definitions for the timer/counter 2 of the Atmel 89x52 microcontroller
sfr at 0xC9 T2MOD  ;
// Not not directly accessible T2MOD bits
#define DCEN            0x01
#define T2OE            0x02
// Alternate names
#define DCEN_           0x01
#define T2OE_           0x02
#endif

#ifdef TA
#undef TA
// DS80C320 specific
sfr at 0xC7 TA ;
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

#ifdef WCON
#undef WCON
sfr at 0x96 WCON   ;   // AT89S53 specific
// Not directly accesible bits
#define WDTEN  0x01
#define WDTRST 0x02
#define DPS    0x04
#define PS0    0x20
#define PS1    0x40
#define PS2    0x80
#endif

#ifdef WDCON
#undef WDCON
// DS80C320 specific
sfr at 0xD8 WDCON ;
// Not directly accessible bits
#define RWT    0x01    /* Timed-Access protected */
#define EWT    0x02    /* Timed-Access protected */
#define WTRF   0x04
#define WDIF   0x08    /* Timed-Access protected */
#define PFI    0x10
#define EPFI   0x20
#define POR    0x40    /* Timed-Access protected */
#define SMOD_1 0x80
#endif

#ifdef WDTREL
#undef WDTREL
sfr at 0x86 WDTREL    ; // Watchdof Timer reload register
#endif

#ifdef XPAGE
#undef XPAGE
sfr at 0x91 XPAGE     ; // Page Address Register for Extended On-Chip Ram - Infineon / Siemens SAB80515A specific
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

#ifdef MICROCONTROLLER_AT89S53
#define TF2_VECTOR      5       /* 0x2B timer 2 */
#define EX2_VECTOR      5       /* 0x2B external interrupt 2 */
#endif

#ifdef MICROCONTROLLER_AT89X52
#define TF2_VECTOR      5       /* 0x2B timer 2 */
#define EX2_VECTOR      5       /* 0x2B external interrupt 2 */
#endif

#ifdef MICROCONTROLLER_AT89X55
#define TF2_VECTOR      5       /* 0x2B timer 2 */
#define EX2_VECTOR      5       /* 0x2B external interrupt 2 */
#endif

#ifdef MICROCONTROLLER_DS80C32X
#define TF2_VECTOR      5  /* 0x2B */
#define PFI_VECTOR      6  /* 0x33 */
#define SIO1_VECTOR     7  /* 0x3B */
#define IE2_VECTOR      8  /* 0x43 */
#define IE3_VECTOR      9  /* 0x4B */
#define IE4_VECTOR      10 /* 0x53 */
#define IE5_VECTOR      11 /* 0x5B */
#define WDI_VECTOR      12 /* 0x63 */
#endif

#ifdef MICROCONTROLLER_DS8XC520
#define TF2_VECTOR      5  /* 0x2B */
#define PFI_VECTOR      6  /* 0x33 */
#define SIO1_VECTOR     7  /* 0x3B */
#define IE2_VECTOR      8  /* 0x43 */
#define IE3_VECTOR      9  /* 0x4B */
#define IE4_VECTOR      10 /* 0x53 */
#define IE5_VECTOR      11 /* 0x5B */
#define WDI_VECTOR      12 /* 0x63 */
#endif

#ifdef MICROCONTROLLER_SAB80515
#define TF2_VECTOR      5       // 0x2B timer 2
#define EX2_VECTOR      5       // 0x2B external interrupt 2
#define IADC_VECTOR     8       // 0x43 A/D converter interrupt
#define IEX2_VECTOR     9       // 0x4B external interrupt 2
#define IEX3_VECTOR    10       // 0x53 external interrupt 3
#define IEX4_VECTOR    11       // 0x5B external interrupt 4
#define IEX5_VECTOR    12       // 0x63 external interrupt 5
#define IEX6_VECTOR    13       // 0x6B external interrupt 6
#endif

#ifdef MICROCONTROLLER_SAB80515A
#define TF2_VECTOR      5       // 0x2B timer 2
#define EX2_VECTOR      5       // 0x2B external interrupt 2
#define IADC_VECTOR     8       // 0x43 A/D converter interrupt
#define IEX2_VECTOR     9       // 0x4B external interrupt 2
#define IEX3_VECTOR    10       // 0x53 external interrupt 3
#define IEX4_VECTOR    11       // 0x5B external interrupt 4
#define IEX5_VECTOR    12       // 0x63 external interrupt 5
#define IEX6_VECTOR    13       // 0x6B external interrupt 6
#endif

#ifdef MICROCONTROLLER_SAB80517
#define TF2_VECTOR      5       // 0x2B timer 2
#define EX2_VECTOR      5       // 0x2B external interrupt 2
#define IADC_VECTOR     8       // 0x43 A/D converter interrupt
#define IEX2_VECTOR     9       // 0x4B external interrupt 2
#define IEX3_VECTOR    10       // 0x53 external interrupt 3
#define IEX4_VECTOR    11       // 0x5B external interrupt 4
#define IEX5_VECTOR    12       // 0x63 external interrupt 5
#define IEX6_VECTOR    13       // 0x6B external interrupt 6
                                // 0x73 not used
                                // 0x7B not used
#define SI1_VECTOR     16       // 0x83 serial port 1
                                // 0x8B not used
                                // 0x93 not used
#define COMPARE_VECTOR 19       // 0x9B compare
#endif

#endif	// End of the header -> #ifndef MCS51REG_H