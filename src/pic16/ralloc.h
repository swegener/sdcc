/*-------------------------------------------------------------------------

  ralloc.h - header file register allocation

	Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
	PIC port   - T. Scott Dattalo scott@dattalo.com (2000)
	PIC16 port   - Martin Dubuc m.dubuc@rogers.com (2002)

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

extern unsigned int stackPos;

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
//    unsigned isMapped:1;        /* The Register's address has been mapped to physical RAM */
    unsigned isBitField:1;      /* True if reg is type bit OR is holder for several bits */
    unsigned isEmitted:1;       /* True if the reg has been written to a .asm file */
    unsigned accessBank:1;	/* True if the reg is explicit placed in access bank */
    unsigned isLocal:1;		/* True if the reg is allocated in function's local frame */
    unsigned address;           /* reg's address if isFixed | isMapped is true */
    unsigned size;              /* 0 for byte, 1 for int, 4 for long */
    unsigned alias;             /* Alias mask if register appears in multiple banks */
    struct regs *reg_alias;     /* If more than one register share the same address 
				 * then they'll point to each other. (primarily for bits)*/
    operand *regop;		/* reference to the operand used to create the register */
    pCodeRegLives reglives; /* live range mapping */
  }
regs;
extern regs regspic16[];
extern int pic16_nRegs;
extern int pic16_Gstack_base_addr;

/*
  As registers are created, they're added to a set (based on the
  register type). Here are the sets of registers that are supported
  in the PIC port:
*/
extern set *pic16_dynAllocRegs;
extern set *pic16_dynStackRegs;
extern set *pic16_dynProcessorRegs;
extern set *pic16_dynDirectRegs;
extern set *pic16_dynDirectBitRegs;
extern set *pic16_dynInternalRegs;

extern set *pic16_builtin_functions;

extern set *pic16_rel_udata;
extern set *pic16_fix_udata;
extern set *pic16_equ_data;
extern set *pic16_int_regs;

regs *pic16_regWithIdx (int);
regs *pic16_typeRegWithIdx(int, int, int);
regs *pic16_dirregWithName (char *name );
void  pic16_freeAllRegs ();
void  pic16_deallocateAllRegs ();
regs *pic16_findFreeReg(short type);
regs *pic16_allocWithIdx (int idx);

regs *pic16_allocDirReg (operand *op );
regs *pic16_allocRegByName (char *name, int size, operand *op);

regs* newReg(short type, short pc_type, int rIdx, char *name, int size, int alias, operand *refop);

/* Define register address that are constant across PIC16 family */
#define IDX_TMR0    0xfd6
#define IDX_PCL     0xff9
#define IDX_STATUS  0xfd8
#define IDX_PCLATH  0xffa
#define IDX_PCLATU  0xffb /* patch 14 */
#define IDX_INTCON  0xff2
#define IDX_WREG    0xfe8
#define IDX_BSR     0xfe0

#define IDX_TOSL    0xffd /* patch 14 */
#define IDX_TOSH    0xffe /* patch 14 */
#define IDX_TOSU    0xfff /* patch 14 */

#define IDX_TBLPTRL 0xff6 /* patch 15 */
#define IDX_TBLPTRH 0xff7 /* patch 15 */
#define IDX_TBLPTRU 0xff8 /* patch 15 */
#define IDX_TABLAT  0xff5 /* patch 15 */

#define IDX_FSR0    0xfe9
#define IDX_FSR0L   0xfe9
#define IDX_FSR0H   0xfea
#define IDX_FSR1L   0xfe1
#define IDX_FSR1H   0xfe2
#define IDX_FSR2L   0xfd9
#define IDX_FSR2H   0xfda

#define IDX_INDF0       0xfef
#define IDX_POSTINC0    0xfee
#define IDX_POSTDEC0    0xfed
#define IDX_PREINC0     0xfec
#define IDX_PLUSW0      0xfeb

#define IDX_INDF1       0xfe7
#define IDX_POSTINC1    0xfe6
#define IDX_POSTDEC1    0xfe5
#define IDX_PREINC1     0xfe4
#define IDX_PLUSW1      0xfe3

#define IDX_INDF2       0xfdf
#define IDX_POSTINC2    0xfde
#define IDX_POSTDEC2    0xfdd
#define IDX_PREINC2     0xfdc
#define IDX_PLUSW2      0xfdb

#define IDX_PRODL       0xff3
#define IDX_PRODH       0xff4

/* EEPROM registers */
#define IDX_EECON1	0xfa6
#define IDX_EECON2	0xfa7
#define IDX_EEDATA	0xfa8
#define IDX_EEADR	0xfa9

#define IDX_KZ      0x7fff   /* Known zero - actually just a general purpose reg. */
#define IDX_WSAVE   0x7ffe
#define IDX_SSAVE   0x7ffd

#endif
