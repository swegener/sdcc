/*-------------------------------------------------------------------------
   pic18fregs.h - 18F Family Device Library Header

   Copyright (C) 2004, Vangelis Rokas <vrokas AT otenet.gr>

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

#ifndef __PIC18FREGS_H__
#define __PIC18FREGS_H__

#if defined(__SDCC_PIC18F242)
#  include <pic18f242.h>

#elif defined(__SDCC_PIC18F248)
#  include <pic18f248.h>

#elif defined(__SDCC_PIC18F252)
#  include <pic18f252.h>

#elif defined(__SDCC_PIC18F258)
#  include <pic18f258.h>

#elif defined(__SDCC_PIC18F442)
#  include <pic18f442.h>

#elif defined(__SDCC_PIC18F448)
#  include <pic18f448.h>

#elif defined(__SDCC_PIC18F452)
#  include <pic18f452.h>

#elif defined(__SDCC_PIC18F458)
#  include <pic18f458.h>

#elif defined(__SDCC_PIC18F1220)
#  include <pic18f1220.h>

#elif defined(__SDCC_PIC18F1230)
#  include <pic18f1230.h>

#elif defined(__SDCC_PIC18F1320)
#  include <pic18f1320.h>

#elif defined(__SDCC_PIC18F1330)
#  include <pic18f1330.h>

#elif defined(__SDCC_PIC18F13K50)
#  include <pic18f13k50.h>

#elif defined(__SDCC_PIC18F14K50)
#  include <pic18f14k50.h>

#elif defined(__SDCC_PIC18F2220)
#  include <pic18f2220.h>

#elif defined(__SDCC_PIC18F2221)
#  include <pic18f2221.h>

#elif defined(__SDCC_PIC18F2320)
#  include <pic18f2320.h>

#elif defined(__SDCC_PIC18F2321)
#  include <pic18f2321.h>

#elif defined(__SDCC_PIC18F2331)
#  include <pic18f2331.h>

#elif defined(__SDCC_PIC18F23K20)
#  include <pic18f23k20.h>

#elif defined(__SDCC_PIC18F23K22)
#  include <pic18f23k22.h>

#elif defined(__SDCC_PIC18F2410)
#  include <pic18f2410.h>

#elif defined(__SDCC_PIC18F2420)
#  include <pic18f2420.h>

#elif defined(__SDCC_PIC18F2423)
#  include <pic18f2423.h>

#elif defined(__SDCC_PIC18F2431)
#  include <pic18f2431.h>

#elif defined(__SDCC_PIC18F2450)
#  include <pic18f2450.h>

#elif defined(__SDCC_PIC18F2455)
#  include <pic18f2455.h>

#elif defined(__SDCC_PIC18F2480)
#  include <pic18f2480.h>

#elif defined(__SDCC_PIC18F24J10)
#  include <pic18f24j10.h>

#elif defined(__SDCC_PIC18F24J50)
#  include <pic18f24j50.h>

#elif defined(__SDCC_PIC18F24K20)
#  include <pic18f24k20.h>

#elif defined(__SDCC_PIC18F24K22)
#  include <pic18f24k22.h>

#elif defined(__SDCC_PIC18F2510)
#  include <pic18f2510.h>

#elif defined(__SDCC_PIC18F2515)
#  include <pic18f2515.h>

#elif defined(__SDCC_PIC18F2520)
#  include <pic18f2520.h>

#elif defined(__SDCC_PIC18F2523)
#  include <pic18f2523.h>

#elif defined(__SDCC_PIC18F2525)
#  include <pic18f2525.h>

#elif defined(__SDCC_PIC18F2550)
#  include <pic18f2550.h>

#elif defined(__SDCC_PIC18F2580)
#  include <pic18f2580.h>

#elif defined(__SDCC_PIC18F2585)
#  include <pic18f2585.h>

#elif defined(__SDCC_PIC18F25J10)
#  include <pic18f25j10.h>

#elif defined(__SDCC_PIC18F25J50)
#  include <pic18f25j50.h>

#elif defined(__SDCC_PIC18F25K20)
#  include <pic18f25k20.h>

#elif defined(__SDCC_PIC18F25K22)
#  include <pic18f25k22.h>

#elif defined(__SDCC_PIC18F2610)
#  include <pic18f2610.h>

#elif defined(__SDCC_PIC18F2620)
#  include <pic18f2620.h>

#elif defined(__SDCC_PIC18F2680)
#  include <pic18f2680.h>

#elif defined(__SDCC_PIC18F2682)
#  include <pic18f2682.h>

#elif defined(__SDCC_PIC18F2685)
#  include <pic18f2685.h>

#elif defined(__SDCC_PIC18F26J50)
#  include <pic18f26j50.h>

#elif defined(__SDCC_PIC18F26K20)
#  include <pic18f26k20.h>

#elif defined(__SDCC_PIC18F26K22)
#  include <pic18f26k22.h>

#elif defined(__SDCC_PIC18F4220)
#  include <pic18f4220.h>

#elif defined(__SDCC_PIC18F4221)
#  include <pic18f4221.h>

#elif defined(__SDCC_PIC18F4320)
#  include <pic18f4320.h>

#elif defined(__SDCC_PIC18F4321)
#  include <pic18f4321.h>

#elif defined(__SDCC_PIC18F4331)
#  include <pic18f4331.h>

#elif defined(__SDCC_PIC18F43K20)
#  include <pic18f43k20.h>

#elif defined(__SDCC_PIC18F43K22)
#  include <pic18f43k22.h>

#elif defined(__SDCC_PIC18F4410)
#  include <pic18f4410.h>

#elif defined(__SDCC_PIC18F4420)
#  include <pic18f4420.h>

#elif defined(__SDCC_PIC18F4423)
#  include <pic18f4423.h>

#elif defined(__SDCC_PIC18F4431)
#  include <pic18f4431.h>

#elif defined(__SDCC_PIC18F4450)
#  include <pic18f4450.h>

#elif defined(__SDCC_PIC18F4455)
#  include <pic18f4455.h>

#elif defined(__SDCC_PIC18F4480)
#  include <pic18f4480.h>

#elif defined(__SDCC_PIC18F44J10)
#  include <pic18f44j10.h>

#elif defined(__SDCC_PIC18F44J50)
#  include <pic18f44j50.h>

#elif defined(__SDCC_PIC18F44K20)
#  include <pic18f44k20.h>

#elif defined(__SDCC_PIC18F44K22)
#  include <pic18f44k22.h>

#elif defined(__SDCC_PIC18F4510)
#  include <pic18f4510.h>

#elif defined(__SDCC_PIC18F4515)
#  include <pic18f4515.h>

#elif defined(__SDCC_PIC18F4520)
#  include <pic18f4520.h>

#elif defined(__SDCC_PIC18F4523)
#  include <pic18f4523.h>

#elif defined(__SDCC_PIC18F4525)
#  include <pic18f4525.h>

#elif defined(__SDCC_PIC18F4550)
#  include <pic18f4550.h>

#elif defined(__SDCC_PIC18F4580)
#  include <pic18f4580.h>

#elif defined(__SDCC_PIC18F4585)
#  include <pic18f4585.h>

#elif defined(__SDCC_PIC18F45J10)
#  include <pic18f45j10.h>

#elif defined(__SDCC_PIC18F45J50)
#  include <pic18f45j50.h>

#elif defined(__SDCC_PIC18F45K20)
#  include <pic18f45k20.h>

#elif defined(__SDCC_PIC18F45K22)
#  include <pic18f45k22.h>

#elif defined(__SDCC_PIC18F4610)
#  include <pic18f4610.h>

#elif defined(__SDCC_PIC18F4620)
#  include <pic18f4620.h>

#elif defined(__SDCC_PIC18F4680)
#  include <pic18f4680.h>

#elif defined(__SDCC_PIC18F4682)
#  include <pic18f4682.h>

#elif defined(__SDCC_PIC18F4685)
#  include <pic18f4685.h>

#elif defined(__SDCC_PIC18F46J50)
#  include <pic18f46j50.h>

#elif defined(__SDCC_PIC18F46K20)
#  include <pic18f46k20.h>

#elif defined(__SDCC_PIC18F46K22)
#  include <pic18f46k22.h>

#elif defined(__SDCC_PIC18F6520)
#  include <pic18f6520.h>

#elif defined(__SDCC_PIC18F6527)
#  include <pic18f6527.h>

#elif defined(__SDCC_PIC18F6585)
#  include <pic18f6585.h>

#elif defined(__SDCC_PIC18F65J50)
#  include <pic18f65j50.h>

#elif defined(__SDCC_PIC18F6620)
#  include <pic18f6620.h>

#elif defined(__SDCC_PIC18F6622)
#  include <pic18f6622.h>

#elif defined(__SDCC_PIC18F6627)
#  include <pic18f6627.h>

#elif defined(__SDCC_PIC18F6680)
#  include <pic18f6680.h>

#elif defined(__SDCC_PIC18F66J50)
#  include <pic18f66j50.h>

#elif defined(__SDCC_PIC18F66J55)
#  include <pic18f66j55.h>

#elif defined(__SDCC_PIC18F66J60)
#  include <pic18f66j60.h>

#elif defined(__SDCC_PIC18F66J65)
#  include <pic18f66j65.h>

#elif defined(__SDCC_PIC18F6720)
#  include <pic18f6720.h>

#elif defined(__SDCC_PIC18F6722)
#  include <pic18f6722.h>

#elif defined(__SDCC_PIC18F67J50)
#  include <pic18f67j50.h>

#elif defined(__SDCC_PIC18F67J60)
#  include <pic18f67j60.h>

#elif defined(__SDCC_PIC18F8520)
#  include <pic18f8520.h>

#elif defined(__SDCC_PIC18F8527)
#  include <pic18f8527.h>

#elif defined(__SDCC_PIC18F8585)
#  include <pic18f8585.h>

#elif defined(__SDCC_PIC18F85J50)
#  include <pic18f85j50.h>

#elif defined(__SDCC_PIC18F8620)
#  include <pic18f8620.h>

#elif defined(__SDCC_PIC18F8622)
#  include <pic18f8622.h>

#elif defined(__SDCC_PIC18F8627)
#  include <pic18f8627.h>

#elif defined(__SDCC_PIC18F8680)
#  include <pic18f8680.h>

#elif defined(__SDCC_PIC18F86J50)
#  include <pic18f86j50.h>

#elif defined(__SDCC_PIC18F86J55)
#  include <pic18f86j55.h>

#elif defined(__SDCC_PIC18F86J60)
#  include <pic18f86j60.h>

#elif defined(__SDCC_PIC18F86J65)
#  include <pic18f86j65.h>

#elif defined(__SDCC_PIC18F8720)
#  include <pic18f8720.h>

#elif defined(__SDCC_PIC18F8722)
#  include <pic18f8722.h>

#elif defined(__SDCC_PIC18F87J50)
#  include <pic18f87j50.h>

#elif defined(__SDCC_PIC18F87J60)
#  include <pic18f87j60.h>

#elif defined(__SDCC_PIC18F96J60)
#  include <pic18f96j60.h>

#elif defined(__SDCC_PIC18F96J65)
#  include <pic18f96j65.h>

#elif defined(__SDCC_PIC18F97J60)
#  include <pic18f97j60.h>


#else
#  error Unsupported processor

#endif

#ifndef __CONCAT
#define __CONCAT(a,b)   __CONCAT2(a,b)
#endif  // !__CONCAT

#ifndef __CONCAT2
#define __CONCAT2(a,b)   a##b
#endif  // !__CONCAT2

#define __CONFIG(ADDR,VAL)  \
    static const __code unsigned char __at(ADDR) __CONCAT(_conf,__LINE__) = (VAL)

#define Nop()           do { __asm nop __endasm; } while(0)
#define ClrWdt()        do { __asm clrwdt __endasm; } while(0)
#define Sleep()         do { __asm sleep __endasm; } while(0)
#define Reset()         do { __asm reset __endasm; } while(0)

#endif /* __PIC18FREGS_H__ */
