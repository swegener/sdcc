/*-------------------------------------------------------------------------
  gen.c - source file for code generation for DS80C390

  Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
         and -  Jean-Louis VERN.jlvern@writeme.com (1999)
  Bug Fixes  -  Wojciech Stryjewski  wstryj1@tiger.lsu.edu (1999 v2.1.9a)
  DS390 adaptation by Kevin Vigor <kevin@vigor.nu>

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "ralloc.h"
#include "gen.h"
#include "SDCCglobl.h"
#include "newalloc.h"

#ifdef HAVE_SYS_ISA_DEFS_H
#include <sys/isa_defs.h>
#else
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#else
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#else
#if !defined(__BORLANDC__) && !defined(_MSC_VER) && !defined(__MINGW32__) && !defined(__CYGWIN__)
#warning "Cannot determine ENDIANESS of this machine assuming LITTLE_ENDIAN"
#warning "If you running sdcc on an INTEL 80x86 Platform you are okay"
#endif
#endif
#endif
#endif

#define BETTER_LITERAL_SHIFT

char *aopLiteral (value * val, int offset);
extern int allocInfo;

/* this is the down and dirty file with all kinds of
   kludgy & hacky stuff. This is what it is all about
   CODE GENERATION for a specific MCU . some of the
   routines may be reusable, will have to see */

static char *zero = "#0";
static char *one = "#1";
static char *spname;

#define D(x) x

#define TR_DPTR(s) if (options.model != MODEL_FLAT24) { emitcode(";", " Use_DPTR1 %s ", s); }
#define TR_AP(s) if (options.model != MODEL_FLAT24) { emitcode(";", " Use_AP %s ", s); }

unsigned fReturnSizeDS390 = 5;	/* shared with ralloc.c */
static char *fReturn24[] =
{"dpl", "dph", "dpx", "b", "a"};
static char *fReturn16[] =
{"dpl", "dph", "b", "a"};
static char **fReturn = fReturn24;
static char *accUse[] =
{"a", "b"};
static char *dptrn[2][3];
static char *javaRet[] = { "r0","r1","r2","r3"};
static short rbank = -1;

static struct
  {
    short r0Pushed;
    short r1Pushed;
    short accInUse;
    short bInUse;
    short inLine;
    short debugLine;
    short nRegsSaved;
    short dptrInUse;
    short dptr1InUse;
    set *sendSet;
  }
_G;

static char *rb1regs[] = {
    "b1_0","b1_1","b1_2","b1_3","b1_4","b1_5","b1_6","b1_7"
};

static void saveRBank (int, iCode *, bool);

#define RESULTONSTACK(x) \
                         (IC_RESULT(x) && IC_RESULT(x)->aop && \
                         IC_RESULT(x)->aop->type == AOP_STK )

/* #define MOVA(x) if (strcmp(x,"a") && strcmp(x,"acc")) emitcode("mov","a,%s",x); */
#define MOVA(x) { char *_mova_tmp = strdup(x); \
                 if (strcmp(_mova_tmp,"a") && strcmp(_mova_tmp,"acc")) \
                 { \
                    emitcode("mov","a,%s",_mova_tmp); \
                 } \
                 free(_mova_tmp); \
                }
#define MOVB(x) { char *_movb_tmp = strdup(x); \
                 if (strcmp(_movb_tmp,"b")) \
                 { \
                    emitcode("mov","b,%s",_movb_tmp); \
                 } \
                 free(_movb_tmp); \
                }
#define CLRC    emitcode("clr","c")
#define SETC    emitcode("setb","c")

// A scratch register which will be used to hold
// result bytes from operands in far space via DPTR2.
#define DP2_RESULT_REG	"_ap"

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
#define PROTECT_SP 	{if (options.protect_sp_update) { 			\
				symbol *lbl = newiTempLabel(NULL);		\
				emitcode ("setb","F1");				\
				emitcode ("jbc","EA,!tlabel",lbl->key+100);	\
				emitcode ("clr","F1"); 				\
				emitcode ("","!tlabeldef",lbl->key+100);	\
    			}}
#define UNPROTECT_SP 	{ if (options.protect_sp_update) { 			\
    				symbol *lbl = newiTempLabel(NULL);		\
				emitcode ("jnb","F1,!tlabel",lbl->key+100);	\
    				emitcode ("setb","EA");				\
				emitcode ("","!tlabeldef",lbl->key+100);	\
    			}}


/*-----------------------------------------------------------------*/
/* emitcode - writes the code into a file : for now it is simple    */
/*-----------------------------------------------------------------*/
static void
emitcode (char *inst, char *fmt,...)
{
    va_list ap;
    char lb[INITIAL_INLINEASM];
    char *lbp = lb;
    
    va_start (ap, fmt);
    
    if (inst && *inst)
    {
	if (fmt && *fmt)
	{
	    SNPRINTF (lb, sizeof(lb), "%s\t", inst);
	}
	else
	{
	    SNPRINTF (lb, sizeof(lb), "%s", inst);
	}
	
	tvsprintf (lb + strlen(lb), sizeof(lb) - strlen(lb), 
		   fmt, ap);
    }
    else
    {
	tvsprintf (lb, sizeof(lb), fmt, ap);
    }
    

    while (isspace (*lbp))
    {
	lbp++;
    }

    if (lbp && *lbp)
    {
	lineCurr = (lineCurr ?
		    connectLine (lineCurr, newLineNode (lb)) :
		    (lineHead = newLineNode (lb)));
    }
    
    lineCurr->isInline = _G.inLine;
    lineCurr->isDebug = _G.debugLine;
    va_end (ap);
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

      return (*aopp)->aopu.aop_ptr = ds390_regWithIdx (R0_IDX);
    }

  /* if no usage of r1 then return it */
  if (!r1iu && !r1ou)
    {
      ic->rUsed = bitVectSetBit (ic->rUsed, R1_IDX);
      (*aopp)->type = AOP_R1;

      return (*aopp)->aopu.aop_ptr = ds390_regWithIdx (R1_IDX);
    }

  /* now we know they both have usage */
  /* if r0 not used in this instruction */
  if (!r0iu)
    {
      /* push it if not already pushed */
      if (!_G.r0Pushed)
	{
	  emitcode ("push", "%s",
		    ds390_regWithIdx (R0_IDX)->dname);
	  _G.r0Pushed++;
	}

      ic->rUsed = bitVectSetBit (ic->rUsed, R0_IDX);
      (*aopp)->type = AOP_R0;

      return (*aopp)->aopu.aop_ptr = ds390_regWithIdx (R0_IDX);
    }

  /* if r1 not used then */

  if (!r1iu)
    {
      /* push it if not already pushed */
      if (!_G.r1Pushed)
	{
	  emitcode ("push", "%s",
		    ds390_regWithIdx (R1_IDX)->dname);
	  _G.r1Pushed++;
	}

      ic->rUsed = bitVectSetBit (ic->rUsed, R1_IDX);
      (*aopp)->type = AOP_R1;
      return ds390_regWithIdx (R1_IDX);
    }

endOfWorld:
  /* I said end of world but not quite end of world yet */
  /* if this is a result then we can push it on the stack */
  if (result)
    {
      (*aopp)->type = AOP_STK;
      return NULL;
    }

  /* other wise this is true end of the world */
  werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
	  "getFreePtr should never reach here");
  exit (1);
    
  return NULL; // notreached, but makes compiler happy.
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
  return aop;
}

static int _currentDPS;		/* Current processor DPS. */
static int _desiredDPS;		/* DPS value compiler thinks we should be using. */
static int _lazyDPS = 0;	/* if non-zero, we are doing lazy evaluation of DPS changes. */

/*-----------------------------------------------------------------*/
/* genSetDPTR: generate code to select which DPTR is in use (zero  */
/* selects standard DPTR (DPL/DPH/DPX), non-zero selects DS390     */
/* alternate DPTR (DPL1/DPH1/DPX1).          */
/*-----------------------------------------------------------------*/
static void
genSetDPTR (int n)
{

  /* If we are doing lazy evaluation, simply note the desired
   * change, but don't emit any code yet.
   */
  if (_lazyDPS)
    {
      _desiredDPS = n;
      return;
    }

  if (!n)
    {
      emitcode ("mov", "dps,#0");
    }
  else
    {
      TR_DPTR("#1");
      emitcode ("mov", "dps,#1");
    }
}

/*-----------------------------------------------------------------*/
/* _startLazyDPSEvaluation: call to start doing lazy DPS evaluation */
/*                   */
/* Any code that operates on DPTR (NB: not on the individual     */
/* components, like DPH) *must* call _flushLazyDPS() before using  */
/* DPTR within a lazy DPS evaluation block.        */
/*                   */
/* Note that aopPut and aopGet already contain the proper calls to */
/* _flushLazyDPS, so it is safe to use these calls within a lazy   */
/* DPS evaluation block.             */
/*                   */
/* Also, _flushLazyDPS must be called before any flow control      */
/* operations that could potentially branch out of the block.    */
/*                         */
/* Lazy DPS evaluation is simply an optimization (though an      */
/* important one), so if in doubt, leave it out.       */
/*-----------------------------------------------------------------*/
static void
_startLazyDPSEvaluation (void)
{
  _currentDPS = 0;
  _desiredDPS = 0;
#ifdef BETTER_LITERAL_SHIFT  
  _lazyDPS++;
#else
  _lazyDPS = 1;
#endif  
}

/*-----------------------------------------------------------------*/
/* _flushLazyDPS: emit code to force the actual DPS setting to the */
/* desired one. Call before using DPTR within a lazy DPS evaluation */
/* block.                */
/*-----------------------------------------------------------------*/
static void
_flushLazyDPS (void)
{
  if (!_lazyDPS)
    {
      /* nothing to do. */
      return;
    }

  if (_desiredDPS != _currentDPS)
    {
      if (_desiredDPS)
	{
	  emitcode ("inc", "dps");
	}
      else
	{
	  emitcode ("dec", "dps");
	}
      _currentDPS = _desiredDPS;
    }
}

/*-----------------------------------------------------------------*/
/* _endLazyDPSEvaluation: end lazy DPS evaluation block.     */
/*                   */
/* Forces us back to the safe state (standard DPTR selected).    */
/*-----------------------------------------------------------------*/
static void
_endLazyDPSEvaluation (void)
{
#ifdef BETTER_LITERAL_SHIFT  
  _lazyDPS--;
#else
  _lazyDPS = 0;
#endif    
  if (!_lazyDPS)
  {
    if (_currentDPS)
    {
      genSetDPTR (0);
      _flushLazyDPS ();
    }
    _currentDPS = 0;
    _desiredDPS = 0;
  }
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
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *
aopForSym (iCode * ic, symbol * sym, bool result, bool useDP2)
{
  asmop *aop;
  memmap *space = SPEC_OCLS (sym->etype);

  /* if already has one */
  if (sym->aop)
    return sym->aop;

  /* assign depending on the storage class */
  /* if it is on the stack or indirectly addressable */
  /* space we need to assign either r0 or r1 to it   */
  if ((sym->onStack && !options.stack10bit) || sym->iaccess)
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
	      if (_G.accInUse)
		emitcode ("push", "acc");

	      if (_G.bInUse)
		emitcode ("push", "b");

	      emitcode ("mov", "a,_bp");
	      emitcode ("add", "a,#!constbyte",
			((sym->stack < 0) ?
			 ((char) (sym->stack - _G.nRegsSaved)) :
			 ((char) sym->stack)) & 0xff);
	      emitcode ("mov", "%s,a",
			aop->aopu.aop_ptr->name);

	      if (_G.bInUse)
		emitcode ("pop", "b");

	      if (_G.accInUse)
		emitcode ("pop", "acc");
	    }
	  else
	    emitcode ("mov", "%s,#%s",
		      aop->aopu.aop_ptr->name,
		      sym->rname);
	  aop->paged = space->paged;
	}
      else
	aop->aopu.aop_stk = sym->stack;
      return aop;
    }

  if (sym->onStack && options.stack10bit)
    {
	short stack_val = -((sym->stack < 0) ?
			    ((short) (sym->stack - _G.nRegsSaved)) :
			    ((short) sym->stack)) ;
	if (useDP2 && _G.dptr1InUse) {
	    emitcode ("push","dpl1");
	    emitcode ("push","dph1");
	    emitcode ("push","dpx1");
	} else if (_G.dptrInUse	) {
	    emitcode ("push","dpl");
	    emitcode ("push","dph");
	    emitcode ("push","dpx");
	}
      /* It's on the 10 bit stack, which is located in
       * far data space.
       */	    
	if (stack_val < 0 && stack_val > -5) { /* between -5 & -1 */
	    if (useDP2) {
		if (options.model == MODEL_FLAT24)
		    emitcode ("mov", "dpx1,#!constbyte", (options.stack_loc >> 16) & 0xff);
		TR_DPTR("#2");
		emitcode ("mov", "dph1,_bpx+1");
		emitcode ("mov", "dpl1,_bpx");
		emitcode ("mov","dps,#1");
	    } else {
		if (options.model == MODEL_FLAT24)
		    emitcode ("mov", "dpx,#!constbyte", (options.stack_loc >> 16) & 0xff);
		emitcode ("mov", "dph,_bpx+1");
		emitcode ("mov", "dpl,_bpx");
	    }
	    stack_val = -stack_val;
	    while (stack_val--) {
		emitcode ("inc","dptr");
	    }
	    if (useDP2) {
		emitcode("mov","dps,#0");
	    }
	}  else {
	    if (_G.accInUse)
		emitcode ("push", "acc");
	    
	    if (_G.bInUse)
		emitcode ("push", "b");
	
	    emitcode ("mov", "a,_bpx");
	    emitcode ("clr","c");
	    emitcode ("subb", "a,#!constbyte", stack_val & 0xff);
	    emitcode ("mov","b,a");
	    emitcode ("mov","a,_bpx+1");
	    emitcode ("subb","a,#!constbyte",(stack_val >> 8) & 0xff);
	    if (useDP2) {
		if (options.model == MODEL_FLAT24)
		    emitcode ("mov", "dpx1,#!constbyte", (options.stack_loc >> 16) & 0xff);
		TR_DPTR("#2");
		emitcode ("mov", "dph1,a");
		emitcode ("mov", "dpl1,b");
	    } else {
		if (options.model == MODEL_FLAT24)
		    emitcode ("mov", "dpx,#!constbyte", (options.stack_loc >> 16) & 0xff);
		emitcode ("mov", "dph,a");
		emitcode ("mov", "dpl,b");
	    }
	    
	    if (_G.bInUse)
		emitcode ("pop", "b");
	    
	    if (_G.accInUse)
		emitcode ("pop", "acc");
	}
	sym->aop = aop = newAsmop ((short) (useDP2 ? AOP_DPTR2 : AOP_DPTR));
	aop->size = getSize (sym->type);
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
      aop->size = FPTRSIZE;
      return aop;
    }
  
  /* only remaining is far space */
  /* in which case DPTR gets the address */
  sym->aop = aop = newAsmop ((short) (useDP2 ? AOP_DPTR2 : AOP_DPTR));
  if (useDP2)
    {
      genSetDPTR (1);
      _flushLazyDPS ();
      emitcode ("mov", "dptr,#%s", sym->rname);
      genSetDPTR (0);
    }
  else
    {
      emitcode ("mov", "dptr,#%s", sym->rname);
    }
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
  int ptr_type =0;
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
	      ptr_type = DCL_TYPE(from_type);
	      if (ptr_type == IPOINTER) {
		// bug #481053
		ptr_type = POINTER;
	      }
	      continue ;
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
      if (IS_ASSIGN_ICODE(ic) && isOperandLiteral(IC_RIGHT(ic)))
      {
	  SNPRINTF(buffer, sizeof(buffer), 
		   "0x%x",(int) operandLitValue (IC_RIGHT (ic)));
      }
      else
      {
	  strncpyz (buffer, OP_SYMBOL (IC_LEFT (ic))->rname, sizeof(buffer));
      }
  }

  aop->aopu.aop_immd.aop_immd1 = Safe_strdup(buffer);  
  /* set immd2 field if required */
  if (aop->aopu.aop_immd.from_cast_remat) 
  {
      tsprintf(buffer, sizeof(buffer), "#!constbyte",ptr_type);
      aop->aopu.aop_immd.aop_immd2 = Safe_strdup(buffer);
  }

  return aop;
}

/*-----------------------------------------------------------------*/
/* aopHasRegs - returns true if aop has regs between from-to       */
/*-----------------------------------------------------------------*/
static int aopHasRegs(asmop *aop, int from, int to)
{
    int size =0;

    if (aop->type != AOP_REG) return 0; /* if not assigned to regs */

    for (; size < aop->size ; size++) {
	int reg;
	for (reg = from ; reg <= to ; reg++)
	    if (aop->aopu.aop_reg[size] == ds390_regWithIdx(reg)) return 1;
    }
    return 0;
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

  /* if they not symbols */
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

  if (strcmp (sym1->rname, sym2->rname) == 0)
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
/* sameRegs - two asmops have the same registers                   */
/*-----------------------------------------------------------------*/
static bool
sameRegs (asmop * aop1, asmop * aop2)
{
  int i;

  if (aop1 == aop2)
    {
      if (aop1->type == AOP_DPTR || aop1->type == AOP_DPTR2)
	{
	  return FALSE;
	}
      return TRUE;
    }

  if (aop1->type != AOP_REG ||
      aop2->type != AOP_REG)
    return FALSE;

  if (aop1->size != aop2->size)
    return FALSE;

  for (i = 0; i < aop1->size; i++)
    if (aop1->aopu.aop_reg[i] !=
	aop2->aopu.aop_reg[i])
      return FALSE;

  return TRUE;
}

/*-----------------------------------------------------------------*/
/* aopOp - allocates an asmop for an operand  :                    */
/*-----------------------------------------------------------------*/
static void
aopOp (operand * op, iCode * ic, bool result, bool useDP2)
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
    return;

  /* if the underlying symbol has a aop */
  if (IS_SYMOP (op) && OP_SYMBOL (op)->aop)
    {
      op->aop = OP_SYMBOL (op)->aop;
      return;
    }

  /* if this is a true symbol */
  if (IS_TRUE_SYMOP (op))
    {
      op->aop = aopForSym (ic, OP_SYMBOL (op), result, useDP2);
      return;
    }

  /* this is a temporary : this has
     only four choices :
     a) register
     b) spillocation
     c) rematerialize
     d) conditional
     e) can be a return use only */

  sym = OP_SYMBOL (op);


  /* if the type is a conditional */
  if (sym->regType == REG_CND)
    {
      aop = op->aop = sym->aop = newAsmop (AOP_CRY);
      aop->size = 0;
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
	  sym->aop = op->aop = aop =
	    aopForRemat (sym);
	  aop->size = getSize (sym->type);
	  return;
	}

      if (sym->accuse)
	{
	  int i;
	  aop = op->aop = sym->aop = newAsmop (AOP_ACC);
	  aop->size = getSize (sym->type);
	  for (i = 0; i < 2; i++)
	    aop->aopu.aop_str[i] = accUse[i];
	  return;
	}

      if (sym->ruonly)
	{
	  int i;

	  if (useDP2)
	    {
	      /* a AOP_STR uses DPTR, but DPTR is already in use;
	       * we're just hosed.
	       */
		werror(E_INTERNAL_ERROR,__FILE__,__LINE__,"AOP_STR with DPTR in use!");
	    }

	  aop = op->aop = sym->aop = newAsmop (AOP_STR);
	  aop->size = getSize (sym->type);
	  for (i = 0; i < (int) fReturnSizeDS390; i++)
	    aop->aopu.aop_str[i] = fReturn[i];
	  return;
	}
      
      if (sym->dptr) { /* has been allocated to a DPTRn */
	  aop = op->aop = sym->aop = newAsmop (AOP_DPTRn);
	  aop->size = getSize (sym->type);
	  aop->aopu.dptr = sym->dptr;
	  return ;
      }
      /* else spill location  */
      if (sym->usl.spillLoc && getSize(sym->type) != getSize(sym->usl.spillLoc->type)) {
	  /* force a new aop if sizes differ */
	  sym->usl.spillLoc->aop = NULL;
      }
      sym->aop = op->aop = aop =
	aopForSym (ic, sym->usl.spillLoc, result, useDP2);
      aop->size = getSize (sym->type);
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

  if (aop->freed)
    goto dealloc;

  aop->freed = 1;

  /* depending on the asmop type only three cases need work AOP_RO
     , AOP_R1 && AOP_STK */
  switch (aop->type)
    {
    case AOP_R0:
      if (_G.r0Pushed)
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
      if (_G.r1Pushed)
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
	int stk = aop->aopu.aop_stk + aop->size;
	bitVectUnSetBit (ic->rUsed, R0_IDX);
	bitVectUnSetBit (ic->rUsed, R1_IDX);

	getFreePtr (ic, &aop, FALSE);

	if (options.stack10bit)
	  {
	    /* I'm not sure what to do here yet... */
	    /* #STUB */
	    fprintf (stderr,
		     "*** Warning: probably generating bad code for "
		     "10 bit stack mode.\n");
	  }

	if (stk)
	  {
	    emitcode ("mov", "a,_bp");
	    emitcode ("add", "a,#!constbyte", ((char) stk) & 0xff);
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
	if (_G.r0Pushed)
	  {
	    emitcode ("pop", "ar0");
	    _G.r0Pushed--;
	  }

	if (_G.r1Pushed)
	  {
	    emitcode ("pop", "ar1");
	    _G.r1Pushed--;
	  }
      }
    case AOP_DPTR2:
	if (_G.dptr1InUse) {
	    emitcode ("pop","dpx1");
	    emitcode ("pop","dph1");
	    emitcode ("pop","dpl1");
	}
	break;
    case AOP_DPTR:
	if (_G.dptrInUse) {
	    emitcode ("pop","dpx");
	    emitcode ("pop","dph");
	    emitcode ("pop","dpl");
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

/*-------------------------------------------------------------------*/
/* aopGet - for fetching value of the aop                            */
/*                    						     */
/* Set saveAcc to NULL if you are sure it is OK to clobber the value */
/* in the accumulator. Set it to the name of a free register         */
/* if acc must be preserved; the register will be used to preserve   */
/* acc temporarily and to return the result byte.		     */
/*-------------------------------------------------------------------*/

static char *
aopGet (asmop *aop,
	int   offset,
	bool  bit16,
	bool  dname,
	char  *saveAcc)
{
  /* offset is greater than
     size then zero */
  if (offset > (aop->size - 1) &&
      aop->type != AOP_LIT)
    return zero;

  /* depending on type */
  switch (aop->type)
    {

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

    case AOP_DPTRn:
	assert(offset <= 3);
	return dptrn[aop->aopu.dptr][offset];

    case AOP_DPTR:
    case AOP_DPTR2:

      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (1);
	  if (saveAcc)
	    {
		    TR_AP("#1");
		    emitcode ("xch", "a, %s", saveAcc);
	    }
	}

      _flushLazyDPS ();

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

      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (0);
	  if (saveAcc)
	    {
       TR_AP("#2");
	      emitcode ("xch", "a, %s", saveAcc);
	      return saveAcc;
	    }
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
	  switch (offset) {
	  case 1:
	      tsprintf(buffer, sizeof(buffer),
		       "#!his",aop->aopu.aop_immd.aop_immd1);
	      break;
	  case 2:
	      tsprintf(buffer, sizeof(buffer), 
		       "#!hihis",aop->aopu.aop_immd.aop_immd1);
	      break;
	  case 3:
	      tsprintf(buffer, sizeof(buffer),
		       "#!hihihis",aop->aopu.aop_immd.aop_immd1);
	      break;
	  default: /* should not need this (just in case) */
	      SNPRINTF (buffer, sizeof(buffer), 
			"#(%s >> %d)",
		       aop->aopu.aop_immd.aop_immd1,
		       offset * 8);
	  }
      }
      else
      {
	SNPRINTF (buffer, sizeof(buffer), 
		  "#%s", aop->aopu.aop_immd.aop_immd1);
      }
      return Safe_strdup(buffer);	

    case AOP_DIR:
      if (offset)
      {
	SNPRINTF (buffer, sizeof(buffer),
		  "(%s + %d)",
		 aop->aopu.aop_dir,
		 offset);
      }
      else
      {
	SNPRINTF(buffer, sizeof(buffer), 
		 "%s", aop->aopu.aop_dir);
      }

      return Safe_strdup(buffer);

    case AOP_REG:
      if (dname)
	return aop->aopu.aop_reg[offset]->dname;
      else
	return aop->aopu.aop_reg[offset]->name;

    case AOP_CRY:
      emitcode ("clr", "a");
      emitcode ("mov", "c,%s", aop->aopu.aop_dir);
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
    
  return NULL;  // not reached, but makes compiler happy.
}
/*-----------------------------------------------------------------*/
/* aopPut - puts a string for a aop                                */
/*-----------------------------------------------------------------*/
static void
aopPut (asmop * aop, char *s, int offset)
{
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
    case AOP_DIR:
        if (offset)
	{
	    SNPRINTF (buffer, sizeof(buffer),
		      "(%s + %d)",
		      aop->aopu.aop_dir, offset);
	}
	else
	{
	    SNPRINTF (buffer, sizeof(buffer), 
		     "%s", aop->aopu.aop_dir);
	}
	

	if (strcmp (buffer, s))
	{
	    emitcode ("mov", "%s,%s", buffer, s);
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

    case AOP_DPTRn:
	emitcode ("mov","%s,%s",dptrn[aop->aopu.dptr][offset],s);
	break;

    case AOP_DPTR:
    case AOP_DPTR2:

      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (1);
	}
      _flushLazyDPS ();

      if (aop->code)
	{
	  werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
		  "aopPut writting to code space");
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

      /* if not in accumulater */
      MOVA (s);

      emitcode ("movx", "@dptr,a");

      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (0);
	}
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
	  char buff[10];
	  SNPRINTF(buff, sizeof(buff), 
		   "a%s", s);
	  emitcode ("mov", "@%s,%s",
		    aop->aopu.aop_ptr->name, buff);
	}
	else
	{
	    emitcode ("mov", "@%s,%s", aop->aopu.aop_ptr->name, s);
	}
      break;

    case AOP_STK:
      if (strcmp (s, "a") == 0)
	emitcode ("push", "acc");
      else
	if (*s=='@') {
	  MOVA(s);
	  emitcode ("push", "acc");
	} else {
	  emitcode ("push", s);
	}

      break;

    case AOP_CRY:
      /* if bit variable */
      if (!aop->aopu.aop_dir)
	{
	  emitcode ("clr", "a");
	  emitcode ("rlc", "a");
	}
      else
	{
	  if (s == zero)
	    emitcode ("clr", "%s", aop->aopu.aop_dir);
	  else if (s == one)
	    emitcode ("setb", "%s", aop->aopu.aop_dir);
	  else if (!strcmp (s, "c"))
	    emitcode ("mov", "%s,c", aop->aopu.aop_dir);
	  else
	    {
	      if (strcmp (s, "a"))
		{
		  MOVA (s);
		}
	      {
		/* set C, if a >= 1 */
		emitcode ("add", "a,#!constbyte",0xff);
		emitcode ("mov", "%s,c", aop->aopu.aop_dir);
	      }
	    }
	}
      break;

    case AOP_STR:
      aop->coff = offset;
      if (strcmp (aop->aopu.aop_str[offset], s))
	emitcode ("mov", "%s,%s", aop->aopu.aop_str[offset], s);
      break;

    case AOP_ACC:
      aop->coff = offset;
      if (!offset && (strcmp (s, "acc") == 0))
	break;

      if (strcmp (aop->aopu.aop_str[offset], s))
	emitcode ("mov", "%s,%s", aop->aopu.aop_str[offset], s);
      break;

    default:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
	      "aopPut got unsupported aop->type");
      exit (1);
    }

}


/*--------------------------------------------------------------------*/
/* reAdjustPreg - points a register back to where it should (coff==0) */
/*--------------------------------------------------------------------*/
static void
reAdjustPreg (asmop * aop)
{
  if ((aop->coff==0) || (aop->size <= 1)) {
    return;
  }

  switch (aop->type)
    {
    case AOP_R0:
    case AOP_R1:
      while (aop->coff--)
	emitcode ("dec", "%s", aop->aopu.aop_ptr->name);
      break;
    case AOP_DPTR:
    case AOP_DPTR2:
      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (1);
	  _flushLazyDPS ();
	}
      while (aop->coff--)
	{
	  emitcode ("lcall", "__decdptr");
	}

      if (aop->type == AOP_DPTR2)
	{
	  genSetDPTR (0);
	}
      break;

    }
  aop->coff=0;
}

#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define IS_AOP_PREG(x) (AOP(x) && (AOP_TYPE(x) == AOP_R1 || \
                       AOP_TYPE(x) == AOP_R0))

#define AOP_NEEDSACC(x) (AOP(x) && (AOP_TYPE(x) == AOP_CRY ||  \
                        AOP_TYPE(x) == AOP_DPTR || AOP_TYPE(x) == AOP_DPTR2 || \
                         AOP(x)->paged))

#define AOP_INPREG(x) (x && (x->type == AOP_REG &&                        \
                      (x->aopu.aop_reg[0] == ds390_regWithIdx(R0_IDX) || \
                      x->aopu.aop_reg[0] == ds390_regWithIdx(R1_IDX) )))
#define AOP_INDPTRn(x) (AOP_TYPE(x) == AOP_DPTRn)
#define AOP_USESDPTR(x) ((AOP_TYPE(x) == AOP_DPTR) || (AOP_TYPE(x) == AOP_STR))
#define AOP_USESDPTR2(x) ((AOP_TYPE(x) == AOP_DPTR2) || (AOP_TYPE(x) == AOP_DPTRn))

// The following two macros can be used even if the aop has not yet been aopOp'd.
#define AOP_IS_STR(x) (IS_SYMOP(x) && OP_SYMBOL(x)->ruonly)
#define AOP_IS_DPTRn(x) (IS_SYMOP(x) && OP_SYMBOL(x)->dptr)

/* Workaround for DS80C390 bug: div ab may return bogus results
 * if A is accessed in instruction immediately before the div.
 *
 * Will be fixed in B4 rev of processor, Dallas claims.
 */

#define LOAD_AB_FOR_DIV(LEFT, RIGHT, L)       \
    if (!AOP_NEEDSACC(RIGHT))         \
    {               \
      /* We can load A first, then B, since     \
       * B (the RIGHT operand) won't clobber A,   \
       * thus avoiding touching A right before the div. \
       */             \
      D(emitcode(";", "DS80C390 div bug: rearranged ops.");); \
      L = aopGet(AOP(LEFT),0,FALSE,FALSE,NULL);     \
      MOVA(L);            \
      L = aopGet(AOP(RIGHT),0,FALSE,FALSE,"b"); \
      MOVB(L); \
    }               \
    else              \
    {               \
      /* Just stuff in a nop after loading A. */    \
      emitcode("mov","b,%s",aopGet(AOP(RIGHT),0,FALSE,FALSE,NULL));\
      L = aopGet(AOP(LEFT),0,FALSE,FALSE,NULL);   \
      MOVA(L);            \
      emitcode("nop", "; workaround for DS80C390 div bug.");  \
    }

/*-----------------------------------------------------------------*/
/* genNotFloat - generates not for float operations              */
/*-----------------------------------------------------------------*/
static void
genNotFloat (operand * op, operand * res)
{
  int size, offset;
  char *l;
  symbol *tlbl;

  D (emitcode (";", "genNotFloat "););

  /* we will put 127 in the first byte of
     the result */
  aopPut (AOP (res), "#127", 0);
  size = AOP_SIZE (op) - 1;
  offset = 1;

  _startLazyDPSEvaluation ();
  l = aopGet (op->aop, offset++, FALSE, FALSE, NULL);
  MOVA (l);

  while (size--)
    {
      emitcode ("orl", "a,%s",
		aopGet (op->aop,
			offset++, FALSE, FALSE,
			DP2_RESULT_REG));
    }
  _endLazyDPSEvaluation ();

  tlbl = newiTempLabel (NULL);
  aopPut (res->aop, one, 1);
  emitcode ("jz", "!tlabel", (tlbl->key + 100));
  aopPut (res->aop, zero, 1);
  emitcode ("", "!tlabeldef", (tlbl->key + 100));

  size = res->aop->size - 2;
  offset = 2;
  /* put zeros in the rest */
  while (size--)
    aopPut (res->aop, zero, offset++);
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
      aopPut (AOP (result), "a", 0);
      size--;
      offset = 1;
      /* unsigned or positive */
      while (size--)
	{
	  aopPut (AOP (result), zero, offset++);
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
      aopPut (AOP (result), "c", 0);
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
  int 	size = AOP_SIZE (oper) - 1;
  int 	offset = 1;
  bool usedB = FALSE;

  /* The generic part of a generic pointer should
   * not participate in it's truth value.
   *
   * i.e. 0x10000000 is zero.
   */
  if (opIsGptr (oper))
    {
      D (emitcode (";", "toBoolean: generic ptr special case."););
      size--;
    }

  _startLazyDPSEvaluation ();
  if (AOP_NEEDSACC (oper) && size)
    {
      usedB = TRUE;
      emitcode ("push", "b");
      MOVB (aopGet (AOP (oper), 0, FALSE, FALSE, "b"));
    }
  else
    {
      MOVA (aopGet (AOP (oper), 0, FALSE, FALSE, NULL));
    }
    
  while (size--)
    {
      if (usedB)
	{
	  emitcode ("orl", "b,%s",
		    aopGet (AOP (oper), offset++, FALSE, FALSE, NULL));
	}
      else
	{
	  emitcode ("orl", "a,%s",
		    aopGet (AOP (oper), offset++, FALSE, FALSE, DP2_RESULT_REG));
	}
    }
  _endLazyDPSEvaluation ();

  if (usedB)
    {
      emitcode ("mov", "a,b");
      emitcode ("pop", "b");
    }
}


/*-----------------------------------------------------------------*/
/* genNot - generate code for ! operation                          */
/*-----------------------------------------------------------------*/
static void
genNot (iCode * ic)
{
  symbol *tlbl;
  sym_link *optype = operandType (IC_LEFT (ic));

  D (emitcode (";", "genNot "););

  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE, AOP_USESDPTR(IC_LEFT (ic)));

  /* if in bit space then a special case */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", IC_LEFT (ic)->aop->aopu.aop_dir);
      emitcode ("cpl", "c");
      outBitC (IC_RESULT (ic));
      goto release;
    }

  /* if type float then do float */
  if (IS_FLOAT (optype))
    {
      genNotFloat (IC_LEFT (ic), IC_RESULT (ic));
      goto release;
    }

  toBoolean (IC_LEFT (ic));

  tlbl = newiTempLabel (NULL);
  emitcode ("cjne", "a,#1,!tlabel", tlbl->key + 100);
  emitcode ("", "!tlabeldef", tlbl->key + 100);
  outBitC (IC_RESULT (ic));

release:
  /* release the aops */
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
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

  D (emitcode (";", "genCpl "););


  /* assign asmOps to operand & result */
  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE, AOP_USESDPTR(IC_LEFT (ic)));

  /* special case if in bit space */
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY) {
    if (AOP_TYPE (IC_LEFT (ic)) == AOP_CRY) {
      emitcode ("mov", "c,%s", IC_LEFT (ic)->aop->aopu.aop_dir);
      emitcode ("cpl", "c");
      emitcode ("mov", "%s,c", IC_RESULT (ic)->aop->aopu.aop_dir);
      goto release;
    }
    tlbl=newiTempLabel(NULL);
    emitcode ("cjne", "%s,#0x01,%05d$", 
	      aopGet(AOP(IC_LEFT(ic)), 0, FALSE,FALSE,NULL), tlbl->key+100);
    emitcode ("", "%05d$:", tlbl->key+100);
    outBitC (IC_RESULT(ic));
    goto release;
  }

  size = AOP_SIZE (IC_RESULT (ic));
  _startLazyDPSEvaluation ();
  while (size--)
    {
      MOVA (aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL));
      emitcode ("cpl", "a");
      aopPut (AOP (IC_RESULT (ic)), "a", offset++);
    }
  _endLazyDPSEvaluation ();


release:
  /* release the aops */
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genUminusFloat - unary minus for floating points                */
/*-----------------------------------------------------------------*/
static void
genUminusFloat (operand * op, operand * result)
{
  int size, offset = 0;
    
  D(emitcode (";", "genUminusFloat"););
  
  /* for this we just copy and then flip the bit */
    
  _startLazyDPSEvaluation ();
  size = AOP_SIZE (op) - 1;

  while (size--)
  {
      aopPut (AOP (result),
	      aopGet (AOP (op), offset, FALSE, FALSE, NULL),
	      offset);
      offset++;
    }
  
  MOVA(aopGet (AOP (op), offset, FALSE, FALSE, NULL));

  emitcode ("cpl", "acc.7");
  aopPut (AOP (result), "a", offset);    
  _endLazyDPSEvaluation ();
}

/*-----------------------------------------------------------------*/
/* genUminus - unary minus code generation                         */
/*-----------------------------------------------------------------*/
static void
genUminus (iCode * ic)
{
  int offset, size;
  sym_link *optype;

  D (emitcode (";", "genUminus "););

  /* assign asmops */
  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
  aopOp (IC_RESULT (ic), ic, TRUE, AOP_USESDPTR(IC_LEFT (ic)));

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
  _startLazyDPSEvaluation ();
  while (size--)
    {
      char *l = aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL);
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
      aopPut (AOP (IC_RESULT (ic)), "a", offset++);
    }
  _endLazyDPSEvaluation ();

  /* if any remaining bytes in the result */
  /* we just need to propagate the sign   */
  if ((size = (AOP_SIZE(IC_RESULT(ic)) - AOP_SIZE(IC_LEFT(ic)))) != 0)
    {
      emitcode ("rlc", "a");
      emitcode ("subb", "a,acc");
      while (size--)
	aopPut (AOP (IC_RESULT (ic)), "a", offset++);
    }

release:
  /* release the aops */
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? 0 : 1));
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* savermask - saves registers in the mask                         */
/*-----------------------------------------------------------------*/
static void savermask(bitVect *rs_mask)
{
    int i;
    if (options.useXstack) {
	if (bitVectBitValue (rs_mask, R0_IDX))
	    emitcode ("mov", "b,r0");
	emitcode ("mov", "r0,%s", spname);
	for (i = 0; i < ds390_nRegs; i++) {
	    if (bitVectBitValue (rs_mask, i)) {
		if (i == R0_IDX)
		    emitcode ("mov", "a,b");
		else
		    emitcode ("mov", "a,%s", ds390_regWithIdx (i)->name);
		emitcode ("movx", "@r0,a");
		emitcode ("inc", "r0");
	    }
	}
	emitcode ("mov", "%s,r0", spname);
	if (bitVectBitValue (rs_mask, R0_IDX))
	    emitcode ("mov", "r0,b");
    } else {
	for (i = 0; i < ds390_nRegs; i++) {
	    if (bitVectBitValue (rs_mask, i))
		emitcode ("push", "%s", ds390_regWithIdx (i)->dname);
	}
    }
}

/*-----------------------------------------------------------------*/
/* saveRegisters - will look for a call and save the registers     */
/*-----------------------------------------------------------------*/
static void
saveRegisters (iCode * lic)
{
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

  /* if the registers have been saved already then
     do nothing */
  if (ic->regsSaved || IFFUNC_ISNAKED(OP_SYM_TYPE(IC_LEFT(ic)))) return ;

  /* special case if DPTR alive across a function call then must save it 
     even though callee saves */
  if (IFFUNC_CALLEESAVES(OP_SYMBOL (IC_LEFT (ic))->type)) {
      int i;
      rsave = newBitVect(ic->rMask->size);
      for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	  if (bitVectBitValue(ic->rMask,i))
	      rsave = bitVectSetBit(rsave,i);
      }
      rsave = bitVectCplAnd(rsave,ds390_rUmaskForOp (IC_RESULT(ic)));
  } else {
    /* safe the registers in use at this time but skip the
       ones for the result */
    rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
			   ds390_rUmaskForOp (IC_RESULT(ic)));
  }
  ic->regsSaved = 1;
  savermask(rsave);
}

/*-----------------------------------------------------------------*/
/* usavermask - restore registers with mask                        */
/*-----------------------------------------------------------------*/
static void unsavermask(bitVect *rs_mask)
{
    int i;
    if (options.useXstack) {
	emitcode ("mov", "r0,%s", spname);
	for (i = ds390_nRegs; i >= 0; i--) {
	    if (bitVectBitValue (rs_mask, i)) {
		emitcode ("dec", "r0");
		emitcode ("movx", "a,@r0");
		if (i == R0_IDX)
		    emitcode ("mov", "b,a");
		else
		    emitcode ("mov", "%s,a", ds390_regWithIdx (i)->name);
	    }	    
	}
	emitcode ("mov", "%s,r0", spname);
	if (bitVectBitValue (rs_mask, R0_IDX))
	    emitcode ("mov", "r0,b");
    } else {
	for (i = ds390_nRegs; i >= 0; i--) {
	    if (bitVectBitValue (rs_mask, i))
		emitcode ("pop", "%s", ds390_regWithIdx (i)->dname);
	}
    }
}

/*-----------------------------------------------------------------*/
/* unsaveRegisters - pop the pushed registers                      */
/*-----------------------------------------------------------------*/
static void
unsaveRegisters (iCode * ic)
{
  bitVect *rsave;

  if (IFFUNC_CALLEESAVES(OP_SYMBOL (IC_LEFT (ic))->type)) {
      int i;
      rsave = newBitVect(ic->rMask->size);
      for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	  if (bitVectBitValue(ic->rMask,i))
	      rsave = bitVectSetBit(rsave,i);
      }
      rsave = bitVectCplAnd(rsave,ds390_rUmaskForOp (IC_RESULT(ic)));
  } else {
    /* restore the registers in use at this time but skip the
       ones for the result */
    rsave = bitVectCplAnd (bitVectCopy (ic->rMask), 
			   ds390_rUmaskForOp (IC_RESULT(ic)));
  }
  unsavermask(rsave);
}


/*-----------------------------------------------------------------*/
/* pushSide -                */
/*-----------------------------------------------------------------*/
static void
pushSide (operand * oper, int size)
{
  int offset = 0;
  _startLazyDPSEvaluation ();
  while (size--)
    {
      char *l = aopGet (AOP (oper), offset++, FALSE, TRUE, NULL);
      if (AOP_TYPE (oper) != AOP_REG &&
	  AOP_TYPE (oper) != AOP_DIR &&
	  strcmp (l, "a"))
	{
	  emitcode ("mov", "a,%s", l);
	  emitcode ("push", "acc");
	}
      else
	emitcode ("push", "%s", l);
    }
  _endLazyDPSEvaluation ();
}

/*-----------------------------------------------------------------*/
/* assignResultValue -               */
/*-----------------------------------------------------------------*/
static void
assignResultValue (operand * oper)
{
  int offset = 0;
  int size = AOP_SIZE (oper);

  _startLazyDPSEvaluation ();
  while (size--)
    {
      aopPut (AOP (oper), fReturn[offset], offset);
      offset++;
    }
  _endLazyDPSEvaluation ();
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

  D (emitcode (";", "genXpush ");
    );

  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
  r = getFreePtr (ic, &aop, FALSE);


  emitcode ("mov", "%s,_spx", r->name);

  size = AOP_SIZE (IC_LEFT (ic));
  _startLazyDPSEvaluation ();
  while (size--)
    {

      MOVA (aopGet (AOP (IC_LEFT (ic)),
			offset++, FALSE, FALSE, NULL));
      emitcode ("movx", "@%s,a", r->name);
      emitcode ("inc", "%s", r->name);

    }
  _endLazyDPSEvaluation ();


  emitcode ("mov", "_spx,%s", r->name);

  freeAsmop (NULL, aop, ic, TRUE);
  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genIpush - generate code for pushing this gets a little complex  */
/*-----------------------------------------------------------------*/
static void
genIpush (iCode * ic)
{
  int size, offset = 0;
  char *l;

  D (emitcode (";", "genIpush ");
    );

  /* if this is not a parm push : ie. it is spill push
     and spill push is always done on the local stack */
  if (!ic->parmPush)
    {

      /* and the item is spilt then do nothing */
      if (OP_SYMBOL (IC_LEFT (ic))->isspilt || OP_SYMBOL(IC_LEFT(ic))->dptr)
	return;

      aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
      size = AOP_SIZE (IC_LEFT (ic));
      /* push it on the stack */
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  l = aopGet (AOP (IC_LEFT (ic)), offset++, FALSE, TRUE, NULL);
	  if (*l == '#')
	    {
	      MOVA (l);
	      l = "acc";
	    }
	  emitcode ("push", "%s", l);
	}
      _endLazyDPSEvaluation ();
      return;
    }

  /* this is a paramter push: in this case we call
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
  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);

  // pushSide(IC_LEFT(ic), AOP_SIZE(IC_LEFT(ic)));
  size = AOP_SIZE (IC_LEFT (ic));

  _startLazyDPSEvaluation ();
  while (size--)
    {
      l = aopGet (AOP (IC_LEFT (ic)), offset++, FALSE, TRUE, NULL);
      if (AOP_TYPE (IC_LEFT (ic)) != AOP_REG &&
	  AOP_TYPE (IC_LEFT (ic)) != AOP_DIR &&
	  strcmp (l, "acc"))
	{
	  emitcode ("mov", "a,%s", l);
	  emitcode ("push", "acc");
	}
      else
	{
	    emitcode ("push", "%s", l);
	}
    }
  _endLazyDPSEvaluation ();

  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genIpop - recover the registers: can happen only for spilling   */
/*-----------------------------------------------------------------*/
static void
genIpop (iCode * ic)
{
  int size, offset;

  D (emitcode (";", "genIpop ");
    );


  /* if the temp was not pushed then */
  if (OP_SYMBOL (IC_LEFT (ic))->isspilt || OP_SYMBOL (IC_LEFT (ic))->dptr)
    return;

  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);
  size = AOP_SIZE (IC_LEFT (ic));
  offset = (size - 1);
  _startLazyDPSEvaluation ();
  while (size--)
    {
      emitcode ("pop", "%s", aopGet (AOP (IC_LEFT (ic)), offset--,
				     FALSE, TRUE, NULL));
    }
  _endLazyDPSEvaluation ();

  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* unsaveRBank - restores the resgister bank from stack            */
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
      	  r = ds390_regWithIdx (R0_IDX);;          
      }	
      else
      {
	  aop = newAsmop (0);
	  r = getFreePtr (ic, &aop, FALSE);
      }
      emitcode ("mov", "%s,_spx", r->name);      
  }
  
  if (popPsw)
    {
      if (options.useXstack)
      {
	  emitcode ("movx", "a,@%s", r->name);
	  emitcode ("mov", "psw,a");
	  emitcode ("dec", "%s", r->name);
	}
      else
      {
	emitcode ("pop", "psw");
      }
    }

  for (i = 7; i >= 0; i--) /* only R7-R0 needs to be popped */
    {
      if (options.useXstack)
	{
	  emitcode ("movx", "a,@%s", r->name);
	  emitcode ("mov", "(%s+%d),a",
		    regs390[i].base, 8 * bank + regs390[i].offset);
	  emitcode ("dec", "%s", r->name);

	}
      else
	emitcode ("pop", "(%s+%d)",
		  regs390[i].base, 8 * bank + regs390[i].offset);
    }

  if (options.useXstack)
    {
      emitcode ("mov", "_spx,%s", r->name);
    }
    
  if (aop)
  {
      freeAsmop (NULL, aop, ic, TRUE);  
  }    
}

/*-----------------------------------------------------------------*/
/* saveRBank - saves an entire register bank on the stack          */
/*-----------------------------------------------------------------*/
static void
saveRBank (int bank, iCode * ic, bool pushPsw)
{
  int i;
  asmop *aop = NULL;
  regs *r = NULL;

  if (options.useXstack)
    {
        if (!ic)
        {
       	  /* Assume r0 is available for use. */
        	  r = ds390_regWithIdx (R0_IDX);;
        }
        else
        {
       	  aop = newAsmop (0);
       	  r = getFreePtr (ic, &aop, FALSE);
        }
        emitcode ("mov", "%s,_spx", r->name);    
    }

  for (i = 0; i < 8 ; i++) /* only R0-R7 needs saving */
    {
      if (options.useXstack)
	{
	  emitcode ("inc", "%s", r->name);
	  emitcode ("mov", "a,(%s+%d)",
		    regs390[i].base, 8 * bank + regs390[i].offset);
	  emitcode ("movx", "@%s,a", r->name);
	}
      else
	emitcode ("push", "(%s+%d)",
		  regs390[i].base, 8 * bank + regs390[i].offset);
    }

  if (pushPsw)
    {
      if (options.useXstack)
	{
	  emitcode ("mov", "a,psw");
	  emitcode ("movx", "@%s,a", r->name);
	  emitcode ("inc", "%s", r->name);
	  emitcode ("mov", "_spx,%s", r->name);
	}
      else
      {
	emitcode ("push", "psw");
      }

      emitcode ("mov", "psw,#!constbyte", (bank << 3) & 0x00ff);
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
/* genSend - gen code for SEND	                                   */
/*-----------------------------------------------------------------*/
static void genSend(set *sendSet)
{
    iCode *sic;
    int sendCount = 0 ;
    static int rb1_count = 0;

    for (sic = setFirstItem (sendSet); sic;
	 sic = setNextItem (sendSet)) {	    
	int size, offset = 0;
	
	size=getSize(operandType(IC_LEFT(sic)));
	D (emitcode (";", "genSend argreg = %d, size = %d ",sic->argreg,size));
	if (sendCount == 0) { /* first parameter */
	    // we know that dpl(hxb) is the result, so
	    rb1_count = 0 ;
	    _startLazyDPSEvaluation ();
	    if (size>1) {
		aopOp (IC_LEFT (sic), sic, FALSE, 
		       (AOP_IS_STR(IC_LEFT(sic)) ? FALSE : TRUE));
	    } else {
		aopOp (IC_LEFT (sic), sic, FALSE, FALSE);
	    }
	    while (size--) {
		char *l = aopGet (AOP (IC_LEFT (sic)), offset,
				  FALSE, FALSE, NULL);
		if (strcmp (l, fReturn[offset])) {
		    emitcode ("mov", "%s,%s",
			      fReturn[offset],
			      l);
		}
		offset++;
	    }
	    _endLazyDPSEvaluation ();
	    freeAsmop (IC_LEFT (sic), NULL, sic, TRUE);
	    rb1_count =0;
	} else { /* if more parameter in registers */
	    aopOp (IC_LEFT (sic), sic, FALSE, TRUE);
	    while (size--) {
		emitcode ("mov","b1_%d,%s",rb1_count++,aopGet (AOP (IC_LEFT (sic)), offset++,
								FALSE, FALSE, NULL));
	    }
	    freeAsmop (IC_LEFT (sic), NULL, sic, TRUE);
	}
	sendCount++;
    }
}

/*-----------------------------------------------------------------*/
/* genCall - generates a call statement                            */
/*-----------------------------------------------------------------*/
static void
genCall (iCode * ic)
{
  sym_link *dtype;
  bool restoreBank = FALSE;
  bool swapBanks = FALSE;

  D (emitcode (";", "genCall "););

  /* if we are calling a not _naked function that is not using
     the same register bank then we need to save the
     destination registers on the stack */
  dtype = operandType (IC_LEFT (ic));
  if (currFunc && dtype && !IFFUNC_ISNAKED(dtype) &&
      (FUNC_REGBANK (currFunc->type) != FUNC_REGBANK (dtype)) &&
      IFFUNC_ISISR (currFunc->type))
  {
      if (!ic->bankSaved) 
      {
           /* This is unexpected; the bank should have been saved in
            * genFunction.
            */
    	   saveRBank (FUNC_REGBANK (dtype), ic, FALSE);
    	   restoreBank = TRUE;
      }
      swapBanks = TRUE;
  }
  
    /* if caller saves & we have not saved then */
    if (!ic->regsSaved)
      saveRegisters (ic);
  
  /* if send set is not empty the assign */
  /* We've saved all the registers we care about;
  * therefore, we may clobber any register not used
  * in the calling convention (i.e. anything not in
  * fReturn.
  */
  if (_G.sendSet)
    {
	if (IFFUNC_ISREENT(dtype)) { /* need to reverse the send set */
	    genSend(reverseSet(_G.sendSet));
	} else {
	    genSend(_G.sendSet);
	}
      _G.sendSet = NULL;
    }  
    
  if (swapBanks)
  {
        emitcode ("mov", "psw,#!constbyte", 
           ((FUNC_REGBANK(dtype)) << 3) & 0xff);
  }

  /* make the call */
  emitcode ("lcall", "%s", (OP_SYMBOL (IC_LEFT (ic))->rname[0] ?
			    OP_SYMBOL (IC_LEFT (ic))->rname :
			    OP_SYMBOL (IC_LEFT (ic))->name));

  if (swapBanks)
  {
       emitcode ("mov", "psw,#!constbyte", 
          ((FUNC_REGBANK(currFunc->type)) << 3) & 0xff);
  }

  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs ||
	OP_SYMBOL (IC_RESULT (ic))->accuse ||
	OP_SYMBOL (IC_RESULT (ic))->spildir)) ||
      IS_TRUE_SYMOP (IC_RESULT (ic)))
    {
      if (isOperandInFarSpace (IC_RESULT (ic))
	  && getSize (operandType (IC_RESULT (ic))) <= 2)
	{
	  int size = getSize (operandType (IC_RESULT (ic)));

	  /* Special case for 1 or 2 byte return in far space. */
	  MOVA (fReturn[0]);
	  if (size > 1)
	    {
	      emitcode ("mov", "b,%s", fReturn[1]);
	    }

	  aopOp (IC_RESULT (ic), ic, FALSE, FALSE);
	  aopPut (AOP (IC_RESULT (ic)), "a", 0);

	  if (size > 1)
	    {
	      aopPut (AOP (IC_RESULT (ic)), "b", 1);
	    }
	  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
	}
      else
	{
	  _G.accInUse++;
	  aopOp (IC_RESULT (ic), ic, FALSE, TRUE);
	  _G.accInUse--;

	  assignResultValue (IC_RESULT (ic));

	  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
	}
    }

  /* adjust the stack for parameters if
     required */
  if (ic->parmBytes) {
      int i;
      if (options.stack10bit) {
	  if (ic->parmBytes <= 10) {
	      emitcode(";","stack adjustment for parms");
	      for (i=0; i < ic->parmBytes ; i++) {
		  emitcode("pop","acc");
	      }
	  } else {	      
	      PROTECT_SP;
	      emitcode ("clr","c");
	      emitcode ("mov","a,sp");
	      emitcode ("subb","a,#!constbyte",ic->parmBytes & 0xff);
	      emitcode ("mov","sp,a");
	      emitcode ("mov","a,esp");
	      emitcode ("anl","a,#3");
	      emitcode ("subb","a,#!constbyte",(ic->parmBytes >> 8) & 0xff);
	      emitcode ("mov","esp,a");	  
	      UNPROTECT_SP;
	  }
      } else {
	  if (ic->parmBytes > 3) {
	      emitcode ("mov", "a,%s", spname);
	      emitcode ("add", "a,#!constbyte", (-ic->parmBytes) & 0xff);
	      emitcode ("mov", "%s,a", spname);
	  } else
	      for (i = 0; i < ic->parmBytes; i++)
		  emitcode ("dec", "%s", spname);
      }
  }

  /* if we hade saved some registers then unsave them */
  if (ic->regsSaved)
    unsaveRegisters (ic);

  /* if register bank was saved then pop them */
  if (restoreBank)
    unsaveRBank (FUNC_REGBANK (dtype), ic, FALSE);
}

/*-----------------------------------------------------------------*/
/* genPcall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void
genPcall (iCode * ic)
{
  sym_link *dtype;
  symbol *rlbl = newiTempLabel (NULL);
  bool restoreBank=FALSE;

  D (emitcode (";", "genPcall ");
    );


  /* if caller saves & we have not saved then */
  if (!ic->regsSaved)
    saveRegisters (ic);

  /* if we are calling a function that is not using
     the same register bank then we need to save the
     destination registers on the stack */
  dtype = operandType (IC_LEFT (ic));
  if (currFunc && dtype && !IFFUNC_ISNAKED(dtype) &&
      IFFUNC_ISISR (currFunc->type) &&
      (FUNC_REGBANK (currFunc->type) != FUNC_REGBANK (dtype))) {
    saveRBank (FUNC_REGBANK (dtype), ic, TRUE);
    restoreBank=TRUE;
  }

  /* push the return address on to the stack */
  emitcode ("mov", "a,#!tlabel", (rlbl->key + 100));
  emitcode ("push", "acc");
  emitcode ("mov", "a,#!hil", (rlbl->key + 100));
  emitcode ("push", "acc");

  if (options.model == MODEL_FLAT24)
    {
      emitcode ("mov", "a,#!hihil", (rlbl->key + 100));
      emitcode ("push", "acc");
    }

  /* now push the calling address */
  aopOp (IC_LEFT (ic), ic, FALSE, FALSE);

  pushSide (IC_LEFT (ic), FPTRSIZE);

  freeAsmop (IC_LEFT (ic), NULL, ic, TRUE);

  /* if send set is not empty the assign */
  if (_G.sendSet)
    {
	genSend(reverseSet(_G.sendSet));
	_G.sendSet = NULL;
    }

  emitcode ("ret", "");
  emitcode ("", "!tlabeldef", (rlbl->key + 100));


  /* if we need assign a result value */
  if ((IS_ITEMP (IC_RESULT (ic)) &&
       (OP_SYMBOL (IC_RESULT (ic))->nRegs ||
	OP_SYMBOL (IC_RESULT (ic))->spildir)) ||
      IS_TRUE_SYMOP (IC_RESULT (ic)))
    {

      _G.accInUse++;
      aopOp (IC_RESULT (ic), ic, FALSE, TRUE);
      _G.accInUse--;

      assignResultValue (IC_RESULT (ic));

      freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
    }

  /* adjust the stack for parameters if
     required */
  if (ic->parmBytes)
    {
      int i;
      if (options.stack10bit) {
	  if (ic->parmBytes <= 10) {
	      emitcode(";","stack adjustment for parms");
	      for (i=0; i < ic->parmBytes ; i++) {
		  emitcode("pop","acc");
	      }
	  } else {	      
	      PROTECT_SP;
	      emitcode ("clr","c");
	      emitcode ("mov","a,sp");
	      emitcode ("subb","a,#!constbyte",ic->parmBytes & 0xff);
	      emitcode ("mov","sp,a");
	      emitcode ("mov","a,esp");
	      emitcode ("anl","a,#3");
	      emitcode ("subb","a,#!constbyte",(ic->parmBytes >> 8) & 0xff);
	      emitcode ("mov","esp,a");	  
	      UNPROTECT_SP;
	  }
      } else {
	  if (ic->parmBytes > 3) {
	      emitcode ("mov", "a,%s", spname);
	      emitcode ("add", "a,#!constbyte", (-ic->parmBytes) & 0xff);
	      emitcode ("mov", "%s,a", spname);
	  }
	  else
	      for (i = 0; i < ic->parmBytes; i++)
		  emitcode ("dec", "%s", spname);
	  
      }
    }
  /* if register bank was saved then unsave them */
  if (restoreBank)
    unsaveRBank (FUNC_REGBANK (dtype), ic, TRUE);
  
  /* if we hade saved some registers then
     unsave them */
  if (ic->regsSaved)
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

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

/*-----------------------------------------------------------------*/
/* inExcludeList - return 1 if the string is in exclude Reg list   */
/*-----------------------------------------------------------------*/
static bool
inExcludeList (char *s)
{
  int i = 0;

  if (options.excludeRegs[i] &&
      STRCASECMP (options.excludeRegs[i], "none") == 0)
    return FALSE;

  for (i = 0; options.excludeRegs[i]; i++)
    {
      if (options.excludeRegs[i] &&
	  STRCASECMP (s, options.excludeRegs[i]) == 0)
	return TRUE;
    }
  return FALSE;
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void
genFunction (iCode * ic)
{
  symbol *sym;
  sym_link *ftype;
  bool   switchedPSW = FALSE;

  D (emitcode (";", "genFunction "););

  _G.nRegsSaved = 0;
  /* create the function header */
  emitcode (";", "-----------------------------------------");
  emitcode (";", " function %s", (sym = OP_SYMBOL (IC_LEFT (ic)))->name);
  emitcode (";", "-----------------------------------------");

  emitcode ("", "%s:", sym->rname);
  ftype = operandType (IC_LEFT (ic));

  if (IFFUNC_ISNAKED(ftype))
  {
      emitcode(";", "naked function: no prologue.");
      return;
  }
  
  if (options.stack_probe) 
      emitcode ("lcall","__stack_probe");
  /* if critical function then turn interrupts off */
  if (IFFUNC_ISCRITICAL (ftype))
    emitcode ("clr", "ea");

  /* here we need to generate the equates for the
     register bank if required */
  if (FUNC_REGBANK (ftype) != rbank)
    {
      int i;

      rbank = FUNC_REGBANK (ftype);
      for (i = 0; i < ds390_nRegs; i++)
	{
	  if (regs390[i].print) {
	      if (strcmp (regs390[i].base, "0") == 0)
		  emitcode ("", "%s !equ !constbyte",
			    regs390[i].dname,
			    8 * rbank + regs390[i].offset);
	      else
		  emitcode ("", "%s !equ %s + !constbyte",
			    regs390[i].dname,
			    regs390[i].base,
			    8 * rbank + regs390[i].offset);
	  }
	}
    }

  /* if this is an interrupt service routine then
     save acc, b, dpl, dph  */
  if (IFFUNC_ISISR (sym->type))
      { /* is ISR */
      if (!inExcludeList ("acc"))
	emitcode ("push", "acc");
      if (!inExcludeList ("b"))
	emitcode ("push", "b");
      if (!inExcludeList ("dpl"))
	emitcode ("push", "dpl");
      if (!inExcludeList ("dph"))
	emitcode ("push", "dph");
      if (options.model == MODEL_FLAT24 && !inExcludeList ("dpx"))
	{
	  emitcode ("push", "dpx");
	  /* Make sure we're using standard DPTR */
	  emitcode ("push", "dps");
	  emitcode ("mov", "dps,#0");
	  if (options.stack10bit)
	    {
	      /* This ISR could conceivably use DPTR2. Better save it. */
	      emitcode ("push", "dpl1");
	      emitcode ("push", "dph1");
	      emitcode ("push", "dpx1");
	      emitcode ("push",  DP2_RESULT_REG);
	    }
	}
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
		      if (bitVectBitValue (sym->regsUsed, i) ||
			  (ds390_ptrRegReq && (i == R0_IDX || i == R1_IDX)))
			emitcode ("push", "%s", ds390_regWithIdx (i)->dname);
		    }
		}

	    }
	  else
	    {
	      /* this function has  a function call cannot
	         determines register usage so we will have to push the
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
		    emitcode ("mov", "psw,#!constbyte", 
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
	    // jwk: this needs a closer look
	    SPEC_ISR_SAVED_BANKS(currFunc->etype) = banksToSave;
	}
    }
  else
    {
      /* if callee-save to be used for this function
         then save the registers being used in this function */
      if (IFFUNC_CALLEESAVES(sym->type))
	{
	  int i;

	  /* if any registers used */
	  if (sym->regsUsed)
	    {
	      /* save the registers used */
	      for (i = 0; i < sym->regsUsed->size; i++)
		{
		  if (bitVectBitValue (sym->regsUsed, i) ||
		      (ds390_ptrRegReq && (i == R0_IDX || i == R1_IDX)))
		    {
		      emitcode ("push", "%s", ds390_regWithIdx (i)->dname);
		      _G.nRegsSaved++;
		    }
		}
	    }
	}
    }

  /* set the register bank to the desired value */
  if ((FUNC_REGBANK (sym->type) || FUNC_ISISR (sym->type))
   && !switchedPSW)
    {
      emitcode ("push", "psw");
      emitcode ("mov", "psw,#!constbyte", (FUNC_REGBANK (sym->type) << 3) & 0x00ff);
    }

  if ( (IFFUNC_ISREENT (sym->type) || options.stackAuto) &&
       (sym->stack || FUNC_HASSTACKPARM(sym->type))) {
      if (options.stack10bit) {
	  emitcode ("push","_bpx");
	  emitcode ("push","_bpx+1");
	  emitcode ("mov","_bpx,%s",spname);
	  emitcode ("mov","_bpx+1,esp");
	  emitcode ("anl","_bpx+1,#3");
      } else {
	  if (options.useXstack) {
	      emitcode ("mov", "r0,%s", spname);
	      emitcode ("mov", "a,_bp");
	      emitcode ("movx", "@r0,a");
	      emitcode ("inc", "%s", spname);
	  } else {
	      /* set up the stack */
	      emitcode ("push", "_bp");	/* save the callers stack  */
	  }
	  emitcode ("mov", "_bp,%s", spname);
      }
  }

  /* adjust the stack for the function */
  if (sym->stack) {
      int i = sym->stack;
      if (options.stack10bit) {
	  if ( i > 1024) werror (W_STACK_OVERFLOW, sym->name);	  
	  assert (sym->recvSize <= 4);
	  if (sym->stack <= 8) {
	      while (i--) emitcode ("push","acc");
	  } else {
	      PROTECT_SP;
	      emitcode ("mov","a,sp");
	      emitcode ("add","a,#!constbyte", ((short) sym->stack & 0xff));
	      emitcode ("mov","sp,a");
	      emitcode ("mov","a,esp");
	      emitcode ("anl","a,#3");
	      emitcode ("addc","a,#!constbyte", (((short) sym->stack) >> 8) & 0xff);
	      emitcode ("mov","esp,a");
	      UNPROTECT_SP;
	  }
      } else {
	  if (i > 256)
	      werror (W_STACK_OVERFLOW, sym->name);
	  
	  if (i > 3 && sym->recvSize < 4) {
	      
	      emitcode ("mov", "a,sp");
	      emitcode ("add", "a,#!constbyte", ((char) sym->stack & 0xff));
	      emitcode ("mov", "sp,a");
	      
	  } else
	      while (i--)
		  emitcode ("inc", "sp");
      }
  }

  if (sym->xstack)
    {

      emitcode ("mov", "a,_spx");
      emitcode ("add", "a,#!constbyte", ((char) sym->xstack & 0xff));
      emitcode ("mov", "_spx,a");
    }

}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void
genEndFunction (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));

  D (emitcode (";", "genEndFunction "););

  if (IFFUNC_ISNAKED(sym->type))
  {
      emitcode(";", "naked function: no epilogue.");
      return;
  }

  if ((IFFUNC_ISREENT (sym->type) || options.stackAuto) &&
       (sym->stack || FUNC_HASSTACKPARM(sym->type))) {

      if (options.stack10bit) {
	  PROTECT_SP;	  
	  emitcode ("mov", "sp,_bpx", spname);
	  emitcode ("mov", "esp,_bpx+1", spname);
	  UNPROTECT_SP;
      } else {
	  emitcode ("mov", "%s,_bp", spname);
      }
  }

  /* if use external stack but some variables were
     added to the local stack then decrement the
     local stack */
  if (options.useXstack && sym->stack) {
      emitcode ("mov", "a,sp");
      emitcode ("add", "a,#!constbyte", ((char) -sym->stack) & 0xff);
      emitcode ("mov", "sp,a");
  }


  if ((IFFUNC_ISREENT (sym->type) || options.stackAuto) &&
       (sym->stack || FUNC_HASSTACKPARM(sym->type))) {

      if (options.useXstack) {
	  emitcode ("mov", "r0,%s", spname);
	  emitcode ("movx", "a,@r0");
	  emitcode ("mov", "_bp,a");
	  emitcode ("dec", "%s", spname);
      } else {
	  if (options.stack10bit) {
	      emitcode ("pop", "_bpx+1");
	      emitcode ("pop", "_bpx");
	  } else {
	      emitcode ("pop", "_bp");
	  }
      }
  }

  /* restore the register bank  */
  if (FUNC_REGBANK (sym->type) || IFFUNC_ISISR (sym->type))
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
      { /* is ISR */  

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
		      if (bitVectBitValue (sym->regsUsed, i) ||
			  (ds390_ptrRegReq && (i == R0_IDX || i == R1_IDX)))
			emitcode ("pop", "%s", ds390_regWithIdx (i)->dname);
		    }
		}

	    }
	  else
	    {
	      /* this function has  a function call cannot
	         determines register usage so we will have to pop the
	         entire bank */
	      if (options.parms_in_bank1) {
		  for (i = 7 ; i >= 0 ; i-- ) {
		      emitcode ("pop","%s",rb1regs[i]);
		  }
	      }
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
	  // jwk: this needs a closer look
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

      if (options.model == MODEL_FLAT24 && !inExcludeList ("dpx"))
	{
	  if (options.stack10bit)
	    {
	      emitcode ("pop", DP2_RESULT_REG);
	      emitcode ("pop", "dpx1");
	      emitcode ("pop", "dph1");
	      emitcode ("pop", "dpl1");
	    }
	  emitcode ("pop", "dps");
	  emitcode ("pop", "dpx");
	}
      if (!inExcludeList ("dph"))
	emitcode ("pop", "dph");
      if (!inExcludeList ("dpl"))
	emitcode ("pop", "dpl");
      if (!inExcludeList ("b"))
	emitcode ("pop", "b");
      if (!inExcludeList ("acc"))
	emitcode ("pop", "acc");

      if (IFFUNC_ISCRITICAL (sym->type))
	emitcode ("setb", "ea");

      /* if debug then send end of function */
      if (options.debug && currFunc) {
	  _G.debugLine = 1;
	  emitcode ("", "C$%s$%d$%d$%d ==.",
		    FileBaseName (ic->filename), currFunc->lastLine,
		    ic->level, ic->block);
	  if (IS_STATIC (currFunc->etype))
	    emitcode ("", "XF%s$%s$0$0 ==.", moduleName, currFunc->name);
	  else
	    emitcode ("", "XG$%s$0$0 ==.", currFunc->name);
	  _G.debugLine = 0;
	}

      emitcode ("reti", "");
    }
  else
    {
      if (IFFUNC_ISCRITICAL (sym->type))
	emitcode ("setb", "ea");

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
		      (ds390_ptrRegReq && (i == R0_IDX || i == R1_IDX)))
		    emitcode ("pop", "%s", ds390_regWithIdx (i)->dname);
		}
	    }

	}

      /* if debug then send end of function */
      if (options.debug && currFunc)
	{
	  _G.debugLine = 1;
	  emitcode ("", "C$%s$%d$%d$%d ==.",
		    FileBaseName (ic->filename), currFunc->lastLine,
		    ic->level, ic->block);
	  if (IS_STATIC (currFunc->etype))
	    emitcode ("", "XF%s$%s$0$0 ==.", moduleName, currFunc->name);
	  else
	    emitcode ("", "XG$%s$0$0 ==.", currFunc->name);
	  _G.debugLine = 0;
	}

      emitcode ("ret", "");
    }

}

/*-----------------------------------------------------------------*/
/* genJavaNativeRet - generate code for return JavaNative          */
/*-----------------------------------------------------------------*/
static void genJavaNativeRet(iCode *ic)
{
    int i, size;

    aopOp (IC_LEFT (ic), ic, FALSE, 
	   AOP_IS_STR(IC_LEFT(ic)) ? FALSE :TRUE);
    size = AOP_SIZE (IC_LEFT (ic));

    assert (size <= 4);

    /* it is assigned to GPR0-R3 then push them */
    if (aopHasRegs(AOP(IC_LEFT(ic)),R0_IDX,R1_IDX) ||
	aopHasRegs(AOP(IC_LEFT(ic)),R2_IDX,R3_IDX)) {
	for (i = 0 ; i < size ; i++ ) {
	    emitcode ("push","%s",
		      aopGet(AOP(IC_LEFT(ic)),i,FALSE,TRUE,DP2_RESULT_REG));
	}
	for (i = (size-1) ; i >= 0 ; i--) {
	    emitcode ("pop","a%s",javaRet[i]);
	}
    } else {
	for (i = 0 ; i < size ; i++) 
	    emitcode ("mov","%s,%s",javaRet[i],
		      aopGet(AOP(IC_LEFT(ic)),i,FALSE,TRUE,DP2_RESULT_REG));
    }
    for (i = size ; i < 4 ; i++ )
	    emitcode ("mov","%s,#0",javaRet[i]);
    return;
}

/*-----------------------------------------------------------------*/
/* genRet - generate code for return statement                     */
/*-----------------------------------------------------------------*/
static void
genRet (iCode * ic)
{
  int size, offset = 0, pushed = 0;

  D (emitcode (";", "genRet "););

  /* if we have no return value then
     just generate the "ret" */
  if (!IC_LEFT (ic))
    goto jumpret;

  /* if this is a JavaNative function then return 
     value in different register */
  if (IFFUNC_ISJAVANATIVE(currFunc->type)) {
      genJavaNativeRet(ic);
      goto jumpret;
  }
  /* we have something to return then
     move the return value into place */
  aopOp (IC_LEFT (ic), ic, FALSE, 
	 (AOP_IS_STR(IC_LEFT(ic)) ? FALSE :TRUE));
  size = AOP_SIZE (IC_LEFT (ic));

  _startLazyDPSEvaluation ();
  while (size--)
    {
      char *l;
      if (AOP_TYPE (IC_LEFT (ic)) == AOP_DPTR)
	{
	  l = aopGet (AOP (IC_LEFT (ic)), offset++,
		      FALSE, TRUE, NULL);
	  emitcode ("push", "%s", l);
	  pushed++;
	}
      else
	{
	  /* Since A is the last element of fReturn,
	   * is is OK to clobber it in the aopGet.
	   */
	  l = aopGet (AOP (IC_LEFT (ic)), offset,
		      FALSE, FALSE, NULL);
	  if (strcmp (fReturn[offset], l))
	    emitcode ("mov", "%s,%s", fReturn[offset++], l);
	}
    }
  _endLazyDPSEvaluation ();

  if (pushed)
    {
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

    emitcode ("ljmp", "!tlabel", (returnLabel->key + 100));

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

  D (emitcode (";", "genLabel ");
    );

  emitcode ("", "!tlabeldef", (IC_LABEL (ic)->key + 100));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a ljmp                                      */
/*-----------------------------------------------------------------*/
static void
genGoto (iCode * ic)
{
  D (emitcode (";", "genGoto ");
    );
  emitcode ("ljmp", "!tlabel", (IC_LABEL (ic)->key + 100));
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
	  /* printf("findLabelBackwards = %d\n", count); */
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

  /* if the literal value of the right hand side
     is greater than 4 then it is not worth it */
  if ((icount = (unsigned int) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit)) > 4)
    return FALSE;

  if (size == 1 && AOP(IC_LEFT(ic)) == AOP(IC_RESULT(ic)) &&
      AOP_TYPE(IC_LEFT(ic)) == AOP_DIR ) {
      while (icount--) {
	  emitcode("inc","%s",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE,NULL));
      }
      return TRUE;
  }
  /* if increment 16 bits in register */
  if (
       AOP_TYPE (IC_LEFT (ic)) == AOP_REG &&
       AOP_TYPE (IC_RESULT (ic)) == AOP_REG &&
       sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) &&
       (size > 1) &&
       (icount == 1))
    {
      symbol  *tlbl;
      int     emitTlbl;
      int     labelRange;
      char    *l;

      /* If the next instruction is a goto and the goto target
       * is <= 5 instructions previous to this, we can generate
       * jumps straight to that target.
       */
      if (ic->next && ic->next->op == GOTO
	  && (labelRange = findLabelBackwards (ic, IC_LABEL (ic->next)->key)) != 0
	  && labelRange <= 5)
	{
	  D(emitcode (";", "tail increment optimized (range %d)", labelRange););
	  tlbl = IC_LABEL (ic->next);
	  emitTlbl = 0;
	}
      else
	{
	  tlbl = newiTempLabel (NULL);
	  emitTlbl = 1;
	}
	
      l = aopGet (AOP (IC_RESULT (ic)), LSB, FALSE, FALSE, NULL);
      emitcode ("inc", "%s", l);
      
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
	  IS_AOP_PREG (IC_RESULT (ic)))
      {	  
	emitcode ("cjne", "%s,#0,!tlabel", l, tlbl->key + 100);
      }
      else
      {
	  emitcode ("clr", "a");
	  emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
      }

      l = aopGet (AOP (IC_RESULT (ic)), MSB16, FALSE, FALSE, NULL); 
      emitcode ("inc", "%s", l);
      if (size > 2)
	{
	    if (!strcmp(l, "acc"))
	    {
		emitcode("jnz", "!tlabel", tlbl->key + 100);
	    }
	    else if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
		     IS_AOP_PREG (IC_RESULT (ic)))
	    {
		emitcode ("cjne", "%s,#0,!tlabel", l, tlbl->key + 100);
	    }
	    else
	    {
		emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
	    }

	    l = aopGet (AOP (IC_RESULT (ic)), MSB24, FALSE, FALSE, NULL);
	    emitcode ("inc", "%s", l);
	}
      if (size > 3)
	{
	    if (!strcmp(l, "acc"))
	    {
		emitcode("jnz", "!tlabel", tlbl->key + 100);
	    }
	    else if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
		     IS_AOP_PREG (IC_RESULT (ic)))
	    {
		emitcode ("cjne", "%s,#0,!tlabel", l, tlbl->key + 100);
	    }
	    else
	    {
		emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
	    }

	    l = aopGet (AOP (IC_RESULT (ic)), MSB32, FALSE, FALSE, NULL);
	    emitcode ("inc", "%s", l);	}

      if (emitTlbl)
	{
	  emitcode ("", "!tlabeldef", tlbl->key + 100);
	}
      return TRUE;
    }

  if (AOP_TYPE(IC_RESULT(ic))==AOP_STR && IS_ITEMP(IC_RESULT(ic)) &&
      !AOP_USESDPTR(IC_LEFT(ic)) && icount <= 5 && size <= 3 && 
      options.model == MODEL_FLAT24 ) {

      switch (size) {
      case 3:
	  emitcode ("mov","dpx,%s",aopGet(AOP (IC_LEFT (ic)), 2, FALSE, FALSE, NULL));
      case 2:
	  emitcode ("mov","dph,%s",aopGet(AOP (IC_LEFT (ic)), 1, FALSE, FALSE, NULL));
      case 1:
	  emitcode ("mov","dpl,%s",aopGet(AOP (IC_LEFT (ic)), 0, FALSE, FALSE, NULL));
	  break;
      }
      while (icount--) emitcode ("inc","dptr");      
      return TRUE;
  }

  if (AOP_INDPTRn(IC_LEFT(ic)) && AOP_INDPTRn(IC_RESULT(ic)) &&
      AOP(IC_LEFT(ic))->aopu.dptr == AOP(IC_RESULT(ic))->aopu.dptr &&
      icount <= 5 ) {
      emitcode ("mov","dps,#!constbyte",AOP(IC_LEFT(ic))->aopu.dptr);
      while (icount--) emitcode ("inc","dptr");
      emitcode ("mov","dps,#0");
      return TRUE;
  }

  /* if the sizes are greater than 1 then we cannot */
  if (AOP_SIZE (IC_RESULT (ic)) > 1 ||
      AOP_SIZE (IC_LEFT (ic)) > 1)
    return FALSE;

  /* we can if the aops of the left & result match or
     if they are in registers and the registers are the
     same */
  if (
       AOP_TYPE (IC_LEFT (ic)) == AOP_REG &&
       AOP_TYPE (IC_RESULT (ic)) == AOP_REG &&
       sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {

      if (icount > 3)
	{
	  MOVA (aopGet (AOP (IC_LEFT (ic)), 0, FALSE, FALSE, NULL));
	  emitcode ("add", "a,#!constbyte", ((char) icount) & 0xff);
	  aopPut (AOP (IC_RESULT (ic)), "a", 0);
	}
      else
	{

	  _startLazyDPSEvaluation ();
	  while (icount--)
	    {
	      emitcode ("inc", "%s", aopGet (AOP (IC_LEFT (ic)), 0, FALSE, FALSE, NULL));
	    }
	  _endLazyDPSEvaluation ();
	}

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
      aopPut (AOP (result), "a", 0);
    }
  else
    {
      emitcode ("jz", "!tlabel", tlbl->key + 100);
      emitcode ("mov", "a,%s", one);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      outAcc (result);
    }
}

/*-----------------------------------------------------------------*/
/* genPlusBits - generates code for addition of two bits           */
/*-----------------------------------------------------------------*/
static void
genPlusBits (iCode * ic)
{
  D (emitcode (";", "genPlusBits "););
    
  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
    {
      symbol *lbl = newiTempLabel (NULL);
      emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
      emitcode ("jnb", "%s,!tlabel", AOP (IC_RIGHT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("cpl", "c");
      emitcode ("", "!tlabeldef", (lbl->key + 100));
      outBitC (IC_RESULT (ic));
    }
  else
    {
      emitcode ("clr", "a");
      emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
      emitcode ("rlc", "a");
      emitcode ("mov", "c,%s", AOP (IC_RIGHT (ic))->aopu.aop_dir);
      emitcode ("addc", "a,#0");
      outAcc (IC_RESULT (ic));
    }
}

static void
adjustArithmeticResult (iCode * ic)
{
  if (opIsGptr (IC_RESULT (ic)) &&
      opIsGptr (IC_LEFT (ic)) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))))
    {
      aopPut (AOP (IC_RESULT (ic)),
	      aopGet (AOP (IC_LEFT (ic)), GPTRSIZE - 1, FALSE, FALSE, NULL),
	      GPTRSIZE - 1);
    }

  if (opIsGptr (IC_RESULT (ic)) &&
      opIsGptr (IC_RIGHT (ic)) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    {
      aopPut (AOP (IC_RESULT (ic)),
	    aopGet (AOP (IC_RIGHT (ic)), GPTRSIZE - 1, FALSE, FALSE, NULL),
	      GPTRSIZE - 1);
    }

  if (opIsGptr (IC_RESULT (ic)) &&
      AOP_SIZE (IC_LEFT (ic)) < GPTRSIZE &&
      AOP_SIZE (IC_RIGHT (ic)) < GPTRSIZE &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_LEFT (ic))) &&
      !sameRegs (AOP (IC_RESULT (ic)), AOP (IC_RIGHT (ic))))
    {
      char buff[5];
      SNPRINTF (buff, sizeof(buff), 
		"#%d", pointerCode (getSpec (operandType (IC_LEFT (ic)))));
      aopPut (AOP (IC_RESULT (ic)), buff, GPTRSIZE - 1);
    }
}

// The guts of AOP_OP_3_NOFATAL. Generates the left & right opcodes of an IC,
// generates the result if possible. If result is generated, returns TRUE; otherwise
// returns false and caller must deal with fact that result isn't aopOp'd.
bool aopOp3(iCode * ic)
{
    bool dp1InUse, dp2InUse;
    bool useDp2;
    
    // First, generate the right opcode. DPTR may be used if neither left nor result are
    // of type AOP_STR.
    
//    D(emitcode(";", "aopOp3: AOP_IS_STR left: %s right: %s result: %s",
//	       AOP_IS_STR(IC_LEFT(ic)) ? "true" : "false",
//	       AOP_IS_STR(IC_RIGHT(ic)) ? "true" : "false",
//	       AOP_IS_STR(IC_RESULT(ic)) ? "true" : "false");
//      );
//    D(emitcode(";", "aopOp3: AOP_IS_DPTRn left: %s right: %s result: %s",
//	       AOP_IS_DPTRn(IC_LEFT(ic)) ? "true" : "false",
//	       AOP_IS_DPTRn(IC_RIGHT(ic)) ? "true" : "false",
//	       AOP_IS_DPTRn(IC_RESULT(ic)) ? "true" : "false");
//      );      

    
    // Right uses DPTR unless left or result is an AOP_STR.
    aopOp (IC_RIGHT(ic),ic,FALSE, AOP_IS_STR(IC_LEFT(ic)) || AOP_IS_STR(IC_RESULT(ic)));
    
    // if the right used DPTR, left MUST use DPTR2.
    // if the right used DPTR2, left MUST use DPTR.
    // if both are still available, we prefer to use DPTR. But if result is an AOP_STR
    // and left is not an AOP_STR, then we will get better code if we use DP2 for left,
    // enabling us to assign DPTR to result.
     
    if (AOP_USESDPTR(IC_RIGHT(ic)))
    {
	useDp2 = TRUE;
    }
    else if (AOP_USESDPTR2(IC_RIGHT(ic)))
    {
	useDp2 = FALSE;
    }
    else
    {
	if (AOP_IS_STR(IC_RESULT(ic)) && !AOP_IS_STR(IC_LEFT(ic)))
	{
	    useDp2 = TRUE;
	}
	else
	{
	    useDp2 = FALSE;
	}
    }

    aopOp(IC_LEFT(ic), ic, FALSE, useDp2);
    
    // We've op'd the left & right. So, if left or right are the same operand as result, 
    // we know aopOp will succeed, and we can just do it & bail.
    if (isOperandEqual(IC_LEFT(ic),IC_RESULT(ic)) ||
	isOperandEqual(IC_RIGHT(ic),IC_RESULT(ic)))
    {
//	D(emitcode(";", "aopOp3: (left | right) & result equal"););
	aopOp(IC_RESULT(ic),ic,TRUE, FALSE);
	return TRUE;
    }
    
    // Note which dptrs are currently in use.
    dp1InUse = AOP_USESDPTR(IC_LEFT(ic)) || AOP_USESDPTR(IC_RIGHT(ic));
    dp2InUse = AOP_USESDPTR2(IC_LEFT(ic)) || AOP_USESDPTR2(IC_RIGHT(ic));
    
    // OK, now if either left or right uses DPTR and the result is an AOP_STR, we cannot 
    // generate it.
    if (dp1InUse && AOP_IS_STR(IC_RESULT(ic)))
    {
	return FALSE;
    }
    
    // Likewise, if left or right uses DPTR2 and the result is a DPTRn, we cannot generate it.
    if (dp2InUse && AOP_IS_DPTRn(IC_RESULT(ic)))
    {
	return FALSE;
    }
    
    // or, if both dp1 & dp2 are in use and the result needs a dptr, we're out of luck    
    if (dp1InUse && dp2InUse && isOperandInFarSpace(IC_RESULT(ic)))
    {
	return FALSE;
    }

    aopOp (IC_RESULT(ic),ic,TRUE, dp1InUse);

    // Some sanity checking...
    if (dp1InUse && AOP_USESDPTR(IC_RESULT(ic)))
    {
	fprintf(stderr,
		"Internal error: got unexpected DPTR (%s:%d %s:%d)\n",
		__FILE__, __LINE__, ic->filename, ic->lineno);  
	emitcode(";", ">>> unexpected DPTR here.");
    }
    
    if (dp2InUse && AOP_USESDPTR2(IC_RESULT(ic)))
    {
	fprintf(stderr,
		"Internal error: got unexpected DPTR2 (%s:%d %s:%d)\n",
		__FILE__, __LINE__, ic->filename, ic->lineno);  
	emitcode(";", ">>> unexpected DPTR2 here.");
    }    
    
    return TRUE;
}

// Macro to aopOp all three operands of an ic. If this cannot be done, 
// the IC_LEFT and IC_RIGHT operands will be aopOp'd, and the rc parameter
// will be set TRUE. The caller must then handle the case specially, noting
// that the IC_RESULT operand is not aopOp'd.
// 
#define AOP_OP_3_NOFATAL(ic, rc) \
	    do { rc = !aopOp3(ic); } while (0)

// aopOp the left & right operands of an ic.
#define AOP_OP_2(ic) \
    aopOp (IC_RIGHT(ic),ic,FALSE, AOP_IS_STR(IC_LEFT(ic))); \
    aopOp (IC_LEFT(ic),ic,FALSE, AOP_USESDPTR(IC_RIGHT(ic)));

// convienience macro.
#define AOP_SET_LOCALS(ic) \
    left = IC_LEFT(ic); \
    right = IC_RIGHT(ic); \
    result = IC_RESULT(ic);


// Given an integer value of pushedSize bytes on the stack,
// adjust it to be resultSize bytes, either by discarding
// the most significant bytes or by zero-padding.
//
// On exit from this macro, pushedSize will have been adjusted to
// equal resultSize, and ACC may be trashed.
#define ADJUST_PUSHED_RESULT(pushedSize, resultSize) 		\
      /* If the pushed data is bigger than the result,		\
       * simply discard unused bytes. Icky, but works.		\
       */							\
      while (pushedSize > resultSize)				\
      {								\
	  D (emitcode (";", "discarding unused result byte."););\
	  emitcode ("pop", "acc");				\
	  pushedSize--;						\
      }								\
      if (pushedSize < resultSize)				\
      {								\
	  emitcode ("clr", "a");				\
	  /* Conversly, we haven't pushed enough here.		\
	   * just zero-pad, and all is well.			\
	   */							\
	  while (pushedSize < resultSize)			\
	  {							\
	      emitcode("push", "acc");				\
	      pushedSize++;					\
	  }							\
      }								\
      assert(pushedSize == resultSize);

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void
genPlus (iCode * ic)
{
  int size, offset = 0;
  bool pushResult;
  int rSize;

  D (emitcode (";", "genPlus "););

  /* special cases :- */
  if ( AOP_IS_STR(IC_LEFT(ic)) &&
      isOperandLiteral(IC_RIGHT(ic)) && OP_SYMBOL(IC_RESULT(ic))->ruonly) {
      aopOp (IC_RIGHT (ic), ic, TRUE, FALSE);
      size = floatFromVal (AOP (IC_RIGHT(ic))->aopu.aop_lit);
      if (size <= 9) {
	  while (size--) emitcode ("inc","dptr");
      } else {
	  emitcode ("mov","a,dpl");
	  emitcode ("add","a,#!constbyte",size & 0xff);
	  emitcode ("mov","dpl,a");
	  emitcode ("mov","a,dph");
	  emitcode ("addc","a,#!constbyte",(size >> 8) & 0xff);
	  emitcode ("mov","dph,a");
	  emitcode ("mov","a,dpx");
	  emitcode ("addc","a,#!constbyte",(size >> 16) & 0xff);
	  emitcode ("mov","dpx,a");
      }
      freeAsmop (IC_RIGHT (ic), NULL, ic, FALSE);
      return ;
  }
  if ( IS_SYMOP(IC_LEFT(ic)) && 
       OP_SYMBOL(IC_LEFT(ic))->remat &&
       isOperandInFarSpace(IC_RIGHT(ic))) {
      operand *op = IC_RIGHT(ic);
      IC_RIGHT(ic) = IC_LEFT(ic);
      IC_LEFT(ic) = op;
  }
		
  AOP_OP_3_NOFATAL (ic, pushResult);
    
  if (pushResult)
    {
      D (emitcode (";", "genPlus: must push result: 3 ops in far space"););
    }

  if (!pushResult)
    {
      /* if literal, literal on the right or
         if left requires ACC or right is already
         in ACC */
      if ((AOP_TYPE (IC_LEFT (ic)) == AOP_LIT)
       || ((AOP_NEEDSACC (IC_LEFT (ic))) && !(AOP_NEEDSACC (IC_RIGHT (ic))))
	  || AOP_TYPE (IC_RIGHT (ic)) == AOP_ACC)
	{
	  operand *t = IC_RIGHT (ic);
	  IC_RIGHT (ic) = IC_LEFT (ic);
	  IC_LEFT (ic) = t;
	  emitcode (";", "Swapped plus args.");
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
	      _startLazyDPSEvaluation ();
	      while (size--)
		{
		  MOVA (aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, NULL));
		  emitcode ("addc", "a,#0");
		  aopPut (AOP (IC_RESULT (ic)), "a", offset++);
		}
	      _endLazyDPSEvaluation ();
	    }
	  goto release;
	}

      /* if I can do an increment instead
         of add then GOOD for ME */
      if (genPlusIncr (ic) == TRUE)
	{
	  emitcode (";", "did genPlusIncr");
	  goto release;
	}

    }
  size = getDataSize (pushResult ? IC_LEFT (ic) : IC_RESULT (ic));

  _startLazyDPSEvaluation ();
  while (size--)
    {
      if (AOP_TYPE(IC_LEFT(ic)) == AOP_ACC && !AOP_NEEDSACC(IC_RIGHT(ic)))
	{
	  MOVA (aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL));
	  if (offset == 0)
	    emitcode ("add", "a,%s",
		 aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, DP2_RESULT_REG));
	  else
	    emitcode ("addc", "a,%s",
		 aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, DP2_RESULT_REG));
	}
      else
	{
	  if (AOP_TYPE(IC_LEFT(ic)) == AOP_ACC && (offset == 0))
	  {
	      /* right is going to use ACC or we would have taken the
	       * above branch.
	       */
	      assert(AOP_NEEDSACC(IC_RIGHT(ic)));
       TR_AP("#3");
	      D(emitcode(";", "+ AOP_ACC special case."););
	      emitcode("xch", "a, %s", DP2_RESULT_REG);
	  }
	  MOVA (aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, NULL));
	  if (offset == 0)
	  {
	    if (AOP_TYPE(IC_LEFT(ic)) == AOP_ACC)
	    {
         TR_AP("#4");
	        emitcode("add", "a, %s", DP2_RESULT_REG); 
	    }
	    else
	    {
	    	emitcode ("add", "a,%s",
			  aopGet (AOP(IC_LEFT(ic)), offset, FALSE, FALSE,
				  DP2_RESULT_REG));
	    }
          }
	  else
	  {
	    emitcode ("addc", "a,%s",
		  aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE,
			  DP2_RESULT_REG));
	  }
	}
      if (!pushResult)
	{
	  aopPut (AOP (IC_RESULT (ic)), "a", offset);
	}
      else
	{
	  emitcode ("push", "acc");
	}
      offset++;
    }
  _endLazyDPSEvaluation ();

  if (pushResult)
    {
      aopOp (IC_RESULT (ic), ic, TRUE, FALSE);

      size = getDataSize (IC_LEFT (ic));
      rSize = getDataSize (IC_RESULT (ic));

      ADJUST_PUSHED_RESULT(size, rSize);

      _startLazyDPSEvaluation ();
      while (size--)
	{
	  emitcode ("pop", "acc");
	  aopPut (AOP (IC_RESULT (ic)), "a", size);
	}
      _endLazyDPSEvaluation ();
    }

  adjustArithmeticResult (ic);

release:
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genMinusDec :- does subtraction with deccrement if possible     */
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

  if (size == 1 && AOP(IC_LEFT(ic)) == AOP(IC_RESULT(ic)) &&
      AOP_TYPE(IC_LEFT(ic)) == AOP_DIR ) {
      while (icount--) {
	  emitcode("dec","%s",aopGet(AOP(IC_RESULT(ic)),0,FALSE,FALSE,NULL));
      }
      return TRUE;
  }
  /* if decrement 16 bits in register */
  if (AOP_TYPE (IC_LEFT (ic)) == AOP_REG &&
      AOP_TYPE (IC_RESULT (ic)) == AOP_REG &&
      sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))) &&
      (size > 1) &&
      (icount == 1))
    {
      symbol *tlbl;
      int    emitTlbl;
      int    labelRange;
      char   *l;

      /* If the next instruction is a goto and the goto target
         * is <= 5 instructions previous to this, we can generate
         * jumps straight to that target.
       */
      if (ic->next && ic->next->op == GOTO
	  && (labelRange = findLabelBackwards (ic, IC_LABEL (ic->next)->key)) != 0
	  && labelRange <= 5)
	{
	  emitcode (";", "tail decrement optimized (range %d)", labelRange);
	  tlbl = IC_LABEL (ic->next);
	  emitTlbl = 0;
	}
      else
	{
	  tlbl = newiTempLabel (NULL);
	  emitTlbl = 1;
	}

      l = aopGet (AOP (IC_RESULT (ic)), LSB, FALSE, FALSE, NULL);
      emitcode ("dec", "%s", l);
 
      if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
	  AOP_TYPE (IC_RESULT (ic)) == AOP_DPTR ||
	  IS_AOP_PREG (IC_RESULT (ic)))
      {	    
	  emitcode ("cjne", "%s,#!constbyte,!tlabel", l, 0xff, tlbl->key + 100);
      }
      else
      {
	  emitcode ("mov", "a,#!constbyte",0xff);
	  emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
      }
      l = aopGet (AOP (IC_RESULT (ic)), MSB16, FALSE, NULL, NULL);
      emitcode ("dec", "%s", l);
      if (size > 2)
	{
	    if (!strcmp(l, "acc"))
	    {
		emitcode("jnz", "!tlabel", tlbl->key + 100);
	    }
	    else if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
		     AOP_TYPE (IC_RESULT (ic)) == AOP_DPTR ||
		     IS_AOP_PREG (IC_RESULT (ic)))
	    {	    
		emitcode ("cjne", "%s,#!constbyte,!tlabel", l, 0xff, tlbl->key + 100);
	    }
	    else
	    {
		emitcode ("mov", "a,#!constbyte",0xff);
		emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
	    }
	    l = aopGet (AOP (IC_RESULT (ic)), MSB24, FALSE, NULL, NULL);
	    emitcode ("dec", "%s", l);
	}
      if (size > 3)
	{
	    if (!strcmp(l, "acc"))
	    {
		emitcode("jnz", "!tlabel", tlbl->key + 100);
	    }
	    else if (AOP_TYPE (IC_RESULT (ic)) == AOP_REG ||
		     AOP_TYPE (IC_RESULT (ic)) == AOP_DPTR ||
		     IS_AOP_PREG (IC_RESULT (ic)))
	    {	    
		emitcode ("cjne", "%s,#!constbyte,!tlabel", l, 0xff, tlbl->key + 100);
	    }
	    else
	    {
		emitcode ("mov", "a,#!constbyte",0xff);
		emitcode ("cjne", "a,%s,!tlabel", l, tlbl->key + 100);
	    }	    
	    l = aopGet (AOP (IC_RESULT (ic)), MSB32, FALSE, NULL, NULL);
	    emitcode ("dec", "%s", l);
	}
      if (emitTlbl)
	{
	  emitcode ("", "!tlabeldef", tlbl->key + 100);
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
  if (
       AOP_TYPE (IC_LEFT (ic)) == AOP_REG &&
       AOP_TYPE (IC_RESULT (ic)) == AOP_REG &&
       sameRegs (AOP (IC_LEFT (ic)), AOP (IC_RESULT (ic))))
    {

      _startLazyDPSEvaluation ();
      while (icount--)
	{
	  emitcode ("dec", "%s",
		    aopGet (AOP (IC_RESULT (ic)), 0, FALSE, FALSE, NULL));
	}
      _endLazyDPSEvaluation ();

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
      _startLazyDPSEvaluation();
      if (sign)
	{
	  emitcode ("rlc", "a");
	  emitcode ("subb", "a,acc");
	  while (size--)
	  {
	    aopPut (AOP (result), "a", offset++);
	  }
	}
      else
      {
	while (size--)
	{
	  aopPut (AOP (result), zero, offset++);
	}
      }
      _endLazyDPSEvaluation();
    }
}

/*-----------------------------------------------------------------*/
/* genMinusBits - generates code for subtraction  of two bits      */
/*-----------------------------------------------------------------*/
static void
genMinusBits (iCode * ic)
{
  symbol *lbl = newiTempLabel (NULL);

  D (emitcode (";", "genMinusBits "););

  if (AOP_TYPE (IC_RESULT (ic)) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (IC_LEFT (ic))->aopu.aop_dir);
      emitcode ("jnb", "%s,!tlabel", AOP (IC_RIGHT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("cpl", "c");
      emitcode ("", "!tlabeldef", (lbl->key + 100));
      outBitC (IC_RESULT (ic));
    }
  else
    {
      emitcode ("mov", "c,%s", AOP (IC_RIGHT (ic))->aopu.aop_dir);
      emitcode ("subb", "a,acc");
      emitcode ("jnb", "%s,!tlabel", AOP (IC_LEFT (ic))->aopu.aop_dir, (lbl->key + 100));
      emitcode ("inc", "a");
      emitcode ("", "!tlabeldef", (lbl->key + 100));
      aopPut (AOP (IC_RESULT (ic)), "a", 0);
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
    int rSize;
    long lit = 0L;
    bool pushResult;

    D (emitcode (";", "genMinus "););

    AOP_OP_3_NOFATAL(ic, pushResult);	

    if (!pushResult)
    {
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

    }

  size = getDataSize (pushResult ? IC_LEFT (ic) : IC_RESULT (ic));

  if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT)
    {
      CLRC;
    }
  else
    {
      lit = (long) floatFromVal (AOP (IC_RIGHT (ic))->aopu.aop_lit);
      lit = -lit;
    }


  /* if literal, add a,#-lit, else normal subb */
  _startLazyDPSEvaluation ();
  while (size--) {
      if (AOP_TYPE (IC_RIGHT (ic)) != AOP_LIT) {
	  if (AOP_USESDPTR(IC_RIGHT(ic))) {
	      emitcode ("mov","b,%s",
			aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, NULL));
	      MOVA(aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL));
	      emitcode ("subb","a,b");
	  } else {
	      MOVA (aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL));
	      emitcode ("subb", "a,%s",
			aopGet (AOP (IC_RIGHT (ic)), offset, FALSE, FALSE, 
				DP2_RESULT_REG));
	  }
      } else {
	  MOVA (aopGet (AOP (IC_LEFT (ic)), offset, FALSE, FALSE, NULL));
	  /* first add without previous c */
	  if (!offset) {
	      if (!size && lit==-1) {
		  emitcode ("dec", "a");
	      } else {
		  emitcode ("add", "a,#!constbyte",
			    (unsigned int) (lit & 0x0FFL));
	      }
	  } else {
	      emitcode ("addc", "a,#!constbyte",
			(unsigned int) ((lit >> (offset * 8)) & 0x0FFL));
	  }
      }
      
      if (pushResult) {
	  emitcode ("push", "acc");
      } else {
	  aopPut (AOP (IC_RESULT (ic)), "a", offset);
      }
      offset++;
  }
  _endLazyDPSEvaluation ();
  
  if (pushResult)
    {
      aopOp (IC_RESULT (ic), ic, TRUE, FALSE);

      size = getDataSize (IC_LEFT (ic));
      rSize = getDataSize (IC_RESULT (ic));

      ADJUST_PUSHED_RESULT(size, rSize);

      _startLazyDPSEvaluation ();
      while (size--)
	{
	  emitcode ("pop", "acc");
	  aopPut (AOP (IC_RESULT (ic)), "a", size);
	}
      _endLazyDPSEvaluation ();
    }

  adjustArithmeticResult (ic);

release:
  freeAsmop (IC_LEFT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (IC_RIGHT (ic), NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}


/*-----------------------------------------------------------------*/
/* genMultbits :- multiplication of bits                           */
/*-----------------------------------------------------------------*/
static void
genMultbits (operand * left,
	     operand * right,
	     operand * result,
	     iCode   * ic)
{
  emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
  emitcode ("anl", "c,%s", AOP (right)->aopu.aop_dir);
  aopOp(result, ic, TRUE, FALSE);
  outBitC (result);
}


/*-----------------------------------------------------------------*/
/* genMultOneByte : 8*8=8/16 bit multiplication                    */
/*-----------------------------------------------------------------*/
static void
genMultOneByte (operand * left,
		operand * right,
		operand * result,
		iCode   * ic)
{
  sym_link *opetype = operandType (result);
  symbol *lbl;


  /* (if two literals: the value is computed before) */
  /* if one literal, literal on the right */
  if (AOP_TYPE (left) == AOP_LIT)
    {
      operand *t = right;
      right = left;
      left = t;
      emitcode (";", "swapped left and right");
    }

  if (SPEC_USIGN(opetype)
      // ignore the sign of left and right, what else can we do?
      || (SPEC_USIGN(operandType(left)) && 
	  SPEC_USIGN(operandType(right)))) {
    // just an unsigned 8*8=8/16 multiply
    //emitcode (";","unsigned");
    emitcode ("mov", "b,%s", aopGet (AOP (right), 0, FALSE, FALSE, NULL));
    MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));
    emitcode ("mul", "ab");
   
    _G.accInUse++; _G.bInUse++;
    aopOp(result, ic, TRUE, FALSE);
      
      if (AOP_SIZE(result)<1 || AOP_SIZE(result)>2) 
      {
	  // this should never happen
	  fprintf (stderr, "size!=1||2 (%d) in %s at line:%d \n", 
		   AOP_SIZE(result), __FILE__, lineno);
	  exit (1);
      }      
      
    aopPut (AOP (result), "a", 0);
    _G.accInUse--; _G.bInUse--;
    if (AOP_SIZE(result)==2) 
    {
      aopPut (AOP (result), "b", 1);
    }
    return;
  }

  // we have to do a signed multiply

  emitcode (";", "signed");
  emitcode ("clr", "F0"); // reset sign flag
  MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));

  lbl=newiTempLabel(NULL);
  emitcode ("jnb", "acc.7,!tlabel",  lbl->key+100);
  // left side is negative, 8-bit two's complement, this fails for -128
  emitcode ("setb", "F0"); // set sign flag
  emitcode ("cpl", "a");
  emitcode ("inc", "a");

  emitcode ("", "!tlabeldef", lbl->key+100);

  /* if literal */
  if (AOP_TYPE(right)==AOP_LIT) {
    signed char val=floatFromVal (AOP (right)->aopu.aop_lit);
    /* AND literal negative */
    if ((int) val < 0) {
      emitcode ("cpl", "F0"); // complement sign flag
      emitcode ("mov", "b,#!constbyte", -val);
    } else {
      emitcode ("mov", "b,#!constbyte", val);
    }
  } else {
    lbl=newiTempLabel(NULL);
    emitcode ("mov", "b,a");
    emitcode ("mov", "a,%s", aopGet (AOP (right), 0, FALSE, FALSE, NULL));
    emitcode ("jnb", "acc.7,!tlabel", lbl->key+100);
    // right side is negative, 8-bit two's complement
    emitcode ("cpl", "F0"); // complement sign flag
    emitcode ("cpl", "a");
    emitcode ("inc", "a");
    emitcode ("", "!tlabeldef", lbl->key+100);
  }
  emitcode ("mul", "ab");
    
  _G.accInUse++;_G.bInUse++;
  aopOp(result, ic, TRUE, FALSE);
    
  if (AOP_SIZE(result)<1 || AOP_SIZE(result)>2) 
  {
    // this should never happen
      fprintf (stderr, "size!=1||2 (%d) in %s at line:%d \n", 
	       AOP_SIZE(result), __FILE__, lineno);
      exit (1);
  }    
    
  lbl=newiTempLabel(NULL);
  emitcode ("jnb", "F0,!tlabel", lbl->key+100);
  // only ONE op was negative, we have to do a 8/16-bit two's complement
  emitcode ("cpl", "a"); // lsb
  if (AOP_SIZE(result)==1) {
    emitcode ("inc", "a");
  } else {
    emitcode ("add", "a,#1");
    emitcode ("xch", "a,b");
    emitcode ("cpl", "a"); // msb
    emitcode ("addc", "a,#0");
    emitcode ("xch", "a,b");
  }

  emitcode ("", "!tlabeldef", lbl->key+100);
  aopPut (AOP (result), "a", 0);
  _G.accInUse--;_G.bInUse--;
  if (AOP_SIZE(result)==2) {
    aopPut (AOP (result), "b", 1);
  }
}

/*-----------------------------------------------------------------*/
/* genMultTwoByte - use the DS390 MAC unit to do 16*16 multiply    */
/*-----------------------------------------------------------------*/
static void genMultTwoByte (operand *left, operand *right, 
			    operand *result, iCode *ic)
{
	sym_link *retype = getSpec(operandType(right));
	sym_link *letype = getSpec(operandType(left));
	int umult = SPEC_USIGN(retype) | SPEC_USIGN(letype);
	symbol *lbl;

	if (AOP_TYPE (left) == AOP_LIT) {
		operand *t = right;
		right = left;
		left = t;
	}
	/* save EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("setb","F1");
	emitcode ("jbc","EA,!tlabel",lbl->key+100);
	emitcode ("clr","F1");
	emitcode("","!tlabeldef",lbl->key+100);

	/* load up MB with right */
	if (!umult) {
		emitcode("clr","F0");
		if (AOP_TYPE(right) == AOP_LIT) {
			int val=floatFromVal (AOP (right)->aopu.aop_lit);
			if (val < 0) {
				emitcode("setb","F0");
				val = -val;
			}
			emitcode ("mov","mb,#!constbyte",val & 0xff);
			emitcode ("mov","mb,#!constbyte",(val >> 8) & 0xff);		    
		} else {
			lbl = newiTempLabel(NULL);
			emitcode ("mov","b,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
			emitcode ("mov","a,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
			emitcode ("jnb","acc.7,!tlabel",lbl->key+100);		
			emitcode ("xch", "a,b");
			emitcode ("cpl","a");
			emitcode ("add", "a,#1");
			emitcode ("xch", "a,b");
			emitcode ("cpl", "a"); // msb
			emitcode ("addc", "a,#0");
			emitcode ("setb","F0");
			emitcode ("","!tlabeldef",lbl->key+100);
			emitcode ("mov","mb,b");
			emitcode ("mov","mb,a");
		}
	} else {
		emitcode ("mov","mb,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
		emitcode ("mov","mb,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
	}
	/* load up MA with left */
	if (!umult) {
		lbl = newiTempLabel(NULL);
		emitcode ("mov","b,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","a,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
		emitcode ("jnb","acc.7,!tlabel",lbl->key+100);
		emitcode ("xch", "a,b");
		emitcode ("cpl","a");
		emitcode ("add", "a,#1");
		emitcode ("xch", "a,b");
		emitcode ("cpl", "a"); // msb
		emitcode ("addc","a,#0");
		emitcode ("jbc","F0,!tlabel",lbl->key+100);
		emitcode ("setb","F0");
		emitcode ("","!tlabeldef",lbl->key+100);
		emitcode ("mov","ma,b");
		emitcode ("mov","ma,a");
	} else {
		emitcode ("mov","ma,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","ma,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
	}
	/* wait for multiplication to finish */
	lbl = newiTempLabel(NULL);
	emitcode("","!tlabeldef", lbl->key+100);
	emitcode("mov","a,mcnt1");
	emitcode("anl","a,#!constbyte",0x80);
	emitcode("jnz","!tlabel",lbl->key+100);
	
	freeAsmop (left, NULL, ic, TRUE);
	freeAsmop (right, NULL, ic,TRUE);
	aopOp(result, ic, TRUE, FALSE);

	/* if unsigned then simple */	
	if (umult) {
		emitcode ("mov","a,ma");
		if (AOP_SIZE(result) >= 4) aopPut(AOP(result),"a",3);
		emitcode ("mov","a,ma");
		if (AOP_SIZE(result) >= 3) aopPut(AOP(result),"a",2);
		aopPut(AOP(result),"ma",1);
		aopPut(AOP(result),"ma",0);
	} else {
		emitcode("push","ma");
		emitcode("push","ma");
		emitcode("push","ma");
		MOVA("ma");
		/* negate result if needed */
		lbl = newiTempLabel(NULL); 	
		emitcode("jnb","F0,!tlabel",lbl->key+100);
		emitcode("cpl","a");
		emitcode("add","a,#1");
		emitcode("","!tlabeldef", lbl->key+100);
	        if (AOP_TYPE(result) == AOP_ACC)
	        {
		    D(emitcode(";", "ACC special case."););
		    /* We know result is the only live aop, and 
		     * it's obviously not a DPTR2, so AP is available.
		     */
		    emitcode("mov", "%s,acc", DP2_RESULT_REG);
	        }
	        else
	        {
		    aopPut(AOP(result),"a",0);
		}
	    
		emitcode("pop","acc");
		lbl = newiTempLabel(NULL); 	
		emitcode("jnb","F0,!tlabel",lbl->key+100);
		emitcode("cpl","a");
		emitcode("addc","a,#0");
		emitcode("","!tlabeldef", lbl->key+100);
		aopPut(AOP(result),"a",1);
		emitcode("pop","acc");
		if (AOP_SIZE(result) >= 3) {
			lbl = newiTempLabel(NULL); 	
			emitcode("jnb","F0,!tlabel",lbl->key+100);
			emitcode("cpl","a");
			emitcode("addc","a,#0");			
			emitcode("","!tlabeldef", lbl->key+100);
			aopPut(AOP(result),"a",2);
		}
		emitcode("pop","acc");
		if (AOP_SIZE(result) >= 4) {
			lbl = newiTempLabel(NULL); 	
			emitcode("jnb","F0,!tlabel",lbl->key+100);
			emitcode("cpl","a");
			emitcode("addc","a,#0");			
			emitcode("","!tlabeldef", lbl->key+100);
			aopPut(AOP(result),"a",3);
		}
	        if (AOP_TYPE(result) == AOP_ACC)
	        {
		    /* We stashed the result away above. */
		    emitcode("mov", "acc,%s", DP2_RESULT_REG);
	        }	    
		
	}
	freeAsmop (result, NULL, ic, TRUE);

	/* restore EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("jnb","F1,!tlabel",lbl->key+100);
	emitcode ("setb","EA");
	emitcode("","!tlabeldef",lbl->key+100);
	return ;
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

  D (emitcode (";", "genMult "););

  /* assign the amsops */
  AOP_OP_2 (ic);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genMultbits (left, right, result, ic);
      goto release;
    }

  /* if both are of size == 1 */
  if (AOP_SIZE (left) == 1 &&
      AOP_SIZE (right) == 1)
    {
      genMultOneByte (left, right, result, ic);
      goto release;
    }

  if (AOP_SIZE (left) == 2 && AOP_SIZE(right) == 2) {
	  /* use the ds390 ARITHMETIC accel UNIT */
	  genMultTwoByte (left, right, result, ic);
	  return ;
  }
  /* should have been converted to function call */
  assert (0);

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genDivbits :- division of bits                                  */
/*-----------------------------------------------------------------*/
static void
genDivbits (operand * left,
	    operand * right,
	    operand * result,
	    iCode   * ic)
{

  char *l;

  /* the result must be bit */
  LOAD_AB_FOR_DIV (left, right, l);
  emitcode ("div", "ab");
  emitcode ("rrc", "a");
  aopOp(result, ic, TRUE, FALSE);
    
  aopPut (AOP (result), "c", 0);
}

/*-----------------------------------------------------------------*/
/* genDivOneByte : 8 bit division                                  */
/*-----------------------------------------------------------------*/
static void
genDivOneByte (operand * left,
	       operand * right,
	       operand * result,
	       iCode   * ic)
{
  sym_link *opetype = operandType (result);
  char *l;
  symbol *lbl;
  int size, offset;

  offset = 1;
  /* signed or unsigned */
  if (SPEC_USIGN (opetype))
    {
	/* unsigned is easy */
	LOAD_AB_FOR_DIV (left, right, l);
	emitcode ("div", "ab");

	_G.accInUse++;
	aopOp(result, ic, TRUE, FALSE);
	aopPut (AOP (result), "a", 0);
	_G.accInUse--;

	size = AOP_SIZE (result) - 1;
	
	while (size--)
	{
	    aopPut (AOP (result), zero, offset++);
	}
      return;
    }

  /* signed is a little bit more difficult */

  /* save the signs of the operands */
  MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));
  emitcode ("xrl", "a,%s", aopGet (AOP (right), 0, FALSE, TRUE, FALSE));
  emitcode ("push", "acc");	/* save it on the stack */

  /* now sign adjust for both left & right */
  MOVA (aopGet (AOP (right), 0, FALSE, FALSE, NULL));
  lbl = newiTempLabel (NULL);
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  emitcode ("cpl", "a");
  emitcode ("inc", "a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));
  emitcode ("mov", "b,a");

  /* sign adjust left side */
  MOVA( aopGet (AOP (left), 0, FALSE, FALSE, NULL));

  lbl = newiTempLabel (NULL);
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  emitcode ("cpl", "a");
  emitcode ("inc", "a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));

  /* now the division */
  emitcode ("nop", "; workaround for DS80C390 div bug.");
  emitcode ("div", "ab");
  /* we are interested in the lower order
     only */
  emitcode ("mov", "b,a");
  lbl = newiTempLabel (NULL);
  emitcode ("pop", "acc");
  /* if there was an over flow we don't
     adjust the sign of the result */
  emitcode ("jb", "ov,!tlabel", (lbl->key + 100));
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  CLRC;
  emitcode ("clr", "a");
  emitcode ("subb", "a,b");
  emitcode ("mov", "b,a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));

  /* now we are done */
  _G.accInUse++;     _G.bInUse++;
    aopOp(result, ic, TRUE, FALSE);
    
    aopPut (AOP (result), "b", 0);
    
    size = AOP_SIZE (result) - 1;
    
    if (size > 0)
    {
      emitcode ("mov", "c,b.7");
      emitcode ("subb", "a,acc");
    }
    while (size--)
    {
	aopPut (AOP (result), "a", offset++);
    }
    _G.accInUse--;     _G.bInUse--;

}

/*-----------------------------------------------------------------*/
/* genDivTwoByte - use the DS390 MAC unit to do 16/16 divide       */
/*-----------------------------------------------------------------*/
static void genDivTwoByte (operand *left, operand *right, 
			    operand *result, iCode *ic)
{
	sym_link *retype = getSpec(operandType(right));
	sym_link *letype = getSpec(operandType(left));
	int umult = SPEC_USIGN(retype) | SPEC_USIGN(letype);
	symbol *lbl;

	/* save EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("setb","F1");
	emitcode ("jbc","EA,!tlabel",lbl->key+100);
	emitcode ("clr","F1");
	emitcode("","!tlabeldef",lbl->key+100);

	/* load up MA with left */
	if (!umult) {
		emitcode("clr","F0");
		lbl = newiTempLabel(NULL);
		emitcode ("mov","b,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","a,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
		emitcode ("jnb","acc.7,!tlabel",lbl->key+100);
		emitcode ("xch", "a,b");
		emitcode ("cpl","a");
		emitcode ("add", "a,#1");
		emitcode ("xch", "a,b");
		emitcode ("cpl", "a"); // msb
		emitcode ("addc","a,#0");
		emitcode ("setb","F0");
		emitcode ("","!tlabeldef",lbl->key+100);
		emitcode ("mov","ma,b");
		emitcode ("mov","ma,a");
	} else {
		emitcode ("mov","ma,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","ma,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
	}

	/* load up MB with right */
	if (!umult) {
		if (AOP_TYPE(right) == AOP_LIT) {
			int val=floatFromVal (AOP (right)->aopu.aop_lit);
			if (val < 0) {
				lbl = newiTempLabel(NULL);
				emitcode ("jbc","F0,!tlabel",lbl->key+100);
				emitcode("setb","F0");
				emitcode ("","!tlabeldef",lbl->key+100);
				val = -val;
			} 
			emitcode ("mov","mb,#!constbyte",val & 0xff);		    
			emitcode ("mov","mb,#!constbyte",(val >> 8) & 0xff);
		} else {
			lbl = newiTempLabel(NULL);
			emitcode ("mov","b,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
			emitcode ("mov","a,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
			emitcode ("jnb","acc.7,!tlabel",lbl->key+100);		
			emitcode ("xch", "a,b");
			emitcode ("cpl","a");
			emitcode ("add", "a,#1");
			emitcode ("xch", "a,b");
			emitcode ("cpl", "a"); // msb
			emitcode ("addc", "a,#0");
			emitcode ("jbc","F0,!tlabel",lbl->key+100);
			emitcode ("setb","F0");
			emitcode ("","!tlabeldef",lbl->key+100);
			emitcode ("mov","mb,b");
			emitcode ("mov","mb,a");
		}
	} else {
		emitcode ("mov","mb,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
		emitcode ("mov","mb,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
	}

	/* wait for multiplication to finish */
	lbl = newiTempLabel(NULL);
	emitcode("","!tlabeldef", lbl->key+100);
	emitcode("mov","a,mcnt1");
	emitcode("anl","a,#!constbyte",0x80);
	emitcode("jnz","!tlabel",lbl->key+100);
	
	freeAsmop (left, NULL, ic, TRUE);
	freeAsmop (right, NULL, ic,TRUE);
	aopOp(result, ic, TRUE, FALSE);

	/* if unsigned then simple */	
	if (umult) {
		aopPut(AOP(result),"ma",1);
		aopPut(AOP(result),"ma",0);
	} else {
		emitcode("push","ma");
		MOVA("ma");
		/* negate result if needed */
		lbl = newiTempLabel(NULL); 	
		emitcode("jnb","F0,!tlabel",lbl->key+100);
		emitcode("cpl","a");
		emitcode("add","a,#1");
		emitcode("","!tlabeldef", lbl->key+100);
		aopPut(AOP(result),"a",0);
		emitcode("pop","acc");
		lbl = newiTempLabel(NULL); 	
		emitcode("jnb","F0,!tlabel",lbl->key+100);
		emitcode("cpl","a");
		emitcode("addc","a,#0");
		emitcode("","!tlabeldef", lbl->key+100);
		aopPut(AOP(result),"a",1);
	}
	freeAsmop (result, NULL, ic, TRUE);
	/* restore EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("jnb","F1,!tlabel",lbl->key+100);
	emitcode ("setb","EA");
	emitcode("","!tlabeldef",lbl->key+100);
	return ;
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

  D (emitcode (";", "genDiv "););

  /* assign the amsops */
  AOP_OP_2 (ic);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genDivbits (left, right, result, ic);
      goto release;
    }

  /* if both are of size == 1 */
  if (AOP_SIZE (left) == 1 &&
      AOP_SIZE (right) == 1)
    {
      genDivOneByte (left, right, result, ic);
      goto release;
    }

  if (AOP_SIZE (left) == 2 && AOP_SIZE(right) == 2) {
	  /* use the ds390 ARITHMETIC accel UNIT */
	  genDivTwoByte (left, right, result, ic);
	  return ;
  }
  /* should have been converted to function call */
  assert (0);
release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genModbits :- modulus of bits                                   */
/*-----------------------------------------------------------------*/
static void
genModbits (operand * left,
	    operand * right,
	    operand * result,
	    iCode   * ic)
{

  char *l;

  /* the result must be bit */
  LOAD_AB_FOR_DIV (left, right, l);
  emitcode ("div", "ab");
  emitcode ("mov", "a,b");
  emitcode ("rrc", "a");
  aopOp(result, ic, TRUE, FALSE);
  aopPut (AOP (result), "c", 0);
}

/*-----------------------------------------------------------------*/
/* genModOneByte : 8 bit modulus                                   */
/*-----------------------------------------------------------------*/
static void
genModOneByte (operand * left,
	       operand * right,
	       operand * result,
	       iCode   * ic)
{
  sym_link *opetype = operandType (result);
  char *l;
  symbol *lbl;

  /* signed or unsigned */
  if (SPEC_USIGN (opetype))
    {
      /* unsigned is easy */
      LOAD_AB_FOR_DIV (left, right, l);
      emitcode ("div", "ab");
      aopOp(result, ic, TRUE, FALSE);	
      aopPut (AOP (result), "b", 0);
      return;
    }

  /* signed is a little bit more difficult */

  /* save the signs of the operands */
  MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));

  emitcode ("xrl", "a,%s", aopGet (AOP (right), 0, FALSE, FALSE, FALSE));
  emitcode ("push", "acc");	/* save it on the stack */

  /* now sign adjust for both left & right */
  MOVA (aopGet (AOP (right), 0, FALSE, FALSE, NULL));

  lbl = newiTempLabel (NULL);
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  emitcode ("cpl", "a");
  emitcode ("inc", "a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));
  emitcode ("mov", "b,a");

  /* sign adjust left side */
  MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));

  lbl = newiTempLabel (NULL);
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  emitcode ("cpl", "a");
  emitcode ("inc", "a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));

  /* now the multiplication */
  emitcode ("nop", "; workaround for DS80C390 div bug.");
  emitcode ("div", "ab");
  /* we are interested in the lower order
     only */
  lbl = newiTempLabel (NULL);
  emitcode ("pop", "acc");
  /* if there was an over flow we don't
     adjust the sign of the result */
  emitcode ("jb", "ov,!tlabel", (lbl->key + 100));
  emitcode ("jnb", "acc.7,!tlabel", (lbl->key + 100));
  CLRC;
  emitcode ("clr", "a");
  emitcode ("subb", "a,b");
  emitcode ("mov", "b,a");
  emitcode ("", "!tlabeldef", (lbl->key + 100));
  
  _G.bInUse++;
  /* now we are done */
  aopOp(result, ic, TRUE, FALSE);    
  aopPut (AOP (result), "b", 0);
  _G.bInUse--;

}

/*-----------------------------------------------------------------*/
/* genModTwoByte - use the DS390 MAC unit to do 16%16 modulus      */
/*-----------------------------------------------------------------*/
static void genModTwoByte (operand *left, operand *right, 
			    operand *result, iCode *ic)
{
	sym_link *retype = getSpec(operandType(right));
	sym_link *letype = getSpec(operandType(left));
	int umult = SPEC_USIGN(retype) | SPEC_USIGN(letype);
	symbol *lbl;

	/* load up MA with left */
	/* save EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("setb","F1");
	emitcode ("jbc","EA,!tlabel",lbl->key+100);
	emitcode ("clr","F1");
	emitcode("","!tlabeldef",lbl->key+100);

	if (!umult) {
		lbl = newiTempLabel(NULL);
		emitcode ("mov","b,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","a,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
		emitcode ("jnb","acc.7,!tlabel",lbl->key+100);
		emitcode ("xch", "a,b");
		emitcode ("cpl","a");
		emitcode ("add", "a,#1");
		emitcode ("xch", "a,b");
		emitcode ("cpl", "a"); // msb
		emitcode ("addc","a,#0");
		emitcode ("","!tlabeldef",lbl->key+100);
		emitcode ("mov","ma,b");
		emitcode ("mov","ma,a");
	} else {
		emitcode ("mov","ma,%s",aopGet(AOP(left),0,FALSE,FALSE,NULL));
		emitcode ("mov","ma,%s",aopGet(AOP(left),1,FALSE,FALSE,NULL));
	}

	/* load up MB with right */
	if (!umult) {
		if (AOP_TYPE(right) == AOP_LIT) {
			int val=floatFromVal (AOP (right)->aopu.aop_lit);
			if (val < 0) {
				val = -val;
			} 
			emitcode ("mov","mb,#!constbyte",val & 0xff);
			emitcode ("mov","mb,#!constbyte",(val >> 8) & 0xff);		    
		} else {
			lbl = newiTempLabel(NULL);
			emitcode ("mov","b,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
			emitcode ("mov","a,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
			emitcode ("jnb","acc.7,!tlabel",lbl->key+100);		
			emitcode ("xch", "a,b");
			emitcode ("cpl","a");
			emitcode ("add", "a,#1");
			emitcode ("xch", "a,b");
			emitcode ("cpl", "a"); // msb
			emitcode ("addc", "a,#0");
			emitcode ("","!tlabeldef",lbl->key+100);
			emitcode ("mov","mb,b");
			emitcode ("mov","mb,a");
		}
	} else {
		emitcode ("mov","mb,%s",aopGet(AOP(right),0,FALSE,FALSE,NULL));
		emitcode ("mov","mb,%s",aopGet(AOP(right),1,FALSE,FALSE,NULL));
	}

	/* wait for multiplication to finish */
	lbl = newiTempLabel(NULL);
	emitcode("","!tlabeldef", lbl->key+100);
	emitcode("mov","a,mcnt1");
	emitcode("anl","a,#!constbyte",0x80);
	emitcode("jnz","!tlabel",lbl->key+100);
	
	freeAsmop (left, NULL, ic, TRUE);
	freeAsmop (right, NULL, ic,TRUE);
	aopOp(result, ic, TRUE, FALSE);

	aopPut(AOP(result),"mb",1);
	aopPut(AOP(result),"mb",0);
	freeAsmop (result, NULL, ic, TRUE);

	/* restore EA bit in F1 */
	lbl = newiTempLabel(NULL);
	emitcode ("jnb","F1,!tlabel",lbl->key+100);
	emitcode ("setb","EA");
	emitcode("","!tlabeldef",lbl->key+100);
	return ;
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

  D (emitcode (";", "genMod "); );

  /* assign the amsops */
  AOP_OP_2 (ic);

  /* special cases first */
  /* both are bits */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      genModbits (left, right, result, ic);
      goto release;
    }

  /* if both are of size == 1 */
  if (AOP_SIZE (left) == 1 &&
      AOP_SIZE (right) == 1)
    {
      genModOneByte (left, right, result, ic);
      goto release;
    }

  if (AOP_SIZE (left) == 2 && AOP_SIZE(right) == 2) {
	  /* use the ds390 ARITHMETIC accel UNIT */
	  genModTwoByte (left, right, result, ic);
	  return ;
  }

  /* should have been converted to function call */
  assert (0);

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genIfxJump :- will create a jump depending on the ifx           */
/*-----------------------------------------------------------------*/
static void
genIfxJump (iCode * ic, char *jval)
{
  symbol *jlbl;
  symbol *tlbl = newiTempLabel (NULL);
  char *inst;

  D (emitcode (";", "genIfxJump"););

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
    emitcode (inst, "%s,!tlabel", jval, (tlbl->key + 100));
  else
    emitcode (inst, "!tlabel", tlbl->key + 100);
  emitcode ("ljmp", "!tlabel", jlbl->key + 100);
  emitcode ("", "!tlabeldef", tlbl->key + 100);

  /* mark the icode as generated */
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genCmp :- greater or less than comparison                       */
/*-----------------------------------------------------------------*/
static void
genCmp (operand * left, operand * right,
	iCode * ic, iCode * ifx, int sign)
{
  int size, offset = 0;
  unsigned long lit = 0L;
  operand *result;

  D (emitcode (";", "genCmp"););

  result = IC_RESULT (ic);

  /* if left & right are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
      emitcode ("anl", "c,/%s", AOP (left)->aopu.aop_dir);
    }
  else
    {
      /* subtract right from left if at the
         end the carry flag is set then we know that
         left is greater than right */
      size = max (AOP_SIZE (left), AOP_SIZE (right));

      /* if unsigned char cmp with lit, do cjne left,#right,zz */
      if ((size == 1) && !sign 
	  && (AOP_TYPE (right) == AOP_LIT && AOP_TYPE (left) != AOP_DIR && AOP_TYPE (left) != AOP_STR))
	{
	  symbol *lbl = newiTempLabel (NULL);
	  emitcode ("cjne", "%s,%s,!tlabel",
		    aopGet (AOP (left), offset, FALSE, FALSE, NULL),
		    aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		    lbl->key + 100);
	  emitcode ("", "!tlabeldef", lbl->key + 100);
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
		      MOVA (aopGet (AOP (left), AOP_SIZE (left) - 1, FALSE, FALSE, NULL));

		      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
		      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));

		      aopOp (result, ic, FALSE, FALSE);

		      if (!(AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result)) && ifx)
			{
			  freeAsmop (result, NULL, ic, TRUE);
			  genIfxJump (ifx, "acc.7");
			  return;
			}
		      else
			{
			  emitcode ("rlc", "a");
			}
		      goto release_freedLR;
		    }
		  goto release;
		}
	    }
	  CLRC;
	  while (size--)
	    {
	      //emitcode (";", "genCmp #1: %d/%d/%d", size, sign, offset);
	      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
	      //emitcode (";", "genCmp #2");
	      if (sign && (size == 0))
		{
		    //emitcode (";", "genCmp #3");
		  emitcode ("xrl", "a,#!constbyte",0x80);
		  if (AOP_TYPE (right) == AOP_LIT)
		    {
		      unsigned long lit = (unsigned long)
		      floatFromVal (AOP (right)->aopu.aop_lit);
		      //emitcode (";", "genCmp #3.1");
		      emitcode ("subb", "a,#!constbyte",
				0x80 ^ (unsigned int) ((lit >> (offset * 8)) & 0x0FFL));
		    }
		  else
		    {
		      //emitcode (";", "genCmp #3.2");
		      if (AOP_NEEDSACC (right) && AOP_TYPE(right) != AOP_DPTR2)
			{
			  emitcode ("push", "acc");
			}
			
		      MOVB(aopGet (AOP (right), offset++, FALSE, FALSE, "b"));
		      emitcode ("xrl", "b,#!constbyte",0x80);
			
		      if (AOP_NEEDSACC (right) && AOP_TYPE(right) != AOP_DPTR2)
			{
			  emitcode ("pop", "acc");
			}
		      emitcode ("subb", "a,b");
		    }
		}
	      else
		{
		  const char *s;

		  //emitcode (";", "genCmp #4");
		  if (AOP_NEEDSACC (right))
		    {
		      /* Yuck!! */
		      //emitcode (";", "genCmp #4.1");
		      emitcode ("xch", "a, b");
		      MOVA (aopGet (AOP (right), offset++, FALSE, FALSE, NULL));
		      emitcode ("xch", "a, b");	
		      s = "b";
		    }
		  else
		    {
		      //emitcode (";", "genCmp #4.2");
		      s = aopGet (AOP (right), offset++, FALSE, FALSE, DP2_RESULT_REG);
		    }

		  emitcode ("subb", "a,%s", s);
		}
	    }
	}
    }

release:
/* Don't need the left & right operands any more; do need the result. */
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));

  aopOp (result, ic, FALSE, FALSE);

release_freedLR:

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
	  genIfxJump (ifx, "c");
	}
      else
	{
	  outBitC (result);
	}
      /* leave the result in acc */
    }
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genCmpGt :- greater than comparison                             */
/*-----------------------------------------------------------------*/
static void
genCmpGt (iCode * ic, iCode * ifx)
{
  operand *left, *right;
  sym_link *letype, *retype;
  int sign;

  D (emitcode (";", "genCmpGt ");
    );

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);

  letype = getSpec (operandType (left));
  retype = getSpec (operandType (right));
  sign = !(SPEC_USIGN (letype) | SPEC_USIGN (retype));

  /* assign the left & right amsops */
  AOP_OP_2 (ic);

  genCmp (right, left, ic, ifx, sign);
}

/*-----------------------------------------------------------------*/
/* genCmpLt - less than comparisons                                */
/*-----------------------------------------------------------------*/
static void
genCmpLt (iCode * ic, iCode * ifx)
{
  operand *left, *right;
  sym_link *letype, *retype;
  int sign;

  D (emitcode (";", "genCmpLt "););

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);

  letype = getSpec (operandType (left));
  retype = getSpec (operandType (right));
  sign = !(SPEC_USIGN (letype) | SPEC_USIGN (retype));

  /* assign the left & right amsops */
  AOP_OP_2 (ic);

  genCmp (left, right, ic, ifx, sign);
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

  D (emitcode (";", "gencjneshort");
    );

  /* if the left side is a literal or
     if the right is in a pointer register and left
     is not */
  if ((AOP_TYPE (left) == AOP_LIT) ||
      (IS_AOP_PREG (right) && !IS_AOP_PREG (left)))
    {
      operand *t = right;
      right = left;
      left = t;
    }

  if (AOP_TYPE (right) == AOP_LIT)
    lit = (unsigned long) floatFromVal (AOP (right)->aopu.aop_lit);

  if (opIsGptr (left) || opIsGptr (right))
    {
      /* We are comparing a generic pointer to something.
       * Exclude the generic type byte from the comparison.
       */
      size--;
      D (emitcode (";", "cjneshort: generic ptr special case."););
    }


  /* if the right side is a literal then anything goes */
  if (AOP_TYPE (right) == AOP_LIT &&
      AOP_TYPE (left) != AOP_DIR)
    {
      while (size--)
	{
	  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
	  emitcode ("cjne", "a,%s,!tlabel",
		    aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG),
		    lbl->key + 100);
	  offset++;
	}
    }

  /* if the right side is in a register or in direct space or
     if the left is a pointer register & right is not */
  else if (AOP_TYPE (right) == AOP_REG ||
	   AOP_TYPE (right) == AOP_DIR ||
	   (AOP_TYPE (left) == AOP_DIR && AOP_TYPE (right) == AOP_LIT) ||
	   (IS_AOP_PREG (left) && !IS_AOP_PREG (right)))
    {
      while (size--)
	{
	  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
	  if ((AOP_TYPE (left) == AOP_DIR && AOP_TYPE (right) == AOP_LIT) &&
	      ((unsigned int) ((lit >> (offset * 8)) & 0x0FFL) == 0))
	    emitcode ("jnz", "!tlabel", lbl->key + 100);
	  else
	    emitcode ("cjne", "a,%s,!tlabel",
		      aopGet (AOP (right), offset, FALSE, TRUE, DP2_RESULT_REG),
		      lbl->key + 100);
	  offset++;
	}
    }
  else
    {
      /* right is a pointer reg need both a & b */
      while (size--)
	{
	  MOVB (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
	  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
	  emitcode ("cjne", "a,b,!tlabel", lbl->key + 100);
	  offset++;
	}
    }
}

/*-----------------------------------------------------------------*/
/* gencjne - compare and jump if not equal                         */
/*-----------------------------------------------------------------*/
static void
gencjne (operand * left, operand * right, symbol * lbl)
{
  symbol *tlbl = newiTempLabel (NULL);

  D (emitcode (";", "gencjne");
    );

  gencjneshort (left, right, lbl);

  emitcode ("mov", "a,%s", one);
  emitcode ("sjmp", "!tlabel", tlbl->key + 100);
  emitcode ("", "!tlabeldef", lbl->key + 100);
  emitcode ("clr", "a");
  emitcode ("", "!tlabeldef", tlbl->key + 100);
}

/*-----------------------------------------------------------------*/
/* genCmpEq - generates code for equal to                          */
/*-----------------------------------------------------------------*/
static void
genCmpEq (iCode * ic, iCode * ifx)
{
  operand *left, *right, *result;

  D (emitcode (";", "genCmpEq ");
    );

  AOP_OP_2 (ic);
  AOP_SET_LOCALS (ic);

  /* if literal, literal on the right or
     if the right is in a pointer register and left
     is not */
  if ((AOP_TYPE (IC_LEFT (ic)) == AOP_LIT) ||
      (IS_AOP_PREG (right) && !IS_AOP_PREG (left)))
    {
      operand *t = IC_RIGHT (ic);
      IC_RIGHT (ic) = IC_LEFT (ic);
      IC_LEFT (ic) = t;
    }

  if (ifx &&			/* !AOP_SIZE(result) */
      OP_SYMBOL (result) &&
      OP_SYMBOL (result)->regType == REG_CND)
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
	      emitcode ("jb", "%s,!tlabel", AOP (right)->aopu.aop_dir, (lbl->key + 100));
	      emitcode ("cpl", "c");
	      emitcode ("", "!tlabeldef", (lbl->key + 100));
	    }
	  /* if true label then we jump if condition
	     supplied is true */
	  tlbl = newiTempLabel (NULL);
	  if (IC_TRUE (ifx))
	    {
	      emitcode ("jnc", "!tlabel", tlbl->key + 100);
	      emitcode ("ljmp", "!tlabel", IC_TRUE (ifx)->key + 100);
	    }
	  else
	    {
	      emitcode ("jc", "!tlabel", tlbl->key + 100);
	      emitcode ("ljmp", "!tlabel", IC_FALSE (ifx)->key + 100);
	    }
	  emitcode ("", "!tlabeldef", tlbl->key + 100);
	}
      else
	{
	  tlbl = newiTempLabel (NULL);
	  gencjneshort (left, right, tlbl);
	  if (IC_TRUE (ifx))
	    {
	      emitcode ("ljmp", "!tlabel", IC_TRUE (ifx)->key + 100);
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	    }
	  else
	    {
	      symbol *lbl = newiTempLabel (NULL);
	      emitcode ("sjmp", "!tlabel", lbl->key + 100);
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	      emitcode ("ljmp", "!tlabel", IC_FALSE (ifx)->key + 100);
	      emitcode ("", "!tlabeldef", lbl->key + 100);
	    }
	}
      /* mark the icode as generated */
      ifx->generated = 1;

      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      return;
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
	  emitcode ("jb", "%s,!tlabel", AOP (right)->aopu.aop_dir, (lbl->key + 100));
	  emitcode ("cpl", "c");
	  emitcode ("", "!tlabeldef", (lbl->key + 100));
	}

      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));

      aopOp (result, ic, TRUE, FALSE);

      /* c = 1 if egal */
      if (AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result))
	{
	  outBitC (result);
	  goto release;
	}
      if (ifx)
	{
	  genIfxJump (ifx, "c");
	  goto release;
	}
      /* if the result is used in an arithmetic operation
         then put the result in place */
      outBitC (result);
    }
  else
    {
      gencjne (left, right, newiTempLabel (NULL));

      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));

      aopOp (result, ic, TRUE, FALSE);

      if (AOP_TYPE (result) == AOP_CRY && AOP_SIZE (result))
	{
	  aopPut (AOP (result), "a", 0);
	  goto release;
	}
      if (ifx)
	{
	  genIfxJump (ifx, "a");
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
      if (bitVectBitValue(OP_USES(op),lic->key) || (unsigned) lic->defKey == op->key) {
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

  D (emitcode (";", "genAndOp "););

  /* note here that && operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  AOP_OP_2 (ic);
  AOP_SET_LOCALS (ic);

  /* if both are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
      emitcode ("anl", "c,%s", AOP (right)->aopu.aop_dir);
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  
      aopOp (result,ic,FALSE, FALSE);
      outBitC (result);
    }
  else
    {
      tlbl = newiTempLabel (NULL);
      toBoolean (left);
      emitcode ("jz", "!tlabel", tlbl->key + 100);
      toBoolean (right);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  
      aopOp (result,ic,FALSE, FALSE);
      outBitAcc (result);
    }
    freeAsmop (result, NULL, ic, TRUE);
}


/*-----------------------------------------------------------------*/
/* genOrOp - for || operation                                      */
/*-----------------------------------------------------------------*/
static void
genOrOp (iCode * ic)
{
  operand *left, *right, *result;
  symbol *tlbl;

  D (emitcode (";", "genOrOp "););

  /* note here that || operations that are in an
     if statement are taken away by backPatchLabels
     only those used in arthmetic operations remain */
  AOP_OP_2 (ic);
  AOP_SET_LOCALS (ic);

  /* if both are bit variables */
  if (AOP_TYPE (left) == AOP_CRY &&
      AOP_TYPE (right) == AOP_CRY)
    {
      emitcode ("mov", "c,%s", AOP (left)->aopu.aop_dir);
      emitcode ("orl", "c,%s", AOP (right)->aopu.aop_dir);
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  
      aopOp (result,ic,FALSE, FALSE);
      
      outBitC (result);
    }
  else
    {
      tlbl = newiTempLabel (NULL);
      toBoolean (left);
      emitcode ("jnz", "!tlabel", tlbl->key + 100);
      toBoolean (right);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
      freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  
      aopOp (result,ic,FALSE, FALSE);
      
      outBitAcc (result);
    }

  freeAsmop (result, NULL, ic, TRUE);
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
    emitcode ("ljmp", "!tlabel", IC_TRUE (ic)->key + 100);
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* jmpIfTrue -                                                     */
/*-----------------------------------------------------------------*/
static void
jumpIfTrue (iCode * ic)
{
  if (!IC_TRUE (ic))
    emitcode ("ljmp", "!tlabel", IC_FALSE (ic)->key + 100);
  ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* jmpTrueOrFalse -                                                */
/*-----------------------------------------------------------------*/
static void
jmpTrueOrFalse (iCode * ic, symbol * tlbl)
{
  // ugly but optimized by peephole
  if (IC_TRUE (ic))
    {
      symbol *nlbl = newiTempLabel (NULL);
      emitcode ("sjmp", "!tlabel", nlbl->key + 100);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      emitcode ("ljmp", "!tlabel", IC_TRUE (ic)->key + 100);
      emitcode ("", "!tlabeldef", nlbl->key + 100);
    }
  else
    {
      emitcode ("ljmp", "!tlabel", IC_FALSE (ic)->key + 100);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
    }
  ic->generated = 1;
}

// Generate code to perform a bit-wise logic operation
// on two operands in far space (assumed to already have been 
// aopOp'd by the AOP_OP_3_NOFATAL macro), storing the result
// in far space. This requires pushing the result on the stack
// then popping it into the result.
static void
genFarFarLogicOp(iCode *ic, char *logicOp)
{
      int size, resultSize, compSize;
      int offset = 0;
      
      TR_AP("#5");
      D(emitcode(";", "%s special case for 3 far operands.", logicOp););
      compSize = AOP_SIZE(IC_LEFT(ic)) < AOP_SIZE(IC_RIGHT(ic)) ? 
      		  AOP_SIZE(IC_LEFT(ic)) : AOP_SIZE(IC_RIGHT(ic));
      
      _startLazyDPSEvaluation();
      for (size = compSize; (size--); offset++)
      {
      	  MOVA (aopGet (AOP (IC_LEFT(ic)), offset, FALSE, FALSE, NULL));
      	  emitcode ("mov", "%s, acc", DP2_RESULT_REG);
      	  MOVA (aopGet (AOP (IC_RIGHT(ic)), offset, FALSE, FALSE, NULL));
      	  
      	  emitcode (logicOp, "a,%s", DP2_RESULT_REG);
      	  emitcode ("push", "acc");
      }
      _endLazyDPSEvaluation();
     
      freeAsmop (IC_LEFT(ic), NULL, ic, RESULTONSTACK (ic) ? FALSE : TRUE);
      freeAsmop (IC_RIGHT(ic), NULL, ic, RESULTONSTACK (ic) ? FALSE : TRUE);
      aopOp (IC_RESULT(ic),ic,TRUE, FALSE);
     
      resultSize = AOP_SIZE(IC_RESULT(ic));

      ADJUST_PUSHED_RESULT(compSize, resultSize);

      _startLazyDPSEvaluation();
      while (compSize--)
      {
	  emitcode ("pop", "acc");
	  aopPut (AOP (IC_RESULT (ic)), "a", compSize);
      }
      _endLazyDPSEvaluation();
      freeAsmop(IC_RESULT (ic), NULL, ic, TRUE);
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
  int bytelit;
  char buff[10];
  bool pushResult;

  D (emitcode (";", "genAnd "););

  AOP_OP_3_NOFATAL (ic, pushResult);
  AOP_SET_LOCALS (ic);

  if (pushResult)
  {
      genFarFarLogicOp(ic, "anl");
      return;
  }  

#ifdef DEBUG_TYPE
  emitcode ("", "; Type res[%d] = l[%d]&r[%d]",
	    AOP_TYPE (result),
	    AOP_TYPE (left), AOP_TYPE (right));
  emitcode ("", "; Size res[%d] = l[%d]&r[%d]",
	    AOP_SIZE (result),
	    AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not then exchange them */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT)
#ifdef LOGIC_OPS_BROKEN      
    ||  AOP_NEEDSACC (left)
#endif
    )
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
	      MOVA (aopGet (AOP (right), 0, FALSE, FALSE, NULL));
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
	genIfxJump (ifx, "c");
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
	  MOVA (aopGet (AOP (left), posbit >> 3, FALSE, FALSE, NULL));
	  // bit = left & 2^n
	  if (size)
	    emitcode ("mov", "c,acc.%d", posbit & 0x07);
	  // if(left &  2^n)
	  else
	    {
	      if (ifx)
		{
		  SNPRINTF (buff, sizeof(buff), 
			    "acc.%d", posbit & 0x07);
		  genIfxJump (ifx, buff);
		}
	      else 
		  {
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
		  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		  // byte ==  2^n ?
		  if ((posbit = isLiteralBit (bytelit)) != 0)
		    emitcode ("jb", "acc.%d,!tlabel", (posbit - 1) & 0x07, tlbl->key + 100);
		  else
		    {
		      if (bytelit != 0x0FFL)
			emitcode ("anl", "a,%s",
			  aopGet (AOP (right), offset, FALSE, TRUE, DP2_RESULT_REG));
		      emitcode ("jnz", "!tlabel", tlbl->key + 100);
		    }
		}
	      offset++;
	    }
	  // bit = left & literal
	  if (size)
	    {
	      emitcode ("clr", "c");
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	    }
	  // if(left & literal)
	  else
	    {
	      if (ifx)
		jmpTrueOrFalse (ifx, tlbl);
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
	      if ((bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL)) == 0x0FF)
		continue;
	      else if (bytelit == 0)
		aopPut (AOP (result), zero, offset);
	      else if (IS_AOP_PREG (result))
		{
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  emitcode ("anl", "a,%s",
			    aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		  aopPut (AOP (result), "a", offset);
		}
	      else
		emitcode ("anl", "%s,%s",
			  aopGet (AOP (left), offset, FALSE, TRUE, NULL),
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	    }
	  else
	    {
	      if (AOP_TYPE (left) == AOP_ACC)
		emitcode ("anl", "a,%s",
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	      else
		{
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  if (IS_AOP_PREG (result))
		    {
		      emitcode ("anl", "a,%s",
				aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		      aopPut (AOP (result), "a", offset);
		    }
		  else
		    emitcode ("anl", "%s,a",
			   aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
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
	      if (AOP_TYPE(right)==AOP_REG && AOP_TYPE(left)==AOP_ACC) {
		emitcode ("anl", "a,%s",
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	      } else {
		if (AOP_TYPE(left)==AOP_ACC) {
		  emitcode("mov", "b,a");
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  emitcode("anl", "a,b");
		}else {
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  emitcode ("anl", "a,%s",
			    aopGet (AOP (left), offset, FALSE, FALSE, DP2_RESULT_REG));
		}
	      }
	      emitcode ("jnz", "!tlabel", tlbl->key + 100);
	      offset++;
	    }
	  if (size)
	    {
	      CLRC;
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	      outBitC (result);
	    }
	  else if (ifx)
	    jmpTrueOrFalse (ifx, tlbl);
	}
      else
	{
	  for (; (size--); offset++)
	    {
	      // normal case
	      // result = left & right
	      if (AOP_TYPE (right) == AOP_LIT)
		{
		  if ((bytelit = (int) ((lit >> (offset * 8)) & 0x0FFL)) == 0x0FF)
		    {
		      aopPut (AOP (result),
			   aopGet (AOP (left), offset, FALSE, FALSE, NULL),
			      offset);
		      continue;
		    }
		  else if (bytelit == 0)
		    {
		      aopPut (AOP (result), zero, offset);
		      continue;
		    }
		  D (emitcode (";", "better literal AND."););
		  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		  emitcode ("anl", "a, %s", aopGet (AOP (right), offset,
						    FALSE, FALSE, DP2_RESULT_REG));

		}
	      else
		{
		  // faster than result <- left, anl result,right
		  // and better if result is SFR
		  if (AOP_TYPE (left) == AOP_ACC)
		    {
		      emitcode ("anl", "a,%s", 
				aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
		    }
		  else
		    {
		      char *rOp = aopGet (AOP (right), offset, FALSE, FALSE, NULL);
		      if (!strcmp(rOp, "a") || !strcmp(rOp, "acc"))
		      {
			  emitcode("mov", "b,a");
			  rOp = "b";
		      }
			
		      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		      emitcode ("anl", "a,%s", rOp);
		    }			
		}
	      aopPut (AOP (result), "a", offset);
	    }
	}
    }

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
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
  bool 	   pushResult;

  D (emitcode (";", "genOr "););

  AOP_OP_3_NOFATAL (ic, pushResult);
  AOP_SET_LOCALS (ic);

  if (pushResult)
  {
      genFarFarLogicOp(ic, "orl");
      return;
  }


#ifdef DEBUG_TYPE
  emitcode ("", "; Type res[%d] = l[%d]&r[%d]",
	    AOP_TYPE (result),
	    AOP_TYPE (left), AOP_TYPE (right));
  emitcode ("", "; Size res[%d] = l[%d]&r[%d]",
	    AOP_SIZE (result),
	    AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not then exchange them */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT)
#ifdef LOGIC_OPS_BROKEN
   || AOP_NEEDSACC (left) // I think this is a net loss now.
#endif      
      )
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
	  // c = bit & literal;
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
	      emitcode ("jb", "%s,!tlabel",
			AOP (left)->aopu.aop_dir, tlbl->key + 100);
	      toBoolean (right);
	      emitcode ("jnz", "!tlabel", tlbl->key + 100);
	      if ((AOP_TYPE (result) == AOP_CRY) && ifx)
		{
		  jmpTrueOrFalse (ifx, tlbl);
		  goto release;
		}
	      else
		{
		  CLRC;
		  emitcode ("", "!tlabeldef", tlbl->key + 100);
		}
	    }
	}
      // bit = c
      // val = c
      if (size)
	outBitC (result);
      // if(bit | ...)
      else if ((AOP_TYPE (result) == AOP_CRY) && ifx)
	   genIfxJump (ifx, "c");
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
	      emitcode ("jnz", "!tlabel", tlbl->key + 100);
	      CLRC;
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	    }
	  else
	    {
	      genIfxJump (ifx, "a");
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
	      if (((lit >> (offset * 8)) & 0x0FFL) == 0x00L)
		{
		  continue;
		}
	      else
		{
		  if (IS_AOP_PREG (left))
		    {
		      MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		      emitcode ("orl", "a,%s",
				aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		      aopPut (AOP (result), "a", offset);
		    }
		  else
		    {
		      emitcode ("orl", "%s,%s",
				aopGet (AOP (left), offset, FALSE, TRUE, NULL),
				aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
		    }
		}
	    }
	  else
	    {
	      if (AOP_TYPE (left) == AOP_ACC)
		{
		  emitcode ("orl", "a,%s",
			    aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
		}
	      else
		{
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  if (IS_AOP_PREG (left))
		    {
		      emitcode ("orl", "a,%s", 
				aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		      aopPut (AOP (result), "a", offset);
		    }
		  else
		    {
		      emitcode ("orl", "%s,a",
			   aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
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
	      if (AOP_TYPE(right)==AOP_REG && AOP_TYPE(left)==AOP_ACC) {
		emitcode ("orl", "a,%s",
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	      } else {
		MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		emitcode ("orl", "a,%s",
			  aopGet (AOP (left), offset, FALSE, FALSE, DP2_RESULT_REG));
	      }
	      emitcode ("jnz", "!tlabel", tlbl->key + 100);
	      offset++;
	    }
	  if (size)
	    {
	      CLRC;
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	      outBitC (result);
	    }
	  else if (ifx)
	    jmpTrueOrFalse (ifx, tlbl);
	}
      else
	{
	    _startLazyDPSEvaluation();
	  for (; (size--); offset++)
	    {
	      // normal case
	      // result = left & right
	      if (AOP_TYPE (right) == AOP_LIT)
		{
		  if (((lit >> (offset * 8)) & 0x0FFL) == 0x00L)
		    {
		      aopPut (AOP (result),
			   aopGet (AOP (left), offset, FALSE, FALSE, NULL),
			      offset);
		      continue;
		    }
		  D (emitcode (";", "better literal OR."););
		  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		  emitcode ("orl", "a, %s",
			    aopGet (AOP (right), offset,
				    FALSE, FALSE, DP2_RESULT_REG));

		}
	      else
		{
		  // faster than result <- left, anl result,right
		  // and better if result is SFR
		  if (AOP_TYPE (left) == AOP_ACC)
		    {
		      emitcode ("orl", "a,%s",
				aopGet (AOP (right), offset,
					FALSE, FALSE, DP2_RESULT_REG));
		    }
		  else
		    {
		      char *rOp = aopGet (AOP (right), offset, FALSE, FALSE, NULL);
			
		      if (!strcmp(rOp, "a") || !strcmp(rOp, "acc"))
		      {
			  emitcode("mov", "b,a");
			  rOp = "b";
		      }
			
		      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		      emitcode ("orl", "a,%s", rOp);
		    }
		}
	      aopPut (AOP (result), "a", offset);
	    }
	    _endLazyDPSEvaluation();
	}
    }

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
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
  bool pushResult;

  D (emitcode (";", "genXor "););

  AOP_OP_3_NOFATAL (ic, pushResult);
  AOP_SET_LOCALS (ic);

  if (pushResult)
  {
      genFarFarLogicOp(ic, "xrl");
      return;
  }  

#ifdef DEBUG_TYPE
  emitcode ("", "; Type res[%d] = l[%d]&r[%d]",
	    AOP_TYPE (result),
	    AOP_TYPE (left), AOP_TYPE (right));
  emitcode ("", "; Size res[%d] = l[%d]&r[%d]",
	    AOP_SIZE (result),
	    AOP_SIZE (left), AOP_SIZE (right));
#endif

  /* if left is a literal & right is not ||
     if left needs acc & right does not */
  if ((AOP_TYPE (left) == AOP_LIT && AOP_TYPE (right) != AOP_LIT) 
#ifdef LOGIC_OPS_BROKEN      
      || (AOP_NEEDSACC (left) && !AOP_NEEDSACC (right))
#endif
     )
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
	      emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
	    }
	  else
	    {
	      int sizer = AOP_SIZE (right);
	      // c = bit ^ val
	      // if val>>1 != 0, result = 1
	      emitcode ("setb", "c");
	      while (sizer)
		{
		  MOVA (aopGet (AOP (right), sizer - 1, FALSE, FALSE, NULL));
		  if (sizer == 1)
		    // test the msb of the lsb
		    emitcode ("anl", "a,#!constbyte",0xfe);
		  emitcode ("jnz", "!tlabel", tlbl->key + 100);
		  sizer--;
		}
	      // val = (0,1)
	      emitcode ("rrc", "a");
	    }
	  emitcode ("jnb", "%s,!tlabel", AOP (left)->aopu.aop_dir, (tlbl->key + 100));
	  emitcode ("cpl", "c");
	  emitcode ("", "!tlabeldef", (tlbl->key + 100));
	}
      // bit = c
      // val = c
      if (size)
	outBitC (result);
      // if(bit | ...)
      else if ((AOP_TYPE (result) == AOP_CRY) && ifx)
	genIfxJump (ifx, "c");
      goto release;
    }

  if (sameRegs (AOP (result), AOP (left)))
    {
      /* if left is same as result */
      for (; size--; offset++)
	{
	  if (AOP_TYPE (right) == AOP_LIT)
	    {
	      if (((lit >> (offset * 8)) & 0x0FFL) == 0x00L)
		continue;
	      else if (IS_AOP_PREG (left))
		{
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  emitcode ("xrl", "a,%s",
			    aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		  aopPut (AOP (result), "a", offset);
		}
	      else
		emitcode ("xrl", "%s,%s",
			  aopGet (AOP (left), offset, FALSE, TRUE, NULL),
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	    }
	  else
	    {
	      if (AOP_TYPE (left) == AOP_ACC)
		emitcode ("xrl", "a,%s",
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	      else
		{
		  MOVA (aopGet (AOP (right), offset, FALSE, FALSE, NULL));
		  if (IS_AOP_PREG (left))
		    {
		      emitcode ("xrl", "a,%s",
				aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
		      aopPut (AOP (result), "a", offset);
		    }
		  else
		    emitcode ("xrl", "%s,a",
			   aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG));
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
		  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		}
	      else
		{
		  if (AOP_TYPE(right)==AOP_REG && AOP_TYPE(left)==AOP_ACC) {
		    emitcode ("xrl", "a,%s",
			      aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
		  } else {
		      char *rOp = aopGet (AOP (right), offset, FALSE, FALSE, NULL);
		      if (!strcmp(rOp, "a") || !strcmp(rOp, "acc"))
		      {
			  emitcode("mov", "b,a");
			  rOp = "b";
		      }
			
		      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		      emitcode ("xrl", "a,%s", rOp);		      
		  }
		}
	      emitcode ("jnz", "!tlabel", tlbl->key + 100);
	      offset++;
	    }
	  if (size)
	    {
	      CLRC;
	      emitcode ("", "!tlabeldef", tlbl->key + 100);
	      outBitC (result);
	    }
	  else if (ifx)
	    jmpTrueOrFalse (ifx, tlbl);
	}
      else
	{
	for (; (size--); offset++)
	  {
	    // normal case
	    // result = left & right
	    if (AOP_TYPE (right) == AOP_LIT)
	      {
		if (((lit >> (offset * 8)) & 0x0FFL) == 0x00L)
		  {
		    aopPut (AOP (result),
			    aopGet (AOP (left), offset, FALSE, FALSE, NULL),
			    offset);
		    continue;
		  }
		D (emitcode (";", "better literal XOR."););
		MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		emitcode ("xrl", "a, %s",
			  aopGet (AOP (right), offset, FALSE, FALSE, DP2_RESULT_REG));
	      }
	    else
	      {
		// faster than result <- left, anl result,right
		// and better if result is SFR
		if (AOP_TYPE (left) == AOP_ACC)
		  {
		    emitcode ("xrl", "a,%s",
			      aopGet (AOP (right), offset,
				      FALSE, FALSE, DP2_RESULT_REG));
		  }
		else
		  {
		      char *rOp = aopGet (AOP (right), offset, FALSE, FALSE, NULL);
		      if (!strcmp(rOp, "a") || !strcmp(rOp, "acc"))
		      {
			  emitcode("mov", "b,a");
			  rOp = "b";
		      }
			
		      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
		      emitcode ("xrl", "a,%s", rOp);
		  }
	      }
	    aopPut (AOP (result), "a", offset);
	  }
	}
	
    }

release:
  freeAsmop (left, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (right, NULL, ic, (RESULTONSTACK (ic) ? FALSE : TRUE));
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genInline - write the inline code out                           */
/*-----------------------------------------------------------------*/
static void
genInline (iCode * ic)
{
  char *buffer, *bp, *bp1;

  D (emitcode (";", "genInline "); );

  _G.inLine += (!options.asmpeep);

  buffer = Safe_strdup(IC_INLINE(ic));
  bp = buffer;
  bp1 = buffer;

  /* emit each line as a code */
  while (*bp)
    {
      if (*bp == '\n')
	{
	  *bp++ = '\0';
	  emitcode (bp1, "");
	  bp1 = bp;
	}
      else
	{
	  if (*bp == ':')
	    {
	      bp++;
	      *bp = '\0';
	      bp++;
	      emitcode (bp1, "");
	      bp1 = bp;
	    }
	  else
	    bp++;
	}
    }
  if (bp1 != bp)
    emitcode (bp1, "");
  /*     emitcode("",buffer); */
  _G.inLine -= (!options.asmpeep);
}

/*-----------------------------------------------------------------*/
/* genRRC - rotate right with carry                                */
/*-----------------------------------------------------------------*/
static void
genRRC (iCode * ic)
{
  operand *left, *result;
  int 	  size, offset;

  D (emitcode (";", "genRRC "););

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

  /* move it to the result */
  size = AOP_SIZE (result);
  offset = size - 1;
  CLRC;

  _startLazyDPSEvaluation ();
  while (size--)
    {
      MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
      emitcode ("rrc", "a");
      if (AOP_SIZE (result) > 1)
	aopPut (AOP (result), "a", offset--);
    }
  _endLazyDPSEvaluation ();

  /* now we need to put the carry into the
     highest order byte of the result */
  if (AOP_SIZE (result) > 1)
    {
      MOVA (aopGet (AOP (result), AOP_SIZE (result) - 1, FALSE, FALSE, NULL));
    }
  emitcode ("mov", "acc.7,c");
  aopPut (AOP (result), "a", AOP_SIZE (result) - 1);
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
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

  D (emitcode (";", "genRLC "););

  /* rotate right with carry */
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

  /* move it to the result */
  size = AOP_SIZE (result);
  offset = 0;
  if (size--)
    {
      l = aopGet (AOP (left), offset, FALSE, FALSE, NULL);
      MOVA (l);
      emitcode ("add", "a,acc");
      if (AOP_SIZE (result) > 1)
	{
	  aopPut (AOP (result), "a", offset++);
	}

      _startLazyDPSEvaluation ();
      while (size--)
	{
	  l = aopGet (AOP (left), offset, FALSE, FALSE, NULL);
	  MOVA (l);
	  emitcode ("rlc", "a");
	  if (AOP_SIZE (result) > 1)
	    aopPut (AOP (result), "a", offset++);
	}
      _endLazyDPSEvaluation ();
    }
  /* now we need to put the carry into the
     highest order byte of the result */
  if (AOP_SIZE (result) > 1)
    {
      l = aopGet (AOP (result), 0, FALSE, FALSE, NULL);
      MOVA (l);
    }
  emitcode ("mov", "acc.0,c");
  aopPut (AOP (result), "a", 0);
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGetHbit - generates code get highest order bit               */
/*-----------------------------------------------------------------*/
static void
genGetHbit (iCode * ic)
{
  operand *left, *result;
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

  D (emitcode (";", "genGetHbit "););

  /* get the highest order byte into a */
  MOVA (aopGet (AOP (left), AOP_SIZE (left) - 1, FALSE, FALSE, NULL));
  if (AOP_TYPE (result) == AOP_CRY)
    {
      emitcode ("rlc", "a");
      outBitC (result);
    }
  else
    {
      emitcode ("rl", "a");
      emitcode ("anl", "a,#1");
      outAcc (result);
    }


  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* AccRol - rotate left accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void
AccRol (int shCount)
{
  shCount &= 0x0007;		// shCount : 0..7

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
	  emitcode ("anl", "a,#!constbyte", SLMask[shCount]);
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
	  emitcode ("anl", "a,#!constbyte", SRMask[shCount]);
	}
    }
}

#ifdef BETTER_LITERAL_SHIFT
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
	  emitcode ("anl", "a,#!constbyte", SRMask[shCount]);
	  emitcode ("jnb", "acc.%d,!tlabel", 7 - shCount, tlbl->key + 100);
	  emitcode ("orl", "a,#!constbyte",
		    (unsigned char) ~SRMask[shCount]);
	  emitcode ("", "!tlabeldef", tlbl->key + 100);
	}
    }
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* shiftR1Left2Result - shift right one byte from left to result   */
/*-----------------------------------------------------------------*/
static void
shiftR1Left2Result (operand * left, int offl,
		    operand * result, int offr,
		    int shCount, int sign)
{
  MOVA (aopGet (AOP (left), offl, FALSE, FALSE, NULL));
  /* shift right accumulator */
  if (sign)
    AccSRsh (shCount);
  else
    AccRsh (shCount);
  aopPut (AOP (result), "a", offr);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* shiftL1Left2Result - shift left one byte from left to result    */
/*-----------------------------------------------------------------*/
static void
shiftL1Left2Result (operand * left, int offl,
		    operand * result, int offr, int shCount)
{
  MOVA(aopGet (AOP (left), offl, FALSE, FALSE, NULL));
  /* shift left accumulator */
  AccLsh (shCount);
  aopPut (AOP (result), "a", offr);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
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
      l = aopGet (AOP (left), offl, FALSE, FALSE, NULL);

      if (*l == '@' && (IS_AOP_PREG (result)))
      {
	  emitcode ("mov", "a,%s", l);
	  aopPut (AOP (result), "a", offr);
      }
      else
      {
	  if (!sign)
	  {
	    aopPut (AOP (result), l, offr);
	  }
	  else
	    {
	      /* MSB sign in acc.7 ! */
	      if (getDataSize (left) == offl + 1)
		{
		  emitcode ("mov", "a,%s", l);
		  aopPut (AOP (result), "a", offr);
		}
	    }
      }
  }
}
#endif

#ifdef BETTER_LITERAL_SHIFT
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
#endif

#ifdef BETTER_LITERAL_SHIFT
//REMOVE ME!!!
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
#endif

#ifdef BETTER_LITERAL_SHIFT
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
#endif

#ifdef BETTER_LITERAL_SHIFT
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
    case 5:			// AAAAABBB:CCCCCDDD

      AccRol (shCount);		// BBBAAAAA:CCCCCDDD

      emitcode ("anl", "a,#!constbyte",
		SLMask[shCount]);	// BBB00000:CCCCCDDD

      emitcode ("xch", "a,%s", x);	// CCCCCDDD:BBB00000

      AccRol (shCount);		// DDDCCCCC:BBB00000

      emitcode ("xch", "a,%s", x);	// BBB00000:DDDCCCCC

      emitcode ("xrl", "a,%s", x);	// (BBB^DDD)CCCCC:DDDCCCCC

      emitcode ("xch", "a,%s", x);	// DDDCCCCC:(BBB^DDD)CCCCC

      emitcode ("anl", "a,#!constbyte",
		SLMask[shCount]);	// DDD00000:(BBB^DDD)CCCCC

      emitcode ("xch", "a,%s", x);	// (BBB^DDD)CCCCC:DDD00000

      emitcode ("xrl", "a,%s", x);	// BBBCCCCC:DDD00000

      break;
    case 6:			// AAAAAABB:CCCCCCDD
      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000000BB:CCCCCCDD
      emitcode ("mov", "c,acc.0");	// c = B
      emitcode ("xch", "a,%s", x);	// CCCCCCDD:000000BB
#if 0
      AccAXRrl1 (x);		// BCCCCCCD:D000000B
      AccAXRrl1 (x);		// BBCCCCCC:DD000000
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
    case 7:			// a:x <<= 7

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 0000000B:CCCCCCCD

      emitcode ("mov", "c,acc.0");	// c = B

      emitcode ("xch", "a,%s", x);	// CCCCCCCD:0000000B

      AccAXRrl1 (x);		// BCCCCCCC:D0000000

      break;
    default:
      break;
    }
}
#endif

#ifdef BETTER_LITERAL_SHIFT
//REMOVE ME!!!
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
      AccAXRrl1 (x);		// 0->a:x

      break;
    case 2:
      CLRC;
      AccAXRrl1 (x);		// 0->a:x

      CLRC;
      AccAXRrl1 (x);		// 0->a:x

      break;
    case 3:
    case 4:
    case 5:			// AAAAABBB:CCCCCDDD = a:x

      AccRol (8 - shCount);	// BBBAAAAA:DDDCCCCC

      emitcode ("xch", "a,%s", x);	// CCCCCDDD:BBBAAAAA

      AccRol (8 - shCount);	// DDDCCCCC:BBBAAAAA

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000CCCCC:BBBAAAAA

      emitcode ("xrl", "a,%s", x);	// BBB(CCCCC^AAAAA):BBBAAAAA

      emitcode ("xch", "a,%s", x);	// BBBAAAAA:BBB(CCCCC^AAAAA)

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000AAAAA:BBB(CCCCC^AAAAA)

      emitcode ("xch", "a,%s", x);	// BBB(CCCCC^AAAAA):000AAAAA

      emitcode ("xrl", "a,%s", x);	// BBBCCCCC:000AAAAA

      emitcode ("xch", "a,%s", x);	// 000AAAAA:BBBCCCCC

      break;
    case 6:			// AABBBBBB:CCDDDDDD

      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);		// ABBBBBBC:CDDDDDDA

      AccAXLrl1 (x);		// BBBBBBCC:DDDDDDAA

      emitcode ("xch", "a,%s", x);	// DDDDDDAA:BBBBBBCC

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000000AA:BBBBBBCC

      break;
    case 7:			// ABBBBBBB:CDDDDDDD

      emitcode ("mov", "c,acc.7");	// c = A

      AccAXLrl1 (x);		// BBBBBBBC:DDDDDDDA

      emitcode ("xch", "a,%s", x);	// DDDDDDDA:BBBBBBCC

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 0000000A:BBBBBBBC

      break;
    default:
      break;
    }
}
#endif

#ifdef BETTER_LITERAL_SHIFT
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
      AccAXRrl1 (x);		// s->a:x

      break;
    case 2:
      emitcode ("mov", "c,acc.7");
      AccAXRrl1 (x);		// s->a:x

      emitcode ("mov", "c,acc.7");
      AccAXRrl1 (x);		// s->a:x

      break;
    case 3:
    case 4:
    case 5:			// AAAAABBB:CCCCCDDD = a:x

      tlbl = newiTempLabel (NULL);
      AccRol (8 - shCount);	// BBBAAAAA:CCCCCDDD

      emitcode ("xch", "a,%s", x);	// CCCCCDDD:BBBAAAAA

      AccRol (8 - shCount);	// DDDCCCCC:BBBAAAAA

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000CCCCC:BBBAAAAA

      emitcode ("xrl", "a,%s", x);	// BBB(CCCCC^AAAAA):BBBAAAAA

      emitcode ("xch", "a,%s", x);	// BBBAAAAA:BBB(CCCCC^AAAAA)

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000AAAAA:BBB(CCCCC^AAAAA)

      emitcode ("xch", "a,%s", x);	// BBB(CCCCC^AAAAA):000AAAAA

      emitcode ("xrl", "a,%s", x);	// BBBCCCCC:000AAAAA

      emitcode ("xch", "a,%s", x);	// 000SAAAA:BBBCCCCC

      emitcode ("jnb", "acc.%d,!tlabel", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#!constbyte",
		(unsigned char) ~SRMask[shCount]);	// 111AAAAA:BBBCCCCC

      emitcode ("", "!tlabeldef", tlbl->key + 100);
      break;			// SSSSAAAA:BBBCCCCC

    case 6:			// AABBBBBB:CCDDDDDD

      tlbl = newiTempLabel (NULL);
      emitcode ("mov", "c,acc.7");
      AccAXLrl1 (x);		// ABBBBBBC:CDDDDDDA

      AccAXLrl1 (x);		// BBBBBBCC:DDDDDDAA

      emitcode ("xch", "a,%s", x);	// DDDDDDAA:BBBBBBCC

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 000000AA:BBBBBBCC

      emitcode ("jnb", "acc.%d,!tlabel", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#!constbyte",
		(unsigned char) ~SRMask[shCount]);	// 111111AA:BBBBBBCC

      emitcode ("", "!tlabeldef", tlbl->key + 100);
      break;
    case 7:			// ABBBBBBB:CDDDDDDD

      tlbl = newiTempLabel (NULL);
      emitcode ("mov", "c,acc.7");	// c = A

      AccAXLrl1 (x);		// BBBBBBBC:DDDDDDDA

      emitcode ("xch", "a,%s", x);	// DDDDDDDA:BBBBBBCC

      emitcode ("anl", "a,#!constbyte",
		SRMask[shCount]);	// 0000000A:BBBBBBBC

      emitcode ("jnb", "acc.%d,!tlabel", 7 - shCount, tlbl->key + 100);
      emitcode ("orl", "a,#!constbyte",
		(unsigned char) ~SRMask[shCount]);	// 1111111A:BBBBBBBC

      emitcode ("", "!tlabeldef", tlbl->key + 100);
      break;
    default:
      break;
    }
}
#endif

#ifdef BETTER_LITERAL_SHIFT
static void
_loadLeftIntoAx(char 	**lsb, 
		operand *left, 
		operand *result,
		int	offl,
		int 	offr)
{
  // Get the initial value from left into a pair of registers.
  // MSB must be in A, LSB can be any register.
  //
  // If the result is held in registers, it is an optimization
  // if the LSB can be held in the register which will hold the,
  // result LSB since this saves us from having to copy it into
  // the result following AccAXLsh.
  //
  // If the result is addressed indirectly, this is not a gain.
  if (AOP_NEEDSACC(result))
  {
       char *leftByte;
       
       _startLazyDPSEvaluation();
      if (AOP_TYPE(left) == AOP_DPTR2)
       {
           // Get MSB in A.
       	   MOVA(aopGet(AOP(left), offl + MSB16, FALSE, FALSE, NULL));
       	   // get LSB in DP2_RESULT_REG.
       	   leftByte = aopGet(AOP(left), offl, FALSE, FALSE, DP2_RESULT_REG);
       	   assert(!strcmp(leftByte, DP2_RESULT_REG));
       }
       else
       {
           // get LSB into DP2_RESULT_REG
       	   leftByte = aopGet (AOP(left), offl, FALSE, FALSE, NULL);
           if (strcmp(leftByte, DP2_RESULT_REG))
           {
               TR_AP("#7");
               emitcode("mov","%s,%s", DP2_RESULT_REG, leftByte);
       	   }
       	   // And MSB in A.
       	   leftByte = aopGet(AOP(left), offl + MSB16, FALSE, FALSE, NULL);
       	   assert(strcmp(leftByte, DP2_RESULT_REG));
       	   MOVA(leftByte);
       }
       _endLazyDPSEvaluation();
       *lsb = DP2_RESULT_REG;
  }
  else
  {
      if (sameRegs (AOP (result), AOP (left)) &&
      	((offl + MSB16) == offr))
      {
      	  /* don't crash result[offr] */
      	  MOVA(aopGet(AOP(left), offl, FALSE, FALSE, NULL));
      	  emitcode ("xch", "a,%s", 
      	  	    aopGet(AOP(left), offl + MSB16, FALSE, FALSE, DP2_RESULT_REG));
      }
      else
      {
      	  movLeft2Result (left, offl, result, offr, 0);
      	  MOVA (aopGet (AOP (left), offl + MSB16, FALSE, FALSE, NULL));
      }
      *lsb = aopGet(AOP (result), offr, FALSE, FALSE, DP2_RESULT_REG);
      assert(strcmp(*lsb,"a"));      
  }
}

static void
_storeAxResults(char 	*lsb,
		operand *result,
		int	offr)
{
  _startLazyDPSEvaluation();
  if (AOP_NEEDSACC(result))
  {
      /* We have to explicitly update the result LSB.
       */
      emitcode("xch","a,%s", lsb);
      aopPut(AOP(result), "a", offr);
      emitcode("mov","a,%s", lsb);
  }
  if (getDataSize (result) > 1)
  {
      aopPut (AOP (result), "a", offr + MSB16);
  }
  _endLazyDPSEvaluation();
}

/*-----------------------------------------------------------------*/
/* shiftL2Left2Result - shift left two bytes from left to result   */
/*-----------------------------------------------------------------*/
static void
shiftL2Left2Result (operand * left, int offl,
		    operand * result, int offr, int shCount)
{
  char *lsb;

  _loadLeftIntoAx(&lsb, left, result, offl, offr);
  
  AccAXLsh (lsb, shCount);
  
  _storeAxResults(lsb, result, offr);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* shiftR2Left2Result - shift right two bytes from left to result  */
/*-----------------------------------------------------------------*/
static void
shiftR2Left2Result (operand * left, int offl,
		    operand * result, int offr,
		    int shCount, int sign)
{
  char *lsb;
  
  _loadLeftIntoAx(&lsb, left, result, offl, offr);
  
  /* a:x >> shCount (x = lsb(result)) */
  if (sign)
  {
     AccAXRshS(lsb, shCount);
  }
  else
  {
    AccAXRsh(lsb, shCount);
  }
  
  _storeAxResults(lsb, result, offr);
}
#endif

#if 0
//REMOVE ME!!!
/*-----------------------------------------------------------------*/
/* shiftLLeftOrResult - shift left one byte from left, or to result */
/*-----------------------------------------------------------------*/
static void
shiftLLeftOrResult (operand * left, int offl,
		    operand * result, int offr, int shCount)
{
  MOVA (aopGet (AOP (left), offl, FALSE, FALSE, NULL));
  /* shift left accumulator */
  AccLsh (shCount);
  /* or with result */
  emitcode ("orl", "a,%s",
	    aopGet (AOP (result), offr, FALSE, FALSE, DP2_RESULT_REG));
  /* back to result */
  aopPut (AOP (result), "a", offr);
}
#endif

#if 0
//REMOVE ME!!!
/*-----------------------------------------------------------------*/
/* shiftRLeftOrResult - shift right one byte from left,or to result */
/*-----------------------------------------------------------------*/
static void
shiftRLeftOrResult (operand * left, int offl,
		    operand * result, int offr, int shCount)
{
  MOVA (aopGet (AOP (left), offl, FALSE, FALSE, NULL));
  /* shift right accumulator */
  AccRsh (shCount);
  /* or with result */
  emitcode ("orl", "a,%s",
	    aopGet (AOP (result), offr, FALSE, FALSE, DP2_RESULT_REG));
  /* back to result */
  aopPut (AOP (result), "a", offr);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genlshOne - left shift a one byte quantity by known count       */
/*-----------------------------------------------------------------*/
static void
genlshOne (operand * result, operand * left, int shCount)
{
  D (emitcode (";", "genlshOne "););
  shiftL1Left2Result (left, LSB, result, LSB, shCount);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genlshTwo - left shift two bytes by known amount != 0           */
/*-----------------------------------------------------------------*/
static void
genlshTwo (operand * result, operand * left, int shCount)
{
  int size;

  D (emitcode (";", "genlshTwo "););

  size = getDataSize (result);

  /* if shCount >= 8 */
  if (shCount >= 8)
  {
      shCount -= 8;

      _startLazyDPSEvaluation();

      if (size > 1)
	{
	  if (shCount)
	  {
	    _endLazyDPSEvaluation();
	    shiftL1Left2Result (left, LSB, result, MSB16, shCount);
      	    aopPut (AOP (result), zero, LSB);	    
	  }
	  else
	  {
	    movLeft2Result (left, LSB, result, MSB16, 0);
	    aopPut (AOP (result), zero, LSB);
	    _endLazyDPSEvaluation();
	  }
	}
	else
	{
	  aopPut (AOP (result), zero, LSB);
	  _endLazyDPSEvaluation();
	}
  }

  /*  1 <= shCount <= 7 */
  else
    {
      if (size == 1)
      {
	shiftL1Left2Result (left, LSB, result, LSB, shCount);
      }
      else
      {
	shiftL2Left2Result (left, LSB, result, LSB, shCount);
      }
    }
}
#endif

#if 0
//REMOVE ME!!!
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
      l = aopGet (AOP (left), LSB, FALSE, FALSE, NULL);
      MOVA (l);
      emitcode ("add", "a,acc");
      if (sameRegs (AOP (left), AOP (result)) &&
	  size >= MSB16 + offr && offr != LSB)
	emitcode ("xch", "a,%s",
		  aopGet (AOP (left), LSB + offr, FALSE, FALSE, DP2_RESULT_REG));
      else
	aopPut (AOP (result), "a", LSB + offr);
    }

  if (size >= MSB16 + offr)
    {
      if (!(sameRegs (AOP (result), AOP (left)) && size >= MSB16 + offr && offr != LSB))
	{
	  MOVA (aopGet (AOP (left), MSB16, FALSE, FALSE, TRUE));
	}
      emitcode ("rlc", "a");
      if (sameRegs (AOP (left), AOP (result)) &&
	  size >= MSB24 + offr && offr != LSB)
	emitcode ("xch", "a,%s",
		  aopGet (AOP (left), MSB16 + offr, FALSE, FALSE, DP2_RESULT_REG));
      else
	aopPut (AOP (result), "a", MSB16 + offr);
    }

  if (size >= MSB24 + offr)
    {
      if (!(sameRegs (AOP (left), AOP (left)) && size >= MSB24 + offr && offr != LSB))
	{
	  MOVA (aopGet (AOP (left), MSB24, FALSE, FALSE, NULL));
	}
      emitcode ("rlc", "a");
      if (sameRegs (AOP (left), AOP (result)) &&
	  size >= MSB32 + offr && offr != LSB)
	emitcode ("xch", "a,%s",
		  aopGet (AOP (left), MSB24 + offr, FALSE, FALSE, DP2_RESULT_REG));
      else
	aopPut (AOP (result), "a", MSB24 + offr);
    }

  if (size > MSB32 + offr)
    {
      if (!(sameRegs (AOP (result), AOP (left)) && size >= MSB32 + offr && offr != LSB))
	{
	  MOVA (aopGet (AOP (left), MSB32, FALSE, FALSE, NULL));
	}
      emitcode ("rlc", "a");
      aopPut (AOP (result), "a", MSB32 + offr);
    }
  if (offr != LSB)
    aopPut (AOP (result), zero, LSB);
}
#endif

#if 0
//REMOVE ME!!!
/*-----------------------------------------------------------------*/
/* genlshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genlshFour (operand * result, operand * left, int shCount)
{
  int size;

  D (emitcode (";", "genlshFour ");
    );

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
      aopPut (AOP (result), zero, LSB);
      aopPut (AOP (result), zero, MSB16);
      aopPut (AOP (result), zero, MSB24);
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
      aopPut (AOP (result), zero, MSB16);
      aopPut (AOP (result), zero, LSB);
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
	{			/* size = 4 */
	  if (shCount == 0)
	    {
	      movLeft2Result (left, MSB24, result, MSB32, 0);
	      movLeft2Result (left, MSB16, result, MSB24, 0);
	      movLeft2Result (left, LSB, result, MSB16, 0);
	      aopPut (AOP (result), zero, LSB);
	    }
	  else if (shCount == 1)
	    shiftLLong (left, result, MSB16);
	  else
	    {
	      shiftL2Left2Result (left, MSB16, result, MSB24, shCount);
	      shiftL1Left2Result (left, LSB, result, MSB16, shCount);
	      shiftRLeftOrResult (left, LSB, result, MSB24, 8 - shCount);
	      aopPut (AOP (result), zero, LSB);
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
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genLeftShiftLiteral - left shifting by known count              */
/*-----------------------------------------------------------------*/
static bool
genLeftShiftLiteral (operand * left,
		     operand * right,
		     operand * result,
		     iCode * ic)
{
  int shCount = (int) floatFromVal (AOP (right)->aopu.aop_lit);
  int size;

  size = getSize (operandType (result));

  D(emitcode (";", "genLeftShiftLiteral (%d), size %d", shCount, size););

  /* We only handle certain easy cases so far. */
  if ((shCount != 0)
   && (shCount < (size * 8))
   && (size != 1)
   && (size != 2))
  {
      D(emitcode (";", "genLeftShiftLiteral wimping out"););	
      return FALSE;
  }

  freeAsmop (right, NULL, ic, TRUE);

  aopOp(left, ic, FALSE, FALSE);
  aopOp(result, ic, FALSE, AOP_USESDPTR(left));

#if 0 // debug spew
  if (IS_SYMOP(left) && OP_SYMBOL(left)->aop)
  {
  	emitcode(";", "left (%s) is %d", OP_SYMBOL(left)->rname, AOP_TYPE(left));
  	if (!IS_TRUE_SYMOP(left) && OP_SYMBOL(left)->usl.spillLoc)
  	{
  	   emitcode(";", "\taka %s", OP_SYMBOL(left)->usl.spillLoc->rname);
  	}
  }
  if (IS_SYMOP(result) && OP_SYMBOL(result)->aop)
  {
  	emitcode(";", "result (%s) is %d", OP_SYMBOL(result)->rname, AOP_TYPE(result));
  	if (!IS_TRUE_SYMOP(result) && OP_SYMBOL(result)->usl.spillLoc)
  	{
  	   emitcode(";", "\taka %s", OP_SYMBOL(result)->usl.spillLoc->rname);
  	}  	
  }  
#endif
  
#if VIEW_SIZE
  emitcode ("; shift left ", "result %d, left %d", size,
	    AOP_SIZE (left));
#endif

  /* I suppose that the left size >= result size */
  if (shCount == 0)
  {
  	_startLazyDPSEvaluation();
      	while (size--)
	{
	  movLeft2Result (left, size, result, size, 0);
	}
	_endLazyDPSEvaluation();
  }
  else if (shCount >= (size * 8))
  {
    _startLazyDPSEvaluation();
    while (size--)
    {
      aopPut (AOP (result), zero, size);
    }
    _endLazyDPSEvaluation();
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
#if 0
	case 4:
	  genlshFour (result, left, shCount);
	  break;
#endif
	default:
	  fprintf(stderr, "*** ack! mystery literal shift!\n");	  
	  break;
	}
    }
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
  return TRUE;
}
#endif

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

  D (emitcode (";", "genLeftShift "););

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, FALSE, FALSE);


#ifdef BETTER_LITERAL_SHIFT
  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      if (genLeftShiftLiteral (left, right, result, ic))
      {
      	return;
      }
    }
#endif

  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  if (AOP_TYPE (right) == AOP_LIT)
  {
      /* Really should be handled by genLeftShiftLiteral,
       * but since I'm too lazy to fix that today, at least we can make
       * some small improvement.
       */
       emitcode("mov", "b,#!constbyte",
       	        ((int) floatFromVal (AOP (right)->aopu.aop_lit)) + 1);
  }
  else
  {
      MOVB(aopGet (AOP (right), 0, FALSE, FALSE, "b"));
      emitcode ("inc", "b");
  }
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

  /* now move the left to the result if they are not the
     same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {

      size = AOP_SIZE (result);
      offset = 0;
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  l = aopGet (AOP (left), offset, FALSE, TRUE, DP2_RESULT_REG);
	  if (*l == '@' && (IS_AOP_PREG (result)))
	    {

	      emitcode ("mov", "a,%s", l);
	      aopPut (AOP (result), "a", offset);
	    }
	  else
	    aopPut (AOP (result), l, offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
    }

  tlbl = newiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = 0;
  tlbl1 = newiTempLabel (NULL);

  /* if it is only one byte then */
  if (size == 1)
    {
      symbol *tlbl1 = newiTempLabel (NULL);

      MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));
      emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      emitcode ("add", "a,acc");
      emitcode ("", "!tlabeldef", tlbl1->key + 100);
      emitcode ("djnz", "b,!tlabel", tlbl->key + 100);
      aopPut (AOP (result), "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));

  emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
  emitcode ("", "!tlabeldef", tlbl->key + 100);
  MOVA (aopGet (AOP (result), offset, FALSE, FALSE, NULL));
  emitcode ("add", "a,acc");
  aopPut (AOP (result), "a", offset++);
  _startLazyDPSEvaluation ();
  while (--size)
    {
      MOVA (aopGet (AOP (result), offset, FALSE, FALSE, NULL));
      emitcode ("rlc", "a");
      aopPut (AOP (result), "a", offset++);
    }
  _endLazyDPSEvaluation ();
  reAdjustPreg (AOP (result));

  emitcode ("", "!tlabeldef", tlbl1->key + 100);
  emitcode ("djnz", "b,!tlabel", tlbl->key + 100);
release:
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genrshOne - right shift a one byte quantity by known count      */
/*-----------------------------------------------------------------*/
static void
genrshOne (operand * result, operand * left,
	   int shCount, int sign)
{
  D (emitcode (";", "genrshOne"););
  shiftR1Left2Result (left, LSB, result, LSB, shCount, sign);
}
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genrshTwo - right shift two bytes by known amount != 0          */
/*-----------------------------------------------------------------*/
static void
genrshTwo (operand * result, operand * left,
	   int shCount, int sign)
{
  D (emitcode (";", "genrshTwo"););

  /* if shCount >= 8 */
  if (shCount >= 8)
    {
      shCount -= 8;
      _startLazyDPSEvaluation();
      if (shCount)
      {
	shiftR1Left2Result (left, MSB16, result, LSB,
			    shCount, sign);
      }			    
      else
      {
	movLeft2Result (left, MSB16, result, LSB, sign);
      }
      addSign (result, MSB16, sign);
      _endLazyDPSEvaluation();
    }

  /*  1 <= shCount <= 7 */
  else
  {
    shiftR2Left2Result (left, LSB, result, LSB, shCount, sign);
  }
}
#endif

#if 0
//REMOVE ME!!!
/*-----------------------------------------------------------------*/
/* shiftRLong - shift right one long from left to result           */
/* offl = LSB or MSB16                                             */
/*-----------------------------------------------------------------*/
static void
shiftRLong (operand * left, int offl,
	    operand * result, int sign)
{
  int isSameRegs=sameRegs(AOP(left),AOP(result));

  if (isSameRegs && offl>1) {
    // we are in big trouble, but this shouldn't happen
    werror(E_INTERNAL_ERROR, __FILE__, __LINE__);
  }

  MOVA (aopGet (AOP (left), MSB32, FALSE, NULL));
  
  if (offl==MSB16) {
    // shift is > 8
    if (sign) {
      emitcode ("rlc", "a");
      emitcode ("subb", "a,acc");
      emitcode ("xch", "a,%s",
		aopGet(AOP(left), MSB32, FALSE, DP2_RESULT_REG));
    } else {
      aopPut (AOP(result), zero, MSB32);
    }
  }

  if (!sign) {
    emitcode ("clr", "c");
  } else {
    emitcode ("mov", "c,acc.7");
  }

  emitcode ("rrc", "a");

  if (isSameRegs && offl==MSB16) {
    emitcode ("xch",
	      "a,%s",aopGet (AOP (left), MSB24, FALSE, DP2_RESULT_REG));
  } else {
    aopPut (AOP (result), "a", MSB32);
    MOVA (aopGet (AOP (left), MSB24, FALSE, NULL));
  }

  emitcode ("rrc", "a");
  if (isSameRegs && offl==1) {
    emitcode ("xch", "a,%s",
	      aopGet (AOP (left), MSB16, FALSE, DP2_RESULT_REG));
  } else {
    aopPut (AOP (result), "a", MSB24);
    MOVA (aopGet (AOP (left), MSB16, FALSE, NULL));
  }
  emitcode ("rrc", "a");
  aopPut (AOP (result), "a", MSB16 - offl);

  if (offl == LSB)
    {
      MOVA (aopGet (AOP (left), LSB, FALSE, NULL));
      emitcode ("rrc", "a");
      aopPut (AOP (result), "a", LSB);
    }
}
#endif

#if 0
//REMOVE ME!!!
/*-----------------------------------------------------------------*/
/* genrshFour - shift four byte by a known amount != 0             */
/*-----------------------------------------------------------------*/
static void
genrshFour (operand * result, operand * left,
	    int shCount, int sign)
{
  D (emitcode (";", "genrshFour");
    );

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
	shiftRLong (left, MSB16, result, sign);
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
    {				/* 1 <= shCount <= 7 */
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
#endif

#ifdef BETTER_LITERAL_SHIFT
/*-----------------------------------------------------------------*/
/* genRightShiftLiteral - right shifting by known count            */
/*-----------------------------------------------------------------*/
static bool
genRightShiftLiteral (operand * left,
		      operand * right,
		      operand * result,
		      iCode * ic,
		      int sign)
{
  int shCount = (int) floatFromVal (AOP (right)->aopu.aop_lit);
  int size;

  size = getSize (operandType (result));

  D(emitcode (";", "genRightShiftLiteral (%d), size %d", shCount, size););

  /* We only handle certain easy cases so far. */
  if ((shCount != 0)
   && (shCount < (size * 8))
   && (size != 1)
   && (size != 2))
  {
      D(emitcode (";", "genRightShiftLiteral wimping out"););	
      return FALSE;
  }

  freeAsmop (right, NULL, ic, TRUE);

  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

#if VIEW_SIZE
  emitcode ("; shift right ", "result %d, left %d", AOP_SIZE (result),
	    AOP_SIZE (left));
#endif

  /* test the LEFT size !!! */

  /* I suppose that the left size >= result size */
  if (shCount == 0)
  {
      size = getDataSize (result);
      _startLazyDPSEvaluation();
      while (size--)
      {
	movLeft2Result (left, size, result, size, 0);
      }
      _endLazyDPSEvaluation();
  }
  else if (shCount >= (size * 8))
    {
      if (sign)
      {
	/* get sign in acc.7 */
	MOVA (aopGet (AOP (left), size - 1, FALSE, FALSE, NULL));
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
#if 0
	case 4:
	  genrshFour (result, left, shCount, sign);
	  break;
#endif	  
	default:
	  break;
	}
    }
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
  
  return TRUE;
}
#endif

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

  D (emitcode (";", "genSignedRightShift "););

  /* we do it the hard way put the shift count in b
     and loop thru preserving the sign */

  right = IC_RIGHT (ic);
  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

  aopOp (right, ic, FALSE, FALSE);

#ifdef BETTER_LITERAL_SHIFT
  if (AOP_TYPE (right) == AOP_LIT)
    {
      if (genRightShiftLiteral (left, right, result, ic, 1))
      {
      	return;
      }
    }
#endif
  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */

  if (AOP_TYPE (right) == AOP_LIT)
  {
      /* Really should be handled by genRightShiftLiteral,
       * but since I'm too lazy to fix that today, at least we can make
       * some small improvement.
       */
       emitcode("mov", "b,#!constbyte",
       	        ((int) floatFromVal (AOP (right)->aopu.aop_lit)) + 1);
  }
  else
  {
  	MOVB(aopGet (AOP (right), 0, FALSE, FALSE, "b"));
  	emitcode ("inc", "b");
  }
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left)); 

  /* now move the left to the result if they are not the
     same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {

      size = AOP_SIZE (result);
      offset = 0;
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  l = aopGet (AOP (left), offset, FALSE, TRUE, NULL);
	  if (*l == '@' && IS_AOP_PREG (result))
	    {

	      emitcode ("mov", "a,%s", l);
	      aopPut (AOP (result), "a", offset);
	    }
	  else
	    aopPut (AOP (result), l, offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
    }

  /* mov the highest order bit to OVR */
  tlbl = newiTempLabel (NULL);
  tlbl1 = newiTempLabel (NULL);

  size = AOP_SIZE (result);
  offset = size - 1;
  MOVA (aopGet (AOP (left), offset, FALSE, FALSE, NULL));
  emitcode ("rlc", "a");
  emitcode ("mov", "ov,c");
  /* if it is only one byte then */
  if (size == 1)
    {
      MOVA( aopGet (AOP (left), 0, FALSE, FALSE, NULL));
      emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      emitcode ("mov", "c,ov");
      emitcode ("rrc", "a");
      emitcode ("", "!tlabeldef", tlbl1->key + 100);
      emitcode ("djnz", "b,!tlabel", tlbl->key + 100);
      aopPut (AOP (result), "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));
  emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
  emitcode ("", "!tlabeldef", tlbl->key + 100);
  emitcode ("mov", "c,ov");
  _startLazyDPSEvaluation ();
  while (size--)
    {
      MOVA (aopGet (AOP (result), offset, FALSE, FALSE, NULL));
      emitcode ("rrc", "a");
      aopPut (AOP (result), "a", offset--);
    }
  _endLazyDPSEvaluation ();
  reAdjustPreg (AOP (result));
  emitcode ("", "!tlabeldef", tlbl1->key + 100);
  emitcode ("djnz", "b,!tlabel", tlbl->key + 100);

release:
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genRightShift - generate code for right shifting                */
/*-----------------------------------------------------------------*/
static void
genRightShift (iCode * ic)
{
  operand *right, *left, *result;
  sym_link *retype;
  int size, offset;
  char *l;
  symbol *tlbl, *tlbl1;

  D (emitcode (";", "genRightShift "););

  /* if signed then we do it the hard way preserve the
     sign bit moving it inwards */
  retype = getSpec (operandType (IC_RESULT (ic)));

  if (!SPEC_USIGN (retype))
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

  aopOp (right, ic, FALSE, FALSE);

#ifdef BETTER_LITERAL_SHIFT
  /* if the shift count is known then do it
     as efficiently as possible */
  if (AOP_TYPE (right) == AOP_LIT)
    {
      if (genRightShiftLiteral (left, right, result, ic, 0))
      {
      	return;
      }
    }
#endif

  /* shift count is unknown then we have to form
     a loop get the loop count in B : Note: we take
     only the lower order byte since shifting
     more that 32 bits make no sense anyway, ( the
     largest size of an object can be only 32 bits ) */
  
  if (AOP_TYPE (right) == AOP_LIT)
  {
      /* Really should be handled by genRightShiftLiteral,
       * but since I'm too lazy to fix that today, at least we can make
       * some small improvement.
       */
       emitcode("mov", "b,#!constbyte",
       	        ((int) floatFromVal (AOP (right)->aopu.aop_lit)) + 1);
  }
  else
  {
      MOVB(aopGet (AOP (right), 0, FALSE, FALSE, "b"));
      emitcode ("inc", "b");
  }
  freeAsmop (right, NULL, ic, TRUE);
  aopOp (left, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(left));

  /* now move the left to the result if they are not the
     same */
  if (!sameRegs (AOP (left), AOP (result)) &&
      AOP_SIZE (result) > 1)
    {

      size = AOP_SIZE (result);
      offset = 0;
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  l = aopGet (AOP (left), offset, FALSE, TRUE, NULL);
	  if (*l == '@' && IS_AOP_PREG (result))
	    {

	      emitcode ("mov", "a,%s", l);
	      aopPut (AOP (result), "a", offset);
	    }
	  else
	    aopPut (AOP (result), l, offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
    }

  tlbl = newiTempLabel (NULL);
  tlbl1 = newiTempLabel (NULL);
  size = AOP_SIZE (result);
  offset = size - 1;

  /* if it is only one byte then */
  if (size == 1)
    {
      MOVA (aopGet (AOP (left), 0, FALSE, FALSE, NULL));
      emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
      emitcode ("", "!tlabeldef", tlbl->key + 100);
      CLRC;
      emitcode ("rrc", "a");
      emitcode ("", "!tlabeldef", tlbl1->key + 100);
      emitcode ("djnz", "b,!tlabel", tlbl->key + 100);
      aopPut (AOP (result), "a", 0);
      goto release;
    }

  reAdjustPreg (AOP (result));
  emitcode ("sjmp", "!tlabel", tlbl1->key + 100);
  emitcode ("", "!tlabeldef", tlbl->key + 100);
  CLRC;
  _startLazyDPSEvaluation ();
  while (size--)
    {
      MOVA (aopGet (AOP (result), offset, FALSE, FALSE, NULL));
      emitcode ("rrc", "a");
      aopPut (AOP (result), "a", offset--);
    }
  _endLazyDPSEvaluation ();
  reAdjustPreg (AOP (result));

  emitcode ("", "!tlabeldef", tlbl1->key + 100);
  emitcode ("djnz", "b,!tlabel", tlbl->key + 100);

release:
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genUnpackBits - generates code for unpacking bits               */
/*-----------------------------------------------------------------*/
static void
genUnpackBits (operand * result, char *rname, int ptype)
{
  int shCnt;
  int rlen;
  sym_link *etype;
  int offset = 0;

  D (emitcode (";", "genUnpackBits "););

  etype = getSpec (operandType (result));

  /* read the first byte  */
  switch (ptype)
    {

    case POINTER:
    case IPOINTER:
      emitcode ("mov", "a,@%s", rname);
      break;

    case PPOINTER:
      emitcode ("movx", "a,@%s", rname);
      break;

    case FPOINTER:
      emitcode ("movx", "a,@dptr");
      break;

    case CPOINTER:
      emitcode ("clr", "a");
      emitcode ("movc", "a,@a+dptr");
      break;

    case GPOINTER:
      emitcode ("lcall", "__gptrget");
      break;
    }

  /* if we have bitdisplacement then it fits   */
  /* into this byte completely or if length is */
  /* less than a byte                          */
  if (((shCnt = SPEC_BSTR (etype)) != 0) || (SPEC_BLEN (etype) <= 8))
    {

      /* shift right acc */
      AccRsh (shCnt);

      emitcode ("anl", "a,#!constbyte",
		((unsigned char) -1) >> (8 - SPEC_BLEN (etype)));
      aopPut (AOP (result), "a", offset);
      return;
    }

  /* bit field did not fit in a byte  */
  rlen = SPEC_BLEN (etype) - 8;
  aopPut (AOP (result), "a", offset++);

  while (1)
    {

      switch (ptype)
	{
	case POINTER:
	case IPOINTER:
	  emitcode ("inc", "%s", rname);
	  emitcode ("mov", "a,@%s", rname);
	  break;

	case PPOINTER:
	  emitcode ("inc", "%s", rname);
	  emitcode ("movx", "a,@%s", rname);
	  break;

	case FPOINTER:
	  emitcode ("inc", "dptr");
	  emitcode ("movx", "a,@dptr");
	  break;

	case CPOINTER:
	  emitcode ("clr", "a");
	  emitcode ("inc", "dptr");
	  emitcode ("movc", "a,@a+dptr");
	  break;

	case GPOINTER:
	  emitcode ("inc", "dptr");
	  emitcode ("lcall", "__gptrget");
	  break;
	}

      rlen -= 8;
      /* if we are done */
      if (rlen < 8)
	break;

      aopPut (AOP (result), "a", offset++);

    }

  if (rlen)
    {
      emitcode ("anl", "a,#!constbyte", ((unsigned char) -1) >> (rlen));
      aopPut (AOP (result), "a", offset);
    }

  return;
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
  char buff[256];
  int size, offset = 0;
  aopOp (result, ic, TRUE, FALSE);

  /* get the string representation of the name */
  l = aopGet (AOP (left), 0, FALSE, TRUE, NULL);
  size = AOP_SIZE (result);
  _startLazyDPSEvaluation ();
  while (size--)
    {
	if (offset)
	{
	    SNPRINTF (buff, sizeof(buff), 
		      "(%s + %d)", l + 1, offset);
	}
	else
	{
	    SNPRINTF (buff, sizeof(buff), 
		      "%s", l + 1);
	}
      aopPut (AOP (result), buff, offset++);
    }
  _endLazyDPSEvaluation ();

  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genNearPointerGet - emitcode for near pointer fetch             */
/*-----------------------------------------------------------------*/
static void
genNearPointerGet (operand * left,
		   operand * result,
		   iCode * ic,
		   iCode *pi)
{
  asmop *aop = NULL;
  regs *preg;
  char *rname;
  sym_link *rtype, *retype, *letype;
  sym_link *ltype = operandType (left);
  char buff[80];

  rtype = operandType (result);
  retype = getSpec (rtype);
  letype = getSpec (ltype);

  aopOp (left, ic, FALSE, FALSE);

  /* if left is rematerialisable and
     result is not bit variable type and
     the left is pointer to data space i.e
     lower 128 bytes of space */
  if (AOP_TYPE (left) == AOP_IMMD &&
      !IS_BITVAR (retype) &&
      !IS_BITVAR (letype) &&
      DCL_TYPE (ltype) == POINTER)
    {
      genDataPointerGet (left, result, ic);
      return;
    }

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (left)))
    {
      /* otherwise get a free pointer register */
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
		preg->name,
		aopGet (AOP (left), 0, FALSE, TRUE, DP2_RESULT_REG));
      rname = preg->name;
    }
  else
    rname = aopGet (AOP (left), 0, FALSE, FALSE, DP2_RESULT_REG);

  freeAsmop (left, NULL, ic, TRUE);
  aopOp (result, ic, FALSE, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    genUnpackBits (result, rname, POINTER);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (result);
      int offset = 0;

      while (size--)
	{
	  if (IS_AOP_PREG (result) || AOP_TYPE (result) == AOP_STK)
	    {

	      emitcode ("mov", "a,@%s", rname);
	      aopPut (AOP (result), "a", offset);
	    }
	  else
	    {
	      SNPRINTF (buff, sizeof(buff), "@%s", rname);
	      aopPut (AOP (result), buff, offset);
	    }
	  offset++;
	  if (size || pi)
	    {
		emitcode ("inc", "%s", rname);
	    }
	}
    }

  /* now some housekeeping stuff */
  if (aop)
    {
      /* we had to allocate for this iCode */
      if (pi) { /* post increment present */
	aopPut(AOP ( left ),rname,0);
      }
      freeAsmop (NULL, aop, ic, TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if (AOP_SIZE (result) > 1 &&
	  !OP_SYMBOL (left)->remat &&
	  (OP_SYMBOL (left)->liveTo > ic->seq ||
	   ic->depth) &&
	  !pi)
	{
	  int size = AOP_SIZE (result) - 1;
	  while (size--)
	    emitcode ("dec", "%s", rname);
	}
    }

  /* done */
  freeAsmop (result, NULL, ic, TRUE);
  if (pi) pi->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genPagedPointerGet - emitcode for paged pointer fetch           */
/*-----------------------------------------------------------------*/
static void
genPagedPointerGet (operand * left,
		    operand * result,
		    iCode * ic,
		    iCode * pi)
{
  asmop *aop = NULL;
  regs *preg;
  char *rname;
  sym_link *rtype, *retype, *letype;

  rtype = operandType (result);
  retype = getSpec (rtype);
  letype = getSpec (operandType (left));
  aopOp (left, ic, FALSE, FALSE);

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (left)))
    {
      /* otherwise get a free pointer register */
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
		preg->name,
		aopGet (AOP (left), 0, FALSE, TRUE, NULL));
      rname = preg->name;
    }
  else
    rname = aopGet (AOP (left), 0, FALSE, FALSE, NULL);

  freeAsmop (left, NULL, ic, TRUE);
  aopOp (result, ic, FALSE, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    genUnpackBits (result, rname, PPOINTER);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (result);
      int offset = 0;

      while (size--)
	{

	  emitcode ("movx", "a,@%s", rname);
	  aopPut (AOP (result), "a", offset);

	  offset++;

	  if (size || pi)
	    emitcode ("inc", "%s", rname);
	}
    }

  /* now some housekeeping stuff */
  if (aop)
    {
      /* we had to allocate for this iCode */
      if (pi) aopPut ( AOP (left), rname, 0);
      freeAsmop (NULL, aop, ic, TRUE);
    }
  else
    {
      /* we did not allocate which means left
         already in a pointer register, then
         if size > 0 && this could be used again
         we have to point it back to where it
         belongs */
      if (AOP_SIZE (result) > 1 &&
	  !OP_SYMBOL (left)->remat &&
	  (OP_SYMBOL (left)->liveTo > ic->seq ||
	   ic->depth) &&
	  !pi)
	{
	  int size = AOP_SIZE (result) - 1;
	  while (size--)
	    emitcode ("dec", "%s", rname);
	}
    }

  /* done */
  freeAsmop (result, NULL, ic, TRUE);
  if (pi) pi->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genFarPointerGet - gget value from far space                    */
/*-----------------------------------------------------------------*/
static void
genFarPointerGet (operand * left,
		  operand * result, iCode * ic, iCode *pi)
{
    int size, offset, dopi=1;
  sym_link *retype = getSpec (operandType (result));
  sym_link *letype = getSpec (operandType (left));
  D (emitcode (";", "genFarPointerGet"););

  aopOp (left, ic, FALSE, FALSE);

  /* if the operand is already in dptr
     then we do nothing else we move the value to dptr */
  if (AOP_TYPE (left) != AOP_STR && !AOP_INDPTRn(left) )
    {
      /* if this is remateriazable */
      if (AOP_TYPE (left) == AOP_IMMD)
	{
	  emitcode ("mov", "dptr,%s", aopGet (AOP (left), 0, TRUE, FALSE, NULL));
	}
      else
	{
	  /* we need to get it byte by byte */
	  _startLazyDPSEvaluation ();
	  if (AOP_TYPE (left) != AOP_DPTR)
	    {
	      emitcode ("mov", "dpl,%s", aopGet (AOP (left), 0, FALSE, FALSE, NULL));
	      emitcode ("mov", "dph,%s", aopGet (AOP (left), 1, FALSE, FALSE, NULL));
	      if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (left), 2, FALSE, FALSE, NULL));
	    }
	  else
	    {
	      /* We need to generate a load to DPTR indirect through DPTR. */
	      D (emitcode (";", "genFarPointerGet -- indirection special case.");
		);
	      emitcode ("push", "%s", aopGet (AOP (left), 0, FALSE, TRUE, NULL));
	      emitcode ("push", "%s", aopGet (AOP (left), 1, FALSE, TRUE, NULL));
	      if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (left), 2, FALSE, FALSE, NULL));
	      emitcode ("pop", "dph");
	      emitcode ("pop", "dpl");
	      dopi =0;
	    }
	  _endLazyDPSEvaluation ();
	}
    }
  /* so dptr know contains the address */
  aopOp (result, ic, FALSE, (AOP_INDPTRn(left) ? FALSE : TRUE));

  /* if bit then unpack */
  if (IS_BITVAR (retype) || IS_BITVAR (letype)) {
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(AOP(left)->aopu.dptr);
      }
      genUnpackBits (result, "dptr", FPOINTER);
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(0);
      }
  } else
    {
      size = AOP_SIZE (result);
      offset = 0;

      if (AOP_INDPTRn(left) && AOP_USESDPTR(result)) {
	  while (size--) {
	      genSetDPTR(AOP(left)->aopu.dptr);
	      emitcode ("movx", "a,@dptr");
	      if (size || (dopi && pi && AOP_TYPE (left) != AOP_IMMD))
		  emitcode ("inc", "dptr");
	      genSetDPTR (0);
	      aopPut (AOP (result), "a", offset++);
	  }
      } else {
	  _startLazyDPSEvaluation ();
	  while (size--) {
	      if (AOP_INDPTRn(left)) {
		  genSetDPTR(AOP(left)->aopu.dptr);
	      } else {
		  genSetDPTR (0);
	      }
	      _flushLazyDPS ();
	      
	      emitcode ("movx", "a,@dptr");
	      if (size || (dopi && pi && AOP_TYPE (left) != AOP_IMMD))
		  emitcode ("inc", "dptr");
	      
	      aopPut (AOP (result), "a", offset++);
	  }
	  _endLazyDPSEvaluation ();
      }
    }
  if (dopi && pi && AOP_TYPE (left) != AOP_IMMD) {
      if (!AOP_INDPTRn(left)) {
	  aopPut ( AOP (left), "dpl", 0);
	  aopPut ( AOP (left), "dph", 1);
	  if (options.model == MODEL_FLAT24)
	      aopPut ( AOP (left), "dpx", 2);
      }
    pi->generated = 1;
  } else if ((OP_SYMBOL(left)->ruonly || AOP_INDPTRn(left)) && 
	     AOP_SIZE(result) > 1 &&
	     (OP_SYMBOL (left)->liveTo > ic->seq || ic->depth)) {
      
      size = AOP_SIZE (result) - 1;
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(AOP(left)->aopu.dptr);
      }
      while (size--) emitcode ("lcall","__decdptr");
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(0);
      }
  }

  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genCodePointerGet - get value from code space                  */
/*-----------------------------------------------------------------*/
static void
genCodePointerGet (operand * left,
		    operand * result, iCode * ic, iCode *pi)
{
  int size, offset, dopi=1;
  sym_link *retype = getSpec (operandType (result));

  aopOp (left, ic, FALSE, FALSE);

  /* if the operand is already in dptr
     then we do nothing else we move the value to dptr */
  if (AOP_TYPE (left) != AOP_STR && !AOP_INDPTRn(left))
    {
      /* if this is remateriazable */
      if (AOP_TYPE (left) == AOP_IMMD)
	{
	  emitcode ("mov", "dptr,%s", aopGet (AOP (left), 0, TRUE, FALSE, NULL));
	}
      else
	{			/* we need to get it byte by byte */
	  _startLazyDPSEvaluation ();
	  if (AOP_TYPE (left) != AOP_DPTR)
	    {
	      emitcode ("mov", "dpl,%s", aopGet (AOP (left), 0, FALSE, FALSE, NULL));
	      emitcode ("mov", "dph,%s", aopGet (AOP (left), 1, FALSE, FALSE, NULL));
	      if (options.model == MODEL_FLAT24)
	        emitcode ("mov", "dpx,%s", aopGet (AOP (left), 2, FALSE, FALSE, NULL));
	    }
	  else
	    {
	      /* We need to generate a load to DPTR indirect through DPTR. */
	      D (emitcode (";", "gencodePointerGet -- indirection special case.");
		);
	      emitcode ("push", "%s", aopGet (AOP (left), 0, FALSE, TRUE, NULL));
	      emitcode ("push", "%s", aopGet (AOP (left), 1, FALSE, TRUE, NULL));
	      if (options.model == MODEL_FLAT24)
	        emitcode ("mov", "dpx,%s", aopGet (AOP (left), 2, FALSE, FALSE, NULL));
	      emitcode ("pop", "dph");
	      emitcode ("pop", "dpl");
	      dopi=0;
	    }
	  _endLazyDPSEvaluation ();
	}
    }
  /* so dptr know contains the address */
  aopOp (result, ic, FALSE, (AOP_INDPTRn(left) ? FALSE : TRUE));

  /* if bit then unpack */
  if (IS_BITVAR (retype)) {
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(AOP(left)->aopu.dptr);
      }
      genUnpackBits (result, "dptr", CPOINTER);
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(0);
      }
  } else
    {
      size = AOP_SIZE (result);
      offset = 0;
      if (AOP_INDPTRn(left) && AOP_USESDPTR(result)) {
	  while (size--) {
	      genSetDPTR(AOP(left)->aopu.dptr);
	      emitcode ("clr", "a");
	      emitcode ("movc", "a,@a+dptr");
	      if (size || (dopi && pi && AOP_TYPE (left) != AOP_IMMD))
		  emitcode ("inc", "dptr");
	      genSetDPTR (0);
	      aopPut (AOP (result), "a", offset++);
	  }
      } else {
	  _startLazyDPSEvaluation ();
	  while (size--)
	      {
		  if (AOP_INDPTRn(left)) {
		      genSetDPTR(AOP(left)->aopu.dptr);
		  } else {
		      genSetDPTR (0);
		  }
		  _flushLazyDPS ();
		  
		  emitcode ("clr", "a");
		  emitcode ("movc", "a,@a+dptr");
		  if (size || (dopi && pi && AOP_TYPE (left) != AOP_IMMD))
		      emitcode ("inc", "dptr");
		  aopPut (AOP (result), "a", offset++);
	      }
	  _endLazyDPSEvaluation ();
      }
    }
  if (dopi && pi && AOP_TYPE (left) != AOP_IMMD) {
      if (!AOP_INDPTRn(left)) {
	  aopPut ( AOP (left), "dpl", 0);
	  aopPut ( AOP (left), "dph", 1);
	  if (options.model == MODEL_FLAT24)
	      aopPut ( AOP (left), "dpx", 2);
      }
      pi->generated = 1;
  } else if ((OP_SYMBOL(left)->ruonly || AOP_INDPTRn(left)) && 
	     AOP_SIZE(result) > 1 &&
	     (OP_SYMBOL (left)->liveTo > ic->seq || ic->depth)) {
      
      size = AOP_SIZE (result) - 1;
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(AOP(left)->aopu.dptr);
      }
      while (size--) emitcode ("lcall","__decdptr");
      if (AOP_INDPTRn(left)) {
	  genSetDPTR(0);
      }
  }
  
  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGenPointerGet - gget value from generic pointer space        */
/*-----------------------------------------------------------------*/
static void
genGenPointerGet (operand * left,
		  operand * result, iCode * ic, iCode * pi)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (result));
  sym_link *letype = getSpec (operandType (left));

  D (emitcode (";", "genGenPointerGet "); );

  aopOp (left, ic, FALSE, (AOP_IS_STR(left) ? FALSE : TRUE));

  /* if the operand is already in dptr
     then we do nothing else we move the value to dptr */
  if (AOP_TYPE (left) != AOP_STR)
    {
      /* if this is remateriazable */
      if (AOP_TYPE (left) == AOP_IMMD)
	{
	  emitcode ("mov", "dptr,%s", aopGet (AOP (left), 0, TRUE, FALSE, NULL));
	  if (AOP(left)->aopu.aop_immd.from_cast_remat) 
	    {
		MOVB(aopGet(AOP (left), AOP_SIZE(left)-1, FALSE, FALSE, NULL));
	    }
	    else
	    {
		emitcode ("mov", "b,#%d", pointerCode (retype));
	    }
	}
      else
	{			/* we need to get it byte by byte */
	    _startLazyDPSEvaluation ();
	    emitcode ("mov", "dpl,%s", aopGet (AOP(left),0,FALSE,FALSE,NULL));
	    emitcode ("mov", "dph,%s", aopGet (AOP(left),1,FALSE,FALSE,NULL));
	    if (options.model == MODEL_FLAT24) {
		emitcode ("mov", "dpx,%s", aopGet (AOP(left),2,FALSE,FALSE,NULL));
		emitcode ("mov", "b,%s", aopGet (AOP(left),3,FALSE,FALSE,NULL));
	    } else {
		emitcode ("mov", "b,%s", aopGet (AOP(left),2,FALSE,FALSE,NULL));
	    }
	    _endLazyDPSEvaluation ();
	}
    }

  /* so dptr-b now contains the address */
  _G.bInUse++;
  aopOp (result, ic, FALSE, TRUE);
  _G.bInUse--;

  /* if bit then unpack */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
  {
    genUnpackBits (result, "dptr", GPOINTER);
  }
  else
    {
	size = AOP_SIZE (result);
	offset = 0;

	while (size--)
	{
	    if (size)
	    {
		// Get two bytes at a time, results in _AP & A.
		// dptr will be incremented ONCE by __gptrgetWord.
		//
		// Note: any change here must be coordinated
		// with the implementation of __gptrgetWord
		// in device/lib/_gptrget.c
		emitcode ("lcall", "__gptrgetWord");
		aopPut (AOP (result), DP2_RESULT_REG, offset++);
		aopPut (AOP (result), "a", offset++);
		size--;
	    }
	    else
	    {
		// Only one byte to get.
		emitcode ("lcall", "__gptrget");
		aopPut (AOP (result), "a", offset++);
	    }
	    
	    if (size || (pi && AOP_TYPE (left) != AOP_IMMD))
	    {
		emitcode ("inc", "dptr");
	    }
	}
    }

  if (pi && AOP_TYPE (left) != AOP_IMMD) {
    aopPut ( AOP (left), "dpl", 0);
    aopPut ( AOP (left), "dph", 1);
    if (options.model == MODEL_FLAT24) {
	aopPut ( AOP (left), "dpx", 2);
	aopPut ( AOP (left), "b", 3);	
    } else  aopPut ( AOP (left), "b", 2);	
    pi->generated = 1;
  } else if (OP_SYMBOL(left)->ruonly && AOP_SIZE(result) > 1 &&
	     (OP_SYMBOL (left)->liveTo > ic->seq || ic->depth)) {
      
      size = AOP_SIZE (result) - 1;
      while (size--) emitcode ("lcall","__decdptr");
  }

  freeAsmop (left, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void
genPointerGet (iCode * ic, iCode *pi)
{
  operand *left, *result;
  sym_link *type, *etype;
  int p_type;

  D (emitcode (";", "genPointerGet ");
    );

  left = IC_LEFT (ic);
  result = IC_RESULT (ic);

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
      IS_CAST_ICODE(OP_SYMBOL(left)->rematiCode)) {
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
      genNearPointerGet (left, result, ic, pi);
      break;

    case PPOINTER:
      genPagedPointerGet (left, result, ic, pi);
      break;

    case FPOINTER:
      genFarPointerGet (left, result, ic, pi);
      break;

    case CPOINTER:
      genCodePointerGet (left, result, ic, pi);
      break;

    case GPOINTER:
      genGenPointerGet (left, result, ic, pi);
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
  int offset = 0;
  int rLen;
  int blen, bstr;
  char *l;

  blen = SPEC_BLEN (etype);
  bstr = SPEC_BSTR (etype);

  MOVA (aopGet (AOP (right), offset++, FALSE, FALSE, NULL));

  /* if the bit lenth is less than or    */
  /* it exactly fits a byte then         */
  if (SPEC_BLEN (etype) <= 8)
    {
      /* shift left acc */
      AccLsh (SPEC_BSTR (etype));

      if (SPEC_BLEN (etype) < 8)
	{			/* if smaller than a byte */


	  switch (p_type)
	    {
	    case POINTER:
	      emitcode ("mov", "b,a");
	      emitcode ("mov", "a,@%s", rname);
	      break;

	    case FPOINTER:
	      emitcode ("mov", "b,a");
	      emitcode ("movx", "a,@dptr");
	      break;

	    case GPOINTER:
	      emitcode ("push", "b");
	      emitcode ("push", "acc");
	      emitcode ("lcall", "__gptrget");
	      emitcode ("pop", "b");
	      break;
	    }

	  emitcode ("anl", "a,#!constbyte", (unsigned char)
		    ((unsigned char) (0xFF << (blen + bstr)) |
		     (unsigned char) (0xFF >> (8 - bstr))));
	  emitcode ("orl", "a,b");
	  if (p_type == GPOINTER)
	    emitcode ("pop", "b");
	}
    }

  switch (p_type)
    {
    case POINTER:
      emitcode ("mov", "@%s,a", rname);
      break;

    case FPOINTER:
      emitcode ("movx", "@dptr,a");
      break;

    case GPOINTER:
      emitcode ("lcall", "__gptrput");
      break;
    }

  /* if we r done */
  if (SPEC_BLEN (etype) <= 8)
    return;

  emitcode ("inc", "%s", rname);
  rLen = SPEC_BLEN (etype);

  /* now generate for lengths greater than one byte */
  while (1)
    {

      l = aopGet (AOP (right), offset++, FALSE, TRUE, NULL);

      rLen -= 8;
      if (rLen < 8)
	break;

      switch (p_type)
	{
	case POINTER:
	  if (*l == '@')
	    {
	      MOVA (l);
	      emitcode ("mov", "@%s,a", rname);
	    }
	  else
	    emitcode ("mov", "@%s,%s", rname, l);
	  break;

	case FPOINTER:
	  MOVA (l);
	  emitcode ("movx", "@dptr,a");
	  break;

	case GPOINTER:
	  MOVA (l);
	  emitcode ("lcall", "__gptrput");
	  break;
	}
      emitcode ("inc", "%s", rname);
    }

  MOVA (l);

  /* last last was not complete */
  if (rLen)
    {
      /* save the byte & read byte */
      switch (p_type)
	{
	case POINTER:
	  emitcode ("mov", "b,a");
	  emitcode ("mov", "a,@%s", rname);
	  break;

	case FPOINTER:
	  emitcode ("mov", "b,a");
	  emitcode ("movx", "a,@dptr");
	  break;

	case GPOINTER:
	  emitcode ("push", "b");
	  emitcode ("push", "acc");
	  emitcode ("lcall", "__gptrget");
	  emitcode ("pop", "b");
	  break;
	}

      emitcode ("anl", "a,#!constbyte", ((unsigned char) -1 << rLen));
      emitcode ("orl", "a,b");
    }

  if (p_type == GPOINTER)
    emitcode ("pop", "b");

  switch (p_type)
    {

    case POINTER:
      emitcode ("mov", "@%s,a", rname);
      break;

    case FPOINTER:
      emitcode ("movx", "@dptr,a");
      break;

    case GPOINTER:
      emitcode ("lcall", "__gptrput");
      break;
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
  char *l, buff[256];

  aopOp (right, ic, FALSE, FALSE);

  l = aopGet (AOP (result), 0, FALSE, TRUE, NULL);
  size = AOP_SIZE (right);
  while (size--)
    {
      if (offset)
	{
	    SNPRINTF (buff, sizeof(buff), "(%s + %d)", l + 1, offset);
	}
      else
	{
	    SNPRINTF (buff, sizeof(buff), "%s", l + 1);
	}
	
      emitcode ("mov", "%s,%s", buff,
		aopGet (AOP (right), offset++, FALSE, FALSE, NULL));
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
  char *rname, *l;
  sym_link *retype, *letype;
  sym_link *ptype = operandType (result);

  retype = getSpec (operandType (right));
  letype = getSpec (ptype);

  aopOp (result, ic, FALSE, FALSE);

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
      /* otherwise get a free pointer register */
      regs *preg;
	
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
		preg->name,
		aopGet (AOP (result), 0, FALSE, TRUE, NULL));
      rname = preg->name;
    }
  else
    rname = aopGet (AOP (result), 0, FALSE, FALSE, NULL);

  aopOp (right, ic, FALSE, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    genPackBits ((IS_BITVAR (retype) ? retype : letype), right, rname, POINTER);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (right);
      int offset = 0;

      while (size--)
	{
	  l = aopGet (AOP (right), offset, FALSE, TRUE, NULL);
	  if (*l == '@')
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
  if (aop)
    {
      /* we had to allocate for this iCode */
      if (pi) aopPut (AOP (result),rname,0);
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
	   ic->depth) &&
	  !pi)
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
/* genPagedPointerSet - emitcode for Paged pointer put             */
/*-----------------------------------------------------------------*/
static void
genPagedPointerSet (operand * right,
		    operand * result,
		    iCode * ic,
		    iCode *pi)
{
  asmop *aop = NULL;
  char *rname;
  sym_link *retype, *letype;

  retype = getSpec (operandType (right));
  letype = getSpec (operandType (result));

  aopOp (result, ic, FALSE, FALSE);

  /* if the value is already in a pointer register
     then don't need anything more */
  if (!AOP_INPREG (AOP (result)))
    {
      /* otherwise get a free pointer register */
      regs *preg;
	
      aop = newAsmop (0);
      preg = getFreePtr (ic, &aop, FALSE);
      emitcode ("mov", "%s,%s",
		preg->name,
		aopGet (AOP (result), 0, FALSE, TRUE, NULL));
      rname = preg->name;
    }
  else
    rname = aopGet (AOP (result), 0, FALSE, FALSE, NULL);

  aopOp (right, ic, FALSE, FALSE);

  /* if bitfield then unpack the bits */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    genPackBits ((IS_BITVAR (retype) ? retype : letype), right, rname, PPOINTER);
  else
    {
      /* we have can just get the values */
      int size = AOP_SIZE (right);
      int offset = 0;

      while (size--)
	{
	  MOVA (aopGet (AOP (right), offset, FALSE, TRUE, NULL));

	  emitcode ("movx", "@%s,a", rname);

	  if (size || pi)
	    emitcode ("inc", "%s", rname);

	  offset++;
	}
    }

  /* now some housekeeping stuff */
  if (aop)
    {
      if (pi) aopPut (AOP (result),rname,0);
      /* we had to allocate for this iCode */
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
	   ic->depth) &&
	  !pi)
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
		  operand * result, iCode * ic, iCode *pi)
{
  int size, offset, dopi=1;
  sym_link *retype = getSpec (operandType (right));
  sym_link *letype = getSpec (operandType (result));

  aopOp (result, ic, FALSE, FALSE);

  /* if the operand is already in dptr
     then we do nothing else we move the value to dptr */
  if (AOP_TYPE (result) != AOP_STR && !AOP_INDPTRn(result))
    {
      /* if this is remateriazable */
      if (AOP_TYPE (result) == AOP_IMMD)
	emitcode ("mov", "dptr,%s", 
		  aopGet (AOP (result), 0, TRUE, FALSE, NULL));
      else
	{
	  /* we need to get it byte by byte */
	  _startLazyDPSEvaluation ();
	  if (AOP_TYPE (result) != AOP_DPTR)
	    {
	      emitcode ("mov", "dpl,%s", aopGet (AOP (result), 0, FALSE, FALSE, NULL));
	      emitcode ("mov", "dph,%s", aopGet (AOP (result), 1, FALSE, FALSE, NULL));
	      if (options.model == MODEL_FLAT24)
	        emitcode ("mov", "dpx,%s", aopGet (AOP (result), 2, FALSE, FALSE, NULL));
	    }
	  else
	    {
	      /* We need to generate a load to DPTR indirect through DPTR. */
	      D (emitcode (";", "genFarPointerSet -- indirection special case."););
		
	      emitcode ("push", "%s", aopGet (AOP (result), 0, FALSE, TRUE, NULL));
	      emitcode ("push", "%s", aopGet (AOP (result), 1, FALSE, TRUE, NULL));
	      if (options.model == MODEL_FLAT24)
	        emitcode ("mov", "dpx,%s", aopGet (AOP (result), 2, FALSE, FALSE, NULL));
	      emitcode ("pop", "dph");
	      emitcode ("pop", "dpl");
	      dopi=0;
	    }
	  _endLazyDPSEvaluation ();
	}
    }
  /* so dptr know contains the address */
  aopOp (right, ic, FALSE, (AOP_INDPTRn(result) ? FALSE : TRUE));

  /* if bit then unpack */
  if (IS_BITVAR (retype) || IS_BITVAR (letype)) {
      if (AOP_INDPTRn(result)) {
	  genSetDPTR(AOP(result)->aopu.dptr);
      }
      genPackBits ((IS_BITVAR (retype) ? retype : letype), right, "dptr", FPOINTER);
      if (AOP_INDPTRn(result)) {
	  genSetDPTR(0);
      }
  } else {
      size = AOP_SIZE (right);
      offset = 0;
      if (AOP_INDPTRn(result) && AOP_USESDPTR(right)) {
	  while (size--) {
	      MOVA (aopGet (AOP (right), offset++, FALSE, FALSE, NULL));
	      
	      genSetDPTR(AOP(result)->aopu.dptr);
	      emitcode ("movx", "@dptr,a");
	      if (size || (dopi && pi && AOP_TYPE (result) != AOP_IMMD))
		  emitcode ("inc", "dptr");
	      genSetDPTR (0);
	  }
      } else {
	  _startLazyDPSEvaluation ();
	  while (size--) {
	      MOVA (aopGet (AOP (right), offset++, FALSE, FALSE, NULL));
	      
	      if (AOP_INDPTRn(result)) {
		  genSetDPTR(AOP(result)->aopu.dptr);
	      } else {
		  genSetDPTR (0);
	      }
	      _flushLazyDPS ();
	      
	      emitcode ("movx", "@dptr,a");
	      if (size || (dopi && pi && AOP_TYPE (result) != AOP_IMMD))
		  emitcode ("inc", "dptr");
	  }
	  _endLazyDPSEvaluation ();
      }
  }
  
  if (dopi && pi && AOP_TYPE (result) != AOP_IMMD) {
      if (!AOP_INDPTRn(result)) {
	  aopPut (AOP(result),"dpl",0);
	  aopPut (AOP(result),"dph",1);
	  if (options.model == MODEL_FLAT24)
	      aopPut (AOP(result),"dpx",2);
      }
      pi->generated=1;
  } else if ((OP_SYMBOL(result)->ruonly || AOP_INDPTRn(result)) && 
	     AOP_SIZE(right) > 1 &&
	     (OP_SYMBOL (result)->liveTo > ic->seq || ic->depth)) {
      
      size = AOP_SIZE (right) - 1;
      if (AOP_INDPTRn(result)) {
	  genSetDPTR(AOP(result)->aopu.dptr);
      } 
      while (size--) emitcode ("lcall","__decdptr");
      if (AOP_INDPTRn(result)) {
	  genSetDPTR(0);
      }
  }
  freeAsmop (result, NULL, ic, TRUE);
  freeAsmop (right, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genGenPointerSet - set value from generic pointer space         */
/*-----------------------------------------------------------------*/
static void
genGenPointerSet (operand * right,
		  operand * result, iCode * ic, iCode *pi)
{
  int size, offset;
  sym_link *retype = getSpec (operandType (right));
  sym_link *letype = getSpec (operandType (result));

  aopOp (result, ic, FALSE, AOP_IS_STR(result) ? FALSE : TRUE);

  /* if the operand is already in dptr
     then we do nothing else we move the value to dptr */
  if (AOP_TYPE (result) != AOP_STR)
    {
      _startLazyDPSEvaluation ();
      /* if this is remateriazable */
      if (AOP_TYPE (result) == AOP_IMMD)
	{
	  emitcode ("mov", "dptr,%s", aopGet (AOP (result), 0, TRUE, FALSE, NULL));
	  if (AOP(result)->aopu.aop_immd.from_cast_remat) 
	  {
	      MOVB(aopGet(AOP (result), AOP_SIZE(result)-1, FALSE, FALSE, NULL));
	  }
	  else
	  {
	      emitcode ("mov", 
			"b,%s + 1", aopGet (AOP (result), 0, TRUE, FALSE, NULL));
	  }
	}
      else
	{			/* we need to get it byte by byte */
	  emitcode ("mov", "dpl,%s", aopGet (AOP (result), 0, FALSE, FALSE, NULL));
	  emitcode ("mov", "dph,%s", aopGet (AOP (result), 1, FALSE, FALSE, NULL));
	  if (options.model == MODEL_FLAT24) {
	    emitcode ("mov", "dpx,%s", aopGet (AOP (result), 2, FALSE, FALSE, NULL));
	    emitcode ("mov", "b,%s", aopGet (AOP (result), 3, FALSE, FALSE, NULL));
	  } else {
	    emitcode ("mov", "b,%s", aopGet (AOP (result), 2, FALSE, FALSE, NULL));
	  }
	}
      _endLazyDPSEvaluation ();
    }
  /* so dptr know contains the address */
  aopOp (right, ic, FALSE, TRUE);

  /* if bit then unpack */
  if (IS_BITVAR (retype) || IS_BITVAR (letype))
    genPackBits ((IS_BITVAR (retype) ? retype : letype), right, "dptr", GPOINTER);
  else
    {
      size = AOP_SIZE (right);
      offset = 0;

      _startLazyDPSEvaluation ();
      while (size--)
	{
	  MOVA (aopGet (AOP (right), offset++, FALSE, FALSE, NULL));

	  genSetDPTR (0);
	  _flushLazyDPS ();

	  emitcode ("lcall", "__gptrput");
	  if (size || (pi && AOP_TYPE (result) != AOP_IMMD))
	    emitcode ("inc", "dptr");
	}
      _endLazyDPSEvaluation ();
    }

  if (pi && AOP_TYPE (result) != AOP_IMMD) {
      aopPut (AOP(result),"dpl",0);
      aopPut (AOP(result),"dph",1);
      if (options.model == MODEL_FLAT24) {
	  aopPut (AOP(result),"dpx",2);
	  aopPut (AOP(result),"b",3);
      } else {
	  aopPut (AOP(result),"b",2);
      }
      pi->generated=1;
  } else if (OP_SYMBOL(result)->ruonly && AOP_SIZE(right) > 1 &&
	     (OP_SYMBOL (result)->liveTo > ic->seq || ic->depth)) {
      
      size = AOP_SIZE (right) - 1;
      while (size--) emitcode ("lcall","__decdptr");
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

  D (emitcode (";", "genPointerSet "););

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

  D (emitcode (";", "genIfx "););

  aopOp (cond, ic, FALSE, FALSE);

  /* get the value into acc */
  if (AOP_TYPE (cond) != AOP_CRY)
    {
	toBoolean (cond);
    }
  else
    {
	isbit = 1;
    }
    
  /* the result is now in the accumulator */
  freeAsmop (cond, NULL, ic, TRUE);

  /* if there was something to be popped then do it */
  if (popIc)
    genIpop (popIc);

  /* if the condition is  a bit variable */
  if (isbit && IS_ITEMP (cond) &&
      SPIL_LOC (cond))
    {
	genIfxJump (ic, SPIL_LOC (cond)->rname);
    }
  else if (isbit && !IS_ITEMP (cond))
    {
	genIfxJump (ic, OP_SYMBOL (cond)->rname);
    }
  else
    {
	genIfxJump (ic, "a");
    }

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

  D (emitcode (";", "genAddrOf ");
    );

  aopOp (IC_RESULT (ic), ic, FALSE, FALSE);

  /* if the operand is on the stack then we
     need to get the stack offset of this
     variable */
  if (sym->onStack) {
      
      /* if 10 bit stack */
      if (options.stack10bit) {
	  char buff[10];
	  tsprintf(buff, sizeof(buff), 
		   "#!constbyte",(options.stack_loc >> 16) & 0xff);
	  /* if it has an offset then we need to compute it */
/* 	  emitcode ("subb", "a,#!constbyte", */
/* 		    -((sym->stack < 0) ? */
/* 		      ((short) (sym->stack - _G.nRegsSaved)) : */
/* 		      ((short) sym->stack)) & 0xff); */
/* 	  emitcode ("mov","b,a"); */
/* 	  emitcode ("mov","a,#!constbyte",(-((sym->stack < 0) ? */
/* 					 ((short) (sym->stack - _G.nRegsSaved)) : */
/* 					 ((short) sym->stack)) >> 8) & 0xff); */
	  if (sym->stack) {
	      emitcode ("mov", "a,_bpx");
	      emitcode ("add", "a,#!constbyte", ((sym->stack < 0) ? 
					     ((char) (sym->stack - _G.nRegsSaved)) :
					     ((char) sym->stack )) & 0xff);
	      emitcode ("mov", "b,a");
	      emitcode ("mov", "a,_bpx+1");
	      emitcode ("addc","a,#!constbyte", (((sym->stack < 0) ? 
					      ((short) (sym->stack - _G.nRegsSaved)) :
					      ((short) sym->stack )) >> 8) & 0xff);
	      aopPut (AOP (IC_RESULT (ic)), "b", 0);
	      aopPut (AOP (IC_RESULT (ic)), "a", 1);
	      aopPut (AOP (IC_RESULT (ic)), buff, 2);
	  } else {
	      /* we can just move _bp */
	      aopPut (AOP (IC_RESULT (ic)), "_bpx", 0);
	      aopPut (AOP (IC_RESULT (ic)), "_bpx+1", 1);
	      aopPut (AOP (IC_RESULT (ic)), buff, 2);
	  }	  
      } else {
	  /* if it has an offset then we need to compute it */
	  if (sym->stack) {
	      emitcode ("mov", "a,_bp");
	      emitcode ("add", "a,#!constbyte", ((char) sym->stack & 0xff));
	      aopPut (AOP (IC_RESULT (ic)), "a", 0);
	  } else {
	      /* we can just move _bp */
	      aopPut (AOP (IC_RESULT (ic)), "_bp", 0);
	  }
	  /* fill the result with zero */
	  size = AOP_SIZE (IC_RESULT (ic)) - 1;
	  
	  
	  if (options.stack10bit && size < (FPTRSIZE - 1)) {
	      fprintf (stderr,
		       "*** warning: pointer to stack var truncated.\n");
	  }

	  offset = 1;
	  while (size--) {
	      aopPut (AOP (IC_RESULT (ic)), zero, offset++);
	  }      
      }
      goto release;
  }

  /* object not on stack then we need the name */
  size = AOP_SIZE (IC_RESULT (ic));
  offset = 0;

  while (size--)
    {
      char s[SDCC_NAME_MAX];
      if (offset) {
	  switch (offset) {
	  case 1:
	      tsprintf(s, sizeof(s), "!his",sym->rname);
	      break;
	  case 2:
	      tsprintf(s, sizeof(s), "!hihis",sym->rname);
	      break;
	  case 3:
	      tsprintf(s, sizeof(s), "!hihihis",sym->rname);
	      break;
	  default: /* should not need this (just in case) */
	      SNPRINTF (s, sizeof(s), "#(%s >> %d)",
		       sym->rname,
		       offset * 8);
	  }
      } 
      else
      {
	  SNPRINTF (s, sizeof(s), "#%s", sym->rname);
      }
	
      aopPut (AOP (IC_RESULT (ic)), s, offset++);
    }

release:
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);

}

/*-----------------------------------------------------------------*/
/* genArrayInit - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void
genArrayInit (iCode * ic)
{
    literalList *iLoop;
    int         ix, count;
    int         elementSize = 0, eIndex;
    unsigned    val, lastVal;
    sym_link    *type;
    operand     *left=IC_LEFT(ic);
    
    D (emitcode (";", "genArrayInit "););

    aopOp (IC_LEFT(ic), ic, FALSE, FALSE);
    
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_IMMD)
    {
	// Load immediate value into DPTR.
	emitcode("mov", "dptr, %s",
	     aopGet(AOP(IC_LEFT(ic)), 0, TRUE, FALSE, NULL));
    }
    else if (AOP_TYPE(IC_LEFT(ic)) != AOP_DPTR)
    {
#if 0
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
	      "Unexpected operand to genArrayInit.\n");
      exit(1);
#else
      // a regression because of SDCCcse.c:1.52
      emitcode ("mov", "dpl,%s", aopGet (AOP (left), 0, FALSE, FALSE, NULL));
      emitcode ("mov", "dph,%s", aopGet (AOP (left), 1, FALSE, FALSE, NULL));
      if (options.model == MODEL_FLAT24)
	emitcode ("mov", "dpx,%s", aopGet (AOP (left), 2, FALSE, FALSE, NULL));
#endif
    }
    
    type = operandType(IC_LEFT(ic));
    
    if (type && type->next)
    {
	elementSize = getSize(type->next);
    }
    else
    {
	werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
		                "can't determine element size in genArrayInit.\n");
	exit(1);
    }
    
    iLoop = IC_ARRAYILIST(ic);
    lastVal = 0xffff;
    
    while (iLoop)
    {
	bool firstpass = TRUE;
	
	emitcode(";", "store %d x 0x%x to DPTR (element size %d)", 
		 iLoop->count, (int)iLoop->literalValue, elementSize);
	
	ix = iLoop->count;
	
	while (ix)
	{
	    symbol *tlbl = NULL;
	    
	    count = ix > 256 ? 256 : ix;
	    
	    if (count > 1)
	    {
		tlbl = newiTempLabel (NULL);
		if (firstpass || (count & 0xff))
		{
		    emitcode("mov", "b, #!constbyte", count & 0xff);
		}
		
		emitcode ("", "!tlabeldef", tlbl->key + 100);
	    }
	    
	    firstpass = FALSE;
		
	    for (eIndex = 0; eIndex < elementSize; eIndex++)
	    {
		val = (((int)iLoop->literalValue) >> (eIndex * 8)) & 0xff;
		if (val != lastVal)
		{
		    emitcode("mov", "a, #!constbyte", val);
		    lastVal = val;
		}
		
		emitcode("movx", "@dptr, a");
		emitcode("inc", "dptr");
	    }
	    
	    if (count > 1)
	    {
		emitcode("djnz", "b, !tlabel", tlbl->key + 100);
	    }
	    
	    ix -= count;
	}
	
	iLoop = iLoop->next;
    }
    
    freeAsmop (IC_LEFT(ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genFarFarAssign - assignment when both are in far space         */
/*-----------------------------------------------------------------*/
static void
genFarFarAssign (operand * result, operand * right, iCode * ic)
{
  int size = AOP_SIZE (right);
  int offset = 0;
  symbol *rSym = NULL;

  if (size == 1)
  {
      /* quick & easy case. */
      D(emitcode(";","genFarFarAssign (1 byte case)"););      
      MOVA(aopGet(AOP(right), 0, FALSE, FALSE, NULL));
      freeAsmop (right, NULL, ic, FALSE);
      /* now assign DPTR to result */
      _G.accInUse++;
      aopOp(result, ic, FALSE, FALSE);
      _G.accInUse--;
      aopPut(AOP(result), "a", 0);
      freeAsmop(result, NULL, ic, FALSE);
      return;
  }
  
  /* See if we've got an underlying symbol to abuse. */
  if (IS_SYMOP(result) && OP_SYMBOL(result))
  {
      if (IS_TRUE_SYMOP(result))
      {
	  rSym = OP_SYMBOL(result);
      }
      else if (IS_ITEMP(result) && OP_SYMBOL(result)->isspilt && OP_SYMBOL(result)->usl.spillLoc)
      {
	  rSym = OP_SYMBOL(result)->usl.spillLoc;
      }
  }
	     
  if (size > 1 && rSym && rSym->rname && !rSym->onStack)
  {
      /* We can use the '390 auto-toggle feature to good effect here. */
      
      D(emitcode(";","genFarFarAssign (390 auto-toggle fun)"););
      emitcode("mov", "dps,#!constbyte",0x21); 	/* Select DPTR2 & auto-toggle. */
      emitcode ("mov", "dptr,#%s", rSym->rname); 
      /* DP2 = result, DP1 = right, DP1 is current. */
      while (size)
      {
          emitcode("movx", "a,@dptr");
          emitcode("movx", "@dptr,a");
          if (--size)
          {
               emitcode("inc", "dptr");
               emitcode("inc", "dptr");
          }
      }
      emitcode("mov", "dps,#0");
      freeAsmop (right, NULL, ic, FALSE);
#if 0
some alternative code for processors without auto-toggle
no time to test now, so later well put in...kpb
        D(emitcode(";","genFarFarAssign (dual-dptr fun)"););
        emitcode("mov", "dps,#1"); 	/* Select DPTR2. */
        emitcode ("mov", "dptr,#%s", rSym->rname); 
        /* DP2 = result, DP1 = right, DP1 is current. */
        while (size)
        {
          --size;
          emitcode("movx", "a,@dptr");
          if (size)
            emitcode("inc", "dptr");
          emitcode("inc", "dps");
          emitcode("movx", "@dptr,a");
          if (size)
            emitcode("inc", "dptr");
          emitcode("inc", "dps");
        }
        emitcode("mov", "dps,#0");
        freeAsmop (right, NULL, ic, FALSE);
#endif
  }
  else
  {
      D (emitcode (";", "genFarFarAssign"););
      aopOp (result, ic, TRUE, TRUE);

      _startLazyDPSEvaluation ();
      
      while (size--)
	{
	  aopPut (AOP (result),
		  aopGet (AOP (right), offset, FALSE, FALSE, NULL), offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
      freeAsmop (result, NULL, ic, FALSE);
      freeAsmop (right, NULL, ic, FALSE);
  }
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

  D (emitcode (";", "genAssign ");
    );

  result = IC_RESULT (ic);
  right = IC_RIGHT (ic);

  /* if they are the same */
  if (operandsEqu (IC_RESULT (ic), IC_RIGHT (ic)))
    return;

  aopOp (right, ic, FALSE, FALSE);

  emitcode (";", "genAssign: resultIsFar = %s",
	    isOperandInFarSpace (result) ?
	    "TRUE" : "FALSE");

  /* special case both in far space */
  if ((AOP_TYPE (right) == AOP_DPTR ||
       AOP_TYPE (right) == AOP_DPTR2) &&
  /* IS_TRUE_SYMOP(result)       && */
      isOperandInFarSpace (result))
    {
      genFarFarAssign (result, right, ic);
      return;
    }

  aopOp (result, ic, TRUE, FALSE);

  /* if they are the same registers */
  if (sameRegs (AOP (right), AOP (result)))
    goto release;

  /* if the result is a bit */
  if (AOP_TYPE (result) == AOP_CRY) /* works only for true symbols */
    {
      /* if the right size is a literal then
         we know what the value is */
      if (AOP_TYPE (right) == AOP_LIT)
	{
	  if (((int) operandLitValue (right)))
	    aopPut (AOP (result), one, 0);
	  else
	    aopPut (AOP (result), zero, 0);
	  goto release;
	}

      /* the right is also a bit variable */
      if (AOP_TYPE (right) == AOP_CRY)
	{
	  emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
	  aopPut (AOP (result), "c", 0);
	  goto release;
	}

      /* we need to or */
      toBoolean (right);
      aopPut (AOP (result), "a", 0);
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
      !IS_FLOAT (operandType (right)))
    {
      _startLazyDPSEvaluation ();
      while (size && ((unsigned int) (lit >> (offset * 8)) != 0))
	{
	  aopPut (AOP (result),
		  aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		  offset);
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
	  aopPut (AOP (result), "a", offset++);
	}
      _endLazyDPSEvaluation ();
    }
  else
    {
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  aopPut (AOP (result),
		  aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		  offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
    }

release:
  freeAsmop (right, NULL, ic, FALSE);
  freeAsmop (result, NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genJumpTab - generates code for jump table                      */
/*-----------------------------------------------------------------*/
static void
genJumpTab (iCode * ic)
{
  symbol *jtab;
  char *l;

  D (emitcode (";", "genJumpTab ");
    );

  aopOp (IC_JTCOND (ic), ic, FALSE, FALSE);
  /* get the condition into accumulator */
  l = aopGet (AOP (IC_JTCOND (ic)), 0, FALSE, FALSE, NULL);
  MOVA (l);
  /* multiply by four! */
  emitcode ("add", "a,acc");
  emitcode ("add", "a,acc");
  freeAsmop (IC_JTCOND (ic), NULL, ic, TRUE);

  jtab = newiTempLabel (NULL);
  emitcode ("mov", "dptr,#!tlabel", jtab->key + 100);
  emitcode ("jmp", "@a+dptr");
  emitcode ("", "!tlabeldef", jtab->key + 100);
  /* now generate the jump labels */
  for (jtab = setFirstItem (IC_JTLABELS (ic)); jtab;
       jtab = setNextItem (IC_JTLABELS (ic)))
    emitcode ("ljmp", "!tlabel", jtab->key + 100);

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

  D (emitcode (";", "genCast "););

  /* if they are equivalent then do nothing */
  if (operandsEqu (IC_RESULT (ic), IC_RIGHT (ic)))
    return;

  aopOp (right, ic, FALSE, FALSE);
  aopOp (result, ic, FALSE, AOP_USESDPTR(right));

  /* if the result is a bit */
  // if (AOP_TYPE (result) == AOP_CRY) /* works only for true symbols */
  if (IS_BITVAR(OP_SYMBOL(result)->type))
    {
      /* if the right size is a literal then
         we know what the value is */
      if (AOP_TYPE (right) == AOP_LIT)
	{
	  if (((int) operandLitValue (right)))
	    aopPut (AOP (result), one, 0);
	  else
	    aopPut (AOP (result), zero, 0);

	  goto release;
	}

      /* the right is also a bit variable */
      if (AOP_TYPE (right) == AOP_CRY)
	{
	  emitcode ("mov", "c,%s", AOP (right)->aopu.aop_dir);
	  aopPut (AOP (result), "c", 0);
	  goto release;
	}

      /* we need to or */
      toBoolean (right);
      aopPut (AOP (result), "a", 0);
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
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  aopPut (AOP (result),
		  aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		  offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
      goto release;
    }


  /* if the result is of type pointer */
  if (IS_PTR (ctype))
    {

      int p_type;
      sym_link *type = operandType (right);

      /* pointer to generic pointer */
      if (IS_GENPTR (ctype))
	{
	  if (IS_PTR (type))
	    {
	      p_type = DCL_TYPE (type);
	    }
	  else
	    {
#if OLD_CAST_BEHAVIOR
	      /* KV: we are converting a non-pointer type to
	       * a generic pointer. This (ifdef'd out) code
	       * says that the resulting generic pointer
	       * should have the same class as the storage
	       * location of the non-pointer variable.
	       *
	       * For example, converting an int (which happens
	       * to be stored in DATA space) to a pointer results
	       * in a DATA generic pointer; if the original int
	       * in XDATA space, so will be the resulting pointer.
	       *
	       * I don't like that behavior, and thus this change:
	       * all such conversions will be forced to XDATA and
	       * throw a warning. If you want some non-XDATA
	       * type, or you want to suppress the warning, you
	       * must go through an intermediate cast, like so:
	       *
	       * char _generic *gp = (char _xdata *)(intVar);
	       */
	      sym_link *etype = getSpec (type);

	      /* we have to go by the storage class */
	      if (SPEC_OCLS (etype) != generic)
		{
		  p_type = PTR_TYPE (SPEC_OCLS (etype));
		}
	      else
#endif
		{
		  /* Converting unknown class (i.e. register variable)
		   * to generic pointer. This is not good, but
		   * we'll make a guess (and throw a warning).
		   */
		  p_type = FPOINTER;
		  werror (W_INT_TO_GEN_PTR_CAST);
		}
	    }

	  /* the first two bytes are known */
	  size = GPTRSIZE - 1;
	  offset = 0;
	  _startLazyDPSEvaluation ();
	  while (size--)
	    {
	      aopPut (AOP (result),
		      aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		      offset);
	      offset++;
	    }
	  _endLazyDPSEvaluation ();

	  /* the last byte depending on type */
	    {
		int gpVal = pointerTypeToGPByte(p_type, NULL, NULL);
		char gpValStr[10];
	    
		if (gpVal == -1)
		{
		    // pointerTypeToGPByte will have bitched.
		    exit(1);
		}
	    
		SNPRINTF(gpValStr, sizeof(gpValStr), "#0x%d", gpVal);
		aopPut (AOP (result), gpValStr, GPTRSIZE - 1);
	    }
	  goto release;
	}

      /* just copy the pointers */
      size = AOP_SIZE (result);
      offset = 0;
      _startLazyDPSEvaluation ();
      while (size--)
	{
	  aopPut (AOP (result),
		  aopGet (AOP (right), offset, FALSE, FALSE, NULL),
		  offset);
	  offset++;
	}
      _endLazyDPSEvaluation ();
      goto release;
    }

  /* so we now know that the size of destination is greater
     than the size of the source */
  /* we move to result for the size of source */
  size = AOP_SIZE (right);
  offset = 0;
  _startLazyDPSEvaluation ();
  while (size--)
    {
      aopPut (AOP (result),
	      aopGet (AOP (right), offset, FALSE, FALSE, NULL),
	      offset);
      offset++;
    }
  _endLazyDPSEvaluation ();

  /* now depending on the sign of the source && destination */
  size = AOP_SIZE (result) - AOP_SIZE (right);
  /* if unsigned or not an integral type */
  /* also, if the source is a bit, we don't need to sign extend, because
   * it can't possibly have set the sign bit.
   */
  if (SPEC_USIGN (rtype) || !IS_SPEC (rtype) || AOP_TYPE (right) == AOP_CRY)
    {
      while (size--)
	{
	  aopPut (AOP (result), zero, offset++);
	}
    }
  else
    {
      /* we need to extend the sign :{ */
      MOVA (aopGet (AOP (right), AOP_SIZE (right) - 1,
			FALSE, FALSE, NULL));
      emitcode ("rlc", "a");
      emitcode ("subb", "a,acc");
      while (size--)
	aopPut (AOP (result), "a", offset++);
    }

  /* we are done hurray !!!! */

release:
  freeAsmop (right, NULL, ic, TRUE);
  freeAsmop (result, NULL, ic, TRUE);

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

  /* if the minus is not of the form
     a = a - 1 */
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
  D(emitcode(";", "genDjnz"););

  lbl = newiTempLabel (NULL);
  lbl1 = newiTempLabel (NULL);

  aopOp (IC_RESULT (ic), ic, FALSE, FALSE);

  if (AOP_NEEDSACC(IC_RESULT(ic)))
  {
      /* If the result is accessed indirectly via
       * the accumulator, we must explicitly write
       * it back after the decrement.
       */
      char *rByte = aopGet(AOP(IC_RESULT(ic)), 0, FALSE, FALSE, NULL);
      
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
      aopPut(AOP(IC_RESULT(ic)), rByte, 0);
      emitcode ("jnz", "!tlabel", lbl->key + 100);
  }
  else if (IS_AOP_PREG (IC_RESULT (ic)))
    {
      emitcode ("dec", "%s",
		aopGet (AOP (IC_RESULT (ic)), 0, FALSE, FALSE, NULL));
      emitcode ("mov", "a,%s", aopGet (AOP (IC_RESULT (ic)), 0, FALSE, FALSE, NULL));
      emitcode ("jnz", "!tlabel", lbl->key + 100);
    }
  else
    {
      emitcode ("djnz", "%s,!tlabel", aopGet (AOP (IC_RESULT (ic)), 0, FALSE, TRUE, NULL),
		lbl->key + 100);
    }
  emitcode ("sjmp", "!tlabel", lbl1->key + 100);
  emitcode ("", "!tlabeldef", lbl->key + 100);
  emitcode ("ljmp", "!tlabel", IC_TRUE (ifx)->key + 100);
  emitcode ("", "!tlabeldef", lbl1->key + 100);

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
    int rb1off ;
    
    D (emitcode (";", "genReceive ");
       );

  if (ic->argreg == 1) { /* first parameter */
      if (isOperandInFarSpace (IC_RESULT (ic)) &&
	  (OP_SYMBOL (IC_RESULT (ic))->isspilt ||
	   IS_TRUE_SYMOP (IC_RESULT (ic))))
	  {
	      offset = fReturnSizeDS390 - size;
	      while (size--)
		  {
		      emitcode ("push", "%s", (strcmp (fReturn[fReturnSizeDS390 - offset - 1], "a") ?
					       fReturn[fReturnSizeDS390 - offset - 1] : "acc"));
		      offset++;
		  }
	      aopOp (IC_RESULT (ic), ic, FALSE, FALSE);
	      size = AOP_SIZE (IC_RESULT (ic));
	      offset = 0;
	      while (size--)
		  {
		      emitcode ("pop", "acc");
		      aopPut (AOP (IC_RESULT (ic)), "a", offset++);
		  }
	      
	  } else {
	      _G.accInUse++;
	      aopOp (IC_RESULT (ic), ic, FALSE, FALSE);
	      _G.accInUse--;
	      assignResultValue (IC_RESULT (ic));
	  }
  } else { /* second receive onwards */
      /* this gets a little tricky since unused recevies will be
	 eliminated, we have saved the reg in the type field . and
	 we use that to figure out which register to use */
      aopOp (IC_RESULT (ic), ic, FALSE, FALSE);
      rb1off = ic->argreg;
      while (size--) {
	  aopPut (AOP (IC_RESULT (ic)), rb1regs[rb1off++ -5], offset++);
      }
      
  }
  freeAsmop (IC_RESULT (ic), NULL, ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* genMemcpyX2X - gen code for memcpy xdata to xdata               */
/*-----------------------------------------------------------------*/
static void genMemcpyX2X( iCode *ic, int nparms, operand **parms, int fromc)
{
    operand *from , *to , *count;
    symbol *lbl;
    bitVect *rsave;
    int i;

    /* we know it has to be 3 parameters */
    assert (nparms == 3);
    
    rsave = newBitVect(16);
    /* save DPTR if it needs to be saved */
    for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	    if (bitVectBitValue(ic->rMask,i))
		    rsave = bitVectSetBit(rsave,i);
    }
    rsave = bitVectIntersect(rsave,bitVectCplAnd (bitVectCopy (ic->rMask),
						  ds390_rUmaskForOp (IC_RESULT(ic))));
    savermask(rsave);
    
    to = parms[0];
    from = parms[1];
    count = parms[2];

    aopOp (from, ic->next, FALSE, FALSE);

    /* get from into DPTR1 */
    emitcode ("mov", "dpl1,%s", aopGet (AOP (from), 0, FALSE, FALSE, NULL));
    emitcode ("mov", "dph1,%s", aopGet (AOP (from), 1, FALSE, FALSE, NULL));
    if (options.model == MODEL_FLAT24) {
	emitcode ("mov", "dpx1,%s", aopGet (AOP (from), 2, FALSE, FALSE, NULL));
    }

    freeAsmop (from, NULL, ic, FALSE);
    aopOp (to, ic, FALSE, FALSE);
    /* get "to" into DPTR */
    /* if the operand is already in dptr
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE (to) != AOP_STR) {
	/* if already in DPTR then we need to push */
	if (AOP_TYPE(to) == AOP_DPTR) {
	    emitcode ("push", "%s", aopGet (AOP (to), 0, FALSE, TRUE, NULL));
	    emitcode ("push", "%s", aopGet (AOP (to), 1, FALSE, TRUE, NULL));
	    if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
	    emitcode ("pop", "dph");
	    emitcode ("pop", "dpl");	    
	} else {
	    _startLazyDPSEvaluation ();
	    /* if this is remateriazable */
	    if (AOP_TYPE (to) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", aopGet (AOP (to), 0, TRUE, FALSE, NULL));
	    } else {			/* we need to get it byte by byte */
		emitcode ("mov", "dpl,%s", aopGet (AOP (to), 0, FALSE, FALSE, NULL));
		emitcode ("mov", "dph,%s", aopGet (AOP (to), 1, FALSE, FALSE, NULL));
		if (options.model == MODEL_FLAT24) {
		    emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
		}
	    }
	    _endLazyDPSEvaluation ();
	}
    }
    freeAsmop (to, NULL, ic, FALSE);
    _G.dptrInUse = _G.dptr1InUse = 1;
    aopOp (count, ic->next->next, FALSE,FALSE);
    lbl =newiTempLabel(NULL);

    /* now for the actual copy */
    if (AOP_TYPE(count) == AOP_LIT && 
	(int)floatFromVal (AOP(count)->aopu.aop_lit) <= 256) {
	emitcode ("mov", "b,%s",aopGet(AOP(count),0,FALSE,FALSE,NULL));
	if (fromc) {
	    emitcode ("lcall","__bi_memcpyc2x_s");
	} else {
	    emitcode ("lcall","__bi_memcpyx2x_s");
	}
	freeAsmop (count, NULL, ic, FALSE);
    } else {
	symbol *lbl1 = newiTempLabel(NULL);
	
	emitcode (";"," Auto increment but no djnz");
	emitcode ("mov","_ap,%s",aopGet (AOP (count), 0, FALSE, TRUE, NULL));
	emitcode ("mov","b,%s",aopGet (AOP (count), 1, FALSE, TRUE, NULL));
	freeAsmop (count, NULL, ic, FALSE);
	emitcode ("mov", "dps,#!constbyte",0x21); 	/* Select DPTR2 & auto-toggle. */
	emitcode ("","!tlabeldef",lbl->key+100);
	if (fromc) {
	    emitcode ("clr","a");
	    emitcode ("movc", "a,@a+dptr");
	} else 
	    emitcode ("movx", "a,@dptr");
	emitcode ("movx", "@dptr,a");
	emitcode ("inc", "dptr");
	emitcode ("inc", "dptr");
	emitcode ("mov","a,b");
	emitcode ("orl","a,_ap");
	emitcode ("jz","!tlabel",lbl1->key+100);
	emitcode ("mov","a,_ap");
	emitcode ("add","a,#!constbyte",0xFF);
	emitcode ("mov","_ap,a");
	emitcode ("mov","a,b");
	emitcode ("addc","a,#!constbyte",0xFF);
	emitcode ("mov","b,a");
	emitcode ("sjmp","!tlabel",lbl->key+100);
	emitcode ("","!tlabeldef",lbl1->key+100);
    }
    emitcode ("mov", "dps,#0"); 
    _G.dptrInUse = _G.dptr1InUse = 0;
    unsavermask(rsave);

}

/*-----------------------------------------------------------------*/
/* genMemcmpX2X - gen code for memcmp xdata to xdata               */
/*-----------------------------------------------------------------*/
static void genMemcmpX2X( iCode *ic, int nparms, operand **parms, int fromc)
{
    operand *from , *to , *count;
    symbol *lbl,*lbl2;
    bitVect *rsave;
    int i;

    /* we know it has to be 3 parameters */
    assert (nparms == 3);
    
    rsave = newBitVect(16);
    /* save DPTR if it needs to be saved */
    for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	    if (bitVectBitValue(ic->rMask,i))
		    rsave = bitVectSetBit(rsave,i);
    }
    rsave = bitVectIntersect(rsave,bitVectCplAnd (bitVectCopy (ic->rMask),
						  ds390_rUmaskForOp (IC_RESULT(ic))));
    savermask(rsave);
    
    to = parms[0];
    from = parms[1];
    count = parms[2];

    aopOp (from, ic->next, FALSE, FALSE);

    /* get from into DPTR1 */
    emitcode ("mov", "dpl1,%s", aopGet (AOP (from), 0, FALSE, FALSE, NULL));
    emitcode ("mov", "dph1,%s", aopGet (AOP (from), 1, FALSE, FALSE, NULL));
    if (options.model == MODEL_FLAT24) {
	emitcode ("mov", "dpx1,%s", aopGet (AOP (from), 2, FALSE, FALSE, NULL));
    }

    freeAsmop (from, NULL, ic, FALSE);
    aopOp (to, ic, FALSE, FALSE);
    /* get "to" into DPTR */
    /* if the operand is already in dptr
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE (to) != AOP_STR) {
	/* if already in DPTR then we need to push */
	if (AOP_TYPE(to) == AOP_DPTR) {
	    emitcode ("push", "%s", aopGet (AOP (to), 0, FALSE, TRUE, NULL));
	    emitcode ("push", "%s", aopGet (AOP (to), 1, FALSE, TRUE, NULL));
	    if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
	    emitcode ("pop", "dph");
	    emitcode ("pop", "dpl");	    
	} else {
	    _startLazyDPSEvaluation ();
	    /* if this is remateriazable */
	    if (AOP_TYPE (to) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", aopGet (AOP (to), 0, TRUE, FALSE, NULL));
	    } else {			/* we need to get it byte by byte */
		emitcode ("mov", "dpl,%s", aopGet (AOP (to), 0, FALSE, FALSE, NULL));
		emitcode ("mov", "dph,%s", aopGet (AOP (to), 1, FALSE, FALSE, NULL));
		if (options.model == MODEL_FLAT24) {
		    emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
		}
	    }
	    _endLazyDPSEvaluation ();
	}
    }
    freeAsmop (to, NULL, ic, FALSE);
    _G.dptrInUse = _G.dptr1InUse = 1;
    aopOp (count, ic->next->next, FALSE,FALSE);
    lbl =newiTempLabel(NULL);
    lbl2 =newiTempLabel(NULL);

    /* now for the actual compare */
    if (AOP_TYPE(count) == AOP_LIT && 
	(int)floatFromVal (AOP(count)->aopu.aop_lit) <= 256) {
	emitcode ("mov", "b,%s",aopGet(AOP(count),0,FALSE,FALSE,NULL));
	if (fromc)
	    emitcode("lcall","__bi_memcmpc2x_s");
	else
	    emitcode("lcall","__bi_memcmpx2x_s");
	freeAsmop (count, NULL, ic, FALSE);
	aopOp (IC_RESULT(ic), ic, FALSE,FALSE);
	aopPut(AOP(IC_RESULT(ic)),"a",0);
	freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
    } else {
	symbol *lbl1 = newiTempLabel(NULL);

	emitcode("push","ar0"); 	
	emitcode (";"," Auto increment but no djnz");
	emitcode ("mov","_ap,%s",aopGet (AOP (count), 0, FALSE, TRUE, NULL));
	emitcode ("mov","b,%s",aopGet (AOP (count), 1, FALSE, TRUE, NULL));
	freeAsmop (count, NULL, ic, FALSE);
	emitcode ("mov", "dps,#!constbyte",0x21); 	/* Select DPTR2 & auto-toggle. */
	emitcode ("","!tlabeldef",lbl->key+100);
	if (fromc) {
	    emitcode ("clr","a");
	    emitcode ("movc", "a,@a+dptr");
	} else 
	    emitcode ("movx", "a,@dptr");
	emitcode ("mov","r0,a");
	emitcode ("movx", "a,@dptr");
	emitcode ("clr","c");
	emitcode ("subb","a,r0");
	emitcode ("jnz","!tlabel",lbl2->key+100);
	emitcode ("inc", "dptr");
	emitcode ("inc", "dptr");
	emitcode ("mov","a,b");
	emitcode ("orl","a,_ap");
	emitcode ("jz","!tlabel",lbl1->key+100);
	emitcode ("mov","a,_ap");
	emitcode ("add","a,#!constbyte",0xFF);
	emitcode ("mov","_ap,a");
	emitcode ("mov","a,b");
	emitcode ("addc","a,#!constbyte",0xFF);
	emitcode ("mov","b,a");
	emitcode ("sjmp","!tlabel",lbl->key+100);
	emitcode ("","!tlabeldef",lbl1->key+100);
	emitcode ("clr","a");
	emitcode ("","!tlabeldef",lbl2->key+100);
	aopOp (IC_RESULT(ic), ic, FALSE,FALSE);
	aopPut(AOP(IC_RESULT(ic)),"a",0);
	freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
	emitcode("pop","ar0");
	emitcode ("mov", "dps,#0");      
    }
    _G.dptrInUse = _G.dptr1InUse = 0;
    unsavermask(rsave);

}

/*-----------------------------------------------------------------*/
/* genInp - gen code for __builtin_inp read data from a mem mapped */
/* port, first parameter output area second parameter pointer to   */
/* port third parameter count                                      */
/*-----------------------------------------------------------------*/
static void genInp( iCode *ic, int nparms, operand **parms)
{
    operand *from , *to , *count;
    symbol *lbl;
    bitVect *rsave;
    int i;

    /* we know it has to be 3 parameters */
    assert (nparms == 3);
    
    rsave = newBitVect(16);
    /* save DPTR if it needs to be saved */
    for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	    if (bitVectBitValue(ic->rMask,i))
		    rsave = bitVectSetBit(rsave,i);
    }
    rsave = bitVectIntersect(rsave,bitVectCplAnd (bitVectCopy (ic->rMask),
						  ds390_rUmaskForOp (IC_RESULT(ic))));
    savermask(rsave);
    
    to = parms[0];
    from = parms[1];
    count = parms[2];

    aopOp (from, ic->next, FALSE, FALSE);

    /* get from into DPTR1 */
    emitcode ("mov", "dpl1,%s", aopGet (AOP (from), 0, FALSE, FALSE, NULL));
    emitcode ("mov", "dph1,%s", aopGet (AOP (from), 1, FALSE, FALSE, NULL));
    if (options.model == MODEL_FLAT24) {
	emitcode ("mov", "dpx1,%s", aopGet (AOP (from), 2, FALSE, FALSE, NULL));
    }

    freeAsmop (from, NULL, ic, FALSE);
    aopOp (to, ic, FALSE, FALSE);
    /* get "to" into DPTR */
    /* if the operand is already in dptr
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE (to) != AOP_STR) {
	/* if already in DPTR then we need to push */
	if (AOP_TYPE(to) == AOP_DPTR) {
	    emitcode ("push", "%s", aopGet (AOP (to), 0, FALSE, TRUE, NULL));
	    emitcode ("push", "%s", aopGet (AOP (to), 1, FALSE, TRUE, NULL));
	    if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
	    emitcode ("pop", "dph");
	    emitcode ("pop", "dpl");	    
	} else {
	    _startLazyDPSEvaluation ();
	    /* if this is remateriazable */
	    if (AOP_TYPE (to) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", aopGet (AOP (to), 0, TRUE, FALSE, NULL));
	    } else {			/* we need to get it byte by byte */
		emitcode ("mov", "dpl,%s", aopGet (AOP (to), 0, FALSE, FALSE, NULL));
		emitcode ("mov", "dph,%s", aopGet (AOP (to), 1, FALSE, FALSE, NULL));
		if (options.model == MODEL_FLAT24) {
		    emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
		}
	    }
	    _endLazyDPSEvaluation ();
	}
    }
    freeAsmop (to, NULL, ic, FALSE);

    _G.dptrInUse = _G.dptr1InUse = 1;
    aopOp (count, ic->next->next, FALSE,FALSE);
    lbl =newiTempLabel(NULL);

    /* now for the actual copy */
    if (AOP_TYPE(count) == AOP_LIT && 
	(int)floatFromVal (AOP(count)->aopu.aop_lit) <= 256) {
	emitcode (";","OH  JOY auto increment with djnz (very fast)");
	emitcode ("mov", "dps,#!constbyte",0x1); 	/* Select DPTR2 */
	emitcode ("mov", "b,%s",aopGet(AOP(count),0,FALSE,FALSE,NULL));
	freeAsmop (count, NULL, ic, FALSE);
	emitcode ("","!tlabeldef",lbl->key+100);
	emitcode ("movx", "a,@dptr");	/* read data from port */
	emitcode ("dec","dps");		/* switch to DPTR */
	emitcode ("movx", "@dptr,a");	/* save into location */
	emitcode ("inc", "dptr");	/* point to next area */
	emitcode ("inc","dps");		/* switch to DPTR2 */
	emitcode ("djnz","b,!tlabel",lbl->key+100);
    } else {
	symbol *lbl1 = newiTempLabel(NULL);
	
	emitcode (";"," Auto increment but no djnz");
	emitcode ("mov","_ap,%s",aopGet (AOP (count), 0, FALSE, TRUE, NULL));
	emitcode ("mov","b,%s",aopGet (AOP (count), 1, FALSE, TRUE, NULL));
	freeAsmop (count, NULL, ic, FALSE);
	emitcode ("mov", "dps,#!constbyte",0x1); 	/* Select DPTR2 */
	emitcode ("","!tlabeldef",lbl->key+100);
	emitcode ("movx", "a,@dptr");
	emitcode ("dec","dps");		/* switch to DPTR */
	emitcode ("movx", "@dptr,a");
	emitcode ("inc", "dptr");
	emitcode ("inc","dps");		/* switch to DPTR2 */
/* 	emitcode ("djnz","b,!tlabel",lbl->key+100); */
/* 	emitcode ("djnz","_ap,!tlabel",lbl->key+100); */
	emitcode ("mov","a,b");
	emitcode ("orl","a,_ap");
	emitcode ("jz","!tlabel",lbl1->key+100);
	emitcode ("mov","a,_ap");
	emitcode ("add","a,#!constbyte",0xFF);
	emitcode ("mov","_ap,a");
	emitcode ("mov","a,b");
	emitcode ("addc","a,#!constbyte",0xFF);
	emitcode ("mov","b,a");
	emitcode ("sjmp","!tlabel",lbl->key+100);
	emitcode ("","!tlabeldef",lbl1->key+100);
    }
    emitcode ("mov", "dps,#0"); 
    _G.dptrInUse = _G.dptr1InUse = 0;
    unsavermask(rsave);

}

/*-----------------------------------------------------------------*/
/* genOutp - gen code for __builtin_inp write data to a mem mapped */
/* port, first parameter output area second parameter pointer to   */
/* port third parameter count                                      */
/*-----------------------------------------------------------------*/
static void genOutp( iCode *ic, int nparms, operand **parms)
{
    operand *from , *to , *count;
    symbol *lbl;
    bitVect *rsave;
    int i;

    /* we know it has to be 3 parameters */
    assert (nparms == 3);
    
    rsave = newBitVect(16);
    /* save DPTR if it needs to be saved */
    for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	    if (bitVectBitValue(ic->rMask,i))
		    rsave = bitVectSetBit(rsave,i);
    }
    rsave = bitVectIntersect(rsave,bitVectCplAnd (bitVectCopy (ic->rMask),
						  ds390_rUmaskForOp (IC_RESULT(ic))));
    savermask(rsave);
    
    to = parms[0];
    from = parms[1];
    count = parms[2];

    aopOp (from, ic->next, FALSE, FALSE);

    /* get from into DPTR1 */
    emitcode ("mov", "dpl1,%s", aopGet (AOP (from), 0, FALSE, FALSE, NULL));
    emitcode ("mov", "dph1,%s", aopGet (AOP (from), 1, FALSE, FALSE, NULL));
    if (options.model == MODEL_FLAT24) {
	emitcode ("mov", "dpx1,%s", aopGet (AOP (from), 2, FALSE, FALSE, NULL));
    }

    freeAsmop (from, NULL, ic, FALSE);
    aopOp (to, ic, FALSE, FALSE);
    /* get "to" into DPTR */
    /* if the operand is already in dptr
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE (to) != AOP_STR) {
	/* if already in DPTR then we need to push */
	if (AOP_TYPE(to) == AOP_DPTR) {
	    emitcode ("push", "%s", aopGet (AOP (to), 0, FALSE, TRUE, NULL));
	    emitcode ("push", "%s", aopGet (AOP (to), 1, FALSE, TRUE, NULL));
	    if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
	    emitcode ("pop", "dph");
	    emitcode ("pop", "dpl");	    
	} else {
	    _startLazyDPSEvaluation ();
	    /* if this is remateriazable */
	    if (AOP_TYPE (to) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", aopGet (AOP (to), 0, TRUE, FALSE, NULL));
	    } else {			/* we need to get it byte by byte */
		emitcode ("mov", "dpl,%s", aopGet (AOP (to), 0, FALSE, FALSE, NULL));
		emitcode ("mov", "dph,%s", aopGet (AOP (to), 1, FALSE, FALSE, NULL));
		if (options.model == MODEL_FLAT24) {
		    emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
		}
	    }
	    _endLazyDPSEvaluation ();
	}
    }
    freeAsmop (to, NULL, ic, FALSE);

    _G.dptrInUse = _G.dptr1InUse = 1;
    aopOp (count, ic->next->next, FALSE,FALSE);
    lbl =newiTempLabel(NULL);

    /* now for the actual copy */
    if (AOP_TYPE(count) == AOP_LIT && 
	(int)floatFromVal (AOP(count)->aopu.aop_lit) <= 256) {
	emitcode (";","OH  JOY auto increment with djnz (very fast)");
	emitcode ("mov", "dps,#!constbyte",0x0); 	/* Select DPTR */
	emitcode ("mov", "b,%s",aopGet(AOP(count),0,FALSE,FALSE,NULL));
	emitcode ("","!tlabeldef",lbl->key+100);
	emitcode ("movx", "a,@dptr");	/* read data from port */
	emitcode ("inc","dps");		/* switch to DPTR2 */
	emitcode ("movx", "@dptr,a");	/* save into location */
	emitcode ("inc", "dptr");	/* point to next area */
	emitcode ("dec","dps");		/* switch to DPTR */
	emitcode ("djnz","b,!tlabel",lbl->key+100);
	freeAsmop (count, NULL, ic, FALSE);
    } else {
	symbol *lbl1 = newiTempLabel(NULL);
	
	emitcode (";"," Auto increment but no djnz");
	emitcode ("mov","_ap,%s",aopGet (AOP (count), 0, FALSE, TRUE, NULL));
	emitcode ("mov","b,%s",aopGet (AOP (count), 1, FALSE, TRUE, NULL));
	freeAsmop (count, NULL, ic, FALSE);
	emitcode ("mov", "dps,#!constbyte",0x0); 	/* Select DPTR */
	emitcode ("","!tlabeldef",lbl->key+100);
	emitcode ("movx", "a,@dptr");
	emitcode ("inc", "dptr");
	emitcode ("inc","dps");		/* switch to DPTR2 */
	emitcode ("movx", "@dptr,a");
	emitcode ("dec","dps");		/* switch to DPTR */
	emitcode ("mov","a,b");
	emitcode ("orl","a,_ap");
	emitcode ("jz","!tlabel",lbl1->key+100);
	emitcode ("mov","a,_ap");
	emitcode ("add","a,#!constbyte",0xFF);
	emitcode ("mov","_ap,a");
	emitcode ("mov","a,b");
	emitcode ("addc","a,#!constbyte",0xFF);
	emitcode ("mov","b,a");
	emitcode ("sjmp","!tlabel",lbl->key+100);
	emitcode ("","!tlabeldef",lbl1->key+100);
    }
    emitcode ("mov", "dps,#0"); 
    _G.dptrInUse = _G.dptr1InUse = 0;
    unsavermask(rsave);

}

/*-----------------------------------------------------------------*/
/* genSwapW - swap lower & high order bytes                        */
/*-----------------------------------------------------------------*/
static void genSwapW(iCode *ic, int nparms, operand **parms)
{
    operand *dest;
    operand *src;
    assert (nparms==1);

    src = parms[0];
    dest=IC_RESULT(ic);

    assert(getSize(operandType(src))==2);

    aopOp (src, ic, FALSE, FALSE);
    emitcode ("mov","a,%s",aopGet(AOP(src),0,FALSE,FALSE,NULL));
    _G.accInUse++;
    MOVB(aopGet(AOP(src),1,FALSE,FALSE,"b"));
    _G.accInUse--;
    freeAsmop (src, NULL, ic, FALSE);
    
    aopOp (dest,ic, FALSE, FALSE);
    aopPut(AOP(dest),"b",0);
    aopPut(AOP(dest),"a",1);
    freeAsmop (dest, NULL, ic, FALSE);    
}

/*-----------------------------------------------------------------*/
/* genMemsetX - gencode for memSetX data                           */
/*-----------------------------------------------------------------*/
static void genMemsetX(iCode *ic, int nparms, operand **parms)
{
    operand *to , *val , *count;
    symbol *lbl;
    char *l;
    int i;
    bitVect *rsave;

    /* we know it has to be 3 parameters */
    assert (nparms == 3);
    
    to = parms[0];
    val = parms[1];
    count = parms[2];
        
    /* save DPTR if it needs to be saved */
    rsave = newBitVect(16);
    for (i = DPL_IDX ; i <= B_IDX ; i++ ) {
	    if (bitVectBitValue(ic->rMask,i))
		    rsave = bitVectSetBit(rsave,i);
    }
    rsave = bitVectIntersect(rsave,bitVectCplAnd (bitVectCopy (ic->rMask),
						  ds390_rUmaskForOp (IC_RESULT(ic))));
    savermask(rsave);

    aopOp (to, ic, FALSE, FALSE);
    /* get "to" into DPTR */
    /* if the operand is already in dptr
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE (to) != AOP_STR) {
	/* if already in DPTR then we need to push */
	if (AOP_TYPE(to) == AOP_DPTR) {
	    emitcode ("push", "%s", aopGet (AOP (to), 0, FALSE, TRUE, NULL));
	    emitcode ("push", "%s", aopGet (AOP (to), 1, FALSE, TRUE, NULL));
	    if (options.model == MODEL_FLAT24)
		emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
	    emitcode ("pop", "dph");
	    emitcode ("pop", "dpl");	    
	} else {
	    _startLazyDPSEvaluation ();
	    /* if this is remateriazable */
	    if (AOP_TYPE (to) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", aopGet (AOP (to), 0, TRUE, FALSE, NULL));
	    } else {			/* we need to get it byte by byte */
		emitcode ("mov", "dpl,%s", aopGet (AOP (to), 0, FALSE, FALSE, NULL));
		emitcode ("mov", "dph,%s", aopGet (AOP (to), 1, FALSE, FALSE, NULL));
		if (options.model == MODEL_FLAT24) {
		    emitcode ("mov", "dpx,%s", aopGet (AOP (to), 2, FALSE, FALSE, NULL));
		}
	    }
	    _endLazyDPSEvaluation ();
	}
    }
    freeAsmop (to, NULL, ic, FALSE);

    aopOp (val, ic->next->next, FALSE,FALSE);
    aopOp (count, ic->next->next, FALSE,FALSE);    
    lbl =newiTempLabel(NULL);
    /* now for the actual copy */
    if (AOP_TYPE(count) == AOP_LIT && 
	(int)floatFromVal (AOP(count)->aopu.aop_lit) <= 256) {
	l = aopGet(AOP (val), 0, FALSE, FALSE, NULL);
	emitcode ("mov", "b,%s",aopGet(AOP(count),0,FALSE,FALSE,NULL));
	MOVA(l);
	emitcode ("","!tlabeldef",lbl->key+100);
	emitcode ("movx", "@dptr,a");
	emitcode ("inc", "dptr");
	emitcode ("djnz","b,!tlabel",lbl->key+100);
    } else {
	symbol *lbl1 = newiTempLabel(NULL);
	
	emitcode ("mov","_ap,%s",aopGet (AOP (count), 0, FALSE, TRUE, NULL));
	emitcode ("mov","b,%s",aopGet (AOP (count), 1, FALSE, TRUE, NULL));
	emitcode ("","!tlabeldef",lbl->key+100);
	MOVA (aopGet(AOP (val), 0, FALSE, FALSE, NULL));
	emitcode ("movx", "@dptr,a");
	emitcode ("inc", "dptr");
	emitcode ("mov","a,b");
	emitcode ("orl","a,_ap");
	emitcode ("jz","!tlabel",lbl1->key+100);
	emitcode ("mov","a,_ap");
	emitcode ("add","a,#!constbyte",0xFF);
	emitcode ("mov","_ap,a");
	emitcode ("mov","a,b");
	emitcode ("addc","a,#!constbyte",0xFF);
	emitcode ("mov","b,a");
	emitcode ("sjmp","!tlabel",lbl->key+100);
	emitcode ("","!tlabeldef",lbl1->key+100);
    }
    freeAsmop (count, NULL, ic, FALSE);
    unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genNatLibLoadPrimitive - calls TINI api function to load primitive */
/*-----------------------------------------------------------------*/
static void genNatLibLoadPrimitive(iCode *ic, int nparms, operand **parms,int size)
{
	bitVect *rsave ;
	operand *pnum, *result;
	int i;
    
	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));
    
	pnum = parms[0]; 
	aopOp (pnum, ic, FALSE, FALSE);
	emitcode ("mov","a,%s",aopGet(AOP(pnum),0,FALSE,FALSE,DP2_RESULT_REG));
	freeAsmop (pnum, NULL, ic, FALSE);
	emitcode ("lcall","NatLib_LoadPrimitive");
	aopOp (result=IC_RESULT(ic), ic, FALSE, FALSE);
	if (aopHasRegs(AOP(result),R0_IDX,R1_IDX) || 
	    aopHasRegs(AOP(result),R2_IDX,R3_IDX) ) {
		for (i = (size-1) ; i >= 0 ; i-- ) {
			emitcode ("push","a%s",javaRet[i]);
		}
		for (i=0; i < size ; i++ ) {
			emitcode ("pop","a%s",
				  aopGet(AOP(result),i,FALSE,FALSE,DP2_RESULT_REG));
		}
	} else {
		for (i = 0 ; i < size ; i++ ) {
			aopPut(AOP(result),javaRet[i],i);
		}
	}    
	freeAsmop (result, NULL, ic, FALSE);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genNatLibLoadPointer - calls TINI api function to load pointer  */
/*-----------------------------------------------------------------*/
static void genNatLibLoadPointer(iCode *ic, int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *pnum, *result;
	int size = 3;
	int i;
    
	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));
    
	pnum = parms[0]; 
	aopOp (pnum, ic, FALSE, FALSE);
	emitcode ("mov","a,%s",aopGet(AOP(pnum),0,FALSE,FALSE,DP2_RESULT_REG));
	freeAsmop (pnum, NULL, ic, FALSE);
	emitcode ("lcall","NatLib_LoadPointer");
	aopOp (result=IC_RESULT(ic), ic, FALSE, FALSE);
	if (AOP_TYPE(result)!=AOP_STR) {
		for (i = 0 ; i < size ; i++ ) {
			aopPut(AOP(result),fReturn[i],i);
		}
	}    
	freeAsmop (result, NULL, ic, FALSE);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genNatLibInstallStateBlock - 				   */
/*-----------------------------------------------------------------*/
static void genNatLibInstallStateBlock(iCode *ic, int nparms, 
				       operand **parms, const char *name)
{
	bitVect *rsave ;
	operand *psb, *handle;
	assert (nparms==2);

	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));
	psb = parms[0];
	handle = parms[1];

	/* put pointer to state block into DPTR1 */
	aopOp (psb, ic, FALSE, FALSE);
	if (AOP_TYPE (psb) == AOP_IMMD) {
		emitcode ("mov","dps,#1");
		emitcode ("mov", "dptr,%s",
			  aopGet (AOP (psb), 0, TRUE, FALSE, DP2_RESULT_REG));
		emitcode ("mov","dps,#0");
	} else {
		emitcode ("mov","dpl1,%s",aopGet(AOP(psb),0,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dph1,%s",aopGet(AOP(psb),1,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dpx1,%s",aopGet(AOP(psb),2,FALSE,FALSE,DP2_RESULT_REG));
	}
	freeAsmop (psb, NULL, ic, FALSE);

	/* put libraryID into DPTR */
	emitcode ("mov","dptr,#LibraryID");

	/* put handle into r3:r2 */
	aopOp (handle, ic, FALSE, FALSE);
	if (aopHasRegs(AOP(handle),R2_IDX,R3_IDX)) {
		emitcode ("push","%s",aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("push","%s",aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("pop","ar3");
		emitcode ("pop","ar2");
	} else {	
		emitcode ("mov","r2,%s",aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));	
		emitcode ("mov","r3,%s",aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
	}
	freeAsmop (psb, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","NatLib_Install%sStateBlock",name);

	/* put return value into place*/
	_G.accInUse++;
	aopOp (IC_RESULT(ic), ic, FALSE, FALSE);
	_G.accInUse--;
	aopPut(AOP(IC_RESULT(ic)),"a",0);
	freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genNatLibRemoveStateBlock -                                     */
/*-----------------------------------------------------------------*/
static void genNatLibRemoveStateBlock(iCode *ic,int nparms,const char *name)
{
	bitVect *rsave ;

	assert(nparms==0);

	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));

	/* put libraryID into DPTR */
	emitcode ("mov","dptr,#LibraryID");
	/* make the call */
	emitcode ("lcall","NatLib_Remove%sStateBlock",name);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genNatLibGetStateBlock -                                        */
/*-----------------------------------------------------------------*/
static void genNatLibGetStateBlock(iCode *ic,int nparms,
				   operand **parms,const char *name)
{
	bitVect *rsave ;
	symbol *lbl = newiTempLabel(NULL);
	
	assert(nparms==0);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));

	/* put libraryID into DPTR */
	emitcode ("mov","dptr,#LibraryID");
	/* make the call */
	emitcode ("lcall","NatLib_Remove%sStateBlock",name);
	emitcode ("jnz","!tlabel",lbl->key+100);

	/* put return value into place */
	aopOp(IC_RESULT(ic),ic,FALSE,FALSE);
	if (aopHasRegs(AOP(IC_RESULT(ic)),R2_IDX,R3_IDX)) {
		emitcode ("push","ar3");
		emitcode ("push","ar2");
		emitcode ("pop","%s",
			  aopGet(AOP(IC_RESULT(ic)),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("pop","%s",
			  aopGet(AOP(IC_RESULT(ic)),1,FALSE,TRUE,DP2_RESULT_REG));
	} else {
		aopPut(AOP(IC_RESULT(ic)),"r2",0);
		aopPut(AOP(IC_RESULT(ic)),"r3",1);
	}
	freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
	emitcode ("","!tlabeldef",lbl->key+100);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genMMMalloc -                                                   */
/*-----------------------------------------------------------------*/
static void genMMMalloc (iCode *ic,int nparms, operand **parms,
			 int size, const char *name)
{
	bitVect *rsave ;
	operand *bsize;
	symbol *rsym;
	symbol *lbl = newiTempLabel(NULL);

	assert (nparms == 1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	
	
	bsize=parms[0];
	aopOp (bsize,ic,FALSE,FALSE);

	/* put the size in R4-R2 */
	if (aopHasRegs(AOP(bsize),R2_IDX, (size==3 ? R4_IDX: R3_IDX))) {
		emitcode("push","%s",aopGet(AOP(bsize),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("push","%s",aopGet(AOP(bsize),1,FALSE,TRUE,DP2_RESULT_REG));
		if (size==3) {
			emitcode("push","%s",aopGet(AOP(bsize),2,FALSE,TRUE,DP2_RESULT_REG));
			emitcode("pop","ar4");
		}
		emitcode("pop","ar3");
		emitcode("pop","ar2");		
	} else {
		emitcode ("mov","r2,%s",aopGet(AOP(bsize),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r3,%s",aopGet(AOP(bsize),1,FALSE,TRUE,DP2_RESULT_REG));
		if (size==3) {
			emitcode("mov","r4,%s",aopGet(AOP(bsize),2,FALSE,TRUE,DP2_RESULT_REG));
		}
	}
	freeAsmop (bsize, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","MM_%s",name);
	emitcode ("jz","!tlabel",lbl->key+100);
	emitcode ("mov","r2,#!constbyte",0xff);
	emitcode ("mov","r3,#!constbyte",0xff);
	emitcode ("","!tlabeldef",lbl->key+100);
	/* we don't care about the pointer : we just save the handle */
	rsym = OP_SYMBOL(IC_RESULT(ic));
	if (rsym->liveFrom != rsym->liveTo) {
		aopOp(IC_RESULT(ic),ic,FALSE,FALSE);
		if (aopHasRegs(AOP(IC_RESULT(ic)),R2_IDX,R3_IDX)) {
			emitcode ("push","ar3");
			emitcode ("push","ar2");
			emitcode ("pop","%s",
				  aopGet(AOP(IC_RESULT(ic)),0,FALSE,TRUE,DP2_RESULT_REG));
			emitcode ("pop","%s",
				  aopGet(AOP(IC_RESULT(ic)),1,FALSE,TRUE,DP2_RESULT_REG));
		} else {
			aopPut(AOP(IC_RESULT(ic)),"r2",0);
			aopPut(AOP(IC_RESULT(ic)),"r3",1);
		}
		freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
	}
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genMMDeref -                                                    */
/*-----------------------------------------------------------------*/
static void genMMDeref (iCode *ic,int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *handle;

	assert (nparms == 1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	
	
	handle=parms[0];
	aopOp (handle,ic,FALSE,FALSE);

	/* put the size in R4-R2 */
	if (aopHasRegs(AOP(handle),R2_IDX,R3_IDX)) {
		emitcode("push","%s",
			 aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("push","%s",
			 aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("pop","ar3");
		emitcode("pop","ar2");		
	} else {
		emitcode ("mov","r2,%s",
			  aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r3,%s",
			  aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
	}
	freeAsmop (handle, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","MM_Deref");
	
	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {			
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			if (AOP_TYPE(IC_RESULT(ic)) != AOP_STR) {
				aopPut(AOP(IC_RESULT(ic)),"dpl",0);
				aopPut(AOP(IC_RESULT(ic)),"dph",1);
				aopPut(AOP(IC_RESULT(ic)),"dpx",2);
			}
		}
	}
	freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genMMUnrestrictedPersist -                                      */
/*-----------------------------------------------------------------*/
static void genMMUnrestrictedPersist(iCode *ic,int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *handle;

	assert (nparms == 1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	
	
	handle=parms[0];
	aopOp (handle,ic,FALSE,FALSE);

	/* put the size in R3-R2 */
	if (aopHasRegs(AOP(handle),R2_IDX,R3_IDX)) {
		emitcode("push","%s",
			 aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("push","%s",
			 aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("pop","ar3");
		emitcode("pop","ar2");		
	} else {
		emitcode ("mov","r2,%s",
			  aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r3,%s",
			  aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
	}
	freeAsmop (handle, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","MM_UnrestrictedPersist");

	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {	
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			aopPut(AOP(IC_RESULT(ic)),"a",0);
			freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
		}
	}
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemExecJavaProcess -                                      */
/*-----------------------------------------------------------------*/
static void genSystemExecJavaProcess(iCode *ic,int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *handle, *pp;

	assert (nparms==2);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	
	
	pp = parms[0];
	handle = parms[1];
	
	/* put the handle in R3-R2 */
	aopOp (handle,ic,FALSE,FALSE);
	if (aopHasRegs(AOP(handle),R2_IDX,R3_IDX)) {
		emitcode("push","%s",
			 aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("push","%s",
			 aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode("pop","ar3");
		emitcode("pop","ar2");		
	} else {
		emitcode ("mov","r2,%s",
			  aopGet(AOP(handle),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r3,%s",
			  aopGet(AOP(handle),1,FALSE,TRUE,DP2_RESULT_REG));
	}
	freeAsmop (handle, NULL, ic, FALSE);
	
	/* put pointer in DPTR */
	aopOp (pp,ic,FALSE,FALSE);
	if (AOP_TYPE(pp) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s",
			  aopGet (AOP (pp), 0, TRUE, FALSE, NULL));
	} else if (AOP_TYPE(pp) != AOP_STR) { /* not already in dptr */
		emitcode ("mov","dpl,%s",aopGet(AOP(pp),0,FALSE,FALSE,NULL));
		emitcode ("mov","dph,%s",aopGet(AOP(pp),1,FALSE,FALSE,NULL));
		emitcode ("mov","dpx,%s",aopGet(AOP(pp),2,FALSE,FALSE,NULL));
	}
	freeAsmop (handle, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","System_ExecJavaProcess");
	
	/* put result in place */
	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {	
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			aopPut(AOP(IC_RESULT(ic)),"a",0);
			freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
		}
	}
	
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemRTCRegisters -                                         */
/*-----------------------------------------------------------------*/
static void genSystemRTCRegisters(iCode *ic,int nparms, operand **parms,
				  char *name)
{
	bitVect *rsave ;
	operand *pp;

	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	
	
	pp=parms[0];
	/* put pointer in DPTR */
	aopOp (pp,ic,FALSE,FALSE);
	if (AOP_TYPE (pp) == AOP_IMMD) {
		emitcode ("mov","dps,#1");
		emitcode ("mov", "dptr,%s", 
			  aopGet (AOP (pp), 0, TRUE, FALSE, NULL));
		emitcode ("mov","dps,#0");
	} else {
		emitcode ("mov","dpl1,%s",
			  aopGet(AOP(pp),0,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dph1,%s",
			  aopGet(AOP(pp),1,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dpx1,%s",
			  aopGet(AOP(pp),2,FALSE,FALSE,DP2_RESULT_REG));
	}
	freeAsmop (pp, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","System_%sRTCRegisters",name);

	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemThreadSleep -                                          */
/*-----------------------------------------------------------------*/
static void genSystemThreadSleep(iCode *ic,int nparms, operand **parms, char *name)
{
	bitVect *rsave ;
	operand *to, *s;

	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));	

	to = parms[0];
	aopOp(to,ic,FALSE,FALSE);
	if (aopHasRegs(AOP(to),R2_IDX,R3_IDX) ||
	    aopHasRegs(AOP(to),R0_IDX,R1_IDX) ) {
		emitcode ("push","%s",
			  aopGet(AOP(to),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("push","%s",
			  aopGet(AOP(to),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("push","%s",
			  aopGet(AOP(to),2,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("push","%s",
			  aopGet(AOP(to),3,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("pop","ar3");
		emitcode ("pop","ar2");
		emitcode ("pop","ar1");
		emitcode ("pop","ar0");
	} else {
		emitcode ("mov","r0,%s",
			  aopGet(AOP(to),0,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r1,%s",
			  aopGet(AOP(to),1,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r2,%s",
			  aopGet(AOP(to),2,FALSE,TRUE,DP2_RESULT_REG));
		emitcode ("mov","r3,%s",
			  aopGet(AOP(to),3,FALSE,TRUE,DP2_RESULT_REG));
	}
	freeAsmop (to, NULL, ic, FALSE);

	/* suspend in acc */
	s = parms[1];
	aopOp(s,ic,FALSE,FALSE);
	emitcode ("mov","a,%s",
		  aopGet(AOP(s),0,FALSE,TRUE,NULL));
	freeAsmop (s, NULL, ic, FALSE);

	/* make the call */
	emitcode ("lcall","System_%s",name);

	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemThreadResume -                                         */
/*-----------------------------------------------------------------*/
static void genSystemThreadResume(iCode *ic,int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *tid,*pid;

	assert (nparms==2);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));
	
	tid = parms[0];
	pid = parms[1];
	
	/* PID in R0 */
	aopOp(pid,ic,FALSE,FALSE);
	emitcode ("mov","r0,%s",
		  aopGet(AOP(pid),0,FALSE,TRUE,DP2_RESULT_REG));
	freeAsmop (pid, NULL, ic, FALSE);
	
	/* tid into ACC */
	aopOp(tid,ic,FALSE,FALSE);
	emitcode ("mov","a,%s",
		  aopGet(AOP(tid),0,FALSE,TRUE,DP2_RESULT_REG));
	freeAsmop (tid, NULL, ic, FALSE);
	
	emitcode ("lcall","System_ThreadResume");

	/* put result into place */
	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {	
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			aopPut(AOP(IC_RESULT(ic)),"a",0);
			freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
		}
	}
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemProcessResume -                                        */
/*-----------------------------------------------------------------*/
static void genSystemProcessResume(iCode *ic,int nparms, operand **parms)
{
	bitVect *rsave ;
	operand *pid;

	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));
	
	pid = parms[0];
	
	/* pid into ACC */
	aopOp(pid,ic,FALSE,FALSE);
	emitcode ("mov","a,%s",
		  aopGet(AOP(pid),0,FALSE,TRUE,DP2_RESULT_REG));
	freeAsmop (pid, NULL, ic, FALSE);
	
	emitcode ("lcall","System_ProcessResume");

	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystem -                                                     */
/*-----------------------------------------------------------------*/
static void genSystem (iCode *ic,int nparms,char *name)
{
	assert(nparms == 0);

	emitcode ("lcall","System_%s",name);
}

/*-----------------------------------------------------------------*/
/* genSystemPoll -                                                  */
/*-----------------------------------------------------------------*/
static void genSystemPoll(iCode *ic,int nparms, operand **parms,char *name)
{
	bitVect *rsave ;
	operand *fp;

	assert (nparms==1);
	/* save registers that need to be saved */
	savermask(rsave = bitVectCplAnd (bitVectCopy (ic->rMask),
					 ds390_rUmaskForOp (IC_RESULT(ic))));

	fp = parms[0];
	aopOp (fp,ic,FALSE,FALSE);
	if (AOP_TYPE (fp) == AOP_IMMD) {
		emitcode ("mov", "dptr,%s", 
			  aopGet (AOP (fp), 0, TRUE, FALSE, DP2_RESULT_REG));
	} else if (AOP_TYPE(fp) != AOP_STR) { /* not already in dptr */
		emitcode ("mov","dpl,%s",
			  aopGet(AOP(fp),0,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dph,%s",
			  aopGet(AOP(fp),1,FALSE,FALSE,DP2_RESULT_REG));
		emitcode ("mov","dpx,%s",
			  aopGet(AOP(fp),2,FALSE,FALSE,DP2_RESULT_REG));
	}
	freeAsmop (fp, NULL, ic, FALSE);

	emitcode ("lcall","System_%sPoll",name);

	/* put result into place */
	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {	
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			aopPut(AOP(IC_RESULT(ic)),"a",0);
			freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
		}
	}
	unsavermask(rsave);
}

/*-----------------------------------------------------------------*/
/* genSystemGetCurrentID -                                         */
/*-----------------------------------------------------------------*/
static void genSystemGetCurrentID(iCode *ic,int nparms, operand **parms,char *name)
{
	assert (nparms==0);

	emitcode ("lcall","System_GetCurrent%sId",name);
	/* put result into place */
	{
		symbol *rsym = OP_SYMBOL(IC_RESULT(ic));
		if (rsym->liveFrom != rsym->liveTo) {	
			aopOp (IC_RESULT(ic),ic,FALSE,FALSE);
			aopPut(AOP(IC_RESULT(ic)),"a",0);
			freeAsmop (IC_RESULT(ic), NULL, ic, FALSE);
		}
	}
}

/*-----------------------------------------------------------------*/
/* genBuiltIn - calls the appropriate function to  generating code */
/* for a built in function 					   */
/*-----------------------------------------------------------------*/
static void genBuiltIn (iCode *ic)
{
	operand *bi_parms[MAX_BUILTIN_ARGS];
	int nbi_parms;
	iCode *bi_iCode;
	symbol *bif;

	/* get all the arguments for a built in function */
	bi_iCode = getBuiltinParms(ic,&nbi_parms,bi_parms);

	/* which function is it */
	bif = OP_SYMBOL(IC_LEFT(bi_iCode));
	if (strcmp(bif->name,"__builtin_memcpy_x2x")==0) {
		genMemcpyX2X(bi_iCode,nbi_parms,bi_parms,0);
	} else if (strcmp(bif->name,"__builtin_memcpy_c2x")==0) {
		genMemcpyX2X(bi_iCode,nbi_parms,bi_parms,1);
	} else	if (strcmp(bif->name,"__builtin_memcmp_x2x")==0) {
		genMemcmpX2X(bi_iCode,nbi_parms,bi_parms,0);
	} else if (strcmp(bif->name,"__builtin_memcmp_c2x")==0) {
		genMemcmpX2X(bi_iCode,nbi_parms,bi_parms,1);
	} else if (strcmp(bif->name,"__builtin_memset_x")==0) {
		genMemsetX(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"__builtin_inp")==0) {
		genInp(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"__builtin_outp")==0) {
		genOutp(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"__builtin_swapw")==0) {
	        genSwapW(bi_iCode,nbi_parms,bi_parms);
		/* JavaNative builtIns */		
	} else if (strcmp(bif->name,"NatLib_LoadByte")==0) {
		genNatLibLoadPrimitive(bi_iCode,nbi_parms,bi_parms,1);
	} else if (strcmp(bif->name,"NatLib_LoadShort")==0) {
		genNatLibLoadPrimitive(bi_iCode,nbi_parms,bi_parms,2);
	} else if (strcmp(bif->name,"NatLib_LoadInt")==0) {
		genNatLibLoadPrimitive(bi_iCode,nbi_parms,bi_parms,4);
	} else if (strcmp(bif->name,"NatLib_LoadPointer")==0) {
		genNatLibLoadPointer(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"NatLib_InstallImmutableStateBlock")==0) {
		genNatLibInstallStateBlock(bi_iCode,nbi_parms,bi_parms,"Immutable");
	} else if (strcmp(bif->name,"NatLib_InstallEphemeralStateBlock")==0) {
		genNatLibInstallStateBlock(bi_iCode,nbi_parms,bi_parms,"Ephemeral");
	} else if (strcmp(bif->name,"NatLib_RemoveImmutableStateBlock")==0) {
		genNatLibRemoveStateBlock(bi_iCode,nbi_parms,"Immutable");
	} else if (strcmp(bif->name,"NatLib_RemoveEphemeralStateBlock")==0) {
		genNatLibRemoveStateBlock(bi_iCode,nbi_parms,"Ephemeral");
	} else if (strcmp(bif->name,"NatLib_GetImmutableStateBlock")==0) {
		genNatLibGetStateBlock(bi_iCode,nbi_parms,bi_parms,"Immutable");
	} else if (strcmp(bif->name,"NatLib_GetEphemeralStateBlock")==0) {
		genNatLibGetStateBlock(bi_iCode,nbi_parms,bi_parms,"Ephemeral");
	} else if (strcmp(bif->name,"MM_XMalloc")==0) {
		genMMMalloc(bi_iCode,nbi_parms,bi_parms,3,"XMalloc");
	} else if (strcmp(bif->name,"MM_Malloc")==0) {
		genMMMalloc(bi_iCode,nbi_parms,bi_parms,2,"Malloc");
	} else if (strcmp(bif->name,"MM_ApplicationMalloc")==0) {
		genMMMalloc(bi_iCode,nbi_parms,bi_parms,2,"ApplicationMalloc");
	} else if (strcmp(bif->name,"MM_Free")==0) {
		genMMMalloc(bi_iCode,nbi_parms,bi_parms,2,"Free");
	} else if (strcmp(bif->name,"MM_Deref")==0) {
		genMMDeref(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"MM_UnrestrictedPersist")==0) {
		genMMUnrestrictedPersist(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_ExecJavaProcess")==0) {
		genSystemExecJavaProcess(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_GetRTCRegisters")==0) {
		genSystemRTCRegisters(bi_iCode,nbi_parms,bi_parms,"Get");
	} else if (strcmp(bif->name,"System_SetRTCRegisters")==0) {
		genSystemRTCRegisters(bi_iCode,nbi_parms,bi_parms,"Set");
	} else if (strcmp(bif->name,"System_ThreadSleep")==0) {
		genSystemThreadSleep(bi_iCode,nbi_parms,bi_parms,"ThreadSleep");
	} else if (strcmp(bif->name,"System_ThreadSleep_ExitCriticalSection")==0) {
		genSystemThreadSleep(bi_iCode,nbi_parms,bi_parms,"ThreadSleep_ExitCriticalSection");
	} else if (strcmp(bif->name,"System_ProcessSleep")==0) {
		genSystemThreadSleep(bi_iCode,nbi_parms,bi_parms,"ProcessSleep");
	} else if (strcmp(bif->name,"System_ProcessSleep_ExitCriticalSection")==0) {
		genSystemThreadSleep(bi_iCode,nbi_parms,bi_parms,"ProcessSleep_ExitCriticalSection");
	} else if (strcmp(bif->name,"System_ThreadResume")==0) {
		genSystemThreadResume(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_SaveThread")==0) {
		genSystemThreadResume(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_ThreadResume")==0) {
		genSystemThreadResume(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_ProcessResume")==0) {
		genSystemProcessResume(bi_iCode,nbi_parms,bi_parms);
	} else if (strcmp(bif->name,"System_SaveJavaThreadState")==0) {
		genSystem(bi_iCode,nbi_parms,"SaveJavaThreadState");
	} else if (strcmp(bif->name,"System_RestoreJavaThreadState")==0) {
		genSystem(bi_iCode,nbi_parms,"RestoreJavaThreadState");
	} else if (strcmp(bif->name,"System_ProcessYield")==0) {
		genSystem(bi_iCode,nbi_parms,"ProcessYield");
	} else if (strcmp(bif->name,"System_ProcessSuspend")==0) {
		genSystem(bi_iCode,nbi_parms,"ProcessSuspend");
	} else if (strcmp(bif->name,"System_RegisterPoll")==0) {
		genSystemPoll(bi_iCode,nbi_parms,bi_parms,"Register");
	} else if (strcmp(bif->name,"System_RemovePoll")==0) {
		genSystemPoll(bi_iCode,nbi_parms,bi_parms,"Remove");
	} else if (strcmp(bif->name,"System_GetCurrentThreadId")==0) {
		genSystemGetCurrentID(bi_iCode,nbi_parms,bi_parms,"Thread");
	} else if (strcmp(bif->name,"System_GetCurrentProcessId")==0) {
		genSystemGetCurrentID(bi_iCode,nbi_parms,bi_parms,"Process");
	} else {
		werror(E_INTERNAL_ERROR,__FILE__,__LINE__,"unknown builtin function encountered\n");
		return ;
	}
	return ;    
}

/*-----------------------------------------------------------------*/
/* gen390Code - generate code for Dallas 390 based controllers     */
/*-----------------------------------------------------------------*/
void
gen390Code (iCode * lic)
{
  iCode *ic;
  int cln = 0;

  lineHead = lineCurr = NULL;
  dptrn[1][0] = "dpl1";
  dptrn[1][1] = "dph1";
  dptrn[1][2] = "dpx1";
  
  if (options.model == MODEL_FLAT24) {
    fReturnSizeDS390 = 5;
    fReturn = fReturn24;
  } else {
    fReturnSizeDS390 = 4;
    fReturn = fReturn16;
    options.stack10bit=0;
  }
#if 1
  /* print the allocation information */
  if (allocInfo)
    printAllocInfo (currFunc, codeOutFile);
#endif
  /* if debug information required */
  if (options.debug && currFunc)
    {
      cdbSymbol (currFunc, cdbFile, FALSE, TRUE);
      _G.debugLine = 1;
      if (IS_STATIC (currFunc->etype))
	emitcode ("", "F%s$%s$0$0 ==.", moduleName, currFunc->name);
      else
	emitcode ("", "G$%s$0$0 ==.", currFunc->name);
      _G.debugLine = 0;
    }
  /* stack pointer name */
  if (options.useXstack)
    spname = "_spx";
  else
    spname = "sp";


  for (ic = lic; ic; ic = ic->next)
    {

      if (ic->lineno && cln != ic->lineno)
	{
	  if (options.debug)
	    {
	      _G.debugLine = 1;
	      emitcode ("", "C$%s$%d$%d$%d ==.",
			FileBaseName (ic->filename), ic->lineno,
			ic->level, ic->block);
	      _G.debugLine = 0;
	    }
	  if (!options.noCcodeInAsm) {
	    emitcode ("", ";\t%s:%d: %s", ic->filename, ic->lineno, 
		      printCLine(ic->filename, ic->lineno));
	  }
	  cln = ic->lineno;
	}
      if (options.iCodeInAsm) {
	emitcode("", ";ic:%d: %s", ic->key, printILine(ic));
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
	     during parsing SDCC.y */
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

	case LEFT_OP:
	  genLeftShift (ic);
	  break;

	case RIGHT_OP:
	  genRightShift (ic);
	  break;

	case GET_VALUE_AT_ADDRESS:
	  genPointerGet (ic,hasInc(IC_LEFT(ic),ic, getSize(operandType(IC_RESULT(ic)))));
	  break;

	case '=':
	  if (POINTER_SET (ic))
	    genPointerSet (ic,hasInc(IC_RESULT(ic),ic,getSize(operandType(IC_RIGHT(ic)))));
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
	  if (ic->builtinSEND) genBuiltIn(ic);
	  else addSet (&_G.sendSet, ic);
	  break;

	case ARRAYINIT:
	    genArrayInit(ic);
	    break;
	    
	default:
	  ic = ic;
	}
    }


  /* now we are ready to call the
     peep hole optimizer */
  if (!options.nopeep)
    peepHole (&lineHead);

  /* now do the actual printing */
  printLine (lineHead, codeOutFile);
  return;
}
