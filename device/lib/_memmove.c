/*-------------------------------------------------------------------------
  _memmove.c - part of string library functions

             Adapted By -  Erik Petrich  . epetrich@users.sourceforge.net
             from _memcpy.c which was originally
             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.
   
   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/
#include "string.h" 
#include <sdcc-lib.h>

#ifndef _SDCC_PORT_PROVIDES_MEMMOVE
#define _SDCC_PORT_PROVIDES_MEMMOVE 0
#endif

#if !_SDCC_PORT_PROVIDES_MEMMOVE

void * memmove (
	void * dst,
	void * src,
	size_t acount
	) 
{
#if _SDCC_Z80_STYLE_LIB_OPT

#pragma NOINDUCTION

	char * d;
	char * s;
	/* PENDING: Divide first to get around sign problems */
	int count = -(acount >> 2);

	if (((int)src < (int)dst) && ((((int)src)+acount) > (int)dst)) {
		/*
		 * copy from higher addresses to lower addresses
		 */
		d = ((char *)dst)+acount-1;
		s = ((char *)src)+acount-1;
	        while (count) {
			*d-- = *s--;
			*d-- = *s--;
			*d-- = *s--;
			*d-- = *s--;
                	count++;
	        }

	        if (acount & 2) {
			*d-- = *s--;
			*d-- = *s--;
	        }
	        if (acount & 1) {
			*d-- = *s--;
	        }
	}
	else {
		/*
		 * copy from lower addresses to higher addresses
		 */
		d = dst;
		s = src;
	        while (count) {
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
                	count++;
	        }

	        if (acount & 2) {
			*d++ = *s++; 
			*d++ = *s++;
	        }
	        if (acount & 1) {
			*d++ = *s++;
	        }
	}
	return dst;
#else
	void * ret = dst;
	char * d;
	char * s;

	if (((int)src < (int)dst) && ((((int)src)+acount) > (int)dst)) {
		/*
		 * copy from higher addresses to lower addresses
		 */
		d = ((char *)dst)+acount-1;
		s = ((char *)src)+acount-1;
		while (acount--) {
			*d-- = *s--;
		}
	}
	else {
		/*
		 * copy from lower addresses to higher addresses
		 */
		d = dst;
		s = src;
		while (acount--) {
			*d++ = *s++;
		}
	}

	return(ret);
#endif
}
#endif
