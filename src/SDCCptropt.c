/*-------------------------------------------------------------------------

  SDCCptropt.c - source file for pointer arithmetic Optimizations

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

#include "common.h"

/*-----------------------------------------------------------------------*/
/* findPointerGetSet - find the pointer get or set for a operand         */
/*-----------------------------------------------------------------------*/
static iCode *
findPointerGetSet (iCode * sic, operand * op)
{
  iCode *ic = sic;

  for (; ic; ic = ic->next)
    {
      if ((POINTER_SET (ic) && isOperandEqual (op, IC_RESULT (ic))) ||
	  (POINTER_GET (ic) && isOperandEqual (op, IC_LEFT (ic))))
	return ic;

      /* if we find any other usage or definition of op null */
      if (IC_RESULT (ic) && isOperandEqual (IC_RESULT (ic), op))
	return NULL;

      if (IC_RIGHT (ic) && isOperandEqual (IC_RIGHT (ic), op))
	return NULL;

      if (IC_LEFT (ic) && isOperandEqual (IC_LEFT (ic), op))
	return NULL;

    }

  return NULL;
}

static int pattern1 (iCode *sic)
{
	/* this is what we do. look for sequences like

	iTempX := _SOME_POINTER_;
	iTempY := _SOME_POINTER_ + nn ;   nn  = sizeof (pointed to object) 	sic->next
	_SOME_POINTER_ := iTempY;						sic->next->next
	either       
     	iTempZ := @[iTempX];							sic->next->next->next
	or
     	*(iTempX) := ..something..						sic->next->next->next
	if we find this then transform this to
	iTempX := _SOME_POINTER_;
	either       
     	iTempZ := @[iTempX];
	or 
     	*(iTempX) := ..something..
	iTempY := _SOME_POINTER_ + nn ;   nn  = sizeof (pointed to object)
	_SOME_POINTER_ := iTempY; */
	
	/* sounds simple enough so lets start , here I use -ve
	   tests all the way to return if any test fails */
	iCode *pgs, *sh, *st;

	if (!(sic->next && sic->next->next && sic->next->next->next))
		return 0;
	if (sic->next->op != '+' && sic->next->op != '-')
		return 0;
	if (!(sic->next->next->op == '=' &&
	      !POINTER_SET (sic->next->next)))
		return 0;
	if (!isOperandEqual (IC_LEFT (sic->next), IC_RIGHT (sic)) ||
	    !IS_OP_LITERAL (IC_RIGHT (sic->next)))
		return 0;
	if (operandLitValue (IC_RIGHT (sic->next)) !=
	    getSize (operandType (IC_RIGHT (sic))->next))
		return 0;
	if (!isOperandEqual (IC_RESULT (sic->next->next),
			     IC_RIGHT (sic)))
		return 0;
	if (!isOperandEqual (IC_RESULT (sic->next), IC_RIGHT (sic->next->next)))
		return 0;
	if (!(pgs = findPointerGetSet (sic->next->next, IC_RESULT (sic))))
		return 0;

	/* found the patter .. now do the transformation */
	sh = sic->next;
	st = sic->next->next;

	/* take the two out of the chain */
	sic->next = st->next;
	st->next->prev = sic;

	/* and put them after the pointer get/set icode */
	if ((st->next = pgs->next))
		st->next->prev = st;
	pgs->next = sh;
	sh->prev = pgs;
	return 1;
}

static int pattern2 (iCode *sic)
{
	/* this is what we do. look for sequences like

	iTempX := _SOME_POINTER_;
	iTempY := _SOME_POINTER_ + nn ;   nn  = sizeof (pointed to object) 	sic->next
	iTempK := iTempY;							sic->next->next
	_SOME_POINTER_ := iTempK;						sic->next->next->next
	either       
     	iTempZ := @[iTempX];							sic->next->next->next->next
	or
     	*(iTempX) := ..something..						sic->next->next->next->next
	if we find this then transform this to
	iTempX := _SOME_POINTER_;
	either       
     	iTempZ := @[iTempX];
	or 
     	*(iTempX) := ..something..
	iTempY := _SOME_POINTER_ + nn ;   nn  = sizeof (pointed to object)
	iTempK := iTempY;	
	_SOME_POINTER_ := iTempK; */
	
	/* sounds simple enough so lets start , here I use -ve
	   tests all the way to return if any test fails */
	iCode *pgs, *sh, *st;

	if (!(sic->next && sic->next->next && sic->next->next->next && 
	      sic->next->next->next->next))
		return 0;

	/* yes I can OR them together and make one large if... but I have
	   simple mind and like to keep things simple & readable */
	if (!(sic->next->op == '+' || sic->next->op == '-')) return 0;
	if (!isOperandEqual(IC_RIGHT(sic),IC_LEFT(sic->next))) return 0;
	if (!IS_OP_LITERAL (IC_RIGHT (sic->next))) return 0;
	if (operandLitValue (IC_RIGHT (sic->next)) !=
	    getSize (operandType (IC_RIGHT (sic))->next)) return 0;
	if (!IS_ASSIGN_ICODE(sic->next->next)) return 0;
	if (!isOperandEqual(IC_RIGHT(sic->next->next),IC_RESULT(sic->next))) return 0;
	if (!IS_ASSIGN_ICODE(sic->next->next->next)) return 0;
	if (!isOperandEqual(IC_RIGHT(sic->next->next->next),IC_RESULT(sic->next->next))) return 0;
	if (!isOperandEqual(IC_RESULT(sic->next->next->next),IC_LEFT(sic->next))) return 0;
	if (!(pgs = findPointerGetSet (sic->next->next->next, IC_RESULT (sic)))) return 0;
	
	/* found the patter .. now do the transformation */
	sh = sic->next;
	st = sic->next->next->next;

	/* take the three out of the chain */
	sic->next = st->next;
	st->next->prev = sic;

	/* and put them after the pointer get/set icode */
	if ((st->next = pgs->next))
		st->next->prev = st;
	pgs->next = sh;
	sh->prev = pgs;
	return 1;
}

/*-----------------------------------------------------------------------*/
/* ptrPostIncDecOpts - will do some pointer post increment optimizations */
/*                     this will help register allocation amongst others */
/*-----------------------------------------------------------------------*/
void 
ptrPostIncDecOpt (iCode * sic)
{
	if (pattern1(sic)) return ;
	pattern2(sic);
}

/*-----------------------------------------------------------------------*/
/* addPattern1 - transform addition to pointer of variables              */
/*-----------------------------------------------------------------------*/
static int addPattern1(iCode *ic)
{
	iCode *dic;
	operand *tmp;
	/* transform :
	   iTempAA = iTempBB + iTempCC
	   iTempDD = iTempAA + CONST
	   to
	   iTempAA = iTempBB + CONST
	   iTempDD = iTempAA + iTempCC
	*/
	if (!isOperandLiteral(IC_RIGHT(ic))) return 0;
	if ((dic=findBackwardDef(IC_LEFT(ic),ic->prev)) == NULL) return 0;
	if (bitVectnBitsOn(OP_SYMBOL(IC_RESULT(dic))->uses) > 1) return 0;
	if (dic->op != '+') return 0;
	tmp = IC_RIGHT(ic);
	IC_RIGHT(ic) = IC_RIGHT(dic);
	IC_RIGHT(dic) = tmp;
	return 1;
}

/*-----------------------------------------------------------------------*/
/* ptrAddition - optimize pointer additions                              */
/*-----------------------------------------------------------------------*/
int ptrAddition (iCode *sic)
{
	if (addPattern1(sic)) return 1;
	return 0;
}
