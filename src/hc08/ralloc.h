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

enum
  {
    A_IDX,
    H_IDX,
    X_IDX,
    HX_IDX,
    XA_IDX,
    CND_IDX,
    SP_IDX
  };


#define REG_PTR 0x01
#define REG_GPR 0x02
#define REG_CND 0x04

/* definition for the registers */
typedef struct regs
  {
    short type;			/* can have value 
				   REG_GPR, REG_PTR or REG_CND */
    short rIdx;			/* index into register table */
    char *name;			/* name */
    short mask;			/* bitmask for pair allocation */
    struct asmop *aop;		/* last operand */
    int aopofs;			/* last operand offset */
    unsigned isFree:1;		/* is currently unassigned  */
  }
regs;
extern regs regshc08[];
extern regs *hc08_reg_a;
extern regs *hc08_reg_x;
extern regs *hc08_reg_h;
extern regs *hc08_reg_hx;
extern regs *hc08_reg_xa;
extern regs *hc08_reg_sp;

regs *hc08_regWithIdx (int);
void hc08_useReg (regs * reg);
void hc08_freeReg (regs * reg);
void hc08_dirtyReg (regs * reg, bool freereg);
bitVect *hc08_rUmaskForOp (operand * op);

#endif
