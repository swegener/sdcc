/*-------------------------------------------------------------------------
   string.h - ANSI functions forward declarations    
  
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


#ifndef __SDC51_STRING_H
#define __SDC51_STRING_H 1

#ifndef NULL
# define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
# define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

extern char *strcpy (char *, const char *)  ;
extern char *strncpy(char *, const char *, size_t )  ;
extern char *strcat (char *, const char *)  ;
extern char *strncat(char *, const char *, size_t )  ;
extern int   strcmp (const char *, const char *)  ;
extern int   strncmp(const char *, const char *, size_t )  ;
extern char *strchr (const char *, char  )  ;
extern char *strrchr(const char *, char  )  ;
extern int   strspn (const char *, const char *)  ;
extern int   strcspn(const char *, const char *)  ;
extern char *strpbrk(const char *, const char *)  ;
extern char *strstr (const char *, const char *)  ;
extern int   strlen (const char *  )  ;
extern char *strtok (char *, const char *)  ;
extern void *memcpy (void *, const void *, size_t )  ;
extern int   memcmp (const void *, const void *, size_t )  ;
extern void *memset (void *, unsigned char  , size_t )  ;
extern void *memmove (void *, const void *, size_t )  ;

#ifdef SDCC_ds390
extern void __xdata * memcpyx(void __xdata *, void __xdata *, int) __naked;
#endif

#ifdef SDCC_z80
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#endif

#endif
