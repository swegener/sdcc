/** @name Z80 Register allocation functions.
    @author Michael Hope

    Note: much of this is ripped straight from Sandeep's mcs51 code.

    This code maps the virtual symbols and code onto the real
    hardware.  It allocates based on usage and how long the varible
    lives into registers or temporary memory on the stack.

    On the Z80 hl and ix and a are reserved for the code generator,
    leaving bc and de for allocation.  iy is unusable due to currently
    as it's only adressable as a pair.  The extra register pressure
    from reserving hl is made up for by how much easier the sub
    operations become.  You could swap hl for iy if the undocumented
    iyl/iyh instructions are available.

    The stack frame is the common ix-bp style.  Basically:

    ix+4+n:     param 1
    ix+4:       param 0
    ix+2:       return address
    ix+0:       calling functions ix
    ix-n:       local varibles
    ...
    sp:         end of local varibles

    There is currently no support for bit spaces or banked functions.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2, or (at
    your option) any later version.  This program is distributed in the
    hope that it will be useful, but WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
    USA.  In other words, you are welcome to use, share and improve
    this program.  You are forbidden to forbid anyone else to use,
    share and improve what you give them.  Help stamp out
    software-hoarding!
*/

#include "z80.h"
#include "SDCCicode.h"
#include "dbuf_string.h"

/* Flags to turn off optimisations.
 */
enum
{
  DISABLE_PACK_ACC = 0,
  DISABLE_PACK_ASSIGN = 0,
  DISABLE_PACK_ONE_USE = 0,
  DISABLE_PACK_HL = 0,
  DISABLE_PACK_IY = 0
};

/* Flags to turn on debugging code.
 */
enum
{
  D_ALLOC = 0,
  D_ALLOC2 = 0,
  D_ACCUSE2 = 0,
  D_ACCUSE2_VERBOSE = 0,
  D_HLUSE = 0,
  D_HLUSE2 = 0,
  D_HLUSE2_VERBOSE = 0,
  D_FILL_GAPS = 0,
  D_PACK_IY = 0,
  D_PACK_HLUSE3 = 0
};

// #define D_ALLOC 1

#if 1
#define D(_a, _s)       if (_a)  { printf _s; fflush(stdout); }
#else
#define D(_a, _s)
#endif

#define DISABLE_PACKREGSFORSUPPORT      1
#define DISABLE_PACKREGSFORACCUSE       1

/** Local static variables */
static struct
{
  bitVect *spiltSet;
  set *stackSpil;
  bitVect *regAssigned;
  bitVect *totRegAssigned;      /* final set of LRs that got into registers */
  short blockSpil;
  int slocNum;
  /* registers used in a function */
  bitVect *funcrUsed;
  int stackExtend;
  int dataExtend;
  int nRegs;
} _G;

static reg_info _gbz80_regs[] = {
  {REG_GPR, A_IDX, "a", 1},
  {REG_GPR, C_IDX, "c", 1},
  {REG_GPR, B_IDX, "b", 1},
  {REG_GPR, E_IDX, "e", 1},
  {REG_GPR, D_IDX, "d", 1},
  {REG_GPR, L_IDX, "l", 1},
  {REG_GPR, H_IDX, "h", 1},
  {REG_CND, CND_IDX, "c", 1}
};

static reg_info _z80_regs[] = {
  {REG_GPR, A_IDX, "a", 1},
  {REG_GPR, C_IDX, "c", 1},
  {REG_GPR, B_IDX, "b", 1},
  {REG_GPR, E_IDX, "e", 1},
  {REG_GPR, D_IDX, "d", 1},
  {REG_GPR, L_IDX, "l", 1},
  {REG_GPR, H_IDX, "h", 1},
  {REG_GPR, IYL_IDX, "iyl", 1},
  {REG_GPR, IYH_IDX, "iyh", 1},
  {REG_CND, CND_IDX, "c", 1}
};

reg_info *regsZ80;

/** Number of usable registers (all but C) */
#define Z80_MAX_REGS ((sizeof(_z80_regs)/sizeof(_z80_regs[0]))-1)
#define GBZ80_MAX_REGS ((sizeof(_gbz80_regs)/sizeof(_gbz80_regs[0]))-1)

void z80SpillThis (symbol *);
static void freeAllRegs ();

/** Returns pointer to register wit index number
 */
reg_info *
regWithIdx (int idx)
{
  int i;

  for (i = C_IDX; i < _G.nRegs; i++)
    {
      if (regsZ80[i].rIdx == idx)
        {
          return &regsZ80[i];
        }
    }

  wassertl (0, "regWithIdx not found");
  exit (1);
}

/** Frees a register.
 */
static void
freeReg (reg_info *reg)
{
  wassert (!reg->isFree);
  reg->isFree = 1;
  D (D_ALLOC, ("freeReg: freed %p\n", reg));
}

/** noOverLap - will iterate through the list looking for over lap
 */
static int
noOverLap (set *itmpStack, symbol *fsym)
{
  symbol *sym;

  for (sym = setFirstItem (itmpStack); sym; sym = setNextItem (itmpStack))
    {
      if (bitVectBitValue (sym->clashes, fsym->key))
        return 0;
#if 0
      // if sym starts before (or on) our end point
      // and ends after (or on) our start point,
      // it is an overlap.
      if (sym->liveFrom <= fsym->liveTo && sym->liveTo >= fsym->liveFrom)
        {
          return 0;
        }
#endif
    }
  return 1;
}

/*-----------------------------------------------------------------*/
/* isFree - will return 1 if the a free spil location is found     */
/*-----------------------------------------------------------------*/
DEFSETFUNC (isFree)
{
  symbol *sym = item;
  V_ARG (symbol **, sloc);
  V_ARG (symbol *, fsym);

  /* if already found */
  if (*sloc)
    return 0;

  /* if it is free && and the itmp assigned to
     this does not have any overlapping live ranges
     with the one currently being assigned and
     the size can be accomodated  */
  if (sym->isFree && noOverLap (sym->usl.itmpStack, fsym) && getSize (sym->type) >= getSize (fsym->type))
    {
      *sloc = sym;
      return 1;
    }

  return 0;
}

/*-----------------------------------------------------------------*/
/* createStackSpil - create a location on the stack to spil        */
/*-----------------------------------------------------------------*/
static symbol *
createStackSpil (symbol *sym)
{
  symbol *sloc = NULL;
  struct dbuf_s dbuf;

  D (D_ALLOC, ("createStackSpil: for sym %p (%s)\n", sym, sym->name));

  /* first go try and find a free one that is already
     existing on the stack */
  if (applyToSet (_G.stackSpil, isFree, &sloc, sym) && USE_OLDSALLOC)
    {
      /* found a free one : just update & return */
      sym->usl.spillLoc = sloc;
      sym->stackSpil = 1;
      sloc->isFree = 0;
      addSetHead (&sloc->usl.itmpStack, sym);
      D (D_ALLOC, ("createStackSpil: found existing\n"));
      return sym;
    }

  /* could not then have to create one , this is the hard part
     we need to allocate this on the stack : this is really a
     hack!! but cannot think of anything better at this time */

  dbuf_init (&dbuf, 128);
  dbuf_printf (&dbuf, "sloc%d", _G.slocNum++);
  sloc = newiTemp (dbuf_c_str (&dbuf));
  dbuf_destroy (&dbuf);

  /* set the type to the spilling symbol */
  sloc->type = copyLinkChain (sym->type);
  sloc->etype = getSpec (sloc->type);
  SPEC_SCLS (sloc->etype) = S_AUTO;
  SPEC_EXTR (sloc->etype) = 0;
  SPEC_STAT (sloc->etype) = 0;
  SPEC_VOLATILE (sloc->etype) = 0;

  allocLocal (sloc);

  sloc->isref = 1;              /* to prevent compiler warning */

  wassertl (currFunc, "Local variable used outside of function.");

  /* if it is on the stack then update the stack */
  if (IN_STACK (sloc->etype))
    {
      if (currFunc)
        currFunc->stack += getSize (sloc->type);
      _G.stackExtend += getSize (sloc->type);
    }
  else
    {
      _G.dataExtend += getSize (sloc->type);
    }

  /* add it to the stackSpil set */
  addSetHead (&_G.stackSpil, sloc);
  sym->usl.spillLoc = sloc;
  sym->stackSpil = 1;

  /* add it to the set of itempStack set
     of the spill location */
  addSetHead (&sloc->usl.itmpStack, sym);

  D (D_ALLOC, ("createStackSpil: created new %s for %s\n", sloc->name, sym->name));
  return sym;
}

/*-----------------------------------------------------------------*/
/* spillThis - spils a specific operand                            */
/*-----------------------------------------------------------------*/
void
z80SpillThis (symbol * sym)
{
  int i;

  D (D_ALLOC, ("z80SpillThis: spilling %p (%s)\n", sym, sym->name));

  /* if this is rematerializable or has a spillLocation
     we are okay, else we need to create a spillLocation
     for it */
  if (!(sym->remat || sym->usl.spillLoc) || (sym->usl.spillLoc && !sym->usl.spillLoc->onStack)) // z80 port currently only supports on-stack spill locations in code generation.
    createStackSpil (sym);
  else
    D (D_ALLOC, ("Already has spilllocation %p, %s\n", sym->usl.spillLoc, sym->usl.spillLoc->name));

  /* mark it as spilt & put it in the spilt set */
  sym->isspilt = sym->spillA = 1;
  _G.spiltSet = bitVectSetBit (_G.spiltSet, sym->key);

  bitVectUnSetBit (_G.regAssigned, sym->key);
  bitVectUnSetBit (_G.totRegAssigned, sym->key);

  for (i = 0; i < sym->nRegs; i++)
    {
      if (sym->regs[i])
        {
          freeReg (sym->regs[i]);
          sym->regs[i] = NULL;
        }
    }

  if (sym->usl.spillLoc && !sym->remat)
    {
      sym->usl.spillLoc->allocreq++;
    }
  return;
}

/** Symbol has a given register.
 */
static bool
symHasReg (symbol *sym, const reg_info *reg)
{
  int i;

  for (i = 0; i < sym->nRegs; i++)
    if (sym->regs[i] == reg)
      return TRUE;

  return FALSE;
}

/** Check the live to and if they have registers & are not spilt then
    free up the registers
*/
static void
deassignLRs (iCode *ic, eBBlock *ebp)
{
  symbol *sym;
  int k;

  for (sym = hTabFirstItem (liveRanges, &k); sym; sym = hTabNextItem (liveRanges, &k))
    {

      symbol *psym = NULL;
      /* if it does not end here */
      if (sym->liveTo > ic->seq)
        continue;

      /* if it was spilt on stack then we can
         mark the stack spil location as free */
      if (sym->isspilt)
        {
          if (sym->stackSpil)
            {
              sym->usl.spillLoc->isFree = 1;
              sym->stackSpil = 0;
            }
          continue;
        }

      if (!bitVectBitValue (_G.regAssigned, sym->key))
        continue;

      /* special case check if this is an IFX &
         the privious one was a pop and the
         previous one was not spilt then keep track
         of the symbol */
      if (ic->op == IFX && ic->prev && ic->prev->op == IPOP && !ic->prev->parmPush && !OP_SYMBOL (IC_LEFT (ic->prev))->isspilt)
        psym = OP_SYMBOL (IC_LEFT (ic->prev));

      D (D_ALLOC, ("deassignLRs: in loop on sym %p nregs %u\n", sym, sym->nRegs));

      if (sym->nRegs)
        {
          int i = 0;

          bitVectUnSetBit (_G.regAssigned, sym->key);

          /* free the remaining */
          for (; i < sym->nRegs; i++)
            {
              if (psym)
                {
                  if (!symHasReg (psym, sym->regs[i]))
                    freeReg (sym->regs[i]);
                }
              else
                freeReg (sym->regs[i]);
            }
        }
    }
}

/** Reassign this to registers.
 */
static void
reassignLR (operand *op)
{
  symbol *sym = OP_SYMBOL (op);
  int i;

  D (D_ALLOC, ("reassingLR: on sym %p\n", sym));

  /* not spilt any more */
  sym->isspilt = sym->spillA = sym->blockSpil = sym->remainSpil = 0;
  bitVectUnSetBit (_G.spiltSet, sym->key);

  _G.regAssigned = bitVectSetBit (_G.regAssigned, sym->key);
  _G.totRegAssigned = bitVectSetBit (_G.totRegAssigned, sym->key);

  _G.blockSpil--;

  for (i = 0; i < sym->nRegs; i++)
    sym->regs[i]->isFree = 0;
}

/*------------------------------------------------------------------*/
/* verifyRegsAssigned - make sure an iTemp is properly initialized; */
/* it should either have registers or have beed spilled. Otherwise, */
/* there was an uninitialized variable, so just spill this to get   */
/* the operand in a valid state.                                    */
/*------------------------------------------------------------------*/
static void
verifyRegsAssigned (operand *op, iCode *ic)
{
  symbol *sym;

  if (!op)
    return;
  if (!IS_ITEMP (op))
    return;

  sym = OP_SYMBOL (op);
  if (sym->isspilt)
    return;
  if (!sym->nRegs)
    return;
  if (sym->regs[0])
    return;

  // Don't warn for new allocator, since this is now used by default.
  if (options.oldralloc)
    werrorfl (ic->filename, ic->lineno, W_LOCAL_NOINIT, sym->prereqv ? sym->prereqv->name : sym->name);
  z80SpillThis (sym);
}

/*-----------------------------------------------------------------*/
/* rUmaskForOp :- returns register mask for an operand             */
/*-----------------------------------------------------------------*/
bitVect *
rUmaskForOp (const operand * op)
{
  bitVect *rumask;
  symbol *sym;
  int j;

  /* only temporaries are assigned registers */
  if (!IS_ITEMP (op))
    return NULL;

  sym = OP_SYMBOL_CONST (op);

  /* if spilt or no registers assigned to it
     then nothing */
  if (sym->isspilt || !sym->nRegs)
    return NULL;

  rumask = newBitVect (_G.nRegs + (IS_GB ? 0 : 2));

  for (j = 0; j < sym->nRegs; j++)
    {
      if (!(sym->regs[j]) || sym->regs[j]->rIdx < 0 || sym->regs[j]->rIdx > CND_IDX)
        {
          werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "rUmaskForOp: Register not found");
          exit (0);
        }
      rumask = bitVectSetBit (rumask, sym->regs[j]->rIdx);
    }

  return rumask;
}

bitVect *
z80_rUmaskForOp (const operand * op)
{
  return rUmaskForOp (op);
}

/** Returns bit vector of registers used in iCode.
 */
bitVect *
regsUsedIniCode (iCode * ic)
{
  bitVect *rmask = newBitVect (_G.nRegs + (IS_GB ? 0 : 2));

  /* do the special cases first */
  if (ic->op == IFX)
    {
      rmask = bitVectUnion (rmask, rUmaskForOp (IC_COND (ic)));
      goto ret;
    }

  /* for the jumptable */
  if (ic->op == JUMPTABLE)
    {
      rmask = bitVectUnion (rmask, rUmaskForOp (IC_JTCOND (ic)));

      goto ret;
    }

  /* of all other cases */
  if (IC_LEFT (ic))
    rmask = bitVectUnion (rmask, rUmaskForOp (IC_LEFT (ic)));

  if (IC_RIGHT (ic))
    rmask = bitVectUnion (rmask, rUmaskForOp (IC_RIGHT (ic)));

  if (IC_RESULT (ic))
    rmask = bitVectUnion (rmask, rUmaskForOp (IC_RESULT (ic)));

ret:
  return rmask;
}

/** For each instruction will determine the regsUsed.
 */
static void
createRegMask (eBBlock ** ebbs, int count)
{
  int i;

  /* for all blocks */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      if (ebbs[i]->noPath && (ebbs[i]->entryLabel != entryLabel && ebbs[i]->entryLabel != returnLabel))
        continue;

      /* for all instructions */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {

          int j;

          if (SKIP_IC2 (ic) || !ic->rlive)
            continue;

          /* first mark the registers used in this
             instruction */

          ic->rSurv = newBitVect(port->num_regs);
          ic->rUsed = regsUsedIniCode (ic);
          _G.funcrUsed = bitVectUnion (_G.funcrUsed, ic->rUsed);

          /* now create the register mask for those
             registers that are in use : this is a
             super set of ic->rUsed */
          ic->rMask = newBitVect (_G.nRegs + 1 + (IS_GB ? 0 : 2));

          /* for all live Ranges alive at this point */
          for (j = 1; j < ic->rlive->size; j++)
            {
              symbol *sym;
              int k;

              /* if not alive then continue */
              if (!bitVectBitValue (ic->rlive, j))
                continue;

              /* find the live range we are interested in */
              if (!(sym = hTabItemWithKey (liveRanges, j)))
                {
                  werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "createRegMask cannot find live range");
                  exit (0);
                }

              /* if no register assigned to it */
              if (!sym->nRegs || sym->isspilt)
                continue;

              /* for all the registers allocated to it */
              for (k = 0; k < sym->nRegs; k++)
                {
                  if (!sym->regs[k])
                    continue;
                  ic->rMask = bitVectSetBit (ic->rMask, sym->regs[k]->rIdx);
                  if (sym->liveTo != ic->key)
                    ic->rSurv = bitVectSetBit (ic->rSurv, sym->regs[k]->rIdx);
                }
            }
        }
    }
}

#if 0
/** Returns the rematerialized string for a remat var.
 */
static char *
rematStr (symbol * sym)
{
  iCode *ic = sym->rematiCode;
  int offset = 0;

  while (1)
    {
      /* if plus adjust offset to right hand side */
      if (ic->op == '+')
        {
          offset += (int) operandLitValue (IC_RIGHT (ic));
          ic = OP_SYMBOL (IC_LEFT (ic))->rematiCode;
          continue;
        }

      /* if minus adjust offset to right hand side */
      if (ic->op == '-')
        {
          offset -= (int) operandLitValue (IC_RIGHT (ic));
          ic = OP_SYMBOL (IC_LEFT (ic))->rematiCode;
          continue;
        }

      /* cast then continue */
      if (IS_CAST_ICODE (ic))
        {
          ic = OP_SYMBOL (IC_RIGHT (ic))->rematiCode;
          continue;
        }
      /* we reached the end */
      break;
    }

  if (offset)
    {
      SNPRINTF (buffer, sizeof (buffer),
                "(%s %c 0x%04x)", OP_SYMBOL (IC_LEFT (ic))->rname, offset >= 0 ? '+' : '-', abs (offset) & 0xffff);
    }
  else
    {
      strncpyz (buffer, OP_SYMBOL (IC_LEFT (ic))->rname, sizeof (buffer));
    }
  return buffer;
}
#endif

/*-----------------------------------------------------------------*/
/* regTypeNum - computes the type & number of registers required   */
/*-----------------------------------------------------------------*/
static void
regTypeNum (void)
{
  symbol *sym;
  int k;

  /* for each live range do */
  for (sym = hTabFirstItem (liveRanges, &k); sym; sym = hTabNextItem (liveRanges, &k))
    {
      /* if used zero times then no registers needed */
      if ((sym->liveTo - sym->liveFrom) == 0 && getSize (sym->type) <= 4)
        continue;
      else if ((sym->liveTo - sym->liveFrom) == 0 && bitVectnBitsOn (sym->defs) <= 1)
        {
          iCode *dic = hTabItemWithKey (iCodehTab, bitVectFirstBit (sym->defs));
          if (!dic || dic->op != CALL && dic->op != PCALL)
            continue;
        }

      D (D_ALLOC, ("regTypeNum: loop on sym %p\n", sym));

      /* if the live range is a temporary */
      if (sym->isitmp)
        {
          /* if the type is marked as a conditional */
          if (sym->regType == REG_CND)
            continue;

          /* if used in return only then we don't
             need registers */
          if (sym->ruonly || sym->accuse)
            {
              if (IS_AGGREGATE (sym->type) || sym->isptr)
                sym->type = aggrToPtr (sym->type, FALSE);
              continue;
            }

          /* if not then we require registers */
          D (D_ALLOC,
             ("regTypeNum: isagg %u nRegs %u type %p\n", IS_AGGREGATE (sym->type) || sym->isptr, sym->nRegs, sym->type));
          sym->nRegs =
            ((IS_AGGREGATE (sym->type)
              || sym->isptr) ? getSize (sym->type = aggrToPtr (sym->type, FALSE)) : getSize (sym->type));
          D (D_ALLOC, ("regTypeNum: setting nRegs of %s (%p) to %u\n", sym->name, sym, sym->nRegs));

          D (D_ALLOC, ("regTypeNum: setup to assign regs sym %p\n", sym));

          if (sym->nRegs > 8)
            {
              fprintf (stderr, "allocated more than 8 registers for type ");
              printTypeChain (sym->type, stderr);
              fprintf (stderr, "\n");
            }

          /* determine the type of register required */
          /* Always general purpose */
          sym->regType = REG_GPR;
        }
      else
        {
          /* for the first run we don't provide */
          /* registers for true symbols we will */
          /* see how things go                  */
          D (D_ALLOC, ("regTypeNum: #2 setting num of %p to 0\n", sym));
          sym->nRegs = 0;
        }
    }
}

/** Mark all registers as free.
 */
static void
freeAllRegs ()
{
  int i;

  D (D_ALLOC, ("freeAllRegs: running.\n"));

  for (i = C_IDX; i < _G.nRegs; i++)
    regsZ80[i].isFree = 1;
}

/*-----------------------------------------------------------------*/
/* deallocStackSpil - this will set the stack pointer back         */
/*-----------------------------------------------------------------*/
static
DEFSETFUNC (deallocStackSpil)
{
  symbol *sym = item;

  deallocLocal (sym);
  return 0;
}

/** Register reduction for assignment.
 */
static int
packRegsForAssign (iCode * ic, eBBlock * ebp)
{
  iCode *dic, *sic;

  D (D_ALLOC, ("packRegsForAssign: running on ic %p\n", ic));

  if (!IS_ITEMP (IC_RIGHT (ic)) || OP_SYMBOL (IC_RIGHT (ic))->isind || OP_LIVETO (IC_RIGHT (ic)) > ic->seq)
    return 0;
  
  /* Avoid having multiple named address spaces in one iCode. */
  if (IS_SYMOP (IC_RESULT (ic)) && SPEC_ADDRSPACE (OP_SYMBOL (IC_RESULT (ic))->etype))
    return 0;

  /* find the definition of iTempNN scanning backwards if we find a
     a use of the true symbol in before we find the definition then
     we cannot */
  for (dic = ic->prev; dic; dic = dic->prev)
    {
      /* PENDING: Don't pack across function calls. */
      if (dic->op == CALL || dic->op == PCALL || dic->op == INLINEASM || dic->op == CRITICAL || dic->op == ENDCRITICAL)
        {
          dic = NULL;
          break;
        }

      if (SKIP_IC2 (dic))
        continue;

      if (dic->op == IFX)
        {
          if (IS_SYMOP (IC_COND (dic)) &&
              (IC_COND (dic)->key == IC_RESULT (ic)->key || IC_COND (dic)->key == IC_RIGHT (ic)->key))
            {
              dic = NULL;
              break;
            }
        }
      else
        {
          if (IS_TRUE_SYMOP (IC_RESULT (dic)) && IS_OP_VOLATILE (IC_RESULT (dic)))
            {
              dic = NULL;
              break;
            }

          if (IS_SYMOP (IC_RESULT (dic)) && IC_RESULT (dic)->key == IC_RIGHT (ic)->key)
            {
              if (POINTER_SET (dic))
                dic = NULL;

              break;
            }

          if (IS_SYMOP (IC_RIGHT (dic)) &&
              (IC_RIGHT (dic)->key == IC_RESULT (ic)->key || IC_RIGHT (dic)->key == IC_RIGHT (ic)->key))
            {
              dic = NULL;
              break;
            }

          if (IS_SYMOP (IC_LEFT (dic)) &&
              (IC_LEFT (dic)->key == IC_RESULT (ic)->key || IC_LEFT (dic)->key == IC_RIGHT (ic)->key))
            {
              dic = NULL;
              break;
            }

          if (IS_SYMOP (IC_RESULT (dic)) && IC_RESULT (dic)->key == IC_RESULT (ic)->key)
            {
              dic = NULL;
              break;
            }
        }
    }

  if (!dic)
    return 0;                   /* did not find */

  /* if assignment then check that right is not a bit */
  if (ASSIGNMENT (ic) && !POINTER_SET (ic))
    {
      sym_link *etype = operandType (IC_RESULT (dic));
      if (IS_BITFIELD (etype))
        {
          /* if result is a bit too then it's ok */
          etype = operandType (IC_RESULT (ic));
          if (!IS_BITFIELD (etype))
            {
              return 0;
            }
        }
    }

  /* if the result is on stack or iaccess then it must be
     the same atleast one of the operands */
  if (OP_SYMBOL (IC_RESULT (ic))->onStack || OP_SYMBOL (IC_RESULT (ic))->iaccess)
    {
      /* the operation has only one symbol
         operator then we can pack */
      if ((IC_LEFT (dic) && !IS_SYMOP (IC_LEFT (dic))) || (IC_RIGHT (dic) && !IS_SYMOP (IC_RIGHT (dic))))
        goto pack;

      if (!((IC_LEFT (dic) &&
             IC_RESULT (ic)->key == IC_LEFT (dic)->key) || (IC_RIGHT (dic) && IC_RESULT (ic)->key == IC_RIGHT (dic)->key)))
        return 0;
    }

pack:
  /* Keep assignment if it is an sfr write  - not all of code generation can deal with result in sfr */
  if (IC_RESULT (ic) && IS_TRUE_SYMOP (IC_RESULT (ic)) && SPEC_OCLS (OP_SYMBOL (IC_RESULT (ic))->etype) && IN_REGSP (SPEC_OCLS (OP_SYMBOL (IC_RESULT (ic))->etype)) &&
    (dic->op == LEFT_OP || dic->op == RIGHT_OP))
    return 0;

  /* found the definition */

  /* delete from liverange table also
     delete from all the points inbetween and the new
     one */
  for (sic = dic; sic != ic; sic = sic->next)
    {
      bitVectUnSetBit (sic->rlive, IC_RESULT (ic)->key);
      if (IS_ITEMP (IC_RESULT (dic)))
        bitVectSetBit (sic->rlive, IC_RESULT (dic)->key);
    }

  /* replace the result with the result of */
  /* this assignment and remove this assignment */
  bitVectUnSetBit (OP_SYMBOL (IC_RESULT (dic))->defs, dic->key);
  IC_RESULT (dic) = IC_RESULT (ic);

  if (IS_ITEMP (IC_RESULT (dic)) && OP_SYMBOL (IC_RESULT (dic))->liveFrom > dic->seq)
    {
      OP_SYMBOL (IC_RESULT (dic))->liveFrom = dic->seq;
    }

  remiCodeFromeBBlock (ebp, ic);
  // PENDING: Check vs mcs51
  bitVectUnSetBit (OP_SYMBOL (IC_RESULT (ic))->defs, ic->key);
  hTabDeleteItem (&iCodehTab, ic->key, ic, DELETE_ITEM, NULL);
  OP_DEFS (IC_RESULT (dic)) = bitVectSetBit (OP_DEFS (IC_RESULT (dic)), dic->key);
  return 1;
}

/** Scanning backwards looks for first assig found.
 */
iCode *
findAssignToSym (operand * op, iCode * ic)
{
  iCode *dic;

  for (dic = ic->prev; dic; dic = dic->prev)
    {

      /* if definition by assignment */
      if (dic->op == '=' && !POINTER_SET (dic) && IC_RESULT (dic)->key == op->key)
        /*      &&  IS_TRUE_SYMOP(IC_RIGHT(dic)) */
        {

          /* we are interested only if defined in far space */
          /* or in stack space in case of + & - */

          /* if assigned to a non-symbol then return
             true */
          if (!IS_SYMOP (IC_RIGHT (dic)))
            break;

          /* if the symbol is in far space then
             we should not */
          if (isOperandInFarSpace (IC_RIGHT (dic)))
            return NULL;

          /* for + & - operations make sure that
             if it is on the stack it is the same
             as one of the three operands */
          if ((ic->op == '+' || ic->op == '-') && OP_SYMBOL (IC_RIGHT (dic))->onStack)
            {

              if (IC_RESULT (ic)->key != IC_RIGHT (dic)->key &&
                  IC_LEFT (ic)->key != IC_RIGHT (dic)->key && IC_RIGHT (ic)->key != IC_RIGHT (dic)->key)
                return NULL;
            }

          break;

        }

      /* if we find an usage then we cannot delete it */
      if (IC_LEFT (dic) && IC_LEFT (dic)->key == op->key)
        return NULL;

      if (IC_RIGHT (dic) && IC_RIGHT (dic)->key == op->key)
        return NULL;

      if (POINTER_SET (dic) && IC_RESULT (dic)->key == op->key)
        return NULL;
    }

  /* now make sure that the right side of dic
     is not defined between ic & dic */
  if (dic)
    {
      iCode *sic = dic->next;

      for (; sic != ic; sic = sic->next)
        if (IC_RESULT (sic) && IC_RESULT (sic)->key == IC_RIGHT (dic)->key)
          return NULL;
    }

  return dic;


}

#if !DISABLE_PACKREGSFORSUPPORT
// PENDING

/*-----------------------------------------------------------------*/
/* packRegsForSupport :- reduce some registers for support calls   */
/*-----------------------------------------------------------------*/
static int
packRegsForSupport (iCode * ic, eBBlock * ebp)
{
  int change = 0;
  /* for the left & right operand :- look to see if the
     left was assigned a true symbol in far space in that
     case replace them */
  D (D_ALLOC, ("packRegsForSupport: running on ic %p\n", ic));

  if (IS_ITEMP (IC_LEFT (ic)) && OP_SYMBOL (IC_LEFT (ic))->liveTo <= ic->seq)
    {
      iCode *dic = findAssignToSym (IC_LEFT (ic), ic);
      iCode *sic;

      if (!dic)
        goto right;

      /* found it we need to remove it from the
         block */
      for (sic = dic; sic != ic; sic = sic->next)
        bitVectUnSetBit (sic->rlive, IC_LEFT (ic)->key);

      IC_LEFT (ic)->operand.symOperand = IC_RIGHT (dic)->operand.symOperand;
      IC_LEFT (ic)->key = IC_RIGHT (dic)->operand.symOperand->key;
      remiCodeFromeBBlock (ebp, dic);
      bitVectUnSetBit (OP_SYMBOL (IC_RESULT (dic))->defs, dic->key);
      hTabDeleteItem (&iCodehTab, dic->key, dic, DELETE_ITEM, NULL);
      // PENDING: Check vs mcs51
      change++;
    }

  /* do the same for the right operand */
right:
  if (!change && IS_ITEMP (IC_RIGHT (ic)) && OP_SYMBOL (IC_RIGHT (ic))->liveTo <= ic->seq)
    {
      iCode *dic = findAssignToSym (IC_RIGHT (ic), ic);
      iCode *sic;

      if (!dic)
        return change;

      /* found it we need to remove it from the block */
      for (sic = dic; sic != ic; sic = sic->next)
        bitVectUnSetBit (sic->rlive, IC_RIGHT (ic)->key);

      IC_RIGHT (ic)->operand.symOperand = IC_RIGHT (dic)->operand.symOperand;
      IC_RIGHT (ic)->key = IC_RIGHT (dic)->operand.symOperand->key;

      remiCodeFromeBBlock (ebp, dic);
      bitVectUnSetBit (OP_SYMBOL (IC_RESULT (dic))->defs, dic->key);
      hTabDeleteItem (&iCodehTab, dic->key, dic, DELETE_ITEM, NULL);
      // PENDING: vs mcs51
      change++;
    }

  return change;
}
#endif

/** Will reduce some registers for single use.
 */
static iCode *
packRegsForOneuse (iCode * ic, operand * op, eBBlock * ebp)
{
  bitVect *uses;
  iCode *dic, *sic;

  // PENDING: Disable
  D (D_ALLOC, ("packRegsForOneUse: running on ic %p\n", ic));

  /* if returning a literal then do nothing */
  if (!IS_SYMOP (op))
    return NULL;

  /* only upto 2 bytes since we cannot predict
     the usage of b, & acc */
  if (getSize (operandType (op)) > 2)
    return NULL;

  if (ic->op != RETURN && ic->op != SEND)
    return NULL;

  /* this routine will mark the a symbol as used in one
     instruction use only && if the defintion is local
     (ie. within the basic block) && has only one definition &&
     that definiion is either a return value from a
     function or does not contain any variables in
     far space */
  uses = bitVectCopy (OP_USES (op));
  bitVectUnSetBit (uses, ic->key);      /* take away this iCode */
  if (!bitVectIsZero (uses))    /* has other uses */
    return NULL;

  /* if it has only one defintion */
  if (bitVectnBitsOn (OP_DEFS (op)) > 1)
    return NULL;                /* has more than one definition */

  /* get that definition */
  if (!(dic = hTabItemWithKey (iCodehTab, bitVectFirstBit (OP_DEFS (op)))))
    return NULL;

  /* found the definition now check if it is local */
  if (dic->seq < ebp->fSeq || dic->seq > ebp->lSeq)
    return NULL;                /* non-local */

  /* now check if it is the return from a function call */
  if (dic->op == CALL || dic->op == PCALL)
    {
      if (ic->op != SEND && ic->op != RETURN && !POINTER_SET (ic) && !POINTER_GET (ic))
        {
          OP_SYMBOL (op)->ruonly = 1;
          return dic;
        }
      dic = dic->next;
    }

  /* otherwise check that the definition does
     not contain any symbols in far space */
  if (isOperandInFarSpace (IC_LEFT (dic)) ||
      isOperandInFarSpace (IC_RIGHT (dic)) || IS_OP_RUONLY (IC_LEFT (ic)) || IS_OP_RUONLY (IC_RIGHT (ic)))
    {
      return NULL;
    }

  /* if pointer set then make sure the pointer is one byte */
  if (POINTER_SET (dic))
    return NULL;

  if (POINTER_GET (dic))
    return NULL;

  sic = dic;

  /* also make sure the intervenening instructions
     don't have any thing in far space */
  for (dic = dic->next; dic && dic != ic; dic = dic->next)
    {
      /* if there is an intervening function call then no */
      if (dic->op == CALL || dic->op == PCALL)
        return NULL;
      /* if pointer set then make sure the pointer
         is one byte */
      if (POINTER_SET (dic))
        return NULL;

      if (POINTER_GET (dic))
        return NULL;

      /* if address of & the result is remat the okay */
      if (dic->op == ADDRESS_OF && OP_SYMBOL (IC_RESULT (dic))->remat)
        continue;

      /* if left or right or result is in far space */
      if (isOperandInFarSpace (IC_LEFT (dic)) ||
          isOperandInFarSpace (IC_RIGHT (dic)) ||
          isOperandInFarSpace (IC_RESULT (dic)) ||
          IS_OP_RUONLY (IC_LEFT (dic)) || IS_OP_RUONLY (IC_RIGHT (dic)) || IS_OP_RUONLY (IC_RESULT (dic)))
        {
          return NULL;
        }
    }

  /* Fixes #2646174, but there might be a better way */
  if (ic->op == SEND)
    return NULL;

  /* Fixes #2982135, but there might be a better way */
  if (ic->op == RETURN)
    return NULL;

  OP_SYMBOL (op)->ruonly = 1;
  return sic;
}

/*-----------------------------------------------------------------*/
/* isBitwiseOptimizable - requirements of JEAN LOUIS VERN          */
/*-----------------------------------------------------------------*/
static bool
isBitwiseOptimizable (iCode * ic)
{
  sym_link *rtype = getSpec (operandType (IC_RIGHT (ic)));

  /* bitwise operations are considered optimizable
     under the following conditions (Jean-Louis VERN)

     x & lit
     bit & bit
     bit & x
     bit ^ bit
     bit ^ x
     x   ^ lit
     x   | lit
     bit | bit
     bit | x
   */
  if (IS_LITERAL (rtype))
    return TRUE;
  return FALSE;
}

static iCode *
packRegsForHLUse3 (iCode * lic, operand * op, eBBlock * ebp)
{
  int i, key;
  symbol *sym;
  iCode *ic, *dic;
  bool isFirst = TRUE;
  bool exstk = (currFunc && currFunc->stack > 127);

  D (D_PACK_HLUSE3,
     ("Checking HL on %p lic key %u first def %u line %u:\n", OP_SYMBOL (op), lic->key, bitVectFirstBit (OP_DEFS (op)),
      lic->lineno));
  if (D_PACK_HLUSE3)
    piCode (lic, NULL);

  if (OP_SYMBOL (op)->accuse)
    {
      return NULL;
    }

  if (OP_SYMBOL (op)->remat)
    {
      return NULL;
    }

  /* Only defined once */
  if (bitVectnBitsOn (OP_DEFS (op)) > 1)
    return NULL;

  if (getSize (operandType (op)) != 2)
    return NULL;

  /* And this is the definition */
  if (bitVectFirstBit (OP_DEFS (op)) != lic->key)
    return NULL;

  /* first check if any overlapping liverange has already been
     assigned to DPTR */
  if (OP_SYMBOL (op)->clashes)
    {
      for (i = 0; i < OP_SYMBOL (op)->clashes->size; i++)
        {
          if (bitVectBitValue (OP_SYMBOL (op)->clashes, i))
            {
              sym = hTabItemWithKey (liveRanges, i);
              if (sym->accuse == ACCUSE_SCRATCH)
                {
                  return NULL;
                }
            }
        }
    }

  /* Nothing else that clashes with this is using the scratch
     register.  Scan through all of the intermediate instructions and
     see if any of them could nuke HL.
   */
  dic = ic = hTabFirstItemWK (iCodeSeqhTab, OP_SYMBOL (op)->liveFrom);

  for (; ic && ic->seq <= OP_SYMBOL (op)->liveTo; ic = hTabNextItem (iCodeSeqhTab, &key))
    {
      if (D_PACK_HLUSE3)
        piCode (ic, NULL);
      D (D_PACK_HLUSE3, ("(On %p: op: %u next: %p)\n", ic, ic->op, ic->next));

      if (isFirst)
        {
          isFirst = FALSE;
          if (ic->op == ADDRESS_OF)
            continue;
          if (POINTER_GET (ic))
            continue;
          if (ic->op == '=' && !POINTER_SET (ic))
            continue;
        }

      if (IC_RESULT (ic) && IS_SYMOP (IC_RESULT (ic)) && (isOperandInDirSpace (IC_RESULT (ic)) || exstk))
        return NULL;

      if (IC_LEFT (ic) && IS_SYMOP (IC_LEFT (ic)) && isOperandInDirSpace (IC_LEFT (ic)))
        return NULL;

      if (IC_RIGHT (ic) && IS_SYMOP (IC_RIGHT (ic)) && isOperandInDirSpace (IC_RIGHT (ic)))
        return NULL;

      /* Handle the non left/right/result ones first */
      if (ic->op == IFX)
        continue;
      if (ic->op == JUMPTABLE)
        return NULL;

      if (SKIP_IC2 (ic))
        continue;

      if (ic->op == CAST)
        continue;

      if (ic->op == IPUSH && isOperandEqual (op, IC_LEFT (ic)))
        continue;

      if (ic->op == SEND && isOperandEqual (op, IC_LEFT (ic)))
        continue;

      if (ic->op == CALL && isOperandEqual (op, IC_RESULT (ic)))
        continue;

      if (ic->op == LEFT_OP && isOperandLiteral (IC_RIGHT (ic)))
        continue;

      if (ic->op == '+' && (isOperandEqual (op, IC_LEFT (ic)) || isOperandEqual (op, IC_RIGHT (ic))))
        continue;

      if ((ic->op == '=' && !POINTER_SET (ic)) ||
          ic->op == UNARYMINUS ||
          ic->op == RETURN ||
          ic->op == RIGHT_OP ||
          (ic->op == '-'  && getSize (operandType (IC_RESULT (ic))) == 1) ||
          ic->op == BITWISEAND ||
          ic->op == '|' ||
          ic->op == '>' || ic->op == '<' || ic->op == EQ_OP || (ic->op == '+' && getSize (operandType (IC_RESULT (ic))) == 1))
        /* 16 bit addition uses add hl, rr */
        continue;

      if (ic->op == '*' && isOperandEqual (op, IC_LEFT (ic)))
        continue;

      if (POINTER_SET (ic) && isOperandEqual (op, IC_RESULT (ic)))
        continue;

      if (POINTER_GET (ic) && isOperandEqual (op, IC_LEFT (ic)))
        continue;

      if (IS_VALOP (IC_RIGHT (ic)) && (ic->op == EQ_OP || 0))
        {
          continue;
        }

      /* By default give up */
      D (D_PACK_HLUSE3, ("packRegsForHLUse3 giving up at icode %u\n", (unsigned) (ic->op)));
      return NULL;
    }

  D (D_PACK_HLUSE3, ("Succeeded!\n")) OP_SYMBOL (op)->accuse = ACCUSE_SCRATCH;
  return dic;
}

static iCode *
packRegsForIYUse (iCode * lic, operand * op, eBBlock * ebp)
{
  int i, key;
  symbol *sym;
  iCode *ic, *dic;
  bitVect *uses;

  D (D_PACK_IY,
     ("Checking IY on %p lic key %u first def %u line %u:\n", OP_SYMBOL (op), lic->key, bitVectFirstBit (OP_DEFS (op)),
      lic->lineno));
  if (D_PACK_IY)
    piCode (lic, NULL);

  if (OP_SYMBOL (op)->accuse)
    {
      return NULL;
    }

  if (OP_SYMBOL (op)->remat)
    {
      return NULL;
    }

  /* Only defined once */
  if (bitVectnBitsOn (OP_DEFS (op)) > 1)
    return NULL;

  /* And this is the definition */
  if (bitVectFirstBit (OP_DEFS (op)) != lic->key)
    return NULL;

  /* first check if any overlapping liverange has already been
     assigned to DPTR */
  if (OP_SYMBOL (op)->clashes)
    {
      for (i = 0; i < OP_SYMBOL (op)->clashes->size; i++)
        {
          if (bitVectBitValue (OP_SYMBOL (op)->clashes, i))
            {
              sym = hTabItemWithKey (liveRanges, i);
              if (sym->accuse == ACCUSE_IY)
                {
                  return NULL;
                }
            }
        }
    }

  /* Only a few instructions can load into IY */
  if (lic->op != '=')
    {
      return NULL;
    }

  if (getSize (operandType (op)) != 2)
    {
      D (D_PACK_IY, ("  + Dropping as operation has size is too big\n"));
      return FALSE;
    }

  /* Nothing else that clashes with this is using the scratch
     register.  Scan through all of the intermediate instructions and
     see if any of them could nuke HL.
   */
  dic = ic = hTabFirstItemWK (iCodeSeqhTab, OP_SYMBOL (op)->liveFrom);
  uses = OP_USES (op);

  for (; ic && ic->seq <= OP_SYMBOL (op)->liveTo; ic = hTabNextItem (iCodeSeqhTab, &key))
    {
      if (D_PACK_IY)
        piCode (ic, NULL);

      if (ic->op == PCALL || ic->op == CALL || ic->op == JUMPTABLE)
        return NULL;

      if (SKIP_IC2 (ic))
        continue;

      /* Be pessamistic. */
      if (ic->op == IFX)
        return NULL;

      D (D_PACK_IY, ("  op: %u uses %u result: %d left: %d right: %d\n", ic->op, bitVectBitValue (uses, ic->key),
                     IC_RESULT (ic) && IS_SYMOP (IC_RESULT (ic)) ? isOperandInDirSpace (IC_RESULT (ic)) : -1,
                     IC_LEFT (ic) && IS_SYMOP (IC_LEFT (ic)) ? isOperandInDirSpace (IC_LEFT (ic)) : -1,
                     IC_RIGHT (ic) && IS_SYMOP (IC_RIGHT (ic)) ? isOperandInDirSpace (IC_RIGHT (ic)) : -1));

      if (IC_RESULT (ic) && IS_SYMOP (IC_RESULT (ic)) && isOperandInDirSpace (IC_RESULT (ic)))
        return NULL;

      if (IC_RIGHT (ic) && IS_SYMOP (IC_RIGHT (ic)) && isOperandInDirSpace (IC_RIGHT (ic)))
        return NULL;

      if (IC_LEFT (ic) && IS_SYMOP (IC_LEFT (ic)) && isOperandInDirSpace (IC_LEFT (ic)))
        return NULL;

      /* Only certain rules will work against IY.  Check if this iCode uses
         this symbol. */
      if (bitVectBitValue (uses, ic->key) != 0)
        {
          if (ic->op == '=' && isOperandEqual (IC_RESULT (ic), op))
            continue;

          if (ic->op == GET_VALUE_AT_ADDRESS && isOperandEqual (IC_LEFT (ic), op))
            continue;

          if (isOperandEqual (IC_RESULT (ic), IC_LEFT (ic)) == FALSE)
            return NULL;

          if (IC_RIGHT (ic) && IS_VALOP (IC_RIGHT (ic)))
            {
              if (ic->op == '+' || ic->op == '-')
                {
                  /* Only works if the constant is small */
                  if (operandLitValue (IC_RIGHT (ic)) < 4)
                    continue;
                }
            }

          return NULL;
        }
      else
        {
          /* This iCode doesn't use the sym.  See if this iCode preserves IY.
           */
          continue;
        }

      /* By default give up */
      return NULL;
    }

  D (D_PACK_IY, ("Succeeded IY!\n"));

  OP_SYMBOL (op)->accuse = ACCUSE_IY;
  return dic;
}

/** Returns TRUE if this operation can use acc and if it preserves the value.
 */
static bool
opPreservesA (iCode * uic)
{
  if (uic->op == IFX)
    {
      /* If we've gotten this far then the thing to compare must be
         small enough and must be in A.
       */
      return TRUE;
    }

  if (uic->op == JUMPTABLE)
    {
      D (D_ACCUSE2, ("  + Dropping as operation is a Jumptable\n"));
      return FALSE;
    }

  /* A pointer assign preserves A if A is the left value. */
  if (uic->op == '=' && POINTER_SET (uic) && !IY_RESERVED)
    {
      return TRUE;
    }

  /* if the usage has only one operand then we can */
  /* PENDING: check */
  if (IC_LEFT (uic) == NULL || IC_RIGHT (uic) == NULL)
    {
      D (D_ACCUSE2, ("  + Dropping as operation has only one operand\n"));
      return FALSE;
    }

  /* PENDING: check this rule */
  if (getSize (operandType (IC_RESULT (uic))) > 1)
    {
      D (D_ACCUSE2, ("  + Dropping as operation has size is too big\n"));
      return FALSE;
    }

  return FALSE;
}

/* Return TRUE if this operation can use A (it doesn't have to keep A unchanged) */
static bool
opCanUseA (iCode * uic)
{
  if (uic->op == IFX)
    {
      /* If we've gotten this far then the thing to compare must be
         small enough and must be in A.
       */
      return TRUE;
    }
  if (uic->op == JUMPTABLE)
    {
      D (D_ACCUSE2, ("  + Dropping as operation is a Jumptable\n"));
      return FALSE;
    }

  if (uic->op == '=' && !(IY_RESERVED && POINTER_SET (uic)))
    {
      return TRUE;
    }

  if ((uic->op == RIGHT_OP || uic->op == LEFT_OP) && IS_OP_LITERAL (IC_RIGHT (uic)))
    {
      return TRUE;
    }

  /* Somehow fails for unary minus. */
  if (IC_LEFT (uic) == NULL || IC_RIGHT (uic) == NULL)
    {
      D (D_ACCUSE2, ("  + Dropping as operation has only one operand\n"));
      return FALSE;
    }

  if (IS_BITWISE_OP (uic))
    {
      return TRUE;
    }

  return FALSE;
}

/** Returns true if this operand preserves the value of A.
 */
static bool
opIgnoresA (iCode * ic, iCode * uic)
{
  /* A increment of an iTemp by a constant is OK. */
  if (uic->op == '+' && IS_ITEMP (IC_LEFT (uic)) && IS_ITEMP (IC_RESULT (uic)) && IS_OP_LITERAL (IC_RIGHT (uic)))
    {
      unsigned int icount = (unsigned int) ulFromVal (OP_VALUE (IC_RIGHT (uic)));

      /* Being an ITEMP means that we're already a symbol. */
      if (icount == 1 && OP_KEY (IC_RESULT (uic)) == OP_KEY (IC_LEFT (uic)))
        {
          return TRUE;
        }
    }
  else if (uic->op == '=' && !POINTER_SET (uic))
    {
      /* If they are equal and get optimised out then things are OK. */
      if (isOperandEqual (IC_RESULT (uic), IC_RIGHT (uic)))
        {
          /* Straight assign is OK. */
          return TRUE;
        }
    }

  return FALSE;
}

/** Pack registers for acc use.
    When the result of this operation is small and short lived it may
    be able to be stored in the accumulator.

    Note that the 'A preserving' list is currently emperical :)
 */
static void
packRegsForAccUse2 (iCode * ic)
{
  iCode *uic;

  D (D_ACCUSE2, ("packRegsForAccUse2: running on ic %p line %u\n", ic, ic->lineno));
  if (D_ACCUSE2)
    piCode (ic, NULL);

  /* Filter out all but those 'good' commands */
  if (!POINTER_GET (ic) &&
      ic->op != '+' &&
      ic->op != '-' &&
      !IS_BITWISE_OP (ic) &&
      ic->op != '=' &&
      ic->op != EQ_OP &&
      ic->op != '<' &&
      ic->op != '>' &&
      ic->op != CAST && ic->op != GETHBIT && !((ic->op == LEFT_OP || ic->op == RIGHT_OP) && IS_OP_LITERAL (IC_RIGHT (ic))))
    {
      D (D_ACCUSE2, ("  + Dropping as not a 'good' source command\n"));
      return;
    }

  /* if + or - then it has to be one byte result.
     MLH: Ok.
   */
  if ((ic->op == '+' || ic->op == '-' || ic->op == LEFT_OP || ic->op == RIGHT_OP) && getSize (operandType (IC_RESULT (ic))) > 1)
    {
      D (D_ACCUSE2, ("  + Dropping as it's a big one\n"));
      return;
    }

  /* has only one definition */
  if (bitVectnBitsOn (OP_DEFS (IC_RESULT (ic))) > 1)
    {
      D (D_ACCUSE2, ("  + Dropping as it has more than one definition\n"));
      return;
    }

  /* Right.  We may be able to propagate it through if:
     For each in the chain of uses the intermediate is OK.
   */
  /* Get next with 'uses result' bit on
     If this->next == next
     Validate use of next
     If OK, increase count
   */
  /* and the usage immediately follows this iCode */
  if (!(uic = hTabItemWithKey (iCodehTab, bitVectFirstBit (OP_USES (IC_RESULT (ic))))))
    {
      D (D_ACCUSE2, ("  + Dropping as usage does not follow first\n"));
      return;
    }

  {
    /* Create a copy of the OP_USES bit vect */
    bitVect *uses = bitVectCopy (OP_USES (IC_RESULT (ic)));
    int setBit;
    iCode *scan = ic, *next;

    do
      {
        setBit = bitVectFirstBit (uses);
        next = hTabItemWithKey (iCodehTab, setBit);
        if (scan->next == next)
          {
            D (D_ACCUSE2_VERBOSE, ("  ! Is next in line\n"));

            bitVectUnSetBit (uses, setBit);

            if (bitVectIsZero (uses) && opCanUseA (next))
              {
                D (D_ACCUSE2, ("  Arrived at last op safely.\n"));
                break;
              }
            /* Still contigous. */
            if (!opPreservesA (next))
              {
                D (D_ACCUSE2, ("  + Dropping as operation doesn't preserve A\n"));
                return;
              }
            D (D_ACCUSE2_VERBOSE, ("  ! Preserves A, so continue scanning\n"));
            scan = next;
          }
        /*else if (scan->next == NULL && bitVectnBitsOn (uses) == 1 && next != NULL)
           {
           if (next->prev == NULL)
           {
           if (!opPreservesA (next))
           {
           D (D_ACCUSE2, ("  + Dropping as operation doesn't preserve A #2\n"));
           return;
           }
           bitVectUnSetBit (uses, setBit);
           scan = next;
           }
           else
           {
           D (D_ACCUSE2, ("  + Dropping as last in list and next doesn't start a block\n"));
           return;
           }
           } //This caused bug #1292721 */
        else if (scan->next == NULL)
          {
            D (D_ACCUSE2, ("  + Dropping as hit the end of the list\n"));
            D (D_ACCUSE2, ("  + Next in htab: %p\n", next));
            return;
          }
        else
          {
            if (opIgnoresA (ic, scan->next))
              {
                /* Safe for now. */
                scan = scan->next;
                D (D_ACCUSE2_VERBOSE, ("  ! Op ignores A, so continue scanning\n"));
              }
            else
              {
                D (D_ACCUSE2, ("  + Dropping as parts are not consecuitive and intermediate might use A\n"));
                return;
              }
          }
      }
    while (!bitVectIsZero (uses));

    OP_SYMBOL (IC_RESULT (ic))->accuse = ACCUSE_A;
    return;
  }
}

/** Does some transformations to reduce register pressure.
 */
static void
packRegisters (eBBlock * ebp)
{
  iCode *ic;
  int change = 0;

  D (D_ALLOC, ("packRegisters: entered.\n"));

  while (1 && !DISABLE_PACK_ASSIGN)
    {
      change = 0;
      /* look for assignments of the form */
      /* iTempNN = TRueSym (someoperation) SomeOperand */
      /*       ....                       */
      /* TrueSym := iTempNN:1             */
      for (ic = ebp->sch; ic; ic = ic->next)
        {
          /* find assignment of the form TrueSym := iTempNN:1 */
          if (ic->op == '=' && !POINTER_SET (ic))
            change += packRegsForAssign (ic, ebp);
        }
      if (!change)
        break;
    }

  for (ic = ebp->sch; ic; ic = ic->next)
    {
      D (D_ALLOC, ("packRegisters: looping on ic %p\n", ic));

      /* Safe: address of a true sym is always constant. */
      /* if this is an itemp & result of a address of a true sym
         then mark this as rematerialisable   */
      if (ic->op == ADDRESS_OF &&
          IS_ITEMP (IC_RESULT (ic)) && bitVectnBitsOn (OP_DEFS (IC_RESULT (ic))) == 1 && !IS_PARM (IC_RESULT (ic)) /* The receiving of the paramter isnot accounted for in DEFS */ &&
          IS_TRUE_SYMOP (IC_LEFT (ic)) && !OP_SYMBOL (IC_LEFT (ic))->onStack)
        {
          OP_SYMBOL (IC_RESULT (ic))->remat = 1;
          OP_SYMBOL (IC_RESULT (ic))->rematiCode = ic;
          OP_SYMBOL (IC_RESULT (ic))->usl.spillLoc = NULL;
        }

      /* Safe: just propagates the remat flag */
      /* if straight assignment then carry remat flag if this is the
         only definition */
      if (ic->op == '=' && !POINTER_SET (ic) && IS_SYMOP (IC_RIGHT (ic)) && OP_SYMBOL (IC_RIGHT (ic))->remat &&
        !isOperandGlobal (IC_RESULT (ic)) && bitVectnBitsOn (OP_SYMBOL (IC_RESULT (ic))->defs) == 1 && !IS_PARM (IC_RESULT (ic)) && /* The receiving of the paramter isnot accounted for in DEFS */
        !OP_SYMBOL (IC_RESULT (ic))->addrtaken)
        {
          OP_SYMBOL (IC_RESULT (ic))->remat = OP_SYMBOL (IC_RIGHT (ic))->remat;
          OP_SYMBOL (IC_RESULT (ic))->rematiCode = OP_SYMBOL (IC_RIGHT (ic))->rematiCode;
        }

      /* if cast to a generic pointer & the pointer being
         cast is remat, then we can remat this cast as well */
      if (ic->op == CAST &&
          IS_SYMOP (IC_RIGHT (ic)) && OP_SYMBOL (IC_RIGHT (ic))->remat &&
          !isOperandGlobal (IC_RESULT (ic)) && bitVectnBitsOn (OP_DEFS (IC_RESULT (ic))) == 1 && !IS_PARM (IC_RESULT (ic)) && /* The receiving of the paramter isnot accounted for in DEFS */
          !OP_SYMBOL (IC_RESULT (ic))->addrtaken)
        {
          sym_link *to_type = operandType (IC_LEFT (ic));
          sym_link *from_type = operandType (IC_RIGHT (ic));
          if ((IS_PTR (to_type) || IS_INT (to_type)) && IS_PTR (from_type))
            {
              OP_SYMBOL (IC_RESULT (ic))->remat = 1;
              OP_SYMBOL (IC_RESULT (ic))->rematiCode = ic;
              OP_SYMBOL (IC_RESULT (ic))->usl.spillLoc = NULL;
            }
        }

      /* if this is a +/- operation with a rematerizable
         then mark this as rematerializable as well */
      if ((ic->op == '+' || ic->op == '-') &&
          (IS_SYMOP (IC_LEFT (ic)) &&
           IS_ITEMP (IC_RESULT (ic)) &&
           IS_OP_LITERAL (IC_RIGHT (ic))) &&
          OP_SYMBOL (IC_LEFT (ic))->remat &&
          (!IS_SYMOP (IC_RIGHT (ic)) || !IS_CAST_ICODE (OP_SYMBOL (IC_RIGHT (ic))->rematiCode)) &&
          bitVectnBitsOn (OP_DEFS (IC_RESULT (ic))) == 1)
        {
          OP_SYMBOL (IC_RESULT (ic))->remat = 1;
          OP_SYMBOL (IC_RESULT (ic))->rematiCode = ic;
          OP_SYMBOL (IC_RESULT (ic))->usl.spillLoc = NULL;
        }

      /* if the condition of an if instruction is defined in the
         previous instruction then mark the itemp as a conditional */
      if ((IS_CONDITIONAL (ic) ||
           ((ic->op == BITWISEAND ||
             ic->op == '|' ||
             ic->op == '^') &&
            isBitwiseOptimizable (ic))) &&
          ic->next && ic->next->op == IFX &&
          bitVectnBitsOn (OP_USES (IC_RESULT (ic))) == 1 &&
          isOperandEqual (IC_RESULT (ic), IC_COND (ic->next)) && OP_SYMBOL (IC_RESULT (ic))->liveTo <= ic->next->seq)
        {

          OP_SYMBOL (IC_RESULT (ic))->regType = REG_CND;
          continue;
        }

      /* some cases the redundant moves can
         can be eliminated for return statements */
      if (ic->op == RETURN || ic->op == SEND)
        {
          packRegsForOneuse (ic, IC_LEFT (ic), ebp);
        }

      /* if pointer set & left has a size more than
         one and right is not in far space */
      if (!DISABLE_PACK_ONE_USE && POINTER_SET (ic) && IS_SYMOP (IC_RESULT (ic)) &&
          /* MLH: no such thing.
             !isOperandInFarSpace(IC_RIGHT(ic)) && */
          !OP_SYMBOL (IC_RESULT (ic))->remat &&
          !IS_OP_RUONLY (IC_RIGHT (ic)) && getSize (aggrToPtr (operandType (IC_RESULT (ic)), FALSE)) > 1)
        {
          packRegsForOneuse (ic, IC_RESULT (ic), ebp);
        }

      /* if pointer get */
      if (!DISABLE_PACK_ONE_USE && POINTER_GET (ic) && IS_SYMOP (IC_LEFT (ic)) &&
          /* MLH: dont have far space
             !isOperandInFarSpace(IC_RESULT(ic))&& */
          !OP_SYMBOL (IC_LEFT (ic))->remat &&
          !IS_OP_RUONLY (IC_RESULT (ic)) && getSize (aggrToPtr (operandType (IC_LEFT (ic)), FALSE)) > 1)
        {
          packRegsForOneuse (ic, IC_LEFT (ic), ebp);
        }

      /* pack registers for accumulator use, when the result of an
         arithmetic or bit wise operation has only one use, that use is
         immediately following the defintion and the using iCode has
         only one operand or has two operands but one is literal & the
         result of that operation is not on stack then we can leave the
         result of this operation in acc:b combination */

      if (!OPTRALLOC_HL && !DISABLE_PACK_HL && IS_ITEMP (IC_RESULT (ic)))
        if (!IS_GB && !IY_RESERVED)
          packRegsForHLUse3 (ic, IC_RESULT (ic), ebp);

      if (!OPTRALLOC_IY && !DISABLE_PACK_IY && !IY_RESERVED && IS_ITEMP (IC_RESULT (ic)) && !IS_GB)
        packRegsForIYUse (ic, IC_RESULT (ic), ebp);
    }
}

/** Joins together two byte constant pushes into one word push.
 */
static iCode *
joinPushes (iCode * lic)
{
  iCode *ic, *uic, *fic;

  for (ic = lic; ic; ic = ic->next)
    {
      int first, second;
      value *val;
      struct dbuf_s dbuf;

      uic = ic->next;

      /* Anything past this? */
      if (uic == NULL)
        continue;

      /* This and the next pushes? */
      if (ic->op != IPUSH || uic->op != IPUSH)
        continue;

      /* Find call */
      for(fic = uic; fic->op == IPUSH; fic = fic->next);
      if (ic->op != CALL && fic->op != PCALL)
        continue;
      {
        sym_link *dtype = operandType (IC_LEFT (fic));
        sym_link *ftype = IS_FUNCPTR (dtype) ? dtype->next : dtype;
        if (IFFUNC_ISSMALLC (ftype)) /* SmallC calling convention pushes 8-bit values as 16 bit */
          continue;
      }

      /* Both literals? */
      if (!IS_OP_LITERAL (IC_LEFT (ic)) || !IS_OP_LITERAL (IC_LEFT (uic)))
        continue;

      /* Both characters? */
      if (getSize (operandType (IC_LEFT (ic))) != 1 || getSize (operandType (IC_LEFT (uic))) != 1)
        {
          continue;
        }
      /* Pull out the values, make a new type, and create the new iCode for it.
       */
      first = (int) operandLitValue (IC_LEFT (ic));
      second = (int) operandLitValue (IC_LEFT (uic));

      dbuf_init (&dbuf, 128);
      dbuf_printf (&dbuf, "%uu", ((first << 8) | (second & 0xFF)) & 0xFFFFU);
      val = constVal (dbuf_c_str (&dbuf));
      dbuf_destroy (&dbuf);
      SPEC_NOUN (val->type) = V_INT;
      IC_LEFT (ic) = operandFromValue (val);

      /* Now remove the second one from the list. */
      ic->next = uic->next;
      if (uic->next)
        {
          /* Patch up the reverse link */
          uic->next->prev = ic;
        }
    }

  return lic;
}

/** Serially allocate registers to the variables.
    This was the main register allocation function.  It is called after
    packing.
    In the new register allocator it only serves to mark variables for the new register allocator.
 */
static void
serialRegMark (eBBlock ** ebbs, int count)
{
  int i;
  short int max_alloc_bytes = SHRT_MAX; // Byte limit. Set this to a low value to pass only few variables to the register allocator. This can be useful for debugging.

  /* for all blocks */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      if (ebbs[i]->noPath && (ebbs[i]->entryLabel != entryLabel && ebbs[i]->entryLabel != returnLabel))
        continue;

      /* for all instructions do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          /* if this is an ipop that means some live
             range will have to be assigned again */
          if (ic->op == IPOP)
            {
              wassert (0);
              reassignLR (IC_LEFT (ic));
            }

          /* if result is present && is a true symbol */
          if (IC_RESULT (ic) && ic->op != IFX && IS_TRUE_SYMOP (IC_RESULT (ic)))
            {
              OP_SYMBOL (IC_RESULT (ic))->allocreq++;
            }

          /* take away registers from live
             ranges that end at this instruction */
          deassignLRs (ic, ebbs[i]);

          /* some don't need registers */
          if (SKIP_IC2 (ic) ||
              ic->op == JUMPTABLE || ic->op == IFX || ic->op == IPUSH || ic->op == IPOP || (IC_RESULT (ic) && POINTER_SET (ic)))
            {
              continue;
            }

          /* now we need to allocate registers only for the result */
          if (IC_RESULT (ic))
            {
              symbol *sym = OP_SYMBOL (IC_RESULT (ic));

              D (D_ALLOC, ("serialRegAssign: in loop on result %p (%s)\n", sym, sym->name));

              /* Make sure any spill location is definately allocated */
              if (sym->isspilt && !sym->remat && sym->usl.spillLoc && !sym->usl.spillLoc->allocreq)
                {
                  sym->usl.spillLoc->allocreq++;
                }

              /* if it does not need or is spilt
                 or is already assigned to registers (or marked for the new allocator)
                 or will not live beyond this instructions */
              if (!sym->nRegs ||
                  sym->isspilt || bitVectBitValue (_G.regAssigned, sym->key) || sym->for_newralloc || (sym->liveTo <= ic->seq && (sym->nRegs <= 4 || ic->op != CALL && ic->op != PCALL)))
                {
                  D (D_ALLOC, ("serialRegAssign: won't live long enough.\n"));
                  continue;
                }

              /* if some liverange has been spilt at the block level
                 and this one live beyond this block then spil this
                 to be safe */
              if (_G.blockSpil && sym->liveTo > ebbs[i]->lSeq)
                {
                  D (D_ALLOC, ("serialRegAssign: \"spilling to be safe.\"\n"));
                  sym->for_newralloc = 0;
                  z80SpillThis (sym);
                  continue;
                }

              if (sym->usl.spillLoc && !sym->usl.spillLoc->_isparm && !USE_OLDSALLOC) // I have no idea where these spill locations come from. Sometime two symbols even have the same spill location, whic tends to mess up stack allocation. Those that come from previous iterations in this loop would be okay, but those from outside are a problem.
                {
                  sym->usl.spillLoc = 0;
                  sym->isspilt = false;
                }

              if (sym->nRegs > 4) /* TODO. Change this once we can allocate bigger variables (but still spill when its a big return value). Also change in ralloc2.cc, operand_on-stack in that case*/
                {
                  D (D_ALLOC, ("Spilling %s (too large)\n", sym->name));
                  sym->for_newralloc = 0;
                  z80SpillThis (sym);
                }
              else if (max_alloc_bytes >= sym->nRegs)
                {
                  sym->for_newralloc = 1;
                  max_alloc_bytes -= sym->nRegs;
                }
              else if (!sym->for_newralloc)
                {
                  z80SpillThis (sym);
                  printf ("Spilt %s due to byte limit.\n", sym->name);
                }
            }
        }
    }
}

void
Z80RegFix (eBBlock ** ebbs, int count)
{
  int i;

  /* Check for and fix any problems with uninitialized operands */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      if (ebbs[i]->noPath && (ebbs[i]->entryLabel != entryLabel && ebbs[i]->entryLabel != returnLabel))
        continue;

      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          deassignLRs (ic, ebbs[i]);

          if (SKIP_IC2 (ic))
            continue;

          if (ic->op == IFX)
            {
              verifyRegsAssigned (IC_COND (ic), ic);
              continue;
            }

          if (ic->op == JUMPTABLE)
            {
              verifyRegsAssigned (IC_JTCOND (ic), ic);
              continue;
            }

          verifyRegsAssigned (IC_RESULT (ic), ic);
          verifyRegsAssigned (IC_LEFT (ic), ic);
          verifyRegsAssigned (IC_RIGHT (ic), ic);
        }
    }
}

void z80_init_asmops (void);

/*-----------------------------------------------------------------*/
/* Register allocator                                          */
/*-----------------------------------------------------------------*/
void
z80_ralloc (ebbIndex *ebbi)
{
  eBBlock **ebbs = ebbi->bbOrder;
  int count = ebbi->count;
  iCode *ic;
  int i;

  D (D_ALLOC, ("\n-> z80_ralloc: entered for %s.\n", currFunc ? currFunc->name : "[no function]"));

  setToNull ((void *) &_G.funcrUsed);
  setToNull ((void *) &_G.totRegAssigned);
  _G.stackExtend = _G.dataExtend = 0;

  if (IS_GB)
    {
      _G.nRegs = GBZ80_MAX_REGS;
      regsZ80 = _gbz80_regs;
    }
  else
    {
      _G.nRegs = Z80_MAX_REGS;
      regsZ80 = _z80_regs;
    }

  z80_init_asmops ();

  /* change assignments this will remove some
     live ranges reducing some register pressure */
  for (i = 0; i < count; i++)
    packRegisters (ebbs[i]);

  /* liveranges probably changed by register packing
     so we compute them again */
  recomputeLiveRanges (ebbs, count, FALSE);

  if (options.dump_i_code)
    dumpEbbsToFileExt (DUMP_PACK, ebbi);

  /* first determine for each live range the number of
     registers & the type of registers required for each */
  regTypeNum ();

  /* Mark variables for assignment by the new allocator */
  serialRegMark (ebbs, count);

  joinPushes (iCodeLabelOptimize(iCodeFromeBBlock (ebbs, count)));

  /* The new register allocator invokes its magic */
  ic = z80_ralloc2_cc (ebbi);

  if (options.dump_i_code)
    {
      dumpEbbsToFileExt (DUMP_RASSGN, ebbi);
      dumpLiveRanges (DUMP_LRANGE, liveRanges);
    }

  genZ80Code (ic);

  /* free up any stackSpil locations allocated */
  applyToSet (_G.stackSpil, deallocStackSpil);
  _G.slocNum = 0;
  setToNull ((void *) &_G.stackSpil);
  setToNull ((void *) &_G.spiltSet);
  /* mark all registers as free */
  freeAllRegs ();

  return;
}

/*-----------------------------------------------------------------*/
/* assignRegisters - assigns registers to each live range as need  */
/*-----------------------------------------------------------------*/
void
z80_assignRegisters (ebbIndex * ebbi)
{
  z80_ralloc (ebbi);
}

