/*-------------------------------------------------------------------------
   Register Declarations for the Oki MSM80C154S and MSM83C154S

   Written By -  Matthias Arndt / marndt@asmsoftware.de (July 2005)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#ifndef MSM8xC154S_H
#define MSM8xC154S_H

#include <8052.h>     /* load definitions for the 8052 core */

#ifdef REG8052_H
#undef REG8052_H
#endif

/* byte SFRs */
__sfr __at (0xf8) 	IOCON;	/* IOCON register */

/* bit locations */
__sbit __at (0xf8)	ALF;	/* floating status on power down control */
__sbit __at (0xf9)	P1HZ;	/* P1 high impedance input control */
__sbit __at (0xfa)	P2HZ;	/* P2 high impedance input control */
__sbit __at (0xfb)	P3HZ;	/* P3 high impedance input control */
__sbit __at (0xfc)	IZC;	/* 10kO pull-up resistor control */
__sbit __at (0xfd)	SERR;	/* Serial port reception flag */
__sbit __at (0xfe)	T32;	/* interconnect T0 and T1 to 32bit timer/counter */

/* Bits in IP (0xb8) */
__sbit __at (0xbf)	PCT;	/* Priority interrupt circuit control bit */

/* Bits in PCON (0x87) */
#define RPD 0x20            /* Bit used to specify cancellation of CPU power down mode */
#define HPD 0x40            /* The hard power down setting mode is enabled when this bit is set to "1". */

#endif
