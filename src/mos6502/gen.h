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

// keep this in sync with __sdcc_regs.s in the library
#define NUM_TEMP_REGS 6

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

#define IS_AOP_WITH_A(x) (((x)->regmask & M6502MASK_A) != 0)
#define IS_AOP_WITH_X(x) (((x)->regmask & M6502MASK_X) != 0)
#define IS_AOP_WITH_Y(x) (((x)->regmask & M6502MASK_Y) != 0)

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

struct m6502_state_t
{
  int stackOfs;
  int funcHasBasePtr;
  int stackPushes;
  int tsxStackPushes;
  //  int baseStackPushes;
  set *sendSet;
  int tempOfs;
  unsigned carryValid:1;
  unsigned carry:1;
  int lastflag;
  struct attr_t tempAttr[NUM_TEMP_REGS];
  struct attr_t DPTRAttr[2];
};

// globals
extern asmop tsxaop;
extern unsigned fReturnSizeM6502;
extern bool m6502_assignment_optimal;
extern struct m6502_state_t _S;

extern const char *IMMDFMT; // = "#0x%02x";
extern const char *TEMPFMT; // = "*(REGTEMP+%d)";
extern const char *TEMPFMT_IND; // = "[REGTEMP+%d]";
//extern char *TEMPFMT_IY; // = "[REGTEMP+%d],y";

extern const char *IDXFMT_X; // = "0x%x,x";
//extern char *TEMPFMT_IX; // = "[(REGTEMP+%d),x]";
extern const char *DPTRFMT; // = "*(DPTR+%d)";
extern const char *DPTRFMT_IY; // = "[DPTR],y";
extern const char *INDFMT_IY; // = "[%s],y";

extern const int STACK_TOP; // = 0x100;

void genm6502Code (iCode *);
void m6502_emitDebuggerSymbol (const char *);

// utility functions
const char * regInfoStr();
void printIC(iCode *ic);

void emitComment (unsigned int level, const char *fmt, ...);

void aopOp (operand *op, const iCode * ic);
void freeAsmop (operand * op, asmop * aaop);


symbol * safeNewiTempLabel(const char * a);
void safeEmitLabel(symbol * a);
int safeLabelNum(symbol * a);

bool emitCmp (reg_info *reg, unsigned char v);
void emitBranch (char *branchop, symbol * tlbl);
void emitTSX(void);
void emit6502op (const char *inst, const char *fmt, ...);
void emitSetCarry(int c);

void genIfxJump (iCode * ic, char *jval);

void loadRegFromConst (reg_info * reg, int c);
void loadRegFromAop (reg_info * reg, asmop * aop, int loffset);
void storeConstToAop (int c, asmop * aop, int loffset);
void transferAopAop (asmop * srcaop, int srcofs, asmop * dstaop, int dstofs);
void storeRegToAop (reg_info *reg, asmop * aop, int loffset);
void transferRegReg (reg_info *sreg, reg_info *dreg, bool freesrc);
void accopWithAop (char *accop, asmop *aop, int loffset);
void rmwWithAop (char *rmwop, asmop * aop, int loffset);
void rmwWithReg (char *rmwop, reg_info * reg);
void storeRegToFullAop (reg_info *reg, asmop *aop, bool isSigned);
void genCopy (operand * result, operand * source);

void updateCFA (void);

bool smallAdjustReg (reg_info *reg, int n);
bool aopCanIncDec (asmop * aop);
bool sameRegs (asmop * aop1, asmop * aop2);
unsigned long long litmask (int size);
int isLiteralBit (unsigned long long lit);
reg_info* getDeadByteReg();
reg_info* getFreeByteReg();
reg_info* getFreeIdxReg();
bool canBitOp (const operand* aop);


// stack
void pushReg (reg_info * reg, bool freereg);
bool pushRegIfUsed (reg_info *reg);
bool pushRegIfSurv (reg_info *reg);
void pullReg (reg_info * reg);
void pullOrFreeReg (reg_info * reg, bool needpull);
void pullNull (int n);
void adjustStack (int n);

// regtemp
//void storeRegTempi (reg_info * reg, bool freereg, bool force);
void storeRegTemp (reg_info * reg, bool freereg);
void storeRegTempAlways (reg_info * reg, bool freereg);
bool storeRegTempIfUsed (reg_info *reg);
bool storeRegTempIfSurv (reg_info *reg);
void loadRegTemp (reg_info * reg);
void loadOrFreeRegTemp (reg_info * reg, bool needload);
void loadRegTempAt (reg_info * reg, int offset);
void loadRegTempNoFlags (reg_info * reg, bool needpull);
void emitRegTempOp(char *op, int offset);
int getLastTempOfs();
void dirtyRegTemp (int temp_reg_idx);
void signExtendA();

// gen functions
void genOr (iCode * ic, iCode * ifx);
void genXor (iCode * ic, iCode * ifx);
void genAnd (iCode * ic, iCode * ifx);
void genPlus (iCode * ic);
void genMinus (iCode * ic);

void XAccRsh (int shCount, bool sign);
void XAccLsh (reg_info *msb_reg, int shCount);
void AccRsh (int shCount, bool sign);
void AccLsh (int shCount);
void genRightShift (iCode * ic);
void genLeftShift (iCode * ic);
bool aopCanShift (asmop * aop);
void addSign (operand * result, int offset, int sign);


#endif

