/*-------------------------------------------------------------------------
  CPU depending Declarations

   Written By - Dipl.-Ing. (FH) Michael Schmitt
    Bug-Fix Oct 15 1999
    mschmitt@mainz-online.de
    michael.schmitt@t-online.de

   Thanks to Josef Wolf <jw@raven.inka.de> for the serial io functions
   via interrupts

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

// This file contains CPU depending utilities
// some of these parts are taken from sources made public by the sdcc project
// special thanks to sandeep for his great work and all other

// First some usseful definitions
#define FALSE 0
#define TRUE !FALSE

// here is a definition of a single nop command as it has to be declared under keil-C and sdcc
#ifdef SDCC
	#define NOP _asm nop _endasm
#else
	// This is for Keil-C
	#define NOP _nop_()
#endif

// ===========================================================================================
// Here are some CPU depending definitions that have to checked

#ifndef EXTENDED_CPU_IDLE
void CpuIdle( void )
{
    PCON |= 0x01; /* Setting IDL-Bit of Register PCON */
}
#else
	#define CpuIdle()  PCON |= 0x01; PCON |= 0x20          // start the idle mode...
#endif

void WatchDog( void )
{
    // no watchdog yet
}


// ===========================================================================================
// Serial IO using the internal UART in interrupt mode
#ifdef SERIAL_VIA_INTERRUPT

#define SERIAL_IS_DEFINED

    // Transmit Buffersize
#ifndef SERIAL_VIA_INTERRUPT_XBUFLEN
	#error "SERIAL_VIA_INTERRUPT_XBUFLEN not defined using default"
	#define SERIAL_VIA_INTERRUPT_XBUFLEN 8
#endif

    // Receive Buffer Size
#ifndef SERIAL_VIA_INTERRUPT_RBUFLEN
	#error "SERIAL_VIA_INTERRUPT_RBUFLEN not defined using default"
	#define SERIAL_VIA_INTERRUPT_RBUFLEN 8
#endif

// for interrupt mode we implement a ring buffer
volatile xdata static unsigned char SERIAL_VIA_INTERRUPT_RBUF[SERIAL_VIA_INTERRUPT_RBUFLEN];
volatile xdata static unsigned char SERIAL_VIA_INTERRUPT_XBUF[SERIAL_VIA_INTERRUPT_XBUFLEN];
volatile static unsigned char SERIAL_VIA_INTERRUPT_RCNT;
volatile static unsigned char SERIAL_VIA_INTERRUPT_XCNT;
volatile static unsigned char SERIAL_VIA_INTERRUPT_RPOS;
volatile static unsigned char SERIAL_VIA_INTERRUPT_XPOS;
volatile static unsigned char SERIAL_VIA_INTERRUPT_BUSY;

#endif // SERIAL_VIA_INTERRUPT

#ifdef SERIAL_VIA_POLLING
	#define SERIAL_IS_DEFINED
#endif // SERIAL_VIA_POLLING

#ifdef SERIAL_IS_DEFINED
    // Here are some definitions about the desired serial interface
	#ifndef CPUCLKHZ
		#error "CPUCLKHZ is not defined !"
	#endif
	#ifndef BAUDRATE
	#error "BAUDRATE is not defined !"
	#endif

	#define TIMER1MODE2RELOADVALUE  (256-(1*CPUCLKHZ/32/12/BAUDRATE))
#endif // SERIAL_IS_DEFINED

#ifdef SERIAL_VIA_INTERRUPT
void ser_handler (void) interrupt 4
{
   if (RI) {
       RI = 0;
       /* don't overwrite chars already in buffer */
       if (SERIAL_VIA_INTERRUPT_RCNT < SERIAL_VIA_INTERRUPT_RBUFLEN)
           SERIAL_VIA_INTERRUPT_RBUF [(SERIAL_VIA_INTERRUPT_RPOS+SERIAL_VIA_INTERRUPT_RCNT++) % SERIAL_VIA_INTERRUPT_RBUFLEN] = SBUF;
   }
   if (TI) {
       TI = 0;
       if (SERIAL_VIA_INTERRUPT_BUSY = SERIAL_VIA_INTERRUPT_XCNT) {   /* Assignment, _not_ comparison! */
           SERIAL_VIA_INTERRUPT_XCNT--;
           SBUF = SERIAL_VIA_INTERRUPT_XBUF [SERIAL_VIA_INTERRUPT_XPOS++];
           if (SERIAL_VIA_INTERRUPT_XPOS >= SERIAL_VIA_INTERRUPT_XBUFLEN)
               SERIAL_VIA_INTERRUPT_XPOS = 0;
       }
   }
}

void putchar(  char Byte )
{
   while (SERIAL_VIA_INTERRUPT_XCNT >= SERIAL_VIA_INTERRUPT_XBUFLEN) /* wait for room in buffer */
   {
        WatchDog();
        CpuIdle();
   }
   ES = 0;
   if (SERIAL_VIA_INTERRUPT_BUSY) {
       SERIAL_VIA_INTERRUPT_XBUF[(SERIAL_VIA_INTERRUPT_XPOS+SERIAL_VIA_INTERRUPT_XCNT++) % SERIAL_VIA_INTERRUPT_XBUFLEN] = Byte;
   } else {
       SBUF = Byte;
       SERIAL_VIA_INTERRUPT_BUSY = 1;
   }
   ES = 1;
}

unsigned char getchar( void )
{
   unsigned char c;
   while (!SERIAL_VIA_INTERRUPT_RCNT)   /* wait for character */
   {
        WatchDog();
        CpuIdle();
   }
   ES = 0;
   SERIAL_VIA_INTERRUPT_RCNT--;
   c = SERIAL_VIA_INTERRUPT_RBUF [SERIAL_VIA_INTERRUPT_RPOS++];
   if (SERIAL_VIA_INTERRUPT_RPOS >= SERIAL_VIA_INTERRUPT_RBUFLEN)
       SERIAL_VIA_INTERRUPT_RPOS = 0;
   ES = 1;
   return (c);
}
#endif

// ===========================================================================================
// Now the Internal UART Handling if Polling Method is used
#ifdef SERIAL_VIA_POLLING
void putchar(  char Byte )
{
    while( !TI ) /* wait for TI==1 */
    {
        WatchDog();
        CpuIdle();
    }
    SBUF = Byte;
    TI = 0;
}

unsigned char getchar( void )
{
    while( !RI ) /* wait for RI==1 */
    {
        WatchDog();
        CpuIdle();
    }
    RI=0;
    return (SBUF);
}
#endif

// ===========================================================================================
// System Timer
#ifdef USE_SYSTEM_TIMER

volatile unsigned long SystemTicks1msec;

// Here are the definitions of the 1kHz Timer 0
// used for delay( xx_msec )
#ifndef CPUCLKHZ
#error "CPUCLKHZ is not defined !"
#endif

#define TIMER0INTSPERSECOND     1000
#define TIMER0MODE1RELOADVALUE  (-((CPUCLKHZ/TIMER0INTSPERSECOND)/12))

void delayMsec( unsigned long delayTime )
{
    delayTime += SystemTicks1msec;
    while( SystemTicks1msec < delayTime )
    {
        CpuIdle();
    }
}

void timer0_isr (void) interrupt 1
{
    /* Interruptrate will be slightly slower than wanted */
    /* because TL0 is not 0 here, not bug-fix yet */
    TL0 = (TIMER0MODE1RELOADVALUE & 0x00FF);
    TH0 = (TIMER0MODE1RELOADVALUE >> 8);
    SystemTicks1msec++;
}
#endif // USE_SYSTEM_TIMER

// ===========================================================================================
// Global Hardware Init
void init_hardware( void )
{
#ifdef USE_SYSTEM_TIMER
    SystemTicks1msec = 0x0;
#endif // USE_SYSTEM_TIMER

    EA = 0;         /* Disable ALL Ints */
    ES = 0;         /* Disable Serial Int */
    ET1 = 0;        /* Disable Timer 1 Int */
    EX1 = 0;        /* Disable External Interrupt 1 */
    ET0 = 0;        /* Disable Timer 0 Int */
    EX0 = 0;        /* Disable External Interrupt 0 */

    TR1 = 0;        /* Stop Timer 1 */
    TR0 = 0;        /* Stop Timer 0 */

#ifdef SERIAL_VIA_INTERRUPT
    SERIAL_VIA_INTERRUPT_RCNT = SERIAL_VIA_INTERRUPT_XCNT = SERIAL_VIA_INTERRUPT_RPOS = SERIAL_VIA_INTERRUPT_XPOS = 0;  /* init buffers */
    SERIAL_VIA_INTERRUPT_BUSY = 0;
#endif

#ifdef SERIAL_IS_DEFINED
    /* Init UART 9600Baud 8-Bit using Timer 1 in Mode 2*/
    /* Set Reload Values for CPU-Clk and BaudRate */
    TH1 = TIMER1MODE2RELOADVALUE;
    TL1 = TIMER1MODE2RELOADVALUE;
    TMOD &= 0x0f;       /* First we set Timer 1 */
    TMOD |= 0x20;       /* Setting Timer 1 as GATE=0 TIMER and MODE 2 8-bit auto reload */
    TR1  = 1;           /* Enabling Timer 1 */
#endif

#ifdef USE_SYSTEM_TIMER
    /* Init Timer 0 as Software Interrupt for 1mSec Timer */
    TL0 = (TIMER0MODE1RELOADVALUE & 0x00FF);
    TH0 = (TIMER0MODE1RELOADVALUE >> 8);
    TMOD &= 0xf0;       /* Now Timer 0 */
    TMOD |= 0x01;       /* Setting Timer 0 as GATE=0 TIMER and MODE 1 16-bit Timer mode */
    TR0  = 1;           /* Enabling Timer 0 */
#endif

#ifdef SERIAL_IS_DEFINED
    SCON  = 0x40;       /* Init Serial Port as 8-Bit UART with variable Baudrate */
    SCON |= 0x10;       /* Enabling Serial reception */
    SCON |= 0x02;       /* Setting TI-Bit */
#endif

#ifdef USE_SYSTEM_TIMER
    TR0 = 1;        /* Start Timer 0 */
    ET0 = 1;        /* Enable Timer 0 Interrupt */
#endif

#ifdef SERIAL_VIA_INTERRUPT
    ES = 1;         /* Enable Serial Interrupt */
#endif
    EA = 1;         /* Enable all Enabled Interrupts */

}
