/*-------------------------------------------------------------------------

   pcode.h - post code generation
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

#include "pcode.h"

// Eventually this will go into device depended files:
pCodeOp pc_status    = {PO_STATUS,  "status"};
pCodeOp pc_indf      = {PO_INDF,    "indf"};
pCodeOp pc_fsr       = {PO_FSR,     "fsr"};


//static char *PIC_mnemonics[] = {
static char *scpADDLW = "ADDLW";
static char *scpADDWF = "ADDWF";
static char *scpANDLW = "ANDLW";
static char *scpANDWF = "ANDWF";
static char *scpBCF = "BCF";
static char *scpBSF = "BSF";
static char *scpBTFSC = "BTFSC";
static char *scpBTFSS = "BTFSS";
static char *scpCALL = "CALL";
static char *scpCOMF = "COMF";
static char *scpCLRF = "CLRF";
static char *scpCLRW = "CLRW";
static char *scpDECF = "DECF";
static char *scpDECFSZ = "DECFSZ";
static char *scpGOTO = "GOTO";
static char *scpINCF = "INCF";
static char *scpINCFSZ = "INCFSZ";
static char *scpIORLW = "IORLW";
static char *scpIORWF = "IORWF";
static char *scpMOVF = "MOVF";
static char *scpMOVLW = "MOVLW";
static char *scpMOVWF = "MOVWF";
static char *scpNEGF = "NEGF";
static char *scpRETLW = "RETLW";
static char *scpRETURN = "RETURN";
static char *scpSUBLW = "SUBLW";
static char *scpSUBWF = "SUBWF";
static char *scpTRIS = "TRIS";
static char *scpXORLW = "XORLW";
static char *scpXORWF = "XORWF";


static pFile *the_pFile = NULL;

/****************************************************************/
/****************************************************************/
static pBlock *peepSnippets=NULL;

/****************************************************************/
/*                      Forward declarations                    */
/****************************************************************/

static void unlink(pCode *pc);
static void genericAnalyze(pCode *pc);
static void AnalyzeGOTO(pCode *pc);
static void AnalyzeSKIP(pCode *pc);
static void AnalyzeRETURN(pCode *pc);

static void genericDestruct(pCode *pc);
static void genericPrint(FILE *of,pCode *pc);

static void pCodePrintLabel(FILE *of, pCode *pc);
static void pCodePrintFunction(FILE *of, pCode *pc);
static char *get_op( pCodeInstruction *pcc);

void copypCode(FILE *of, char dbName)
{
  pBlock *pb;

  if(!of || !the_pFile)
    return;

  fprintf(of,";dumping pcode to a file");

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->cmemmap->dbName == dbName)
      printpBlock(of,pb);
  }

}
void pcode_test(void)
{

  printf("pcode is alive!\n");

  if(the_pFile) {

    pBlock *pb;
    FILE *pFile;
    char buffer[100];

    /* create the file name */
    strcpy(buffer,srcFileName);
    strcat(buffer,".p");

    if( !(pFile = fopen(buffer, "w" ))) {
      werror(E_FILE_OPEN_ERR,buffer);
      exit(1);
    }

    fprintf(pFile,"pcode dump\n\n");

    for(pb = the_pFile->pbHead; pb; pb = pb->next) {
      fprintf(pFile,"\n\tNew pBlock\n\n");
      fprintf(pFile,"%s, dbName =%c\n",pb->cmemmap->sname,pb->cmemmap->dbName);
      printpBlock(pFile,pb);
    }
  }
}

/*-----------------------------------------------------------------*/
/* newpCode - create and return a newly initialized pCode          */
/*-----------------------------------------------------------------*/
pCode *newpCode (PIC_OPCODE op, pCodeOp *pcop)
{
  pCodeInstruction *pci ;
    

  _ALLOC(pci,sizeof(pCodeInstruction));
  pci->pc.analyze = genericAnalyze;
  pci->pc.destruct = genericDestruct;
  pci->pc.type = PC_OPCODE;
  pci->op = op;
  pci->pc.prev = pci->pc.next = NULL;
  pci->pcop = pcop;
  pci->dest = 0;
  pci->bit_inst = 0;
  pci->num_ops = 2;
  pci->pc.print = genericPrint;
  pci->pc.from = pci->pc.to = pci->pc.label = NULL;

  if(pcop && pcop->name)
    printf("newpCode  operand name %s\n",pcop->name);

  switch(op) { 

  case POC_ANDLW:
    pci->mnemonic = scpANDLW;
    pci->num_ops = 1;
    break;
  case POC_ANDWF:
    pci->dest = 1;
  case POC_ANDFW:
    pci->mnemonic = scpANDWF;
    break;

  case POC_ADDLW:
    pci->mnemonic = scpADDLW;
    pci->num_ops = 1;
    break;
  case POC_ADDWF:
    pci->dest = 1;
  case POC_ADDFW:
    pci->mnemonic = scpADDWF;
    break;
  case POC_BCF:
    pci->bit_inst = 1;
    pci->mnemonic = scpBCF;
    break;
  case POC_BSF:
    pci->bit_inst = 1;
    pci->mnemonic = scpBSF;
    break;
  case POC_BTFSC:
    pci->bit_inst = 1;
    pci->mnemonic = scpBTFSC;
    pci->pc.analyze = AnalyzeSKIP;
    break;
  case POC_BTFSS:
    pci->bit_inst = 1;
    pci->mnemonic = scpBTFSS;
    pci->pc.analyze = AnalyzeSKIP;
    break;
  case POC_CALL:
    pci->num_ops = 1;
    pci->mnemonic = scpCALL;
    break;
  case POC_COMF:
    pci->mnemonic = scpCOMF;
    break;
  case POC_CLRF:
    pci->num_ops = 1;
    pci->mnemonic = scpCLRF;
    break;
  case POC_CLRW:
    pci->num_ops = 0;
    pci->mnemonic = scpCLRW;
    break;
  case POC_DECF:
    pci->dest = 1;
  case POC_DECFW:
    pci->mnemonic = scpDECF;
    break;
  case POC_DECFSZ:
    pci->dest = 1;
  case POC_DECFSZW:
    pci->mnemonic = scpDECFSZ;
    pci->pc.analyze = AnalyzeSKIP;
    break;
  case POC_GOTO:
    pci->num_ops = 1;
    pci->mnemonic = scpGOTO;
    pci->pc.analyze = AnalyzeGOTO;
    break;
  case POC_INCF:
    pci->dest = 1;
  case POC_INCFW:
    pci->mnemonic = scpINCF;
    break;
  case POC_INCFSZ:
    pci->dest = 1;
  case POC_INCFSZW:
    pci->mnemonic = scpINCFSZ;
    pci->pc.analyze = AnalyzeSKIP;
    break;
  case POC_IORLW:
    pci->num_ops = 1;
    pci->mnemonic = scpIORLW;
    break;
  case POC_IORWF:
    pci->dest = 1;
  case POC_IORFW:
    pci->mnemonic = scpIORWF;
    break;
  case POC_MOVF:
    pci->dest = 1;
  case POC_MOVFW:
    pci->mnemonic = scpMOVF;
    break;
  case POC_MOVLW:
    pci->num_ops = 1;
    pci->mnemonic = scpMOVLW;
    break;
  case POC_MOVWF:
    pci->num_ops = 1;
    pci->mnemonic = scpMOVWF;
    break;
  case POC_NEGF:
    pci->mnemonic = scpNEGF;
    break;
  case POC_RETLW:
    pci->num_ops = 1;
    pci->mnemonic = scpRETLW;
    pci->pc.analyze = AnalyzeRETURN;
    break;
  case POC_RETURN:
    pci->num_ops = 0;
    pci->mnemonic = scpRETURN;
    pci->pc.analyze = AnalyzeRETURN;
    break;
  case POC_SUBLW:
    pci->mnemonic = scpSUBLW;
    pci->num_ops = 1;
    break;
  case POC_SUBWF:
    pci->dest = 1;
  case POC_SUBFW:
    pci->mnemonic = scpSUBWF;
    break;
  case POC_TRIS:
    pci->mnemonic = scpTRIS;
    break;
  case POC_XORLW:
    pci->num_ops = 1;
    pci->mnemonic = scpXORLW;
    break;
  case POC_XORWF:
    pci->dest = 1;
  case POC_XORFW:
    pci->mnemonic = scpXORWF;
    break;

  default:
    pci->pc.print = genericPrint;
  }
   
  return (pCode *)pci;
}     	


/*-----------------------------------------------------------------*/
/* newPcodeCharP - create a new pCode from a char string           */
/*-----------------------------------------------------------------*/

pCode *newpCodeCharP(char *cP)
{

  pCodeComment *pcc ;
    
  _ALLOC(pcc,sizeof(pCodeComment));
   
  pcc->pc.type = PC_COMMENT;
  pcc->pc.prev = pcc->pc.next = NULL;
  pcc->pc.from = pcc->pc.to = pcc->pc.label = NULL;

  pcc->pc.analyze = genericAnalyze;
  pcc->pc.destruct = genericDestruct;
  pcc->pc.print = genericPrint;

  if(cP) {
    _ALLOC_ATOMIC(pcc->comment,strlen(cP)+1);
    strcpy(pcc->comment,cP);
  } else
    pcc->comment = NULL;

  return ( (pCode *)pcc);

}

/*-----------------------------------------------------------------*/
/* newpCodeGLabel - create a new global label                      */
/*-----------------------------------------------------------------*/


pCode *newpCodeFunction(char *mod,char *f)
{
  pCodeFunction *pcf;

  _ALLOC(pcf,sizeof(pCodeFunction));

  pcf->pc.type = PC_FUNCTION;
  pcf->pc.prev = pcf->pc.next = NULL;
  pcf->pc.from = pcf->pc.to = pcf->pc.label = NULL;

  pcf->pc.analyze = genericAnalyze;
  pcf->pc.destruct = genericDestruct;
  pcf->pc.print = pCodePrintFunction;

  if(mod) {
    _ALLOC_ATOMIC(pcf->modname,strlen(mod)+1);
    strcpy(pcf->modname,mod);
  } else
    pcf->modname = NULL;

  if(f) {
    _ALLOC_ATOMIC(pcf->fname,strlen(f)+1);
    strcpy(pcf->fname,f);
  } else
    pcf->fname = NULL;

  return ( (pCode *)pcf);

}


pCode *newpCodeLabel(int key)
{

  pCodeLabel *pcl;
    
  _ALLOC(pcl,sizeof(pCodeLabel));
   
  pcl->pc.type = PC_LABEL;
  pcl->pc.prev = pcl->pc.next = NULL;
  pcl->pc.from = pcl->pc.to = pcl->pc.label = NULL;

  pcl->pc.analyze = genericAnalyze;
  pcl->pc.destruct = genericDestruct;
  pcl->pc.print = pCodePrintLabel;

  pcl->key = key;

  return ( (pCode *)pcl);

}

/*-----------------------------------------------------------------*/
/* newpBlock - create and return a pointer to a new pBlock         */
/*-----------------------------------------------------------------*/
pBlock *newpBlock(void)
{

  pBlock *PpB;

  _ALLOC(PpB,sizeof(pBlock));
  PpB->next = PpB->prev = NULL;

  return PpB;

}

/*-----------------------------------------------------------------*/
/* newpCodeChain - create a new chain of pCodes                    */
/*-----------------------------------------------------------------*
 *
 *  This function will create a new pBlock and the pointer to the
 *  pCode that is passed in will be the first pCode in the block.
 *-----------------------------------------------------------------*/


pBlock *newpCodeChain(memmap *cm,pCode *pc)
{

  pBlock *pB = newpBlock();

  pB->pcHead = pB->pcTail = pc;
  pB->cmemmap = cm;

  return pB;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

pCodeOp *newpCodeOp(char *name)
{
  pCodeOp *pcop;

  _ALLOC(pcop,sizeof(pCodeOp) );
  pcop->type = PO_NONE;
  pcop->name = strdup(name);   

  return pcop;
}

pCodeOp *newpCodeOpLabel(int key)
{
  char *s = buffer;
  pCodeOp *pcop;

  _ALLOC(pcop,sizeof(pCodeOpLabel) );
  sprintf(s,"_%05d_DS_",key);
  pcop->type = PO_LABEL;
  pcop->name = strdup(s);
  ((pCodeOpLabel *)pcop)->key = key;

  return pcop;
}

pCodeOp *newpCodeOpLit(int lit)
{
  char *s = buffer;
  pCodeOp *pcop;


  _ALLOC(pcop,sizeof(pCodeOpLit) );
  pcop->type = PO_LITERAL;
  sprintf(s,"0x%02x",lit);
  _ALLOC_ATOMIC(pcop->name,strlen(s)+1);
  strcpy(pcop->name,s);
  ((pCodeOpLit *)pcop)->lit = lit;

  return pcop;
}

pCodeOp *newpCodeOpWild(int id)
{
  char *s = buffer;
  pCodeOp *pcop;


  _ALLOC(pcop,sizeof(pCodeOpWild) );
  pcop->type = PO_WILD;
  sprintf(s,"%%%d",id);
  _ALLOC_ATOMIC(pcop->name,strlen(s)+1);
  strcpy(pcop->name,s);
  ((pCodeOpWild *)pcop)->id = id;

  return pcop;
}

pCodeOp *newpCodeOpBit(char *s, int bit)
{
  pCodeOp *pcop;

  _ALLOC(pcop,sizeof(pCodeOpBit) );
  pcop->type = PO_BIT;
  pcop->name = strdup(s);   
  ((pCodeOpBit *)pcop)->bit = bit;
  ((pCodeOpBit *)pcop)->inBitSpace = 1;

  return pcop;
}

/*-----------------------------------------------------------------*/
/* addpCode2pBlock - place the pCode into the pBlock linked list   */
/*-----------------------------------------------------------------*/
void addpCode2pBlock(pBlock *pb, pCode *pc)
{

  pb->pcTail->next = pc;
  pc->prev = pb->pcTail;
  pc->next = NULL;
  pb->pcTail = pc;
}

/*-----------------------------------------------------------------*/
/* addpBlock - place a pBlock into the pFile                       */
/*-----------------------------------------------------------------*/
void addpBlock(pBlock *pb)
{

  if(!the_pFile) {
    /* First time called, we'll pass through here. */
    _ALLOC(the_pFile,sizeof(the_pFile));
    the_pFile->pbHead = the_pFile->pbTail = pb;
    the_pFile->functions = NULL;
    return;
  }

  the_pFile->pbTail->next = pb;
  pb->prev = the_pFile->pbTail;
  pb->next = NULL;
  the_pFile->pbTail = pb;
}


/*-----------------------------------------------------------------*/
/* printpCode - write the contents of a pCode to a file            */
/*-----------------------------------------------------------------*/
void printpCode(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  if(pc->print) {
    pc->print(of,pc);
    return;
  }

  fprintf(of,"warning - unable to print pCode\n");
}

/*-----------------------------------------------------------------*/
/* printpBlock - write the contents of a pBlock to a file          */
/*-----------------------------------------------------------------*/
void printpBlock(FILE *of, pBlock *pb)
{
  pCode *pc;

  if(!pb)
    return;

  if(!of)
    of = stderr;

  for(pc = pb->pcHead; pc; pc = pc->next)
    printpCode(of,pc);

}

/*-----------------------------------------------------------------*/
/*                                                                 */
/*       pCode processing                                          */
/*                                                                 */
/*    The stuff that follows is very PIC specific!                 */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

static void unlink(pCode *pc)
{
  if(pc  && pc->prev && pc->next) {

    pc->prev->next = pc->next;
    pc->next->prev = pc->prev;
  }
}
static void genericDestruct(pCode *pc)
{
  unlink(pc);

  fprintf(stderr,"warning, calling default pCode destructor\n");
  free(pc);
}

static char *get_op( pCodeInstruction *pcc)
{
  if(pcc && pcc->pcop && pcc->pcop->name)
    return pcc->pcop->name;
  return "NO operand";
#if 0
  operand *op;

  switch(pcc->lrr) {

  case POT_RESULT:
    op = IC_RESULT(pcc->ic);
    break;
  case POT_LEFT:
    op = IC_LEFT(pcc->ic);
    break;
  case POT_RIGHT:
    op = IC_RIGHT(pcc->ic);
    break;

  default:
    return "get_op bad lrr";
  }

  return(OP_SYMBOL(op)->rname[0] ? OP_SYMBOL(op)->rname : OP_SYMBOL(op)->name);
  
#endif
}

/*-----------------------------------------------------------------*/
/* genericPrint - the contents of a pCode to a file                */
/*-----------------------------------------------------------------*/
static void genericPrint(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  switch(pc->type) {
  case PC_COMMENT:
    fprintf(of,";%s\n", ((pCodeComment *)pc)->comment);
    break;

  case PC_OPCODE:
    // If the opcode has a label, print that first
    {
      pBranch *pbl = pc->label;
      while(pbl) {
	if(pbl->pc->type == PC_LABEL)
	  pCodePrintLabel(of, pbl->pc);
	pbl = pbl->next;
      }
    }

    fprintf(of, "\t%s\t", PCI(pc)->mnemonic);
    if( (PCI(pc)->num_ops >= 1) && (PCI(pc)->pcop)) {

      if(PCI(pc)->bit_inst) {
	if(PCI(pc)->pcop->type == PO_BIT) {
	  if( (((pCodeOpBit *)(PCI(pc)->pcop))->inBitSpace) )
	    fprintf(of,"(%s >> 3), (%s & 7)", 
		    PCI(pc)->pcop->name ,
		    PCI(pc)->pcop->name );
	  else
	    fprintf(of,"%s,%d", get_op(PCI(pc)), (((pCodeOpBit *)(PCI(pc)->pcop))->bit ));
	} else
	  fprintf(of,"%s,0 ; ?bug", get_op(PCI(pc)));
	//PCI(pc)->pcop->t.bit );
      } else {

	if(PCI(pc)->pcop->type == PO_BIT) {
	  if( PCI(pc)->num_ops == 2)
	    fprintf(of,"(%s >> 3),%c",PCI(pc)->pcop->name,((PCI(pc)->dest) ? 'F':'W'));
	  else
	    fprintf(of,"(1 << (%s & 7))",PCI(pc)->pcop->name);
	}else {
	  fprintf(of,"%s",get_op(PCI(pc)));

	  if( PCI(pc)->num_ops == 2)
	    fprintf(of,",%c", ( (PCI(pc)->dest) ? 'F':'W'));
	}
      }
    }

    {
      pBranch *dpb = pc->to;   // debug
      while(dpb) {
	switch ( dpb->pc->type) {
	case PC_OPCODE:
	  fprintf(of, "\t;%s", PCI(dpb->pc)->mnemonic);
	  break;
	case PC_LABEL:
	  fprintf(of, "\t;label %d", PCL(dpb->pc)->key);
	  break;
	case PC_FUNCTION:
	  fprintf(of, "\t;function %s", ( (PCF(dpb->pc)->fname) ? (PCF(dpb->pc)->fname) : "[END]"));
	  break;
	case PC_COMMENT:
	case PC_WILD:
	  break;
	}
	dpb = dpb->next;
      }
      fprintf(of,"\n");
    }

    break;

  case PC_LABEL:
  default:
    fprintf(of,"unknown pCode type %d\n",pc->type);
  }

}

/*-----------------------------------------------------------------*/
/* pCodePrintFunction - prints function begin/end                  */
/*-----------------------------------------------------------------*/

static void pCodePrintFunction(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  if( ((pCodeFunction *)pc)->modname) 
    fprintf(of,"F_%s",((pCodeFunction *)pc)->modname);

  if(PCF(pc)->fname) {
    pBranch *exits = pc->to;
    int i=0;
    fprintf(of,"%s\t;Function start\n",PCF(pc)->fname);
    while(exits) {
      i++;
      exits = exits->next;
    }
    if(i) i--;
    fprintf(of,"; %d exit point%c\n",i, ((i==1) ? ' ':'s'));
    
  }else {
    if(pc->from && 
       pc->from->pc->type == PC_FUNCTION &&
       PCF(pc->from->pc)->fname) 
      fprintf(of,"; exit point of %s\n",PCF(pc->from->pc)->fname);
    else
      fprintf(of,"; exit point [can't find entry point]\n");
  }
}
/*-----------------------------------------------------------------*/
/* pCodePrintLabel - prints label                                  */
/*-----------------------------------------------------------------*/

static void pCodePrintLabel(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  fprintf(of,"_%05d_DS_:\n",((pCodeLabel *)pc)->key);

}

/*-----------------------------------------------------------------*/

static pBranch * pBranchAppend(pBranch *h, pBranch *n)
{
  pBranch *b;

  if(!h)
    return n;

  b = h;
  while(b->next)
    b = b->next;

  b->next = n;

  return h;
  
}  
/*-----------------------------------------------------------------*/
/* pBranchLink - given two pcodes, this function will link them    */
/*               together through their pBranches                  */
/*-----------------------------------------------------------------*/
static void pBranchLink(pCode *f, pCode *t)
{
  pBranch *b;

  // Declare a new branch object for the 'from' pCode.

  _ALLOC(b,sizeof(pBranch));
  b->pc = t;                    // The link to the 'to' pCode.
  b->next = NULL;

  f->to = pBranchAppend(f->to,b);

  // Now do the same for the 'to' pCode.

  _ALLOC(b,sizeof(pBranch));
  b->pc = f;
  b->next = NULL;

  t->from = pBranchAppend(t->from,b);
  
}

#if 0
/*-----------------------------------------------------------------*/
/* pBranchFind - find the pBranch in a pBranch chain that contains */
/*               a pCode                                           */
/*-----------------------------------------------------------------*/
static pBranch *pBranchFind(pBranch *pb,pCode *pc)
{
  while(pb) {

    if(pb->pc == pc)
      return pb;

    pb = pb->next;
  }

  return NULL;
}

/*-----------------------------------------------------------------*/
/* pCodeUnlink - Unlink the given pCode from its pCode chain.      */
/*-----------------------------------------------------------------*/
static void pCodeUnlink(pCode *pc)
{
  pBranch *pb1,*pb2;
  pCode *pc1;

  if(!pc->prev || !pc->next) {
    fprintf(stderr,"unlinking bad pCode in %s:%d\n",__FILE__,__LINE__);
    exit(1);
  }

  /* first remove the pCode from the chain */
  pc->prev->next = pc->next;
  pc->next->prev = pc->prev;

  /* Now for the hard part... */

  /* Remove the branches */

  pb1 = pc->from;
  while(pb1) {
    pc1 = pb1->pc;    /* Get the pCode that branches to the
		       * one we're unlinking */

    /* search for the link back to this pCode (the one we're
     * unlinking) */
    if(pb2 = pBranchFind(pc1->to,pc)) {
      pb2->pc = pc->to->pc;  // make the replacement

      /* if the pCode we're unlinking contains multiple 'to'
       * branches (e.g. this a skip instruction) then we need
       * to copy these extra branches to the chain. */
      if(pc->to->next)
	pBranchAppend(pb2, pc->to->next);
    }
    
    pb1 = pb1->next;
  }


}
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void genericAnalyze(pCode *pc)
{
  switch(pc->type) {
  case PC_WILD:
  case PC_COMMENT:
    return;
  case PC_LABEL:
  case PC_FUNCTION:
  case PC_OPCODE:
    {
      // Go through the pCodes that are in pCode chain and link
      // them together through the pBranches. Note, the pCodes
      // are linked together as a contiguous stream like the 
      // assembly source code lines. The linking here mimics this
      // except that comments are not linked in.
      // 
      pCode *npc = pc->next;
      while(npc) {
	if(npc->type == PC_OPCODE || npc->type == PC_LABEL) {
	  pBranchLink(pc,npc);
	  return;
	} else
	  npc = npc->next;
      }
    }
  }
}

/*-----------------------------------------------------------------*/
/* findLabel - Search the pCode for a particular label             */
/*-----------------------------------------------------------------*/
pCode * findLabel(pCodeOpLabel *pcop_label)
{
  pBlock *pb;
  pCode  *pc;
  pBranch *pbr;

  if(!the_pFile)
    return NULL;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    for(pc = pb->pcHead; pc; pc = pc->next) {
      if(pc->type == PC_LABEL) {
	if( ((pCodeLabel *)pc)->key ==  pcop_label->key)
	  return pc;
      }
      if(pc->type == PC_OPCODE) {
	pbr = pc->label;
	while(pbr) {
	  if(pbr->pc->type == PC_LABEL) {
	    if( ((pCodeLabel *)(pbr->pc))->key ==  pcop_label->key)
	      return pc;
	  }
	  pbr = pbr->next;
	}
      }

    }
  }

  fprintf(stderr,"Couldn't find label %s", pcop_label->pcop.name);
  return NULL;
}

/*-----------------------------------------------------------------*/
/* findNextInstruction - given a pCode, find the next instruction  */
/*                       in the linked list                        */
/*-----------------------------------------------------------------*/
pCode * findNextInstruction(pCode *pc)
{

  while(pc) {
    if(pc->type == PC_OPCODE)
      return pc;

    pc = pc->next;
  }

  fprintf(stderr,"Couldn't find instruction\n");
  return NULL;
}

/*-----------------------------------------------------------------*/
/* findFunctionEnd - given a pCode find the end of the function    */
/*                   that contains it     t                        */
/*-----------------------------------------------------------------*/
pCode * findFunctionEnd(pCode *pc)
{

  while(pc) {
    if(pc->type == PC_FUNCTION &&  !(PCF(pc)->fname))
      return pc;

    pc = pc->next;
  }

  fprintf(stderr,"Couldn't find function end\n");
  return NULL;
}

#if 0
/*-----------------------------------------------------------------*/
/* AnalyzeLabel - if the pCode is a label, then merge it with the  */
/*                instruction with which it is associated.         */
/*-----------------------------------------------------------------*/
static void AnalyzeLabel(pCode *pc)
{

  pCodeUnlink(pc);

}
#endif

static void AnalyzeGOTO(pCode *pc)
{

  pBranchLink(pc,findLabel( (pCodeOpLabel *) (PCI(pc)->pcop) ));

}

static void AnalyzeSKIP(pCode *pc)
{

  pBranchLink(pc,findNextInstruction(pc->next));
  pBranchLink(pc,findNextInstruction(pc->next->next));

}

static void AnalyzeRETURN(pCode *pc)
{

  //  branch_link(pc,findFunctionEnd(pc->next));

}


void optimizepBlock(pBlock *pb)
{
  pCode *pc;

  if(!pb)
    return;

  for(pc = pb->pcHead; pc; pc = pc->next)
    pc->analyze(pc);

}
/*-----------------------------------------------------------------*/
/* pBlockMergeLabels - remove the pCode labels from the pCode      */
/*                     chain and put them into pBranches that are  */
/*                     associated with the appropriate pCode       */
/*                     instructions.                               */
/*-----------------------------------------------------------------*/
void pBlockMergeLabels(pBlock *pb)
{
  pBranch *pbr;
  pCode *pc, *pcnext=NULL;

  if(!pb)
    return;

  for(pc = pb->pcHead; pc; pc = pc->next) {

    if(pc->type == PC_LABEL) {
      if( !(pcnext = findNextInstruction(pc)) ) 
	return;  // Couldn't find an instruction associated with this label

      // Unlink the pCode label from it's pCode chain
      if(pc->prev) 
	pc->prev->next = pc->next;
      if(pc->next)
	pc->next->prev = pc->prev;

      // And link it into the instruction's pBranch labels. (Note, since
      // it's possible to have multiple labels associated with one instruction
      // we must provide a means to accomodate the additional labels. Thus
      // the labels are placed into the singly-linked list "label" as 
      // opposed to being a single member of the pCodeInstruction.)

      _ALLOC(pbr,sizeof(pBranch));
      pbr->pc = pc;
      pbr->next = NULL;

      pcnext->label = pBranchAppend(pcnext->label,pbr);
    }

  }

}
/*-----------------------------------------------------------------*/
/* AnalyzepCode - parse the pCode that has been generated and form */
/*                all of the logical connections.                  */
/*                                                                 */
/* Essentially what's done here is that the pCode flow is          */
/* determined.                                                     */
/*-----------------------------------------------------------------*/

void AnalyzepCode(char dbName)
{
  pBlock *pb;
  pCode *pc;
  pBranch *pbr;

  if(!the_pFile)
    return;

  fprintf(stderr," Analyzing pCode");

  /* First, merge the labels with the instructions */
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->cmemmap->dbName == dbName)
      pBlockMergeLabels(pb);
  }

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->cmemmap->dbName == dbName)
      optimizepBlock(pb);
  }

  /* Now build the call tree.
     First we examine all of the pCodes for functions.
     
   */
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->cmemmap->dbName == dbName) {
      pCode *pc_fstart=NULL;
      for(pc = pb->pcHead; pc; pc = pc->next) {
	if(pc->type == PC_FUNCTION) {
	  if (PCF(pc)->fname) {
	    // Found the beginning of a function.
	    _ALLOC(pbr,sizeof(pBranch));
	    pbr->pc = pc_fstart = pc;
	    pbr->next = NULL;

	    the_pFile->functions = pBranchAppend(the_pFile->functions,pbr);
	  } else {
	    // Found an exit point in a function, e.g. return
	    // (Note, there may be more than one return per function)
	    if(pc_fstart)
	      pBranchLink(pc_fstart, pc);
	  }
	}
      }
    }
  }
}

/*-----------------------------------------------------------------*/
/* ispCodeFunction - returns true if *pc is the pCode of a         */
/*                   function                                      */
/*-----------------------------------------------------------------*/
bool ispCodeFunction(pCode *pc)
{

  if(pc && pc->type == PC_FUNCTION && PCF(pc)->fname)
    return 1;

  return 0;
}

void printCallTree(FILE *of)
{
  pBranch *pbr;

  if(!the_pFile)
    return;

  if(!of)
    of = stderr;

  pbr = the_pFile->functions;

  fprintf(of,"Call Tree\n");
  while(pbr) {
    if(pbr->pc) {
      pCode *pc = pbr->pc;
      if(!ispCodeFunction(pc))
	fprintf(of,"bug in call tree");


      fprintf(of,"Function: %s\n", PCF(pc)->fname);

      while(pc->next && !ispCodeFunction(pc->next)) {
	pc = pc->next;
	if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL)
	  fprintf(of,"\t%s\n",get_op(PCI(pc)));
      }
    }

    pbr = pbr->next;
  }
}
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

  if(!peepSnippets)
    _ALLOC(peepSnippets,sizeof(pBlock));


  pb = newpCodeChain(NULL,newpCodeCharP("; Starting pCode block"));

  if(!peepSnippets->next)
    peepSnippets->next = pb;
  else {
    peepSnippets->next->next = pb;
    pb->prev = peepSnippets->next;
  }
  // now create some sample peep pcodes

  addpCode2pBlock( pb, newpCode(POC_MOVWF, newpCodeOpWild(1)) );
  addpCode2pBlock( pb, newpCode(POC_MOVFW, newpCodeOpWild(1)) );
  // addpBlock(pb);

}
