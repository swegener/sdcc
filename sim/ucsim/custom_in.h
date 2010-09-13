/*
 * Simulator of microcontrollers (custom.h)
 *
 * This file contains manual additions to #include into ddconfig.h
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 *
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/

#define DD_TRUE     1
#define DD_FALSE    0
#define NIL         0

/*
 * find out the endianess of host machine
 */
#if defined __APPLE__ && (defined __BIG_ENDIAN__ || defined _BIG_ENDIAN)
/* 1) trust the compiler
 * in order to be able to make Mac OS X unified binaries */
# define WORDS_BIGENDIAN 1
#elif defined __APPLE__ && (defined __LITTLE_ENDIAN__ || defined _LITTLE_ENDIAN)
/* do nothing */
#elif defined __sun && defined HAVE_SYS_ISA_DEFS_H
/* Solaris defines endianness in <sys/isa_defs.h> */
# include <sys/isa_defs.h>
# ifdef _BIG_ENDIAN
#   define WORDS_BIGENDIAN 1
# endif
#else
# ifdef HAVE_ENDIAN_H
#   include <endian.h>
# elif defined HAVE_SYS_ENDIAN_H
#   include <sys/endian.h>
# elif defined HAVE_MACHINE_ENDIAN_H
#   include <machine/endian.h>
# endif
# if (defined BYTE_ORDER && defined BIG_ENDIAN && defined LITTLE_ENDIAN && BYTE_ORDER && BIG_ENDIAN && LITTLE_ENDIAN)
/* 2) trust the header files */
#   if BYTE_ORDER == BIG_ENDIAN
#     define WORDS_BIGENDIAN 1
#   endif
# else
/* 3) trust the configure; this actually doesn't work for unified Mac OS X binaries :-( */
#   if (defined BUILD_WORDS_BIGENDIAN && BUILD_WORDS_BIGENDIAN)
#     define WORDS_BIGENDIAN  1
#   endif
/* 4) assume that host is a little endian machine */
# endif
#endif

/* End of custom.h */
