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

#ifndef SDCC_mcs51
#define _generic
#define reentrant
#endif

#define memmove memcpy

#ifdef SDCC_STACK_AUTO
#ifdef SDCC_mcs51
#warning Make sure you recompiled _str*.c and _mem*.c library functions as 'reentrant'
#endif

extern char _generic *strcpy (char _generic *, char _generic *) reentrant ;
extern char _generic *strncpy(char _generic *, char _generic *,int ) reentrant ;
extern char _generic *strcat (char _generic *, char _generic *) reentrant ;
extern char _generic *strncat(char _generic *, char _generic *,int ) reentrant ;
extern int            strcmp (char _generic *, char _generic *) reentrant ;
extern int            strncmp(char _generic *, char _generic *,int ) reentrant ;
extern char _generic *strchr (char _generic *, char           ) reentrant ;
extern char _generic *strrchr(char _generic *, char           ) reentrant ;
extern int            strspn (char _generic *, char _generic *) reentrant ;
extern int            strcspn(char _generic *, char _generic *) reentrant ;
extern char _generic *strpbrk(char _generic *, char _generic *) reentrant ;
extern char _generic *strstr (char _generic *, char _generic *) reentrant ;
extern int            strlen (char _generic *                 ) reentrant ;
extern char _generic *strtok (char _generic *, char _generic *) reentrant ;
extern void _generic *memcpy (void _generic *, void _generic *, int ) reentrant ;
extern int            memcmp (void _generic *, void _generic *, int ) reentrant ;
extern void _generic *memset (void _generic *, unsigned char  , int ) reentrant ;
 
#else

extern char _generic *strcpy (char _generic *, char _generic *)  ;
extern char _generic *strncpy(char _generic *, char _generic *,int )  ;
extern char _generic *strcat (char _generic *, char _generic *)  ;
extern char _generic *strncat(char _generic *, char _generic *,int )  ;
extern int            strcmp (char _generic *, char _generic *)  ;
extern int            strncmp(char _generic *, char _generic *,int )  ;
extern char _generic *strchr (char _generic *, char           )  ;
extern char _generic *strrchr(char _generic *, char           )  ;
extern int            strspn (char _generic *, char _generic *)  ;
extern int            strcspn(char _generic *, char _generic *)  ;
extern char _generic *strpbrk(char _generic *, char _generic *)  ;
extern char _generic *strstr (char _generic *, char _generic *)  ;
extern int            strlen (char _generic *  )  ;
extern char _generic *strtok (char _generic *, char _generic *)  ;
extern void _generic *memcpy (void _generic *, void _generic *, int )  ;
extern int            memcmp (void _generic *, void _generic *, int )  ;
extern void _generic *memset (void _generic *, unsigned char  , int )  ;

#if SDCC_ds390
extern void _xdata *memcpyx(void _xdata *, void _xdata *, int);
#endif

#endif

#endif
