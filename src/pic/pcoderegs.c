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

extern void dbg_dumpregusage(void);
void unlinkpCode(pCode *pc);

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

      if(PCC_REGISTER & PCI(pc)->inCond)
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

	//fprintf(stderr," reg %s used only once\n",reg->name);

	pc = setFirstItem(reg->reglives.usedpCodes);
	if(isPCI(pc)) {
	  if(PCI(pc)->label) {
	    pCode *pcn = findNextInstruction(pc->next);

	    if(pcn && PCI(pcn)->label) {
	      fprintf(stderr,"can't delete instruction with label...\n");
	      pc->print(stderr,pc);
	      continue;
	    } 
	    /* Move the label to the next instruction */

	    PCI(pcn)->label = PCI(pc)->label;

	  }

	  if(isPCI_SKIP(pc))
	    fprintf(stderr, "WARNING, a skip instruction is being optimized out\n");

	  unlinkpCode(pc);
	  deleteSetItem (&(reg->reglives.usedpCodes),pc);
	  reg->isFree = 1;
	  reg->wasUsed = 0;
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


  //fprintf(stderr,"InternalRegs:\n");
  RemoveRegsFromSet(dynInternalRegs);
  //fprintf(stderr,"AllocRegs:\n");
  RemoveRegsFromSet(dynAllocRegs);
  //fprintf(stderr,"StackRegs:\n");
  RemoveRegsFromSet(dynStackRegs);

  /*
    don't do DirectRegs yet - there's a problem with arrays
  //fprintf(stderr,"DirectRegs:\n");
  RemoveRegsFromSet(dynDirectRegs);
  */
  //fprintf(stderr,"DirectBitRegs:\n");
  RemoveRegsFromSet(dynDirectBitRegs);



}
