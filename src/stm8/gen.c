/*-------------------------------------------------------------------------
  gen.c - code generator for STM8.

  Copyright (C) 2012 - 2013, Philipp Klaus Krause pkk@spth.de, philipp@informatik.uni-frankfurt.de)
                2011, Vaclav Peroutka

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

#include "ralloc.h"
#include "gen.h"

/* Use the D macro for basic (unobtrusive) debugging messages */
#define D(x) do if (options.verboseAsm) { x; } while (0)

static bool regalloc_dry_run;
static unsigned int regalloc_dry_run_cost_bytes;
static unsigned int regalloc_dry_run_cost_cycles;

static struct
{
  short debugLine;
  struct
    {
      int pushed;
    } stack;
  bool saved;
}
_G;

enum asminst
{
  A_ADC,
  A_ADD,
  A_AND,
  A_CLR,
  A_CP,
  A_CPL,
  A_CPLW,
  A_DEC,
  A_DECW,
  A_INC,
  A_INCW,
  A_LD,
  A_MOV,
  A_NEG,
  A_NEGW,
  A_OR,
  A_RLC,
  A_RLCW,
  A_RLWA,
  A_RRC,
  A_RRCW,
  A_RRWA,
  A_SBC,
  A_SLL,
  A_SLLW,
  A_SRA,
  A_SRAW,
  A_SRL,
  A_SRLW,
  A_SUB,
  A_TNZ,
  A_TNZW,
  A_XOR
};

static const char *asminstnames[] =
{
  "adc",
  "add",
  "and",
  "clr",
  "cp",
  "cpl",
  "cplw",
  "dec",
  "decw",
  "inc",
  "incw",
  "ld",
  "mov",
  "neg",
  "negw",
  "or",
  "rlc",
  "rlcw",
  "rlwa",
  "rrc",
  "rrcw",
  "rrwa",
  "sbc",
  "sll",
  "sllw",
  "sra",
  "sraw",
  "srl",
  "srlw",
  "sub",
  "tnz",
  "tnzw",
  "xor"
};

static struct asmop asmop_a, asmop_x, asmop_y, asmop_xy, asmop_zero, asmop_one;
static struct asmop *const ASMOP_A = &asmop_a;
static struct asmop *const ASMOP_X = &asmop_x;
static struct asmop *const ASMOP_Y = &asmop_y;
static struct asmop *const ASMOP_XY = &asmop_xy;
static struct asmop *const ASMOP_ZERO = &asmop_zero;
static struct asmop *const ASMOP_ONE = &asmop_one;

void
stm8_init_asmops (void)
{
  asmop_a.type = AOP_REG;
  asmop_a.size = 1;
  asmop_a.aopu.bytes[0].in_reg = TRUE;
  asmop_a.aopu.bytes[0].byteu.reg = stm8_regs + A_IDX;

  asmop_x.type = AOP_REG;
  asmop_x.size = 2;
  asmop_x.aopu.bytes[0].in_reg = TRUE;
  asmop_x.aopu.bytes[0].byteu.reg = stm8_regs + XL_IDX;
  asmop_x.aopu.bytes[1].in_reg = TRUE;
  asmop_x.aopu.bytes[1].byteu.reg = stm8_regs + XH_IDX;

  asmop_y.type = AOP_REG;
  asmop_y.size = 2;
  asmop_y.aopu.bytes[0].in_reg = TRUE;
  asmop_y.aopu.bytes[0].byteu.reg = stm8_regs + YL_IDX;
  asmop_y.aopu.bytes[1].in_reg = TRUE;
  asmop_y.aopu.bytes[1].byteu.reg = stm8_regs + YH_IDX;

  asmop_xy.type = AOP_REG;
  asmop_xy.size = 4;
  asmop_xy.aopu.bytes[0].in_reg = TRUE;
  asmop_xy.aopu.bytes[0].byteu.reg = stm8_regs + XL_IDX;
  asmop_xy.aopu.bytes[1].in_reg = TRUE;
  asmop_xy.aopu.bytes[1].byteu.reg = stm8_regs + XH_IDX;
  asmop_xy.aopu.bytes[2].in_reg = TRUE;
  asmop_xy.aopu.bytes[2].byteu.reg = stm8_regs + YL_IDX;
  asmop_xy.aopu.bytes[3].in_reg = TRUE;
  asmop_xy.aopu.bytes[3].byteu.reg = stm8_regs + YH_IDX;

  asmop_zero.type = AOP_LIT;
  asmop_zero.size = 1;
  asmop_zero.aopu.aop_lit = constVal ("0");

  asmop_one.type = AOP_LIT;
  asmop_one.size = 1;
  asmop_one.aopu.aop_lit = constVal ("1");
}

/*-----------------------------------------------------------------*/
/* aopRS - asmop in register or on stack                           */
/*-----------------------------------------------------------------*/
static bool
aopRS (const asmop *aop)
{
  return (aop->type == AOP_REG || aop->type == AOP_REGSTK || aop->type == AOP_STK);
}

/*-----------------------------------------------------------------*/
/* aopInREg - asmop from offset in the register                    */
/*-----------------------------------------------------------------*/
static bool
aopInReg (const asmop *aop, int offset, short rIdx)
{
  if (!aopRS (aop))
    return (FALSE);

  if (offset >= aop->size)
    return (FALSE);

  if (rIdx == X_IDX)
    return (aopInReg (aop, offset, XL_IDX) && aopInReg (aop, offset + 1, XH_IDX));

  if (rIdx == Y_IDX)
    return (aopInReg (aop, offset, YL_IDX) && aopInReg (aop, offset + 1, YH_IDX));

  return (aop->aopu.bytes[offset].in_reg && aop->aopu.bytes[offset].byteu.reg->rIdx == rIdx);
}

/*-----------------------------------------------------------------*/
/* aopInREg - asmop from offset on stack                           */
/*-----------------------------------------------------------------*/
static bool
aopOnStack (const asmop *aop, int offset, int size)
{
  int i;

  if (!aopRS (aop))
    return (FALSE);

  if (offset + size > aop->size)
    return (FALSE);

  for (i = offset; i < offset + size; i++)
    if (aop->aopu.bytes[i].in_reg)
      return (FALSE);

  return (TRUE);
}

static void
cost(unsigned int bytes, unsigned int cycles)
{
  regalloc_dry_run_cost_bytes += bytes;
  regalloc_dry_run_cost_cycles += cycles;
}

static const char *
aopGet(const asmop *aop, int offset)
{
  static char buffer[256];

  /* Don't really need the value during dry runs, so save some time. */
  if (regalloc_dry_run)
    return ("");

  if (offset >= aop->size)
    return ("#0x00");

  if (aop->type == AOP_LIT)
    {
      snprintf (buffer, 256, "#0x%02x", byteOfVal (aop->aopu.aop_lit, offset));
      return (buffer);
    }

  if (aopRS (aop) && aop->aopu.bytes[offset].in_reg)
    return (aop->aopu.bytes[offset].byteu.reg->name);

  if (aopRS (aop) && !aop->aopu.bytes[offset].in_reg)
    {
      int soffset = aop->aopu.bytes[offset].byteu.stk + _G.stack.pushed;

      if (soffset > 255)
        {
          if (!regalloc_dry_run)
            wassertl (0, "Unimplemented extended stack access.");
          cost (80, 80);
        }

      snprintf (buffer, 256, "(0x%02x, sp)", soffset);
      return (buffer);
    }

  if (aop->type == AOP_IMMD)
    {
      wassertl (offset < 2, "Immediate operand out of range");
      snprintf (buffer, 256, offset ? "#>%s" : "#<%s", aop->aopu.aop_immd);
      return (buffer);
    }

  if (aop->type == AOP_DIR)
    {
      snprintf (buffer, 256, "%s+%d", aop->aopu.aop_dir, aop->size - 1 - offset);
      return (buffer);
    }

  wassert (0);
  return ("dummy");
}

static const char *
aopGet2(const asmop *aop, int offset)
{
  static char buffer[256];

  /* Don't really need the value during dry runs, so save some time. */
  if (regalloc_dry_run)
    return ("");

  if (aopInReg (aop, offset, X_IDX))
    return("x");
  if (aopInReg (aop, offset, Y_IDX))
    return("y");

  wassert (aop->type == AOP_LIT || aopOnStack (aop, offset, 2) || aop->type == AOP_IMMD || aop->type == AOP_DIR);

  if (aop->type == AOP_LIT)
    {
      snprintf (buffer, 256, "#0x%02x%02x", byteOfVal (aop->aopu.aop_lit, offset + 1), byteOfVal (aop->aopu.aop_lit, offset));
      return (buffer);
    }
  else if (aop->type == AOP_IMMD)
    {
      snprintf (buffer, 256, "#%s", aop->aopu.aop_immd);
      return (buffer);
    }

  return (aopGet (aop, offset + 1));
}

/* For operations that always have the accumulator as left operand. */
static void
op8_cost (const asmop *op2, int offset2)
{
  AOP_TYPE op2type = op2->type;
  int r2Idx = ((aopRS (op2) && op2->aopu.bytes[offset2].in_reg)) ? op2->aopu.bytes[offset2].byteu.reg->rIdx : -1;

  if (offset2 >= op2->size)
    op2type = AOP_LIT;

  switch (op2type)
    {
    case AOP_LIT:
    case AOP_IMMD:
      cost (2, 1);
      return;
    case AOP_DIR:
      cost (3, 1);
      return;
    case AOP_REG:
    case AOP_REGSTK:
    case AOP_STK:
      if (r2Idx != -1)
        goto error;
      cost (2, 1);
      return;
    default:
      goto error;
    }
error:
  fprintf(stderr, "op2 type: %d, offset %d, rIdx %d\n", op2type, offset2, r2Idx);
  wassert (0);
  cost (8, 4 * 8);
}

/* For 8-bit operations that have only one operand, i.e. tnz */
static void
op_cost (const asmop *op1, int offset1)
{
  AOP_TYPE op1type;
  int r1Idx;

  wassert (op1);

  op1type = op1->type;
  r1Idx = ((aopRS (op1) && op1->aopu.bytes[offset1].in_reg)) ? op1->aopu.bytes[offset1].byteu.reg->rIdx : -1;

  switch (op1type)
    {
    case AOP_DIR:
      cost (4, 1);
      return;
    case AOP_REG:
    case AOP_REGSTK:
    case AOP_STK:
      if (r1Idx == A_IDX)
        {
          cost (1, 1);
          return;
        }
      if (r1Idx != -1)
        goto error;
      cost (2, 1);
      return;
    default:
      goto error;
    }
error:
  fprintf(stderr, "op1 type: %d, offset %d, rIdx %d\n", op1type, offset1, r1Idx);
  wassert (0);
  cost (8, 4 * 8);
}

/* For cheap 16-bit operations that have only one operand, i.e. incw */
static void
opw_cost (const asmop *op1, int offset1)
{
  wassert (op1);

  if (aopInReg (op1, offset1, X_IDX))
    {
      cost (1, 1);
      return;
    }
  else if (aopInReg (op1, offset1, Y_IDX))
    {
      cost (2, 1);
      return;
    }

  wassert (0);
  cost (8, 4 * 8);
}

/* For 16-bit operations that have only one operand, i.e. tnzw */
static void
opw_cost2 (const asmop *op1, int offset1)
{
  wassert (op1);

  if (aopInReg (op1, offset1, X_IDX))
    {
      cost (1, 2);
      return;
    }
  else if (aopInReg (op1, offset1, Y_IDX))
    {
      cost (2, 2);
      return;
    }

  wassert (0);
  cost (8, 4 * 8);
}

static void
ld_cost (const asmop *op1, int offset1, const asmop *op2, int offset2)
{
  AOP_TYPE op1type = op1->type;
  AOP_TYPE op2type = op2->type;

  /* Costs are symmetric */
  if (aopRS (op2) || op2type == AOP_DUMMY)
    {
      const asmop *tmp = op1;
      const int tmpo = offset1;
      op1 = op2;
      op2 = tmp;
      offset1 = offset2;
      offset2 = tmpo;
      op1type = op1->type;
      op2type = op2->type;
    }

  int r1Idx = ((aopRS (op1) && op1->aopu.bytes[offset1].in_reg)) ? op1->aopu.bytes[offset1].byteu.reg->rIdx : -1;
  int r2Idx = ((aopRS (op2) && op2->aopu.bytes[offset2].in_reg)) ? op2->aopu.bytes[offset2].byteu.reg->rIdx : -1;

  if (offset2 >= op2->size)
    op2type = AOP_LIT;

  switch (op1type)
    {
    case AOP_REG:
    case AOP_REGSTK:
    case AOP_STK:
      switch (op2type)
        {
        case AOP_LIT:
        case AOP_IMMD:
          if (r1Idx != A_IDX)
            goto error;
          cost (2, 1);
          return;
        case AOP_REG:
        case AOP_REGSTK:
        case AOP_STK:
          switch (r1Idx)
            {
            case A_IDX:
              switch (r2Idx)
                {
                case XL_IDX:
                case XH_IDX:
                  cost (1, 1);
                  return;
                case YL_IDX:
                case YH_IDX:
                case -1:
                  cost (2, 1);
                  return;
                default:
                  goto error;
                }
            case XL_IDX:
            case XH_IDX:
              if (r2Idx != A_IDX)
                goto error;
              cost (1, 1);
              return;
            case YL_IDX:
            case YH_IDX:
            case -1:
              if (r2Idx != A_IDX)
                goto error;
              cost (2, 1);
              return;
          }
        case AOP_DIR:
          if (r1Idx != A_IDX)
            goto error;
          cost (3, 2);
          return;
        default:
          goto error;
        }
    case AOP_DIR:
      if (r2Idx != A_IDX)
        goto error;
      cost (3, 2);
      return;
    default:
      goto error;
    }
error:
  fprintf(stderr, "op1 type: %d, offset %d, rIdx %d\n", op1type, offset1, r1Idx);
  fprintf(stderr, "op2 type: %d, offset %d, rIdx %d\n", op2type, offset2, r2Idx);
  wassert (0);
  cost (8, 4 * 8);
}

static void
mov_cost (const asmop *op1, const asmop *op2)
{
  if (op2->type == AOP_LIT || op2->type == AOP_IMMD)
    cost (4, 1);
  else
    cost (5, 1);
}

static void
emit3cost (enum asminst inst, const asmop *op1, int offset1, const asmop *op2, int offset2)
{
  switch (inst)
  {
  case A_ADC:
  case A_ADD:
  case A_AND:
    op8_cost (op2, offset2);
    break;
  case A_CLR:
    op_cost (op1, offset1);
    break;
  case A_CP:
    op8_cost (op2, offset2);
    break;
  case A_CPL:
    op_cost (op1, offset1);
    break;
  case A_INC:
  case A_DEC:
    op_cost (op1, offset1);
    break;
  case A_LD:
    ld_cost (op1, offset1, op2, offset2);
    break;
  case A_MOV:
    mov_cost (op1, op2);
    break;
  case A_NEG:
    op_cost (op1, offset1);
    break;
  case A_OR:
    op8_cost (op2, offset2);
    break;
  case A_RLC:
  case A_RRC:
    op_cost (op1, offset1);
    break;
  case A_SBC:
    op8_cost (op2, offset2);
    break;
  case A_SLL:
  case A_SRA:
  case A_SRL:
    op_cost (op1, offset1);
    break;
  case A_SUB:
    op8_cost (op2, offset2);
    break;
  case A_TNZ:
    op_cost (op1, offset1);
    break;
  case A_XOR:
    op8_cost (op2, offset2);
    break;
  default:
    wassertl (0, "Tried to get cost for unknown 8-bit instruction");
  }
}

static void
emit3wcost (enum asminst inst, const asmop *op1, int offset1, const asmop *op2, int offset2)
{
  switch (inst)
  {
  case A_CPLW:
    opw_cost2 (op1, offset1);
    break;
  case A_DECW:
  case A_INCW:
    opw_cost (op1, offset1);
    break;
  case A_NEGW:
  case A_RLCW:
    opw_cost2 (op1, offset1);
    break;
  case A_RLWA:
    opw_cost (op1, offset1);
    break;
  case A_RRCW:
    opw_cost2 (op1, offset1);
    break;
  case A_RRWA:
    opw_cost (op1, offset1);
    break;
  case A_SLLW:
  case A_SRAW:
  case A_SRLW:
  case A_TNZW:
    opw_cost2 (op1, offset1);
    break;
  default:
    wassertl (0, "Tried to get cost for unknown 16-bit instruction");
  }
}

static void
emit3_o (enum asminst inst, asmop *op1, int offset1, asmop *op2, int offset2)
{
  emit3cost (inst, op1, offset1, op2, offset2);
  if (regalloc_dry_run)
    return;

  if (op2)
    {
      char *l = Safe_strdup (aopGet (op1, offset1));
      emitcode (asminstnames[inst], "%s, %s", l, aopGet (op2, offset2));
      Safe_free (l);
    }
  else
    emitcode (asminstnames[inst], "%s", aopGet (op1, offset1));
}

static void
emit3w_o (enum asminst inst, asmop *op1, int offset1, asmop *op2, int offset2)
{
  emit3wcost (inst, op1, offset1, op2, offset2);
  if (regalloc_dry_run)
    return;

  if (op2)
    {
      char *l = Safe_strdup (aopGet2 (op1, offset1));
      emitcode (asminstnames[inst], "%s, %s", l, aopGet2 (op2, offset2));
      Safe_free (l);
    }
  else
    emitcode (asminstnames[inst], "%s", aopGet2 (op1, offset1));
}

static void
emit3 (enum asminst inst, asmop *op1, asmop *op2)
{
  emit3_o (inst, op1, 0, op2, 0);
}

static void
emit3w (enum asminst inst, asmop *op1, asmop *op2)
{
  emit3w_o (inst, op1, 0, op2, 0);
}

static bool
regFree (int idx, const iCode *ic)
{
  if (idx == X_IDX)
    return (regFree (XL_IDX, ic) && regFree (XH_IDX, ic));
  if (idx == Y_IDX)
    return (regFree (YL_IDX, ic) && regFree (YH_IDX, ic));
  return (!bitVectBitValue (ic->rMask, idx));
}

static bool
regDead (int idx, const iCode *ic)
{
  if (idx == X_IDX)
    return (regDead (XL_IDX, ic) && regDead (XH_IDX, ic));
  if (idx == Y_IDX)
    return (regDead (YL_IDX, ic) && regDead (YH_IDX, ic));
  return (!bitVectBitValue (ic->rSurv, idx));
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

  aop->regs[A_IDX] = -1;
  aop->regs[XL_IDX] = -1;
  aop->regs[XH_IDX] = -1;
  aop->regs[YL_IDX] = -1;
  aop->regs[YH_IDX] = -1;
  aop->regs[C_IDX] = -1;

  return aop;
}

/*-----------------------------------------------------------------*/
/* freeAsmop - free up the asmop given to an operand               */
/*----------------------------------------------------------------*/
static void
freeAsmop (operand *op)
{
  asmop *aop;

  wassert (op);

  aop = op->aop;

  if (!aop)
    return;

  Safe_free (aop);

  op->aop = NULL;
  if (IS_SYMOP (op) && SPIL_LOC (op))
    SPIL_LOC (op)->aop = NULL;
}

/*-----------------------------------------------------------------*/
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *
aopForSym (const iCode *ic, symbol *sym)
{
  asmop *aop;

  wassert (ic);
  wassert (sym);
  wassert (sym->etype);

  // Unlike other ports we really free asmops; to avoid a double-free, we need to support multiple asmops for the same symbol.

  if (IS_FUNC (sym->type))
    {
      aop = newAsmop (AOP_IMMD);
      aop->aopu.aop_immd = sym->rname;
      aop->size = 2;
    }
  /* Assign depending on the storage class */
  else if (sym->onStack || sym->iaccess)
    {
      int offset;

      aop = newAsmop (AOP_STK);
      aop->size = getSize (sym->type);
      
      for(offset = 0; offset < aop->size; offset++)
        aop->aopu.bytes[offset].byteu.stk = sym->stack + aop->size - offset;
    }
  else
    {
      aop = newAsmop (AOP_DIR);
      aop->aopu.aop_dir = sym->rname;
      aop->size = getSize (sym->type);
    }

  return aop;
}

/*-----------------------------------------------------------------*/
/* aopOp - allocates an asmop for an operand  :                    */
/*-----------------------------------------------------------------*/
static void
aopOp (operand *op, const iCode *ic)
{
  symbol *sym;
  int i;

  wassert (op);

  /* if already has an asmop */
  if (op->aop)
    return;

  /* if this a literal */
  if (IS_OP_LITERAL (op))
    {
      asmop *aop = newAsmop (AOP_LIT);
      aop->aopu.aop_lit = OP_VALUE (op);
      aop->size = getSize (operandType (op));
      op->aop = aop;
      return;
    }

  sym = OP_SYMBOL (op);

  /* if this is a true symbol */
  if (IS_TRUE_SYMOP (op))
    {
      op->aop = aopForSym (ic, sym);
      return;
    }

  if (sym->remat)
    {
      wassertl (0, "Unimplemented remat asmop.");
    }

  /* None of the above, which only leaves temporaries. */
  { 
    bool completly_in_regs = TRUE;
    bool completly_on_stack = TRUE;
    asmop *aop = newAsmop (AOP_REGSTK);

    aop->size = getSize (operandType (op));
    op->aop = aop;

    for (i = 0; i < getSize (operandType (op)); i++)
      {
        aop->aopu.bytes[i].in_reg = !!sym->regs[i];
        if (sym->regs[i])
          {
            completly_on_stack = FALSE;
            aop->aopu.bytes[i].byteu.reg = sym->regs[i];
            aop->regs[sym->regs[i]->rIdx] = i;
          }
        else if (sym->usl.spillLoc || sym->nRegs && regalloc_dry_run)
          {
            completly_in_regs = FALSE;
            if (!regalloc_dry_run)
              {
                aop->aopu.bytes[i].byteu.stk = sym->usl.spillLoc->stack + aop->size - i;
                wassertl (sym->usl.spillLoc->stack + i < 200, "Unimplemented EXSTK.");
              }
          }
        else // Dummy iTemp.
          {
            aop->type = AOP_DUMMY;
            return;
          }

        if (!completly_in_regs && (!currFunc || GcurMemmap == statsg))
          {
            if (!regalloc_dry_run)
              wassertl (0, "Stack asmop outside of function.");
            cost (80, 80);
          }
      }

    if (completly_in_regs)
      aop->type = AOP_REG;
    else if (completly_on_stack)
      aop->type = AOP_STK;

    return;
  }
}

static void
push (const asmop *op, int offset, int size)
{
  if (size == 1)
    {
      emitcode ("push", "%s", aopGet (op, offset));
      if (op->type == AOP_LIT)
        cost (2, 1);
      else if (op->type == AOP_IMMD)
        cost (2, 1);
      else if (aopInReg (op, offset, A_IDX))
        cost (1, 1);
      else if (op->type == AOP_DIR)
        cost (3, 1);
      else
        wassertl (0, "Invalid aop type for size 1 for push");
    }
  else if (size == 2)
    {
      if (aopInReg (op, offset, X_IDX))
        {
          emitcode ("pushw", "x");
          cost (1, 2);
        }
      else if  (aopInReg (op, offset, Y_IDX))
        {
          emitcode ("pushw", "y");
          cost (2, 2);
        }
      else
        wassertl (0, "Invalid aop type for size 2 for pushw");
    }
  else
    wassertl (0, "Invalid size for push/pushw");

  _G.stack.pushed += size;
}

static void
pop (const asmop *op, int offset, int size)
{
  if (size == 1)
    {
      emitcode ("pop", "%s", aopGet (op, offset));
      if (aopInReg (op, offset, A_IDX))
        cost (1, 1);
      else if (op->type == AOP_DIR)
        cost (3, 1);
      else
        wassertl (0, "Invalid aop type for size 1 for pop");
    }
  else if (size == 2)
    {
      if (aopInReg (op, offset, X_IDX))
        {
          emitcode ("popw", "x");
          cost (1, 2);
        }
      else if  (aopInReg (op, offset, Y_IDX))
        {
          emitcode ("popw", "y");
          cost (2, 2);
        }
      else
        wassertl (0, "Invalid aop type for size 2 for popw");
    }
  else
    wassertl (0, "Invalid size for pop/popw");

  _G.stack.pushed -= size;
}

void swap_to_a(int idx)
{
  switch (idx)
    {
    case XL_IDX:
      emitcode ("exg", "a, xl");
      cost (1, 1);
      break;
    case XH_IDX:
      emit3w (A_RLWA, ASMOP_X, 0);
      break;
    case YL_IDX:
      emitcode ("exg", "a, yl");
      cost (1, 1);
      break;
    case YH_IDX:
      emit3w (A_RLWA, ASMOP_Y, 0);
      break;
    default:
      wassert (0);
    }
}

void swap_from_a(int idx)
{
  switch (idx)
    {
    case XL_IDX:
      emitcode ("exg", "a, xl");
      cost (1, 1);
      break;
    case XH_IDX:
      emit3w (A_RRWA, ASMOP_X, 0);
      break;
    case YL_IDX:
      emitcode ("exg", "a, yl");
      cost (1, 1);
      break;
    case YH_IDX:
      emit3w (A_RRWA, ASMOP_Y, 0);
      break;
    default:
      wassert (0);
    }
}

/*-----------------------------------------------------------------*/
/* stackAop - put xl, xh, yl, yh aop on stack                      */
/*-----------------------------------------------------------------*/
static
const asmop *stack_aop (const asmop *aop, int i, int *offset)
{
  asmop *stacked = NULL;

  if (aopRS (aop) && !aopOnStack (aop, i, 1) && !aopInReg (aop, i, A_IDX))
    {
      if (aop->aopu.bytes[i].byteu.reg->rIdx == XL_IDX)
        {
          stacked = ASMOP_X;
          *offset = 2;
        }
      else if (aop->aopu.bytes[i].byteu.reg->rIdx == XH_IDX)
        {
          stacked = ASMOP_X;
          *offset = 1;
        }
      else if (aop->aopu.bytes[i].byteu.reg->rIdx == YL_IDX)
        {
          stacked = ASMOP_Y;
          *offset = 2;
        }
      else if (aop->aopu.bytes[i].byteu.reg->rIdx == YH_IDX)
        {
          stacked = ASMOP_Y;
          *offset = 1;
        }
      else
        wassert (0);
      push (stacked, 0, 2);
    }

  return (stacked);
}

/*--------------------------------------------------------------------------*/
/* adjustStack - Adjust the stack pointer by n bytes.                       */
/*--------------------------------------------------------------------------*/
static void
adjustStack (int n)
{
  while (n)
    {
      // The manual is ambigious (not even documenting if the #byte is signed), but it from experimenting with the hardware it
      // seems addw sp, byte has a signed operand, while sub sp, #byte has an unsigned operand, also, in contrast to what the
      // manual states, addw sp, #byte only takes 1 cycle.

      // todo: For small n, adjust stack by using pop where these are dead.
      // todo: For big n, use addition in X or Y when free. Need to fix calling convention before that though.
      if (n > 127)
        {
          emitcode ("addw","sp, #127");
          cost (2, 1);
          n -= 127;
          _G.stack.pushed -= 127;
        }
      else if (n < -255)
        {
          emitcode ("sub","sp, #255");
          cost (2, 1);
          n += 255;
          _G.stack.pushed += 255;
        }
      else if (n == -2  && optimize.codeSize)
        {
          push (ASMOP_X, 0, 2); // 1 Byte, 2 cycles - cheaper than addw sp, #byte when optimizing for code size.
          n += 2;
        }
      else if (n == -1 && optimize.codeSize)
        {
          push (ASMOP_A, 0, 1); // 1 Byte, 1 cycle - cheaper than addw sp, #byte.
          n++;
        }
      else if (n > 0)
        {
          emitcode ("addw", "sp, #%d", n);
          cost (2, 1);
          _G.stack.pushed -= n;
          return;
        }
	  else 
	    {
		  emitcode ("sub", "sp, #%d", -n);
          cost (2, 1);
          _G.stack.pushed += -n;
          return;
        }
    }
}

/*-----------------------------------------------------------------*/
/* cheapMove - Copy a byte from one asmop to another               */
/*-----------------------------------------------------------------*/
static void
cheapMove (asmop *result, int roffset, asmop *source, int soffset, bool save_a)
{
  bool dummy = (result->type == AOP_DUMMY || source->type == AOP_DUMMY);

  if (aopRS (result) && aopRS (source) &&
    result->aopu.bytes[roffset].in_reg && source->aopu.bytes[soffset].in_reg &&
    result->aopu.bytes[roffset].byteu.reg == source->aopu.bytes[soffset].byteu.reg)
    return;
  else if (!dummy && (!aopRS (result) || aopInReg (result, roffset, A_IDX) || aopOnStack (result, roffset, 1)) && (source->type == AOP_LIT && !byteOfVal (source->aopu.aop_lit, soffset) || soffset >= source->size))
    emit3_o (A_CLR, result, roffset, 0, 0);
  else if (!dummy && (aopInReg (result, roffset, A_IDX) || aopInReg (source, soffset, A_IDX)))
    emit3_o (A_LD, result, roffset, source, soffset);
  else if (result->type == AOP_DIR && (source->type == AOP_DIR || source->type == AOP_LIT))
    emit3_o (A_MOV, result, roffset, source, soffset);
  else
    {
      if (save_a)
        push (ASMOP_A, 0, 1);
      if (!aopInReg (source, soffset, A_IDX) && source->type != AOP_DUMMY)
        emit3_o (A_LD, ASMOP_A, 0, source, soffset);
      if (!aopInReg (result, roffset, A_IDX) && result->type != AOP_DUMMY)
        emit3_o (A_LD, result, roffset, ASMOP_A, 0);
      if (save_a)
        pop (ASMOP_A, 0, 1);
    }
}

/*-----------------------------------------------------------------*/
/* genCopy - Copy the value - stack to stack only                  */
/*-----------------------------------------------------------------*/
static void
genCopyStack (asmop *result, int roffset, asmop *source, int soffset, int n, bool *assigned, int *size, bool a_free, bool x_free, bool y_free, bool really_do_it_now)
{
  int i;

#if 0
  D (emitcode("; genCopyStack", "%d %d %d", a_free, x_free, y_free));
#endif

  for (i = 0; i < n;)
    {
      // Same location.
      if (!assigned[i] && !result->aopu.bytes[roffset + i].in_reg && !source->aopu.bytes[soffset + i].in_reg &&
        result->aopu.bytes[roffset + i].byteu.stk == source->aopu.bytes[soffset + i].byteu.stk)
        {
          wassert (*size >= 1);

          assigned[i] = TRUE;
          (*size)--;
          i++;
        }
      // Could transfer two bytes at a time now.
      if (i + 1 < n &&
        !assigned[i] && !assigned[i + 1] &&
        !result->aopu.bytes[roffset + i].in_reg && !result->aopu.bytes[roffset + i + 1].in_reg &&
        !source->aopu.bytes[soffset + i].in_reg && !source->aopu.bytes[soffset + i + 1].in_reg)
        {
          wassert(*size >= 2);

          if (y_free) // Unlike with other operations, loading between y and stk is as efficient as for x, so we try y first here.
            {
              emitcode ("ldw", "y, %s", aopGet2 (source, soffset + i));
              emitcode ("ldw", "%s, y", aopGet2 (result, roffset + i));
            }
          else if (x_free)
            {
              emitcode ("ldw", "x, %s", aopGet2 (source, soffset + i));
              emitcode ("ldw", "%s, x", aopGet2 (result, roffset + i));
            }
          else
            {
              i++;
              continue;
            }
          cost (4, 4);  
          assigned[i] = TRUE;
          assigned[i + 1] = TRUE;
          (*size) -= 2;
          i += 2;
        }
      else
        i++;
    }

  for (i = 0; i < n;)
    {
      // Just one byte to transfer.
      if ((a_free || really_do_it_now) && !assigned[i] &&
        (i + 1 >= n || assigned[i + 1] || really_do_it_now) &&
        !result->aopu.bytes[roffset + i].in_reg && !source->aopu.bytes[soffset + i].in_reg)
        {
          wassert(*size >= 1);
          cheapMove (result, roffset + i, source, soffset + i, !a_free);
          assigned[i] = TRUE;
          (*size)--;
          i++;
        }
      else
        i++;
    }

  wassertl (*size >= 0, "genCopyStack() copied more than there is to be copied.");
}

/*-----------------------------------------------------------------*/
/* genCopy - Copy the value from one reg/stk asmop to another      */
/*-----------------------------------------------------------------*/
static void
genCopy (asmop *result, int roffset, asmop *source, int soffset, int sizex, bool a_dead, bool x_dead, bool y_dead)
{
  int i, regsize, size, n = (sizex < source->size - soffset) ? sizex : (source->size - soffset);
  bool assigned[8] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
  bool a_free, x_free, y_free;

#if 0
  D (emitcode(";  genCopy", "%d %d %d", a_dead, x_dead, y_dead));
#endif

  wassertl (n <= 8, "Invalid size for genCopy().");
  wassertl (aopRS (source), "Invalid source type.");
  wassertl (aopRS (result), "Invalid result type.");

  size = n;
  for (i = 0, regsize = 0; i < n; i++)
    regsize += source->aopu.bytes[soffset + i].in_reg;

  // Do nothing for coalesced bytes.
  for (i = 0; i < n; i++)
    if (result->aopu.bytes[roffset + i].in_reg && source->aopu.bytes[soffset + i].in_reg && result->aopu.bytes[roffset + i].byteu.reg == source->aopu.bytes[soffset + i].byteu.reg)
      {
        assigned[i] = TRUE;
        regsize--;
        size--;
      }

  // Move everything from registers to the stack.
  for (i = 0; i < n;)
    {
      if (i < n - 1 && (aopInReg (source, soffset + i, X_IDX) || aopInReg (source, soffset + i, Y_IDX)) && aopOnStack (result, roffset + i, 2))
        {
          wassert (size >= 2);

          emitcode ("ldw", aopInReg (source, soffset + i, X_IDX) ? "%s, x" : "%s, y", aopGet2 (result, roffset + i));
          cost (2, 2);
          assigned[i] = TRUE;
          assigned[i + 1] = TRUE;
          regsize -= 2;
          size -= 2;
          i += 2;
        }
      else if (aopRS (source) && !aopOnStack (source, soffset + i, 1) && aopOnStack (result, roffset + i, 1))
        {
          wassert (size >= 1);

          if (!aopInReg (source, soffset + i, A_IDX))
            swap_to_a (source->aopu.bytes[soffset + i].byteu.reg->rIdx);
          emit3_o (A_LD, result, roffset + i, ASMOP_A, 0);
          if (!aopInReg (source, soffset + i, A_IDX))
            swap_from_a (source->aopu.bytes[soffset + i].byteu.reg->rIdx);
          assigned[i] = TRUE;
          regsize--;
          size--;
          i++;
        }
      else // This byte is not a register-to-stack copy.
        i++;
    }

  // Copy (stack-to-stack) what we can with whatever free regs we have.
  a_free = a_dead;
  x_free = x_dead;
  y_free = y_dead;
  for (i = 0; i < n; i++)
    {
      asmop *operand;
      int offset;

      if (!assigned[i])
        {
          operand = source;
          offset = soffset + i;
        }
      else
        {
          operand = result;
          offset = roffset + i;
        }

      if (aopInReg (operand, offset, A_IDX))
        a_free = FALSE;
      else if (aopInReg (operand, offset, XL_IDX) || aopInReg (operand, offset, XH_IDX))
        x_free = FALSE;
      else if (aopInReg (operand, offset, YL_IDX) || aopInReg (operand, offset, YH_IDX))
        y_free = FALSE;
    }
  genCopyStack (result, roffset, source, soffset, n, assigned, &size, a_free, x_free, y_free, FALSE);

  // Now do the register shuffling.

  // Try to use exgw x, y.
  if (regsize >= 4)
    {
      int ex[4] = {-1, -1, -1, -1};

      // Find XL and check that it is exchanged with YL, find XH and check that it is exchanged with YH.
      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, XL_IDX) && aopInReg (source, soffset + i, YL_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YL_IDX) && aopInReg (source, soffset + i, XL_IDX))
            ex[1] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, XH_IDX) && aopInReg (source, soffset + i, YH_IDX))
            ex[2] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YH_IDX) && aopInReg (source, soffset + i, XH_IDX))
            ex[3] = i;
        }
      if (ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0 && ex[3] >= 0)
        {
          emitcode ("exgw", "x, y");
          cost (1, 1);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          assigned[ex[2]] = TRUE;
          assigned[ex[3]] = TRUE;
          regsize -= 4;
          size -= 4;
        }
    }

  // Try to use rlwa x.
  if (regsize >= 3)
    {
      int ex[3] = {-1, -1, -1};

      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, XL_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, XH_IDX) && aopInReg (source, soffset + i, XL_IDX))
            ex[1] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, XH_IDX))
            ex[2] = i;
        }
     if (ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0)
        {
          emit3w (A_RLWA, ASMOP_X, 0);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          assigned[ex[2]] = TRUE;
          regsize -= 3;
          size -= 3;
        }
    }

  // Try to use rrwa x.
  if (regsize >= 3)
    {
      int ex[3] = {-1, -1, -1};

      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, XL_IDX) && aopInReg (source, soffset + i, XH_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, XH_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[1] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, XL_IDX))
            ex[2] = i;
        }
     if (ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0)
        {
          emit3w (A_RRWA, ASMOP_X, 0);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          assigned[ex[2]] = TRUE;
          regsize -= 3;
          size -= 3;
        }
    }

  // Try to use rlwa y.
  if (regsize >= 3)
    {
      int ex[3] = {-1, -1, -1};

      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, YL_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YH_IDX) && aopInReg (source, soffset + i, YL_IDX))
            ex[1] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, YH_IDX))
            ex[2] = i;
        }
     if (ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0)
        {
          emit3w (A_RLWA, ASMOP_Y, 0);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          assigned[ex[2]] = TRUE;
          regsize -= 3;
          size -= 3;
        }
    }

  // Try to use rrwa y.
  if (regsize >= 3)
    {
      int ex[3] = {-1, -1, -1};

      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, YL_IDX) && aopInReg (source, soffset + i, YH_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YH_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[1] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, YL_IDX))
            ex[2] = i;
        }
     if (ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0)
        {
          emit3w (A_RRWA, ASMOP_Y, 0);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          assigned[ex[2]] = TRUE;
          regsize -= 3;
          size -= 3;
        }
    }

  // Try to use exg a, xl.
  if (regsize >= 2)
    {
      int ex[2] = {-1, -1};

      // Find XL and check that it is exchanged with XH.
      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, XL_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, XL_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[1] = i;
        }
      if (ex[0] >= 0 && ex[1] >= 0)
        {
          emitcode ("exg", "a, xl");
          cost (1, 1);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          regsize -= 2;
          size -= 2;
        }
    }

  // Try to use exg a, yl.
  if (regsize >= 2)
    {
      int ex[2] = {-1, -1};

      // Find XL and check that it is exchanged with XH.
      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, A_IDX) && aopInReg (source, soffset + i, YL_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YL_IDX) && aopInReg (source, soffset + i, A_IDX))
            ex[1] = i;
        }
      if (ex[0] >= 0 && ex[1] >= 0)
        {
          emitcode ("exg", "a, yl");
          cost (1, 1);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          regsize -= 2;
          size -= 2;
        }
    }

  // Try to use swapw x.
  if (regsize >= 2)
    {
      int ex[2] = {-1, -1};

      // Find XL and check that it is exchanged with XH.
      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, XL_IDX) && aopInReg (source, soffset + i, XH_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, XH_IDX) && aopInReg (source, soffset + i, XL_IDX))
            ex[1] = i;
        }
      if (ex[0] >= 0 && ex[1] >= 0)
        {
          emitcode ("swapw", "x");
          cost (1, 1);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          regsize -= 2;
          size -= 2;
        }
    }

  // Try to use swapw y.
  if (regsize >= 2)
    {
      int ex[2] = {-1, -1};

      // Find YL and check that it is exchanged with YH.
      for (i = 0; i < n; i++)
        {
          if (!assigned[i] && aopInReg (result, roffset + i, YL_IDX) && aopInReg (source, soffset + i, YH_IDX))
            ex[0] = i;
          if (!assigned[i] && aopInReg (result, roffset + i, YH_IDX) && aopInReg (source, soffset + i, YL_IDX))
            ex[1] = i;
        }
      if (ex[0] >= 0 && ex[1] >= 0)
        {
          emitcode ("swapw", "y");
          cost (2, 1);
          assigned[ex[0]] = TRUE;
          assigned[ex[1]] = TRUE;
          regsize -= 2;
          size -= 2;
        }
    }

  while (regsize)
    {
      // Find lowest byte that can be assigned and needs to be assigned.
      for (i = 0; i < n; i++)
        {
          size_t j;

          if (assigned[i] || !source->aopu.bytes[soffset + i].in_reg)
            continue;

          for (j = 0; j < n; j++)
            {
              if (!source->aopu.bytes[soffset + j].in_reg || !result->aopu.bytes[roffset + i].in_reg)
                continue;
              if (!assigned[j] && i != j && result->aopu.bytes[roffset + i].byteu.reg == source->aopu.bytes[soffset + j].byteu.reg)
                goto skip_byte; // We can't write this one without overwriting the source.
            }

          break;                // Found byte that can be written safely.

skip_byte:
          ;
        }

      if (i < n)
        {
          cheapMove (result, roffset + i, source, soffset + i, TRUE);       // We can safely assign a byte.
          regsize--;
          size--;
          assigned[i] = TRUE;
          continue;
        }

      // No byte can be assigned safely (i.e. the assignment is a permutation).
      if (!regalloc_dry_run)
        wassertl (0, "Unimplemented.");
      cost (80, 80);
      return;
    }  

  // Copy (stack-to-stack) what we can with whatever free regs we have now.
  a_free = a_dead;
  x_free = x_dead;
  y_free = y_dead;
  for (i = 0; i < n; i++)
    {
      if (!assigned[i])
        continue;
      if (aopInReg (result, roffset + i, A_IDX))
        a_free = FALSE;
      else if (aopInReg (result, roffset + i, XL_IDX) || aopInReg (result, roffset + i, XH_IDX))
        x_free = FALSE;
      else if (aopInReg (result, roffset + i, YL_IDX) || aopInReg (result, roffset + i, YH_IDX))
        y_free = FALSE;
    }
  genCopyStack (result, roffset, source, soffset, n, assigned, &size, a_free, x_free, y_free, FALSE);

  // Last, move everything from stack to registers.
  for (i = 0; i < n;)
    {
      if (i < n - 1 && (aopInReg (result, roffset + i, X_IDX) || aopInReg (result, roffset + i, Y_IDX)) && aopOnStack (source, soffset + i, 2))
        {
          wassert (size >= 2);
          emitcode ("ldw", aopInReg (result, roffset + i, X_IDX) ? "x, %s" : "y, %s", aopGet2 (source, soffset + i));
          cost (2, 2);
          assigned[i] = TRUE;
          assigned[i + 1] = TRUE;
          size -= 2;
          i += 2;
        }
      // todo: Try to use ldw to load xl, xh, yl, yh when the other half is not in use.
      else if (aopRS (result) && !aopOnStack (result, roffset + i, 1) && aopOnStack (source, soffset + i, 1))
        {
          wassert (size >= 1);
          if (!aopInReg (result, roffset + i, A_IDX))
            swap_to_a (result->aopu.bytes[roffset + i].byteu.reg->rIdx);
          emit3_o (A_LD, ASMOP_A, 0, source, soffset + i);
          if (!aopInReg (result, roffset + i, A_IDX))
            swap_from_a (result->aopu.bytes[roffset + i].byteu.reg->rIdx);
          assigned[i] = TRUE;
          size--;
          i++;
        }
      else // This byte is not a stack-to-register copy.
        i++;
    }

  // Free a reg to copy (stack-to-stack) whatever is left.
  if (size)
    {
      push (ASMOP_A, 0, 1);
      genCopyStack (result, roffset, source, soffset, n, assigned, &size, TRUE, x_free, y_free, TRUE);
      pop (ASMOP_A, 0, 1);
    }

  wassertl (size >= 0, "genCopy() copied more than there is to be copied.");

  // Place leading zeroes.
  for (i = source->size; i < sizex - soffset; i++)
    {
      cheapMove (result, roffset + i, ASMOP_ZERO, 0, !a_free);
      if (aopInReg (result, roffset + i, A_IDX))
        a_free = FALSE;
    }

  if (size)
    {
      if (!regalloc_dry_run)
        {
          wassertl (0, "genCopy failed to completly copy operands.");
          fprintf (stderr, "%d bytes left.\n", size);
          fprintf (stderr, "left type %d source type %d\n", result->type, source->type);
          for (i = 0; i < n ; i++)
            fprintf (stderr, "Byte %d, result in reg %d, source in reg %d. %s assigned.\n", i, result->aopu.bytes[roffset + i].in_reg ? result->aopu.bytes[roffset + i].byteu.reg->rIdx : -1, source->aopu.bytes[soffset + i].in_reg ? source->aopu.bytes[soffset + i].byteu.reg->rIdx : -1, assigned[i] ? "" : "not");
        }
      cost (80, 80);
    }
}

/*-----------------------------------------------------------------*/
/* genMove_o - Copy part of one asmop to another                   */
/*-----------------------------------------------------------------*/
static void
genMove_o (asmop *result, int roffset, asmop *source, int soffset, int size, bool a_dead, bool x_dead, bool y_dead)
{
  int i;

  wassertl (result->type != AOP_LIT, "Trying to write to literal.");
  wassertl (result->type != AOP_IMMD, "Trying to write to immediate.");
  wassertl (roffset + size <= result->size, "Trying to writer beyond end of operand");

#if 0
  D (emitcode(";  genMove_o", "%d %d %d", a_dead, x_dead, y_dead));
#endif

  if (aopRS (result) && aopRS (source))
    {
      genCopy (result, roffset, source, soffset, size, a_dead, x_dead, y_dead);
      return;
    }

  for (i = 0; i < size;)
    {
      if (i + 1 < size && aopInReg (result, roffset + i, X_IDX) && source->type == AOP_LIT && !byteOfVal (source->aopu.aop_lit, soffset + i) && !byteOfVal (source->aopu.aop_lit, soffset + i + 1))
        {
          emitcode ("clrw", "x");
          cost (1, 1);
          i += 2;
        }
      else if (i + 1 < size && aopInReg (result, roffset + i, Y_IDX) && source->type == AOP_LIT && !byteOfVal (source->aopu.aop_lit, soffset + i) && !byteOfVal (source->aopu.aop_lit, soffset + i + 1))
        {
          emitcode ("clrw", "y");
          cost (2, 1);
          i += 2;
        }
      else if ((!aopRS (result) || aopOnStack(result, roffset + i, 1) || aopInReg (result, roffset + i, A_IDX)) && source->type == AOP_LIT && !byteOfVal (source->aopu.aop_lit, soffset + i))
        {
          emit3_o (A_CLR, result, roffset + i, 0, 0);
          i++;
        }
      else if (i + 1 < size && aopInReg (result, roffset + i, X_IDX) && (source->type == AOP_LIT || aopOnStack (source, soffset + i, 2) || source->type == AOP_DIR && soffset + i + 1 < source->size || source->type == AOP_IMMD))
        {emitcode(";", "type %d", source->type);
          emitcode ("ldw", "x, %s", aopGet2 (source, soffset + i));
          cost (3, 2);
          i += 2;
        }
      else if (i + 1 < size && aopInReg (result, roffset + i, Y_IDX) && (source->type == AOP_LIT || aopOnStack (source, soffset + i, 2) || source->type == AOP_DIR && soffset + i + 1 < source->size || source->type == AOP_IMMD))
        {
          emitcode ("ldw", "y, %s", aopGet2 (source, soffset + i));
          cost (4, 2);
          i += 2;
        }
      else if (i + 1 < size && (aopOnStack (result, roffset + i, 2) || result->type == AOP_DIR) && aopInReg (source, soffset + i, X_IDX))
        {
          emitcode ("ldw", "%s, x", aopGet2 (result, roffset + i));
          cost (3, 2);
          i += 2;
        }
      else if (i + 1 < size && (aopOnStack (result, roffset + i, 2) || result->type == AOP_DIR) && aopInReg (source, soffset + i, Y_IDX))
        {
          emitcode ("ldw", "%s, y", aopGet2 (result, roffset + i));
          cost (4, 2);
          i += 2;
        }
      else
        {
          cheapMove (result, roffset + i, source, soffset + i, !(a_dead && (result->regs[A_IDX] >= i || result->regs[A_IDX] == -1) && source->regs[A_IDX] <= i));
          i++;
        }
    }
}

/*-----------------------------------------------------------------*/
/* genMove - Copy the value from one asmop to another              */
/*-----------------------------------------------------------------*/
static void
genMove (asmop *result, asmop *source, bool a_dead, bool x_dead, bool y_dead)
{
  genMove_o (result, 0, source, 0, result->size, a_dead, x_dead, y_dead);
}

/*---------------------------------------------------------------------*/
/* stm8_emitDebuggerSymbol - associate the current code location       */
/*   with a debugger symbol                                            */
/*---------------------------------------------------------------------*/
void
stm8_emitDebuggerSymbol (const char *debugSym)
{
  _G.debugLine = 1;
  emitcode ("", "%s ==.", debugSym);
  _G.debugLine = 0;
}

/*-----------------------------------------------------------------*/
/* genNot - generates code for !                                   */
/*-----------------------------------------------------------------*/
static void
genNot (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  int i;
  int pushed_a = FALSE;

  D (emitcode ("; genNot", ""));

  aopOp (left, ic);
  aopOp (result, ic);

  for (i = 1; i < left->aop->size; i++)
    if (aopInReg (left->aop, i, A_IDX))
      {
        push (ASMOP_A, 0, 1);
        pushed_a = TRUE;
        break;
      }

  if (!regDead (A_IDX, ic) && !pushed_a)
    push (ASMOP_A, 0, 1);

  for (i = 0; i < left->aop->size; i++)
    {
      if (i && aopInReg (left->aop, i, A_IDX))
        {
          emitcode ("ld", "a, (1, sp)");
          cost (2, 1);
        }
      else
        cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

      if (IS_FLOAT (operandType (left)) && i == left->aop->size - 1)
        {
          emitcode ("and", "a, #0x7f");
          cost (2, 1);
        }

      if(!i)
        emit3 (A_SUB, ASMOP_A, ASMOP_ONE);
      else
        emit3 (A_SBC, ASMOP_A, ASMOP_ZERO);
    }

  emit3 (A_CLR, ASMOP_A, 0);
  emit3 (A_RLC, ASMOP_A, 0);

  cheapMove (result->aop, 0, ASMOP_A, 0, FALSE);

  for (i = 1; i < result->aop->size; i++)
    cheapMove (result->aop, 0, ASMOP_ZERO, 0, TRUE);

  if (!regDead (A_IDX, ic))
    pop (ASMOP_A, 0, 1);
  else if (pushed_a)
    adjustStack (1);

  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genCpl - generate code for complement                           */
/*-----------------------------------------------------------------*/
static void
genCpl (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  int left_in_a = 0;
  bool result_in_a = FALSE;
  bool destroyed_a = FALSE;
  bool pushed_a = FALSE;
  bool result_pushed = FALSE;
  int i, size;

  D (emitcode ("; genCpl", ""));

  aopOp (left, ic);
  aopOp (result, ic);

  size = result->aop->size;

  for (i = 1; i < left->aop->size; i++)
    if (aopInReg (left->aop, i, A_IDX))
      {
        left_in_a = i;
        break;
      }

  for (i = 0; i < size;)
    {
      // todo: Complement in source where dead and more efficient.
      if (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX))
        {
          genMove_o (result->aop, i, left->aop, i, 2, (regDead (A_IDX, ic) || pushed_a) && !result_in_a && !(left_in_a > i), regFree (X_IDX, ic), regFree (Y_IDX, ic)); // todo: More aggressively report state of X and Y.

          emit3w_o (A_CPLW, result->aop, i, 0, 0);

          i += 2;
        }
      else
        {
          bool pushed_left = destroyed_a && aopInReg (left->aop, i, A_IDX);

          if ((left_in_a > i || !regDead (A_IDX, ic) || result_in_a) && !pushed_a)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
              if (result_in_a)
                {
                  result_in_a = FALSE;
                  result_pushed = TRUE;
                }
            }

          if (pushed_left && !regDead (A_IDX, ic))
            {
              pop (ASMOP_A, 0, 1);
              pushed_a = FALSE;
            }
          else if (pushed_left)
            {
              emitcode ("ld", "a, (1, sp)");
              cost (2, 1);
            }
          else
            cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

          destroyed_a = TRUE;

          emit3 (A_CPL, ASMOP_A, 0);

          cheapMove (result->aop, i, ASMOP_A, 0, FALSE);

          if (aopInReg (result->aop, i, A_IDX))
            result_in_a = TRUE;

          i++;
        }
    }

  if (pushed_a && !regDead (A_IDX, ic) || result_pushed)
    pop (ASMOP_A, 0, 1);
  else if (pushed_a)
    adjustStack (1);

  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genSub - generates code for subtraction                         */
/*-----------------------------------------------------------------*/
static void
genSub (const iCode *ic, asmop *result_aop, asmop *left_aop, asmop *right_aop)
{
  int size, i;
  bool started;
  bool pushed_a = FALSE;
  int left_in_a = -1;
  bool result_in_a = FALSE;

  size = result_aop->size;

  for (i = 0; i < size; i++)
    if (aopInReg (left_aop, i, A_IDX))
      {
        left_in_a = i;
        break;
      }

  for (i = 0, started = FALSE; i < size;)
    {
      if (0) // todo: Use subw, incw, decw where it provides an advantage.
        ;
      else if (aopInReg (right_aop, i, A_IDX)) // todo: Be more flexible and handle this.
        {
          if (!regalloc_dry_run)
            wassertl (0, "Unimplemented subtraction operand.");
          cost (80, 80);
          i++;
        }
      else
        {
          if ((!regDead (A_IDX, ic) || left_in_a > i || result_in_a) && !pushed_a)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
              result_in_a = FALSE;
            }

          if (left_in_a == i && pushed_a)
            {
              emitcode ("ld", "a, (1, sp)");
              cost (2, 1);
            }
          else
            cheapMove (ASMOP_A, 0, left_aop, i, FALSE);

          if (!started && right_aop->type == AOP_LIT && !byteOfVal (right_aop->aopu.aop_lit, i))
            {
              // Skip over this byte.
            }
          else // todo: Use dec / inc.
            {
              const asmop *right_stacked = NULL;
              int right_offset;

              right_stacked = stack_aop (right_aop, i, &right_offset);

              if (!right_stacked)
                emit3_o (started ? A_SBC : A_SUB, ASMOP_A, 0, right_aop, i);
              else
                {
                  emitcode (started ? "sbc" : "sub", "a, (#%d, sp)", right_offset);
                  cost (2, 1);
                }

              if (right_stacked)
                pop (right_stacked, 0, 2);

              started = TRUE;
            }

          cheapMove (result_aop, i, ASMOP_A, 0, FALSE);

          if (aopInReg (result_aop, i, A_IDX))
            result_in_a = TRUE;
            
          i++;
        }
    }

  if (pushed_a && !result_in_a)
    pop (ASMOP_A, 0, 1);
  else if (pushed_a)
    adjustStack (1);
}

/*-----------------------------------------------------------------*/
/* genUminus - generates code for unary minus                      */
/*-----------------------------------------------------------------*/
static void
genUminusFloat (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);

  D (emitcode ("; genUminusFloat", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  genMove_o (result->aop, 0, left->aop, 0, result->aop->size - 1, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

  // todo: Use bcpl, rlcw with ccf, only save A when necessary
  push (ASMOP_A, 0, 1);

  cheapMove (ASMOP_A, 0, left->aop, left->aop->size - 1, FALSE);
  emitcode ("xor", "a, #0x80");
  cost (2, 1);
  cheapMove (result->aop, result->aop->size - 1, ASMOP_A, 0, FALSE);

  pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genUminus - generates code for unary minus                      */
/*-----------------------------------------------------------------*/
static void
genUminus (const iCode *ic)
{
  if (IS_FLOAT (operandType (IC_LEFT (ic))))
    {
      genUminusFloat (ic);
      return;
    }

  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);

  D (emitcode ("; genUminus", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  genSub (ic, result->aop, ASMOP_ZERO, left->aop);

  freeAsmop (left);
  freeAsmop (result);
}

static void
saveRegsForCall (const iCode * ic)
{
  if (_G.saved && !regalloc_dry_run)
    return;

  //if (!regDead (C_IDX, ic))
  //  push (ASMOP_C, 0, 1);

  if (!regDead (A_IDX, ic))
    push (ASMOP_A, 0, 1);

  if (!regDead (X_IDX, ic))
    push (ASMOP_X, 0, 2);

  if (!regDead (Y_IDX, ic))
    push (ASMOP_Y, 0, 2);

  _G.saved = TRUE;
}

/*-----------------------------------------------------------------*/
/* genIpush - generate code for pushing this gets a little complex */
/*-----------------------------------------------------------------*/
static void
genIpush (const iCode * ic)
{
  int size, offset = 0;
  iCode *walk;

  D (emitcode ("; genIPush", ""));

  if (!ic->parmPush)
    {
      wassertl (0, "Encountered an unsupported spill push.");
      return;
    }

  /* Caller saves, and this is the first iPush. */
  /* Scan ahead until we find the function that we are pushing parameters to.
     Count the number of addSets on the way to figure out what registers
     are used in the send set.
   */
  for (walk = ic->next; walk->op != CALL && walk->op != PCALL; walk = walk->next);
  if (!_G.saved  && !regalloc_dry_run /* Cost is counted at CALL or PCALL instead */ )
    saveRegsForCall (walk);

  /* then do the push */
  aopOp (IC_LEFT (ic), ic);

  for (size = IC_LEFT (ic)->aop->size, offset = 0; size;)
    {
      // todo: For AOP_IMMD, if X is free, when optimizing for code size, ldw x, m  pushw x is better than push m push m+1.
      // todo: Use x (or even y) when free for stack operands.

      if (aopInReg (IC_LEFT (ic)->aop, offset, X_IDX) || aopInReg (IC_LEFT (ic)->aop, offset, Y_IDX))
        {
          push (IC_LEFT (ic)->aop, offset, 2);
          offset += 2;
          size -= 2;
        }
      else if (IC_LEFT (ic)->aop->type == AOP_LIT || aopInReg (IC_LEFT (ic)->aop, offset, A_IDX) || IC_LEFT (ic)->aop->type == AOP_DIR || IC_LEFT (ic)->aop->type == AOP_IMMD)
        {
          push (IC_LEFT (ic)->aop, offset, 1);
          offset++;
          size--;
        }
      else
        {
          cheapMove (ASMOP_A, 0, IC_LEFT (ic)->aop, offset, FALSE);
          push (ASMOP_A, 0, 1);
          offset++;
          size--;
        }
    }

  freeAsmop (IC_LEFT (ic));
}

static void
emitCall (const iCode *ic, bool ispcall)
{
  bool SomethingReturned, bigreturn;
  sym_link *dtype = operandType (IC_LEFT (ic));
  sym_link *etype = getSpec (dtype);
  sym_link *ftype = IS_FUNCPTR (dtype) ? dtype->next : dtype;

  saveRegsForCall (ic);

  /* Return value of big type or returning struct or union. */
  bigreturn = (getSize (ftype->next) > 4);
  if (bigreturn)
    {
      symbol *sym;

      wassertl (IC_RESULT (ic), "Unused return value in call to function returning large type.");

      sym = OP_SYMBOL (IC_RESULT (ic));
      aopOp (IC_RESULT (ic), ic);

      if (IC_RESULT (ic)->aop->type != AOP_STK)
        {
          if (!regalloc_dry_run)
            {
              fprintf (stderr, "Type %d\n", IC_RESULT (ic)->aop->type);
              wassertl (0, "Unimplemented return value size / type combination.");
            }
          cost (80, 80);
        }

      emitcode ("ldw", "x, sp");
      emitcode ("addw", "x, #%d", sym->stack + _G.stack.pushed + 1);
      cost (2 + 4, 1 + 2);
      push (ASMOP_X, 0, 2);

      freeAsmop (IC_RESULT (ic));
    }

  if (ispcall)
    {
      operand *left = IC_LEFT (ic);

      aopOp (left, ic);

      if (left->aop->type == AOP_LIT || left->aop->type == AOP_IMMD)
        {
          emitcode ("call", "%s", aopGet2 (left->aop, 0));
          cost (3, 4);
        }
      else
        {
          genMove (ASMOP_X, left->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));
          
          emitcode ("call", "(x)");
          cost (1, 4);
        }
      freeAsmop (left);
    }
  else
    {
      if (IS_LITERAL (etype))
        {
          emitcode ("call", "0x%04X", ulFromVal (OP_VALUE (IC_LEFT (ic))));
          cost (3, 4);
        }
      else
        {
          bool jump = (!ic->parmBytes && IFFUNC_ISNORETURN (OP_SYMBOL (IC_LEFT (ic))->type));
          emitcode (jump ? "jp" : "call", "%s",
            (OP_SYMBOL (IC_LEFT (ic))->rname[0] ? OP_SYMBOL (IC_LEFT (ic))->rname : OP_SYMBOL (IC_LEFT (ic))->name));
          cost (3, jump ? 1 : 4);
        }
    }

  SomethingReturned = (IS_ITEMP (IC_RESULT (ic)) &&
                       (OP_SYMBOL (IC_RESULT (ic))->nRegs || OP_SYMBOL (IC_RESULT (ic))->spildir))
                       || IS_TRUE_SYMOP (IC_RESULT (ic));

  if (ic->parmBytes || bigreturn)
    adjustStack (ic->parmBytes + bigreturn * 2);

  /* if we need assign a result value */
  if (SomethingReturned && !bigreturn)
    {
      aopOp (IC_RESULT (ic), ic);

      wassert (getSize (ftype->next) == 1 || getSize (ftype->next) == 2 || getSize (ftype->next) == 4);

      genMove (IC_RESULT (ic)->aop, getSize (ftype->next) == 1 ? ASMOP_A : (getSize (ftype->next) == 2 ? ASMOP_X : ASMOP_XY), TRUE, TRUE, TRUE);

      freeAsmop (IC_RESULT (ic));
    }

  // Restore regs.
  if (!regDead (Y_IDX, ic))
    if (regDead (YH_IDX, ic))
        {
          adjustStack (1);
          swap_to_a (YL_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(YL_IDX);
        }
      else if (regDead (YL_IDX, ic))
        {
          swap_to_a (YH_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(YH_IDX);
          adjustStack (1);
        }
      else
        pop (ASMOP_Y, 0, 2);

  if (!regDead (X_IDX, ic))
    {
      if (regDead (XH_IDX, ic))
        {
          adjustStack (1);
          swap_to_a (XL_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XL_IDX);
        }
      else if (regDead (XL_IDX, ic))
        {
          swap_to_a (XH_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XH_IDX);
          adjustStack (1);
        }
      else
        pop (ASMOP_X, 0, 2);
    }

  if (!regDead (A_IDX, ic))
    pop (ASMOP_A, 0, 1);

  //if (!regDead (C_IDX, ic))
  //  pop (ASMOP_C, 0, 1);

  _G.saved = FALSE;
}

/*-----------------------------------------------------------------*/
/* genCall - generates a call statement                            */
/*-----------------------------------------------------------------*/
static void
genCall (const iCode *ic)
{
  D (emitcode ("; genCall", ""));

  emitCall (ic, FALSE);
}

/*-----------------------------------------------------------------*/
/* genPCall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void
genPCall (const iCode *ic)
{
  D (emitcode ("; genPcall", ""));

  emitCall (ic, TRUE);
}


/*---------------------------------------------------------------------*/
/* genCritical - mask interrupts until important block completes       */
/*---------------------------------------------------------------------*/

static void
genCritical (iCode * ic)
{
  emitcode("sim", "");
  cost (1, 1);
}

static void
genEndCritical (iCode * ic)
{
  emitcode("rim", "");
  cost (1, 1);
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void
genFunction (iCode *ic)
{
  const symbol *sym = OP_SYMBOL_CONST (IC_LEFT (ic));
  sym_link *ftype = operandType (IC_LEFT (ic));

  _G.stack.pushed = 0;

  /* create the function header */
  emitcode (";", "-----------------------------------------");
  emitcode (";", " function %s", sym->name);
  emitcode (";", "-----------------------------------------");

  D (emitcode (";", stm8_assignment_optimal ? "Register assignment is optimal." : "Register assignment might be sub-optimal."));
  D (emitcode (";", "Stack space usage: %d bytes.", sym->stack));

  emitcode ("", "%s:", sym->rname);
  genLine.lineCurr->isLabel = 1;


  if (IFFUNC_ISNAKED(ftype))
  {
      emitcode(";", "naked function: no prologue.");
      return;
  }

  if (IFFUNC_ISCRITICAL (ftype))
      genCritical (NULL);

  /* adjust the stack for the function */
  if (sym->stack)
    adjustStack (-sym->stack);
}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void
genEndFunction (iCode *ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));

  D (emitcode ("; genEndFunction", ""));

  wassert (!regalloc_dry_run);

  if (IFFUNC_ISNAKED(sym->type))
  {
      D (emitcode (";", "naked function: no epilogue."));
      if (options.debug && currFunc)
        debugFile->writeEndFunction (currFunc, ic, 0);
      return;
  }

  /* adjust the stack for the function */
  if (sym->stack)
    adjustStack (sym->stack);

  wassertl (!_G.stack.pushed, "Unbalanced stack.");

  if (IFFUNC_ISCRITICAL (sym->type))
      genEndCritical (NULL);

  if (IFFUNC_ISISR (sym->type))
    {
      /* if debug then send end of function */
      if (options.debug && currFunc)
        {
          debugFile->writeEndFunction (currFunc, ic, 1);
        }

      emitcode ("iret", "");
      cost (1, 11);
    }
  else
    {
      /* if debug then send end of function */
      if (options.debug && currFunc)
        debugFile->writeEndFunction (currFunc, ic, 1);

      emitcode ("ret", "");
      cost (1, 4);
    }
}

/*-----------------------------------------------------------------*/
/* genReturn - generate code for return statement                  */
/*-----------------------------------------------------------------*/
static void
genReturn (const iCode *ic)
{
  operand *left = IC_LEFT (ic);
  int size, i;
  bool stacked = FALSE;

  D (emitcode ("; genReturn", ""));

  /* if we have no return value then
     just generate the "ret" */
  if (!IC_LEFT (ic))
    goto jumpret;

  /* we have something to return then
     move the return value into place */
  aopOp (left, ic);
  size = left->aop->size;

  switch (size)
    {
    case 1:
      cheapMove (ASMOP_A, 0, left->aop, 0, FALSE);
      break;
    case 2:
      genMove (ASMOP_X, left->aop, TRUE, TRUE, TRUE);
      break;
    case 4:
      genMove (ASMOP_XY, left->aop, TRUE, TRUE, TRUE);
      break;
    default:
      wassertl (size > 4, "Return not implemented for return value of this size.");

      for(i = 0; i < size; i++)
        if (aopInReg (left->aop, i, XL_IDX) || aopInReg (left->aop, i, XH_IDX))
          {
            push (ASMOP_X, 0, 2);
            stacked = TRUE;
            break;
          }

      emitcode ("ldw", "x, sp");
      cost (1, 1);
      emitcode ("addw", "x, #0x%04x", _G.stack.pushed);
      cost (3, 2);

      // Clear a first.
      for(i = 0; i < size; i++)
        if (aopInReg (left->aop, i, A_IDX))
          {
            emitcode ("ld", "(#%d, x), a", size - 1 - i);
            cost (2, 1);
            break;
          }

      for(i = 0; i < size;)
        {
          if (aopInReg (left->aop, i, Y_IDX))
            {
              emitcode ("ldw", "(#%d, x), y", size - 2 - i);
              cost (2, 2);
              i += 2;
            }
          else if (aopInReg (left->aop, i, XL_IDX) || aopInReg (left->aop, i, XH_IDX))
            {
              emitcode ("ld", "a, (#%d, sp)", (int)(aopInReg (left->aop, i, XL_IDX)));
              emitcode ("ld", "(#%d, x), a", size - 1 - i);
              cost (4, 2);
              i++;
            }
          else if (!aopInReg (left->aop, i, A_IDX))
            {
              cheapMove (ASMOP_A, 0, left->aop, i, FALSE);
              emitcode ("ld", "(#%d, x), a", size - 1 - i);
              cost (2, 1);
              i++;
            }
          else
            i++;
        }

      if (stacked)
        adjustStack (2);
    }

  freeAsmop (left);

jumpret:
  /* generate a jump to the return label
     if the next is not the return statement */
  if (!(ic->next && ic->next->op == LABEL && IC_LABEL (ic->next) == returnLabel))
    {
      if (!regalloc_dry_run)
        emitcode ("jp", "%05d$", labelKey2num (returnLabel->key));
      cost (3, 1);
    }
}

/*-----------------------------------------------------------------*/
/* genLabel - generates a label                                    */
/*-----------------------------------------------------------------*/
static void
genLabel (const iCode *ic)
{
  D (emitcode ("; genLabel", ""));

  /* special case never generate */
  if (IC_LABEL (ic) == entryLabel)
    return;

  emitLabel (IC_LABEL (ic));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a jump                                      */
/*-----------------------------------------------------------------*/
static void
genGoto (const iCode *ic)
{
  D (emitcode ("; genGoto", ""));

  emitcode ("jp", "%05d$", labelKey2num (IC_LABEL (ic)->key));
  cost (3, 1);
}

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void
genPlus (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  int size, i;
  bool started;
  bool pushed_a = FALSE;
  int left_in_a = -1;
  bool result_in_a = FALSE;

  D (emitcode ("; genPlus", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  size = result->aop->size;

  /* Swap if left is literal or right is in A. */
  if (left->aop->type == AOP_LIT || left->aop->type == AOP_IMMD || aopInReg (right->aop, 0, A_IDX)) // todo: Swap in more cases when right in reg, left not. Swap individually per-byte.
    {
      operand *t = right;
      right = left;
      left = t;
    }

  for (i = 0; i < size; i++)
    if (aopInReg (left->aop, i, A_IDX))
      {
        left_in_a = i;
        break;
      }
  
  for(i = 0, started = FALSE; i < size;)
    {// Todo: incw /decw x twice is cheaper than addw x, #2 / addw x, #254. 16-bit operation in dead source might be cheaper than add.
      // We can use incw / decw only for the only, top non-zero word, since it neither takes into account an existing carry nor does it update the carry.
      if (!started && i == size - 2 &&
        (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX)) &&
        right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 1 && byteOfVal (right->aop->aopu.aop_lit, i + 1) == 0)
        {
          bool x = aopInReg (result->aop, i, X_IDX);
          genMove_o (x ? ASMOP_X : ASMOP_Y, 0, left->aop, i, 2,
            pushed_a || regDead (A_IDX, ic) && left_in_a <= i && !result_in_a, x, !x);
          emit3w (A_INCW, x ? ASMOP_X : ASMOP_Y, 0);
          cost (x ? 1 : 2, 1);
          started = TRUE;
          i += 2;
        }
      else if (!started && i == size - 2 &&
        (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX)) &&
        right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 255 && byteOfVal (right->aop->aopu.aop_lit, i + 1) == 0)
        {
          bool x = aopInReg (result->aop, i, X_IDX);
          genMove_o (x ? ASMOP_X : ASMOP_Y, 0, left->aop, i, 2,
            pushed_a || regDead (A_IDX, ic) && left_in_a <= i && !result_in_a, x, !x);
          emit3w (A_DECW, x ? ASMOP_X : ASMOP_Y, 0);
          started = TRUE;
          i += 2;
        }
      else if (!started &&
        (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX)) &&
        (right->aop->type == AOP_LIT || right->aop->type == AOP_IMMD || aopOnStack (right->aop, i, 2)))
        {
          bool x = aopInReg (result->aop, i, X_IDX);
          genMove_o (x ? ASMOP_X : ASMOP_Y, 0, left->aop, i, 2,
            pushed_a || regDead (A_IDX, ic) && left_in_a <= i && !result_in_a, x, !x);
          if (right->aop->type != AOP_LIT || byteOfVal (right->aop->aopu.aop_lit, i) || byteOfVal (right->aop->aopu.aop_lit, i + 1))
            {
              emitcode ("addw", x ? "x, %s" : "y, %s", aopGet2 (right->aop, i));
              cost ((x || aopOnStack (right->aop, 0, 2)) ? 3 : 4, 2);
              started = TRUE;
            }
          i += 2;
        }
      else if (right->aop->type == AOP_REG || right->aop->type == AOP_REGSTK && !aopOnStack (right->aop, i, 1)) //todo: Implement handling of right operands that can't be directly added to a.
        {
          if (!regalloc_dry_run)
            wassertl (0, "Unimplemented addition operand.");
          cost (80, 80);
          i++;
        }
      else
        {
          if ((!regDead (A_IDX, ic) || left_in_a > i || result_in_a) && !pushed_a)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
              result_in_a = FALSE;
            }

          if (left_in_a == i && pushed_a)
            {
              emitcode ("ld", "a, (1, sp)");
              cost (2, 1);
            }
          else
            cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

          if (!started && right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
            {
              // Skip over this byte.
            }
          // We can use inc / dec only for the only, top non-zero byte, since it neither takes into account an existing carry nor does it update the carry.
          else if (!started && i == size - 1 && right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 1)
            {
              emit3 (A_INC, ASMOP_A, 0);
              started = TRUE;
            }
          else if (!started && i == size - 1 && right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 255)
            {
              emit3 (A_DEC, ASMOP_A, 0);
              started = TRUE;
            }
          else
            {
              emit3_o (started ? A_ADC : A_ADD, ASMOP_A, 0, i < right->aop->size ? right->aop : ASMOP_ZERO, i);
              started = TRUE;
            }

          cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
          if (aopInReg (result->aop, i, A_IDX))
            result_in_a = TRUE;

          i++;
        }
    }

  if (pushed_a && !result_in_a)
    pop (ASMOP_A, 0, 1);
  else if (pushed_a)
    adjustStack (1);

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genMult - generates code for multiplication                     */
/*-----------------------------------------------------------------*/
static void
genMult (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  bool use_y;

  D (emitcode ("; genMult", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  if (left->aop->size > 1 || right->aop->size > 1 || result->aop->size > 2)
    wassertl (0, "Large multiplication is handled through support function calls.");

  /* Swap if left is literal or right is in A. */
  if (aopInReg (left->aop, 0, A_IDX) || aopInReg (right->aop, 0, XL_IDX) || aopInReg (right->aop, 0, YL_IDX) && !aopInReg (result->aop, 0, X_IDX)) // todo: Swap in more cases when right in reg, left not.
    {
      operand *t = right;
      right = left;
      left = t;
    }

  use_y = aopInReg (result->aop, 0, Y_IDX) || aopInReg (left->aop, 0, YL_IDX) && !aopInReg (result->aop, 0, X_IDX);

  if (!regDead (use_y ? Y_IDX : X_IDX, ic))
    push (use_y ? ASMOP_Y : ASMOP_X, 0, 2);
  if (!regDead (A_IDX, ic))
    push (ASMOP_A, 0, 1);

  cheapMove (use_y ? ASMOP_Y : ASMOP_X, 0, left->aop, 0, FALSE); // todo: Allow use of a.
  cheapMove (ASMOP_A, 0, right->aop, 0, TRUE);

  emitcode ("mul", use_y ? "y, a" : "x, a");
  cost (1 + use_y, 4);

  genMove (result->aop, use_y ? ASMOP_Y : ASMOP_X,  TRUE, !use_y || regDead (X_IDX, ic), use_y || regDead (Y_IDX, ic)); // todo: Allow use of x, y.

  if (!regDead (A_IDX, ic))
    pop (ASMOP_A, 0, 1);
  if (!regDead (use_y ? Y_IDX : X_IDX, ic))
    {
      if (regDead (XH_IDX, ic))
        {
          adjustStack (1);
          swap_to_a (XL_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XL_IDX);
        }
      else if (regDead (XL_IDX, ic))
        {
          swap_to_a (XH_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XH_IDX);
          adjustStack (1);
        }
      pop (use_y ? ASMOP_Y : ASMOP_X, 0, 2);
    }

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genDivMod2 - generates code for unsigned division               */
/* any operands and results of up to 2 bytes                       */
/*-----------------------------------------------------------------*/
static void
genDivMod2 (const iCode *ic)
{
  D (emitcode ("; genDivMod2", ""));

  wassertl (0, "Unimplemented");
}

/*-----------------------------------------------------------------*/
/* genDivMod1 - generates code for unsigned division               */
/* left operand up to 2 bytes                                      */
/* right operand 1 byte                                            */
/* result up to 2 bytes for division, 1 byte for modulo            */
/*-----------------------------------------------------------------*/
static void
genDivMod1 (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  bool use_y;

  use_y = ic->op == '/' && aopInReg (result->aop, 0, Y_IDX) || aopInReg (left->aop, 0, YL_IDX) && !(ic->op == '/' && aopInReg (result->aop, 0, X_IDX));

  if (!regDead (use_y ? Y_IDX : X_IDX, ic))
    push (use_y ? ASMOP_Y : ASMOP_X, 0, 2);
  if (!regDead (A_IDX, ic))
    push (ASMOP_A, 0, 1);

  genMove_o (use_y ? ASMOP_Y : ASMOP_X, 0, left->aop, 0, 2, FALSE, FALSE, FALSE); // todo: Allow more.
  cheapMove (ASMOP_A, 0, right->aop, 0, TRUE);

  emitcode ("div", use_y ? "y, a" : "x, a");
  cost (1 + use_y, 17);

  genMove_o (result->aop, 0, ic->op == '/' ? (use_y ? ASMOP_Y : ASMOP_X) : ASMOP_A, 0, result->aop->size, TRUE, FALSE, FALSE); // todo: Allow more.

  if (!regDead (A_IDX, ic))
    pop (ASMOP_A, 0, 1);
  if (!regDead (use_y ? Y_IDX : X_IDX, ic))
    {
      if (regDead (XH_IDX, ic))
        {
          adjustStack (1);
          swap_to_a (XL_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XL_IDX);
        }
      else if (regDead (XL_IDX, ic))
        {
          swap_to_a (XH_IDX);
          pop (ASMOP_A, 0, 1);
          swap_from_a(XH_IDX);
          adjustStack (1);
        }
      pop (use_y ? ASMOP_Y : ASMOP_X, 0, 2);
    }
}

/*-----------------------------------------------------------------*/
/* genDivMod - generates code for unsigned division                */
/*-----------------------------------------------------------------*/
static void
genDivMod (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);

  D (emitcode ("; genDivMod", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  if (result->aop->size <= (ic->op == '/' ? 2 : 1) && left->aop->size <= 2 && right->aop->size <= 1)
    genDivMod1(ic);
  else
    genDivMod2(ic);

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genMinus - generates code for minus                             */
/*-----------------------------------------------------------------*/
static void
genMinus (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);

  D (emitcode ("; genMinus", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  genSub (ic, result->aop, left->aop, right->aop);

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
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
        return "jrslt";
      else
        return "jrc";
    case '>':
      if (sign)
        return "jrsgt";
      else
        return "jrugt";
    case LE_OP:
      if (sign)
        return "jrsle";
      else
        return "jrule";
    case GE_OP:
      if (sign)
        return "jrsge";
      else
        return "jrnc";
    case NE_OP:
      return "jrne";
    case EQ_OP:
      return "jreq";
    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "opcode not a comparison");
    }
  return "brn";
}

/*------------------------------------------------------------------*/
/* genCmp :- greater or less than (and maybe with equal) comparison */
/*------------------------------------------------------------------*/
static void
genCmp (iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  sym_link *letype, *retype;
  int sign, opcode;
  int size, i;
  bool exchange = FALSE;

  D (emitcode ("; genCmp", ""));

  opcode = ic->op;
  sign = 0;
  if (IS_SPEC (operandType (left)) && IS_SPEC (operandType (right)))
    {
      letype = getSpec (operandType (left));
      retype = getSpec (operandType (right));
      sign = !(SPEC_USIGN (letype) | SPEC_USIGN (retype));
    }

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  size = max (left->aop->size, right->aop->size);

  /* Prefer literal operand on right */
  if (left->aop->type == AOP_LIT ||
    right->aop->type != AOP_LIT && left->aop->type == AOP_DIR ||
    (aopInReg (right->aop, 0, A_IDX) || aopInReg (right->aop, 0, X_IDX) || aopInReg (right->aop, 0, Y_IDX)) && left->aop->type == AOP_STK)
    exchange = TRUE;

  /* Cannot do multibyte signed comparison, except for 2-byte using cpw */
  if (size > 1 && !(size == 2 && (right->aop->type == AOP_LIT || right->aop->type == AOP_DIR || right->aop->type == AOP_STK)))
    {
      if (exchange && (opcode ==  '<' || opcode == GE_OP))
        exchange = FALSE;
      if (!exchange && (opcode == '>' || opcode == LE_OP))
        exchange = TRUE;
    }

  if (exchange)
    {
      operand *temp = left;
      left = right;
      right = temp;
      opcode = exchangedCmp (opcode);
    }

  if (size == 1 &&
    (right->aop->type == AOP_LIT || right->aop->type == AOP_DIR || right->aop->type == AOP_STK) &&
    aopInReg (left->aop, 0, A_IDX))
    emit3 (A_CP, ASMOP_A, right->aop);
  else if (size == 2 && (right->aop->type == AOP_LIT || right->aop->type == AOP_DIR || right->aop->type == AOP_STK))
    {
      if (aopInReg (left->aop, 0, Y_IDX) && right->aop->type == AOP_STK)
        {
          if (regDead (X_IDX, ic) && regDead (Y_IDX, ic))
            {
              emitcode ("ldw", "x, y");
              emitcode ("cpw", "x, %s", aopGet2 (right->aop, 0));
              cost (3, 3);
            }
          else
            {
              emitcode ("exgw", "x, y");
              emitcode ("cpw", "x, %s", aopGet2 (right->aop, 0));
              emitcode ("exgw", "x, y");
              cost (4, 4);
            }
        }
      else
        {
          bool save_x = !regDead (X_IDX, ic) && !aopInReg (left->aop, 0, X_IDX) && !aopInReg (left->aop, 0, Y_IDX);
          if (save_x)
            push (ASMOP_X, 0, 2);

          if (!aopInReg (left->aop, 0, Y_IDX))
            genMove (ASMOP_X, left->aop, regDead (A_IDX, ic), TRUE, regDead (Y_IDX, ic));

          emitcode ("cpw", aopInReg (left->aop, 0, Y_IDX) ? "y, %s" : "x, %s", aopGet2 (right->aop, 0));
          cost (3 + aopInReg (left->aop, 0, Y_IDX), 2);

          if (save_x)
            pop (ASMOP_X, 0, 2);
        }
    }
  else
    {
      bool pushed_a = FALSE;

      for (i = 0; i < size; i++)
        if (i && aopInReg (left->aop, i, A_IDX) || aopInReg (right->aop, i, A_IDX))
          {
            push (ASMOP_A, 0, 1);
            pushed_a = TRUE;
            break;
          }

      if (!regDead (A_IDX, ic) && !pushed_a)
        push (ASMOP_A, 0, 1);

      for (i = 0; i < size; i++)
        {
          const asmop *right_stacked = NULL;
          int right_offset;

          right_stacked = stack_aop (right->aop, i, &right_offset);

          if (i && aopInReg (left->aop, i, A_IDX) && regDead (A_IDX, ic))
            {
              pop (ASMOP_A, 0, 1);
              pushed_a = FALSE;
            }
          else if (i && aopInReg (left->aop, i, A_IDX))
            {
              emitcode ("ld", "a, (1, sp)");
              cost (2, 1);
            }
          else
            cheapMove (ASMOP_A, 0, left->aop, i, FALSE);
          
          if (right_stacked || aopInReg (right->aop, i, A_IDX))
            {
              emitcode (i ? "sbc" : "sub", "a, (%d, sp)", right_stacked ? right_offset : 1);
              cost (2, 1);
            }
          else
            emit3_o (i ? A_SBC : A_SUB, ASMOP_A, 0, right->aop, i);

          if (right_stacked)
            pop (right_stacked, 0, 2);
        }

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);
      else if (pushed_a)
        adjustStack (1);
    }

  {
    symbol *tlbl1 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
    symbol *tlbl2 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));

    if (tlbl1)
      emitcode (branchInstCmp (opcode, sign), "%05d$", labelKey2num (tlbl1->key));
    cheapMove (result->aop, 0, ASMOP_ZERO, 0, !regDead (A_IDX, ic));
    if (tlbl2)
      emitcode ("jp", "%05d$", labelKey2num (tlbl2->key));
    cost (3, 1);
    emitLabel (tlbl1);
    cheapMove (result->aop, 0, ASMOP_ONE, 0, !regDead (A_IDX, ic));
    emitLabel (tlbl2);
  }

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genCmpEQorNE - equal or not equal comparison                    */
/*-----------------------------------------------------------------*/
static void
genCmpEQorNE (iCode *ic)
{
  operand *left, *right, *result;
  int opcode;
  int size, i;
  symbol *tlbl_NE = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  symbol *tlbl = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  bool pushed_a = FALSE;

  D (emitcode ("; genCmpEQorNE", ""));

  result = IC_RESULT (ic);
  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);

  opcode = ic->op;

  /* assign the amsops */
  aopOp (left, ic);
  aopOp (right, ic);
  aopOp (result, ic);

  size = max (left->aop->size, right->aop->size);

  for (i = 0; i < size;)
    {
      /* Prefer literal operand on right */
      if (left->aop->type == AOP_LIT ||
        right->aop->type != AOP_LIT && left->aop->type == AOP_DIR ||
        (aopInReg (right->aop, 0, A_IDX) || aopInReg (right->aop, 0, X_IDX) || aopInReg (right->aop, 0, Y_IDX)) && aopOnStack (right->aop, i, 1))
        {
          operand *temp = left;
          left = right;
          right = temp;
        }

      if (i <= size - 2 && (right->aop->type == AOP_LIT || right->aop->type == AOP_DIR || aopOnStack (right->aop, i, 2)))
        {
          bool x_dead = regDead (X_IDX, ic) && left->aop->regs[XL_IDX] <= i + 1 && left->aop->regs[XH_IDX] <= i + 1 && right->aop->regs[XL_IDX] <= i + 1 && right->aop->regs[XH_IDX] <= i + 1;
          if (aopInReg (left->aop, i, Y_IDX) && aopOnStack (right->aop, i, 2))
            {
              if (x_dead)
                {
                  emitcode ("ldw", "x, y");
                  emitcode ("cpw", "x, %s", aopGet2 (right->aop, i));
                  cost (3, 3);
                }
              else
                {
                  emitcode ("exgw", "x, y");
                  emitcode ("cpw", "x, %s", aopGet2 (right->aop, i));
                  emitcode ("exgw", "x, y");
                  cost (4, 4);
                }
            }
          else
            {
              if (!x_dead && !aopInReg (left->aop, i, X_IDX))
                push (ASMOP_X, 0, 2);

              genMove_o (ASMOP_X, 0, left->aop, i, 2, regDead (A_IDX, ic), TRUE, regDead (Y_IDX, ic));

              emitcode ("cpw", aopInReg (left->aop, i, Y_IDX) ? "y, %s" : "x, %s", aopGet2 (right->aop, i));
              cost (3 + aopInReg (left->aop, i, Y_IDX), 2);

              if (!x_dead && !aopInReg (left->aop, i, X_IDX))
                pop (ASMOP_X, 0, 2);
            }

          i += 2;
        }
      else if (right->aop->type == AOP_LIT || right->aop->type == AOP_DIR || aopOnStack (right->aop, i, 1))
        {
          if (!regDead (A_IDX, ic) && !aopInReg (left->aop, i, A_IDX) && !pushed_a)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }

          cheapMove (ASMOP_A, 0, left->aop, i, FALSE);
          emit3_o (A_CP, ASMOP_A, 0, right->aop, i);

          i++;
        }
      else
        {
          if (!regalloc_dry_run)
            {
              fprintf(stderr, "ltype %d, lsize %d, rtype %d, rsize %d\n", left->aop->type, left->aop->size, right->aop->type, right->aop->size);
              wassertl (0, "Unimplemented comparison operands.");
            }
          cost (80, 80);

          i++;
        }
      if (tlbl_NE)
        emitcode ("jrne", "%05d$", labelKey2num (tlbl_NE->key));
    }

  if (pushed_a)
    pop (ASMOP_A, 0 ,1);

  wassertl (result->aop->size == 1, "Unimplemented result size.");

  cheapMove (result->aop, 0, opcode == EQ_OP ? ASMOP_ONE : ASMOP_ZERO, 0, !regDead (A_IDX, ic));
  if (tlbl)
    emitcode ("jp", "%05d$", labelKey2num (tlbl->key));
  cost (3, 0);
  emitLabel (tlbl_NE);
  cheapMove (result->aop, 0, opcode == NE_OP ? ASMOP_ONE : ASMOP_ZERO, 0, !regDead (A_IDX, ic));
  emitLabel (tlbl);

  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genXor - code for or                                            */
/*-----------------------------------------------------------------*/
static void
genXor (const iCode *ic)
{
  operand *left, *right, *result;
  int size, i, j, omitbyte = -1;
  bool pushed_a = FALSE;

  D (emitcode ("; genXor", ""));

  aopOp ((left = IC_LEFT (ic)), ic);
  aopOp ((right = IC_RIGHT (ic)), ic);
  aopOp ((result = IC_RESULT (ic)), ic);

  size = getSize (operandType (result));

  /* Prefer literal operand on right */
  if (left->aop->type == AOP_LIT ||
    right->aop->type != AOP_LIT && left->aop->type == AOP_DIR ||
    (aopInReg (right->aop, 0, A_IDX) || aopInReg (right->aop, 0, X_IDX) || aopInReg (right->aop, 0, Y_IDX)) && left->aop->type == AOP_STK)
    {
      operand *temp = left;
      left = right;
      right = temp;
    }

  // todo: Use bit complement instructions where it is faster.
  if (!regDead (A_IDX, ic))
    {
      push (ASMOP_A, 0, 1);
      pushed_a = TRUE;
    }

  // Byte in a needs to be handled first.
  for (i = 0; i < size; i++)
    if (aopInReg (left->aop, i, A_IDX) || aopInReg (right->aop, i, A_IDX))
      {
        const asmop *other_stacked = NULL;
        int other_offset;
        asmop *other = (aopInReg (left->aop, i, A_IDX) ? right : left)->aop;

        other_stacked = stack_aop (other, i, &other_offset);

        if (right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
          ;
        else if (right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 0xff)
          emit3 (A_CPL, ASMOP_A, 0);
        else if (!other_stacked)
          emit3_o (A_XOR, ASMOP_A, 0, other, i);
        else
          {
            emitcode ("xor", "a, (%d, sp)", other_offset);
            cost (2, 1);
          }
        omitbyte = i;

        if (other_stacked)
          pop (other_stacked, 0, 2);

        if (aopInReg (result->aop, i, A_IDX) && size > 1)
          {
            push (ASMOP_A, 0, 1);
            pushed_a = TRUE;
          }
        else
          {
            // Avoid overwriting operand.
            if (aopRS (result->aop) && !aopOnStack (result->aop, i, 1))
              for (j = 0; j < size; j++)
                {
                  if (i == j)
                    continue;
                  if (j < left->aop->size && aopRS (left->aop) && !aopOnStack (left->aop, j, 1) &&
                    left->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx ||
                    j < right->aop->size && aopRS (right->aop) && !aopOnStack (right->aop, j, 1) &&
                    right->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx)
                    {
                      if (!regalloc_dry_run)
                        wassertl (0, "Unimplemented xor operand.");
                      cost (80, 80);
                    }
                }

            cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
          }
        break;
      }

  for (i = 0; i < size; i++)
    {
      const asmop *right_stacked = NULL;
      int right_offset;

      if (omitbyte == i)
        continue;

      if (right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
        {
          cheapMove (result->aop, i, left->aop, i, FALSE);
          if (aopInReg (result->aop, i, A_IDX) && i != size - 1)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }
          continue;
        }

      right_stacked = stack_aop (right->aop, i, &right_offset);

      cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

      if (right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 0xff)
        emit3 (A_CPL, ASMOP_A, 0);
      else if (!right_stacked && !(i && aopInReg (right->aop, i, A_IDX)))
        emit3_o (A_XOR, ASMOP_A, 0, right->aop, i);
      else
        {
          emitcode ("xor", "a, (%d, sp)", right_offset);
          cost (2, 1);
        }

      if (right_stacked)
        pop (right_stacked, 0, 2);

      if (!aopInReg (result->aop, i, A_IDX) || i == size - 1)
        cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
      else
        {
          push (ASMOP_A, 0, 1);
          pushed_a = TRUE;
        }
    }

  if (pushed_a)
    pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (right);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genOr - code for or                                             */
/*-----------------------------------------------------------------*/
static void
genOr (const iCode *ic)
{
  operand *left, *right, *result;
  int size, i, j, omitbyte = -1;
  bool pushed_a = FALSE;

  D (emitcode ("; genOr", ""));

  aopOp ((left = IC_LEFT (ic)), ic);
  aopOp ((right = IC_RIGHT (ic)), ic);
  aopOp ((result = IC_RESULT (ic)), ic);

  size = getSize (operandType (result));

  /* Prefer literal operand on right */
  if (left->aop->type == AOP_LIT ||
    right->aop->type != AOP_LIT && left->aop->type == AOP_DIR ||
    (aopInReg (right->aop, 0, A_IDX) || aopInReg (right->aop, 0, X_IDX) || aopInReg (right->aop, 0, Y_IDX)) && left->aop->type == AOP_STK)
    {
      operand *temp = left;
      left = right;
      right = temp;
    }

  // todo: Use bit set instructions where it is faster.
  if (!regDead (A_IDX, ic))
    {
      push (ASMOP_A, 0, 1);
      pushed_a = TRUE;
    }

  // Byte in a needs to be handled first.
  for (i = 0; i < size; i++)
    if (aopInReg (left->aop, i, A_IDX) || aopInReg (right->aop, i, A_IDX))
      {
        const asmop *other_stacked = NULL;
        int other_offset;
        asmop *other = (aopInReg (left->aop, i, A_IDX) ? right : left)->aop;

        other_stacked = stack_aop (other, i, &other_offset);

        if (right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
          ;
        else if (!other_stacked)
          emit3_o (A_OR, ASMOP_A, 0, other, i);
        else
          {
            emitcode ("or", "a, (%d, sp)", other_offset);
            cost (2, 1);
          }
        omitbyte = i;

        if (other_stacked)
          pop (other_stacked, 0, 2);

        if (aopInReg (result->aop, i, A_IDX) && size > 1)
          {
            push (ASMOP_A, 0, 1);
            pushed_a = TRUE;
          }
        else
          {
            // Avoid overwriting operand.
            if (aopRS (result->aop) && !aopOnStack (result->aop, i, 1))
              for (j = 0; j < size; j++)
                {
                  if (i == j)
                    continue;
                  if (j < left->aop->size && aopRS (left->aop) && !aopOnStack (left->aop, j, 1) &&
                    left->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx ||
                    j < right->aop->size && aopRS (right->aop) && !aopOnStack (right->aop, j, 1) &&
                    right->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx)
                    {
                      if (!regalloc_dry_run)
                        wassertl (0, "Unimplemented or operand.");
                      cost (80, 80);
                    }
                }

            cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
          }

        break;
      }

  for (i = 0; i < size; i++)
    {
      const asmop *right_stacked = NULL;
      int right_offset;

      if (omitbyte == i)
        continue;

      if (right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
        {
          cheapMove (result->aop, i, left->aop, i, FALSE);
          if (aopInReg (result->aop, i, A_IDX) && i != size - 1)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }
          continue;
        }

      right_stacked = stack_aop (right->aop, i, &right_offset);

      cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

      if (!right_stacked && !(i && aopInReg (right->aop, i, A_IDX)))
        emit3_o (A_OR, ASMOP_A, 0, right->aop, i);
      else
        {
          emitcode ("or", "a, (%d, sp)", right_offset);
          cost (2, 1);
        }

      if (right_stacked)
        pop (right_stacked, 0, 2);

      if (!aopInReg (result->aop, i, A_IDX) || i == size - 1)
        cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
      else
        {
          push (ASMOP_A, 0, 1);
          pushed_a = TRUE;
        }
    }

  if (pushed_a)
    pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (right);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genAnd - code for and                                           */
/*-----------------------------------------------------------------*/
static void
genAnd (const iCode *ic)
{
  operand *left, *right, *result;
  int size, i, j, omitbyte = -1;
  bool pushed_a = FALSE;

  D (emitcode ("; genAnd", ""));

  aopOp ((left = IC_LEFT (ic)), ic);
  aopOp ((right = IC_RIGHT (ic)), ic);
  aopOp ((result = IC_RESULT (ic)), ic);

  size = getSize (operandType (result));

  /* Prefer literal operand on right */
  if (left->aop->type == AOP_LIT ||
    right->aop->type != AOP_LIT && left->aop->type == AOP_DIR ||
    (aopInReg (right->aop, 0, A_IDX) || aopInReg (right->aop, 0, X_IDX) || aopInReg (right->aop, 0, Y_IDX)) && left->aop->type == AOP_STK)
    {
      operand *temp = left;
      left = right;
      right = temp;
    }

  // todo: Use bit reset instructions where it is faster.
  if (!regDead (A_IDX, ic))
    {
      push (ASMOP_A, 0, 1);
      pushed_a = TRUE;
    }

  // Byte in a needs to be handled first.
  for (i = 0; i < size; i++)
    if (aopInReg (left->aop, i, A_IDX) || aopInReg (right->aop, i, A_IDX))
      {
        const asmop *other_stacked = NULL;
        int other_offset;
        asmop *other = (aopInReg (left->aop, i, A_IDX) ? right : left)->aop;

        other_stacked = stack_aop (other, i, &other_offset);

        if (right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 0xff)
          ;
        else if (!other_stacked)
          emit3_o (A_AND, ASMOP_A, 0, other, i);
        else
          {
            emitcode ("and", "a, (%d, sp)", other_offset);
            cost (2, 1);
          }
        omitbyte = i;

        if (other_stacked)
          pop (other_stacked, 0, 2);

       if (aopInReg (result->aop, i, A_IDX) && size > 1)
          {
            push (ASMOP_A, 0, 1); // todo: Do not push, if other bytes do not affect a (e.g. due to using clr).
            pushed_a = TRUE;
          }
        else
          {
            // Avoid overwriting operand.
            if (aopRS (result->aop) && !aopOnStack (result->aop, i, 1))
              for (j = 0; j < size; j++)
                {
                  if (i == j)
                    continue;
                  if (j < left->aop->size && aopRS (left->aop) && !aopOnStack (left->aop, j, 1) &&
                    left->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx ||
                    j < right->aop->size && aopRS (right->aop) && !aopOnStack (right->aop, j, 1) &&
                    right->aop->aopu.bytes[j].byteu.reg->rIdx == result->aop->aopu.bytes[i].byteu.reg->rIdx)
                    {
                      if (!regalloc_dry_run)
                        wassertl (0, "Unimplemented and operand.");
                      cost (80, 80);
                    }
                }

            cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
          }

        break;
      }

  for (i = 0; i < size; i++)
    {
      const asmop *right_stacked = NULL;
      int right_offset;

      if (omitbyte == i)
        continue;

      if (right->aop->type == AOP_LIT && byteOfVal (right->aop->aopu.aop_lit, i) == 0xff)
        {
          cheapMove (result->aop, i, left->aop, i, FALSE);
          if (aopInReg (result->aop, i, A_IDX) && i != size - 1)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }
          continue;
        }
      else if (right->aop->type == AOP_LIT && !byteOfVal (right->aop->aopu.aop_lit, i))
        {
          cheapMove (result->aop, i, ASMOP_ZERO, 0, FALSE);
          if (aopInReg (result->aop, i, A_IDX) && i != size - 1)
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }
          continue;
        }

      right_stacked = stack_aop (right->aop, i, &right_offset);

      cheapMove (ASMOP_A, 0, left->aop, i, FALSE);

      if (!right_stacked && !(i && aopInReg (right->aop, i, A_IDX)))
        emit3_o (A_AND, ASMOP_A, 0, right->aop, i);
      else
        {
          emitcode ("and", "a, (%d, sp)", right_offset);
          cost (2, 1);
        }

      if (right_stacked)
        pop (right_stacked, 0, 2);

      if (!aopInReg (result->aop, i, A_IDX) || i == size - 1)
        cheapMove (result->aop, i, ASMOP_A, 0, FALSE);
      else
        {
          push (ASMOP_A, 0, 1);
          pushed_a = TRUE;
        }
    }

  if (pushed_a)
    pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (right);
  freeAsmop (result);
}

/*------------------------------------------------------------------*/
/* genLeftShiftLiteral - left shifting by known count for size <= 2 */
/*------------------------------------------------------------------*/
static void
genLeftShiftLiteral (operand *left, operand *right, operand *result, const iCode *ic)
{
  int shCount = (int) ulFromVal (right->aop->aopu.aop_lit);
  int size;

  D (emitcode ("; genLeftShiftLiteral", ""));

  size = getSize (operandType (result));

  /* I suppose that the left size >= result size */
  wassert (getSize (operandType (left)) >= size);

  if (shCount >= (size * 8))
    {
      aopOp (left, ic);
      aopOp (result, ic);

      while (size--)
        emit3_o (A_CLR, result->aop, size, 0, 0);
    }
  else
    {
      int i;

      wassertl (size <= 2, "Shifting of longs should be handled by generic function.");

      aopOp (left, ic);
      aopOp (result, ic);

      genMove (result->aop, left->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

      while (shCount--)
        for(i = 0; i < size;)
          {
            if (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX))
              {
                emit3w (i ? A_RLCW : A_SLLW, result->aop, 0);
                i += 2;
              }
            else
              {
                int swapidx = -1;
                if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg)
                  swapidx = result->aop->aopu.bytes[i].byteu.reg->rIdx;

                if (swapidx == -1)
                  emit3_o (i ? A_RLC : A_SLL, result->aop, i, 0, 0);
                else
                  {
                    swap_to_a (swapidx);
                    emit3 (i ? A_RLC : A_SLL, ASMOP_A, 0);
                    swap_from_a (swapidx);
                  }

                i++;
              }
          }
    }

  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genLeftShift - generates code for left shifting                 */
/*-----------------------------------------------------------------*/
static void
genLeftShift (const iCode *ic)
{
  operand *left, *right, *result;
  int i, size;
  bool save_a;
  symbol *tlbl1, *tlbl2;

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (right->aop->type == AOP_LIT && getSize (operandType (result)) <= 2)
    {
      genLeftShiftLiteral (left, right, result, ic);
      freeAsmop (right);
      return;
    }

  D (emitcode ("; genLeftShift", ""));

  aopOp (result, ic);
  aopOp (left, ic);

  genMove (result->aop, left->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

  size = result->aop->size;

  save_a = !regDead (A_IDX, ic);
  for (i = 0; i < size; i++)
    {
      if (aopInReg (result->aop, i, A_IDX))
        save_a = TRUE;
      if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg &&
        right->aop->regs[result->aop->aopu.bytes[i].byteu.reg->rIdx] == 0)
        {
          if (!regalloc_dry_run)
            wassertl (0, "Overwriting shift count");
          cost (80, 80);
        }
    }

  if (save_a)
    push (ASMOP_A, 0, 1);

  tlbl1 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  tlbl2 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  cheapMove (ASMOP_A, 0, right->aop, 0, FALSE);

  if (right->aop->type != AOP_LIT || !byteOfVal (right->aop->aopu.aop_lit, 0))
    {
      emit3 (A_TNZ, ASMOP_A, 0);
      if (tlbl2)
        emitcode ("jreq", "!tlabel", labelKey2num (tlbl2->key));
      cost (2, 0);
    }

  emitLabel (tlbl1);
  // todo: Shift in left if dead and cheaper?
  for (i = 0; i < size;)
     {
        int swapidx = -1;

        if (aopInReg (result->aop, i, X_IDX) || aopInReg (result->aop, i, Y_IDX))
          {
            emit3w_o (i ? A_RLCW : A_SLLW, result->aop, i, 0, 0);
            i += 2;
            continue;
          }

        if (aopInReg (result->aop, i, A_IDX))
          {
            emitcode (i ? "rlc" : "sll", "(1, sp)");
            cost (2, 1);
            i++;
            continue;
          }

        if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg)
          swapidx = result->aop->aopu.bytes[i].byteu.reg->rIdx;

        if (swapidx == -1)
          emit3_o (i ? A_RLC : A_SLL, result->aop, i, 0, 0);
        else
          {
            swap_to_a (swapidx);
            emit3 (i ? A_RLC : A_SLL, ASMOP_A, 0);
            swap_from_a (swapidx);
          }
        i++;
     }

  emit3 (A_DEC, ASMOP_A, 0);
  if (tlbl1)
    emitcode ("jrne", "!tlabel", labelKey2num (tlbl1->key));
  cost (2, 0);
  emitLabel (tlbl2);

  if (save_a)
    pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (result);
  freeAsmop (right);
}

/*------------------------------------------------------------------*/
/* genRightShiftLiteral - right shifting by known count for size <= 2*/
/*------------------------------------------------------------------*/
static void
genRightShiftLiteral (operand *left, operand *right, operand *result, const iCode *ic)
{
  int shCount = (int) ulFromVal (right->aop->aopu.aop_lit);
  int size;
  bool sign;

  D (emitcode ("; genRightShiftLiteral", ""));

  size = getSize (operandType (result));

  sign =  !SPEC_USIGN (getSpec (operandType (left)));

  /* I suppose that the left size >= result size */
  wassert (getSize (operandType (left)) >= size);

  if (!sign && shCount >= (size * 8))
    {
      aopOp (left, ic);
      aopOp (result, ic);

      genMove(result->aop, ASMOP_ZERO, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));
    }
  else
    {
      int i;

      wassertl (size <= 2, "Shifting of longs should be handled by generic function.");

      aopOp (left, ic);
      aopOp (result, ic);

      genMove (result->aop, left->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

      while (shCount--)
        for(i = size - 1; i >= 0;)
          {
            if (i > 0 && (aopInReg (result->aop, i - 1, X_IDX) || aopInReg (result->aop, i - 1, Y_IDX)))
              {
                emit3w_o ((i != size - 1) ? A_RRCW : (sign ? A_SRAW : A_SRLW), result->aop, i - 1, 0, 0);
                i -= 2;
              }
            else
              {
                int swapidx = -1;
                if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg)
                  swapidx = result->aop->aopu.bytes[i].byteu.reg->rIdx;

                if (swapidx == -1)
                  emit3_o ((i != size - 1) ? A_RRC : (sign ? A_SRA : A_SRL), result->aop, i, 0, 0);
                else
                  {
                    swap_to_a (swapidx);
                    emit3 ((i != size - 1) ? A_RRC : (sign ? A_SRA : A_SRL), ASMOP_A, 0);
                    swap_from_a (swapidx);
                  }

                i--;
              }
          }
    }

  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genRightShift - generates code for right shifting               */
/*-----------------------------------------------------------------*/
static void
genRightShift (const iCode *ic)
{
  operand *left, *right, *result;
  int i, size;
  bool save_a;
  symbol *tlbl1, *tlbl2;
  bool sign;

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic);

  /* if the shift count is known then do it
     as efficiently as possible */
  if (right->aop->type == AOP_LIT && getSize (operandType (result)) <= 2)
    {
      genRightShiftLiteral (left, right, result, ic);
      freeAsmop (right);
      return;
    }

  D (emitcode ("; genRightShift", ""));

  sign =  !SPEC_USIGN (getSpec (operandType (left)));

  aopOp (result, ic);
  aopOp (left, ic);

  genMove (result->aop, left->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

  size = result->aop->size;

  save_a = !regDead (A_IDX, ic);
  for (i = 0; i < size; i++)
    {
      if (aopInReg (result->aop, i, A_IDX))
        save_a = TRUE;
      if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg &&
        right->aop->regs[result->aop->aopu.bytes[i].byteu.reg->rIdx] == 0)
        {
          if (!regalloc_dry_run)
            wassertl (0, "Overwriting shift count");
          cost (80, 80);
        }
    }
  if (save_a)
    push (ASMOP_A, 0, 1);

  tlbl1 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  tlbl2 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  cheapMove (ASMOP_A, 0, right->aop, 0, FALSE);

  if (right->aop->type != AOP_LIT || !byteOfVal (right->aop->aopu.aop_lit, 0))
    {
      emit3 (A_TNZ, ASMOP_A, 0);
      if (tlbl2)
        emitcode ("jreq", "!tlabel", labelKey2num (tlbl2->key));
      cost (2, 0);
    }

  emitLabel (tlbl1);
  // todo: Shift in left if free and cheaper, use sllw.
  for (i = size - 1; i >= 0;)
     {
        int swapidx = -1;

        if (i > 0 && (aopInReg (result->aop, i - 1, X_IDX) || aopInReg (result->aop, i - 1, Y_IDX)))
          {
            emit3w_o ((i != size - 1) ? A_RRCW : (sign ? A_SRAW : A_SRLW), result->aop, i - 1, 0, 0);
            i -= 2;
            continue;
          }
        else if (aopInReg (result->aop, i, A_IDX))
          {
            emitcode ((i != size - 1) ? "rrc" : (sign ? "sra" : "srl"), "(1, sp)");
            cost (2, 1);
            i--;
            continue;
          }

        if (aopRS (result->aop) && !aopInReg (result->aop, i, A_IDX) && result->aop->aopu.bytes[i].in_reg)
          swapidx = result->aop->aopu.bytes[i].byteu.reg->rIdx;

        if (swapidx == -1)
          emit3_o ((i != size - 1) ? A_RRC : (sign ? A_SRA : A_SRL), result->aop, i, 0, 0);
        else
          {
            swap_to_a (swapidx);
            emit3 ((i != size - 1) ? A_RRC : (sign ? A_SRA : A_SRL), ASMOP_A, 0);
            swap_from_a (swapidx);
          }
        i--;
     }

  emit3 (A_DEC, ASMOP_A, 0);
  if (tlbl1)
    emitcode ("jrne", "!tlabel", labelKey2num (tlbl1->key));
  cost (2, 0);
  emitLabel (tlbl2);

  if (save_a)
    pop (ASMOP_A, 0, 1);

  freeAsmop (left);
  freeAsmop (result);
  freeAsmop (right);
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void
genPointerGet (const iCode *ic)
{
  operand *result = IC_RESULT (ic);
  operand *left = IC_LEFT (ic);
  operand *right = IC_RIGHT (ic);
  int size, i;
  unsigned offset;
  bool use_y;
  bool pushed_a = FALSE;
  int blen, bstr;
  bool bit_field = IS_BITVAR (getSpec (operandType (result)));
  symbol *const tlbl = ((regalloc_dry_run || !bit_field) ? 0 : newiTempLabel (NULL));
  
  blen = bit_field ? SPEC_BLEN (getSpec (operandType (result))) : 0;
  bstr = bit_field ? SPEC_BSTR (getSpec (operandType (result))) : 0;

  D (emitcode ("; genPointerGet", ""));

  aopOp (IC_LEFT (ic), ic);
  aopOp (IC_RIGHT (ic), ic);
  aopOp (IC_RESULT (ic), ic);

  if (result->aop->type == AOP_DUMMY)
    D (emitcode ("; Dummy read", ""));

  wassertl (right, "GET_VALUE_AT_ADDRESS without right operand");
  wassertl (IS_OP_LITERAL (IC_RIGHT (ic)), "GET_VALUE_AT_ADDRESS with non-literal right operand");

  // todo: Handle this more gracefully, save x instead of using y, when doing so is more efficient, use ldw, etc.
  use_y = !regDead (X_IDX, ic);
  if (use_y && !regDead (Y_IDX, ic))
    {
      if (!regalloc_dry_run)
        wassertl (0, "No free reg for pointer.");
      cost (80, 80);
      goto release;
    }

  if (!regDead (A_IDX, ic))
    {
      push (ASMOP_A, 0, 1);
      pushed_a = TRUE;
    }

  genMove (use_y ? ASMOP_Y : ASMOP_X, left->aop, TRUE, regDead (X_IDX, ic), regDead (Y_IDX, ic));

  // todo: What if right operand is negative?
  offset = byteOfVal (right->aop->aopu.aop_lit, 0) * 256 + byteOfVal (right->aop->aopu.aop_lit, 0);

  // Get all the bytes. todo: Get the byte in a last (if not a bit-field), so we do not need to save a.
  size = result->aop->size;
  for (i = 0; !bit_field ? i < size : blen > 0; i++, blen -= 8)
    {
      if (!((bit_field ? i : size - 1 - i) + offset))
        {
          emitcode ("ld", use_y ? "a, (y)" : "a, (x)");
          cost (1 + use_y, 1);
        }
      else
        {
          emitcode ("ld", use_y ? "a, (0x%x, y)" : "a, (0x%x, x)", (bit_field ? i : size - 1 - i) + offset);
          cost ((size - 1 - i + offset < 256 ? 2 : 3) + use_y, 1);
        }

      if (bit_field && blen < 8 && !i) // The only byte might need shifting.
        while (bstr--)
          emit3 (A_SRL, ASMOP_A, 0);
      if (bit_field && blen < 8) // The partial byte.
        {
          emitcode ("and", "a, #0x%02x", 0xff >> (8 - blen));
          cost (2, 1);
        }

      if (bit_field && blen <= 8 && !SPEC_USIGN (getSpec (operandType (result)))) // Sign extension for partial byte of signed bit-field
        {   
          push (ASMOP_A, 0, 1);
          emitcode ("and", "a, #0x%02x", 0x80 >> (8 - blen));
          pop (ASMOP_A, 0, 1);
          if (tlbl)
            emitcode ("jreq", "!tlabel", labelKey2num (tlbl->key));
          cost (2, 0);
          emitcode ("or", "a, #0x%02x", (0xff00 >> (8 - blen)) & 0xff);
          emitLabel (tlbl);
        }

      if (result->aop->type == AOP_DUMMY)
        continue;

      else if ((!bit_field ? i < size - 1 : blen > 8) && aopInReg (result->aop, i, A_IDX))
        {
          push (ASMOP_A, 0, 1);
          pushed_a = TRUE;
        }
      else
        cheapMove (result->aop, i, ASMOP_A, 0, FALSE);

      if (i < size - 1 && (use_y ? aopInReg (result->aop, i, YL_IDX) || aopInReg (result->aop, i, YH_IDX) : aopInReg (result->aop, i, XL_IDX) || aopInReg (result->aop, i, XH_IDX)))
        {
          if (!regalloc_dry_run)
            wassertl (0, "Overwriting pointer");
          cost (80, 80);
        }
    }

  if (pushed_a)
    pop (ASMOP_A, 0, 1);

  if (bit_field && i < size)
    {
      if (SPEC_USIGN (getSpec (operandType (result))))
        genMove_o (result->aop, i, ASMOP_ZERO, 0, bit_field ? i : size - i - 1, FALSE, FALSE, FALSE);
      else
        wassertl (0, "Unimplemented multibyte sign extension for bit-field.");
    }

release:
  freeAsmop (right);
  freeAsmop (left);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genAssign - generate code for assignment                        */
/*-----------------------------------------------------------------*/
static void
genAssign (const iCode *ic)
{
  operand *result, *right;

  D (emitcode ("; genAssign", ""));

  result = IC_RESULT (ic);
  right = IC_RIGHT (ic);

  aopOp (right, ic);
  aopOp (result, ic);

  wassert (result->aop->type != AOP_DUMMY || right->aop->type != AOP_DUMMY);

  if (right->aop->type == AOP_DUMMY)
    {
      int i;
      D (emitcode ("; Dummy write", ""));
      for (i = 0; i < result->aop->size; i++)
        cheapMove (result->aop, i, ASMOP_A, 0, TRUE);
    }
  else if (result->aop->type == AOP_DUMMY)
    {
      int i;
      D (emitcode ("; Dummy read", ""));
      push (ASMOP_A, 0, 1);
      for (i = 0; i < right->aop->size; i++)
        cheapMove (ASMOP_A, 0, right->aop, i, FALSE);
      pop (ASMOP_A, 0, 1);
    }
  else
    genMove(result->aop, right->aop, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

  wassert (result->aop != right->aop);
  freeAsmop (right);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void
genPointerSet (iCode * ic)
{
  operand *result = IC_RESULT (ic);
  operand *right = IC_RIGHT (ic);
  int size, i, j;
  bool use_y;
  int pushed_a = 0;
  int blen, bstr;
  bool bit_field = IS_BITVAR (getSpec (operandType (right))) || IS_BITVAR (getSpec (operandType (result)));
  
  blen = bit_field ? (SPEC_BLEN (getSpec (operandType (IS_BITVAR (getSpec (operandType (right))) ? right : result)))) : 0;
  bstr = bit_field ? (SPEC_BSTR (getSpec (operandType (IS_BITVAR (getSpec (operandType (right))) ? right : result)))) : 0;

  D (emitcode ("; genPointerSet", ""));

  aopOp (result, ic);
  aopOp (right, ic);

  size = right->aop->size;

  // todo: Handle this more gracefully, save x instead of using y, when doing so is more efficient, use ldw, etc.
  use_y = !regDead (X_IDX, ic);
  if (use_y && !regDead (Y_IDX, ic))
    {
      if (!regalloc_dry_run)
        wassertl (0, "No free reg for pointer.");
      cost (80, 80);
      goto release;
    }

  for(i = 1; i < size; i++)
    if (aopInReg (right->aop, i, A_IDX))
      {
        push (ASMOP_A, 0, 1);
        pushed_a = 1;
        break;
      }

  if (!regDead (A_IDX, ic) && !pushed_a)
    {
      push (ASMOP_A, 0, 1);
      pushed_a = TRUE;
    }

  genMove (use_y ? ASMOP_Y : ASMOP_X, result->aop, !aopInReg (right->aop, 0, A_IDX), regDead (X_IDX, ic), regDead (Y_IDX, ic));

  for (i = 0; !bit_field ? i < size : blen > 0; i++, blen -= 8)
    {
      if (use_y ? aopInReg (right->aop, i, YL_IDX) || aopInReg (right->aop, i, YH_IDX) : aopInReg (right->aop, i, XL_IDX) || aopInReg (right->aop, i, XH_IDX))
        {
          if (!regalloc_dry_run)
            wassertl (0, "Overwriting pointer");
          cost (80, 80);
        }

      if (i && aopInReg (right->aop, i, A_IDX))
        {
          emitcode ("ld", "a, (1, sp)");
          cost (2, 1);
        }
      else
        cheapMove (ASMOP_A, 0, right->aop, i, FALSE);

      if (bit_field && blen < 8)
        {
          for (j = 0; j < bstr; j++)
            emit3 (A_SLL, ASMOP_A, 0);
          emitcode ("and", "a, #0x%02x", (0xff >> (8 - blen)) << bstr);
          cost (2, 1);
          push (ASMOP_A, 0, 1);
          pushed_a++;
          emitcode ("ld", "a, #0x%02x", ~((0xff >> (8 - blen)) << bstr) & 0xff);
          cost (2, 1);
          if (!i)
            {
              emitcode ("and", use_y ? "a, (y)" : "a, (x)", i);
              cost (1 + use_y, 1);
            }
          else
            {
              emitcode ("and", use_y ? "a, (0x%x, y)" : "a, (0x%x, x)", i);
              cost ((size - 1 - i < 256 ? 2 : 3) + use_y, 1);
            }
          emitcode ("or", "a, (1, sp)");
          cost (2, 1);
        }

      if (!(bit_field ? i : size - 1 - i))
        {
          emitcode ("ld", use_y ? "(y), a" : "(x), a");
          cost (1 + use_y, 1);
        }
      else
        {
          emitcode ("ld", use_y ? "(0x%x, y), a" : "(0x%x, x), a", bit_field ? i : size - 1 - i);
          cost ((size - 1 - i < 256 ? 2 : 3) + use_y, 1);
        }
    }

  while (pushed_a--)
    pop (ASMOP_A, 0, 1);

release:
  freeAsmop (right);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genIfx - generate code for Ifx statement                        */
/*-----------------------------------------------------------------*/
static void
genIfx (const iCode *ic)
{
  // todo: This function currently reports code size costs only, other costs will depend on profiler information.
  bool inv = FALSE;
  operand *const cond = IC_COND (ic);
  symbol *const tlbl = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
  symbol *tlbl2 = NULL;
  aopOp (cond, ic);

  D (emitcode ("; genIfx", ""));

  if (IS_BOOL (operandType (cond)) && cond->aop->type == AOP_DIR)
    {
      if (tlbl)
        emitcode (IC_FALSE (ic) ? "btjt" : "btjf", "%s, #0, !tlabel", aopGet (cond->aop, 0), labelKey2num (tlbl->key));
      cost (5, 0);
    }
  else if (aopInReg (cond->aop, 0, C_IDX))
    {
      wassertl (IS_BOOL (operandType (cond)), "Variable of type other than _Bool in carry bit.");
      if (tlbl)
        emitcode (IC_FALSE (ic) ? "jrc" : "jrnc", "!tlabel", labelKey2num (tlbl->key));
      cost (2, 0);
    }
  else if (cond->aop->size == 2 &&
    (aopInReg (cond->aop, 0, X_IDX) || aopInReg (cond->aop, 0, Y_IDX) ||
    (cond->aop->type == AOP_REG && (cond->aop->aopu.bytes[0].byteu.reg->rIdx == XH_IDX && cond->aop->aopu.bytes[1].byteu.reg->rIdx == XL_IDX || cond->aop->aopu.bytes[0].byteu.reg->rIdx == YH_IDX && cond->aop->aopu.bytes[1].byteu.reg->rIdx == YL_IDX))))
    {
      bool in_x = (aopInReg (cond->aop, 0, X_IDX) || cond->aop->aopu.bytes[0].byteu.reg->rIdx == XH_IDX && cond->aop->aopu.bytes[1].byteu.reg->rIdx == XL_IDX);

      emit3w (A_TNZW, in_x ? ASMOP_X : ASMOP_Y, 0);
      if (tlbl)
        emitcode (IC_FALSE (ic) ? "jrne" : "jreq", "!tlabel", labelKey2num (tlbl->key));
      cost (2, 0);
    }
  else if (aopRS (cond->aop) || cond->aop->type == AOP_DIR)
    {
      int i;

      if(cond->aop->size > 1 && !IC_FALSE (ic))
        {
          tlbl2 = (regalloc_dry_run ? 0 : newiTempLabel (NULL));
          inv = true;
        }

      for (i = 0; i < cond->aop->size; i++) // todo: Use tnzw; test a first, if dead, to free a; use swapw followed by exg to test xh if xl is dead (same for yh), use tnzw independently of where in the operand xl and xh are.
        {
          if ((aopInReg (cond->aop, i, XL_IDX) || aopInReg (cond->aop, i, XH_IDX) || aopInReg (cond->aop, i, YH_IDX)) && regDead (A_IDX, ic) && cond->aop->regs[A_IDX] <= i)
            {
              cheapMove (ASMOP_A, 0, cond->aop, i, FALSE);
              emit3(A_TNZ, ASMOP_A, 0);
            }
          else if (aopInReg (cond->aop, i, XL_IDX))
            {
              emitcode ("exg", "a, xl");
              cost (1, 1);
              emit3(A_TNZ, ASMOP_A, 0);
              emitcode ("exg", "a, xl");
              cost (1, 1);
            }
          else if (aopInReg (cond->aop, i, YL_IDX))
            {
              emitcode ("exg", "a, yl");
              cost (1, 1);
              emit3(A_TNZ, ASMOP_A, 0);
              emitcode ("exg", "a, yl");
              cost (1, 1);
            }
          else if (aopInReg (cond->aop, i, XH_IDX))
            {
              push (ASMOP_X, 0, 2);
              emitcode ("tnz", "(1, sp)");
              adjustStack (2);
            }
          else if (aopInReg (cond->aop, i, YH_IDX))
            {
              push (ASMOP_Y, 0, 2);
              emitcode ("tnz", "(1, sp)");
              adjustStack (2);
            }
          else
            emit3_o (A_TNZ, cond->aop , i, 0, 0);

          if (tlbl)
            emitcode ((!!IC_FALSE (ic) ^ (inv && i != cond->aop->size - 1)) ? "jrne" : "jreq", "!tlabel", labelKey2num ((inv && i == cond->aop->size - 1) ? tlbl2->key : tlbl->key));
          cost (2, 0);
        }
    }
  else
    {
      if (!regalloc_dry_run)
        {
          printf ("cond aop type %d, size %d\n", cond->aop->type, cond->aop->size);
          wassertl (0, "Unimplemented conditional jump.");
        }
      cost (80, 80);
    }

  if (inv)
    {
      emitLabel (tlbl);
      emitcode ("jp", "!tlabel", labelKey2num ((IC_TRUE (ic) ? IC_TRUE (ic) : IC_FALSE (ic))->key));
      cost (3, 0);
      emitLabel (tlbl2);
    }
  else
    {
      emitcode ("jp", "!tlabel", labelKey2num ((IC_TRUE (ic) ? IC_TRUE (ic) : IC_FALSE (ic))->key));
      cost (3, 0);
      emitLabel (tlbl);
    }

  freeAsmop (cond);
}

/*-----------------------------------------------------------------*/
/* genAddrOf - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void
genAddrOf (const iCode *ic)
{
  const symbol *sym;
  operand *result, *left;

  D (emitcode ("; genAddrOf", ""));

  result = IC_RESULT (ic);
  left = IC_LEFT (ic);

  wassert (result);
  wassert (left);
  wassert (IS_TRUE_SYMOP (left));
  sym = OP_SYMBOL_CONST (left);
  wassert (sym);

  aopOp (result, ic);

  // todo: When optimizing for size, putting on-stack address into y when y is free is cheaper calculating in x, then using exgw.
  if (aopInReg (result->aop, 0, Y_IDX) || regDead (Y_IDX, ic) && !regDead (X_IDX, ic))
    {
      if (!sym->onStack)
        {
          wassert (sym->name);
          emitcode ("ldw", "y, #%s", sym->rname);
          cost (4, 2);
        }
      else
        {
          emitcode ("ldw", "y, sp");
          emitcode ("addw", "y, #%d", sym->stack + _G.stack.pushed + 1);
          cost (6, 3);
        }
      genMove (result->aop, ASMOP_Y, regDead (A_IDX, ic), FALSE, regDead (X_IDX, ic));
    }
  else if (regDead (X_IDX, ic))
    {
      if (!sym->onStack)
        {
          wassert (sym->name);
          emitcode ("ldw", "x, #%s", sym->rname);
          cost (3, 2);
        }
      else
        {
          emitcode ("ldw", "x, sp");
          emitcode ("addw", "x, #%d", sym->stack + _G.stack.pushed + 1);
          cost (4, 3);
        }
      genMove (result->aop, ASMOP_X, regDead (A_IDX, ic), TRUE, regDead (Y_IDX, ic));
    }
  else // todo: Handle case of both X and Y alive; todo: Use mov when destination is a global variable.
    {
      if (!regalloc_dry_run)
        wassertl (0, "Unimplemented GET_VALUE_AT_ADDRESS deadness");
      cost (80, 80);
    }

  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genCast - gen code for casting                                  */
/*-----------------------------------------------------------------*/
static void
genCast (const iCode *ic)
{
  operand *result, *right;
  int size, offset;
  sym_link *rtype;

  D (emitcode ("; genCast", ""));

  result = IC_RESULT (ic);
  right = IC_RIGHT (ic);
  rtype = operandType (right);

  aopOp (right, ic);
  aopOp (result, ic);

  // todo: More efficient casts to _Bool especially for result in XL or YL.

  if (result->aop->size <= right->aop->size && (!IS_BOOL (operandType (result)) || IS_BOOL (operandType (right))))
    {
      freeAsmop (right);
      freeAsmop (result);
      genAssign (ic);
      return;
    }
  else if (IS_BOOL (operandType (result)) && right->aop->size == 1 &&
    (aopInReg (right->aop, 0, A_IDX) || (right->aop->type != AOP_REG && right->aop->type != AOP_REGSTK) || !right->aop->aopu.bytes[0].in_reg))
    {
      if (!regDead (A_IDX, ic))
        push (ASMOP_A, 0, 1);

      if (aopInReg(right->aop, 0, A_IDX))
        {
          emit3 (A_NEG, ASMOP_A, 0);
          emit3 (A_CLR, ASMOP_A, 0);
        }
      else
        {
          emit3 (A_CLR, ASMOP_A, 0);
          emit3 (A_CP, ASMOP_A, right->aop);
        }
      emit3 (A_RLC, ASMOP_A, 0);
      cheapMove (result->aop, 0, ASMOP_A, 0, FALSE);

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);
    }
  else if (IS_BOOL (operandType (result)) && right->aop->size == 2 &&
    (aopInReg (right->aop, 0, X_IDX) && regDead (X_IDX, ic) || aopInReg (right->aop, 0, Y_IDX) && regDead (Y_IDX, ic)))
    {
      if (!regDead (A_IDX, ic))
        push (ASMOP_A, 0, 1);
      
      emit3w (A_NEGW, right->aop, 0);
      cost (aopInReg (right->aop, 0, X_IDX) ? 1 : 2, 2);
      emit3 (A_CLR, ASMOP_A, 0);
      emit3 (A_RLC, ASMOP_A, 0);
      cheapMove (result->aop, 0, ASMOP_A, 0, FALSE);

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);
    }
  else if (IS_BOOL (operandType (result)))
    {
      bool pushed_a = FALSE;
      size = right->aop->size;

      for(offset = 1; offset < size; offset++)
        if (aopInReg (right->aop, offset, A_IDX))
          {
            push (ASMOP_A, 0, 1);
            pushed_a = TRUE;
            break;
          }

      if (!regDead (A_IDX, ic) && !pushed_a)
        {
          push (ASMOP_A, 0, 1);
          pushed_a = TRUE;
        }

      for(offset = 0; offset < size; offset++)
        {
          const asmop *right_stacked = NULL;
          int right_offset;

          right_stacked = stack_aop (right->aop, offset, &right_offset);

          if (offset && aopInReg (right->aop, offset, A_IDX))
            {
              right_stacked = ASMOP_A;
              right_offset = 0;
            }   

          if (!offset && aopInReg (right->aop, offset, A_IDX))
            {
              emit3 (A_NEG, ASMOP_A, 0);
              emit3 (A_CLR, ASMOP_A, 0);
            }
          else if (!right_stacked)
            {
              emit3 (A_CLR, ASMOP_A, 0);
              emit3_o(offset ? A_SBC : A_SUB, ASMOP_A, 0, right->aop, offset);
            }
          else
            {
              emit3 (A_CLR, ASMOP_A, 0);
              emitcode (offset ? "sbc" : "sub", "a, (%d, sp)", right_offset);
              if (!aopInReg (right->aop, offset, A_IDX))
                pop (right_stacked, 0, 2);
            }
        }
      emit3 (A_RLC, ASMOP_A, 0);
      cheapMove (result->aop, 0, ASMOP_A, 0, FALSE);

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);
      else if (pushed_a)
        adjustStack (1);
    }
  else
    {
      bool result_in_a = FALSE;
      bool result_in_x = FALSE;
      bool result_in_y = FALSE;
      int i;

      genMove_o (result->aop, 0, right->aop, 0, right->aop->size, regDead (A_IDX, ic), regDead (X_IDX, ic), regDead (Y_IDX, ic));

      for (i = 0; i < right->aop->size; i++)
        if (aopInReg (result->aop, i, A_IDX))
          result_in_a = TRUE;
        else if (aopInReg (result->aop, i, XL_IDX) || aopInReg (result->aop, i, XH_IDX))
          result_in_x = TRUE;
        else if (aopInReg (result->aop, i, YL_IDX) || aopInReg (result->aop, i, YH_IDX))
          result_in_y = TRUE;

      /* now depending on the sign of the destination */
      size = result->aop->size - right->aop->size;
      offset = right->aop->size;

      /* Unsigned or not an integral type - fill with zeros */
      if (IS_BOOL (rtype) || !IS_SPEC (rtype) || SPEC_USIGN (rtype))
        genMove_o (result->aop, offset, ASMOP_ZERO, 0, size, regDead (A_IDX, ic) && !result_in_a, regDead (X_IDX, ic) && !result_in_x, regDead (Y_IDX, ic) && !result_in_y);
      else
        {
          bool pushed_a = FALSE;

          if (result_in_a || !regDead (A_IDX, ic))
            {
              push (ASMOP_A, 0, 1);
              pushed_a = TRUE;
            }

          cheapMove (ASMOP_A, 0, result->aop, right->aop->size - 1, FALSE);
          emit3 (A_RLC, ASMOP_A, 0);
          emit3 (A_CLR, ASMOP_A, 0);
          emit3 (A_SBC, ASMOP_A, ASMOP_ZERO);
          while (size--)
            {
              if (size && aopInReg (result->aop, offset, A_IDX))
                {
                  push (ASMOP_A, 0, 1);
                  pushed_a = TRUE;
                }
              else
                cheapMove (result->aop, offset, ASMOP_A, 0, FALSE);
              offset++;
            }

          if (pushed_a)
            pop (ASMOP_A, 0, 1);
        }
    }

  freeAsmop (right);
  freeAsmop (result);
}

/*-----------------------------------------------------------------*/
/* genDummyRead - generate code for dummy read of volatiles        */
/*-----------------------------------------------------------------*/
static void
genDummyRead (const iCode *ic)
{
  operand *op;
  int i;

  if ((op = IC_RIGHT (ic)) && IS_SYMOP (op))
    {
      aopOp (op, ic);

      D (emitcode ("; genDummyRead", ""));

      if (!regDead (A_IDX, ic))
        push (ASMOP_A, 0 ,1);

      for (i = 0; i < op->aop->size; i++)
        cheapMove (ASMOP_A, 0, op->aop, i, FALSE);

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);

      freeAsmop (op);
    }

  if ((op = IC_LEFT (ic)) && IS_SYMOP (op))
    {
      aopOp (op, ic);

      D (emitcode ("; genDummyRead", ""));

      if (!regDead (A_IDX, ic))
        push (ASMOP_A, 0 ,1);

      for (i = 0; i < op->aop->size; i++)
        cheapMove (ASMOP_A, 0, op->aop, i, FALSE);

      if (!regDead (A_IDX, ic))
        pop (ASMOP_A, 0, 1);

      freeAsmop (op);
    }
}

/*---------------------------------------------------------------------*/
/* genSTM8Code - generate code for STM8 for a single iCode instruction */
/*---------------------------------------------------------------------*/
static void
genSTM8iCode (iCode *ic)
{
#if 0
  if (!regalloc_dry_run)
    printf ("ic %d op %d stack pushed %d\n", ic->key, ic->op, _G.stack.pushed);
#endif

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
      wassertl (0, "Unimplemented iCode");
      break;

    case CALL:
      genCall (ic);
      break;

    case PCALL:
      genPCall (ic);
      break;

    case FUNCTION:
      genFunction (ic);
      break;

    case ENDFUNCTION:
      genEndFunction (ic);
      break;

    case RETURN:
      genReturn (ic);
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
    case '%':
      genDivMod (ic);
      break;

    case '>':
    case '<':
    case LE_OP:
    case GE_OP:
      genCmp(ic);
      break;

    case NE_OP:
    case EQ_OP:
      genCmpEQorNE (ic);
      break;

    case AND_OP:
    case OR_OP:
      wassertl (0, "Unimplemented iCode");
      break;

    case '^':
      genXor (ic);
      break;

    case '|':
      genOr (ic);
      break;

    case BITWISEAND:
      genAnd (ic);
      break;

    case INLINEASM:
      genInline (ic);
      break;

    case RRC:
    case RLC:
      wassertl (0, "Unimplemented iCode");
      break;

    case GETHBIT:
      wassertl (0, "Unimplemented iCode");
      break;

    case GETABIT:
      wassertl (0, "Unimplemented iCode");
      break;

    case LEFT_OP:
      genLeftShift (ic);
      break;

    case RIGHT_OP:
      genRightShift (ic);
      break;

    case GET_VALUE_AT_ADDRESS:
      genPointerGet (ic);
      break;

    case '=':
      if (!POINTER_SET (ic))
        genAssign (ic);
      else
        genPointerSet (ic);
      break;

    case IFX:
      genIfx (ic);
      break;

    case ADDRESS_OF:
      genAddrOf (ic);
      break;

    case JUMPTABLE:
      wassertl (0, "Unimplemented iCode");
      break;

    case CAST:
      genCast (ic);
      break;

    case RECEIVE:
    case SEND:
      wassertl (0, "Unimplemented iCode");
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

unsigned int
drySTM8iCode (iCode *ic)
{
  regalloc_dry_run = TRUE;
  regalloc_dry_run_cost_bytes = 0;
  regalloc_dry_run_cost_cycles = 0;

  initGenLineElement ();

  genSTM8iCode (ic);

  destroy_line_list ();

  wassert (regalloc_dry_run);

  return (regalloc_dry_run_cost_bytes);
}

/*---------------------------------------------------------------------*/
/* genSTM8Code - generate code for STM8 for a block of intructions     */
/*---------------------------------------------------------------------*/
void
genSTM8Code (iCode *lic)
{
  iCode *ic;
  int cln = 0;
  regalloc_dry_run = FALSE;

  for (ic = lic; ic; ic = ic->next)
    {
      initGenLineElement ();

      if (ic->lineno && cln != ic->lineno)
        {
          if (!options.noCcodeInAsm)
            emitcode (";", "%s: %d: %s", ic->filename, ic->lineno, printCLine (ic->filename, ic->lineno));
          cln = ic->lineno;
        }

      regalloc_dry_run_cost_bytes = 0;
      regalloc_dry_run_cost_cycles = 0;

      if (options.iCodeInAsm)
        {
          const char *iLine = printILine (ic);
          emitcode ("; ic:", "%d: %s", ic->key, iLine);
          dbuf_free (iLine);
        }

      genSTM8iCode(ic);

#if 0
      D (emitcode (";", "Cost for generated ic %d : (%d, %d)", ic->key, regalloc_dry_run_cost_bytes, regalloc_dry_run_cost_cycles));
#endif
    }

  /* now we are ready to call the
     peephole optimizer */
  if (!options.nopeep)
    peepHole (&genLine.lineHead);

  /* now do the actual printing */
  printLine (genLine.lineHead, codeOutBuf);

  /* destroy the line list */
  destroy_line_list ();
}

