
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

#elif defined(pic18f2220)
#  include <pic18f2220.h>

#elif defined(pic18f2550)
#  include <pic18f2550.h>

#elif defined(pic18f4331)
#  include <pic18f4331.h>

#elif defined(pic18f4455)
#  include <pic18f4455.h>

#elif defined(pic18f4520)
#  include <pic18f4520.h>

#elif defined(pic18f6520)
#  include <pic18f6520.h>

#elif defined(pic18f6620)
#  include <pic18f6620.h>

#elif defined(pic18f6680)
#  include <pic18f6680.h>

#elif defined(pic18f6720)
#  include <pic18f6720.h>

#elif defined(pic18f8520)
#  include <pic18f8520.h>

#elif defined(pic18f8620)
#  include <pic18f8620.h>

#elif defined(pic18f8680)
#  include <pic18f8680.h>

#elif defined(pic18f8720)
#  include <pic18f8720.h>

#else
#  error Unsupported processor

#endif


#define Nop()           { _asm nop _endasm; }
#define ClrWdt()        { _asm clrwdt _endasm; }
#define Sleep()         { _asm sleep _endasm; }
#define Reset()         { _asm reset _endasm; }


#endif /* __PIC18FREGS_H__ */
