/*-------------------------------------------------------------------------
  ctype.h - ANSI functions forward declarations
 
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

#ifndef __SDC51_CTYPE_H
#define __SDC51_CTYPE_H 1

#define isalnum(x)   (isalpha(x) || isdigit(x))
#define isalpha(x)   (isupper(x) || islower(c))

#ifdef SDCC_STACK_AUTO
#warning Make sure you recompile _is*.c files as 'reentrant'

extern char  iscntrl   (unsigned char ) reentrant ;
extern char  isdigit   (unsigned char ) reentrant ;
extern char  isgraph   (unsigned char ) reentrant ;
extern char  islower   (unsigned char ) reentrant ;
extern char  isupper   (unsigned char ) reentrant ;
extern char  isprint   (unsigned char ) reentrant ;
extern char  ispunct   (unsigned char ) reentrant ;
extern char  isspace   (unsigned char ) reentrant ;
extern char  isxdigit  (unsigned char ) reentrant ;

#else

extern char  iscntrl   (unsigned char )  ;
extern char  isdigit   (unsigned char )  ;
extern char  isgraph   (unsigned char )  ;
extern char  islower   (unsigned char )  ;
extern char  isupper   (unsigned char )  ;
extern char  isprint   (unsigned char )  ;
extern char  ispunct   (unsigned char )  ;
extern char  isspace   (unsigned char )  ;
extern char  isxdigit  (unsigned char )  ;

#endif
#endif






