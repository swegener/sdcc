/*-------------------------------------------------------------------------
  stdio.h - ANSI functions forward declarations
 
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

#ifndef __SDC51_STDIO_H
#define __SDC51_STDIO_H 1

#include <stdarg.h>

#if defined(SDCC_MODEL_LARGE) || defined(SDCC_MODEL_FLAT24)
#ifndef TINIBIOS_H
#include <tinibios.h>
#endif TINIBIOS_H
#define XSPEC xdata
#else
#define XSPEC
#endif

extern printf_small (char *,...) reentrant;
extern printf (XSPEC const char *,...) reentrant;
extern int vprintf (const char *, va_list);
extern int sprintf (const char *, XSPEC const char *, ...) reentrant;
extern int vsprintf (const char *, const char *, va_list);
extern int puts(const char *);
extern char *gets(char *);
extern char getchar(void);
extern void putchar(char);
#endif






