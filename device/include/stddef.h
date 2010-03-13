/*-------------------------------------------------------------------------
   stddef.h - ANSI functions forward declarations

   Copyright (C) 2004, Maarten Brock / sourceforge.brock@dse.nl

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License.  This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef __SDC51_STDDEF_H
#define __SDC51_STDDEF_H 1

#ifndef NULL
  #define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

#if defined(SDCC_z80) || defined(SDCC_gbz80)
  #define offsetof(s,m)   (size_t)&(((s *)0)->m)
#else
  /* temporary hack to fix bug 1518273 */
  #define offsetof(s,m)   (size_t)&(((s __code *)0)->m)
#endif

#endif
