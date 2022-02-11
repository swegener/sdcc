/*-------------------------------------------------------------------------
  gen.c - source file for code generation for the MOS6502

  Copyright (C) 1998, Sandeep Dutta . sandeep.dutta@usa.net
  Copyright (C) 1999, Jean-Louis VERN.jlvern@writeme.com
  Bug Fixes - Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  Hacked for the HC08:
  Copyright (C) 2003, Erik Petrich
  Hacked for the MOS6502:
  Copyright (C) 2020, Steven Hugg  hugg@fasterlight.com
  Copyright (C) 2021-2022, Gabriele Gorla

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

#include "m6502.h"
#include "ralloc.h"
#include "gen.h"
#include "dbuf_string.h"

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

#define DBG_MSG (REGALLOC)
//#define DBG_MSG (REGALLOC|TRACEGEN|COST)
//#define DBG_MSG (DEBUG_ALL/*|VVDBG*/)
//#define DBG_MSG ((DEBUG_ALL|VVDBG)&~COST)

extern int allocInfo;
unsigned fReturnSizeM6502 = 4;   /* shared with ralloc.c */

static bool regalloc_dry_run;
static unsigned int regalloc_dry_run_cost_bytes;
static float regalloc_dry_run_cost_cycles;

struct attr_t {
  bool isLiteral;
  unsigned char literalValue;
};

static struct
{
  int stackOfs;
  int funcHasBasePtr;
  int stackPushes;
  int tsxStackPushes;
//  int baseStackPushes;
  set *sendSet;
  int tempOfs;
  struct attr_t tempAttr[NUM_TEMP_REGS];
}
_G;

extern int m6502_ptrRegReq;
extern int m6502_nRegs;
extern struct dbuf_s *codeOutBuf;

static void pushReg (reg_info * reg, bool freereg);
static bool pushRegIfUsed (reg_info *reg);
static bool pushRegIfSurv (reg_info *reg);
static void pullReg (reg_info * reg);
static void pullOrFreeReg (reg_info * reg, bool needpull);

static void storeRegTemp (reg_info * reg, bool freereg);
static bool storeRegTempIfUsed (reg_info *reg);
static bool storeRegTempIfSurv (reg_info *reg);
static void loadRegTemp (reg_info * reg);
static void loadOrFreeRegTemp (reg_info * reg, bool needload);


static void adjustStack (int n);
static void loadRegFromConst (reg_info * reg, int c);

static void transferAopAop (asmop * srcaop, int srcofs, asmop * dstaop, int dstofs);
static bool operandsEqu (operand * op1, operand * op2);
static asmop *newAsmop (short type);
static void aopAdrPrepare (asmop * aop, int loffset);
static const char *aopAdrStr (asmop * aop, int loffset, bool bit16);
static void aopAdrUnprepare (asmop * aop, int loffset);
static void updateiTempRegisterUse (operand * op);
static void rmwWithReg (char *rmwop, reg_info * reg);

static asmop *m6502_aop_pass[8];
static asmop tsxaop;

static char *IMMDFMT = "#0x%02x";
static char *TEMPFMT = "*(__TEMP+%d)";
static char *TEMPFMT_IND = "[__TEMP+%d]";
static char *TEMPFMT_IY = "[__TEMP+%d],y";
//static char *TEMPFMT_IX = "[(__TEMP+%d),x]";

//static char *BASEPTR = "*(__BASEPTR)";

const int STACK_TOP = 0x100;


#define RESULTONSTACK(x) \
                         (IC_RESULT(x) && IC_RESULT(x)->aop && \
                         IC_RESULT(x)->aop->type == AOP_STK )
#define IS_AOP_YX(x) ((x)->regmask == M6502MASK_YX)
#define IS_AOP_XA(x) ((x)->regmask == M6502MASK_XA)
#define IS_AOP_AX(x) ((x)->regmask == M6502MASK_AX)
#define IS_AOP_A(x) ((x)->regmask == M6502MASK_A)
#define IS_AOP_X(x) ((x)->regmask == M6502MASK_X)
#define IS_AOP_Y(x) ((x)->regmask == M6502MASK_Y)
#define IS_AOP_WITH_A(x) (((x)->regmask & M6502MASK_A) != 0)
#define IS_AOP_WITH_X(x) (((x)->regmask & M6502MASK_X) != 0)
#define IS_AOP_WITH_Y(x) (((x)->regmask & M6502MASK_Y) != 0)

#define IS_AOPOFS_A(x,o) (((x)->type == AOP_REG) && ((x)->aopu.aop_reg[o]->mask == M6502MASK_A))
#define IS_AOPOFS_X(x,o) (((x)->type == AOP_REG) && ((x)->aopu.aop_reg[o]->mask == M6502MASK_X))
#define IS_AOPOFS_Y(x,o) (((x)->type == AOP_REG) && ((x)->aopu.aop_reg[o]->mask == M6502MASK_Y))

#define safeLabelKey2num(a) ((regalloc_dry_run)?0:labelKey2num(a))
#define safeNewiTempLabel(a) ((regalloc_dry_run)?0:newiTempLabel(a))
#define safeEmitLabel(a) do { if(!regalloc_dry_run && a) emitLabel(a); } while(0)


#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3

#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define AOP_OP(aop) aop->op

/*------------------------------------------------------------------*/
/* m6502_opcodeCycles - returns the cycle count for the instruction */
/*------------------------------------------------------------------*/
int
m6502_opcodeCycles(const m6502opcodedata *opcode, const char *arg)
{
  int lastpos;

  lastpos=(*arg)?strlen(arg)-1:0;

  switch (opcode->type)
    {
      case M6502OP_INH: /* Inherent addressing mode */
      case M6502OP_IDD:
      case M6502OP_IDI:
      case M6502OP_BR:  /* Branch (1 byte signed offset) */
        if(opcode->name[0]=='r'&&opcode->name[1]=='t') // rti and rts
          return 6;
        return 2;
      case M6502OP_SPH:
        return 3;
      case M6502OP_SPL:
        return 4;
      case M6502OP_BBR:  /* Branch on bit (1 byte signed offset) */
        return 3;
      case M6502OP_RMW: /* read/modify/write instructions */
        if (!strcmp(arg, "a"))  /* accumulator */
          return 2;
     	if (arg[0] == '*') /* Zero page */
    	  return 5;
        if(lastpos>2 && arg[lastpos-1]!=',' && arg[lastpos]=='x' )
          return 7;
        return 6;  /* absolute */
        
      case M6502OP_REG: /* standard instruction */
      case M6502OP_LD:
        if (arg[0] == '#') /* Immediate addressing mode */
	      return 2;
        if (arg[0] == '*') { /* Zero page */
          if(arg[lastpos]=='x' || arg[lastpos]=='y')
            return 4;	
          return 3;
        }
        if (arg[0] == '[') { /* indirect */
          if(arg[lastpos]==']')
            return 6;
          return 5;
        }
	return 4; /* Otherwise, must be extended addressing mode */

      case M6502OP_ST:
        if (arg[0] == '*') { /* Zero page */
          if(arg[lastpos]=='x' || arg[lastpos]=='y')
            return 4;	
          return 3;
        }
        if (arg[0] == '[')  /* indirect */
            return 6;
        if(arg[lastpos]=='x' || arg[lastpos]=='y')
          return 5;	
        return 4;

      case M6502OP_JMP:
        if(opcode->name[1]=='s') return 6;
        if(arg[0]=='[') return 5;
        return 3;
      default:
         werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "unknown instruction type in m6502_opcodeSize");
        return 3;
    }
}

/*-----------------------------------------------------------------*/
/* emitComment - emits comments and debug messages                 */
/*-----------------------------------------------------------------*/
void
emitComment (unsigned int level, const char *fmt, ...)
{
  bool print=false;
  va_list ap;

  va_start (ap, fmt);

  if ( level&DBG_MSG ) {
     if(!(level&VVDBG)) print=true;
     else if(DBG_MSG&VVDBG) print=true; 
  }
  if (level==VASM && options.verboseAsm) print=true;
  if (level==ALWAYS) print=true;

  if(print) va_emitcode (";", fmt, ap);
  va_end (ap);
}

/*-----------------------------------------------------------------*/
/* regInfoStr - returns register state as a string                 */
/*-----------------------------------------------------------------*/
const char *
regInfoStr()
{
  static char outstr[30];
  char regstring[3][10];

  if(m6502_reg_a->isLitConst) snprintf(regstring[0],10,"A:%c%c:%02x",
                (m6502_reg_a->isFree)?'-':'U',
                (m6502_reg_a->isDead)?'-':'L',
                m6502_reg_a->litConst );
  else snprintf(regstring[0],10,"A:%c%c:??",
                (m6502_reg_a->isFree)?'-':'U',
                (m6502_reg_a->isDead)?'-':'L');

  if(m6502_reg_x->isLitConst) snprintf(regstring[1],10,"X:%c%c:%02x",
                (m6502_reg_x->isFree)?'-':'U',
                (m6502_reg_x->isDead)?'-':'L',
                m6502_reg_x->litConst );
  else snprintf(regstring[1],10,"X:%c%c:??",
                (m6502_reg_x->isFree)?'-':'U',
                (m6502_reg_x->isDead)?'-':'L');

  if(m6502_reg_y->isLitConst) snprintf(regstring[2],10,"Y:%c%c:%02x",
                (m6502_reg_y->isFree)?'-':'U',
                (m6502_reg_y->isDead)?'-':'L',
                m6502_reg_y->litConst );
  else snprintf(regstring[2],10,"Y:%c%c:??",
                (m6502_reg_y->isFree)?'-':'U',
                (m6502_reg_y->isDead)?'-':'L');

  snprintf(outstr, 30, "%s %s %s",
           regstring[0], regstring[1], regstring[2] );

  return outstr;
}

/*-----------------------------------------------------------------*/
/* emit6502op - emits opcopdes, updates cost and register state    */
/*-----------------------------------------------------------------*/
void
emit6502op (const char *inst, const char *fmt, ...)
{
  static char verboseFmt[512];
  va_list ap;
  int isize = 0;
  float cycles = 0;
  float probability=1;

  const m6502opcodedata *opcode = m6502_getOpcodeData(inst);

  if(fmt==0) werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "NULL fmt in emit6502op");


  if(opcode)  {

    isize = m6502_opcodeSize(opcode, fmt);
    cycles = m6502_opcodeCycles(opcode, fmt);
    reg_info *dst_reg = m6502_regWithIdx(opcode->dest);

    // mark the destination register dirty as necessary
    // load and transfer are handled in the instruction generator
    switch (opcode->type) {
      case M6502OP_REG: // target is accumulator
#if 1
        if(dst_reg && dst_reg->isLitConst && fmt[0]=='#' && isdigit(fmt[1]))
          {
            unsigned char b=strtol(&fmt[1],NULL,0);
            if(!strcmp(inst,"and")) {
              dst_reg->litConst&=b;
              break;
            }
            if(!strcmp(inst,"ora")) {
              dst_reg->litConst|=b;
              break;
            }
            if(!strcmp(inst,"eor")) {
              dst_reg->litConst^=b;
              break;
            }
          }
#endif
        m6502_dirtyReg (m6502_regWithIdx(opcode->dest));
        break;
      case M6502OP_RMW: // target is accumulator
        if (!strcmp(fmt, "a")) m6502_dirtyReg (m6502_reg_a);
        break;
      case M6502OP_SPL: // stack pull
        _G.stackPushes--;
        break;
      case M6502OP_SPH: // stack push
        _G.stackPushes++;
        break;
      case M6502OP_IDD: // index decrement
        if(dst_reg->isLitConst)
          dst_reg->litConst--;
        break;
      case M6502OP_IDI: // index increment
        if(dst_reg->isLitConst)
          dst_reg->litConst++;
        break;
      case M6502OP_BR: // add penalty for taken branches
        // this assumes:
        // 50% not taken (2 cycles)
        // 30% taken with target in the same page (3 cycles)
        // 20% taken with target in a different page (4 cycles)
        cycles += (0.3 * 1) + (0.2 * 2);
        break;
    }
  } else {
    emitComment(ALWAYS,"unkwnon opcode %s",inst);
    isize=10;
    //werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "NULL opcode in emit6502op");
  }

  regalloc_dry_run_cost_bytes += isize;
  regalloc_dry_run_cost_cycles += cycles * probability;

  va_start (ap, fmt);
  if (options.verboseAsm) {
    char dstring[3][64];
    dstring[0][0]=0;
    dstring[1][0]=0;
    dstring[2][0]=0;
    
  if (DBG_MSG&COST) {
      snprintf(dstring[0], 64, "sz=%d cl=%f p=%f",
               isize, cycles, probability);
   }

  if (DBG_MSG&REGALLOC) {
      snprintf(dstring[1], 64, "%s",
               regInfoStr() );
    }
  if (DBG_MSG&TRACE_STACK) {
      snprintf(dstring[2], 64, "stkpush=%d",
               _G.stackPushes );
    }

    snprintf(verboseFmt, 512, "%s \t; %s %s %s",
             fmt, dstring[0], dstring[1], dstring[2]);
    va_emitcode (inst, verboseFmt, ap);
  } 
  else
  {
    va_emitcode (inst, fmt, ap);
  }
  va_end (ap);
}

static void
emitSignedBranch (bool gt, bool eq, symbol * tlbl)
{
  symbol *tlbl2 = safeNewiTempLabel (NULL);
  symbol *tlbl3 = safeNewiTempLabel (NULL);
  if (eq && !gt)
    emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl->key));
  if (!eq && gt)
    emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl2->key));
  emit6502op (gt ? "bvs" : "bvc", "%05d$", safeLabelKey2num (tlbl2->key));
  emit6502op ("bpl", "%05d$", safeLabelKey2num (tlbl->key));
  emit6502op ("bmi", "%05d$", safeLabelKey2num (tlbl3->key));
  safeEmitLabel (tlbl2);
  emit6502op ("bmi", "%05d$", safeLabelKey2num (tlbl->key));
  safeEmitLabel (tlbl3);
}

static void
emitUnsignedBranch (bool gt, bool eq, symbol * tlbl)
{
  symbol *tlbl2 = safeNewiTempLabel (NULL);
  if (eq && !gt)
    emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl->key));
  if (!eq && gt)
    emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl2->key));
  emit6502op (gt ? "bcs" : "bcc", "%05d$", safeLabelKey2num (tlbl->key));
  safeEmitLabel (tlbl2);
}

static void
emitBranch (char *branchop, symbol * tlbl)
{
  if (!strcmp("bls", branchop)) {
    emitUnsignedBranch(0, 1, tlbl);
  } else if (!strcmp("bhi", branchop)) {
    emitUnsignedBranch(1, 0, tlbl);
  } else if (!strcmp("blt", branchop)) {
    emitSignedBranch(0, 0, tlbl);
  } else if (!strcmp("bgt", branchop)) {
    emitSignedBranch(1, 0, tlbl);
  } else if (!strcmp("ble", branchop)) {
    emitSignedBranch(0, 1, tlbl);
  } else if (!strcmp("bge", branchop)) {
    emitSignedBranch(1, 1, tlbl);
  } else {
    if (!IS_MOS65C02 && !strcmp(branchop, "bra"))
      branchop = "jmp";
    emit6502op (branchop, "%05d$", safeLabelKey2num (tlbl->key));
  }  
}

/*--------------------------------------------------------------------------*/
/* smallAdjustReg - Adjust register by n bytes if possible.                              */
/*--------------------------------------------------------------------------*/
static int
smallAdjustReg (reg_info *reg, int n)
{
  int regidx = reg->rIdx;

  emitComment (REGOPS, __func__ );

  if( (regidx!=X_IDX) && (regidx!=Y_IDX) )
    return n;  
  
  if (n <= -4 || n >= 4) {
    return n;
  }

  while (n < 0) {
    rmwWithReg ("dec", reg); /* 1 byte,  2 cycles */
    n++;
  }
  while (n > 0) {
    rmwWithReg ("inc", reg); /* 1 byte,  2 cycles */
    n--;
  }
  return 0;
}

/*-----------------------------------------------------------------*/
/* m6502_emitDebuggerSymbol - associate the current code location   */
/*   with a debugger symbol                                        */
/*-----------------------------------------------------------------*/
void
m6502_emitDebuggerSymbol (const char *debugSym)
{
  genLine.lineElement.isDebug = 1;
  emitcode ("", "%s ==.", debugSym);
  genLine.lineElement.isDebug = 0;
}

/*--------------------------------------------------------------------------*/
/* transferRegReg - Transfer from register(s) sreg to register(s) dreg. If  */
/*                  freesrc is true, sreg is marked free and available for  */
/*                  reuse. sreg and dreg must be of equal size              */
/*--------------------------------------------------------------------------*/
static void
transferRegReg (reg_info *sreg, reg_info *dreg, bool freesrc)
{
  int srcidx;
  int dstidx;
  char error = 0;

  /* Nothing to do if no destination. */
  if (!dreg)
    return;

  /* But it's definitely an error if there's no source. */
  if (!sreg)
    {
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "NULL sreg in transferRegReg");
      return;
    }

  emitComment (REGOPS, "  transferRegReg(%s,%s)", sreg->name, dreg->name);
  emitComment (REGOPS, "  %s %s", __func__, regInfoStr() );

  srcidx = sreg->rIdx;
  dstidx = dreg->rIdx;

  if (srcidx == dstidx)
    {
      m6502_useReg (dreg);
      return;
    }

// TODO: make sure regs are killed if clobbered
  switch (dstidx)
    {
    case A_IDX:
      switch (srcidx)
        {
        case Y_IDX:            /* Y to A */
          emit6502op ("tya", "");
          break;
        case X_IDX:            /* X to A */
          emit6502op ("txa", "");
          break;
        default:
          error = 1;
        }
      break;
    case Y_IDX:
      switch (srcidx)
        {
        case A_IDX:            /* A to Y */
          emit6502op ("tay", "");
          break;
        case X_IDX:            /* X to Y */
          if(m6502_reg_x->isLitConst) {
            emit6502op ("ldy", IMMDFMT, m6502_reg_x->litConst);
          } else if(m6502_reg_a->isFree) {
          emit6502op ("txa", "");
          emit6502op ("tay", "");
          } else {
            storeRegTemp (m6502_reg_x, false);
            loadRegTemp (m6502_reg_y);
          }
          break;
        default:
          error = 1;
        }
      break;
    case X_IDX:
      switch (srcidx)
        {
        case A_IDX:            /* A to X */
          emit6502op ("tax", "");
          break;
        case Y_IDX:            /* Y to X */
          if(m6502_reg_y->isLitConst) {
            emit6502op ("ldx", IMMDFMT, m6502_reg_y->litConst);
          } else if(m6502_reg_a->isFree) {
          emit6502op ("tya", "");
          emit6502op ("tax", "");
          } else {
            storeRegTemp (m6502_reg_y, false);
            loadRegTemp (m6502_reg_x);
          }
          break;
        default:
          error = 1;
        }
      break;
    case YX_IDX:
      switch (srcidx)
        {
        case XA_IDX:           /* XA to YX */
          storeRegTemp (m6502_reg_x, false);
          transferRegReg(m6502_reg_a, m6502_reg_x, true);
          loadRegTemp (m6502_reg_y);
          break;
        default:
          error = 1;
        }
      break;
    case XA_IDX:
      switch (srcidx)
        {
        case YX_IDX:           /* YX to XA */
          transferRegReg(m6502_reg_x, m6502_reg_a, true);
          storeRegTemp (m6502_reg_y, true);
          loadRegTemp (m6502_reg_x);
          break;
        default:
          error = 1;
        }
      break;
    default:
      error = 1;
    }

  wassertl (!error, "bad combo in transferRegReg");

  m6502_useReg (dreg);

  if(sreg->isLitConst) {
    dreg->isLitConst = sreg->isLitConst;
    dreg->litConst = sreg->litConst;
  } else {
    m6502_dirtyReg (dreg);
  }
//  dreg->aop = sreg->aop;
//  dreg->aopofs = sreg->aopofs;

  if (freesrc)
    m6502_freeReg (sreg);
}

/*--------------------------------------------------------------------------*/
/* updateCFA - update the debugger information to reflect the current       */
/*             connonical frame address relative to the stack pointer       */
/*--------------------------------------------------------------------------*/
static void
updateCFA (void)
{
  /* there is no frame unless there is a function */
  if (!currFunc)
    return;

  if (options.debug && !regalloc_dry_run)
    debugFile->writeFrameAddress (NULL, m6502_reg_sp, 1 + _G.stackOfs + _G.stackPushes);
}

/*--------------------------------------------------------------------------*/
/* storeRegTemp - Store register onto the __TEMP stack. If freereg is true, */
/*                reg is marked free and available for reuse.               */
/*--------------------------------------------------------------------------*/
static void
storeRegTemp (reg_info * reg, bool freereg)
{
  emitComment (REGOPS, "  storeRegTemp(%s) %s", reg ? reg->name : "-", freereg?"free":"");

  int regidx = reg->rIdx;

  if(regidx<=Y_IDX) {
    _G.tempAttr[_G.tempOfs].isLiteral=reg->isLitConst;
    _G.tempAttr[_G.tempOfs].literalValue=reg->litConst;
  }

  switch (regidx) {
    case A_IDX:
      emit6502op ("sta", TEMPFMT, _G.tempOfs++);
      break;
    case X_IDX:
      emit6502op ("stx", TEMPFMT, _G.tempOfs++);
      break;
    case Y_IDX:
      emit6502op ("sty", TEMPFMT, _G.tempOfs++);
      break;
    case YX_IDX:
      storeRegTemp (m6502_reg_x, freereg);
      storeRegTemp (m6502_reg_y, freereg);
      break;
    case XA_IDX:
      storeRegTemp (m6502_reg_a, freereg);
      storeRegTemp (m6502_reg_x, freereg);
      break;
    default:
      wassertl (0, "storeRegTemp()");
      break;
  }

  if (freereg)
    m6502_freeReg (reg);

  //emitcode("", "storeRegTemp overflow");
  wassertl (_G.tempOfs <= NUM_TEMP_REGS, "storeRegTemp(): overflow");
}

/*--------------------------------------------------------------------------*/
/* loadRegTemp - Pull register reg off the stack.                               */
/*--------------------------------------------------------------------------*/
static void
loadRegTemp (reg_info * reg)
{
  // pop off stack, unused
  if (reg == NULL) {
    _G.tempOfs--;
    return;
  }
    
  int regidx = reg->rIdx;
  if(regidx<=Y_IDX) {
    _G.tempOfs--;
    if(_G.tempAttr[_G.tempOfs].isLiteral) {
      loadRegFromConst(reg, _G.tempAttr[_G.tempOfs].literalValue);
      return;
    }
  }
   
  switch (regidx) {
    case A_IDX:
      emit6502op ("lda", TEMPFMT, _G.tempOfs);
      break;
    case X_IDX:
      emit6502op ("ldx", TEMPFMT, _G.tempOfs);
      break;
    case Y_IDX:
      emit6502op ("ldy", TEMPFMT, _G.tempOfs);
      break;
    case YX_IDX:
      loadRegTemp(m6502_reg_y);
      loadRegTemp(m6502_reg_x);
      break;
    case XA_IDX:
      loadRegTemp(m6502_reg_x);
      loadRegTemp(m6502_reg_a);
      break;
    default:
      wassertl (0, "loadRegTemp()");
      break;
  }

// FIXME: figure out if register pairs are literals

  m6502_useReg (reg);
  m6502_dirtyReg (reg);
}

// TODO: note that needpull has diff. semantics than loadRegTemp()
static void
loadRegTempNoFlags (reg_info * reg, bool needpull)
{
  if (needpull) {
    emit6502op("php", "");
    loadRegTemp (reg);
    emit6502op("plp", "");
  } else {
    m6502_freeReg (reg);
  }
}

static bool
storeRegTempIfSurv (reg_info *reg)
{
  if (!reg->isDead) {
    storeRegTemp (reg, true);
    return true;
  }
  return false;
}

static bool
storeRegTempIfUsed (reg_info *reg)
{
  if (!reg->isFree)
    {
      storeRegTemp (reg, true);
      return true;
    }
  return false;
}

static void
loadOrFreeRegTemp (reg_info * reg, bool needpull)
{
  if (needpull)
    loadRegTemp (reg);
  else
    m6502_freeReg (reg);
}

static void
dirtyRegTemp (int temp_reg_idx)
{
  _G.tempAttr[temp_reg_idx].isLiteral=false;
}

/*--------------------------------------------------------------------------*/
/* pushReg - Push register reg onto the stack. If freereg is true, reg is   */
/*           marked free and available for reuse.                           */
/*--------------------------------------------------------------------------*/
static void
pushReg (reg_info * reg, bool freereg)
{
  int regidx = reg->rIdx;

  emitComment (REGOPS, "; pushReg(%s) %s %s", reg->name, reg->isFree?"free":"", reg->isDead?"dead":"");

  switch (regidx)
    {
    case A_IDX:
      emit6502op ("pha", "");
      updateCFA ();
      break;
    case X_IDX:
      if (IS_MOS65C02) {
        emit6502op ("phx", "");
      } else {
        bool needloada = storeRegTempIfUsed (m6502_reg_a);
        transferRegReg (m6502_reg_x, m6502_reg_a, false);
        pushReg (m6502_reg_a, true);
        loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
      updateCFA ();
      break;
    case Y_IDX:
      if (IS_MOS65C02) {
        emit6502op ("phy", "");
      } else {
        bool needloada = storeRegTempIfUsed (m6502_reg_a);
        transferRegReg (m6502_reg_y, m6502_reg_a, true);
        pushReg (m6502_reg_a, true);
        loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
      updateCFA ();
      break;
    // little-endian order
    case YX_IDX:
      pushReg(m6502_reg_y, freereg);
      pushReg(m6502_reg_x, freereg);
      break;
    case XA_IDX:
      pushReg(m6502_reg_x, freereg);
      pushReg(m6502_reg_a, freereg);
      break;
    default:
      wassertl(0, "pushReg() error");
      break;
    }
  if (freereg)
    m6502_freeReg (reg);
}

/*--------------------------------------------------------------------------*/
/* pullReg - Pull register reg off the stack.                               */
/*--------------------------------------------------------------------------*/
static void
pullReg (reg_info * reg)
{
  int regidx = reg->rIdx;

  emitComment (REGOPS, __func__ );

  switch (regidx)
    {
    case A_IDX:
      emit6502op ("pla", "");
      updateCFA ();
      break;
    case X_IDX:
      if (IS_MOS65C02) {
        emit6502op ("plx", "");
      } else {
        // FIXME: saving A makes regression fail
//	bool needloada = storeRegTempIfUsed (m6502_reg_a);
        pullReg (m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_x, true);
//	loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
      updateCFA ();
      break;
    case Y_IDX:
      if (IS_MOS65C02) {
        emit6502op ("ply", "");
      } else {
        // FIXME: saving A makes regression fail
//	bool needloada = storeRegTempIfUsed (m6502_reg_a);
        pullReg (m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_y, true);
//	loadOrFreeRegTemp (m6502_reg_a, needloada);
      }
      updateCFA ();
      break;
    // little-endian order
    case YX_IDX:
      pullReg(m6502_reg_x);
      pullReg(m6502_reg_y);
      break;
    case XA_IDX:
      pullReg(m6502_reg_a);
      pullReg(m6502_reg_x);
      break;
    default:
      break;
    }
  m6502_useReg (reg);
  m6502_dirtyReg (reg);
}

/*--------------------------------------------------------------------------*/
/* pullNull - Discard n bytes off the top of the stack                      */
/*--------------------------------------------------------------------------*/
static void
pullNull (int n)
{
  emitComment (REGOPS, __func__ );
  wassert (n >= 0);
  adjustStack (n);
}

/*--------------------------------------------------------------------------*/
/* pushRegIfUsed - Push register reg if marked in use. Returns true if the  */
/*                 push was performed, false otherwise.                     */
/*--------------------------------------------------------------------------*/
static bool
pushRegIfUsed (reg_info *reg)
{
  if (!reg->isFree)
    {
      pushReg (reg, true);
      return true;
    }
  else
    return false;
}

/*--------------------------------------------------------------------------*/
/* pushRegIfSurv - Push register reg if marked surviving. Returns true if   */
/*                 the push was performed, false otherwise.                 */
/*--------------------------------------------------------------------------*/
static bool
pushRegIfSurv (reg_info *reg)
{
  if (!reg->isDead)
    {
      pushReg (reg, true);
      return true;
    }
  else
    return false;
}

/*--------------------------------------------------------------------------*/
/* pullOrFreeReg - If needpull is true, register reg is pulled from the     */
/*                 stack. Otherwise register reg is marked as free.         */
/*--------------------------------------------------------------------------*/
static void
pullOrFreeReg (reg_info * reg, bool needpull)
{
  if (needpull)
    pullReg (reg);
  else
    m6502_freeReg (reg);
}

/*--------------------------------------------------------------------------*/
/* adjustStack - Adjust the stack pointer by n bytes.                       */
/*--------------------------------------------------------------------------*/
// TODO: optimize for 65C02
static void
adjustStack (int n)
{
  emitComment (REGOPS, __func__ );
  emitComment (REGOPS, "  %s reg:  %s", __func__, regInfoStr());

  if (n <= -8 || n >= 8) {
    // TODO: too big, consider subroutine
    storeRegTemp(m6502_reg_xa, true);
    emit6502op ("tsx", "");
    transferRegReg (m6502_reg_x, m6502_reg_a, true);
    emit6502op ("clc", "");
    emit6502op ("adc", IMMDFMT, n&0xff);
    transferRegReg (m6502_reg_a, m6502_reg_x, true);
    emit6502op ("txs", "");
    _G.stackPushes -= n;
    n = 0;
    loadRegTemp(m6502_reg_xa);
  }
  while (n < 0) {
    emit6502op ("pha", "");      /* 1 byte,  3 cycles */
    n++;
  }

  if (n > 0) {
    // FIXME: A is incorrectly marked free and makes many regression fail
    //    bool needloada=storeRegTempIfUsed (m6502_reg_a);
    bool needloada = true;
    storeRegTemp(m6502_reg_a, true);
    
    while (n > 0) {
      emit6502op ("pla", "");      /* 1 byte,  4 cycles */
      n--;
    }
    loadOrFreeRegTemp(m6502_reg_a, needloada);
  }
  updateCFA ();
}

/*------------------------------------------------------------------*/
/* swapXA - swap hi and low bytes ox XA                             */
/*------------------------------------------------------------------*/
static void
swapXA ()
{
  emitComment (REGOPS, __func__ );

  if(m6502_reg_a->isLitConst) {
    unsigned char t=m6502_reg_a->litConst;
    transferRegReg(m6502_reg_x, m6502_reg_a, true);
    loadRegFromConst(m6502_reg_a,t);
  } else if(m6502_reg_x->isLitConst) {
    unsigned char t=m6502_reg_x->litConst;
    transferRegReg(m6502_reg_a, m6502_reg_x, true);
    loadRegFromConst(m6502_reg_a,t);
  } else {
    storeRegTemp (m6502_reg_a, false);
    transferRegReg (m6502_reg_x, m6502_reg_a, false);
    loadRegTemp (m6502_reg_x);
  }

  if(m6502_reg_a->isLitConst && m6502_reg_x->isLitConst) {
     m6502_reg_xa->isLitConst=true;
     m6502_reg_xa->litConst=(m6502_reg_x->litConst<<8)|m6502_reg_a->litConst;
    }
}

/*--------------------------------------------------------------------------*/
/* aopName - Return a string with debugging information about an asmop.     */
/*--------------------------------------------------------------------------*/
static char *
aopName (asmop * aop)
{
  static char buffer[262];
  char *buf = buffer;

  if (!aop)
    return "(asmop*)NULL";

  switch (aop->type)
    {
    case AOP_IMMD:
      sprintf (buf, "IMMD(%s)", aop->aopu.aop_immd.aop_immd1);
      return buf;
    case AOP_LIT:
      sprintf (buf, "LIT(%s)", aopLiteral (aop->aopu.aop_lit, 0));
      return buf;
    case AOP_DIR:
      sprintf (buf, "DIR(%s)", aop->aopu.aop_dir);
      return buf;
    case AOP_EXT:
      sprintf (buf, "EXT(%s)", aop->aopu.aop_dir);
      return buf;
    case AOP_SOF:
      sprintf (buf, "SOF(%s)", OP_SYMBOL (aop->op)->name);
      return buf;
    case AOP_REG:
      sprintf (buf, "REG(%s,%s,%s,%s)",
               aop->aopu.aop_reg[3] ? aop->aopu.aop_reg[3]->name : "-",
               aop->aopu.aop_reg[2] ? aop->aopu.aop_reg[2]->name : "-",
               aop->aopu.aop_reg[1] ? aop->aopu.aop_reg[1]->name : "-",
               aop->aopu.aop_reg[0] ? aop->aopu.aop_reg[0]->name : "-");
      return buf;
    case AOP_STK:
      return "STK";
    default:
      sprintf (buf, "?%d", aop->type);
      return buf;
    }

  return "?";
}

// can we BIT aop ?
static bool 
canBitOp (const operand* aop)
{
  switch (AOP_TYPE(aop)) {
    // bit aa, bit aaaa
    case AOP_DIR:
    case AOP_EXT:
      return true;
    // bit #aa
    case AOP_LIT:
      return IS_MOS65C02;
    // TODO: ind,x for 65c02?
  }
  return false;
}

/*--------------------------------------------------------------------------*/
/* loadRegFromAop - Load register reg from logical offset loffset of aop.   */
/*                  For multi-byte registers, loffset is of the lsb reg.    */
/*--------------------------------------------------------------------------*/
static void
loadRegFromAop (reg_info * reg, asmop * aop, int loffset)
{
  int regidx = reg->rIdx;

  if (aop->stacked && aop->stk_aop[loffset])
    {
      loadRegFromAop (reg, aop->stk_aop[loffset], 0);
      return;
    }

  emitComment (REGOPS, "      loadRegFromAop (%s, %s, %d)", reg->name, aopName (aop), loffset);

#if 0
  /* If operand is volatile, we cannot optimize. */
  if (!aop->op || isOperandVolatile (aop->op, false))
    goto forceload;


  /* If this register already has this offset of the operand
     then we need only mark it as in use. */
  if (reg->aop && reg->aop->op && aop->op && operandsEqu (reg->aop->op, aop->op) && (reg->aopofs == loffset))
    {
      m6502_useReg (reg);
      emitComment (REGOPS, "  already had correct value for %s", reg->name);
      return;
    }

  /* TODO: check to see if we can transfer from another register */

  if (m6502_reg_hy>aop && m6502_reg_y->aop->op && aop->op
      && operandsEqu (m6502_reg_y->aop->op, aop->op) && (m6502_reg_y->aopofs == loffset))
    {
      emitComment (REGOPS, "  found correct value for %s in h", reg->name);
      transferRegReg (m6502_reg_y, reg, false);
      m6502_useReg (reg);
      return;
    }


  if (m6502_reg_x->aop && m6502_reg_x->aop->op && aop->op
      && operandsEqu (m6502_reg_x->aop->op, aop->op) && (m6502_reg_x->aopofs == loffset))
    {
      emitComment (REGOPS, "  found correct value for %s in x", reg->name);
      transferRegReg (m6502_reg_x, reg, false);
      m6502_useReg (reg);
      return;
    }

  if (m6502_reg_a->aop && m6502_reg_a->aop->op && aop->op
      && operandsEqu (m6502_reg_a->aop->op, aop->op) && (m6502_reg_a->aopofs == loffset))
    {
      emitComment (REGOPS, "  found correct value for %s in a", reg->name);
      transferRegReg (m6502_reg_a, reg, false);
      m6502_useReg (reg);
      return;
    }

forceload:
#endif

  switch (regidx)
    {
    case X_IDX:
    case A_IDX:
    case Y_IDX:
      if (aop->type == AOP_REG)
        {
          if (loffset < aop->size)
            transferRegReg (aop->aopu.aop_reg[loffset], reg, false);
          else
            loadRegFromConst (reg, 0); /* TODO: handle sign extension */
        }
      else if (aop->type == AOP_LIT)
        {
          loadRegFromConst (reg, byteOfVal (aop->aopu.aop_lit, loffset));
        }
      // no such thing as stx aa,x
      else if (aop->type == AOP_SOF && regidx != A_IDX)
        {
          bool needloada = storeRegTempIfUsed(m6502_reg_a);
          loadRegFromAop(m6502_reg_a, aop, loffset);
          transferRegReg(m6502_reg_a, reg, false);
          loadOrFreeRegTemp(m6502_reg_a,needloada);
          break;
        }
      else
        {
          aopAdrPrepare(aop, loffset);
          const char *l = aopAdrStr (aop, loffset, false);
          emit6502op (regidx == A_IDX ? "lda" : regidx == X_IDX ? "ldx" : "ldy", l);
          aopAdrUnprepare(aop, loffset);
          m6502_dirtyReg (reg);
        }
      break;

    case YX_IDX:
      if (IS_AOP_YX (aop))
        break;
      else if (IS_AOP_XA (aop))
        transferRegReg (m6502_reg_xa, m6502_reg_yx, false);
      else if (IS_AOP_AX (aop))
        transferRegReg (m6502_reg_a, m6502_reg_y, false);
      else
        {
          loadRegFromAop (m6502_reg_x, aop, loffset);
          loadRegFromAop (m6502_reg_y, aop, loffset + 1);
        }
      break;

    case XA_IDX:
      if (IS_AOP_XA (aop))
        break;
      else if (IS_AOP_YX (aop))
        transferRegReg (m6502_reg_yx, m6502_reg_xa, false);
      else if (IS_AOP_AX (aop))
        {
          emitComment(REGOPS, "   * AX->XA");
          swapXA();
        }
      else
        {
          emitComment (REGOPS, "XA");
          loadRegFromAop (m6502_reg_a, aop, loffset);
          loadRegFromAop (m6502_reg_x, aop, loffset + 1);
        }
      break;
    }

  m6502_useReg (reg);
}

// get any free 8-bit register
static reg_info*
getFreeByteReg()
{
  if (m6502_reg_a->isFree)
    return m6502_reg_a;
  else if (m6502_reg_x->isFree)
    return m6502_reg_x;
  else if (m6502_reg_y->isFree)
    return m6502_reg_y;
  else
    return NULL;
}

// TODO: move more to this one?
static reg_info*
getDeadByteReg()
{
  if (m6502_reg_a->isDead)
    return m6502_reg_a;
  else if (m6502_reg_x->isDead)
    return m6502_reg_x;
  else if (m6502_reg_y->isDead)
    return m6502_reg_y;
  else
    return NULL;
}

/*--------------------------------------------------------------------------*/
/* storeRegToAop - Store register reg to logical offset loffset of aop.     */
/*                 For multi-byte registers, loffset is of the lsb reg.     */
/*--------------------------------------------------------------------------*/
static void
storeRegToAop (reg_info *reg, asmop * aop, int loffset)
{
  int regidx = reg->rIdx;

  emitComment (TRACE_AOP, "      storeRegToAop (%s, %s, %d), stacked=%d",
                reg->name, aopName (aop), loffset, aop->stacked);

  if ((regidx == YX_IDX) && aop->stacked && (aop->stk_aop[loffset] || aop->stk_aop[loffset + 1]))
    {
      storeRegToAop (m6502_reg_y, aop, loffset + 1);
      storeRegToAop (m6502_reg_x, aop, loffset);
      return;
    }

  if ((regidx == XA_IDX) && aop->stacked && (aop->stk_aop[loffset] || aop->stk_aop[loffset + 1]))
    {
      storeRegToAop (m6502_reg_x, aop, loffset + 1);
      storeRegToAop (m6502_reg_a, aop, loffset);
      return;
    }

  if (aop->stacked && aop->stk_aop[loffset])
    {
      storeRegToAop (reg, aop->stk_aop[loffset], 0);
      return;
    }

  if (aop->type == AOP_DUMMY)
    return;

  if (aop->type == AOP_CRY)     /* This can only happen if IFX was optimized */
    return;                     /* away, so just toss the result */

  switch (regidx)
    {
    case A_IDX:
      if ((aop->type == AOP_REG) && (loffset < aop->size))
        transferRegReg (reg, aop->aopu.aop_reg[loffset], false);
      else
        {
          emitComment (TRACE_AOP|VVDBG, "      storeRegToAop: A");

          // TODO: i think this does not assemble?
          aopAdrPrepare(aop, loffset);
          emit6502op ("sta", aopAdrStr (aop, loffset, false));
          aopAdrUnprepare(aop, loffset);
        }
      break;
    case X_IDX:
    case Y_IDX:
      if ((aop->type == AOP_REG) && (loffset < aop->size))
        transferRegReg (reg, aop->aopu.aop_reg[loffset], false);
      else if (aop->type == AOP_SOF)
        {
          // TODO: push if live
          bool needpulla = pushRegIfUsed (m6502_reg_a);
          transferRegReg (reg, m6502_reg_a, false);
          storeRegToAop (m6502_reg_a, aop, loffset);
          pullOrFreeReg (m6502_reg_a, needpulla);
        }
      else
        {
          aopAdrPrepare(aop, loffset);
          emit6502op (regidx==X_IDX?"stx":"sty", aopAdrStr (aop, loffset, false));
          aopAdrUnprepare(aop, loffset);
        }
      break;
    case YX_IDX:
      if (aop->type == AOP_SOF) // TODO: will fail assemble
        {
          //     werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "storeRegToAop: aop->type == AOP_SOF");
	  //     int offset = (_G.stackOfs + _G.stackPushes + aop->aopu.aop_stk + aop->size - loffset - 1);
        }
      if (aop->type == AOP_DIR || aop->type == AOP_EXT)
        {
          aopAdrPrepare(aop, loffset);
          emit6502op ("stx", aopAdrStr (aop, loffset, true));
          emit6502op ("sty", aopAdrStr (aop, loffset+1, true));
          aopAdrUnprepare(aop, loffset);
        }
      else if (IS_AOP_XA (aop))
        transferRegReg (reg, m6502_reg_xa, false);
      else if (IS_AOP_YX (aop))
        break;
      else if (m6502_reg_a->isFree)
        {
          bool needpula;
          needpula = pushRegIfUsed (m6502_reg_a);
          transferRegReg (m6502_reg_y, m6502_reg_a, false);
          storeRegToAop (m6502_reg_a, aop, loffset + 1);
          storeRegToAop (m6502_reg_x, aop, loffset);
          pullOrFreeReg (m6502_reg_a, needpula);
        }
      else
        {
          bool needloadx;
          storeRegToAop (m6502_reg_x, aop, loffset);
          needloadx = storeRegTempIfUsed (m6502_reg_x);
          transferRegReg (m6502_reg_y, m6502_reg_x, false);
          storeRegToAop (m6502_reg_x, aop, loffset + 1);
          loadOrFreeRegTemp (m6502_reg_x, needloadx);
        }
      break;
    case XA_IDX:
      if (IS_AOP_YX (aop))
        transferRegReg (reg, m6502_reg_yx, false);
      else if (IS_AOP_XA (aop))
        break;
      else if (IS_AOP_AX (aop))
        {
          swapXA();
        }
      else
        {
          storeRegToAop (m6502_reg_a, aop, loffset);
          storeRegToAop (m6502_reg_x, aop, loffset + 1);
        }
      break;
    default:
      wassert (0);
    }

  /* Disable the register tracking for now */
#if 0
  //if (!reg->aop || (reg->aop && (reg->aop != aop)))
  {
    //if (reg->aop!=aop)
    for (otheridx = 0; otheridx < m6502_nRegs; otheridx++)
      {
        otherreg = m6502_regWithIdx (otheridx);
        if (otherreg && otherreg->aop
            && otherreg->aop->op && aop->op && operandsEqu (otherreg->aop->op, aop->op) && (otherreg->aopofs == loffset))
          {
            emitComment (TRACE_AOP|VVDBG, "  marking %s stale", otherreg->name);
            otherreg->aop = NULL;
          }
      }
    if ((!m6502_reg_x->aop || !m6502_reg_y->aop) && m6502_reg_yx->aop)
      {
        m6502_reg_yx->aop = NULL;
        emitComment (TRACE_AOP|VVDBG, "  marking yx stale");
      }
    if ((!m6502_reg_x->aop || !m6502_reg_a->aop) && m6502_reg_xa->aop)
      {
        m6502_reg_xa->aop = NULL;
        emitComment (TRACE_AOP|VVDBG, "  marking xa stale");
      }

    reg->aop = aop;
    reg->aopofs = loffset;
  }
#endif
}

/*--------------------------------------------------------------------------*/
/* loadRegFromConst - Load register reg from constant c.                    */
/*--------------------------------------------------------------------------*/
static void
loadRegFromConst (reg_info * reg, int c)
{
  emitComment (REGOPS, __func__ );

  switch (reg->rIdx)
    {
    case A_IDX:
      c &= 0xff;
      if (reg->isLitConst && reg->litConst == c)
            break;

      if (m6502_reg_y->isLitConst && m6502_reg_y->litConst == c)
        transferRegReg (m6502_reg_y, reg, false);
      else if (m6502_reg_x->isLitConst && m6502_reg_x->litConst == c)
        transferRegReg (m6502_reg_x, reg, false);
      else
        {
          emit6502op ("lda", IMMDFMT, c);
        }
      break;
    case X_IDX:
      c &= 0xff;
      if (reg->isLitConst)
        {
          if (reg->litConst == c)
            break;
          if (((reg->litConst + 1) & 0xff) == c)
            {
              emit6502op ("inx", "");
              break;
            }
          if (((reg->litConst - 1) & 0xff) == c)
            {
              emit6502op ("dex", "");
              break;
            }
        }

      if (m6502_reg_a->isLitConst && m6502_reg_a->litConst == c)
        transferRegReg (m6502_reg_a, reg, false);
        /*
        TODO does not work for X<->Y
      else if (m6502_reg_y->isLitConst && m6502_reg_y->litConst == c)
        transferRegReg (m6502_reg_y, reg, false);
        */
      else
        {
          emit6502op ("ldx", IMMDFMT, c);
        }
      break;
    case Y_IDX:
      c &= 0xff;
      if (reg->isLitConst)
        {
          if (reg->litConst == c)
            break;
          if (((reg->litConst + 1) & 0xff) == c)
            {
              emit6502op ("iny", "");
              break;
            }
          if (((reg->litConst - 1) & 0xff) == c)
            {
              emit6502op ("dey", "");
              break;
            }
        }

      if (m6502_reg_a->isLitConst && m6502_reg_a->litConst == c)
        transferRegReg (m6502_reg_a, reg, false);
        /*
        TODO does not work for X<->Y
      else if (m6502_reg_x->isLitConst && m6502_reg_x->litConst == c)
        transferRegReg (m6502_reg_x, reg, false);
        */
      else
        {
          emit6502op ("ldy", IMMDFMT, c);
        }
      break;
    case YX_IDX:
      c &= 0xffff;
      if (reg->isLitConst && reg->litConst == c)
	break;
      loadRegFromConst (m6502_reg_x, c);
      loadRegFromConst (m6502_reg_y, c >> 8);
      break;
    case XA_IDX:
      c &= 0xffff;
      if (reg->isLitConst && reg->litConst == c)
	break;
      loadRegFromConst (m6502_reg_a, c);
      loadRegFromConst (m6502_reg_x, c >> 8);
      break;
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "Bad rIdx in loadRegFromConst");
      return;
    }

  m6502_dirtyReg (reg);
  reg->isLitConst = 1;
  reg->litConst = c;
  if (reg->rIdx == YX_IDX)
    {
      m6502_reg_x->isLitConst = 1;
      m6502_reg_x->litConst = c & 0xff;
      m6502_reg_y->isLitConst = 1;
      m6502_reg_y->litConst = (c >> 8) & 0xff;
    }
  if (reg->rIdx == XA_IDX)
    {
      m6502_reg_a->isLitConst = 1;
      m6502_reg_a->litConst = c & 0xff;
      m6502_reg_x->isLitConst = 1;
      m6502_reg_x->litConst = (c >> 8) & 0xff;
    }
  m6502_useReg (reg);
}

/*--------------------------------------------------------------------------*/
/* loadRegFromImm - Load register reg from immediate value c.               */
/*--------------------------------------------------------------------------*/
static void
loadRegFromImm (reg_info * reg, char * c)
{
  emitComment (REGOPS, __func__ );

  if(!c) {
     werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "loadRegFromImm called with a null arg pointer");
  }

  if (*c == '#')
    c++;
  switch (reg->rIdx)
    {
    case A_IDX:
      emit6502op ("lda", "#%s", c);
      break;
    case X_IDX:
      emit6502op ("ldx", "#%s", c);
      break;
    case Y_IDX:
      emit6502op ("ldy", "#%s", c);
      break;
    case YX_IDX:
      emit6502op ("ldx", "#%s", c);
      emit6502op ("ldy", "#%s >> 8", c);
      break;
    case XA_IDX:
      emit6502op ("lda", "#%s", c);
      emit6502op ("ldx", "#%s >> 8", c);
      break;
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "Bad rIdx in loadRegFromConst");
      return;
    }
  m6502_dirtyReg (reg);
  m6502_useReg (reg);
}

/*--------------------------------------------------------------------------*/
/* storeConstToAop- Store constant c to logical offset loffset of asmop aop.*/
/*--------------------------------------------------------------------------*/
static void
storeConstToAop (int c, asmop * aop, int loffset)
{
  emitComment (REGOPS, __func__ );

  if (aop->stacked && aop->stk_aop[loffset])
    {
      storeConstToAop (c, aop->stk_aop[loffset], 0);
      return;
    }

  /* If the value needed is already in A, X or Y just store it */
  if (m6502_reg_a->isLitConst && m6502_reg_a->litConst == c)
    {
      storeRegToAop (m6502_reg_a, aop, loffset);
      return;
    }
  if (m6502_reg_x->isLitConst && m6502_reg_x->litConst == c)
    {
      storeRegToAop (m6502_reg_x, aop, loffset);
      return;
    }
  if (m6502_reg_y->isLitConst && m6502_reg_y->litConst == c)
    {
      storeRegToAop (m6502_reg_y, aop, loffset);
      return;
    }

  switch (aop->type)
    {
    case AOP_REG:
      if (loffset > (aop->size - 1))
        break;
      loadRegFromConst (aop->aopu.aop_reg[loffset], c);
      break;
    case AOP_DUMMY:
      break;
    case AOP_DIR:
    case AOP_EXT:
      /* clr operates with read-modify-write cycles, so don't use if the */
      /* destination is volatile to avoid the read side-effect. */
      // TODO: same for 6502?
#if 0
      if (!c && IS_MOS65C02 && !(aop->op && isOperandVolatile (aop->op, false)))
        {
	  // FIXME: unimplemented
          aopAdrPrepare(aop, loffset);
          emitcode ("stz", "%s", aopAdrStr (aop, loffset, false));
          aopAdrUnprepare(aop, loffset);
          break;
        }
#endif
    default:
      if (m6502_reg_x->isFree && aop->type != AOP_SOF)
        {
          loadRegFromConst (m6502_reg_x, c);
          storeRegToAop (m6502_reg_x, aop, loffset);
          m6502_freeReg (m6502_reg_x);
        }
      else if (m6502_reg_y->isFree && aop->type != AOP_SOF)
        {
          loadRegFromConst (m6502_reg_y, c);
          storeRegToAop (m6502_reg_y, aop, loffset);
          m6502_freeReg (m6502_reg_y);
        }
      else
        {
          bool needpulla = pushRegIfUsed (m6502_reg_a);
 //         pushReg (m6502_reg_a, true);
          loadRegFromConst (m6502_reg_a, c);
          storeRegToAop (m6502_reg_a, aop, loffset);
//          pullReg (m6502_reg_a);
          pullOrFreeReg (m6502_reg_a, needpulla);
        }
    }
}

/*--------------------------------------------------------------------------*/
/* storeImmToAop- Store immediate value c to logical offset loffset of asmop aop.*/
/*--------------------------------------------------------------------------*/
static void
storeImmToAop (char *c, asmop * aop, int loffset)
{
  emitComment (TRACE_AOP, __func__ );

  if (aop->stacked && aop->stk_aop[loffset])
    {
      storeImmToAop (c, aop->stk_aop[loffset], 0);
      return;
    }

  switch (aop->type)
    {
    case AOP_REG:
      if (loffset > (aop->size - 1))
        break;
      loadRegFromImm (aop->aopu.aop_reg[loffset], c);
      break;
    case AOP_DUMMY:
      break;
    case AOP_DIR:
      /* clr operates with read-modify-write cycles, so don't use if the */
      /* destination is volatile to avoid the read side-effect. */
      if (!strcmp (c, "#0x00") && IS_MOS65C02 && !(aop->op && isOperandVolatile (aop->op, false)))
        {
	  // FIXME: unimplemented
          aopAdrPrepare(aop, loffset);
          emitcode ("stz", "%s", aopAdrStr (aop, loffset, false));
          aopAdrUnprepare(aop, loffset);
          break;
        }
    default:
      if (m6502_reg_x->isFree)
        {
          loadRegFromImm (m6502_reg_x, c);
          storeRegToAop (m6502_reg_x, aop, loffset);
          m6502_freeReg (m6502_reg_x);
        }
      else if (m6502_reg_y->isFree)
        {
          loadRegFromImm (m6502_reg_y, c);
          storeRegToAop (m6502_reg_y, aop, loffset);
          m6502_freeReg (m6502_reg_y);
        }
      else
        {
          bool needpulla = pushRegIfUsed (m6502_reg_a);
          loadRegFromImm (m6502_reg_a, c);
          storeRegToAop (m6502_reg_a, aop, loffset);
          pullOrFreeReg (m6502_reg_a, needpulla);
        }
    }
}

static void
signExtendA()
{
      emit6502op ("asl", "a");
      loadRegFromConst (m6502_reg_a, 0);
      emit6502op ("adc", "#0xff");
      emit6502op ("eor", "#0xff");
}

/*--------------------------------------------------------------------------*/
/* storeRegSignToUpperAop - If isSigned is true, the sign bit of register   */
/*                          reg is extended to fill logical offsets loffset */
/*                          and above of asmop aop. Otherwise, logical      */
/*                          offsets loffset and above of asmop aop are      */
/*                          zeroed. reg must be an 8-bit register.          */
/*--------------------------------------------------------------------------*/
static void
storeRegSignToUpperAop (reg_info * reg, asmop * aop, int loffset, bool isSigned)
{
  emitComment (TRACE_AOP, __func__ );

//  int regidx = reg->rIdx;
  int size = aop->size;

  if (size <= loffset)
    return;

  if (!isSigned)
    {
      /* Unsigned case */
      while (loffset < size)
        storeConstToAop (0, aop, loffset++);
    }
  else
    {
      /* Signed case */
      transferRegReg (reg, m6502_reg_a, false);
      signExtendA();
      m6502_useReg (m6502_reg_a);
      while (loffset < size)
        storeRegToAop (m6502_reg_a, aop, loffset++);
      m6502_freeReg (m6502_reg_a);
    }
}

/*--------------------------------------------------------------------------*/
/* storeRegToFullAop - Store register reg to asmop aop with appropriate     */
/*                     padding and/or truncation as needed. If isSigned is  */
/*                     true, sign extension will take place in the padding. */
/*--------------------------------------------------------------------------*/
static void
storeRegToFullAop (reg_info *reg, asmop *aop, bool isSigned)
{
  int regidx = reg->rIdx;
  int size = aop->size;

  emitComment (TRACE_AOP, __func__ );

  switch (regidx)
    {
    case A_IDX:
    case X_IDX:
    case Y_IDX:
      storeRegToAop (reg, aop, 0);
      if (size > 1 && isSigned && aop->type == AOP_REG && aop->aopu.aop_reg[0]->rIdx == A_IDX)
        pushReg (m6502_reg_a, true);
      storeRegSignToUpperAop (reg, aop, 1, isSigned);
      if (size > 1 && isSigned && aop->type == AOP_REG && aop->aopu.aop_reg[0]->rIdx == A_IDX)
        pullReg (m6502_reg_a);
      break;
    case YX_IDX:
      if (size == 1)
        {
          storeRegToAop (m6502_reg_x, aop, 0);
        }
      else
        {
          storeRegToAop (reg, aop, 0);
          storeRegSignToUpperAop (m6502_reg_y, aop, 2, isSigned);
        }
      break;
    case XA_IDX:
      if (size == 1)
        {
          storeRegToAop (m6502_reg_a, aop, 0);
        }
      else
        {
          storeRegToAop (reg, aop, 0);
          storeRegSignToUpperAop (m6502_reg_x, aop, 2, isSigned);
        }
      break;
    default:
      wassert (0);
    }
}

/*--------------------------------------------------------------------------*/
/* transferAopAop - Transfer the value at logical offset srcofs of asmop    */
/*                  srcaop to logical offset dstofs of asmop dstaop.        */
/*--------------------------------------------------------------------------*/
static void
transferAopAop (asmop *srcaop, int srcofs, asmop *dstaop, int dstofs)
{
  bool needpula = false;
  reg_info *reg = NULL;
  bool keepreg = false;

  emitComment (TRACE_AOP, __func__ );

  wassert (srcaop && dstaop);

  /* ignore transfers at the same byte, unless its volatile */
  if (srcaop->op && !isOperandVolatile (srcaop->op, false)
      && dstaop->op && !isOperandVolatile (dstaop->op, false)
      && operandsEqu (srcaop->op, dstaop->op) && srcofs == dstofs && dstaop->type == srcaop->type)
    return;

  if (srcaop->stacked && srcaop->stk_aop[srcofs])
    {
      transferAopAop (srcaop->stk_aop[srcofs], 0, dstaop, dstofs);
      return;
    }

  if (dstaop->stacked && dstaop->stk_aop[srcofs])
    {
      transferAopAop (srcaop, srcofs, dstaop->stk_aop[dstofs], 0);
      return;
    }

  emitComment (TRACE_AOP|VVDBG, "  transferAopAop from (%s, %d)", aopName (srcaop), srcofs);
  emitComment (TRACE_AOP|VVDBG, "  transferAopAop to (%s, %d)", aopName (dstaop), dstofs);
//  DD(emitcode ("", "; srcaop->type = %d, regmask = %x", srcaop->type, srcaop->regmask));
//  DD(emitcode ("", "; dstaop->type = %d, regmask = %x", dstaop->type, dstaop->regmask));

  if (dstofs >= dstaop->size)
    return;

  // same registers and offset, no transfer
  if (srcaop->type == AOP_REG && dstaop->type == AOP_REG)
    {
      transferRegReg(srcaop->aopu.aop_reg[srcofs], dstaop->aopu.aop_reg[dstofs], false);
      return;    
    }

  if (srcaop->type == AOP_LIT)
    {
      storeConstToAop (byteOfVal (srcaop->aopu.aop_lit, srcofs), dstaop, dstofs);
      return;
    }
  if (dstaop->type == AOP_REG)
    {
      reg = dstaop->aopu.aop_reg[dstofs];
      keepreg = true;
    }
  else if ((srcaop->type == AOP_REG) && (srcaop->aopu.aop_reg[srcofs]))
    {
      reg = srcaop->aopu.aop_reg[srcofs];
      keepreg = true;
    }

  // TODO: pick reg based on if can load op?
  if (!reg)
    {
      reg = getFreeByteReg();
      if (reg == NULL)
        {
	  pushReg (m6502_reg_a, true);
	  needpula = true;
          reg = m6502_reg_a;
        }
    }

  loadRegFromAop (reg, srcaop, srcofs);
  storeRegToAop (reg, dstaop, dstofs);

  if (!keepreg)
    pullOrFreeReg (reg, needpula);
}

#if 0
/*--------------------------------------------------------------------------*/
/* forceStackedAop - Reserve space on the stack for asmop aop; when         */
/*                   freeAsmop is called with aop, the stacked data will    */
/*                   be copied to the original aop location.                */
/*--------------------------------------------------------------------------*/
// TODO????
static asmop *
forceStackedAop (asmop * aop, bool copyOrig)
{
  reg_info *reg = NULL;
  int offset;
  bool needpula = false;
  asmop *newaop = newAsmop (AOP_DIR);
  memcpy (newaop, aop, sizeof (*newaop));
  newaop->aopu.aop_dir = "__TEMP";

  emitComment (TRACE_AOP|VVDBG, "  forcedStackedAop %s", aopName (aop));

  if (copyOrig) {
    reg = getFreeByteReg();
    if (reg == NULL) {
      reg = m6502_reg_a;
      storeRegTemp(reg, true);
      needpula = true;
    }
  }
  for (offset=0; offset<newaop->size; offset++)
  {
    asmop *aopsof = newAsmop (AOP_SOF);
    aopsof->size = 1;
    if (copyOrig)
      {
        loadRegFromAop (reg, aop, offset);
        pushReg (reg, false);
      } else {
        pushReg (m6502_reg_a, false);
      }
    aopsof->aopu.aop_stk = -_G.stackOfs - _G.stackPushes;
    aopsof->op = aop->op;
    newaop->stk_aop[offset] = aopsof;
  }

  if (!reg && copyOrig)
    {
      for (offset = 0; offset < newaop->size; offset++)
        {
          transferAopAop (aop, offset, newaop, offset);
        }
    }  
  newaop->stacked = 1;
  // FIXME: use loadOrFree
  if (needpula) loadRegTemp(reg);
  return newaop;
}
#endif

// TODO: fix these
/*--------------------------------------------------------------------------*/
/* accopWithAop - Emit accumulator modifying instruction accop with the     */
/*                byte at logical offset loffset of asmop aop.              */
/*                Supports: adc, and, cmp, eor, ora, sbc                    */
/*--------------------------------------------------------------------------*/
static void
accopWithAop (char *accop, asmop *aop, int loffset)
{
  emitComment (TRACE_AOP, __func__ );

  if (aop->stacked && aop->stk_aop[loffset])
    {
      accopWithAop (accop, aop->stk_aop[loffset], 0);
      return;
    }

  if (aop->type == AOP_DUMMY)
    return;

  if (loffset >= aop->size)
    {
      emit6502op (accop, "#0x00");
    }
  else if (aop->type == AOP_REG)
    {
      // TODO FIXME: this needs forcestore
      storeRegTemp (aop->aopu.aop_reg[loffset], true);
      emit6502op (accop, TEMPFMT, _G.tempOfs - 1);
      loadRegTemp(NULL);
    }
  else
    {
      aopAdrPrepare(aop, loffset);
      emit6502op (accop, aopAdrStr (aop, loffset, false));
      aopAdrUnprepare(aop, loffset);
    }
}


/*--------------------------------------------------------------------------*/
/* rmwWithReg - Emit read/modify/write instruction rmwop with register reg. */
/*              byte at logical offset loffset of asmop aop. Register reg   */
/*              must be 8-bit.                                              */
/*              Supports: com, dec, inc, lsl, lsr, neg, rol, ror            */
/*--------------------------------------------------------------------------*/
static void
rmwWithReg (char *rmwop, reg_info * reg)
{
  if (reg->rIdx == A_IDX)
    {
      if (!strcmp(rmwop, "inc")) {
        if (IS_MOS65C02) {
          emit6502op (rmwop, "a");
        } else {
          emit6502op ("clc", "");
          emit6502op ("adc", "#0x01");
        }
      } else if (!strcmp(rmwop, "dec")) {
        if (IS_MOS65C02) {
          emit6502op (rmwop, "a");
        } else {
          emit6502op ("sec", "");
          emit6502op ("sbc", "#0x01");
        }
      } else if (!strcmp(rmwop, "com")) {
        emit6502op ("eor", "#0xff");
      } else if (!strcmp(rmwop, "neg")) {
        emit6502op ("eor", "#0xff");
        emit6502op ("clc", "");
        emit6502op ("adc", "#0x01");
      } else if (!strcmp(rmwop, "asr")) {
        emit6502op ("cmp", "#0x80");
        emit6502op ("ror", "a");
      } else if (!strcmp(rmwop, "bit")) { // TODO???
        // werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "rmwWithReg: rmwop==\"bit\"");
        emit6502op ("cmp", "#0x00");
      } else {
        emit6502op (rmwop, "a");
      }
    }
  else if (reg->rIdx == X_IDX)
    {
      if (!strcmp(rmwop, "inc")) {
        emit6502op ("inx", "");
      } else if (!strcmp(rmwop, "dec")) {
        emit6502op ("dex", "");
      } else {
        bool needpulla = pushRegIfUsed (m6502_reg_a);
        transferRegReg (m6502_reg_x, m6502_reg_a, true);
        rmwWithReg (rmwop, m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_x, true);
        pullOrFreeReg (m6502_reg_a, needpulla);
      }
    }
  else if (reg->rIdx == Y_IDX)
    {
      if (!strcmp(rmwop, "inc")) {
        emit6502op ("iny", "");
      } else if (!strcmp(rmwop, "dec")) {
        emit6502op ("dey", "");
      } else {
        bool needpulla = pushRegIfUsed (m6502_reg_a);
        transferRegReg (m6502_reg_y, m6502_reg_a, true);
        rmwWithReg (rmwop, m6502_reg_a);
        transferRegReg (m6502_reg_a, m6502_reg_y, true);
        pullOrFreeReg (m6502_reg_a, needpulla);
      }
    }
  else
    {
      wassertl(0, "rmwWithReg()");
    }
  // always dirty dest. register
  //  m6502_dirtyReg (reg);
}

/*--------------------------------------------------------------------------*/
/* rmwWithAop - Emit read/modify/write instruction rmwop with the byte at   */
/*                logical offset loffset of asmop aop.                      */
/*                Supports: bit, dec, inc, lsl, lsr, neg, rol, ror          */
/*--------------------------------------------------------------------------*/
static void
rmwWithAop (char *rmwop, asmop * aop, int loffset)
{
  bool needpull = false;
  emitComment (TRACE_AOP, __func__ );

  if (aop->stacked && aop->stk_aop[loffset])
    {
      rmwWithAop (rmwop, aop->stk_aop[loffset], 0);
      return;
    }

  switch (aop->type)
    {
    case AOP_REG:
      rmwWithReg (rmwop, aop->aopu.aop_reg[loffset]);
      break;
    case AOP_DIR:
    case AOP_EXT:
      aopAdrPrepare(aop, loffset);
      // TODO: this sucks
      if (!strcmp("asr", rmwop)) {
        emit6502op ("pha", "");
        emit6502op ("lda", aopAdrStr(aop, loffset, false)); // load 
        emit6502op ("cmp", "#0x80");
        emit6502op ("pla", "");
        rmwop = "ror";
      }
      emit6502op (rmwop, aopAdrStr(aop, loffset, false));
      aopAdrUnprepare(aop, loffset);
      break;
    case AOP_DUMMY:
      break;
    case AOP_SOF:
      {
	 emitComment (TRACE_AOP, "  rmwWithAop AOP_SOF");
        // TODO: does anything but A make sense here?
        reg_info * reg = getFreeByteReg();
        if (!reg) reg = m6502_reg_a;
        int offset = loffset; // SEH: aop->size - 1 - loffset;
        offset += _G.stackOfs + _G.stackPushes + aop->aopu.aop_stk + 1;
	//    if ((offset > 0xff) || (offset < 0))
          {
	    emitComment (TRACE_AOP, "  rmwWithAop large offset");
            /* Unfortunately, the rmw class of instructions only support a */
            /* single byte stack pointer offset and we need two. */
            needpull = pushRegIfUsed (reg);
            loadRegFromAop (reg, aop, loffset);
            rmwWithReg (rmwop, reg);
            if (strcmp ("tst", rmwop)) //TODO: no tst
              storeRegToAop (reg, aop, loffset);
            pullOrFreeReg (reg, needpull);
            break;
          }
        /* If the offset is small enough, fall through to default case */
      }
    default:
      emitComment (TRACE_AOP, "  rmwWithAop small offset ");
      // TODO: [aa],y dosn't work with inc/dec
//      emitcode (rmwop, "%s ;type %d", aopAdrStr (aop, loffset, false), aop->type);
      emit6502op (rmwop, aopAdrStr (aop, loffset, false));
    }

}

/*--------------------------------------------------------------------------*/
/* loadRegIndexed - Load a register using indexed addressing mode.          */
/*                  NOTE: offset is physical (not logical)                  */
/*--------------------------------------------------------------------------*/
static void
loadRegIndexed (reg_info * reg, int offset, char * rematOfs)
{
  bool needpula = false;

  emitComment (REGOPS, "      loadRegIndexed (%s, %d, %s)", reg->name, offset, rematOfs);

  /* The rematerialized offset may have a "#" prefix; skip over it */
  if (rematOfs && rematOfs[0] == '#')
    rematOfs++;
  if (rematOfs && !rematOfs[0])
    rematOfs = NULL;

  /* force offset to signed 16-bit range */
  offset &= 0xffff;
  if (offset & 0x8000)
    offset = 0x10000 - offset;

// TODO: mostly awful
  switch (reg->rIdx)
    {
    case A_IDX:
      // TODO: have to remove dead Y loads
      // addr is in (y,x) but y == 0, so 8-bit offset
      if (!rematOfs && offset >= 0 && offset <= 0xff)
        {
          // FIXME: lots of failures if switch to isDead
          bool restoreY = ! m6502_reg_y->isFree;
//          bool restoreY = ! m6502_reg_y->isDead;
          storeRegTemp (m6502_reg_x, false);
          storeRegTemp (m6502_reg_y, false);
          loadRegFromConst(m6502_reg_y, offset);
          emit6502op ("lda", TEMPFMT_IY, _G.tempOfs - 2);
          // TODO: isDead?
// check here *****
          loadRegTemp (restoreY ? m6502_reg_y: NULL );
          loadRegTemp (NULL); // x not changed
        }
      else
        {
          // add remat + offset + YX
          transferRegReg (m6502_reg_x, m6502_reg_a, true);
          emit6502op ("clc", "");
          emit6502op ("adc", "#<(%s+%d)", rematOfs, offset);
          int tIdx=_G.tempOfs;
          storeRegTemp(m6502_reg_a, true);
          transferRegReg (m6502_reg_y, m6502_reg_a, true);
          emit6502op ("adc", "#>(%s+%d)", rematOfs, offset);
          storeRegTemp(m6502_reg_a, true);
          bool py = storeRegTempIfSurv(m6502_reg_y);
          loadRegFromConst(m6502_reg_y, 0);
          emit6502op ("lda", TEMPFMT_IY, tIdx);
          loadOrFreeRegTemp(m6502_reg_y,py);
          loadRegTemp(NULL);
          loadRegTemp(NULL);
          }
      break;
    case X_IDX:
    case Y_IDX:	
//      pushReg (m6502_reg_a, false);
//      needpula = true;
      needpula = pushRegIfSurv(m6502_reg_a);
      loadRegIndexed (m6502_reg_a, offset, rematOfs);
      transferRegReg (m6502_reg_a, reg, true);
      break;
    case YX_IDX:
      needpula = pushRegIfSurv(m6502_reg_a);
      loadRegIndexed (m6502_reg_a, offset+1, rematOfs);
      storeRegTemp (m6502_reg_a, true);
      loadRegIndexed (m6502_reg_a, offset, rematOfs);
      transferRegReg (m6502_reg_a, m6502_reg_x, true);
      loadRegTemp (m6502_reg_y);
      break;
    case XA_IDX:
      loadRegIndexed (m6502_reg_a, offset, rematOfs);
      pushReg (m6502_reg_a, true);
      needpula = true;
//      needpula = pushRegIfSurv(m6502_reg_a);
      loadRegIndexed (m6502_reg_x, offset+1, rematOfs);
      break;
    default:
      wassert (0);
    }
  pullOrFreeReg (m6502_reg_a, needpula);
}

static int prepTempOfs = -1;
static char* tempRematOfs = "???";
static bool prepSwapAY;

/*--------------------------------------------------------------------------*/
/* storeRegIndexed - Store a register using indexed addressing mode.        */
/*                   NOTE: offset is physical (not logical)                 */
/*--------------------------------------------------------------------------*/
static void
storeRegIndexed (reg_info * reg, int offset, char * rematOfs)
{
  bool needpula = false;

  emitComment (REGOPS, "      storeRegIndexed (%s, %d, %s)", reg->name, offset, rematOfs);

  /* The rematerialized offset may have a "#" prefix; skip over it */
  if (rematOfs && rematOfs[0] == '#')
    rematOfs++;
  if (rematOfs && !rematOfs[0])
    rematOfs = NULL;

  /* force offset to signed 16-bit range */
  offset &= 0xffff;
  if (offset & 0x8000)
    offset = offset - 0x10000;

  switch (reg->rIdx)
    {
    case A_IDX:
      if (!rematOfs && offset >= 0 && offset <= 0xff)
        {
          bool restoreY = ! m6502_reg_y->isFree;
          storeRegTemp (m6502_reg_x, false);
          storeRegTemp (m6502_reg_y, false);
          loadRegFromConst(m6502_reg_y, offset);
          emit6502op ("sta", TEMPFMT_IY, _G.tempOfs - 2);
          // FIXME: changing this to isDead makes regression fail
// *****
          loadRegTemp (restoreY ? m6502_reg_y : NULL);
          loadRegTemp (NULL); // x is not clobbered
        }
      else
        {
          // add remat + offset + YX
          int ptrOfs;
          bool needloady;

          if(!rematOfs) rematOfs="0";

          pushReg(m6502_reg_a, false);
          ptrOfs=_G.tempOfs;
          transferRegReg (m6502_reg_x, m6502_reg_a, true);
          emit6502op ("clc", "");
          emit6502op ("adc", "#<(%s+%d)", rematOfs, offset);
          storeRegTemp (m6502_reg_a, true);
          transferRegReg (m6502_reg_y, m6502_reg_a, true);
          emit6502op ("adc", "#>(%s+%d)", rematOfs, offset);
          storeRegTemp (m6502_reg_a, true);
          needloady = storeRegTempIfSurv (m6502_reg_y);
          loadRegFromConst(m6502_reg_y, 0);
          pullReg(m6502_reg_a);
          emit6502op ("sta", TEMPFMT_IY, ptrOfs); // [aa],y
          loadOrFreeRegTemp(m6502_reg_y, needloady);
          loadRegTemp(NULL);
          loadRegTemp(NULL);
        }
      break;
    case X_IDX:
      needpula = pushRegIfUsed (m6502_reg_a);
      transferRegReg (m6502_reg_x, m6502_reg_a, true);
      storeRegIndexed (m6502_reg_a, offset, rematOfs);
      pullOrFreeReg (m6502_reg_a, needpula);
      break;
    case Y_IDX:
      needpula = pushRegIfUsed (m6502_reg_a);
      transferRegReg (m6502_reg_y, m6502_reg_a, true);
      storeRegIndexed (m6502_reg_a, offset, rematOfs);
      pullOrFreeReg (m6502_reg_a, needpula);
      break;
    case YX_IDX:
      storeRegIndexed (m6502_reg_y, offset+1, rematOfs);
      storeRegIndexed (m6502_reg_x, offset, rematOfs);
      break;
    case XA_IDX:
      /* This case probably won't happen, but it's easy to implement */
      /* SEH: it did happen in bug-1029883? */
      storeRegIndexed (m6502_reg_x, offset+1, rematOfs);
      storeRegIndexed (m6502_reg_a, offset, rematOfs);
      break;
    default:
      wassert (0);
    }
}

#if 0
/*--------------------------------------------------------------------------*/
/* storeRegIndexed2 - Store a register using indexed addressing mode.        */
/*                   NOTE: offset is physical (not logical)                 */
/* must call preparePointer() first */
/*--------------------------------------------------------------------------*/
static void
storeRegIndexed2 (reg_info * reg, int offset)
{
  bool needpula = false;

  emitComment (REGOPS, "      storeRegIndexed (%s, %d)", reg->name, offset);

  /* force offset to signed 16-bit range */
  offset &= 0xffff;
  if (offset & 0x8000)
    offset = offset - 0x10000;

  switch (reg->rIdx)
    {
    case A_IDX:
      if (prepTempOfs < 0)
        {
          emit6502op ("sta", "(%s+%d),y", tempRematOfs, offset);
        }
      else
        {
          loadRegFromConst(m6502_reg_y, offset);
          emit6502op ("sta", TEMPFMT_IY, prepTempOfs);
        }
      break;
    case X_IDX:
      needpula = pushRegIfUsed (m6502_reg_a);
      transferRegReg (m6502_reg_x, m6502_reg_a, true);
      storeRegIndexed2 (m6502_reg_a, offset);
      pullOrFreeReg (m6502_reg_a, needpula);
      break;
    case Y_IDX:
      needpula = pushRegIfUsed (m6502_reg_a);
      transferRegReg (m6502_reg_y, m6502_reg_a, true);
      storeRegIndexed2 (m6502_reg_a, offset);
      pullOrFreeReg (m6502_reg_a, needpula);
      break;
    case YX_IDX:
      storeRegIndexed2 (m6502_reg_y, offset+1);
      storeRegIndexed2 (m6502_reg_x, offset);
      break;
    case XA_IDX:
      /* This case probably won't happen, but it's easy to implement */
      /* SEH: it did happen in bug-1029883? */
      storeRegIndexed2 (m6502_reg_x, offset+1);
      storeRegIndexed2 (m6502_reg_a, offset);
      break;
    default:
      wassert (0);
    }
}
#endif

/*-----------------------------------------------------------------*/
/* newAsmop - creates a new asmOp                                  */
/*-----------------------------------------------------------------*/
static asmop *
newAsmop (short type)
{
  asmop *aop;
// TODO: are these ever freed?
  aop = Safe_calloc (1, sizeof (asmop));
  aop->type = type;
  aop->op = NULL;
  return aop;
}


/*-----------------------------------------------------------------*/
/* operandConflictsWithYX - true if operand in h and/or x register */
/*-----------------------------------------------------------------*/
static bool
operandConflictsWithYX (operand *op)
{
  symbol *sym;
  int i;

  if (IS_ITEMP (op))
    {
      sym = OP_SYMBOL (op);
      if (!sym->isspilt)
        {
          for(i = 0; i < sym->nRegs; i++)
            if (sym->regs[i] == m6502_reg_y || sym->regs[i] == m6502_reg_x)
              return true;
        }
    }

  return false;
}

/*-----------------------------------------------------------------*/
/* operandConflictsWithX - true if operand in x register */
/*-----------------------------------------------------------------*/
static bool
operandConflictsWithX (operand *op)
{
  symbol *sym;
  int i;

  if (IS_ITEMP (op))
    {
      sym = OP_SYMBOL (op);
      if (!sym->isspilt)
        {
          for(i = 0; i < sym->nRegs; i++)
            if (sym->regs[i] == m6502_reg_x)
              return true;
        }
    }

  return false;
}

/*-----------------------------------------------------------------*/
/* operandOnStack - returns True if operand is on the stack        */
/*-----------------------------------------------------------------*/
static bool
operandOnStack(operand *op)
{
  symbol *sym;

  if (!op || !IS_SYMOP (op))
    return false;
  sym = OP_SYMBOL (op);
  if (!sym->isspilt && sym->onStack)
    return true;
  if (sym->isspilt)
    {
      sym = sym->usl.spillLoc;
      if (sym && sym->onStack)
        return true;
    }
  return false;
}

/*-----------------------------------------------------------------*/
/* tsxUseful - returns True if tsx could help at least one         */
/*             anticipated stack reference                         */
/*-----------------------------------------------------------------*/
static bool
tsxUseful(iCode *ic)
{
  int uses = 0;

  if (ic->op == CALL)
    {
      if (IC_RESULT (ic) && operandSize (IC_RESULT (ic)) < 2 && operandOnStack (IC_RESULT (ic)))
        {
          uses++;
          ic = ic->next;
        }
    }

  while (ic && uses < 1)
    {
      if (ic->op == IFX)
        {
          if (operandOnStack (IC_COND (ic)))
            uses += operandSize(IC_COND (ic));
          break;
        }
      else if (ic->op == JUMPTABLE)
        {
          if (operandOnStack (IC_JTCOND (ic)))
            uses++;
          break;
        }
      else if (ic->op == ADDRESS_OF)
        {
          if (operandOnStack (IC_RIGHT (ic)))
            break;
        }
      else if (ic->op == LABEL || ic->op == GOTO || ic->op == CALL || ic->op == PCALL)
        break;
      else if (POINTER_SET (ic) || POINTER_GET (ic))
        break;
      else
        {
          if (operandConflictsWithYX (IC_RESULT (ic)))
            break;
          if (operandOnStack (IC_LEFT (ic)))
            uses += operandSize (IC_LEFT (ic));
          if (operandOnStack (IC_RIGHT (ic)))
            uses += operandSize (IC_RIGHT (ic));
          if (operandOnStack (IC_RESULT (ic)))
            uses += operandSize (IC_RESULT (ic));
        }

      ic = ic->next;
    }

  return uses >= 1;
}

static void doTSX()
{
  // already did TSX
  if (m6502_reg_x->aop == &tsxaop)
    return;
  // put stack pointer in X
  if(!m6502_reg_x->isFree)
    emitcode("ERROR","doTSX called with X in use");
  emit6502op ("tsx", "");
  m6502_dirtyReg (m6502_reg_x);
  m6502_reg_x->aop = &tsxaop;
  _G.tsxStackPushes = _G.stackPushes;
}

// TODO: make these subroutines
static void saveBasePtr()
{
#if 0
  storeRegTemp (m6502_reg_x, true); // TODO: only when used?
  // TODO: if X is free should we call doTSX() to mark X=S?
  doTSX();
  emit6502op ("stx", BASEPTR);
  _G.baseStackPushes = _G.stackPushes;
  loadRegTemp (m6502_reg_x);
#endif
}

static void
restoreBasePtr()
{
  // we recompute with saveBasePtr() after each jsr
}

/*-----------------------------------------------------------------*/
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *
aopForSym (iCode * ic, symbol * sym, bool result)
{
  asmop *aop;
  memmap *space;

  wassertl (ic != NULL, "Got a null iCode");
  wassertl (sym != NULL, "Got a null symbol");

//  printf("in aopForSym for symbol %s\n", sym->name);

  space = SPEC_OCLS (sym->etype);

  /* if already has one */
  if (sym->aop)
    {
      return sym->aop;
    }

  /* special case for a function */
  if (IS_FUNC (sym->type))
    {
      sym->aop = aop = newAsmop (AOP_IMMD);
      aop->aopu.aop_immd.aop_immd1 = Safe_calloc (1, strlen (sym->rname) + 1 + 6);
      sprintf (aop->aopu.aop_immd.aop_immd1, "(%s - 1)", sym->rname); // function pointer; take back one for RTS
      aop->size = FARPTRSIZE;
      return aop;
    }

  /* if it is on the stack */
  if (sym->onStack)
    {
      sym->aop = aop = newAsmop (AOP_SOF);
      aop->aopu.aop_dir = sym->rname;
      aop->size = getSize (sym->type);
      aop->aopu.aop_stk = sym->stack;

      if (!regalloc_dry_run && m6502_reg_x->isFree && m6502_reg_x->aop != &tsxaop)
        {
          if (!m6502_reg_x->isDead)
            return aop;
          if (ic->op == IFX && operandConflictsWithX (IC_COND (ic)))
            return aop;
          else if (ic->op == JUMPTABLE && operandConflictsWithX (IC_JTCOND (ic)))
            return aop;
          else
            {
              // FIXME: this is likely incorrect at YX is not a adr register in the 6502
              /* If this is a pointer gen/set, then hx is definitely in use */
              if (POINTER_SET (ic) || POINTER_GET (ic))
                return aop;
              if (ic->op == ADDRESS_OF)
                return aop;
              if (operandConflictsWithX (IC_LEFT (ic)))
                return aop;
              if (operandConflictsWithX (IC_RIGHT (ic)))
                return aop;
            }
          // TODO?
          /* It's safe to use tsx here. */
          if (!tsxUseful (ic))
            return aop;
          // transfer S to X
          doTSX();
        }
      return aop;
    }

  /* if it is in direct space */
  if (IN_DIRSPACE (space))
    {
      sym->aop = aop = newAsmop (AOP_DIR);
      aop->aopu.aop_dir = sym->rname;
      aop->size = getSize (sym->type);
      return aop;
    }

  /* default to far space */
  sym->aop = aop = newAsmop (AOP_EXT);
  aop->aopu.aop_dir = sym->rname;
  aop->size = getSize (sym->type);
  return aop;
}

/*-----------------------------------------------------------------*/
/* aopForRemat - rematerializes an object                          */
/*-----------------------------------------------------------------*/
static asmop *
aopForRemat (symbol * sym)
{
  iCode *ic = sym->rematiCode;
  asmop *aop = NULL;
  int val = 0;

  if (!ic)
    {
      fprintf (stderr, "Symbol %s to be rematerialized, but has no rematiCode.\n", sym->name);
      wassert (0);
    }

  for (;;)
    {
      if (ic->op == '+')
        val += (int) operandLitValue (IC_RIGHT (ic));
      else if (ic->op == '-')
        val -= (int) operandLitValue (IC_RIGHT (ic));
      else if (IS_CAST_ICODE (ic))
        {
          ic = OP_SYMBOL (IC_RIGHT (ic))->rematiCode;
          continue;
        }
      else
        break;

      ic = OP_SYMBOL (IC_LEFT (ic))->rematiCode;
    }

  if (ic->op == ADDRESS_OF)
    {
      if (val)
        {
          SNPRINTF (buffer, sizeof (buffer),
                    "(%s %c 0x%04x)", OP_SYMBOL (IC_LEFT (ic))->rname, val >= 0 ? '+' : '-', abs (val) & 0xffff);
        }
      else
        {
          strncpyz (buffer, OP_SYMBOL (IC_LEFT (ic))->rname, sizeof (buffer));
        }

      aop = newAsmop (AOP_IMMD);
      aop->aopu.aop_immd.aop_immd1 = Safe_strdup (buffer);
      /* set immd2 field if required */
    }
  else if (ic->op == '=')
    {
      val += (int) operandLitValue (IC_RIGHT (ic));
      val &= 0xffff;
      SNPRINTF (buffer, sizeof (buffer), "0x%04x", val);
      aop = newAsmop (AOP_LIT);
      aop->aopu.aop_lit = constVal (buffer);
    }
  else
    {
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "unexpected rematerialization");
    }

  return aop;
}

/*-----------------------------------------------------------------*/
/* regsInCommon - two operands have some registers in common       */
/*-----------------------------------------------------------------*/
static bool
regsInCommon (operand * op1, operand * op2)
{
  symbol *sym1, *sym2;
  int i;

  /* if they have registers in common */
  if (!IS_SYMOP (op1) || !IS_SYMOP (op2))
    return false;

  sym1 = OP_SYMBOL (op1);
  sym2 = OP_SYMBOL (op2);

  if (sym1->nRegs == 0 || sym2->nRegs == 0)
    return false;

  for (i = 0; i < sym1->nRegs; i++)
    {
      int j;
      if (!sym1->regs[i])
        continue;

      for (j = 0; j < sym2->nRegs; j++)
        {
          if (!sym2->regs[j])
            continue;

          if (sym2->regs[j] == sym1->regs[i])
            return true;
        }
    }

  return false;
}

/*-----------------------------------------------------------------*/
/* operandsEqu - equivalent                                        */
/*-----------------------------------------------------------------*/
static bool
operandsEqu (operand *op1, operand *op2)
{
  symbol *sym1, *sym2;

  /* if they not symbols */
  if (!IS_SYMOP (op1) || !IS_SYMOP (op2))
    return false;

  sym1 = OP_SYMBOL (op1);
  sym2 = OP_SYMBOL (op2);

  /* if both are itemps & one is spilt
     and the other is not then false */
  if (IS_ITEMP (op1) && IS_ITEMP (op2) && sym1->isspilt != sym2->isspilt)
    return false;

  /* if they are the same */
  if (sym1 == sym2)
    return true;

  /* if they have the same rname */
  if (sym1->rname[0] && sym2->rname[0] && strcmp (sym1->rname, sym2->rname) == 0)
    return true;

  /* if left is a tmp & right is not */
  if (IS_ITEMP (op1) && !IS_ITEMP (op2) && sym1->isspilt && (sym1->usl.spillLoc == sym2))
    return true;

  if (IS_ITEMP (op2) && !IS_ITEMP (op1) && sym2->isspilt && sym1->level > 0 && (sym2->usl.spillLoc == sym1))
    return true;

  return false;
}

/*-----------------------------------------------------------------*/
/* sameRegs - two asmops have the same registers                   */
/*-----------------------------------------------------------------*/
static bool
sameRegs (asmop * aop1, asmop * aop2)
{
  int i;

  if (aop1 == aop2)
    return true;

//  if (aop1->size != aop2->size)
//    return false;

  if (aop1->type == aop2->type)
    {
      switch (aop1->type)
        {
        case AOP_REG:
          for (i = 0; i < aop1->size; i++)
            if (aop1->aopu.aop_reg[i] != aop2->aopu.aop_reg[i])
              return false;
          return true;
        case AOP_SOF:
          return (aop1->aopu.aop_stk == aop2->aopu.aop_stk);
        case AOP_DIR:
//          if (regalloc_dry_run)
//            return false; // TODO: why?
        case AOP_EXT:
          return (!strcmp (aop1->aopu.aop_dir, aop2->aopu.aop_dir));
        }
    }

  return false;
}

/*-----------------------------------------------------------------*/
/* aopCanIncDec - asmop is EXT or DIR or X/Y                            */
/*-----------------------------------------------------------------*/
static bool
aopCanIncDec (asmop * aop)
{
  switch (aop->type) {
    case AOP_REG:
      if(aop->aopu.aop_reg[0]->rIdx == A_IDX) return false;
    case AOP_DIR:
    case AOP_EXT:
      return true;
  }
  return false;
}

/*-----------------------------------------------------------------*/
/* aopCanShift - asmop is EXT or DIR or A                          */
/*-----------------------------------------------------------------*/
static bool
aopCanShift (asmop * aop)
{
  switch (aop->type) {
    case AOP_REG:
      return ((aop->size == 1) && (aop->aopu.aop_reg[0]->rIdx == A_IDX));
    case AOP_DIR:
    case AOP_EXT:
      return true;
  }
  return false;
}

/*-----------------------------------------------------------------*/
/* aopOp - allocates an asmop for an operand  :                    */
/*-----------------------------------------------------------------*/
static void
aopOp (operand *op, iCode * ic, bool result)
{
  asmop *aop = NULL;
  symbol *sym;
  int i;

  emitComment (TRACE_AOP, __func__);

  if (!op)
    return;

  /* if already has an asmop */
  if (op->aop)
    return;

  // Is this a pointer set result?
  //
  if ((op == IC_RESULT (ic)) && POINTER_SET (ic))
    {
    }

//  printf("checking literal\n");
  /* if this a literal */
  if (IS_OP_LITERAL (op))
    {
      //op->aop = 
      aop = newAsmop (AOP_LIT);
      aop->aopu.aop_lit = OP_VALUE (op);
      aop->size = getSize (operandType (op));
      op->aop = aop;
      return;
    }


//  printf("checking underlying sym\n");
  /* if the underlying symbol has a aop */
  if (IS_SYMOP (op) && OP_SYMBOL (op)->aop)
    {
      op->aop = aop = Safe_calloc (1, sizeof (*aop));
      memcpy (aop, OP_SYMBOL (op)->aop, sizeof (*aop));
      //op->aop = aop = OP_SYMBOL (op)->aop;
      aop->size = getSize (operandType (op));
      //printf ("reusing underlying symbol %s\n",OP_SYMBOL (op)->name);
      //printf (" with size = %d\n", aop->size);

      aop->op = op;
      return;
    }

//  printf("checking true sym\n");
  /* if this is a true symbol */
  if (IS_TRUE_SYMOP (op))
    {
      op->aop = aop = aopForSym (ic, OP_SYMBOL (op), result);
      aop->op = op;
      //printf ("new symbol %s\n", OP_SYMBOL (op)->name);
      //printf (" with size = %d\n", aop->size);
      return;
    }

  /* this is a temporary : this has
     only five choices :
     a) register
     b) spillocation
     c) rematerialize
     d) conditional
     e) can be a return use only */

  if (!IS_SYMOP (op))
    piCode (ic, NULL);
  sym = OP_SYMBOL (op);

//  printf("checking conditional\n");
  /* if the type is a conditional */
  if (sym->regType == REG_CND)
    {
      sym->aop = op->aop = aop = newAsmop (AOP_CRY);
      aop->size = 0;
      aop->op = op;
      return;
    }

//  printf("checking spilt\n");
  /* if it is spilt then two situations
     a) is rematerialize
     b) has a spill location */
  if (sym->isspilt || sym->nRegs == 0)
    {
//      printf("checking remat\n");
      /* rematerialize it NOW */
      if (sym->remat)
        {
          sym->aop = op->aop = aop = aopForRemat (sym);
          aop->size = getSize (sym->type);
          aop->op = op;
          return;
        }

       wassertl (!sym->ruonly, "sym->ruonly not supported");

       if (regalloc_dry_run)     // Todo: Handle dummy iTemp correctly
        {
          if (options.stackAuto || (currFunc && IFFUNC_ISREENT (currFunc->type)))
            {
              sym->aop = op->aop = aop = newAsmop (AOP_SOF);
              aop->aopu.aop_stk = 8; /* bogus stack offset, high enough to prevent optimization */
            }
          else
            {
              sym->aop = op->aop = aop = newAsmop (AOP_DIR);
              aop->aopu.aop_dir = sym->name; //TODO? avoids crashing in sameRegs()
            }
          aop->size = getSize (sym->type);
          aop->op = op;
          return;
        }

      /* else spill location  */
      if (sym->isspilt && sym->usl.spillLoc || regalloc_dry_run)
        {
          asmop *oldAsmOp = NULL;

          if (sym->usl.spillLoc->aop && sym->usl.spillLoc->aop->size != getSize (sym->type))
            {
              /* force a new aop if sizes differ */
              oldAsmOp = sym->usl.spillLoc->aop;
              sym->usl.spillLoc->aop = NULL;
              //printf ("forcing new aop\n");
            }
          sym->aop = op->aop = aop = aopForSym (ic, sym->usl.spillLoc, result);
          if (sym->usl.spillLoc->aop->size != getSize (sym->type))
            {
              /* Don't reuse the new aop, go with the last one */
              sym->usl.spillLoc->aop = oldAsmOp;
            }
          aop->size = getSize (sym->type);
          aop->op = op;
          //printf ("spill symbol %s\n", OP_SYMBOL (op)->name);
          //printf (" with size = %d\n", aop->size);
          return;
        }

      /* else must be a dummy iTemp */
      sym->aop = op->aop = aop = newAsmop (AOP_DUMMY);
      aop->size = getSize (sym->type);
      aop->op = op;
      return;
    }

//  printf("assuming register\n");
  /* must be in a register */
  wassert (sym->nRegs);
  sym->aop = op->aop = aop = newAsmop (AOP_REG);
  aop->size = sym->nRegs;
  for (i = 0; i < sym->nRegs; i++)
    {
       wassert (sym->regs[i] < regsm6502 + 3);
       aop->aopu.aop_reg[i] = sym->regs[i];
       aop->regmask |= sym->regs[i]->mask;
    }
  if ((sym->nRegs > 1) && (sym->regs[0]->mask > sym->regs[1]->mask))
    aop->regmask |= M6502MASK_REV;
  aop->op = op;
}

/*-----------------------------------------------------------------*/
/* freeAsmop - free up the asmop given to an operand               */
/*----------------------------------------------------------------*/
static void
freeAsmop (operand * op, asmop * aaop, iCode * ic, bool pop)
{
  asmop *aop;

  if (!op)
    aop = aaop;
  else
    aop = op->aop;

  if (!aop)
    return;

  if (aop->freed)
    goto dealloc;

  aop->freed = 1;

  if (aop->stacked)
    {
      int stackAdjust;
      int loffset;

      emitComment (TRACE_AOP, "  freeAsmop restoring stacked %s", aopName (aop));
      aop->stacked = 0;
      stackAdjust = 0;
      for (loffset = 0; loffset < aop->size; loffset++)
        if (aop->stk_aop[loffset])
          {
            transferAopAop (aop->stk_aop[loffset], 0, aop, loffset);
            stackAdjust++;
          }
      pullNull (stackAdjust);
    }

dealloc:
  /* all other cases just dealloc */
  if (op)
    {
      op->aop = NULL;
      if (IS_SYMOP (op))
        {
          OP_SYMBOL (op)->aop = NULL;
          /* if the symbol has a spill */
          if (SPIL_LOC (op))
            SPIL_LOC (op)->aop = NULL;
        }
    }
}


/*-----------------------------------------------------------------*/
/* aopDerefAop - treating the aop parameter as a pointer, return   */
/*               an asmop for the object it references             */
/*-----------------------------------------------------------------*/
static asmop *
aopDerefAop (asmop * aop, int offset)
{
  int adr;
  asmop *newaop = NULL;
  sym_link *type, *etype;
  int p_type;
  struct dbuf_s dbuf;

  emitComment (TRACE_AOP, "      aopDerefAop(%s)", aopName (aop));
  if (aop->op)
    {

      type = operandType (aop->op);
      etype = getSpec (type);
      /* if op is of type of pointer then it is simple */
      if (IS_PTR (type) && !IS_FUNC (type->next))
        p_type = DCL_TYPE (type);
      else
        {
          /* we have to go by the storage class */
          p_type = PTR_TYPE (SPEC_OCLS (etype));
        }
    }
  else
    p_type = UPOINTER;

  switch (aop->type)
    {
    case AOP_IMMD:
      if (p_type == POINTER)
        newaop = newAsmop (AOP_DIR);
      else
        newaop = newAsmop (AOP_EXT);
      if (!offset)
        newaop->aopu.aop_dir = aop->aopu.aop_immd.aop_immd1;
      else
        {
          dbuf_init (&dbuf, 64);
          dbuf_printf (&dbuf, "(%s+%d)", aop->aopu.aop_immd.aop_immd1, offset);
          newaop->aopu.aop_dir = dbuf_detach_c_str (&dbuf);
        }
      break;
    case AOP_LIT:
      adr = (int) ulFromVal (aop->aopu.aop_lit);
      if (p_type == POINTER)
        adr &= 0xff;
      adr = (adr + offset) & 0xffff;
      dbuf_init (&dbuf, 64);

      if (adr < 0x100)
        {
          newaop = newAsmop (AOP_DIR);
          dbuf_printf (&dbuf, "0x%02x", adr);
        }
      else
        {
          newaop = newAsmop (AOP_EXT);
          dbuf_printf (&dbuf, "0x%04x", adr);
        }
      newaop->aopu.aop_dir = dbuf_detach_c_str (&dbuf);
      break;
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "unsupported asmop");
      return NULL;
    }


  return newaop;
}


/*-----------------------------------------------------------------*/
/* aopOpExtToIdx - attempt to convert AOP_EXT to AOP_IDX           */
/*-----------------------------------------------------------------*/
static void
aopOpExtToIdx(asmop * result, asmop *left, asmop *right)
{
  int accesses=0;
  int resultAccesses=0;
  int leftAccesses=0;
  int rightAccesses=0;
  asmop * winner;
  int winnerAccesses;
  
  return; //TODO: makes things worse often

  if (!m6502_reg_x->isFree || !m6502_reg_y->isFree)
    return;

  /* Need to replace at least two extended mode accesses with indexed */
  /* to break even with the extra cost of loading HX. Do a quick check */
  /* to see if anything is using extended mode at all. */
  if (result && result->type == AOP_EXT)
    accesses += result->size;
  if (left && left->type == AOP_EXT)
    accesses += left->size;
  if (right && right->type == AOP_EXT)
    accesses += right->size;
  if (accesses<2)
    return;
  
  /* If an operand is already using or going to H or X then we cannot */
  /* use indexed addressing mode at the same time. */
  if (result && (IS_AOP_WITH_Y (result) || IS_AOP_WITH_X (result)))
    return;
  if (left && (IS_AOP_WITH_Y (left) || IS_AOP_WITH_X (left)))
    return;
  if (right && (IS_AOP_WITH_Y (right) || IS_AOP_WITH_X (right)))
    return;

  /* Decide which is the best asmop to make indexed. */
  if (result && result->type == AOP_EXT)
    {
      resultAccesses = result->size;
      if (result->op && left && left->op && result->op->key == left->op->key)
        resultAccesses += result->size;
      if (result->op && right && right->op && result->op->key == right->op->key)
        resultAccesses += result->size;
    }
  if (left && left->type == AOP_EXT)
    {
      leftAccesses = left->size;
      if (left->op && right && right->op && left->op->key == right->op->key)
        leftAccesses += left->size;
    }
  if (right && right->type == AOP_EXT)
    {
      rightAccesses = right->size;
    }

  winner = result; winnerAccesses = resultAccesses;
  if (leftAccesses > winnerAccesses)
    {
      winnerAccesses = leftAccesses;
      winner = left;
    }
  if (rightAccesses > winnerAccesses)
    {
      winnerAccesses = rightAccesses;
      winner = right;
    }

  /* Make sure there were enough accesses of a single variable to be worthwhile. */ 
  if (winnerAccesses < 2)
    return;

  if (winner->op && result && result->op && winner->op->key == result->op->key)
    result->type = AOP_IDX;
  if (winner->op && left && left->op && winner->op->key == left->op->key)
    left->type = AOP_IDX;
  if (winner->op && right && right->op && winner->op->key == right->op->key)
    right->type = AOP_IDX;
  loadRegFromImm (m6502_reg_yx, winner->aopu.aop_dir);
}

// is it safe to aopAdrStr?
static bool
isAddrSafe(operand* op, reg_info* reg)
{
  switch (AOP(op)->type) 
  {
    case AOP_IMMD:	// #nn
    case AOP_LIT:
    case AOP_DIR:	// aa
    case AOP_EXT:	// aaaa
      return true;
    case AOP_SOF:       // (BASEPTR),y
      if (reg == m6502_reg_a && (m6502_reg_x->isFree || m6502_reg_y->isFree))
        return true;
  }
  
  return false;
}

static int aopPrepareStoreTemp = 0;
static int aopPreparePreserveFlags = 0;

// TODO: make sure this is called before/after aopAdrStr if indexing might be used
static void
aopAdrPrepare (asmop * aop, int loffset)
{
  aopPreparePreserveFlags = 0;
  if (loffset > (aop->size - 1))
    return;

  switch (aop->type)
    {
    case AOP_SOF:
        // can we get stack pointer?
        if (m6502_reg_x->isFree) {
          doTSX();
          aopPrepareStoreTemp=0;
        } else {
#if 0
          // code for lda [BASEPTR],y 
          aopPrepareStoreTemp = storeRegTemp(m6502_reg_y, false);
	  // FIXME: offset is wrong
          emitComment (TRACE_AOP, "ofs=%d base=%d tsx=%d push=%d stk=%d loffset=%d", _G.stackOfs, _G.baseStackPushes, _G.tsxStackPushes, _G.stackPushes, aop->aopu.aop_stk, loffset);
          loadRegFromConst(m6502_reg_y, _G.stackOfs + _G.baseStackPushes + aop->aopu.aop_stk + loffset + 1);
	  // ORIG: loadRegFromConst(m6502_reg_y, _G.stackOfs - _G.baseStackPushes + aop->aopu.aop_stk + loffset + 1);
          m6502_reg_y->aop = &tsxaop;
#else
          // FIXME: check if used/dead is ok
//	  aopPrepareStoreTemp = storeRegTempIfSurv(m6502_reg_x);
          storeRegTemp(m6502_reg_x, true);
          aopPrepareStoreTemp = true;
//          m6502_reg_x->isFree=true;
          doTSX();
#endif
          aopPreparePreserveFlags = 1; // TODO: also need to make sure flags are needed by caller
      }
    }
}

static void
aopAdrUnprepare (asmop * aop, int loffset)
{
  if (loffset > (aop->size - 1))
    return;

  switch (aop->type)
    {
    case AOP_SOF:
        if (aopPrepareStoreTemp) {
          if (aopPreparePreserveFlags) {
            emit6502op("php", ""); // TODO: sucks
          }
#if 0
          loadRegTemp(m6502_reg_y);
          m6502_dirtyReg(m6502_reg_y, false);
#else
	  loadRegTemp(m6502_reg_x);
#endif
	  if (aopPreparePreserveFlags) {
	    emit6502op("plp", ""); // TODO: sucks
          }
	  aopPreparePreserveFlags = 0;
	  aopPrepareStoreTemp = 0;
        }
      }
    }

/*-----------------------------------------------------------------*/
/* aopAdrStr - for referencing the address of the aop              */
/*-----------------------------------------------------------------*/
/* loffset seems to have a weird meaning here. It seems to be nonzero in some places where one would expect an offset to be zero */
static const char *
aopAdrStr (asmop * aop, int loffset, bool bit16)
{
  char *s = buffer;
  char *rs;
  int offset = loffset; // SEH: aop->size - 1 - loffset - (bit16 ? 1 : 0);
  int xofs;

  /* offset is greater than
     size then zero */
  if (loffset > (aop->size - 1) && aop->type != AOP_LIT)
    return "#0x00";
  
  /* depending on type */
  switch (aop->type)
    {
    case AOP_DUMMY:
      return "#0x00";

    case AOP_IMMD:
      if (loffset)
        {
          if (loffset > 1)
            sprintf (s, "#(%s >> %d)", aop->aopu.aop_immd.aop_immd1, loffset * 8);
          else
            sprintf (s, "#>%s", aop->aopu.aop_immd.aop_immd1);
        }
      else
        sprintf (s, "#%s", aop->aopu.aop_immd.aop_immd1);
      rs = Safe_calloc (1, strlen (s) + 1);
      strcpy (rs, s);
      return rs;

    case AOP_DIR:
      if (regalloc_dry_run)
        return "*dry";
      if (offset)
        sprintf (s, "*(%s + %d)", aop->aopu.aop_dir, offset);
      else
        sprintf (s, "*%s", aop->aopu.aop_dir);
      rs = Safe_calloc (1, strlen (s) + 1);
      strcpy (rs, s);
      return rs;

    case AOP_EXT:
      if (regalloc_dry_run)
        return "dry";
      if (offset)
        sprintf (s, "(%s + %d)", aop->aopu.aop_dir, offset);
      else
        sprintf (s, "%s", aop->aopu.aop_dir);
      rs = Safe_calloc (1, strlen (s) + 1);
      strcpy (rs, s);
      return rs;

    case AOP_REG:
      return aop->aopu.aop_reg[loffset]->name;

    case AOP_LIT:
      if (bit16)
        return aopLiteralLong (aop->aopu.aop_lit, loffset, 2);
      else
        return aopLiteral (aop->aopu.aop_lit, loffset);

    case AOP_SOF: // TODO?
      if (regalloc_dry_run) {
        return "1,x"; // fake result, not needed
      } else {
        // did we get stack pointer in X?
        if (m6502_reg_x->aop == &tsxaop) {
          // hc08's tsx returns +1, ours returns +0
          //DD( emitcode( "", "; %d + %d + %d + %d + 1", _G.stackOfs, _G.tsxStackPushes, aop->aopu.aop_stk, offset ));
          xofs = STACK_TOP + _G.stackOfs + _G.tsxStackPushes + aop->aopu.aop_stk + offset + 1;
          sprintf (s, "0x%x,x", xofs);
          rs = Safe_calloc (1, strlen (s) + 1);
          strcpy (rs, s);
          return rs;
        // did we get base ptr in Y?
        } else {
          // FIXME: should X be saved?
          doTSX();
          xofs = STACK_TOP + _G.stackOfs + _G.tsxStackPushes + aop->aopu.aop_stk + offset + 1;
          sprintf (s, "0x%x,x", xofs);
          rs = Safe_calloc (1, strlen (s) + 1);
          strcpy (rs, s);
          return rs;

        } 
#if 0
else if (m6502_reg_y->aop == &tsxaop) {
          return "[__BASEPTR],y";
        } else {
	  // FIXME: unimplemented
//          loadRegFromConst(m6502_reg_x, offset);
          return "ERROR [__BASEPTR],y"; // TODO: is base ptr or Y loaded?
        }
#endif
      }
    case AOP_IDX:
      xofs = offset; /* For now, assume yx points to the base address of operand */
      // TODO: slow
      storeRegTemp (m6502_reg_yx, true);
      if (m6502_reg_y->aop == &tsxaop) {
        loadRegFromConst(m6502_reg_y, offset);
        return "ERROR [__TEMP],y"; // TODO: what if != 0 tempOfs?
      } else
        return "ERROR"; // TODO: error
    }

  werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "aopAdrStr got unsupported aop->type");
  exit (1);
}


/*-----------------------------------------------------------------*/
/* getDataSize - get the operand data size                         */
/*-----------------------------------------------------------------*/
static int
getDataSize (operand *op)
{
  int size;
  size = AOP_SIZE (op);
  return size;
}

/*-----------------------------------------------------------------*/
/* asmopToBool - Emit code to convert an asmop to a boolean.       */
/*               Result left in A (0=false, 1=true) if ResultInA,  */
/*               otherwise result left in Z flag (1=false, 0=true) */
/*-----------------------------------------------------------------*/
static void
asmopToBool (asmop *aop, bool resultInA)
{
  bool isFloat; 
  symbol *tlbl, *tlbl1;
  int size = aop->size;
  bool needpula = false;
  bool flagsonly = true;
  int offset = size - 1;
  sym_link *type;

  emitComment (TRACE_AOP, "asmopToBool resultinA %s", resultInA?"yes":"no");

  wassert (aop);
  type = operandType (AOP_OP (aop));
  isFloat = IS_FLOAT (type);

  if (resultInA)
    m6502_freeReg (m6502_reg_a);

  if (IS_BOOL (type))
    {
      // result -> A
      if (resultInA)
        {
          loadRegFromAop (m6502_reg_a, aop, 0);
        }
      // result -> flags
      else if (IS_AOP_A(aop))
        {
          emit6502op ("cmp", "#0x00");
        }
      else if (IS_AOP_X(aop))
        {
          emit6502op ("cpx", "#0x00");
        }
      else if (IS_AOP_Y(aop))
        {
          emit6502op ("cpy", "#0x00");
        }
      else  // TODO: more cases? use transfer reg?
      {
        // ldx or ldy? or lda?
        reg_info* freereg = getDeadByteReg();
        if (freereg) {
          loadRegFromAop (freereg, aop, 0);
        } else {
          // no choice, all regs are full
          storeRegTemp (m6502_reg_a, true);
          loadRegFromAop (m6502_reg_a, aop, 0);
          loadRegTempNoFlags (m6502_reg_a, true); // TODO?
        }
      }
      return;
    }

  if (resultInA && size == 1)
    {
      loadRegFromAop (m6502_reg_a, aop, 0);
      emit6502op ("cmp", "#0x01");
      loadRegFromConst (m6502_reg_a, 0);
      rmwWithReg ("rol", m6502_reg_a);
      return;
    }

  switch (aop->type)
    {
    case AOP_REG:
      if (IS_AOP_A (aop))
        {
          emit6502op ("cmp", "#0x00");
          flagsonly = false; // because it's in A
        }
      else if (IS_AOP_X (aop))
        {
          emit6502op ("cpx", "#0x00");
        }
      else if (IS_AOP_Y (aop))
        {
          emit6502op ("cpy", "#0x00");
        }
      else if (IS_AOP_YX (aop))
        {
          symbol *tlbl = safeNewiTempLabel (NULL);
          emit6502op ("cpy", "#0x00");
          emit6502op ("bne", "%05d$", safeLabelKey2num (tlbl->key));
          emit6502op ("cpx", "#0x00");
          safeEmitLabel (tlbl);
        }
      else if (IS_AOP_XA (aop) || IS_AOP_AX (aop))
        {
          symbol *tlbl = safeNewiTempLabel (NULL);
          emit6502op ("cmp", "#0x00");
          emit6502op ("bne", "%05d$", safeLabelKey2num (tlbl->key));
          emit6502op ("cpx", "#0x00");
          safeEmitLabel (tlbl);
        }
      else
        {
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "Bad rIdx in asmopToBool");
          return;
        }
      break;
    case AOP_DIR:
    case AOP_EXT:
        emitComment (TRACE_AOP|VVDBG, "asmopToBool - AOP_DIR || AOP_EXT");

      if (!resultInA && (size == 1) && !IS_AOP_A (aop) && !m6502_reg_a->isFree && m6502_reg_x->isFree)
        {
          loadRegFromAop (m6502_reg_x, aop, 0);
          break;
        }
      if (resultInA)
        needpula = false;
      else
        needpula = storeRegTempIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, aop, offset--);
      if (isFloat)
        {
          emit6502op ("and", "#0x7F");      //clear sign bit
        }
      while (--size)
        accopWithAop ("ora", aop, offset--);
      if (needpula)
        {
          loadRegTempNoFlags (m6502_reg_a, true);
        }
      else
        {
          m6502_freeReg (m6502_reg_a);
          flagsonly = false;
        }
      break;
    case AOP_LIT:
      /* Higher levels should optimize this case away but let's be safe */
      if (ulFromVal (aop->aopu.aop_lit))
        loadRegFromConst (m6502_reg_a, 1);
      else
        loadRegFromConst (m6502_reg_a, 0);
      m6502_freeReg (m6502_reg_a);
      break;
    default:
      if (size == 1)
        {
          if (resultInA)
            {
              loadRegFromAop (m6502_reg_a, aop, 0);
              m6502_freeReg (m6502_reg_a);
              flagsonly = false;
            }
          else
            {
              bool needloada;
              emitComment (TRACE_AOP|VVDBG, "asmopToBool - loadreg");
              needloada = storeRegTempIfUsed (m6502_reg_a);
              loadRegFromAop (m6502_reg_a, aop, 0);
              loadRegTempNoFlags(m6502_reg_a,needloada);
            }
          break;
        }
      else if (size == 2)
        {
          if (m6502_reg_a->isFree)
            {
              loadRegFromAop (m6502_reg_a, aop, 0);
              accopWithAop ("ora", aop, 1);
              m6502_freeReg (m6502_reg_a);
              flagsonly = false;
            }
          else
            {
#if 0
              // FIXME: unimplemented
              tlbl = safeNewiTempLabel (NULL);
              emitcode ("bit3", "%s", aopAdrStr (aop, 0, false));
              emitcode ("bne", "%05d$", safeLabelKey2num (tlbl->key));
              emitcode ("bit4", "%s", aopAdrStr (aop, 1, false));
              safeEmitLabel (tlbl);
#endif
              emitComment(TRACE_AOP|VVDBG, "  asmopToBool default case");
              bool needloada = storeRegTempIfUsed (m6502_reg_a);
              bool needloadx = storeRegTempIfUsed (m6502_reg_x);
              m6502_freeReg(m6502_reg_x);
              emit6502op ("lda", aopAdrStr (aop, 0, false));
              emit6502op ("ora", aopAdrStr (aop, 1, false));
              emit6502op ("php", "" );
              loadOrFreeRegTemp (m6502_reg_x, needloadx);
              loadOrFreeRegTemp (m6502_reg_a, needloada);
              emit6502op ("plp", "" );

              break;
            }
        }
      else
        {
          needpula = storeRegTempIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, aop, offset--);
          if (isFloat)
            {
              emit6502op ("and", "#0x7F");
            }
          while (--size)
            accopWithAop ("ora", aop, offset--);
          if (needpula)
            loadRegTempNoFlags (m6502_reg_a, true);
          else
            {
              m6502_freeReg (m6502_reg_a);
              flagsonly = false;
            }
          // TODO: can we assume a=0?
        }
    }

  if (resultInA)
    {
      tlbl = safeNewiTempLabel (NULL);

      if (flagsonly)
        {
          tlbl1 = safeNewiTempLabel (NULL);
          emitBranch ("bne", tlbl1);
          loadRegFromConst (m6502_reg_a, 0);
          emitBranch ("bra", tlbl);
          safeEmitLabel (tlbl1);
          m6502_dirtyReg (m6502_reg_a);
          loadRegFromConst (m6502_reg_a, 1);
        }
      else
        {
          emitBranch ("beq", tlbl);
          loadRegFromConst (m6502_reg_a, 1);
        }
      safeEmitLabel (tlbl);
      m6502_dirtyReg (m6502_reg_a);
      m6502_useReg (m6502_reg_a);
    }
}

/*-----------------------------------------------------------------*/
/* genCopy - Copy the value from one operand to another            */
/*           The caller is responsible for aopOp and freeAsmop     */
/*-----------------------------------------------------------------*/
static void
genCopy (operand * result, operand * source)
{
  int size = AOP_SIZE (result);
  int srcsize = AOP_SIZE (source);
  int offset = 0;

  emitComment (TRACEGEN, __func__);
  emitComment (TRACEGEN|VVDBG, "      genCopy - size %d -> %d", srcsize, size);
  emitComment (TRACEGEN|VVDBG, "      genCopy - regmask %02x -> %02x", 
               AOP(source)->regmask, AOP(result)->regmask );

  /* if they are the same and not volatile */
  if (operandsEqu (result, source) && !isOperandVolatile (result, false) &&
      !isOperandVolatile (source, false))
    return;

  /* The source and destinations may be different size due to optimizations. */
  /* This is not a cast, so there is no need to worry about sign extension. */
  /* When this happens, it is usually just 1 byte source to 2 byte dest, so */
  /* nothing significant to optimize. */
  if (srcsize < size)
    {
      size -= srcsize;
      while (srcsize)
        {
          transferAopAop (AOP (source), offset, AOP (result), offset);
          offset++;
          srcsize--;
        }
      while (size)
        {
          storeConstToAop (0, AOP (result), offset);
          offset++;
          size--;
        }

      return;
    }

  /* if they are the same registers */
  if (sameRegs (AOP (source), AOP (result)))
    return;

  /* either source or result is two-byte register pair */
  if (IS_AOP_YX (AOP (result)) && srcsize == 2)
    {
      loadRegFromAop (m6502_reg_yx, AOP (source), 0);
      return;
    }
  if (IS_AOP_YX (AOP (source)) && size == 2)
    {
      storeRegToAop (m6502_reg_yx, AOP (result), 0);
      return;
    }
  if (IS_AOP_XA (AOP (result)) && srcsize == 2)
    {
      loadRegFromAop (m6502_reg_xa, AOP (source), 0);
      return;
    }
  if (IS_AOP_XA (AOP (source)) && size == 2)
    {
      storeRegToAop (m6502_reg_xa, AOP (result), 0);
      return;
    }

  // TODO?
  //  if (IS_MOS6502 && (size > 2))
  //    aopOpExtToIdx (AOP (result), NULL, AOP (source));

  /* general case */
  // TODO: sucks for copying registers
  emitComment (TRACEGEN|VVDBG, "      genCopy (general case)", "");
  for (offset=0; offset<size; offset++) {
    transferAopAop( AOP(source), offset, AOP(result), offset);
  }
}

/*-----------------------------------------------------------------*/
/* genNot - generate code for ! operation                          */
/*-----------------------------------------------------------------*/
static void
genNot (iCode * ic)
{
  bool needpulla;

  emitComment (TRACEGEN, __func__);

  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, false);
  aopOp (IC_RESULT (ic), ic, true);
  needpulla = pushRegIfSurv (m6502_reg_a);
  asmopToBool (AOP (IC_LEFT (ic)), true);

  emit6502op ("eor", "#0x01");
  storeRegToFullAop (m6502_reg_a, AOP (IC_RESULT (ic)), false);
  pullOrFreeReg (m6502_reg_a, needpulla);

  freeAsmop (IC_RESULT (ic), NULL, ic, true);
  freeAsmop (IC_LEFT (ic), NULL, ic, true);
}


/*-----------------------------------------------------------------*/
/* genCpl - generate code for complement                           */
/*-----------------------------------------------------------------*/
static void
genCpl (iCode * ic)
{
  int offset = 0;
  int size;
  reg_info *reg;
  bool needpullreg;

  emitComment (TRACEGEN, __func__);

  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, false);
  aopOp (IC_RESULT (ic), ic, true);
  size = AOP_SIZE (IC_RESULT (ic));

  // TODO: use sameRegs?
  if(AOP_TYPE (IC_LEFT (ic)) == AOP_REG && AOP_TYPE (IC_RESULT (ic)) == AOP_REG &&
    AOP (IC_RESULT (ic))->aopu.aop_reg[0] == AOP (IC_LEFT (ic))->aopu.aop_reg[0] &&
    (size < 2 || AOP (IC_RESULT (ic))->aopu.aop_reg[1] == AOP (IC_LEFT (ic))->aopu.aop_reg[1]))
    {
      while (size--)
        rmwWithReg ("com", AOP (IC_RESULT (ic))->aopu.aop_reg[offset++]);
      goto release;
    }

  if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP_TYPE (IC_LEFT (ic)) == AOP_REG)
    {
      while (size--)
        {
          if ((reg = AOP (IC_RESULT (ic))->aopu.aop_reg[offset]) != m6502_reg_y)
            {
              transferAopAop (AOP (IC_LEFT (ic)), offset, AOP (IC_RESULT (ic)), offset);
              rmwWithReg ("com", reg);
            }
          else
            {
              if ((reg = AOP (IC_LEFT (ic))->aopu.aop_reg[offset]) == m6502_reg_y)
                reg = m6502_reg_a->isDead ? m6502_reg_a : m6502_reg_x;
              needpullreg = pushRegIfSurv (reg);
              loadRegFromAop (reg, AOP (IC_LEFT (ic)), offset);
              rmwWithReg ("com", reg);
              storeRegToAop (reg, AOP (IC_RESULT (ic)), offset);
              if (needpullreg)
                pullReg (reg);
            }
          offset++;
        }
      goto release;
    }

  reg = (m6502_reg_a->isDead && !(AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP (IC_RESULT (ic))->aopu.aop_reg[0] == m6502_reg_a) ? m6502_reg_a : m6502_reg_x);

  needpullreg = pushRegIfSurv (reg);
  while (size--)
    {
      bool needpullreg2 = (!size && AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP (IC_RESULT (ic))->aopu.aop_reg[0] == reg || size && AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP (IC_RESULT (ic))->aopu.aop_reg[1] == reg);
      if (needpullreg2)
        pushReg (reg, true);
      loadRegFromAop (reg, AOP (IC_LEFT (ic)), offset);
      rmwWithReg ("com", reg);
      m6502_useReg (reg);
      storeRegToAop (reg, AOP (IC_RESULT (ic)), offset);
      m6502_freeReg (reg);
      if (needpullreg2)
        pullReg (reg);
      offset++;
    }
  pullOrFreeReg (reg, needpullreg);

  /* release the aops */
release:
  freeAsmop (IC_RESULT (ic), NULL, ic, true);
  freeAsmop (IC_LEFT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genUminusFloat - unary minus for floating points                */
/*-----------------------------------------------------------------*/
static void
genUminusFloat (operand * op, operand * result)
{
  int size, offset = 0;
  bool needpula;

  emitComment (TRACEGEN, __func__);

  /* for this we just copy and then flip the bit */

  size = AOP_SIZE (op) - 1;

  while (size--)
    {
      transferAopAop (AOP (op), offset, AOP (result), offset);
      offset++;
    }

  needpula = pushRegIfSurv (m6502_reg_a);
  loadRegFromAop (m6502_reg_a, AOP (op), offset);
  emit6502op ("eor", "#0x80");
  m6502_useReg (m6502_reg_a);
  storeRegToAop (m6502_reg_a, AOP (result), offset);
  pullOrFreeReg (m6502_reg_a, needpula);
}

/*-----------------------------------------------------------------*/
/* genUminus - unary minus code generation                         */
/*-----------------------------------------------------------------*/
static void
genUminus (iCode * ic)
{
  int offset, size;
  sym_link *optype;
  bool carry = true;
  bool needpula;
  asmop *result;

  emitComment (TRACEGEN, __func__);

  /* assign asmops */
  aopOp (IC_LEFT (ic), ic, false);
  aopOp (IC_RESULT (ic), ic, true);

  optype = operandType (IC_LEFT (ic));

  /* if float then do float stuff */
  if (IS_FLOAT (optype))
    {
      genUminusFloat (IC_LEFT (ic), IC_RESULT (ic));
      goto release;
    }

  /* otherwise subtract from zero */
  size = AOP_SIZE (IC_LEFT (ic));
  offset = 0;

  if (size == 1)
    {
      needpula = pushRegIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (ic)), 0);
      rmwWithReg ("neg", m6502_reg_a);
      m6502_freeReg (m6502_reg_a);
      storeRegToFullAop (m6502_reg_a, AOP (IC_RESULT (ic)), SPEC_USIGN (operandType (IC_LEFT (ic))));
      pullOrFreeReg (m6502_reg_a, needpula);
      goto release;
    }

  /* If either left or result are in registers, handle this carefully to     */
  /* avoid prematurely overwriting register values. The 1 byte case was      */
  /* handled above and there aren't enough registers to handle 4 byte values */
  /* so this case only needs to deal with 2 byte values. */
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG || AOP_TYPE (IC_LEFT (ic)) == AOP_REG)
    {
      reg_info *result0 = NULL;
      reg_info *left0 = NULL;
      reg_info *left1 = NULL;
      if (AOP_TYPE (IC_LEFT (ic)) == AOP_REG)
        {
          left0 = AOP (IC_LEFT (ic))->aopu.aop_reg[0];
          left1 = AOP (IC_LEFT (ic))->aopu.aop_reg[1];
        }
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG)
        {
          result0 = AOP (IC_RESULT (ic))->aopu.aop_reg[0];
        }
      needpula = pushRegIfSurv (m6502_reg_a);
      if (left1 == m6502_reg_a)
        pushReg (left1, true);

      if (left0 == m6502_reg_a) // TODO?
        rmwWithReg ("neg", m6502_reg_a);
      else
        {
          loadRegFromConst (m6502_reg_a, 0);
          emit6502op("sec", "");
          accopWithAop ("sbc", AOP (IC_LEFT (ic)), 0);
        }
      if (result0 == m6502_reg_a || (result0 && result0 == left1))
        pushReg (m6502_reg_a, true);
      else
        storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), 0);
      loadRegFromConst (m6502_reg_a, 0);
      if (left1 == m6502_reg_a)
        {
	  // FIXME: unimplemented
          emitcode ("sbc1","%d,s", (result0 == m6502_reg_a || (result0 && result0 == left1)) ? 2 : 1);
          m6502_dirtyReg (m6502_reg_a);
        }
      else
        {
          accopWithAop ("sbc", AOP (IC_LEFT (ic)), 1);
        }
      storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), 1);
      if (result0 == m6502_reg_a || (result0 && result0 == left1))
        pullReg (result0);
      if (left1 == m6502_reg_a)
        pullNull (1);
      pullOrFreeReg (m6502_reg_a, needpula);
      goto release;
    }

  result = AOP (IC_RESULT (ic));

  needpula = pushRegIfSurv (m6502_reg_a);
  while (size--)
    {
      loadRegFromConst (m6502_reg_a, 0);
      if (carry) {
        emit6502op("sec", "");
      }
      accopWithAop ("sbc", AOP (IC_LEFT (ic)), offset);
      storeRegToAop (m6502_reg_a, result, offset++);
      carry = false;
    }
  storeRegSignToUpperAop (m6502_reg_a, result, offset, SPEC_USIGN (operandType (IC_LEFT (ic))));
  pullOrFreeReg (m6502_reg_a, needpula);

release:
  /* release the aops */
  freeAsmop (IC_RESULT (ic), NULL, ic, true);
  freeAsmop (IC_LEFT (ic), NULL, ic, false);
}

/*-----------------------------------------------------------------*/
/* saveRegisters - will look for a call and save the registers     */
/*-----------------------------------------------------------------*/
static void
saveRegisters (iCode *lic)
{
  int i;
  iCode *ic;

  /* look for call */
  for (ic = lic; ic; ic = ic->next)
    if (ic->op == CALL || ic->op == PCALL)
      break;

  if (!ic)
    {
      fprintf (stderr, "found parameter push with no function call\n");
      return;
    }

  /* if the registers have been saved already or don't need to be then
     do nothing */
  if (ic->regsSaved)
    return;
  if (IS_SYMOP (IC_LEFT (ic)) &&
      (IFFUNC_CALLEESAVES (OP_SYMBOL (IC_LEFT (ic))->type) || IFFUNC_ISNAKED (OP_SYM_TYPE (IC_LEFT (ic)))))
    return;

  if (!regalloc_dry_run)
    ic->regsSaved = 1;

  emitComment (REGOPS, "  saveRegisters");
  
  // make sure not to clobber A
  // TODO: why does isUsed not set?
  // TODO: only clobbered if m6502_reg_a->isFree
  // TODO: 65C02
  bool clobbers_a = !IS_MOS65C02 
    && (bitVectBitValue(ic->rSurv, X_IDX) || bitVectBitValue(ic->rSurv, Y_IDX))
    && !bitVectBitValue(ic->rSurv, A_IDX);
  if (clobbers_a)
    storeRegTemp (m6502_reg_a, true);
  for (i = A_IDX; i <= Y_IDX; i++)
    {
      if (bitVectBitValue (ic->rSurv, i))
        pushReg (m6502_regWithIdx (i), false);
    }
  if (clobbers_a)
    loadRegTemp (m6502_reg_a);
}

/*-----------------------------------------------------------------*/
/* unsaveRegisters - pop the pushed registers                      */
/*-----------------------------------------------------------------*/
static void
unsaveRegisters (iCode *ic)
{
  int i;

  emitComment (REGOPS, "; unsaveRegisters");

  // TODO: only clobbered if m6502_reg_a->isFree
  // TODO: 65C02
  bool clobbers_a = !IS_MOS65C02 
    && (bitVectBitValue(ic->rSurv, X_IDX) || bitVectBitValue(ic->rSurv, Y_IDX))
    && !bitVectBitValue(ic->rSurv, A_IDX);
  if (clobbers_a)
    storeRegTemp (m6502_reg_a, true);
  for (i = Y_IDX; i >= A_IDX; i--)
    {
      if (bitVectBitValue (ic->rSurv, i))
        pullReg (m6502_regWithIdx (i));
    }
  if (clobbers_a)
    loadRegTemp (m6502_reg_a);
}

/*-----------------------------------------------------------------*/
/* pushSide -                                                      */
/*-----------------------------------------------------------------*/
static void
pushSide (operand *oper, int size, iCode *ic)
{
  int offset = 0;
  bool xIsFree = m6502_reg_x->isFree;

  m6502_useReg (m6502_reg_x);
  aopOp (oper, ic, false);

  if (AOP_TYPE (oper) == AOP_REG)
    {
      /* The operand is in registers; we can push them directly */
      for (offset=size-1; offset>=0; offset--) 
        {
          pushReg (AOP (oper)->aopu.aop_reg[offset], true);
        }
    }
  else
    {
      // push A if not free
      // TODO: consider other regs for 65C02
      bool needloada = storeRegTempIfUsed(m6502_reg_a);
      /* A is free, so piecewise load operand into a and push A */
      for (offset=size-1; offset>=0; offset--) 
        {
          loadRegFromAop (m6502_reg_a, AOP (oper), offset);
          pushReg (m6502_reg_a, true);
        }
      loadOrFreeRegTemp(m6502_reg_a, needloada);
    }

  freeAsmop (oper, NULL, ic, true);
  if (xIsFree)
    m6502_freeReg (m6502_reg_x);
}

/*-----------------------------------------------------------------*/
/* assignResultValue -                                             */
/*-----------------------------------------------------------------*/
static void
assignResultValue (operand * oper)
{
  int size = AOP_SIZE (oper);
  int offset = 0;
  bool delayed_x = false;

  emitComment (TRACEGEN, __func__);

  while (size--)
    {
      if (!offset && AOP_TYPE (oper) == AOP_REG && AOP_SIZE (oper) > 1 && AOP (oper)->aopu.aop_reg[0]->rIdx == X_IDX)
        {
          storeRegTemp (m6502_reg_a, true);
          delayed_x = true;
        }
      else
        transferAopAop (m6502_aop_pass[offset], 0, AOP (oper), offset);
      if (m6502_aop_pass[offset]->type == AOP_REG)
        m6502_freeReg (m6502_aop_pass[offset]->aopu.aop_reg[0]);
      offset++;
    }
  if (delayed_x)
    loadRegTemp (m6502_reg_x);
}



/*-----------------------------------------------------------------*/
/* genIpush - generate code for pushing this gets a little complex */
/*-----------------------------------------------------------------*/
static void
genIpush (iCode * ic)
{
  int size, offset = 0;

  emitComment (TRACEGEN, __func__);

  /* if this is not a parm push : ie. it is spill push
     and spill push is always done on the local stack */
  if (!ic->parmPush)
    {
      /* and the item is spilt then do nothing */
      if (OP_SYMBOL (IC_LEFT (ic))->isspilt)
        return;

      aopOp (IC_LEFT (ic), ic, false);
      size = AOP_SIZE (IC_LEFT (ic));
      /* push it on the stack */
      for (offset=size-1; offset>=0; offset--)
        {
          loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (ic)), offset);
          pushReg (m6502_reg_a, true);
        }
      return;
    }

  /* this is a parameter push: in this case we call
     the routine to find the call and save those
     registers that need to be saved */
  if (!regalloc_dry_run) /* Cost for saving registers is counted at CALL or PCALL */
    saveRegisters (ic);

  /* then do the push */
  aopOp (IC_LEFT (ic), ic, false);

  // pushSide(IC_LEFT(ic), AOP_SIZE(IC_LEFT(ic)));
  size = AOP_SIZE (IC_LEFT (ic));

//  l = aopGet (AOP (IC_LEFT (ic)), 0, false, true);
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_IMMD || AOP_TYPE (IC_LEFT (ic)) == AOP_LIT ||IS_AOP_YX (AOP (IC_LEFT (ic))))
    {
      if ((size == 2) && m6502_reg_yx->isDead || IS_AOP_YX (AOP (IC_LEFT (ic))))
        {
          loadRegFromAop (m6502_reg_yx, AOP (IC_LEFT (ic)), 0);
          pushReg (m6502_reg_yx, true);
          goto release;
        }
    }

  if (AOP_TYPE (IC_LEFT (ic)) == AOP_REG)
    {
      for (offset=size-1; offset>=0; offset--)
        pushReg (AOP (IC_LEFT (ic))->aopu.aop_reg[offset], true);
      goto release;
    }

  for (offset=size-1; offset>=0; offset--)
    {
//      printf("loading %d\n", offset);
      loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (ic)), offset);
//      printf("pushing \n");
      pushReg (m6502_reg_a, true);
    }

release:
  freeAsmop (IC_LEFT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genIpop - recover the registers: can happen only for spilling   */
/*-----------------------------------------------------------------*/
static void
genIpop (iCode * ic)
{
  int size, offset;

  emitComment (TRACEGEN, __func__);

  /* if the temp was not pushed then */
  if (OP_SYMBOL (IC_LEFT (ic))->isspilt)
    return;

  aopOp (IC_LEFT (ic), ic, false);
  size = AOP_SIZE (IC_LEFT (ic));
  offset = size - 1;
  while (size--)
    {
      pullReg (m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (IC_LEFT (ic)), offset--);
    }

  freeAsmop (IC_LEFT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genSend - gen code for SEND                                     */
/*-----------------------------------------------------------------*/
static void
genSend (set *sendSet)
{
  iCode *send1;
  iCode *send2;

  emitComment (TRACEGEN, __func__);

  /* case 1: single parameter in A
   * case 2: single parameter in XA
   * case 3: first parameter in A, second parameter in X
   */
  send1 = setFirstItem (sendSet);
  send2 = setNextItem (sendSet);

  if (!send2)
    {
      int size;
      /* case 1 or 2, this is fairly easy */
      aopOp (IC_LEFT (send1), send1, false);
      size = AOP_SIZE (IC_LEFT (send1));
      wassert (size <= 2);
      if (size == 1)
        {
          loadRegFromAop (send1->argreg == 2 ? m6502_reg_x : m6502_reg_a, AOP (IC_LEFT (send1)), 0);
        }
      else if (AOP (IC_LEFT (send1))->type == AOP_REG)
        loadRegFromAop (m6502_reg_xa, AOP (IC_LEFT (send1)), 0);
      else if (isOperandVolatile (IC_LEFT (send1), false))
        {
          /* use lsb to msb order for volatile operands */
          loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (send1)), 0);
          loadRegFromAop (m6502_reg_x, AOP (IC_LEFT (send1)), 1);
        }
      else
        {
          /* otherwise perfer to load a last */
          loadRegFromAop (m6502_reg_x, AOP (IC_LEFT (send1)), 1);
          loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (send1)), 0);

        }
      freeAsmop (IC_LEFT (send1), NULL, send1, true);
    }
  else
    {
      /* case 3 */
      /* make sure send1 is the first argument and swap with send2 if not */
      if (send1->argreg > send2->argreg)
        {
          iCode * sic = send1;
          send1 = send2;
          send2 = sic;
        }
      aopOp (IC_LEFT (send1), send1, false);
      aopOp (IC_LEFT (send2), send2, false);
      if (IS_AOP_X (AOP (IC_LEFT (send1))) && IS_AOP_A (AOP (IC_LEFT (send2))))
        {
          /* If the parameters' register assignment is exactly backwards */
          /* from what is needed, then swap the registers. */
          swapXA();
        }
      else if (IS_AOP_A (AOP (IC_LEFT (send2))))
        {
          loadRegFromAop (m6502_reg_x, AOP (IC_LEFT (send2)), 0);
          loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (send1)), 0);
        }
      else
        {
          loadRegFromAop (m6502_reg_a, AOP (IC_LEFT (send1)), 0);
          loadRegFromAop (m6502_reg_x, AOP (IC_LEFT (send2)), 0);
        }
      freeAsmop (IC_LEFT (send2), NULL, send2, true);
      freeAsmop (IC_LEFT (send1), NULL, send1, true);
    }
}

/*-----------------------------------------------------------------*/
/* genCall - generates a call statement                            */
/*-----------------------------------------------------------------*/
static void
genCall (iCode * ic)
{
  sym_link *dtype;
  sym_link *etype;
//  bool restoreBank = false;
//  bool swapBanks = false;

  emitComment (TRACEGEN, __func__);

  /* if caller saves & we have not saved then */
  if (!ic->regsSaved)
    saveRegisters (ic);

  dtype = operandType (IC_LEFT (ic));
  etype = getSpec (dtype);
  /* if send set is not empty then assign */
  if (_G.sendSet && !regalloc_dry_run)
    {
      if (IFFUNC_ISREENT (dtype))       /* need to reverse the send set */
        {
	  //genSend (_G.sendSet);
          genSend (reverseSet (_G.sendSet));
        }
      else
        {
          genSend (_G.sendSet);
        }
      _G.sendSet = NULL;
    }

  /* make the call */
  if (IS_LITERAL (etype))
    {
      emit6502op ("jsr", "0x%04X", ulFromVal (OP_VALUE (IC_LEFT (ic))));
    }
  else
    {
      bool jump = (!ic->parmBytes && IFFUNC_ISNORETURN (OP_SYMBOL (IC_LEFT (ic))->type));

      emit6502op (jump ? "jmp" : "jsr", "%s", (OP_SYMBOL (IC_LEFT (ic))->rname[0] ?
                              OP_SYMBOL (IC_LEFT (ic))->rname : OP_SYMBOL (IC_LEFT (ic))->name));
    }

  m6502_dirtyReg (m6502_reg_a);
  m6502_dirtyReg (m6502_reg_x);
  m6502_dirtyReg (m6502_reg_y);

  /* do we need to recompute the base ptr? */
  if (_G.funcHasBasePtr) {
    saveBasePtr();
  }
  
  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs || OP_SYMBOL (IC_RESULT (ic))->spildir)) || IS_TRUE_SYMOP (IC_RESULT (ic)))
    {
      m6502_useReg (m6502_reg_a);
      if (operandSize (IC_RESULT (ic)) > 1)
        m6502_useReg (m6502_reg_x);
      aopOp (IC_RESULT (ic), ic, false);

      assignResultValue (IC_RESULT (ic));

      freeAsmop (IC_RESULT (ic), NULL, ic, true);
    }

  /* adjust the stack for parameters if required */
  if (ic->parmBytes)
    {
      pullNull (ic->parmBytes);
    }

  /* if we had saved some registers then unsave them */
  if (ic->regsSaved && !IFFUNC_CALLEESAVES (dtype))
    unsaveRegisters (ic);
}

/*-----------------------------------------------------------------*/
/* genPcall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void
genPcall (iCode * ic)
{
  sym_link *dtype;
  sym_link *etype;
  symbol *rlbl = safeNewiTempLabel (NULL);
  symbol *tlbl = safeNewiTempLabel (NULL);
  iCode * sendic;

  emitComment (TRACEGEN, __func__);

  dtype = operandType (IC_LEFT (ic))->next;
  etype = getSpec (dtype);
  /* if caller saves & we have not saved then */
  if (!ic->regsSaved)
    saveRegisters (ic);

  /* Go through the send set and mark any registers used by iTemps as */
  /* in use so we don't clobber them while setting up the return address */
  for (sendic = setFirstItem (_G.sendSet); sendic; sendic = setNextItem (_G.sendSet))
    {
      updateiTempRegisterUse (IC_LEFT (sendic));
    }

  // TODO: handle DIR/EXT with jmp [aa] or jmp [aaaa]
  
  if (!IS_LITERAL (etype))
    {
      /* push the return address on to the stack */
      emitBranch ("jsr", tlbl);
      emitBranch ("bra", rlbl);
      safeEmitLabel (tlbl);
      _G.stackPushes += 2;          /* account for the bsr return address now on stack */
      updateCFA ();
      /* now push the function address */
      pushSide (IC_LEFT (ic), FARPTRSIZE, ic); // -1 is baked into initialization

      /* load into TEMP0-1 */
      /*
      asmop *tempaop = newAsmop (AOP_DIR);
      tempaop->size = 2;
      tempaop->aopu.aop_dir = "__TEMP";
      transferAopAop( AOP(IC_LEFT(ic)), 0, tempaop, 0 );
      transferAopAop( AOP(IC_LEFT(ic)), 1, tempaop, 1 );
      _G.tempOfs += 2;
      */
    }

  /* if send set is not empty then assign */
  if (_G.sendSet && !regalloc_dry_run)
    {
      genSend (reverseSet (_G.sendSet));
      _G.sendSet = NULL;
    }

  /* make the call */
  if (!IS_LITERAL (etype))
    {
      emit6502op("rts", "");
      //emitcode("jmp", "[__TEMP]");
      //_G.tempOfs -= 2;

      safeEmitLabel (rlbl);
      _G.stackPushes -= 4;          /* account for rts here & in called function */
      updateCFA ();
    }
  else
    {
      emit6502op ("jsr", "0x%04X", ulFromVal (OP_VALUE (IC_LEFT (ic))));
    }

  m6502_dirtyReg (m6502_reg_a);
  m6502_dirtyReg (m6502_reg_x);
  m6502_dirtyReg (m6502_reg_y);

  /* do we need to recompute the base ptr? */
  if (_G.funcHasBasePtr) {
    saveBasePtr();
  }

  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs || OP_SYMBOL (IC_RESULT (ic))->spildir)) || IS_TRUE_SYMOP (IC_RESULT (ic)))
    {
      m6502_useReg (m6502_reg_a);
      if (operandSize (IC_RESULT (ic)) > 1)
        m6502_useReg (m6502_reg_x);
      aopOp (IC_RESULT (ic), ic, false);

      assignResultValue (IC_RESULT (ic));

      freeAsmop (IC_RESULT (ic), NULL, ic, true);
    }

  /* adjust the stack for parameters if required */
  if (ic->parmBytes)
    {
      pullNull (ic->parmBytes);
    }

  /* if we had saved some registers then unsave them */
  if (ic->regsSaved && !IFFUNC_CALLEESAVES (dtype))
    unsaveRegisters (ic);
}

/*-----------------------------------------------------------------*/
/* resultRemat - result  is rematerializable                       */
/*-----------------------------------------------------------------*/
static int
resultRemat (iCode * ic)
{
  if (SKIP_IC (ic) || ic->op == IFX)
    return 0;

  if (IC_RESULT (ic) && IS_ITEMP (IC_RESULT (ic)))
    {
      symbol *sym = OP_SYMBOL (IC_RESULT (ic));
      if (sym->remat && !POINTER_SET (ic))
        return 1;
    }

  return 0;
}

/*-----------------------------------------------------------------*/
/* inExcludeList - return 1 if the string is in exclude Reg list   */
/*-----------------------------------------------------------------*/
static int
regsCmp (void *p1, void *p2)
{
  return (STRCASECMP ((char *) p1, (char *) (p2)) == 0);
}

static bool
inExcludeList (char *s)
{
  const char *p = setFirstItem (options.excludeRegsSet);

  if (p == NULL || STRCASECMP (p, "none") == 0)
    return false;


  return isinSetWith (options.excludeRegsSet, s, regsCmp);
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void
genFunction (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));
  sym_link *ftype;
  iCode *ric = (ic->next && ic->next->op == RECEIVE) ? ic->next : NULL;
  int stackAdjust = sym->stack;
  //  int accIsFree = sym->recvSize == 0;

  /* create the function header */
  emitComment (ALWAYS, "-----------------------------------------");
  emitComment (ALWAYS, " function %s", sym->name);
  emitComment (ALWAYS, "-----------------------------------------");
  emitComment (ALWAYS, m6502_assignment_optimal ? "Register assignment is optimal." : "Register assignment might be sub-optimal.");
  emitComment (ALWAYS, "Stack space usage: %d bytes.", sym->stack);

  emitcode ("", "%s:", sym->rname);
  genLine.lineCurr->isLabel = 1;
  ftype = operandType (IC_LEFT (ic));

  _G.stackOfs = 0;
  _G.stackPushes = 0;
  if (options.debug && !regalloc_dry_run)
    debugFile->writeFrameAddress (NULL, m6502_reg_sp, 0);

  if (IFFUNC_ISNAKED (ftype))
    {
      emitComment (ALWAYS, "naked function: no prologue.");
      return;
    }

  /* if this is an interrupt service routine then
     save h  */
  if (IFFUNC_ISISR (sym->type))
    {
      if (!inExcludeList ("y"))
        pushReg (m6502_reg_y, false);
    }

  /* For some cases it is worthwhile to perform a RECEIVE iCode */
  /* before setting up the stack frame completely. */
  int numStackParams = 0;
  while (ric && ric->next && ric->next->op == RECEIVE)
    ric = ric->next;
  while (ric && IC_RESULT (ric))
    {
      symbol *rsym = OP_SYMBOL (IC_RESULT (ric));
      int rsymSize = rsym ? getSize (rsym->type) : 0;

      if (rsym->isitmp)
        {
          if (rsym && rsym->regType == REG_CND)
            rsym = NULL;
          if (rsym && (/*rsym->accuse ||*/ rsym->ruonly))
            rsym = NULL;
          if (rsym && (rsym->isspilt || rsym->nRegs == 0) && rsym->usl.spillLoc)
            rsym = rsym->usl.spillLoc;
        }

      /* If the RECEIVE operand immediately spills to the first entry on the  */
      /* stack, we can push it directly rather than use an sp relative store. */
      if (rsym && rsym->onStack && rsym->stack == -_G.stackPushes - rsymSize)
        {
          int ofs;

          genLine.lineElement.ic = ric;
          emitComment (TRACEGEN, "genReceive", "size=%d", rsymSize);
	  //          for (ofs = 0; ofs < rsymSize; ofs++)
          m6502_reg_a->isFree=false;
	  for (ofs = rsymSize-1; ofs >=0; ofs--)
            {
              reg_info *reg = m6502_aop_pass[ofs + (ric->argreg - 1)]->aopu.aop_reg[0];
	      emitComment (TRACEGEN, "pushreg", "ofs=%d", ofs);
              pushReg (reg, true);
	      //              if (reg->rIdx == A_IDX)
	      //                accIsFree = 1;
              stackAdjust--;
            }
          genLine.lineElement.ic = ic;
          ric->generated = 1;
        }
      ric = (ric->prev && ric->prev->op == RECEIVE) ? ric->prev : NULL;
    }

  /* adjust the stack for the function */
  if (stackAdjust)
    {
      adjustStack (-stackAdjust);
    }
  _G.stackOfs = sym->stack;
  _G.stackPushes = 0;
  _G.funcHasBasePtr = 0;
  // TODO: how to see if needed? how to count params?
  if ( stackAdjust || sym->stack || numStackParams || IFFUNC_ISREENT(sym->type) )
    {
      saveBasePtr();
      _G.funcHasBasePtr = 1;
    }

  /* if critical function then turn interrupts off */
  if (IFFUNC_ISCRITICAL (ftype))
    {
      emit6502op ("php", "");
      emit6502op ("sei", "");
    }
}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void
genEndFunction (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));

  emitComment (TRACEGEN, __func__);
  emitComment (REGOPS, "  %s %s", __func__, regInfoStr() );


  if (IFFUNC_ISNAKED (sym->type))
    {
      emitComment (ALWAYS, "naked function: no epilogue.");
      if (options.debug && currFunc && !regalloc_dry_run)
        debugFile->writeEndFunction (currFunc, ic, 0);
      return;
    }

  if (IFFUNC_ISCRITICAL (sym->type))
    {
      emit6502op ("plp", "");
    }

  if (IFFUNC_ISREENT (sym->type) || options.stackAuto)
    {
    }
    
  if (_G.funcHasBasePtr)
    restoreBasePtr();

  if(_G.stackPushes)
    emitcode("ERROR","_G.stackPushes=%d in genEndFunction");

  if (sym->stack)
    {
      _G.stackPushes += sym->stack;
      adjustStack (sym->stack);
    }


  if ((IFFUNC_ISREENT (sym->type) || options.stackAuto))
    {
    }

  if (IFFUNC_ISISR (sym->type))
    {

      if (!inExcludeList ("y"))
        pullReg (m6502_reg_y); // TODO?


      /* if debug then send end of function */
      if (options.debug && currFunc && !regalloc_dry_run)
        {
          debugFile->writeEndFunction (currFunc, ic, 1);
        }

      emit6502op ("rti", "");
    }
  else
    {
      if (IFFUNC_CALLEESAVES (sym->type))
        {
          int i;

          /* if any registers used */
          if (sym->regsUsed)
            {
              /* save the registers used */
              for (i = sym->regsUsed->size; i >= 0; i--)
                {
                  if (bitVectBitValue (sym->regsUsed, i) || (m6502_ptrRegReq && (i == YX_IDX || i == YX_IDX)))
		    // FIXME
                    emitcode ("pop", "%s", m6502_regWithIdx (i)->name); /* Todo: Cost. Can't find this instruction in manual! */
                }
            }
        }

      /* if debug then send end of function */
      if (options.debug && currFunc && !regalloc_dry_run)
        {
          debugFile->writeEndFunction (currFunc, ic, 1);
        }

      emit6502op ("rts", "");
    }
}

/*-----------------------------------------------------------------*/
/* genRet - generate code for return statement                     */
/*-----------------------------------------------------------------*/
static void
genRet (iCode * ic)
{
  int size, offset = 0;
//  int pushed = 0;
  bool delayed_x = false;

  emitComment (TRACEGEN, __func__);
  emitComment (REGOPS, "  %s %s", __func__, regInfoStr() );


  /* if we have no return value then
     just generate the "ret" */
  if (!IC_LEFT (ic))
    goto jumpret;

  /* we have something to return then
     move the return value into place */
  aopOp (IC_LEFT (ic), ic, false);
  size = AOP_SIZE (IC_LEFT (ic));

  if (AOP_TYPE (IC_LEFT (ic)) == AOP_LIT)
    {
      /* If returning a literal, we can load the bytes of the return value */
      /* in any order. By loading A and X first, any other bytes that match */
      /* can use the shorter sta and stx instructions. */
      offset = 0;
      while (size--)
        {
          transferAopAop (AOP (IC_LEFT (ic)), offset, m6502_aop_pass[offset], 0);
          offset++;
        }
    }
  else
    {
      /* Take care when swapping a and x */
      if (AOP_TYPE (IC_LEFT (ic)) == AOP_REG && size > 1 && AOP (IC_LEFT (ic))->aopu.aop_reg[0]->rIdx == X_IDX)
        {
          delayed_x = true;
          pushReg (m6502_reg_x, true);
        }

      offset = size - 1;
      while (size--)
        {
          if (!(delayed_x && !offset))
            transferAopAop (AOP (IC_LEFT (ic)), offset, m6502_aop_pass[offset], 0);
          offset--;
        }

      if (delayed_x)
        pullReg (m6502_reg_a);
    }

  freeAsmop (IC_LEFT (ic), NULL, ic, true);

jumpret:
  /* generate a jump to the return label
     if the next is not the return statement */
  if (!(ic->next && ic->next->op == LABEL && IC_LABEL (ic->next) == returnLabel))
    {
      emit6502op ("jmp", "%05d$", safeLabelKey2num (returnLabel->key));
    }
}

/*-----------------------------------------------------------------*/
/* genLabel - generates a label                                    */
/*-----------------------------------------------------------------*/
static void
genLabel (iCode * ic)
{
  int i;

  emitComment (TRACEGEN, __func__);
  emitComment (REGOPS, "  %s %s", __func__, regInfoStr() );


  /* For the high level labels we cannot depend on any */
  /* register's contents. Amnesia time.                */
  for (i = A_IDX; i <= XA_IDX; i++) {
      m6502_dirtyReg (m6502_regWithIdx (i));
      m6502_useReg (m6502_regWithIdx (i));
        }
  /* special case never generate */
  if (IC_LABEL (ic) == entryLabel)
    return;

  if (options.debug && !regalloc_dry_run)
    debugFile->writeLabel (IC_LABEL (ic), ic);

  emitLabel (IC_LABEL (ic));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a jmp                                      */
/*-----------------------------------------------------------------*/
static void
genGoto (iCode * ic)
{
  emitComment (TRACEGEN, __func__);
  emit6502op ("jmp", "%05d$", safeLabelKey2num (IC_LABEL (ic)->key));
}


/*-----------------------------------------------------------------*/
/* genPlusIncr :- does addition with increment if possible         */
/*-----------------------------------------------------------------*/
static bool
genPlusIncr (iCode * ic)
{
  int icount;
  operand *left;
  operand *result;
  bool needpula;
  unsigned int size = getDataSize (IC_RESULT (ic));
  unsigned int offset;
  symbol *tlbl = NULL;

  emitComment (TRACEGEN, __func__);

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  /* will try to generate an increment */
  /* if the right side is not a literal
     we cannot */
  if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT)
    return false;

  icount = (unsigned int) ulFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);

  emitComment (TRACEGEN|VVDBG, "  icount = %d, sameRegs=%d", icount, sameRegs (AOP (left), AOP (result)));

  if (icount > 255)
    return false;

  if (!sameRegs (AOP (left), AOP (result)))
    return false;

  // TODO: can inc blah,x
  if (!aopCanIncDec (AOP (result)))
    return false;

  emitComment (TRACEGEN|VVDBG, "   genPlusIncr");

  if (size==1 && AOP(result)->type==AOP_REG) {
    // if it's in a 8-bit register try to do small adjust
    if(smallAdjustReg(AOP(result)->aopu.aop_reg[0], icount)==0) return true;   
  }

  if(icount < 0 )
    return false;

  aopOpExtToIdx (AOP (result), AOP (left), NULL);

  if (size > 1)
    tlbl = safeNewiTempLabel (NULL);

  if (icount == 1)
    {
      needpula = false;
      rmwWithAop ("inc", AOP (result), 0);
      if (size > 1)
        emitBranch ("bne", tlbl);
    }
  else
    {
     if (!IS_AOP_A (AOP (result)) && !IS_AOP_XA (AOP (result)))
        needpula = pushRegIfUsed (m6502_reg_a);
      else
        needpula = false;
      loadRegFromAop (m6502_reg_a, AOP (result), 0);
      emit6502op("clc", "");
      accopWithAop ("adc", AOP (IC_RIGHT (ic)), 0);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      if (size > 1)
        emitBranch ("bcc", tlbl);
    }
  for (offset = 1; offset < size; offset++)
    {
      rmwWithAop ("inc", AOP (result), offset);
      if(AOP(result)->type==AOP_REG) m6502_dirtyReg(AOP(result)->aopu.aop_reg[offset]);
      if ((offset + 1) < size)
        emitBranch ("bne", tlbl);
    }

  if (size > 1)
    safeEmitLabel (tlbl);

  pullOrFreeReg (m6502_reg_a, needpula);

  return true;
}



/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void
genPlus (iCode * ic)
{
  int size, offset = 0;
  bool clc = true;
  asmop *leftOp, *rightOp;
  bool needpulla;
  bool earlystore = false;
  bool delayedstore = false;
  bool mayskip = true;
  bool skip = false;

  /* special cases :- */

  emitComment (TRACEGEN, __func__);

  aopOp (IC_LEFT (ic), ic, false);
  aopOp (IC_RIGHT (ic), ic, false);
  aopOp (IC_RESULT (ic), ic, true);

  /* we want registers on the left and literals on the right */
  if ((AOP_TYPE (IC_LEFT (ic)) == AOP_LIT) || (AOP_TYPE (IC_RIGHT (ic)) == AOP_REG && !IS_AOP_WITH_A (AOP (IC_LEFT (ic)))))
    {
      operand *t = IC_RIGHT (ic);
      IC_RIGHT (ic) = IC_LEFT (ic);
      IC_LEFT (ic) = t;
    }

  /* if I can do an increment instead
     of add then GOOD for ME */
  if (genPlusIncr (ic) == true)
    goto release;

  emitComment (TRACEGEN|VVDBG, "   left size = %d", getDataSize (IC_LEFT (ic)));
  emitComment (TRACEGEN|VVDBG, "   right size = %d", getDataSize (IC_RIGHT (ic)));
  emitComment (TRACEGEN|VVDBG, "   result size = %d", getDataSize (IC_RESULT (ic)));

  aopOpExtToIdx (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic)), AOP (IC_RIGHT (ic)));

  size = getDataSize (IC_RESULT (ic));

  leftOp = AOP (IC_LEFT (ic));
  rightOp = AOP (IC_RIGHT (ic));

  offset = 0;
  needpulla = pushRegIfSurv (m6502_reg_a);

  if(size > 1 && IS_AOP_AX (AOP (IC_LEFT (ic))))
    {
      earlystore = true;
      pushReg (m6502_reg_a, true);
    }

  while (size--)
    {
      if (earlystore && offset == 1)
        pullReg (m6502_reg_a);
      else
        loadRegFromAop (m6502_reg_a, leftOp, offset);
      if (clc)
        emit6502op ("clc", "");
      if (!mayskip || AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT || (byteOfVal (AOP (IC_RIGHT (ic))->aopu.aop_lit, offset) != 0x00) )
        {
          accopWithAop ("adc", rightOp, offset);
          mayskip = false;
          skip = false;
        }
      else
        skip = true;
      if (size && AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP (IC_RESULT (ic))->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          pushReg (m6502_reg_a, true);
          delayedstore = true;
        }
      else
        storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), offset);
      offset++;
      m6502_freeReg (m6502_reg_a);
      if (!skip)
        clc = false;              /* further adds must propagate carry */
    }
 if (delayedstore)
   pullReg (m6502_reg_a);
 pullOrFreeReg (m6502_reg_a, needpulla);

 wassert (!earlystore || !delayedstore);

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, true);
  freeAsmop (IC_RIGHT (ic), NULL, ic, true);
  freeAsmop (IC_LEFT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genMinusDec :- does subtraction with decrement if possible      */
/*-----------------------------------------------------------------*/
static bool
genMinusDec (iCode * ic)
{
  int icount;
  operand *left;
  operand *result;
  unsigned int size = getDataSize (IC_RESULT (ic));
//  int offset;
//  symbol *tlbl;

  emitComment (TRACEGEN, __func__);

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  /* will try to generate an increment */
  /* if the right side is not a literal
     we cannot */
  if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT)
    return false;
    
  icount = (unsigned int) ulFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);
  // TODO: genPlusIncr has a lot more, can merge?

  if (!sameRegs (AOP (left), AOP (result)))
    return false;

  if (size != 1)
    return false;

  // TODO: can inc blah,x
  if (!aopCanIncDec (AOP (result)))
    return false;

  // do dex/dey and inx/iny if icount is negative
  if(!smallAdjustReg(AOP(result)->aopu.aop_reg[0],-icount))
      return true;

  if ((icount > 1) || (icount < 0))
    return false;

  emitComment (TRACEGEN|VVDBG, "     genMinusDec");

  aopOpExtToIdx (AOP (result), AOP (left), NULL);

  rmwWithAop ("dec", AOP (result), 0);

  return true;
}

/*-----------------------------------------------------------------*/
/* addSign - complete with sign                                    */
/*-----------------------------------------------------------------*/
static void
addSign (operand * result, int offset, int sign)
{
  int size = (getDataSize (result) - offset);
  if (size > 0)
    {
      if (sign)
        {
          signExtendA();
          while (size--)
            storeRegToAop (m6502_reg_a, AOP (result), offset++);
        }
      else
        while (size--)
          storeConstToAop (0, AOP (result), offset++);
    }
}


/*-----------------------------------------------------------------*/
/* genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
static void
genMinus (iCode * ic)
{
  bool carry = true;
  int size, offset = 0;
  bool needpulla;
  bool earlystore = false;
  bool delayedstore = false;

  asmop *leftOp, *rightOp;

  emitComment (TRACEGEN, __func__);

  aopOp (IC_LEFT (ic), ic, false);
  aopOp (IC_RIGHT (ic), ic, false);
  aopOp (IC_RESULT (ic), ic, true);

  /* special cases :- */
  /* if I can do an decrement instead
     of subtract then GOOD for ME */
  if (genMinusDec (ic) == true)
    goto release;

  emitComment (TRACEGEN|VVDBG, "    genMinus - Can't Dec");
  aopOpExtToIdx (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic)), AOP (IC_RIGHT (ic)));

  size = getDataSize (IC_RESULT (ic));


  leftOp = AOP (IC_LEFT (ic));
  rightOp = AOP (IC_RIGHT (ic));
  offset = 0;

  if (IS_AOP_A (rightOp))
    {
      // op - a = neg(a - op) = not(a - op) + 1 = not(a - op - 1)
      needpulla = pushRegIfSurv (m6502_reg_a);
      emit6502op("clc", "");
      accopWithAop ("sbc", leftOp, offset);
      emit6502op("eor", "#0xff");
      storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), offset);
      pullOrFreeReg (m6502_reg_a, needpulla);
      goto release;
    }

  needpulla = pushRegIfSurv (m6502_reg_a);

  if (size > 1 && (IS_AOP_AX (AOP (IC_LEFT (ic))) || IS_AOP_AX (AOP (IC_RIGHT (ic)))))
    {
      earlystore = true;
      pushReg (m6502_reg_a, true);
    }

  while (size--)
    {
      if (earlystore &&
        (AOP_TYPE (IC_LEFT (ic)) == AOP_REG && AOP (IC_LEFT (ic))->aopu.aop_reg[offset]->rIdx == A_IDX ||
        AOP_TYPE (IC_RIGHT (ic)) == AOP_REG && AOP (IC_RIGHT (ic))->aopu.aop_reg[offset]->rIdx == A_IDX))
        pullReg (m6502_reg_a);
      if (AOP_TYPE (IC_RIGHT (ic)) == AOP_REG && AOP (IC_RIGHT (ic))->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          storeRegTemp (m6502_reg_a, true);
          loadRegFromAop (m6502_reg_a, leftOp, offset);
          if (carry) {
            emit6502op("sec", "");
            }
          emit6502op ("sbc", TEMPFMT, _G.tempOfs - 1);
          loadRegTemp (NULL);
        }
      else
        {
          emitComment (TRACEGEN|VVDBG, "    - default path");

          loadRegFromAop (m6502_reg_a, leftOp, offset);
          if (carry) {
            emit6502op("sec", "");
            }
          accopWithAop ("sbc", rightOp, offset);
        }
      if (size && AOP_TYPE (IC_RESULT (ic)) == AOP_REG && AOP (IC_RESULT (ic))->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          emitComment (TRACEGEN|VVDBG, "    - push");
          pushReg (m6502_reg_a, true);
          delayedstore = true;
        }
      else
        {
          emitComment (TRACEGEN|VVDBG, "    - store");
        storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), offset);
        }
      offset++;
      carry = false;
    }
  if(delayedstore)
    pullReg (m6502_reg_a);
  pullOrFreeReg (m6502_reg_a, needpulla);

  wassert (!earlystore || !delayedstore);

release:
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (IC_RESULT (ic), NULL, ic, true);
}


/*-----------------------------------------------------------------*/
/* genMult - generates code for multiplication                     */
/*-----------------------------------------------------------------*/
static void
genMult (iCode * ic)
{
  /* Shouldn't occur - all done through function calls */
  wassertl (0, "Multiplication is handled through support function calls");
}

/*-----------------------------------------------------------------*/
/* genDiv - generates code for division                            */
/*-----------------------------------------------------------------*/
static void
genDiv (iCode * ic)
{
  /* Shouldn't occur - all done through function calls */
  wassertl (0, "Division is handled through support function calls");
}

/*-----------------------------------------------------------------*/
/* genMod - generates code for division                            */
/*-----------------------------------------------------------------*/
static void
genMod (iCode * ic)
{
  /* Shouldn't occur - all done through function calls */
  wassertl (0, "Division is handled through support function calls");
}

/*-----------------------------------------------------------------*/
/* genIfxJump :- will create a jump depending on the ifx           */
/*-----------------------------------------------------------------*/
static void
genIfxJump (iCode * ic, char *jval)
{
  symbol *jlbl;
  symbol *tlbl = safeNewiTempLabel (NULL);
  char *inst;

  emitComment (TRACEGEN, __func__);

  /* if true label then we jump if condition
     supplied is true */
  if (IC_TRUE (ic))
    {
      jlbl = IC_TRUE (ic);
      if (!strcmp (jval, "a"))
        inst = "beq";
      else if (!strcmp (jval, "c"))
        inst = "bcc";
      else if (!strcmp (jval, "n"))
        inst = "bpl";
      else if (!strcmp (jval, "v"))
        inst = "bvc";
      else
        inst = "bge";
    }
  else
    {
      /* false label is present */
      jlbl = IC_FALSE (ic);
      if (!strcmp (jval, "a"))
        inst = "bne";
      else if (!strcmp (jval, "c"))
        inst = "bcs";
      else if (!strcmp (jval, "n"))
        inst = "bmi";
      else if (!strcmp (jval, "v"))
        inst = "bvs";
      else
        inst = "blt";
    }
  emitBranch (inst, tlbl);
  emitBranch ("jmp", jlbl);
  safeEmitLabel (tlbl);

  /* mark the icode as generated */
  ic->generated = 1;
}


/*-----------------------------------------------------------------*/
/* exchangedCmp : returns the opcode need if the two operands are  */
/*                exchanged in a comparison                        */
/*-----------------------------------------------------------------*/
static int
exchangedCmp (int opcode)
{
  switch (opcode)
    {
    case '<':
      return '>';
    case '>':
      return '<';
    case LE_OP:
      return GE_OP;
    case GE_OP:
      return LE_OP;
    case NE_OP:
      return NE_OP;
    case EQ_OP:
      return EQ_OP;
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "opcode not a comparison");
    }
  return EQ_OP;                 /* shouldn't happen, but need to return something */
}

/*------------------------------------------------------------------*/
/* negatedCmp : returns the equivalent opcode for when a comparison */
/*              if not true                                         */
/*------------------------------------------------------------------*/
static int
negatedCmp (int opcode)
{
  switch (opcode)
    {
    case '<':
      return GE_OP;
    case '>':
      return LE_OP;
    case LE_OP:
      return '>';
    case GE_OP:
      return '<';
    case NE_OP:
      return EQ_OP;
    case EQ_OP:
      return NE_OP;
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "opcode not a comparison");
    }
  return EQ_OP;                 /* shouldn't happen, but need to return something */
}

/*------------------------------------------------------------------*/
/* nameCmp : helper function for human readable debug output        */
/*------------------------------------------------------------------*/
static char *
nameCmp (int opcode)
{
  switch (opcode)
    {
    case '<':
      return "<";
    case '>':
      return ">";
    case LE_OP:
      return "<=";
    case GE_OP:
      return ">=";
    case NE_OP:
      return "!=";
    case EQ_OP:
      return "==";
    default:
      return "invalid";
    }
}

/*------------------------------------------------------------------*/
/* branchInstCmp : returns the conditional branch instruction that  */
/*                 will branch if the comparison is true            */
/*------------------------------------------------------------------*/
static char *
branchInstCmp (int opcode, int sign)
{
  switch (opcode)
    {
    case '<':
      if (sign)
        return "blt";
      else
        return "bcc";
    case '>':
      if (sign)
        return "bgt";
      else
        return "bhi";
    case LE_OP:
      if (sign)
        return "ble";
      else
        return "bls";
    case GE_OP:
      if (sign)
        return "bge";
      else
        return "bcs";
    case NE_OP:
      return "bne";
    case EQ_OP:
      return "beq";
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "opcode not a comparison");
    }
  return "brn";
}

/*------------------------------------------------------------------*/
/* genCmp :- greater or less than (and maybe with equal) comparison */
/*------------------------------------------------------------------*/
static void
genCmp (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  sym_link *letype, *retype;
  int sign, opcode;
  int size, offset = 0;
  unsigned long long lit = 0ull;
  char *sub;
  symbol *jlbl = NULL;
  bool needloada = false;

  emitComment (TRACEGEN, __func__);

  opcode = ic->op;

  // TODO: optimize for cmp regs with 0 or constant

  result = IC_RESULT (ic);
  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);

  sign = 0;
  // TODO: don't use signed when unsigned will do
  if (IS_SPEC (operandType (left)) && IS_SPEC (operandType (right)))
    {
      letype = getSpec (operandType (left));
      retype = getSpec (operandType (right));
      sign = !(SPEC_USIGN (letype) | SPEC_USIGN (retype));
    }

  /* assign the amsops */
  aopOp (left, ic, false);
  aopOp (right, ic, false);
  aopOp (result, ic, true);

  /* need register operand on left, prefer literal operand on right */
  if ((AOP_TYPE (right) == AOP_REG) || AOP_TYPE (left) == AOP_LIT)
    {
      operand *temp = left;
      left = right;
      right = temp;
      opcode = exchangedCmp (opcode);
    }
  // TODO: special case for compare with 0

  if (ifx)
    {
      if (IC_TRUE (ifx))
        {
          jlbl = IC_TRUE (ifx);
          opcode = negatedCmp (opcode);
        }
      else
        {
          /* false label is present */
          jlbl = IC_FALSE (ifx);
        }
    }

  size = max (AOP_SIZE (left), AOP_SIZE (right));

  emitComment (TRACEGEN|VVDBG, "      genCmp", "(%s, size %d, sign %d)", nameCmp (opcode), size, sign);
  
  if (!sign && size == 1 && IS_AOP_X (AOP (left)) && isAddrSafe(right, m6502_reg_x) )
    {
      accopWithAop ("cpx", AOP (right), offset);
    }
  else if (!sign && size == 1 && IS_AOP_Y (AOP (left)) && isAddrSafe(right, m6502_reg_y) )
    {
      accopWithAop ("cpy", AOP (right), offset);
    }
  else if (!sign && size == 1 && IS_AOP_A (AOP (left)) && isAddrSafe(right, m6502_reg_a))
    {
      accopWithAop ("cmp", AOP (right), offset);
    }
  else
    {
      offset = 0;
      // need V flag for signed compare
      // FIXME: is this covered above?
      if (size == 1 && sign == 0)
        sub = "cmp";
      else
        {
          sub = "sub";

          /* These conditions depend on the Z flag bit, but Z is */
          /* only valid for the last byte of the comparison, not */
          /* the whole value. So exchange the operands to get a  */
          /* comparison that doesn't depend on Z. (This is safe  */
          /* to do here since ralloc won't assign multi-byte     */
          /* operands to registers for comparisons)              */
          if ((opcode == '>') || (opcode == LE_OP))
            {
              operand *temp = left;
              left = right;
              right = temp;
              opcode = exchangedCmp (opcode);
            }

          if ((AOP_TYPE (right) == AOP_LIT) && !isOperandVolatile (left, false))
            {
              lit = ullFromVal (AOP (right)->aopu.aop_lit);
              while ((size > 1) && (((lit >> (8 * offset)) & 0xff) == 0))
                {
                  offset++;
                  size--;
                }
            }
        }
      needloada = storeRegTempIfSurv (m6502_reg_a);
      while (size--)
        {
          emitComment (TRACEGEN|VVDBG, "   GenCmp - size counter = %d", size);

          if (AOP_TYPE (right) == AOP_REG && AOP(right)->aopu.aop_reg[offset]->rIdx == A_IDX)
            {
              storeRegTemp(m6502_reg_a, true);
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              if (!strcmp(sub, "sub")) {
                emit6502op ("sec", "");
                emit6502op ("sbc", TEMPFMT, _G.tempOfs - 1);
              } else {         
                emit6502op (sub, TEMPFMT, _G.tempOfs - 1);
              }
              loadRegTemp(NULL);
            }
          else
            {
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              if (!strcmp(sub, "sub")) {
                emit6502op ("sec", "");
                accopWithAop ("sbc", AOP (right), offset);
              } else {
                accopWithAop (sub, AOP (right), offset);
              }
            }
          m6502_freeReg (m6502_reg_a);
          offset++;
          sub = "sbc";
        }
    }
  freeAsmop (right, NULL, ic, false);
  freeAsmop (left, NULL, ic, false);

  if (ifx)
    {
      symbol *tlbl = safeNewiTempLabel (NULL);
      char *inst;

      if(needloada) 
          loadRegTempNoFlags(m6502_reg_a, needloada);
      else
          m6502_freeReg(m6502_reg_a);

      freeAsmop (result, NULL, ic, true);

      inst = branchInstCmp (opcode, sign);
      emitBranch (inst, tlbl);
      emitBranch ("jmp", jlbl);
      safeEmitLabel (tlbl);

      /* mark the icode as generated */
      ifx->generated = 1;
    }
  else
    {
      symbol *tlbl1 = safeNewiTempLabel (NULL);
      symbol *tlbl2 = safeNewiTempLabel (NULL);

      if (!needloada)
        needloada = storeRegTempIfSurv (m6502_reg_a);

      emitBranch (branchInstCmp (opcode, sign), tlbl1);
      loadRegFromConst (m6502_reg_a, 0);
      emitBranch ("bra", tlbl2);
      safeEmitLabel (tlbl1);
      loadRegFromConst (m6502_reg_a, 1);
      safeEmitLabel (tlbl2);
      m6502_dirtyReg (m6502_reg_a);
      storeRegToFullAop (m6502_reg_a, AOP (result), false);
      loadOrFreeRegTemp (m6502_reg_a, needloada);
      freeAsmop (result, NULL, ic, true);
    }
}

/*-----------------------------------------------------------------*/
/* genCmpEQorNE - equal or not equal comparison                    */
/*-----------------------------------------------------------------*/
static void
genCmpEQorNE (iCode * ic, iCode * ifx)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  int opcode;
  int size;
  symbol *jlbl = NULL;
  symbol *tlbl_NE = NULL;
  symbol *tlbl_EQ = NULL;
  bool needloada = false;
  int offset = 0;

  emitComment (TRACEGEN, __func__);

  opcode = ic->op;

  emitComment (TRACEGEN|VVDBG, "      genCmpEQorNE (%s)", nameCmp (opcode));

  /* assign the amsops */
  aopOp (left, ic, false);
  aopOp (right, ic, false);
  aopOp (result, ic, true);

  /* need register operand on left, prefer literal operand on right */
  if ((AOP_TYPE (right) == AOP_REG) || AOP_TYPE (left) == AOP_LIT)
    {
      operand *temp = left;
      left = right;
      right = temp;
      opcode = exchangedCmp (opcode);
    }

  size = max (AOP_SIZE (left), AOP_SIZE (right));

  if (ifx)
    {
      if (IC_TRUE (ifx))
        {
          jlbl = IC_TRUE (ifx);
          opcode = negatedCmp (opcode);
        }
      else
        {
          /* false label is present */
          jlbl = IC_FALSE (ifx);
        }
    }

  if(AOP_TYPE (left) == AOP_REG)
  emitComment (TRACEGEN|VVDBG, "   genCmpEQorNE left is reg: %s",AOP (left)->aopu.aop_reg[offset]->name);
  else
  emitComment (TRACEGEN|VVDBG, "   genCmpEQorNE left is not not a reg");

// TODO: could clobber A if reg = XA?
    {
      offset = 0;
      while (size--)
        {
          if (AOP_TYPE (left) == AOP_REG && AOP (left)->aopu.aop_reg[offset]->rIdx == X_IDX && isAddrSafe(right, m6502_reg_x))
            accopWithAop ("cpx", AOP (right), offset);
          else if (AOP_TYPE (left) == AOP_REG && AOP (left)->aopu.aop_reg[offset]->rIdx == Y_IDX && isAddrSafe(right, m6502_reg_y))
            accopWithAop ("cpy", AOP (right), offset);
          else
            {
              emitComment (TRACEGEN|VVDBG, "      genCmpEQorNE can't cpx or cpy");

            // TODO? why do we push when we could cpx?
              if (!(AOP_TYPE (left) == AOP_REG && AOP (left)->aopu.aop_reg[offset]->rIdx == A_IDX))
                {
                  // FIXME: always?
                  storeRegTemp (m6502_reg_a, true);
                  needloada = true;
//                  needloada = storeRegTempIfUsed (m6502_reg_a);
                  loadRegFromAop (m6502_reg_a, AOP (left), offset);
                }
              accopWithAop ("cmp", AOP (right), offset);
              loadRegTempNoFlags (m6502_reg_a, needloada);
              needloada = false;
            }
          if (size)
            {
              symbol *tmp_label = safeNewiTempLabel (NULL);;
              if (!tlbl_NE)
                tlbl_NE = safeNewiTempLabel (NULL);
              if (!needloada && !ifx)
                needloada = storeRegTempIfSurv (m6502_reg_a);

              emitBranch ("beq", tmp_label);
              emitBranch ("jmp", tlbl_NE);
              safeEmitLabel (tmp_label);

              loadOrFreeRegTemp (m6502_reg_a, needloada);
              needloada = false;
            }
          offset++;
          }
    }
  freeAsmop (right, NULL, ic, false);
  freeAsmop (left, NULL, ic, false);

  if (ifx)
    {
      freeAsmop (result, NULL, ic, true);

      if (opcode == EQ_OP)
        {
          if (!tlbl_EQ)
            tlbl_EQ = safeNewiTempLabel (NULL);
          emitBranch ("beq", tlbl_EQ);
          if (tlbl_NE)
            safeEmitLabel (tlbl_NE);
          emitBranch ("jmp", jlbl);
          safeEmitLabel (tlbl_EQ);
        }
      else
        {
          if (!tlbl_NE)
            tlbl_NE = safeNewiTempLabel (NULL);
          emitBranch ("bne", tlbl_NE);
          emitBranch ("jmp", jlbl);
          safeEmitLabel (tlbl_NE);
        }

      /* mark the icode as generated */
      ifx->generated = 1;
    }
  else
    {
      symbol *tlbl = safeNewiTempLabel (NULL);

      if (!needloada)
        needloada = storeRegTempIfSurv (m6502_reg_a);
      if (opcode == EQ_OP)
        {
          if (!tlbl_EQ)
            tlbl_EQ = safeNewiTempLabel (NULL);
          emitBranch ("beq", tlbl_EQ);
          if (tlbl_NE)
            emitLabel (tlbl_NE);
          loadRegFromConst (m6502_reg_a, 0);
          emitBranch ("bra", tlbl);
          safeEmitLabel (tlbl_EQ);
          loadRegFromConst (m6502_reg_a, 1);
        }
      else
        {
          if (!tlbl_NE)
            tlbl_NE = safeNewiTempLabel (NULL);
          emitBranch ("bne", tlbl_NE);
          loadRegFromConst (m6502_reg_a, 0);
          emitBranch ("bra", tlbl);
          safeEmitLabel (tlbl_NE);
          loadRegFromConst (m6502_reg_a, 1);
        }

      safeEmitLabel (tlbl);
      m6502_dirtyReg (m6502_reg_a);
      storeRegToFullAop (m6502_reg_a, AOP (result), false);
      loadOrFreeRegTemp (m6502_reg_a, needloada);
      freeAsmop (result, NULL, ic, true);
    }
}

/*-----------------------------------------------------------------*/
/* genAndOp - for && operation                                     */
/*-----------------------------------------------------------------*/
static void
genAndOp (iCode * ic)
{
  operand *left, *right, *result;
  symbol *tlbl, *tlbl0;
  bool needpulla;

  emitComment (TRACEGEN, __func__);
  
  // TODO: optimize & 0xff as cast when signed

  /* note here that && operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  aopOp ((left = IC_LEFT (ic)), ic, false);
  aopOp ((right = IC_RIGHT (ic)), ic, false);
  aopOp ((result = IC_RESULT (ic)), ic, false);

  tlbl = safeNewiTempLabel (NULL);
  tlbl0 = safeNewiTempLabel (NULL);

  needpulla = pushRegIfSurv (m6502_reg_a);
  asmopToBool (AOP (left), false);
  emitBranch ("beq", tlbl0);
  asmopToBool (AOP (right), false);
  emitBranch ("beq", tlbl0);
  loadRegFromConst (m6502_reg_a, 1);
  emitBranch ("bra", tlbl);
  safeEmitLabel (tlbl0);
//  m6502_dirtyReg (m6502_reg_a);
  loadRegFromConst (m6502_reg_a, 0);
  safeEmitLabel (tlbl);
  m6502_dirtyReg (m6502_reg_a);

//  m6502_useReg (m6502_reg_a);
//  m6502_freeReg (m6502_reg_a);

  storeRegToFullAop (m6502_reg_a, AOP (result), false);
  pullOrFreeReg(m6502_reg_a, needpulla);

  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (result, NULL, ic, true);
}


/*-----------------------------------------------------------------*/
/* genOrOp - for || operation                                      */
/*-----------------------------------------------------------------*/
static void
genOrOp (iCode * ic)
{
  operand *left, *right, *result;
  symbol *tlbl, *tlbl0;
  bool needpulla;

  emitComment (TRACEGEN, __func__);

  /* note here that || operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  aopOp ((left = IC_LEFT (ic)), ic, false);
  aopOp ((right = IC_RIGHT (ic)), ic, false);
  aopOp ((result = IC_RESULT (ic)), ic, false);

  tlbl = safeNewiTempLabel (NULL);
  tlbl0 = safeNewiTempLabel (NULL);

  needpulla = pushRegIfSurv (m6502_reg_a);
  asmopToBool (AOP (left), false);
  emitBranch ("bne", tlbl0);
  asmopToBool (AOP (right), false);
  emitBranch ("bne", tlbl0);
  loadRegFromConst (m6502_reg_a, 0);
  emitBranch ("bra", tlbl);
  safeEmitLabel (tlbl0);
//  m6502_dirtyReg (m6502_reg_a);
  loadRegFromConst (m6502_reg_a, 1);
  safeEmitLabel (tlbl);
  m6502_dirtyReg (m6502_reg_a);

//  m6502_useReg (m6502_reg_a);
//  m6502_freeReg (m6502_reg_a);

  storeRegToFullAop (m6502_reg_a, AOP (result), false);
  pullOrFreeReg(m6502_reg_a, needpulla);

  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* isLiteralBit - test if lit == 2^n                               */
/*-----------------------------------------------------------------*/
static int
isLiteralBit (unsigned long lit)
{
  unsigned long pw[32] =
  {
    1L, 2L, 4L, 8L, 16L, 32L, 64L, 128L,
    0x100L, 0x200L, 0x400L, 0x800L,
    0x1000L, 0x2000L, 0x4000L, 0x8000L,
    0x10000L, 0x20000L, 0x40000L, 0x80000L,
    0x100000L, 0x200000L, 0x400000L, 0x800000L,
    0x1000000L, 0x2000000L, 0x4000000L, 0x8000000L,
    0x10000000L, 0x20000000L, 0x40000000L, 0x80000000L
  };
  int idx;

  for (idx = 0; idx < 32; idx++)
    if (lit == pw[idx])
      return idx + 1;
  return 0;
}


/*-----------------------------------------------------------------*/
/* genAnd  - code for and                                          */
/*-----------------------------------------------------------------*/
static void
genAnd (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  unsigned long long lit = 0ll;
  unsigned long long litinv;
  int bitpos = -1;
  unsigned char bytemask;
  bool needpulla = false;
  bool earlystore = false;

  emitComment (TRACEGEN, __func__);

  aopOp ((left = IC_LEFT (ic)), ic, false);
  aopOp ((right = IC_RIGHT (ic)), ic, false);
  aopOp ((result = IC_RESULT (ic)), ic, true);

  emitComment (TRACEGEN|VVDBG, "  Type res[%d] = l[%d]&r[%d]", AOP_TYPE (result), AOP_TYPE (left), AOP_TYPE (right));
  emitComment (TRACEGEN|VVDBG, "  Size res[%d] = l[%d]&r[%d]", AOP_SIZE (result), AOP_SIZE (left), AOP_SIZE (right));

  /* if left is a literal & right is not then exchange them */
  if (AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if right is accumulator & left is not then exchange them */
  if (AOP_TYPE (right) == AOP_REG && ! IS_AOP_WITH_A (AOP (left)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  size = (AOP_SIZE (left) >= AOP_SIZE (right)) ? AOP_SIZE (left) : AOP_SIZE (right);

  if (AOP_TYPE (right) == AOP_LIT)
    {
      lit = ullFromVal (AOP (right)->aopu.aop_lit);
      if (size == 1)
        lit &= 0xff;
      else if (size == 2)
        lit &= 0xffff;
      else if (size == 4)
        lit &= 0xffffffff;
      else if (size == 8)
        lit &= 0xffffffffffffffff;
      bitpos = isLiteralBit (lit) - 1;
    }

  emitComment (TRACEGEN|VVDBG, "  lit=%04x bitpos=%d", lit, bitpos );
  
#if 0
  if (IS_MOS65C02 && ifx && AOP_TYPE (result) == AOP_CRY && AOP_TYPE (right) == AOP_LIT && AOP_TYPE (left) == AOP_DIR && bitpos >= 0)
    {
      symbol *tlbl = NULL;
      tlbl = safeNewiTempLabel (NULL);
      if (IC_TRUE (ifx))
        {
	  // FIXME: unimplemented
          emitcode ("brclr", "#%d,%s,%05d$", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false), safeLabelKey2num ((tlbl->key)));
          emitBranch ("jmp", IC_TRUE (ifx));
          safeEmitLabel (tlbl);
          if (IC_FALSE (ifx))
            emitBranch ("jmp", IC_FALSE (ifx));
        }
      else
        {
	  // FIXME: unimplemented
          if (!regalloc_dry_run)
            emitcode ("brset", "#%d,%s,%05d$", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false), safeLabelKey2num ((tlbl->key)));
          emitBranch ("jmp", IC_FALSE (ifx));
          safeEmitLabel (tlbl);
        }
      ifx->generated = true;
      goto release;
    }
#endif

/* TODO
  if (AOP_TYPE (result) == AOP_CRY && size > 1 && (isOperandVolatile (left, false) || isOperandVolatile (right, false)))
    {
      needpulla = pushRegIfSurv (m6502_reg_a);

      // this generates ugly code, but meets volatility requirements
      loadRegFromConst (m6502_reg_a, 0);
      pushReg (m6502_reg_a, true);

      offset = 0;
      while (size--)
        {
	  // FIXME: unimplemented
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          accopWithAop ("and", AOP (right), offset);
          emitcode ("ora", "1,s");
          emitcode ("sta", "1,s");
          offset++;
        }

      pullReg (m6502_reg_a);
      emit6502op ("cmp", "#0x00");

      pullOrFreeReg (m6502_reg_a, needpulla);

      genIfxJump (ifx, "a");

      goto release;
    }
  */

  #if 0
  // special case for bit 7 and 6
  if (AOP_TYPE (result) == AOP_CRY && AOP_TYPE (right) == AOP_LIT)
    {
      if (bitpos >= 0 && (bitpos & 7) == 7)
        {
          rmwWithAop ("bit", AOP (left), bitpos >> 3);
          genIfxJump (ifx, "n");
          goto release;
        }
      if (bitpos >= 0 && (bitpos & 7) == 6)
        {
          rmwWithAop ("bit", AOP (left), bitpos >> 3);
          genIfxJump (ifx, "v");
          goto release;
        }
    }
  #endif

  // test A for flags only
  if (AOP_TYPE (result) == AOP_CRY && size == 1 && (IS_AOP_A (AOP (left)) || IS_AOP_A (AOP (right))))
    {
      if (IS_AOP_A (AOP (left)) && m6502_reg_a->isDead)
        accopWithAop ("and", AOP (right), 0);
      else if (IS_AOP_A (AOP (right)) && m6502_reg_a->isDead)
        accopWithAop ("and", AOP (left), 0);
      else if (IS_AOP_A (AOP (left)) && canBitOp(right))
        accopWithAop ("bit", AOP (right), 0);
      else if (IS_AOP_A (AOP (right)) && canBitOp(left))
        accopWithAop ("bit", AOP (left), 0);
      else {
        storeRegTemp(m6502_reg_a, true);
        loadRegFromAop (m6502_reg_a, IS_AOP_A(AOP(left)) ? AOP(right) : AOP(left), offset);
        emit6502op ("bit", TEMPFMT, _G.tempOfs - 1);
        loadRegTempNoFlags(m6502_reg_a, true); // preserve flags
      }
      genIfxJump (ifx, "a");
      goto release;
    }

  // test for flags only (general case)
  if (AOP_TYPE (result) == AOP_CRY)
    {
      symbol *tlbl = NULL;

      needpulla = storeRegTempIfSurv (m6502_reg_a);

      offset = 0;
      while (size--)
        {
          bytemask = (lit >> (offset * 8)) & 0xff;

          if (AOP_TYPE (right) == AOP_LIT && bytemask == 0)
            {
              /* do nothing */
            }
          else if (AOP_TYPE (right) == AOP_LIT && bytemask == 0xff)
            {
              // TODO: what if A? are flags set?
              loadRegFromAop (m6502_reg_a, AOP(left), offset);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          // don't clobber A (TODO: other ops?)
          else if (IS_AOPOFS_A(AOP(right), offset))
            {
              accopWithAop ("and", AOP(left), offset);
              m6502_freeReg (m6502_reg_a);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          else if (IS_AOPOFS_A(AOP(left), offset))
            {
              accopWithAop ("and", AOP(right), offset);
              m6502_freeReg (m6502_reg_a);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          else
            {
              // TODO: fix other ops like this?
              loadRegFromAop (m6502_reg_a, AOP(right), offset);
              accopWithAop ("and", AOP(left), offset);
              m6502_freeReg (m6502_reg_a);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          offset++;
        }
      if (tlbl)
        emitLabel (tlbl);

      // TODO: better way to preserve flags?
      if (ifx) {
        loadRegTempNoFlags (m6502_reg_a, needpulla);
        genIfxJump (ifx, "a");
      } else {
        if (needpulla) loadRegTemp (NULL);
      }
      goto release;
    }

  size = AOP_SIZE (result);

  if (AOP_TYPE (right) == AOP_LIT && IS_MOS65C02)
    {
      litinv = (~lit) & (((unsigned int) 0xffffffff) >> (8 * (4 - size)));
      if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) && (AOP_TYPE (left) == AOP_DIR) && isLiteralBit (litinv))
        {
	  // FIXME: unimplemented
          bitpos = isLiteralBit (litinv) - 1;
          emitcode ("bclr", "#%d,%s", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false));
          goto release;
        }
    }

  needpulla = pushRegIfSurv (m6502_reg_a);

  offset = 0;
  if (size >= 2 && IS_AOP_AX (AOP (left)))
    {
      pushReg (m6502_reg_a, true);
      earlystore = true;
    }
  while (size--)
    {
      bytemask = (lit >> (offset * 8)) & 0xff;

      if (earlystore && offset == 1)
        pullReg (m6502_reg_a);
      if (AOP_TYPE (right) == AOP_LIT && bytemask == 0)
        {
          if (isOperandVolatile (left, false))
            {
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              m6502_freeReg (m6502_reg_a);
            }
          storeConstToAop (0, AOP (result), offset);
        }
      else if (AOP_TYPE (right) == AOP_LIT && bytemask == 0xff)
        {
          transferAopAop (AOP (left), offset, AOP (result), offset);
        }
      else
        {
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          accopWithAop ("and", AOP (right), offset);
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          m6502_freeReg (m6502_reg_a);
        }
      if (AOP_TYPE (result) == AOP_REG && size && AOP (result)->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          pushReg (m6502_reg_a, true);
          needpulla = true;
        }
      offset++;
    }

  pullOrFreeReg (m6502_reg_a, needpulla);

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genOr  - code for or                                            */
/*-----------------------------------------------------------------*/
static void
genOr (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  unsigned long long lit = 0ull;
  unsigned char bytemask;
  bool needpulla = false;
  bool earlystore = false;

  emitComment (TRACEGEN, __func__);

  aopOp ((left = IC_LEFT (ic)), ic, false);
  aopOp ((right = IC_RIGHT (ic)), ic, false);
  aopOp ((result = IC_RESULT (ic)), ic, true);

  emitComment (TRACEGEN|VVDBG, "  Type res[%d] = l[%d]&r[%d]", AOP_TYPE (result), AOP_TYPE (left), AOP_TYPE (right));
  emitComment (TRACEGEN|VVDBG, "  Size res[%d] = l[%d]&r[%d]", AOP_SIZE (result), AOP_SIZE (left), AOP_SIZE (right));

  /* if left is a literal & right is not then exchange them */
  if (AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if left is accumulator & right is not then exchange them */
  if (AOP_TYPE (right) == AOP_REG && !IS_AOP_WITH_A (AOP (left)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  if (AOP_TYPE (right) == AOP_LIT)
    lit = ullFromVal (AOP (right)->aopu.aop_lit);

  size = (AOP_SIZE (left) >= AOP_SIZE (right)) ? AOP_SIZE (left) : AOP_SIZE (right);

/* TODO
  if (AOP_TYPE (result) == AOP_CRY && size > 1 && (isOperandVolatile (left, false) || isOperandVolatile (right, false)))
    {
      needpulla = pushRegIfSurv (m6502_reg_a);

      // this generates ugly code, but meets volatility requirements 
      loadRegFromConst (m6502_reg_a, 0);
      storeRegTemp (m6502_reg_a, true);

      offset = 0;
      while (size--)
        {
	  // FIXME: unimplemented
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          accopWithAop ("ora", AOP (right), offset);
          emitcode ("ora", "1,s"); // TODO :P
          emitcode ("sta", "1,s");
          offset++;
        }

      loadRegTemp (m6502_reg_a);
      emit6502op ("cmp", "#0x00");

      pullOrFreeRegNoFlags (m6502_reg_a, needpulla);

      genIfxJump (ifx, "a");

      goto release;
    }
*/    
  // TODO: (x | const) should always be known result

  if (AOP_TYPE (result) == AOP_CRY)
    {
      symbol *tlbl = NULL;

      needpulla = storeRegTempIfSurv (m6502_reg_a);

      offset = 0;
      while (size--)
        {
          bytemask = (lit >> (offset * 8)) & 0xff;

          if (AOP_TYPE (right) == AOP_LIT && bytemask == 0x00)
            {
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          else
            {
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              accopWithAop ("ora", AOP (right), offset);
              m6502_freeReg (m6502_reg_a);
              if (size)
                {
                  if (!tlbl)
                    tlbl = safeNewiTempLabel (NULL);
                  emitBranch ("bne", tlbl);
                }
            }
          offset++;
        }
      if (tlbl)
        emitLabel (tlbl);


      if (ifx) {
        loadRegTempNoFlags (m6502_reg_a, needpulla);
        genIfxJump (ifx, "a");
      } else {
        if (needpulla) loadRegTemp (NULL);
      }

      goto release;
    }

  if (AOP_TYPE (right) == AOP_LIT)
    lit = ullFromVal (AOP (right)->aopu.aop_lit);

  size = AOP_SIZE (result);

  if (IS_MOS65C02 && sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) &&
      (AOP_TYPE (right) == AOP_LIT) && isLiteralBit (lit) && (AOP_TYPE (left) == AOP_DIR))
    {
      // FIXME: unimplemented
      int bitpos = isLiteralBit (lit) - 1;
      emitcode ("bset", "#%d,%s", bitpos & 7, aopAdrStr (AOP (left), bitpos >> 3, false));
      goto release;
    }

  needpulla = pushRegIfSurv (m6502_reg_a);

  offset = 0;
  if (size >= 2 && IS_AOP_AX (AOP (left)))
    {
      pushReg (m6502_reg_a, true);
      earlystore = true;
    }
  while (size--)
    {
      bytemask = (lit >> (offset * 8)) & 0xff;

      if (earlystore && offset == 1)
        pullReg (m6502_reg_a);
      if (AOP_TYPE (right) == AOP_LIT && bytemask == 0xff)
        {
          if (isOperandVolatile (left, false))
            {
              loadRegFromAop (m6502_reg_a, AOP (left), offset);
              m6502_freeReg (m6502_reg_a);
            }
          transferAopAop (AOP (right), offset, AOP (result), offset);
        }
      else if (AOP_TYPE (right) == AOP_LIT && bytemask == 0)
        {
          transferAopAop (AOP (left), offset, AOP (result), offset);
        }
      else
        {
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          accopWithAop ("ora", AOP (right), offset);
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          m6502_freeReg (m6502_reg_a);
        }
      if (AOP_TYPE (result) == AOP_REG && size && AOP (result)->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          pushReg (m6502_reg_a, true);
          needpulla = true;
        }
      offset++;
    }

  pullOrFreeReg (m6502_reg_a, needpulla);

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genXor - code for Exclusive or                                  */
/*-----------------------------------------------------------------*/
static void
genXor (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  bool needpulla = false;
  bool earlystore = false;

  emitComment (TRACEGEN, __func__);

  aopOp ((left = IC_LEFT (ic)), ic, false);
  aopOp ((right = IC_RIGHT (ic)), ic, false);
  aopOp ((result = IC_RESULT (ic)), ic, true);

  emitComment (TRACEGEN|VVDBG, " Type res[%d] = l[%d]&r[%d]", AOP_TYPE (result), AOP_TYPE (left), AOP_TYPE (right));
  emitComment (TRACEGEN|VVDBG, " Size res[%d] = l[%d]&r[%d]", AOP_SIZE (result), AOP_SIZE (left), AOP_SIZE (right));

  /* if left is a literal & right is not ||
     if left needs acc & right does not */
  if (AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if left is accumulator & right is not then exchange them */
  if (AOP_TYPE (right) == AOP_REG && !IS_AOP_WITH_A (AOP (left)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  needpulla = storeRegTempIfSurv (m6502_reg_a);

  if (AOP_TYPE (result) == AOP_CRY)
    {
      symbol *tlbl;

      tlbl = safeNewiTempLabel (NULL);
      size = (AOP_SIZE (left) >= AOP_SIZE (right)) ? AOP_SIZE (left) : AOP_SIZE (right);
      offset = 0;
      while (size--)
        {
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          if (AOP_TYPE (right) == AOP_LIT && ((ullFromVal (AOP (right)->aopu.aop_lit) >> (offset * 8)) & 0xff) == 0)
            {
              emit6502op ("cmp", "#0x00");
            }
          else
            accopWithAop ("eor", AOP (right), offset);

          m6502_freeReg (m6502_reg_a);
          if (size)
            emitBranch ("bne", tlbl);
          else
            {
              /*
               * I think this is all broken here, (see simulation mismatch in bug1875933.c)
               *   multiple calls to emitLabel() ?!
               * and we can't genIfxJump, if there is none
               */
              safeEmitLabel (tlbl);
              if (ifx) {
                loadRegTempNoFlags (m6502_reg_a, needpulla);
                genIfxJump (ifx, "a");
              } else {
                if (needpulla) loadRegTemp (NULL);
              }
            }
          offset++;
        }
      goto release;
    }

  size = AOP_SIZE (result);
  offset = 0;
  // TODO?
  if (size >= 2 && IS_AOP_AX (AOP (left)))
    {
      pushReg (m6502_reg_a, true);
      earlystore = true;
    }
  while (size--)
    {
      if (earlystore && offset == 1)
        pullReg (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (left), offset);
      accopWithAop ("eor", AOP (right), offset);
      storeRegToAop (m6502_reg_a, AOP (result), offset);
      if (AOP_TYPE (result) == AOP_REG && size && AOP (result)->aopu.aop_reg[offset]->rIdx == A_IDX)
        {
          storeRegTemp (m6502_reg_a, true);
          needpulla = true;
        }
      m6502_freeReg (m6502_reg_a);
      offset++;
    }
// FIXME: check for loadOrFree
  loadOrFreeRegTemp (m6502_reg_a, needpulla);

release:

  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? false : true));
  freeAsmop (result, NULL, ic, true);
}

static const char *
expand_symbols (iCode * ic, const char *inlin)
{
  const char *begin = NULL, *p = inlin;
  bool inIdent = false;
  struct dbuf_s dbuf;

  dbuf_init (&dbuf, 128);

  while (*p)
    {
      if (inIdent)
        {
          if ('_' == *p || isalnum (*p))
            /* in the middle of identifier */
            ++p;
          else
            {
              /* end of identifier */
              symbol *sym, *tempsym;
              char *symname = Safe_strndup (p + 1, p - begin - 1);

              inIdent = 0;

              tempsym = newSymbol (symname, ic->level);
              tempsym->block = ic->block;
              sym = (symbol *) findSymWithLevel (SymbolTab, tempsym);
              if (!sym)
                {
                  dbuf_append (&dbuf, begin, p - begin);
                }
              else
                {
                  asmop *aop = aopForSym (ic, sym, false);
                  const char *l = aopAdrStr (aop, aop->size - 1, true);

                  if ('#' == *l)
                    l++;
                  sym->isref = 1;
                  if (sym->level && !sym->allocreq && !sym->ismyparm)
                    {
                      werror (E_ID_UNDEF, sym->name);
                      werror (W_CONTINUE,
                              "  Add 'volatile' to the variable declaration so that it\n"
                              "  can be referenced within inline assembly");
                    }
                  dbuf_append_str (&dbuf, l);
                }
              Safe_free (symname);
              begin = p++;
            }
        }
      else if ('_' == *p)
        {
          /* begin of identifier */
          inIdent = true;
          if (begin)
            dbuf_append (&dbuf, begin, p - begin);
          begin = p++;
        }
      else
        {
          if (!begin)
            begin = p;
          p++;
        }
    }

  if (begin)
    dbuf_append (&dbuf, begin, p - begin);

  return dbuf_detach_c_str (&dbuf);
}

/*-----------------------------------------------------------------*/
/* genInline - write the inline code out                           */
/*-----------------------------------------------------------------*/
static void
m6502_genInline (iCode * ic)
{
  char *buf, *bp, *begin;
  const char *expanded;
  bool inComment = false;

  emitComment (TRACEGEN, __func__);

  genLine.lineElement.isInline += (!options.asmpeep);

  buf = bp = begin = Safe_strdup (IC_INLINE (ic));

  /* emit each line as a code */
  while (*bp)
    {
      switch (*bp)
        {
        case ';':
          inComment = true;
          ++bp;
          break;

        case '\x87':
        case '\n':
          inComment = false;
          *bp++ = '\0';
          expanded = expand_symbols (ic, begin);
          emitcode (expanded, NULL);
          dbuf_free (expanded);
          begin = bp;
          break;

        default:
          /* Add \n for labels, not dirs such as c:\mydir */
          if (!inComment && (*bp == ':') && (isspace ((unsigned char) bp[1])))
            {
              ++bp;
              *bp = '\0';
              ++bp;
              emitcode (begin, NULL);
              begin = bp;
            }
          else
            ++bp;
          break;
        }
    }
  if (begin != bp)
    {
      expanded = expand_symbols (ic, begin);
      emitcode (expanded, NULL);
      dbuf_free (expanded);
    }

  Safe_free (buf);

  /* consumed; we can free it here */
  dbuf_free (IC_INLINE (ic));

  genLine.lineElement.isInline -= (!options.asmpeep);
}

// TODO: are these called?
/*-----------------------------------------------------------------*/
/* genRRC - rotate right with carry                                */
/*-----------------------------------------------------------------*/
static void
genRRC (iCode * ic)
{
  operand *left, *result;
  int size, offset = 0;
  bool needpula = false;
  bool resultInA = false;
  char *shift;

  emitComment (TRACEGEN, __func__);

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, false);
  aopOp (result, ic, false);

  if ((AOP_TYPE (result) == AOP_REG) && (AOP (result)->aopu.aop_reg[0]->rIdx == A_IDX))
    resultInA = true;

  size = AOP_SIZE (result);
  offset = size - 1;

  shift = "lsr";
  if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {
      while (size--)
        {
          rmwWithAop (shift, AOP (result), offset--);
          shift = "ror";
        }
    }
  else
    {
      while (size--)
        {
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          rmwWithReg (shift, m6502_reg_a);
          storeRegToAop (m6502_reg_a, AOP (result), offset--);
          m6502_freeReg (m6502_reg_a);
          shift = "ror";
        }
    }

  if ((!m6502_reg_a->isFree) || resultInA)
    {
      pushReg (m6502_reg_a, true);
      needpula = true;
    }

  /* now we need to put the carry into the
     highest order byte of the result */
  offset = AOP_SIZE (result) - 1;
  loadRegFromConst(m6502_reg_a, 0);
  emit6502op ("ror", "a");
  if (resultInA)
    {
      // FIXME: unimplemented
      emitcode ("ora10", "1,s");
      pullNull (1);
      m6502_dirtyReg (m6502_reg_a);
      needpula = false;
    }
  else
    accopWithAop ("ora", AOP (result), offset);
  storeRegToAop (m6502_reg_a, AOP (result), offset);

  pullOrFreeReg (m6502_reg_a, needpula);

  freeAsmop (left, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genRLC - generate code for rotate left with carry               */
/*-----------------------------------------------------------------*/
static void
genRLC (iCode * ic)
{
  operand *left, *result;
  int size, offset = 0;
  char *shift;
  bool resultInA = false;
  bool needpula = false;

  emitComment (TRACEGEN, __func__);

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, false);
  aopOp (result, ic, false);

  if ((AOP_TYPE (result) == AOP_REG) && (AOP (result)->aopu.aop_reg[0]->rIdx == A_IDX))
    resultInA = true;

  size = AOP_SIZE (result);
  offset = 0;

  shift = "asl";
  if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {
      while (size--)
        {
          rmwWithAop (shift, AOP (result), offset++);
          shift = "rol";
        }
    }
  else
    {
      while (size--)
        {
          loadRegFromAop (m6502_reg_a, AOP (left), offset);
          rmwWithReg (shift, m6502_reg_a);
          storeRegToAop (m6502_reg_a, AOP (result), offset++);
          m6502_freeReg (m6502_reg_a);
          shift = "rol";
        }
    }

  if ((!m6502_reg_a->isFree) || resultInA)
    {
      pushReg (m6502_reg_a, true);
      needpula = true;
    }

  /* now we need to put the carry into the
     lowest order byte of the result */
  offset = 0;
  loadRegFromConst(m6502_reg_a, 0);
  emit6502op ("rol", "a");
  if (resultInA)
    {
      // FIXME: unimplemented
      emitcode ("ora11", "1,s");
      pullNull (1);
      m6502_dirtyReg (m6502_reg_a);
      needpula = false;
    }
  else
    accopWithAop ("ora", AOP (result), offset);
  storeRegToAop (m6502_reg_a, AOP (result), offset);

  pullOrFreeReg (m6502_reg_a, needpula);

  freeAsmop (left, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* AccLsh - left shift accumulator by known count                  */
/*-----------------------------------------------------------------*/
static void
AccLsh (int shCount)
{
  int i;

  shCount &= 0x0007;            // shCount : 0..7

  /* For shift counts of 6 and 7, the unrolled loop is never optimal.      */
  switch (shCount)
    {
    case 6:
      emit6502op ("ror", "a");
      emit6502op ("ror", "a");
      emit6502op ("ror", "a");
      emit6502op ("and", "#0xc0");
      /* total: 8 cycles, 5 bytes */
      return;
    case 7:
      emit6502op ("ror", "a");
      loadRegFromConst(m6502_reg_a, 0);
      emit6502op ("ror", "a");
      /* total: 6 cycles, 4 bytes */
      return;
    }

  /* asl a is 2 cycles and 1 byte, so an unrolled loop is the   */
  /* fastest and shortest (shCount<6).                            */
  for (i = 0; i < shCount; i++)
    emit6502op ("asl", "a");
}


/*-----------------------------------------------------------------*/
/* AccSRsh - signed right shift accumulator by known count         */
/*-----------------------------------------------------------------*/
static void
AccSRsh (int shCount)
{
  int i;

  shCount &= 0x0007;            // shCount : 0..7

  if (shCount == 7)
    {
      emit6502op ("rol", "a");
      loadRegFromConst(m6502_reg_a, 0);
      emit6502op ("adc", "#0xff");
      emit6502op ("eor", "#0xff");
      return;
    }
  if (shCount == 6) {
      symbol *tlbl = safeNewiTempLabel (NULL);
      emit6502op ("ora", "#0x3f");
      emit6502op ("sec", "");
      emit6502op ("bmi", "%05d$", safeLabelKey2num (tlbl->key));
      emit6502op ("and", "#0xc0");
      emit6502op ("clc", "");
      safeEmitLabel(tlbl);
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      return;
  }
  // TODO: optimize? (asr?)
  for (i = 0; i < shCount; i++) {
    emit6502op ("cmp", "#0x80");
    emit6502op ("ror", "a");
  }
}

/*-----------------------------------------------------------------*/
/* AccRsh - right shift accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void
AccRsh (int shCount, bool sign)
{
  int i;

  if (sign)
    {
      AccSRsh (shCount);
      return;
    }

  shCount &= 0x0007;            // shCount : 0..7

  /* For shift counts of 6 and 7, the unrolled loop is never optimal.      */
  switch (shCount)
    {
    case 6:
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("rol", "a");
      emit6502op ("and", "#0x03");
      /* total: 8 cycles, 5 bytes */
      return;
    case 7:
      emit6502op ("rol", "a");
      loadRegFromConst(m6502_reg_a, 0);
      emit6502op ("rol", "a");
      /* total: 6 cycles, 4 bytes */
      return;
    }

  /* lsra is 2 cycles and 1 byte, so an unrolled loop is the      */
  /* the fastest and shortest (shCount<6).            */
  for (i = 0; i < shCount; i++)
    emit6502op ("lsr", "a");
}


/*-----------------------------------------------------------------*/
/* XAccLsh - left shift register pair XA by known count            */
/*-----------------------------------------------------------------*/
static void
XAccLsh (int shCount)
{
  int i;

  shCount &= 0x000f;            // shCount : 0..15

  if (shCount >= 8)
    {
      AccLsh (shCount - 8);
      transferRegReg (m6502_reg_a, m6502_reg_x, false);
      loadRegFromConst (m6502_reg_a, 0);
      return;
    }

  /* if we can beat 2n cycles or bytes for some special case, do it here */
  switch (shCount)
    {
    case 7:
      storeRegTemp(m6502_reg_x, true);
      emit6502op ("lsr", TEMPFMT, _G.tempOfs - 1);
      dirtyRegTemp(_G.tempOfs - 1);
      rmwWithReg ("ror", m6502_reg_a);
      transferRegReg (m6502_reg_a, m6502_reg_x, false);
      loadRegFromConst (m6502_reg_a, 0);
      rmwWithReg ("ror", m6502_reg_a);
      loadRegTemp(NULL);
      return;
    case 0:
      return;
    default:
  /* lsla/rolx is only 2 cycles and bytes, so an unrolled loop is often  */
  /* the fastest and shortest.                                           */
      storeRegTemp(m6502_reg_x, true);

  for (i = 0; i < shCount; i++)
    {
      rmwWithReg ("asl", m6502_reg_a);
      emit6502op ("rol", TEMPFMT, _G.tempOfs - 1);
      dirtyRegTemp(_G.tempOfs - 1);
    }
  loadRegTemp(m6502_reg_x);

    }
}

/*-----------------------------------------------------------------*/
/* XAccSRsh - signed right shift register pair XA by known count   */
/*-----------------------------------------------------------------*/
static void
XAccSRsh (int shCount)
{
  symbol *tlbl;
  int i;

  shCount &= 0x000f;            // shCount : 0..7

  /* if we can beat 2n cycles or bytes for some special case, do it here */
  switch (shCount) // TODO
    {
    case 15:
    case 14:
    case 13:
    case 12:
    case 11:
    case 10:
    case 9:
      transferRegReg (m6502_reg_x, m6502_reg_a, false);
      loadRegFromConst (m6502_reg_x, 0);
      AccSRsh (shCount - 8);
      tlbl = safeNewiTempLabel (NULL);
      emit6502op ("bpl", "%05d$", safeLabelKey2num (tlbl->key));
      loadRegFromConst (m6502_reg_x, 0xff);
      safeEmitLabel(tlbl);
      break;
    case 8:
      transferRegReg (m6502_reg_x, m6502_reg_a, false);
      loadRegFromConst (m6502_reg_x, 0);
      emit6502op("cmp","#0x00");
      tlbl = safeNewiTempLabel (NULL);
      emit6502op ("bpl", "%05d$", safeLabelKey2num (tlbl->key));
      loadRegFromConst (m6502_reg_x, 0xff);
      safeEmitLabel(tlbl);
      break;

    default:
      /* asrx/rora is only 2 cycles and bytes, so an unrolled loop is often  */
      /* the fastest and shortest.                                           */
      storeRegTemp(m6502_reg_x, true);
      for (i = 0; i < shCount; i++)
        {
          // TODO: this is so bad
          emit6502op ("cpx", "#0x80");
          emit6502op ("ror", TEMPFMT, _G.tempOfs - 1);
          dirtyRegTemp(_G.tempOfs - 1);
          rmwWithReg ("ror", m6502_reg_a);
        }
      loadRegTemp(m6502_reg_x);
    }
}

/*-----------------------------------------------------------------*/
/* XAccRsh - right shift register pair XA by known count           */
/*-----------------------------------------------------------------*/
static void
XAccRsh (int shCount, bool sign)
{
  int i;

  if (sign)
    {
      XAccSRsh (shCount);
      return;
    }

  shCount &= 0x000f;            // shCount : 0..f

  /* if we can beat 2n cycles or bytes for some special case, do it here */
 switch (shCount)
   {
    case 15:
    case 14:
    case 13:
    case 12:
    case 11:
    case 10:
    case 9:
    case 8:
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      AccRsh (shCount - 8, false);
      loadRegFromConst (m6502_reg_x, 0);
      break;
    case 7:
      storeRegTemp(m6502_reg_x, true);
      rmwWithReg ("rol", m6502_reg_a);
      emit6502op ("rol", TEMPFMT, _G.tempOfs - 1);
      dirtyRegTemp(_G.tempOfs - 1);
      loadRegFromConst (m6502_reg_a, 0);
      rmwWithReg ("rol", m6502_reg_a);
      transferRegReg(m6502_reg_a, m6502_reg_x, true);
      loadRegTemp(m6502_reg_a);
      break;
    case 0:
      break;
    default:
      /* lsrx/rora is only 2 cycles and bytes, so an unrolled loop is often  */
      /* the fastest and shortest.                                           */
      storeRegTemp(m6502_reg_x, true);
      for (i = 0; i < shCount; i++)
        {
          emit6502op ("lsr", TEMPFMT, _G.tempOfs - 1);
          dirtyRegTemp(_G.tempOfs - 1);
          rmwWithReg ("ror", m6502_reg_a);
        }
      loadRegTemp(m6502_reg_x);
  }
}


/*-----------------------------------------------------------------*/
/* shiftL1Left2Result - shift left one byte from left to result    */
/*-----------------------------------------------------------------*/
static void
shiftL1Left2Result (operand * left, int offl, operand * result, int offr, int shCount)
{
  // TODO: shift > 2?
  if (sameRegs (AOP (left), AOP (result)) && aopCanShift(AOP(left)) && offr == offl)
    {
      while (shCount--)
        rmwWithAop ("asl", AOP (result), 0);
    }
  else
    {
      bool needpulla = pushRegIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (left), offl);
      /* shift left accumulator */
      AccLsh (shCount);
      storeRegToAop (m6502_reg_a, AOP (result), offr);
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
}

/*-----------------------------------------------------------------*/
/* movLeft2Result - move byte from left to result                  */
/*-----------------------------------------------------------------*/
static void
movLeft2Result (operand * left, int offl, operand * result, int offr, int sign)
{
  if (!sameRegs (AOP (left), AOP (result)) || (offl != offr))
    {
      transferAopAop (AOP (left), offl, AOP (result), offr);
    }
}


/*-----------------------------------------------------------------*/
/* shiftL2Left2Result - shift left two bytes from left to result   */
/*-----------------------------------------------------------------*/
static void
shiftL2Left2Result (operand * left, int offl, operand * result, int offr, int shCount)
{
  int i;
  bool needpula = false;
  bool needpulx = false;

  if (!IS_AOP_XA (AOP (left)) && !IS_AOP_A (AOP (left)))
    needpula = pushRegIfUsed (m6502_reg_a);
  else
    needpula = false;
  if (!IS_AOP_XA (AOP (left)))
    needpulx = pushRegIfUsed (m6502_reg_x);
  else
    needpulx = false;

  loadRegFromAop (m6502_reg_xa, AOP (left), offl);

  switch (shCount)
    {
    case 7:
      rmwWithReg ("lsr", m6502_reg_x);
      rmwWithReg ("ror", m6502_reg_a);
      transferRegReg (m6502_reg_a, m6502_reg_x, false);
      loadRegFromConst (m6502_reg_a, 0);
      rmwWithReg ("ror", m6502_reg_a);
      break;
    default:
      for (i = 0; i < shCount; i++)
        {
          rmwWithReg ("asl", m6502_reg_a);
          rmwWithReg ("rol", m6502_reg_x);
        }
    }
  storeRegToAop (m6502_reg_xa, AOP (result), offr);

  pullOrFreeReg (m6502_reg_x, needpulx);
  pullOrFreeReg (m6502_reg_a, needpula);

}



/*-----------------------------------------------------------------*/
/* shiftRLeftOrResult - shift right one byte from left,or to result */
/*-----------------------------------------------------------------*/
static void
shiftRLeftOrResult (operand * left, int offl, operand * result, int offr, int shCount)
{
  loadRegFromAop (m6502_reg_a, AOP (left), offl);
  /* shift left accumulator */
  AccRsh (shCount, false);
  /* or with result */
  accopWithAop ("ora", AOP (result), offr);
  /* back to result */
  storeRegToAop (m6502_reg_a, AOP (result), offr);
  m6502_freeReg (m6502_reg_a);
}

/*-----------------------------------------------------------------*/
/* genlshOne - left shift a one byte quantity by known count       */
/*-----------------------------------------------------------------*/
static void
genlshOne (operand * result, operand * left, int shCount)
{
  emitComment (TRACEGEN, __func__);

  shiftL1Left2Result (left, LSB, result, LSB, shCount);
}

/*-----------------------------------------------------------------*/
/* genlshTwo - left shift two bytes by known amount != 0           */
/*-----------------------------------------------------------------*/
static void
genlshTwo (operand * result, operand * left, int shCount)
{
  bool needpulla, needpullx;

  emitComment (TRACEGEN, __func__);

  /* if shCount >= 8 */
  if (shCount >= 8)
    {
      shCount -= 8;
      // TODO
      needpulla = pushRegIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, AOP (left), 0);
          AccLsh (shCount);
          storeRegToAop (m6502_reg_a, AOP (result), 1);
      storeConstToAop (0, AOP (result), LSB);
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
  /*  1 <= shCount <= 7 */
  // TODO: count > 2 efficient?
  else if (sameRegs (AOP (left), AOP (result)) && aopCanShift(AOP(result)) && shCount <= 4)
    {
      while (shCount--) {
        rmwWithAop ("asl", AOP (result), 0);
        rmwWithAop ("rol", AOP (result), 1);
      }
    }
  else
    {
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      needpullx = storeRegTempIfSurv (m6502_reg_x);
      loadRegFromAop (m6502_reg_xa, AOP (left), 0);
      XAccLsh (shCount);
      storeRegToFullAop (m6502_reg_xa, AOP (result), 0);
// FIXME: check for loadOrFree
      loadOrFreeRegTemp (m6502_reg_x, needpullx);
      loadOrFreeRegTemp (m6502_reg_a, needpulla);
    }
}

/*-----------------------------------------------------------------*/
/* shiftLLong - shift left one long from left to result            */
/* offr = LSB or MSB16                                             */
/*-----------------------------------------------------------------*/
static void
shiftLLong (operand * left, operand * result, int offr)
{
  bool needpulla = false;
  bool needloadx = false;

  needpulla = pushRegIfUsed (m6502_reg_a);

  switch(offr) {
  case LSB:
    loadRegFromAop (m6502_reg_a, AOP (left), 0);
    rmwWithReg ("asl", m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (result), 0);
    loadRegFromAop (m6502_reg_a, AOP (left), 1);
    rmwWithReg ("rol", m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (result), 1);
    loadRegFromAop (m6502_reg_a, AOP (left), 2);
    rmwWithReg ("rol", m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (result), 2);
    loadRegFromAop (m6502_reg_a, AOP (left), 3);
    rmwWithReg ("rol", m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (result), 3);
    break;
  case MSB16:
    needloadx = storeRegTempIfUsed (m6502_reg_x);
    loadRegFromAop (m6502_reg_a, AOP (left), 0);
    rmwWithReg ("asl", m6502_reg_a);
    loadRegFromAop (m6502_reg_x, AOP (left), 1);
    storeRegToAop (m6502_reg_a, AOP (result), 1);
    transferRegReg (m6502_reg_x, m6502_reg_a, true);
    rmwWithReg ("rol", m6502_reg_a);
    loadRegFromAop (m6502_reg_x, AOP (left), 2);
    storeRegToAop (m6502_reg_a, AOP (result), 2);
    transferRegReg (m6502_reg_x, m6502_reg_a, true);
    rmwWithReg ("rol", m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (result), 3);
    storeConstToAop (0, AOP (result), 0);    
    break;
  }
    
  loadOrFreeRegTemp (m6502_reg_x, needloadx);
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/*-----------------------------------------------------------------*/
/* genlshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genlshFour (operand * result, operand * left, int shCount)
{
  emitComment (TRACEGEN, __func__);
  emitComment (TRACEGEN, "  %s - shift=%d", __func__, shCount);

  if(AOP_SIZE (result)!=4) abort();

  /* TODO: deal with the &result == &left case */

  /* if shifting more that 3 bytes */
  if (shCount >= 24)
    {
      shCount -= 24;
      if (shCount)
        /* lowest order of left goes to the highest
           order of the destination */
        shiftL1Left2Result (left, LSB, result, MSB32, shCount);
      else
        movLeft2Result (left, LSB, result, MSB32, 0);
      storeConstToAop (0, AOP (result), LSB);
      storeConstToAop (0, AOP (result), MSB16);
      storeConstToAop (0, AOP (result), MSB24);
      return;
    }

  /* more than two bytes */
  else if (shCount >= 16)
    {
      /* lower order two bytes goes to higher order two bytes */
      shCount -= 16;
      /* if some more remaining */
      if (shCount)
        shiftL2Left2Result (left, LSB, result, MSB24, shCount);
      else
        {
          movLeft2Result (left, MSB16, result, MSB32, 0);
          movLeft2Result (left, LSB, result, MSB24, 0);
        }
      storeConstToAop (0, AOP (result), LSB);
      storeConstToAop (0, AOP (result), MSB16);
      return;
    }

  /* if more than 1 byte */
  else if (shCount >= 8)
    {
      /* lower order three bytes goes to higher order  three bytes */
      shCount -= 8;
      if (shCount == 0)
        {
          movLeft2Result (left, MSB24, result, MSB32, 0);
          movLeft2Result (left, MSB16, result, MSB24, 0);
          movLeft2Result (left, LSB, result, MSB16, 0);
          storeConstToAop (0, AOP (result), LSB);
       }
      else if (shCount == 1)
        shiftLLong (left, result, MSB16);
      else
        {
          shiftL2Left2Result (left, MSB16, result, MSB24, shCount);
          shiftL1Left2Result (left, LSB, result, MSB16, shCount);
          shiftRLeftOrResult (left, LSB, result, MSB24, 8 - shCount);
          storeConstToAop (0, AOP (result), LSB);
        }
    }

  /* 1 <= shCount <= 2 */
    else if (shCount <= 2)
    {
      shiftLLong (left, result, LSB);
      if (shCount == 2)
        shiftLLong (result, result, LSB);
	} 
  else
    {
  /* 3 <= shCount <= 7, optimize */
#if 1
      shiftLLong (left, result, LSB);	
      while(--shCount)
        shiftLLong (result , result, LSB);	
#else
      // FIXME: bug 2825
      shiftL2Left2Result (left, MSB24, result, MSB24, shCount);
      shiftRLeftOrResult (left, MSB16, result, MSB24, 8 - shCount);
      shiftL2Left2Result (left, LSB, result, LSB, shCount);
#endif
    }
}

/*-----------------------------------------------------------------*/
/* genLeftShiftLiteral - left shifting by known count              */
/*-----------------------------------------------------------------*/
static void
genLeftShiftLiteral (operand * left, operand * right, operand * result, iCode * ic)
{
  int shCount = (int) ulFromVal (AOP (right)->aopu.aop_lit);
  int size;

  emitComment (TRACEGEN, __func__);

  freeAsmop (right, NULL, ic, true);

  aopOp (left, ic, false);
  aopOp (result, ic, false);

  size = AOP_SIZE (result);

#if VIEW_SIZE
  emitComment (TRACEGEN|VVDBG, "  shift left ", "result %d, left %d", size, AOP_SIZE (left));
#endif

  if (shCount == 0)
    {
      genCopy (result, left);
    }
  else if (shCount >= (size * 8))
    {
      while (size--)
        storeConstToAop (0, AOP (result), size);
    }
  else
    {
      switch (size)
        {
        case 1:
          genlshOne (result, left, shCount);
          break;

        case 2:
          genlshTwo (result, left, shCount);
          break;

        case 4:
          genlshFour (result, left, shCount);
          break;
        default:
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "*** ack! mystery literal shift!\n");
          break;
        }
    }
  freeAsmop (left, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genLeftShift - generates code for left shifting                 */
/*-----------------------------------------------------------------*/
static void
genLeftShift (iCode * ic)
{
  operand *left, *right, *result;
  int size, offset;
  symbol *tlbl, *tlbl1;
  char *shift;
  asmop *aopResult;
  reg_info *countreg = NULL;
  int count_offset = 0;

  emitComment (TRACEGEN, __func__);

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, false);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      genLeftShiftLiteral (left, right, result, ic);
      return;
    }

  /* shift count is unknown then we have to form
     a loop get the loop count in X : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  aopOp (result, ic, false);
  aopOp (left, ic, false);
  aopResult = AOP (result);

// TODO
#if 0
  if (sameRegs (AOP (right), AOP (result)) || regsInCommon (right, result) || IS_AOP_XA (AOP (result)) || isOperandVolatile (result, false))
    aopResult = forceZeropageAop (AOP (result), sameRegs (AOP (left), AOP (result)));
#endif

  /* load the count register */
  if (m6502_reg_y->isDead && !IS_AOP_WITH_Y (AOP (result)) && !IS_AOP_WITH_Y (AOP (left)))
    countreg = m6502_reg_y;
  else if (m6502_reg_x->isDead && !IS_AOP_WITH_X (AOP (result)) && !IS_AOP_WITH_X (AOP (left)))
    countreg = m6502_reg_x;
  else if (m6502_reg_a->isDead && !IS_AOP_WITH_A (AOP (result)) && !IS_AOP_WITH_A (AOP (left)))
    countreg = m6502_reg_a;
    
  if(countreg)
    {
      countreg->isFree = false;
      emitComment (TRACEGEN|VVDBG, "  load countreg");
      loadRegFromAop (countreg, AOP (right), 0);
    } else {
      // FIXME FIXME : something odd with load/store/pull reg
      emitComment (TRACEGEN|VVDBG, "  count is not a register");
      bool needpully = pushRegIfUsed (m6502_reg_y);
      loadRegFromAop (m6502_reg_y, AOP (right), 0);
      count_offset=_G.tempOfs;
      storeRegTemp (m6502_reg_y, true);
      pullOrFreeReg(m6502_reg_y, needpully);
    }

  /* now move the left to the result if they are not the
     same */
  if (IS_AOP_YX (AOP (result)))
    loadRegFromAop (m6502_reg_yx, AOP (left), 0);
  else if (IS_AOP_AX (AOP (result)) && IS_AOP_XA (AOP (left)) || IS_AOP_XA (AOP (result)) && IS_AOP_AX (AOP (left)))
    {
      swapXA();
    }
  else if (!sameRegs (AOP (left), aopResult))
    {
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          transferAopAop (AOP (left), offset, aopResult, offset);
          offset++;
        }
    }
  freeAsmop (left, NULL, ic, true);
  AOP (result) = aopResult;

  tlbl = safeNewiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = 0;
  tlbl1 = safeNewiTempLabel (NULL);

  if (countreg)
    {
      // TODO: can combine these with load of count reg?
      if (countreg == m6502_reg_a)
        emit6502op("cmp", "#0x00");
      if (countreg == m6502_reg_x)
        emit6502op("cpx", "#0x00");
      if (countreg == m6502_reg_y)
        emit6502op("cpy", "#0x00");
      emitBranch ("beq", tlbl1);
    }
  else
    {
      emit6502op ("dec", TEMPFMT, count_offset);
      // FIXME: could keep it literal
      dirtyRegTemp(_G.tempOfs - 1);
      emitBranch ("bmi", tlbl1);
    }

  safeEmitLabel (tlbl);

  shift = "asl";
  for (offset = 0; offset < size; offset++)
    {
      rmwWithAop (shift, AOP (result), offset);
      shift = "rol";
    }

  if (countreg) {
      rmwWithReg("dec", countreg);
      emit6502op("bne", "%05d$", safeLabelKey2num (tlbl->key));
  } else {
      emit6502op("dec", TEMPFMT, count_offset );
      // FIXME: could keep it literal
      dirtyRegTemp(_G.tempOfs - 1);
      emit6502op("bpl", "%05d$", safeLabelKey2num (tlbl->key));
    }

  safeEmitLabel (tlbl1);

  if (!countreg) {
    emitComment (TRACEGEN|VVDBG, "  pull null (1) ");
    loadRegTemp(NULL);
  }

  freeAsmop (result, NULL, ic, true);
  freeAsmop (right, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genrshOne - right shift a one byte quantity by known count      */
/*-----------------------------------------------------------------*/
static void
genrshOne (operand * result, operand * left, int shCount, int sign)
{
  bool needpulla;
  emitComment (TRACEGEN, __func__);
  if (shCount==0) return;
  needpulla = pushRegIfSurv (m6502_reg_a);
  loadRegFromAop (m6502_reg_a, AOP (left), 0);
  AccRsh (shCount, sign);
  storeRegToFullAop (m6502_reg_a, AOP (result), sign);
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/*-----------------------------------------------------------------*/
/* genrshTwo - right shift two bytes by known amount != 0          */
/*-----------------------------------------------------------------*/
static void
genrshTwo (operand * result, operand * left, int shCount, int sign)
{
  bool needpulla, needpullx;
  emitComment (TRACEGEN, __func__);

  /* if shCount >= 8 */
  if (shCount >= 8)
    {
      if (shCount != 8 || sign)
        {
          needpulla = pushRegIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, AOP (left), 1);
          AccRsh (shCount - 8, sign);
          storeRegToFullAop (m6502_reg_a, AOP (result), sign);
          pullOrFreeReg (m6502_reg_a, needpulla);
        }
      else
        {
          transferAopAop (AOP (left), 1, AOP (result), 0);
          storeConstToAop (0, AOP (result), 1);
        }
    }
  /*  1 <= shCount <= 7 */
  else
    {
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      needpullx = storeRegTempIfSurv (m6502_reg_x);
      loadRegFromAop (m6502_reg_xa, AOP (left), 0);
      XAccRsh (shCount, sign);
      storeRegToAop (m6502_reg_xa, AOP (result), 0);
      loadOrFreeRegTemp (m6502_reg_x, needpullx);
      loadOrFreeRegTemp (m6502_reg_a, needpulla);
    }
}

/*-----------------------------------------------------------------*/
/* shiftRLong - shift right one long from left to result           */
/* offl = LSB or MSB16                                             */
/*-----------------------------------------------------------------*/
static void
shiftRLong (operand * left, int offl, operand * result, int sign)
{
  bool needpulla = pushRegIfSurv (m6502_reg_a);
  bool needloadx = false;

  switch(offl)
    {
    case LSB:
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      if(sign)
        {
          emit6502op("cmp","#0x80");
          rmwWithReg ("ror", m6502_reg_a);
        }
      else 
        rmwWithReg ("lsr", m6502_reg_a);

      storeRegToAop (m6502_reg_a, AOP (result), 3);
      loadRegFromAop (m6502_reg_a, AOP (left), 2);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      loadRegFromAop (m6502_reg_a, AOP (left), 1);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      loadRegFromAop (m6502_reg_a, AOP (left), 0);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      break;
    case MSB16:
      needloadx = storeRegTempIfSurv (m6502_reg_x);

      loadRegFromConst(m6502_reg_x,0);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      if(sign) {
          symbol *tlbl = safeNewiTempLabel (NULL);
          emit6502op("clc","");
          emit6502op("bpl","%05d$", safeLabelKey2num (tlbl->key));
          emit6502op("sec","");
          loadRegFromConst(m6502_reg_x,0xff);
          safeEmitLabel(tlbl);
          rmwWithReg ("ror", m6502_reg_a);
      } else {
          rmwWithReg ("lsr", m6502_reg_a);
      }
      storeRegToAop (m6502_reg_x, AOP (result), 3);    

      loadRegFromAop (m6502_reg_x, AOP (left), 2);
      storeRegToAop (m6502_reg_a, AOP (result), 2);
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      rmwWithReg ("ror", m6502_reg_a);

      loadRegFromAop (m6502_reg_x, AOP (left), 1);
      storeRegToAop (m6502_reg_a, AOP (result), 1);
      transferRegReg(m6502_reg_x, m6502_reg_a, true);
      rmwWithReg ("ror", m6502_reg_a);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      break;
 //   default:
      // error
    }
  loadOrFreeRegTemp (m6502_reg_x, needloadx);
  pullOrFreeReg (m6502_reg_a, needpulla);
}

/*-----------------------------------------------------------------*/
/* genrshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genrshFour (operand * result, operand * left, int shCount, int sign)
{
  bool needpulla = false;
  bool needpullx = false;

  /* TODO: handle cases where left == result */

  emitComment (TRACEGEN, __func__);
  emitComment (TRACEGEN, "  %s - shift=%d", __func__, shCount);

  /* if shifting more that 3 bytes */
  if (shCount >= 24)
    {
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (left), 3);
      AccRsh (shCount - 24, sign);
      storeRegToFullAop (m6502_reg_a, AOP (result), sign);
    }
  else if (shCount >= 16)
    {
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      needpullx = storeRegTempIfSurv (m6502_reg_x);
      loadRegFromAop (m6502_reg_xa, AOP (left), 2);
      XAccRsh (shCount - 16, sign);
      storeRegToFullAop (m6502_reg_xa, AOP (result), sign);
    }
  else if (shCount >= 8)
    {
      if (shCount == 8)
        {
          needpulla = storeRegTempIfSurv (m6502_reg_a);
          transferAopAop (AOP (left), 1, AOP (result), 0);
          transferAopAop (AOP (left), 2, AOP (result), 1);
          loadRegFromAop (m6502_reg_a, AOP (left), 3);
          storeRegToAop (m6502_reg_a, AOP (result), 2);
          storeRegSignToUpperAop (m6502_reg_a, AOP (result), 3, sign);
        }
      else if (shCount == 9)
        {
          shiftRLong (left, MSB16, result, sign);
          return;
        }
      else
        {
          needpulla = storeRegTempIfSurv (m6502_reg_a);
          needpullx = storeRegTempIfSurv (m6502_reg_x);
          loadRegFromAop (m6502_reg_xa, AOP (left), 1);
          XAccRsh (shCount - 8, false);
          storeRegToAop (m6502_reg_xa, AOP (result), 0);
          loadRegFromAop (m6502_reg_x, AOP (left), 3);
          loadRegFromConst (m6502_reg_a, 0);
          XAccRsh (shCount - 8, sign);
          accopWithAop ("ora", AOP (result), 1);
          storeRegToAop (m6502_reg_xa, AOP (result), 1);
          storeRegSignToUpperAop (m6502_reg_x, AOP (result), 3, sign);
        }
    }
  else
    {
      /* 1 <= shCount <= 7 */
      if (shCount == 1)
        {
          shiftRLong (left, LSB, result, sign);
          return;
        }
      else
        {
          needpulla = storeRegTempIfSurv (m6502_reg_a);
          needpullx = storeRegTempIfSurv (m6502_reg_x);
          loadRegFromAop (m6502_reg_xa, AOP (left), 0);
          XAccRsh (shCount, false);
          storeRegToAop (m6502_reg_xa, AOP (result), 0);
          loadRegFromAop (m6502_reg_a, AOP (left), 2);
          AccLsh (8 - shCount);
          accopWithAop ("ora", AOP (result), 1);
          storeRegToAop (m6502_reg_a, AOP (result), 1);
          loadRegFromAop (m6502_reg_xa, AOP (left), 2);
          XAccRsh (shCount, sign);
          storeRegToAop (m6502_reg_xa, AOP (result), 2);
        }
    }
  loadOrFreeRegTemp (m6502_reg_x, needpullx);
  loadOrFreeRegTemp (m6502_reg_a, needpulla);
}

/*-----------------------------------------------------------------*/
/* genRightShiftLiteral - right shifting by known count            */
/*-----------------------------------------------------------------*/
static void
genRightShiftLiteral (operand * left, operand * right, operand * result, iCode * ic, int sign)
{
  int shCount = (int) ulFromVal (AOP (right)->aopu.aop_lit);
  int size;

  emitComment (TRACEGEN, __func__);

  freeAsmop (right, NULL, ic, true);

  aopOp (left, ic, false);
  aopOp (result, ic, false);

#if VIEW_SIZE
  emitComment (TRACEGEN|VVDBG, "  shift right ", "result %d, left %d", AOP_SIZE (result), AOP_SIZE (left));
#endif

  size = getDataSize (left);
  /* test the LEFT size !!! */

  /* I suppose that the left size >= result size */
  if (shCount == 0)
    {
      genCopy (result, left);
    }
  else if (shCount >= (size * 8))
    {
      bool needpulla = pushRegIfSurv (m6502_reg_a);
      if (sign)
        {
          
          /* get sign in acc.7 */
          loadRegFromAop (m6502_reg_a, AOP (left), size - 1);
        }
      addSign (result, LSB, sign);
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
  else
    {
      switch (size)
        {
        case 1:
          genrshOne (result, left, shCount, sign);
          break;

        case 2:
          genrshTwo (result, left, shCount, sign);
          break;

        case 4:
          genrshFour (result, left, shCount, sign);
          break;
        default:
          wassertl (0, "Invalid operand size in right shift.");
          break;
        }
    }
  freeAsmop (left, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);
}

// TODO: can refactor lots of left shift with right shift

/*-----------------------------------------------------------------*/
/* genRightShift - generate code for right shifting                */
/*-----------------------------------------------------------------*/
static void
genRightShift (iCode * ic)
{
  operand *right, *left, *result;
  sym_link *retype;
  int size, offset;
  symbol *tlbl, *tlbl1;
  char *shift;
  bool sign;
  asmop *aopResult;
  reg_info *countreg = NULL;
  int count_offset=0;

  emitComment (TRACEGEN, __func__);

  /* if signed then we do it the hard way preserve the
     sign bit moving it inwards */
  retype = getSpec (operandType (IC_RESULT (ic)));
  sign = !SPEC_USIGN (retype);

  /* signed & unsigned types are treated the same : i.e. the
     signed is NOT propagated inwards : quoting from the
     ANSI - standard : "for E1 >> E2, is equivalent to division
     by 2**E2 if unsigned or if it has a non-negative value,
     otherwise the result is implementation defined ", MY definition
     is that the sign does not get propagated */

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, false);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      genRightShiftLiteral (left, right, result, ic, sign);
      return;
    }

  /* shift count is unknown then we have to form
     a loop get the loop count in X : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  aopOp (result, ic, false);
  aopOp (left, ic, false);
  aopResult = AOP (result);

// TODO
#if 0
  if (sameRegs (AOP (right), AOP (result)) || regsInCommon (right, result) || IS_AOP_XA (AOP (result)) || isOperandVolatile (result, false))
    aopResult = forceZeropageAop (AOP (result), sameRegs (AOP (left), AOP (result)));
#endif

  /* load the count register */
  if (m6502_reg_y->isDead && !IS_AOP_WITH_Y (AOP (result)) && !IS_AOP_WITH_Y (AOP (left)))
    countreg = m6502_reg_y;
  else if (m6502_reg_x->isDead && !IS_AOP_WITH_X (AOP (result)) && !IS_AOP_WITH_X (AOP (left)))
    countreg = m6502_reg_x;
  else if (m6502_reg_a->isDead && !IS_AOP_WITH_A (AOP (result)) && !IS_AOP_WITH_A (AOP (left)))
    countreg = m6502_reg_a;
    
  if(countreg)
    {
      countreg->isFree = false;
      loadRegFromAop (countreg, AOP (right), 0);
    } else {
      // FIXME FIXME
      emitComment (TRACEGEN|VVDBG, "  count is not a register");
      bool needpully = pushRegIfUsed (m6502_reg_y);
      loadRegFromAop (m6502_reg_y, AOP (right), 0);
      count_offset=_G.tempOfs;
      storeRegTemp (m6502_reg_y, true);
      pullOrFreeReg(m6502_reg_y, needpully);
    }

  /* now move the left to the result if they are not the
     same */
  // TODO: can we keep it in A?
  if (IS_AOP_YX (AOP (result)))
    {
      loadRegFromAop (m6502_reg_yx, AOP (left), 0);
    }
  else if (IS_AOP_AX (AOP (result)) && IS_AOP_XA (AOP (left)) || IS_AOP_XA (AOP (result)) && IS_AOP_AX (AOP (left)))
    {
      swapXA();
    }
  else if (!sameRegs (AOP (left), aopResult))
    {
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          transferAopAop (AOP (left), offset, aopResult, offset);
          offset++;
        }
    }
  freeAsmop (left, NULL, ic, true);
  AOP (result) = aopResult;

  tlbl = safeNewiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = 0;
  tlbl1 = safeNewiTempLabel (NULL);

  if (countreg) // TODO
    {
      // TODO: combine with load index?
      if (countreg == m6502_reg_a)
        emit6502op("cmp", "#0x00");
      if (countreg == m6502_reg_x)
        emit6502op("cpx", "#0x00");
      if (countreg == m6502_reg_y)
        emit6502op("cpy", "#0x00");
      emitBranch ("beq", tlbl1);
    }
  else
    {
      emit6502op ("dec", TEMPFMT, count_offset);
      // could keep it literal
      dirtyRegTemp(_G.tempOfs - 1);
      emitBranch ("bmi", tlbl1);
    }

  safeEmitLabel (tlbl);

  shift = sign ? "asr" : "lsr";
  for (offset = size - 1; offset >= 0; offset--)
    {
      rmwWithAop (shift, AOP (result), offset);
      shift = "ror";
    }

  if (countreg) {
      rmwWithReg("dec", countreg);
        emit6502op("bne", "%05d$", safeLabelKey2num (tlbl->key));
  } else {
      emit6502op("dec", TEMPFMT, count_offset );
      // FIXME: could keep it literal
      dirtyRegTemp(_G.tempOfs - 1);
      emit6502op("bpl", "%05d$", safeLabelKey2num (tlbl->key));
    }

  safeEmitLabel (tlbl1);

  if (!countreg) 
    loadRegTemp(NULL);

  freeAsmop (result, NULL, ic, true);
  freeAsmop (right, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* decodePointerOffset - decode a pointer offset operand into a    */
/*                    literal offset and a rematerializable offset */
/*-----------------------------------------------------------------*/
static void
decodePointerOffset (operand * opOffset, int * litOffset, char ** rematOffset)
{
  *litOffset = 0;
  *rematOffset = NULL;

  if (!opOffset)
    return;

  if (IS_OP_LITERAL (opOffset))
    {
      *litOffset = (int)operandLitValue (opOffset);
    }
  else if (IS_ITEMP (opOffset) && OP_SYMBOL (opOffset)->remat)
    {
      asmop * aop = aopForRemat (OP_SYMBOL (opOffset));

      if (aop->type == AOP_LIT)
        *litOffset = (int) floatFromVal (aop->aopu.aop_lit);
      else if (aop->type == AOP_IMMD)
        *rematOffset = aop->aopu.aop_immd.aop_immd1;
    }
  else
    wassertl (0, "Pointer get/set with non-constant offset");
}


/*-----------------------------------------------------------------*/
/* genUnpackBits - generates code for unpacking bits               */
/*-----------------------------------------------------------------*/
static void
genUnpackBits (operand * result, operand * left, operand * right, iCode * ifx)
{
  int offset = 0;               /* result byte offset */
  int rsize;                    /* result size */
  int rlen = 0;                 /* remaining bitfield length */
  sym_link *etype;              /* bitfield type information */
  unsigned blen;                /* bitfield length */
  unsigned bstr;                /* bitfield starting bit within byte */
  bool needpulla = false;
  bool needpully = false;
  bool needpullx = false;
  int litOffset = 0;
  char * rematOffset = NULL;

  emitComment (TRACEGEN, __func__);

  decodePointerOffset (right, &litOffset, &rematOffset);
  etype = getSpec (operandType (result));
  rsize = getSize (operandType (result));
  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  needpulla = pushRegIfSurv (m6502_reg_a);

  if (!IS_AOP_YX (AOP (left)))
    {
      needpullx = pushRegIfSurv (m6502_reg_x);
      needpully = pushRegIfSurv (m6502_reg_y);
    }

  /* if the operand is already in yx
     then we do nothing else we move the value to yx */
  loadRegFromAop (m6502_reg_yx, AOP (left), 0);
  /* so yx now contains the address */

  if (ifx && blen <= 8)
    {
      loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
      if (blen < 8)
        {
          emit6502op ("and", IMMDFMT, (((unsigned char) - 1) >> (8 - blen)) << bstr);
        }
//      emit6502op("php", "");//TODO
      pullOrFreeReg (m6502_reg_y, needpully);
      pullOrFreeReg (m6502_reg_x, needpullx);
      pullOrFreeReg (m6502_reg_a, needpulla);
//      emit6502op("plp", "");
      genIfxJump (ifx, "a");
      return;
    }
  wassert (!ifx);

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
      AccRsh (bstr, false);
      emit6502op ("and", IMMDFMT, ((unsigned char) - 1) >> (8 - blen));
      if (!SPEC_USIGN (etype))
        {
          /* signed bitfield */
          symbol *tlbl = safeNewiTempLabel (NULL);

	  // FIXME
          emitcode ("bit11", IMMDFMT, 1 << (blen - 1));
          emitcode ("beq", "%05d$", safeLabelKey2num (tlbl->key));
          emitcode ("ora", IMMDFMT, (unsigned char) (0xff << blen));
          safeEmitLabel (tlbl);
        }
      storeRegToAop (m6502_reg_a, AOP (result), offset++);
      goto finish;
    }

  /* Bit field did not fit in a byte. Copy all
     but the partial byte at the end.  */
  for (rlen = blen; rlen >= 8; rlen -= 8)
    {
      loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
      if (rlen > 8 && AOP_TYPE (result) == AOP_REG)
        pushReg (m6502_reg_a, true);
      else
        storeRegToAop (m6502_reg_a, AOP (result), offset);
      offset++;
      if (rlen > 8)
        {
          litOffset++;
        }
    }

  /* Handle the partial byte at the end */
  if (rlen)
    {
      loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
      emit6502op ("and", IMMDFMT, ((unsigned char) - 1) >> (8 - rlen));
      if (!SPEC_USIGN (etype))
        {
          /* signed bitfield */
          symbol *tlbl = safeNewiTempLabel (NULL);

	  // FIXME: unimplemented
          emitcode ("bit12", IMMDFMT, 1 << (rlen - 1));
          emitcode ("beq", "%05d$", safeLabelKey2num (tlbl->key));
          emitcode ("ora", IMMDFMT, (unsigned char) (0xff << rlen));
          safeEmitLabel (tlbl);
        }
      storeRegToAop (m6502_reg_a, AOP (result), offset++);
    }
  if (blen > 8 && AOP_TYPE (result) == AOP_REG)
    {
      pullReg (AOP (result)->aopu.aop_reg[0]);
    }

finish:
  if (offset < rsize)
    {
      rsize -= offset;
      if (SPEC_USIGN (etype))
        {
          while (rsize--)
            storeConstToAop (0, AOP (result), offset++);
        }
      else
        {
          /* signed bitfield: sign extension with 0x00 or 0xff */
          signExtendA();
          while (rsize--)
            storeRegToAop (m6502_reg_a, AOP (result), offset++);
        }
    }
  pullOrFreeReg (m6502_reg_y, needpully);
  pullOrFreeReg (m6502_reg_x, needpullx);
  pullOrFreeReg (m6502_reg_a, needpulla);
}

// does a BIT A with a constant, even for non-65C02
// TODO: lookup table for each new const?
void bitAConst(int val)
{
  wassertl (val >= 0 && val <= 0xff, "bitAConst()");
  if (IS_MOS65C02) {
    emit6502op ("bit", IMMDFMT, val);
  } else {
    storeRegTemp (m6502_reg_a, true);
    emit6502op ("and", IMMDFMT, val);
    loadRegTempNoFlags (m6502_reg_a, true);
  }
}

/*-----------------------------------------------------------------*/
/* genUnpackBitsImmed - generates code for unpacking bits          */
/*-----------------------------------------------------------------*/
static void
genUnpackBitsImmed (operand * left, operand *right, operand * result, iCode * ic, iCode * ifx)
{
  int size;
  int offset = 0;               /* result byte offset */
  int litOffset = 0;
  char * rematOffset = NULL;
  int rsize;                    /* result size */
  int rlen = 0;                 /* remaining bitfield length */
  sym_link *etype;              /* bitfield type information */
  unsigned blen;                /* bitfield length */
  unsigned bstr;                /* bitfield starting bit within byte */
  asmop *derefaop;
  bool delayed_a = false;
  bool assigned_a = false;
  bool needpulla = false;

  emitComment (TRACEGEN, __func__);

  decodePointerOffset (right, &litOffset, &rematOffset);
  wassert (rematOffset==NULL);

  aopOp (result, ic, true);
  size = AOP_SIZE (result);

  derefaop = aopDerefAop (AOP (left), litOffset);
  freeAsmop (left, NULL, ic, true);
  derefaop->size = size;

  etype = getSpec (operandType (result));
  rsize = getSize (operandType (result));
  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  needpulla = pushRegIfSurv (m6502_reg_a);

  /* if the bitfield is a single bit in the direct page */
  if (blen == 1 && derefaop->type == AOP_DIR)
    {
      if (!ifx && bstr)
        {
          symbol *tlbl = safeNewiTempLabel (NULL);

	  // FIXME: unimplemented
          loadRegFromConst (m6502_reg_a, 0);
          emitcode ("brclr", "#%d,%s,%05d$", bstr, aopAdrStr (derefaop, 0, false), safeLabelKey2num ((tlbl->key)));
          if (SPEC_USIGN (etype))
            rmwWithReg ("inc", m6502_reg_a);
          else
            rmwWithReg ("dec", m6502_reg_a);
          safeEmitLabel (tlbl);
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          if (AOP_TYPE (result) == AOP_REG && AOP(result)->aopu.aop_reg[offset]->rIdx == A_IDX)
            assigned_a = true;
          m6502_freeReg (m6502_reg_a);
          offset++;
          goto finish;
        }
      else if (ifx)
        {
          symbol *tlbl = safeNewiTempLabel (NULL);
          symbol *jlbl;
          char *inst;

	  // FIXME
          if (IC_TRUE (ifx))
            {
              jlbl = IC_TRUE (ifx);
              inst = "brclr";
            }
          else
            {
              jlbl = IC_FALSE (ifx);
              inst = "brset";
            }
          emitcode (inst, "#%d,%s,%05d$", bstr, aopAdrStr (derefaop, 0, false), safeLabelKey2num ((tlbl->key)));
          emitBranch ("jmp", jlbl);
          safeEmitLabel (tlbl);
          ifx->generated = 1;
          offset++;
          goto finish;
        }
    }

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      loadRegFromAop (m6502_reg_a, derefaop, 0);
      if (!ifx)
        {
          // TODO: inefficient if just getting flags
          AccRsh (bstr, false);
          emit6502op ("and", IMMDFMT, ((unsigned char) - 1) >> (8 - blen));
          if (!SPEC_USIGN (etype))
            {
              /* signed bitfield */
              symbol *tlbl = safeNewiTempLabel (NULL);
              bitAConst(1 << (blen - 1));
              emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl->key));
              emit6502op ("ora", IMMDFMT, (unsigned char) (0xff << blen));
              safeEmitLabel (tlbl);
            }
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          if (AOP_TYPE (result) == AOP_REG && AOP(result)->aopu.aop_reg[offset]->rIdx == A_IDX)
            assigned_a = true;
        }
      else
        {
          emit6502op ("and", IMMDFMT, (((unsigned char) - 1) >> (8 - blen)) << bstr);
        }
      offset++;
      goto finish;
    }

  /* Bit field did not fit in a byte. Copy all
     but the partial byte at the end.  */
  for (rlen = blen; rlen >= 8; rlen -= 8)
    {
      if (assigned_a && !delayed_a)
        {
          pushReg (m6502_reg_a, true);
          delayed_a = true;
        }
      loadRegFromAop (m6502_reg_a, derefaop, offset);
      if (!ifx)
        {
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          if (AOP_TYPE (result) == AOP_REG && AOP(result)->aopu.aop_reg[offset]->rIdx == A_IDX)
            assigned_a = true;
        }
      else
        {
          emit6502op ("cmp", "#0x00");
        }
      offset++;
    }

  /* Handle the partial byte at the end */
  if (rlen)
    {
      if (assigned_a && !delayed_a)
        {
          pushReg (m6502_reg_a, true);
          delayed_a = true;
        }
      loadRegFromAop (m6502_reg_a, derefaop, offset);
      emit6502op ("and", IMMDFMT, ((unsigned char) - 1) >> (8 - rlen));
      if (!SPEC_USIGN (etype))
        {
          /* signed bitfield */
          symbol *tlbl = safeNewiTempLabel (NULL);

          bitAConst (1 << (rlen - 1));
          emit6502op ("beq", "%05d$", safeLabelKey2num (tlbl->key));
          emit6502op ("ora", IMMDFMT, (unsigned char) (0xff << rlen));
          safeEmitLabel (tlbl);
        }
      storeRegToAop (m6502_reg_a, AOP (result), offset);
      if (AOP_TYPE (result) == AOP_REG && AOP(result)->aopu.aop_reg[offset]->rIdx == A_IDX)
        assigned_a = true;
      offset++;
    }

finish:
  if (offset < rsize)
    {
      rsize -= offset;
      if (SPEC_USIGN (etype))
        {
          while (rsize--)
            storeConstToAop (0, AOP (result), offset++);
        }
      else
        {
          if (assigned_a && !delayed_a)
            {
              pushReg (m6502_reg_a, true);
              delayed_a = true;
            }

          /* signed bitfield: sign extension with 0x00 or 0xff */
          signExtendA();
          while (rsize--)
            storeRegToAop (m6502_reg_a, AOP (result), offset++);
        }
    }

  freeAsmop (NULL, derefaop, ic, true);
  freeAsmop (result, NULL, ic, true);

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a");
    }
  if (delayed_a)
    pullReg (m6502_reg_a);

    // TODO? wrong plac?
  pullOrFreeReg (m6502_reg_a, needpulla);
}


/*-----------------------------------------------------------------*/
/* genDataPointerGet - generates code when ptr offset is known     */
/*-----------------------------------------------------------------*/
static void
genDataPointerGet (operand * left, operand * right, operand * result, iCode * ic, iCode * ifx)
{
  int size;
  int litOffset = 0;
  char * rematOffset = NULL;
  asmop *derefaop;
  bool needpulla = false;

  emitComment (TRACEGEN, __func__);

  decodePointerOffset (right, &litOffset, &rematOffset);
  wassert (rematOffset==NULL);

  aopOp (result, ic, true);
  size = AOP_SIZE (result);

  // TODO: aopDerefAop(IMMD(_ftest_a_65536_8)), why?
  derefaop = aopDerefAop (AOP (left), litOffset);
  freeAsmop (left, NULL, ic, true);
  derefaop->size = size;
  
  if (ifx)
    needpulla = storeRegTempIfSurv (m6502_reg_a);

  if (IS_AOP_YX (AOP (result)))
    loadRegFromAop (m6502_reg_yx, derefaop, 0);
  else
    while (size--)
      {
        if (!ifx)
          transferAopAop (derefaop, size, AOP (result), size);
        else
          loadRegFromAop (m6502_reg_a, derefaop, size);
      }

  freeAsmop (NULL, derefaop, ic, true);
  freeAsmop (result, NULL, ic, true);

  if (ifx && !ifx->generated)
    {
      loadRegTempNoFlags (m6502_reg_a, needpulla);
      genIfxJump (ifx, "a");
    }
    else
    {
      if (needpulla) loadRegTemp (NULL);
    }
}

/* copy pointer into TEMP+i zero-page location, and load Y if needed */
/* return -1 if can be absolute indexed */
/* left is address, right is value to write or NULL */
static int
preparePointer (operand* left, int offset, char* rematOfs, operand* right)
{
  // TODO: really do we need this?
  asmop *newaop = newAsmop (AOP_DIR);
  newaop->aopu.aop_dir = Safe_calloc (1, 10+4);
  snprintf(newaop->aopu.aop_dir, 14, "(__TEMP+%d)", _G.tempOfs);

  emitComment (TRACEGEN|VVDBG, "      preparePointer: %s", newaop->aopu.aop_dir);
  newaop->size = 2;
  
  /* The rematerialized offset may have a "#" prefix; skip over it */
  if (rematOfs && rematOfs[0] == '#')
    rematOfs++;
  if (rematOfs && !rematOfs[0])
    rematOfs = NULL;

  /* force offset to signed 16-bit range */
  offset &= 0xffff;
  if (offset & 0x8000)
    offset = 0x10000 - offset;

  emitComment (TRACEGEN|VVDBG, "      preparePointer (%s, off=%d, remat=%s) tempOfs %d", aopName(AOP(left)), offset, rematOfs, _G.tempOfs);

  tempRematOfs = rematOfs;
  prepSwapAY = false;

  if(right) {
  emitComment (TRACEGEN|VVDBG, "      preparePointer AOP_TYPE(right)==AOP_REG: %s", (AOP_TYPE(right) == AOP_REG)?"yes":"no");
  emitComment (TRACEGEN|VVDBG, "      preparePointer AOP_SIZE(right)=%d", AOP_SIZE(right));
}
  if(left) {
  emitComment (TRACEGEN|VVDBG, "      preparePointer AOP_TYPE(left)==AOP_REG: %s", (AOP_TYPE(left) == AOP_REG)?"yes":"no");
  emitComment (TRACEGEN|VVDBG, "      preparePointer AOP_SIZE(left)=%d", AOP_SIZE(left));
//  emitComment (TRACEGEN|VVDBG, "      preparePointer (%s, off=%d, remat=%s) tempOfs %d", aopName(AOP(left)), offset, rematOfs, _G.tempOfs);
}
  
#if 1
  // FIXME: this code sometimes improperly detects 8-bit offsets
  // triggers failure on bug-477927 when using zero weight for cycles cost
  // FIXME FIXME: this is a great optimization opportunity
  // 8-bit absolute offset? (remat+offset,y)
  // TODO: better way than checking register?
  if (rematOfs && AOP_TYPE(left) == AOP_REG 
      && (AOP_SIZE(left) == 1 || AOP(left)->aopu.aop_reg[1]->isLitConst && AOP(left)->aopu.aop_reg[1]->litConst == 0))
    {
      emitComment (TRACEGEN|VVDBG, "        preparePointer: 8-bit offset");
      // TODO: what if Y is right and AX is pointer?
      prepSwapAY = right && AOP(left)->aopu.aop_reg[0] == m6502_reg_a && AOP_TYPE(right) == AOP_REG && IS_AOP_WITH_Y(AOP(right));
      if (prepSwapAY) {
        // swap A and Y
        emitComment (TRACEGEN|VVDBG, "        preparePointer: swap A and Y");
        storeRegTemp(m6502_reg_a, true);
        transferRegReg(m6502_reg_y, m6502_reg_a, true);
        loadRegTemp(m6502_reg_y);
      } else {
        // transfer lower byte of offset to Y
        // FIXME: should allow offset in X as well
        transferRegReg(AOP(left)->aopu.aop_reg[0], m6502_reg_y, true);
      }
      return (prepTempOfs = -1);
    }
#endif

  // need to do pointer math?
  if (!rematOfs && offset >= 0 && offset <= 255-4)
    {
      prepTempOfs = _G.tempOfs;
      _G.tempOfs += 2;

      // just copy, we can add the remaining offset to Y later
      transferAopAop(AOP(left), 0, newaop, 0);
      transferAopAop(AOP(left), 1, newaop, 1);
    }
  else
    {
      bool needpulla = pushRegIfUsed(m6502_reg_a);

      emit6502op ("clc", "");
      prepTempOfs = _G.tempOfs;

      if (IS_AOP_AX(AOP(left))) {
        pushReg(m6502_reg_a, true);
        loadRegFromAop(m6502_reg_a, AOP(left), 0);
        emit6502op ("adc", "#<(%s+%d)", rematOfs, offset);
        storeRegTemp(m6502_reg_a, true);
        pullReg(m6502_reg_a);
        emit6502op ("adc", "#>(%s+%d)", rematOfs, offset);
        storeRegTemp(m6502_reg_a, true);
      } else {
        loadRegFromAop(m6502_reg_a, AOP(left), 0);
      emit6502op ("adc", "#<(%s+%d)", rematOfs, offset);
      storeRegTemp(m6502_reg_a, true);
      loadRegFromAop(m6502_reg_a, AOP(left), 1);
      emit6502op ("adc", "#>(%s+%d)", rematOfs, offset);
      storeRegTemp(m6502_reg_a, true);
      }
      pullOrFreeReg (m6502_reg_a, needpulla);
    }
    
  Safe_free (newaop->aopu.aop_dir);
  Safe_free (newaop);

  emitComment (TRACEGEN|VVDBG, "      preparePointer: prepoff=%d  G.off=%d", prepTempOfs, _G.tempOfs);

  wassertl (_G.tempOfs <= NUM_TEMP_REGS, "preparePointer(): overflow");
  return prepTempOfs;
}

static void unpreparePointer()
{
  emitComment (TRACEGEN|VVDBG, "      unpreparePointer: prepoff=%d  G.off=%d", prepTempOfs, _G.tempOfs);

  if (prepTempOfs >= 0)
    {
      loadRegTemp(NULL);
      loadRegTemp(NULL);
      prepTempOfs = -2;
    }
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void
genPointerGet (iCode * ic, iCode * ifx)
{
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  operand *result = IC_RESULT (ic);
  int size, offset;
  int litOffset = 0;
  char * rematOffset = NULL;
  sym_link *retype = getSpec (operandType (result));
  bool needpulla = false;
  // TODO? bool vol = false;
  int tIdx;

  emitComment (TRACEGEN, __func__);
 
  // result = right (remat+literal_offset) + left (register offset)
 
  size = getSize (operandType (result));

  if (size > 1)
    ifx = NULL;

  aopOp (left, ic, false);

  /* if left is rematerialisable */
  if (AOP_TYPE (left) == AOP_IMMD || AOP_TYPE (left) == AOP_LIT)
    {
      /* if result is not bit variable type */
      if (!IS_BITVAR (retype))
          genDataPointerGet (left, right, result, ic, ifx);
      else
          genUnpackBitsImmed (left, right, result, ic, ifx);
          return;
    }

  aopOp (result, ic, false);

  /* if bit then unpack */
  if (IS_BITVAR (retype))
    {
      genUnpackBits (result, left, right, ifx);
      goto release;
    }

  // TODO? 
  aopOp (right, ic, false);

  decodePointerOffset (right, &litOffset, &rematOffset);

  emitComment (TRACEGEN|VVDBG,"   %s -   res: %s ", __func__, aopName(AOP(result)));
  emitComment (TRACEGEN|VVDBG,"   %s -  left: %s ", __func__, aopName(AOP(left)));
  emitComment (TRACEGEN|VVDBG,"   %s - right: %s ", __func__, aopName(AOP(right)));

  /* force offset to signed 16-bit range */
  litOffset &= 0xffff;
  if (litOffset & 0x8000)
    litOffset = 0x10000 - litOffset;
  
  emitComment (TRACEGEN|VVDBG, "      genPointerGet (%s) size=%d loff=%d rmoff=%s", 
               aopName(AOP(left)), size, litOffset, rematOffset );  
  
  
  if (AOP_TYPE (left) == AOP_DIR && !rematOffset && litOffset >= 0 && litOffset <= 256-size) 
  {
    // pointer is already in zero page & 8-bit offset
    emitComment (TRACEGEN|VVDBG, "      genPointerGet - pointer already in zp");
    bool needloady = storeRegTempIfSurv(m6502_reg_y);

#if 0
    // seem to make perf worse
    if (size == 1 && litOffset == 0 
          && ( /*m6502_reg_x->isDead || */ (m6502_reg_x->isLitConst && m6502_reg_x->litConst == 0) ) ) 
      {
      // [aa,x] x == 0
      loadRegFromConst(m6502_reg_x,0);
      emit6502op ("lda", "[%s,x]", aopAdrStr ( AOP(left), 0, true ) );
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      goto release;
    }
#endif

    if (sameRegs(AOP(left), AOP(result)) ) {
      // pointer and destination is the same - need avoid overwriting
      emitComment (TRACEGEN|VVDBG, "        genPointerGet - sameregs");
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      for (int i=size-1; i>=0; i--) {
        loadRegFromConst(m6502_reg_y, litOffset + i);
        emit6502op ("lda", "[*%s],y", AOP(left)->aopu.aop_dir);
        if(i>1) {
        storeRegToAop (m6502_reg_a, AOP (result), i);
        } else if(i==1) {
          pushReg(m6502_reg_a, false);
        } else if(i==0) {
      storeRegToAop (m6502_reg_a, AOP (result), 0);
          if(size>1) {
            pullReg(m6502_reg_a);
            storeRegToAop (m6502_reg_a, AOP (result), 1);
          }
        }
      }
    } else {
      // otherwise use [aa],y
      if (IS_AOP_XA(AOP(result))) {
        // reverse order so A is last
        for (int i=size-1; i>=0; i--) {
          loadRegFromConst(m6502_reg_y, litOffset + i);
          emit6502op ("lda", "[%s],y", aopAdrStr ( AOP(left), 0, true ) );
          storeRegToAop (m6502_reg_a, AOP (result), i);
        }
      } else {
        // forward order
        if (!IS_AOP_WITH_A(AOP(result))) needpulla = storeRegTempIfSurv (m6502_reg_a);
        for (int i=0; i<size; i++) {
          loadRegFromConst(m6502_reg_y, litOffset + i);
          emit6502op ("lda", "[%s],y", aopAdrStr ( AOP(left), 0, true ) );
          storeRegToAop (m6502_reg_a, AOP (result), i);
        }
      }
    }
    loadOrFreeRegTemp (m6502_reg_a, needpulla);
    loadOrFreeRegTemp(m6502_reg_y, needloady);
    goto release;
  }
  
  // try absolute indexed
#if 0
  // allow index to be in memory
  if (rematOffset 
      && ( AOP_SIZE(left)==1
          || ( AOP_TYPE(left) == AOP_REG && AOP(left)->aopu.aop_reg[1]->isLitConst ) ) )
#else
  // index can only be a register
  if (rematOffset && AOP_TYPE(left) == AOP_REG && 
      (AOP_SIZE(left) == 1|| AOP(left)->aopu.aop_reg[1]->isLitConst ))
#endif
{
      emitComment (TRACEGEN|VVDBG,"   %s - absolute with 8-bit index", __func__);
         unsigned int hi_offset=0;
         char *dst_reg;
         char idx_reg;

         if(AOP_SIZE(left)==2)
             hi_offset=(AOP(left)->aopu.aop_reg[1]->litConst)<<8;

         if(AOP_TYPE(left)==AOP_REG) {
           switch(AOP(left)->aopu.aop_reg[0]->rIdx) {
             case X_IDX: idx_reg='x'; break;
             case Y_IDX: idx_reg='y'; break;
             case A_IDX: idx_reg='A'; break;
             default: idx_reg='E'; break;
           }
         } else {
           idx_reg='M';
         }
 
         if(AOP_TYPE(result)==AOP_REG) {
           switch(AOP(result)->aopu.aop_reg[0]->rIdx) {
             case A_IDX: dst_reg="lda"; break;
             case X_IDX: dst_reg="ldx"; break;
             case Y_IDX: dst_reg="ldy"; break;
             default: dst_reg="ERROR"; break;
           }
         } else {
           dst_reg="MEM";
         }

         bool px = false;
         bool py = false;
         bool pa = false;

         if(idx_reg=='A' || idx_reg=='M') {
           if(dst_reg[2]=='y') {
             px = storeRegTempIfSurv(m6502_reg_x);
             loadRegFromAop(m6502_reg_x, AOP(left), 0 );
             idx_reg='x';
           } else if(dst_reg[2]=='x') {
             py = storeRegTempIfSurv(m6502_reg_y);
             loadRegFromAop(m6502_reg_y, AOP(left), 0 );
             idx_reg='y';
           } else {
             // FIXME: should check for a free reg to avoid saving if possible
             py = storeRegTempIfSurv(m6502_reg_y);
             loadRegFromAop(m6502_reg_y, AOP(left), 0 );
             idx_reg='y';
           }
           
         }

         if(dst_reg[2] == idx_reg || dst_reg[0]=='M') {
//             loadRegFromAop (m6502_reg_a, AOP (right), 0);
//             dst_reg="lda";
             pa = storeRegTempIfSurv(m6502_reg_a);
             emit6502op("lda", "(%s+%d+0x%04x),%c",
                         rematOffset, litOffset, hi_offset, idx_reg );

            storeRegToAop (m6502_reg_a, AOP (result), 0);
            loadOrFreeRegTemp(m6502_reg_a,pa);
         } else {
           emit6502op(dst_reg, "(%s+%d+0x%04x),%c",
                     rematOffset, litOffset, hi_offset, idx_reg );
         }


       loadOrFreeRegTemp(m6502_reg_x,px);
       loadOrFreeRegTemp(m6502_reg_y,py);
      loadOrFreeRegTemp (m6502_reg_a, needpulla);
      goto release;
  }
 
  // indirect with a 8-bit offset
  if ( !rematOffset && litOffset>=0 && litOffset<=256-size) {
      emitComment (TRACEGEN|VVDBG,"   %s - indirect with 8-bit offset", __func__);
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      bool needloady = storeRegTempIfSurv(m6502_reg_y);
      tIdx = _G.tempOfs;
      if(AOP_TYPE(left) == AOP_REG) {
        storeRegTemp(AOP(left)->aopu.aop_reg[0] ,true);
        storeRegTemp(AOP(left)->aopu.aop_reg[1] ,true);
      } else if(AOP_TYPE(left) == AOP_EXT || AOP_TYPE(left) == AOP_SOF) {
        loadRegFromAop(m6502_reg_a, AOP(left), 0);
        storeRegTemp(m6502_reg_a ,true);
        loadRegFromAop(m6502_reg_a, AOP(left), 1);
        storeRegTemp(m6502_reg_a ,true);        
      } else if(AOP_TYPE(left) == AOP_DIR) {
      
      } else {
        emitcode("ERROR","AOP_TYPE=%02x",AOP_TYPE(left));
      }
      if(AOP_TYPE(result) == AOP_REG && IS_AOP_XA(AOP(result) )) {
        // reverse order to prevent overwriting A
        for (offset=size-1; offset>=0; offset--) {
          loadRegFromConst(m6502_reg_y, litOffset+offset);
          emit6502op("lda",TEMPFMT_IY,tIdx);
          storeRegToAop (m6502_reg_a, AOP (result), offset);      
        }
      } else {
        // normal order
        for (offset=0; offset<size; offset++) {
          loadRegFromConst(m6502_reg_y, litOffset+offset);
          emit6502op("lda",TEMPFMT_IY,tIdx);
          storeRegToAop (m6502_reg_a, AOP (result), offset);      
        }
      }
      loadRegTemp(NULL);
      loadRegTemp(NULL);
      loadOrFreeRegTemp(m6502_reg_y, needloady);
      loadOrFreeRegTemp (m6502_reg_a, needpulla);
      goto release;
  }

{
  // general case
  needpulla = storeRegTempIfSurv (m6502_reg_a);
  bool needloady = storeRegTempIfSurv(m6502_reg_y);
  bool savea = false;
  if(IS_AOP_AX(AOP(left))) {
      savea = true;
      transferRegReg(m6502_reg_a, m6502_reg_y, true);
  }
  emit6502op ("clc", "");
  loadRegFromAop(m6502_reg_a, AOP(left), 0);
  tIdx = _G.tempOfs;
  emit6502op ("adc", "#<(%s+%d)", rematOffset, litOffset);
  storeRegTemp(m6502_reg_a, true);
  if(savea) transferRegReg(m6502_reg_y, m6502_reg_a, true);
  else 
  loadRegFromAop(m6502_reg_a, AOP(left), 1);
  emit6502op ("adc", "#>(%s+%d)", rematOffset, litOffset);
  storeRegTemp(m6502_reg_a, true);


  if(AOP_TYPE (result) == AOP_REG && size>1) {
    // special case the 2 bytes registers to avoid overwriting
          if(size>2) emitcode("ERROR"," size=%d", size);
            if (IS_AOP_YX (AOP (IC_RESULT (ic))))
            {
              loadRegFromConst(m6502_reg_y, 0);
          emit6502op("lda", TEMPFMT_IY, tIdx);
          transferRegReg(m6502_reg_a, m6502_reg_x, true);
              loadRegFromConst(m6502_reg_y, 1);
          emit6502op("lda", TEMPFMT_IY, tIdx);
          transferRegReg(m6502_reg_a, m6502_reg_y, true);
            }
          else if (IS_AOP_XA (AOP (IC_RESULT (ic))))
            {
          loadRegFromConst(m6502_reg_y, 1);
          emit6502op("lda", TEMPFMT_IY, tIdx);
          transferRegReg(m6502_reg_a, m6502_reg_x, true);
          loadRegFromConst(m6502_reg_y, 0);
          emit6502op("lda", TEMPFMT_IY, tIdx);
            }
          else if (IS_AOP_AX (AOP (IC_RESULT (ic)))) // TODO?
            {
          loadRegFromConst(m6502_reg_y, 0);
          emit6502op("lda", TEMPFMT_IY, tIdx);
          transferRegReg(m6502_reg_a, m6502_reg_x, true);
          loadRegFromConst(m6502_reg_y, 1);
          emit6502op("lda", TEMPFMT_IY, tIdx);
            }
  } else {
        for (offset=0; offset<size; offset++) {
          loadRegFromConst(m6502_reg_y, offset);
          emit6502op("lda",TEMPFMT_IY,tIdx);
          storeRegToAop (m6502_reg_a, AOP (result), offset);
        }
  }
  loadRegTemp(NULL);
  loadRegTemp(NULL);
  loadOrFreeRegTemp(m6502_reg_y, needloady);
  loadOrFreeRegTemp (m6502_reg_a, needpulla);
  goto release;
}

release:
  size = AOP_SIZE (result);
  freeAsmop (left, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a");
    }
}

/*-----------------------------------------------------------------*/
/* genPackBits - generates code for packed bit storage             */
/*-----------------------------------------------------------------*/
static void
genPackBits (operand * result, operand * left, sym_link * etype, operand * right)
{
  int offset = 0;               /* source byte offset */
  int rlen = 0;                 /* remaining bitfield length */
  unsigned blen;                /* bitfield length */
  unsigned bstr;                /* bitfield starting bit within byte */
  int litval;                   /* source literal value (if AOP_LIT) */
  unsigned char mask;           /* bitmask within current byte */
  int litOffset = 0;
  char *rematOffset = NULL;
  bool needpulla;

  emitComment (TRACEGEN, __func__);

  decodePointerOffset (left, &litOffset, &rematOffset);
  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  needpulla = pushRegIfSurv (m6502_reg_a);
  if (AOP_TYPE (right) == AOP_REG)
    {
      /* Not optimal, but works for any register sources. */
      /* Just push the source values onto the stack and   */
      /* pull them off any needed. Better optimzed would  */
      /* be to do some of the shifting/masking now and    */
      /* push the intermediate result. */
      if (blen > 8)
        pushReg (AOP (right)->aopu.aop_reg[1], true);
      pushReg (AOP (right)->aopu.aop_reg[0], true);
    }
  loadRegFromAop (m6502_reg_yx, AOP (result), 0);

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      mask = ((unsigned char) (0xFF << (blen + bstr)) | (unsigned char) (0xFF >> (8 - bstr)));

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with a bitfield length <8 and literal source
           */
          litval = (int) ulFromVal (AOP (right)->aopu.aop_lit);
          litval <<= bstr;
          litval &= (~mask) & 0xff;

          loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
          if ((mask | litval) != 0xff)
            {
              emit6502op ("and", IMMDFMT, mask);
            }
          if (litval)
            {
              emit6502op ("ora", IMMDFMT, litval);
            }
          m6502_dirtyReg (m6502_reg_a);
          storeRegIndexed (m6502_reg_a, litOffset, rematOffset);

          pullOrFreeReg (m6502_reg_a, needpulla);
          return;
        }

      /* Case with a bitfield length < 8 and arbitrary source
       */
      if (AOP_TYPE (right) == AOP_REG)
        pullReg (m6502_reg_a);
      else
        loadRegFromAop (m6502_reg_a, AOP (right), 0);
      /* shift and mask source value */
      AccLsh (bstr);
      emit6502op ("and", IMMDFMT, (~mask) & 0xff);
      storeRegTemp (m6502_reg_a, true);

      loadRegIndexed (m6502_reg_a, litOffset, rematOffset);
      emit6502op ("and", IMMDFMT, mask);
      emit6502op ("ora", TEMPFMT, _G.tempOfs-1);
      storeRegIndexed (m6502_reg_a, litOffset, rematOffset);
//      loadRegTemp (m6502_reg_a);
      loadRegTemp (NULL);
      // TODO? redundant?
      pullOrFreeReg (m6502_reg_a, needpulla);
      return;
    }

  /* Bit length is greater than 7 bits. In this case, copy  */
  /* all except the partial byte at the end                 */
  for (rlen = blen; rlen >= 8; rlen -= 8)
    {
      if (!litOffset && !rematOffset && AOP (right)->type == AOP_DIR)
        {
	  // FIXME: unimplemented
          emitcode ("mov", "%s,x+", aopAdrStr (AOP (right), offset, false));
          litOffset--;
        }
      else
        {
          if (AOP_TYPE (right) == AOP_REG)
            pullReg (m6502_reg_a);
          else
            loadRegFromAop (m6502_reg_a, AOP (right), offset);
          storeRegIndexed (m6502_reg_a, litOffset+offset, rematOffset);
        }
      offset++;
    }

  /* If there was a partial byte at the end */
  if (rlen)
    {
      mask = (((unsigned char) - 1 << rlen) & 0xff);

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with partial byte and literal source
           */
          litval = (int) ulFromVal (AOP (right)->aopu.aop_lit);
          litval >>= (blen - rlen);
          litval &= (~mask) & 0xff;
          loadRegIndexed (m6502_reg_a, litOffset+offset, rematOffset);
          if ((mask | litval) != 0xff)
            {
              emit6502op ("and", IMMDFMT, mask);
            }
          if (litval)
            {
              emit6502op ("ora", IMMDFMT, litval);
            }
          m6502_dirtyReg (m6502_reg_a);
          storeRegIndexed (m6502_reg_a, litOffset+offset, rematOffset);
          pullOrFreeReg (m6502_reg_a, needpulla);
          return;
        }

      /* Case with partial byte and arbitrary source
       */
      if (AOP_TYPE (right) == AOP_REG)
        pullReg (m6502_reg_a);
      else
        loadRegFromAop (m6502_reg_a, AOP (right), offset);
      emit6502op ("and", IMMDFMT, (~mask) & 0xff);
      pushReg (m6502_reg_a, true);

      // FIXME: unimplemented
      loadRegIndexed(m6502_reg_a, litOffset+offset, rematOffset);
      emitcode ("and", IMMDFMT, mask);
      emitcode ("ora19", "1,s");
      storeRegIndexed (m6502_reg_a, litOffset+offset, rematOffset);
      pullReg (m6502_reg_a);
    }

  pullOrFreeReg (m6502_reg_a, needpulla);
}

/*-----------------------------------------------------------------*/
/* genPackBitsImmed - generates code for packed bit storage        */
/*-----------------------------------------------------------------*/
static void
genPackBitsImmed (operand * result, operand * left, sym_link * etype, operand * right, iCode * ic)
{
  asmop *derefaop;
  int size;
  int offset = 0;               /* source byte offset */
  int rlen = 0;                 /* remaining bitfield length */
  unsigned blen;                /* bitfield length */
  unsigned bstr;                /* bitfield starting bit within byte */
  int litval;                   /* source literal value (if AOP_LIT) */
  unsigned char mask;           /* bitmask within current byte */
  bool needpulla;
  int litOffset = 0;
  char *rematOffset = NULL;

  emitComment (TRACEGEN, __func__);

  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  aopOp (right, ic, false);
  size = AOP_SIZE (right);
  decodePointerOffset (left, &litOffset, &rematOffset);
  wassert (!rematOffset);

  derefaop = aopDerefAop (AOP (result), litOffset);
  freeAsmop (result, NULL, ic, true);
  derefaop->size = size;

  /* if the bitfield is a single bit in the direct page */
  if (blen == 1 && derefaop->type == AOP_DIR)
    {
      if (AOP_TYPE (right) == AOP_LIT)
        {
          litval = (int) ulFromVal (AOP (right)->aopu.aop_lit);
	  // FIXME: unimplemented
          emitcode ((litval & 1) ? "bset" : "bclr", "#%d,%s", bstr, aopAdrStr (derefaop, 0, false));
        }
      else
        {
          symbol *tlbl1 = safeNewiTempLabel (NULL);
          symbol *tlbl2 = safeNewiTempLabel (NULL);

	  // FIXME: unimplemented
          needpulla = pushRegIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, AOP (right), 0);
          emit6502op ("lsr", "a");
          emitBranch ("bcs", tlbl1);
          emitcode ("bclr", "#%d,%s", bstr, aopAdrStr (derefaop, 0, false));
          emitBranch ("bra", tlbl2);
          safeEmitLabel (tlbl1);
          emitcode ("bset", "#%d,%s", bstr, aopAdrStr (derefaop, 0, false));
          safeEmitLabel (tlbl2);
          pullOrFreeReg (m6502_reg_a, needpulla);
        }
      goto release;
    }

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      mask = ((unsigned char) (0xFF << (blen + bstr)) | (unsigned char) (0xFF >> (8 - bstr)));

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with a bitfield length <8 and literal source
           */
          litval = (int) ulFromVal (AOP (right)->aopu.aop_lit);
          litval <<= bstr;
          litval &= (~mask) & 0xff;

          needpulla = pushRegIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, derefaop, 0);
          if ((mask | litval) != 0xff)
            {
              emit6502op ("and", IMMDFMT, mask);
            }
          if (litval)
            {
              emit6502op ("ora", IMMDFMT, litval);
            }
          m6502_dirtyReg (m6502_reg_a);
          storeRegToAop (m6502_reg_a, derefaop, 0);

          pullOrFreeReg (m6502_reg_a, needpulla);
          goto release;
        }

      /* Case with a bitfield length < 8 and arbitrary source
       */
      needpulla = pushRegIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (right), 0);
      /* shift and mask source value */
      AccLsh (bstr);
      emit6502op ("and", IMMDFMT, (~mask) & 0xff);
      storeRegTemp(m6502_reg_a, true);

      loadRegFromAop (m6502_reg_a, derefaop, 0);
      emit6502op ("and", IMMDFMT, mask);
      emit6502op ("ora", TEMPFMT, _G.tempOfs - 1);
      storeRegToAop (m6502_reg_a, derefaop, 0);

      pullOrFreeReg (m6502_reg_a, needpulla);
      loadRegTemp(NULL);
      goto release;
    }

  /* Bit length is greater than 7 bits. In this case, copy  */
  /* all except the partial byte at the end                 */
  for (rlen = blen; rlen >= 8; rlen -= 8)
    {
      transferAopAop (AOP (right), offset, derefaop, offset);
      offset++;
    }

  /* If there was a partial byte at the end */
  if (rlen)
    {
      mask = (((unsigned char) - 1 << rlen) & 0xff);

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with partial byte and literal source
           */
          litval = (int) ulFromVal (AOP (right)->aopu.aop_lit);
          litval >>= (blen - rlen);
          litval &= (~mask) & 0xff;
          needpulla = pushRegIfSurv (m6502_reg_a);
          loadRegFromAop (m6502_reg_a, derefaop, offset);
          if ((mask | litval) != 0xff)
            {
              emit6502op ("and", IMMDFMT, mask);
            }
          if (litval)
            {
              emit6502op ("ora", IMMDFMT, litval);
            }
          m6502_dirtyReg (m6502_reg_a);
          storeRegToAop (m6502_reg_a, derefaop, offset);
          m6502_dirtyReg (m6502_reg_a);
          pullOrFreeReg (m6502_reg_a, needpulla);
          goto release;
        }

      /* Case with partial byte and arbitrary source
       */
      needpulla = pushRegIfSurv (m6502_reg_a);
      loadRegFromAop (m6502_reg_a, AOP (right), offset);
      emit6502op ("and", IMMDFMT, (~mask) & 0xff);
      storeRegTemp (m6502_reg_a, true);

      loadRegFromAop (m6502_reg_a, derefaop, offset);
      emit6502op ("and", IMMDFMT, mask);
      emit6502op ("ora", TEMPFMT, _G.tempOfs - 1);
      storeRegToAop (m6502_reg_a, derefaop, offset);
      pullOrFreeReg (m6502_reg_a, needpulla);
      loadRegTemp (NULL);
    }

  m6502_freeReg (m6502_reg_a);

release:
  freeAsmop (right, NULL, ic, true);
  freeAsmop (NULL, derefaop, ic, true);
}

/*-----------------------------------------------------------------*/
/* genDataPointerSet - remat pointer to data space                 */
/*-----------------------------------------------------------------*/
static void
genDataPointerSet (operand * left, operand * right, operand * result, iCode * ic)
{
  int size;
  asmop *derefaop;
  int litOffset = 0;
  char *rematOffset = NULL;

  emitComment (TRACEGEN, __func__);

  aopOp (right, ic, false);
  size = AOP_SIZE (right);
  decodePointerOffset (left, &litOffset, &rematOffset);
  wassert (!rematOffset);

  derefaop = aopDerefAop (AOP (result), litOffset);
  freeAsmop (result, NULL, ic, true);
  derefaop->size = size;

      while (size--)
        {
          transferAopAop (AOP (right), size, derefaop, size);
    }

  freeAsmop (right, NULL, ic, true);
  freeAsmop (NULL, derefaop, ic, true);
}


/*-----------------------------------------------------------------*/
/* genPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void
genPointerSet (iCode * ic)
{
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  operand *result = IC_RESULT (ic);
  int size, offset;
  sym_link *retype = getSpec (operandType (right));
  sym_link *letype = getSpec (operandType (result));
  bool needpulla = false;
  bool needpullx = false;
  bool needpully = false;
//  bool vol = false;
  int litOffset = 0;
  char *rematOffset = NULL;

  emitComment (TRACEGEN, __func__);
  
  // *(result (reg) + left (rematoffset+litoffset) = right

  
  aopOp (result, ic, false);

  /* if the result is rematerializable */
  if (AOP_TYPE (result) == AOP_IMMD || AOP_TYPE (result) == AOP_LIT)
    {
      if (!IS_BITVAR (retype) && !IS_BITVAR (letype))
          genDataPointerSet (left, right, result, ic);
      else
          genPackBitsImmed (result, left, (IS_BITVAR (retype) ? retype : letype), right, ic);
          return;
    }

  aopOp (right, ic, false);
  //aopOp (left, ic, false);

  size = AOP_SIZE (right);

  decodePointerOffset (left, &litOffset, &rematOffset);

  emitComment (TRACEGEN|VVDBG,"   %s -   res: %s ", __func__, aopName(AOP(result)));
//  emitComment (TRACEGEN|VVDBG,"   %s -  left: %s ", __func__, aopName(AOP(left)));
  emitComment (TRACEGEN|VVDBG,"   %s - right: %s ", __func__, aopName(AOP(right)));

  emitComment (TRACEGEN|VVDBG, "      genPointerSet (%s), size=%d, litoffset=%d, rematoffset=%s", 
               aopName(AOP(right)), size, litOffset, rematOffset );

  // shortcut for [aa],y (or [aa,x]) if already in zero-page
  // and we're not storing to the same pointer location

  if (!(IS_BITVAR (retype) || IS_BITVAR (letype))
      && AOP_TYPE (result) == AOP_DIR && !rematOffset && litOffset >= 0 && litOffset <= 256-size
      && !sameRegs(AOP(right), AOP(result)) ) {
  
#if 0
    if (size == 1 && litOffset == 0 && m6502_reg_x->isLitConst && m6502_reg_x->litConst == 0) {
      // use [aa,x] if only 1 byte and offset is 0
      loadRegFromAop (m6502_reg_a, AOP (right), 0);
      emit6502op ("sta", "[%s,x]", aopAdrStr ( AOP(result), 0, true ) );
    } 
    else
#endif
    {
      needpulla = storeRegTempIfSurv (m6502_reg_a);
      needpully = storeRegTempIfUsed (m6502_reg_y);

      if (IS_AOP_AX(AOP(right))) {
        // reverse order so A is first
        for (int i=size-1; i>=0; i--) {
          loadRegFromAop (m6502_reg_a, AOP (right), i);
          loadRegFromConst(m6502_reg_y, litOffset + i);
          emit6502op ("sta", "[%s],y", aopAdrStr ( AOP(result), 0, true ) );
        }
      } else {
        // forward order
        for (int i=0; i<size; i++) {
          loadRegFromAop (m6502_reg_a, AOP (right), i);
          loadRegFromConst(m6502_reg_y, litOffset + i);
          emit6502op ("sta", "[%s],y", aopAdrStr ( AOP(result), 0, true ) );
        }
      }
    }
    goto release;
  }

#if 1
    // abs,x or abs,y with index in register or memory
  if (rematOffset 
      && ( AOP_SIZE(result)==1
           || ( AOP_TYPE(result) == AOP_REG && AOP(result)->aopu.aop_reg[1]->isLitConst ) ) )
#else
    // abs,x or abs,y with index in register
  if (rematOffset && AOP_TYPE(result)== AOP_REG
           && ( AOP_SIZE(result) == 1 || AOP(result)->aopu.aop_reg[1]->isLitConst ) )
#endif
      {
      emitComment (TRACEGEN|VVDBG,"   %s - absolute with 8-bit index", __func__);
      emitComment(TRACEGEN|VVDBG," reg : %d  size:%d", AOP(result)->aopu.aop_reg[0]->rIdx,  AOP_SIZE(result) );

      emitComment (TRACEGEN|VVDBG,"AOP TYPE(result)=%d",AOP_TYPE (result));
      emitComment (TRACEGEN|VVDBG,"AOP(result) reg=%d",AOP(result)->aopu.aop_reg[0]->rIdx);
         unsigned int hi_offset=0;
         bool src_reg_is_y = false;
         char idx_reg;
         bool px = false;
         bool py = false;
         bool pa = false;

         pa=pushRegIfSurv(m6502_reg_a);


         if(AOP_SIZE(result)==2)
             hi_offset=(AOP(result)->aopu.aop_reg[1]->litConst)<<8;

//  if ( ( AOP_TYPE(result) == AOP_REG && AOP(result)->aopu.aop_reg[0]->isLitConst ) )
//	emitcode("ERROR","");


         if(AOP_TYPE(result)==AOP_REG) {
           switch(AOP(result)->aopu.aop_reg[0]->rIdx) {
             case X_IDX: idx_reg='x'; break;
             case Y_IDX: idx_reg='y'; break;
             case A_IDX: idx_reg='A'; break;
             default: idx_reg='E'; break;
           }
         } else {
           idx_reg='M';
         }

         if(AOP_TYPE(right)==AOP_REG 
            && AOP(right)->aopu.aop_reg[0]->rIdx == Y_IDX ) 
               src_reg_is_y = true;

         if(idx_reg=='A' || idx_reg=='M') {
           if(src_reg_is_y) {
             px = storeRegTempIfSurv(m6502_reg_x);
             loadRegFromAop(m6502_reg_x, AOP(result), 0 );
             idx_reg='x';
           } else {
             py = storeRegTempIfSurv(m6502_reg_y);
             loadRegFromAop(m6502_reg_y, AOP(result), 0 );
             idx_reg='y';
           }
         }

             loadRegFromAop (m6502_reg_a, AOP (right), 0);

         emit6502op("sta", "(%s+%d+0x%04x),%c",
                     rematOffset, litOffset, hi_offset, idx_reg );

       pullOrFreeReg(m6502_reg_a, pa);
       loadOrFreeRegTemp(m6502_reg_x,px);
       loadOrFreeRegTemp(m6502_reg_y,py);

       goto release;
     } 


  // general case
  needpulla = storeRegTempIfSurv (m6502_reg_a);
  needpullx = storeRegTempIfSurv (m6502_reg_x);
  needpully = storeRegTempIfSurv (m6502_reg_y);
  int tIdx;

  if(needpulla) m6502_reg_a->isFree=false;

  // general case

  /* if bit then pack */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    {
      //int ptrofs =
      preparePointer (result, litOffset, rematOffset, right);
      genPackBits (result, left, (IS_BITVAR (retype) ? retype : letype), right);
      unpreparePointer();
      goto release;
    }

#if 0
  bool savea = false;
  if(!m6502_reg_a->isFree) {
      savea = true;
      transferRegReg(m6502_reg_a, m6502_reg_y, true);
  }
#endif

    tIdx = _G.tempOfs;
    bool pa=pushRegIfUsed(m6502_reg_a);
  if(!rematOffset && !litOffset) {
     // just register content, no remat and no offset
     if(AOP_TYPE(result)==AOP_REG) {
       // already in registers just save to TEMP
       storeRegTemp(AOP(result)->aopu.aop_reg[0], true);
       storeRegTemp(AOP(result)->aopu.aop_reg[1], true);
     } else {
       loadRegFromAop(m6502_reg_a, AOP(result), 0);
       storeRegTemp(m6502_reg_a, true);
       loadRegFromAop(m6502_reg_a, AOP(result), 1);
       storeRegTemp(m6502_reg_a, true);
     }
  } else {
    if(!rematOffset || rematOffset[0]==0)
       rematOffset="0";

    emit6502op ("clc", "");
    loadRegFromAop(m6502_reg_a, AOP(result), 0);
    emit6502op ("adc", "#<(%s+%d)", rematOffset, litOffset);
    storeRegTemp(m6502_reg_a, true);
    loadRegFromAop(m6502_reg_a, AOP(result), 1);
    emit6502op ("adc", "#>(%s+%d)", rematOffset, litOffset);
    storeRegTemp(m6502_reg_a, true);
  }

    pullOrFreeReg(m6502_reg_a,pa);
//  if(savea) transferRegReg(m6502_reg_y, m6502_reg_a, true);


    if (IS_AOP_YX (AOP(right)) || IS_AOP_AX (AOP(right)))
          {
            // reverse order to avoid overwriting A or Y
            loadRegFromAop (m6502_reg_a, AOP (right), 1);
            loadRegFromConst(m6502_reg_y, 1);
            emit6502op("sta",TEMPFMT_IY,tIdx);
            loadRegFromAop (m6502_reg_a, AOP (right), 0);
            loadRegFromConst(m6502_reg_y, 0);
            emit6502op("sta",TEMPFMT_IY,tIdx);
          }
 else
 {
    for (offset=0; offset<size; offset++) {
      loadRegFromAop (m6502_reg_a, AOP (right), offset);
      loadRegFromConst(m6502_reg_y, offset);
      emit6502op("sta",TEMPFMT_IY,tIdx);
    }
  }
  loadRegTemp(NULL);
  loadRegTemp(NULL);
  goto release;

release:
  freeAsmop (result, NULL, ic, true);
  freeAsmop (right, NULL, ic, true);

  loadOrFreeRegTemp (m6502_reg_y, needpully);
  loadOrFreeRegTemp (m6502_reg_x, needpullx);
  loadOrFreeRegTemp (m6502_reg_a, needpulla);
}

// TODO: genIfx sometimes does a cmp #0 but has flags already, peephole might fix
/*-----------------------------------------------------------------*/
/* genIfx - generate code for Ifx statement                        */
/*-----------------------------------------------------------------*/
static void
genIfx (iCode * ic, iCode * popIc)
{
  operand *cond = IC_COND (ic);
  
  emitComment (TRACEGEN, __func__);

  aopOp (cond, ic, false);

  /* If the condition is a literal, we can just do an unconditional */
  /* branch or no branch */
  if (AOP_TYPE (cond) == AOP_LIT)
    {
      unsigned long long lit = ullFromVal (AOP (cond)->aopu.aop_lit);
      freeAsmop (cond, NULL, ic, true);

      /* if there was something to be popped then do it */
      if (popIc)
        genIpop (popIc);
      if (lit)
        {
          if (IC_TRUE (ic))
            emitBranch ("jmp", IC_TRUE (ic));
        }
      else
        {
          if (IC_FALSE (ic))
            emitBranch ("jmp", IC_FALSE (ic));
        }
      ic->generated = 1;
      return;
    }

  /* evaluate the operand */
  if (AOP_TYPE (cond) != AOP_CRY) {
    emitComment (TRACEGEN|VVDBG, "     genIfx - !AOP_CRY");
    asmopToBool (AOP (cond), false);
  }
  /* the result is now in the z flag bit */
  freeAsmop (cond, NULL, ic, true);

  /* if there was something to be popped then do it */
  if (popIc)
      genIpop (popIc);

// TODO: redundant bne/beq 
  emitComment (TRACEGEN|VVDBG, "      genIfx - call jump");
  genIfxJump (ic, "a");

  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genAddrOf - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void
genAddrOf (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));
  //  asmop *aopr;
  int size, offset;
  bool needpulla, needpullx;
  struct dbuf_s dbuf;

  emitComment (TRACEGEN, __func__);

  aopOp (IC_RESULT (ic), ic, false);
  //  aopr = AOP (IC_RESULT (ic));

  /* if the operand is on the stack then we
     need to get the stack offset of this
     variable */
  if (sym->onStack)
    {
      needpulla = pushRegIfSurv (m6502_reg_a);
      needpullx = pushRegIfSurv (m6502_reg_x);
      /* if it has an offset then we need to compute it */
      offset = _G.stackOfs + _G.stackPushes + sym->stack + 1;
      m6502_useReg (m6502_reg_xa);
      emit6502op ("tsx", "");
      m6502_dirtyReg (m6502_reg_x);
      offset=smallAdjustReg(m6502_reg_x, offset);
      transferRegReg (m6502_reg_x, m6502_reg_a, true);
      if (offset) {
          emit6502op ("clc", "");
          emit6502op ("adc", IMMDFMT, offset&0xff);
      }
      loadRegFromConst(m6502_reg_x, 0x01); // stack top = 0x100
      storeRegToFullAop (m6502_reg_xa, AOP (IC_RESULT (ic)), false);
      pullOrFreeReg (m6502_reg_x, needpullx);
      pullOrFreeReg (m6502_reg_a, needpulla);
      goto release;
    }

  /* object not on stack then we need the name */
  size = AOP_SIZE (IC_RESULT (ic));
  offset = 0;

  while (size--)
    {
      dbuf_init (&dbuf, 64);
      switch (offset)
        {
        case 0:
          dbuf_printf (&dbuf, "#%s", sym->rname);
          break;
        case 1:
          dbuf_printf (&dbuf, "#>%s", sym->rname);
          break;
        default:
          dbuf_printf (&dbuf, "#0");
        }
      storeImmToAop (dbuf_detach_c_str (&dbuf), AOP (IC_RESULT (ic)), offset++);
    }

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genAssignLit - Try to generate code for literal assignment.     */
/*                result and right should already be asmOped       */
/*-----------------------------------------------------------------*/
static bool
genAssignLit (operand * result, operand * right)
{
  char assigned[8];
  unsigned char value[sizeof(assigned)];
  int size;
  int offset,offset2;

  emitComment (TRACEGEN, __func__);

  /* Make sure this is a literal assignment */
  if (AOP_TYPE (right) != AOP_LIT)
    return false;

  /* The general case already handles register assignment well */
  if (AOP_TYPE (result) == AOP_REG)
    return false;

  /* Some hardware registers require MSB to LSB assignment order */
  /* so don't optimize the assignment order if volatile */
  if (isOperandVolatile (result, false))
    return false;

  /* Make sure the assignment is not larger than we can handle */
  size = AOP_SIZE (result);
  if (size > sizeof(assigned))
    return false;

  for (offset=0; offset<size; offset++)
    {
      assigned[offset] = 0;
      value[offset] = byteOfVal (AOP (right)->aopu.aop_lit, offset);
    }

  for (offset=0; offset<size; offset++)
    {
      if (assigned[offset])
        continue;
      storeConstToAop (value[offset], AOP (result), offset);
      assigned[offset] = 1;
      // look for duplicates  
      if ((AOP_TYPE (result) != AOP_DIR ))
        {
          for(offset2=offset+1; offset2<size; offset2++)
            {
              if(value[offset]==value[offset2])
                {
                  storeConstToAop (value[offset], AOP (result), offset2);
                  assigned[offset2] = 1;
                }
            }
        }
    }

  return true;
}


/*-----------------------------------------------------------------*/
/* genAssign - generate code for assignment                        */
/*-----------------------------------------------------------------*/
static void
genAssign (iCode * ic)
{
  operand *result, *right;

  emitComment (TRACEGEN, __func__);

  result = IC_RESULT (ic);
  right = IC_RIGHT (ic);
  
  aopOp (right, ic, false);
  aopOp (result, ic, true);

  if (!genAssignLit (result, right))
    genCopy (result, right);

  freeAsmop (right, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genJumpTab - generates code for jump table                       */
/*-----------------------------------------------------------------*/
static void
genJumpTab (iCode * ic)
{
  symbol *jtab;
  symbol *jtablo = safeNewiTempLabel (NULL);
  symbol *jtabhi = safeNewiTempLabel (NULL);

  emitComment (TRACEGEN, __func__);

  aopOp (IC_JTCOND (ic), ic, false);

// TODO
    {
      bool needpulla = pushRegIfSurv (m6502_reg_a);
      // use X or Y for index?
      bool needpullind = false;
      reg_info* indreg;
      if (IS_AOP_X (AOP (IC_JTCOND (ic)))) {
        indreg = m6502_reg_x;
      } else if (IS_AOP_Y (AOP (IC_JTCOND (ic)))) {
        indreg = m6502_reg_y;
      } else {
        indreg = m6502_reg_x->isFree ? m6502_reg_x : m6502_reg_y;
        needpullind = pushRegIfSurv (indreg);
        /* get the condition into indreg */
        loadRegFromAop (indreg, AOP (IC_JTCOND (ic)), 0);
      }
      freeAsmop (IC_JTCOND (ic), NULL, ic, true);

          if (indreg == m6502_reg_x) {
            emit6502op ("lda", "%05d$,x", safeLabelKey2num (jtablo->key));
            storeRegTemp (m6502_reg_a, true);
            emit6502op ("lda", "%05d$,x", safeLabelKey2num (jtabhi->key));
            storeRegTemp (m6502_reg_a, true);
          } else {
            emit6502op ("lda", "%05d$,y", safeLabelKey2num (jtablo->key));
            storeRegTemp (m6502_reg_a, true);
            emit6502op ("lda", "%05d$,y", safeLabelKey2num (jtabhi->key));
            storeRegTemp (m6502_reg_a, true);
          }
      loadRegTemp(NULL);
      loadRegTemp(NULL);
      if (needpullind) pullReg(indreg);
      if (needpulla) pullReg(m6502_reg_a);
      emit6502op ("jmp", TEMPFMT_IND, _G.tempOfs);

      m6502_dirtyReg (m6502_reg_a);
      m6502_dirtyReg (m6502_reg_x);
      m6502_dirtyReg (m6502_reg_y);
      m6502_freeReg (m6502_reg_a);
      m6502_freeReg (m6502_reg_x);
      m6502_freeReg (m6502_reg_y);
    }

  /* now generate the jump labels */
  safeEmitLabel (jtablo);
  // FIXME: add this to gen6502op
  for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab; jtab = setNextItem (IC_JTLABELS (ic)))
    {
      emitcode (".db", "%05d$", labelKey2num (jtab->key));
      regalloc_dry_run_cost_bytes++;
    }
  safeEmitLabel (jtabhi);
  for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab; jtab = setNextItem (IC_JTLABELS (ic)))
    {
      emitcode (".db", ">%05d$", labelKey2num (jtab->key));
      regalloc_dry_run_cost_bytes++;
    }
}

/*-----------------------------------------------------------------*/
/* genCast - generate code for cast                                */
/*-----------------------------------------------------------------*/
static void
genCast (iCode * ic)
{
  operand *result = IC_RESULT (ic);
  sym_link *rtype = operandType (IC_RIGHT (ic));
  operand *right = IC_RIGHT (ic);
  int size, offset;
  bool signExtend;
  bool save_a;

  emitComment (TRACEGEN, __func__);

  /* if they are equivalent then do nothing */
  if (operandsEqu (IC_RESULT (ic), IC_RIGHT (ic)))
    return;

  aopOp (right, ic, false);
  aopOp (result, ic, false);

  emitComment (TRACEGEN|VVDBG, "      genCast - size %d -> %d", right?AOP_SIZE(right):0, result?AOP_SIZE(result):0);

  if (IS_BOOL (operandType (result)))
    {
      bool needpulla = pushRegIfSurv (m6502_reg_a);
      asmopToBool (AOP (right), true);
      storeRegToAop (m6502_reg_a, AOP (result), 0);
      pullOrFreeReg (m6502_reg_a, needpulla);
      goto release;
    }

  /* If the result is 1 byte, then just copy the one byte; there is */
  /* nothing special required. */
  if (AOP_SIZE (result) == 1)
    {
      transferAopAop (AOP (right), 0, AOP (result), 0);
      goto release;
    }

  signExtend = AOP_SIZE (result) > AOP_SIZE (right) && !IS_BOOL (rtype) && IS_SPEC (rtype) && !SPEC_USIGN (rtype);

  /* If the result is 2 bytes and in registers, we have to be careful */
  /* to make sure the registers are not overwritten prematurely. */
  if (AOP_SIZE (result) == 2 && AOP (result)->type == AOP_REG)
    {
      if (IS_AOP_YX (AOP (result)) && (AOP_SIZE (right) == 2))
        {
          loadRegFromAop (m6502_reg_yx, AOP (right), 0);
          goto release;
        }

      if (AOP_SIZE (right) == 1)
        {
          transferAopAop (AOP (right), 0, AOP (result), 0);
          if (!signExtend)
            {
              storeConstToAop (0, AOP (result), 1);
            }
          else
            {
              save_a = (AOP (result)->aopu.aop_reg[0] == m6502_reg_a || !m6502_reg_a->isDead);

              /* we need to extend the sign :{ */
              // TODO: try to avoid doing this
              if (save_a)
                pushReg(m6502_reg_a, false);
              if (AOP (result)->aopu.aop_reg[0] != m6502_reg_a)
                loadRegFromAop (m6502_reg_a, AOP (right), 0);
              signExtendA();
              storeRegToAop (m6502_reg_a, AOP (result), 1);
              if (save_a)
                pullReg(m6502_reg_a);
            }
          goto release;
        }

      if (AOP (right)->type == AOP_REG)
        {
          wassert (AOP_SIZE (right) == 2);
          /* Source and destination are the same size; no need for sign */
          /* extension or zero padding. Just copy in the order that */
          /* won't prematurely overwrite the source. */
          if (AOP (result)->aopu.aop_reg[0] == AOP (right)->aopu.aop_reg[1])
            {
              transferAopAop (AOP (right), 1, AOP (result), 1);
              transferAopAop (AOP (right), 0, AOP (result), 0);
            }
          else
            {
              transferAopAop (AOP (right), 0, AOP (result), 0);
              transferAopAop (AOP (right), 1, AOP (result), 1);
            }
          goto release;
        }
      else
        {
          /* Source is at least 2 bytes and not in registers; no need */
          /* for sign extension or zero padding. Just copy. */
          transferAopAop (AOP (right), 0, AOP (result), 0);
          transferAopAop (AOP (right), 1, AOP (result), 1);
          goto release;
        }
    }

  wassert (AOP (result)->type != AOP_REG);
  
  save_a = !m6502_reg_a->isDead && signExtend;
  if (save_a)
    pushReg(m6502_reg_a, true);

  offset = 0;
  size = AOP_SIZE (right);
  if (AOP_SIZE (result) < size)
    size = AOP_SIZE (result);
  while (size)
    {
      if (size == 1 && signExtend)
        {
          loadRegFromAop (m6502_reg_a, AOP (right), offset);
          storeRegToAop (m6502_reg_a, AOP (result), offset);
          offset++;
          size--;
        }
      else if ((size > 2 || size >= 2 && !signExtend) && m6502_reg_y->isDead && m6502_reg_x->isDead && 
        (AOP_TYPE (right) == AOP_IMMD || IS_MOS65C02 && AOP_TYPE (right) == AOP_EXT) &&
        (AOP_TYPE (result) == AOP_DIR || IS_MOS65C02 && AOP_TYPE (result) == AOP_EXT))
        {
          // FIXME: the above exception for 65C02 is likely incorrect
          loadRegFromAop (m6502_reg_yx, AOP (right), offset);
          storeRegToAop (m6502_reg_yx, AOP (result), offset);
          offset += 2;
          size -= 2;
        }
      else
        {
          transferAopAop (AOP (right), offset, AOP (result), offset);
          offset++;
          size--;
        }
    }

  size = AOP_SIZE (result) - offset;
  if (size && !signExtend)
    {
      while (size--)
        storeConstToAop (0, AOP (result), offset++);
    }
  else if (size)
    {
      signExtendA();
      while (size--)
        storeRegToAop (m6502_reg_a, AOP (result), offset++);
    }

  if (save_a)
    pullReg(m6502_reg_a);

  /* we are done hurray !!!! */

release:
  freeAsmop (right, NULL, ic, true);
  freeAsmop (result, NULL, ic, true);

}

/*-----------------------------------------------------------------*/
/* genReceive - generate code for a receive iCode                  */
/*-----------------------------------------------------------------*/
static void
genReceive (iCode * ic)
{
  int size;
  int offset;
  bool delayed_x = false;

  emitComment (TRACEGEN, __func__);

  aopOp (IC_RESULT (ic), ic, false);
  size = AOP_SIZE (IC_RESULT (ic));
  offset = 0;

  if (ic->argreg && IS_AOP_YX (AOP (IC_RESULT (ic))) && (offset + (ic->argreg - 1)) == 0)
    {
      storeRegTemp (m6502_reg_x, true);
      transferRegReg (m6502_reg_a, m6502_reg_x, true);
      loadRegTemp (m6502_reg_y);
    }
  else if (ic->argreg)
    {
      while (size--)
        {
          if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG && !(offset + (ic->argreg - 1)) && AOP (IC_RESULT (ic))->aopu.aop_reg[0]->rIdx == X_IDX && size)
            {
              storeRegTemp (m6502_reg_a, true);
              delayed_x = true;
            }
          else
            transferAopAop (m6502_aop_pass[offset + (ic->argreg - 1)], 0, AOP (IC_RESULT (ic)), offset);
          if (m6502_aop_pass[offset]->type == AOP_REG)
            m6502_freeReg (m6502_aop_pass[offset]->aopu.aop_reg[0]);
          offset++;
        }
    }

  if (delayed_x)
    loadRegTemp (m6502_reg_x);

  freeAsmop (IC_RESULT (ic), NULL, ic, true);
}

// support routine for genDummyRead
static void
dummyRead (iCode* ic, operand* op, reg_info* reg)
{
  if (op && IS_SYMOP (op))
    {
      aopOp (op, ic, false);
      int size = AOP_SIZE (op);
      for (int offset=0; offset<size; offset++) {
        loadRegFromAop (reg, AOP (op), offset);
      }
      freeAsmop (op, NULL, ic, true);
    }
}

/*-----------------------------------------------------------------*/
/* genDummyRead - generate code for dummy read of volatiles        */
/*-----------------------------------------------------------------*/
static void
genDummyRead (iCode * ic)
{
  bool needpulla = false;

  emitComment (TRACEGEN, __func__);

  reg_info* reg = getFreeByteReg();
  if (!reg) {
    needpulla = pushRegIfSurv (m6502_reg_a);
    reg = m6502_reg_a;
  }
  
  // TODO: use BIT? STA?
  dummyRead(ic, IC_RIGHT(ic), reg);
  dummyRead(ic, IC_LEFT(ic), reg);

  pullOrFreeReg (reg, needpulla);
}

/*-----------------------------------------------------------------*/
/* genCritical - generate code for start of a critical sequence    */
/*-----------------------------------------------------------------*/
static void
genCritical (iCode * ic)
{
  emitComment (TRACEGEN, __func__);

  if (IC_RESULT (ic))
    aopOp (IC_RESULT (ic), ic, true);

  emit6502op ("php", "");
  emit6502op ("sei", "");

  if (IC_RESULT (ic)) {
    emit6502op ("plp", "");
    m6502_dirtyReg (m6502_reg_a);
    storeRegToAop (m6502_reg_a, AOP (IC_RESULT (ic)), 0);
  }

  m6502_freeReg (m6502_reg_a);
  if (IC_RESULT (ic))
    freeAsmop (IC_RESULT (ic), NULL, ic, true);
}

/*-----------------------------------------------------------------*/
/* genEndCritical - generate code for end of a critical sequence   */
/*-----------------------------------------------------------------*/
static void
genEndCritical (iCode * ic)
{
  emitComment (TRACEGEN, __func__);

  if (IC_RIGHT (ic))
    {
      aopOp (IC_RIGHT (ic), ic, false);
      loadRegFromAop (m6502_reg_a, AOP (IC_RIGHT (ic)), 0);
      emit6502op ("pha", "");
      emit6502op ("plp", "");
      m6502_freeReg (m6502_reg_a);
      freeAsmop (IC_RIGHT (ic), NULL, ic, true);
    }
  else
    {
      emit6502op ("plp", "");
    }
}

static void
updateiTempRegisterUse (operand * op)
{
  symbol *sym;

  if (IS_ITEMP (op))
    {
      sym = OP_SYMBOL (op);
      if (!sym->isspilt)
        {
	  /* If only used by IFX, there might not be any register assigned */
          int i;
          for(i = 0; i < sym->nRegs; i++)
            if (sym->regs[i])
              m6502_useReg (sym->regs[i]);
        }
    }
}

/*---------------------------------------------------------------------*/
/* genm6502iCode - generate code for M6502 based controllers for a single iCode instruction */
/*---------------------------------------------------------------------*/
static void
genm6502iCode (iCode *ic)
  {
    int i;
    reg_info *reg;

    initGenLineElement ();
    genLine.lineElement.ic = ic;

#if 0
  if (!regalloc_dry_run)
    printf ("ic %d op %d stack pushed %d\n", ic->key, ic->op, G.stack.pushed);
#endif

  if (resultRemat (ic))
    {
      if (!regalloc_dry_run)
        emitComment(TRACEGEN, "skipping iCode since result will be rematerialized");
      return;
    }

  if (ic->generated)
    {
      if (!regalloc_dry_run)
        emitComment(TRACEGEN, "skipping generated iCode");
      return;
    }

    for (i = A_IDX; i <= XA_IDX; i++)
      {
        reg = m6502_regWithIdx (i);
        //if (reg->aop)
        //  emitcode ("", "; %s = %s offset %d", reg->name, aopName (reg->aop), reg->aopofs);
        reg->isFree = true;
        if (regalloc_dry_run)
          reg->isLitConst = 0;
      }

    if (ic->op == IFX)
      updateiTempRegisterUse (IC_COND (ic));
    else if (ic->op == JUMPTABLE)
      updateiTempRegisterUse (IC_JTCOND (ic));
    else if (ic->op == RECEIVE)
      {
        m6502_useReg (m6502_reg_a);
        m6502_useReg (m6502_reg_x); // TODO: x really is free if function only receives 1 byte
      }
    else
      {
        if (POINTER_SET (ic))
          updateiTempRegisterUse (IC_RESULT (ic));
        updateiTempRegisterUse (IC_LEFT (ic));
        updateiTempRegisterUse (IC_RIGHT (ic));
      }

    for (i = A_IDX; i <= Y_IDX; i++)
      {
        if (bitVectBitValue (ic->rSurv, i))
          {
            m6502_regWithIdx (i)->isDead = false;
            m6502_regWithIdx (i)->isFree = false;
          }
        else
          m6502_regWithIdx (i)->isDead = true;
      }

  /* depending on the operation */
  switch (ic->op)
    {
    case '!':
      genNot (ic);
      break;

    case '~':
      genCpl (ic);
      break;

    case UNARYMINUS:
          genUminus (ic);
          break;

    case IPUSH:
          genIpush (ic);
          break;

    case IPOP:
      /* IPOP happens only when trying to restore a
         spilt live range, if there is an ifx statement
         following this pop then the if statement might
         be using some of the registers being popped which
         would destory the contents of the register so
         we need to check for this condition and handle it */
      if (ic->next && ic->next->op == IFX && regsInCommon (IC_LEFT (ic), IC_COND (ic->next)))
        genIfx (ic->next, ic);
      else
        genIpop (ic);
      break;

    case CALL:
      genCall (ic);
      break;

    case PCALL:
      genPcall (ic);
      break;

    case FUNCTION:
      genFunction (ic);
      break;

    case ENDFUNCTION:
      genEndFunction (ic);
      break;

    case RETURN:
      genRet (ic);
      break;

    case LABEL:
      genLabel (ic);
      break;

    case GOTO:
      genGoto (ic);
      break;

    case '+':
      genPlus (ic);
      break;

    case '-':
      genMinus (ic);
      break;

    case '*':
      genMult (ic);
      break;

    case '/':
      genDiv (ic);
      break;

    case '%':
      genMod (ic);
      break;

    case '>':
    case '<':
    case LE_OP:
    case GE_OP:
      genCmp (ic, ifxForOp (IC_RESULT (ic), ic));
      break;

    case NE_OP:
    case EQ_OP:
      genCmpEQorNE (ic, ifxForOp (IC_RESULT (ic), ic));
      break;

    case AND_OP:
      genAndOp (ic);
      break;

    case OR_OP:
      genOrOp (ic);
      break;

    case '^':
      genXor (ic, ifxForOp (IC_RESULT (ic), ic));
      break;

    case '|':
      genOr (ic, ifxForOp (IC_RESULT (ic), ic));
      break;

    case BITWISEAND:
      genAnd (ic, ifxForOp (IC_RESULT (ic), ic));
      break;

    case INLINEASM:
      m6502_genInline (ic);
      break;

    case RRC:
      genRRC (ic);
      break;

    case RLC:
      genRLC (ic);
      break;

    case SWAP:
    case GETABIT:
    case GETBYTE:
    case GETWORD:
      wassertl (0, "Unimplemented iCode");
      break;

    case LEFT_OP:
      genLeftShift (ic);
      break;

    case RIGHT_OP:
      genRightShift (ic);
      break;

    case GET_VALUE_AT_ADDRESS:
      genPointerGet (ic, NULL); // TODO? ifxForOp (IC_RESULT (ic), ic));
      break;

   case SET_VALUE_AT_ADDRESS:
      genPointerSet (ic);
      break;

    case '=':
      if (POINTER_SET (ic))
        genPointerSet (ic);
      else
        genAssign (ic);
      break;

    case IFX:
      genIfx (ic, NULL);
      break;

    case ADDRESS_OF:
      genAddrOf (ic);
      break;

    case JUMPTABLE:
      genJumpTab (ic);
      break;

    case CAST:
      genCast (ic);
      break;

    case RECEIVE:
      genReceive (ic);
      break;

    case SEND:
      if (!regalloc_dry_run)
        addSet (&_G.sendSet, ic);
      else
        {
          set * sendSet = NULL;
          addSet (&sendSet, ic);
          genSend (sendSet);
          deleteSet (&sendSet);
        }
      break;

    case DUMMY_READ_VOLATILE:
      genDummyRead (ic);
      break;

    case CRITICAL:
      genCritical (ic);
      break;

    case ENDCRITICAL:
      genEndCritical (ic);
      break;

    default:
      wassertl (0, "Unknown iCode");
    }
}

static void
init_aop_pass(void)
{
  if (m6502_aop_pass[0])
    return;

  m6502_aop_pass[0] = newAsmop (AOP_REG);
  m6502_aop_pass[0]->size = 1;
  m6502_aop_pass[0]->aopu.aop_reg[0] = m6502_reg_a;
  m6502_aop_pass[1] = newAsmop (AOP_REG);
  m6502_aop_pass[1]->size = 1;
  m6502_aop_pass[1]->aopu.aop_reg[0] = m6502_reg_x;
  m6502_aop_pass[2] = newAsmop (AOP_DIR);
  m6502_aop_pass[2]->size = 1;
  m6502_aop_pass[2]->aopu.aop_dir = "___SDCC_m6502_ret2";
  m6502_aop_pass[3] = newAsmop (AOP_DIR);
  m6502_aop_pass[3]->size = 1;
  m6502_aop_pass[3]->aopu.aop_dir = "___SDCC_m6502_ret3";
  m6502_aop_pass[4] = newAsmop (AOP_DIR);
  m6502_aop_pass[4]->size = 1;
  m6502_aop_pass[4]->aopu.aop_dir = "___SDCC_m6502_ret4";
  m6502_aop_pass[5] = newAsmop (AOP_DIR);
  m6502_aop_pass[5]->size = 1;
  m6502_aop_pass[5]->aopu.aop_dir = "___SDCC_m6502_ret5";
  m6502_aop_pass[6] = newAsmop (AOP_DIR);
  m6502_aop_pass[6]->size = 1;
  m6502_aop_pass[6]->aopu.aop_dir = "___SDCC_m6502_ret6";
  m6502_aop_pass[7] = newAsmop (AOP_DIR);
  m6502_aop_pass[7]->size = 1;
  m6502_aop_pass[7]->aopu.aop_dir = "___SDCC_m6502_ret7";
}

float
drym6502iCode (iCode *ic)
{
  regalloc_dry_run = true;
  regalloc_dry_run_cost_bytes = 0;
  regalloc_dry_run_cost_cycles = 0;

  init_aop_pass();
  
  genm6502iCode (ic);

  destroy_line_list ();
  /*freeTrace (&_G.trace.aops);*/

  wassert (regalloc_dry_run);

  int byte_cost_weight = 1;
  if(optimize.codeSize) byte_cost_weight*=2;
  if(!optimize.codeSpeed) byte_cost_weight*=4;

//   return (2* (float)regalloc_dry_run_cost_bytes + (float)regalloc_dry_run_cost_cycles* ic->count);

  return ((float)regalloc_dry_run_cost_bytes * byte_cost_weight + regalloc_dry_run_cost_cycles * ic->count);
}

/*---------------------------------------------------------------------*/
/* genm6502Code - generate code for M6502 based controllers            */
/*---------------------------------------------------------------------*/
void
genm6502Code (iCode *lic)
{
  iCode *ic;
  int clevel = 0;
  int cblock = 0;
  int cln = 0;
  regalloc_dry_run = false;

  m6502_dirtyReg (m6502_reg_a);
  m6502_dirtyReg (m6502_reg_y);
  m6502_dirtyReg (m6502_reg_x);
  _G.tempOfs = 0;

  /* print the allocation information */
  if (allocInfo && currFunc)
    printAllocInfo (currFunc, codeOutBuf);
  /* if debug information required */
  if (options.debug && currFunc && !regalloc_dry_run)
      debugFile->writeFunction (currFunc, lic);

  if (options.debug && !regalloc_dry_run)
    debugFile->writeFrameAddress (NULL, NULL, 0); /* have no idea where frame is now */

  init_aop_pass();

  for (ic = lic; ic; ic = ic->next)
    ic->generated = false;

  for (ic = lic; ic; ic = ic->next)
    {
      initGenLineElement ();

      genLine.lineElement.ic = ic;

      if (ic->level != clevel || ic->block != cblock)
        {
          if (options.debug)
              debugFile->writeScope (ic);
          clevel = ic->level;
          cblock = ic->block;
        }

      if (ic->lineno && cln != ic->lineno)
        {
          if (options.debug)
              debugFile->writeCLine (ic);

          if (!options.noCcodeInAsm)
            emitComment (ALWAYS, "%s: %d: %s", ic->filename, ic->lineno, printCLine (ic->filename, ic->lineno));
          cln = ic->lineno;
        }

      regalloc_dry_run_cost_bytes = 0;
      regalloc_dry_run_cost_cycles = 0;

      if (options.iCodeInAsm)
        {
          char regsSurv[4];
          const char *iLine;

          regsSurv[0] = (bitVectBitValue (ic->rSurv, A_IDX)) ? 'a' : '-';
          regsSurv[1] = (bitVectBitValue (ic->rSurv, Y_IDX)) ? 'y' : '-';
          regsSurv[2] = (bitVectBitValue (ic->rSurv, X_IDX)) ? 'x' : '-';
          regsSurv[3] = 0;
          iLine = printILine (ic);
          emitComment (ALWAYS, " [%s] ic:%d: %s", regsSurv, ic->key, iLine);
          dbuf_free (iLine);
        }

      genm6502iCode(ic);
      emitComment (TRACEGEN, "Raw cost for generated ic %d : (%d, %f) count=%f", ic->key, regalloc_dry_run_cost_bytes, regalloc_dry_run_cost_cycles, ic->count);

      // TODO: should be asserts?
/*
      if (!m6502_reg_a->isFree)
        emitComment (REGOPS|VVDBG, "  forgot to free a");
      if (!m6502_reg_x->isFree)
        emitComment (REGOPS|VVDBG, "  forgot to free x");
      if (!m6502_reg_y->isFree)
        emitComment (REGOPS|VVDBG, "  forgot to free y");
      if (!m6502_reg_yx->isFree)
        emitComment (REGOPS|VVDBG, "  forgot to free yx");
      if (!m6502_reg_xa->isFree)
        emitComment (REGOPS|VVDBG, "  forgot to free xa");
*/

      if (_G.tempOfs != 0)
        emitcode("ERROR", "; forgot to free temp stack (%d)", _G.tempOfs);
    }

  if (options.debug)
    debugFile->writeFrameAddress (NULL, NULL, 0); /* have no idea where frame is now */

  /* now we are ready to call the
     peep hole optimizer */
  if (!options.nopeep)
    peepHole (&genLine.lineHead);

  /* now do the actual printing */
  printLine (genLine.lineHead, codeOutBuf);

  /* destroy the line list */
  destroy_line_list ();
}

