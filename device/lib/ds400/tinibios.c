/*-------------------------------------------------------------------------
  tinibios.c - startup and serial routines for the DS80C400 (tested on TINIM400)
  
   Written By - Johan Knol, johan.knol@iduna.nl
   
   Further hacked by Kevin Vigor with invaluable assistance from Bob Heise.
    
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

#include <tinibios.h>

#define TIMED_ACCESS(sfr,value) { TA=0xaa; TA=0x55; sfr=value; }

// FIXME: Doesn't work, maybe?
static void _installInterrupt(void (*isrPtr)(void), unsigned char offset)
{
    unsigned char xdata * vectPtr = (unsigned char xdata *) offset;
    unsigned long isr = (unsigned long)isrPtr;
    
    *vectPtr++ = 0x02;
    *vectPtr++ = (unsigned char)(isr >> 16);
    *vectPtr++ = (unsigned char)(isr >> 8);
    *vectPtr = (unsigned char)isr;
}

unsigned char _sdcc_external_startup(void)
{
#if 0    
    int i, j;

    // Do some blinking of the LED.
    for  (j = 0; j < 10; j++)
    {
	P5 |= 4;
	for (i = 0; i < 32767; i++)
	{
	    ;
	}
	P5 &= ~4;
	for (i = 0; i < 32767; i++)
	{
	    ;
	}	
    }
#endif    
    
    IE = 0; // Disable all interrupts.
    
  _asm
    ; save the 24-bit return address
    pop ar2; msb
    pop ar1
    pop ar0; lsb


    mov _ESP,#0x00; reinitialize the stack
    mov _SP,#0x00

    ; restore the 24-bit return address
    push ar0; lsb
    push ar1
    push ar2; msb
  _endasm;    
    
    // Stub: call rom_init here, then fixup IVT.
    // 
    
    Serial0Init(1, 0); // baud argument ignored.
    
    IE = 0x80; // Enable interrupts.
    
    return 0;
}

// now the serial0 stuff

// just to make the code more readable 
#define S0RBS SERIAL_0_RECEIVE_BUFFER_SIZE

// this is a ring buffer and can overflow at anytime!
static volatile unsigned char receive0Buffer[S0RBS];
static volatile int receive0BufferHead=0;
static volatile int receive0BufferTail=0;
// no buffering for transmit
static volatile char transmit0IsBusy=0;

static data unsigned char serial0Buffered;

/* Initialize serial0.

   Available baudrates are from 110 upto 115200 (using 16-bit timer 2)
   If baud==0, the port is disabled.

   If buffered!=0, characters received are buffered using an interrupt
*/

void Serial0Init (unsigned long baud, unsigned char buffered) {
  
  ES0 = 0; // disable serial channel 0 interrupt

  // Need no port setup, done by boot rom.
  
  baud;
  
  // hack serial ISR in, no magic support for ISR routines yet.
  //
  // FIXME: this doesn't work, use only non-buffered mode!
  
  _installInterrupt(Serial0IrqHandler, 0x23);  
    
  serial0Buffered=buffered;
 
 if (buffered) {
    RI_0=TI_0=0; // clear "pending" interrupts
    ES0 = 1; // enable serial channel 0 interrupt
  } else {
    RI_0=0; // receive buffer empty
    TI_0=1; // transmit buffer empty
  }
}

void Serial0Baud(unsigned long baud) {
  TR2=0; // stop timer
  baud=-((long)OSCILLATOR/(32*baud));
  TL2=RCAP2L= baud;
  TH2=RCAP2H= baud>>8;
  TF2=0; // clear overflow flag
  TR2=1; // start timer
}  

void Serial0IrqHandler (void) interrupt 4 {
  P5 |= 4;
  if (RI_0) {
    receive0Buffer[receive0BufferHead]=SBUF0;
    receive0BufferHead=(receive0BufferHead+1)&(S0RBS-1);
    if (receive0BufferHead==receive0BufferTail) {
      /* buffer overrun, sorry :) */
      receive0BufferTail=(receive0BufferTail+1)&(S0RBS-1);
    }
    RI_0=0;
  }
  if (TI_0) {
    TI_0=0;
    transmit0IsBusy=0;
  }
}

char Serial0CharArrived(void) {
  if (serial0Buffered) {
    if (receive0BufferHead!=receive0BufferTail)
      return receive0Buffer[receive0BufferTail];
  } else {
    if (RI_0)
      return SBUF0;
  }
  return 0;
}

void Serial0PutChar (char c)
{
  if (serial0Buffered) {
    while (transmit0IsBusy)
      ;
    transmit0IsBusy=1;
    SBUF0=c;
  } else {
    while (!TI_0)
      ;
    SBUF0=c;
    TI_0=0;
  }
}

char Serial0GetChar (void)
{
  char c;
  if (serial0Buffered) {
    while (receive0BufferHead==receive0BufferTail)
      ;
    c=receive0Buffer[receive0BufferTail];
    ES0=0; // disable serial interrupts
    receive0BufferTail=(receive0BufferTail+1)&(S0RBS-1);
    ES0=1; // enable serial interrupts
  } else {
    while (!RI_0)
      ;
    c=SBUF0;
    RI_0=0;
  }
  return c;
}

#if 0 
// FIXME: no ClockMilliSecondsDelay yet.
void Serial0SendBreak() {
  P3 &= ~0x02;
  ClockMilliSecondsDelay(2);
  P3 |= 0x02;
}
#endif

void Serial0Flush() {
  ES0=0; // disable interrupts
  receive0BufferHead=receive0BufferTail=0;
  RI_0=0;
  if (serial0Buffered) {
    TI_0=0;
    ES0=1; // enable interrupts
  } else {
    TI_0=1;
  }
}
