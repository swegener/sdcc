/*-------------------------------------------------------------------------
  gen.h - header file for code generation for mos6502

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
-------------------------------------------------------------------------*/

#ifndef SDCCGENM6502_H
#define SDCCGENM6502_H

// keep this in sync with _temp.s in the library
#define NUM_TEMP_REGS 8

enum debug_messages {
  ALWAYS=0x01,
  VASM=0x02,
  TRACEGEN=0x04,
  DEEPTRACE=0x08,
  COST=0x10,
  REGALLOC=0x20,
  REGOPS=0x40,
  TRACE_AOP=0x80,
  TRACE_STACK=0x100,
  VVDBG=0x80000000,
  DEBUG_ALL=0x7fffffff
};

//#define DBG_MSG (REGALLOC)
#define DBG_MSG (REGALLOC|TRACEGEN/*|COST*/)
//#define DBG_MSG (REGALLOC|TRACEGEN|REGOPS/*|COST*/)
//#define DBG_MSG (REGALLOC|TRACEGEN|TRACE_STACK/*|COST*/)
//#define DBG_MSG (DEBUG_ALL/*|VVDBG*/)
//#define DBG_MSG ((DEBUG_ALL|VVDBG)&~COST)

#define DEBUG_UNIMPLEMENTED

#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3

#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define AOP_OP(aop) aop->op

#define IS_AOP_A(x) ((x)->regmask == M6502MASK_A)
#define IS_AOP_X(x) ((x)->regmask == M6502MASK_X)
#define IS_AOP_Y(x) ((x)->regmask == M6502MASK_Y)
#define IS_AOP_XA(x) ((x)->regmask == M6502MASK_XA)
#define IS_AOP_YX(x) ((x)->regmask == M6502MASK_YX)

typedef enum
  {
  AOP_INVALID,
  AOP_LIT = 1,   /* operand is a literal value */
  AOP_REG,       /* is in registers */
  AOP_DIR,       /* operand using direct addressing mode */
  AOP_STK,       /* should be pushed on stack this
                    can happen only for the result */
  AOP_IMMD,      /* immediate value for eg. remateriazable */
  AOP_STR,       /* array of strings */
  AOP_CRY,       /* carry contains the value of this */
  AOP_EXT,       /* operand using extended addressing mode */
  AOP_SOF,       /* operand at an offset on the stack */
  AOP_DUMMY,     /* Read undefined, discard writes */
  AOP_IDX        /* operand using indexed addressing mode */
}
AOP_TYPE;

/* asmop: A homogenised type for all the different
   spaces an operand can be in */
typedef struct asmop
  {
    AOP_TYPE type;		
    short coff;			/* current offset */
    short size;			/* total size */    	
    short regmask;              /* register mask if AOP_REG */
    operand *op;		/* originating operand */
    unsigned freed:1;		/* already freed    */
    unsigned stacked:1;		/* partial results stored on stack */
    struct asmop *stk_aop[4];	/* asmops for the results on the stack */
    union
      {
	value *aop_lit;		/* if literal */
	reg_info *aop_reg[4];	/* array of registers */
	char *aop_dir;		/* if direct  */
        char *aop_immd;         /* if immediate */
	int aop_stk;		/* stack offset when AOP_STK */
    } aopu;
  }
asmop;

struct attr_t
{
  bool isLiteral;
  unsigned char literalValue;
  struct asmop *aop;		/* last operand */
  int aopofs;			/* last operand offset */
};

void genm6502Code (iCode *);
void m6502_emitDebuggerSymbol (const char *);

extern unsigned fReturnSizeM6502;

extern bool m6502_assignment_optimal;

#endif

