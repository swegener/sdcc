/*-------------------------------------------------------------------------
  Include file for serial390.c. You can change some default's here.
  
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

#ifndef SERIAL390_H
#define SERIAL390_H

/* Here we have to decide whether or not to use interrupts for serial output.
   If your debug messages (don't we use them all :), doesn't show up
   like you expect (e.g. when you screwed up the stack or tilted the cpu 
   otherwise) or if you're expecting a lot of them that might overflow 
   the ring buffer, disable it. It will slow down other things you might be
   doing, but will leave serial output synchronized with that. */
#define USE_SERIAL_INTERRUPTS 1

/* Define the size of the ring buffer.
   This must be a binary number, since we shouldn't use mod (%) in
   interrupt routines */
#define SERIAL_RECEIVE_BUFFER_SIZE 1024

/* I'd rather had this calculation be done in Serial390Init(),
   but since the compiler generates e.g. _divuchar() calls
   which aren't in the library yet, we do it here. */
#define BAUD_RATE 115200
#define OSCILLATOR 18432000L
#define TIMER1_RELOAD_VALUE -(OSCILLATOR/(32L*BAUD_RATE))

/* there shouldn't be any reason to change anything below this line.
   If there is, please let me know */

extern void Serial390Init (void);

#if USE_SERIAL_INTERRUPTS

/* the interrupt vector will be automaticly set when the 
   main program #include-s this file */
extern void Serial390Handler (void) interrupt 4;

#endif

#endif SERIAL390_H
