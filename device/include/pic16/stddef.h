/*-------------------------------------------------------------------------
   stddef.h - ANSI functions forward declarations

   Ported to PIC16 port by Raphael Neider, 2005 (rneider@web.de)

   Written By -  Maarten Brock / sourceforge.brock@dse.nl (June 2004)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
-------------------------------------------------------------------------*/

/*
** $Id$
*/ 

#ifndef __PIC16_STDDEF_H
#define __PIC16_STDDEF_H 1

#ifndef NULL
  #define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

#define offsetof(s,m)   (size_t)&(((s *)0)->m)

#endif  /* __PIC16_STDDEF_H */
