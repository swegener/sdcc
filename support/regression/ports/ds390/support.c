/*-------------------------------------------------------------------------
  support.c - startup for regression tests with uCsim
  
   Copied from tinibios.c, which was written By - Johan Knol, johan.knol@iduna.nl
    
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

#include <ds80c390.h>

#define TIMED_ACCESS(sfr,value) { TA=0xaa; TA=0x55; sfr=value; }

signed char _sdcc_external_startup(void)
{
  TI_0 = 1;	// init transmitter

/* copied from tinibios.h */

  IE=0; // disable ALL interrupts

  // use A19..16 and !CE3..0, no CAN
  TIMED_ACCESS(P4CNT,0x3f);

  // use !PCE3..0, serial 1 at P5.2/3
  TIMED_ACCESS(P5CNT,0x27);

  // disable watchdog
  EWT=0;

  // watchdog set to 9.1 seconds
  // CKCON|=0xc0;

  // default stretch cycles for MOVX
  //CKCON = (CKCON&0xf8)|(CPU_MOVX_STRETCH&0x07);
  CKCON=0xf9;

  // use internal 4k RAM as data(stack) memory at 0x400000 and
  // move CANx memory access to 0x401000 and upwards
  // use !CE* for program and/or data memory access
  TIMED_ACCESS(MCON,0xaf);

  // select default cpu speed
  //CpuSpeed(CPU_SPEED); will hang on s51

  _asm
    ; save the 24-bit return address
    pop ar2; msb
    pop ar1
    pop ar0; lsb


    mov _TA,#0xaa; timed access
    mov _TA,#0x55
    mov _ACON,#0x06; 24 bit addresses, 10 bit stack at 0x400000

    mov _ESP,#0x00; reinitialize the stack
    mov _SP,#0x00

    ; restore the 24-bit return address
    push ar0; lsb
    push ar1
    push ar2; msb
  _endasm;

  // global interrupt enable, all masks cleared
  // let the Gods be with us :)
  // IE = 0x80;

  //Serial0Init(SERIAL_0_BAUD,1);
  //Serial1Init(SERIAL_1_BAUD,1);
  //ClockInit();
  //RtcInit();
  //WatchDogInit();

  // signal _sdcc_gsinit_startup to initialize data (call _sdcc_init_data)
  return 0;
}

void
_putchar (char c)
{
  while (!TI_0)
    ;
  TI_0 = 0;
  SBUF0 = c;
}

void
_exitEmu (void)
{
  * (char idata *) 0 = * (char xdata *) 0x7654;
}
