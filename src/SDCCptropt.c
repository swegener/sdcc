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

#include <stdio.h>
#include <stdlib.h>
#include "SDCCglobl.h"
#include "SDCCast.h"
#include "SDCCmem.h"
#include "SDCCy.h"
#include "SDCChasht.h"
#include "SDCCbitv.h"
#include "SDCCset.h"
#include "SDCCicode.h"
#include "SDCClabel.h"
#include "SDCCBBlock.h"
#include "SDCCcflow.h"
#include "SDCCloop.h"
#include "SDCCcse.h"


/*-----------------------------------------------------------------*/
/* pointerArithOpts - does some pointer arithmetic operations      */
/*-----------------------------------------------------------------*/
 int pointerArithOpts (iCode *ic,  hTab *ptrs)
{
    operand *asym ;
    long  cval;
    hashtItem *htip;
    induction *ip ;
    int lr = 0 ;

    /* the aim here is to reduce the arithmetics performed */
    /* during repeated pointer access . for eg. there are  */
    /* lot of places the code generated looks like this    */
    /*  iTempNN = &[somevar]                               */
    /*  iTempNX = iTempNN + constant-1                     */
    /*  *(iTempNX) := somevalue                            */
    /*  iTempNY = iTempNN + constant-2                     */
    /*  *(iTempNY) := somevalue                            */
    /*          we want to change this sequence to         */
    /*  iTempNN = &[somevar]                               */
    /*  iTempNN = iTempNN + constant-1                     */
    /*  (*iTempNN) := someValue                            */
    /* WHY ? this will reduce the number of definitions    */
    /* as well as help us in register allocation           */
    
    /* first check if +/- */
    if (ic->op != '+' && ic->op != '-' )
	return 0;

    /* if the result is not an iTemp */
    if (!IS_ITEMP(IC_RESULT(ic)))
	return 0;

    /* if +/- then check if one of them is literal */
    if (! IS_OP_LITERAL(IC_LEFT(ic)) &&
	! IS_OP_LITERAL(IC_RIGHT(ic)) )
	return 0;

    /* get the operand & literal value */
    if  (IS_OP_LITERAL(IC_LEFT(ic))) {
	    cval = operandLitValue(IC_LEFT(ic));
	    asym = IC_RIGHT(ic) ;
    } else {
	cval = operandLitValue(IC_RIGHT(ic)) ;
	lr++ ;
	asym = IC_LEFT(ic);
    }

    /* if this symbol does not exist the address of */
    /* or if the symbol is the same as the result   */
    if (asym->key == IC_RESULT(ic)->key ||
	(htip = hTabSearch(ptrs,asym->key)) == NULL )
	return 0;
    
    ip = htip->item;

    
    /* if the constant value in induction is zero */
    /* then we just change the operand            */
    if (ip->cval == 0) {	
	ip->asym = IC_RESULT(ic) ;
	ip->cval = cval ; /* update the offset info */
	return 0;
    }
    /* we can do the substitution */
    replaceAllSymBySym (ic->next, IC_RESULT(ic),ip->asym);
    IC_RESULT(ic) = operandFromOperand(ip->asym);
    
    /* now we know that the constant value is greater than zero */
    /* we have to do some arithmetic here : if the offset is greater */
    /* than what we want then rechange it to subtraction        */
    if ( ip->cval > cval) {
	ic->op = '-' ;
	if (lr) {
	    IC_LEFT(ic) = operandFromOperand(ip->asym);
	    IC_RIGHT(ic) = operandFromLit(ip->cval - cval);
	}
	else {
	    IC_RIGHT(ic) = operandFromOperand(ip->asym);
	    IC_LEFT(ic) = operandFromLit(ip->cval - cval);
	}
    } else {
	if (lr) {
	    IC_LEFT(ic) = operandFromOperand(ip->asym);
	    IC_RIGHT(ic) = operandFromLit(cval - ip->cval);
	}
	else {
	    IC_RIGHT(ic) = operandFromOperand (ip->asym);
	    IC_LEFT(ic) = operandFromLit(cval - ip->cval);
	}
    }
    
    ip->cval = cval ;
    return 1;
}

/*-----------------------------------------------------------------*/
/* ptrAopts - pointer arithmetic optimizations for a basic block   */
/*-----------------------------------------------------------------*/
void ptrAopts (eBBlock **ebbs, int count)
{
    int i;

    hTab *pointers ; 

    /* for each basic block do */
    for (i = 0 ; i < count ; i++ ) {
	iCode *ic;

	setToNull((void **) &pointers);
	/* iterate thru intructions in the basic block */
	for (ic = ebbs[i]->sch ; ic ; ic = ic->next ) {

	    if (SKIP_IC(ic) ||ic->op == IFX)
		continue ;
	    
	    if (pointers)
		pointerArithOpts(ic,pointers);

	    if (!POINTER_SET(ic) && IC_RESULT(ic))
		/* delete from pointer access */
		hTabDeleteItem (&pointers, IC_RESULT(ic)->key,NULL,DELETE_CHAIN,NULL);

	    	/* set up for pointers */
	    if ((ic->op == ADDRESS_OF  || ADD_SUBTRACT_ITEMP(ic)) &&
		IS_ITEMP(IC_RESULT(ic)) ) {
		operand *sym ;
		operand *asym ;
		long cval    ;
		
		if ( ic->op == ADDRESS_OF){	      
		    sym = IC_RESULT(ic);
		    asym = NULL ;
		    cval = 0;
		} else {
		    sym = IC_LEFT(ic);
		    asym = IC_RESULT(ic);
		    cval = operandLitValue(IC_RIGHT(ic));
		}
		
		/* put it in the pointer set */	   		
	    }	
	}

    }
}
