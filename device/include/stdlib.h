/*-------------------------------------------------------------------------
  stdlib.h - ANSI functions forward declarations

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

#ifndef __SDC51_STDLIB_H
#define __SDC51_STDLIB_H 1

#ifndef NULL
# define NULL (void *)0
#endif

#include <malloc.h>

int abs(int j);
long int labs(long int j);

extern float atof (char *);
extern int atoi (char *);
extern long atol (char *);

extern void _uitoa(unsigned int, char*, unsigned char);
extern void _itoa(unsigned int, char*, unsigned char);

extern void _ultoa(unsigned long, char*, unsigned char);
extern void _ltoa(unsigned long, char*, unsigned char);

#define RAND_MAX 32767

int rand(void);
void srand(unsigned int seed);
#endif
