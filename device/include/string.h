/*-------------------------------------------------------------------------
   string.h - ISO header for string library functions

   Written By -  Philipp Klaus Krause . pkk@spth.de (2009)

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
   
   As a special exception, you may use this file as part of a free software
   library without restriction.  Specifically, if other files instantiate
   templates or use macros or inline functions from this file, or you compile
   this file and link it with other files to produce an executable, this
   file does not by itself cause the resulting executable to be covered by
   the GNU General Public License.  This exception does not however
   invalidate any other reasons why the executable file might be covered by
   the GNU General Public License.
-------------------------------------------------------------------------*/


#ifndef __SDC51_STRING_H
#define __SDC51_STRING_H 1

#ifndef NULL
# define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
# define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

/* The function prototypes are ordered as in the ISO C99 standard. */

/* Todo: fix the "restrict" stuff for C99 compliance. */

/* Copying functions: */
extern void *memcpy (void * /*restrict */ dest, const void * /*restrict*/ src, size_t n);
extern void *memmove (void *dest, const void *src, size_t n);
extern char *strcpy (char * /*restrit*/ dest, const char * /*restrict*/ src);
extern char *strncpy(char * /*restrict*/ dest, const char * /*restrict*/ src, size_t n);

/* Concatenation functions: */
extern char *strcat (char * /*restrict*/ dest, const char * /*restrict*/ src);
extern char *strncat(char * /*restrict*/ dest, const char * /*restrict*/ src, size_t n);

/* Comparison functions: */
extern int memcmp (const void *s1, const void *s2, size_t n);
extern int strcmp (const char *s1, const char *s2);
/*inline int strcoll(const char *s1, const char *s2) {return strcmp(s1, s2);}*/
extern int strncmp(const char *s1, const char *s2, size_t n);
/*inline size_t strxfrm(char *dest, const char *src, size_t n) {strncpy(dest, src, n); return strlen(src);}*/

/* Search functions: */
extern void *memchr (const void *s, int c, size_t n);
extern char *strchr (const char *s, char c); /* c should be int according to standard. */
extern size_t strcspn(const char *s, const char *reject);
extern char *strpbrk(const char *s, const char *accept);
extern char *strrchr(const char *s, char c); /* c should be int according to standard. */
extern size_t strspn (const char *s, const char *accept);
extern char *strstr (const char *haystack, const char *needle);
extern char *strtok (char * /* restrict*/ str, const char * /*restrict*/ delim);

/* Miscanelleous functions: */
extern void *memset (void *s, unsigned char c, size_t n); /* c should be int according to standard. */
/* extern char *strerror(int errnum); */
extern size_t strlen (const char *s);

#ifdef SDCC_ds390
extern void __xdata * memcpyx(void __xdata *, void __xdata *, int) __naked;
#endif

#ifdef SDCC_z80
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#endif

#endif

