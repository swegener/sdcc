/*-------------------------------------------------------------------------
  SDCCopt.c - calls all the optimizations routines and does some of the
              hackier transformations, these include translating iCodes
              to function calls and replacing local variables with their
              register equivalents etc. Also contains the driver routine
              for dead code elimination

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

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#include <math.h>
#include "common.h"

/*-----------------------------------------------------------------*/
/* global variables */
int cseDefNum = 0;

char flowChanged = 0;


/*-----------------------------------------------------------------*/
/* printSymName - prints the symbol names                          */
/*-----------------------------------------------------------------*/
int
printSymName (void *vsym)
{
  symbol *sym = vsym;
  fprintf (stdout, " %s ", sym->name);
  return 0;
}

/*-----------------------------------------------------------------*/
/* cnvToFcall - does the actual conversion to function call        */
/*-----------------------------------------------------------------*/
static void
cnvToFcall (iCode * ic, eBBlock * ebp)
{
  iCode *ip;
  iCode *newic;
  operand *left;
  operand *right;
  symbol *func = NULL;
  char *filename = ic->filename;
  int lineno = ic->lineno;
  int bytesPushed=0;

  ip = ic->next;                /* insertion point */
  /* remove it from the iCode */
  remiCodeFromeBBlock (ebp, ic);

  left = IC_LEFT (ic);
  right = IC_RIGHT (ic);

  if (IS_SYMOP (left))
      bitVectUnSetBit (OP_USES (left), ic->key);
  if (IS_SYMOP (right))
      bitVectUnSetBit (OP_USES (right), ic->key);

  if (IS_FLOAT (operandType (right)))
    {
      switch (ic->op)
        {
        case '+':
          func = fsadd;
          break;
        case '-':
          func = fssub;
          break;
        case '/':
          func = fsdiv;
          break;
        case '*':
          func = fsmul;
          break;
        case EQ_OP:
          func = fseq;
          break;
        case NE_OP:
          func = fsneq;
          break;
        case '<':
          func = fslt;
          break;
        case '>':
          func = fsgt;
          break;
        case LE_OP:
          func = fslteq;
          break;
        case GE_OP:
          func = fsgteq;
          break;
        }
    }
  else if (IS_FIXED16X16 (operandType (right)))
    {
      switch (ic->op)
        {
        case '+':
          func = fps16x16_add;
          break;
        case '-':
          func = fps16x16_sub;
          break;
        case '/':
          func = fps16x16_div;
          break;
        case '*':
          func = fps16x16_mul;
          break;
        case EQ_OP:
          func = fps16x16_eq;
          break;
        case NE_OP:
          func = fps16x16_neq;
          break;
        case '<':
          func = fps16x16_lt;
          break;
        case '>':
          func = fps16x16_gt;
          break;
        case LE_OP:
          func = fps16x16_lteq;
          break;
        case GE_OP:
          func = fps16x16_gteq;
          break;
        }
    }

  /* if float support routines NOT compiled as reentrant */
  if (!options.float_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, left, NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, left);
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }

      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
      if (IS_SYMOP (left))
          OP_USES (left) = bitVectSetBit (OP_USES (left), newic->key);

      /* second one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->next->etype))
        {
          newic = newiCode (SEND, right, NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->next->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, right);
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type)->next);
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
      if (IS_SYMOP (right))
          OP_USES (right) = bitVectSetBit (OP_USES (right), newic->key);
    }
  else
    {
      /* push right */
      if (IS_REGPARM (FUNC_ARGS(func->type)->next->etype))
        {
          newic = newiCode (SEND, right, NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->next->etype);
        }
      else
        {
          newic = newiCode (IPUSH, right, NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(right));
        }

      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
      if (IS_SYMOP (right))
          OP_USES (right) = bitVectSetBit (OP_USES (right), newic->key);

      /* insert push left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, left, NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, left, NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(left));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
      if (IS_SYMOP (left))
        OP_USES (left) = bitVectSetBit (OP_USES (left), newic->key);
    }
  /* insert the call */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  bitVectUnSetBit (OP_DEFS (IC_RESULT (ic)), ic->key);
  OP_USES (IC_RESULT (newic)) = bitVectSetBit (OP_USES (IC_RESULT (newic)), newic->key);
  newic->filename = filename;
  newic->lineno = lineno;
  newic->parmBytes += bytesPushed;
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to mark as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
}

/*-----------------------------------------------------------------*/
/* cnvToFloatCast - converts casts to floats to function calls     */
/*-----------------------------------------------------------------*/
static void
cnvToFloatCast (iCode * ic, eBBlock * ebp)
{
  iCode *ip, *newic;
  symbol *func = NULL;
  sym_link *type = operandType (IC_RIGHT (ic));
  int linenno = ic->lineno;
  int bwd, su;
  int bytesPushed=0;

  ip = ic->next;
  /* remove it from the iCode */
  remiCodeFromeBBlock (ebp, ic);
  /* depending on the type */
  for (bwd = 0; bwd < 3; bwd++)
    {
      for (su = 0; su < 2; su++)
        {
          if (compareType (type, multypes[bwd][su]) == 1)
            {
              func = conv[0][bwd][su];
              goto found;
            }
        }
    }

  if (compareType (type, fixed16x16Type) == 1)
    {
      func = fp16x16conv[0][3][0];
      goto found;
    }

  assert (0);
found:

  /* if float support routines NOT compiled as reentrant */
  if (!options.float_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_RIGHT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = linenno;
    }
  else
    {
      /* push the left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_RIGHT (ic), NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(IC_RIGHT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = linenno;
    }

  /* make the call */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  newic->parmBytes+=bytesPushed;
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to marked as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
  newic->filename = filename;
  newic->lineno = linenno;
}

/*----------------------------------------------------------------------*/
/* cnvToFixed16x16Cast - converts casts to fixed16x16 to function calls */
/*----------------------------------------------------------------------*/
static void
cnvToFixed16x16Cast (iCode * ic, eBBlock * ebp)
{
  iCode *ip, *newic;
  symbol *func = NULL;
  sym_link *type = operandType (IC_RIGHT (ic));
  int linenno = ic->lineno;
  int bwd, su;
  int bytesPushed=0;

  ip = ic->next;
  /* remove it from the iCode */
  remiCodeFromeBBlock (ebp, ic);
  /* depending on the type */
  for (bwd = 0; bwd < 3; bwd++)
    {
      for (su = 0; su < 2; su++)
        {
          if (compareType (type, multypes[bwd][su]) == 1)
            {
              func = fp16x16conv[0][bwd][su];
              goto found;
            }
        }
    }
  assert (0);
found:

  /* if float support routines NOT compiled as reentrant */
  if (!options.float_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_RIGHT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = linenno;
    }
  else
    {
      /* push the left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_RIGHT (ic), NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(IC_RIGHT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = linenno;
    }

  /* make the call */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  newic->parmBytes+=bytesPushed;
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to marked as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
  newic->filename = filename;
  newic->lineno = linenno;
}

/*-----------------------------------------------------------------*/
/* cnvFromFloatCast - converts casts From floats to function calls */
/*-----------------------------------------------------------------*/
static void
cnvFromFloatCast (iCode * ic, eBBlock * ebp)
{
  iCode *ip, *newic;
  symbol *func = NULL;
  sym_link *type = operandType (IC_LEFT (ic));
  char *filename = ic->filename;
  int lineno = ic->lineno;
  int bwd, su;
  int bytesPushed=0;

  ip = ic->next;
  /* remove it from the iCode */
  remiCodeFromeBBlock (ebp, ic);

  /* depending on the type */
  for (bwd = 0; bwd < 3; bwd++)
    {
      for (su = 0; su < 2; su++)
        {
          if (compareType (type, multypes[bwd][su]) == 1)
            {
              func = conv[1][bwd][su];
              goto found;
            }
        }
    }
  assert (0);
found:

  /* if float support routines NOT compiled as reentrant */
  if (!options.float_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_RIGHT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }
  else
    {
      /* push the left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_RIGHT (ic), NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(IC_RIGHT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }

  /* make the call */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  newic->parmBytes+=bytesPushed;
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to marked as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
  newic->filename = filename;
  newic->lineno = lineno;
}

/*--------------------------------------------------------------------------*/
/* cnvFromFixed16x16Cast - converts casts from fixed16x16 to function calls */
/*--------------------------------------------------------------------------*/
static void
cnvFromFixed16x16Cast (iCode * ic, eBBlock * ebp)
{
  iCode *ip, *newic;
  symbol *func = NULL;
  sym_link *type = operandType (IC_LEFT (ic));
  char *filename = ic->filename;
  int lineno = ic->lineno;
  int bwd, su;
  int bytesPushed=0;

  ip = ic->next;
  /* remove it from the iCode */
  remiCodeFromeBBlock (ebp, ic);

  /* depending on the type */
  for (bwd = 0; bwd < 3; bwd++)
    {
      for (su = 0; su < 2; su++)
        {
          if (compareType (type, multypes[bwd][su]) == 1)
            {
              func = fp16x16conv[1][bwd][su];
              goto found;
            }
        }
    }

  if (compareType (type, floatType) == 1)
    {
      func = fp16x16conv[1][3][0];
      goto found;
    }

  assert (0);
found:

  /* if float support routines NOT compiled as reentrant */
  if (!options.float_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_RIGHT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }
  else
    {
      /* push the left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_RIGHT (ic), NULL);
          newic->parmPush = 1;
          bytesPushed += getSize(operandType(IC_RIGHT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }

  /* make the call */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  newic->parmBytes+=bytesPushed;
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to marked as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
  newic->filename = filename;
  newic->lineno = lineno;
}

extern operand *geniCodeRValue (operand *, bool);

/*-----------------------------------------------------------------*/
/* convilong - converts int or long mults or divs to fcalls        */
/*-----------------------------------------------------------------*/
static void
convilong (iCode * ic, eBBlock * ebp)
{
  int op = ic->op;
  symbol *func = NULL;
  iCode *ip = ic->next;
  iCode *newic;
  char *filename = ic->filename;
  int lineno = ic->lineno;
  int bwd;
  int su;
  int bytesPushed=0;
  sym_link *leftType = operandType (IC_LEFT (ic));
  sym_link *rightType = operandType (IC_RIGHT (ic));

  remiCodeFromeBBlock (ebp, ic);

  if (getSize (leftType) == 1 && getSize (rightType) == 1)
    {
      int muldivmod;

      if (op == '*')
        muldivmod = 0;
      else if (op == '/')
        muldivmod = 1;
      else if (op == '%')
        muldivmod = 2;
      else
        muldivmod = -1;

      for (su = 0; su < 4 && muldivmod >= 0; su++)
        {
          if ((compareType (leftType, multypes[0][su%2]) == 1) &&
              (compareType (rightType, multypes[0][su/2]) == 1))
            {
              func = muldiv[muldivmod][0][su];
              goto found;
            }
        }
    }

  /* depending on the type */
  for (bwd = 0; bwd < 3; bwd++)
    {
      for (su = 0; su < 2; su++)
        {
          if (compareType (leftType, multypes[bwd][su]) == 1)
            {
              if ((op=='*' || op=='/' || op=='%'))
                {
                  int ret = compareType (rightType, multypes[bwd][su]);
                  if (ret != 1)
                    {
                      assert(0);
                    }
                }
              if (op == '*')
                func = muldiv[0][bwd][su];
              else if (op == '/')
                func = muldiv[1][bwd][su];
              else if (op == '%')
                func = muldiv[2][bwd][su];
              else if (op == RRC)
                func = rlrr[1][bwd][su];
              else if (op == RLC)
                func = rlrr[0][bwd][su];
              else if (op == RIGHT_OP)
                func = rlrr[1][bwd][su];
              else if (op == LEFT_OP)
                func = rlrr[0][bwd][su];
              else
                assert (0);
              goto found;
            }
        }
    }
  assert (0);
found:
  /* if int & long support routines NOT compiled as reentrant */
  if (!options.intlong_rent)
    {
      /* first one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_LEFT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_LEFT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;

      /* second one */
      if (IS_REGPARM (FUNC_ARGS(func->type)->next->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->next->etype);
        }
      else
        {
          newic = newiCode ('=', NULL, IC_RIGHT (ic));
          IC_RESULT (newic) = operandFromValue (FUNC_ARGS(func->type)->next);
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }
  else
    {
      /* compiled as reentrant then push */
      /* push right */
      if (IS_REGPARM (FUNC_ARGS(func->type)->next->etype))
        {
          newic = newiCode (SEND, IC_RIGHT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->next->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_RIGHT (ic), NULL);
          newic->parmPush = 1;

          bytesPushed += getSize(operandType(IC_RIGHT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;

      /* insert push left */
      if (IS_REGPARM (FUNC_ARGS(func->type)->etype))
        {
          newic = newiCode (SEND, IC_LEFT (ic), NULL);
          newic->argreg = SPEC_ARGREG(FUNC_ARGS(func->type)->etype);
        }
      else
        {
          newic = newiCode (IPUSH, IC_LEFT (ic), NULL);
          newic->parmPush = 1;

          bytesPushed += getSize(operandType(IC_LEFT(ic)));
        }
      addiCodeToeBBlock (ebp, newic, ip);
      newic->filename = filename;
      newic->lineno = lineno;
    }

  /* for the result */
  newic = newiCode (CALL, operandFromSymbol (func), NULL);
  IC_RESULT (newic) = IC_RESULT (ic);
  newic->filename = filename;
  newic->lineno = lineno;
  newic->parmBytes+=bytesPushed; // to clear the stack after the call
  ebp->hasFcall = 1;
  if (currFunc)
    FUNC_HASFCALL (currFunc->type) = 1;

  if (TARGET_PIC_LIKE)
    {
      /* normally these functions aren't marked external, so we can use their
       * _extern field to marked as already added to symbol table */

      if (!SPEC_EXTR(func->etype))
        {
          memmap *seg = SPEC_OCLS(OP_SYMBOL(IC_LEFT(newic))->etype);

          SPEC_EXTR(func->etype) = 1;
          seg = SPEC_OCLS( func->etype );
          addSet(&seg->syms, func);
        }
    }

  addiCodeToeBBlock (ebp, newic, ip);
}

/*-----------------------------------------------------------------*/
/* convertbuiltin - maybe convert some builtins back               */
/*-----------------------------------------------------------------*/
static void
convbuiltin (iCode *const ic, eBBlock *ebp)
{
  sym_link *ftype;
  symbol *bif;
  int stack;

  iCode *icc = ic, *icp = ic->prev, *ico;
  iCode *lastparam = ic;
  while (icc->op != CALL)
    {
      if (icc->op != SEND || !icc->builtinSEND)
        return;
      lastparam = icc;
      icc = icc->next;
    }

  if (!IS_SYMOP (IC_LEFT(icc)))
    return;

  ftype = operandType (IC_LEFT(icc));
  if (!IFFUNC_ISBUILTIN (ftype))
    return;

  bif = OP_SYMBOL (IC_LEFT (icc));

  /* Now we can be sure to have found a builtin function. */

  if (TARGET_IS_Z80 && !strcmp (bif->name, "__builtin_memcpy"))
    {
      /* Replace iff return value is used or last parameter is not an integer constant. */
      if (bitVectIsZero (OP_USES (IC_RESULT (icc))) && IS_OP_LITERAL (IC_LEFT (lastparam)))
        return;
      
      strcpy(OP_SYMBOL (IC_LEFT (icc))->rname, "_memcpy");
    }
  else
    return;

  /* Convert parameter passings from SEND to PUSH. */
  stack = 0;
  for (icc = ic; icc->op != CALL; icc = icc->next)
    {
      icc->builtinSEND = 0;
      icc->op = IPUSH;
      icc->parmPush = 1;
      stack += getSize (operandType (IC_LEFT (icc)));
    }
  icc->parmBytes = stack;

  /* Reverse parameters. */
  for (icc = ic; icc->op != CALL; icc = icc->next)
    {
      if(icc->next->op != CALL)
        icc->prev = icc->next;
      else
        icc->prev = icp;
    }
  if(icc != ic)
    {
      if(icp)
        icp->next = icc->prev;
      icc->prev = ic;
    }
  for(; icc != icp; ico = icc, icc = icc->prev)
    {
      if(icc->op != CALL)
        icc->next = ico;
    }
}

static void
convoptlink (iCode *ic, eBBlock *ebp)
{
  iCode *icc, *icp, *ico;

  assert (ic->op == CALL || ic->op == PCALL);

  for(icc = ic->prev; icc && icc->op == IPUSH; icc = icc->prev);
  icp = icc;
  ic = icp->next;

  /* Reverse parameters. */
  for (icc = ic; icc->op != CALL && icc->op != PCALL; icc = icc->next)
    {
      if(icc->next->op != CALL && icc->next->op != PCALL)
        icc->prev = icc->next;
      else
        icc->prev = icp;
    }
  if(icc != ic)
    {
      if(icp)
        icp->next = icc->prev;
      icc->prev = ic;
    }
  for(; icc != icp; ico = icc, icc = icc->prev)
    {
      if(icc->op != CALL && icc->op != PCALL)
        icc->next = ico;
    }
}

/*-----------------------------------------------------------------*/
/* convertToFcall - converts some operations to fcalls             */
/*-----------------------------------------------------------------*/
static void
convertToFcall (eBBlock ** ebbs, int count)
{
  int i;

  /* for all blocks do */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      /* for all instructions in the block do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          /* floating point operations are
             converted to function calls */
          if ((IS_CONDITIONAL (ic) || IS_ARITHMETIC_OP (ic)) &&
              (IS_FLOAT (operandType (IC_RIGHT (ic))) || IS_FIXED( operandType (IC_RIGHT (ic)))))
            {
              cnvToFcall (ic, ebbs[i]);
            }

          /* casting is a little different */
          if (ic->op == CAST)
            {
              if (IS_FLOAT (operandType (IC_RIGHT (ic))))
                cnvFromFloatCast (ic, ebbs[i]);
              else if (IS_FLOAT (operandType (IC_LEFT (ic))))
                cnvToFloatCast (ic, ebbs[i]);
              if (IS_FIXED16X16 (operandType (IC_RIGHT (ic))))
                cnvFromFixed16x16Cast (ic, ebbs[i]);
              else if (IS_FIXED16X16 (operandType (IC_LEFT (ic))))
                cnvToFixed16x16Cast (ic, ebbs[i]);
            }

          // Easy special case which avoids function call: modulo by a literal power
          // of two can be replaced by a bitwise AND.
          if (ic->op == '%' && isOperandLiteral(IC_RIGHT(ic)) &&
              IS_UNSIGNED(operandType(IC_LEFT(ic))))
            {
              unsigned litVal = abs((unsigned) double2ul (operandLitValue(IC_RIGHT(ic))));

              /* modulo by 1: no remainder */
              if (litVal == 1)
                {
                  ic->op = '=';
                  IC_RIGHT (ic) = operandFromLit(0);
                  IC_LEFT (ic) = NULL;
                  continue;
                }
              // See if literal value is a power of 2.
              while (litVal && !(litVal & 1))
                {
                  litVal >>= 1;
                }
              if (litVal)
                {
                  // discard lowest set bit.
                  litVal >>= 1;
                }

              if (!litVal)
                {
                  ic->op = BITWISEAND;
                  IC_RIGHT(ic) = operandFromLit(operandLitValue(IC_RIGHT(ic)) - 1);
                  continue;
                }
            }

          /* if long / int mult or divide or mod */
          if (ic->op == '*' || ic->op == '/' || ic->op == '%')
            {
              sym_link *leftType = operandType (IC_LEFT (ic));

              if (IS_INTEGRAL (leftType) && getSize (leftType) > port->support.muldiv)
                {
                  sym_link *rightType = operandType (IC_RIGHT (ic));

                  if (port->hasNativeMulFor != NULL &&
                      port->hasNativeMulFor (ic, leftType, rightType))
                    {
                      /* Leave as native */
                    }
                  else
                    {
                      convilong (ic, ebbs[i]);
                    }
                }
            }

          if (ic->op == RRC || ic->op == RLC || ic->op == LEFT_OP || ic->op == RIGHT_OP)
            {
              sym_link *type = operandType (IC_LEFT (ic));

              if (IS_INTEGRAL (type) && getSize (type) > port->support.shift && port->support.shift >= 0)
                {
                  convilong (ic, ebbs[i]);
                }
            }
          if (ic->op == SEND && ic->builtinSEND)
            {
              convbuiltin (ic, ebbs[i]);
            }
          if ((ic->op == CALL || ic->op == PCALL) && IFFUNC_ISOPTLINK (operandType (IC_LEFT (ic))))
            {
              convoptlink (ic, ebbs[i]);
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* isLocalWithoutDef - return 1 if sym might be used without a     */
/*                     defining iCode                              */
/*-----------------------------------------------------------------*/
static int
isLocalWithoutDef (symbol * sym)
{
  if (!IS_AUTO (sym))
    return 0;

  if (IS_VOLATILE (sym->type))
    return 0;

  if (sym->_isparm)
    return 0;

  if (IS_AGGREGATE (sym->type))
    return 0;

  if (sym->addrtaken)
    return 0;

  return !sym->defs;
}

/*-----------------------------------------------------------------*/
/* replaceRegEqv - replace all local variables with their reqv     */
/*-----------------------------------------------------------------*/
static void
replaceRegEqv (ebbIndex * ebbi)
{
  eBBlock ** ebbs = ebbi->bbOrder;
  int count = ebbi->count;
  int i;

  /* Update the symbols' def bitvector so we know if there is   */
  /* a defining iCode or not. Only replace a local variable     */
  /* with its register equivalent if there is a defining iCode; */
  /* otherwise, the port's register allocater may choke.        */
  cseAllBlocks (ebbi, TRUE);

  for (i = 0; i < count; i++)
    {
      iCode *ic;

      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          if (SKIP_IC2 (ic))
            continue;

          if (ic->op == IFX)
            {
              if (IC_COND (ic) &&
                  IS_TRUE_SYMOP (IC_COND (ic)) &&
                  isLocalWithoutDef (OP_SYMBOL (IC_COND (ic))))
                {
                  werrorfl (ic->filename, ic->lineno,
                          W_LOCAL_NOINIT,
                          OP_SYMBOL (IC_COND (ic))->name);
                  OP_REQV (IC_COND (ic)) = NULL;
                  OP_SYMBOL (IC_COND (ic))->allocreq = 1;
                }

              if (IS_TRUE_SYMOP (IC_COND (ic)) && OP_REQV (IC_COND (ic)))
                IC_COND (ic) = opFromOpWithDU (OP_REQV (IC_COND (ic)),
                                               OP_DEFS (IC_COND (ic)),
                                               OP_USES (IC_COND (ic)));
              continue;
            }

          if (ic->op == JUMPTABLE)
            {
              if (IC_JTCOND (ic) &&
                  IS_TRUE_SYMOP (IC_JTCOND (ic)) &&
                  isLocalWithoutDef (OP_SYMBOL (IC_JTCOND (ic))))
                {
                  werrorfl (ic->filename, ic->lineno,
                          W_LOCAL_NOINIT,
                          OP_SYMBOL (IC_JTCOND (ic))->name);
                  OP_REQV (IC_JTCOND (ic)) = NULL;
                  OP_SYMBOL (IC_JTCOND (ic))->allocreq = 1;
                }

              if (IS_TRUE_SYMOP (IC_JTCOND (ic)) && OP_REQV (IC_JTCOND (ic)))
                IC_JTCOND (ic) = opFromOpWithDU (OP_REQV (IC_JTCOND (ic)),
                                                 OP_DEFS (IC_JTCOND (ic)),
                                                 OP_USES (IC_JTCOND (ic)));
              continue;
            }

          if (ic->op == RECEIVE)
            {
              if (OP_SYMBOL (IC_RESULT (ic))->addrtaken)
                OP_SYMBOL (IC_RESULT (ic))->isspilt = 1;
            }

          /* general case */
          if (IC_RESULT (ic) &&
              IS_TRUE_SYMOP (IC_RESULT (ic)) &&
              OP_REQV (IC_RESULT (ic)))
            {
              if (POINTER_SET (ic))
                {
                  IC_RESULT (ic) = opFromOpWithDU (OP_REQV (IC_RESULT (ic)),
                                                   OP_DEFS (IC_RESULT (ic)),
                                                   OP_USES (IC_RESULT (ic)));
                  IC_RESULT (ic)->isaddr = 1;
                }
              else
                {
                  IC_RESULT (ic) = opFromOpWithDU (OP_REQV (IC_RESULT (ic)),
                                                   OP_DEFS (IC_RESULT (ic)),
                                                   OP_USES (IC_RESULT (ic)));
                }
            }

          if (IC_RIGHT (ic) &&
              IS_TRUE_SYMOP (IC_RIGHT (ic)) &&
              isLocalWithoutDef (OP_SYMBOL (IC_RIGHT (ic))))
            {
              werrorfl (ic->filename, ic->lineno,
                        W_LOCAL_NOINIT,
                        OP_SYMBOL (IC_RIGHT (ic))->name);
              OP_REQV (IC_RIGHT (ic)) = NULL;
              OP_SYMBOL (IC_RIGHT (ic))->allocreq = 1;
            }

          if (IC_RIGHT (ic) &&
              IS_TRUE_SYMOP (IC_RIGHT (ic)) &&
              OP_REQV (IC_RIGHT (ic)))
            {
              IC_RIGHT (ic) = opFromOpWithDU (OP_REQV (IC_RIGHT (ic)),
                                              OP_DEFS (IC_RIGHT (ic)),
                                              OP_USES (IC_RIGHT (ic)));
              IC_RIGHT (ic)->isaddr = 0;
            }

          if (IC_LEFT (ic) &&
              IS_TRUE_SYMOP (IC_LEFT (ic)) &&
              isLocalWithoutDef (OP_SYMBOL (IC_LEFT (ic))))
            {
              werrorfl (ic->filename, ic->lineno,
                        W_LOCAL_NOINIT,
                        OP_SYMBOL (IC_LEFT (ic))->name);
              OP_REQV (IC_LEFT (ic)) = NULL;
              OP_SYMBOL (IC_LEFT (ic))->allocreq = 1;
            }

          if (IC_LEFT (ic) &&
              IS_TRUE_SYMOP (IC_LEFT (ic)) &&
              OP_REQV (IC_LEFT (ic)))
            {
              IC_LEFT (ic) = opFromOpWithDU (OP_REQV (IC_LEFT (ic)),
                                             OP_DEFS (IC_LEFT (ic)),
                                             OP_USES (IC_LEFT (ic)));
              IC_LEFT (ic)->isaddr = 0;
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* findReqv - search for a register equivalent                     */
/*-----------------------------------------------------------------*/
operand *
findReqv (symbol * prereqv, eBBlock ** ebbs, int count)
{
  int i;
  iCode * ic;

  /* for all blocks do */
  for (i=0; i<count; i++)
    {
      /* for all instructions in the block do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          if (ic->op == IFX)
            {
              if (IS_ITEMP (IC_COND (ic))
                  && OP_SYMBOL (IC_COND (ic))->prereqv == prereqv)
                return IC_COND (ic);
            }
          else if (ic->op == JUMPTABLE)
            {
              if (IS_ITEMP (IC_JTCOND (ic))
                  && OP_SYMBOL (IC_JTCOND (ic))->prereqv == prereqv)
                return IC_JTCOND (ic);
            }
          else
            {
              if (IS_ITEMP (IC_LEFT (ic))
                  && OP_SYMBOL (IC_LEFT (ic))->prereqv == prereqv)
                return IC_LEFT (ic);
              if (IS_ITEMP (IC_RIGHT (ic))
                  && OP_SYMBOL (IC_RIGHT (ic))->prereqv == prereqv)
                return IC_RIGHT (ic);
              if (IS_ITEMP (IC_RESULT (ic))
                  && OP_SYMBOL (IC_RESULT (ic))->prereqv == prereqv)
                return IC_RESULT (ic);
            }
        }
    }

  return NULL;
}

/*-----------------------------------------------------------------*/
/* killDeadCode - eliminates dead assignments                      */
/*-----------------------------------------------------------------*/
int
killDeadCode (ebbIndex * ebbi)
{
  eBBlock ** ebbs = ebbi->dfOrder;
  int count = ebbi->count;
  int change = 1;
  int gchange = 0;
  int i = 0;

  /* basic algorithm :-                                          */
  /* first the exclusion rules :-                                */
  /*  1. if result is a global or volatile then skip             */
  /*  2. if assignment and result is a temp & isaddr  then skip  */
  /*     since this means array & pointer access, will be taken  */
  /*     care of by alias analysis.                              */
  /*  3. if the result is used in the remainder of the block skip */
  /*  4. if this definition does not reach the end of the block  */
  /*     i.e. the result is not present in the outExprs then KILL */
  /*  5. if it reaches the end of block & is used by some success */
  /*     or then skip                                            */
  /*     else            KILL                                    */
  /* this whole process is carried on iteratively till no change */
  do
    {
      change = 0;
      /* for all blocks do */
      for (i = 0; i < count; i++)
        {
          iCode *ic;

          /* for all instructions in the block do */
          for (ic = ebbs[i]->sch; ic; ic = ic->next)
            {
              int kill, j;
              kill = 0;

              if (SKIP_IC (ic) ||
                  ic->op == IFX ||
                  ic->op == RETURN ||
                  ic->op == DUMMY_READ_VOLATILE ||
                  ic->op == CRITICAL ||
                  ic->op == ENDCRITICAL)
                continue;

              /* Since both IFX & JUMPTABLE (in SKIP_IC) have been tested for */
              /* it is now safe to assume IC_LEFT, IC_RIGHT, & IC_RESULT are  */
              /* valid. */

              /* if the result is volatile then continue */
              if (IC_RESULT (ic) && isOperandVolatile (IC_RESULT (ic), FALSE))
                continue;

              /* if the result is a temp & isaddr then skip */
              if (IC_RESULT (ic) && POINTER_SET (ic))
                continue;

              if (POINTER_GET (ic) && IS_VOLATILE (operandType (IC_LEFT (ic))->next)
                  && !SPIL_LOC (IC_RESULT (ic)))
                continue;

              /* if the result is used in the remainder of the */
              /* block then skip */
              if (usedInRemaining (IC_RESULT (ic), ic->next))
                continue;

              /* does this definition reach the end of the block
                 or the usage is zero then we can kill */
              if (!bitVectBitValue (ebbs[i]->outDefs, ic->key))
                kill = 1;       /* if not we can kill it */
              else
                {
                  /* if this is a global variable or function parameter */
                  /* we cannot kill anyway             */
                  if (isOperandGlobal (IC_RESULT (ic)) ||
                      (OP_SYMBOL (IC_RESULT (ic))->_isparm &&
                       !OP_SYMBOL (IC_RESULT (ic))->ismyparm))
                    continue;

                  /* if we are sure there are no usages */
                  if (bitVectIsZero (OP_USES (IC_RESULT (ic))))
                    {
                      kill = 1;
                      goto kill;
                    }

                  /* reset visited flag */
                  for (j = 0; j < count; ebbs[j++]->visited = 0);

                  /* find out if this definition is alive */
                  if (applyToSet (ebbs[i]->succList, isDefAlive, ic))
                    continue;

                  kill = 1;
                }

            kill:
              /* kill this one if required */
              if (kill)
                {
                  bool volLeft = IS_SYMOP (IC_LEFT (ic))
                                 && isOperandVolatile (IC_LEFT (ic), FALSE);
                  bool volRight = IS_SYMOP (IC_RIGHT (ic))
                                  && isOperandVolatile (IC_RIGHT (ic), FALSE);

                  /* a dead address-of operation should die, even if volatile */
                  if (ic->op == ADDRESS_OF)
                    volLeft = FALSE;

                  if (ic->next && ic->seqPoint == ic->next->seqPoint
                      && (ic->next->op == '+' || ic->next->op == '-'))
                    {
                      if (isOperandEqual (IC_LEFT(ic), IC_LEFT(ic->next))
                          || isOperandEqual (IC_LEFT(ic), IC_RIGHT(ic->next)))
                        volLeft = FALSE;
                      if (isOperandEqual (IC_RIGHT(ic), IC_LEFT(ic->next))
                          || isOperandEqual (IC_RIGHT(ic), IC_RIGHT(ic->next)))
                        volRight = FALSE;
                    }

                  if (POINTER_GET (ic) && IS_VOLATILE (operandType (IC_LEFT (ic))->next))
                    {
                      if (SPIL_LOC (IC_RESULT (ic)))
                        {
                          IC_RESULT (ic) = newiTempFromOp (IC_RESULT (ic));
                          SPIL_LOC (IC_RESULT (ic)) = NULL;
                        }
                      continue;
                    }

                  change = 1;
                  gchange++;

                  /* now delete from defUseSet */
                  deleteItemIf (&ebbs[i]->outExprs, ifDiCodeIsX, ic);
                  bitVectUnSetBit (ebbs[i]->outDefs, ic->key);

                  /* and defset of the block */
                  bitVectUnSetBit (ebbs[i]->defSet, ic->key);

                  /* If this is the last of a register equivalent, */
                  /* look for a successor register equivalent. */
                  bitVectUnSetBit (OP_DEFS (IC_RESULT (ic)), ic->key);
                  if (IS_ITEMP (IC_RESULT (ic))
                      && OP_SYMBOL (IC_RESULT (ic))->isreqv
                      && bitVectIsZero (OP_DEFS (IC_RESULT (ic))))
                    {
                      symbol * resultsym = OP_SYMBOL (IC_RESULT (ic));
                      symbol * prereqv = resultsym->prereqv;

                      if (prereqv && prereqv->reqv && (OP_SYMBOL (prereqv->reqv) == resultsym))
                        {
                          operand * newreqv;

                          IC_RESULT (ic) = NULL;
                          newreqv = findReqv (prereqv, ebbs, count);
                          if (newreqv)
                            {
                              prereqv->reqv = newreqv;
                            }
                        }
                    }

                  /* delete the result */
                  IC_RESULT (ic) = NULL;

                  if (volLeft || volRight)
                    {
                      /* something is volatile, so keep the iCode */
                      /* and change the operator instead */
                      ic->op = DUMMY_READ_VOLATILE;

                      /* keep only the volatile operands */
                      if (!volLeft)
                        IC_LEFT (ic) = NULL;
                      if (!volRight)
                        IC_RIGHT (ic) = NULL;
                    }
                  else
                    {
                      /* nothing is volatile, eliminate the iCode */
                      remiCodeFromeBBlock (ebbs[i], ic);

                      /* for the left & right remove the usage */
                      if (IS_SYMOP (IC_LEFT (ic)))
                        bitVectUnSetBit (OP_USES (IC_LEFT (ic)), ic->key);

                      if (IS_SYMOP (IC_RIGHT (ic)))
                        bitVectUnSetBit (OP_USES (IC_RIGHT (ic)), ic->key);
                    }
                }
            }                   /* end of all instructions */

          if (!ebbs[i]->sch && !ebbs[i]->noPath)
            disconBBlock (ebbs[i], ebbi);
        }                       /* end of for all blocks */
    }                           /* end of do */
  while (change);

  return gchange;
}

/*-----------------------------------------------------------------*/
/* printCyclomatic - prints the cyclomatic information             */
/*-----------------------------------------------------------------*/
static void
printCyclomatic (eBBlock ** ebbs, int count)
{
  int nEdges = elementsInSet (graphEdges);
  int i, nNodes = 0;

  for (i = 0; i < count; i++)
    nNodes += (!ebbs[i]->noPath);

  /* print the information */
  werror (I_CYCLOMATIC, currFunc->name, nEdges, nNodes, nEdges - nNodes + 2);
}

/*-----------------------------------------------------------------*/
/* discardDeadParamReceives - remove any RECEIVE opcodes which     */
/* refer to dead variables.                                        */
/*-----------------------------------------------------------------*/
static void
discardDeadParamReceives (eBBlock ** ebbs, int count)
{
  int i;
  iCode *ic;
  iCode dummyIcode;

  for (i = 0; i < count; i++)
    {
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          if (ic->op == RECEIVE)
            {
              if (IC_RESULT (ic) && OP_SYMBOL (IC_RESULT (ic))
                  && !OP_SYMBOL (IC_RESULT (ic))->used)
                {
#if 0
                  fprintf (stderr, "discarding dead receive for %s\n",
                           OP_SYMBOL (IC_RESULT (ic))->name);
#endif
                  dummyIcode.next = ic->next;
                  remiCodeFromeBBlock (ebbs[i], ic);
                  ic = &dummyIcode;
                }
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* optimizeCastCast - remove unneeded intermediate casts.          */
/* Integer promotion may cast (un)signed char to int and then      */
/* recast the int to (un)signed long. If the signedness of the     */
/* char and long are the same, the cast can be safely performed in */
/* a single step.                                                  */
/*-----------------------------------------------------------------*/
static void
optimizeCastCast (eBBlock ** ebbs, int count)
{
  int i;
  iCode *ic;
  iCode *uic;
  sym_link * type1;
  sym_link * type2;
  sym_link * type3;
  symbol * sym;

  for (i = 0; i < count; i++)
    {
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
          if (ic->op == CAST && IC_RESULT (ic) && IS_ITEMP (IC_RESULT (ic)))
            {
              type1 = operandType (IC_RIGHT (ic));
              type2 = operandType (IC_RESULT (ic));

              /* Look only for a cast from an integer type to an */
              /* integer type that has no loss of bits */
              if (!IS_INTEGRAL (type1) || !IS_INTEGRAL (type2))
                continue;
              if (getSize (type2) < getSize (type1))
                continue;

              /* There must be only one use of this first result */
              if (bitVectnBitsOn (OP_USES (IC_RESULT (ic))) != 1)
                continue;

              /* This use must be a second cast */
              uic = hTabItemWithKey (iCodehTab,
                        bitVectFirstBit (OP_USES (IC_RESULT (ic))));
              if (!uic || uic->op != CAST)
                continue;

              /* It must be a cast to another integer type that */
              /* has no loss of bits */
              type3 = operandType (IC_RESULT (uic));
              if (!IS_INTEGRAL (type3))
                 continue;
              if (getSize (type3) < getSize (type2))
                 continue;

              /* The signedness between the first and last types */
              /* must match */
              if (SPEC_USIGN (type3) != SPEC_USIGN (type1))
                 continue;

              /* Change the first cast to a simple assignment and */
              /* let the second cast do all the work */
              ic->op = '=';
              IC_LEFT (ic) = NULL;
              sym = OP_SYMBOL (IC_RESULT (ic));
              sym->type = copyLinkChain (type1);
              sym->etype = getSpec (sym->type);
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* eBBlockFromiCode - creates extended basic blocks from iCode     */
/*                    will return an array of eBBlock pointers     */
/*-----------------------------------------------------------------*/
eBBlock **
eBBlockFromiCode (iCode * ic)
{
  ebbIndex *ebbi = NULL;
  int change = 1;
  int lchange = 0;
  int kchange = 0;
  hTab *loops;

  /* if nothing passed then return nothing */
  if (!ic)
    return NULL;

  eBBNum = 0;

  /* optimize the chain for labels & gotos
     this will eliminate redundant labels and
     will change jump to jumps by jumps */
  ic = iCodeLabelOptimize (ic);

  /* break it down into basic blocks */
  ebbi = iCodeBreakDown (ic);

  /* hash the iCode keys so that we can quickly index */
  /* them in the rest of the optimization steps */
  setToNull ((void *) &iCodehTab);
  iCodehTab = newHashTable (iCodeKey);
  hashiCodeKeys (ebbi->bbOrder, ebbi->count);

  /* compute the control flow */
  computeControlFlow (ebbi);

  /* dumpraw if asked for */
  if (options.dump_raw)
    dumpEbbsToFileExt (DUMP_RAW0, ebbi);

  /* replace the local variables with their
     register equivalents : the liveRange computation
     along with the register allocation will determine
     if it finally stays in the registers */
  replaceRegEqv (ebbi);

  /* create loop regions */
  loops = createLoopRegions (ebbi);

  /* dumpraw if asked for */
  if (options.dump_raw)
    dumpEbbsToFileExt (DUMP_RAW1, ebbi);

  optimizeCastCast (ebbi->bbOrder, ebbi->count);

  /* Burn the corpses, so the dead may rest in peace,
     safe from cse necromancy */
  computeDataFlow (ebbi);
  killDeadCode (ebbi);

  /* do common subexpression elimination for each block */
  change = cseAllBlocks (ebbi, FALSE);

  /* dumpraw if asked for */
  if (options.dump_raw)
    dumpEbbsToFileExt (DUMP_CSE, ebbi);

  /* compute the data flow */
  computeDataFlow (ebbi);

  /* dumpraw if asked for */
  if (options.dump_raw)
    dumpEbbsToFileExt (DUMP_DFLOW, ebbi);

  /* global common subexpression elimination  */
  if (optimize.global_cse)
    {
      change += cseAllBlocks (ebbi, FALSE);
      if (options.dump_gcse)
        dumpEbbsToFileExt (DUMP_GCSE, ebbi);
    }
  else
    {
      // compute the dataflow only
      assert(cseAllBlocks (ebbi, TRUE)==0);
    }
  /* kill dead code */
  kchange = killDeadCode (ebbi);

  if (options.dump_kill)
    dumpEbbsToFileExt (DUMP_DEADCODE, ebbi);

  /* do loop optimizations */
  change += (lchange = loopOptimizations (loops, ebbi));
  if (options.dump_loop)
    dumpEbbsToFileExt (DUMP_LOOP, ebbi);

  /* recompute the data flow and apply global cse again
     if loops optimizations or dead code caused a change:
     loops will brings out of the loop which then may be
     available for use in the later blocks: dead code
     elimination could potentially disconnect some blocks
     conditional flow may be efected so we need to apply
     subexpression once more */
  if (lchange || kchange)
    {
      computeDataFlow (ebbi);
      change += cseAllBlocks (ebbi, FALSE);
      if (options.dump_loop)
        dumpEbbsToFileExt (DUMP_LOOPG, ebbi);

      /* if loop optimizations caused a change then do
         dead code elimination once more : this will
         get rid of the extra assignments to the induction
         variables created during loop optimizations */
      killDeadCode (ebbi);

      if (options.dump_loop)
        dumpEbbsToFileExt (DUMP_LOOPD, ebbi);
    }

  /* sort it back by block number */
  //qsort (ebbs, saveCount, sizeof (eBBlock *), bbNumCompare);

  if (!options.lessPedantic)
    {
      // this is a good place to check missing return values
      if (currFunc)
        {
          // the user is on his own with naked functions...
          if (!IS_VOID(currFunc->etype) && !FUNC_ISNAKED(currFunc->type))
            {
              eBBlock *bp;
              // make sure all predecessors of the last block end in a return
              for (bp=setFirstItem(ebbi->bbOrder[ebbi->count-1]->predList);
                   bp;
                   bp=setNextItem(ebbi->bbOrder[ebbi->count-1]->predList))
                {
                  if (bp->ech->op != RETURN)
                    {
                      werrorfl (bp->ech->filename, bp->ech->lineno, W_VOID_FUNC, currFunc->name);
                    }
                }
            }
        }
    }

  /* if cyclomatic info requested then print it */
  if (options.cyclomatic)
    printCyclomatic (ebbi->bbOrder, ebbi->count);

  /* convert operations with support routines
     written in C to function calls : I am doing
     this at this point since I want all the
     operations to be as they are for optimizations */
  convertToFcall (ebbi->bbOrder, ebbi->count);

  /* compute the live ranges */
  computeLiveRanges (ebbi->bbOrder, ebbi->count, TRUE);

  if (options.dump_range)
    dumpEbbsToFileExt (DUMP_RANGE, ebbi);

  /* Now that we have the live ranges, discard parameter
   * receives for unused parameters.
   */
  discardDeadParamReceives (ebbi->bbOrder, ebbi->count);

  /* allocate registers & generate code */
  port->assignRegisters (ebbi);

  /* throw away blocks */
  setToNull ((void *) &graphEdges);

  return NULL;
}


/* (add-hook 'c-mode-hook (lambda () (setq c-basic-offset 4))) */
