/*-------------------------------------------------------------------------
  CPU Startup Code

   Written By - Dipl.-Ing. (FH) Michael Schmitt
    Bug-Fix Oct 15 1999
    mschmitt@mainz-online.de
    michael.schmitt@t-online.de

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

/* here are some definition about the CPU */
#include <at89c55.h>
#include <stdio.h>
#define CPUCLKHZ                11059200
//#define CPUCLKHZ                24000000
#define BAUDRATE                9600

// For serial com. we use the internal UART and data exchange is done by interrupt and not via polling
#define SERIAL_VIA_INTERRUPT
#define SERIAL_VIA_INTERRUPT_XBUFLEN 64
#define SERIAL_VIA_INTERRUPT_RBUFLEN 4
//#define SERIAL_VIA_POLLING

// include a 1msec timer
#define USE_SYSTEM_TIMER

// all 8051-CPU utils can be found here
#include "cpu_tools.h"
#include "cpu_tools.c"

// that is needed for printf, will be removed after the debug version

// if the following files are copied to the local folder, enable the next two lines and comment out
// the two lines above
//#include "vprintf.c"
//#include "printf_large.c"

//#include "harddisk.c"

void main()
{
	// This has to be done first
    init_hardware();


    printf( "\n\r<<< RESET >>>\r\n" );

    printf( "\n\rStartTime : " );
    printf( "%ld \n\r",SystemTicks1msec );

    while( 1 )
    {
        printf( "\r%ld ", SystemTicks1msec);
        delayMsec( 999 );
    }
}
