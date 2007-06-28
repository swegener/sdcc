/*
 * Simulator of microcontrollers (ddconfig.h)
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

#ifndef DDCONFIG_HEADER
#define DDCONFIG_HEADER

#define DD_TRUE  1
#define DD_FALSE 0
#define NIL     0

#undef STDC_HEADERS
#undef HAVE_GETOPT_H
#undef HAVE_UNISTD_H
#undef HAVE_DIRENT_H
#undef HAVE_SYS_NDIR_H
#undef HAVE_SYS_DIR_H
#undef HAVE_NDIR_H
#undef HAVE_SYS_SOCKET_H
#undef HAVE_WINSOCK2_H
#undef HAVE_DLFCN_H
#undef HAVE_CURSES_H
#undef HAVE_TERMIOS_H

#undef SOCKET_AVAIL
#undef SOCKLEN_T
#undef UCSOCKET_T
#undef HEADER_SOCKET

#undef FD_NEED_TIME_H
#undef FD_NEED_TYPES_H
#undef FD_NEED_SELECT_H
#undef FD_NEED_WINSOCK2_H
#undef HEADER_FD
#undef FD_HEADER_OK

#undef SCANF_A
#undef GNU_GETCWD

#undef HAVE_STRLEN
#undef HAVE_STRCPY
#undef HAVE_STRCAT
#undef HAVE_STRSTR
#undef HAVE_STRCMP
#undef HAVE_STRERROR
#undef HAVE_STRTOK
#undef HAVE_STRDUP
#undef HAVE_STRCHR

#undef HAVE_MEMCPY

#undef HAVE_VPRINTF
#undef HAVE_DOPRNT
#undef HAVE_VSNPRINTF
#undef HAVE__VSNPRINTF
#undef HAVE_VASPRINTF

#undef HAVE_GETLINE
#undef HAVE_GETDELIM
#undef HAVE_FGETS

#undef HAVE_YYLEX
#undef HAVE_FLEX
#undef YYTEXT_POINTER

#undef RETSIGTYPE
#undef SIZEOF_CHAR
#undef SIZEOF_SHORT
#undef SIZEOF_INT
#undef SIZEOF_LONG
#undef SIZEOF_LONG_LONG
#undef TYPE_BYTE
#undef TYPE_WORD
#undef TYPE_DWORD
#define TYPE_UBYTE unsigned TYPE_BYTE
#define TYPE_UWORD unsigned TYPE_WORD
#define TYPE_UDWORD unsigned TYPE_DWORD
#undef _M_
#undef _A_

/*
 * find out the endianess of host machine
 * in order to be able to make Mac OS X unified binaries
 */
#if __BIG_ENDIAN__ || _BIG_ENDIAN
/* 1) trust the compiler */
# define WORDS_BIGENDIAN 1
#elif __LITTLE_ENDIAN__
/* do nothing */
#elif (defined BYTE_ORDER && defined BIG_ENDIAN && defined LITTLE_ENDIAN && BYTE_ORDER && BIG_ENDIAN && LITTLE_ENDIAN)
/* 2) trust the header files */
# if BYTE_ORDER == BIG_ENDIAN 
#   define WORDS_BIGENDIAN 1
# endif
#else 
/* 3) trust the configure; this actually doesn't work for unified Mac OS X binaries :-( */
# undef BUILD_WORDS_BIGENDIAN
# if (defined BUILD_WORDS_BIGENDIAN && BUILD_WORDS_BIGENDIAN)
#   define WORDS_BIGENDIAN  1
# endif
/* 4) assume that host is a little endian machine */
#endif

#undef VERSIONSTR
#undef VERSIONHI
#undef VERSIONLO
#undef VERSIONP

#undef ACCEPT_SOCKLEN_T

#undef STATISTIC

#endif

/* End of ddconfig.h */
