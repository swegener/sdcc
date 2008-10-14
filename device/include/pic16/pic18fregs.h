
/*
 * pic18fregs.h - 18F Family Device Library Header
 *
 * This file is part of the GNU PIC Library.
 *
 * January, 2004
 * The GNU PIC Library is maintained by,
 * 	Vangelis Rokas <vrokas@otenet.gr>
 *
 * $Id$
 *
 */

#ifndef __PIC18FREGS_H__
#define __PIC18FREGS_H__

#if defined(pic18f242)
#  include <pic18f242.h>

#elif defined(pic18f248)
#  include <pic18f248.h>

#elif defined(pic18f252)
#  include <pic18f252.h>

#elif defined(pic18f258)
#  include <pic18f258.h>

#elif defined(pic18f442)
#  include <pic18f442.h>

#elif defined(pic18f448)
#  include <pic18f448.h>

#elif defined(pic18f452)
#  include <pic18f452.h>

#elif defined(pic18f458)
#  include <pic18f458.h>

#elif defined(pic18f1220)
#  include <pic18f1220.h>

#elif defined(pic18f1320)
#  include <pic18f1320.h>

#elif defined(pic18f2220)
#  include <pic18f2220.h>

#elif defined(pic18f2221)
#  include <pic18f2221.h>

#elif defined(pic18f2320)
#  include <pic18f2320.h>

#elif defined(pic18f2321)
#  include <pic18f2321.h>

#elif defined(pic18f2331)
#  include <pic18f2331.h>

#elif defined(pic18f2420)
#  include <pic18f2420.h>

#elif defined(pic18f2423)
#  include <pic18f2423.h>

#elif defined(pic18f2431)
#  include <pic18f2431.h>

#elif defined(pic18f2450)
#  include <pic18f2450.h>

#elif defined(pic18f2455)
#  include <pic18f2455.h>

#elif defined(pic18f2480)
#  include <pic18f2480.h>

#elif defined(pic18f24j10)
#  include <pic18f24j10.h>

#elif defined(pic18f2520)
#  include <pic18f2520.h>

#elif defined(pic18f2523)
#  include <pic18f2523.h>

#elif defined(pic18f2525)
#  include <pic18f2525.h>

#elif defined(pic18f2550)
#  include <pic18f2550.h>

#elif defined(pic18f2580)
#  include <pic18f2580.h>

#elif defined(pic18f2585)
#  include <pic18f2585.h>

#elif defined(pic18f25j10)
#  include <pic18f25j10.h>

#elif defined(pic18f2620)
#  include <pic18f2620.h>

#elif defined(pic18f2680)
#  include <pic18f2680.h>

#elif defined(pic18f2682)
#  include <pic18f2682.h>

#elif defined(pic18f2685)
#  include <pic18f2685.h>

#elif defined(pic18f4220)
#  include <pic18f4220.h>

#elif defined(pic18f4221)
#  include <pic18f4221.h>

#elif defined(pic18f4320)
#  include <pic18f4320.h>

#elif defined(pic18f4321)
#  include <pic18f4321.h>

#elif defined(pic18f4331)
#  include <pic18f4331.h>

#elif defined(pic18f4420)
#  include <pic18f4420.h>

#elif defined(pic18f4423)
#  include <pic18f4423.h>

#elif defined(pic18f4431)
#  include <pic18f4431.h>

#elif defined(pic18f4450)
#  include <pic18f4450.h>

#elif defined(pic18f4455)
#  include <pic18f4455.h>

#elif defined(pic18f4480)
#  include <pic18f4480.h>

#elif defined(pic18f44j10)
#  include <pic18f44j10.h>

#elif defined(pic18f4520)
#  include <pic18f4520.h>

#elif defined(pic18f4523)
#  include <pic18f4523.h>

#elif defined(pic18f4525)
#  include <pic18f4525.h>

#elif defined(pic18f4550)
#  include <pic18f4550.h>

#elif defined(pic18f4580)
#  include <pic18f4580.h>

#elif defined(pic18f4585)
#  include <pic18f4585.h>

#elif defined(pic18f45j10)
#  include <pic18f45j10.h>

#elif defined(pic18f4620)
#  include <pic18f4620.h>

#elif defined(pic18f4680)
#  include <pic18f4680.h>

#elif defined(pic18f4682)
#  include <pic18f4682.h>

#elif defined(pic18f4685)
#  include <pic18f4685.h>

#elif defined(pic18f6520)
#  include <pic18f6520.h>

#elif defined(pic18f6585)
#  include <pic18f6585.h>

#elif defined(pic18f6620)
#  include <pic18f6620.h>

#elif defined(pic18f6680)
#  include <pic18f6680.h>

#elif defined(pic18f66j60)
#  include <pic18f66j60.h>

#elif defined(pic18f66j65)
#  include <pic18f66j65.h>

#elif defined(pic18f6720)
#  include <pic18f6720.h>

#elif defined(pic18f67j60)
#  include <pic18f67j60.h>

#elif defined(pic18f8520)
#  include <pic18f8520.h>

#elif defined(pic18f8585)
#  include <pic18f8585.h>

#elif defined(pic18f8620)
#  include <pic18f8620.h>

#elif defined(pic18f8680)
#  include <pic18f8680.h>

#elif defined(pic18f86j60)
#  include <pic18f86j60.h>

#elif defined(pic18f86j65)
#  include <pic18f86j65.h>

#elif defined(pic18f8720)
#  include <pic18f8720.h>

#elif defined(pic18f87j60)
#  include <pic18f87j60.h>

#elif defined(pic18f96j60)
#  include <pic18f96j60.h>

#elif defined(pic18f96j65)
#  include <pic18f96j65.h>

#elif defined(pic18f97j60)
#  include <pic18f97j60.h>


#else
#  error Unsupported processor

#endif


#define Nop()           do { __asm nop __endasm; } while(0)
#define ClrWdt()        do { __asm clrwdt __endasm; } while(0)
#define Sleep()         do { __asm sleep __endasm; } while(0)
#define Reset()         do { __asm reset __endasm; } while(0)


#endif /* __PIC18FREGS_H__ */

