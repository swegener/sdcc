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

#ifndef __STDIO_H
#define __STDIO_H 1

/* link the C library */
#pragma library c

#include <stdarg.h>

#include <sdcc-lib.h>

#ifndef NULL
  #define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif


/* stream descriptor definition */
typedef char *FILE;


/* USART and MSSP module stream descriptors */

/* since FILE is declared as a generic pointer,
 * the upper byte is used to dereference the pointer
 * information. For the stream descriptors we
 * use the 5th bit and the lower nubble bits.
 * Descriptors are denoted by an 1 in bit 5,
 * further dereference is made for:
 * <stream>     <3:0> bits
 *     USART	 0 (0x0)
 *     MSSP      1 (0x1)
 *     USER     15 (0xf)
 *
 * There is a special value for GPSIM specific (see below)
 * which is:
 *     GPSIM    14 (0xe)
 *
 *
 * if further stream descriptors need to be added then more
 * bits of the upper byte can be used
 */


#define USART_DEREF	0x0
#define MSSP_DEREF	0x1
#define USER_DEREF	0xf

#define STREAM_USART	((FILE *)(0x00200000UL))
#define STREAM_MSSP	((FILE *)(0x00210000UL))
#define STREAM_USER	((FILE *)(0x002f0000UL))


/* this is a custom dereference which points to a custom
 * port of GPSIM simulator. This port redirects characters
 * to /tmp/gpsim.debug.1 file (used for debugging purposes)
 * NOTICE: This feature is not part of the official gpsim
 * distribution. Contact vrokas AT users.sourceforge.net
 * for more info */
#define GPSIM_DEREF	0xe
#define STREAM_GPSIM	((FILE *)(0x002e0000UL))

extern FILE * stdin;
extern FILE * stdout;

//typedef void (*pfn_outputchar)(char c, void* p) _REENTRANT;
//extern int _print_format (pfn_outputchar pfn, void* pvoid, const char *format, va_list ap);

/* printf_small() supports float print */
void printf_small(char *, ...);

/* printf_tiny() does not support float print */
void printf_tiny(char *, ...);  // __reentrant;

extern unsigned int printf (char *,...);
extern unsigned int sprintf (char *, char *, ...);
extern unsigned int fprintf(FILE *, char *, ...);

extern unsigned int vsprintf (char *, char *, va_list);
extern unsigned int vprintf (char *, va_list);
extern unsigned int vfprintf(FILE *, char *, va_list);

extern int puts(char *);

extern void __stream_putchar(FILE *, unsigned char);

#define PUTCHAR(arg)	void putchar(unsigned char arg) __wparam
extern PUTCHAR(c);

extern void __stream_usart_putchar(unsigned char c) __wparam __naked;
extern void __stream_mssp_putchar(unsigned char c) __wparam __naked;
extern void __stream_gpsim_putchar(unsigned char c) __wparam __naked;

extern char *gets(char *);
extern char getchar(void);

#endif /* __STDIO_H */
