/*-------------------------------------------------------------------------

   pcodeflow.c - post code generation flow analysis

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
  pcodeflow.c

  The purpose of the code in this file is to analyze the flow of the
  pCode.

*/

#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"
#include "ralloc.h"
#include "device.h"
#include "pcode.h"
#include "pcoderegs.h"
#include "pcodeflow.h"

#if 0

/* 
   In the section that follows, an exhaustive flow "tree" is built.
   It's not a tree that's really built, but instead every possible
   flow path is constructed. 

   This is really overkill...
*/

static set *FlowTree=NULL;

void dbg_dumpFlowTree(set *FlowTree)
{
  set *segment;
  pCodeFlow *pcflow;

  fprintf(stderr,"Flow Tree: \n");

  for(segment = setFirstItem(FlowTree); segment; segment=setNextItem(FlowTree)) {

    fprintf(stderr,"Segment:\n");

    for(pcflow=PCFL(setFirstItem(segment)); pcflow; pcflow=PCFL(setNextItem(segment))) {
      fprintf(stderr, "  0x%03x",pcflow->pc.seq);
    }
    fprintf(stderr,"\n");
  }

}




/*-----------------------------------------------------------------*
 * void BuildFlowSegment(set *segment, pCodeFlow *pcflow)
 *-----------------------------------------------------------------*/
void BuildFlowSegment(set *segment, pCodeFlow *pcflow)
{

  int nNextFlow=0;
  pCodeFlowLink *pcflowLink=NULL;

  /* Add this flow to the set if it's not in there already */
  if(isinSet(segment, pcflow)) {
    addSetHead(&FlowTree, segment);
    return;
  }

  addSetHead(&segment, pcflow);

  /* Continue to add contiguous flows */

  while( pcflow->to && ((nNextFlow = elementsInSet(pcflow->to)) == 1)) {
    pcflowLink = (pCodeFlowLink *)(setFirstItem(pcflow->to));
    pcflow = pcflowLink->pcflow;

    if(isinSet(segment, pcflow)) {
      addSetIfnotP(&FlowTree, segment);
      return;
    }

    addSetIfnotP(&segment, pcflow);

  }

  /* Branch: for each branch, duplicate the set and recursively call */
  if(pcflow->to && nNextFlow>=2) {
    pCodeFlow *pcflow_to;

    set *branch_segment=NULL;
    
    pcflowLink = (pCodeFlowLink *)(setFirstItem(pcflow->to));
    pcflow_to = pcflowLink->pcflow;

    addSetIfnotP(&segment, pcflow);

    pcflowLink = (pCodeFlowLink *)(setNextItem(pcflow->to));

    while(pcflowLink) {

      branch_segment = setFromSet(segment);
      BuildFlowSegment(setFromSet(segment),pcflowLink->pcflow);
      pcflowLink = (pCodeFlowLink *)(setNextItem(pcflow->to));
    }

    /* add the first branch to this segment */
    BuildFlowSegment(segment,pcflow_to);

  }

  addSetIfnotP(&FlowTree, segment);

  /* done */
}

/*-----------------------------------------------------------------*
 * void BuildFlowTree(pBlock *pb)
 *-----------------------------------------------------------------*/
void BuildFlowTree(pBlock *pb)
{
  pCodeFlow *pcflow;
  set *segment;

  FlowTree = newSet();

  /* Start with the first flow object of this pBlock */

  pcflow = PCFL(findNextpCode(pb->pcHead, PC_FLOW));

  segment = newSet();
  BuildFlowSegment(segment, pcflow);

  pb->FlowTree = FlowTree;

  dbg_dumpFlowTree(FlowTree);
}
#endif

void LinkFlow(pBlock *pb)
{
  pCode *pc=NULL;
  pCode *pcflow;
  pCode *pct;

  //fprintf(stderr,"linkflow \n");

  for( pcflow = findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = findNextpCode(pcflow->next, PC_FLOW) ) {

    if(!isPCFL(pcflow))
      fprintf(stderr, "LinkFlow - pcflow is not a flow object ");


  }
}
