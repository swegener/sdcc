/*-------------------------------------------------------------------------
  Register Declarations for Motorola MC68HC908QY & MC68HC908QT
  
   Written By - Erik Petrich
    epetrich@users.sourceforge.net (2003)
    
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
-------------------------------------------------------------------------*/

#ifndef MC68HC908QY_H
#define MC68HC908QY_H

#ifndef UINT8
 #define UINT8 unsigned char
#endif
#ifndef UINT16
 #define UINT16 unsigned int
#endif
#ifndef VOLDATA
 #define VOLDATA volatile data
#endif
#ifndef VOLXDATA
 #define VOLXDATA volatile xdata
#endif

struct _hc08_bits
{
  unsigned int bit0:1;
  unsigned int bit1:1;
  unsigned int bit2:1;
  unsigned int bit3:1;
  unsigned int bit4:1;
  unsigned int bit5:1;
  unsigned int bit6:1;
  unsigned int bit7:1;
};
  

VOLDATA UINT8 at 0x00 PTA;     /* Port A Data Register */
  #define PTA0 ((struct _hc08_bits *)(&PTA))->bit0
  #define PTA1 ((struct _hc08_bits *)(&PTA))->bit1
  #define PTA2 ((struct _hc08_bits *)(&PTA))->bit2
  #define PTA3 ((struct _hc08_bits *)(&PTA))->bit3
  #define PTA4 ((struct _hc08_bits *)(&PTA))->bit4
  #define PTA5 ((struct _hc08_bits *)(&PTA))->bit5
  #define AWUL ((struct _hc08_bits *)(&PTA))->bit6
  
VOLDATA UINT8 at 0x01 PTB;     /* Port B Data Register */
  #define PTB0 ((struct _hc08_bits *)(&PTB))->bit0
  #define PTB1 ((struct _hc08_bits *)(&PTB))->bit1
  #define PTB2 ((struct _hc08_bits *)(&PTB))->bit2
  #define PTB3 ((struct _hc08_bits *)(&PTB))->bit3
  #define PTB4 ((struct _hc08_bits *)(&PTB))->bit4
  #define PTB5 ((struct _hc08_bits *)(&PTB))->bit5
  #define PTB6 ((struct _hc08_bits *)(&PTB))->bit6
  #define PTB7 ((struct _hc08_bits *)(&PTB))->bit7

VOLDATA UINT8 at 0x04 DDRA;    /* Data Direction Register A */
  #define DDRA0 ((struct _hc08_bits *)(&DDRA))->bit0
  #define DDRA1 ((struct _hc08_bits *)(&DDRA))->bit1
  #define DDRA3 ((struct _hc08_bits *)(&DDRA))->bit3
  #define DDRA4 ((struct _hc08_bits *)(&DDRA))->bit4
  #define DDRA5 ((struct _hc08_bits *)(&DDRA))->bit5

VOLDATA UINT8 at 0x05 DDRB;    /* Data Direction Register B */
  #define DDRB0 ((struct _hc08_bits *)(&DDRB))->bit0
  #define DDRB1 ((struct _hc08_bits *)(&DDRB))->bit1
  #define DDRB2 ((struct _hc08_bits *)(&DDRB))->bit2
  #define DDRB3 ((struct _hc08_bits *)(&DDRB))->bit3
  #define DDRB4 ((struct _hc08_bits *)(&DDRB))->bit4
  #define DDRB5 ((struct _hc08_bits *)(&DDRB))->bit5
  #define DDRB6 ((struct _hc08_bits *)(&DDRB))->bit6
  #define DDRB7 ((struct _hc08_bits *)(&DDRB))->bit7

VOLDATA UINT8 at 0x0b PTAPUE;  /* Port A Input Pullup Enable Register */
  #define PTAPUE0 ((struct _hc08_bits *)(&PTAPUE))->bit0
  #define PTAPUE1 ((struct _hc08_bits *)(&PTAPUE))->bit1
  #define PTAPUE2 ((struct _hc08_bits *)(&PTAPUE))->bit2
  #define PTAPUE3 ((struct _hc08_bits *)(&PTAPUE))->bit3
  #define PTAPUE4 ((struct _hc08_bits *)(&PTAPUE))->bit4
  #define PTAPUE5 ((struct _hc08_bits *)(&PTAPUE))->bit5
  #define OSC2EN ((struct _hc08_bits *)(&PTAPUE))->bit7

VOLDATA UINT8 at 0x0c PTBPUE;  /* Port B Input Pullup Enable Register */
  #define PTBPUE0 ((struct _hc08_bits *)(&PTBPUE))->bit0
  #define PTBPUE1 ((struct _hc08_bits *)(&PTBPUE))->bit1
  #define PTBPUE2 ((struct _hc08_bits *)(&PTBPUE))->bit2
  #define PTBPUE3 ((struct _hc08_bits *)(&PTBPUE))->bit3
  #define PTBPUE4 ((struct _hc08_bits *)(&PTBPUE))->bit4
  #define PTBPUE5 ((struct _hc08_bits *)(&PTBPUE))->bit5
  #define PTBPUE6 ((struct _hc08_bits *)(&PTBPUE))->bit6
  #define PTBPUE7 ((struct _hc08_bits *)(&PTBPUE))->bit7

VOLDATA UINT8 at 0x1a KBSCR;   /* Keyboard Status and Control Register */
  #define MODEK ((struct _hc08_bits *)(&KBSCR))->bit0
  #define IMASKK ((struct _hc08_bits *)(&KBSCR))->bit1
  #define ACKK ((struct _hc08_bits *)(&KBSCR))->bit2
  #define KEYF ((struct _hc08_bits *)(&KBSCR))->bit3

VOLDATA UINT8 at 0x1b KBIER;   /* Keyboard Interrupt Enable Register */
  #define KBIE0 ((struct _hc08_bits *)(&KBIER))->bit0
  #define KBIE1 ((struct _hc08_bits *)(&KBIER))->bit1
  #define KBIE2 ((struct _hc08_bits *)(&KBIER))->bit2
  #define KBIE3 ((struct _hc08_bits *)(&KBIER))->bit3
  #define KBIE4 ((struct _hc08_bits *)(&KBIER))->bit4
  #define KBIE5 ((struct _hc08_bits *)(&KBIER))->bit5
  #define AWUIE ((struct _hc08_bits *)(&KBIER))->bit6

VOLDATA UINT8 at 0x1d INTSCR;  /* IRQ Status and Control Register */
  #define MODE1 ((struct _hc08_bits *)(&INTSCR))->bit0
  #define IMASK1 ((struct _hc08_bits *)(&INTSCR))->bit1
  #define ACK1 ((struct _hc08_bits *)(&INTSCR))->bit2
  #define IRQF1 ((struct _hc08_bits *)(&INTSCR))->bit3

VOLDATA UINT8 at 0x1e CONFIG2; /* Configuration Register 2 */
/* CONFIG2 is one-time writeable, so can't use bitfields */

VOLDATA UINT8 at 0x1f CONFIG1; /* Configuration Register 1 */
/* CONFIG1 is one-time writeable, so can't use bitfields */

VOLDATA UINT8 at 0x20 TSC;     /* TIM Status and Control */
  #define PS0 ((struct _hc08_bits *)(&TSC))->bit0
  #define PS1 ((struct _hc08_bits *)(&TSC))->bit1
  #define PS2 ((struct _hc08_bits *)(&TSC))->bit2
  #define TRST ((struct _hc08_bits *)(&TSC))->bit4
  #define TSTOP ((struct _hc08_bits *)(&TSC))->bit5
  #define TOIE ((struct _hc08_bits *)(&TSC))->bit6
  #define TOF ((struct _hc08_bits *)(&TSC))->bit7

VOLDATA UINT8 at 0x21 TCNTH;   /* TIM Counter Register High */
VOLDATA UINT8 at 0x22 TCNTL;   /* TIM Counter Register Low */
VOLDATA UINT16 at 0x21 TCNT;   /* TIM Counter High & Low Registers */

VOLDATA UINT8 at 0x23 TMODH;   /* TIM Counter Modulo Register High */
VOLDATA UINT8 at 0x24 TMODL;   /* TIM Counter Modulo Register Low */
VOLDATA UINT16 at 0x23 TMOD;   /* TIM Counter Modulo High & Low Registers */

VOLDATA UINT8 at 0x25 TSC0;    /* TIM Channel 0 Status and Control Register */
  #define CH0MAX ((struct _hc08_bits *)(&TSC0))->bit0
  #define TOV0 ((struct _hc08_bits *)(&TSC0))->bit1
  #define ELS0A ((struct _hc08_bits *)(&TSC0))->bit2
  #define ELS0B ((struct _hc08_bits *)(&TSC0))->bit3
  #define MS0A ((struct _hc08_bits *)(&TSC0))->bit4
  #define MS0B ((struct _hc08_bits *)(&TSC0))->bit5
  #define CH0IE ((struct _hc08_bits *)(&TSC0))->bit6
  #define CH0F ((struct _hc08_bits *)(&TSC0))->bit7

VOLDATA UINT8 at 0x26 TCH0H;   /* TIM Channel 0 Register High */
VOLDATA UINT8 at 0x27 TCH0L;   /* TIM Channel 0 Register Low */
VOLDATA UINT16 at 0x26 TCH0;   /* TIM Channel 0 High & Low Registers */

VOLDATA UINT8 at 0x28 TSC1;    /* TIM Channel 1 Status and Control Register */
  #define CH1MAX ((struct _hc08_bits *)(&TSC1))->bit0
  #define TOV1 ((struct _hc08_bits *)(&TSC1))->bit1
  #define ELS1A ((struct _hc08_bits *)(&TSC1))->bit2
  #define ELS1B ((struct _hc08_bits *)(&TSC1))->bit3
  #define MS1A ((struct _hc08_bits *)(&TSC1))->bit4
  #define MS1B ((struct _hc08_bits *)(&TSC1))->bit5
  #define CH1IE ((struct _hc08_bits *)(&TSC1))->bit6
  #define CH1F ((struct _hc08_bits *)(&TSC1))->bit7

VOLDATA UINT8 at 0x29 TCH1H;   /* TIM Channel 1 Register High */
VOLDATA UINT8 at 0x2a TCH1L;   /* TIM Channel 1 Register Low */
VOLDATA UINT16 at 0x29 TCH1;   /* TIM Channel 1 High & Low Registers */

VOLDATA UINT8 at 0x36 OSCSTAT; /* Oscillator Status Register */
  #define ECGST ((struct _hc08_bits *)(&OSCSTAT))->bit0
  #define ECGON ((struct _hc08_bits *)(&OSCSTAT))->bit1

VOLDATA UINT8 at 0x38 OSCTRIM; /* Oscillator Trim Register */

VOLDATA UINT8 at 0x3c ADSCR;   /* ADC Status and Control Register */
  #define CH0 ((struct _hc08_bits *)(&ADSCR))->bit0
  #define CH1 ((struct _hc08_bits *)(&ADSCR))->bit1
  #define CH2 ((struct _hc08_bits *)(&ADSCR))->bit2
  #define CH3 ((struct _hc08_bits *)(&ADSCR))->bit3
  #define CH4 ((struct _hc08_bits *)(&ADSCR))->bit4
  #define ADC0 ((struct _hc08_bits *)(&ADSCR))->bit5
  #define AIEN ((struct _hc08_bits *)(&ADSCR))->bit6
  #define COCO ((struct _hc08_bits *)(&ADSCR))->bit7

VOLDATA UINT8 at 0x3e ADR;     /* ADC Data Register */

VOLDATA UINT8 at 0x3f ADICLK;  /* ADS Input Clock Register */
  #define ADIV0 ((struct _hc08_bits *)(&ADICLK))->bit5
  #define ADIV1 ((struct _hc08_bits *)(&ADICLK))->bit6
  #define ADIV2 ((struct _hc08_bits *)(&ADICLK))->bit7

VOLXDATA UINT8 at 0xfe00 BSR;     /* Break Status Register */
  #define SBSW ((struct _hc08_bits *)(&BSR))->bit1
  
VOLXDATA UINT8 at 0xfe01 SRSR;    /* SIM Reset Status Register */
  #define LVI ((struct _hc08_bits *)(&SRSR))->bit1
  #define MODRST ((struct _hc08_bits *)(&SRSR))->bit2
  #define ILAD ((struct _hc08_bits *)(&SRSR))->bit3
  #define ILOP ((struct _hc08_bits *)(&SRSR))->bit4
  #define COP ((struct _hc08_bits *)(&SRSR))->bit5
  #define PIN ((struct _hc08_bits *)(&SRSR))->bit6
  #define POR ((struct _hc08_bits *)(&SRSR))->bit7
  
VOLXDATA UINT8 at 0xfe02 BRKAR;   /* Break Auxilliary Register */
  #define BDCOP ((struct _hc08_bits *)(&BRKAR))->bit0

VOLXDATA UINT8 at 0xfe03 BFCF;    /* Break Flag Control Register */
  #define BFCF ((struct _hc08_bits *)(&BFCF))->bit7

VOLXDATA UINT8 at 0xfe04 INT1;    /* Interrupt Status Register 1 */
  #define IF1 ((struct _hc08_bits *)(&INT1))->bit2
  #define IF3 ((struct _hc08_bits *)(&INT1))->bit4
  #define IF4 ((struct _hc08_bits *)(&INT1))->bit5
  #define IF5 ((struct _hc08_bits *)(&INT1))->bit6

VOLXDATA UINT8 at 0xfe05 INT2;    /* Interrupt Status Register 2 */
  #define IF14 ((struct _hc08_bits *)(&INT2))->bit7

VOLXDATA UINT8 at 0xfe06 INT3;    /* Interrupt Status Register 3 */
  #define IF15 ((struct _hc08_bits *)(&INT3))->bit0

VOLXDATA UINT8 at 0xfe08 FLCR;    /* FLASH Control Register */
  #define PGM ((struct _hc08_bits *)(&FLCR))->bit0
  #define ERASE ((struct _hc08_bits *)(&FLCR))->bit1
  #define MASS ((struct _hc08_bits *)(&FLCR))->bit2
  #define HVEN ((struct _hc08_bits *)(&FLCR))->bit3

VOLXDATA UINT8 at 0xfe09 BRKH;    /* Break Address High Register */
VOLXDATA UINT8 at 0xfe0a BRKL;    /* Break Address Low Register */
VOLXDATA UINT16 at 0xfe09 BRK;    /* Break Address High & Low Registers */

VOLXDATA UINT8 at 0xfe0b BRKSCR;  /* Break Status and Control Register */
  #define BRKA ((struct _hc08_bits *)(&BRKSCR))->bit6
  #define BRKE ((struct _hc08_bits *)(&BRKSCR))->bit7

VOLXDATA UINT8 at 0xfe0c LVISR;   /* LVI Status Register */
  #define LVIOUT ((struct _hc08_bits *)(&LVISR))->bit7

VOLXDATA UINT8 at 0xfebe FLBPR;   /* FLASH Block Protect Register */
VOLXDATA UINT8 at 0xfec0 OSCTRIMVAL; /* Oscillator Trim Value */
VOLXDATA UINT8 at 0xffff COPCTL;  /* COP Control Register */


#endif

