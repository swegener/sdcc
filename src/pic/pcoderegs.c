/*-------------------------------------------------------------------------

   pcoderegs.c - post code generation register optimizations

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

/*
  pcoderegs.c

  The purpose of the code in this file is to optimize the register usage.

*/
#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"
#include "ralloc.h"
#include "device.h"
#include "pcode.h"
#include "pcoderegs.h"
#include "pcodeflow.h"

extern void pCodeInsertAfter(pCode *pc1, pCode *pc2);
extern void dbg_dumpregusage(void);
extern pCode * findPrevInstruction(pCode *pci);
void unlinkpCode(pCode *pc);

int total_registers_saved=0;

/*-----------------------------------------------------------------*
 * void AddRegToFlow(regs *reg, pCodeFlow *pcfl)
 *-----------------------------------------------------------------*/
/*
void AddRegToFlow(regs *reg, pCodeFlow *pcfl)
{

  if(!reg || ! pcfl || !isPCFL(pcflow))
    return;

  if(!pcfl->registers) 
    pcfl->registers =  newSet();

}
*/


/*-----------------------------------------------------------------*
 * 
 *-----------------------------------------------------------------*/
void dbg_regusage(set *fregs)
{
  regs *reg;
  pCode *pcfl;
  pCode *pc;


  for (reg = setFirstItem(fregs) ; reg ;
       reg = setNextItem(fregs)) {

    if(elementsInSet(reg->reglives.usedpCodes)) {
    
      fprintf (stderr, "%s  addr=0x%03x rIdx=0x%03x",
	       reg->name,
	       reg->address,
	       reg->rIdx);

      pcfl = setFirstItem(reg->reglives.usedpFlows);
      if(pcfl)
	fprintf(stderr, "\n   used in seq");

      while(pcfl) {
	fprintf(stderr," 0x%03x",pcfl->seq);
	pcfl = setNextItem(reg->reglives.usedpFlows);
      }

      pcfl = setFirstItem(reg->reglives.assignedpFlows);
      if(pcfl)
	fprintf(stderr, "\n   assigned in seq");

      while(pcfl) {
	fprintf(stderr," 0x%03x",pcfl->seq);
	pcfl = setNextItem(reg->reglives.assignedpFlows);
      }

      pc = setFirstItem(reg->reglives.usedpCodes);
      if(pc)
	fprintf(stderr, "\n   used in instructions ");

      while(pc) {
	pcfl = PCODE(PCI(pc)->pcflow);
	if(pcfl)
	  fprintf(stderr," 0x%03x:",pcfl->seq);
	fprintf(stderr,"0x%03x",pc->seq);

	pc = setNextItem(reg->reglives.usedpCodes);
      }

      fprintf(stderr, "\n");
    }
  }
}

/*-----------------------------------------------------------------*
 * 
 *-----------------------------------------------------------------*/
void dbg_dumpregusage(void)
{

  fprintf(stderr,"***  Register Usage  ***\n");
  fprintf(stderr,"InternalRegs:\n");
  dbg_regusage(dynInternalRegs);
  fprintf(stderr,"AllocRegs:\n");
  dbg_regusage(dynAllocRegs);
  fprintf(stderr,"StackRegs:\n");
  dbg_regusage(dynStackRegs);
  fprintf(stderr,"DirectRegs:\n");
  dbg_regusage(dynDirectRegs);
  fprintf(stderr,"DirectBitRegs:\n");
  dbg_regusage(dynDirectBitRegs);
  fprintf(stderr,"ProcessorRegs:\n");
  dbg_regusage(dynProcessorRegs);

}


/*-----------------------------------------------------------------*
 * void pCodeRegMapLiveRangesInFlow(pCodeFlow *pcfl)
 *-----------------------------------------------------------------*/
void pCodeRegMapLiveRangesInFlow(pCodeFlow *pcfl)
{

  pCode *pc=NULL;
  pCode *pcprev=NULL;

  regs *reg;

  if(!pcfl)
    return;


  pc = findNextInstruction(pcfl->pc.next);

  while(isPCinFlow(pc,PCODE(pcfl))) {


    reg = getRegFromInstruction(pc);

    if(reg) {
/*
      fprintf(stderr, "flow seq %d, inst seq %d  %s  ",PCODE(pcfl)->seq,pc->seq,reg->name);
      fprintf(stderr, "addr = 0x%03x, type = %d  rIdx=0x%03x\n",
	      reg->address,reg->type,reg->rIdx);
*/

      addSetIfnotP(& (PCFL(pcfl)->registers), reg);

      if((PCC_REGISTER | PCC_LITERAL) & PCI(pc)->inCond)
	addSetIfnotP(& (reg->reglives.usedpFlows), pcfl);

      if(PCC_REGISTER & PCI(pc)->outCond)
	addSetIfnotP(& (reg->reglives.assignedpFlows), pcfl);

      addSetIfnotP(& (reg->reglives.usedpCodes), pc);
    }


    pcprev = pc;
    pc = findNextInstruction(pc->next);

  }

}

/*-----------------------------------------------------------------*
 * void pCodeRegMapLiveRanges(pBlock *pb) 
 *-----------------------------------------------------------------*/
void pCodeRegMapLiveRanges(pBlock *pb)
{
  pCode *pcflow;

  for( pcflow = findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = findNextpCode(pcflow->next, PC_FLOW) ) {

    if(!isPCFL(pcflow)) {
      fprintf(stderr, "pCodeRegMapLiveRanges - pcflow is not a flow object ");
      continue;
    }
    pCodeRegMapLiveRangesInFlow(PCFL(pcflow));
  }


  for( pcflow = findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = findNextpCode(pcflow->next, PC_FLOW) ) {

    regs *r = setFirstItem(PCFL(pcflow)->registers);
    //fprintf(stderr,"flow seq %d\n", pcflow->seq);
    while (r) {
      //fprintf(stderr, "  %s\n",r->name);
      r = setNextItem(PCFL(pcflow)->registers);

    }

  }

//  dbg_dumpregusage();

}


/*-----------------------------------------------------------------*
 * void RemoveRegsFromSet(set *regset)
 *
 *-----------------------------------------------------------------*/
void  RemoveRegsFromSet(set *regset)
{
  regs *reg;
  int used;
  for (reg = setFirstItem(regset) ; reg ;
       reg = setNextItem(regset)) {


    used = elementsInSet(reg->reglives.usedpCodes);

    if(used <= 1) {

      //fprintf(stderr," reg %s isfree=%d, wasused=%d\n",reg->name,reg->isFree,reg->wasUsed);

      if(used == 0) {
	//fprintf(stderr," getting rid of reg %s\n",reg->name);
	reg->isFree = 1;
	reg->wasUsed = 0;
      } else {
	pCode *pc;


	pc = setFirstItem(reg->reglives.usedpCodes);
	if(isPCI(pc)) {
	  if(PCI(pc)->label) {
	    pCode *pcn = findNextInstruction(pc->next);

	    if(pcn && PCI(pcn)->label) {
	      //fprintf(stderr,"can't delete instruction with label...\n");
	      //pc->print(stderr,pc);
	      continue;
	    } 
	    /* Move the label to the next instruction */

	    PCI(pcn)->label = PCI(pc)->label;

	  }

	  if(isPCI_SKIP(pc))
	    fprintf(stderr, "WARNING, a skip instruction is being optimized out\n");

	  //fprintf(stderr," removing reg %s because it is used only once\n",reg->name);
	  unlinkpCode(pc);
	  deleteSetItem (&(reg->reglives.usedpCodes),pc);
	  reg->isFree = 1;
	  reg->wasUsed = 0;
	  total_registers_saved++;  // debugging stats.
	}
      }
    }

  }
}
/*-----------------------------------------------------------------*
 * void RemoveUnusedRegisters(void)
 *
 *-----------------------------------------------------------------*/
void RemoveUnusedRegisters(void)
{
  /* First, get rid of registers that are used only one time */


  RemoveRegsFromSet(dynInternalRegs);
  RemoveRegsFromSet(dynAllocRegs);
  RemoveRegsFromSet(dynStackRegs);
  /*
    don't do DirectRegs yet - there's a problem with arrays
  RemoveRegsFromSet(dynDirectRegs);
  */
  RemoveRegsFromSet(dynDirectBitRegs);

  if(total_registers_saved) fprintf(stderr, " *** Saved %d registers ***\n", total_registers_saved);
}


/*-----------------------------------------------------------------*
 *
 *-----------------------------------------------------------------*/
static void Remove2pcodes(pCode *pcflow, pCode *pc1, pCode *pc2, regs *reg)
{
  if(!reg)
    return;

  if(pc1) {
    deleteSetItem (&(reg->reglives.usedpCodes),pc1);
    pc1->destruct(pc1);
  }

  if(pc2) {
    deleteSetItem (&(reg->reglives.usedpCodes),pc2);
    pc2->destruct(pc2);

    deleteSetItem (&(PCFL(pcflow)->registers), reg);

    reg->isFree = 1;
    reg->wasUsed = 0;

  }

  pCodeRegMapLiveRangesInFlow(PCFL(pcflow));
}

/*-----------------------------------------------------------------*
 *
 *-----------------------------------------------------------------*/
int regUsedinRange(pCode *pc1, pCode *pc2, regs *reg)
{
  int i=0;
  regs *testreg;

  do {
    testreg = getRegFromInstruction(pc1);
    if(testreg && (testreg->rIdx == reg->rIdx)) {
      return 1;
    }

    pc1 = findNextInstruction(pc1->next);

  } while (pc1 && (pc1 != pc2) && (i++ < 100)) ;

  if(i >= 100)
    fprintf(stderr, "warning, regUsedinRange searched through too many pcodes\n");

  return 0;
}

/*-----------------------------------------------------------------*
 * void pCodeRegOptimeRegUsage(pBlock *pb) 
 *-----------------------------------------------------------------*/
void OptimizeRegUsage(set *fregs)
{
  regs *reg;
  int used;


  while(fregs) {
      pCode *pcfl_used, *pcfl_assigned;

    /* Step through the set by directly accessing the 'next' pointer.
     * We could also step through by using the set API, but the 
     * the (debug) calls to print instructions affect the state
     * of the set pointers */

    reg = fregs->item;
    fregs = fregs->next;


    pcfl_used = setFirstItem(reg->reglives.usedpFlows);
    pcfl_assigned = setFirstItem(reg->reglives.assignedpFlows);

    used = elementsInSet(reg->reglives.usedpCodes);
    if(used == 2) { 

      /*
       * In this section, all registers that are used in only in two 
       * instructions are examined. If possible, they're optimized out.
       */

      pCode *pc1, *pc2;
      pCode *pct1, *pct2; /* two temporaries */
      regs  *reg1, *reg2;

/*
      fprintf (stderr, "OptimizeRegUsage: %s  addr=0x%03x rIdx=0x%03x  used=%d\n",
	       reg->name,
	       reg->address,
	       reg->rIdx, used);
*/
      pc1 = setFirstItem(reg->reglives.usedpCodes);
      pc2 = setNextItem(reg->reglives.usedpCodes);

      if(pcfl_used && pcfl_assigned) {

	/* 
	   expected case - the register has been assigned a value and is
	   subsequently used 
	*/

	//fprintf(stderr," used only twice\n");
	if(pcfl_used->seq == pcfl_assigned->seq) {

	  //fprintf(stderr, "  and used in same flow\n");
	  if(pc2->seq < pc1->seq) {
	    pct1 = pc2;
	    pc2 = pc1;
	    pc1 = pct1;
	  }

	  //pc1->print(stderr,pc1);
	  //pc2->print(stderr,pc2);


	  /* ADHOC pattern checking 
	   * Now look for specific sequences that are easy to optimize.
	   * Many of these sequences are characteristic of the compiler
	   * (i.e. it'd probably be a waste of time to apply these adhoc
	   * checks to hand written assembly.)
	   * 
	   */
	  if((PCI(pc1)->op == POC_CLRF) && (PCI(pc2)->op == POC_MOVFW) ){

	    /*
	         clrf  reg
		 stuff...
		 movf  reg,w

		 can be replaced with

		 stuff...
		 movlw 0
	    */

	    pCode *newpc;
	    //fprintf(stderr, "   CLRF/MOVFW. instruction after MOVFW is:\n");
	    pct1 = findNextInstruction(pc2->next);
	    //t->print(stderr,t);

	    if(PCI(pct1)->op == POC_MOVWF) {
	      newpc = newpCode(POC_CLRF, PCI(pct1)->pcop);
	      pct1->destruct(pct1);
	    } else {
	      newpc = newpCode(POC_MOVLW, newpCodeOpLit(0));
	    }

	    pCodeInsertAfter(pc2, newpc);
	    PCI(newpc)->pcflow = PCFL(pcfl_used);
	    newpc->seq = pc2->seq;

	    Remove2pcodes(pcfl_used, pc1, pc2, reg);
	    total_registers_saved++;  // debugging stats.

	  } else if((PCI(pc1)->op == POC_CLRF) && (PCI(pc2)->op == POC_IORFW) ){
	    //fprintf(stderr, "   CLRF/IORWF.\n");

	    Remove2pcodes(pcfl_used, pc1, pc2, reg);
	    total_registers_saved++;  // debugging stats.

	  }  else if(PCI(pc1)->op == POC_MOVWF) {

	    pct2 = findNextInstruction(pc2->next);

	    if(PCI(pc2)->op == POC_MOVFW) {
	      //fprintf(stderr, "   MOVWF/MOVFW. instruction after MOVFW is:\n");
	      // t->print(stderr,t);

	      if(PCI(pct2)->op == POC_MOVWF) {
		reg2 = getRegFromInstruction(pct2);
		if(reg2 && !regUsedinRange(pc1,pc2,reg2)) {
		  pct2->seq = pc1->seq;
		  unlinkpCode(pct2);
		  pCodeInsertAfter(pc1,pct2);
		  Remove2pcodes(pcfl_used, pc1, pc2, reg);
		  total_registers_saved++;  // debugging stats.
		  continue;
		}
	      }
	    }

	    pct1 = findPrevInstruction(pc1->prev);
	    if(pct1 && 
	       (PCI(pct1)->pcflow == PCI(pc1)->pcflow) && 
	       (PCI(pct1)->op == POC_MOVFW)) {

	      reg1 = getRegFromInstruction(pct1);
	      if(reg1 && !regUsedinRange(pc1,pc2,reg1)) {
	      /*
		movf   reg1,w
		movwf  reg

		stuff...
		opcode reg,w
	      */
		pct2 = newpCode(PCI(pc2)->op, PCI(pct1)->pcop);
		pCodeInsertAfter(pc2, pct2);
		PCI(pct2)->pcflow = PCFL(pcfl_used);
		pct2->seq = pc2->seq;

		Remove2pcodes(pcfl_used, pc1, pc2, reg);
		Remove2pcodes(pcfl_used, pct1, NULL, reg1);
		total_registers_saved++;  // debugging stats.

	      }
	    }


	  }

	} else {
	  // fprintf(stderr, "  and used in different flows\n");

	  //pc1->print(stderr,pc1);
	  //pc2->print(stderr,pc2);
	}

      } else if(pcfl_used) {

	/*
	  register has been used twice without ever being assigned */
	fprintf(stderr,"WARNING %s: reg %s used without being assigned\n",__FUNCTION__,reg->name);

      } else {
	//fprintf(stderr,"WARNING %s: reg %s assigned without being used\n",__FUNCTION__,reg->name);
	Remove2pcodes(pcfl_assigned, pc1, pc2, reg);
	total_registers_saved++;  // debugging stats.
      }
    } else {

      if(used && !pcfl_used && pcfl_assigned) {
	pCode *pc;

	//fprintf(stderr,"WARNING %s: reg %s assigned without being used\n",__FUNCTION__,reg->name);

	pc = setFirstItem(reg->reglives.usedpCodes);
	while(pc) {
	  pcfl_assigned = PCI(pc)->pcflow;
	  deleteSetItem (&(PCFL(pcfl_assigned)->registers), reg);
	  deleteSetItem (&(reg->reglives.usedpCodes),pc);
	  pc->destruct(pc);

	  pc = setNextItem(reg->reglives.usedpCodes);
	}


	reg->isFree = 1;
	reg->wasUsed = 0;

	total_registers_saved++;  // debugging stats.
      }

    }

  }

}
/*-----------------------------------------------------------------*
 * void pCodeRegOptimeRegUsage(pBlock *pb) 
 *-----------------------------------------------------------------*/
void pCodeRegOptimizeRegUsage(void)
{

  int passes = 4;
  int saved = 0;

  do {
    saved = total_registers_saved;

    /* Identify registers used in one flow sequence */
    OptimizeRegUsage(dynAllocRegs);
    OptimizeRegUsage(dynStackRegs);
    OptimizeRegUsage(dynDirectRegs);

    if(total_registers_saved != saved)
      fprintf(stderr, " *** Saved %d registers, total saved %d ***\n", total_registers_saved-saved,total_registers_saved);


  } while( passes-- && (total_registers_saved != saved));
/*
  fprintf(stderr,"dynamically allocated regs:\n");
  dbg_regusage(dynAllocRegs);
  fprintf(stderr,"stack regs:\n");
  dbg_regusage(dynStackRegs);
  fprintf(stderr,"direct regs:\n");
  dbg_regusage(dynDirectRegs);
*/


}
