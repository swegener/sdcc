/*-------------------------------------------------------------------------
  stdlib.h - ANSI functions forward declarations

   Ported to PIC16 port by Vangelis Rokas, 2004 (vrokas@otenet.gr)
   
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

#ifndef __STDLIB_H__
#define __STDLIB_H__ 1

#pragma library c

#include <stdint.h>


#ifndef NULL
# define NULL (void *)0
#endif

#define RAND_MAX        0x7fffffff

/* initialize random seed */
void srand(unsigned long seed);

/* return a random number between 0 and RAND_MAX */
long rand(void);

/* reentrant version of rand() */
long rand_r(unsigned long *ctx);


/* returns the CRC16 checksum of the data buffer, takes as
 * last argument an old value of crc16 checksum */
uint16_t crc16(uint8_t *, uint32_t, uint16_t);


/* convert a ASCII string to float */
float atof (char *);

/* convert a ASCII string to integer */
int atoi (char *);

/* convert a ASCII string to long */
long atol (char *);

/* convert an unsigned/signed integer to ASCII string */
void uitoa(unsigned int, __data char *, unsigned char);
void itoa(int, __data char*, unsigned char);

/* convert an unsigned/signed long integer to ASCII string */
void ultoa(unsigned long, __data unsigned char *, unsigned char);
void ltoa(long, __data unsigned char*, unsigned char);

/* helper functions: convert a float to ASCII string */
extern char x_ftoa(float, __data char *, unsigned char, unsigned char);

/* George M. Gallant's version of ftoa() */
extern void g_ftoa(__data char *, float, char);


#endif	/* __STDLIB_H__ */
