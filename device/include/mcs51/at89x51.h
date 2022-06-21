/*-------------------------------------------------------------------------
   at89x51.h - register declarations for ATMEL 89x51 processors

   Copyright (C) 1999, Bernd Bartmann <bernd.bartmann AT gmail.com>

   Based on reg51.h by Sandeep Dutta sandeep.dutta AT usa.net
   KEIL C compatible definitions are included

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef AT89x51_H
#define AT89x51_H

#include <compiler.h>

/* BYTE addressable registers */
SFR(P0,		0x80);
SFR(SP,		0x81);
SFR(DPL,	0x82);
SFR(DPH,	0x83);
SFR(PCON,	0x87);
SFR(TCON,	0x88);
SFR(TMOD,	0x89);
SFR(TL0,	0x8A);
SFR(TL1,	0x8B);
SFR(TH0,	0x8C);
SFR(TH1,	0x8D);
SFR(P1,		0x90);
SFR(SCON,	0x98);
SFR(SBUF,	0x99);
SFR(P2,		0xA0);
SFR(IE,		0xA8);
SFR(P3,		0xB0);
SFR(IP,		0xB8);
SFR(PSW,	0xD0);
SFR(ACC,	0xE0);
SFR(A,		0xE0);
SFR(B,		0xF0);


/* BIT addressable registers */
/* P0 */
SBIT(P0_0,	0x80);
SBIT(P0_1,	0x81);
SBIT(P0_2,	0x82);
SBIT(P0_3,	0x83);
SBIT(P0_4,	0x84);
SBIT(P0_5,	0x85);
SBIT(P0_6,	0x86);
SBIT(P0_7,	0x87);

/* TCON */
SBIT(IT0,	0x88);
SBIT(IE0,	0x89);
SBIT(IT1,	0x8A);
SBIT(IE1,	0x8B);
SBIT(TR0,	0x8C);
SBIT(TF0,	0x8D);
SBIT(TR1,	0x8E);
SBIT(TF1,	0x8F);

/* P1 */
SBIT(P1_0,	0x90);
SBIT(P1_1,	0x91);
SBIT(P1_2,	0x92);
SBIT(P1_3,	0x93);
SBIT(P1_4,	0x94);
SBIT(P1_5,	0x95);
SBIT(P1_6,	0x96);
SBIT(P1_7,	0x97);

/* SCON */
SBIT(RI,	0x98);
SBIT(TI,	0x99);
SBIT(RB8,	0x9A);
SBIT(TB8,	0x9B);
SBIT(REN,	0x9C);
SBIT(SM2,	0x9D);
SBIT(SM1,	0x9E);
SBIT(SM0,	0x9F);

/* P2 */
SBIT(P2_0,	0xA0);
SBIT(P2_1,	0xA1);
SBIT(P2_2,	0xA2);
SBIT(P2_3,	0xA3);
SBIT(P2_4,	0xA4);
SBIT(P2_5,	0xA5);
SBIT(P2_6,	0xA6);
SBIT(P2_7,	0xA7);

/* IE */
SBIT(EX0,	0xA8);
SBIT(ET0,	0xA9);
SBIT(EX1,	0xAA);
SBIT(ET1,	0xAB);
SBIT(ES,	0xAC);
SBIT(EA,	0xAF);

/* P3 */
SBIT(P3_0,	0xB0);
SBIT(P3_1,	0xB1);
SBIT(P3_2,	0xB2);
SBIT(P3_3,	0xB3);
SBIT(P3_4,	0xB4);
SBIT(P3_5,	0xB5);
SBIT(P3_6,	0xB6);
SBIT(P3_7,	0xB7);

SBIT(RXD,	0xB0);
SBIT(TXD,	0xB1);
SBIT(INT0,	0xB2);
SBIT(INT1,	0xB3);
SBIT(T0,	0xB4);
SBIT(T1,	0xB5);
SBIT(WR,	0xB6);
SBIT(RD,	0xB7);

/* IP */ 
SBIT(PX0,	0xB8);
SBIT(PT0,	0xB9);
SBIT(PX1,	0xBA);
SBIT(PT1,	0xBB);
SBIT(PS,	0xBC);

/* PSW */
SBIT(P,		0xD0);
SBIT(FL,	0xD1);
SBIT(OV,	0xD2);
SBIT(RS0,	0xD3);
SBIT(RS1,	0xD4);
SBIT(F0,	0xD5);
SBIT(AC,	0xD6);
SBIT(CY,	0xD7);


/* BIT definitions for bits that are not directly accessible */
/* PCON bits */
#define IDL             0x01
#define PD              0x02
#define GF0             0x04
#define GF1             0x08
#define SMOD            0x80

#define IDL_            0x01
#define PD_             0x02
#define GF0_            0x04
#define GF1_            0x08
#define SMOD_           0x80

/* TMOD bits */
#define M0_0            0x01
#define M1_0            0x02
#define C_T0            0x04
#define GATE0           0x08
#define M0_1            0x10
#define M1_1            0x20
#define C_T1            0x40
#define GATE1           0x80

#define M0_0_           0x01
#define M1_0_           0x02
#define C_T0_           0x04
#define GATE0_          0x08
#define M0_1_           0x10
#define M1_1_           0x20
#define C_T1_           0x40
#define GATE1_          0x80

#define T0_M0           0x01
#define T0_M1           0x02
#define T0_CT           0x04
#define T0_GATE         0x08
#define T1_M0           0x10
#define T1_M1           0x20
#define T1_CT           0x40
#define T1_GATE         0x80

#define T0_M0_          0x01
#define T0_M1_          0x02
#define T0_CT_          0x04
#define T0_GATE_        0x08
#define T1_M0_          0x10
#define T1_M1_          0x20
#define T1_CT_          0x40
#define T1_GATE_        0x80

#define T0_MASK         0x0F
#define T1_MASK         0xF0

#define T0_MASK_        0x0F
#define T1_MASK_        0xF0


/* Interrupt numbers: address = (number * 8) + 3 */
#define IE0_VECTOR      0       /* 0x03 external interrupt 0 */
#define TF0_VECTOR      1       /* 0x0b timer 0 */
#define IE1_VECTOR      2       /* 0x13 external interrupt 1 */
#define TF1_VECTOR      3       /* 0x1b timer 1 */
#define SI0_VECTOR      4       /* 0x23 serial port 0 */
 
#endif
