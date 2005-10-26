/*-------------------------------------------------------------------------
   string.h - ANSI functions forward declarations    
  
   Modified for pic16 port by Vangelis Rokas, 2004, vrokas@otenet.gr
   
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

#ifndef __STRING_H	/* { */
#define __STRING_H 1

#define _STRING_SPEC	__data

#pragma library c

#ifndef NULL
# define NULL (void *)0
#endif

#ifndef _SIZE_T_DEFINED
# define _SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

char *strcat (char *, char *);
char *strchr (char *, char);
int   strcmp (char *, char *);
char *strcpy (char *, char *);
int   strcspn(char *, char *);
int   strlen (char *);
char *strlwr (char *);
char *strncat(char *, char *, size_t );
int   strncmp(char *, char *, size_t );
char *strncpy(char *, char *, size_t );
char *strpbrk(char *, char *);
char *strrchr(char *, char);
int   strspn (char *, char *);
char *strstr (char *, char *);
char *strtok (char *, char *);
char *strupr (char *);

void *memccpy(void *, void *, char, size_t);
void *memchr(void *, char, size_t);
int   memcmp (void *, void *, size_t);
void *memcpy (void *, void *, size_t);
void *memmove (void *, void *, size_t);
void *memrchr(void *, char, size_t);
void *memset (_STRING_SPEC void *, unsigned char, size_t );

__code void *memchrpgm(__code void *, char, size_t);
__data void *memchrram(__data void *, char, size_t);
__data void *memcpypgm2ram(__data void *, __code void *, size_t);
__data void *memcpyram2ram(__data void *, __data void *, size_t);


#endif	/* } */
