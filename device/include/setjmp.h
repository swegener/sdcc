/*-------------------------------------------------------------------------
  setjmp.h - header file for setjmp & longjmp ANSI routines

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

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

#ifndef SDCC_SETJMP_H
#define SDCC_SETJMP_H

#define SP_SIZE		1

#ifdef SDCC_STACK_AUTO
#define BP_SIZE		SP_SIZE
#else
#define BP_SIZE		0
#endif

#ifdef SDCC_USE_XSTACK
#define SPX_SIZE	1
#else
#define SPX_SIZE	0
#endif

#define BPX_SIZE	SPX_SIZE

#define RET_SIZE	2

typedef unsigned char jmp_buf[RET_SIZE + SP_SIZE + BP_SIZE + SPX_SIZE + BPX_SIZE];

int setjmp (jmp_buf);
int longjmp(jmp_buf, int);

#undef RET_SIZE
#undef SP_SIZE
#undef BP_SIZE
#undef SPX_SIZE
#undef BPX_SIZE

#endif
