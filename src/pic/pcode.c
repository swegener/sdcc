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
#include "newalloc.h"
#include "ralloc.h"

#include "pcode.h"

// Eventually this will go into device dependent files:
pCodeOp pc_status    = {PO_STATUS,  "STATUS"};
pCodeOp pc_indf      = {PO_INDF,    "INDF"};
pCodeOp pc_fsr       = {PO_FSR,     "FSR"};

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
static int peepOptimizing = 1;
static int GpCodeSequenceNumber = 1;

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
static void pCodeOpPrint(FILE *of, pCodeOp *pcop);
static char *get_op( pCodeInstruction *pcc);
int pCodePeepMatchLine(pCodePeep *peepBlock, pCode *pcs, pCode *pcd);
int pCodePeepMatchRule(pCode *pc);


char *Safe_strdup(char *str)
{
  char *copy;

  if(!str)
    return NULL;

  copy = strdup(str);
  if(!copy) {
    fprintf(stderr, "out of memory %s,%d\n",__FUNCTION__,__LINE__);
    exit(1);
  }

  return copy;
    
}

char getpBlock_dbName(pBlock *pb)
{
  if(!pb)
    return 0;

  if(pb->cmemmap)
    return pb->cmemmap->dbName;

  return pb->dbName;
}
/*-----------------------------------------------------------------*/
/* movepBlock2Head - given the dbname of a pBlock, move all        */
/*                   instances to the front of the doubly linked   */
/*                   list of pBlocks                               */
/*-----------------------------------------------------------------*/

void movepBlock2Head(char dbName)
{
  pBlock *pb;

  pb = the_pFile->pbHead;

  while(pb) {

    if(getpBlock_dbName(pb) == dbName) {
      pBlock *pbn = pb->next;
      pb->next = the_pFile->pbHead;
      the_pFile->pbHead->prev = pb;
      the_pFile->pbHead = pb;

      if(pb->prev)
	pb->prev->next = pbn;

      // If the pBlock that we just moved was the last
      // one in the link of all of the pBlocks, then we
      // need to point the tail to the block just before
      // the one we moved.
      // Note: if pb->next is NULL, then pb must have 
      // been the last pBlock in the chain.

      if(pbn)
	pbn->prev = pb->prev;
      else
	the_pFile->pbTail = pb->prev;

      pb = pbn;

    } else
      pb = pb->next;

  }

}

void copypCode(FILE *of, char dbName)
{
  pBlock *pb;

  if(!of || !the_pFile)
    return;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(getpBlock_dbName(pb) == dbName)
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
      if(pb->cmemmap)
	fprintf(pFile,"%s",pb->cmemmap->sname);
      else
	fprintf(pFile,"internal pblock");

      fprintf(pFile,", dbName =%c\n",getpBlock_dbName(pb));
      printpBlock(pFile,pb);
    }
  }
}

/*-----------------------------------------------------------------*/
/* newpCode - create and return a newly initialized pCode          */
/*                                                                 */
/*  fixme - rename this                                            */
/*                                                                 */
/* The purpose of this routine is to create a new Instruction      */
/* pCode. This is called by gen.c while the assembly code is being */
/* generated.                                                      */
/*                                                                 */
/* Inouts:                                                         */
/*  PIC_OPCODE op - the assembly instruction we wish to create.    */
/*                  (note that the op is analogous to but not the  */
/*                  same thing as the opcode of the instruction.)  */
/*  pCdoeOp *pcop - pointer to the operand of the instruction.     */
/*                                                                 */
/* Outputs:                                                        */
/*  a pointer to the new malloc'd pCode is returned.               */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/
pCode *newpCode (PIC_OPCODE op, pCodeOp *pcop)
{
  pCodeInstruction *pci ;
    

  pci = Safe_calloc(1, sizeof(pCodeInstruction));

  pci->pc.analyze  = genericAnalyze;       // pointers to the generic functions, it
  pci->pc.destruct = genericDestruct;      // doesn't hurt to think about C++ virtual
  pci->pc.print    = genericPrint;         // functions here.
  pci->pc.type = PC_OPCODE;                // 
  pci->op = op;                            // the "opcode" for the instruction.
  pci->pc.prev = pci->pc.next = NULL;      // The pCode gets linked in later
  pci->pcop = pcop;                        // The operand of the instruction
  pci->dest = 0;                           // e.g. W or F 
  pci->bit_inst = 0;                       // e.g. btfxx instructions
  pci->num_ops = 2;                        // Most instructions have two ops...
  pci->inCond = pci->outCond = PCC_NONE;   /* input/output conditions. This is used during
					    * optimization to ascertain instruction dependencies.
					    * For example, if an instruction affects the Z bit,
					    * then the output condition for this instruction
					    * is "z bit is affected". The "conditions" are bit
					    * constants defined in pcode.h. */
  pci->pc.from = pci->pc.to = NULL;        // Flow linkages are defined later
  pci->pc.label = NULL;                    // Labels get merged into instructions here.
  pci->pc.pb = NULL;                       // The pBlock to which this instruction belongs

  if(pcop && pcop->name)
    printf("newpCode  operand name %s\n",pcop->name);

  // The most pic dependent portion of the pCode logic:
  switch(op) { 

  case POC_ANDLW:
    pci->inCond   = PCC_W;
    pci->outCond  = PCC_W | PCC_Z;
    pci->mnemonic = scpANDLW;
    pci->num_ops  = 1;
    break;
  case POC_ANDWF:
    pci->dest = 1;
    pci->inCond   = PCC_W | PCC_REGISTER;
    pci->outCond  = PCC_REGISTER | PCC_Z;
    pci->mnemonic = scpANDWF;
    break;
  case POC_ANDFW:
    pci->inCond   = PCC_W | PCC_REGISTER;
    pci->outCond  = PCC_W | PCC_Z;
    pci->mnemonic = scpANDWF;
    break;

  case POC_ADDLW:
    pci->inCond   = PCC_W;
    pci->outCond  = PCC_W | PCC_Z | PCC_C | PCC_DC;
    pci->mnemonic = scpADDLW;
    pci->num_ops = 1;
    break;
  case POC_ADDWF:
    pci->dest     = 1;
    pci->inCond   = PCC_W | PCC_REGISTER;
    pci->outCond  = PCC_REGISTER | PCC_Z | PCC_C | PCC_DC;
    pci->mnemonic = scpADDWF;
    break;
  case POC_ADDFW:
    pci->inCond   = PCC_W | PCC_REGISTER;
    pci->outCond  = PCC_W | PCC_Z | PCC_C | PCC_DC;
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
/* newpCodeWild - create a "wild" as in wild card pCode            */
/*                                                                 */
/* Wild pcodes are used during the peep hole optimizer to serve    */
/* as place holders for any instruction. When a snippet of code is */
/* compared to a peep hole rule, the wild card opcode will match   */
/* any instruction. However, the optional operand and label are    */
/* additional qualifiers that must also be matched before the      */
/* line (of assembly code) is declared matched. Note that the      */
/* operand may be wild too.                                        */
/*                                                                 */
/*-----------------------------------------------------------------*/

pCode *newpCodeWild(int pCodeID, pCodeOp *optional_operand, pCodeOp *optional_label)
{

  pCodeWild *pcw;
    
  pcw = Safe_calloc(1,sizeof(pCodeWild));

  pcw->pc.type = PC_WILD;
  pcw->pc.prev = pcw->pc.next = NULL;
  pcw->pc.from = pcw->pc.to = pcw->pc.label = NULL;
  pcw->pc.pb = NULL;

  pcw->pc.analyze = genericAnalyze;
  pcw->pc.destruct = genericDestruct;
  pcw->pc.print = genericPrint;

  pcw->id = pCodeID;
  pcw->operand = optional_operand;
  pcw->label   = optional_label;

  return ( (pCode *)pcw);
  
}

/*-----------------------------------------------------------------*/
/* newPcodeCharP - create a new pCode from a char string           */
/*-----------------------------------------------------------------*/

pCode *newpCodeCharP(char *cP)
{

  pCodeComment *pcc ;
    
  pcc = Safe_calloc(1,sizeof(pCodeComment));

  pcc->pc.type = PC_COMMENT;
  pcc->pc.prev = pcc->pc.next = NULL;
  pcc->pc.from = pcc->pc.to = pcc->pc.label = NULL;
  pcc->pc.pb = NULL;

  pcc->pc.analyze = genericAnalyze;
  pcc->pc.destruct = genericDestruct;
  pcc->pc.print = genericPrint;

  pcc->comment = Safe_strdup(cP);

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
  pcf->pc.pb = NULL;

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

  char *s = buffer;
  pCodeLabel *pcl;
    
  pcl = Safe_calloc(1,sizeof(pCodeLabel) );

  pcl->pc.type = PC_LABEL;
  pcl->pc.prev = pcl->pc.next = NULL;
  pcl->pc.from = pcl->pc.to = pcl->pc.label = NULL;
  pcl->pc.pb = NULL;

  pcl->pc.analyze = genericAnalyze;
  pcl->pc.destruct = genericDestruct;
  pcl->pc.print = pCodePrintLabel;

  pcl->key = key;

  if(key>0) {
    sprintf(s,"_%05d_DS_",key);
    pcl->label = Safe_strdup(s);
  } else
    pcl->label = NULL;

  return ( (pCode *)pcl);

}
pCode *newpCodeLabelStr(char *str)
{
  pCode *pc = newpCodeLabel(-1);

  PCL(pc)->label = Safe_strdup(str);

  return pc;
}

/*-----------------------------------------------------------------*/
/* newpBlock - create and return a pointer to a new pBlock         */
/*-----------------------------------------------------------------*/
pBlock *newpBlock(void)
{

  pBlock *PpB;

  _ALLOC(PpB,sizeof(pBlock));
  PpB->next = PpB->prev = NULL;

  PpB->function_entries = PpB->function_exits = PpB->function_calls = NULL;
  PpB->registers = NULL;
  PpB->visited = 0;

  return PpB;

}

/*-----------------------------------------------------------------*/
/* newpCodeChain - create a new chain of pCodes                    */
/*-----------------------------------------------------------------*
 *
 *  This function will create a new pBlock and the pointer to the
 *  pCode that is passed in will be the first pCode in the block.
 *-----------------------------------------------------------------*/


pBlock *newpCodeChain(memmap *cm,char c, pCode *pc)
{

  pBlock *pB  = newpBlock();

  pB->pcHead  = pB->pcTail = pc;
  pB->cmemmap = cm;
  pB->dbName  = c;

  return pB;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

pCodeOp *newpCodeOp(char *name, PIC_OPTYPE type)
{
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOp) );
  pcop->type = type;
  pcop->name = Safe_strdup(name);   

  return pcop;
}

/*-----------------------------------------------------------------*/
/* newpCodeOpLabel - Create a new label given the key              */
/*  Note, a negative key means that the label is part of wild card */
/*  (and hence a wild card label) used in the pCodePeep            */
/*   optimizations).                                               */
/*-----------------------------------------------------------------*/

pCodeOp *newpCodeOpLabel(int key)
{
  char *s = buffer;
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpLabel) );
  pcop->type = PO_LABEL;

  if(key>0) {
    sprintf(s,"_%05d_DS_",key);
    pcop->name = Safe_strdup(s);
  } else
    pcop->name = NULL;

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

pCodeOp *newpCodeOpWild(int id, pCodePeep *pcp, pCodeOp *subtype)
{
  char *s = buffer;
  pCodeOp *pcop;


  if(!pcp || !subtype) {
    fprintf(stderr, "Wild opcode declaration error: %s-%d\n",__FILE__,__LINE__);
    exit(1);
  }

  pcop = Safe_calloc(1,sizeof(pCodeOpWild));
  pcop->type = PO_WILD;
  sprintf(s,"%%%d",id);
  pcop->name = Safe_strdup(s);

  PCOW(pcop)->id = id;
  PCOW(pcop)->pcp = pcp;
  PCOW(pcop)->subtype = subtype;
  PCOW(pcop)->matched = NULL;

  return pcop;
}

pCodeOp *newpCodeOpBit(char *s, int bit)
{
  pCodeOp *pcop;

  _ALLOC(pcop,sizeof(pCodeOpBit) );
  pcop->type = PO_BIT;
  pcop->name = Safe_strdup(s);   
  PCOB(pcop)->bit = bit;
  if(bit>=0)
    PCOB(pcop)->inBitSpace = 1;
  else
    PCOB(pcop)->inBitSpace = 0;

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
  pc->pb = pb;
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

void printpCodeString(FILE *of, pCode *pc, int max)
{
  int i=0;

  while(pc && (i++<max)) {
    pc->print(of,pc);
    pc = pc->next;
  }
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


void pBlockRegs(FILE *of, pBlock *pb)
{

  regs  *r;

  r = setFirstItem(pb->registers);
  while (r) {
    fprintf(of,"   %s\n",r->name);
    r = setNextItem(pb->registers);
  }
}


static char *get_op( pCodeInstruction *pcc)
{
  if(pcc && pcc->pcop) {



  if(pcc->pcop->type == PO_GPR_TEMP) {
    regs *r;
    r = pic14_regWithIdx(PCOR(pcc->pcop)->r->rIdx);
    //pcc->pcop->name = Safe_strdup(r->name);
    //sprintf(buffer, "R0X%x",PCOR(pcc->pcop)->rIdx);
    //pcc->pcop->name = Safe_strdup(PCOR(pcc->pcop)->r->name);
    fprintf(stderr,"getop: getting %s\nfrom:\n",r->name); //pcc->pcop->name);
    pBlockRegs(stderr,pcc->pc.pb);
    return r->name;
  }

  if  (pcc->pcop->name)
    return pcc->pcop->name;

  }
  return "NO operand";
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pCodeOpPrint(FILE *of, pCodeOp *pcop)
{

  fprintf(of,"pcodeopprint\n");
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
	    fprintf(of,"(%s >> 3),%c",get_op(PCI(pc)),((PCI(pc)->dest) ? 'F':'W'));
	  else
	    fprintf(of,"(1 << (%s & 7))",get_op(PCI(pc)));

/*
	  if( PCI(pc)->num_ops == 2)
	    fprintf(of,"(%s >> 3),%c",PCI(pc)->pcop->name,((PCI(pc)->dest) ? 'F':'W'));
	  else
	    fprintf(of,"(1 << (%s & 7))",PCI(pc)->pcop->name);
*/
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

  case PC_WILD:
    fprintf(of,";\tWild opcode: id=%d\n",PCW(pc)->id);
    if(PCW(pc)->operand) {
      fprintf(of,";\toperand  ");
      pCodeOpPrint(of,PCW(pc)->operand );
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
    //if(i) i--;
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

  if(PCL(pc)->label) 
    fprintf(of,"%s\n",PCL(pc)->label);
  else if (PCL(pc)->key >=0) 
    fprintf(of,"_%05d_DS_:\n",PCL(pc)->key);
  else
    fprintf(of,";wild card label\n");

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


void AnalyzepBlock(pBlock *pb)
{
  pCode *pc;

  if(!pb)
    return;

  /* Find all of the registers used in this pBlock */
  for(pc = pb->pcHead; pc; pc = pc->next) {
    if(pc->type == PC_OPCODE) {
      if(PCI(pc)->pcop && PCI(pc)->pcop->type == PO_GPR_TEMP) {

	/* Loop through all of the registers declared so far in
	   this block and see if we find this new there */

	regs *r = setFirstItem(pb->registers);

	while(r) {
	  if(r->rIdx == PCOR(PCI(pc)->pcop)->r->rIdx) {
	    PCOR(PCI(pc)->pcop)->r = r;
	    break;
	  }
	  r = setNextItem(pb->registers);
	}

	if(!r) {
	  /* register wasn't found */
	  r = Safe_calloc(1, sizeof(regs));
	  memcpy(r,PCOR(PCI(pc)->pcop)->r, sizeof(regs));
	  addSet(&pb->registers, r);
	  PCOR(PCI(pc)->pcop)->r = r;
	  fprintf(stderr,"added register to pblock: reg %d\n",r->rIdx);
	} else 
	  fprintf(stderr,"found register in pblock: reg %d\n",r->rIdx);
      }
    }
  }
}

int OptimizepBlock(pBlock *pb)
{
  pCode *pc;
  int matches =0;

  if(!pb || !peepOptimizing)
    return 0;

  fprintf(stderr," Optimizing pBlock\n");

  for(pc = pb->pcHead; pc; pc = pc->next)
    matches += pCodePeepMatchRule(pc);

  return matches;

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
/*-----------------------------------------------------------------*/
void OptimizepCode(char dbName)
{
#define MAX_PASSES 4

  int matches = 0;
  int passes = 0;
  pBlock *pb;

  if(!the_pFile)
    return;

  fprintf(stderr," Optimizing pCode\n");

  do {
    for(pb = the_pFile->pbHead; pb; pb = pb->next) {
      if('*' == dbName || getpBlock_dbName(pb) == dbName)
	matches += OptimizepBlock(pb);
    }
  }
  while(matches && ++passes < MAX_PASSES);

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
    if('*' == dbName || getpBlock_dbName(pb) == dbName) {
      pBlockMergeLabels(pb);
      AnalyzepBlock(pb);
    }
  }

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if('*' == dbName || getpBlock_dbName(pb) == dbName)
      OptimizepBlock(pb);
  }

  /* Now build the call tree.
     First we examine all of the pCodes for functions.
     Keep in mind that the function boundaries coincide
     with pBlock boundaries. 

     The algorithm goes something like this:
     We have two nested loops. The outer loop iterates
     through all of the pBlocks/functions. The inner
     loop iterates through all of the pCodes for
     a given pBlock. When we begin iterating through
     a pBlock, the variable pc_fstart, pCode of the start
     of a function, is cleared. We then search for pCodes
     of type PC_FUNCTION. When one is encountered, we
     initialize pc_fstart to this and at the same time
     associate a new pBranch object that signifies a 
     branch entry. If a return is found, then this signifies
     a function exit point. We'll link the pCodes of these
     returns to the matching pc_fstart.

     When we're done, a doubly linked list of pBranches
     will exist. The head of this list is stored in
     `the_pFile', which is the meta structure for all
     of the pCode. Look at the printCallTree function
     on how the pBranches are linked together.

   */
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if('*' == dbName || getpBlock_dbName(pb) == dbName) {
      pCode *pc_fstart=NULL;
      for(pc = pb->pcHead; pc; pc = pc->next) {
	if(pc->type == PC_FUNCTION) {
	  if (PCF(pc)->fname) {
	    // I'm not liking this....
	    // Found the beginning of a function.
	    _ALLOC(pbr,sizeof(pBranch));
	    pbr->pc = pc_fstart = pc;
	    pbr->next = NULL;

	    the_pFile->functions = pBranchAppend(the_pFile->functions,pbr);

	    // Here's a better way of doing the same:
	    addSet(&pb->function_entries, pc);

	  } else {
	    // Found an exit point in a function, e.g. return
	    // (Note, there may be more than one return per function)
	    if(pc_fstart)
	      pBranchLink(pc_fstart, pc);

	    addSet(&pb->function_exits, pc);
	  }
	} else	if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
	  addSet(&pb->function_calls,pc);
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

/*-----------------------------------------------------------------*/
/* findFunction - Search for a function by name (given the name)   */
/*                in the set of all functions that are in a pBlock */
/* (note - I expect this to change because I'm planning to limit   */
/*  pBlock's to just one function declaration                      */
/*-----------------------------------------------------------------*/
pCode *findFunction(char *fname)
{
  pBlock *pb;
  pCode *pc;
  if(!fname)
    return NULL;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {

    pc = setFirstItem(pb->function_entries);
    while(pc) {
    
      if((pc->type == PC_FUNCTION) &&
	 (PCF(pc)->fname) && 
	 (strcmp(fname, PCF(pc)->fname)==0))
	return pc;

      pc = setNextItem(pb->function_entries);

    }

  }
  return NULL;
}

void MarkUsedRegisters(set *regset)
{

  regs *r1,*r2;

  for(r1=setFirstItem(regset); r1; r1=setNextItem(regset)) {
    r2 = pic14_regWithIdx(r1->rIdx);
    r2->isFree = 0;
    r2->wasUsed = 1;
  }
}

void pBlockStats(FILE *of, pBlock *pb)
{

  pCode *pc;
  regs  *r;

  fprintf(of,"***\n  pBlock Stats\n***\n");

  // for now just print the first element of each set
  pc = setFirstItem(pb->function_entries);
  if(pc) {
    fprintf(of,"entry\n");
    pc->print(of,pc);
  }
  pc = setFirstItem(pb->function_exits);
  if(pc) {
    fprintf(of,"has an exit\n");
    pc->print(of,pc);
  }

  pc = setFirstItem(pb->function_calls);
  if(pc) {
    fprintf(of,"functions called\n");

    while(pc) {
      pc->print(of,pc);
      pc = setNextItem(pb->function_calls);
    }
  }

  r = setFirstItem(pb->registers);
  if(r) {
    int n = elementsInSet(pb->registers);

    fprintf(of,"%d compiler assigned register%c:\n",n, ( (n!=1) ? 's' : ' '));

    while (r) {
      fprintf(of,"   %s\n",r->name);
      r = setNextItem(pb->registers);
    }
  }
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void sequencepCode(void)
{
  pBlock *pb;
  pCode *pc;


  for(pb = the_pFile->pbHead; pb; pb = pb->next) {

    pb->seq = GpCodeSequenceNumber+1;

    for( pc = pb->pcHead; pc; pc = pc->next)
      pc->seq = ++GpCodeSequenceNumber;
  }

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
set *register_usage(pBlock *pb)
{
  pCode *pc,*pcn;
  set *registers=NULL;
  set *registersInCallPath = NULL;

  /* check recursion */

  pc = setFirstItem(pb->function_entries);

  if(!pc)
    return registers;

  pb->visited = 1;

  if(pc->type != PC_FUNCTION)
    fprintf(stderr,"%s, first pc is not a function???\n",__FUNCTION__);

  pc = setFirstItem(pb->function_calls);
  for( ; pc; pc = setNextItem(pb->function_calls)) {

    if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
      char *dest = get_op(PCI(pc));

      pcn = findFunction(dest);
      if(pcn) 
	registersInCallPath = register_usage(pcn->pb);
    } else
      fprintf(stderr,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

  }


  pBlockStats(stderr,pb);  // debug
  if(registersInCallPath) {
    /* registers were used in the functions this pBlock has called */
    /* so now, we need to see if these collide with the ones we are */
    /* using here */

    regs *r1,*r2, *newreg;

    fprintf(stderr,"comparing registers\n");

    r1 = setFirstItem(registersInCallPath);
    while(r1) {

      r2 = setFirstItem(pb->registers);

      while(r2) {

	if(r2->rIdx == r1->rIdx) {
	  newreg = pic14_findFreeReg();


	  if(!newreg) {
	    fprintf(stderr,"Bummer, no more registers.\n");
	    exit(1);
	  }

	  fprintf(stderr,"Cool found register collision nIdx=%d moving to %d\n",
		  r1->rIdx, newreg->rIdx);
	  r2->rIdx = newreg->rIdx;
	  //if(r2->name) free(r2->name);
	  r2->name = Safe_strdup(newreg->name);
	  newreg->isFree = 0;
	  newreg->wasUsed = 1;
	}
	r2 = setNextItem(pb->registers);
      }

      r1 = setNextItem(registersInCallPath);
    }

    /* Collisions have been resolved. Now free the registers in the call path */
    r1 = setFirstItem(registersInCallPath);
    while(r1) {
      newreg = pic14_regWithIdx(r1->rIdx);
      newreg->isFree = 1;
      r1 = setNextItem(registersInCallPath);
    }

  } else
    MarkUsedRegisters(pb->registers);

  registers = unionSets(pb->registers, registersInCallPath, THROW_NONE);

  if(registers) 
    fprintf(stderr,"returning regs\n");
  else
    fprintf(stderr,"not returning regs\n");

  fprintf(stderr,"pBlock after register optim.\n");
  pBlockStats(stderr,pb);  // debug


  return registers;
}

/*-----------------------------------------------------------------*/
/* printCallTree - writes the call tree to a file                  */
/*                                                                 */
/*-----------------------------------------------------------------*/
void pct2(FILE *of,pBlock *pb,int indent)
{
  pCode *pc,*pcn;
  int i;
  //  set *registersInCallPath = NULL;

  if(!of)
    return;// registers;

  if(indent > 10)
    return; // registers;   //recursion ?

  pc = setFirstItem(pb->function_entries);

  if(!pc)
    return;

  pb->visited = 0;

  for(i=0;i<indent;i++)   // Indentation
    fputc(' ',of);

  if(pc->type == PC_FUNCTION)
    fprintf(of,"%s\n",PCF(pc)->fname);
  else
    return;  // ???


  pc = setFirstItem(pb->function_calls);
  for( ; pc; pc = setNextItem(pb->function_calls)) {

    if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
      char *dest = get_op(PCI(pc));

      pcn = findFunction(dest);
      if(pcn) 
	pct2(of,pcn->pb,indent+1);
    } else
      fprintf(of,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

  }


}

#if 0
  fprintf(stderr,"pBlock before register optim.\n");
  pBlockStats(stderr,pb);  // debug

  if(registersInCallPath) {
    /* registers were used in the functions this pBlock has called */
    /* so now, we need to see if these collide with the ones we are using here */

    regs *r1,*r2, *newreg;

    fprintf(stderr,"comparing registers\n");

    r1 = setFirstItem(registersInCallPath);
    while(r1) {

      r2 = setFirstItem(pb->registers);

      while(r2) {

	if(r2->rIdx == r1->rIdx) {
	  newreg = pic14_findFreeReg();


	  if(!newreg) {
	    fprintf(stderr,"Bummer, no more registers.\n");
	    exit(1);
	  }

	  fprintf(stderr,"Cool found register collision nIdx=%d moving to %d\n",
		  r1->rIdx, newreg->rIdx);
	  r2->rIdx = newreg->rIdx;
	  //if(r2->name) free(r2->name);
	  r2->name = Safe_strdup(newreg->name);
	  newreg->isFree = 0;
	  newreg->wasUsed = 1;
	}
	r2 = setNextItem(pb->registers);
      }

      r1 = setNextItem(registersInCallPath);
    }

    /* Collisions have been resolved. Now free the registers in the call path */
    r1 = setFirstItem(registersInCallPath);
    while(r1) {
      newreg = pic14_regWithIdx(r1->rIdx);
      newreg->isFree = 1;
      r1 = setNextItem(registersInCallPath);
    }

  } else
    MarkUsedRegisters(pb->registers);

  registers = unionSets(pb->registers, registersInCallPath, THROW_NONE);

  if(registers) 
    fprintf(stderr,"returning regs\n");
  else
    fprintf(stderr,"not returning regs\n");

  fprintf(stderr,"pBlock after register optim.\n");
  pBlockStats(stderr,pb);  // debug


  return registers;

#endif


/*-----------------------------------------------------------------*/
/* printCallTree - writes the call tree to a file                  */
/*                                                                 */
/*-----------------------------------------------------------------*/

void printCallTree(FILE *of)
{
  pBranch *pbr;
  pBlock  *pb;
  pCode   *pc;

  if(!the_pFile)
    return;

  if(!of)
    of = stderr;

  fprintf(of, "\npBlock statistics\n");
  for(pb = the_pFile->pbHead; pb;  pb = pb->next )
    pBlockStats(stderr,pb);



  fprintf(of,"Call Tree\n");
  pbr = the_pFile->functions;
  while(pbr) {
    if(pbr->pc) {
      pc = pbr->pc;
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


  /* Re-allocate the registers so that there are no collisions
   * between local variables when one function call another */

  pic14_deallocateAllRegs();

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(!pb->visited)
      register_usage(pb);
  }

  fprintf(of,"\n**************\n\na better call tree\n");
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->visited)
      pct2(of,pb,0);
  }

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    fprintf(of,"block dbname: %c\n", getpBlock_dbName(pb));
  }
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
