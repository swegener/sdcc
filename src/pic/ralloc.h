/*-------------------------------------------------------------------------

  SDCCralloc.h - header file register allocation

                Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
		PIC port   - T. Scott Dattalo scott@dattalo.com (2000)

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

#include "pcoderegs.h"


enum
  {
    R2_IDX = 0, R3_IDX, R4_IDX,
    R5_IDX, R6_IDX, R7_IDX,
    R0_IDX, R1_IDX, X8_IDX,
    X9_IDX, X10_IDX, X11_IDX,
    X12_IDX, CND_IDX
  };

enum {
 REG_PTR=1,
 REG_GPR,
 REG_CND,
 REG_SFR,
 REG_STK,
 REG_TMP
};
//#define REG_PTR 0x01
//#define REG_GPR 0x02
//#define REG_CND 0x04
//#define REG_SFR 0x08
//#define REG_STK 0x10  /* Use a register as a psuedo stack */
//#define REG_TMP 0x20  

/* definition for the registers */
typedef struct regs
  {
    short type;			/* can have value 
				 * REG_GPR, REG_PTR or REG_CND 
				 * This like the "meta-type" */
    short pc_type;              /* pcode type */
    short rIdx;			/* index into register table */
    //    short otype;        
    char *name;			/* name */

    unsigned isFree:1;		/* is currently unassigned  */
    unsigned wasUsed:1;		/* becomes true if register has been used */
    unsigned isFixed:1;         /* True if address can't change */
    unsigned isMapped:1;        /* The Register's address has been mapped to physical RAM */
    unsigned isBitField:1;      /* True if reg is type bit OR is holder for several bits */
    unsigned isEmitted:1;       /* True if the reg has been written to a .asm file */
    unsigned isPublic:1;        /* True if the reg is not static and can be modified in another module (ie a another c or asm file) */
    unsigned isExtern:1;        /* True if the reg is in another module */
    unsigned address;           /* reg's address if isFixed | isMapped is true */
    unsigned size;              /* 0 for byte, 1 for int, 4 for long */
    unsigned alias;             /* Alias mask if register appears in multiple banks */
    struct regs *reg_alias;     /* If more than one register share the same address 
				 * then they'll point to each other. (primarily for bits)*/
    pCodeRegLives reglives; /* live range mapping */
  }
regs;
extern regs regspic14[];
extern int pic14_nRegs;
extern int Gstack_base_addr;

/*
  As registers are created, they're added to a set (based on the
  register type). Here are the sets of registers that are supported
  in the PIC port:
*/
extern set *dynAllocRegs;
extern set *dynStackRegs;
extern set *dynProcessorRegs;
extern set *dynDirectRegs;
extern set *dynDirectBitRegs;
extern set *dynInternalRegs;


regs *pic14_regWithIdx (int);
regs *dirregWithName (char *name );
void  pic14_freeAllRegs ();
void  pic14_deallocateAllRegs ();
regs *pic14_findFreeReg(short type);
regs *pic14_allocWithIdx (int idx);
regs *typeRegWithIdx (int idx, int type, int fixed);
regs *regFindWithName (const char *name);

regs *allocDirReg (operand *op );
regs *allocRegByName (char *name, int size );
regs *allocNewDirReg (sym_link *symlnk,const char *name);

/* Define register address that are constant across PIC family */
#define IDX_INDF    0
#define IDX_TMR0    1
#define IDX_PCL     2
#define IDX_STATUS  3
#define IDX_FSR     4
#define IDX_PORTA   5
#define IDX_PORTB   6
#define IDX_PCLATH  0x0a
#define IDX_INTCON  0x0b

#define IDX_KZ      0x7fff   /* Known zero - actually just a general purpose reg. */
#define IDX_WSAVE   0x7ffe
#define IDX_SSAVE   0x7ffd
#define IDX_PSAVE   0x7ffc

#endif
