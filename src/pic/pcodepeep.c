/*-------------------------------------------------------------------------

   pcodepeep.c - post code generation
   Written By -  Scott Dattalo scott@dattalo.com

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

#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "pcode.h"
#include "ralloc.h"


pCodeOp *newpCodeOpWild(int id, pCodePeep *pcp, pCodeOp *subtype);
pCode *newpCodeWild(int pCodeID, pCodeOp *optional_operand, pCodeOp *optional_label);
pCode * findNextInstruction(pCode *pc);
char *Safe_strdup(char *str);


/****************************************************************/
/*
 * rootRules - defined in SDCCpeep.c
 *  This is a pointer to the (parsed) peephole rules that are
 * defined in peep.def.
 */

extern peepRule *rootRules;


/****************************************************************/
/****************************************************************/
typedef struct _DLL {
  struct _DLL *prev;
  struct _DLL *next;
  //  void *data;
} _DLL;


typedef struct pCodePeepSnippets
{
  _DLL dll;
  pCodePeep *peep;
} pCodePeepSnippets;


static pCodePeepSnippets  *peepSnippets=NULL;


void printpCodeString(FILE *of, pCode *pc, int max)
{
  int i=0;

  while(pc && (i++<max)) {
    pc->print(of,pc);
    pc = pc->next;
  }
}

/*-----------------------------------------------------------------*/
/* _DLL * DLL_append                                               */
/*                                                                 */ 
/*  Append a _DLL object to the end of a _DLL (doubly linked list) */ 
/* If The list to which we want to append is non-existant then one */ 
/* is created. Other wise, the end of the list is sought out and   */ 
/* a new DLL object is appended to it. In either case, the void    */
/* *data is added to the newly created DLL object.                 */
/*-----------------------------------------------------------------*/

static void * DLL_append(_DLL *list, _DLL *next)
{
  _DLL *b;


  /* If there's no list, then create one: */
  if(!list) {
    next->next = next->prev = NULL;
    return next;
  }


  /* Search for the end of the list. */
  b = list;
  while(b->next)
    b = b->next;

  /* Now append the new DLL object */
  b->next = next;
  b->next->prev = b;
  b = b->next; 
  b->next = NULL;

  return list;
  
}  


/*-----------------------------------------------------------------

  pCode peephole optimization


  The pCode "peep hole" optimization is not too unlike the peep hole
  optimization in SDCCpeeph.c. The major difference is that here we
  use pCode's whereas there we use ASCII strings. The advantage with
  pCode's is that we can ascertain flow information in the instructions
  being optimized.


<FIX ME> - elaborate...

  -----------------------------------------------------------------*/

#if 0
/*-----------------------------------------------------------------*/
/* pCodePeep */
/*-----------------------------------------------------------------*/
int pCodePeepCompare(pCode *pc, pCodePeep *pcp)
{
  pCode *pcfrom,*pcto;

  pcfrom = pc;
  for( pcto=pcp->target; pcto; pcto=pcto->next) {

    pcfrom = findNextInstruction(pcfrom);

    if( pcfrom &&  
	(PCI(pcfrom)->op == PCI(pcto)->op || 
	 PCI(pcto)->op == POC_WILD))
      continue;
    return 0;
  }
  return 0;
}

/*-----------------------------------------------------------------*/
/* pCodePeep */
/*-----------------------------------------------------------------*/
void pCodePeepSearch(pCodePeep *snippet)
{
  pBlock *pb;
  pCode *pc;

  if(!the_pFile)
    return;

  /* compare the chain to the pCode that we've 
     got so far. If a match is found, then replace
     the pCode chain.
  */
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    for(pc = pb->pcHead; pc; pc = pc->next) {
      pCodePeepCompare(pc,snippet);
    }
  }

}
#endif

#if 0
pBlock *pBlockAppend(pBlock *pb1, pBlock *pb2)
{
  pBlock *pb;

  if(!pb1->tail)
    return pb2;

  pb = pb1->tail;

  pb2->head = pb1;
  pb2->tail = NULL;
  pb1->tail = pb2;

}

#endif

void pCodePeepInit(void)
{
  pBlock *pb;
  //  pCode *pc;
  pCodePeep *pcp;
  pCodePeepSnippets *pcps;

  /* Declare a peep code snippet */
  /* <FIXME> do I really need a separate struct just to DLL the snippets? */
  /* e.g. I could put the DLL into the pCodePeep structure */
  pcps = Safe_calloc(1,sizeof(pCodePeepSnippets));
  pcp = pcps->peep  = Safe_calloc(1,sizeof(pCodePeep));
  peepSnippets = DLL_append((_DLL*)peepSnippets,(_DLL*)pcps);


  pb = newpCodeChain(NULL, 'W', newpCode(POC_MOVWF, newpCodeOpWild(0,pcp,newpCodeOp(NULL,PO_GPR_REGISTER))) );
  addpCode2pBlock( pb,     newpCode(POC_MOVFW, newpCodeOpWild(0,pcp,newpCodeOp(NULL,PO_GPR_REGISTER))) );

  pcp->target = pb;

  pcp->replace = newpCodeChain(NULL, 'W',newpCode(POC_MOVWF, newpCodeOpWild(0,pcp,newpCodeOp(NULL,PO_GPR_REGISTER))) );

  /* Allocate space to store pointers to the wildcard variables */
  pcp->nvars = 1;
  pcp->vars  = Safe_calloc(pcp->nvars, sizeof(char *));
  pcp->nwildpCodes = 0;
  pcp->wildpCodes  = NULL;

  pcp->postFalseCond = PCC_Z;
  pcp->postTrueCond  = PCC_NONE;

  fprintf(stderr,"Peep rule\nTarget:\n");
  printpCodeString(stderr,pcp->target->pcHead, 10);
  fprintf(stderr,"Replaced with:\n");
  printpCodeString(stderr,pcp->replace->pcHead, 10);

  /* Now for another peep example */
  pcps = Safe_calloc(1,sizeof(pCodePeepSnippets));
  pcp = pcps->peep  = Safe_calloc(1,sizeof(pCodePeep));
  peepSnippets = DLL_append((_DLL*)peepSnippets,(_DLL*)pcps);

  {
    pCodeOp *pcl;
    pCodeOp *pcw;
    pCodeOp *pcwb;

    pcwb =  newpCodeOpWild(0,pcp,newpCodeOpBit(NULL,-1));
    pb = newpCodeChain(NULL, 'W',newpCode(POC_BTFSC,pcwb));

    pcl = newpCodeOpLabel(-1);
    pcw = newpCodeOpWild(1, pcp, pcl);
    addpCode2pBlock( pb,     newpCode(POC_GOTO,  pcw));
    addpCode2pBlock( pb,     newpCodeWild(0,NULL,NULL));
    addpCode2pBlock( pb,     newpCodeWild(1,NULL,pcw));


    pcp->target = pb;

    pb = newpCodeChain(NULL, 'W',newpCode(POC_BTFSS, pcwb));
    addpCode2pBlock( pb,     newpCodeWild(0,NULL,NULL));
    addpCode2pBlock( pb,     newpCodeWild(1,NULL,pcw));

    pcp->replace = pb;

    /* Allocate space to store pointers to the wildcard variables */
    pcp->nvars = 2;
    pcp->vars = Safe_calloc(pcp->nvars, sizeof(char *));
    pcp->nwildpCodes = 2;
    pcp->wildpCodes = Safe_calloc(pcp->nwildpCodes, sizeof(pCode *));

    pcp->postFalseCond = PCC_NONE;
    pcp->postTrueCond  = PCC_NONE;
  }










  //-------------

  /* Now for another peep example */
  pcps = Safe_calloc(1,sizeof(pCodePeepSnippets));
  pcp = pcps->peep  = Safe_calloc(1,sizeof(pCodePeep));
  peepSnippets = DLL_append((_DLL*)peepSnippets,(_DLL*)pcps);

  {
    pCodeOp *pcw;

    pcw = newpCodeOpWild(0,pcp,newpCodeOp(NULL,PO_GPR_REGISTER));

    pb = newpCodeChain(NULL, 'W', newpCode(POC_MOVWF, pcw));
    addpCode2pBlock( pb,     newpCode(POC_MOVWF, pcw));

    pcp->target = pb;

    pb = newpCodeChain(NULL, 'W',newpCode(POC_MOVWF, pcw));

    pcp->replace = pb;

    /* Allocate space to store pointers to the wildcard variables */
    pcp->nvars = 1;
    pcp->vars = Safe_calloc(pcp->nvars, sizeof(char *));
    pcp->nwildpCodes = 0;
    pcp->wildpCodes = NULL;

    pcp->postFalseCond = PCC_NONE;
    pcp->postTrueCond  = PCC_NONE;
  }




}

/*-----------------------------------------------------------------*/
/* pCodeSearchCondition - Search a pCode chain for a 'condition'   */
/*                                                                 */
/* return conditions                                               */
/*  1 - The Condition was found for a pCode's input                */
/*  0 - No matching condition was found for the whole chain        */
/* -1 - The Condition was found for a pCode's output               */
/*                                                                 */
/*-----------------------------------------------------------------*/
int pCodeSearchCondition(pCode *pc, unsigned int cond)
{

  while(pc) {

    /* If we reach a function end (presumably an end since we most
       probably began the search in the middle of a function), then
       the condition was not found. */
    if(pc->type == PC_FUNCTION)
      return 0;

    if(pc->type == PC_OPCODE) {
      if(PCI(pc)->inCond & cond)
	return 1;
      if(PCI(pc)->outCond & cond)
	return -1;
    }

    pc = pc->next;
  }

  return 0;
}
/*-----------------------------------------------------------------*/
/* pCodePeepMatchLine - Compare source and destination pCodes to   */
/*                      see they're the same.                      */
/*-----------------------------------------------------------------*/
int pCodePeepMatchLine(pCodePeep *peepBlock, pCode *pcs, pCode *pcd)
{
  int index;   // index into wild card arrays

  if(pcs->type == pcd->type) {

    if(pcs->type == PC_OPCODE) {

      /* If the opcodes don't match then the line doesn't match */
      if(PCI(pcs)->op != PCI(pcd)->op)
	return 0;

      fprintf(stderr,"%s comparing\n",__FUNCTION__);
      pcs->print(stderr,pcs);
      pcd->print(stderr,pcd);

      /* Compare the operands */
      if(PCI(pcd)->pcop) {
	if (PCI(pcd)->pcop->type == PO_WILD) {
	  index = PCOW(PCI(pcd)->pcop)->id;

	  fprintf(stderr,"destination is wild\n");
#ifdef DEBUG_PCODEPEEP
	  if (index > peepBlock->nvars) {
	    fprintf(stderr,"%s - variables exceeded\n",__FUNCTION__);
	    exit(1);
	  }
#endif
	  PCOW(PCI(pcd)->pcop)->matched = PCI(pcs)->pcop;
	  {
	    char *n;

	    if(PCI(pcs)->pcop->type == PO_GPR_TEMP)
	      n = PCOR(PCI(pcs)->pcop)->r->name;
	    else
	      n = PCI(pcs)->pcop->name;

	    if(peepBlock->vars[index])
	      return  (strcmp(peepBlock->vars[index],n) == 0);
	    else {
	      peepBlock->vars[index] = n; //PCI(pcs)->pcop->name;
	      return 1;
	    }
	  }
	}
      } else
	/* The pcd has no operand. Lines match if pcs has no operand either*/
	return (PCI(pcs)->pcop == NULL);
    }
  }


  if((pcd->type == PC_WILD) && (pcs->type == PC_OPCODE)) {

    int labindex;

    index = PCW(pcd)->id;

    fprintf(stderr,"%s comparing wild cards\n",__FUNCTION__);
    pcs->print(stderr,pcs);
    pcd->print(stderr,pcd);

    peepBlock->wildpCodes[PCW(pcd)->id] = pcs;

    /* Check for a label associated with this wild pCode */
    // If the wild card has a label, make sure the source code does too.
    if(PCW(pcd)->label) {
      if(!pcs->label)
	return 0;

      labindex = PCOW(PCW(pcd)->label)->id;
      if(peepBlock->vars[labindex] == NULL) {
	// First time to encounter this label
	peepBlock->vars[labindex] = PCL(pcs->label->pc)->label;
	fprintf(stderr,"first time for a label\n");
      } else {
	if(strcmp(peepBlock->vars[labindex],PCL(pcs->label->pc)->label) != 0) {
	  fprintf(stderr,"labels don't match\n");
	  return 0;
	}
	fprintf(stderr,"matched a label\n");
      }

    }

    if(PCW(pcd)->operand) {
      PCOW(PCI(pcd)->pcop)->matched = PCI(pcs)->pcop;
      if(peepBlock->vars[index]) {
	int i = (strcmp(peepBlock->vars[index],PCI(pcs)->pcop->name) == 0);
	if(i)
	  fprintf(stderr," (matched)\n");
	else {
	  fprintf(stderr," (no match: wild card operand mismatch\n");
	  fprintf(stderr,"  peepblock= %s,  pcodeop= %s\n",
		  peepBlock->vars[index],
		  PCI(pcs)->pcop->name);
	}
	return i;
      } else {
	peepBlock->vars[index] = PCI(pcs)->pcop->name;
	return 1;
      }
    }

    pcs = findNextInstruction(pcs->next); 
    if(pcs) {
      fprintf(stderr," (next to match)\n");
      pcs->print(stderr,pcs);
    }

    return 1; /*  wild card matches */
  }

  return 0;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void pCodePeepClrVars(pCodePeep *pcp)
{

  int i;

  for(i=0;i<pcp->nvars; i++)
    pcp->vars[i] = NULL;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void pCodeInsertAfter(pCode *pc1, pCode *pc2)
{

  if(!pc1 || !pc2)
    return;

  pc2->next = pc1->next;
  if(pc1->next)
    pc1->next->prev = pc2;

  pc2->prev = pc1;
  pc1->next = pc2;

}

/*-----------------------------------------------------------------*/
/* pCodeOpCopy - copy a pcode operator                             */
/*-----------------------------------------------------------------*/
static pCodeOp *pCodeOpCopy(pCodeOp *pcop)
{
  pCodeOp *pcopnew=NULL;

  if(!pcop)
    return NULL;

  switch(pcop->type) { 
  case PO_CRY:
  case PO_BIT:
    pcopnew = Safe_calloc(1,sizeof(pCodeOpBit) );
    PCOB(pcopnew)->bit = PCOB(pcop)->bit;
    PCOB(pcopnew)->inBitSpace = PCOB(pcop)->inBitSpace;

    break;

  case PO_WILD:
    /* Here we expand the wild card into the appropriate type: */
    /* By recursively calling pCodeOpCopy */
    if(PCOW(pcop)->matched)
      pcopnew = pCodeOpCopy(PCOW(pcop)->matched);
    else {
      // Probably a label
      pcopnew = pCodeOpCopy(PCOW(pcop)->subtype);
      pcopnew->name = Safe_strdup(PCOW(pcop)->pcp->vars[PCOW(pcop)->id]);
      fprintf(stderr,"copied a wild op named %s\n",pcopnew->name);
    }

    return pcopnew;
    break;

  case PO_LABEL:
    pcopnew = Safe_calloc(1,sizeof(pCodeOpLabel) );
    PCOLAB(pcopnew)->key =  PCOLAB(pcop)->key;
    break;

  case PO_LITERAL:
  case PO_IMMEDIATE:
    pcopnew = Safe_calloc(1,sizeof(pCodeOpLit) );
    PCOL(pcopnew)->lit = PCOL(pcop)->lit;
    break;

  case PO_GPR_REGISTER:
  case PO_GPR_TEMP:
  case PO_SFR_REGISTER:
  case PO_DIR:
  case PO_STR:
  case PO_NONE:
  case PO_W:
  case PO_STATUS:
  case PO_FSR:
  case PO_INDF:

    pcopnew = Safe_calloc(1,sizeof(pCodeOp) );

  }

  pcopnew->type = pcop->type;
  pcopnew->name = Safe_strdup(pcop->name);

  return pcopnew;
}

#if 0
/*-----------------------------------------------------------------*/
/* pCodeCopy - copy a pcode                                        */
/*-----------------------------------------------------------------*/
static pCode *pCodeCopy(pCode *pc)
{

  pCode *pcnew;

  pcnew = newpCode(pc->type,pc->pcop);
}
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void pCodeDeleteChain(pCode *f,pCode *t)
{
  pCode *pc;

  while(f && f!=t) {
    fprintf(stderr,"delete pCode:\n");
    pc = f->next;
    f->print(stderr,f);
    //f->delete(f);
    f = pc;
  }

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
int pCodePeepMatchRule(pCode *pc)
{
  pCodePeep *peepBlock;
  pCode *pct, *pcin;
  _DLL *peeprules;
  int matched;

  peeprules = (_DLL *)peepSnippets;

  while(peeprules) {
    peepBlock = ((pCodePeepSnippets*)peeprules)->peep;
    pCodePeepClrVars(peepBlock);

    pcin = pc;
    pct = peepBlock->target->pcHead;
    matched = 0;
    while(pct && pcin) {

      if(! (matched = pCodePeepMatchLine(peepBlock, pcin,pct)))
	break;

      pcin = findNextInstruction(pcin->next);
      pct = pct->next;
      //debug:
      fprintf(stderr,"    matched\n");
      if(!pcin)
	fprintf(stderr," end of code\n");
      if(!pct)
	fprintf(stderr," end of rule\n");
    }

    if(matched && pcin) {

      /* So far we matched the rule up to the point of the conditions .
       * In other words, all of the opcodes match. Now we need to see
       * if the post conditions are satisfied.
       * First we check the 'postFalseCond'. This means that we check
       * to see if any of the subsequent pCode's in the pCode chain 
       * following the point just past where we have matched depend on
       * the `postFalseCond' as input then we abort the match
       */
      fprintf(stderr,"    matched rule so far, now checking conditions\n");
      if (peepBlock->postFalseCond && 
	  (pCodeSearchCondition(pcin,peepBlock->postFalseCond) > 0) )
	matched = 0;
    }

    if(matched && pcin) {

      pCode *pcprev;
      pCode *pcr;


      /* We matched a rule! Now we have to go through and remove the
	 inefficient code with the optimized version */

      fprintf(stderr, "Found a pcode peep match:\nRule:\n");
      printpCodeString(stderr,peepBlock->target->pcHead,10);
      fprintf(stderr,"first thing matched\n");
      pc->print(stderr,pc);
      fprintf(stderr,"last thing matched\n");
      pcin->print(stderr,pcin);

      /* Unlink the original code */
      pcprev = pc->prev;
      pcprev->next = pcin;
      pcin->prev = pc->prev;
      pCodeDeleteChain(pc,pcin);

      /* Generate the replacement code */
      pc = pcprev;
      pcr = peepBlock->replace->pcHead;  // This is the replacement code
      while (pcr) {
	pCodeOp *pcop=NULL;
	/* If the replace pcode is an instruction with an operand, */
	/* then duplicate the operand (and expand wild cards in the process). */
	if(pcr->type == PC_OPCODE) {
	  if(PCI(pcr)->pcop)
	    pcop = pCodeOpCopy(PCI(pcr)->pcop);

	  pCodeInsertAfter(pc, newpCode(PCI(pcr)->op,pcop));
	} else if (pcr->type == PC_WILD) {
	  pCodeInsertAfter(pc,peepBlock->wildpCodes[PCW(pcr)->id]);
	}


	pc = pc->next;
	pc->print(stderr,pc);
	pcr = pcr->next;
      }

      return 1;
    }

    peeprules = peeprules->next;
  }

  return 0;
}
