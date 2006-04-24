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
#include "main.h"

extern void dbg_dumpregusage(void);
extern pCode * findPrevInstruction(pCode *pci);
extern pBranch * pBranchAppend(pBranch *h, pBranch *n);
void unlinkpCode(pCode *pc);
extern int pCodeSearchCondition(pCode *pc, unsigned int cond, int contIfSkip);
char *pCode2str(char *str, int size, pCode *pc);
void SAFE_snprintf(char **str, size_t *size, const  char  *format, ...);
//static int sameRegs (const regs *reg1, const regs *reg2);

int total_registers_saved=0;
int register_optimization=1;

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
	
	regs *reg;
	
	if(!pcfl)
		return;
	
	
	pc = findNextInstruction(pcfl->pc.next);
	
	while(pc && !isPCFL(pc)) {
		while (pc && !isPCI(pc) && !isPCFL(pc))
		{
			pc = pc->next;
		} // while
		if (!pc || isPCFL(pc)) continue;
		assert( isPCI(pc) );

		reg = getRegFromInstruction(pc);
		#if 0
		pc->print(stderr, pc);
		fprintf( stderr, "--> reg %p (%s,%u), inCond/outCond: %x/%x\n",
			reg, reg ? reg->name : "(null)", reg ? reg->rIdx : -1,
			PCI(pc)->inCond, PCI(pc)->outCond );
		#endif
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
		
		
		//pc = findNextInstruction(pc->next);
		pc = pc->next;
		
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
	
#if 0
	for( pcflow = findNextpCode(pb->pcHead, PC_FLOW); 
	pcflow != NULL;
	pcflow = findNextpCode(pcflow->next, PC_FLOW) ) {
		
		regs *r = setFirstItem(PCFL(pcflow)->registers);
		fprintf(stderr,"flow seq %d\n", pcflow->seq);
		
		while (r) {
			fprintf(stderr, "  %s\n",r->name);
			r = setNextItem(PCFL(pcflow)->registers);
			
		}
		
	}
#endif
	
	//  dbg_dumpregusage();
	
}


/*-----------------------------------------------------------------*
*
*-----------------------------------------------------------------*/
static void Remove1pcode(pCode *pc, regs *reg, int debug_code)
{

	pCode *pcn=NULL;
	
	if(!reg || !pc)
		return;

	deleteSetItem (&(reg->reglives.usedpCodes),pc);
	
	if(PCI(pc)->label) {
		pcn = findNextInstruction(pc->next);
		
		if(pcn)
			PCI(pcn)->label = pBranchAppend(PCI(pcn)->label,PCI(pc)->label);
	}
	
	if(PCI(pc)->cline) {
		if(!pcn)
			pcn = findNextInstruction(pc->next);
		
		if(pcn) {
			if(PCI(pcn)->cline) {
				//fprintf(stderr, "source line has been optimized completely out\n");
				//pc->print(stderr,pc);
			} else {
				PCI(pcn)->cline = PCI(pc)->cline;
			}
		}
	}
	
	
	if(1){
	/*
	Debug stuff. Comment out the instruction we're about to delete.
		*/
		
		char buff1[256];
		size_t size = 256;
		
		char *pbuff,**ppbuff;
		pbuff = buff1;
		ppbuff = &pbuff;
		
		SAFE_snprintf(ppbuff,&size, ";%d", debug_code);
		pCode2str(*ppbuff, size, pc);
		pCodeInsertBefore(pc, newpCodeCharP(buff1));
		//fprintf(stderr,"removing instruction:\n%s\n",buff1);
	}
	
	pc->destruct(pc);
	
}

/*-----------------------------------------------------------------*
* void RemoveRegsFromSet(set *regset)
*
*-----------------------------------------------------------------*/
void  RemoveRegsFromSet(set *regset)
{
	regs *reg;
	int used;
	
	while(regset) {
		reg = regset->item;
		regset = regset->next;
		
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
				
				if(reg->type == REG_SFR || reg->type == REG_STK || reg->isPublic || reg->isExtern) {
					//fprintf(stderr, "not removing SFR reg %s even though used only once\n",reg->name);
					continue;
				}
				
				
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
					
					if(isPCI_SKIP(pc)) {
						regs *r = getRegFromInstruction(pc);
						fprintf(stderr, "WARNING, a skip instruction is being optimized out\n");
						pc->print(stderr,pc);
						fprintf(stderr,"reg %s, type =%d\n",r->name, r->type);
					}
					//fprintf(stderr," removing reg %s because it is used only once\n",reg->name);
					Remove1pcode(pc, reg, 1);
					/*
					unlinkpCode(pc);
					deleteSetItem (&(reg->reglives.usedpCodes),pc);
					*/
					reg->isFree = 1;
					reg->wasUsed = 0;
					total_registers_saved++;  // debugging stats.
				}
			}
		}
		
	}
}

void RegsUnMapLiveRanges(void);
extern pFile *the_pFile;
void pic14_ReMapLiveRanges(void)
{
	pBlock *pb;
	if (!the_pFile) return;
	RegsUnMapLiveRanges();
	for (pb = the_pFile->pbHead; pb; pb = pb->next)
	{
	#if 0
		pCode *pc = findNextpCode(pb->pcHead, PC_FLOW);
		if (pc) {
			pc->print( stderr, pc );
		} else {
			fprintf( stderr, "unnamed pBlock\n");
		}
		pc = findNextInstruction(pb->pcHead);
		while (pc) {
		  pc->print( stderr, pc );
		  pc = findNextInstruction(pc->next);;
		}
	#endif	
		pCodeRegMapLiveRanges(pb);
	} // for
}

/*-----------------------------------------------------------------*
* void RemoveUnusedRegisters(void)
*
*-----------------------------------------------------------------*/
void RemoveUnusedRegisters(void)
{
	/* First, get rid of registers that are used only one time */
	pic14_ReMapLiveRanges();
	
	//RemoveRegsFromSet(dynInternalRegs);
	RemoveRegsFromSet(dynAllocRegs);
	RemoveRegsFromSet(dynStackRegs);
	/*
	don't do DirectRegs yet - there's a problem with arrays
	RemoveRegsFromSet(dynDirectRegs);
	*/
	RemoveRegsFromSet(dynDirectBitRegs);
	
	if(total_registers_saved) DFPRINTF((stderr, " *** Saved %d registers ***\n", total_registers_saved));
}


/*-----------------------------------------------------------------*
*
*-----------------------------------------------------------------*/
static void Remove2pcodes(pCode *pcflow, pCode *pc1, pCode *pc2, regs *reg, int can_free)
{
	static int debug_code=99;
	if(!reg)
		return;
#if 0
	fprintf (stderr, "%s:%d(%s): %d (reg:%s)\n", __FILE__, __LINE__, __FUNCTION__, debug_code, reg ? reg->name : "???");
	printpCode (stderr, pc1);
	printpCode (stderr, pc2);
#endif
	
	//fprintf(stderr,"%s\n",__FUNCTION__);
	if(pc1)
		Remove1pcode(pc1, reg, debug_code++);
	
	if(pc2) {
		Remove1pcode(pc2, reg, debug_code++);
		deleteSetItem (&(PCFL(pcflow)->registers), reg);
		
		if(can_free) {
			reg->isFree = 1;
			reg->wasUsed = 0;
		}
		
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
		if (i++ > 1000) {
			fprintf(stderr, "warning, regUsedinRange searched through too many pcodes\n");
			return 0;
		}
		
		pc1 = findNextInstruction(pc1->next);
		
	} while (pc1 && (pc1 != pc2)) ;
	
	return 0;
}

int regIsSpecial (regs *reg, int mayBeGlobal)
{
  if (!reg) return 0;

  if (reg->type == REG_SFR || reg->type == REG_STK || (!mayBeGlobal && (reg->isPublic || reg->isExtern))) return 1;

  return 0;
}

#if 0
static int regIsLocal (regs *reg)
{
	if (!reg) return 1;
	/* temporaries are local */
	if (reg->type == REG_TMP) return 1;
	/* registers named r0x... are local */
	if (reg->name && !strncmp(reg->name,"r0x", 3))
	{
		//fprintf (stderr, "reg %s is not marked REG_TMP...\n", reg->name);
		return 1;
	}

	return 0;
}
#endif

/*-----------------------------------------------------------------*
* void pCodeOptime2pCodes(pCode *pc1, pCode *pc2) 
*
* ADHOC pattern checking 
* Now look for specific sequences that are easy to optimize.
* Many of these sequences are characteristic of the compiler
* (i.e. it'd probably be a waste of time to apply these adhoc
* checks to hand written assembly.)
* 
*
*-----------------------------------------------------------------*/
int pCodeOptime2pCodes(pCode *pc1, pCode *pc2, pCode *pcfl_used, regs *reg, int can_free, int optimize_level)
{
	pCode *pct1, *pct2;
	regs  *reg1, *reg2;
	
	int t = total_registers_saved;

	if (!isPCI(pc1) || !isPCI(pc2)) return 0;
	if (PCI(pc1)->pcflow != PCI(pc2)->pcflow) return 0;
	
	if(pc2->seq < pc1->seq) {
		pct1 = pc2;
		pc2 = pc1;
		pc1 = pct1;
	}

	/* disable this optimization for now -- it's buggy */
	if(pic14_options.disable_df) return 0;
	
	//fprintf(stderr,"pCodeOptime2pCodes\n");
	//pc1->print(stderr,pc1);
	//pc2->print(stderr,pc2);

	if((PCI(pc1)->op == POC_CLRF) && (PCI(pc2)->op == POC_MOVFW) ){
		pCode *newpc;
		int regUsed = 0;
		int wUsed   = 0;
		int wSaved  = 0;
		/*
		clrf  reg    ; pc1
		stuff...
		movf  reg,w  ; pc2
		
		  can be replaced with (only if following instructions are not going to use W and reg is not used again later)
		  
			stuff...
			movlw 0 or clrf  reg
		*/
		DFPRINTF((stderr, "   optimising CLRF reg ... MOVF reg,W to ... MOVLW 0\n"));
		pct2 = findNextInstruction(pc2->next);
		
		if(pct2 && PCI(pct2)->op == POC_MOVWF) {
			wSaved = wUsed = 1; /* Maybe able to replace with clrf pc2->next->reg. */
		} else {
			wUsed = pCodeSearchCondition(pct2,PCC_W,1) != -1;
		}
		regUsed = regUsedinRange(pct2,0,reg);
		if ((regUsed&&wUsed) || (pCodeSearchCondition(pct2,PCC_Z,0) != -1)) {
			/* Do not optimise as exisiting code is required. */
		} else {
			/* Can optimise. */
			if(regUsed) {
				newpc = newpCode(POC_CLRF, PCI(pc1)->pcop);
			} else if(wSaved && !wUsed) {
				newpc = newpCode(POC_CLRF, PCI(pct2)->pcop);
				pct2->destruct(pct2);
			} else {
				newpc = newpCode(POC_MOVLW, newpCodeOpLit(0));
			}
			
			pCodeInsertAfter(pc2, newpc);
			PCI(newpc)->pcflow = PCFL(pcfl_used);
			newpc->seq = pc2->seq;
			
			//fprintf (stderr, "%s:%d(%s): Remove2pcodes (CLRF reg, ..., MOVF reg,W)\n", __FILE__, __LINE__, __FUNCTION__);
			Remove2pcodes(pcfl_used, pc1, pc2, reg, can_free);
			total_registers_saved++;  // debugging stats.
		}
	} else if((PCI(pc1)->op == POC_CLRF) && (PCI(pc2)->op == POC_IORFW) ){
		DFPRINTF((stderr, "   optimising CLRF/IORFW\n"));
		
		pct2 = findNextInstruction(pc2->next);
		
		/* We must ensure that is destroyed before being read---IORLW must be performed unless this is proven. */
		if(pCodeSearchCondition(pct2, PCC_Z,0) != -1) {
			pct2 = newpCode(POC_IORLW, newpCodeOpLit(0));
			pct2->seq = pc2->seq;
			PCI(pct2)->pcflow = PCFL(pcfl_used);
			pCodeInsertAfter(pc1,pct2);
		}
		//fprintf (stderr, "%s:%d(%s): Remove2pcodes (CLRF/IORFW)\n", __FILE__, __LINE__, __FUNCTION__);
		Remove2pcodes(pcfl_used, pc1, pc2, reg, can_free);
		total_registers_saved++;  // debugging stats.
		
	}  else if(PCI(pc1)->op == POC_MOVWF) {
		// Optimising MOVWF reg ...
		
		pct2 = findNextInstruction(pc2->next);
		
		if(PCI(pc2)->op == POC_MOVFW) {
			// Optimising MOVWF reg ... MOVF reg,W
			
			if(PCI(pct2)->op == POC_MOVWF) {
			/*
			Change:
			
			  movwf   reg    ; pc1
			  stuff...
			  movf    reg,w  ; pc2
			  movwf   reg2   ; pct2
			  
				To: ( as long as 'stuff' does not use reg2 or if following instructions do not use W or reg is not used later)
				
				  movwf   reg2
				  stuff...
				  
				*/
				reg2 = getRegFromInstruction(pct2);
				/* Check reg2 is not used for something else before it is loaded with reg */
				if (reg2 && !regIsSpecial (reg2, 1) && !regUsedinRange(pc1,pc2,reg2)) {
					pCode *pct3 = findNextInstruction(pct2->next);
					/* Check following instructions are not relying on the use of W or the Z flag condiction */
					/* XXX: We must ensure that this value is destroyed before use---otherwise it might be used in
					 *      subsequent flows (checking for < 1 is insufficient). */
					if ((pCodeSearchCondition(pct3,PCC_Z,0) == -1) && (pCodeSearchCondition(pct3,PCC_W,0) == -1)) {
						DFPRINTF((stderr, "   optimising MOVF reg ... MOVF reg,W MOVWF reg2 to MOVWF reg2 ...\n"));
						pct2->seq = pc1->seq;
						unlinkpCode(pct2);
						pCodeInsertBefore(pc1,pct2);
						if(regUsedinRange(pct2,0,reg))
						{
							//fprintf (stderr, "%s:%d(%s): Remove2pcodes IF (MOVWF reg, ..., MOVW reg,W  MOVWF reg2)\n", __FILE__, __LINE__, __FUNCTION__);
							Remove2pcodes(pcfl_used, pc2, NULL, reg, can_free);
						} else {
							//fprintf (stderr, "%s:%d(%s): Remove2pcodes ELSE (MOVWF reg, ..., MOVW reg,W  MOVWF reg2)\n", __FILE__, __LINE__, __FUNCTION__);
							Remove2pcodes(pcfl_used, pc1, pc2, reg, can_free);
						}
						total_registers_saved++;  // debugging stats.
						return 1;
					}
				}
			}
		}
		
		pct1 = findPrevInstruction(pc1->prev);
		if(pct1 && (PCI(pct1)->pcflow == PCI(pc1)->pcflow)) {
			
			if ( (PCI(pct1)->op == POC_MOVFW) &&
				(PCI(pc2)->op == POC_MOVFW)) {
				
				reg1 = getRegFromInstruction(pct1);
				if(reg1 && !regIsSpecial (reg1, 0) && !regUsedinRange(pc1,pc2,reg1)) {
					DFPRINTF((stderr, "   optimising MOVF reg1,W MOVWF reg ... MOVF reg,W\n"));
					/*
					Change:
					
						movf   reg1,w
						movwf  reg

						stuff...
						movf   reg,w
						
					To:

						stuff...

						movf   reg1,w

					Or, if we're not deleting the register then the "To" is:

						stuff...

						movf   reg1,w
						movwf  reg
					*/
					pct2 = newpCode(PCI(pc2)->op, PCI(pct1)->pcop);
					pCodeInsertAfter(pc2, pct2);
					PCI(pct2)->pcflow = PCFL(pcfl_used);
					pct2->seq = pc2->seq;
					
					if(can_free) {
						//fprintf (stderr, "%s:%d(%s): Remove2pcodes CANFREE (MOVF reg1,W; MOVWF reg2; MOVF reg2,W)\n", __FILE__, __LINE__, __FUNCTION__);
						Remove2pcodes(pcfl_used, pc1, pc2, reg, can_free);
					} else {
					/* If we're not freeing the register then that means (probably)
						* the register is needed somewhere else.*/
						unlinkpCode(pc1);
						pCodeInsertAfter(pct2, pc1);
						
						//fprintf (stderr, "%s:%d(%s): Remove2pcodes ELSE (MOVF reg1,W; MOVWF reg2; MOVF reg2,W)\n", __FILE__, __LINE__, __FUNCTION__);
						Remove2pcodes(pcfl_used, pc2, NULL, reg, can_free);
					}
					
					//fprintf (stderr, "%s:%d(%s): Remove2pcodes ALWAYS (MOVF reg1,W; MOVWF reg2; MOVF reg2,W)\n", __FILE__, __LINE__, __FUNCTION__);
					Remove2pcodes(pcfl_used, pct1, NULL, reg1, 0);
					total_registers_saved++;  // debugging stats.
					
				}
			}
		}
  }

  return (total_registers_saved != t);
}

/*-----------------------------------------------------------------*
* void pCodeRegOptimeRegUsage(pBlock *pb) 
*-----------------------------------------------------------------*/
void OptimizeRegUsage(set *fregs, int optimize_multi_uses, int optimize_level)
{
	regs *reg;
	int used;
	pCode *pc1=NULL, *pc2=NULL;
	
	
	while(fregs) {
		pCode *pcfl_used, *pcfl_assigned;
		
		/* Step through the set by directly accessing the 'next' pointer.
		* We could also step through by using the set API, but the 
		* the (debug) calls to print instructions affect the state
		* of the set pointers */
		
		reg = fregs->item;
		fregs = fregs->next;
		/*
		if (strcmp(reg->name,"_SubState")==0)
		fprintf(stderr,"Reg: %s\n",reg->name);
		*/
		
		/* Catch inconsistently set-up live ranges
		 * (see tracker items #1469504 + #1474602)
		 * FIXME: Actually we should rather fix the
		 * computation of the liveranges instead...
		 */
		if (!reg || !reg->reglives.usedpFlows
			|| !reg->reglives.assignedpFlows)
		{
		  //fprintf( stderr, "skipping reg w/o liveranges: %s\n", reg ? reg->name : "(unknown)");
		  continue;
		}

		if(reg->type == REG_SFR || reg->type == REG_STK || reg->isPublic || reg->isExtern|| reg->isFixed) {
			//fprintf(stderr,"skipping SFR: %s\n",reg->name);
			continue;
		}

		pcfl_used = setFirstItem(reg->reglives.usedpFlows);
		pcfl_assigned = setFirstItem(reg->reglives.assignedpFlows);
		
		used = elementsInSet(reg->reglives.usedpCodes);
		if(used == 2) { 
			/*
			In this section, all registers that are used in only in two 
			instructions are examined. If possible, they're optimized out.
			*/
			
			/*
			fprintf (stderr, "OptimizeRegUsage: %s  addr=0x%03x rIdx=0x%03x type=%d used=%d\n",
			reg->name,
			reg->address,
			reg->rIdx, reg->type, used);
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
					
					pCodeOptime2pCodes(pc1, pc2, pcfl_used, reg, 1,optimize_level);
					
				} else {
					// fprintf(stderr, "  and used in different flows\n");
					
				}
				
			} else if(pcfl_used) {
				
				/* register has been used twice without ever being assigned */
				fprintf(stderr,"WARNING %s: reg %s used without being assigned\n",__FUNCTION__,reg->name);
				
			} else {
				//fprintf(stderr,"WARNING %s.1: reg %s assigned without being used\n",__FUNCTION__,reg->name);
				Remove2pcodes(pcfl_assigned, pc1, pc2, reg, 1);
				total_registers_saved++;  // debugging stats.
			}
		} else {
			
			/* register has been used either once, or more than twice */
			
			if(used && !pcfl_used && pcfl_assigned) {
				pCode *pc;
				
				//fprintf(stderr,"WARNING %s.2: reg %s assigned without being used\n",__FUNCTION__,reg->name);
				
				pc = setFirstItem(reg->reglives.usedpCodes);
				while(pc) {
					
					pcfl_assigned = PCODE(PCI(pc)->pcflow);
					Remove1pcode(pc, reg,2);
					
					deleteSetItem (&(PCFL(pcfl_assigned)->registers), reg);
					/*
					deleteSetItem (&(reg->reglives.usedpCodes),pc);
					pc->destruct(pc);
					*/
					pc = setNextItem(reg->reglives.usedpCodes);
				}
				
				
				reg->isFree = 1;
				reg->wasUsed = 0;
				
				total_registers_saved++;  // debugging stats.
			} else if( (used > 2) && optimize_multi_uses) {
				
				set *rset1=NULL;
				set *rset2=NULL;
				int searching=1;
				
				pCodeFlow *pcfl1=NULL, *pcfl2=NULL;
				
				/* examine the number of times this register is used */
				
				
				rset1 = reg->reglives.usedpCodes;
				while(rset1 && searching) {
					
					pc1 = rset1->item;
					rset2 = rset1->next;
					
					if(pc1 && isPCI(pc1) &&  ( (pcfl1 = PCI(pc1)->pcflow) != NULL) ) {
						
						//while(rset2 && searching) {
						if(rset2) {
							
							pc2 = rset2->item;
							if(pc2 && isPCI(pc2)  &&  ( (pcfl2 = PCI(pc2)->pcflow) != NULL) )  {
								if(pcfl2 == pcfl1) {
									
									if(pCodeOptime2pCodes(pc1, pc2, pcfl_used, reg, 0,optimize_level))
										searching = 0;
								}
							}
							
							//rset2 = rset2->next;
							
						}
					}
					rset1 = rset1->next;
				}
			}
		}
	}
}

#if 0

/* The following routines implement pCode optimizations based on
 * dataflow analysis. The effects should be similar to those
 * of pCodeOptime2pCodes() but more powerful.
 *
 * Unfortunately, the current approach (comparing operands by
 * their associated registers) is seriously flawed:
 * Some pCodeOps do not provide access to their repsective regs
 * (PO_DIRs are created from arbitrary strings, immediates need
 * to take offset and index into account, ...)
 *
 * This has to be rewritten based on pCodeOps instead of regs...
 */

/* ------------------------------------------------------------------
   Returns TRUE iff reg1 and reg2 are the same registers.
   ------------------------------------------------------------------ */

static int sameRegs (const regs *reg1, const regs *reg2)
{
	if (reg1 == reg2) return 1;
	if (!reg1 || !reg2) return 0;
	assert (reg1->name && reg2->name);

	/* Compare names, as rIdx is not unique... */
	if (!strcmp(reg1->name, reg2->name)) return 1;

	/* Name mismatch -- not the same register. */
	return 0;
}

/* ------------------------------------------------------------------
   Returns 1 if the register is live at pc (read in this flow),
   returns -1 if the register is NOT live at pc (assigned a new value
   prior to readingi the old value in this flow) or
   return 0 if the register is not mentioned.
   ------------------------------------------------------------------ */

static int checkRegInFlow (regs **reg, int *cond, pCode *pc)
{
	const int verbose = 0;
	/* find next PCI at or after pc */
	while (pc && isPCFL(pc)) pc = pc->next;

	assert (reg && cond);

	/* remove pseudo flags from cond */
	*cond &= ~(PCC_REGISTER | PCC_EXAMINE_PCOP);

	if (verbose && pc)
	{
		fprintf (stderr, "Checking for reg %s, cond %x on pc ", *reg ? (*reg)->name : "<none>", *cond);	pc->print (stderr, pc);
	}
	
	while (pc && !isPCFL(pc))
	{
		if (verbose)
		{
			fprintf (stderr, "        now checking for reg %s, cond %x on pc ", *reg ? (*reg)->name : "<none>", *cond);
			pc->print (stderr, pc);
		}
		if (isPCI(pc))
		{
			pCode *pcprev;
			regs *op = NULL;
			int prevIsSkip = 0;
			
			pcprev = findPrevInstruction (pc);
			if (pcprev && isPCI_SKIP(pcprev))
				prevIsSkip = 1;

			if ((PCI(pc)->inCond | PCI(pc)->outCond) & PCC_REGISTER)
			{
				op = getRegFromInstruction (pc);

				/* pCodeOpRegBits do not provide register information... */
				//if (!op) { __asm__("int3"); pc->print (stderr, pc); }
				//assert (op);
				if (!op) 
				{
					if (reg)
						return 1; /* assume `reg' is alive */
				}

				/* SPECIAL CASE: jump to unknown destination -- assume everything alive */
				if (op->type == PO_PCL)
				{
					return 1;
				}
			} // if

			if (PCI(pc)->inCond & PCC_REGISTER)
			{
				/* `op' is live (possibly read) */
				if (*reg && sameRegs (op, *reg))
				{
					if (verbose)
					{
						fprintf (stderr, "reg is read in pc ");
						pc->print (stderr, pc);
					}
					return 1;
				}
			}

			/* handle additional implicit operands */
			switch (PCI(pc)->op)
			{
			case POC_CALL: /* read arguments from WREG, clobbers WREG */
				if (*cond & PCC_W)
				{
					if (verbose)
					{
						fprintf (stderr, "conditions are read at pc ");
						pc->print (stderr, pc);
					}
					return 1;
				}
				*cond &= ~PCC_W;
				break;
			case POC_RETURN: /* returns WREG to caller */
				//fprintf (stderr, "reached RETURN, reg %s, cond %x\n", *reg ? (*reg)->name : "<none>", *cond);
				if (*cond & PCC_W)
				{
					if (verbose)
					{
						fprintf (stderr, "conditions are read at pc ");
						pc->print (stderr, pc);
					}
					return 1;
				}
				/* afterwards, no condition is alive */
				*cond = 0;
				/* afterwards, all local registers are dead */
				if (*reg && regIsLocal (*reg)) *reg = NULL;
				break;
			case POC_RETLW:
			case POC_RETFIE: /* this does not erturn WREG to the "caller", thus its rather a RETLW */
				/* this discards WREG */
				*cond &= ~PCC_W;
				/* afterwards, no condition is alive */
				*cond = 0;
				/* afterwards, all local registers are dead */
				if (*reg && regIsLocal (*reg)) *reg = NULL;
				break;
			default:
				/* no implicit arguments */
				break;
			}

			if (PCI(pc)->inCond & (*cond))
			{
				/* a condition is read */
				if (verbose)
				{
					fprintf (stderr, "conditions are read at pc ");
					pc->print (stderr, pc);
				}
				return 1;
			}

			/* remove outConds from `cond' */
			(*cond) &= ~(PCI(pc)->outCond);

			if (PCI(pc)->outCond & PCC_REGISTER)
			{
				/* `op' is (possibly) discarded */
				if (*reg && !prevIsSkip && sameRegs (op, *reg))
				{
					if (verbose)
					{
						fprintf (stderr, "reg is assigned (cont'd) at pc ");
						pc->print (stderr, pc);
					}
					*reg = NULL;
				}
			}

			/* have all interesting conditions/registers been discarded? */
			if (!(*reg) && !(*cond))
			{
				if (verbose)
				{
					fprintf (stderr, "reg and conditions are discarded after ");
					pc->print (stderr, pc);
				}
				return -1;
			}
		} // if

		pc = pc->next;
	} // while

	/* no use of (or only conditional writes to) `reg' found in flow */
	if (verbose)
	{
		fprintf (stderr, "analysis inconclusive: reg %s, cond %x remains\n", *reg ? "remains" : "is void", *cond);
	}
	return 0;
}

/* ------------------------------------------------------------------
   This will return 1 only if
   - reg is NULL or of type REG_TMP
   - reg is NULL or its value is discarded after pc
   - cond is 0 or all conditions are overwritten before being used
   ------------------------------------------------------------------ */

typedef struct {
  pCode *pc;
  regs *reg;
  int cond;
} df_state_t;

df_state_t *df_free_states = NULL;

static df_state_t *
df_newState (regs *reg, int cond, pCode *pc)
{
	df_state_t *state;

	if (df_free_states) {
	  state = df_free_states;
	  df_free_states = (df_state_t *)df_free_states->pc;
	} else {
	  state = Safe_calloc(1, sizeof(df_state_t));
	}
	state->pc = pc;
	state->reg = reg;
	state->cond = cond;
	return state;
}

static int
df_containsState (set *set, df_state_t *state)
{
	/* scan set for presence of `state' */
	df_state_t *curr;
	for (curr = setFirstItem (set); curr; curr = setNextItem (set))
	{
		if ((curr->pc == state->pc)
			&& (curr->reg == state->reg)
			&& (curr->cond == state->cond))
		{
			/* `state' found */
			return 1;
		}
	}
	return 0;
}

static void
df_releaseState (df_state_t *state)
{
	state->pc = (pCode *)df_free_states;
	df_free_states = (df_state_t *)state;
	state->reg = NULL;
	state->cond = 0;
}

static void
df_removeStates ()
{
	df_state_t *next;
	while (df_free_states)
	{
		next = (df_state_t *)df_free_states->pc;
		Safe_free(df_free_states);
		df_free_states = next;
	} // while
}

int regIsDead (regs *reg, int cond, pCode *pc)
{
	set *seenStates = NULL;
	set *todo = NULL;
	pCode *curr;
	df_state_t *state;
	int result = 1;
	
	/* sanity checks */
	if (reg && !regIsLocal (reg)) return 0;

	pc = findNextInstruction (pc->next);

	addSet (&todo, df_newState(reg, cond, pc));

	while ((result == 1) && (state = getSet (&todo)))
	{
		int res;
		
		if (df_containsState (seenStates, state)) continue;
		addSet (&seenStates, state);

		curr = state->pc;
		reg = state->reg;
		cond = state->cond;
		
		//fprintf (stderr, "Checking for reg %s/cond %x at pc ", reg ? reg->name : "<none>", cond);
		//curr->print (stderr, curr);

		res = checkRegInFlow (&reg, &cond, curr);
		switch (res)
		{
		case 1: /* `reg' or `cond' is read---not dead */
			result = 0;
			break;
		case -1: /* `reg' and `cond' are discarded in this flow---need to check other open flows */
			break;
		default: /* `reg' is not read and (possibly) not assigned---check successor flows */
			if (curr)
			{
				pCodeFlow *pcfl = PCI(curr)->pcflow;
				pCodeFlowLink *link;
				pCode *first;
				assert (pcfl);

				for (link = setFirstItem(pcfl->to); link; link = setNextItem (pcfl->to))
				{
					/* add the first pCodeInstruction in the new flow to `todo' */
					first = findNextInstruction (&link->pcflow->pc);
					if (first) addSet (&todo, df_newState (reg, cond, first));
				} // for
			}
			break;
		} // switch
	} // while

	/* clean up */
	while (NULL != (state = getSet (&todo))) { df_releaseState (state); }
	while (NULL != (state = getSet (&seenStates))) { df_releaseState (state); }

	/* if result remained 1, `reg' is not even possibly read and thus dead */
	return result;
}



/* ------------------------------------------------------------------
   Safely remove a pCode.
   This needs to check that the previous instruction was no SKIP
   (otherwise the SKIP instruction would have to be removed as well,
   which may not be done for SFRs (side-effects on read possible)).
   ------------------------------------------------------------------ */

extern void unBuildFlow (pBlock *pb);
extern void RegsUnMapLiveRanges ();
extern void BuildFlow (pBlock *pb);
extern void LinkFlow (pBlock *pb);
extern void BuildFlowTree (pBlock *pb);
extern void pCodeRegMapLiveRanges (pBlock *pb);
extern pFile *the_pFile;

static int pCodeRemove (pCode *pc, const char *comment)
{
	pCode *pcprev, *pcnext;
	unsigned int result = 0;
	
	/* Sanity checks. */
	if (!pc) return 0;
	if (!isPCI(pc)) return 0;

	pcprev = findPrevInstruction (pc->prev);
	if (pcprev && isPCI_SKIP(pcprev))
	{
		/* bail out until we know how to fix the Flow... */
		//return 0;

		/* we also need to remove the preceeding SKIP instruction(s) */
		result = pCodeRemove (pcprev, "=DF= removing preceeding SKIP as well");
		if (!result)
		{
			/* previous instruction could not be removed -- this cannot be removed as well */
			return result;
		}
		/* FIXME: We now have to update the flow! */
	} // if

	/* do not remove pCodes with SFRs as operands (even reading them might cause side effects) */
	{
		regs *reg;
		reg = getRegFromInstruction (pc);
		/* accesses to the STATUS register aer always safe, but others... */
		if (reg && reg->type == REG_SFR && reg->pc_type != PO_STATUS) return result;
	}

	/* MUST SUCEED FROM NOW ON (or revert the changes done since NOW ;-)) */
	
	/* fix flow */
	if (PCI(pc)->pcflow && PCI(pc)->pcflow->end == pc)
	{
		pCode *pcprev;
		pcprev = findPrevInstruction (pc->prev);
		if (PCI(pcprev)->pcflow == PCI(pc)->pcflow)
		{
			PCI(pc)->pcflow->end = pcprev;
		} else {
			pBlock *pb = pc->pb;
			RegsUnMapLiveRanges();
			unBuildFlow(pb);
			BuildFlow(pb);
			LinkFlow(pb);
			BuildFlowTree(pb);
			for (pb = the_pFile->pbHead; pb; pb = pb->next)
			{
				pCodeRegMapLiveRanges(pb);
			}
		}
	}
	
	/* attach labels to next instruction */
	pcnext = findNextInstruction (pc->next);
	if (pcnext)
	{
		PCI(pcnext)->label = pBranchAppend (PCI(pcnext)->label, PCI(pc)->label);
		if (!PCI(pcnext)->cline) PCI(pcnext)->cline = PCI(pc)->cline;
	}
	else
	{
		fprintf (stderr, "Cannot move a label...\n");
		exit(-1);
	}
	
	if (comment)
	{
		char buffer[512];
		int size = 512;
		char *pbuff = &buffer[0];
		
		SAFE_snprintf (&pbuff, &size, "; %s:%u(%s): %s", __FILE__, __LINE__, __FUNCTION__, comment);
		pCodeInsertAfter(pc->prev, newpCodeCharP (&buffer[0]));
	} // if

	if (1)
	{
		/* replace removed pCode with out-commented version of itself */
		char buffer[512];
		int size = 512;
		char *pbuff = &buffer[0];
		
		SAFE_snprintf (&pbuff, &size, "; %s:%u(%s): ", __FILE__, __LINE__, __FUNCTION__);
		pCode2str (pbuff, size, pc);
		pCodeInsertAfter(pc->prev, newpCodeCharP (&buffer[0]));
	}

	pc->destruct (pc);
	return result+1;
}

/* ------------------------------------------------------------------
   Find and remove dead pCodes.
   ------------------------------------------------------------------ */

static int removeIfDeadPCI (pCode *pc)
{
	pCode *pcnext = NULL;
	pCode *curr;
	unsigned int outCond;
	unsigned int result = 0;
	regs *dst;
	
	if (!pc) return 0;
	dst = NULL;
	
	/* skip non-PCIs */
	pcnext = findNextInstruction (pc->next);
	if (!isPCI(pc)) return 0;

	switch (PCI(pc)->op)
	{
	case POC_ADDLW:
	case POC_ADDWF:
	case POC_ADDFW:
	case POC_ANDLW:
	case POC_ANDWF:
	case POC_ANDFW:
	case POC_BCF:
	case POC_BSF:
	//case POC_BTFSC:
	//case POC_BTFSS:
	//case POC_CALL:
	case POC_COMF:
	case POC_COMFW:
	case POC_CLRF:
	case POC_CLRW:
	//case POC_CLRWDT:
	case POC_DECF:
	case POC_DECFW:
	//case POC_DECFSZ:
	//case POC_DECFSZW:
	//case POC_GOTO:
	case POC_INCF:
	case POC_INCFW:
	//case POC_INCFSZ:
	//case POC_INCFSZW:
	case POC_IORLW:
	case POC_IORWF:
	case POC_IORFW:
	case POC_MOVF:
	case POC_MOVFW:
	case POC_MOVLW:
	case POC_MOVWF:
	case POC_NOP:
	//case POC_RETLW:
	//case POC_RETURN:
	//case POC_RETFIE:
	case POC_RLF:
	case POC_RLFW:
	case POC_RRF:
	case POC_RRFW:
	case POC_SUBLW:
	case POC_SUBWF:
	case POC_SUBFW:
	case POC_SWAPF:
	case POC_SWAPFW:
	//case POC_TRIS:
	case POC_XORLW:
	case POC_XORWF:
	case POC_XORFW:
	//case POC_BANKSEL:
	//case POC_PAGESEL:
		break;
	
	default:
		/* do not remove unknown PCIs */
		return 0;
	} // switch

	/* redundant checks---those instructions may never be removed */
	if (isPCI_BRANCH(pc)) return 0;
	if (isPCI_SKIP(pc)) return 0;
	
	outCond = PCI(pc)->outCond;
	curr = pcnext;

	/* unknown operands assigned to, then ignore */
	if ((outCond & (PCC_REGISTER | PCC_C | PCC_Z | PCC_DC | PCC_W)) != outCond)
		return 0;
	
	if (outCond & PCC_REGISTER)
	{
		dst = getRegFromInstruction (pc);
		if (!dst) return 0;

		/* special reg? */
		if (!regIsLocal (dst)) return 0;
		if (regIsSpecial (dst,0)) return 0;
	}
	
	//fprintf (stderr, "--> checking for liveness pc "); pc->print (stderr, pc);
	if (regIsDead (dst, outCond, pc))
	{
		/* no result from pc has been used -- pc is `dead' */
		//fprintf (stderr, "--> reg %s and cond %x assumed unused\n", dst ? dst->name : "<none>", outCond);
		//fprintf (stderr, "--> removing dead pc "); pc->print (stderr, pc);
		result = pCodeRemove (pc, "=DF= removed dead pCode");
	}
	
	return result;
}

/* ------------------------------------------------------------------
   This routine is intended to safely replace one pCodeInstruction
   with a different pCodeInstruction.
   If desired, the replaced pCode will be left in (commented out) for
   debugging.
   Further, an optional comment can be inserted to indicate the
   reason for the possible removal of the pCode.
   ------------------------------------------------------------------ */

static void replace_PCI (pCodeInstruction *pc, pCodeInstruction *newpc, char *comment)
{
  newpc->from =  pBranchAppend (newpc->from, pc->from);
  newpc->to = pBranchAppend (newpc->to, pc->to);
  newpc->label = pBranchAppend (newpc->label, pc->label);
  //newpc->pcflow = pc->pcflow; // updated in pCodeInsertAfter, ->pb is as well
  newpc->cline = pc->cline;

  newpc->pc.seq = pc->pc.seq;

  pCodeInsertAfter (&pc->pc, &newpc->pc);

  /* FIXME: replacing pc will break the liverange maps (usedpCodes, ...) */
  pCodeRegMapLiveRanges( pc->pBlock ); /*FIXME:UNTESTED*/
  
  if (comment)
  {
    pCodeInsertAfter (&pc->pc, newpCodeCharP (comment));
  } // if

  if (1)
  {
    /* replace pc with commented out version of itself */
    char buffer[1024], buffer2[1024];
    char *pbuff = &buffer[0];
    int size=1024;
    pCode2str (&buffer2[0],1024,&pc->pc);
    SAFE_snprintf (&pbuff,&size,"%s:%u(%s): removed pCode was %s\t", __FILE__, __LINE__, __FUNCTION__, &buffer2[0]);
    pCodeInsertAfter (&pc->pc, newpCodeCharP (&buffer[0]));
  } // if

  pc->pc.destruct (&pc->pc);
}

/* ------------------------------------------------------------------
   Find the first (unique) assignment to `reg' (prior to pc).
   ------------------------------------------------------------------ */

pCode *findAssignmentToReg (regs *reg, pCode *pc)
{
	pCode *curr;
	
	assert (pc && isPCI(pc) && reg);

	curr = findPrevInstruction (pc->prev);
	
	while (curr && PCI(curr)->pcflow == PCI(pc)->pcflow)
	{
		if (PCI(curr)->outCond & PCC_REGISTER)
		{
			regs *op = getRegFromInstruction (curr);
			if (op && (sameRegs(op,reg)))
				return curr;
		} // if
		curr = findPrevInstruction (curr->prev);
	} // while
	
	/* no assignment to reg found */
	return NULL;
}

/* ------------------------------------------------------------------
   Find a register that holds the same value as `reg' (an alias).
   ------------------------------------------------------------------ */

regs *findRegisterAlias (regs *reg, pCode *pc)
{
	pCode *curr;

	if(!reg) return NULL;

	if (regIsSpecial (reg, 0)) return NULL;

	curr = findAssignmentToReg (reg, pc);

	/* no assignment found --> no alias found */
	if (!curr) return NULL;

	switch (PCI(curr)->op)
	{
	case POC_MOVWF:
		/* find previous assignment to WREG */
		while (curr && !(PCI(curr)->outCond & PCC_W))
			curr = findPrevInstruction (curr->prev);
		if (curr && PCI(curr)->op == POC_MOVFW)
		{
			regs *op = getRegFromInstruction (curr);
			/* alias must not be changed since assignment... */
			if (PCI(curr)->pcop)
			{
				switch (PCI(curr)->pcop->type)
				{
				case PO_GPR_REGISTER:
				case PO_GPR_TEMP:
					/* these operands are ok */
					break;
				default:
					/* not a plain register operand */
					return NULL;
					break;
				}
			}
			if (!op || regIsSpecial (op, 0) || !regIsUnchangedSince (op, pc, curr)) return NULL;
			//fprintf (stderr, "found register alias for %s: %s\n", reg->name, op && op->name ? op->name : "<no name>");
			return op;
		} else {
			/* unknown source to WREG -- unknown register alias */
			return NULL;
		}
		break;
	
	default:
		/* unhandled instruction -- assume unknown source, no alias */
		return NULL;
	}

	/* no alias found */
	return NULL;
}

/* ------------------------------------------------------------------
   Analyze a single pCodeInstruction's dataflow relations and replace
   it with a better variant if possible.
   ------------------------------------------------------------------ */

void analyzeAndReplacePCI (pCodeInstruction *pci)
{
	regs *op_reg, *alias_reg;
	
	assert (pci);

	if (!isPCI(pci)) return;
	
	switch (pci->op)
	{
	case POC_MOVFW:
	case POC_ADDFW:
	case POC_ANDFW:
	case POC_IORFW:
	case POC_XORFW:
		/* try to find a different source register */
		op_reg = getRegFromInstruction (&pci->pc);
		if (pci->op == POC_MOVFW) /* touches Z */
		{
			pCode *assignment = findAssignmentToReg (op_reg, &pci->pc);
			if (assignment && isPCI(assignment) && (PCI(assignment)->op == POC_CLRF))
			{
				replace_PCI (pci, PCI(newpCode(POC_MOVLW, newpCodeOpLit(0))), "replaced with CLRF");
				return;
			}			
		}
		
		alias_reg = findRegisterAlias (op_reg, &pci->pc);
		if (alias_reg)
		{
			replace_PCI (pci, PCI(newpCode(pci->op, newpCodeOpRegFromStr (alias_reg->name))), "=DF= replaced with move from register alias");
		}
		break;

	default:
		/* do not optimize */
		break;
	} // switch
}

extern pFile *the_pFile;

/* ------------------------------------------------------------------
   Find and remove dead pCodes.
   ------------------------------------------------------------------ */

static int removeDeadPCIs (void)
{
	pBlock *pb;
	unsigned int removed = 0;
	
	if (!the_pFile) return removed;
	
	do {
		removed = 0;
		
		/* iterate over all pBlocks */
		for (pb = the_pFile->pbHead; pb; pb = pb->next)
		{
			pCode *pc, *pcnext = NULL;
			
			/* iterate over all pCodes */
			for (pc = findNextInstruction (pb->pcHead); pc; pc = pcnext)
			{
				pcnext = findNextInstruction (pc->next);
				removed += removeIfDeadPCI (pc);
			} // while
		} // for pb

		fprintf (stderr, "[removed %u dead pCodes]\n", removed);
	} while (removed);
	
	return 0;
}

/* ------------------------------------------------------------------
   This routine tries to optimize the dataflow by...
   (1) 
   
   This routine leaves in dead pCodes (assignments whose results are
   not used) -- these should be removed in a following sweep phase.
   ------------------------------------------------------------------ */

void optimizeDataflow (void)
{
	pBlock *pb;
	
	if (!the_pFile) return;

	//fprintf (stderr, "%s:%u(%s): Starting optimization...\n", __FILE__, __LINE__, __FUNCTION__);
	
	/* iterate over all pBlocks */
	for (pb = the_pFile->pbHead; pb; pb = pb->next)
	{
		pCode *pc, *pcnext = NULL;
		
		/* iterate over all pCodes */
		for (pc = findNextInstruction (pb->pcHead); pc; pc = pcnext)
		{
			pcnext = findNextInstruction (pc->next);
			analyzeAndReplacePCI (PCI(pc));
		} // while
	} // for pb

	while (removeDeadPCIs ()) { /* remove dead codes in multiple passes */};
	df_removeStates ();
}

#endif

/*-----------------------------------------------------------------*
* void pCodeRegOptimeRegUsage(pBlock *pb) 
*-----------------------------------------------------------------*/
void pCodeRegOptimizeRegUsage(int level)
{
	
	int passes;
	int saved = 0;
	int t = total_registers_saved;

#if 0
	/* This is currently broken (need rewrite to correctly
	 * handle arbitrary pCodeOps instead of registers only). */
	if (!pic14_options.disable_df)
		optimizeDataflow ();
#endif

	if(!register_optimization)
		return;
#define OPT_PASSES 4
	passes = OPT_PASSES;
	
	do {
		saved = total_registers_saved;
		
		/* Identify registers used in one flow sequence */
		OptimizeRegUsage(dynAllocRegs,level, (OPT_PASSES-passes));
		OptimizeRegUsage(dynStackRegs,level, (OPT_PASSES-passes));
		OptimizeRegUsage(dynDirectRegs,0, (OPT_PASSES-passes));
		
		if(total_registers_saved != saved)
			DFPRINTF((stderr, " *** pass %d, Saved %d registers, total saved %d ***\n", 
			(1+OPT_PASSES-passes),total_registers_saved-saved,total_registers_saved));
		
		passes--;
		
	} while( passes && ((total_registers_saved != saved) || (passes==OPT_PASSES-1)) );
	
	if(total_registers_saved == t) 
		DFPRINTF((stderr, "No registers saved on this pass\n"));


	/*
		fprintf(stderr,"dynamically allocated regs:\n");
		dbg_regusage(dynAllocRegs);
		fprintf(stderr,"stack regs:\n");
		dbg_regusage(dynStackRegs);
		fprintf(stderr,"direct regs:\n");
		dbg_regusage(dynDirectRegs);
	*/
}


/*-----------------------------------------------------------------*
* void RegsUnMapLiveRanges(set *regset)
*
*-----------------------------------------------------------------*/
void  RegsSetUnMapLiveRanges(set *regset)
{
	regs *reg;
	
	while(regset) {
		reg = regset->item;
		regset = regset->next;
		
		
		deleteSet(&reg->reglives.usedpCodes);
		deleteSet(&reg->reglives.usedpFlows);
		deleteSet(&reg->reglives.assignedpFlows);
		
	}
	
}

void  RegsUnMapLiveRanges(void)
{
	
	RegsSetUnMapLiveRanges(dynAllocRegs);
	RegsSetUnMapLiveRanges(dynStackRegs);
	RegsSetUnMapLiveRanges(dynDirectRegs);
	RegsSetUnMapLiveRanges(dynProcessorRegs);
	RegsSetUnMapLiveRanges(dynDirectBitRegs);
	RegsSetUnMapLiveRanges(dynInternalRegs);
	
}
