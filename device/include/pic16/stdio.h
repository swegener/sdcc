/*-------------------------------------------------------------------------
  stdio.h - ANSI functions forward declarations

   Ported to PIC16 port by Vangelis Rokas, 2004 (vrokas@otenet.gr)
   
             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)

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

/*
** $Id$
*/

#ifndef __PIC16_STDIO_H
#define __PIC16_STDIO_H 1

#include <stdarg.h>

#include <sdcc-lib.h>

#ifndef NULL
  #define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

typedef void (*pfn_outputchar)(char c, void* p) _REENTRANT;

extern int _print_format (pfn_outputchar pfn, void* pvoid, const char *format, va_list ap);

/*-----------------------------------------------------------------------*/

extern void printf_small (char *,...);	_REENTRANT;
extern int printf (const char *,...);
extern int vprintf (const char *, va_list);
extern int sprintf (char *, const char *, ...);
extern int vsprintf (char *, const char *, va_list);
extern int puts(const char *);
extern char *gets(char *);
extern char getchar(void);
extern void putchar(char);


#endif /* __PIC16_STDIO_H */
