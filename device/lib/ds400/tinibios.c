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
#include <ds400rom.h>

#define TIMED_ACCESS(sfr,value) { TA=0xaa; TA=0x55; sfr=value; }

unsigned char _sdcc_external_startup(void)
{
    IE = 0; // Disable all interrupts.

    PSW = 0;
    
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

  serial0Buffered=buffered;
 
 if (buffered) {
    installInterrupt(Serial0IrqHandler, 0x23);
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


void Serial0SwitchToBuffered(void)
{
    IE &= ~0x80;
    
    serial0Buffered = 1;
    installInterrupt(Serial0IrqHandler, 0x23);
    RI_0=TI_0=0; // clear "pending" interrupts
    ES0 = 1; // enable serial channel 0 interrupt
    
    IE |= 0x80;
}


void Serial0IrqHandler (void) interrupt 4 {
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
    TI_0 = 0;
    SBUF0=c;
    // TI_0=0;
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
