/* The only ops for now are:
   genAssign
   genIfx
   genAddrOf
   genPointerSet
*/

/*-------------------------------------------------------------------------
  SDCCgen51.c - source file for code generation for 8051

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

//#define D(x)
#define D(x) x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

#include "common.h"
#include "SDCCpeeph.h"
#include "ralloc.h"
#include "gen.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

extern int allocInfo;

/* this is the down and dirty file with all kinds of
   kludgy & hacky stuff. This is what it is all about
   CODE GENERATION for a specific MCU . some of the
   routines may be reusable, will have to see */

static struct
  {
    short inLine;
    short debugLine;
    short nRegsSaved;
    set *sendSet;
  }
_G;

extern int xa51_ptrRegReq;
extern int xa51_nRegs;
extern FILE *codeOutFile;

static lineNode *lineHead = NULL;
static lineNode *lineCurr = NULL;

#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3

void bailOut (char *mesg) {
  fprintf (stderr, "%s: bailing out\n", mesg);
  exit (1);
}

/*-----------------------------------------------------------------*/
/* emitcode - writes the code into a file : for now it is simple    */
/*-----------------------------------------------------------------*/
static void emitcode (char *inst, char *fmt,...) {
  va_list ap;
  char lb[INITIAL_INLINEASM];
  char *lbp = lb;

  va_start (ap, fmt);

  if (inst && *inst)
    {
      if (fmt && *fmt)
	sprintf (lb, "%s\t", inst);
      else
	sprintf (lb, "%s", inst);
      vsprintf (lb + (strlen (lb)), fmt, ap);
    }
  else
    vsprintf (lb, fmt, ap);

  while (isspace ((int)*lbp))
    lbp++;

  if (lbp && *lbp)
    lineCurr = (lineCurr ?
		connectLine (lineCurr, newLineNode (lb)) :
		(lineHead = newLineNode (lb)));
  lineCurr->isInline = _G.inLine;
  lineCurr->isDebug = _G.debugLine;
  va_end (ap);
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

char *aopTypeName(asmop *aop) {
  switch (aop->type)
    {
    case AOP_LIT: return "lit";
    case AOP_REG: return "reg";
    case AOP_DIR: return "dir";
    case AOP_FAR: return "far";
    case AOP_CODE: return "code";
    case AOP_STK: return "stack";
    case AOP_IMMD: return "imm";
    case AOP_CRY: return "bit";
    }
  return "unknown";
}

/*-----------------------------------------------------------------*/
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *aopForSym(symbol *sym, bool result) {
  asmop *aop;

  sym->aop = aop = newAsmop(0);
  aop->size=getSize(sym->type);

  // if it is in registers
  if (sym->nRegs && sym->regs[0]) {
    aop->type=AOP_REG;
    sprintf (aop->name[0], sym->regs[0]->name);
    if (sym->regs[1]) {
      sprintf (aop->name[1], sym->regs[1]->name);
    }
    return aop;
  }

  // if it is on stack
  if (sym->onStack) {
    aop->type=AOP_STK;
    sprintf (aop->name[0], "[r7%+d+0%+d%+d]", sym->stack, 
	     FUNC_ISISR(currFunc->type) ? 6 : 4, _G.nRegsSaved);
    if (aop->size > 2) {
      sprintf (aop->name[1], "[r7%+d+2%+d%+d]", sym->stack,
	       FUNC_ISISR(currFunc->type) ? 6 : 4, _G.nRegsSaved);
    }
    return aop;
  }

  // if it has a spillLoc
  if (sym->usl.spillLoc) {
    return aopForSym (sym->usl.spillLoc, result);
  }

  // if in bit space
  if (IN_BITSPACE(SPEC_OCLS(sym->etype))) {
    aop->type=AOP_CRY;
    sprintf (aop->name[0], sym->rname);
    return aop;
  }

  // if in direct space
  if (IN_DIRSPACE(SPEC_OCLS(sym->etype))) {
    aop->type=AOP_DIR;
    sprintf (aop->name[0], sym->rname);
    return aop;
  }

  // if in code space
  if (IN_CODESPACE(SPEC_OCLS(sym->etype))) {
    if (result) {
      fprintf (stderr, "aopForSym: result can not be in code space\n");
      exit (1);
    }
    aop->type=AOP_CODE;
    emitcode ("mov", "r0,#%s", sym->rname);
    sprintf (aop->name[0], "r0");
    return aop;
  }

  // if in far space
  if (IN_FARSPACE(SPEC_OCLS(sym->etype))) {
    aop->type=AOP_FAR;
    emitcode ("mov", "r0,#%s", sym->rname);
    sprintf (aop->name[0], "r0");
    return aop;
  }

  fprintf (stderr, "aopForSym (%s): What's up?\n", sym->name);
  exit (1);
}

/*-----------------------------------------------------------------*/
/* aopForVal - for a value                                         */
/*-----------------------------------------------------------------*/
static asmop *aopForVal(operand *op) {
  asmop *aop;
  long v=(long long)floatFromVal(OP_VALUE(op));

  if (IS_OP_LITERAL(op)) {
    op->aop = aop = newAsmop (AOP_LIT);
    switch ((aop->size=getSize(operandType(op))))
      {
      case 1:
	sprintf (aop->name[0], "#0x%02lx", v);
	break;
      case 2:
	sprintf (aop->name[0], "#0x%04lx", v);
	break;
      case 4:
	sprintf (aop->name[0], "#(0x%08lx >> 16)", v);
	sprintf (aop->name[1], "#(0x%08lx & 0xffff)", v);
	break;
      default:
	bailOut("aopForVal");
      }
    return aop;
  }

  // must be immediate
  if (IS_SYMOP(op)) {
    op->aop = aop = newAsmop (AOP_IMMD);
    switch ((aop->size=getSize(OP_SYMBOL(op)->type))) 
      {
      case 1:
      case 2:
	sprintf (aop->name[0], "#%s", OP_SYMBOL(op)->rname);
	return aop;
      case 3: // generic pointer
	sprintf (aop->name[0], "#0x%02x", DCL_TYPE(operandType(op)));
	sprintf (aop->name[1], "#%s", OP_SYMBOL(op)->rname);
	return aop;
      }
  }
  fprintf (stderr, "aopForVal: unknown type\n");
  exit (1);
  return NULL;
}

static void aopOp(operand *op, bool result) {

  if (IS_SYMOP(op)) {
    op->aop=aopForSym (OP_SYMBOL(op), result);
    return;
  }
  if (IS_VALOP(op)) {
    if (result) {
      fprintf (stderr, "aopOp: result can not be a value\n");
      exit (1);
    }
    aopForVal (op);
    return;
  }

  fprintf (stderr, "aopOp: unexpected operand\n");
  exit (1);
}

char *opRegName(operand *op, int offset, char *opName) {

  if (IS_SYMOP(op)) {
    if (OP_SYMBOL(op)->onStack) {
      sprintf (opName, "[r7%+d+0%+d%+d]", OP_SYMBOL(op)->stack, 
	       FUNC_ISISR(currFunc->type) ? 6 : 4, _G.nRegsSaved);
      return opName;
    }
    if (IS_TRUE_SYMOP(op))
      return OP_SYMBOL(op)->rname;
    else if (OP_SYMBOL(op)->regs[offset])
      return OP_SYMBOL(op)->regs[offset]->name;
    else
      return "NULL";
  }

  if (IS_VALOP(op)) {
    switch (SPEC_NOUN(OP_VALUE(op)->type)) {
    case V_SBIT:
    case V_BIT:
      if (SPEC_CVAL(OP_VALUE(op)->type).v_int &&
	  SPEC_CVAL(OP_VALUE(op)->type).v_int != 1) {
	fprintf (stderr, "opRegName: invalid bit value (%d)\n",
		 SPEC_CVAL(OP_VALUE(op)->type).v_int);
	exit (1);
      }
      // fall through
    case V_CHAR:
      sprintf (opName, "#0x%02x", SPEC_CVAL(OP_VALUE(op)->type).v_int);
      break;
    case V_INT:
      if (SPEC_LONG(OP_VALUE(op)->type)) {
	sprintf (opName, "#0x%02lx", SPEC_CVAL(OP_VALUE(op)->type).v_long);
      } else {
	sprintf (opName, "#0x%02x", SPEC_CVAL(OP_VALUE(op)->type).v_int);
      }
      break;
    case V_FLOAT:
      sprintf (opName, "#0x%02lx", SPEC_CVAL(OP_VALUE(op)->type).v_long);
      break;
    default: 
      fprintf (stderr, "opRegName: unexpected noun\n");
      exit (1);
    }
    return opName;
  }
  fprintf (stderr, "opRegName: unexpected operand type\n");
  exit (1);
  return NULL; // to keep the compiler happy
}

char * printOp (operand *op) {
  static char line[132];
  sym_link *optype=operandType(op);
  bool isPtr = IS_PTR(optype);

  if (IS_SYMOP(op)) {
    symbol *sym=OP_SYMBOL(op);
    if (!sym->regs[0] && SYM_SPIL_LOC(sym)) {
      sym=SYM_SPIL_LOC(sym);
    }
    if (isPtr) {
      sprintf (line, "[");
      if (DCL_TYPE(optype)==FPOINTER)
	strcat (line, "far * ");
      else if (DCL_TYPE(optype)==CPOINTER)
	strcat (line, "code * ");
      else if (DCL_TYPE(optype)==GPOINTER)
	strcat (line, "gen * ");
      else if (DCL_TYPE(optype)==POINTER)
	strcat (line, "near * ");
      else
	strcat (line, "unknown * ");
      strcat (line, "(");
      strcat (line, nounName(sym->etype));
      strcat (line, ")");
      strcat (line, sym->name);
      strcat (line, "]:");
    } else {
      sprintf (line, "(%s)%s:", nounName(sym->etype), sym->name);
    }
    if (sym->regs[0]) {
      strcat (line, sym->regs[0]->name);
      if (sym->regs[1]) {
	strcat (line, ",");
	strcat (line, sym->regs[1]->name);
      }
      return line;
    }
    if (sym->onStack) {
      sprintf (line+strlen(line), "stack%+d", sym->stack);
      return line;
    }
    if (IN_FARSPACE(SPEC_OCLS(sym->etype))) {
      strcat (line, "far");
      return line;
    }
    if (IN_BITSPACE(SPEC_OCLS(sym->etype))) {
      strcat (line, "bit");
      return line;
    }
    if (IN_DIRSPACE(SPEC_OCLS(sym->etype))) {
      strcat (line, "dir");
      return line;
    }
    strcat (line, "unknown");
    return line;
  } else if (IS_VALOP(op)) {
    opRegName(op, 0, line);
  } else if (IS_TYPOP(op)) {
    sprintf (line, "[");
    if (isPtr) {
      if (DCL_TYPE(optype)==FPOINTER)
	strcat (line, "far * ");
      else if (DCL_TYPE(optype)==CPOINTER)
	strcat (line, "code * ");
      else if (DCL_TYPE(optype)==GPOINTER)
	strcat (line, "gen * ");
      else if (DCL_TYPE(optype)==POINTER)
	strcat (line, "near * ");
      else
	strcat (line, "unknown * ");
    }
    // forget about static, volatile, ... for now
    if (SPEC_USIGN(operandType(op))) strcat (line, "unsigned ");
    if (SPEC_LONG(operandType(op))) strcat (line, "long ");
    strcat (line, nounName(operandType(op)));
    strcat (line, "]");
  } else {
    fprintf (stderr, "printOp: unexpected operand type\n");
    exit (1);
  }
  return line;
}

void printIc (char *op, iCode * ic, bool result, bool left, bool right) {
  char line[132];

  sprintf (line, "%s(%d)", op, ic->lineno);
  if (result) {
    strcat (line, " result=");
    strcat (line, printOp (IC_RESULT(ic)));
  }
  if (left) {
    strcat (line, " left=");
    strcat (line, printOp (IC_LEFT(ic)));
  }
  if (right) {
    strcat (line, " right=");
    strcat (line, printOp (IC_RIGHT(ic)));
  }
  emitcode (";", line);
}

/*-----------------------------------------------------------------*/
/* toBoolean - return bit for operand!=0                           */
/*-----------------------------------------------------------------*/
static char *toBoolean (operand * op) {
  switch (AOP_SIZE(op)) 
    {
    case 1:
    case 2:
      emitcode ("cmp", "%s,#0", AOP_NAME(op));
      return "z";
    }

  fprintf (stderr, "toBoolean: unknown size %d\n", AOP_SIZE(op));
  exit (1);
  return NULL;
}

/*-----------------------------------------------------------------*/
/* regsInCommon - two operands have some registers in common       */
/*-----------------------------------------------------------------*/
static bool regsInCommon (operand * op1, operand * op2) {
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
/* resultRemat - result  is rematerializable                       */
/*-----------------------------------------------------------------*/
static int resultRemat (iCode * ic) {
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
/* genNot - generate code for ! operation                          */
/*-----------------------------------------------------------------*/
static void genNot (iCode * ic) {
  printIc("genNot:", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genCpl - generate code for complement                           */
/*-----------------------------------------------------------------*/
static void genCpl (iCode * ic) {
  printIc("genCpl", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genUminus - unary minus code generation                         */
/*-----------------------------------------------------------------*/
static void genUminus (iCode * ic) {
  printIc("genUminus", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genIpush - genrate code for pushing this gets a little complex  */
/*-----------------------------------------------------------------*/
static void genIpush (iCode * ic) {
  printIc ("genIpush", ic, 0,1,0);
}

/*-----------------------------------------------------------------*/
/* genIpop - recover the registers: can happen only for spilling   */
/*-----------------------------------------------------------------*/
static void genIpop (iCode * ic) {
  printIc ("genIpop", ic, 0,1,0);
}

/*-----------------------------------------------------------------*/
/* genCall - generates a call statement                            */
/*-----------------------------------------------------------------*/
static void genCall (iCode * ic) {
  emitcode (";", "genCall %s result=%s", OP_SYMBOL(IC_LEFT(ic))->name,
	    printOp (IC_RESULT(ic)));
}

/*-----------------------------------------------------------------*/
/* genPcall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void
genPcall (iCode * ic)
{
  emitcode (";", "genPcall %s\n", OP_SYMBOL(IC_LEFT(ic))->name);
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void genFunction (iCode * ic) {
  symbol *sym=OP_SYMBOL(IC_LEFT(ic));
  sym_link *type=sym->type;

  emitcode (";", "-----------------------------------------");
  emitcode (";", " function %s", sym->name);
  emitcode (";", "-----------------------------------------");
  
  emitcode ("", "%s:", sym->rname);

  if (IFFUNC_ISNAKED(type))
  {
      emitcode(";", "naked function: no prologue.");
      return;
  }

  /* if critical function then turn interrupts off */
  if (IFFUNC_ISCRITICAL (type))
    emitcode ("clr", "ea");

}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void
genEndFunction (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));

  if (IFFUNC_ISNAKED(sym->type))
  {
      emitcode(";", "naked function: no epilogue.");
      return;
  }

  printIc ("genEndFunction", ic, 0,0,0);
}

/*-----------------------------------------------------------------*/
/* genRet - generate code for return statement                     */
/*-----------------------------------------------------------------*/
static void genRet (iCode * ic) {
  printIc ("genRet", ic, 0,1,0);
}

/*-----------------------------------------------------------------*/
/* genLabel - generates a label                                    */
/*-----------------------------------------------------------------*/
static void genLabel (iCode * ic) {
  /* special case never generate */
  if (IC_LABEL (ic) == entryLabel)
    return;

  emitcode (";", "genLabel %s", IC_LABEL(ic)->name);
  emitcode ("", "%05d$:", (IC_LABEL (ic)->key + 100));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a ljmp                                      */
/*-----------------------------------------------------------------*/
static void genGoto (iCode * ic) {
  emitcode (";", "genGoto %s", IC_LABEL(ic)->name);
  emitcode ("ljmp", "%05d$", (IC_LABEL (ic)->key + 100));
}

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void genPlus (iCode * ic) {
  printIc ("genPlus", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
static void genMinus (iCode * ic) {
  printIc ("genMinus", ic, 1,1,1);
}


/*-----------------------------------------------------------------*/
/* genMult - generates code for multiplication                     */
/*-----------------------------------------------------------------*/
static void genMult (iCode * ic) {
  printIc ("genMult", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genDiv - generates code for division                            */
/*-----------------------------------------------------------------*/
static void genDiv (iCode * ic) {
  printIc ("genDiv", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genMod - generates code for division                            */
/*-----------------------------------------------------------------*/
static void genMod (iCode * ic) {
  printIc ("genMod", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genCmpGt :- greater than comparison                             */
/*-----------------------------------------------------------------*/
static void genCmpGt (iCode * ic, iCode * ifx) {
  printIc ("genCmpGt", ic, 1,1,1);
}
/*-----------------------------------------------------------------*/
/* genCmpLt - less than comparisons                                */
/*-----------------------------------------------------------------*/
static void genCmpLt (iCode * ic, iCode * ifx) {
  printIc ("genCmpLt", ic, 1,1,1);
}
/*-----------------------------------------------------------------*/
/* genCmpEq - generates code for equal to                          */
/*-----------------------------------------------------------------*/
static void genCmpEq (iCode * ic, iCode * ifx) {
  printIc ("genCmpEq", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* ifxForOp - returns the icode containing the ifx for operand     */
/*-----------------------------------------------------------------*/
static iCode *ifxForOp (operand * op, iCode * ic) {
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
static iCode *hasInc (operand *op, iCode *ic, int osize) {
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
static void genAndOp (iCode * ic) {
  printIc ("genAndOp(&&)", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genOrOp - for || operation                                      */
/*-----------------------------------------------------------------*/
static void genOrOp (iCode * ic) {
  printIc ("genOrOp(||)", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genAnd  - code for and                                            */
/*-----------------------------------------------------------------*/
static void genAnd (iCode * ic, iCode * ifx) {
  printIc ("genAnd", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genOr  - code for or                                            */
/*-----------------------------------------------------------------*/
static void genOr (iCode * ic, iCode * ifx) {
  printIc ("genOr", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genXor - code for xclusive or                                   */
/*-----------------------------------------------------------------*/
static void genXor (iCode * ic, iCode * ifx) {
  printIc ("genXor", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genInline - write the inline code out                           */
/*-----------------------------------------------------------------*/
static void genInline (iCode * ic) {
  printIc ("genInline", ic, 0,0,0);
}

/*-----------------------------------------------------------------*/
/* genRRC - rotate right with carry                                */
/*-----------------------------------------------------------------*/
static void genRRC (iCode * ic) {
  printIc ("genRRC", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genRLC - generate code for rotate left with carry               */
/*-----------------------------------------------------------------*/
static void genRLC (iCode * ic) {
  printIc ("genRLC", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genGetHbit - generates code get highest order bit               */
/*-----------------------------------------------------------------*/
static void genGetHbit (iCode * ic) {
  printIc ("genGetHbit", ic, 1,1,0);
}

/*-----------------------------------------------------------------*/
/* genLeftShift - generates code for left shifting                 */
/*-----------------------------------------------------------------*/
static void genLeftShift (iCode * ic) {
  printIc ("genLeftShift", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genRightShift - generate code for right shifting                */
/*-----------------------------------------------------------------*/
static void genRightShift (iCode * ic) {
  printIc ("genRightShift", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void genPointerGet (iCode * ic, iCode *pi) {
  char *instr="mov";

  operand *result=IC_RESULT(ic), *left=IC_LEFT(ic);

  printIc ("genPointerGet", ic, 1,1,0);

  if (!IS_PTR(operandType(left))) {
    bailOut ("genPointerGet: pointer required");
  }

  aopOp(left,FALSE);
  aopOp(result,TRUE);

  if (IS_GENPTR(operandType(left))) {
    emitcode ("INLINE", "_gptrget %s %s= [%s,%s]", 
	      AOP_NAME(result)[0], AOP_NAME(result)[1],
	      AOP_NAME(left)[0], AOP_NAME(left)[1]);
    return;
  }

  switch (AOP_TYPE(left)) 
    {
    case AOP_CODE:
      instr="movc";
      // fall through
    case AOP_REG:
    case AOP_DIR:
    case AOP_FAR:
    case AOP_STK:
      emitcode (instr, "%s,[%s]", AOP_NAME(result)[0], AOP_NAME(left)[0]);
      if (AOP_SIZE(result) > 2) {
	emitcode (instr, "%s,[%s]", AOP_NAME(result)[1], AOP_NAME(left)[1]);
      }
      return;
    case AOP_GPTR:
    }
  bailOut ("genPointerGet: unknown pointer");
}

/*-----------------------------------------------------------------*/
/* genPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void genPointerSet (iCode * ic, iCode *pi) {
  char *instr="mov";

  operand *result=IC_RESULT(ic), *right=IC_RIGHT(ic);

  printIc ("genPointerSet", ic, 1,0,1);

  if (!IS_PTR(operandType(result))) {
    bailOut ("genPointerSet: pointer required");
  }

  aopOp(right,FALSE);
  aopOp(result,TRUE);

  if (IS_GENPTR(operandType(result))) {
    emitcode ("INLINE", "_gptrset %s,%s= [%s,%s]", 
	      AOP_NAME(result)[0], AOP_NAME(result)[1],
	      AOP_NAME(right)[0], AOP_NAME(right)[1]);
    return;
  }

  switch (AOP_TYPE(result)) 
    {
    case AOP_CODE:
      instr="movc";
      // fall through
    case AOP_REG:
    case AOP_DIR:
    case AOP_FAR:
    case AOP_STK:
      emitcode (instr, "[%s],%s", AOP_NAME(result)[0], AOP_NAME(right)[0]);
      if (AOP_SIZE(result) > 2) {
	emitcode (instr, "[%s],%s", AOP_NAME(result)[1], AOP_NAME(right)[1]);
      }
      return;
    case AOP_GPTR:
    }
  bailOut ("genPointerSet: unknown pointer");
}

/*-----------------------------------------------------------------*/
/* genIfx - generate code for Ifx statement                        */
/*-----------------------------------------------------------------*/
static void genIfx (iCode * ic, iCode * popIc) {
  bool trueOrFalse;
  symbol *jlbl, *tlbl;
  operand *cond=IC_COND(ic);

  emitcode (";", "genIfx cond=%s trueLabel:%s falseLabel:%s", 
	    printOp(cond),
	    IC_TRUE(ic) ? IC_TRUE(ic)->name : "NULL",
	    IC_FALSE(ic) ? IC_FALSE(ic)->name : "NULL");

  aopOp(cond,FALSE);

  if (IC_TRUE(ic)) {
    trueOrFalse=TRUE;
    jlbl=IC_TRUE(ic);
  } else {
    trueOrFalse=FALSE;
    jlbl=IC_FALSE(ic);
  }

  switch (AOP_TYPE(cond) )
    {
    case AOP_CRY:
      emitcode (trueOrFalse ? "jb" : "jbc", "%s,%05d$", 
		AOP_NAME(cond)[0], jlbl->key+100);
      return;
    case AOP_REG:
    case AOP_DIR:
    case AOP_FAR:
    case AOP_STK:
      tlbl=newiTempLabel(NULL);
      emitcode ("cmp", "%s,#0", AOP_NAME(cond)[0]);
      emitcode (trueOrFalse ? "bne" : "beq", "%05d$", tlbl->key+100);
      if (AOP_SIZE(cond) > 2) {
	emitcode ("cmp", "%s,#0", AOP_NAME(cond)[1]);
	emitcode (trueOrFalse ? "bne" : "beq", "%05d$", tlbl->key+100);
      }
      emitcode ("jmp", "%05d$", jlbl->key+100);
      emitcode ("", "%05d$:", tlbl->key+100);
      return;
    }
  bailOut ("genIfx");
}

/*-----------------------------------------------------------------*/
/* genAddrOf - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void genAddrOf (iCode * ic) {
  operand *left=IC_LEFT(ic);

  printIc ("genAddrOf", ic, 1,1,0);

  aopOp (IC_RESULT(ic),TRUE);

  if (isOperandOnStack(left)) {
    aopOp (IC_LEFT(ic),FALSE);
    emitcode ("lea", "%s,%s", AOP_NAME(IC_RESULT(ic))[0],
	      AOP_NAME(IC_LEFT(ic))[0]);
    if (AOP_SIZE(IC_RESULT(ic)) > 2) {
      // this must be a generic pointer
      emitcode ("mov", "%s,#0x01", AOP_NAME(IC_RESULT(ic))[1]);
    }
    return;
  }

  if (isOperandInDirSpace(left) ||
      isOperandInFarSpace(left) ||
      isOperandInCodeSpace(left)) {
    emitcode ("mov", "%s,#%s", AOP_NAME(IC_RESULT(ic))[0],
	      OP_SYMBOL(left));
    if (AOP_SIZE(IC_RESULT(ic)) > 2) {
      // this must be a generic pointer
      int space=0; // dir space
      if (isOperandInFarSpace(left)) {
	space=1;
      } else if (isOperandInCodeSpace(left)) {
	space=2;
      }
      emitcode ("mov", "%s,#0x%02x", AOP_NAME(IC_RESULT(ic))[1], space);
    }
    return;
  }

  bailOut("genAddrOf");
}

/*-----------------------------------------------------------------*/
/* genAssign - generate code for assignment                        */
/*-----------------------------------------------------------------*/
static void genAssign (iCode * ic) {
  operand *result=IC_RESULT(ic), *right=IC_RIGHT(ic);

  printIc ("genAssign", ic, 1,0,1);
  
  if (!IS_SYMOP(result)) {
    fprintf (stderr, "genAssign: result is not a symbol\n");
    exit (1);
  }
  
  aopOp(right, FALSE);
  aopOp(result, TRUE);

  if (result->aop->type==AOP_REG || 
      right->aop->type==AOP_REG ||
      right->aop->type==AOP_LIT ||
      right->aop->type==AOP_STK ||
      right->aop->type==AOP_IMMD) {
    // everything will do
  } else {
    // they have to match
    if (result->aop->type != right->aop->type) {
      fprintf (stderr, "genAssign: types don't match (%s!=%s)\n",
	       aopTypeName(result->aop), aopTypeName(right->aop));
      exit (1);
    }
  }

  /* if result is a bit */
  if (AOP_TYPE(result) == AOP_CRY) {
    /* if right is literal, we know what the value is */
    if (AOP_TYPE(right) == AOP_LIT) {
      if (operandLitValue(right)) {
	emitcode ("setb", AOP_NAME(result)[0]);
      } else {
	emitcode ("clr", AOP_NAME(result)[0]);
      }
      return;
    }
    /* if right is also a bit */
    if (AOP_TYPE(right) == AOP_CRY) {
      emitcode ("mov", "c,%s", AOP_NAME(right));
      emitcode ("mov", "%s,c", AOP_NAME(result));
      return;
    }
    /* we need to or */
    emitcode ("mov", "%s,%s", AOP_NAME(result), toBoolean(right));
    return;
  }

  /* general case */
  emitcode ("mov", "%s,%s", 
	    result->aop->name[0], right->aop->name[0]);
  if (AOP_SIZE(result) > 2) {
    emitcode ("mov", "%s,%s",
	      result->aop->name[1], right->aop->name[1]);
  }
  
}

/*-----------------------------------------------------------------*/
/* genJumpTab - genrates code for jump table                       */
/*-----------------------------------------------------------------*/
static void genJumpTab (iCode * ic) {
  printIc ("genJumpTab", ic, 0,0,0);
}

/*-----------------------------------------------------------------*/
/* genCast - gen code for casting                                  */
/*-----------------------------------------------------------------*/
static void genCast (iCode * ic) {
  printIc ("genCast", ic, 1,1,1);
}

/*-----------------------------------------------------------------*/
/* genDjnz - generate decrement & jump if not zero instrucion      */
/*-----------------------------------------------------------------*/
static bool genDjnz (iCode * ic, iCode * ifx) {
  printIc ("genDjnz", ic, 0,0,0);
  return FALSE;
}

/*-----------------------------------------------------------------*/
/* genReceive - generate code for a receive iCode                  */
/*-----------------------------------------------------------------*/
static void genReceive (iCode * ic) {
  printIc ("genReceive", ic, 1,0,0);
}

/*-----------------------------------------------------------------*/
/* gen51Code - generate code for 8051 based controllers            */
/*-----------------------------------------------------------------*/
void genXA51Code (iCode * lic) {
  iCode *ic;
  int cln = 0;
  
  fprintf (stderr, "genXA51Code\n");
  lineHead = lineCurr = NULL;
  
  /* print the allocation information */
  if (allocInfo)
    printAllocInfo (currFunc, codeOutFile);

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
  
  for (ic = lic; ic; ic = ic->next) {
    if (cln != ic->lineno) {
      if (options.debug) {
	_G.debugLine = 1;
	emitcode ("", "C$%s$%d$%d$%d ==.",
		  FileBaseName (ic->filename), ic->lineno,
		  ic->level, ic->block);
	_G.debugLine = 0;
      }
      emitcode (";", "%s %d", ic->filename, ic->lineno);
      cln = ic->lineno;
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
	genPointerGet (ic, hasInc(IC_LEFT(ic), ic, getSize(operandType(IC_LEFT(ic)))));
	break;
	
      case '=':
	if (POINTER_SET (ic))
	  genPointerSet (ic, hasInc(IC_RESULT(ic), ic, getSize(operandType(IC_RIGHT(ic)))));
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
