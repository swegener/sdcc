/*-------------------------------------------------------------------------
  serial390.c - serial routines for the DS80C390 (tested on TINI)
  
   Written By - Johan Knol, johan.knol@iduna.nl
    
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

// make sure that no int (16 bit) and long (32 bit) integer division, 
// multiplication & modulus operations are used unless the lib-s are
// compiled reentrant !!!!

// if you want to changed defaults, do so in serial390.h

#include <stdio.h>
#include <ds80c390.h>
#include <serial390.h>

#if USE_SERIAL_INTERRUPTS

// this is a ring buffer and can overflow at anytime!
static volatile unsigned char receiveBuffer[SERIAL_RECEIVE_BUFFER_SIZE];
static volatile int receiveBufferHead=0;
static volatile int receiveBufferTail=0;
// no buffering for transmit
static volatile char transmitIsBusy=0;
#endif

void Serial390Init (void) {
  
  ES0 = 0; // disable serial channel 0 interrupt
  TR1 = 0; // stop timer 1
  
  // set 8 bit uart with variable baud from timer 1
  // enable receiver and clear RI and TI
  SCON0 = 0x50;
  
  PCON |= 0x80; // clock is 16x bitrate
  
  TMOD = (TMOD&0x0f) | 0x20; // timer 1 is an 8bit auto-reload counter
  
  TL1 = TH1 = TIMER1_RELOAD_VALUE;
  
  TF1=0; // clear timer 1 overflow flag
  TR1=1; // start timer 1
  
#if USE_SERIAL_INTERRUPTS
  RI_0=TI_0=0; // clear "pending" interrupts
  ES0 = 1; // enable serial channel 0 interrupt
#else
  RI_0=0; // receive buffer empty
  TI_0=1; // transmit buffer empty
#endif
}

#if USE_SERIAL_INTERRUPTS

void Serial390Handler (void) interrupt 4 {
  if (RI_0) {
    receiveBuffer[receiveBufferHead]=SBUF0;
    //receiveBufferHead=(receiveBufferHead+1)%SERIAL_RECEIVE_BUFFER_SIZE;
    receiveBufferHead=(receiveBufferHead+1)&(SERIAL_RECEIVE_BUFFER_SIZE-1);
    if (receiveBufferHead==receiveBufferTail) /* buffer overrun, sorry :) */
      //receiveBufferTail=(receiveBufferTail+1)%SERIAL_RECEIVE_BUFFER_SIZE;
      receiveBufferTail=(receiveBufferTail+1)&(SERIAL_RECEIVE_BUFFER_SIZE-1);
    RI_0=0;
  }
  if (TI_0) {
    TI_0=0;
    transmitIsBusy=0;
  }
}

void putchar (char c)
{
  while (transmitIsBusy)
    ;
  transmitIsBusy=1;
  SBUF0=c;
}

char getchar (void)
{
  char c;
  while (receiveBufferHead==receiveBufferTail)
    ;
  c=receiveBuffer[receiveBufferTail];
  ES0=0; // disable serial interrupts
  //receiveBufferTail=(receiveBufferTail+1)%SERIAL_RECEIVE_BUFFER_SIZE;
  receiveBufferTail=(receiveBufferTail+1)&(SERIAL_RECEIVE_BUFFER_SIZE-1);
  ES0=1; // enable serial interrupts
  return c;
}

#else //ifdef USE_SERIALINTERRUPTS

// dummy interupt handler
void Serial390Handler (void) interrupt 4 {
  // how did we got here?
  ES0=0; // disable serial interrupts
}

void putchar (char c) {
  while (!TI_0)
    ;
  TI_0=0;
  SBUF0=c;
}

char getchar (void) {
  char c;
  while (!RI_0)
    ;
  c=SBUF0;
  RI_0=0;
  return c;
}

#endif // ifdef USE_SERIALINTERRUPTS

#if 0
static char hex[]="0123456789abcdef";

void putint (unsigned int i) {
  putchar (hex[(i>>12)&0x0f]);
  putchar (hex[(i>> 8)&0x0f]);
  putchar (hex[(i>> 4)&0x0f]);
  putchar (hex[(i    )&0x0f]);
}

#endif
