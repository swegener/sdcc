/*-------------------------------------------------------------------------
  gen.c - source file for code generation for 8051

  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)

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

  Notes:
  000123 mlh  Moved aopLiteral to SDCCglue.c to help the split
      Made everything static
-------------------------------------------------------------------------*/

#define D(x) do if (options.verboseAsm) {x;} while(0)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SDCCglobl.h"
#include "newalloc.h"

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "rtrack.h"
#include "gen.h"
#include "dbuf_string.h"

char *aopLiteral (value * val, int offset);
char *aopLiteralLong (value * val, int offset, int size);
extern int allocInfo;

/* this is the down and dirty file with all kinds of
   kludgy & hacky stuff. This is what it is all about
   CODE GENERATION for a specific MCU . some of the
   routines may be reusable, will have to see */

static char *zero = "#0x00";
static char *one = "#0x01";
static char *spname;

char *fReturn8051[] =
{"dpl", "dph", "b", "a"};
unsigned fReturnSizeMCS51 = 4;  /* shared with ralloc.c */
char **fReturn = fReturn8051;
static char *accUse[] =
{"a", "b"};

static unsigned short rbank = -1;

#define REG_WITH_INDEX   mcs51_regWithIdx

#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define IS_AOP_PREG(x) (AOP(x) && (AOP_TYPE(x) == AOP_R1 || \
                        AOP_TYPE(x) == AOP_R0))

#define AOP_NEEDSACC(x) (AOP(x) && (AOP_TYPE(x) == AOP_CRY ||  \
                         AOP_TYPE(x) == AOP_DPTR || \
                         AOP(x)->paged))

#define AOP_INPREG(x) (x && (x->type == AOP_REG &&                        \
                       (x->aopu.aop_reg[0] == REG_WITH_INDEX(R0_IDX) || \
                        x->aopu.aop_reg[0] == REG_WITH_INDEX(R1_IDX) )))

#define SYM_BP(sym)   (SPEC_OCLS (sym->etype)->paged ? "_bpx" : "_bp")

#define R0INB   _G.bu.bs.r0InB
#define R1INB   _G.bu.bs.r1InB
#define OPINB   _G.bu.bs.OpInB
#define BINUSE  _G.bu.BInUse

static struct
  {
    short r0Pushed;
    short r1Pushed;
    union
      {
        struct
          {
            short r0InB : 2;//2 so we can see it overflow
            short r1InB : 2;//2 so we can see it overflow
            short OpInB : 2;//2 so we can see it overflow
          } bs;
        short BInUse;
      } bu;
    short accInUse;
    short inLine;
    short debugLine;
    short nRegsSaved;
    set *sendSet;
    iCode *current_iCode;
    symbol *currentFunc;
  }
_G;

static char *rb1regs[] = {
    "b1_0","b1_1","b1_2","b1_3","b1_4","b1_5","b1_6","b1_7",
    "b0",  "b1",  "b2",  "b3",  "b4",  "b5",  "b6",  "b7"
};

extern struct dbuf_s *codeOutBuf;

#define RESULTONSTACK(x) \
                         (IC_RESULT(x) && IC_RESULT(x)->aop && \
                         IC_RESULT(x)->aop->type == AOP_STK )

#define MOVA(x)  mova(x)  /* use function to avoid multiple eval */
#define MOVB(x)  movb(x)

#define CLRC     emitcode("clr","c")
#define SETC     emitcode("setb","c")

static lineNode *lineHead = NULL;
static lineNode *lineCurr = NULL;

static unsigned char SLMask[] =
{0xFF, 0xFE, 0xFC, 0xF8, 0xF0,
 0xE0, 0xC0, 0x80, 0x00};
static unsigned char SRMask[] =
{0xFF, 0x7F, 0x3F, 0x1F, 0x0F,
 0x07, 0x03, 0x01, 0x00};

#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3

/*-----------------------------------------------------------------*/
/* emitcode - writes the code into a file : for now it is simple    */
/*-----------------------------------------------------------------*/
void
emitcode (const char *inst, const char *fmt,...)
{
  va_list ap;
  struct dbuf_s dbuf;
  const char *lbp, *lb;

  dbuf_init (&dbuf, INITIAL_INLINEASM);

  va_start (ap, fmt);

  if (inst && *inst)
    {
      dbuf_append_str (&dbuf, inst);

      if (fmt && *fmt)
        {
          dbuf_append_char (&dbuf, '\t');
          dbuf_tvprintf (&dbuf, fmt, ap);
        }
    }
  else
    {
      dbuf_tvprintf (&dbuf, fmt, ap);
    }

  lbp = lb = dbuf_c_str(&dbuf);

  while (isspace ((unsigned char)*lbp))
    {
      lbp++;
    }

  if (lbp)
    {
      rtrackUpdate (lbp);

      lineCurr = (lineCurr ?
                  connectLine (lineCurr, newLineNode (lb)) :
                  (lineHead = newLineNode (lb)));

      lineCurr->isInline = _G.inLine;
      lineCurr->isDebug = _G.debugLine;
      lineCurr->ic = _G.current_iCode;
      lineCurr->isComment = (*lbp==';');
    }

  va_end (ap);

  dbuf_destroy(&dbuf);
}

static void
emitLabel (symbol *tlbl)
{
  emitcode ("", "%05d$:", tlbl->key + 100);
  lineCurr->isLabel = 1;
}

/*-----------------------------------------------------------------*/
/* mcs51_emitDebuggerSymbol - associate the current code location  */
/*   with a debugger symbol                                        */
/*-----------------------------------------------------------------*/
void
mcs51_emitDebuggerSymbol (char * debugSym)
{
  _G.debugLine = 1;
  emitcode ("", "%s ==.", debugSym);
  _G.debugLine = 0;
}

/*-----------------------------------------------------------------*/
/* mova - moves specified value into accumulator                   */
/*-----------------------------------------------------------------*/
static void
mova (const char *x)
{
  /* do some early peephole optimization */
  if (!strncmp(x, "a", 2) || !strncmp(x, "acc", 4))
    return;

  /* if it is a literal mov try to get it cheaper */
  if (*x == '#' &&
      rtrackMoveALit(x))
    return;

  emitcode("mov", "a,%s", x);
}

/*-----------------------------------------------------------------*/
/* movb - moves specified value into register b                    */
/*-----------------------------------------------------------------*/
static void
movb (const char *x)
{
  /* do some early peephole optimization */
  if (!strncmp(x, "b", 2))
    return;

  /* if it is a literal mov try to get it cheaper */
  if (*x == '#')
    {
      emitcode("mov","b,%s", rtrackGetLit(x));
      return;
    }

  emitcode("mov","b,%s", x);
}

/*-----------------------------------------------------------------*/
/* pushB - saves register B if necessary                           */
/*-----------------------------------------------------------------*/
static bool
pushB (void)
{
  bool pushedB = FALSE;

  if (BINUSE)
    {
      emitcode ("push", "b");
//    printf("B was in use !\n");
      pushedB = TRUE;
    }
  else
    {
      OPINB++;
    }
  return pushedB;
}

/*-----------------------------------------------------------------*/
/* popB - restores value of register B if necessary                */
/*-----------------------------------------------------------------*/
static void
popB (bool pushedB)
{
  if (pushedB)
    {
      emitcode ("pop", "b");
    }
  else
    {
      OPINB--;
    }
}

/*-----------------------------------------------------------------*/
/* pushReg - saves register                                        */
/*-----------------------------------------------------------------*/
static bool
pushReg (int index, bool bits_pushed)
{
  regs * reg = REG_WITH_INDEX (index);
  if (reg->type == REG_BIT)
    {
      if (!bits_pushed)
        emitcode ("push", "%s", reg->base);
      return TRUE;
    }
  else
    emitcode ("push", "%s", reg->dname);
  return bits_pushed;
}

/*-----------------------------------------------------------------*/
/* popReg - restores register                                      */
/*-----------------------------------------------------------------*/
static bool
popReg (int index, bool bits_popped)
{
  regs * reg = REG_WITH_INDEX (index);
  if (reg->type == REG_BIT)
    {
      if (!bits_popped)
        emitcode ("pop", "%s", reg->base);
      return TRUE;
    }
  else
    emitcode ("pop", "%s", reg->dname);
  return bits_popped;
}

/*-----------------------------------------------------------------*/
/* getFreePtr - returns r0 or r1 whichever is free or can be pushed */
/*-----------------------------------------------------------------*/
static regs *
getFreePtr (iCode * ic, asmop ** aopp, bool result)
{
  bool r0iu, r1iu;
  bool r0ou, r1ou;

  /* the logic: if r0 & r1 used in the instruction
     then we are in trouble otherwise */

  /* first check if r0 & r1 are used by this
     instruction, in which case we are in trouble */
  r0iu = bitVectBitValue (ic->rUsed, R0_IDX);
  r1iu = bitVectBitValue (ic->rUsed, R1_IDX);
  if (r0iu && r1iu) {
      goto endOfWorld;
    }

  r0ou = bitVectBitValue (ic->rMask, R0_IDX);
  r1ou = bitVectBitValue (ic->rMask, R1_IDX);

  /* if no usage of r0 then return it */
  if (!r0iu && !r0ou)
    {
      ic->rUsed = bitVectSetBit (ic->rUsed, R0_IDX);
      (*aopp)->type = AOP_R0;

      return (*aopp)->aopu.aop_ptr = REG_WITH_INDEX (R0_IDX);
    }

  /* if no usage of r1 then return it */
  if (!r1iu && !r1ou)
    {
      ic->rUsed = bitVectSetBit (ic->rUsed, R1_IDX);
      (*aopp)->type = AOP_R1;

      return (*aopp)->aopu.aop_ptr = REG_WITH_INDEX (R1_IDX);
    }

  /* now we know they both have usage */
  /* if r0 not used in this instruction */
  if (!r0iu)
    {
      /* push it if not already pushed */
      if (ic->op == IPUSH)
        {
          MOVB (REG_WITH_INDEX (R0_IDX)->dname);
          R0INB++;
        }
      else if (!_G.r0Pushed)
        {
          emitcode ("push", "%s",
                    REG_WITH_INDEX (R0_IDX)->dname);
          _G.r0Pushed++;
        }

      ic->rUsed = bitVectSetBit (ic->rUsed, R0_IDX);
      (*aopp)->type = AOP_R0;

      return (*aopp)->aopu.aop_ptr = REG_WITH_INDEX (R0_IDX);
    }

  /* if r1 not used then */

  if (!r1iu)
    {
      /* push it if not already pushed */
      if (ic->op == IPUSH)
        {
          MOVB (REG_WITH_INDEX (R1_IDX)->dname);
          R1INB++;
        }
      else if (!_G.r1Pushed)
        {
          emitcode ("push", "%s",
                    REG_WITH_INDEX (R1_IDX)->dname);
          _G.r1Pushed++;
        }

      ic->rUsed = bitVectSetBit (ic->rUsed, R1_IDX);
      (*aopp)->type = AOP_R1;
      return REG_WITH_INDEX (R1_IDX);
    }

endOfWorld:
  /* I said end of world, but not quite end of world yet */
  /* if this is a result then we can push it on the stack */
  if (result)
    {
      (*aopp)->type = AOP_STK;
      return NULL;
    }
  /* in the case that result AND left AND right needs a pointer reg
     we can safely use the result's */
  if (bitVectBitValue (mcs51_rUmaskForOp(IC_RESULT(ic)), R0_IDX))
    {
      (*aopp)->type = AOP_R0;
      return REG_WITH_INDEX (R0_IDX);
    }
  if (bitVectBitValue (mcs51_rUmaskForOp(IC_RESULT(ic)), R1_IDX))
    {
      (*aopp)->type = AOP_R1;
      return REG_WITH_INDEX (R1_IDX);
    }

  /* now this is REALLY the end of the world */
  werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
          "getFreePtr should never reach here");
  exit (1);
}


/*-----------------------------------------------------------------*/
/* getTempRegs - initialize an array of pointers to GPR registers */
/*               that are not in use. Returns 1 if the requested   */
/*               number of registers were available, 0 otherwise.  */
/*-----------------------------------------------------------------*/
int
getTempRegs(regs **tempRegs, int size, iCode *ic)
{
  bitVect * freeRegs;
  int i;
  int offset;

  if (!ic)
    ic = _G.current_iCode;
  if (!ic)
    return 0;
  if (!_G.currentFunc)
    return 0;

  freeRegs = newBitVect(8);
  bitVectSetBit (freeRegs, R2_IDX);
  bitVectSetBit (freeRegs, R3_IDX);
  bitVectSetBit (freeRegs, R4_IDX);
  bitVectSetBit (freeRegs, R5_IDX);
  bitVectSetBit (freeRegs, R6_IDX);
  bitVectSetBit (freeRegs, R7_IDX);

  if (IFFUNC_CALLEESAVES(_G.currentFunc->type))
    {
      bitVect * newfreeRegs;
      newfreeRegs = bitVectIntersect (freeRegs, _G.currentFunc->regsUsed);
      freeBitVect(freeRegs);
      freeRegs = newfreeRegs;
    }
  freeRegs = bitVectCplAnd (freeRegs, ic->rMask);

  offset = 0;
  for (i=0; i<freeRegs->size; i++)
    {
      if (bitVectBitValue(freeRegs,i))
        tempRegs[offset++] = REG_WITH_INDEX(i);
      if (offset>=size)
        {
          freeBitVect(freeRegs);
          return 1;
        }
    }

  freeBitVect(freeRegs);
  return 0;
}


/*-----------------------------------------------------------------*/
/* newAsmop - creates a new asmOp                                  */
/*-----------------------------------------------------------------*/
static asmop *
newAsmop (short type)
{
  asmop *aop;

  aop = Safe_calloc (1, sizeof (asmop));
  aop->type = type;
  aop->allocated = 1;
  return aop;
}

/*-----------------------------------------------------------------*/
/* pointerCode - returns the code for a pointer type               */
/*-----------------------------------------------------------------*/
static int
pointerCode (sym_link * etype)
{

  return PTR_TYPE (SPEC_OCLS (etype));

}

/*-----------------------------------------------------------------*/
/* leftRightUseAcc - returns size of accumulator use by operands   */
/*-----------------------------------------------------------------*/
static int
leftRightUseAcc(iCode *ic)
{
  operand *op;
  int size;
  int accuseSize = 0;
  int accuse = 0;

  if (!ic)
    {
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "null iCode pointer");
      return 0;
    }

  if (ic->op == IFX)
    {
      op = IC_COND (ic);
      if (IS_SYMOP (op) && OP_SYMBOL (op) && OP_SYMBOL (op)->accuse)
        {
          accuse = 1;
          size = getSize (OP_SYMBOL (op)->type);
          if (size>accuseSize)
            accuseSize = size;
        }
    }
  else if (ic->op == JUMPTABLE)
    {
      op = IC_JTCOND (ic);
      if (IS_SYMOP (op) && OP_SYMBOL (op) && OP_SYMBOL (op)->accuse)
        {
          accuse = 1;
          size = getSize (OP_SYMBOL (op)->type);
          if (size>accuseSize)
            accuseSize = size;
        }
    }
  else
    {
      op = IC_LEFT (ic);
      if (IS_SYMOP (op) && OP_SYMBOL (op) && OP_SYMBOL (op)->accuse)
        {
          accuse = 1;
          size = getSize (OP_SYMBOL (op)->type);
          if (size>accuseSize)
            accuseSize = size;
        }
      op = IC_RIGHT (ic);
      if (IS_SYMOP (op) && OP_SYMBOL (op) && OP_SYMBOL (op)->accuse)
        {
          accuse = 1;
          size = getSize (OP_SYMBOL (op)->type);
          if (size>accuseSize)
            accuseSize = size;
        }
    }

  if (accuseSize)
    return accuseSize;
  else
    return accuse;
}

/*-----------------------------------------------------------------*/
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *
aopForSym (iCode * ic, symbol * sym, bool result)
{
  asmop *aop;
  memmap *space;
  bool accuse = leftRightUseAcc (ic) || _G.accInUse;

  wassertl (ic != NULL, "Got a null iCode");
  wassertl (sym != NULL, "Got a null symbol");

  space = SPEC_OCLS (sym->etype);

  /* if already has one */
  if (sym->aop)
    {
      sym->aop->allocated++;
      return sym->aop;
    }

  /* assign depending on the storage class */
  /* if it is on the stack or indirectly addressable */
  /* space we need to assign either r0 or r1 to it   */
  if (sym->onStack || sym->iaccess)
    {
      sym->aop = aop = newAsmop (0);
      aop->aopu.aop_ptr = getFreePtr (ic, &aop, result);
      aop->size = getSize (sym->type);

      /* now assign the address of the variable to
         the pointer register */
      if (aop->type != AOP_STK)
        {
          if (sym->onStack)
            {
              signed char offset = ((sym->stack < 0) ?
                         ((signed char) (sym->stack - _G.nRegsSaved)) :
                         ((signed char) sym->stack)) & 0xff;

              if ((abs(offset) <= 3) ||
                  (accuse && (abs(offset) <= 7)))
                {
                  emitcode ("mov", "%s,%s",
                            aop->aopu.aop_ptr->name, SYM_BP (sym));
                  while (offset < 0)
                    {
                      emitcode ("dec", aop->aopu.aop_ptr->name);
                      offset++;
                    }
                  while (offset > 0)
                    {
                      emitcode ("inc", aop->aopu.aop_ptr->name);
                      offset--;
                    }
                }
              else
                {
                  if (accuse)
                    emitcode ("push", "acc");
                  emitcode ("mov", "a,%s", SYM_BP (sym));
                  emitcode ("add", "a,#0x%02x", offset & 0xff);
                  emitcode ("mov", "%s,a", aop->aopu.aop_ptr->name);
                  if (accuse)
                    emitcode ("pop", "acc");
                }
            }
          else
            {
              emitcode ("mov", "%s,#%s",
                        aop->aopu.aop_ptr->name,
                        sym->rname);
            }
          aop->paged = space->paged;
        }
      else
        aop->aopu.aop_stk = sym->stack;
      return aop;
    }

  /* if in bit space */
  if (IN_BITSPACE (space))
    {
      sym->aop = aop = newAsmop (AOP_CRY);
      aop->aopu.aop_dir = sym->rname;
      aop->size = getSize (sym->type);
      return aop;
    }
  /* if it is in direct space */
  if (IN_DIRSPACE (space))
    {
      //printf("aopForSym, using AOP_DIR for %s (%x)\n", sym->name, sym);
      //printTypeChainRaw(sym->type, NULL);
      //printf("space = %s\n", space ? space->sname : "NULL");
      sym->aop = aop = newAsmop (AOP_DIR);
      aop->aopu.aop_dir = sym->rname;
      aop->size = getSize (sym->type);
      return aop;
    }

  /* special case for a function */
  if (IS_FUNC (sym->type))
    {
      sym->aop = aop = newAsmop (AOP_IMMD);
      aop->aopu.aop_immd.aop_immd1 = Safe_strdup(sym->rname);
      aop->size = getSize (sym->type);
      return aop;
    }

  /* only remaining is far space */
  /* in which case DPTR gets the address */
  sym->aop = aop = newAsmop (AOP_DPTR);
  emitcode ("mov", "dptr,#%s", sym->rname);
  aop->size = getSize (sym->type);

  /* if it is in code space */
  if (IN_CODESPACE (space))
    aop->code = 1;

  return aop;
}

/*-----------------------------------------------------------------*/
/* aopForRemat - rematerialzes an object                           */
/*-----------------------------------------------------------------*/
static asmop *
aopForRemat (symbol * sym)
{
  iCode *ic = sym->rematiCode;
  asmop *aop = newAsmop (AOP_IMMD);
  int ptr_type = 0;
  int val = 0;

  for (;;)
    {
      if (ic->op == '+')
        val += (int) operandLitValue (IC_RIGHT (ic));
      else if (ic->op == '-')
        val -= (int) operandLitValue (IC_RIGHT (ic));
      else if (IS_CAST_ICODE(ic)) {
              sym_link *from_type = operandType(IC_RIGHT(ic));
              aop->aopu.aop_immd.from_cast_remat = 1;
              ic = OP_SYMBOL (IC_RIGHT (ic))->rematiCode;
              ptr_type = pointerTypeToGPByte (DCL_TYPE(from_type), NULL, NULL);
              continue;
      } else break;

      ic = OP_SYMBOL (IC_LEFT (ic))->rematiCode;
    }

  if (val)
    {
      SNPRINTF (buffer, sizeof(buffer),
                "(%s %c 0x%04x)",
                OP_SYMBOL (IC_LEFT (ic))->rname,
                val >= 0 ? '+' : '-',
                abs (val) & 0xffff);
    }
  else
    {
      strncpyz (buffer, OP_SYMBOL (IC_LEFT (ic))->rname, sizeof(buffer));
    }

  aop->aopu.aop_immd.aop_immd1 = Safe_strdup(buffer);
  /* set immd2 field if required */
  if (aop->aopu.aop_immd.from_cast_remat)
    {
      SNPRINTF (buffer, sizeof(buffer), "#0x%02x", ptr_type);
      aop->aopu.aop_immd.aop_immd2 = Safe_strdup(buffer);
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
    return FALSE;

  sym1 = OP_SYMBOL (op1);
  sym2 = OP_SYMBOL (op2);

  if (sym1->nRegs == 0 || sym2->nRegs == 0)
    return FALSE;

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
            return TRUE;
        }
    }

  return FALSE;
}

/*-----------------------------------------------------------------*/
/* operandsEqu - equivalent                                        */
/*-----------------------------------------------------------------*/
static bool
operandsEqu (operand * op1, operand * op2)
{
  symbol *sym1, *sym2;

  /* if they're not symbols */
  if (!IS_SYMOP (op1) || !IS_SYMOP (op2))
    return FALSE;

  sym1 = OP_SYMBOL (op1);
  sym2 = OP_SYMBOL (op2);

  /* if both are itemps & one is spilt
     and the other is not then false */
  if (IS_ITEMP (op1) && IS_ITEMP (op2) &&
      sym1->isspilt != sym2->isspilt)
    return FALSE;

  /* if they are the same */
  if (sym1 == sym2)
    return TRUE;

  /* if they have the same rname */
  if (sym1->rname[0] && sym2->rname[0] &&
      strcmp (sym1->rname, sym2->rname) == 0 &&
      !(IS_PARM (op2) && IS_ITEMP (op1)))
    return TRUE;

  /* if left is a tmp & right is not */
  if (IS_ITEMP (op1) &&
      !IS_ITEMP (op2) &&
      sym1->isspilt &&
      (sym1->usl.spillLoc == sym2))
    return TRUE;

  if (IS_ITEMP (op2) &&
      !IS_ITEMP (op1) &&
      sym2->isspilt &&
      sym1->level > 0 &&
      (sym2->usl.spillLoc == sym1))
    return TRUE;

  return FALSE;
}

/*-----------------------------------------------------------------*/
/* sameByte - two asmops have the same address at given offsets    */
/*-----------------------------------------------------------------*/
static bool
sameByte (asmop * aop1, int off1, asmop * aop2, int off2)
{
  if (aop1 == aop2 && off1 == off2)
    return TRUE;

  if (aop1->type != AOP_REG && aop1->type != AOP_CRY)
    return FALSE;

  if (aop1->type != aop2->type)
    return FALSE;

  if (aop1->aopu.aop_reg[off1] != aop2->aopu.aop_reg[off2])
    return FALSE;

  return TRUE;
}

/*-----------------------------------------------------------------*/
/* sameRegs - two asmops have the same registers                   */
/*-----------------------------------------------------------------*/
static bool
sameRegs (asmop * aop1, asmop * aop2)
{
  int i;

  if (aop1 == aop2)
    return TRUE;

  if (aop1->type != AOP_REG && aop1->type != AOP_CRY)
    return FALSE;

  if (aop1->type != aop2->type)
    return FALSE;

  if (aop1->size != aop2->size)
    return FALSE;

  for (i = 0; i < aop1->size; i++)
    if (aop1->aopu.aop_reg[i] != aop2->aopu.aop_reg[i])
      return FALSE;

  return TRUE;
}

/*-----------------------------------------------------------------*/
/* aopOp - allocates an asmop for an operand  :                    */
/*-----------------------------------------------------------------*/
static void
aopOp (operand * op, iCode * ic, bool result)
{
  asmop *aop;
  symbol *sym;
  int i;

  if (!op)
    return;

  /* if this a literal */
  if (IS_OP_LITERAL (op))
    {
      op->aop = aop = newAsmop (AOP_LIT);
      aop->aopu.aop_lit = op->operand.valOperand;
      aop->size = getSize (operandType (op));
      return;
    }

  /* if already has a asmop then continue */
  if (op->aop)
    {
      op->aop->allocated++;
      return;
    }

  /* if the underlying symbol has a aop */
  if (IS_SYMOP (op) && OP_SYMBOL (op)->aop)
    {
      op->aop = OP_SYMBOL (op)->aop;
      op->aop->allocated++;
      return;
    }

  /* if this is a true symbol */
  if (IS_TRUE_SYMOP (op))
    {
      op->aop = aopForSym (ic, OP_SYMBOL (op), result);
      return;
    }

  /* this is a temporary : this has
     only five choices :
     a) register
     b) spillocation
     c) rematerialize
     d) conditional
     e) can be a return use only */

  sym = OP_SYMBOL (op);

  /* if the type is a conditional */
  if (sym->regType == REG_CND)
    {
      sym->aop = op->aop = aop = newAsmop (AOP_CRY);
      aop->size = sym->ruonly ? 1 : 0;
      return;
    }

  /* if it is spilt then two situations
     a) is rematerialize
     b) has a spill location */
  if (sym->isspilt || sym->nRegs == 0)
    {

      /* rematerialize it NOW */
      if (sym->remat)
        {
          sym->aop = op->aop = aop = aopForRemat (sym);
          aop->size = operandSize (op);
          return;
        }

      if (sym->accuse)
        {
          int i;
          sym->aop = op->aop = aop = newAsmop (AOP_ACC);
          aop->size = getSize (sym->type);
          for (i = 0; i < 2; i++)
            aop->aopu.aop_str[i] = accUse[i];
          return;
        }

      if (sym->ruonly)
        {
          unsigned i;

          sym->aop = op->aop = aop = newAsmop (AOP_STR);
          aop->size = getSize (sym->type);
          for (i = 0; i < fReturnSizeMCS51; i++)
            aop->aopu.aop_str[i] = fReturn[i];
          return;
        }

      if (sym->usl.spillLoc)
        {
          asmop *oldAsmOp = NULL;

          if (getSize(sym->type) != getSize(sym->usl.spillLoc->type))
            {
              /* force a new aop if sizes differ */
              oldAsmOp = sym->usl.spillLoc->aop;
              sym->usl.spillLoc->aop = NULL;
            }
          sym->aop = op->aop = aop =
                     aopForSym (ic, sym->usl.spillLoc, result);
          if (getSize(sym->type) != getSize(sym->usl.spillLoc->type))
            {
              /* Don't reuse the new aop, go with the last one */
              sym->usl.spillLoc->aop = oldAsmOp;
            }
          aop->size = getSize (sym->type);
          return;
        }

      /* else must be a dummy iTemp */
      sym->aop = op->aop = aop = newAsmop (AOP_DUMMY);
      aop->size = getSize (sym->type);
      return;
    }

  /* if the type is a bit register */
  if (sym->regType == REG_BIT)
    {
      sym->aop = op->aop = aop = newAsmop (AOP_CRY);
      aop->size = sym->nRegs;//1???
      aop->aopu.aop_reg[0] = sym->regs[0];
      aop->aopu.aop_dir = sym->regs[0]->name;
      return;
    }

  /* must be in a register */
  sym->aop = op->aop = aop = newAsmop (AOP_REG);
  aop->size = sym->nRegs;
  for (i = 0; i < sym->nRegs; i++)
    aop->aopu.aop_reg[i] = sym->regs[i];
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

  aop->allocated--;

  if (aop->allocated)
    goto dealloc;

  /* depending on the asmop type only three cases need work
     AOP_R0, AOP_R1 & AOP_STK */
  switch (aop->type)
    {
    case AOP_R0:
      if (R0INB)
        {
          emitcode ("mov", "r0,b");
          R0INB--;
        }
      else if (_G.r0Pushed)
        {
          if (pop)
            {
              emitcode ("pop", "ar0");
              _G.r0Pushed--;
            }
        }
      bitVectUnSetBit (ic->rUsed, R0_IDX);
      break;

    case AOP_R1:
      if (R1INB)
        {
          emitcode ("mov", "r1,b");
          R1INB--;
        }
      else if (_G.r1Pushed)
        {
          if (pop)
            {
              emitcode ("pop", "ar1");
              _G.r1Pushed--;
            }
        }
      bitVectUnSetBit (ic->rUsed, R1_IDX);
      break;

    case AOP_STK:
      {
        int sz = aop->size;
        int stk = aop->aopu.aop_stk + aop->size - 1;
        bitVectUnSetBit (ic->rUsed, R0_IDX);
        bitVectUnSetBit (ic->rUsed, R1_IDX);

        getFreePtr (ic, &aop, FALSE);

        if (stk)
          {
            emitcode ("mov", "a,_bp");
            emitcode ("add", "a,#0x%02x", ((char) stk) & 0xff);
            emitcode ("mov", "%s,a", aop->aopu.aop_ptr->name);
          }
        else
          {
            emitcode ("mov", "%s,_bp", aop->aopu.aop_ptr->name);
          }

        while (sz--)
          {
            emitcode ("pop", "acc");
            emitcode ("mov", "@%s,a", aop->aopu.aop_ptr->name);
            if (!sz)
              break;
            emitcode ("dec", "%s", aop->aopu.aop_ptr->name);
          }
        op->aop = aop;
        freeAsmop (op, NULL, ic, TRUE);
        if (_G.r1Pushed)
          {
            emitcode ("pop", "ar1");
            _G.r1Pushed--;
          }
        if (_G.r0Pushed)
          {
            emitcode ("pop", "ar0");
            _G.r0Pushed--;
          }
      }
      break;
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

/*------------------------------------------------------------------*/
/* freeForBranchAsmop - partial free up of Asmop for a branch; just */
/*                      pop r0 or r1 off stack if pushed            */
/*------------------------------------------------------------------*/
static void
freeForBranchAsmop (operand * op)
{
  asmop *aop;

  if (!op)
    return;

  aop = op->aop;

  if (!aop)
    return;

  if (!aop->allocated)
    return;

  switch (aop->type)
    {
    case AOP_R0:
      if (R0INB)
        {
          emitcode ("mov", "r0,b");
        }
      else if (_G.r0Pushed)
        {
          emitcode ("pop", "ar0");
        }
      break;

    case AOP_R1:
      if (R1INB)
        {
          emitcode ("mov", "r1,b");
        }
      else if (_G.r1Pushed)
        {
          emitcode ("pop", "ar1");
        }
      break;

    case AOP_STK:
      {
        int sz = aop->size;
        int stk = aop->aopu.aop_stk + aop->size - 1;

        emitcode ("mov", "b,r0");
        if (stk)
          {
            emitcode ("mov", "a,_bp");
            emitcode ("add", "a,#0x%02x", ((char) stk) & 0xff);
            emitcode ("mov", "r0,a");
          }
        else
          {
            emitcode ("mov", "r0,_bp");
          }

        while (sz--)
          {
            emitcode ("pop", "acc");
            emitcode ("mov", "@r0,a");
            if (!sz)
              break;
            emitcode ("dec", "r0");
          }
        emitcode ("mov", "r0,b");
      }
    }

}

/*-----------------------------------------------------------------*/
/* aopGetUsesAcc - indicates ahead of time whether aopGet() will   */
/*                 clobber the accumulator                         */
/*-----------------------------------------------------------------*/
static bool
aopGetUsesAcc (operand * oper, int offset)
{
  asmop * aop = AOP (oper);

  if (offset > (aop->size - 1))
    return FALSE;

  switch (aop->type)
    {

    case AOP_R0:
    case AOP_R1:
      if (aop->paged)
        return TRUE;
      return FALSE;
    case AOP_DPTR:
      return TRUE;
    case AOP_IMMD:
      return FALSE;
    case AOP_DIR:
      return FALSE;
    case AOP_REG:
      wassert(strcmp(aop->aopu.aop_reg[offset]->name, "a"));
      return FALSE;
    case AOP_CRY:
      return TRUE;
    case AOP_ACC:
      if (offset)
        return FALSE;
      return TRUE;
    case AOP_LIT:
      return FALSE;
    case AOP_STR:
      if (strcmp (aop->aopu.aop_str[offset], "a") == 0)
        return TRUE;
      return FALSE;
    case AOP_DUMMY:
      return FALSE;
    default:
      /* Error case --- will have been caught already */
      wassert(0);
      return FALSE;
    }
}

/*-------------------------------------------------------------------*/
/* aopGet - for fetching value of the aop                            */
/*-------------------------------------------------------------------*/
static char *
aopGet (operand * oper, int offset, bool bit16, bool dname)
{
  asmop * aop = AOP (oper);

  /* offset is greater than
     size then zero */
  if (offset > (aop->size - 1) &&
      aop->type != AOP_LIT)
    return zero;

  /* depending on type */
  switch (aop->type)
    {
    case AOP_DUMMY:
      return zero;

    case AOP_R0:
    case AOP_R1:
      /* if we need to increment it */
      while (offset > aop->coff)
        {
          emitcode ("inc", "%s", aop->aopu.aop_ptr->name);
          aop->coff++;
        }

      while (offset < aop->coff)
        {
          emitcode ("dec", "%s", aop->aopu.aop_ptr->name);
          aop->coff--;
        }

      aop->coff = offset;
      if (aop->paged)
        {
          emitcode ("movx", "a,@%s", aop->aopu.aop_ptr->name);
          return (dname ? "acc" : "a");
        }
      SNPRINTF (buffer, sizeof(buffer), "@%s", aop->aopu.aop_ptr->name);
      return Safe_strdup(buffer);

    case AOP_DPTR:
      if (aop->code && aop->coff==0 && offset>=1) {
        emitcode ("mov", "a,#0x%02x", offset);
        emitcode ("movc", "a,@a+dptr");
        return (dname ? "acc" : "a");
      }

      while (offset > aop->coff)
        {
          emitcode ("inc", "dptr");
          aop->coff++;
        }

      while (offset < aop->coff)
        {
          emitcode ("lcall", "__decdptr");
          aop->coff--;
        }

      aop->coff = offset;
      if (aop->code)
        {
          emitcode ("clr", "a");
          emitcode ("movc", "a,@a+dptr");
        }
      else
        {
          emitcode ("movx", "a,@dptr");
        }
      return (dname ? "acc" : "a");

    case AOP_IMMD:
      if (aop->aopu.aop_immd.from_cast_remat && (offset == (aop->size-1)))
        {
          SNPRINTF(buffer, sizeof(buffer),
                   "%s",aop->aopu.aop_immd.aop_immd2);
        }
      else if (bit16)
        {
          SNPRINTF(buffer, sizeof(buffer),
                   "#%s", aop->aopu.aop_immd.aop_immd1);
        }
      else if (offset)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "#(%s >> %d)",
                    aop->aopu.aop_immd.aop_immd1,
                    offset * 8);
        }
      else
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "#%s",
                    aop->aopu.aop_immd.aop_immd1);
        }
      return Safe_strdup(buffer);

    case AOP_DIR:
      if (SPEC_SCLS (getSpec (operandType (oper))) == S_SFR && offset)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "(%s >> %d)",
                    aop->aopu.aop_dir, offset * 8);
        }
      else if (offset)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "(%s + %d)",
                    aop->aopu.aop_dir,
                    offset);
        }
      else
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "%s",
                    aop->aopu.aop_dir);
        }

      return Safe_strdup(buffer);

    case AOP_REG:
      if (dname)
        return aop->aopu.aop_reg[offset]->dname;
      else
        return aop->aopu.aop_reg[offset]->name;

    case AOP_CRY:
      emitcode ("mov", "c,%s", aop->aopu.aop_dir);
      emitcode ("clr", "a");
      emitcode ("rlc", "a");
      return (dname ? "acc" : "a");

    case AOP_ACC:
      if (!offset && dname)
        return "acc";
      return aop->aopu.aop_str[offset];

    case AOP_LIT:
      return aopLiteral (aop->aopu.aop_lit, offset);

    case AOP_STR:
      aop->coff = offset;
      if (strcmp (aop->aopu.aop_str[offset], "a") == 0 &&
          dname)
        return "acc";

      return aop->aopu.aop_str[offset];

    }

  werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
          "aopget got unsupported aop->type");
  exit (1);
}

/*-----------------------------------------------------------------*/
/* aopPutUsesAcc - indicates ahead of time whether aopPut() will   */
/*                 clobber the accumulator                         */
/*-----------------------------------------------------------------*/
static bool
aopPutUsesAcc (operand * oper, const char *s, int offset)
{
  asmop * aop = AOP (oper);

  if (offset > (aop->size - 1))
    return FALSE;

  switch (aop->type)
    {
    case AOP_DUMMY:
      return TRUE;
    case AOP_DIR:
      return FALSE;
    case AOP_REG:
      wassert(strcmp(aop->aopu.aop_reg[offset]->name, "a"));
      return FALSE;
    case AOP_DPTR:
      return TRUE;
    case AOP_R0:
    case AOP_R1:
      return ((aop->paged) || (*s == '@'));
    case AOP_STK:
      return (*s == '@');
    case AOP_CRY:
      return (!aop->aopu.aop_dir || strcmp(s, aop->aopu.aop_dir));
    case AOP_STR:
      return FALSE;
    case AOP_IMMD:
      return FALSE;
    case AOP_ACC:
      return FALSE;
    default:
      /* Error case --- will have been caught already */
      wassert(0);
      return FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* aopPut - puts a string for a aop and indicates if acc is in use */
/*-----------------------------------------------------------------*/
static bool
aopPut (operand * result, const char *s, int offset)
{
  bool bvolatile = isOperandVolatile (result, FALSE);
  bool accuse = FALSE;
  asmop * aop = AOP (result);
  const char *d = NULL;

  if (aop->size && offset > (aop->size - 1))
    {
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "aopPut got offset > aop->size");
      exit (1);
    }

  /* will assign value to value */
  /* depending on where it is ofcourse */
  switch (aop->type)
    {
    case AOP_DUMMY:
      MOVA (s);         /* read s in case it was volatile */
      accuse = TRUE;
      break;

    case AOP_DIR:
      if (SPEC_SCLS (getSpec (operandType (result))) == S_SFR && offset)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "(%s >> %d)",
                    aop->aopu.aop_dir, offset * 8);
        }
      else if (offset)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "(%s + %d)",
                    aop->aopu.aop_dir, offset);
        }
      else
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "%s",
                    aop->aopu.aop_dir);
        }

      if (strcmp (buffer, s) || bvolatile)
        {
          emitcode ("mov", "%s,%s", buffer, s);
        }
      if (!strcmp (buffer, "acc"))
        {
          accuse = TRUE;
        }
      break;

    case AOP_REG:
      if (strcmp (aop->aopu.aop_reg[offset]->name, s) != 0 &&
          strcmp (aop->aopu.aop_reg[offset]->dname, s) != 0)
        {
          if (*s == '@' ||
              strcmp (s, "r0") == 0 ||
              strcmp (s, "r1") == 0 ||
              strcmp (s, "r2") == 0 ||
              strcmp (s, "r3") == 0 ||
              strcmp (s, "r4") == 0 ||
              strcmp (s, "r5") == 0 ||
              strcmp (s, "r6") == 0 ||
              strcmp (s, "r7") == 0)
            {
              emitcode ("mov", "%s,%s",
                        aop->aopu.aop_reg[offset]->dname, s);
            }
          else
            {
              emitcode ("mov", "%s,%s",
                        aop->aopu.aop_reg[offset]->name, s);
            }
        }
      break;

    case AOP_DPTR:
      if (aop->code)
        {
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
                  "aopPut writing to code space");
          exit (1);
        }

      while (offset > aop->coff)
        {
          aop->coff++;
          emitcode ("inc", "dptr");
        }

      while (offset < aop->coff)
        {
          aop->coff--;
          emitcode ("lcall", "__decdptr");
        }

      aop->coff = offset;

      /* if not in accumulator */
      MOVA (s);

      emitcode ("movx", "@dptr,a");
      break;

    case AOP_R0:
    case AOP_R1:
      while (offset > aop->coff)
        {
          aop->coff++;
          emitcode ("inc", "%s", aop->aopu.aop_ptr->name);
        }
      while (offset < aop->coff)
        {
          aop->coff--;
          emitcode ("dec", "%s", aop->aopu.aop_ptr->name);
        }
      aop->coff = offset;

      if (aop->paged)
        {
          MOVA (s);
          emitcode ("movx", "@%s,a", aop->aopu.aop_ptr->name);
        }
      else if (*s == '@')
        {
          MOVA (s);
          emitcode ("mov", "@%s,a", aop->aopu.aop_ptr->name);
        }
      else if (strcmp (s, "r0") == 0 ||
               strcmp (s, "r1") == 0 ||
               strcmp (s, "r2") == 0 ||
               strcmp (s, "r3") == 0 ||
               strcmp (s, "r4") == 0 ||
               strcmp (s, "r5") == 0 ||
               strcmp (s, "r6") == 0 ||
               strcmp (s, "r7") == 0)
        {
          char buffer[10];
          SNPRINTF (buffer, sizeof(buffer), "a%s", s);
          emitcode ("mov", "@%s,%s",
                    aop->aopu.aop_ptr->name, buffer);
        }
      else
        {
          emitcode ("mov", "@%s,%s", aop->aopu.aop_ptr->name, s);
        }
      break;

    case AOP_STK:
      if (strcmp (s, "a") == 0)
        {
          emitcode ("push", "acc");
        }
      else if (*s=='@')
        {
          MOVA(s);
          emitcode ("push", "acc");
        }
      else if (strcmp (s, "r0") == 0 ||
               strcmp (s, "r1") == 0 ||
               strcmp (s, "r2") == 0 ||
               strcmp (s, "r3") == 0 ||
               strcmp (s, "r4") == 0 ||
               strcmp (s, "r5") == 0 ||
               strcmp (s, "r6") == 0 ||
               strcmp (s, "r7") == 0)
        {
          char buffer[10];
          SNPRINTF (buffer, sizeof(buffer), "a%s", s);
          emitcode ("push", buffer);
        }
      else
        {
          emitcode ("push", s);
        }

      break;

    case AOP_CRY:
      // destination is carry for return-use-only
      d = (IS_OP_RUONLY (result)) ? "c" : aop->aopu.aop_dir;
      // source is no literal and not in carry
      if ((s != zero) && (s != one) && strcmp (s, "c"))
        {
          MOVA (s);
          /* set C, if a >= 1 */
          emitcode ("add", "a,#0xff");
          s = "c";
        }
      // now source is zero, one or carry

      /* if result no bit variable */
      if (!d)
        {
          if (!strcmp (s, "c"))
            {
              /* inefficient: move carry into A and use jz/jnz */
              emitcode ("clr", "a");
              emitcode ("rlc", "a");
              accuse = TRUE;
            }
          else
            {
              MOVA (s);
              accuse = TRUE;
            }
        }
      else if (s == zero)
          emitcode ("clr", "%s", d);
      else if (s == one)
          emitcode ("setb", "%s", d);
      else if (strcmp (s, d))
          emitcode ("mov", "%s,c", d);
      break;

    case AOP_STR:
      aop->coff = offset;
      if (strcmp (aop->aopu.aop_str[offset], s) || bvolatile)
        emitcode ("mov", "%s,%s", aop->aopu.aop_str[offset], s);
      break;

    case AOP_ACC:
      accuse = TRUE;
      aop->coff = offset;
      if (!offset && (strcmp (s, "acc") == 0) && !bvolatile)
        break;

      if (strcmp (aop->aopu.aop_str[offset], s) && !bvolatile)
        emitcode ("mov", "%s,%s", aop->aopu.aop_str[offset], s);
      break;

    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "aopPut got unsupported aop->type");
      exit (1);
    }

    return accuse;
}


#if 0
/*-----------------------------------------------------------------*/
/* pointToEnd :- points to the last byte of the operand            */
/*-----------------------------------------------------------------*/
static void
pointToEnd (asmop * aop)
{
  int count;
  if (!aop)
    return;

  aop->coff = count = (aop->size - 1);
  switch (aop->type)
    {
    case AOP_R0:
    case AOP_R1:
      while (count--)
        emitcode ("inc", "%s", aop->aopu.aop_ptr->name);
      break;
    case AOP_DPTR:
      while (count--)
        emitcode ("inc", "dptr");
      break;
    }

}
#endif

/*-----------------------------------------------------------------*/
/* reAdjustPreg - points a register back to where it should        */
/*-----------------------------------------------------------------*/
static void
reAdjustPreg (asmop * aop)
{
  if ((aop->coff==0) || (aop->size <= 1))
    return;

  switch (aop->type)
    {
    case AOP_R0:
    case AOP_R1:
      while (aop->coff--)
        emitcode ("dec", "%s", aop->aopu.aop_ptr->name);
      break;
    case AOP_DPTR:
      while (aop->coff--)
        {
          emitcode ("lcall", "__decdptr");
        }
      break;
    }
  aop->coff = 0;
}

/*-----------------------------------------------------------------*/
/* opIsGptr: returns non-zero if the passed operand is       */
/* a generic pointer type.             */
/*-----------------------------------------------------------------*/
static int
opIsGptr (operand * op)
{
  sym_link *type = operandType (op);

  if ((AOP_SIZE (op) == GPTRSIZE) && IS_GENPTR (type))
    {
      return 1;
    }
  return 0;
}

/*-----------------------------------------------------------------*/
/* getDataSize - get the operand data size                         */
/*-----------------------------------------------------------------*/
static int
getDataSize (operand * op)
{
  int size;
  size = AOP_SIZE (op);
  if (size == GPTRSIZE)
    {
      sym_link *type = operandType (op);
      if (IS_GENPTR (type))
        {
          /* generic pointer; arithmetic operations
           * should ignore the high byte (pointer type).
           */
          size--;
        }
    }
  return size;
}

/*-----------------------------------------------------------------*/
/* outAcc - output Acc                                             */
/*-----------------------------------------------------------------*/
static void
outAcc (operand * result)
{
  int size, offset;
  size = getDataSize (result);
  if (size)
    {
      aopPut (result, "a", 0);
      size--;
      offset = 1;
      /* unsigned or positive */
      while (size--)
        {
          aopPut (result, zero, offset++);
        }
    }
}

/*-----------------------------------------------------------------*/
/* outBitC - output a bit C                                        */
/*-----------------------------------------------------------------*/
static void
outBitC (operand * result)
{
  /* if the result is bit */
  if (AOP_TYPE (result) == AOP_CRY)
    {
      if (!IS_OP_RUONLY (result))
        aopPut (result, "c", 0);
    }
  else
    {
      emitcode ("clr", "a");
      emitcode ("rlc", "a");
      outAcc (result);
    }
}

/*-----------------------------------------------------------------*/
/* toBoolean - emit code for orl a,operator(sizeop)                */
/*-----------------------------------------------------------------*/
static void
toBoolean (operand * oper)
{
  int size = AOP_SIZE (oper) - 1;
  int offset = 1;
  bool AccUsed = FALSE;
  bool pushedB;

  while (!AccUsed && size--)
    {
      AccUsed |= aopGetUsesAcc(oper, offset++);
    }

  size = AOP_SIZE (oper) - 1;
  offset = 1;
  MOVA (aopGet (oper, 0, FALSE, FALSE));
  if (size && AccUsed && (AOP (oper)->type != AOP_ACC))
    {
      pushedB = pushB ();
      emitcode("mov", "b,a");
      while (--size)
        {
          MOVA (aopGet (oper, offset++, FALSE, FALSE));
          emitcode ("orl", "b,a");
        }
      MOVA (aopGet (oper, offset++, FALSE, FALSE));
      emitcode ("orl", "a,b");
      popB (pushedB);
    }
  else
    {
      while (size--)
        {
          emitcode ("orl", "a,%s",
                    aopGet (oper, offset++, FALSE, FALSE));
        }
    }
}

/*-----------------------------------------------------------------*/
/* toCarry - make boolean and move into carry                      */
/*-----------------------------------------------------------------*/
static void
toCarry (operand * oper)
{
  /* if the operand is a literal then
     we know what the value is */
  if (AOP_TYPE (oper) == AOP_LIT)
    {
      if ((int) operandLitValue (oper))
        SETC;
      else
        CLRC;
    }
  else if (AOP_TYPE (oper) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", oper->aop->aopu.aop_dir);
    }
  else
    {
      /* or the operand into a */
      toBoolean (oper);
      /* set C, if a >= 1 */
      emitcode ("add", "a,#0xff");
    }
}

/*-----------------------------------------------------------------*/
/* assignBit - assign operand to bit operand                       */
/*-----------------------------------------------------------------*/
static void
assignBit (operand * result, operand * right)
{
  /* if the right side is a literal then
     we know what the value is */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      if ((int) operandLitValue (right))
        aopPut (result, one, 0);
      else
        aopPut (result, zero, 0);
    }
  else
    {
      toCarry (right);
      aopPut (result, "c", 0);
    }
}


/*-------------------------------------------------------------------*/
/* xch_a_aopGet - for exchanging acc with value of the aop           */
/*-------------------------------------------------------------------*/
static char *
xch_a_aopGet (operand * oper, int offset, bool bit16, bool dname)
{
  char * l;

  if (aopGetUsesAcc (oper, offset))
    {
      emitcode("mov", "b,a");
      MOVA (aopGet (oper, offset, bit16, dname));
      emitcode("xch", "a,b");
      aopPut (oper, "a", offset);
      emitcode("xch", "a,b");
      l = "b";
    }
  else
    {
      l = aopGet (oper, offset, bit16, dname);
      emitcode("xch", "a,%s", l);
    }
  return l;
}


/*-----------------------------------------------------------------*/
/* genNot - generate code for ! operation                          */
/*-----------------------------------------------------------------*/
static void
genNot (iCode * ic)
{
  symbol *tlbl;

  D (emitcode (";", "genNot"));

  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE);

  /* if in bit space then a special case */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY)
    {
      /* if left==result then cpl bit */
      if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
        {
          emitcode ("cpl", "%s", IC_LEFT (ic)->aop->aopu.aop_dir);
        }
      else
        {
          toCarry (IC_LEFT (ic));
          emitcode ("cpl", "c");
          outBitC (IC_RESULT (ic));
        }
      goto release;
    }

  toBoolean (IC_LEFT (ic));

  /* set C, if a == 0 */
  tlbl = newiTempLabel (NULL);
  emitcode ("cjne", "a,#0x01,%05d$", tlbl->key + 100);
  emitLabel (tlbl);
  outBitC (IC_RESULT (ic));

release:
  /* release the aops */
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
}


/*-----------------------------------------------------------------*/
/* genCpl - generate code for complement                           */
/*-----------------------------------------------------------------*/
static void
genCpl (iCode * ic)
{
  int offset = 0;
  int size;
  symbol *tlbl;
  sym_link *letype = getSpec (operandType (IC_LEFT (ic)));

  D(emitcode (";", "genCpl"));

  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE);

  /* special case if in bit space */
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
    {
      char *l;

      if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY ||
          (SPEC_USIGN (letype) && IS_CHAR (letype)))
        {
          /* promotion rules are responsible for this strange result:
             bit -> int -> ~int -> bit
             uchar -> int -> ~int -> bit
          */
          emitcode ("setb", "%s", IC_RESULT (ic)->aop->aopu.aop_dir);
          goto release;
        }

      tlbl=newiTempLabel(NULL);
      l = aopGet (IC_LEFT (ic), offset++, FALSE, FALSE);
      if ((AOP_TYPE (IC_LEFT (ic)) == AOP_ACC && offset == 0) ||
          AOP_TYPE (IC_LEFT (ic)) == AOP_REG ||
          IS_AOP_PREG (IC_LEFT (ic)))
        {
          emitcode ("cjne", "%s,#0xFF,%05d$", l, tlbl->key + 100);
        }
      else
        {
          MOVA (l);
          emitcode ("cjne", "a,#0xFF,%05d$", tlbl->key + 100);
        }
      emitLabel (tlbl);
      outBitC (IC_RESULT(ic));
      goto release;
    }

  size = AOP_SIZE (IC_RESULT (ic));
  while (size--)
    {
      char *l = aopGet (IC_LEFT (ic), offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("cpl", "a");
      aopPut (IC_RESULT (ic), "a", offset++);
    }


release:
  /* release the aops */
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
}

/*-----------------------------------------------------------------*/
/* genUminusFloat - unary minus for floating points                */
/*-----------------------------------------------------------------*/
static void
genUminusFloat (operand * op, operand * result)
{
  int size, offset = 0;
  char *l;

  D (emitcode (";", "genUminusFloat"));

  /* for this we just copy and then flip the bit */

  size = AOP_SIZE (op) - 1;

  while (size--)
    {
      aopPut (result,
              aopGet (op, offset, FALSE, FALSE),
              offset);
      offset++;
    }

  l = aopGet (op, offset, FALSE, FALSE);
  MOVA (l);

  emitcode ("cpl", "acc.7");
  aopPut (result, "a", offset);
}

/*-----------------------------------------------------------------*/
/* genUminus - unary minus code generation                         */
/*-----------------------------------------------------------------*/
static void
genUminus (iCode * ic)
{
  int offset, size;
  sym_link *optype;

  D (emitcode (";", "genUminus"));

  /* assign asmops */
  aopOp (IC_LEFT (ic), ic, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE);

  /* if both in bit space then special
     case */
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY &&
      AOP_TYPE (IC_LEFT (ic)) == AOP_CRY)
    {

      emitcode ("mov", "c,%s", IC_LEFT (ic)->aop->aopu.aop_dir);
      emitcode ("cpl", "c");
      emitcode ("mov", "%s,c", IC_RESULT (ic)->aop->aopu.aop_dir);
      goto release;
    }

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
  while (size--)
    {
      char *l = aopGet (IC_LEFT (ic), offset, FALSE, FALSE);
      if (!strcmp (l, "a"))
        {
          if (offset == 0)
            SETC;
          emitcode ("cpl", "a");
          emitcode ("addc", "a,#0");
        }
      else
        {
          if (offset == 0)
            CLRC;
          emitcode ("clr", "a");
          emitcode ("subb", "a,%s", l);
        }
      aopPut (IC_RESULT (ic), "a", offset++);
    }

  /* if any remaining bytes in the result */
  /* we just need to propagate the sign   */
  if ((size = (AOP_SIZE (IC_RESULT (ic)) - AOP_SIZE (IC_LEFT (ic)))))
    {
      emitcode ("rlc", "a");
      emitcode ("subb", "a,acc");
      while (size--)
        aopPut (IC_RESULT (ic), "a", offset++);
    }

release:
  /* release the aops */
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
}

/*-----------------------------------------------------------------*/
/* saveRegisters - will look for a call and save the registers     */
/*-----------------------------------------------------------------*/
static void
saveRegisters (iCode * lic)
{
  int i;
  iCode *ic;
  bitVect *rsave;

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
  if (IS_SYMOP(IC_LEFT(ic)) &&
      (IFFUNC_CALLEESAVES (OP_SYMBOL (IC_LEFT (ic))->type) ||
       IFFUNC_ISNAKED (OP_SYM_TYPE (IC_LEFT (ic)))))
    return;

  /* save the registers in use at this time but skip the
     ones for the result */
  rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
                         mcs51_rUmaskForOp (IC_RESULT(ic)));

  ic->regsSaved = 1;
  if (options.useXstack)
    {
      bitVect *rsavebits = bitVectIntersect (bitVectCopy (mcs51_allBitregs ()), rsave);
      int nBits = bitVectnBitsOn (rsavebits);
      int count = bitVectnBitsOn (rsave);

      if (nBits != 0)
        {
          count = count - nBits + 1;
          /* remove all but the first bits as they are pushed all at once */
          rsave = bitVectCplAnd (rsave, rsavebits);
          rsave = bitVectSetBit (rsave, bitVectFirstBit (rsavebits));
        }
      freeBitVect (rsavebits);

      if (count == 1)
        {
          regs * reg = REG_WITH_INDEX (bitVectFirstBit (rsave));
          if (reg->type == REG_BIT)
            {
              emitcode ("mov", "a,%s", reg->base);
            }
          else
            {
              emitcode ("mov", "a,%s", reg->name);
            }
          emitcode ("mov", "r0,%s", spname);
          emitcode ("inc", "%s", spname);// allocate before use
          emitcode ("movx", "@r0,a");
          if (bitVectBitValue (rsave, R0_IDX))
            emitcode ("mov", "r0,a");
        }
      else if (count != 0)
        {
          if (bitVectBitValue (rsave, R0_IDX))
            {
              emitcode ("push", "%s", REG_WITH_INDEX (R0_IDX)->dname);
            }
          emitcode ("mov", "r0,%s", spname);
          MOVA ("r0");
          emitcode ("add", "a,#%d", count);
          emitcode ("mov", "%s,a", spname);
          for (i = 0; i < mcs51_nRegs; i++)
            {
              if (bitVectBitValue (rsave, i))
                {
                  regs * reg = REG_WITH_INDEX (i);
                  if (i == R0_IDX)
                    {
                      emitcode ("pop", "acc");
                      emitcode ("push", "acc");
                    }
                  else if (reg->type == REG_BIT)
                    {
                      emitcode ("mov", "a,%s", reg->base);
                    }
                  else
                    {
                      emitcode ("mov", "a,%s", reg->name);
                    }
                  emitcode ("movx", "@r0,a");
                  if (--count)
                    {
                      emitcode ("inc", "r0");
                    }
                }
            }
          if (bitVectBitValue (rsave, R0_IDX))
            {
              emitcode ("pop", "%s", REG_WITH_INDEX (R0_IDX)->dname);
            }
        }
    }
  else
    {
      bool bits_pushed = FALSE;
      for (i = 0; i < mcs51_nRegs; i++)
        {
          if (bitVectBitValue (rsave, i))
            {
              bits_pushed = pushReg (i, bits_pushed);
            }
        }
    }
  freeBitVect (rsave);
}

/*-----------------------------------------------------------------*/
/* unsaveRegisters - pop the pushed registers                      */
/*-----------------------------------------------------------------*/
static void
unsaveRegisters (iCode * ic)
{
  int i;
  bitVect *rsave;

  /* restore the registers in use at this time but skip the
     ones for the result */
  rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
                         mcs51_rUmaskForOp (IC_RESULT(ic)));

  if (options.useXstack)
    {
      bitVect *rsavebits = bitVectIntersect (bitVectCopy (mcs51_allBitregs ()), rsave);
      int nBits = bitVectnBitsOn (rsavebits);
      int count = bitVectnBitsOn (rsave);

      if (nBits != 0)
        {
          count = count - nBits + 1;
          /* remove all but the first bits as they are popped all at once */
          rsave = bitVectCplAnd (rsave, rsavebits);
          rsave = bitVectSetBit (rsave, bitVectFirstBit (rsavebits));
        }
      freeBitVect (rsavebits);

      if (count == 1)
        {
          regs * reg = REG_WITH_INDEX (bitVectFirstBit (rsave));
          emitcode ("mov", "r0,%s", spname);
          emitcode ("dec", "r0");
          emitcode ("movx", "a,@r0");
          if (reg->type == REG_BIT)
            {
              emitcode ("mov", "%s,a", reg->base);
            }
          else
            {
              emitcode ("mov", "%s,a", reg->name);
            }
          emitcode ("dec", "%s", spname);
        }
      else if (count != 0)
        {
          emitcode ("mov", "r0,%s", spname);
          for (i = mcs51_nRegs; i >= 0; i--)
            {
              if (bitVectBitValue (rsave, i))
                {
                  regs * reg = REG_WITH_INDEX (i);
                  emitcode ("dec", "r0");
                  emitcode ("movx", "a,@r0");
                  if (i == R0_IDX)
                    {
                      emitcode ("push", "acc");
                    }
                  else if (reg->type == REG_BIT)
                    {
                      emitcode ("mov", "%s,a", reg->base);
                    }
                  else
                    {
                      emitcode ("mov", "%s,a", reg->name);
                    }
                }
            }
          emitcode ("mov", "%s,r0", spname);
          if (bitVectBitValue (rsave, R0_IDX))
            {
              emitcode ("pop", "ar0");
            }
        }
    }
  else
    {
      bool bits_popped = FALSE;
      for (i = mcs51_nRegs; i >= 0; i--)
        {
          if (bitVectBitValue (rsave, i))
            {
              bits_popped = popReg (i, bits_popped);
            }
        }
    }
  freeBitVect (rsave);
}


/*-----------------------------------------------------------------*/
/* pushSide -                                                      */
/*-----------------------------------------------------------------*/
static void
pushSide (operand * oper, int size)
{
  int offset = 0;
  while (size--)
    {
      char *l = aopGet (oper, offset++, FALSE, TRUE);
      if (AOP_TYPE (oper) != AOP_REG &&
          AOP_TYPE (oper) != AOP_DIR &&
          strcmp (l, "a"))
        {
          MOVA (l);
          emitcode ("push", "acc");
        }
      else
        {
          emitcode ("push", "%s", l);
        }
    }
}

/*-----------------------------------------------------------------*/
/* assignResultValue - also indicates if acc is in use afterwards  */
/*-----------------------------------------------------------------*/
static bool
assignResultValue (operand * oper, operand * func)
{
  int offset = 0;
  int size = AOP_SIZE (oper);
  bool accuse = FALSE;
  bool pushedA = FALSE;

  if (func && IS_BIT (OP_SYM_ETYPE (func)))
    {
      outBitC (oper);
      return FALSE;
    }

  if ((size > 3) && aopPutUsesAcc (oper, fReturn[offset], offset))
    {
      emitcode ("push", "acc");
      pushedA = TRUE;
    }
  while (size--)
    {
      if ((offset == 3) && pushedA)
        emitcode ("pop", "acc");
      accuse |= aopPut (oper, fReturn[offset], offset);
      offset++;
    }
  return accuse;
}


/*-----------------------------------------------------------------*/
/* genXpush - pushes onto the external stack                       */
/*-----------------------------------------------------------------*/
static void
genXpush (iCode * ic)
{
  asmop *aop = newAsmop (0);
  regs *r;
  int size, offset = 0;

  D (emitcode (";", "genXpush"));

  aopOp (IC_LEFT (ic), ic, FALSE);
  r = getFreePtr (ic, &aop, FALSE);

  size = AOP_SIZE (IC_LEFT (ic));

  if (size == 1)
    {
      MOVA (aopGet (IC_LEFT (ic), 0, FALSE, FALSE));
      emitcode ("mov", "%s,%s", r->name, spname);
      emitcode ("inc", "%s", spname); // allocate space first
      emitcode ("movx", "@%s,a", r->name);
    }
  else
    {
      // allocate space first
      emitcode ("mov", "%s,%s", r->name, spname);
      MOVA (r->name);
      emitcode ("add", "a,#%d", size);
      emitcode ("mov", "%s,a", spname);

      while (size--)
        {
          MOVA (aopGet (IC_LEFT (ic), offset++, FALSE, FALSE));
          emitcode ("movx", "@%s,a", r->name);
          emitcode ("inc", "%s", r->name);
        }
    }

  freeAsmop (NULL, aop, ic, TRUE);
  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genIpush - generate code for pushing this gets a little complex */
/*-----------------------------------------------------------------*/
static void
genIpush (iCode * ic)
{
  int size, offset = 0;
  char *l;
  char *prev = "";

  D (emitcode (";", "genIpush"));

  /* if this is not a parm push : ie. it is spill push
     and spill push is always done on the local stack */
  if (!ic->parmPush)
    {

      /* and the item is spilt then do nothing */
      if (OP_SYMBOL (IC_LEFT (ic))->isspilt)
        return;

      aopOp (IC_LEFT (ic), ic, FALSE);
      size = AOP_SIZE (IC_LEFT (ic));
      /* push it on the stack */
      while (size--)
        {
          l = aopGet (IC_LEFT (ic), offset++, FALSE, TRUE);
          if (*l == '#')
            {
              MOVA (l);
              l = "acc";
            }
          emitcode ("push", "%s", l);
        }
      return;
    }

  /* this is a parameter push: in this case we call
     the routine to find the call and save those
     registers that need to be saved */
  saveRegisters (ic);

  /* if use external stack then call the external
     stack pushing routine */
  if (options.useXstack)
    {
      genXpush (ic);
      return;
    }

  /* then do the push */
  aopOp (IC_LEFT (ic), ic, FALSE);

  // pushSide(IC_LEFT(ic), AOP_SIZE(IC_LEFT(ic)));
  size = AOP_SIZE (IC_LEFT (ic));

  while (size--)
    {
      l = aopGet (IC_LEFT (ic), offset++, FALSE, TRUE);
      if (AOP_TYPE (IC_LEFT (ic)) != AOP_REG &&
          AOP_TYPE (IC_LEFT (ic)) != AOP_DIR)
        {
          if (strcmp (l, prev) || *l == '@')
            MOVA (l);
          emitcode ("push", "acc");
        }
      else
        {
          emitcode ("push", "%s", l);
        }
      prev = l;
    }

  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genIpop - recover the registers: can happen only for spilling   */
/*-----------------------------------------------------------------*/
static void
genIpop (iCode * ic)
{
  int size, offset;

  D (emitcode (";", "genIpop"));

  /* if the temp was not pushed then */
  if (OP_SYMBOL (IC_LEFT (ic))->isspilt)
    return;

  aopOp (IC_LEFT (ic), ic, FALSE);
  size = AOP_SIZE (IC_LEFT (ic));
  offset = (size - 1);
  while (size--)
    {
      emitcode ("pop", "%s", aopGet (IC_LEFT (ic), offset--,
                                     FALSE, TRUE));
    }

  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* saveRBank - saves an entire register bank on the stack          */
/*-----------------------------------------------------------------*/
static void
saveRBank (int bank, iCode * ic, bool pushPsw)
{
  int i;
  int count = 8 + (pushPsw ? 1 : 0);
  asmop *aop = NULL;
  regs *r = NULL;

  if (options.useXstack)
    {
      if (!ic)
        {
          /* Assume r0 is available for use. */
          r = REG_WITH_INDEX (R0_IDX);
        }
      else
        {
          aop = newAsmop (0);
          r = getFreePtr (ic, &aop, FALSE);
        }
      // allocate space first
      emitcode ("mov", "%s,%s", r->name, spname);
      MOVA (r->name);
      emitcode ("add", "a,#%d", count);
      emitcode ("mov", "%s,a", spname);
    }

  for (i = 0; i < 8; i++)
    {
      if (options.useXstack)
        {
          emitcode ("mov", "a,(%s+%d)",
                    regs8051[i].base, 8 * bank + regs8051[i].offset);
          emitcode ("movx", "@%s,a", r->name);
          if (--count)
            emitcode ("inc", "%s", r->name);
        }
      else
        emitcode ("push", "(%s+%d)",
                  regs8051[i].base, 8 * bank + regs8051[i].offset);
    }

  if (pushPsw)
    {
      if (options.useXstack)
        {
          emitcode ("mov", "a,psw");
          emitcode ("movx", "@%s,a", r->name);
        }
      else
        {
          emitcode ("push", "psw");
        }

      emitcode ("mov", "psw,#0x%02x", (bank << 3) & 0x00ff);
    }

  if (aop)
    {
      freeAsmop (NULL, aop, ic, TRUE);
    }

  if (ic)
    {
      ic->bankSaved = 1;
    }
}

/*-----------------------------------------------------------------*/
/* unsaveRBank - restores the register bank from stack             */
/*-----------------------------------------------------------------*/
static void
unsaveRBank (int bank, iCode * ic, bool popPsw)
{
  int i;
  asmop *aop = NULL;
  regs *r = NULL;

  if (options.useXstack)
    {
      if (!ic)
        {
          /* Assume r0 is available for use. */
          r = REG_WITH_INDEX (R0_IDX);;
        }
      else
        {
          aop = newAsmop (0);
          r = getFreePtr (ic, &aop, FALSE);
        }
      emitcode ("mov", "%s,%s", r->name, spname);
    }

  if (popPsw)
    {
      if (options.useXstack)
        {
          emitcode ("dec", "%s", r->name);
          emitcode ("movx", "a,@%s", r->name);
          emitcode ("mov", "psw,a");
        }
      else
        {
          emitcode ("pop", "psw");
        }
    }

  for (i = 7; i >= 0; i--)
    {
      if (options.useXstack)
        {
          emitcode ("dec", "%s", r->name);
          emitcode ("movx", "a,@%s", r->name);
          emitcode ("mov", "(%s+%d),a",
                    regs8051[i].base, 8 * bank + regs8051[i].offset);
        }
      else
        {
          emitcode ("pop", "(%s+%d)",
                  regs8051[i].base, 8 * bank + regs8051[i].offset);
        }
    }

  if (options.useXstack)
    {
      emitcode ("mov", "%s,%s", spname, r->name);
    }

  if (aop)
    {
      freeAsmop (NULL, aop, ic, TRUE);
    }
}

/*-----------------------------------------------------------------*/
/* genSend - gen code for SEND                                     */
/*-----------------------------------------------------------------*/
static void genSend(set *sendSet)
{
  iCode *sic;
  int bit_count = 0;

  /* first we do all bit parameters */
  for (sic = setFirstItem (sendSet); sic;
       sic = setNextItem (sendSet))
    {
      if (sic->argreg > 12)
        {
          int bit = sic->argreg-13;

          aopOp (IC_LEFT (sic), sic, FALSE);

          /* if left is a literal then
             we know what the value is */
          if (AOP_TYPE (IC_LEFT (sic)) == AOP_LIT)
            {
              if (((int) operandLitValue (IC_LEFT (sic))))
                  emitcode ("setb", "b[%d]", bit);
              else
                  emitcode ("clr", "b[%d]", bit);
            }
          else
            {
              /* we need to or */
              toCarry (IC_LEFT (sic));
              emitcode ("mov", "b[%d],c", bit);
            }
          bit_count++;
          BitBankUsed = 1;

          freeAsmop (IC_LEFT (sic), NULL, sic, TRUE);
        }
    }

  if (bit_count)
    {
      saveRegisters (setFirstItem (sendSet));
      emitcode ("mov", "bits,b");
    }

  /* then we do all other parameters */
  for (sic = setFirstItem (sendSet); sic;
       sic = setNextItem (sendSet))
    {
      if (sic->argreg <= 12)
        {
          int size, offset = 0;
          aopOp (IC_LEFT (sic), sic, FALSE);
          size = AOP_SIZE (IC_LEFT (sic));

          if (sic->argreg == 1)
            {
              while (size--)
                {
                  char *l = aopGet (IC_LEFT (sic), offset, FALSE, FALSE);
                  if (strcmp (l, fReturn[offset]))
                    {
                      emitcode ("mov", "%s,%s", fReturn[offset], l);
                    }
                  offset++;
                }
            }
          else
            {
              while (size--)
                {
                  emitcode ("mov","%s,%s", rb1regs[sic->argreg+offset-5],
                            aopGet (IC_LEFT (sic), offset,FALSE, FALSE));
                  offset++;
                }
            }
          freeAsmop (IC_LEFT (sic), NULL, sic, TRUE);
        }
    }
}

/*-----------------------------------------------------------------*/
/* selectRegBank - emit code to select the register bank           */
/*-----------------------------------------------------------------*/
static void
selectRegBank (short bank, bool keepFlags)
{
  /* if f.e. result is in carry */
  if (keepFlags)
    {
      emitcode ("anl", "psw,#0xE7");
      if (bank)
        emitcode ("orl", "psw,#0x%02x", (bank << 3) & 0xff);
    }
  else
    {
      emitcode ("mov", "psw,#0x%02x", (bank << 3) & 0xff);
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
//  bool restoreBank = FALSE;
  bool swapBanks = FALSE;
  bool accuse = FALSE;
  bool accPushed = FALSE;
  bool resultInF0 = FALSE;
  bool assignResultGenerated = FALSE;

  D (emitcode (";", "genCall"));

  dtype = operandType (IC_LEFT (ic));
  etype = getSpec(dtype);
  /* if send set is not empty then assign */
  if (_G.sendSet)
    {
        if (IFFUNC_ISREENT(dtype)) { /* need to reverse the send set */
            genSend(reverseSet(_G.sendSet));
        } else {
            genSend(_G.sendSet);
        }
      _G.sendSet = NULL;
    }

  /* if we are calling a not _naked function that is not using
     the same register bank then we need to save the
     destination registers on the stack */
  if (currFunc && dtype && !IFFUNC_ISNAKED(dtype) &&
      (FUNC_REGBANK (currFunc->type) != FUNC_REGBANK (dtype)) &&
       !IFFUNC_ISISR (dtype))
    {
      swapBanks = TRUE;
    }

  /* if caller saves & we have not saved then */
  if (!ic->regsSaved)
      saveRegisters (ic);

  if (swapBanks)
    {
        emitcode ("mov", "psw,#0x%02x",
           ((FUNC_REGBANK(dtype)) << 3) & 0xff);
    }

  /* make the call */
  if (IFFUNC_ISBANKEDCALL (dtype) && !SPEC_STAT(getSpec(dtype)))
    {
      if (IFFUNC_CALLEESAVES(dtype))
        {
          werror (E_BANKED_WITH_CALLEESAVES);
        }
      else
        {
          char *l = (OP_SYMBOL (IC_LEFT (ic))->rname[0] ?
                     OP_SYMBOL (IC_LEFT (ic))->rname :
                     OP_SYMBOL (IC_LEFT (ic))->name);

          emitcode ("mov", "r0,#%s", l);
          emitcode ("mov", "r1,#(%s >> 8)", l);
          emitcode ("mov", "r2,#(%s >> 16)", l);
          emitcode ("lcall", "__sdcc_banked_call");
        }
    }
  else
    {
      emitcode ("lcall", "%s", (OP_SYMBOL (IC_LEFT (ic))->rname[0] ?
                                OP_SYMBOL (IC_LEFT (ic))->rname :
                                OP_SYMBOL (IC_LEFT (ic))->name));
    }

  if (swapBanks)
    {
      selectRegBank (FUNC_REGBANK(currFunc->type), IS_BIT (etype));
    }

  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       !IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs ||
        OP_SYMBOL (IC_RESULT (ic))->accuse ||
        OP_SYMBOL (IC_RESULT (ic))->spildir)) ||
      IS_TRUE_SYMOP (IC_RESULT (ic)))
    {

      _G.accInUse++;
      aopOp (IC_RESULT (ic), ic, FALSE);
      _G.accInUse--;

      accuse = assignResultValue (IC_RESULT (ic), IC_LEFT (ic));
      assignResultGenerated = TRUE;

      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }

  /* adjust the stack for parameters if required */
  if (ic->parmBytes)
    {
      int i;
      if (ic->parmBytes > 3)
        {
          if (accuse)
            {
              emitcode ("push", "acc");
              accPushed = TRUE;
            }
          if (IS_BIT (OP_SYM_ETYPE (IC_LEFT (ic))) &&
              IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))) &&
              !assignResultGenerated)
            {
              emitcode ("mov", "F0,c");
              resultInF0 = TRUE;
            }

          emitcode ("mov", "a,%s", spname);
          emitcode ("add", "a,#0x%02x", (-ic->parmBytes) & 0xff);
          emitcode ("mov", "%s,a", spname);

          /* unsaveRegisters from xstack needs acc, but */
          /* unsaveRegisters from stack needs this popped */
          if (accPushed && !options.useXstack)
            {
              emitcode ("pop", "acc");
              accPushed = FALSE;
            }
        }
      else
        for (i = 0; i < ic->parmBytes; i++)
          emitcode ("dec", "%s", spname);
    }

  /* if we had saved some registers then unsave them */
  if (ic->regsSaved && !IFFUNC_CALLEESAVES(dtype))
    {
      if (accuse && !accPushed && options.useXstack)
        {
          /* xstack needs acc, but doesn't touch normal stack */
          emitcode ("push", "acc");
          accPushed = TRUE;
        }
      unsaveRegisters (ic);
    }

//  /* if register bank was saved then pop them */
//  if (restoreBank)
//    unsaveRBank (FUNC_REGBANK (dtype), ic, FALSE);

  if (IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))) && !assignResultGenerated)
    {
      if (resultInF0)
          emitcode ("mov", "c,F0");

      aopOp (IC_RESULT (ic), ic, FALSE);
      assignResultValue (IC_RESULT (ic), IC_LEFT (ic));
      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }

  if (accPushed)
    emitcode ("pop", "acc");
}

/*-----------------------------------------------------------------*/
/* genPcall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void
genPcall (iCode * ic)
{
  sym_link *dtype;
  sym_link *etype;
  symbol *rlbl = newiTempLabel (NULL);
//  bool restoreBank=FALSE;
  bool swapBanks = FALSE;
  bool resultInF0 = FALSE;

  D (emitcode (";", "genPcall"));

  dtype = operandType (IC_LEFT (ic))->next;
  etype = getSpec(dtype);
  /* if caller saves & we have not saved then */
  if (!ic->regsSaved)
    saveRegisters (ic);

  /* if we are calling a not _naked function that is not using
     the same register bank then we need to save the
     destination registers on the stack */
  if (currFunc && dtype && !IFFUNC_ISNAKED (dtype) &&
      (FUNC_REGBANK (currFunc->type) != FUNC_REGBANK (dtype)) &&
      !IFFUNC_ISISR (dtype))
    {
//    saveRBank (FUNC_REGBANK (dtype), ic, TRUE);
//    restoreBank=TRUE;
      swapBanks = TRUE;
      // need caution message to user here
    }

  if (IS_LITERAL (etype))
    {
      /* if send set is not empty then assign */
      if (_G.sendSet)
        {
          genSend(reverseSet(_G.sendSet));
          _G.sendSet = NULL;
        }

      if (swapBanks)
        {
          emitcode ("mov", "psw,#0x%02x",
           ((FUNC_REGBANK (dtype)) << 3) & 0xff);
        }

      if (IFFUNC_ISBANKEDCALL (dtype) && !SPEC_STAT (getSpec(dtype)))
        {
          if (IFFUNC_CALLEESAVES (dtype))
            {
              werror (E_BANKED_WITH_CALLEESAVES);
            }
          else
            {
              char *l = aopLiteralLong (OP_VALUE (IC_LEFT (ic)), 0, 2);

              emitcode ("mov", "r0,#%s", l);
              emitcode ("mov", "r1,#(%s >> 8)", l);
              emitcode ("mov", "r2,#(%s >> 16)", l);
              emitcode ("lcall", "__sdcc_banked_call");
            }
        }
      else
        {
          emitcode ("lcall", "%s", aopLiteralLong (OP_VALUE (IC_LEFT (ic)), 0, 2));
        }
    }
  else
    {
      if (IFFUNC_ISBANKEDCALL (dtype) && !SPEC_STAT (getSpec(dtype)))
        {
          if (IFFUNC_CALLEESAVES (dtype))
            {
              werror (E_BANKED_WITH_CALLEESAVES);
            }
          else
            {
              aopOp (IC_LEFT (ic), ic, FALSE);

              if (!swapBanks)
                {
                  /* what if aopGet needs r0 or r1 ??? */
                  emitcode ("mov", "ar0,%s", aopGet(IC_LEFT (ic), 0, FALSE, FALSE));
                  emitcode ("mov", "ar1,%s", aopGet(IC_LEFT (ic), 1, FALSE, FALSE));
                  emitcode ("mov", "ar2,%s", aopGet(IC_LEFT (ic), 2, FALSE, FALSE));
                }
              else
                {
                  int reg = ((FUNC_REGBANK(dtype)) << 3) & 0xff;
                  emitcode ("mov", "0x%02x,%s", reg++, aopGet(IC_LEFT (ic), 0, FALSE, FALSE));
                  emitcode ("mov", "0x%02x,%s", reg++, aopGet(IC_LEFT (ic), 1, FALSE, FALSE));
                  emitcode ("mov", "0x%02x,%s", reg,   aopGet(IC_LEFT (ic), 2, FALSE, FALSE));
                }

              freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);

              /* if send set is not empty then assign */
              if (_G.sendSet)
                {
                  genSend(reverseSet(_G.sendSet));
                  _G.sendSet = NULL;
                }

              if (swapBanks)
                {
                  emitcode ("mov", "psw,#0x%02x",
                   ((FUNC_REGBANK (dtype)) << 3) & 0xff);
                }

              /* make the call */
              emitcode ("lcall", "__sdcc_banked_call");
            }
        }
      else if (_G.sendSet)
        {
          /* push the return address on to the stack */
          emitcode ("mov", "a,#%05d$", (rlbl->key + 100));
          emitcode ("push", "acc");
          emitcode ("mov", "a,#(%05d$ >> 8)", (rlbl->key + 100));
          emitcode ("push", "acc");

          /* now push the calling address */
          aopOp (IC_LEFT (ic), ic, FALSE);

          pushSide (IC_LEFT (ic), FPTRSIZE);

          freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);

          /* if send set is not empty the assign */
          if (_G.sendSet)
            {
              genSend(reverseSet(_G.sendSet));
              _G.sendSet = NULL;
            }

          if (swapBanks)
            {
              emitcode ("mov", "psw,#0x%02x",
               ((FUNC_REGBANK (dtype)) << 3) & 0xff);
            }

          /* make the call */
          emitcode ("ret", "");
          emitLabel (rlbl);
        }
      else /* the send set is empty */
        {
          char *l;
          /* now get the calling address into dptr */
          aopOp (IC_LEFT (ic), ic, FALSE);

          l = aopGet (IC_LEFT (ic), 0, FALSE, FALSE);
          if (AOP_TYPE (IC_LEFT (ic)) == AOP_DPTR)
            {
              emitcode ("mov", "r0,%s", l);
              l = aopGet (IC_LEFT (ic), 1, FALSE, FALSE);
              emitcode ("mov", "dph,%s", l);
              emitcode ("mov", "dpl,r0");
            }
          else
            {
              emitcode ("mov", "dpl,%s", l);
              l = aopGet (IC_LEFT (ic), 1, FALSE, FALSE);
              emitcode ("mov", "dph,%s", l);
            }

          freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);

          if (swapBanks)
            {
              emitcode ("mov", "psw,#0x%02x",
               ((FUNC_REGBANK (dtype)) << 3) & 0xff);
            }

          /* make the call */
          emitcode ("lcall", "__sdcc_call_dptr");
        }
    }
  if (swapBanks)
    {
      selectRegBank (FUNC_REGBANK (currFunc->type), IS_BIT (etype));
    }

  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       !IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs ||
        OP_SYMBOL (IC_RESULT (ic))->spildir)) ||
      IS_TRUE_SYMOP (IC_RESULT (ic)))
    {

      _G.accInUse++;
      aopOp (IC_RESULT (ic), ic, FALSE);
      _G.accInUse--;

      assignResultValue (IC_RESULT (ic), IC_LEFT (ic));

      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }

  /* adjust the stack for parameters if required */
  if (ic->parmBytes)
    {
      int i;
      if (ic->parmBytes > 3)
        {
          if (IS_BIT (OP_SYM_ETYPE (IC_LEFT (ic))) &&
              IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))))
            {
              emitcode ("mov", "F0,c");
              resultInF0 = TRUE;
            }

          emitcode ("mov", "a,%s", spname);
          emitcode ("add", "a,#0x%02x", (-ic->parmBytes) & 0xff);
          emitcode ("mov", "%s,a", spname);
        }
      else
        for (i = 0; i < ic->parmBytes; i++)
          emitcode ("dec", "%s", spname);
    }

//  /* if register bank was saved then unsave them */
//  if (restoreBank)
//    unsaveRBank (FUNC_REGBANK (dtype), ic, TRUE);

  /* if we had saved some registers then unsave them */
  if (ic->regsSaved && !IFFUNC_CALLEESAVES (dtype))
    unsaveRegisters (ic);

  if (IS_BIT (OP_SYM_ETYPE (IC_RESULT (ic))))
    {
      if (resultInF0)
          emitcode ("mov", "c,F0");

      aopOp (IC_RESULT (ic), ic, FALSE);
      assignResultValue (IC_RESULT (ic), IC_LEFT (ic));
      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }
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
regsCmp(void *p1, void *p2)
{
  return (STRCASECMP((char *)p1, (char *)(p2)) == 0);
}

static bool
inExcludeList (char *s)
{
  const char *p = setFirstItem(options.excludeRegsSet);

  if (p == NULL || STRCASECMP(p, "none") == 0)
    return FALSE;


  return isinSetWith(options.excludeRegsSet, s, regsCmp);
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void
genFunction (iCode * ic)
{
  symbol   *sym = OP_SYMBOL (IC_LEFT (ic));
  sym_link *ftype;
  bool     switchedPSW = FALSE;
  int      calleesaves_saved_register = -1;
  int      stackAdjust = sym->stack;
  int      accIsFree = sym->recvSize < 4;
  iCode    *ric = (ic->next && ic->next->op == RECEIVE) ? ic->next : NULL;
  bool     fReentrant = (IFFUNC_ISREENT (sym->type) || options.stackAuto);

  _G.nRegsSaved = 0;
  /* create the function header */
  emitcode (";", "-----------------------------------------");
  emitcode (";", " function %s", sym->name);
  emitcode (";", "-----------------------------------------");

  emitcode ("", "%s:", sym->rname);
  lineCurr->isLabel = 1;
  ftype = operandType (IC_LEFT (ic));
  _G.currentFunc = sym;

  if (IFFUNC_ISNAKED(ftype))
  {
      emitcode(";", "naked function: no prologue.");
      return;
  }

  /* here we need to generate the equates for the
     register bank if required */
  if (FUNC_REGBANK (ftype) != rbank)
    {
      int i;

      rbank = FUNC_REGBANK (ftype);
      for (i = 0; i < mcs51_nRegs; i++)
        {
          if (regs8051[i].type != REG_BIT)
            {
              if (strcmp (regs8051[i].base, "0") == 0)
                emitcode ("", "%s = 0x%02x",
                          regs8051[i].dname,
                          8 * rbank + regs8051[i].offset);
              else
                emitcode ("", "%s = %s + 0x%02x",
                          regs8051[i].dname,
                          regs8051[i].base,
                          8 * rbank + regs8051[i].offset);
            }
        }
    }

  /* if this is an interrupt service routine then
     save acc, b, dpl, dph  */
  if (IFFUNC_ISISR (sym->type))
    {
      bitVect *rsavebits;

      rsavebits = bitVectIntersect (bitVectCopy (mcs51_allBitregs ()), sym->regsUsed);
      if (IFFUNC_HASFCALL(sym->type) || !bitVectIsZero (rsavebits))
        {
          emitcode ("push", "bits");
          BitBankUsed = 1;
        }
      freeBitVect (rsavebits);

      if (!inExcludeList ("acc"))
        emitcode ("push", "acc");
      if (!inExcludeList ("b"))
        emitcode ("push", "b");
      if (!inExcludeList ("dpl"))
        emitcode ("push", "dpl");
      if (!inExcludeList ("dph"))
        emitcode ("push", "dph");
      /* if this isr has no bank i.e. is going to
         run with bank 0 , then we need to save more
         registers :-) */
      if (!FUNC_REGBANK (sym->type))
        {
          int i;

          /* if this function does not call any other
             function then we can be economical and
             save only those registers that are used */
          if (!IFFUNC_HASFCALL(sym->type))
            {
              /* if any registers used */
              if (sym->regsUsed)
                {
                  /* save the registers used */
                  for (i = 0; i < sym->regsUsed->size; i++)
                    {
                      if (bitVectBitValue (sym->regsUsed, i))
                        pushReg (i, TRUE);
                    }
                }
            }
          else
            {
              /* this function has a function call. We cannot
                 determine register usage so we will have to push the
                 entire bank */
                saveRBank (0, ic, FALSE);
                if (options.parms_in_bank1) {
                    for (i=0; i < 8 ; i++ ) {
                        emitcode ("push","%s",rb1regs[i]);
                    }
                }
            }
        }
      else
        {
            /* This ISR uses a non-zero bank.
             *
             * We assume that the bank is available for our
             * exclusive use.
             *
             * However, if this ISR calls a function which uses some
             * other bank, we must save that bank entirely.
             */
            unsigned long banksToSave = 0;

            if (IFFUNC_HASFCALL(sym->type))
            {

#define MAX_REGISTER_BANKS 4

                iCode *i;
                int ix;

                for (i = ic; i; i = i->next)
                {
                    if (i->op == ENDFUNCTION)
                    {
                        /* we got to the end OK. */
                        break;
                    }

                    if (i->op == CALL)
                    {
                        sym_link *dtype;

                        dtype = operandType (IC_LEFT(i));
                        if (dtype
                         && FUNC_REGBANK(dtype) != FUNC_REGBANK(sym->type))
                        {
                             /* Mark this bank for saving. */
                             if (FUNC_REGBANK(dtype) >= MAX_REGISTER_BANKS)
                             {
                                 werror(E_NO_SUCH_BANK, FUNC_REGBANK(dtype));
                             }
                             else
                             {
                                 banksToSave |= (1 << FUNC_REGBANK(dtype));
                             }

                             /* And note that we don't need to do it in
                              * genCall.
                              */
                             i->bankSaved = 1;
                        }
                    }
                    if (i->op == PCALL)
                    {
                        /* This is a mess; we have no idea what
                         * register bank the called function might
                         * use.
                         *
                         * The only thing I can think of to do is
                         * throw a warning and hope.
                         */
                        werror(W_FUNCPTR_IN_USING_ISR);
                    }
                }

                if (banksToSave && options.useXstack)
                {
                    /* Since we aren't passing it an ic,
                     * saveRBank will assume r0 is available to abuse.
                     *
                     * So switch to our (trashable) bank now, so
                     * the caller's R0 isn't trashed.
                     */
                    emitcode ("push", "psw");
                    emitcode ("mov", "psw,#0x%02x",
                              (FUNC_REGBANK (sym->type) << 3) & 0x00ff);
                    switchedPSW = TRUE;
                }

                for (ix = 0; ix < MAX_REGISTER_BANKS; ix++)
                {
                     if (banksToSave & (1 << ix))
                     {
                         saveRBank(ix, NULL, FALSE);
                     }
                }
            }
            // TODO: this needs a closer look
            SPEC_ISR_SAVED_BANKS(currFunc->etype) = banksToSave;
        }

      /* Set the register bank to the desired value if nothing else */
      /* has done so yet. */
      if (!switchedPSW)
        {
          emitcode ("push", "psw");
          emitcode ("mov", "psw,#0x%02x", (FUNC_REGBANK (sym->type) << 3) & 0x00ff);
        }
    }
  else
    {
      /* This is a non-ISR function. The caller has already switched register */
      /* banks, if necessary, so just handle the callee-saves option. */

      /* if callee-save to be used for this function
         then save the registers being used in this function */
      if (IFFUNC_CALLEESAVES(sym->type))
        {
          int i;

          /* if any registers used */
          if (sym->regsUsed)
            {
              bool bits_pushed = FALSE;
              /* save the registers used */
              for (i = 0; i < sym->regsUsed->size; i++)
                {
                  if (bitVectBitValue (sym->regsUsed, i))
                    {
                      /* remember one saved register for later usage */
                      if (calleesaves_saved_register < 0)
                        calleesaves_saved_register = i;
                      bits_pushed = pushReg (i, bits_pushed);
                      _G.nRegsSaved++;
                    }
                }
            }
        }
    }

  if (fReentrant)
    {
      if (options.useXstack)
        {
          if (sym->xstack || FUNC_HASSTACKPARM(sym->type))
            {
              emitcode ("mov", "r0,%s", spname);
              emitcode ("inc", "%s", spname);
              emitcode ("xch", "a,_bpx");
              emitcode ("movx", "@r0,a");
              emitcode ("inc", "r0");
              emitcode ("mov", "a,r0");
              emitcode ("xch", "a,_bpx");
            }
          if (sym->stack)
            {
              emitcode ("push", "_bp");     /* save the callers stack  */
              emitcode ("mov", "_bp,sp");
            }
        }
      else
        {
          if (sym->stack || FUNC_HASSTACKPARM(sym->type))
            {
              /* set up the stack */
              emitcode ("push", "_bp");     /* save the callers stack  */
              emitcode ("mov", "_bp,sp");
            }
        }
    }

  /* For some cases it is worthwhile to perform a RECEIVE iCode */
  /* before setting up the stack frame completely. */
  if (ric && ric->argreg == 1 && IC_RESULT (ric))
    {
      symbol * rsym = OP_SYMBOL (IC_RESULT (ric));

      if (rsym->isitmp)
        {
          if (rsym && rsym->regType == REG_CND)
            rsym = NULL;
          if (rsym && (rsym->accuse || rsym->ruonly))
            rsym = NULL;
          if (rsym && (rsym->isspilt || rsym->nRegs == 0) && rsym->usl.spillLoc)
            rsym = rsym->usl.spillLoc;
        }

      /* If the RECEIVE operand immediately spills to the first entry on the */
      /* stack, we can push it directly (since sp = _bp + 1 at this point) */
      /* rather than the usual @r0/r1 machinations. */
      if (!options.useXstack && rsym && rsym->onStack && rsym->stack == 1)
        {
          int ofs;

          _G.current_iCode = ric;
          D(emitcode (";", "genReceive"));
          for (ofs=0; ofs < sym->recvSize; ofs++)
            {
              if (!strcmp (fReturn[ofs], "a"))
                emitcode ("push", "acc");
              else
                emitcode ("push", fReturn[ofs]);
            }
          stackAdjust -= sym->recvSize;
          if (stackAdjust<0)
            {
              assert (stackAdjust>=0);
              stackAdjust = 0;
            }
          _G.current_iCode = ic;
          ric->generated = 1;
          accIsFree = 1;
        }
      /* If the RECEIVE operand is 4 registers, we can do the moves now */
      /* to free up the accumulator. */
      else if (rsym && rsym->nRegs && sym->recvSize == 4)
        {
          int ofs;

          _G.current_iCode = ric;
          D(emitcode (";", "genReceive"));
          for (ofs=0; ofs < sym->recvSize; ofs++)
            {
              emitcode ("mov", "%s,%s", rsym->regs[ofs]->name, fReturn[ofs]);
            }
          _G.current_iCode = ic;
          ric->generated = 1;
          accIsFree = 1;
        }
    }

  /* adjust the stack for the function */
  if (stackAdjust)
    {
      int i = stackAdjust;
      if (i > 256)
        werror (W_STACK_OVERFLOW, sym->name);

      if (i > 3 && accIsFree)
        {
          emitcode ("mov", "a,sp");
          emitcode ("add", "a,#0x%02x", ((char) sym->stack & 0xff));
          emitcode ("mov", "sp,a");
        }
      else if (i > 5)
        {
          /* The accumulator is not free, so we will need another register */
          /* to clobber. No need to worry about a possible conflict with */
          /* the above early RECEIVE optimizations since they would have */
          /* freed the accumulator if they were generated. */

          if (IFFUNC_CALLEESAVES(sym->type))
            {
              /* if it's a callee-saves function we need a saved register */
              if (calleesaves_saved_register >= 0)
                {
                  emitcode ("mov", "%s,a", REG_WITH_INDEX (calleesaves_saved_register)->dname);
                  emitcode ("mov", "a,sp");
                  emitcode ("add", "a,#0x%02x", ((char) sym->stack & 0xff));
                  emitcode ("mov", "sp,a");
                  emitcode ("mov", "a,%s", REG_WITH_INDEX (calleesaves_saved_register)->dname);
                }
              else
                /* do it the hard way */
                while (i--)
                  emitcode ("inc", "sp");
            }
          else
            {
              /* not callee-saves, we can clobber r0 */
              emitcode ("mov", "r0,a");
              emitcode ("mov", "a,sp");
              emitcode ("add", "a,#0x%02x", ((char) sym->stack & 0xff));
              emitcode ("mov", "sp,a");
              emitcode ("mov", "a,r0");
            }
        }
      else
        while (i--)
          emitcode ("inc", "sp");
    }

  if (sym->xstack)
    {
      char i = ((char) sym->xstack & 0xff);

      if (i > 3 && accIsFree)
        {
          emitcode ("mov", "a,_spx");
          emitcode ("add", "a,#0x%02x", i & 0xff);
          emitcode ("mov", "_spx,a");
        }
      else if (i > 5)
        {
          emitcode ("push", "acc");
          emitcode ("mov", "a,_spx");
          emitcode ("add", "a,#0x%02x", i & 0xff);
          emitcode ("mov", "_spx,a");
          emitcode ("pop", "acc");
        }
      else
        {
          while (i--)
            emitcode ("inc", "_spx");
        }
    }

  /* if critical function then turn interrupts off */
  if (IFFUNC_ISCRITICAL (ftype))
    {
      symbol *tlbl = newiTempLabel (NULL);
      emitcode ("setb", "c");
      emitcode ("jbc", "ea,%05d$", (tlbl->key + 100)); /* atomic test & clear */
      emitcode ("clr", "c");
      emitLabel (tlbl);
      emitcode ("push", "psw"); /* save old ea via c in psw */
    }
}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void
genEndFunction (iCode * ic)
{
  symbol   *sym = OP_SYMBOL (IC_LEFT (ic));
  lineNode *lnp = lineCurr;
  bitVect  *regsUsed;
  bitVect  *regsUsedPrologue;
  bitVect  *regsUnneeded;
  int      idx;

  _G.currentFunc = NULL;
  if (IFFUNC_ISNAKED(sym->type))
  {
      emitcode(";", "naked function: no epilogue.");
      if (options.debug && currFunc)
        debugFile->writeEndFunction (currFunc, ic, 0);
      return;
  }

  if (IFFUNC_ISCRITICAL (sym->type))
    {
      if (IS_BIT (OP_SYM_ETYPE (IC_LEFT (ic))))
        {
          emitcode ("rlc", "a");   /* save c in a */
          emitcode ("pop", "psw"); /* restore ea via c in psw */
          emitcode ("mov", "ea,c");
          emitcode ("rrc", "a");   /* restore c from a */
        }
      else
        {
          emitcode ("pop", "psw"); /* restore ea via c in psw */
          emitcode ("mov", "ea,c");
        }
    }

  if ((IFFUNC_ISREENT (sym->type) || options.stackAuto))
    {
      if (options.useXstack)
        {
          if (sym->stack)
            {
              emitcode ("mov", "sp,_bp");
              emitcode ("pop", "_bp");
            }
          if (sym->xstack || FUNC_HASSTACKPARM(sym->type))
            {
              emitcode ("xch", "a,_bpx");
              emitcode ("mov", "r0,a");
              emitcode ("dec", "r0");
              emitcode ("movx", "a,@r0");
              emitcode ("xch", "a,_bpx");
              emitcode ("mov", "%s,r0", spname); //read before freeing stack space (interrupts)
            }
        }
      else if (sym->stack || FUNC_HASSTACKPARM(sym->type))
        {
          if (sym->stack)
            emitcode ("mov", "sp,_bp");
          emitcode ("pop", "_bp");
        }
    }

  /* restore the register bank  */
  if ( /* FUNC_REGBANK (sym->type) || */ IFFUNC_ISISR (sym->type))
  {
    if (!FUNC_REGBANK (sym->type) || !IFFUNC_ISISR (sym->type)
     || !options.useXstack)
    {
        /* Special case of ISR using non-zero bank with useXstack
         * is handled below.
         */
        emitcode ("pop", "psw");
    }
  }

  if (IFFUNC_ISISR (sym->type))
    {
      bitVect *rsavebits;

      /* now we need to restore the registers */
      /* if this isr has no bank i.e. is going to
         run with bank 0 , then we need to save more
         registers :-) */
      if (!FUNC_REGBANK (sym->type))
        {
          int i;
          /* if this function does not call any other
             function then we can be economical and
             save only those registers that are used */
          if (!IFFUNC_HASFCALL(sym->type))
            {
              /* if any registers used */
              if (sym->regsUsed)
                {
                  /* save the registers used */
                  for (i = sym->regsUsed->size; i >= 0; i--)
                    {
                      if (bitVectBitValue (sym->regsUsed, i))
                        popReg (i, TRUE);
                    }
                }
            }
          else
            {
              if (options.parms_in_bank1) {
                  for (i = 7 ; i >= 0 ; i-- ) {
                      emitcode ("pop","%s",rb1regs[i]);
                  }
              }
              /* this function has a function call. We cannot
                 determine register usage so we will have to pop the
                 entire bank */
              unsaveRBank (0, ic, FALSE);
            }
        }
        else
        {
            /* This ISR uses a non-zero bank.
             *
             * Restore any register banks saved by genFunction
             * in reverse order.
             */
            unsigned savedBanks = SPEC_ISR_SAVED_BANKS(currFunc->etype);
            int ix;

            for (ix = MAX_REGISTER_BANKS - 1; ix >= 0; ix--)
            {
                if (savedBanks & (1 << ix))
                {
                    unsaveRBank(ix, NULL, FALSE);
                }
            }

            if (options.useXstack)
            {
                /* Restore bank AFTER calling unsaveRBank,
                 * since it can trash r0.
                 */
                emitcode ("pop", "psw");
            }
        }

      if (!inExcludeList ("dph"))
        emitcode ("pop", "dph");
      if (!inExcludeList ("dpl"))
        emitcode ("pop", "dpl");
      if (!inExcludeList ("b"))
        emitcode ("pop", "b");
      if (!inExcludeList ("acc"))
        emitcode ("pop", "acc");

      rsavebits = bitVectIntersect (bitVectCopy (mcs51_allBitregs ()), sym->regsUsed);
      if (IFFUNC_HASFCALL(sym->type) || !bitVectIsZero (rsavebits))
        emitcode ("pop", "bits");
      freeBitVect (rsavebits);

      /* if debug then send end of function */
      if (options.debug && currFunc)
        {
          debugFile->writeEndFunction (currFunc, ic, 1);
        }

      emitcode ("reti", "");
    }
  else
    {
      if (IFFUNC_CALLEESAVES(sym->type))
        {
          int i;

          /* if any registers used */
          if (sym->regsUsed)
            {
              /* save the registers used */
              for (i = sym->regsUsed->size; i >= 0; i--)
                {
                  if (bitVectBitValue (sym->regsUsed, i) ||
                      (mcs51_ptrRegReq && (i == R0_IDX || i == R1_IDX)))
                    emitcode ("pop", "%s", REG_WITH_INDEX (i)->dname);
                }
            }
          else if (mcs51_ptrRegReq)
            {
              emitcode ("pop", "%s", REG_WITH_INDEX (R1_IDX)->dname);
              emitcode ("pop", "%s", REG_WITH_INDEX (R0_IDX)->dname);
            }

        }

      /* if debug then send end of function */
      if (options.debug && currFunc)
        {
          debugFile->writeEndFunction (currFunc, ic, 1);
        }

      if (IFFUNC_ISBANKEDCALL (sym->type) && !SPEC_STAT(getSpec(sym->type)))
        {
          emitcode ("ljmp", "__sdcc_banked_ret");
        }
      else
        {
          emitcode ("ret", "");
        }
    }

  if (!port->peep.getRegsRead || !port->peep.getRegsWritten || options.nopeep)
    return;

  /* If this was an interrupt handler using bank 0 that called another */
  /* function, then all registers must be saved; nothing to optimized. */
  if (IFFUNC_ISISR (sym->type) && IFFUNC_HASFCALL(sym->type)
      && !FUNC_REGBANK(sym->type))
    return;

  /* There are no push/pops to optimize if not callee-saves or ISR */
  if (!(FUNC_CALLEESAVES (sym->type) || FUNC_ISISR (sym->type)))
    return;

  /* If there were stack parameters, we cannot optimize without also    */
  /* fixing all of the stack offsets; this is too dificult to consider. */
  if (FUNC_HASSTACKPARM(sym->type))
    return;

  /* Compute the registers actually used */
  regsUsed = newBitVect (mcs51_nRegs);
  regsUsedPrologue = newBitVect (mcs51_nRegs);
  while (lnp)
    {
      if (lnp->ic && lnp->ic->op == FUNCTION)
        regsUsedPrologue = bitVectUnion (regsUsedPrologue, port->peep.getRegsWritten(lnp));
      else
        regsUsed = bitVectUnion (regsUsed, port->peep.getRegsWritten(lnp));

      if (lnp->ic && lnp->ic->op == FUNCTION && lnp->prev
          && lnp->prev->ic && lnp->prev->ic->op == ENDFUNCTION)
        break;
      if (!lnp->prev)
        break;
      lnp = lnp->prev;
    }

  if (bitVectBitValue (regsUsedPrologue, CND_IDX)
      && !bitVectBitValue (regsUsed, CND_IDX))
    {
      regsUsed = bitVectUnion (regsUsed, regsUsedPrologue);
      if (IFFUNC_ISISR (sym->type) && !FUNC_REGBANK (sym->type)
          && !sym->stack && !FUNC_ISCRITICAL (sym->type))
        bitVectUnSetBit (regsUsed, CND_IDX);
    }
  else
    regsUsed = bitVectUnion (regsUsed, regsUsedPrologue);

  /* If this was an interrupt handler that called another function */
  /* function, then assume A, B, DPH, & DPL may be modified by it. */
  if (IFFUNC_ISISR (sym->type) && IFFUNC_HASFCALL(sym->type))
    {
      regsUsed = bitVectSetBit (regsUsed, DPL_IDX);
      regsUsed = bitVectSetBit (regsUsed, DPH_IDX);
      regsUsed = bitVectSetBit (regsUsed, B_IDX);
      regsUsed = bitVectSetBit (regsUsed, A_IDX);
      regsUsed = bitVectSetBit (regsUsed, CND_IDX);
    }

  /* Remove the unneeded push/pops */
  regsUnneeded = newBitVect (mcs51_nRegs);
  while (lnp)
    {
      if (lnp->ic && (lnp->ic->op == FUNCTION || lnp->ic->op == ENDFUNCTION))
        {
          if (!strncmp(lnp->line, "push", 4))
            {
              idx = bitVectFirstBit (port->peep.getRegsRead(lnp));
              if (idx>=0 && !bitVectBitValue (regsUsed, idx))
                {
                  connectLine (lnp->prev, lnp->next);
                  regsUnneeded = bitVectSetBit (regsUnneeded, idx);
                }
            }
          if (!strncmp(lnp->line, "pop", 3) || !strncmp(lnp->line, "mov", 3))
            {
              idx = bitVectFirstBit (port->peep.getRegsWritten(lnp));
              if (idx>=0 && !bitVectBitValue (regsUsed, idx))
                {
                  connectLine (lnp->prev, lnp->next);
                  regsUnneeded = bitVectSetBit (regsUnneeded, idx);
                }
            }
        }
      lnp = lnp->next;
    }

  for (idx = 0; idx < regsUnneeded->size; idx++)
    if (bitVectBitValue (regsUnneeded, idx))
      emitcode (";", "eliminated unneeded push/pop %s", REG_WITH_INDEX (idx)->dname);

  freeBitVect (regsUnneeded);
  freeBitVect (regsUsed);
  freeBitVect (regsUsedPrologue);
}

/*-----------------------------------------------------------------*/
/* genRet - generate code for return statement                     */
/*-----------------------------------------------------------------*/
static void
genRet (iCode * ic)
{
  int size, offset = 0, pushed = 0;

  D (emitcode (";", "genRet"));

  /* if we have no return value then
     just generate the "ret" */
  if (!IC_LEFT (ic))
    goto jumpret;

  /* we have something to return then
     move the return value into place */
  aopOp (IC_LEFT (ic), ic, FALSE);
  size = AOP_SIZE (IC_LEFT (ic));

  if (IS_BIT(_G.currentFunc->etype))
    {
      if (!IS_OP_RUONLY (IC_LEFT (ic)))
        toCarry (IC_LEFT (ic));
    }
  else
    {
      while (size--)
        {
          char *l;
          if (AOP_TYPE (IC_LEFT (ic)) == AOP_DPTR)
            {
              /* #NOCHANGE */
              l = aopGet (IC_LEFT (ic), offset++, FALSE, TRUE);
              emitcode ("push", "%s", l);
              pushed++;
            }
          else
            {
              l = aopGet (IC_LEFT (ic), offset, FALSE, FALSE);
              if (strcmp (fReturn[offset], l))
                emitcode ("mov", "%s,%s", fReturn[offset++], l);
            }
        }

      while (pushed)
        {
          pushed--;
          if (strcmp (fReturn[pushed], "a"))
            emitcode ("pop", fReturn[pushed]);
          else
            emitcode ("pop", "acc");
        }
    }
  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);

jumpret:
  /* generate a jump to the return label
     if the next is not the return statement */
  if (!(ic->next && ic->next->op == LABEL &&
        IC_LABEL (ic->next) == returnLabel))

    emitcode ("ljmp", "%05d$", (returnLabel->key + 100));

}

/*-----------------------------------------------------------------*/
/* genLabel - generates a label                                    */
/*-----------------------------------------------------------------*/
static void
genLabel (iCode * ic)
{
  /* special case never generate */
  if (IC_LABEL (ic) == entryLabel)
    return;

  emitLabel (IC_LABEL (ic));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a ljmp                                      */
/*-----------------------------------------------------------------*/
static void
genGoto (iCode * ic)
{
  emitcode ("ljmp", "%05d$", (IC_LABEL (ic)->key + 100));
}

/*-----------------------------------------------------------------*/
/* findLabelBackwards: walks back through the iCode chain looking  */
/* for the given label. Returns number of iCode instructions     */
/* between that label and given ic.          */
/* Returns zero if label not found.          */
/*-----------------------------------------------------------------*/
static int
findLabelBackwards (iCode * ic, int key)
{
  int count = 0;

  while (ic->prev)
    {
      ic = ic->prev;
      count++;

      /* If we have any pushes or pops, we cannot predict the distance.
         I don't like this at all, this should be dealt with in the
         back-end */
      if (ic->op == IPUSH || ic->op == IPOP) {
        return 0;
      }

      if (ic->op == LABEL && IC_LABEL (ic)->key == key)
        {
          return count;
        }
    }

  return 0;
}

/*-----------------------------------------------------------------*/
/* genPlusIncr :- does addition with increment if possible         */
/*-----------------------------------------------------------------*/
static bool
genPlusIncr (iCode * ic)
{
  unsigned int icount;
  unsigned int size = getDataSize (IC_RESULT (ic));

  /* will try to generate an increment */
  /* if the right side is not a literal
     we cannot */
  if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT)
    return FALSE;

  icount = (unsigned int) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);

  D(emitcode (";","genPlusIncr"));

  /* if increment >=16 bits in register or direct space */
  if (( AOP_TYPE(IC_LEFT(ic)) == AOP_REG || 
        AOP_TYPE(IC_LEFT(ic)) == AOP_DIR ||
        (IS_AOP_PREG (IC_LEFT(ic)) && !AOP_NEEDSACC (IC_LEFT(ic))) ) &&
      sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) &&
      !isOperandVolatile (IC_RESULT (ic), FALSE) &&
      (size > 1) &&
      (icount == 1))
    {
      symbol *tlbl;
      int emitTlbl;
      int labelRange;

      /* If the next instruction is a goto and the goto target
       * is < 10 instructions previous to this, we can generate
       * jumps straight to that target.
       */
      if (ic->next && ic->next->op == GOTO
          && (labelRange = findLabelBackwards (ic, IC_LABEL (ic->next)->key)) != 0
          && labelRange <= 10)
        {
          D (emitcode (";", "tail increment optimized (range %d)", labelRange));
          tlbl = IC_LABEL (ic->next);
          emitTlbl = 0;
        }
      else
        {
          tlbl = newiTempLabel (NULL);
          emitTlbl = 1;
        }
      emitcode ("inc", "%s", aopGet (IC_RESULT (ic), LSB, FALSE, FALSE));
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
          IS_AOP_PREG (IC_RESULT (ic)))
        emitcode ("cjne", "%s,#0x00,%05d$",
                  aopGet (IC_RESULT (ic), LSB, FALSE, FALSE),
                  tlbl->key + 100);
      else
        {
          emitcode ("clr", "a");
          emitcode ("cjne", "a,%s,%05d$",
                    aopGet (IC_RESULT (ic), LSB, FALSE, FALSE),
                    tlbl->key + 100);
        }

      emitcode ("inc", "%s", aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE));
      if (size > 2)
        {
          if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
              IS_AOP_PREG (IC_RESULT (ic)))
            emitcode ("cjne", "%s,#0x00,%05d$",
                      aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE),
                      tlbl->key + 100);
          else
            emitcode ("cjne", "a,%s,%05d$",
                      aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE),
                      tlbl->key + 100);

          emitcode ("inc", "%s", aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE));
        }
      if (size > 3)
        {
          if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
              IS_AOP_PREG (IC_RESULT (ic)))
            emitcode ("cjne", "%s,#0x00,%05d$",
                      aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE),
                      tlbl->key + 100);
          else
            {
              emitcode ("cjne", "a,%s,%05d$",
                        aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE),
                        tlbl->key + 100);
            }
          emitcode ("inc", "%s", aopGet (IC_RESULT (ic), MSB32, FALSE, FALSE));
        }

      if (emitTlbl)
        {
          emitLabel (tlbl);
        }
      return TRUE;
    }

  /* if result is dptr */
  if ((AOP_TYPE (IC_RESULT (ic)) == AOP_STR) &&
      (AOP_SIZE (IC_RESULT (ic)) == 2) &&
      !strncmp(AOP (IC_RESULT (ic))->aopu.aop_str[0], "dpl", 4) &&
      !strncmp(AOP (IC_RESULT (ic))->aopu.aop_str[1], "dph", 4))
    {
      if (aopGetUsesAcc (IC_LEFT (ic), 0))
        return FALSE;

      if (icount > 9)
        return FALSE;

      if ((AOP_TYPE (IC_LEFT (ic)) != AOP_DIR) && (icount > 5))
        return FALSE;

      aopPut (IC_RESULT (ic), aopGet (IC_LEFT (ic), 0, FALSE, FALSE), 0);
      aopPut (IC_RESULT (ic), aopGet (IC_LEFT (ic), 1, FALSE, FALSE), 1);
      while (icount--)
        emitcode ("inc", "dptr");

      return TRUE;
    }

  /* if the literal value of the right hand side
     is greater than 4 then it is not worth it */
  if (icount > 4)
    return FALSE;

  /* if the sizes are greater than 1 then we cannot */
  if (AOP_SIZE (IC_RESULT (ic)) > 1 ||
      AOP_SIZE (IC_LEFT (ic)) > 1)
    return FALSE;

  /* we can if the aops of the left & result match or
     if they are in registers and the registers are the
     same */
  if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {
      if (icount > 3)
        {
          MOVA (aopGet (IC_LEFT (ic), 0, FALSE, FALSE));
          emitcode ("add", "a,#0x%02x", ((char) icount) & 0xff);
          aopPut (IC_RESULT (ic), "a", 0);
        }
      else
        {
          while (icount--)
            {
              emitcode ("inc", "%s", aopGet (IC_LEFT (ic), 0, FALSE, FALSE));
            }
        }

      return TRUE;
    }

  if (icount == 1)
    {
      MOVA (aopGet (IC_LEFT (ic), 0, FALSE, FALSE));
      emitcode ("inc", "a");
      aopPut (IC_RESULT (ic), "a", 0);
      return TRUE;
    }

  return FALSE;
}

/*-----------------------------------------------------------------*/
/* outBitAcc - output a bit in acc                                 */
/*-----------------------------------------------------------------*/
static void
outBitAcc (operand * result)
{
  symbol *tlbl = newiTempLabel (NULL);
  /* if the result is a bit */
  if (AOP_TYPE (result) == AOP_CRY)
    {
      aopPut (result, "a", 0);
    }
  else
    {
      emitcode ("jz", "%05d$", tlbl->key + 100);
      emitcode ("mov", "a,%s", one);
      emitLabel (tlbl);
      outAcc (result);
    }
}

/*-----------------------------------------------------------------*/
/* genPlusBits - generates code for addition of two bits           */
/*-----------------------------------------------------------------*/
static void
genPlusBits (iCode * ic)
{
  D (emitcode (";", "genPlusBits"));

  emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
    {
      symbol *lbl = newiTempLabel (NULL);
      emitcode ("jnb", "%s,%05d$", AOP (IC_RIGHT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("cpl", "c");
      emitLabel (lbl);
      outBitC (IC_RESULT (ic));
    }
  else
    {
      emitcode ("clr", "a");
      emitcode ("rlc", "a");
      emitcode ("mov", "c,%s", AOP (IC_RIGHT (ic))->aopu.aop_dir);
      emitcode ("addc", "a,%s", zero);
      outAcc (IC_RESULT (ic));
    }
}

#if 0
/* This is the original version of this code.

 * This is being kept around for reference,
 * because I am not entirely sure I got it right...
 */
static void
adjustArithmeticResult (iCode * ic)
{
  if (AOP_SIZE (IC_RESULT (ic)) == 3 &&
      AOP_SIZE (IC_LEFT (ic)) == 3 &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))))
    aopPut (IC_RESULT (ic),
            aopGet (IC_LEFT (ic)), 2, FALSE, FALSE),
            2);

  if (AOP_SIZE (IC_RESULT (ic)) == 3 &&
      AOP_SIZE (IC_RIGHT (ic)) == 3 &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    aopPut (IC_RESULT (ic),
            aopGet (IC_RIGHT (ic)), 2, FALSE, FALSE),
            2);

  if (AOP_SIZE (IC_RESULT (ic)) == 3 &&
      AOP_SIZE (IC_LEFT (ic)) < 3 &&
      AOP_SIZE (IC_RIGHT (ic)) < 3 &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    {
      char buffer[5];
      sprintf (buffer, "#%d", pointerTypeToGPByte (pointerCode (getSpec (operandType (IC_LEFT (ic)))), NULL, NULL));
      aopPut (IC_RESULT (ic), buffer, 2);
    }
}
#else
/* This is the pure and virtuous version of this code.
 * I'm pretty certain it's right, but not enough to toss the old
 * code just yet...
 */
static void
adjustArithmeticResult (iCode * ic)
{
  if (opIsGptr (IC_RESULT (ic)) &&
      opIsGptr (IC_LEFT (ic)) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))))
    {
      aopPut (IC_RESULT (ic),
              aopGet (IC_LEFT (ic), GPTRSIZE - 1, FALSE, FALSE),
              GPTRSIZE - 1);
    }

  if (opIsGptr (IC_RESULT (ic)) &&
      opIsGptr (IC_RIGHT (ic)) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    {
      aopPut (IC_RESULT (ic),
              aopGet (IC_RIGHT (ic), GPTRSIZE - 1, FALSE, FALSE),
              GPTRSIZE - 1);
    }

  if (opIsGptr (IC_RESULT (ic)) &&
      AOP_SIZE (IC_LEFT (ic)) < GPTRSIZE &&
      AOP_SIZE (IC_RIGHT (ic)) < GPTRSIZE &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    {
      char buffer[5];
      SNPRINTF (buffer, sizeof(buffer),
                "#%d", pointerTypeToGPByte (pointerCode (getSpec (operandType (IC_LEFT (ic)))), NULL, NULL));
      aopPut (IC_RESULT (ic), buffer, GPTRSIZE - 1);
    }
}
#endif

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void
genPlus (iCode * ic)
{
  int size, offset = 0;
  int skip_bytes = 0;
  char *add = "add";
  bool swappedLR = FALSE;
  operand *leftOp, *rightOp;
  operand * op;

  D (emitcode (";", "genPlus"));

  /* special cases :- */

  aopOp (IC_LEFT (ic), ic, FALSE);
  aopOp (IC_RIGHT (ic), ic, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE);

  /* if literal, literal on the right or
     if left requires ACC or right is already
     in ACC */
  if ((AOP_TYPE (IC_LEFT (ic)) == AOP_LIT) ||
      (AOP_NEEDSACC (IC_LEFT (ic))) ||
      AOP_TYPE (IC_RIGHT (ic)) == AOP_ACC)
    {
      operand *t = IC_RIGHT (ic);
      IC_RIGHT (ic) = IC_LEFT (ic);
      IC_LEFT (ic) = t;
      swappedLR = TRUE;
    }

  /* if both left & right are in bit
     space */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY &&
      AOP_TYPE (IC_RIGHT (ic)) == AOP_CRY)
    {
      genPlusBits (ic);
      goto release;
    }

  /* if left in bit space & right literal */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY &&
      AOP_TYPE (IC_RIGHT (ic)) == AOP_LIT)
    {
      emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
      /* if result in bit space */
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
        {
          if ((unsigned long) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit) != 0L)
            emitcode ("cpl", "c");
          outBitC (IC_RESULT (ic));
        }
      else
        {
          size = getDataSize (IC_RESULT (ic));
          while (size--)
            {
              MOVA (aopGet (IC_RIGHT (ic), offset, FALSE, FALSE));
              emitcode ("addc", "a,%s", zero);
              aopPut (IC_RESULT (ic), "a", offset++);
            }
        }
      goto release;
    }

  /* if I can do an increment instead
     of add then GOOD for ME */
  if (genPlusIncr (ic) == TRUE)
    goto release;

  size = getDataSize (IC_RESULT (ic));
  leftOp = IC_LEFT(ic);
  rightOp = IC_RIGHT(ic);
  op = IC_LEFT(ic);

  /* if this is an add for an array access
     at a 256 byte boundary */
  if ( 2 == size
       && AOP_TYPE (op) == AOP_IMMD
       && IS_SYMOP (op)
       && IS_SPEC (OP_SYM_ETYPE (op))
       && SPEC_ABSA (OP_SYM_ETYPE (op))
       && (SPEC_ADDR (OP_SYM_ETYPE (op)) & 0xff) == 0
     )
    {
      D(emitcode (";", "genPlus aligned array"));
      aopPut (IC_RESULT (ic),
              aopGet (rightOp, 0, FALSE, FALSE),
              0);

      if( 1 == getDataSize (IC_RIGHT (ic)) )
        {
          aopPut (IC_RESULT (ic),
                  aopGet (leftOp, 1, FALSE, FALSE),
                  1);
        }
      else
        {
          MOVA (aopGet (IC_LEFT (ic), 1, FALSE, FALSE));
          emitcode ("add", "a,%s", aopGet (rightOp, 1, FALSE, FALSE));
          aopPut (IC_RESULT (ic), "a", 1);
        }
      goto release;
    }

  /* if the lower bytes of a literal are zero skip the addition */
  if (AOP_TYPE (IC_RIGHT (ic)) == AOP_LIT )
    {
       while ((0 == ((unsigned int) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit) & (0xff << skip_bytes*8))) &&
              (skip_bytes+1 < size))
         {
           skip_bytes++;
         }
       if (skip_bytes)
         D(emitcode (";", "genPlus shortcut"));
    }

  while (size--)
    {
      if( offset >= skip_bytes )
        {
          if (aopGetUsesAcc (leftOp, offset) && aopGetUsesAcc (rightOp, offset))
            {
              bool pushedB;
              MOVA (aopGet (leftOp,  offset, FALSE, TRUE));
              pushedB = pushB ();
              emitcode("xch", "a,b");
              MOVA (aopGet (rightOp, offset, FALSE, TRUE));
              emitcode (add, "a,b");
              popB (pushedB);
            }
          else if (aopGetUsesAcc (leftOp, offset))
            {
              MOVA (aopGet (leftOp, offset, FALSE, TRUE));
              emitcode (add, "a,%s", aopGet (rightOp, offset, FALSE, TRUE));
            }
          else
            {
              MOVA (aopGet (rightOp, offset, FALSE, TRUE));
              emitcode (add, "a,%s", aopGet (leftOp, offset, FALSE, TRUE));
            }
          aopPut (IC_RESULT (ic), "a", offset);
          add = "addc";  /* further adds must propagate carry */
        }
      else
        {
          if( !sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) ||
              isOperandVolatile (IC_RESULT (ic), FALSE))
            {
              /* just move */
              aopPut (IC_RESULT (ic),
                      aopGet (leftOp, offset, FALSE, FALSE),
                      offset);
            }
        }
      offset++;
    }

  adjustArithmeticResult (ic);

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  if (!swappedLR)
    {
      freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
    }
  else
    {
      freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
    }
}

/*-----------------------------------------------------------------*/
/* genMinusDec :- does subtraction with decrement if possible      */
/*-----------------------------------------------------------------*/
static bool
genMinusDec (iCode * ic)
{
  unsigned int icount;
  unsigned int size = getDataSize (IC_RESULT (ic));

  /* will try to generate an increment */
  /* if the right side is not a literal
     we cannot */
  if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT)
    return FALSE;

  /* if the literal value of the right hand side
     is greater than 4 then it is not worth it */
  if ((icount = (unsigned int) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit)) > 4)
    return FALSE;

  D (emitcode (";", "genMinusDec"));

  /* if decrement >=16 bits in register or direct space */
  if (( AOP_TYPE(IC_LEFT(ic)) == AOP_REG || 
        AOP_TYPE(IC_LEFT(ic)) == AOP_DIR ||
        (IS_AOP_PREG (IC_LEFT(ic)) && !AOP_NEEDSACC (IC_LEFT(ic))) ) &&
      sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) &&
      (size > 1) &&
      (icount == 1))
    {
      symbol *tlbl;
      int emitTlbl;
      int labelRange;

      /* If the next instruction is a goto and the goto target
       * is <= 10 instructions previous to this, we can generate
       * jumps straight to that target.
       */
      if (ic->next && ic->next->op == GOTO
          && (labelRange = findLabelBackwards (ic, IC_LABEL (ic->next)->key)) != 0
          && labelRange <= 10)
        {
          D (emitcode (";", "tail decrement optimized (range %d)", labelRange));
          tlbl = IC_LABEL (ic->next);
          emitTlbl = 0;
        }
      else
        {
          tlbl = newiTempLabel (NULL);
          emitTlbl = 1;
        }

      emitcode ("dec", "%s", aopGet (IC_RESULT (ic), LSB, FALSE, FALSE));
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
          IS_AOP_PREG (IC_RESULT (ic)))
        emitcode ("cjne", "%s,#0xff,%05d$"
                  ,aopGet (IC_RESULT (ic), LSB, FALSE, FALSE)
                  ,tlbl->key + 100);
      else
        {
          emitcode ("mov", "a,#0xff");
          emitcode ("cjne", "a,%s,%05d$"
                    ,aopGet (IC_RESULT (ic), LSB, FALSE, FALSE)
                    ,tlbl->key + 100);
        }
      emitcode ("dec", "%s", aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE));
      if (size > 2)
        {
          if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
              IS_AOP_PREG (IC_RESULT (ic)))
            emitcode ("cjne", "%s,#0xff,%05d$"
                      ,aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE)
                      ,tlbl->key + 100);
          else
            {
              emitcode ("cjne", "a,%s,%05d$"
                        ,aopGet (IC_RESULT (ic), MSB16, FALSE, FALSE)
                        ,tlbl->key + 100);
            }
          emitcode ("dec", "%s", aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE));
        }
      if (size > 3)
        {
          if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
              IS_AOP_PREG (IC_RESULT (ic)))
            emitcode ("cjne", "%s,#0xff,%05d$"
                      ,aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE)
                      ,tlbl->key + 100);
          else
            {
              emitcode ("cjne", "a,%s,%05d$"
                        ,aopGet (IC_RESULT (ic), MSB24, FALSE, FALSE)
                        ,tlbl->key + 100);
            }
          emitcode ("dec", "%s", aopGet (IC_RESULT (ic), MSB32, FALSE, FALSE));
        }
      if (emitTlbl)
        {
          emitLabel (tlbl);
        }
      return TRUE;
    }

  /* if the sizes are greater than 1 then we cannot */
  if (AOP_SIZE (IC_RESULT (ic)) > 1 ||
      AOP_SIZE (IC_LEFT (ic)) > 1)
    return FALSE;

  /* we can if the aops of the left & result match or
     if they are in registers and the registers are the
     same */
  if (sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {
      char *l;

      if (aopGetUsesAcc (IC_LEFT (ic), 0))
        {
          MOVA (aopGet (IC_RESULT (ic), 0, FALSE, FALSE));
          l = "a";
        }
      else
        {
          l = aopGet (IC_RESULT (ic), 0, FALSE, FALSE);
        }

      while (icount--)
        {
          emitcode ("dec", "%s", l);
        }

      if (AOP_NEEDSACC (IC_RESULT (ic)))
        aopPut (IC_RESULT (ic), "a", 0);

      return TRUE;
    }

  if (icount == 1)
    {
      MOVA (aopGet (IC_LEFT (ic), 0, FALSE, FALSE));
      emitcode ("dec", "a");
      aopPut (IC_RESULT (ic), "a", 0);
      return TRUE;
    }

  return FALSE;
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
          emitcode ("rlc", "a");
          emitcode ("subb", "a,acc");
          while (size--)
            {
              aopPut (result, "a", offset++);
            }
        }
      else
        {
          while (size--)
            {
              aopPut (result, zero, offset++);
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* genMinusBits - generates code for subtraction  of two bits      */
/*-----------------------------------------------------------------*/
static void
genMinusBits (iCode * ic)
{
  symbol *lbl = newiTempLabel (NULL);

  D (emitcode (";", "genMinusBits"));

  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
      emitcode ("jnb", "%s,%05d$", AOP (IC_RIGHT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("cpl", "c");
      emitLabel (lbl);
      outBitC (IC_RESULT (ic));
    }
  else
    {
      emitcode ("mov", "c,%s", AOP (IC_RIGHT (ic))->aopu.aop_dir);
      emitcode ("subb", "a,acc");
      emitcode ("jnb", "%s,%05d$", AOP (IC_LEFT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("inc", "a");
      emitLabel (lbl);
      aopPut (IC_RESULT (ic), "a", 0);
      addSign (IC_RESULT (ic), MSB16, SPEC_USIGN (getSpec (operandType (IC_RESULT (ic)))));
    }
}

/*-----------------------------------------------------------------*/
/* genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
static void
genMinus (iCode * ic)
{
  int size, offset = 0;

  D (emitcode (";", "genMinus"));

  aopOp (IC_LEFT (ic), ic, FALSE);
  aopOp (IC_RIGHT (ic), ic, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE);

  /* special cases :- */
  /* if both left & right are in bit space */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY &&
      AOP_TYPE (IC_RIGHT (ic)) == AOP_CRY)
    {
      genMinusBits (ic);
      goto release;
    }

  /* if I can do an decrement instead
     of subtract then GOOD for ME */
  if (genMinusDec (ic) == TRUE)
    goto release;

  size = getDataSize (IC_RESULT (ic));

  /* if literal, add a,#-lit, else normal subb */
  if (AOP_TYPE (IC_RIGHT (ic)) == AOP_LIT)
    {
      unsigned long lit = 0L;
      bool useCarry = FALSE;

      lit = (unsigned long) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);
      lit = -(long) lit;

      while (size--)
        {
          if (useCarry || ((lit >> (offset * 8)) & 0x0FFL))
            {
              MOVA (aopGet (IC_LEFT (ic), offset, FALSE, FALSE));
              if (!offset && !size && lit== (unsigned long) -1)
                {
                  emitcode ("dec", "a");
                }
              else if (!useCarry)
                {
                  /* first add without previous c */
                  emitcode ("add", "a,#0x%02x",
                            (unsigned int) ((lit >> (offset * 8)) & 0x0FFL));
                  useCarry = TRUE;
                }
              else
                {
                  emitcode ("addc", "a,#0x%02x",
                            (unsigned int) ((lit >> (offset * 8)) & 0x0FFL));
                }
              aopPut (IC_RESULT (ic), "a", offset++);
            }
          else
            {
              /* no need to add zeroes */
              if (!sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))))
                {
                  aopPut (IC_RESULT (ic), aopGet (IC_LEFT (ic), offset, FALSE, FALSE),
                          offset);
                }
              offset++;
            }
        }
    }
  else
    {
      operand *leftOp, *rightOp;

      leftOp = IC_LEFT(ic);
      rightOp = IC_RIGHT(ic);

      while (size--)
        {
          if (aopGetUsesAcc(rightOp, offset)) {
            if (aopGetUsesAcc(leftOp, offset)) {
              bool pushedB;

              MOVA (aopGet (rightOp, offset, FALSE, FALSE));
              pushedB = pushB ();
              emitcode ("mov", "b,a");
              if (offset == 0)
                CLRC;
              MOVA (aopGet (leftOp, offset, FALSE, FALSE));
              emitcode ("subb", "a,b");
              popB (pushedB);
            } else {
              /* reverse subtraction with 2's complement */
              if (offset == 0)
                emitcode( "setb", "c");
              else
                emitcode( "cpl", "c");
              wassertl(!aopGetUsesAcc(leftOp, offset), "accumulator clash");
              MOVA (aopGet(rightOp, offset, FALSE, TRUE));
              emitcode("subb", "a,%s", aopGet(leftOp, offset, FALSE, TRUE));
              emitcode("cpl", "a");
              if (size) /* skip if last byte */
                emitcode( "cpl", "c");
            }
          } else {
            MOVA (aopGet (leftOp, offset, FALSE, FALSE));
            if (offset == 0)
              CLRC;
            emitcode ("subb", "a,%s",
                      aopGet(rightOp, offset, FALSE, TRUE));
          }

          aopPut (IC_RESULT (ic), "a", offset++);
        }
    }

  adjustArithmeticResult (ic);

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}


/*-----------------------------------------------------------------*/
/* genMultbits :- multiplication of bits                           */
/*-----------------------------------------------------------------*/
static void
genMultbits (operand * left,
             operand * right,
             operand * result)
{
  D (emitcode (";", "genMultbits"));

  emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
  emitcode ("anl", "c,%s", AOP (right)->aopu.aop_dir);
  outBitC (result);
}

/*-----------------------------------------------------------------*/
/* genMultOneByte : 8*8=8/16 bit multiplication                    */
/*-----------------------------------------------------------------*/
static void
genMultOneByte (operand * left,
                operand * right,
                operand * result)
{
  symbol *lbl;
  int size = AOP_SIZE (result);
  bool runtimeSign, compiletimeSign;
  bool lUnsigned, rUnsigned, pushedB;

  D (emitcode (";", "genMultOneByte"));

  if (size < 1 || size > 2)
    {
      /* this should never happen */
      fprintf (stderr, "size!=1||2 (%d) in %s at line:%d \n",
               AOP_SIZE(result), __FILE__, lineno);
      exit (1);
    }

  /* (if two literals: the value is computed before) */
  /* if one literal, literal on the right */
  if (AOP_TYPE (left) == AOP_LIT)
    {
      operand *t = right;
      right = left;
      left = t;
      /* emitcode (";", "swapped left and right"); */
    }
  /* if no literal, unsigned on the right: shorter code */
  if (   AOP_TYPE (right) != AOP_LIT
      && SPEC_USIGN (getSpec (operandType (left))))
    {
      operand *t = right;
      right = left;
      left = t;
    }

  lUnsigned = SPEC_USIGN (getSpec (operandType (left)));
  rUnsigned = SPEC_USIGN (getSpec (operandType (right)));

  pushedB = pushB ();

  if (size == 1 /* no, this is not a bug; with a 1 byte result there's
                   no need to take care about the signedness! */
      || (lUnsigned && rUnsigned))
    {
      /* just an unsigned 8 * 8 = 8 multiply
         or 8u * 8u = 16u */
      /* emitcode (";","unsigned"); */
      /* TODO: check for accumulator clash between left & right aops? */

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* moving to accumulator first helps peepholes */
          MOVA (aopGet (left, 0, FALSE, FALSE));
          MOVB (aopGet (right, 0, FALSE, FALSE));
        }
      else
        {
          emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
          MOVA (aopGet (left, 0, FALSE, FALSE));
        }

      emitcode ("mul", "ab");
      aopPut (result, "a", 0);
      if (size == 2)
        aopPut (result, "b", 1);

      popB (pushedB);
      return;
    }

  /* we have to do a signed multiply */
  /* emitcode (";", "signed"); */

  /* now sign adjust for both left & right */

  /* let's see what's needed: */
  /* apply negative sign during runtime */
  runtimeSign = FALSE;
  /* negative sign from literals */
  compiletimeSign = FALSE;

  if (!lUnsigned)
    {
      if (AOP_TYPE(left) == AOP_LIT)
        {
          /* signed literal */
          signed char val = (char) floatFromVal (AOP (left)->aopu.aop_lit);
          if (val < 0)
            compiletimeSign = TRUE;
        }
      else
        /* signed but not literal */
        runtimeSign = TRUE;
    }

  if (!rUnsigned)
    {
      if (AOP_TYPE(right) == AOP_LIT)
        {
          /* signed literal */
          signed char val = (char) floatFromVal (AOP (right)->aopu.aop_lit);
          if (val < 0)
            compiletimeSign ^= TRUE;
        }
      else
        /* signed but not literal */
        runtimeSign = TRUE;
    }

  /* initialize F0, which stores the runtime sign */
  if (runtimeSign)
    {
      if (compiletimeSign)
        emitcode ("setb", "F0"); /* set sign flag */
      else
        emitcode ("clr", "F0"); /* reset sign flag */
    }

  /* save the signs of the operands */
  if (AOP_TYPE(right) == AOP_LIT)
    {
      signed char val = (char) floatFromVal (AOP (right)->aopu.aop_lit);

      if (!rUnsigned && val < 0)
        emitcode ("mov", "b,#0x%02x", -val);
      else
        emitcode ("mov", "b,#0x%02x", (unsigned char) val);
    }
  else /* ! literal */
    {
      if (rUnsigned)  /* emitcode (";", "signed"); */
        emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
      else
        {
          MOVA (aopGet (right, 0, FALSE, FALSE));
          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("cpl", "F0"); /* complement sign flag */
          emitcode ("cpl", "a");  /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
          emitcode ("mov", "b,a");
        }
    }

  if (AOP_TYPE(left) == AOP_LIT)
    {
      signed char val = (char) floatFromVal (AOP (left)->aopu.aop_lit);

      if (!lUnsigned && val < 0)
        emitcode ("mov", "a,#0x%02x", -val);
      else
        emitcode ("mov", "a,#0x%02x", (unsigned char) val);
    }
  else /* ! literal */
    {
      MOVA (aopGet (left, 0, FALSE, FALSE));

      if (!lUnsigned)
        {
          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("cpl", "F0"); /* complement sign flag */
          emitcode ("cpl", "a"); /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
        }
    }

  /* now the multiplication */
  emitcode ("mul", "ab");
  if (runtimeSign || compiletimeSign)
    {
      lbl = newiTempLabel (NULL);
      if (runtimeSign)
        emitcode ("jnb", "F0,%05d$", (lbl->key + 100));
      emitcode ("cpl", "a"); /* lsb 2's complement */
      if (size != 2)
        emitcode ("inc", "a"); /* inc doesn't set carry flag */
      else
        {
          emitcode ("add", "a,#1"); /* this sets carry flag */
          emitcode ("xch", "a,b");
          emitcode ("cpl", "a"); /* msb 2's complement */
          emitcode ("addc", "a,#0");
          emitcode ("xch", "a,b");
        }
      emitLabel (lbl);
    }
  aopPut (result, "a", 0);
  if (size == 2)
    aopPut (result, "b", 1);

  popB (pushedB);
}

/*-----------------------------------------------------------------*/
/* genMult - generates code for multiplication                     */
/*-----------------------------------------------------------------*/
static void
genMult (iCode * ic)
{
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  operand *result = IC_RESULT (ic);

  D (emitcode (";", "genMult"));

  /* assign the asmops */
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, TRUE);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genMultbits (left, right, result);
      goto release;
    }

  /* if both are of size == 1 */
#if 0 // one of them can be a sloc shared with the result
    if (AOP_SIZE (left) == 1 && AOP_SIZE (right) == 1)
#else
  if (getSize(operandType(left)) == 1 &&
      getSize(operandType(right)) == 1)
#endif
    {
      genMultOneByte (left, right, result);
      goto release;
    }

  /* should have been converted to function call */
    fprintf (stderr, "left: %d right: %d\n", getSize(OP_SYMBOL(left)->type),
             getSize(OP_SYMBOL(right)->type));
  assert (0);

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genDivbits :- division of bits                                  */
/*-----------------------------------------------------------------*/
static void
genDivbits (operand * left,
            operand * right,
            operand * result)
{
  char *l;
  bool pushedB;

  D(emitcode (";", "genDivbits"));

  pushedB = pushB ();

  /* the result must be bit */
  emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
  l = aopGet (left, 0, FALSE, FALSE);

  MOVA (l);

  emitcode ("div", "ab");
  emitcode ("rrc", "a");

  popB (pushedB);

  aopPut (result, "c", 0);
}

/*-----------------------------------------------------------------*/
/* genDivOneByte : 8 bit division                                  */
/*-----------------------------------------------------------------*/
static void
genDivOneByte (operand * left,
               operand * right,
               operand * result)
{
  bool lUnsigned, rUnsigned, pushedB;
  bool runtimeSign, compiletimeSign;
  bool accuse = FALSE;
  bool pushedA = FALSE;
  symbol *lbl;
  int size, offset;

  D(emitcode (";", "genDivOneByte"));

  /* Why is it necessary that genDivOneByte() can return an int result?
     Have a look at:

        volatile unsigned char uc;
        volatile signed char sc1, sc2;
        volatile int i;

        uc  = 255;
        sc1 = -1;
        i = uc / sc1;

     Or:

        sc1 = -128;
        sc2 = -1;
        i = sc1 / sc2;

     In all cases a one byte result would overflow, the following cast to int
     would return the wrong result.

     Two possible solution:
        a) cast operands to int, if ((unsigned) / (signed)) or
           ((signed) / (signed))
        b) return an 16 bit signed int; this is what we're doing here!
  */

  size = AOP_SIZE (result) - 1;
  offset = 1;
  lUnsigned = SPEC_USIGN (getSpec (operandType (left)));
  rUnsigned = SPEC_USIGN (getSpec (operandType (right)));

  pushedB = pushB ();

  /* signed or unsigned */
  if (lUnsigned && rUnsigned)
    {
      /* unsigned is easy */
      MOVB (aopGet (right, 0, FALSE, FALSE));
      MOVA (aopGet (left, 0, FALSE, FALSE));
      emitcode ("div", "ab");
      aopPut (result, "a", 0);
      while (size--)
        aopPut (result, zero, offset++);

      popB (pushedB);
      return;
    }

  /* signed is a little bit more difficult */

  /* now sign adjust for both left & right */

  /* let's see what's needed: */
  /* apply negative sign during runtime */
  runtimeSign = FALSE;
  /* negative sign from literals */
  compiletimeSign = FALSE;

  if (!lUnsigned)
    {
      if (AOP_TYPE(left) == AOP_LIT)
        {
          /* signed literal */
          signed char val = (char) floatFromVal (AOP (left)->aopu.aop_lit);
          if (val < 0)
            compiletimeSign = TRUE;
        }
      else
        /* signed but not literal */
        runtimeSign = TRUE;
    }

  if (!rUnsigned)
    {
      if (AOP_TYPE(right) == AOP_LIT)
        {
          /* signed literal */
          signed char val = (char) floatFromVal (AOP (right)->aopu.aop_lit);
          if (val < 0)
            compiletimeSign ^= TRUE;
        }
      else
        /* signed but not literal */
        runtimeSign = TRUE;
    }

  /* initialize F0, which stores the runtime sign */
  if (runtimeSign)
    {
      if (compiletimeSign)
        emitcode ("setb", "F0"); /* set sign flag */
      else
        emitcode ("clr", "F0"); /* reset sign flag */
    }

  /* save the signs of the operands */
  if (AOP_TYPE(right) == AOP_LIT)
    {
      signed char val = (char) floatFromVal (AOP (right)->aopu.aop_lit);

      if (!rUnsigned && val < 0)
        emitcode ("mov", "b,#0x%02x", -val);
      else
        emitcode ("mov", "b,#0x%02x", (unsigned char) val);
    }
  else /* ! literal */
    {
      if (rUnsigned)
        emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
      else
        {
          MOVA (aopGet (right, 0, FALSE, FALSE));
          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("cpl", "F0"); /* complement sign flag */
          emitcode ("cpl", "a");  /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
          emitcode ("mov", "b,a");
        }
    }

  if (AOP_TYPE(left) == AOP_LIT)
    {
      signed char val = (char) floatFromVal (AOP (left)->aopu.aop_lit);

      if (!lUnsigned && val < 0)
        emitcode ("mov", "a,#0x%02x", -val);
      else
        emitcode ("mov", "a,#0x%02x", (unsigned char) val);
    }
  else /* ! literal */
    {
      MOVA (aopGet (left, 0, FALSE, FALSE));

      if (!lUnsigned)
        {
          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("cpl", "F0"); /* complement sign flag */
          emitcode ("cpl", "a");  /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
        }
    }

  /* now the division */
  emitcode ("div", "ab");

  if (runtimeSign || compiletimeSign)
    {
      lbl = newiTempLabel (NULL);
      if (runtimeSign)
        emitcode ("jnb", "F0,%05d$", (lbl->key + 100));
      emitcode ("cpl", "a"); /* lsb 2's complement */
      emitcode ("inc", "a");
      emitLabel (lbl);

      accuse = aopPut (result, "a", 0);
      if (size > 0)
        {
          /* msb is 0x00 or 0xff depending on the sign */
          if (runtimeSign)
            {
              if (accuse)
                {
                  emitcode ("push", "acc");
                  pushedA = TRUE;
                }
              emitcode ("mov", "c,F0");
              emitcode ("subb", "a,acc");
              while (size--)
                aopPut (result, "a", offset++);
            }
          else /* compiletimeSign */
            {
              if (aopPutUsesAcc (result, "#0xFF", offset))
                {
                  emitcode ("push", "acc");
                  pushedA = TRUE;
                }
              while (size--)
                aopPut (result, "#0xff", offset++);
            }
        }
    }
  else
    {
      aopPut (result, "a", 0);
      while (size--)
        aopPut (result, zero, offset++);
    }

  if (pushedA)
    emitcode ("pop", "acc");
  popB (pushedB);
}

/*-----------------------------------------------------------------*/
/* genDiv - generates code for division                            */
/*-----------------------------------------------------------------*/
static void
genDiv (iCode * ic)
{
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  operand *result = IC_RESULT (ic);

  D (emitcode (";", "genDiv"));

  /* assign the asmops */
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, TRUE);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genDivbits (left, right, result);
      goto release;
    }

  /* if both are of size == 1 */
  if (AOP_SIZE (left) == 1 &&
      AOP_SIZE (right) == 1)
    {
      genDivOneByte (left, right, result);
      goto release;
    }

  /* should have been converted to function call */
  assert (0);
release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genModbits :- modulus of bits                                   */
/*-----------------------------------------------------------------*/
static void
genModbits (operand * left,
            operand * right,
            operand * result)
{
  char *l;
  bool pushedB;

  D (emitcode (";", "genModbits"));

  pushedB = pushB ();

  /* the result must be bit */
  emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
  l = aopGet (left, 0, FALSE, FALSE);

  MOVA (l);

  emitcode ("div", "ab");
  emitcode ("mov", "a,b");
  emitcode ("rrc", "a");

  popB (pushedB);

  aopPut (result, "c", 0);
}

/*-----------------------------------------------------------------*/
/* genModOneByte : 8 bit modulus                                   */
/*-----------------------------------------------------------------*/
static void
genModOneByte (operand * left,
               operand * right,
               operand * result)
{
  bool lUnsigned, rUnsigned, pushedB;
  bool runtimeSign, compiletimeSign;
  symbol *lbl;
  int size, offset;

  D (emitcode (";", "genModOneByte"));

  size = AOP_SIZE (result) - 1;
  offset = 1;
  lUnsigned = SPEC_USIGN (getSpec (operandType (left)));
  rUnsigned = SPEC_USIGN (getSpec (operandType (right)));

  /* if right is a literal, check it for 2^n */
  if (AOP_TYPE(right) == AOP_LIT)
    {
      unsigned char val = abs((int) operandLitValue(right));
      symbol *lbl2 = NULL;

      switch (val)
        {
          case 1: /* sometimes it makes sense (on tricky code and hardware)... */
          case 2:
          case 4:
          case 8:
          case 16:
          case 32:
          case 64:
          case 128:
            if (lUnsigned)
              werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
                      "modulus of unsigned char by 2^n literal shouldn't be processed here");
              /* because iCode should have been changed to genAnd  */
              /* see file "SDCCopt.c", function "convertToFcall()" */

            MOVA (aopGet (left, 0, FALSE, FALSE));
            emitcode ("mov", "c,acc.7");
            emitcode ("anl", "a,#0x%02x", val - 1);
            lbl = newiTempLabel (NULL);
            emitcode ("jz", "%05d$", (lbl->key + 100));
            emitcode ("jnc", "%05d$", (lbl->key + 100));
            emitcode ("orl", "a,#0x%02x", 0xff ^ (val - 1));
            if (size)
              {
                int size2 = size;
                int offs2 = offset;

                aopPut (result, "a", 0);
                while (size2--)
                  aopPut (result, "#0xff", offs2++);
                lbl2 = newiTempLabel (NULL);
                emitcode ("sjmp", "%05d$", (lbl2->key + 100));
              }
            emitLabel (lbl);
            aopPut (result, "a", 0);
            while (size--)
              aopPut (result, zero, offset++);
            if (lbl2)
              {
                emitLabel (lbl2);
              }
            return;

          default:
            break;
        }
    }

  pushedB = pushB ();

  /* signed or unsigned */
  if (lUnsigned && rUnsigned)
    {
      /* unsigned is easy */
      MOVB (aopGet (right, 0, FALSE, FALSE));
      MOVA (aopGet (left, 0, FALSE, FALSE));
      emitcode ("div", "ab");
      aopPut (result, "b", 0);
      while (size--)
        aopPut (result, zero, offset++);

      popB (pushedB);
      return;
    }

  /* signed is a little bit more difficult */

  /* now sign adjust for both left & right */

  /* modulus: sign of the right operand has no influence on the result! */
  if (AOP_TYPE(right) == AOP_LIT)
    {
      signed char val = (char) operandLitValue(right);

      if (!rUnsigned && val < 0)
        emitcode ("mov", "b,#0x%02x", -val);
      else
        emitcode ("mov", "b,#0x%02x", (unsigned char) val);
    }
  else /* not literal */
    {
      if (rUnsigned)
        emitcode ("mov", "b,%s", aopGet (right, 0, FALSE, FALSE));
      else
        {
          MOVA (aopGet (right, 0, FALSE, FALSE));
          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("cpl", "a"); /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
          emitcode ("mov", "b,a");
        }
    }

  /* let's see what's needed: */
  /* apply negative sign during runtime */
  runtimeSign = FALSE;
  /* negative sign from literals */
  compiletimeSign = FALSE;

  /* sign adjust left side */
  if (AOP_TYPE(left) == AOP_LIT)
    {
      signed char val = (char) floatFromVal (AOP (left)->aopu.aop_lit);

      if (!lUnsigned && val < 0)
        {
          compiletimeSign = TRUE; /* set sign flag */
          emitcode ("mov", "a,#0x%02x", -val);
        }
      else
        emitcode ("mov", "a,#0x%02x", (unsigned char) val);
    }
  else /* ! literal */
    {
      MOVA (aopGet (left, 0, FALSE, FALSE));

      if (!lUnsigned)
        {
          runtimeSign = TRUE;
          emitcode ("clr", "F0"); /* clear sign flag */

          lbl = newiTempLabel (NULL);
          emitcode ("jnb", "acc.7,%05d$", (lbl->key + 100));
          emitcode ("setb", "F0"); /* set sign flag */
          emitcode ("cpl", "a");   /* 2's complement */
          emitcode ("inc", "a");
          emitLabel (lbl);
        }
    }

  /* now the modulus */
  emitcode ("div", "ab");

  if (runtimeSign || compiletimeSign)
    {
      emitcode ("mov", "a,b");
      lbl = newiTempLabel (NULL);
      if (runtimeSign)
        emitcode ("jnb", "F0,%05d$", (lbl->key + 100));
      emitcode ("cpl", "a"); /* 2's complement */
      emitcode ("inc", "a");
      emitLabel (lbl);

      aopPut (result, "a", 0);
      if (size > 0)
        {
          /* msb is 0x00 or 0xff depending on the sign */
          if (runtimeSign)
            {
              emitcode ("mov", "c,F0");
              emitcode ("subb", "a,acc");
              while (size--)
                aopPut (result, "a", offset++);
            }
          else /* compiletimeSign */
            while (size--)
              aopPut (result, "#0xff", offset++);
        }
    }
  else
    {
      aopPut (result, "b", 0);
      while (size--)
        aopPut (result, zero, offset++);
    }

  popB (pushedB);
}

/*-----------------------------------------------------------------*/
/* genMod - generates code for division                            */
/*-----------------------------------------------------------------*/
static void
genMod (iCode * ic)
{
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  operand *result = IC_RESULT (ic);

  D (emitcode (";", "genMod"));

  /* assign the asmops */
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, TRUE);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genModbits (left, right, result);
      goto release;
    }

  /* if both are of size == 1 */
  if (AOP_SIZE (left) == 1 &&
      AOP_SIZE (right) == 1)
    {
      genModOneByte (left, right, result);
      goto release;
    }

  /* should have been converted to function call */
  assert (0);

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genIfxJump :- will create a jump depending on the ifx           */
/*-----------------------------------------------------------------*/
static void
genIfxJump (iCode * ic, char *jval, operand *left, operand *right, operand *result)
{
  symbol *jlbl;
  symbol *tlbl = newiTempLabel (NULL);
  char *inst;

  D (emitcode (";", "genIfxJump"));

  /* if true label then we jump if condition
     supplied is true */
  if (IC_TRUE (ic))
    {
      jlbl = IC_TRUE (ic);
      inst = ((strcmp (jval, "a") == 0 ? "jz" :
               (strcmp (jval, "c") == 0 ? "jnc" : "jnb")));
    }
  else
    {
      /* false label is present */
      jlbl = IC_FALSE (ic);
      inst = ((strcmp (jval, "a") == 0 ? "jnz" :
               (strcmp (jval, "c") == 0 ? "jc" : "jb")));
    }
  if (strcmp (inst, "jb") == 0 || strcmp (inst, "jnb") == 0)
    emitcode (inst, "%s,%05d$", jval, (tlbl->key + 100));
  else
    emitcode (inst, "%05d$", tlbl->key + 100);
  freeForBranchAsmop (result);
  freeForBranchAsmop (right);
  freeForBranchAsmop (left);
  emitcode ("ljmp", "%05d$", jlbl->key + 100);
  emitLabel (tlbl);

  /* mark the icode as generated */
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genCmp :- greater or less than comparison                       */
/*-----------------------------------------------------------------*/
static void
genCmp (operand * left, operand * right,
        operand * result, iCode * ifx, int sign, iCode *ic)
{
  int size, offset = 0;
  unsigned long lit = 0L;
  bool rightInB;

  D (emitcode (";", "genCmp"));

  /* if left & right are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
      emitcode ("anl", "c,%s", AOP (left)->aopu.aop_dir);
    }
  else
    {
      /* subtract right from left if at the
         end the carry flag is set then we know that
         left is greater than right */
      size = max (AOP_SIZE (left), AOP_SIZE (right));

      /* if unsigned char cmp with lit, do cjne left,#right,zz */
      if ((size == 1) && !sign &&
          (AOP_TYPE (right) == AOP_LIT && AOP_TYPE (left) != AOP_DIR))
        {
          symbol *lbl = newiTempLabel (NULL);
          emitcode ("cjne", "%s,%s,%05d$",
                    aopGet (left, offset, FALSE, FALSE),
                    aopGet (right, offset, FALSE, FALSE),
                    lbl->key + 100);
          emitLabel (lbl);
        }
      else
        {
          if (AOP_TYPE (right) == AOP_LIT)
            {
              lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);
              /* optimize if(x < 0) or if(x >= 0) */
              if (lit == 0L)
                {
                  if (!sign)
                    {
                      CLRC;
                    }
                  else
                    {
                      MOVA (aopGet (left, AOP_SIZE (left) - 1, FALSE, FALSE));
                      if (!(AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result)) && ifx)
                        {
                          genIfxJump (ifx, "acc.7", left, right, result);
                          freeAsmop (right, NULL, ic, TRUE);
                          freeAsmop (left, NULL, ic, TRUE);

                          return;
                        }
                      else
                        {
                          emitcode ("rlc", "a");
                        }
                    }
                  goto release;
                }
              else
                {//nonzero literal
                  int bytelit = ((lit >> (offset * 8)) & 0x0FFL);
                  while (size && (bytelit == 0))
                    {
                      offset++;
                      bytelit = ((lit >> (offset * 8)) & 0x0FFL);
                      size--;
                    }
                  CLRC;
                  while (size--)
                    {
                      MOVA (aopGet (left, offset, FALSE, FALSE));
                      if (sign && size == 0)
                        {
                          emitcode ("xrl", "a,#0x80");
                          emitcode ("subb", "a,#0x%02x",
                                    0x80 ^ (unsigned int) ((lit >> (offset * 8)) & 0x0FFL));
                        }
                      else
                        {
                          emitcode ("subb", "a,%s", aopGet (right, offset, FALSE, FALSE));
                        }
                      offset++;
                    }
                  goto release;
                }
            }
          CLRC;
          while (size--)
            {
              bool pushedB = FALSE;
              rightInB = aopGetUsesAcc(right, offset);
              if (rightInB)
                {
                  pushedB = pushB ();
                  emitcode ("mov", "b,%s", aopGet (right, offset, FALSE, FALSE));
                }
              MOVA (aopGet (left, offset, FALSE, FALSE));
              if (sign && size == 0)
                {
                  emitcode ("xrl", "a,#0x80");
                  if (!rightInB)
                    {
                      pushedB = pushB ();
                      rightInB++;
                      MOVB (aopGet (right, offset, FALSE, FALSE));
                    }
                  emitcode ("xrl", "b,#0x80");
                  emitcode ("subb", "a,b");
                }
              else
                {
                  if (rightInB)
                    emitcode ("subb", "a,b");
                  else
                    emitcode ("subb", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              if (rightInB)
                popB (pushedB);
              offset++;
            }
        }
    }

release:
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  if (AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result))
    {
      outBitC (result);
    }
  else
    {
      /* if the result is used in the next
         ifx conditional branch then generate
         code a little differently */
      if (ifx)
        {
          genIfxJump (ifx, "c", NULL, NULL, result);
        }
      else
        {
          outBitC (result);
        }
      /* leave the result in acc */
    }
}

/*-----------------------------------------------------------------*/
/* genCmpGt :- greater than comparison                             */
/*-----------------------------------------------------------------*/
static void
genCmpGt (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  sym_link *letype, *retype;
  int sign;

  D (emitcode (";", "genCmpGt"));

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);

  letype = getSpec (operandType (left));
  retype = getSpec (operandType (right));
  sign = !((SPEC_USIGN (letype) && !(IS_CHAR (letype) && IS_LITERAL (letype))) ||
           (SPEC_USIGN (retype) && !(IS_CHAR (retype) && IS_LITERAL (retype))));
  /* assign the asmops */
  aopOp (result, ic, TRUE);
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);

  genCmp (right, left, result, ifx, sign, ic);

  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genCmpLt - less than comparisons                                */
/*-----------------------------------------------------------------*/
static void
genCmpLt (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  sym_link *letype, *retype;
  int sign;

  D (emitcode (";", "genCmpLt"));

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);

  letype = getSpec (operandType (left));
  retype = getSpec (operandType (right));
  sign = !((SPEC_USIGN (letype) && !(IS_CHAR (letype) && IS_LITERAL (letype))) ||
           (SPEC_USIGN (retype) && !(IS_CHAR (retype) && IS_LITERAL (retype))));
  /* assign the asmops */
  aopOp (result, ic, TRUE);
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);

  genCmp (left, right, result, ifx, sign, ic);

  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* gencjneshort - compare and jump if not equal                    */
/*-----------------------------------------------------------------*/
static void
gencjneshort (operand * left, operand * right, symbol * lbl)
{
  int size = max (AOP_SIZE (left), AOP_SIZE (right));
  int offset = 0;
  unsigned long lit = 0L;

  D (emitcode (";", "gencjneshort"));

  /* if the left side is a literal or
     if the right is in a pointer register and left
     is not */
  if ((AOP_TYPE (left) == AOP_LIT)  ||
      (AOP_TYPE (left) == AOP_IMMD) ||
      (AOP_TYPE (left) == AOP_DIR)  ||
      (IS_AOP_PREG (right) && !IS_AOP_PREG (left)))
    {
      operand *t = right;
      right = left;
      left = t;
    }

  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  /* if the right side is a literal then anything goes */
  if (AOP_TYPE (right) == AOP_LIT &&
      AOP_TYPE (left) != AOP_DIR  &&
      AOP_TYPE (left) != AOP_IMMD)
    {
      while (size--)
        {
          emitcode ("cjne", "%s,%s,%05d$",
                    aopGet (left, offset, FALSE, FALSE),
                    aopGet (right, offset, FALSE, FALSE),
                    lbl->key + 100);
          offset++;
        }
    }

  /* if the right side is in a register or in direct space or
     if the left is a pointer register & right is not */
  else if (AOP_TYPE (right) == AOP_REG ||
           AOP_TYPE (right) == AOP_DIR ||
           AOP_TYPE (right) == AOP_LIT ||
           AOP_TYPE (right) == AOP_IMMD ||
           (AOP_TYPE (left) == AOP_DIR && AOP_TYPE (right) == AOP_LIT) ||
           (IS_AOP_PREG (left) && !IS_AOP_PREG (right)))
    {
      while (size--)
        {
          MOVA (aopGet (left, offset, FALSE, FALSE));
          if ((AOP_TYPE (left) == AOP_DIR && AOP_TYPE (right) == AOP_LIT) &&
              ((unsigned int) ((lit >> (offset * 8)) & 0x0FFL) == 0))
            emitcode ("jnz", "%05d$", lbl->key + 100);
          else
            emitcode ("cjne", "a,%s,%05d$",
                      aopGet (right, offset, FALSE, TRUE),
                      lbl->key + 100);
          offset++;
        }
    }
  else
    {
      /* right is a pointer reg need both a & b */
      while (size--)
        {
          //if B in use: push B; mov B,left; mov A,right; clrc; subb A,B; pop B; jnz
          wassertl(!BINUSE, "B was in use");
          MOVB (aopGet (left, offset, FALSE, FALSE));
          MOVA (aopGet (right, offset, FALSE, FALSE));
          emitcode ("cjne", "a,b,%05d$", lbl->key + 100);
          offset++;
        }
    }
}

/*-----------------------------------------------------------------*/
/* gencjne - compare and jump if not equal                         */
/*-----------------------------------------------------------------*/
static void
gencjne (operand * left, operand * right, symbol * lbl, bool useCarry)
{
  symbol *tlbl = newiTempLabel (NULL);

  D (emitcode (";", "gencjne"));

  gencjneshort (left, right, lbl);

  if (useCarry)
      SETC;
  else
      MOVA (one);
  emitcode ("sjmp", "%05d$", tlbl->key + 100);
  emitLabel (lbl);
  if (useCarry)
      CLRC;
  else
      MOVA (zero);
  emitLabel (tlbl);
}

/*-----------------------------------------------------------------*/
/* genCmpEq - generates code for equal to                          */
/*-----------------------------------------------------------------*/
static void
genCmpEq (iCode * ic, iCode * ifx)
{
  bool swappedLR = FALSE;
  operand *left, *right, *result;

  D (emitcode (";", "genCmpEq"));

  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, TRUE);

  /* if literal, literal on the right or
     if the right is in a pointer register and left
     is not */
  if ((AOP_TYPE (IC_LEFT (ic)) == AOP_LIT) ||
      (IS_AOP_PREG (right) && !IS_AOP_PREG (left)))
    {
      operand *t = IC_RIGHT (ic);
      IC_RIGHT (ic) = IC_LEFT (ic);
      IC_LEFT (ic) = t;
      swappedLR = TRUE;
    }

  if (ifx && !AOP_SIZE (result))
    {
      symbol *tlbl;
      /* if they are both bit variables */
      if (AOP_TYPE (left) == AOP_CRY &&
          ((AOP_TYPE (right) == AOP_CRY) || (AOP_TYPE (right) == AOP_LIT)))
        {
          if (AOP_TYPE (right) == AOP_LIT)
            {
              unsigned long lit = (unsigned long) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);
              if (lit == 0L)
                {
                  emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
                  emitcode ("cpl", "c");
                }
              else if (lit == 1L)
                {
                  emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
                }
              else
                {
                  emitcode ("clr", "c");
                }
              /* AOP_TYPE(right) == AOP_CRY */
            }
          else
            {
              symbol *lbl = newiTempLabel (NULL);
              emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
              emitcode ("jb", "%s,%05d$", AOP (right)->aopu.aop_dir, (lbl->key + 100));
              emitcode ("cpl", "c");
              emitLabel (lbl);
            }
          /* if true label then we jump if condition
             supplied is true */
          tlbl = newiTempLabel (NULL);
          if (IC_TRUE (ifx))
            {
              emitcode ("jnc", "%05d$", tlbl->key + 100);
              freeForBranchAsmop (result);
              freeForBranchAsmop (right);
              freeForBranchAsmop (left);
              emitcode ("ljmp", "%05d$", IC_TRUE (ifx)->key + 100);
            }
          else
            {
              emitcode ("jc", "%05d$", tlbl->key + 100);
              freeForBranchAsmop (result);
              freeForBranchAsmop (right);
              freeForBranchAsmop (left);
              emitcode ("ljmp", "%05d$", IC_FALSE (ifx)->key + 100);
            }
          emitLabel (tlbl);
        }
      else
        {
          tlbl = newiTempLabel (NULL);
          gencjneshort (left, right, tlbl);
          if (IC_TRUE (ifx))
            {
              freeForBranchAsmop (result);
              freeForBranchAsmop (right);
              freeForBranchAsmop (left);
              emitcode ("ljmp", "%05d$", IC_TRUE (ifx)->key + 100);
              emitLabel (tlbl);
            }
          else
            {
              symbol *lbl = newiTempLabel (NULL);
              emitcode ("sjmp", "%05d$", lbl->key + 100);
              emitLabel (tlbl);
              freeForBranchAsmop (result);
              freeForBranchAsmop (right);
              freeForBranchAsmop (left);
              emitcode ("ljmp", "%05d$", IC_FALSE (ifx)->key + 100);
              emitLabel (lbl);
            }
        }
      /* mark the icode as generated */
      ifx->generated = 1;
      goto release;
    }

  /* if they are both bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      ((AOP_TYPE (right) == AOP_CRY) || (AOP_TYPE (right) == AOP_LIT)))
    {
      if (AOP_TYPE (right) == AOP_LIT)
        {
          unsigned long lit = (unsigned long) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);
          if (lit == 0L)
            {
              emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
              emitcode ("cpl", "c");
            }
          else if (lit == 1L)
            {
              emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
            }
          else
            {
              emitcode ("clr", "c");
            }
          /* AOP_TYPE(right) == AOP_CRY */
        }
      else
        {
          symbol *lbl = newiTempLabel (NULL);
          emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
          emitcode ("jb", "%s,%05d$", AOP (right)->aopu.aop_dir, (lbl->key + 100));
          emitcode ("cpl", "c");
          emitLabel (lbl);
        }
      /* c = 1 if egal */
      if (AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result))
        {
          outBitC (result);
          goto release;
        }
      if (ifx)
        {
          genIfxJump (ifx, "c", left, right, result);
          goto release;
        }
      /* if the result is used in an arithmetic operation
         then put the result in place */
      outBitC (result);
    }
  else
    {
      if (AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result))
        {
          gencjne (left, right, newiTempLabel (NULL), TRUE);
          aopPut (result, "c", 0);
          goto release;
        }
      gencjne (left, right, newiTempLabel (NULL), FALSE);
      if (ifx)
        {
          genIfxJump (ifx, "a", left, right, result);
          goto release;
        }
      /* if the result is used in an arithmetic operation
         then put the result in place */
      if (AOP_TYPE (result) != AOP_CRY)
        outAcc (result);
      /* leave the result in acc */
    }

release:
  freeAsmop (result, NULL, ic, TRUE);
  if (!swappedLR)
    {
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
    }
  else
    {
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
    }
}

/*-----------------------------------------------------------------*/
/* ifxForOp - returns the icode containing the ifx for operand     */
/*-----------------------------------------------------------------*/
static iCode *
ifxForOp (operand * op, iCode * ic)
{
  /* if true symbol then needs to be assigned */
  if (IS_TRUE_SYMOP (op))
    return NULL;

  /* if this has register type condition and
     the next instruction is ifx with the same operand
     and live to of the operand is upto the ifx only then */
  if (ic->next &&
      ic->next->op == IFX &&
      IC_COND (ic->next)->key == op->key &&
      OP_SYMBOL (op)->liveTo <= ic->next->seq)
    return ic->next;

  return NULL;
}

/*-----------------------------------------------------------------*/
/* hasInc - operand is incremented before any other use            */
/*-----------------------------------------------------------------*/
static iCode *
hasInc (operand *op, iCode *ic, int osize)
{
  sym_link *type = operandType(op);
  sym_link *retype = getSpec (type);
  iCode *lic = ic->next;
  int isize ;

  /* this could from a cast, e.g.: "(char xdata *) 0x7654;" */
  if (!IS_SYMOP(op)) return NULL;

  if (IS_BITVAR(retype)||!IS_PTR(type)) return NULL;
  if (IS_AGGREGATE(type->next)) return NULL;
  if (osize != (isize = getSize(type->next))) return NULL;

  while (lic) {
    /* if operand of the form op = op + <sizeof *op> */
    if (lic->op == '+' && isOperandEqual(IC_LEFT(lic),op) &&
        isOperandEqual(IC_RESULT(lic),op) &&
        isOperandLiteral(IC_RIGHT(lic)) &&
        operandLitValue(IC_RIGHT(lic)) == isize) {
      return lic;
    }
    /* if the operand used or deffed */
    if (bitVectBitValue(OP_USES(op),lic->key) || lic->defKey == op->key) {
      return NULL;
    }
    /* if GOTO or IFX */
    if (lic->op == IFX || lic->op == GOTO || lic->op == LABEL) break;
    lic = lic->next;
  }
  return NULL;
}

/*-----------------------------------------------------------------*/
/* genAndOp - for && operation                                     */
/*-----------------------------------------------------------------*/
static void
genAndOp (iCode * ic)
{
  operand *left, *right, *result;
  symbol *tlbl;

  D (emitcode (";", "genAndOp"));

  /* note here that && operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, FALSE);

  /* if both are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
      emitcode ("anl", "c,%s", AOP (right)->aopu.aop_dir);
      outBitC (result);
    }
  else
    {
      tlbl = newiTempLabel (NULL);
      toBoolean (left);
      emitcode ("jz", "%05d$", tlbl->key + 100);
      toBoolean (right);
      emitLabel (tlbl);
      outBitAcc (result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}


/*-----------------------------------------------------------------*/
/* genOrOp - for || operation                                      */
/*-----------------------------------------------------------------*/
static void
genOrOp (iCode * ic)
{
  operand *left, *right, *result;
  symbol *tlbl;

  D (emitcode (";", "genOrOp"));

  /* note here that || operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, FALSE);

  /* if both are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
      emitcode ("orl", "c,%s", AOP (right)->aopu.aop_dir);
      outBitC (result);
    }
  else
    {
      tlbl = newiTempLabel (NULL);
      toBoolean (left);
      emitcode ("jnz", "%05d$", tlbl->key + 100);
      toBoolean (right);
      emitLabel (tlbl);
      outBitAcc (result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* isLiteralBit - test if lit == 2^n                               */
/*-----------------------------------------------------------------*/
static int
isLiteralBit (unsigned long lit)
{
  unsigned long pw[32] =
  {1L, 2L, 4L, 8L, 16L, 32L, 64L, 128L,
   0x100L, 0x200L, 0x400L, 0x800L,
   0x1000L, 0x2000L, 0x4000L, 0x8000L,
   0x10000L, 0x20000L, 0x40000L, 0x80000L,
   0x100000L, 0x200000L, 0x400000L, 0x800000L,
   0x1000000L, 0x2000000L, 0x4000000L, 0x8000000L,
   0x10000000L, 0x20000000L, 0x40000000L, 0x80000000L};
  int idx;

  for (idx = 0; idx < 32; idx++)
    if (lit == pw[idx])
      return idx + 1;
  return 0;
}

/*-----------------------------------------------------------------*/
/* continueIfTrue -                                                */
/*-----------------------------------------------------------------*/
static void
continueIfTrue (iCode * ic)
{
  if (IC_TRUE (ic))
    emitcode ("ljmp", "%05d$", IC_TRUE (ic)->key + 100);
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* jmpIfTrue -                                                     */
/*-----------------------------------------------------------------*/
static void
jumpIfTrue (iCode * ic)
{
  if (!IC_TRUE (ic))
    emitcode ("ljmp", "%05d$", IC_FALSE (ic)->key + 100);
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* jmpTrueOrFalse -                                                */
/*-----------------------------------------------------------------*/
static void
jmpTrueOrFalse (iCode * ic, symbol * tlbl, operand *left, operand *right, operand *result)
{
  // ugly but optimized by peephole
  if (IC_TRUE (ic))
    {
      symbol *nlbl = newiTempLabel (NULL);
      emitcode ("sjmp", "%05d$", nlbl->key + 100);
      emitLabel (tlbl);
      freeForBranchAsmop (result);
      freeForBranchAsmop (right);
      freeForBranchAsmop (left);
      emitcode ("ljmp", "%05d$", IC_TRUE (ic)->key + 100);
      emitLabel (nlbl);
    }
  else
    {
      freeForBranchAsmop (result);
      freeForBranchAsmop (right);
      freeForBranchAsmop (left);
      emitcode ("ljmp", "%05d$", IC_FALSE (ic)->key + 100);
      emitLabel (tlbl);
    }
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genAnd  - code for and                                          */
/*-----------------------------------------------------------------*/
static void
genAnd (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  unsigned long lit = 0L;
  int bytelit = 0;
  char buffer[10];

  D (emitcode (";", "genAnd"));

  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, TRUE);

#ifdef DEBUG_TYPE
  emitcode (";", "Type res[%d] = l[%d]&r[%d]",
            AOP_TYPE (result),
            AOP_TYPE (left), AOP_TYPE (right));
  emitcode (";", "Size res[%d] = l[%d]&r[%d]",
            AOP_SIZE (result),
            AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not then exchange them */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT) ||
      AOP_NEEDSACC (left))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if result = right then exchange left and right */
  if (sameRegs (AOP (result), AOP (right)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if right is bit then exchange them */
  if (AOP_TYPE (right) == AOP_CRY &&
      AOP_TYPE (left) != AOP_CRY)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }
  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  size = AOP_SIZE (result);

  // if(bit & yy)
  // result = bit & yy;
  if (AOP_TYPE (left) == AOP_CRY)
    {
      // c = bit & literal;
      if (AOP_TYPE (right) == AOP_LIT)
        {
          if (lit & 1)
            {
              if (size && sameRegs (AOP (result), AOP (left)))
                // no change
                goto release;
              emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
            }
          else
            {
              // bit(result) = 0;
              if (size && (AOP_TYPE (result) == AOP_CRY))
                {
                  emitcode ("clr", "%s", AOP (result)->aopu.aop_dir);
                  goto release;
                }
              if ((AOP_TYPE (result) == AOP_CRY) && ifx)
                {
                  jumpIfTrue (ifx);
                  goto release;
                }
              emitcode ("clr", "c");
            }
        }
      else
        {
          if (AOP_TYPE (right) == AOP_CRY)
            {
              // c = bit & bit;
              emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
              emitcode ("anl", "c,%s", AOP (left)->aopu.aop_dir);
            }
          else
            {
              // c = bit & val;
              MOVA (aopGet (right, 0, FALSE, FALSE));
              // c = lsb
              emitcode ("rrc", "a");
              emitcode ("anl", "c,%s", AOP (left)->aopu.aop_dir);
            }
        }
      // bit = c
      // val = c
      if (size)
        outBitC (result);
      // if(bit & ...)
      else if ((AOP_TYPE (result) == AOP_CRY) && ifx)
        genIfxJump (ifx, "c", left, right, result);
      goto release;
    }

  // if(val & 0xZZ)       - size = 0, ifx != FALSE  -
  // bit = val & 0xZZ     - size = 1, ifx = FALSE -
  if ((AOP_TYPE (right) == AOP_LIT) &&
      (AOP_TYPE (result) == AOP_CRY) &&
      (AOP_TYPE (left) != AOP_CRY))
    {
      int posbit = isLiteralBit (lit);
      /* left &  2^n */
      if (posbit)
        {
          posbit--;
          MOVA (aopGet (left, posbit >> 3, FALSE, FALSE));
          // bit = left & 2^n
          if (size)
            {
              switch (posbit & 0x07)
                {
                  case 0: emitcode ("rrc", "a");
                          break;
                  case 7: emitcode ("rlc", "a");
                          break;
                  default: emitcode ("mov", "c,acc.%d", posbit & 0x07);
                          break;
                }
            }
          // if(left &  2^n)
          else
            {
              if (ifx)
                {
                  SNPRINTF (buffer, sizeof(buffer),
                            "acc.%d", posbit & 0x07);
                  genIfxJump (ifx, buffer, left, right, result);
                }
              else
                {// what is this case? just found it in ds390/gen.c
                  emitcode ("anl","a,#!constbyte",1 << (posbit & 0x07));
                }
              goto release;
            }
        }
      else
        {
          symbol *tlbl = newiTempLabel (NULL);
          int sizel = AOP_SIZE (left);
          if (size)
            emitcode ("setb", "c");
          while (sizel--)
            {
              if ((bytelit = ((lit >> (offset * 8)) & 0x0FFL)) != 0x0L)
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  // byte ==  2^n ?
                  if ((posbit = isLiteralBit (bytelit)) != 0)
                    emitcode ("jb", "acc.%d,%05d$", (posbit - 1) & 0x07, tlbl->key + 100);
                  else
                    {
                      if (bytelit != 0x0FFL)
                        emitcode ("anl", "a,%s",
                                  aopGet (right, offset, FALSE, TRUE));
                      emitcode ("jnz", "%05d$", tlbl->key + 100);
                    }
                }
              offset++;
            }
          // bit = left & literal
          if (size)
            {
              emitcode ("clr", "c");
              emitLabel (tlbl);
            }
          // if(left & literal)
          else
            {
              if (ifx)
                jmpTrueOrFalse (ifx, tlbl, left, right, result);
              else
                emitLabel (tlbl);
              goto release;
            }
        }
      outBitC (result);
      goto release;
    }

  /* if left is same as result */
  if (sameRegs (AOP (result), AOP (left)))
    {
      for (; size--; offset++)
        {
          if (AOP_TYPE (right) == AOP_LIT)
            {
              bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
              if (bytelit == 0x0FF)
                {
                  /* dummy read of volatile operand */
                  if (isOperandVolatile (left, FALSE))
                    MOVA (aopGet (left, offset, FALSE, FALSE));
                  else
                    continue;
                }
              else if (bytelit == 0)
                {
                  aopPut (result, zero, offset);
                }
              else if (IS_AOP_PREG (result))
                {
                  MOVA (aopGet (left, offset, FALSE, TRUE));
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                emitcode ("anl", "%s,%s",
                          aopGet (left, offset, FALSE, TRUE),
                          aopGet (right, offset, FALSE, FALSE));
            }
          else
            {
              if (AOP_TYPE (left) == AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("anl", "a,b");
                  aopPut (result, "a", offset);
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  if (IS_AOP_PREG (result))
                    {
                      emitcode ("anl", "a,%s", aopGet (left, offset, FALSE, TRUE));
                      aopPut (result, "a", offset);
                    }
                  else
                    emitcode ("anl", "%s,a", aopGet (left, offset, FALSE, TRUE));
                }
            }
        }
    }
  else
    {
      // left & result in different registers
      if (AOP_TYPE (result) == AOP_CRY)
        {
          // result = bit
          // if(size), result in bit
          // if(!size && ifx), conditional oper: if(left & right)
          symbol *tlbl = newiTempLabel (NULL);
          int sizer = min (AOP_SIZE (left), AOP_SIZE (right));
          if (size)
            emitcode ("setb", "c");
          while (sizer--)
            {
              if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("anl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("anl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("anl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                    }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("anl", "a,%s", aopGet (left, offset, FALSE, FALSE));
                }

              emitcode ("jnz", "%05d$", tlbl->key + 100);
              offset++;
            }
          if (size)
            {
              CLRC;
              emitLabel (tlbl);
              outBitC (result);
            }
          else if (ifx)
            jmpTrueOrFalse (ifx, tlbl, left, right, result);
          else
            emitLabel (tlbl);
        }
      else
        {
          for (; (size--); offset++)
            {
              // normal case
              // result = left & right
              if (AOP_TYPE (right) == AOP_LIT)
                {
                  bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
                  if (bytelit == 0x0FF)
                    {
                      aopPut (result,
                              aopGet (left, offset, FALSE, FALSE),
                              offset);
                      continue;
                    }
                  else if (bytelit == 0)
                    {
                      /* dummy read of volatile operand */
                      if (isOperandVolatile (left, FALSE))
                        MOVA (aopGet (left, offset, FALSE, FALSE));
                      aopPut (result, zero, offset);
                      continue;
                    }
                  else if (AOP_TYPE (left) == AOP_ACC)
                    {
                      if (!offset)
                        {
                          emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                          aopPut (result, "a", offset);
                          continue;
                        }
                      else
                        {
                          emitcode ("anl", "b,%s", aopGet (right, offset, FALSE, FALSE));
                          aopPut (result, "b", offset);
                          continue;
                        }
                    }
                }
              // faster than result <- left, anl result,right
              // and better if result is SFR
              if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("anl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("anl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("anl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("anl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("anl", "a,%s", aopGet (left, offset, FALSE, FALSE));
                }
              aopPut (result, "a", offset);
            }
        }
    }

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genOr  - code for or                                            */
/*-----------------------------------------------------------------*/
static void
genOr (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  unsigned long lit = 0L;
  int bytelit = 0;

  D (emitcode (";", "genOr"));

  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, TRUE);

#ifdef DEBUG_TYPE
  emitcode (";", "Type res[%d] = l[%d]&r[%d]",
            AOP_TYPE (result),
            AOP_TYPE (left), AOP_TYPE (right));
  emitcode (";", "Size res[%d] = l[%d]&r[%d]",
            AOP_SIZE (result),
            AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not then exchange them */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT) ||
      AOP_NEEDSACC (left))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if result = right then exchange them */
  if (sameRegs (AOP (result), AOP (right)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if right is bit then exchange them */
  if (AOP_TYPE (right) == AOP_CRY &&
      AOP_TYPE (left) != AOP_CRY)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }
  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  size = AOP_SIZE (result);

  // if(bit | yy)
  // xx = bit | yy;
  if (AOP_TYPE (left) == AOP_CRY)
    {
      if (AOP_TYPE (right) == AOP_LIT)
        {
          // c = bit | literal;
          if (lit)
            {
              // lit != 0 => result = 1
              if (AOP_TYPE (result) == AOP_CRY)
                {
                  if (size)
                    emitcode ("setb", "%s", AOP (result)->aopu.aop_dir);
                  else if (ifx)
                    continueIfTrue (ifx);
                  goto release;
                }
              emitcode ("setb", "c");
            }
          else
            {
              // lit == 0 => result = left
              if (size && sameRegs (AOP (result), AOP (left)))
                goto release;
              emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
            }
        }
      else
        {
          if (AOP_TYPE (right) == AOP_CRY)
            {
              // c = bit | bit;
              emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
              emitcode ("orl", "c,%s", AOP (left)->aopu.aop_dir);
            }
          else
            {
              // c = bit | val;
              symbol *tlbl = newiTempLabel (NULL);
              if (!((AOP_TYPE (result) == AOP_CRY) && ifx))
                emitcode ("setb", "c");
              emitcode ("jb", "%s,%05d$",
                        AOP (left)->aopu.aop_dir, tlbl->key + 100);
              toBoolean (right);
              emitcode ("jnz", "%05d$", tlbl->key + 100);
              if ((AOP_TYPE (result) == AOP_CRY) && ifx)
                {
                  jmpTrueOrFalse (ifx, tlbl, left, right, result);
                  goto release;
                }
              else
                {
                  CLRC;
                  emitLabel (tlbl);
                }
            }
        }
      // bit = c
      // val = c
      if (size)
        outBitC (result);
      // if(bit | ...)
      else if ((AOP_TYPE (result) == AOP_CRY) && ifx)
        genIfxJump (ifx, "c", left, right, result);
      goto release;
    }

  // if(val | 0xZZ)       - size = 0, ifx != FALSE  -
  // bit = val | 0xZZ     - size = 1, ifx = FALSE -
  if ((AOP_TYPE (right) == AOP_LIT) &&
      (AOP_TYPE (result) == AOP_CRY) &&
      (AOP_TYPE (left) != AOP_CRY))
    {
      if (lit)
        {
          // result = 1
          if (size)
            emitcode ("setb", "%s", AOP (result)->aopu.aop_dir);
          else
            continueIfTrue (ifx);
          goto release;
        }
      else
        {
          // lit = 0, result = boolean(left)
          if (size)
            emitcode ("setb", "c");
          toBoolean (right);
          if (size)
            {
              symbol *tlbl = newiTempLabel (NULL);
              emitcode ("jnz", "%05d$", tlbl->key + 100);
              CLRC;
              emitLabel (tlbl);
            }
          else
            {
              genIfxJump (ifx, "a", left, right, result);
              goto release;
            }
        }
      outBitC (result);
      goto release;
    }

  /* if left is same as result */
  if (sameRegs (AOP (result), AOP (left)))
    {
      for (; size--; offset++)
        {
          if (AOP_TYPE (right) == AOP_LIT)
            {
              bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
              if (bytelit == 0)
                {
                  /* dummy read of volatile operand */
                  if (isOperandVolatile (left, FALSE))
                    MOVA (aopGet (left, offset, FALSE, FALSE));
                  else
                    continue;
                }
              else if (bytelit == 0x0FF)
                {
                  aopPut (result, "#0xFF", offset);
                }
              else if (IS_AOP_PREG (left))
                {
                  MOVA (aopGet (left, offset, FALSE, TRUE));
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                {
                  emitcode ("orl", "%s,%s",
                            aopGet (left, offset, FALSE, TRUE),
                            aopGet (right, offset, FALSE, FALSE));
                }
            }
          else
            {
              if (AOP_TYPE (left) == AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("orl", "a,b");
                  aopPut (result, "a", offset);
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  if (IS_AOP_PREG (left))
                    {
                      emitcode ("orl", "a,%s", aopGet (left, offset, FALSE, TRUE));
                      aopPut (result, "a", offset);
                    }
                  else
                    {
                      emitcode ("orl", "%s,a", aopGet (left, offset, FALSE, TRUE));
                    }
                }
            }
        }
    }
  else
    {
      // left & result in different registers
      if (AOP_TYPE (result) == AOP_CRY)
        {
          // result = bit
          // if(size), result in bit
          // if(!size && ifx), conditional oper: if(left | right)
          symbol *tlbl = newiTempLabel (NULL);
          int sizer = max (AOP_SIZE (left), AOP_SIZE (right));
          if (size)
            emitcode ("setb", "c");
          while (sizer--)
            {
              if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("orl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("orl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("orl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("orl", "a,%s", aopGet (left, offset, FALSE, FALSE));
              }

              emitcode ("jnz", "%05d$", tlbl->key + 100);
              offset++;
            }
          if (size)
            {
              CLRC;
              emitLabel (tlbl);
              outBitC (result);
            }
          else if (ifx)
            jmpTrueOrFalse (ifx, tlbl, left, right, result);
          else
            emitLabel (tlbl);
        }
      else
        {
          for (; (size--); offset++)
            {
              // normal case
              // result = left | right
              if (AOP_TYPE (right) == AOP_LIT)
                {
                  bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
                  if (bytelit == 0)
                    {
                      aopPut (result,
                              aopGet (left, offset, FALSE, FALSE),
                              offset);
                      continue;
                    }
                  else if (bytelit == 0x0FF)
                    {
                      /* dummy read of volatile operand */
                      if (isOperandVolatile (left, FALSE))
                        MOVA (aopGet (left, offset, FALSE, FALSE));
                      aopPut (result, "#0xFF", offset);
                      continue;
                    }
                }
              // faster than result <- left, orl result,right
              // and better if result is SFR
              if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("orl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("orl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("orl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("orl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("orl", "a,%s", aopGet (left, offset, FALSE, FALSE));
                }
              aopPut (result, "a", offset);
            }
        }
    }

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genXor - code for xclusive or                                   */
/*-----------------------------------------------------------------*/
static void
genXor (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;
  int size, offset = 0;
  unsigned long lit = 0L;
  int bytelit = 0;

  D (emitcode (";", "genXor"));

  aopOp ((left = IC_LEFT (ic)), ic, FALSE);
  aopOp ((right = IC_RIGHT (ic)), ic, FALSE);
  aopOp ((result = IC_RESULT (ic)), ic, TRUE);

#ifdef DEBUG_TYPE
  emitcode (";", "Type res[%d] = l[%d]&r[%d]",
            AOP_TYPE (result),
            AOP_TYPE (left), AOP_TYPE (right));
  emitcode (";", "Size res[%d] = l[%d]&r[%d]",
            AOP_SIZE (result),
            AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not ||
     if left needs acc & right does not */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT) ||
      (AOP_NEEDSACC (left) && !AOP_NEEDSACC (right)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if result = right then exchange them */
  if (sameRegs (AOP (result), AOP (right)))
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  /* if right is bit then exchange them */
  if (AOP_TYPE (right) == AOP_CRY &&
      AOP_TYPE (left) != AOP_CRY)
    {
      operand *tmp = right;
      right = left;
      left = tmp;
    }

  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  size = AOP_SIZE (result);

  // if(bit ^ yy)
  // xx = bit ^ yy;
  if (AOP_TYPE (left) == AOP_CRY)
    {
      if (AOP_TYPE (right) == AOP_LIT)
        {
          // c = bit & literal;
          if (lit >> 1)
            {
              // lit>>1  != 0 => result = 1
              if (AOP_TYPE (result) == AOP_CRY)
                {
                  if (size)
                    emitcode ("setb", "%s", AOP (result)->aopu.aop_dir);
                  else if (ifx)
                    continueIfTrue (ifx);
                  goto release;
                }
              emitcode ("setb", "c");
            }
          else
            {
              // lit == (0 or 1)
              if (lit == 0)
                {
                  // lit == 0, result = left
                  if (size && sameRegs (AOP (result), AOP (left)))
                    goto release;
                  emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
                }
              else
                {
                  // lit == 1, result = not(left)
                  if (size && sameRegs (AOP (result), AOP (left)))
                    {
                      emitcode ("cpl", "%s", AOP (result)->aopu.aop_dir);
                      goto release;
                    }
                  else
                    {
                      emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
                      emitcode ("cpl", "c");
                    }
                }
            }
        }
      else
        {
          // right != literal
          symbol *tlbl = newiTempLabel (NULL);
          if (AOP_TYPE (right) == AOP_CRY)
            {
              // c = bit ^ bit;
              if (IS_SYMOP (left) && OP_SYMBOL (left) && OP_SYMBOL (left)->accuse)
                {// left already is in the carry
                  operand *tmp = right;
                  right = left;
                  left = tmp;
                }
              else
                {
                  emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
                }
            }
          else
            {
              // c = bit ^ val
              toCarry (right);
            }
          emitcode ("jnb", "%s,%05d$", AOP (left)->aopu.aop_dir, (tlbl->key + 100));
          emitcode ("cpl", "c");
          emitLabel (tlbl);
        }
      // bit = c
      // val = c
      if (size)
        outBitC (result);
      // if(bit ^ ...)
      else if ((AOP_TYPE (result) == AOP_CRY) && ifx)
        genIfxJump (ifx, "c", left, right, result);
      goto release;
    }

  /* if left is same as result */
  if (sameRegs (AOP (result), AOP (left)))
    {
      for (; size--; offset++)
        {
          if (AOP_TYPE (right) == AOP_LIT)
            {
              bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
              if (bytelit == 0)
                {
                  /* dummy read of volatile operand */
                  if (isOperandVolatile (left, FALSE))
                    MOVA (aopGet (left, offset, FALSE, FALSE));
                  else
                    continue;
                }
              else if (IS_AOP_PREG (left))
                {
                  MOVA (aopGet (left, offset, FALSE, TRUE));
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                {
                  emitcode ("xrl", "%s,%s",
                            aopGet (left, offset, FALSE, TRUE),
                            aopGet (right, offset, FALSE, FALSE));
                }
            }
          else
            {
              if (AOP_TYPE (left) == AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,b");
                  aopPut (result, "a", offset);
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                  aopPut (result, "a", offset);
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  if (IS_AOP_PREG (left))
                    {
                      emitcode ("xrl", "a,%s", aopGet (left, offset, FALSE, TRUE));
                      aopPut (result, "a", offset);
                    }
                  else
                    emitcode ("xrl", "%s,a", aopGet (left, offset, FALSE, TRUE));
                }
            }
        }
    }
  else
    {
      // left & result in different registers
      if (AOP_TYPE (result) == AOP_CRY)
        {
          // result = bit
          // if(size), result in bit
          // if(!size && ifx), conditional oper: if(left ^ right)
          symbol *tlbl = newiTempLabel (NULL);
          int sizer = max (AOP_SIZE (left), AOP_SIZE (right));

          if (size)
            emitcode ("setb", "c");
          while (sizer--)
            {
              if ((AOP_TYPE (right) == AOP_LIT) &&
                  (((lit >> (offset * 8)) & 0x0FFL) == 0x00L))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                }
              else if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("xrl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("xrl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,%s", aopGet (left, offset, FALSE, TRUE));
                }

              emitcode ("jnz", "%05d$", tlbl->key + 100);
              offset++;
            }
          if (size)
            {
              CLRC;
              emitLabel (tlbl);
              outBitC (result);
            }
          else if (ifx)
            jmpTrueOrFalse (ifx, tlbl, left, right, result);
        }
      else
        {
          for (; (size--); offset++)
            {
              // normal case
              // result = left ^ right
              if (AOP_TYPE (right) == AOP_LIT)
                {
                  bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL);
                  if (bytelit == 0)
                    {
                      aopPut (result,
                              aopGet (left, offset, FALSE, FALSE),
                              offset);
                      continue;
                    }
                }
              // faster than result <- left, xrl result,right
              // and better if result is SFR
              if ((AOP_TYPE(right)==AOP_REG  || IS_AOP_PREG(right) || AOP_TYPE(right)==AOP_DIR)
                  && AOP_TYPE(left)==AOP_ACC)
                {
                  if (offset)
                    emitcode("mov", "a,b");
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else if (AOP_TYPE(left)==AOP_ACC)
                {
                  if (!offset)
                    {
                      bool pushedB = pushB ();
                      emitcode("mov", "b,a");
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("xrl", "a,b");
                      popB (pushedB);
                    }
                  else
                    {
                      MOVA (aopGet (right, offset, FALSE, FALSE));
                      emitcode("xrl", "a,b");
                    }
                }
              else if (aopGetUsesAcc (left, offset) && aopGetUsesAcc (right, offset))
                {
                  MOVB (aopGet (left, offset, FALSE, FALSE));
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,b");
                }
              else if (aopGetUsesAcc (left, offset))
                {
                  MOVA (aopGet (left, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,%s", aopGet (right, offset, FALSE, FALSE));
                }
              else
                {
                  MOVA (aopGet (right, offset, FALSE, FALSE));
                  emitcode ("xrl", "a,%s", aopGet (left, offset, FALSE, TRUE));
                }
              aopPut (result, "a", offset);
            }
        }
    }

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
}

/*-----------------------------------------------------------------*/
/* genInline - write the inline code out                           */
/*-----------------------------------------------------------------*/
static void
genInline (iCode * ic)
{
  char *buffer, *bp, *bp1;
  bool inComment = FALSE;

  D (emitcode (";", "genInline"));

  _G.inLine += (!options.asmpeep);

  buffer = bp = bp1 = Safe_strdup (IC_INLINE (ic));

  /* emit each line as a code */
  while (*bp)
    {
      switch (*bp)
        {
        case ';':
          inComment = TRUE;
          ++bp;
          break;

        case '\n':
          inComment = FALSE;
          *bp++ = '\0';
          emitcode (bp1, "");
          bp1 = bp;
          break;

        default:
          /* Add \n for labels, not dirs such as c:\mydir */
          if (!inComment && (*bp == ':') && (isspace((unsigned char)bp[1])))
            {
              ++bp;
              *bp = '\0';
              ++bp;
              emitcode (bp1, "");
              bp1 = bp;
            }
          else
            ++bp;
          break;
        }
    }
  if (bp1 != bp)
    emitcode (bp1, "");

  Safe_free (buffer);

  _G.inLine -= (!options.asmpeep);
}

/*-----------------------------------------------------------------*/
/* genRRC - rotate right with carry                                */
/*-----------------------------------------------------------------*/
static void
genRRC (iCode * ic)
{
  operand *left, *result;
  int size, offset;
  char *l;

  D (emitcode (";", "genRRC"));

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* move it to the result */
  size = AOP_SIZE (result);
  offset = size - 1;
  if (size == 1) { /* special case for 1 byte */
      l = aopGet (left, offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("rr", "a");
      goto release;
  }
  /* no need to clear carry, bit7 will be written later */
  while (size--)
    {
      l = aopGet (left, offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("rrc", "a");
      if (AOP_SIZE (result) > 1)
        aopPut (result, "a", offset--);
    }
  /* now we need to put the carry into the
     highest order byte of the result */
  if (AOP_SIZE (result) > 1)
    {
      l = aopGet (result, AOP_SIZE (result) - 1, FALSE, FALSE);
      MOVA (l);
    }
  emitcode ("mov", "acc.7,c");
 release:
  aopPut (result, "a", AOP_SIZE (result) - 1);
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genRLC - generate code for rotate left with carry               */
/*-----------------------------------------------------------------*/
static void
genRLC (iCode * ic)
{
  operand *left, *result;
  int size, offset;
  char *l;

  D (emitcode (";", "genRLC"));

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* move it to the result */
  size = AOP_SIZE (result);
  offset = 0;
  if (size--)
    {
      l = aopGet (left, offset, FALSE, FALSE);
      MOVA (l);
      if (size == 0) { /* special case for 1 byte */
              emitcode("rl","a");
              goto release;
      }
      emitcode("rlc","a"); /* bit0 will be written later */
      if (AOP_SIZE (result) > 1)
        {
          aopPut (result, "a", offset++);
        }

      while (size--)
        {
          l = aopGet (left, offset, FALSE, FALSE);
          MOVA (l);
          emitcode ("rlc", "a");
          if (AOP_SIZE (result) > 1)
            aopPut (result, "a", offset++);
        }
    }
  /* now we need to put the carry into the
     highest order byte of the result */
  if (AOP_SIZE (result) > 1)
    {
      l = aopGet (result, 0, FALSE, FALSE);
      MOVA (l);
    }
  emitcode ("mov", "acc.0,c");
 release:
  aopPut (result, "a", 0);
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGetHbit - generates code get highest order bit               */
/*-----------------------------------------------------------------*/
static void
genGetHbit (iCode * ic)
{
  operand *left, *result;

  D (emitcode (";", "genGetHbit"));

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* get the highest order byte into a */
  MOVA (aopGet (left, AOP_SIZE (left) - 1, FALSE, FALSE));
  if (AOP_TYPE (result) == AOP_CRY)
    {
      emitcode ("rlc", "a");
      outBitC (result);
    }
  else
    {
      emitcode ("rl", "a");
      emitcode ("anl", "a,#0x01");
      outAcc (result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGetAbit - generates code get a single bit                    */
/*-----------------------------------------------------------------*/
static void
genGetAbit (iCode * ic)
{
  operand *left, *right, *result;
  int shCount;

  D (emitcode (";", "genGetAbit"));

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, FALSE);

  shCount = (int) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);

  /* get the needed byte into a */
  MOVA (aopGet (left, shCount / 8, FALSE, FALSE));
  shCount %= 8;
  if (AOP_TYPE (result) == AOP_CRY)
    {
      if ((shCount) == 7)
          emitcode ("rlc", "a");
      else if ((shCount) == 0)
          emitcode ("rrc", "a");
      else
          emitcode ("mov", "c,acc[%d]", shCount);
      outBitC (result);
    }
  else
    {
      switch (shCount)
        {
        case 2:
          emitcode ("rr", "a");
          //fallthrough
        case 1:
          emitcode ("rr", "a");
          //fallthrough
        case 0:
          emitcode ("anl", "a,#0x01");
          break;
        case 3:
        case 5:
          emitcode ("mov", "c,acc[%d]", shCount);
          emitcode ("clr", "a");
          emitcode ("rlc", "a");
          break;
        case 4:
          emitcode ("swap", "a");
          emitcode ("anl", "a,#0x01");
          break;
        case 6:
          emitcode ("rl", "a");
          //fallthrough
        case 7:
          emitcode ("rl", "a");
          emitcode ("anl", "a,#0x01");
          break;
        }
      outAcc (result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGetByte - generates code get a single byte                   */
/*-----------------------------------------------------------------*/
static void
genGetByte (iCode * ic)
{
  operand *left, *right, *result;
  int offset;

  D (emitcode (";", "genGetByte"));

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, FALSE);

  offset = (int)floatFromVal (AOP (right)->aopu.aop_lit) / 8;
  aopPut (result,
          aopGet (left, offset, FALSE, FALSE),
          0);

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGetWord - generates code get two bytes                       */
/*-----------------------------------------------------------------*/
static void
genGetWord (iCode * ic)
{
  operand *left, *right, *result;
  int offset;

  D (emitcode (";", "genGetWord"));

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (right, ic, FALSE);
  aopOp (result, ic, FALSE);

  offset = (int)floatFromVal (AOP (right)->aopu.aop_lit) / 8;
  aopPut (result,
          aopGet (left, offset, FALSE, FALSE),
          0);
  aopPut (result,
          aopGet (left, offset+1, FALSE, FALSE),
          1);

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genSwap - generates code to swap nibbles or bytes               */
/*-----------------------------------------------------------------*/
static void
genSwap (iCode * ic)
{
  operand *left, *result;

  D(emitcode (";", "genSwap"));

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  switch (AOP_SIZE (left))
    {
    case 1: /* swap nibbles in byte */
      MOVA (aopGet (left, 0, FALSE, FALSE));
      emitcode ("swap", "a");
      aopPut (result, "a", 0);
      break;
    case 2: /* swap bytes in word */
      if (AOP_TYPE(left) == AOP_REG && sameRegs(AOP(left), AOP(result)))
        {
          MOVA (aopGet (left, 0, FALSE, FALSE));
          aopPut (result, aopGet (left, 1, FALSE, FALSE), 0);
          aopPut (result, "a", 1);
        }
      else if (operandsEqu (left, result))
        {
          char * reg = "a";
          bool pushedB = FALSE, leftInB = FALSE;

          MOVA (aopGet (left, 0, FALSE, FALSE));
          if (aopGetUsesAcc(left, 1) || aopGetUsesAcc(result, 0))
            {
              pushedB = pushB ();
              emitcode ("mov", "b,a");
              reg = "b";
              leftInB = TRUE;
            }
          aopPut (result, aopGet (left, 1, FALSE, FALSE), 0);
          aopPut (result, reg, 1);

          if (leftInB)
            popB (pushedB);
        }
      else
        {
          aopPut (result, aopGet (left, 1, FALSE, FALSE), 0);
          aopPut (result, aopGet (left, 0, FALSE, FALSE), 1);
        }
      break;
    default:
      wassertl(FALSE, "unsupported SWAP operand size");
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* AccRol - rotate left accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void
AccRol (int shCount)
{
  shCount &= 0x0007;            // shCount : 0..7

  switch (shCount)
    {
    case 0:
      break;
    case 1:
      emitcode ("rl", "a");
      break;
    case 2:
      emitcode ("rl", "a");
      emitcode ("rl", "a");
      break;
    case 3:
      emitcode ("swap", "a");
      emitcode ("rr", "a");
      break;
    case 4:
      emitcode ("swap", "a");
      break;
    case 5:
      emitcode ("swap", "a");
      emitcode ("rl", "a");
      break;
    case 6:
      emitcode ("rr", "a");
      emitcode ("rr", "a");
      break;
    case 7:
      emitcode ("rr", "a");
      break;
    }
}

/*-----------------------------------------------------------------*/
/* AccLsh - left shift accumulator by known count                  */
/*-----------------------------------------------------------------*/
static void
AccLsh (int shCount)
{
  if (shCount != 0)
    {
      if (shCount == 1)
        emitcode ("add", "a,acc");
      else if (shCount == 2)
        {
          emitcode ("add", "a,acc");
          emitcode ("add", "a,acc");
        }
      else
        {
          /* rotate left accumulator */
          AccRol (shCount);
          /* and kill the lower order bits */
          emitcode ("anl", "a,#0x%02x", SLMask[shCount]);
        }
    }
}

/*-----------------------------------------------------------------*/
/* AccRsh - right shift accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void
AccRsh (int shCount)
{
  if (shCount != 0)
    {
      if (shCount == 1)
        {
          CLRC;
          emitcode ("rrc", "a");
        }
      else
        {
          /* rotate right accumulator */
          AccRol (8 - shCount);
          /* and kill the higher order bits */
          emitcode ("anl", "a,#0x%02x", SRMask[shCount]);
        }
    }
}

/*-----------------------------------------------------------------*/
/* AccSRsh - signed right shift accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void
AccSRsh (int shCount)
{
  symbol *tlbl;
  if (shCount != 0)
    {
      if (shCount == 1)
        {
          emitcode ("mov", "c,acc.7");
          emitcode ("rrc", "a");
        }
      else if (shCount == 2)
        {
          emitcode ("mov", "c,acc.7");
          emitcode ("rrc", "a");
          emitcode ("mov", "c,acc.7");
          emitcode ("rrc", "a");
        }
      else
        {
          tlbl = newiTempLabel (NULL);
          /* rotate right accumulator */
          AccRol (8 - shCount);
          /* and kill the higher order bits */
          emitcode ("anl", "a,#0x%02x", SRMask[shCount]);
          emitcode ("jnb", "acc.%d,%05d$", 7 - shCount, tlbl->key + 100);
          emitcode ("orl", "a,#0x%02x",
                    (unsigned char) ~SRMask[shCount]);
          emitLabel (tlbl);
        }
    }
}

/*-----------------------------------------------------------------*/
/* shiftR1Left2Result - shift right one byte from left to result   */
/*-----------------------------------------------------------------*/
static void
shiftR1Left2Result (operand * left, int offl,
                    operand * result, int offr,
                    int shCount, int sign)
{
  MOVA (aopGet (left, offl, FALSE, FALSE));
  /* shift right accumulator */
  if (sign)
    AccSRsh (shCount);
  else
    AccRsh (shCount);
  aopPut (result, "a", offr);
}

/*-----------------------------------------------------------------*/
/* shiftL1Left2Result - shift left one byte from left to result    */
/*-----------------------------------------------------------------*/
static void
shiftL1Left2Result (operand * left, int offl,
                    operand * result, int offr, int shCount)
{
  char *l;
  l = aopGet (left, offl, FALSE, FALSE);
  MOVA (l);
  /* shift left accumulator */
  AccLsh (shCount);
  aopPut (result, "a", offr);
}

/*-----------------------------------------------------------------*/
/* movLeft2Result - move byte from left to result                  */
/*-----------------------------------------------------------------*/
static void
movLeft2Result (operand * left, int offl,
                operand * result, int offr, int sign)
{
  char *l;
  if (!sameRegs (AOP (left), AOP (result)) || (offl != offr))
    {
      l = aopGet (left, offl, FALSE, FALSE);

      if (*l == '@' && (IS_AOP_PREG (result)))
        {
          emitcode ("mov", "a,%s", l);
          aopPut (result, "a", offr);
        }
      else
        {
          if (!sign)
            {
              aopPut (result, l, offr);
            }
          else
            {
              /* MSB sign in acc.7 ! */
              if (getDataSize (left) == offl + 1)
                {
                  MOVA (l);
                  aopPut (result, "a", offr);
                }
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* AccAXRrl1 - right rotate c->a:x->c by 1                         */
/*-----------------------------------------------------------------*/
static void
AccAXRrl1 (char *x)
{
  emitcode ("rrc", "a");
  emitcode ("xch", "a,%s", x);
  emitcode ("rrc", "a");
  emitcode ("xch", "a,%s", x);
}

/*-----------------------------------------------------------------*/
/* AccAXLrl1 - left rotate c<-a:x<-c by 1                          */
/*-----------------------------------------------------------------*/
static void
AccAXLrl1 (char *x)
{
  emitcode ("xch", "a,%s", x);
  emitcode ("rlc", "a");
  emitcode ("xch", "a,%s", x);
  emitcode ("rlc", "a");
}

/*-----------------------------------------------------------------*/
/* AccAXLsh1 - left shift a:x<-0 by 1                              */
/*-----------------------------------------------------------------*/
static void
AccAXLsh1 (char *x)
{
  emitcode ("xch", "a,%s", x);
  emitcode ("add", "a,acc");
  emitcode ("xch", "a,%s", x);
  emitcode ("rlc", "a");
}

/*-----------------------------------------------------------------*/
/* AccAXLsh - left shift a:x by known count (0..7)                 */
/*-----------------------------------------------------------------*/
static void
AccAXLsh (char *x, int shCount)
{
  switch (shCount)
    {
    case 0:
      break;
    case 1:
      AccAXLsh1 (x);
      break;
    case 2:
      AccAXLsh1 (x);
      AccAXLsh1 (x);
      break;
    case 3:
    case 4:
    case 5:                     // AAAAABBB:CCCCCDDD

      AccRol (shCount);         // BBBAAAAA:CCCCCDDD

      emitcode ("anl", "a,#0x%02x",
                SLMask[shCount]);       // BBB00000:CCCCCDDD

      emitcode ("xch", "a,%s", x);      // CCCCCDDD:BBB00000

      AccRol (shCount);         // DDDCCCCC:BBB00000

      emitcode ("xch", "a,%s", x);      // BBB00000:DDDCCCCC

      emitcode ("xrl", "a,%s", x);      // (BBB^DDD)CCCCC:DDDCCCCC

      emitcode ("xch", "a,%s", x);      // DDDCCCCC:(BBB^DDD)CCCCC

      emitcode ("anl", "a,#0x%02x",
                SLMask[shCount]);       // DDD00000:(BBB^DDD)CCCCC

      emitcode ("xch", "a,%s", x);      // (BBB^DDD)CCCCC:DDD00000

      emitcode ("xrl", "a,%s", x);      // BBBCCCCC:DDD00000

      break;
    case 6:                     // AAAAAABB:CCCCCCDD
      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000000BB:CCCCCCDD
      emitcode ("mov", "c,acc.0");      // c = B
      emitcode ("xch", "a,%s", x);      // CCCCCCDD:000000BB
#if 0 // REMOVE ME
      AccAXRrl1 (x);            // BCCCCCCD:D000000B
      AccAXRrl1 (x);            // BBCCCCCC:DD000000
#else
      emitcode("rrc","a");
      emitcode("xch","a,%s", x);
      emitcode("rrc","a");
      emitcode("mov","c,acc.0"); //<< get correct bit
      emitcode("xch","a,%s", x);

      emitcode("rrc","a");
      emitcode("xch","a,%s", x);
      emitcode("rrc","a");
      emitcode("xch","a,%s", x);
#endif
      break;
    case 7:                     // a:x <<= 7

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 0000000B:CCCCCCCD

      emitcode ("mov", "c,acc.0");      // c = B

      emitcode ("xch", "a,%s", x);      // CCCCCCCD:0000000B

      AccAXRrl1 (x);            // BCCCCCCC:D0000000

      break;
    default:
      break;
    }
}

/*-----------------------------------------------------------------*/
/* AccAXRsh - right shift a:x known count (0..7)                   */
/*-----------------------------------------------------------------*/
static void
AccAXRsh (char *x, int shCount)
{
  switch (shCount)
    {
    case 0:
      break;
    case 1:
      CLRC;
      AccAXRrl1 (x);            // 0->a:x

      break;
    case 2:
      CLRC;
      AccAXRrl1 (x);            // 0->a:x

      CLRC;
      AccAXRrl1 (x);            // 0->a:x

      break;
    case 3:
    case 4:
    case 5:                     // AAAAABBB:CCCCCDDD = a:x

      AccRol (8 - shCount);     // BBBAAAAA:DDDCCCCC

      emitcode ("xch", "a,%s", x);      // CCCCCDDD:BBBAAAAA

      AccRol (8 - shCount);     // DDDCCCCC:BBBAAAAA

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000CCCCC:BBBAAAAA

      emitcode ("xrl", "a,%s", x);      // BBB(CCCCC^AAAAA):BBBAAAAA

      emitcode ("xch", "a,%s", x);      // BBBAAAAA:BBB(CCCCC^AAAAA)

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000AAAAA:BBB(CCCCC^AAAAA)

      emitcode ("xch", "a,%s", x);      // BBB(CCCCC^AAAAA):000AAAAA

      emitcode ("xrl", "a,%s", x);      // BBBCCCCC:000AAAAA

      emitcode ("xch", "a,%s", x);      // 000AAAAA:BBBCCCCC

      break;
    case 6:                     // AABBBBBB:CCDDDDDD

      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);            // ABBBBBBC:CDDDDDDA

      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);            // BBBBBBCC:DDDDDDAA

      emitcode ("xch", "a,%s", x);      // DDDDDDAA:BBBBBBCC

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000000AA:BBBBBBCC

      break;
    case 7:                     // ABBBBBBB:CDDDDDDD

      emitcode ("mov", "c,acc.7");      // c = A

      AccAXLrl1 (x);            // BBBBBBBC:DDDDDDDA

      emitcode ("xch", "a,%s", x);      // DDDDDDDA:BBBBBBCC

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 0000000A:BBBBBBBC

      break;
    default:
      break;
    }
}

/*-----------------------------------------------------------------*/
/* AccAXRshS - right shift signed a:x known count (0..7)           */
/*-----------------------------------------------------------------*/
static void
AccAXRshS (char *x, int shCount)
{
  symbol *tlbl;
  switch (shCount)
    {
    case 0:
      break;
    case 1:
      emitcode ("mov", "c,acc.7");
      AccAXRrl1 (x);            // s->a:x

      break;
    case 2:
      emitcode ("mov", "c,acc.7");
      AccAXRrl1 (x);            // s->a:x

      emitcode ("mov", "c,acc.7");
      AccAXRrl1 (x);            // s->a:x

      break;
    case 3:
    case 4:
    case 5:                     // AAAAABBB:CCCCCDDD = a:x

      tlbl = newiTempLabel (NULL);
      AccRol (8 - shCount);     // BBBAAAAA:CCCCCDDD

      emitcode ("xch", "a,%s", x);      // CCCCCDDD:BBBAAAAA

      AccRol (8 - shCount);     // DDDCCCCC:BBBAAAAA

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000CCCCC:BBBAAAAA

      emitcode ("xrl", "a,%s", x);      // BBB(CCCCC^AAAAA):BBBAAAAA

      emitcode ("xch", "a,%s", x);      // BBBAAAAA:BBB(CCCCC^AAAAA)

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000AAAAA:BBB(CCCCC^AAAAA)

      emitcode ("xch", "a,%s", x);      // BBB(CCCCC^AAAAA):000AAAAA

      emitcode ("xrl", "a,%s", x);      // BBBCCCCC:000AAAAA

      emitcode ("xch", "a,%s", x);      // 000SAAAA:BBBCCCCC

      emitcode ("jnb", "acc.%d,%05d$", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#0x%02x",
                (unsigned char) ~SRMask[shCount]);      // 111AAAAA:BBBCCCCC

      emitLabel (tlbl);
      break;                    // SSSSAAAA:BBBCCCCC

    case 6:                     // AABBBBBB:CCDDDDDD

      tlbl = newiTempLabel (NULL);
      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);            // ABBBBBBC:CDDDDDDA

      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);            // BBBBBBCC:DDDDDDAA

      emitcode ("xch", "a,%s", x);      // DDDDDDAA:BBBBBBCC

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 000000AA:BBBBBBCC

      emitcode ("jnb", "acc.%d,%05d$", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#0x%02x",
                (unsigned char) ~SRMask[shCount]);      // 111111AA:BBBBBBCC

      emitLabel (tlbl);
      break;
    case 7:                     // ABBBBBBB:CDDDDDDD

      tlbl = newiTempLabel (NULL);
      emitcode ("mov", "c,acc.7");      // c = A

      AccAXLrl1 (x);            // BBBBBBBC:DDDDDDDA

      emitcode ("xch", "a,%s", x);      // DDDDDDDA:BBBBBBCC

      emitcode ("anl", "a,#0x%02x",
                SRMask[shCount]);       // 0000000A:BBBBBBBC

      emitcode ("jnb", "acc.%d,%05d$", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#0x%02x",
                (unsigned char) ~SRMask[shCount]);      // 1111111A:BBBBBBBC

      emitLabel (tlbl);
      break;
    default:
      break;
    }
}

/*-----------------------------------------------------------------*/
/* shiftL2Left2Result - shift left two bytes from left to result   */
/*-----------------------------------------------------------------*/
static void
shiftL2Left2Result (operand * left, int offl,
                    operand * result, int offr, int shCount)
{
  char * x;
  bool pushedB = FALSE;
  bool usedB = FALSE;

  if (sameRegs (AOP (result), AOP (left)) &&
      ((offl + MSB16) == offr))
    {
      /* don't crash result[offr] */
      MOVA (aopGet (left, offl, FALSE, FALSE));
      x = xch_a_aopGet (left, offl + MSB16, FALSE, FALSE);
      usedB = !strncmp(x, "b", 1);
    }
  else if (aopGetUsesAcc (result, offr))
    {
      movLeft2Result (left, offl, result, offr, 0);
      pushedB = pushB ();
      usedB = TRUE;
      emitcode ("mov", "b,%s", aopGet (left, offl + MSB16, FALSE, FALSE));
      MOVA (aopGet (result, offr, FALSE, FALSE));
      emitcode ("xch", "a,b");
      x = "b";
    }
  else
    {
      movLeft2Result (left, offl, result, offr, 0);
      MOVA (aopGet (left, offl + MSB16, FALSE, FALSE));
      x = aopGet (result, offr, FALSE, FALSE);
    }
  /* ax << shCount (x = lsb(result)) */
  AccAXLsh (x, shCount);
  if (usedB)
    {
      emitcode ("xch", "a,b");
      aopPut (result, "a", offr);
      aopPut (result, "b", offr + MSB16);
      popB (pushedB);
    }
  else
    {
      aopPut (result, "a", offr + MSB16);
    }
}


/*-----------------------------------------------------------------*/
/* shiftR2Left2Result - shift right two bytes from left to result  */
/*-----------------------------------------------------------------*/
static void
shiftR2Left2Result (operand * left, int offl,
                    operand * result, int offr,
                    int shCount, int sign)
{
  char * x;
  bool pushedB = FALSE;
  bool usedB = FALSE;

  if (sameRegs (AOP (result), AOP (left)) &&
      ((offl + MSB16) == offr))
    {
      /* don't crash result[offr] */
      MOVA (aopGet (left, offl, FALSE, FALSE));
      x = xch_a_aopGet (left, offl + MSB16, FALSE, FALSE);
      usedB = !strncmp(x, "b", 1);
    }
  else if (aopGetUsesAcc (result, offr))
    {
      movLeft2Result (left, offl, result, offr, 0);
      pushedB = pushB ();
      usedB = TRUE;
      emitcode ("mov", "b,%s", aopGet (result, offr, FALSE, FALSE));
      MOVA (aopGet (left, offl + MSB16, FALSE, FALSE));
      x = "b";
    }
  else
    {
      movLeft2Result (left, offl, result, offr, 0);
      MOVA (aopGet (left, offl + MSB16, FALSE, FALSE));
      x = aopGet (result, offr, FALSE, FALSE);
    }
  /* a:x >> shCount (x = lsb(result)) */
  if (sign)
    AccAXRshS (x, shCount);
  else
    AccAXRsh (x, shCount);
  if (usedB)
    {
      emitcode ("xch", "a,b");
      aopPut (result, "a", offr);
      emitcode ("xch", "a,b");
      popB (pushedB);
    }
  if (getDataSize (result) > 1)
    aopPut (result, "a", offr + MSB16);
}

/*-----------------------------------------------------------------*/
/* shiftLLeftOrResult - shift left one byte from left, or to result */
/*-----------------------------------------------------------------*/
static void
shiftLLeftOrResult (operand * left, int offl,
                    operand * result, int offr, int shCount)
{
  MOVA (aopGet (left, offl, FALSE, FALSE));
  /* shift left accumulator */
  AccLsh (shCount);
  /* or with result */
  if (aopGetUsesAcc (result, offr))
    {
      emitcode ("xch", "a,b");
      MOVA (aopGet (result, offr, FALSE, FALSE));
      emitcode ("orl", "a,b");
    }
  else
    {
      emitcode ("orl", "a,%s", aopGet (result, offr, FALSE, FALSE));
    }
  /* back to result */
  aopPut (result, "a", offr);
}

/*-----------------------------------------------------------------*/
/* shiftRLeftOrResult - shift right one byte from left,or to result */
/*-----------------------------------------------------------------*/
static void
shiftRLeftOrResult (operand * left, int offl,
                    operand * result, int offr, int shCount)
{
  MOVA (aopGet (left, offl, FALSE, FALSE));
  /* shift right accumulator */
  AccRsh (shCount);
  /* or with result */
  if (aopGetUsesAcc(result, offr))
    {
      emitcode ("xch", "a,b");
      MOVA (aopGet (result, offr, FALSE, FALSE));
      emitcode ("orl", "a,b");
    }
  else
    {
      emitcode ("orl", "a,%s", aopGet (result, offr, FALSE, FALSE));
    }
  /* back to result */
  aopPut (result, "a", offr);
}

/*-----------------------------------------------------------------*/
/* genlshOne - left shift a one byte quantity by known count       */
/*-----------------------------------------------------------------*/
static void
genlshOne (operand * result, operand * left, int shCount)
{
  D (emitcode (";", "genlshOne"));

  shiftL1Left2Result (left, LSB, result, LSB, shCount);
}

/*-----------------------------------------------------------------*/
/* genlshTwo - left shift two bytes by known amount != 0           */
/*-----------------------------------------------------------------*/
static void
genlshTwo (operand * result, operand * left, int shCount)
{
  int size;

  D (emitcode (";", "genlshTwo"));

  size = getDataSize (result);

  /* if shCount >= 8 */
  if (shCount >= 8)
    {
      shCount -= 8;

      if (size > 1)
        {
          if (shCount)
            shiftL1Left2Result (left, LSB, result, MSB16, shCount);
          else
            movLeft2Result (left, LSB, result, MSB16, 0);
        }
      aopPut (result, zero, LSB);
    }

  /*  1 <= shCount <= 7 */
  else
    {
      if (size == 1)
        shiftL1Left2Result (left, LSB, result, LSB, shCount);
      else
        shiftL2Left2Result (left, LSB, result, LSB, shCount);
    }
}

/*-----------------------------------------------------------------*/
/* shiftLLong - shift left one long from left to result            */
/* offl = LSB or MSB16                                             */
/*-----------------------------------------------------------------*/
static void
shiftLLong (operand * left, operand * result, int offr)
{
  char *l;
  int size = AOP_SIZE (result);

  if (size >= LSB + offr)
    {
      l = aopGet (left, LSB, FALSE, FALSE);
      MOVA (l);
      emitcode ("add", "a,acc");
      if (sameRegs (AOP (left), AOP (result)) &&
          size >= MSB16 + offr && offr != LSB)
        xch_a_aopGet (left, LSB + offr, FALSE, FALSE);
      else
        aopPut (result, "a", LSB + offr);
    }

  if (size >= MSB16 + offr)
    {
      if (!(sameRegs (AOP (result), AOP (left)) && size >= MSB16 + offr && offr != LSB))
        {
          l = aopGet (left, MSB16, FALSE, FALSE);
          MOVA (l);
        }
      emitcode ("rlc", "a");
      if (sameRegs (AOP (left), AOP (result)) &&
          size >= MSB24 + offr && offr != LSB)
        xch_a_aopGet (left, MSB16 + offr, FALSE, FALSE);
      else
        aopPut (result, "a", MSB16 + offr);
    }

  if (size >= MSB24 + offr)
    {
      if (!(sameRegs (AOP (result), AOP (left)) && size >= MSB24 + offr && offr != LSB))
        {
          l = aopGet (left, MSB24, FALSE, FALSE);
          MOVA (l);
        }
      emitcode ("rlc", "a");
      if (sameRegs (AOP (left), AOP (result)) &&
          size >= MSB32 + offr && offr != LSB)
        xch_a_aopGet (left, MSB24 + offr, FALSE, FALSE);
      else
        aopPut (result, "a", MSB24 + offr);
    }

  if (size > MSB32 + offr)
    {
      if (!(sameRegs (AOP (result), AOP (left)) && size >= MSB32 + offr && offr != LSB))
        {
          l = aopGet (left, MSB32, FALSE, FALSE);
          MOVA (l);
        }
      emitcode ("rlc", "a");
      aopPut (result, "a", MSB32 + offr);
    }
  if (offr != LSB)
    aopPut (result, zero, LSB);
}

/*-----------------------------------------------------------------*/
/* genlshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genlshFour (operand * result, operand * left, int shCount)
{
  int size;

  D (emitcode (";", "genlshFour"));

  size = AOP_SIZE (result);

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
      aopPut (result, zero, LSB);
      aopPut (result, zero, MSB16);
      aopPut (result, zero, MSB24);
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
      aopPut (result, zero, MSB16);
      aopPut (result, zero, LSB);
      return;
    }

  /* if more than 1 byte */
  else if (shCount >= 8)
    {
      /* lower order three bytes goes to higher order  three bytes */
      shCount -= 8;
      if (size == 2)
        {
          if (shCount)
            shiftL1Left2Result (left, LSB, result, MSB16, shCount);
          else
            movLeft2Result (left, LSB, result, MSB16, 0);
        }
      else
        {                       /* size = 4 */
          if (shCount == 0)
            {
              movLeft2Result (left, MSB24, result, MSB32, 0);
              movLeft2Result (left, MSB16, result, MSB24, 0);
              movLeft2Result (left, LSB, result, MSB16, 0);
              aopPut (result, zero, LSB);
            }
          else if (shCount == 1)
            shiftLLong (left, result, MSB16);
          else
            {
              shiftL2Left2Result (left, MSB16, result, MSB24, shCount);
              shiftL1Left2Result (left, LSB, result, MSB16, shCount);
              shiftRLeftOrResult (left, LSB, result, MSB24, 8 - shCount);
              aopPut (result, zero, LSB);
            }
        }
    }

  /* 1 <= shCount <= 7 */
  else if (shCount <= 2)
    {
      shiftLLong (left, result, LSB);
      if (shCount == 2)
        shiftLLong (result, result, LSB);
    }
  /* 3 <= shCount <= 7, optimize */
  else
    {
      shiftL2Left2Result (left, MSB24, result, MSB24, shCount);
      shiftRLeftOrResult (left, MSB16, result, MSB24, 8 - shCount);
      shiftL2Left2Result (left, LSB, result, LSB, shCount);
    }
}

/*-----------------------------------------------------------------*/
/* genLeftShiftLiteral - left shifting by known count              */
/*-----------------------------------------------------------------*/
static void
genLeftShiftLiteral (operand * left,
                     operand * right,
                     operand * result,
                     iCode * ic)
{
  int shCount = (int) floatFromVal (AOP (right)->aopu.aop_lit);
  int size;

  D (emitcode (";", "genLeftShiftLiteral"));

  freeAsmop (right, NULL, ic, TRUE);

  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  size = getSize (operandType (result));

#if VIEW_SIZE
  emitcode ("; shift left ", "result %d, left %d", size,
            AOP_SIZE (left));
#endif

  /* I suppose that the left size >= result size */
  if (shCount == 0)
    {
      while (size--)
        {
          movLeft2Result (left, size, result, size, 0);
        }
    }
  else if (shCount >= (size * 8))
    {
      while (size--)
        {
          aopPut (result, zero, size);
        }
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
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
                  "*** ack! mystery literal shift!\n");
          break;
        }
    }
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genLeftShift - generates code for left shifting                 */
/*-----------------------------------------------------------------*/
static void
genLeftShift (iCode * ic)
{
  operand *left, *right, *result;
  int size, offset;
  char *l;
  symbol *tlbl, *tlbl1;
  bool pushedB;

  D (emitcode (";", "genLeftShift"));

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, FALSE);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      genLeftShiftLiteral (left, right, result, ic);
      return;
    }

  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  pushedB = pushB ();
  MOVB (aopGet (right, 0, FALSE, FALSE));
  emitcode ("inc", "b");
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* now move the left to the result if they are not the same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {

      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          l = aopGet (left, offset, FALSE, TRUE);
          if (*l == '@' && (IS_AOP_PREG (result)))
            {

              emitcode ("mov", "a,%s", l);
              aopPut (result, "a", offset);
            }
          else
            aopPut (result, l, offset);
          offset++;
        }
    }

  tlbl = newiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = 0;
  tlbl1 = newiTempLabel (NULL);

  /* if it is only one byte then */
  if (size == 1)
    {
      symbol *tlbl1 = newiTempLabel (NULL);

      l = aopGet (left, 0, FALSE, FALSE);
      MOVA (l);
      emitcode ("sjmp", "%05d$", tlbl1->key + 100);
      emitLabel (tlbl);
      emitcode ("add", "a,acc");
      emitLabel (tlbl1);
      emitcode ("djnz", "b,%05d$", tlbl->key + 100);
      popB (pushedB);
      aopPut (result, "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));

  emitcode ("sjmp", "%05d$", tlbl1->key + 100);
  emitLabel (tlbl);
  l = aopGet (result, offset, FALSE, FALSE);
  MOVA (l);
  emitcode ("add", "a,acc");
  aopPut (result, "a", offset++);
  while (--size)
    {
      l = aopGet (result, offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("rlc", "a");
      aopPut (result, "a", offset++);
    }
  reAdjustPreg (AOP (result));

  emitLabel (tlbl1);
  emitcode ("djnz", "b,%05d$", tlbl->key + 100);
  popB (pushedB);
release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genrshOne - right shift a one byte quantity by known count      */
/*-----------------------------------------------------------------*/
static void
genrshOne (operand * result, operand * left,
           int shCount, int sign)
{
  D (emitcode (";", "genrshOne"));

  shiftR1Left2Result (left, LSB, result, LSB, shCount, sign);
}

/*-----------------------------------------------------------------*/
/* genrshTwo - right shift two bytes by known amount != 0          */
/*-----------------------------------------------------------------*/
static void
genrshTwo (operand * result, operand * left,
           int shCount, int sign)
{
  D (emitcode (";", "genrshTwo"));

  /* if shCount >= 8 */
  if (shCount >= 8)
    {
      shCount -= 8;
      if (shCount)
        shiftR1Left2Result (left, MSB16, result, LSB, shCount, sign);
      else
        movLeft2Result (left, MSB16, result, LSB, sign);
      addSign (result, MSB16, sign);
    }

  /*  1 <= shCount <= 7 */
  else
    shiftR2Left2Result (left, LSB, result, LSB, shCount, sign);
}

/*-----------------------------------------------------------------*/
/* shiftRLong - shift right one long from left to result           */
/* offl = LSB or MSB16                                             */
/*-----------------------------------------------------------------*/
static void
shiftRLong (operand * left, int offl,
            operand * result, int sign)
{
  bool overlapping = regsInCommon (left, result) || operandsEqu(left, result);

  if (overlapping && offl>1)
    {
      // we are in big trouble, but this shouldn't happen
      werror(E_INTERNAL_ERROR, __FILE__, __LINE__);
    }

  MOVA (aopGet (left, MSB32, FALSE, FALSE));

  if (offl==MSB16)
    {
      // shift is > 8
      if (sign)
        {
          emitcode ("rlc", "a");
          emitcode ("subb", "a,acc");
          if (overlapping && sameByte (AOP (left), MSB32, AOP (result), MSB32))
            {
              xch_a_aopGet (left, MSB32, FALSE, FALSE);
            }
          else
            {
              aopPut (result, "a", MSB32);
              MOVA (aopGet (left, MSB32, FALSE, FALSE));
            }
        }
      else
        {
          if (aopPutUsesAcc (result, zero, MSB32))
            {
              emitcode("xch", "a,b");
              aopPut (result, zero, MSB32);
              emitcode("xch", "a,b");
            }
          else
            {
              aopPut (result, zero, MSB32);
            }
        }
    }

  if (!sign)
    {
      emitcode ("clr", "c");
    }
  else
    {
      emitcode ("mov", "c,acc.7");
    }

  emitcode ("rrc", "a");

  if (overlapping && offl==MSB16 &&
      sameByte (AOP (left), MSB24, AOP (result), MSB32-offl))
    {
      xch_a_aopGet (left, MSB24, FALSE, FALSE);
    }
  else
    {
      aopPut (result, "a", MSB32 - offl);
      MOVA (aopGet (left, MSB24, FALSE, FALSE));
    }

  emitcode ("rrc", "a");
  if (overlapping && offl==MSB16 &&
      sameByte (AOP (left), MSB16, AOP (result), MSB24-offl))
    {
      xch_a_aopGet (left, MSB16, FALSE, FALSE);
    }
  else
    {
      aopPut (result, "a", MSB24 - offl);
      MOVA (aopGet (left, MSB16, FALSE, FALSE));
    }

  emitcode ("rrc", "a");
  if (offl != LSB)
    {
      aopPut (result, "a", MSB16 - offl);
    }
  else
    {
      if (overlapping &&
          sameByte (AOP (left), LSB, AOP (result), MSB16-offl))
        {
          xch_a_aopGet (left, LSB, FALSE, FALSE);
        }
      else
        {
          aopPut (result, "a", MSB16 - offl);
          MOVA (aopGet (left, LSB, FALSE, FALSE));
        }
      emitcode ("rrc", "a");
      aopPut (result, "a", LSB);
    }
}

/*-----------------------------------------------------------------*/
/* genrshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genrshFour (operand * result, operand * left,
            int shCount, int sign)
{
  D (emitcode (";", "genrshFour"));

  /* if shifting more that 3 bytes */
  if (shCount >= 24)
    {
      shCount -= 24;
      if (shCount)
        shiftR1Left2Result (left, MSB32, result, LSB, shCount, sign);
      else
        movLeft2Result (left, MSB32, result, LSB, sign);
      addSign (result, MSB16, sign);
    }
  else if (shCount >= 16)
    {
      shCount -= 16;
      if (shCount)
        shiftR2Left2Result (left, MSB24, result, LSB, shCount, sign);
      else
        {
          movLeft2Result (left, MSB24, result, LSB, 0);
          movLeft2Result (left, MSB32, result, MSB16, sign);
        }
      addSign (result, MSB24, sign);
    }
  else if (shCount >= 8)
    {
      shCount -= 8;
      if (shCount == 1)
        {
          shiftRLong (left, MSB16, result, sign);
        }
      else if (shCount == 0)
        {
          movLeft2Result (left, MSB16, result, LSB, 0);
          movLeft2Result (left, MSB24, result, MSB16, 0);
          movLeft2Result (left, MSB32, result, MSB24, sign);
          addSign (result, MSB32, sign);
        }
      else
        {
          shiftR2Left2Result (left, MSB16, result, LSB, shCount, 0);
          shiftLLeftOrResult (left, MSB32, result, MSB16, 8 - shCount);
          /* the last shift is signed */
          shiftR1Left2Result (left, MSB32, result, MSB24, shCount, sign);
          addSign (result, MSB32, sign);
        }
    }
  else
    {
      /* 1 <= shCount <= 7 */
      if (shCount <= 2)
        {
          shiftRLong (left, LSB, result, sign);
          if (shCount == 2)
            shiftRLong (result, LSB, result, sign);
        }
      else
        {
          shiftR2Left2Result (left, LSB, result, LSB, shCount, 0);
          shiftLLeftOrResult (left, MSB24, result, MSB16, 8 - shCount);
          shiftR2Left2Result (left, MSB24, result, MSB24, shCount, sign);
        }
    }
}

/*-----------------------------------------------------------------*/
/* genRightShiftLiteral - right shifting by known count            */
/*-----------------------------------------------------------------*/
static void
genRightShiftLiteral (operand * left,
                      operand * right,
                      operand * result,
                      iCode * ic,
                      int sign)
{
  int shCount = (int) floatFromVal (AOP (right)->aopu.aop_lit);
  int size;

  D (emitcode (";", "genRightShiftLiteral"));

  freeAsmop (right, NULL, ic, TRUE);

  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

#if VIEW_SIZE
  emitcode ("; shift right ", "result %d, left %d", AOP_SIZE (result),
            AOP_SIZE (left));
#endif

  size = getDataSize (left);
  /* test the LEFT size !!! */

  /* I suppose that the left size >= result size */
  if (shCount == 0)
    {
      size = getDataSize (result);
      while (size--)
        movLeft2Result (left, size, result, size, 0);
    }

  else if (shCount >= (size * 8))
    {
      if (sign)
        {
          /* get sign in acc.7 */
          MOVA (aopGet (left, size - 1, FALSE, FALSE));
        }
      addSign (result, LSB, sign);
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
          break;
        }
    }
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genSignedRightShift - right shift of signed number              */
/*-----------------------------------------------------------------*/
static void
genSignedRightShift (iCode * ic)
{
  operand *right, *left, *result;
  int size, offset;
  char *l;
  symbol *tlbl, *tlbl1;
  bool pushedB;

  D (emitcode (";", "genSignedRightShift"));

  /* we do it the hard way put the shift count in b
     and loop thru preserving the sign */

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, FALSE);


  if (AOP_TYPE (right) == AOP_LIT)
    {
      genRightShiftLiteral (left, right, result, ic, 1);
      return;
    }
  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  pushedB = pushB ();
  MOVB (aopGet (right, 0, FALSE, FALSE));
  emitcode ("inc", "b");
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* now move the left to the result if they are not the
     same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {

      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          l = aopGet (left, offset, FALSE, TRUE);
          if (*l == '@' && IS_AOP_PREG (result))
            {

              emitcode ("mov", "a,%s", l);
              aopPut (result, "a", offset);
            }
          else
            aopPut (result, l, offset);
          offset++;
        }
    }

  /* mov the highest order bit to OVR */
  tlbl = newiTempLabel (NULL);
  tlbl1 = newiTempLabel (NULL);

  size = AOP_SIZE (result);
  offset = size - 1;
  MOVA (aopGet (left, offset, FALSE, FALSE));
  emitcode ("rlc", "a");
  emitcode ("mov", "ov,c");
  /* if it is only one byte then */
  if (size == 1)
    {
      l = aopGet (left, 0, FALSE, FALSE);
      MOVA (l);
      emitcode ("sjmp", "%05d$", tlbl1->key + 100);
      emitLabel (tlbl);
      emitcode ("mov", "c,ov");
      emitcode ("rrc", "a");
      emitLabel (tlbl1);
      emitcode ("djnz", "b,%05d$", tlbl->key + 100);
      popB (pushedB);
      aopPut (result, "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));
  emitcode ("sjmp", "%05d$", tlbl1->key + 100);
  emitLabel (tlbl);
  emitcode ("mov", "c,ov");
  while (size--)
    {
      l = aopGet (result, offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("rrc", "a");
      aopPut (result, "a", offset--);
    }
  reAdjustPreg (AOP (result));
  emitLabel (tlbl1);
  emitcode ("djnz", "b,%05d$", tlbl->key + 100);
  popB (pushedB);

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genRightShift - generate code for right shifting                */
/*-----------------------------------------------------------------*/
static void
genRightShift (iCode * ic)
{
  operand *right, *left, *result;
  sym_link *letype;
  int size, offset;
  char *l;
  symbol *tlbl, *tlbl1;
  bool pushedB;

  D (emitcode (";", "genRightShift"));

  /* if signed then we do it the hard way preserve the
     sign bit moving it inwards */
  letype = getSpec (operandType (IC_LEFT (ic)));

  if (!SPEC_USIGN (letype))
    {
      genSignedRightShift (ic);
      return;
    }

  /* signed & unsigned types are treated the same : i.e. the
     signed is NOT propagated inwards : quoting from the
     ANSI - standard : "for E1 >> E2, is equivalent to division
     by 2**E2 if unsigned or if it has a non-negative value,
     otherwise the result is implementation defined ", MY definition
     is that the sign does not get propagated */

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, FALSE);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      genRightShiftLiteral (left, right, result, ic, 0);
      return;
    }

  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  pushedB = pushB ();
  MOVB (aopGet (right, 0, FALSE, FALSE));
  emitcode ("inc", "b");
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* now move the left to the result if they are not the
     same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          l = aopGet (left, offset, FALSE, TRUE);
          if (*l == '@' && IS_AOP_PREG (result))
            {

              emitcode ("mov", "a,%s", l);
              aopPut (result, "a", offset);
            }
          else
            aopPut (result, l, offset);
          offset++;
        }
    }

  tlbl = newiTempLabel (NULL);
  tlbl1 = newiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = size - 1;

  /* if it is only one byte then */
  if (size == 1)
    {
      l = aopGet (left, 0, FALSE, FALSE);
      MOVA (l);
      emitcode ("sjmp", "%05d$", tlbl1->key + 100);
      emitLabel (tlbl);
      CLRC;
      emitcode ("rrc", "a");
      emitLabel (tlbl1);
      emitcode ("djnz", "b,%05d$", tlbl->key + 100);
      popB (pushedB);
      aopPut (result, "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));
  emitcode ("sjmp", "%05d$", tlbl1->key + 100);
  emitLabel (tlbl);
  CLRC;
  while (size--)
    {
      l = aopGet (result, offset, FALSE, FALSE);
      MOVA (l);
      emitcode ("rrc", "a");
      aopPut (result, "a", offset--);
    }
  reAdjustPreg (AOP (result));

  emitLabel (tlbl1);
  emitcode ("djnz", "b,%05d$", tlbl->key + 100);
  popB (pushedB);

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* emitPtrByteGet - emits code to get a byte into A through a      */
/*                  pointer register (R0, R1, or DPTR). The        */
/*                  original value of A can be preserved in B.     */
/*-----------------------------------------------------------------*/
static void
emitPtrByteGet (char *rname, int p_type, bool preserveAinB)
{
  switch (p_type)
    {
    case IPOINTER:
    case POINTER:
      if (preserveAinB)
        emitcode ("mov", "b,a");
      emitcode ("mov", "a,@%s", rname);
      break;

    case PPOINTER:
      if (preserveAinB)
        emitcode ("mov", "b,a");
      emitcode ("movx", "a,@%s", rname);
      break;

    case FPOINTER:
      if (preserveAinB)
        emitcode ("mov", "b,a");
      emitcode ("movx", "a,@dptr");
      break;

    case CPOINTER:
      if (preserveAinB)
        emitcode ("mov", "b,a");
      emitcode ("clr", "a");
      emitcode ("movc", "a,@a+dptr");
      break;

    case GPOINTER:
      if (preserveAinB)
        {
          emitcode ("push", "b");
          emitcode ("push", "acc");
        }
      emitcode ("lcall", "__gptrget");
      if (preserveAinB)
        emitcode ("pop", "b");
      break;
    }
}

/*-----------------------------------------------------------------*/
/* emitPtrByteSet - emits code to set a byte from src through a    */
/*                  pointer register (R0, R1, or DPTR).            */
/*-----------------------------------------------------------------*/
static void
emitPtrByteSet (char *rname, int p_type, char *src)
{
  switch (p_type)
    {
    case IPOINTER:
    case POINTER:
      if (*src=='@')
        {
          MOVA (src);
          emitcode ("mov", "@%s,a", rname);
        }
      else
        emitcode ("mov", "@%s,%s", rname, src);
      break;

    case PPOINTER:
      MOVA (src);
      emitcode ("movx", "@%s,a", rname);
      break;

    case FPOINTER:
      MOVA (src);
      emitcode ("movx", "@dptr,a");
      break;

    case GPOINTER:
      MOVA (src);
      emitcode ("lcall", "__gptrput");
      break;
    }
}

/*-----------------------------------------------------------------*/
/* genUnpackBits - generates code for unpacking bits               */
/*-----------------------------------------------------------------*/
static void
genUnpackBits (operand * result, char *rname, int ptype, iCode *ifx)
{
  int offset = 0;       /* result byte offset */
  int rsize;            /* result size */
  int rlen = 0;         /* remaining bitfield length */
  sym_link *etype;      /* bitfield type information */
  int blen;             /* bitfield length */
  int bstr;             /* bitfield starting bit within byte */
  char buffer[10];

  D(emitcode (";", "genUnpackBits"));

  etype = getSpec (operandType (result));
  rsize = getSize (operandType (result));
  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  if (ifx && blen <= 8)
    {
      emitPtrByteGet (rname, ptype, FALSE);
      if (blen == 1)
        {
          SNPRINTF (buffer, sizeof(buffer),
                    "acc.%d", bstr);
          genIfxJump (ifx, buffer, NULL, NULL, NULL);
        }
      else
        {
          if (blen < 8)
            emitcode ("anl", "a,#0x%02x",
                      (((unsigned char) -1) >> (8 - blen)) << bstr);
          genIfxJump (ifx, "a", NULL, NULL, NULL);
        }
      return;
    }
  wassert (!ifx);

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      emitPtrByteGet (rname, ptype, FALSE);
      AccRol (8 - bstr);
      emitcode ("anl", "a,#0x%02x", ((unsigned char) -1) >> (8 - blen));
      if (!SPEC_USIGN (etype))
        {
          /* signed bitfield */
          symbol *tlbl = newiTempLabel (NULL);

          emitcode ("jnb", "acc.%d,%05d$", blen - 1, tlbl->key + 100);
          emitcode ("orl", "a,#0x%02x", (unsigned char) (0xff << blen));
          emitLabel (tlbl);
        }
      aopPut (result, "a", offset++);
      goto finish;
    }

  /* Bit field did not fit in a byte. Copy all
     but the partial byte at the end.  */
  for (rlen=blen;rlen>=8;rlen-=8)
    {
      emitPtrByteGet (rname, ptype, FALSE);
      aopPut (result, "a", offset++);
      if (rlen>8)
        emitcode ("inc", "%s", rname);
    }

  /* Handle the partial byte at the end */
  if (rlen)
    {
      emitPtrByteGet (rname, ptype, FALSE);
      emitcode ("anl", "a,#0x%02x", ((unsigned char) -1) >> (8-rlen));
      if (!SPEC_USIGN (etype))
        {
          /* signed bitfield */
          symbol *tlbl = newiTempLabel (NULL);

          emitcode ("jnb", "acc.%d,%05d$", rlen - 1, tlbl->key + 100);
          emitcode ("orl", "a,#0x%02x", (unsigned char) (0xff << rlen));
          emitLabel (tlbl);
        }
      aopPut (result, "a", offset++);
    }

finish:
  if (offset < rsize)
    {
      char *source;

      if (SPEC_USIGN (etype))
        source = zero;
      else
        {
          /* signed bitfield: sign extension with 0x00 or 0xff */
          emitcode ("rlc", "a");
          emitcode ("subb", "a,acc");

          source = "a";
        }
      rsize -= offset;
      while (rsize--)
        aopPut (result, source, offset++);
    }
}


/*-----------------------------------------------------------------*/
/* genDataPointerGet - generates code when ptr offset is known     */
/*-----------------------------------------------------------------*/
static void
genDataPointerGet (operand * left,
                   operand * result,
                   iCode * ic)
{
  char *l;
  char buffer[256];
  int size, offset = 0;

  D (emitcode (";", "genDataPointerGet"));

  aopOp (result, ic, TRUE);

  /* get the string representation of the name */
  l = aopGet (left, 0, FALSE, TRUE);
  l++; // remove #
  size = AOP_SIZE (result);
  while (size--)
    {
      if (offset)
        {
          SNPRINTF (buffer, sizeof(buffer), "(%s + %d)", l, offset);
        }
      else
        {
          SNPRINTF (buffer, sizeof(buffer), "%s", l);
        }
      aopPut (result, buffer, offset++);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genNearPointerGet - emitcode for near pointer fetch             */
/*-----------------------------------------------------------------*/
static void
genNearPointerGet (operand * left,
                   operand * result,
                   iCode * ic,
                   iCode * pi,
                   iCode * ifx)
{
  asmop *aop = NULL;
  regs *preg = NULL;
  char *rname;
  sym_link *rtype, *retype;
  sym_link *ltype = operandType (left);
  char buffer[80];

  D (emitcode (";", "genNearPointerGet"));

  rtype = operandType (result);
  retype = getSpec (rtype);

  aopOp (left, ic, FALSE);

  /* if left is rematerialisable and
     result is not bitfield variable type and
     the left is pointer to data space i.e
     lower 128 bytes of space */
  if (AOP_TYPE (left) == AOP_IMMD &&
      !IS_BITFIELD (retype) &&
      DCL_TYPE (ltype) == POINTER)
    {
      genDataPointerGet (left, result, ic);
      return;
    }

 /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (left)))
    {
      if (IS_AOP_PREG (left))
        {
          // Aha, it is a pointer, just in disguise.
          rname = aopGet (left, 0, FALSE, FALSE);
          if (*rname != '@')
            {
              fprintf(stderr, "probable internal error: unexpected rname @ %s:%d\n",
                      __FILE__, __LINE__);
            }
          else
            {
              // Expected case.
              emitcode ("mov", "a%s,%s", rname + 1, rname);
              rname++;  // skip the '@'.
            }
        }
      else
        {
          /* otherwise get a free pointer register */
          aop = newAsmop (0);
          preg = getFreePtr (ic, &aop, FALSE);
          emitcode ("mov", "%s,%s",
                    preg->name,
                    aopGet (left, 0, FALSE, TRUE));
          rname = preg->name;
        }
    }
  else
    rname = aopGet (left, 0, FALSE, FALSE);

  //aopOp (result, ic, FALSE);
  aopOp (result, ic, result?TRUE:FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITFIELD (retype))
    genUnpackBits (result, rname, POINTER, ifx);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (result);
      int offset = 0;

      while (size--)
        {
          if (ifx || IS_AOP_PREG (result) || AOP_TYPE (result) == AOP_STK)
            {

              emitcode ("mov", "a,@%s", rname);
              if (!ifx)
                aopPut (result, "a", offset);
            }
          else
            {
              SNPRINTF (buffer, sizeof(buffer), "@%s", rname);
              aopPut (result, buffer, offset);
            }
          offset++;
          if (size || pi)
            emitcode ("inc", "%s", rname);
        }
    }

  /* now some housekeeping stuff */
  if (aop)       /* we had to allocate for this iCode */
    {
      if (pi) { /* post increment present */
        aopPut (left, rname, 0);
      }
      freeAsmop (NULL, aop, ic, RESULTONSTACK (ic) ? FALSE : TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if ((AOP_SIZE (result) > 1 &&
           !OP_SYMBOL (left)->remat &&
           (OP_SYMBOL (left)->liveTo > ic->seq ||
            ic->depth)) &&
          !pi)
        {
          int size = AOP_SIZE (result) - 1;
          while (size--)
            emitcode ("dec", "%s", rname);
        }
    }

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a", left, NULL, result);
    }

  /* done */
  freeAsmop (result, NULL, ic, RESULTONSTACK (ic) ? FALSE : TRUE);
  freeAsmop (left, NULL, ic, TRUE);
  if (pi) pi->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genPagedPointerGet - emitcode for paged pointer fetch           */
/*-----------------------------------------------------------------*/
static void
genPagedPointerGet (operand * left,
                    operand * result,
                    iCode * ic,
                    iCode *pi,
                    iCode *ifx)
{
  asmop *aop = NULL;
  regs *preg = NULL;
  char *rname;
  sym_link *rtype, *retype;

  D (emitcode (";", "genPagedPointerGet"));

  rtype = operandType (result);
  retype = getSpec (rtype);

  aopOp (left, ic, FALSE);

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (left)))
    {
      /* otherwise get a free pointer register */
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
                preg->name,
                aopGet (left, 0, FALSE, TRUE));
      rname = preg->name;
    }
  else
    rname = aopGet (left, 0, FALSE, FALSE);

  aopOp (result, ic, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITFIELD (retype))
    genUnpackBits (result, rname, PPOINTER, ifx);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (result);
      int offset = 0;

      while (size--)
        {

          emitcode ("movx", "a,@%s", rname);
          if (!ifx)
            aopPut (result, "a", offset);

          offset++;

          if (size || pi)
            emitcode ("inc", "%s", rname);
        }
    }

  /* now some housekeeping stuff */
  if (aop) /* we had to allocate for this iCode */
    {
      if (pi)
        aopPut (left, rname, 0);
      freeAsmop (NULL, aop, ic, TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if ((AOP_SIZE (result) > 1 &&
           !OP_SYMBOL (left)->remat &&
           (OP_SYMBOL (left)->liveTo > ic->seq ||
            ic->depth)) &&
          !pi)
        {
          int size = AOP_SIZE (result) - 1;
          while (size--)
            emitcode ("dec", "%s", rname);
        }
    }

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a", left, NULL, result);
    }

  /* done */
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
  if (pi) pi->generated = 1;
}

/*--------------------------------------------------------------------*/
/* loadDptrFromOperand - load dptr (and optionally B) from operand op */
/*--------------------------------------------------------------------*/
static void
loadDptrFromOperand (operand *op, bool loadBToo)
{
  if (AOP_TYPE (op) != AOP_STR)
    {
      /* if this is rematerializable */
      if (AOP_TYPE (op) == AOP_IMMD)
        {
          emitcode ("mov", "dptr,%s", aopGet (op, 0, TRUE, FALSE));
          if (loadBToo)
            {
              if (AOP(op)->aopu.aop_immd.from_cast_remat)
                emitcode ("mov", "b,%s",aopGet (op, AOP_SIZE(op)-1, FALSE, FALSE));
              else
                {
                  wassertl(FALSE, "need pointerCode");
                  emitcode (";", "mov b,???");
                  /* genPointerGet and genPointerSet originally did different
                  ** things for this case. Both seem wrong.
                  ** from genPointerGet:
                  **  emitcode ("mov", "b,#%d", pointerCode (retype));
                  ** from genPointerSet:
                  **  emitcode ("mov", "b,%s + 1", aopGet (result, 0, TRUE, FALSE));
                  */
                }
            }
        }
      else if (AOP_TYPE (op) == AOP_DPTR)
        {
          if (loadBToo)
            {
              MOVA (aopGet (op, 0, FALSE, FALSE));
              emitcode ("push", "acc");
              MOVA (aopGet (op, 1, FALSE, FALSE));
              emitcode ("push", "acc");
              emitcode ("mov", "b,%s", aopGet (op, 2, FALSE, FALSE));
              emitcode ("pop", "dph");
              emitcode ("pop", "dpl");
            }
          else
            {
              MOVA (aopGet (op, 0, FALSE, FALSE));
              emitcode ("push", "acc");
              emitcode ("mov", "dph,%s", aopGet (op, 1, FALSE, FALSE));
              emitcode ("pop", "dpl");
            }
        }
      else
        {                       /* we need to get it byte by byte */
          emitcode ("mov", "dpl,%s", aopGet (op, 0, FALSE, FALSE));
          emitcode ("mov", "dph,%s", aopGet (op, 1, FALSE, FALSE));
          if (loadBToo)
            emitcode ("mov", "b,%s", aopGet (op, 2, FALSE, FALSE));
        }
    }
}

/*-----------------------------------------------------------------*/
/* genFarPointerGet - get value from far space                     */
/*-----------------------------------------------------------------*/
static void
genFarPointerGet (operand * left,
                  operand * result, iCode * ic, iCode * pi, iCode * ifx)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (result));

  D (emitcode (";", "genFarPointerGet"));

  aopOp (left, ic, FALSE);
  loadDptrFromOperand (left, FALSE);

  /* so dptr now contains the address */
  aopOp (result, ic, FALSE);

  /* if bit then unpack */
  if (IS_BITFIELD (retype))
    genUnpackBits (result, "dptr", FPOINTER, ifx);
  else
    {
      size = AOP_SIZE (result);
      offset = 0;

      while (size--)
        {
          emitcode ("movx", "a,@dptr");
          if (!ifx)
            aopPut (result, "a", offset++);
          if (size || pi)
            emitcode ("inc", "dptr");
        }
    }

  if (pi && AOP_TYPE (left) != AOP_IMMD && AOP_TYPE (left) != AOP_STR)
    {
      aopPut (left, "dpl", 0);
      aopPut (left, "dph", 1);
      pi->generated = 1;
    }

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a", left, NULL, result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genCodePointerGet - get value from code space                   */
/*-----------------------------------------------------------------*/
static void
genCodePointerGet (operand * left,
                    operand * result, iCode * ic, iCode *pi, iCode *ifx)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (result));

  D (emitcode (";", "genCodePointerGet"));

  aopOp (left, ic, FALSE);
  loadDptrFromOperand (left, FALSE);

  /* so dptr now contains the address */
  aopOp (result, ic, FALSE);

  /* if bit then unpack */
  if (IS_BITFIELD (retype))
    genUnpackBits (result, "dptr", CPOINTER, ifx);
  else
    {
      size = AOP_SIZE (result);
      offset = 0;

      while (size--)
        {
          emitcode ("clr", "a");
          emitcode ("movc", "a,@a+dptr");
          if (!ifx)
            aopPut (result, "a", offset++);
          if (size || pi)
            emitcode ("inc", "dptr");
        }
    }

  if (pi && AOP_TYPE (left) != AOP_IMMD && AOP_TYPE (left) != AOP_STR)
    {
      aopPut (left, "dpl", 0);
      aopPut (left, "dph", 1);
      pi->generated = 1;
    }

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a", left, NULL, result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGenPointerGet - get value from generic pointer space         */
/*-----------------------------------------------------------------*/
static void
genGenPointerGet (operand * left,
                  operand * result, iCode * ic, iCode *pi, iCode *ifx)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (result));

  D (emitcode (";", "genGenPointerGet"));

  aopOp (left, ic, FALSE);
  loadDptrFromOperand (left, TRUE);

  /* so dptr now contains the address */
  aopOp (result, ic, FALSE);

  /* if bit then unpack */
  if (IS_BITFIELD (retype))
    {
      genUnpackBits (result, "dptr", GPOINTER, ifx);
    }
  else
    {
      size = AOP_SIZE (result);
      offset = 0;

      while (size--)
        {
          emitcode ("lcall", "__gptrget");
          if (!ifx)
            aopPut (result, "a", offset++);
          if (size || pi)
            emitcode ("inc", "dptr");
        }
    }

  if (pi && AOP_TYPE (left) != AOP_IMMD && AOP_TYPE (left) != AOP_STR)
    {
      aopPut (left, "dpl", 0);
      aopPut (left, "dph", 1);
      pi->generated = 1;
    }

  if (ifx && !ifx->generated)
    {
      genIfxJump (ifx, "a", left, NULL, result);
    }

  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (left, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void
genPointerGet (iCode * ic, iCode *pi, iCode *ifx)
{
  operand *left, *result;
  sym_link *type, *etype;
  int p_type;

  D (emitcode (";", "genPointerGet"));

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  if (getSize (operandType (result))>1)
    ifx = NULL;

  /* depending on the type of pointer we need to
     move it to the correct pointer register */
  type = operandType (left);
  etype = getSpec (type);
  /* if left is of type of pointer then it is simple */
  if (IS_PTR (type) && !IS_FUNC (type->next))
    p_type = DCL_TYPE (type);
  else
    {
      /* we have to go by the storage class */
      p_type = PTR_TYPE (SPEC_OCLS (etype));
    }

  /* special case when cast remat */
  if (p_type == GPOINTER && OP_SYMBOL(left)->remat &&
      IS_CAST_ICODE(OP_SYMBOL(left)->rematiCode))
    {
      left = IC_RIGHT(OP_SYMBOL(left)->rematiCode);
      type = operandType (left);
      p_type = DCL_TYPE (type);
    }
  /* now that we have the pointer type we assign
     the pointer values */
  switch (p_type)
    {

    case POINTER:
    case IPOINTER:
      genNearPointerGet (left, result, ic, pi, ifx);
      break;

    case PPOINTER:
      genPagedPointerGet (left, result, ic, pi, ifx);
      break;

    case FPOINTER:
      genFarPointerGet (left, result, ic, pi, ifx);
      break;

    case CPOINTER:
      genCodePointerGet (left, result, ic, pi, ifx);
      break;

    case GPOINTER:
      genGenPointerGet (left, result, ic, pi, ifx);
      break;
    }
}


/*-----------------------------------------------------------------*/
/* genPackBits - generates code for packed bit storage             */
/*-----------------------------------------------------------------*/
static void
genPackBits (sym_link * etype,
             operand * right,
             char *rname, int p_type)
{
  int offset = 0;       /* source byte offset */
  int rlen = 0;         /* remaining bitfield length */
  int blen;             /* bitfield length */
  int bstr;             /* bitfield starting bit within byte */
  int litval;           /* source literal value (if AOP_LIT) */
  unsigned char mask;   /* bitmask within current byte */

  D(emitcode (";", "genPackBits"));

  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  /* If the bitfield length is less than a byte */
  if (blen < 8)
    {
      mask = ((unsigned char) (0xFF << (blen + bstr)) |
              (unsigned char) (0xFF >> (8 - bstr)));

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with a bitfield length <8 and literal source
          */
          litval = (int) floatFromVal (AOP (right)->aopu.aop_lit);
          litval <<= bstr;
          litval &= (~mask) & 0xff;
          emitPtrByteGet (rname, p_type, FALSE);
          if ((mask|litval)!=0xff)
            emitcode ("anl","a,#0x%02x", mask);
          if (litval)
            emitcode ("orl","a,#0x%02x", litval);
        }
      else
        {
          if ((blen==1) && (p_type!=GPOINTER))
            {
              /* Case with a bitfield length == 1 and no generic pointer
              */
              if (AOP_TYPE (right) == AOP_CRY)
                emitcode ("mov", "c,%s", AOP(right)->aopu.aop_dir);
              else
                {
                  MOVA (aopGet (right, 0, FALSE, FALSE));
                  emitcode ("rrc","a");
                }
              emitPtrByteGet (rname, p_type, FALSE);
              emitcode ("mov","acc.%d,c",bstr);
            }
          else
            {
              bool pushedB;
              /* Case with a bitfield length < 8 and arbitrary source
              */
              MOVA (aopGet (right, 0, FALSE, FALSE));
              /* shift and mask source value */
              AccLsh (bstr);
              emitcode ("anl", "a,#0x%02x", (~mask) & 0xff);

              pushedB = pushB ();
              /* transfer A to B and get next byte */
              emitPtrByteGet (rname, p_type, TRUE);

              emitcode ("anl", "a,#0x%02x", mask);
              emitcode ("orl", "a,b");
              if (p_type == GPOINTER)
                emitcode ("pop", "b");

              popB (pushedB);
           }
        }

      emitPtrByteSet (rname, p_type, "a");
      return;
    }

  /* Bit length is greater than 7 bits. In this case, copy  */
  /* all except the partial byte at the end                 */
  for (rlen=blen;rlen>=8;rlen-=8)
    {
      emitPtrByteSet (rname, p_type,
                      aopGet (right, offset++, FALSE, TRUE) );
      if (rlen>8)
        emitcode ("inc", "%s", rname);
    }

  /* If there was a partial byte at the end */
  if (rlen)
    {
      mask = (((unsigned char) -1 << rlen) & 0xff);

      if (AOP_TYPE (right) == AOP_LIT)
        {
          /* Case with partial byte and literal source
          */
          litval = (int) floatFromVal (AOP (right)->aopu.aop_lit);
          litval >>= (blen-rlen);
          litval &= (~mask) & 0xff;
          emitPtrByteGet (rname, p_type, FALSE);
          if ((mask|litval)!=0xff)
            emitcode ("anl","a,#0x%02x", mask);
          if (litval)
            emitcode ("orl","a,#0x%02x", litval);
        }
      else
        {
          bool pushedB;
          /* Case with partial byte and arbitrary source
          */
          MOVA (aopGet (right, offset++, FALSE, FALSE));
          emitcode ("anl", "a,#0x%02x", (~mask) & 0xff);

          pushedB = pushB ();
          /* transfer A to B and get next byte */
          emitPtrByteGet (rname, p_type, TRUE);

          emitcode ("anl", "a,#0x%02x", mask);
          emitcode ("orl", "a,b");
          if (p_type == GPOINTER)
            emitcode ("pop", "b");

          popB (pushedB);
        }
      emitPtrByteSet (rname, p_type, "a");
    }
}


/*-----------------------------------------------------------------*/
/* genDataPointerSet - remat pointer to data space                 */
/*-----------------------------------------------------------------*/
static void
genDataPointerSet (operand * right,
                   operand * result,
                   iCode * ic)
{
  int size, offset = 0;
  char *l, buffer[256];

  D (emitcode (";", "genDataPointerSet"));

  aopOp (right, ic, FALSE);

  l = aopGet (result, 0, FALSE, TRUE);
  l++; //remove #
  size = AOP_SIZE (result);
  while (size--)
    {
      if (offset)
        SNPRINTF (buffer, sizeof(buffer), "(%s + %d)", l, offset);
      else
        SNPRINTF (buffer, sizeof(buffer), "%s", l);
      emitcode ("mov", "%s,%s", buffer,
                aopGet (right, offset++, FALSE, FALSE));
    }

  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genNearPointerSet - emitcode for near pointer put                */
/*-----------------------------------------------------------------*/
static void
genNearPointerSet (operand * right,
                   operand * result,
                   iCode * ic,
                   iCode * pi)
{
  asmop *aop = NULL;
  regs *preg = NULL;
  char *rname, *l;
  sym_link *retype, *letype;
  sym_link *ptype = operandType (result);

  D (emitcode (";", "genNearPointerSet"));

  retype = getSpec (operandType (right));
  letype = getSpec (ptype);

  aopOp (result, ic, FALSE);

  /* if the result is rematerializable &
     in data space & not a bit variable */
  if (AOP_TYPE (result) == AOP_IMMD &&
      DCL_TYPE (ptype) == POINTER &&
      !IS_BITVAR (retype) &&
      !IS_BITVAR (letype))
    {
      genDataPointerSet (right, result, ic);
      return;
    }

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (result)))
    {
        if (
            //AOP_TYPE (result) == AOP_STK
            IS_AOP_PREG(result)
            )
        {
            // Aha, it is a pointer, just in disguise.
            rname = aopGet (result, 0, FALSE, FALSE);
            if (*rname != '@')
            {
                fprintf(stderr, "probable internal error: unexpected rname @ %s:%d\n",
                        __FILE__, __LINE__);
            }
            else
            {
                // Expected case.
                emitcode ("mov", "a%s,%s", rname + 1, rname);
                rname++;  // skip the '@'.
            }
        }
        else
        {
            /* otherwise get a free pointer register */
            aop = newAsmop (0);
            preg = getFreePtr (ic, &aop, FALSE);
            emitcode ("mov", "%s,%s",
                      preg->name,
                      aopGet (result, 0, FALSE, TRUE));
            rname = preg->name;
        }
    }
    else
    {
        rname = aopGet (result, 0, FALSE, FALSE);
    }

  aopOp (right, ic, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITFIELD (retype) || IS_BITFIELD (letype))
    genPackBits ((IS_BITFIELD (retype) ? retype : letype), right, rname, POINTER);
  else
    {
      /* we can just get the values */
      int size = AOP_SIZE (right);
      int offset = 0;

      while (size--)
        {
          l = aopGet (right, offset, FALSE, TRUE);
          if ((*l == '@') || (strcmp (l, "acc") == 0))
            {
              MOVA (l);
              emitcode ("mov", "@%s,a", rname);
            }
          else
            emitcode ("mov", "@%s,%s", rname, l);
          if (size || pi)
            emitcode ("inc", "%s", rname);
          offset++;
        }
    }

  /* now some housekeeping stuff */
  if (aop) /* we had to allocate for this iCode */
    {
      if (pi)
        aopPut (result, rname, 0);
      freeAsmop (NULL, aop, ic, TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if ((AOP_SIZE (right) > 1 &&
           !OP_SYMBOL (result)->remat &&
           (OP_SYMBOL (result)->liveTo > ic->seq ||
            ic->depth)) &&
          !pi)
        {
          int size = AOP_SIZE (right) - 1;
          while (size--)
            emitcode ("dec", "%s", rname);
        }
    }

  /* done */
  if (pi)
    pi->generated = 1;
  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genPagedPointerSet - emitcode for Paged pointer put             */
/*-----------------------------------------------------------------*/
static void
genPagedPointerSet (operand * right,
                    operand * result,
                    iCode * ic,
                    iCode * pi)
{
  asmop *aop = NULL;
  regs *preg = NULL;
  char *rname, *l;
  sym_link *retype, *letype;

  D (emitcode (";", "genPagedPointerSet"));

  retype = getSpec (operandType (right));
  letype = getSpec (operandType (result));

  aopOp (result, ic, FALSE);

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (result)))
    {
      /* otherwise get a free pointer register */
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
                preg->name,
                aopGet (result, 0, FALSE, TRUE));
      rname = preg->name;
    }
  else
    rname = aopGet (result, 0, FALSE, FALSE);

  aopOp (right, ic, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITFIELD (retype) || IS_BITFIELD (letype))
    genPackBits ((IS_BITFIELD (retype) ? retype : letype), right, rname, PPOINTER);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (right);
      int offset = 0;

      while (size--)
        {
          l = aopGet (right, offset, FALSE, TRUE);
          MOVA (l);
          emitcode ("movx", "@%s,a", rname);

          if (size || pi)
            emitcode ("inc", "%s", rname);

          offset++;
        }
    }

  /* now some housekeeping stuff */
  if (aop) /* we had to allocate for this iCode */
    {
      if (pi)
        aopPut (result, rname, 0);
      freeAsmop (NULL, aop, ic, TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if (AOP_SIZE (right) > 1 &&
          !OP_SYMBOL (result)->remat &&
          (OP_SYMBOL (result)->liveTo > ic->seq ||
           ic->depth))
        {
          int size = AOP_SIZE (right) - 1;
          while (size--)
            emitcode ("dec", "%s", rname);
        }
    }

  /* done */
  if (pi) pi->generated = 1;
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genFarPointerSet - set value from far space                     */
/*-----------------------------------------------------------------*/
static void
genFarPointerSet (operand * right,
                  operand * result, iCode * ic, iCode * pi)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (right));
  sym_link *letype = getSpec (operandType (result));

  D(emitcode (";", "genFarPointerSet"));

  aopOp (result, ic, FALSE);
  loadDptrFromOperand (result, FALSE);

  /* so dptr now contains the address */
  aopOp (right, ic, FALSE);

  /* if bit then unpack */
  if (IS_BITFIELD (retype) || IS_BITFIELD (letype))
    genPackBits ((IS_BITFIELD (retype) ? retype : letype), right, "dptr", FPOINTER);
  else
    {
      size = AOP_SIZE (right);
      offset = 0;

      while (size--)
        {
          char *l = aopGet (right, offset++, FALSE, FALSE);
          MOVA (l);
          emitcode ("movx", "@dptr,a");
          if (size || pi)
            emitcode ("inc", "dptr");
        }
    }
  if (pi && AOP_TYPE (result) != AOP_STR && AOP_TYPE (result) != AOP_IMMD) {
    aopPut (result, "dpl", 0);
    aopPut (result, "dph", 1);
    pi->generated=1;
  }
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGenPointerSet - set value from generic pointer space         */
/*-----------------------------------------------------------------*/
static void
genGenPointerSet (operand * right,
                  operand * result, iCode * ic, iCode * pi)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (right));
  sym_link *letype = getSpec (operandType (result));

  D (emitcode (";", "genGenPointerSet"));

  aopOp (result, ic, FALSE);
  loadDptrFromOperand (result, TRUE);

  /* so dptr now contains the address */
  aopOp (right, ic, FALSE);

  /* if bit then unpack */
  if (IS_BITFIELD (retype) || IS_BITFIELD (letype))
    {
      genPackBits ((IS_BITFIELD (retype) ? retype : letype), right, "dptr", GPOINTER);
    }
  else
    {
      size = AOP_SIZE (right);
      offset = 0;

      while (size--)
        {
          char *l = aopGet (right, offset++, FALSE, FALSE);
          MOVA (l);
          emitcode ("lcall", "__gptrput");
          if (size || pi)
            emitcode ("inc", "dptr");
        }
    }

  if (pi && AOP_TYPE (result) != AOP_STR && AOP_TYPE (result) != AOP_IMMD) {
    aopPut (result, "dpl", 0);
    aopPut (result, "dph", 1);
    pi->generated=1;
  }
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void
genPointerSet (iCode * ic, iCode *pi)
{
  operand *right, *result;
  sym_link *type, *etype;
  int p_type;

  D (emitcode (";", "genPointerSet"));

  right = IC_RIGHT (ic);
  result = IC_RESULT (ic);

  /* depending on the type of pointer we need to
     move it to the correct pointer register */
  type = operandType (result);
  etype = getSpec (type);
  /* if left is of type of pointer then it is simple */
  if (IS_PTR (type) && !IS_FUNC (type->next))
    {
      p_type = DCL_TYPE (type);
    }
  else
    {
      /* we have to go by the storage class */
      p_type = PTR_TYPE (SPEC_OCLS (etype));
    }

  /* special case when cast remat */
  if (p_type == GPOINTER && OP_SYMBOL(result)->remat &&
      IS_CAST_ICODE(OP_SYMBOL(result)->rematiCode)) {
          result = IC_RIGHT(OP_SYMBOL(result)->rematiCode);
          type = operandType (result);
          p_type = DCL_TYPE (type);
  }

  /* now that we have the pointer type we assign
     the pointer values */
  switch (p_type)
    {

    case POINTER:
    case IPOINTER:
      genNearPointerSet (right, result, ic, pi);
      break;

    case PPOINTER:
      genPagedPointerSet (right, result, ic, pi);
      break;

    case FPOINTER:
      genFarPointerSet (right, result, ic, pi);
      break;

    case GPOINTER:
      genGenPointerSet (right, result, ic, pi);
      break;

    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "genPointerSet: illegal pointer type");
    }
}

/*-----------------------------------------------------------------*/
/* genIfx - generate code for Ifx statement                        */
/*-----------------------------------------------------------------*/
static void
genIfx (iCode * ic, iCode * popIc)
{
  operand *cond = IC_COND (ic);
  int isbit = 0;
  char *dup = NULL;

  D (emitcode (";", "genIfx"));

  aopOp (cond, ic, FALSE);

  /* get the value into acc */
  if (AOP_TYPE (cond) != AOP_CRY)
    {
      toBoolean (cond);
    }
  else
    {
      isbit = 1;
      if (AOP(cond)->aopu.aop_dir)
        dup = Safe_strdup(AOP(cond)->aopu.aop_dir);
    }

  /* the result is now in the accumulator or a directly addressable bit */
  freeAsmop (cond, NULL, ic, TRUE);

  /* if there was something to be popped then do it */
  if (popIc)
    genIpop (popIc);

  /* if the condition is a bit variable */
  if (isbit && dup)
    genIfxJump(ic, dup, NULL, NULL, NULL);
  else if (isbit && IS_ITEMP (cond) && SPIL_LOC (cond))
    genIfxJump (ic, SPIL_LOC (cond)->rname, NULL, NULL, NULL);
  else if (isbit && !IS_ITEMP (cond))
    genIfxJump (ic, OP_SYMBOL (cond)->rname, NULL, NULL, NULL);
  else
    genIfxJump (ic, "a", NULL, NULL, NULL);

  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genAddrOf - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void
genAddrOf (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));
  int size, offset;

  D (emitcode (";", "genAddrOf"));

  aopOp (IC_RESULT (ic), ic, FALSE);

  /* if the operand is on the stack then we
     need to get the stack offset of this
     variable */
  if (sym->onStack)
    {
      /* if it has an offset then we need to compute it */
      if (sym->stack)
        {
          int stack_offset = ((sym->stack < 0) ?
                              ((char) (sym->stack - _G.nRegsSaved)) :
                              ((char) sym->stack)) & 0xff;
          if ((abs(stack_offset) == 1) &&
              !AOP_NEEDSACC(IC_RESULT (ic)) &&
              !isOperandVolatile (IC_RESULT (ic), FALSE))
            {
              aopPut (IC_RESULT (ic), SYM_BP (sym), 0);
              if (stack_offset > 0)
                emitcode ("inc", "%s", aopGet (IC_RESULT (ic), LSB, FALSE, FALSE));
              else
                emitcode ("dec", "%s", aopGet (IC_RESULT (ic), LSB, FALSE, FALSE));
            }
          else
            {
              emitcode ("mov", "a,%s", SYM_BP (sym));
              emitcode ("add", "a,#0x%02x", stack_offset & 0xff);
              aopPut (IC_RESULT (ic), "a", 0);
            }
        }
      else
        {
          /* we can just move _bp */
          aopPut (IC_RESULT (ic), SYM_BP (sym), 0);
        }
      /* fill the result with zero */
      size = AOP_SIZE (IC_RESULT (ic)) - 1;

      offset = 1;
      while (size--)
        {
          aopPut (IC_RESULT (ic), zero, offset++);
        }
      goto release;
    }

  /* object not on stack then we need the name */
  size = AOP_SIZE (IC_RESULT (ic));
  offset = 0;

  while (size--)
    {
      char s[SDCC_NAME_MAX];
      if (offset)
        sprintf (s, "#(%s >> %d)",
                 sym->rname,
                 offset * 8);
      else
        SNPRINTF (s, sizeof(s), "#%s", sym->rname);
      aopPut (IC_RESULT (ic), s, offset++);
    }

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);

}

/*-----------------------------------------------------------------*/
/* genFarFarAssign - assignment when both are in far space         */
/*-----------------------------------------------------------------*/
static void
genFarFarAssign (operand * result, operand * right, iCode * ic)
{
  int size = AOP_SIZE (right);
  int offset = 0;
  char *l;

  D (emitcode (";", "genFarFarAssign"));

  /* first push the right side on to the stack */
  while (size--)
    {
      l = aopGet (right, offset++, FALSE, FALSE);
      MOVA (l);
      emitcode ("push", "acc");
    }

  freeAsmop (right, NULL, ic, FALSE);
  /* now assign DPTR to result */
  aopOp (result, ic, FALSE);
  size = AOP_SIZE (result);
  while (size--)
    {
      emitcode ("pop", "acc");
      aopPut (result, "a", --offset);
    }
  freeAsmop (result, NULL, ic, FALSE);
}

/*-----------------------------------------------------------------*/
/* genAssign - generate code for assignment                        */
/*-----------------------------------------------------------------*/
static void
genAssign (iCode * ic)
{
  operand *result, *right;
  int size, offset;
  unsigned long lit = 0L;

  D (emitcode (";", "genAssign"));

  result = IC_RESULT (ic);
  right = IC_RIGHT (ic);

  /* if they are the same */
  if (operandsEqu (result, right) &&
      !isOperandVolatile (result, FALSE) &&
      !isOperandVolatile (right, FALSE))
    return;

  aopOp (right, ic, FALSE);

  /* special case both in far space */
  if (AOP_TYPE (right) == AOP_DPTR &&
      IS_TRUE_SYMOP (result) &&
      isOperandInFarSpace (result))
    {
      genFarFarAssign (result, right, ic);
      return;
    }

  aopOp (result, ic, TRUE);

  /* if they are the same registers */
  if (sameRegs (AOP (right), AOP (result)) &&
      !isOperandVolatile (result, FALSE) &&
      !isOperandVolatile (right, FALSE))
    goto release;

  /* if the result is a bit */
  if (AOP_TYPE (result) == AOP_CRY)
    {
      assignBit (result, right);
      goto release;
    }

  /* bit variables done */
  /* general case */
  size = AOP_SIZE (result);
  offset = 0;
  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  if ((size > 1) &&
      (AOP_TYPE (result) != AOP_REG) &&
      (AOP_TYPE (right) == AOP_LIT) &&
      !IS_FLOAT (operandType (right)) &&
      (lit < 256L))
    {
      while ((size) && (lit))
        {
          aopPut (result,
                  aopGet (right, offset, FALSE, FALSE),
                  offset);
          lit >>= 8;
          offset++;
          size--;
        }
      /* And now fill the rest with zeros. */
      if (size)
        {
          emitcode ("clr", "a");
        }
      while (size--)
        {
          aopPut (result, "a", offset);
          offset++;
        }
    }
  else
    {
      while (size--)
        {
          aopPut (result,
                  aopGet (right, offset, FALSE, FALSE),
                  offset);
          offset++;
        }
    }

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genJumpTab - generates code for jump table                      */
/*-----------------------------------------------------------------*/
static void
genJumpTab (iCode * ic)
{
  symbol *jtab,*jtablo,*jtabhi;
  char *l;
  unsigned int count;

  D (emitcode (";", "genJumpTab"));

  count = elementsInSet( IC_JTLABELS (ic) );

  if( count <= 16 )
    {
      /* this algorithm needs 9 cycles and 7 + 3*n bytes
         if the switch argument is in a register.
         (8 cycles and 6+2*n bytes if peepholes can change ljmp to sjmp) */
      /* Peephole may not convert ljmp to sjmp or ret
         labelIsReturnOnly & labelInRange must check
         currPl->ic->op != JUMPTABLE */
      aopOp (IC_JTCOND (ic), ic, FALSE);
      /* get the condition into accumulator */
      l = aopGet (IC_JTCOND (ic), 0, FALSE, FALSE);
      MOVA (l);
      /* multiply by three */
      if (aopGetUsesAcc (IC_JTCOND (ic), 0))
        {
          emitcode ("mov", "b,#3");
          emitcode ("mul", "ab");
        }
      else
        {
          emitcode ("add", "a,acc");
          emitcode ("add", "a,%s", aopGet (IC_JTCOND (ic), 0, FALSE, FALSE));
        }
      freeAsmop (IC_JTCOND (ic), NULL, ic, TRUE);

      jtab = newiTempLabel (NULL);
      emitcode ("mov", "dptr,#%05d$", jtab->key + 100);
      emitcode ("jmp", "@a+dptr");
      emitLabel (jtab);
      /* now generate the jump labels */
      for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab;
           jtab = setNextItem (IC_JTLABELS (ic)))
        emitcode ("ljmp", "%05d$", jtab->key + 100);
    }
  else
    {
      /* this algorithm needs 14 cycles and 13 + 2*n bytes
         if the switch argument is in a register.
         For n>6 this algorithm may be more compact */
      jtablo = newiTempLabel (NULL);
      jtabhi = newiTempLabel (NULL);

      /* get the condition into accumulator.
         Using b as temporary storage, if register push/pop is needed */
      aopOp (IC_JTCOND (ic), ic, FALSE);
      l = aopGet (IC_JTCOND (ic), 0, FALSE, FALSE);
      if ((AOP_TYPE (IC_JTCOND (ic)) == AOP_R0 && _G.r0Pushed) ||
          (AOP_TYPE (IC_JTCOND (ic)) == AOP_R1 && _G.r1Pushed))
        {
          // (MB) what if B is in use???
          wassertl(!BINUSE, "B was in use");
          emitcode ("mov", "b,%s", l);
          l = "b";
        }
      freeAsmop (IC_JTCOND (ic), NULL, ic, TRUE);
      MOVA (l);
      if( count <= 112 )
        {
          emitcode ("add", "a,#(%05d$-3-.)", jtablo->key + 100);
          emitcode ("movc", "a,@a+pc");
          emitcode ("push", "acc");

          MOVA (l);
          emitcode ("add", "a,#(%05d$-3-.)", jtabhi->key + 100);
          emitcode ("movc", "a,@a+pc");
          emitcode ("push", "acc");
        }
      else
        {
          /* this scales up to n<=255, but needs two more bytes
             and changes dptr */
          emitcode ("mov", "dptr,#%05d$", jtablo->key + 100);
          emitcode ("movc", "a,@a+dptr");
          emitcode ("push", "acc");

          MOVA (l);
          emitcode ("mov", "dptr,#%05d$", jtabhi->key + 100);
          emitcode ("movc", "a,@a+dptr");
          emitcode ("push", "acc");
        }

      emitcode ("ret", "");

      /* now generate jump table, LSB */
      emitLabel (jtablo);
      for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab;
           jtab = setNextItem (IC_JTLABELS (ic)))
        emitcode (".db", "%05d$", jtab->key + 100);

      /* now generate jump table, MSB */
      emitLabel (jtabhi);
      for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab;
           jtab = setNextItem (IC_JTLABELS (ic)))
         emitcode (".db", "%05d$>>8", jtab->key + 100);
    }
}

/*-----------------------------------------------------------------*/
/* genCast - gen code for casting                                  */
/*-----------------------------------------------------------------*/
static void
genCast (iCode * ic)
{
  operand *result = IC_RESULT (ic);
  sym_link *ctype = operandType (IC_LEFT (ic));
  sym_link *rtype = operandType (IC_RIGHT (ic));
  operand *right = IC_RIGHT (ic);
  int size, offset;

  D (emitcode (";", "genCast"));

  /* if they are equivalent then do nothing */
  if (operandsEqu (IC_RESULT (ic), IC_RIGHT (ic)))
    return;

  aopOp (right, ic, FALSE);
  aopOp (result, ic, FALSE);

  /* if the result is a bit (and not a bitfield) */
  if (IS_BIT (OP_SYMBOL (result)->type))
    {
      assignBit (result, right);
      goto release;
    }

  /* if they are the same size : or less */
  if (AOP_SIZE (result) <= AOP_SIZE (right))
    {

      /* if they are in the same place */
      if (sameRegs (AOP (right), AOP (result)))
        goto release;

      /* if they in different places then copy */
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          aopPut (result,
                  aopGet (right, offset, FALSE, FALSE),
                  offset);
          offset++;
        }
      goto release;
    }

  /* if the result is of type pointer */
  if (IS_PTR (ctype))
    {

      int p_type;
      sym_link *type = operandType (right);
      sym_link *etype = getSpec (type);

      /* pointer to generic pointer */
      if (IS_GENPTR (ctype))
        {
          if (IS_PTR (type))
            {
              p_type = DCL_TYPE (type);
            }
          else
            {
              if (SPEC_SCLS(etype)==S_REGISTER) {
                // let's assume it is a generic pointer
                p_type=GPOINTER;
              } else {
                /* we have to go by the storage class */
                p_type = PTR_TYPE (SPEC_OCLS (etype));
              }
            }

          /* the first two bytes are known */
          size = GPTRSIZE - 1;
          offset = 0;
          while (size--)
            {
              aopPut (result,
                      aopGet (right, offset, FALSE, FALSE),
                      offset);
              offset++;
            }
          /* the last byte depending on type */
            {
                int gpVal = pointerTypeToGPByte(p_type, NULL, NULL);
                char gpValStr[10];

                if (gpVal == -1)
                {
                    // pointerTypeToGPByte will have bitched.
                    exit(1);
                }

                sprintf(gpValStr, "#0x%x", gpVal);
                aopPut (result, gpValStr, GPTRSIZE - 1);
            }
          goto release;
        }

      /* just copy the pointers */
      size = AOP_SIZE (result);
      offset = 0;
      while (size--)
        {
          aopPut (result,
                  aopGet (right, offset, FALSE, FALSE),
                  offset);
          offset++;
        }
      goto release;
    }

  /* so we now know that the size of destination is greater
     than the size of the source */
  /* we move to result for the size of source */
  size = AOP_SIZE (right);
  offset = 0;
  while (size--)
    {
      aopPut (result,
              aopGet (right, offset, FALSE, FALSE),
              offset);
      offset++;
    }

  /* now depending on the sign of the source && destination */
  size = AOP_SIZE (result) - AOP_SIZE (right);
  /* if unsigned or not an integral type */
  if (!IS_SPEC (rtype) || SPEC_USIGN (rtype) || AOP_TYPE(right)==AOP_CRY)
    {
      while (size--)
        aopPut (result, zero, offset++);
    }
  else
    {
      /* we need to extend the sign :{ */
      char *l = aopGet (right, AOP_SIZE (right) - 1,
                        FALSE, FALSE);
      MOVA (l);
      emitcode ("rlc", "a");
      emitcode ("subb", "a,acc");
      while (size--)
        aopPut (result, "a", offset++);
    }

  /* we are done hurray !!!! */

release:
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genDjnz - generate decrement & jump if not zero instrucion      */
/*-----------------------------------------------------------------*/
static int
genDjnz (iCode * ic, iCode * ifx)
{
  symbol *lbl, *lbl1;
  if (!ifx)
    return 0;

  /* if the if condition has a false label
     then we cannot save */
  if (IC_FALSE (ifx))
    return 0;

  /* if the minus is not of the form a = a - 1 */
  if (!isOperandEqual (IC_RESULT (ic), IC_LEFT (ic)) ||
      !IS_OP_LITERAL (IC_RIGHT (ic)))
    return 0;

  if (operandLitValue (IC_RIGHT (ic)) != 1)
    return 0;

  /* if the size of this greater than one then no
     saving */
  if (getSize (operandType (IC_RESULT (ic))) > 1)
    return 0;

  /* otherwise we can save BIG */

  D (emitcode (";", "genDjnz"));

  lbl = newiTempLabel (NULL);
  lbl1 = newiTempLabel (NULL);

  aopOp (IC_RESULT (ic), ic, FALSE);

  if (AOP_NEEDSACC(IC_RESULT(ic)))
  {
      /* If the result is accessed indirectly via
       * the accumulator, we must explicitly write
       * it back after the decrement.
       */
      char *rByte = aopGet (IC_RESULT(ic), 0, FALSE, FALSE);

      if (strcmp(rByte, "a"))
      {
           /* Something is hopelessly wrong */
           fprintf(stderr, "*** warning: internal error at %s:%d\n",
                   __FILE__, __LINE__);
           /* We can just give up; the generated code will be inefficient,
            * but what the hey.
            */
           freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
           return 0;
      }
      emitcode ("dec", "%s", rByte);
      aopPut (IC_RESULT (ic), rByte, 0);
      emitcode ("jnz", "%05d$", lbl->key + 100);
  }
  else if (IS_AOP_PREG (IC_RESULT (ic)))
    {
      emitcode ("dec", "%s",
                aopGet (IC_RESULT (ic), 0, FALSE, FALSE));
      MOVA (aopGet (IC_RESULT (ic), 0, FALSE, FALSE));
      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
      ifx->generated = 1;
      emitcode ("jnz", "%05d$", lbl->key + 100);
    }
  else
    {
      emitcode ("djnz", "%s,%05d$", aopGet (IC_RESULT (ic), 0, FALSE, FALSE),
                lbl->key + 100);
    }
  emitcode ("sjmp", "%05d$", lbl1->key + 100);
  emitLabel (lbl);
  emitcode ("ljmp", "%05d$", IC_TRUE (ifx)->key + 100);
  emitLabel (lbl1);

  if (!ifx->generated)
      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
  ifx->generated = 1;
  return 1;
}

/*-----------------------------------------------------------------*/
/* genReceive - generate code for a receive iCode                  */
/*-----------------------------------------------------------------*/
static void
genReceive (iCode * ic)
{
  int size = getSize (operandType (IC_RESULT (ic)));
  int offset = 0;

  D (emitcode (";", "genReceive"));

  if (ic->argreg == 1)
    { /* first parameter */
      if ((isOperandInFarSpace (IC_RESULT (ic)) ||
           isOperandInPagedSpace (IC_RESULT (ic))) &&
          (OP_SYMBOL (IC_RESULT (ic))->isspilt ||
           IS_TRUE_SYMOP (IC_RESULT (ic))))
        {
          regs *tempRegs[4];
          int receivingA = 0;
          int roffset = 0;

          for (offset = 0; offset<size; offset++)
            if (!strcmp (fReturn[offset], "a"))
              receivingA = 1;

          if (!receivingA)
            {
              if (size==1 || getTempRegs(tempRegs, size-1, ic))
                {
                  for (offset = size-1; offset>0; offset--)
                    emitcode("mov","%s,%s", tempRegs[roffset++]->name, fReturn[offset]);
                  emitcode("mov","a,%s", fReturn[0]);
                  _G.accInUse++;
                  aopOp (IC_RESULT (ic), ic, FALSE);
                  _G.accInUse--;
                  aopPut (IC_RESULT (ic), "a", offset);
                  for (offset = 1; offset<size; offset++)
                    aopPut (IC_RESULT (ic), tempRegs[--roffset]->name, offset);
                  goto release;
                }
            }
          else
            {
              if (getTempRegs(tempRegs, size, ic))
                {
                  for (offset = 0; offset<size; offset++)
                    emitcode("mov","%s,%s", tempRegs[offset]->name, fReturn[offset]);
                  aopOp (IC_RESULT (ic), ic, FALSE);
                  for (offset = 0; offset<size; offset++)
                    aopPut (IC_RESULT (ic), tempRegs[offset]->name, offset);
                  goto release;
                }
            }

          offset = fReturnSizeMCS51 - size;
          while (size--)
            {
              emitcode ("push", "%s", (strcmp (fReturn[fReturnSizeMCS51 - offset - 1], "a") ?
                                       fReturn[fReturnSizeMCS51 - offset - 1] : "acc"));
              offset++;
            }
          aopOp (IC_RESULT (ic), ic, FALSE);
          size = AOP_SIZE (IC_RESULT (ic));
          offset = 0;
          while (size--)
            {
              emitcode ("pop", "acc");
              aopPut (IC_RESULT (ic), "a", offset++);
            }
        }
      else
        {
          _G.accInUse++;
          aopOp (IC_RESULT (ic), ic, FALSE);
          _G.accInUse--;
          assignResultValue (IC_RESULT (ic), NULL);
        }
    }
  else if (ic->argreg > 12)
    { /* bit parameters */
      if (OP_SYMBOL (IC_RESULT (ic))->regs[0]->rIdx != ic->argreg-5)
        {
          aopOp (IC_RESULT (ic), ic, FALSE);
          emitcode ("mov", "c,%s", rb1regs[ic->argreg-5]);
          outBitC(IC_RESULT (ic));
        }
    }
  else
    { /* other parameters */
      int rb1off ;
      aopOp (IC_RESULT (ic), ic, FALSE);
      rb1off = ic->argreg;
      while (size--)
        {
          aopPut (IC_RESULT (ic), rb1regs[rb1off++ -5], offset++);
        }
    }

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genDummyRead - generate code for dummy read of volatiles        */
/*-----------------------------------------------------------------*/
static void
genDummyRead (iCode * ic)
{
  operand *op;
  int size, offset;

  D (emitcode(";", "genDummyRead"));

  op = IC_RIGHT (ic);
  if (op && IS_SYMOP (op))
    {
      aopOp (op, ic, FALSE);

      /* if the result is a bit */
      if (AOP_TYPE (op) == AOP_CRY)
        emitcode ("mov", "c,%s", AOP (op)->aopu.aop_dir);
      else
        {
          /* bit variables done */
          /* general case */
          size = AOP_SIZE (op);
          offset = 0;
          while (size--)
          {
            MOVA (aopGet (op, offset, FALSE, FALSE));
            offset++;
          }
        }

      freeAsmop (op, NULL, ic, TRUE);
    }

  op = IC_LEFT (ic);
  if (op && IS_SYMOP (op))
    {
      aopOp (op, ic, FALSE);

      /* if the result is a bit */
      if (AOP_TYPE (op) == AOP_CRY)
        emitcode ("mov", "c,%s", AOP (op)->aopu.aop_dir);
      else
        {
          /* bit variables done */
          /* general case */
          size = AOP_SIZE (op);
          offset = 0;
          while (size--)
          {
            MOVA (aopGet (op, offset, FALSE, FALSE));
            offset++;
          }
        }

      freeAsmop (op, NULL, ic, TRUE);
    }
}

/*-----------------------------------------------------------------*/
/* genCritical - generate code for start of a critical sequence    */
/*-----------------------------------------------------------------*/
static void
genCritical (iCode *ic)
{
  symbol *tlbl = newiTempLabel (NULL);

  D (emitcode(";", "genCritical"));

  if (IC_RESULT (ic))
    {
      aopOp (IC_RESULT (ic), ic, TRUE);
      aopPut (IC_RESULT (ic), one, 0); /* save old ea in an operand */
      emitcode ("jbc", "ea,%05d$", (tlbl->key + 100)); /* atomic test & clear */
      aopPut (IC_RESULT (ic), zero, 0);
      emitLabel (tlbl);
      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }
  else
    {
      emitcode ("setb", "c");
      emitcode ("jbc", "ea,%05d$", (tlbl->key + 100)); /* atomic test & clear */
      emitcode ("clr", "c");
      emitLabel (tlbl);
      emitcode ("push", "psw"); /* save old ea via c in psw on top of stack*/
    }
}

/*-----------------------------------------------------------------*/
/* genEndCritical - generate code for end of a critical sequence   */
/*-----------------------------------------------------------------*/
static void
genEndCritical (iCode *ic)
{
  D(emitcode(";", "genEndCritical"));

  if (IC_RIGHT (ic))
    {
      aopOp (IC_RIGHT (ic), ic, FALSE);
      if (AOP_TYPE (IC_RIGHT (ic)) == AOP_CRY)
        {
          emitcode ("mov", "c,%s", IC_RIGHT (ic)->aop->aopu.aop_dir);
          emitcode ("mov", "ea,c");
        }
      else
        {
          if (AOP_TYPE (IC_RIGHT (ic)) != AOP_DUMMY)
            MOVA (aopGet (IC_RIGHT (ic), 0, FALSE, FALSE));
          emitcode ("rrc", "a");
          emitcode ("mov", "ea,c");
        }
      freeAsmop (IC_RIGHT (ic), NULL, ic, TRUE);
    }
  else
    {
      emitcode ("pop", "psw"); /* restore ea via c in psw on top of stack */
      emitcode ("mov", "ea,c");
    }
}

/*-----------------------------------------------------------------*/
/* gen51Code - generate code for 8051 based controllers            */
/*-----------------------------------------------------------------*/
void
gen51Code (iCode * lic)
{
  iCode *ic;
  int cln = 0;
  /* int cseq = 0; */

  _G.currentFunc = NULL;
  lineHead = lineCurr = NULL;

  /* print the allocation information */
  if (allocInfo && currFunc)
    printAllocInfo (currFunc, codeOutBuf);
  /* if debug information required */
  if (options.debug && currFunc)
    {
      debugFile->writeFunction (currFunc, lic);
    }
  /* stack pointer name */
  if (options.useXstack)
    spname = "_spx";
  else
    spname = "sp";


  for (ic = lic; ic; ic = ic->next)
    {
      _G.current_iCode = ic;

      if (ic->lineno && cln != ic->lineno)
        {
          if (options.debug)
            {
              debugFile->writeCLine (ic);
            }
          if (!options.noCcodeInAsm) {
            emitcode (";", "%s:%d: %s", ic->filename, ic->lineno,
                      printCLine(ic->filename, ic->lineno));
          }
          cln = ic->lineno;
        }
      #if 0
      if (ic->seqPoint && ic->seqPoint != cseq)
        {
          emitcode (";", "sequence point %d", ic->seqPoint);
          cseq = ic->seqPoint;
        }
      #endif
      if (options.iCodeInAsm) {
        char regsInUse[80];
        int i;
        char *iLine;

        #if 0
        for (i=0; i<8; i++) {
          sprintf (&regsInUse[i],
                   "%c", ic->riu & (1<<i) ? i+'0' : '-'); /* show riu */
        regsInUse[i]=0;
        #else
        strcpy (regsInUse, "--------");
        for (i=0; i < 8; i++) {
          if (bitVectBitValue (ic->rMask, i))
            {
              int offset = regs8051[i].offset;
              regsInUse[offset] = offset + '0'; /* show rMask */
            }
        #endif
        }
        iLine = printILine(ic);
        emitcode(";", "[%s] ic:%d: %s", regsInUse, ic->seq, iLine);
        dbuf_free(iLine);
      }
      /* if the result is marked as
         spilt and rematerializable or code for
         this has already been generated then
         do nothing */
      if (resultRemat (ic) || ic->generated)
        continue;

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
          if (ic->next &&
              ic->next->op == IFX &&
              regsInCommon (IC_LEFT (ic), IC_COND (ic->next)))
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
          if (!genDjnz (ic, ifxForOp (IC_RESULT (ic), ic)))
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
          genCmpGt (ic, ifxForOp (IC_RESULT (ic), ic));
          break;

        case '<':
          genCmpLt (ic, ifxForOp (IC_RESULT (ic), ic));
          break;

        case LE_OP:
        case GE_OP:
        case NE_OP:

          /* note these two are xlated by algebraic equivalence
             in decorateType() in SDCCast.c */
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
                  "got '>=' or '<=' shouldn't have come here");
          break;

        case EQ_OP:
          genCmpEq (ic, ifxForOp (IC_RESULT (ic), ic));
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
          genInline (ic);
          break;

        case RRC:
          genRRC (ic);
          break;

        case RLC:
          genRLC (ic);
          break;

        case GETHBIT:
          genGetHbit (ic);
          break;

        case GETABIT:
          genGetAbit (ic);
          break;

        case GETBYTE:
          genGetByte (ic);
          break;

        case GETWORD:
          genGetWord (ic);
          break;

        case LEFT_OP:
          genLeftShift (ic);
          break;

        case RIGHT_OP:
          genRightShift (ic);
          break;

        case GET_VALUE_AT_ADDRESS:
          genPointerGet (ic,
                         hasInc (IC_LEFT (ic), ic,
                                 getSize (operandType (IC_RESULT (ic)))),
                         ifxForOp (IC_RESULT (ic), ic) );
          break;

        case '=':
          if (POINTER_SET (ic))
            genPointerSet (ic,
                           hasInc (IC_RESULT (ic), ic,
                                   getSize (operandType (IC_RIGHT (ic)))));
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
          addSet (&_G.sendSet, ic);
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

        case SWAP:
          genSwap (ic);
          break;

        default:
          ic = ic;
        }
    }

  _G.current_iCode = NULL;

  /* now we are ready to call the
     peep hole optimizer */
  if (!options.nopeep)
    peepHole (&lineHead);

  /* now do the actual printing */
  printLine (lineHead, codeOutBuf);
  return;
}
