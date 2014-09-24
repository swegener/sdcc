/*-------------------------------------------------------------------------
   ctype.h

   Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de 2013

   (c) 2013 Goethe-Universität Frankfurt

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
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
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef __SDCC_CTYPE_H
#define __SDCC_CTYPE_H 1

extern int isalnum (int c);
extern int isalpha (int c);
extern int iscntrl (int c);
extern int isgraph (int c);
extern int isprint (int c);
extern int ispunct (int c);
extern int isspace (int c);
extern int isalnum (int c);
extern int isalnum (int c);
extern int isxdigit (int c);

extern int tolower (int c);
extern int toupper (int c);

/* Provide inline versions for the most used functions for efficiency */
#if __STDC_VERSION__ >= 199901L

extern int isblank (int c);

inline int isdigit (int c)
{
  /* using local variable ret seems to give most optimal code */
  unsigned char ret = 0;
  /* we could additionally check (unsigned char)(c >> 8) == 0 */
  if ((unsigned char)c >= '0' && (unsigned char)c <= '9')
    ret = 1;
  return ret;
}

inline int islower (int c)
{
  /* using local variable ret seems to give most optimal code */
  unsigned char ret = 0;
  /* we could additionally check (unsigned char)(c >> 8) == 0 */
  if ((unsigned char)c >= 'a' && (unsigned char)c <= 'z')
    ret = 1;
  return ret;
}

inline int isupper (int c)
{
  /* using local variable ret seems to give most optimal code */
  unsigned char ret = 0;
  /* we could additionally check (unsigned char)(c >> 8) == 0 */
  if ((unsigned char)c >= 'A' && (unsigned char)c <= 'Z')
    ret = 1;
  return ret;
}

#else

extern int isdigit (int c);
extern int islower (int c);
extern int isupper (int c);

#endif

#endif
