/*-------------------------------------------------------------------------

  SDCClrange.c - source file for live range computations

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
#include "limits.h"

int iCodeSeq = 0;
hTab *liveRanges = NULL;
hTab *iCodehTab = NULL;
hTab *iCodeSeqhTab = NULL;

/*-----------------------------------------------------------------*/
/* hashiCodeKeys - add all iCodes to the hash table                */
/*-----------------------------------------------------------------*/
void 
hashiCodeKeys (eBBlock ** ebbs, int count)
{
  int i;

  for (i = 0; i < count; i++)
    {
      iCode *ic;
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
	hTabAddItem (&iCodehTab, ic->key, ic);
    }
}

/*-----------------------------------------------------------------*/
/* sequenceiCode - creates a sequence number for the iCode & add   */
/*-----------------------------------------------------------------*/
void 
sequenceiCode (eBBlock ** ebbs, int count)
{
  int i;

  for (i = 0; i < count; i++)
    {

      iCode *ic;
      ebbs[i]->fSeq = iCodeSeq + 1;
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
	{
	  ic->seq = ++iCodeSeq;
	  ic->depth = ebbs[i]->depth;
	  //hTabAddItem (&iCodehTab, ic->key, ic);
	  hTabAddItem (&iCodeSeqhTab, ic->seq, ic);
	}
      ebbs[i]->lSeq = iCodeSeq;
    }
}

/*-----------------------------------------------------------------*/
/* setFromRange - sets the from range of a given operand           */
/*-----------------------------------------------------------------*/
void
setFromRange (operand * op, int from)
{
  /* only for compiler defined temporaries */
  if (!IS_ITEMP (op))
    return;

  hTabAddItemIfNotP (&liveRanges, op->key, OP_SYMBOL (op));

  if (op->isaddr)
    OP_SYMBOL (op)->isptr = 1;

  if (!OP_LIVEFROM (op) ||
      OP_LIVEFROM (op) > from)
    OP_LIVEFROM (op) = from;
}

/*-----------------------------------------------------------------*/
/* setToRange - set the range to for an operand                    */
/*-----------------------------------------------------------------*/
void 
setToRange (operand * op, int to, bool check)
{
  /* only for compiler defined temps */
  if (!IS_ITEMP (op))
    return;

  OP_SYMBOL (op)->key = op->key;
  hTabAddItemIfNotP (&liveRanges, op->key, OP_SYMBOL (op));

  if (op->isaddr)
    OP_SYMBOL (op)->isptr = 1;

  if (check)
    if (!OP_LIVETO (op))
      OP_LIVETO (op) = to;
    else;
  else
    OP_LIVETO (op) = to;
}

/*-----------------------------------------------------------------*/
/* setFromRange - sets the from range of a given operand           */
/*-----------------------------------------------------------------*/
void
setLiveFrom (symbol * sym, int from)
{
  if (!sym->liveFrom || sym->liveFrom > from)
    sym->liveFrom = from;
}

/*-----------------------------------------------------------------*/
/* setToRange - set the range to for an operand                    */
/*-----------------------------------------------------------------*/
void
setLiveTo (symbol * sym, int to)
{
  if (!sym->liveTo || sym->liveTo < to)
    sym->liveTo = to;
}

/*-----------------------------------------------------------------*/
/* markLiveRanges - for each operand mark the liveFrom & liveTo    */
/*-----------------------------------------------------------------*/
void
markLiveRanges (eBBlock ** ebbs, int count)
{
  int i, key;
  symbol *sym;
  
  for (i = 0; i < count; i++)
    {
      iCode *ic;
      
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
	  if (ic->op == CALL || ic->op == PCALL)
	      if (bitVectIsZero (OP_SYMBOL (IC_RESULT (ic))->uses))
		bitVectUnSetBit (ebbs[i]->defSet, ic->key);

	  /* for all iTemps alive at this iCode */
	  for (key = 1; key < ic->rlive->size; key++)
	    {
	      if (!bitVectBitValue(ic->rlive, key))
	        continue;

	      sym = hTabItemWithKey(liveRanges, key);
	      setLiveTo(sym, ic->seq);
	      setLiveFrom(sym, ic->seq);
	    }

	}
    }
}

/*-----------------------------------------------------------------*/
/* markAlive - marks the operand as alive between sic and eic      */
/*-----------------------------------------------------------------*/
void
markAlive (iCode * sic, iCode * eic, int key)
{
  iCode *dic;

  for (dic = sic; dic != eic->next; dic = dic->next)
    {
      dic->rlive = bitVectSetBit (dic->rlive, key);
    }
}

/*-----------------------------------------------------------------*/
/* findNextUseSym - finds the next use of the symbol and marks it  */
/*                  alive in between                               */
/*-----------------------------------------------------------------*/
int
findNextUseSym (eBBlock *ebp, iCode *ic, symbol * sym)
{
  int retval = 0;
  iCode *uic;
  eBBlock *succ;

  hTabAddItemIfNotP (&liveRanges, sym->key, sym);

  if (!ic)
    goto check_successors;

  /* if we check a complete block and the symbol */
  /* is alive at the beginning of the block */
  /* and not defined in the first instructions */
  /* then a next use exists (return 1) */
  if ((ic == ebp->sch) && bitVectBitValue(ic->rlive, sym->key))
    {
      /* check if the first instruction is a def of this op */
      if (ic->op == JUMPTABLE || ic->op == IFX || SKIP_IC2(ic))
        return 1;

      if (IS_ITEMP(IC_RESULT(ic)))
        if (IC_RESULT(ic)->key == sym->key)
	  return 0;

      return 1;
    }

  if (ebp->visited)
    return 0;

  ebp->visited = 1;

  /* for all remaining instructions in current block */
  for (uic = ic; uic; uic = uic->next)
    {

      if (SKIP_IC2(uic))
        continue;

      if (uic->op == JUMPTABLE)
        {
          if (IS_ITEMP(IC_JTCOND(uic)) && IC_JTCOND(uic)->key == sym->key)
            {
	      markAlive(ic, uic, sym->key);
	      return 1;
	    }
	   continue;
	 }

      if (uic->op == IFX)
        {
          if (IS_ITEMP(IC_COND(uic)) && IC_COND(uic)->key == sym->key)
            {
	      markAlive(ic, uic, sym->key);
	      return 1;
	    }
	   continue;
	 }

      if (IS_ITEMP (IC_LEFT (uic)))
        if (IC_LEFT (uic)->key == sym->key)
          {
	    markAlive(ic, uic, sym->key);
	    return 1;
	  }

      if (IS_ITEMP (IC_RIGHT (uic)))
        if (IC_RIGHT (uic)->key == sym->key)
	  {
	    markAlive (ic, uic, sym->key);
	    return 1;
	  }

      if (IS_ITEMP (IC_RESULT (uic)))
        if (IC_RESULT (uic)->key == sym->key)
	  {
	    if (POINTER_SET (uic))
	      {
	        markAlive (ic, uic, sym->key);
                return 1;
	      }
	    else
	      return 0;
	  }

    }

  /* check all successors */
check_successors:

  succ = setFirstItem (ebp->succList);
  for (; succ; succ = setNextItem (ebp->succList))
    {
      retval += findNextUseSym (succ, succ->sch, sym);
    }

  if (retval)
    {
      if (ic) markAlive (ic, ebp->ech, sym->key);
      return 1;
    }

  return 0;
}

/*-----------------------------------------------------------------*/
/* findNextUse - finds the next use of the operand and marks it    */
/*               alive in between                                  */
/*-----------------------------------------------------------------*/
int
findNextUse (eBBlock *ebp, iCode *ic, operand *op)
{
  if (op->isaddr)
    OP_SYMBOL (op)->isptr = 1;

  OP_SYMBOL (op)->key = op->key;

  return findNextUseSym (ebp, ic, OP_SYMBOL(op));
}

/*-----------------------------------------------------------------*/
/* unvisitBlocks - clears visited in all blocks                    */
/*-----------------------------------------------------------------*/
void unvisitBlocks (eBBlock ** ebbs, int count)
{
  int i;

  for (i = 0; i < count; i++)
    ebbs[i]->visited = 0;
}

/*-----------------------------------------------------------------*/
/* unvisitBlocks - clears visited in all blocks                    */
/*-----------------------------------------------------------------*/
void
incUsed (iCode *ic, operand *op)
{
  if (ic->depth)
    OP_SYMBOL (op)->used += (((unsigned int) 1 << ic->depth) + 1);
  else
    OP_SYMBOL (op)->used += 1;
}

/*-----------------------------------------------------------------*/
/* rliveClear - clears the rlive bitVectors                        */
/*-----------------------------------------------------------------*/
void
rliveClear (eBBlock ** ebbs, int count)
{
  int i;

  /* for all blocks do */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      /* for all instructions in this block do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {
	  freeBitVect (ic->rlive);
	  ic->rlive = NULL;
	}
    }
}

/*-----------------------------------------------------------------*/
/* rlivePoint - for each point compute the ranges that are alive   */
/*-----------------------------------------------------------------*/
void
rlivePoint (eBBlock ** ebbs, int count)
{
  int i, key;
  eBBlock *succ;
  bitVect *alive;

  /* for all blocks do */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      /* for all instructions in this block do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
        {

	  if (!ic->rlive)
	    ic->rlive = newBitVect (operandKey);

	  if (SKIP_IC2(ic))
	    continue;

	  if (ic->op == JUMPTABLE && IS_SYMOP(IC_JTCOND(ic)))
	    {
	      incUsed (ic, IC_JTCOND(ic));

	      if (!IS_ITEMP(IC_JTCOND(ic)))
	        continue;

	      unvisitBlocks(ebbs, count);
	      ic->rlive = bitVectSetBit (ic->rlive, IC_JTCOND(ic)->key);
	      findNextUse (ebbs[i], ic->next, IC_JTCOND(ic));

	      continue;
	    }

	  if (ic->op == IFX && IS_SYMOP(IC_COND(ic)))
	    {
	      incUsed (ic, IC_COND(ic));

	      if (!IS_ITEMP(IC_COND(ic)))
	        continue;

	      unvisitBlocks (ebbs, count);
	      ic->rlive = bitVectSetBit (ic->rlive, IC_COND(ic)->key);
	      findNextUse (ebbs[i], ic->next, IC_COND(ic));

	      continue;
	    }

	  if (IS_SYMOP(IC_LEFT(ic)))
	    {
	      incUsed (ic, IC_LEFT(ic));
	      if (IS_ITEMP(IC_LEFT(ic)))
	        {

	          unvisitBlocks(ebbs, count);
	          ic->rlive = bitVectSetBit (ic->rlive, IC_LEFT(ic)->key);
	          findNextUse (ebbs[i], ic->next, IC_LEFT(ic));

		  /* if this is a send extend the LR to the call */
		  if (ic->op == SEND)
		    {
		      iCode *lic;
		      for (lic = ic; lic; lic = lic->next)
		        {
			  if (lic->op == CALL)
			    {
			      markAlive (ic, lic->prev, IC_LEFT (ic)->key);
			      break;
			    }
			}
		    }
		}
	    }

	  if (IS_SYMOP(IC_RIGHT(ic)))
	    {
	      incUsed (ic, IC_RIGHT(ic));
	      if (IS_ITEMP(IC_RIGHT(ic)))
	        {
	          unvisitBlocks(ebbs, count);
	          ic->rlive = bitVectSetBit (ic->rlive, IC_RIGHT(ic)->key);
	          findNextUse (ebbs[i], ic->next, IC_RIGHT(ic));
		}
	    }

	  if (POINTER_SET(ic) && IS_SYMOP(IC_RESULT(ic)))
	    incUsed (ic, IC_RESULT(ic));

	  if (IS_ITEMP(IC_RESULT(ic)))
	    {
	      unvisitBlocks(ebbs, count);
	      ic->rlive = bitVectSetBit (ic->rlive, IC_RESULT(ic)->key);
	      findNextUse (ebbs[i], ic->next, IC_RESULT(ic));
	    }

	  if (!POINTER_SET(ic) && IC_RESULT(ic))
	    ic->defKey = IC_RESULT(ic)->key;

	}

      /* check all symbols that are alive in the last instruction */
      /* but are not alive in all successors */

      succ = setFirstItem (ebbs[i]->succList);
      if (!succ)
        continue;

      alive = succ->sch->rlive;
      while ((succ = setNextItem (ebbs[i]->succList)))
        {
	  if (succ->sch)
            alive = bitVectIntersect (alive, succ->sch->rlive);
	}

      if (ebbs[i]->ech)
        alive = bitVectCplAnd ( bitVectCopy (ebbs[i]->ech->rlive), alive);

      for (key = 1; key < alive->size; key++)
        {
	  if (!bitVectBitValue (alive, key))
	    continue;

	  unvisitBlocks(ebbs, count);
	  findNextUseSym (ebbs[i], NULL, hTabItemWithKey (liveRanges, key));
	}

    }
}

/*-----------------------------------------------------------------*/
/* computeClash - find out which live ranges collide with others   */
/*-----------------------------------------------------------------*/
static void
computeClash (eBBlock ** ebbs, int count)
{
  int i;

  /* for all blocks do */
  for (i = 0; i < count; i++)
    {
      iCode *ic;

      /* for every iCode do */
      for (ic = ebbs[i]->sch; ic; ic = ic->next)
	{
	  symbol *sym1, *sym2;
	  int key1, key2;

	  /* for all iTemps alive at this iCode */
	  for (key1 = 1; key1 < ic->rlive->size; key1++)
	    {
	      if (!bitVectBitValue(ic->rlive, key1))
	        continue;

	      sym1 = hTabItemWithKey(liveRanges, key1);

	      if (!sym1->isitmp)
	        continue;

	      /* for all other iTemps alive at this iCode */
	      for (key2 = key1+1; key2 < ic->rlive->size; key2++)
	        {
		  if (!bitVectBitValue(ic->rlive, key2))
		    continue;

		  sym2 = hTabItemWithKey(liveRanges, key2);
		  
		  if (!sym2->isitmp)
		    continue;

		  /* if the result and left or right is an iTemp */
		  /* than possibly the iTemps do not clash */
		  if ((ic->op != JUMPTABLE) && (ic->op != IFX) &&
		      IS_ITEMP(IC_RESULT(ic)) &&
		      (IS_ITEMP(IC_LEFT(ic)) || IS_ITEMP(IC_RIGHT(ic))))
		    {
		      if (OP_SYMBOL(IC_RESULT(ic))->key == key1)
		        {
		          if (IS_SYMOP(IC_LEFT(ic)))
			    if (OP_SYMBOL(IC_LEFT(ic))->key == key2)
			      continue;
		          if (IS_SYMOP(IC_RIGHT(ic)))
			    if (OP_SYMBOL(IC_RIGHT(ic))->key == key2)
			      continue;
			}

		      if (OP_SYMBOL(IC_RESULT(ic))->key == key2)
		        {
		          if (IS_SYMOP(IC_LEFT(ic)))
			    if (OP_SYMBOL(IC_LEFT(ic))->key == key1)
			      continue;
		          if (IS_SYMOP(IC_RIGHT(ic)))
			    if (OP_SYMBOL(IC_RIGHT(ic))->key == key1)
			      continue;
			}
		    }

		  /* the iTemps do clash. set the bits in clashes */
		  sym1->clashes = bitVectSetBit (sym1->clashes, key2);
		  sym2->clashes = bitVectSetBit (sym2->clashes, key1);

		  /* check if they share the same spill location */
		  /* what is this good for? */
	          if (SYM_SPIL_LOC(sym1) && SYM_SPIL_LOC(sym2) &&
		      SYM_SPIL_LOC(sym1) == SYM_SPIL_LOC(sym2))
		    {
		      if (sym1->reqv && !sym2->reqv) SYM_SPIL_LOC(sym2)=NULL;
		      else if (sym2->reqv && !sym1->reqv) SYM_SPIL_LOC(sym1)=NULL;
		      else if (sym1->used > sym2->used) SYM_SPIL_LOC(sym2)=NULL;
		      else SYM_SPIL_LOC(sym1)=NULL;
		    }
		}
	    }
	}
    }
}

/*-----------------------------------------------------------------*/
/* allDefsOutOfRange - all definitions are out of a range          */
/*-----------------------------------------------------------------*/
bool
allDefsOutOfRange (bitVect * defs, int fseq, int toseq)
{
  int i;

  if (!defs)
    return TRUE;

  for (i = 0; i < defs->size; i++)
    {
      iCode *ic;

      if (bitVectBitValue (defs, i) &&
	  (ic = hTabItemWithKey (iCodehTab, i)) &&
	  (ic->seq >= fseq && ic->seq <= toseq))

	return FALSE;

    }

  return TRUE;
}

/*-----------------------------------------------------------------*/
/* notUsedInBlock - not used in this block                         */
/*-----------------------------------------------------------------*/
int
notUsedInBlock (symbol * sym, eBBlock * ebp, iCode *ic)
{
  return (!bitVectBitsInCommon (sym->defs, ebp->usesDefs) &&
	  allDefsOutOfRange (sym->defs, ebp->fSeq, ebp->lSeq) &&
	  allDefsOutOfRange (sym->uses, ebp->fSeq, ebp->lSeq));
}

/*-----------------------------------------------------------------*/
/* adjustIChain - correct the sch and ech pointers                 */
/*-----------------------------------------------------------------*/
void
adjustIChain (eBBlock ** ebbs, int count)
{
  int i;
  
  for (i = 0; i < count; i++)
    {
      iCode *ic;
      
      if (ebbs[i]->noPath)
        continue;

      ic = ebbs[i]->sch;
      while (ic->prev)
        ic = ic->prev;
      ebbs[i]->sch = ic;
      
      ic = ebbs[i]->ech;
      while (ic->next)
        ic = ic->next;
      ebbs[i]->ech = ic;
    }
}

/*-----------------------------------------------------------------*/
/* computeLiveRanges - computes the live ranges for variables      */
/*-----------------------------------------------------------------*/
void
computeLiveRanges (eBBlock ** ebbs, int count)
{
  /* first look through all blocks and adjust the
     sch and ech pointers */
  adjustIChain (ebbs, count);

  /* sequence the code the live ranges are computed
     in terms of this sequence additionally the
     routine will also create a hashtable of instructions */
  iCodeSeq = 0;
  setToNull ((void *) &iCodehTab);
  iCodehTab = newHashTable (iCodeKey);
  hashiCodeKeys (ebbs, count);
  setToNull ((void *) &iCodeSeqhTab);
  iCodeSeqhTab = newHashTable (iCodeKey);
  sequenceiCode (ebbs, count);

  /* mark the ranges live for each point */
  setToNull ((void *) &liveRanges);
  rlivePoint (ebbs, count);

  /* mark the from & to live ranges for variables used */
  markLiveRanges (ebbs, count);

  /* compute which overlaps with what */
  computeClash(ebbs, count);
}

/*-----------------------------------------------------------------*/
/* recomputeLiveRanges - recomputes the live ranges for variables  */
/*-----------------------------------------------------------------*/
void
recomputeLiveRanges (eBBlock ** ebbs, int count)
{
  symbol * sym;
  int key;

  /* clear all rlive bitVectors */
  rliveClear (ebbs, count);

  sym = hTabFirstItem (liveRanges, &key);
  if (sym)
    {
      do {
        sym->used = 0;
        sym->liveFrom = 0;
        sym->liveTo = 0;
        freeBitVect (sym->clashes);
        sym->clashes = NULL;
      } while ( (sym = hTabNextItem (liveRanges, &key)));
    }

  /* do the LR computation again */
  computeLiveRanges (ebbs, count);
}

