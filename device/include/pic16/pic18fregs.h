
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

#else
#  error Unsupported processor

#endif

#endif /* __PIC18FREGS_H__ */
