/*-------------------------------------------------------------------------

  SDCCralloc.h - header file register allocation

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
#include "SDCCicode.h"
#include "SDCCBBlock.h"
#ifndef SDCCRALLOC_H
#define SDCCRALLOC_H 1

#define ubyte unsigned char
#define byte ubyte
#define sbyte signed char
#define uword unsigned char
#define word uword
#define sword signed char
#define udword unsigned int
#define dword udword
#define sdword signed int

#define REG_PTR 0x01 // pointer register
#define REG_GPR 0x02 // general purpose register
#define REG_CND 0x04 // condition (bit) register
#define REG_SCR 0x40 // scratch register
#define REG_STK 0x80 // stack pointer register

typedef struct regs {
  ubyte rIdx; // a unique # for this one
  ubyte size; // size of register (0,1,2,4)
  ubyte type; // pointer, general purpose, condition (bit)
  char *name;
  udword regMask;
  uword offset;
  bool isFree;
  symbol *sym;
} regs;

#if 0
/* definition for the registers */
typedef struct regs
  {
    short type;			/* can have value REG_CND, REG_8BITS, 
				   REG_16BITS or REG_32BITS */
    short rIdx;			/* index into register table */
    short otype;
    char *name;			/* name */
    char *dname;		/* name when direct access needed */
    char *base;			/* base address */
    short offset;		/* offset from the base */
    unsigned isFree:1;		/* is currently unassigned  */
  }
regs;
#endif

extern regs regsXA51[];
extern udword xa51RegsInUse;

regs *xa51_regWithIdx (int);

bitVect *xa51_rUmaskForOp (operand * op);

#endif
