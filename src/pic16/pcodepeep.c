/*-------------------------------------------------------------------------

   pcodepeep.c - post code generation
   Written By -  Scott Dattalo scott@dattalo.com
   Ported to PIC16 By - Martin Dubuc m.dubuc@rogers.com

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
#include <stdlib.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"
//#define PCODE_DEBUG
#include "pcode.h"
#include "pcodeflow.h"
#include "ralloc.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

pCodeOp *pic16_popCopyGPR2Bit(pCodeOpReg *pc, int bitval);

pCodeOp *pic16_newpCodeOpWild(int id, pCodeWildBlock *pcwb, pCodeOp *subtype);
pCode *pic16_newpCodeWild(int pCodeID, pCodeOp *optional_operand, pCodeOp *optional_label);
pCode * pic16_findNextInstruction(pCode *pc);
int pic16_getpCode(char *mnem,int dest);
int pic16_getpCodePeepCommand(char *cmd);
void pic16_pBlockMergeLabels(pBlock *pb);
char *pCode2str(char *str, int size, pCode *pc);
char *pic16_get_op( pCodeOp *pcop,char *buf,int buf_size);

extern pCodeInstruction *pic16Mnemonics[];


#define IS_PCCOMMENT(x) ( x && (x->type==PC_COMMENT))

/****************************************************************/
/*
 * rootRules - defined in SDCCpeep.c
 *  This is a pointer to the (parsed) peephole rules that are
 * defined in peep.def.
 */

//extern peepRule *rootRules;




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


/****************************************************************/
/*                                                              */
/* peepSnippets -                                               */
/*                                                              */
/****************************************************************/

static pCodePeepSnippets  *peepSnippets=NULL;

/****************************************************************/
/*                                                              */
/* curPeep                                                      */
/*                                                              */
/****************************************************************/

//static pCodePeep          *curPeep=NULL;

/****************************************************************/
/*                                                              */
/* curBlock                                                     */
/*                                                              */
/****************************************************************/

//static pBlock             *curBlock=NULL;


/****************************************************************/
/*                                                              */
/* max wild cards in a peep rule                                */
/*                                                              */
/****************************************************************/

//static int                sMaxWildVar   = 0;
//static int                sMaxWildMnem  = 0;


typedef struct pCodeToken 
{
  int tt;  // token type;
  union {
    char c;  // character
    int  n;  // number
    char *s; // string
  } tok;

} pCodeToken;

pCodeToken tokArr[50];
static unsigned   tokIdx=0;


typedef enum  {
  PCT_NULL=0,
  PCT_SPACE=1,
  PCT_PERCENT,
  PCT_LESSTHAN,
  PCT_GREATERTHAN,
  PCT_COLON,
  PCT_COMMA,
  PCT_COMMENT,
  PCT_STRING,
  PCT_NUMBER

} pCodeTokens;


typedef struct parsedPattern {
  struct pcPattern *pcp;
  pCodeToken *pct;
} parsedPattern;

#define MAX_PARSEDPATARR 50
parsedPattern parsedPatArr[MAX_PARSEDPATARR];
//static unsigned int parsedPatIdx=0;


typedef enum {
  PCP_LABEL=1,
  PCP_NUMBER,
  PCP_STR,
  PCP_WILDVAR,
  PCP_WILDSTR,
  PCP_COMMA,
  PCP_COMMENT
} pCodePatterns;

static char pcpat_label[]      = {PCT_PERCENT, PCT_NUMBER, PCT_COLON, 0};
static char pcpat_number[]     = {PCT_NUMBER, 0};
static char pcpat_string[]     = {PCT_STRING, 0};
static char pcpat_wildString[] = {PCT_PERCENT, PCT_STRING, 0};
static char pcpat_wildVar[]    = {PCT_PERCENT, PCT_NUMBER, 0};
static char pcpat_comma[]      = {PCT_COMMA, 0};
static char pcpat_comment[]    = {PCT_COMMENT, 0};


typedef struct pcPattern {
  char pt;                 // Pattern type
  char *tokens;           // list of tokens that describe the pattern
  void * (*f) (void *,pCodeWildBlock *);
} pcPattern;

static pcPattern pcpArr[] = {
  {PCP_LABEL,     pcpat_label,      NULL},
  {PCP_WILDSTR,   pcpat_wildString, NULL},
  {PCP_STR,       pcpat_string,     NULL},
  {PCP_WILDVAR,   pcpat_wildVar,    NULL},
  {PCP_COMMA,     pcpat_comma,      NULL},
  {PCP_COMMENT,   pcpat_comment,    NULL},
  {PCP_NUMBER,    pcpat_number,     NULL}
};

#define PCPATTERNS (sizeof(pcpArr)/sizeof(pcPattern))

// Assembly Line Token
typedef enum {
  ALT_LABEL=1,
  ALT_COMMENT,
  ALT_MNEM0,
  ALT_MNEM0A,
  ALT_MNEM1,
  ALT_MNEM1A,
  ALT_MNEM1B,
  ALT_MNEM2,
  ALT_MNEM2A,
  ALT_MNEM3
} altPatterns;

static char alt_comment[]   = { PCP_COMMENT, 0};
static char alt_label[]     = { PCP_LABEL, 0};
static char alt_mnem0[]     = { PCP_STR, 0};
static char alt_mnem0a[]    = { PCP_WILDVAR, 0};
static char alt_mnem1[]     = { PCP_STR, PCP_STR, 0};
static char alt_mnem1a[]    = { PCP_STR, PCP_WILDVAR, 0};
static char alt_mnem1b[]    = { PCP_STR, PCP_NUMBER, 0};
static char alt_mnem2[]     = { PCP_STR, PCP_STR, PCP_COMMA, PCP_STR, 0};
static char alt_mnem2a[]    = { PCP_STR, PCP_WILDVAR, PCP_COMMA, PCP_STR, 0};
static char alt_mnem3[]     = { PCP_STR, PCP_STR, PCP_COMMA, PCP_NUMBER, 0};

static void * cvt_altpat_label(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_comment(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem0(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem0a(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem1(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem1a(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem1b(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem2(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem2a(void *pp,pCodeWildBlock *pcwb);
static void * cvt_altpat_mnem3(void *pp,pCodeWildBlock *pcwb);

static pcPattern altArr[] = {
  {ALT_LABEL,        alt_label,  cvt_altpat_label},
  {ALT_COMMENT,      alt_comment,cvt_altpat_comment},
  {ALT_MNEM3,        alt_mnem3,  cvt_altpat_mnem3},
  {ALT_MNEM2A,       alt_mnem2a, cvt_altpat_mnem2a},
  {ALT_MNEM2,        alt_mnem2,  cvt_altpat_mnem2},
  {ALT_MNEM1B,       alt_mnem1b, cvt_altpat_mnem1b},
  {ALT_MNEM1A,       alt_mnem1a, cvt_altpat_mnem1a},
  {ALT_MNEM1,        alt_mnem1,  cvt_altpat_mnem1},
  {ALT_MNEM0A,       alt_mnem0a, cvt_altpat_mnem0a},
  {ALT_MNEM0,        alt_mnem0,  cvt_altpat_mnem0},

};

#define ALTPATTERNS (sizeof(altArr)/sizeof(pcPattern))

// forward declarations
static void * DLL_append(_DLL *list, _DLL *next);

/*-----------------------------------------------------------------*/
/* cvt_extract_destination - helper function extracts the register */
/*                           destination from a parsedPattern.     */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int cvt_extract_destination(parsedPattern *pp)
{

  if(pp->pct[0].tt == PCT_STRING) {

    // just check first letter for now

    if(toupper(*pp->pct[0].tok.s) == 'F')
      return 1;

  } else if (pp->pct[0].tt == PCT_NUMBER) {

    if(pp->pct[0].tok.n)
      return 1;
  }

  return 0;

}

/*-----------------------------------------------------------------*/
/*  pCodeOp *cvt_extract_status(char *reg, char *bit)              */
/*     if *reg is the "status" register and *bit is one of the     */
/*     status bits, then this function will create a new pCode op  */
/*     containing the status register.                             */
/*-----------------------------------------------------------------*/

static pCodeOp *cvt_extract_status(char *reg, char *bit)
{
  int len;

  if(STRCASECMP(reg, pic16_pc_status.pcop.name))
    return NULL;

  len = strlen(bit);

  if(len == 1) {
    // check C,Z
    if(toupper(*bit) == 'C')
      return PCOP(pic16_popCopyGPR2Bit(&pic16_pc_status,PIC_C_BIT));
    if(toupper(*bit) == 'Z')
      return PCOP(pic16_popCopyGPR2Bit(&pic16_pc_status,PIC_Z_BIT));
  }

  // Check DC
  if(len ==2 && toupper(bit[0]) == 'D' && toupper(bit[1]) == 'C')
    return PCOP(pic16_popCopyGPR2Bit(&pic16_pc_status,PIC_DC_BIT));

  return NULL;

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_label - convert assembly line type to a pCode label  */
/* INPUT: pointer to the parsedPattern                             */
/*                                                                 */
/*  pp[0] - label                                                  */
/*                                                                 */
/* label pattern => '%' number ':'                                 */
/* at this point, we wish to extract only the 'number'             */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_label(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;

  DFPRINTF((stderr,"altpat_label with ID = %d\n",p->pct[1].tok.n));
  return pic16_newpCodeLabel(NULL,-p->pct[1].tok.n);

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_comment - convert assembly line type to a comment    */
/* INPUT: pointer to the parsedPattern                             */
/*                                                                 */
/*  pp[0] - comment                                                */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_comment(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;

  DFPRINTF((stderr,"altpat_comment  = %s\n",p->pct[0].tok.s));
  return pic16_newpCodeCharP(p->pct[0].tok.s);

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem0  - convert assembly line type to a wild pCode   */
/*                    instruction                                  */
/*                                                                 */
/*  pp[0] - str                                                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem0(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;

  DFPRINTF((stderr,"altpat_mnem0 %s\n",  p->pct[0].tok.s));

  opcode = pic16_getpCode(p->pct[0].tok.s,0);

  if(opcode < 0) {
    /* look for special command strings like _NOTBITSKIP_ */

    //fprintf(stderr, "Bad mnemonic\n");

    opcode  = pic16_getpCodePeepCommand(p->pct[0].tok.s);
    //if(opcode > 0)
    //  fprintf(stderr,"  but valid peep command: %s, key = %d\n",p->pct[0].tok.s,opcode);
    return NULL;
  }

  pci = PCI(pic16_newpCode(opcode, NULL));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem0a - convert assembly line type to a wild pCode   */
/*                    instruction                                  */
/*                                                                 */
/*  pp[0] - wild var                                               */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem0a(void *pp, pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;

  DFPRINTF((stderr,"altpat_mnem0a wild mnem # %d\n",  p[0].pct[1].tok.n));

  /* Save the index of the maximum wildcard mnemonic */

  //if(p[0].pct[1].tok.n > sMaxWildVar)
  //  sMaxWildMnem = p[0].pct[1].tok.n;

  if(p[0].pct[1].tok.n > pcwb->nwildpCodes)
    pcwb->nwildpCodes = p[0].pct[1].tok.n;

  return pic16_newpCodeWild(p[0].pct[1].tok.n,NULL,NULL);

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem1 - convert assembly line type to a pCode         */
/*                   instruction with 1 operand.                   */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - Operand                                                */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem1(void *pp,pCodeWildBlock *pcwb)
{

  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  DFPRINTF((stderr,"altpat_mnem1 %s var %s\n",  p->pct[0].tok.s,p[1].pct[0].tok.s));

  opcode = pic16_getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    //fprintf(stderr, "Bad mnemonic\n");
    opcode  = pic16_getpCodePeepCommand(p->pct[0].tok.s);
    //if(opcode > 0)
    //fprintf(stderr,"  but valid peep command: %s, key = %d\n",p->pct[0].tok.s,opcode);

    return NULL;
  }

  if(pic16Mnemonics[opcode]->isBitInst)
    pcosubtype = pic16_newpCodeOp(p[1].pct[0].tok.s,PO_BIT);
  else
    pcosubtype = pic16_newpCodeOp(p[1].pct[0].tok.s,PO_GPR_REGISTER);


  pci = PCI(pic16_newpCode(opcode, pcosubtype));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem1a - convert assembly line type to a pCode        */
/*                    instruction with 1 wild operand.             */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - wild var                                               */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem1a(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  DFPRINTF((stderr,"altpat_mnem1a %s var %d\n",  p->pct[0].tok.s,p[1].pct[1].tok.n));

  opcode = pic16_getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    int cmd_id = pic16_getpCodePeepCommand(p->pct[0].tok.s);
    pCode *pc=NULL;

    if(cmd_id<0) {
      fprintf(stderr, "Bad mnemonic\n");
      return NULL;
    }

    if(p[0].pct[1].tok.n > pcwb->nwildpCodes)
      pcwb->nwildpCodes = p[0].pct[1].tok.n;

    pc =  pic16_newpCodeWild(p[1].pct[1].tok.n,NULL,NULL);

    switch(cmd_id) {
    case NOTBITSKIP:
      PCW(pc)->mustNotBeBitSkipInst = 1;
      break;
    case BITSKIP:
      PCW(pc)->mustBeBitSkipInst = 1;
      break;
    case INVERTBITSKIP:
      PCW(pc)->invertBitSkipInst = 1;
    }
    return pc;
  }

  if(pic16Mnemonics[opcode]->isBitInst)
    pcosubtype = pic16_newpCodeOpBit(NULL,-1,0);
  else
    pcosubtype = pic16_newpCodeOp(NULL,PO_GPR_REGISTER);


  pci = PCI(pic16_newpCode(opcode,
		     pic16_newpCodeOpWild(p[1].pct[1].tok.n, pcwb, pcosubtype)));

  /* Save the index of the maximum wildcard variable */
  //if(p[1].pct[1].tok.n > sMaxWildVar)
  //  sMaxWildVar = p[1].pct[1].tok.n;

  if(p[1].pct[1].tok.n > pcwb->nvars)
    pcwb->nvars = p[1].pct[1].tok.n;

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem1b(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;

  DFPRINTF((stderr,"altpat_mnem1b %s var %d\n",  p->pct[0].tok.s,p[1].pct[0].tok.n));

  opcode = pic16_getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  pci = PCI(pic16_newpCode(opcode, pic16_newpCodeOpLit(p[1].pct[0].tok.n) ));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mnem2                                                */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - var                                                    */
/*  pp[2] - comma                                                  */
/*  pp[3] - destination                                            */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem2(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;
  int dest;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  dest = cvt_extract_destination(&p[3]);

  DFPRINTF((stderr,"altpat_mnem2 %s var %s destination %s(%d)\n",
	  p->pct[0].tok.s,
	  p[1].pct[0].tok.s,
	  p[3].pct[0].tok.s,
	  dest));


  opcode = pic16_getpCode(p->pct[0].tok.s,dest);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic16Mnemonics[opcode]->isBitInst) {
    pcosubtype = cvt_extract_status(p[1].pct[0].tok.s, p[3].pct[0].tok.s);
    if(pcosubtype == NULL) {
      fprintf(stderr, "bad operand?\n");
      return NULL;
    }
      
  } else
    pcosubtype = pic16_newpCodeOp(p[1].pct[0].tok.s,PO_GPR_REGISTER);


  pci = PCI(pic16_newpCode(opcode,pcosubtype));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");

  return pci;

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem2a - convert assembly line type to a pCode        */
/*                    instruction with 1 wild operand and a        */
/*                    destination operand (e.g. w or f)            */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - wild var                                               */
/*  pp[2] - comma                                                  */
/*  pp[3] - destination                                            */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem2a(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;
  int dest;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  if(!pcwb) {
    fprintf(stderr,"ERROR %s:%d - can't assemble line\n",__FILE__,__LINE__);
    return NULL;
  }

  dest = cvt_extract_destination(&p[3]);

  DFPRINTF((stderr,"altpat_mnem2a %s var %d destination %s(%d)\n",
	  p->pct[0].tok.s,
	  p[1].pct[1].tok.n,
	  p[3].pct[0].tok.s,
	  dest));


  opcode = pic16_getpCode(p->pct[0].tok.s,dest);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic16Mnemonics[opcode]->isBitInst)
    pcosubtype = pic16_newpCodeOp(NULL,PO_BIT);
  else
    pcosubtype = pic16_newpCodeOp(NULL,PO_GPR_REGISTER);


  pci = PCI(pic16_newpCode(opcode,
		     pic16_newpCodeOpWild(p[1].pct[1].tok.n, pcwb, pcosubtype)));

  /* Save the index of the maximum wildcard variable */
  //if(p[1].pct[1].tok.n > sMaxWildVar)
  //  sMaxWildVar = p[1].pct[1].tok.n;

  if(p[1].pct[1].tok.n > pcwb->nvars)
    pcwb->nvars = p[1].pct[1].tok.n;

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");

  return pci;

}


/*-----------------------------------------------------------------*/
/* cvt_altpat_mem3 -  convert assembly line type to a pCode        */
/*                    This rule is for bsf/bcf type instructions   */
/*                                                                 */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - register                                               */
/*  pp[2] - comma                                                  */
/*  pp[3] - number                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem3(void *pp,pCodeWildBlock *pcwb)
{
  parsedPattern *p = pp;
  int opcode;
  int dest;  // or could be bit position in the register

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype=NULL;

  dest = cvt_extract_destination(&p[3]);

  DFPRINTF((stderr,"altpat_mnem3 %s var %s bit (%d)\n",
	  p->pct[0].tok.s,
	  p[1].pct[0].tok.s,
	  p[3].pct[0].tok.n));


  opcode = pic16_getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }


  if(pic16Mnemonics[opcode]->isBitInst) {
    //pcosubtype = cvt_extract_status(p[1].pct[0].tok.s, p[3].pct[0].tok.s);

    //if(pcosubtype == NULL) {
    pcosubtype = pic16_newpCodeOpBit(p[1].pct[0].tok.s,p[3].pct[0].tok.n,0);
    //}
  } else
    pcosubtype = pic16_newpCodeOp(p[1].pct[0].tok.s,PO_GPR_REGISTER);

  if(pcosubtype == NULL) {
    fprintf(stderr, "Bad operand\n");
    return NULL;
  }

  pci = PCI(pic16_newpCode(opcode, pcosubtype));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");

  return pci;

}

/*-----------------------------------------------------------------*/
/* tokenizeLineNode - Convert a string (of char's) that was parsed */
/*                    by SDCCpeeph.c into a string of tokens.      */
/*                                                                 */
/*                                                                 */
/* The tokenizer is of the classic type. When an item is encounterd*/
/* it is converted into a token. The token is a structure that     */
/* encodes the item's type and it's value (when appropriate).      */
/*                                                                 */
/* Accepted token types:                                           */
/*    SPACE  NUMBER STRING  %  : ,  ;                              */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/


static void tokenizeLineNode(char *ln)
{
  char *lnstart=ln;
  tokIdx = 0;               // Starting off at the beginning
  tokArr[0].tt = PCT_NULL;  // and assume invalid character for first token.

  if(!ln || !*ln)
    return;


  while(*ln) {

    if(isspace(*ln)) {
      // add a SPACE token and eat the extra spaces.
      tokArr[tokIdx++].tt = PCT_SPACE;
      while (isspace (*ln))
	ln++;
      continue;
    }

    if(isdigit(*ln)) {

      tokArr[tokIdx].tt = PCT_NUMBER;
      tokArr[tokIdx++].tok.n = strtol(ln, &ln, 0);

      continue;

    }

    switch(*ln) {
    case '%':
      tokArr[tokIdx++].tt = PCT_PERCENT;
      break;
    case '<':
      tokArr[tokIdx++].tt = PCT_LESSTHAN;
      break;
    case '>':
      tokArr[tokIdx++].tt = PCT_GREATERTHAN;
      break;
    case ':':
      tokArr[tokIdx++].tt = PCT_COLON;
      break;
    case ';':
      tokArr[tokIdx].tok.s = Safe_strdup(ln);
      tokArr[tokIdx++].tt = PCT_COMMENT;
      tokArr[tokIdx].tt = PCT_NULL;
      return;
    case ',':
      tokArr[tokIdx++].tt = PCT_COMMA;
      break;


    default:
      if(isalpha(*ln) || (*ln == '_') ) {
	char buffer[50];
	int i=0;

	while( (isalpha(*ln)  ||  isdigit(*ln) || (*ln == '_')) && i<49)
	  buffer[i++] = *ln++;

	ln--;
	buffer[i] = 0;

	tokArr[tokIdx].tok.s = Safe_strdup(buffer);
	tokArr[tokIdx++].tt = PCT_STRING;

      } else {
	fprintf(stderr, "Error while parsing peep rules (check peeph.def)\n");
	fprintf(stderr, "Line: %s\n",lnstart);
	fprintf(stderr, "Token: '%c'\n",*ln);
	exit(1);
      }
    }

    /* Advance to next character in input string .
     * Note, if none of the tests passed above, then 
     * we effectively ignore the `bad' character.
     * Since the line has already been parsed by SDCCpeeph,
     * chance are that there are no invalid characters... */

    ln++;

  }

  tokArr[tokIdx].tt = 0;
}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/



static void dump1Token(pCodeTokens tt)
{

  switch(tt) {
  case PCT_SPACE:
    fprintf(stderr, " space ");
    break;
  case PCT_PERCENT:
    fprintf(stderr, " pct %%");
    break;
  case PCT_LESSTHAN:
    fprintf(stderr, " pct <");
    break;
  case PCT_GREATERTHAN:
    fprintf(stderr, " pct >");
    break;
  case PCT_COLON:
    fprintf(stderr, " col :");
    break;
  case PCT_COMMA:
    fprintf(stderr, " comma , ");
    break;
  case PCT_COMMENT:
    fprintf(stderr, " comment ");
    //fprintf(stderr,"%s",tokArr[i].tok.s);
    break;
  case PCT_STRING:
    fprintf(stderr, " str ");
    //fprintf(stderr,"%s",tokArr[i].tok.s);
    break;
  case PCT_NUMBER:
    fprintf(stderr, " num ");
    //fprintf(stderr,"%d",tokArr[i].tok.n);
    break;
  case PCT_NULL:
    fprintf(stderr, " null ");

  }

}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

static int pcComparePattern(pCodeToken *pct, char *pat, int max_tokens)
{
  int i=0;

  if(!pct || !pat || !*pat)
    return 0;

  //DFPRINTF((stderr,"comparing against:\n"));

  while(i < max_tokens) {

    if(*pat == 0){
      //DFPRINTF((stderr,"matched\n"));
      return (i+1);
    }

    //dump1Token(*pat); DFPRINTF((stderr,"\n"));

    if(pct->tt != *pat) 
      return 0;


    pct++;
    pat++;
  }

  return 0;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

static int altComparePattern( char *pct, parsedPattern *pat, int max_tokens)
{
  int i=0;
  
  if(!pct || !pat || !*pct)
    return 0;


  while(i < max_tokens) {

    if(*pct == 0) {
      //DFPRINTF((stderr,"matched\n"));
      return i;
    }

    //dump1Token(*pat); DFPRINTF((stderr,"\n"));

    if( !pat || !pat->pcp )
      return 0;

    if (pat->pcp->pt != *pct)  
      return 0;

    //DFPRINTF((stderr," pct=%d\n",*pct));
    pct++;
    pat++;
    i++;
  }

  return 0;

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

static int advTokIdx(int *v, int amt)
{

  if((unsigned) (*v + amt) > tokIdx)
    return 1;

  *v += amt;
  return 0;

}

/*-----------------------------------------------------------------*/
/* parseTokens - convert the tokens corresponding to a single line */
/*               of a peep hole assembly into a pCode object.      */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/* This is a simple parser that looks for strings of the type      */
/* allowed in the peep hole definition file. Essentially the format*/
/* is the same as a line of assembly:                              */
/*                                                                 */
/*  label:    mnemonic   op1, op2, op3    ; comment                */
/*                                                                 */
/* Some of these items aren't present. It's the job of the parser  */
/* to determine which are and convert those into the appropriate   */
/* pcode.                                                          */
/*-----------------------------------------------------------------*/

static int parseTokens(pCodeWildBlock *pcwb, pCode **pcret)
{
  pCode *pc;
  int error = 0;

  if(!tokIdx)
    return error;

#ifdef PCODE_DEBUG
  {
    unsigned i;
    for(i=0; i<=tokIdx; i++)
      dump1Token(tokArr[i].tt);
    fputc('\n',stderr);
  }
#endif

  {
    int lparsedPatIdx=0;
    int lpcpIdx;
    int ltokIdx =0;
    int matching = 0;
    int j=0;
    int k=0;

    char * cPmnem  = NULL;     // Pointer to non-wild mnemonic (if any)
    char * cP1stop = NULL;
    char * cP2ndop = NULL;

    //pCodeOp *pcl   = NULL;       // Storage for a label
    //pCodeOp *pco1  = NULL;       // 1st operand
    //pCodeOp *pco2  = NULL;       // 2nd operand
    //pCode   *pc    = NULL;       // Mnemonic

    typedef enum {
      PS_START,
      PS_HAVE_LABEL,
      PS_HAVE_MNEM,
      PS_HAVE_1OPERAND,
      PS_HAVE_COMMA,
      PS_HAVE_2OPERANDS
    } ParseStates;

    ParseStates state = PS_START;

    do {

      lpcpIdx=0;
      matching = 0;

      if(  ((tokArr[ltokIdx].tt == PCT_SPACE) )
	   && (advTokIdx(&ltokIdx, 1)) ) // eat space
	break;

      do {
	j = pcComparePattern(&tokArr[ltokIdx], pcpArr[lpcpIdx].tokens, tokIdx +1);
	if( j ) {

	  switch(pcpArr[lpcpIdx].pt) {
	  case  PCP_LABEL:
	    if(state == PS_START){
	      DFPRINTF((stderr,"  label\n"));
	      state = PS_HAVE_LABEL;
	    } else 
	      DFPRINTF((stderr,"  bad state (%d) for label\n",state));
	    break;

	  case  PCP_STR:
	    DFPRINTF((stderr,"  %s is",tokArr[ltokIdx].tok.s));
	    switch(state) {
	    case PS_START:
	    case PS_HAVE_LABEL:
	      DFPRINTF((stderr,"  mnem\n"));
	      cPmnem = tokArr[ltokIdx].tok.s;
	      state = PS_HAVE_MNEM;
	      break;
	    case PS_HAVE_MNEM:
	      DFPRINTF((stderr,"  1st operand\n"));
	      cP1stop = tokArr[ltokIdx].tok.s;
	      //pco1 = pic16_newpCodeOp(NULL,PO_GPR_REGISTER);
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      DFPRINTF((stderr,"  error expecting comma\n"));
	      break;
	    case PS_HAVE_COMMA:
	      DFPRINTF((stderr,"  2 operands\n"));
	      cP2ndop = tokArr[ltokIdx].tok.s;
	      break;
	    case PS_HAVE_2OPERANDS:
	      break;
	    }
	    break;

	  case  PCP_WILDVAR:
	    switch(state) {
	    case PS_START:
	    case PS_HAVE_LABEL:
	      DFPRINTF((stderr,"  wild mnem\n"));
	      state = PS_HAVE_MNEM;
	      break;
	    case PS_HAVE_MNEM:
	      DFPRINTF((stderr,"  1st operand is wild\n"));
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      DFPRINTF((stderr,"  error expecting comma\n"));
	      break;
	    case PS_HAVE_COMMA:
	      DFPRINTF((stderr,"  2nd operand is wild\n"));
	      break;
	    case PS_HAVE_2OPERANDS:
	      break;
	    }
	    break;

	  case  PCP_NUMBER:
	    switch(state) {
	    case PS_START:
	    case PS_HAVE_LABEL:
	      fprintf(stderr,"  ERROR number\n");
	      break;
	    case PS_HAVE_MNEM:
	      DFPRINTF((stderr,"  1st operand is a number\n"));
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      fprintf(stderr,"  error expecting comma\n");
	      break;
	    case PS_HAVE_COMMA:
	      DFPRINTF((stderr,"  2nd operand is a number\n"));
	      break;
	    case PS_HAVE_2OPERANDS:
	      break;
	    }
	    break;

	  case  PCP_WILDSTR:
	    break;
	  case  PCP_COMMA:
	    if(state == PS_HAVE_1OPERAND){
	      DFPRINTF((stderr,"  got a comma\n"));
	      state = PS_HAVE_COMMA;
	    } else
	      fprintf(stderr,"  unexpected comma\n");
	    break;

	  }

	  matching = 1;
	  parsedPatArr[lparsedPatIdx].pcp = &pcpArr[lpcpIdx];
	  parsedPatArr[lparsedPatIdx].pct = &tokArr[ltokIdx];
	  lparsedPatIdx++;

	  //dump1Token(tokArr[ltokIdx].tt);

	  if(advTokIdx(&ltokIdx, strlen(pcpArr[lpcpIdx].tokens) ) ) {
	    DFPRINTF((stderr," reached end \n"));
	    matching = 0;
	    //return;
	  }
	}


      } while ((++lpcpIdx < PCPATTERNS) && !matching);

    } while (matching);

    parsedPatArr[lparsedPatIdx].pcp = NULL;
    parsedPatArr[lparsedPatIdx].pct = NULL;

    j=k=0;
    do {
      int c;

      if( (c=altComparePattern( altArr[k].tokens, &parsedPatArr[j],10) ) ) {

	if( altArr[k].f) {
	  pc = altArr[k].f(&parsedPatArr[j],pcwb);
	  //if(pc && pc->print)
	  //  pc->print(stderr,pc);
	  //if(pc && pc->destruct) pc->destruct(pc); dumps core?

	  //if(curBlock && pc)
	  //pic16_addpCode2pBlock(curBlock, pc);
	  if(pc) {
	    if (pcret) {
	      *pcret = pc;
	      return 0;       // Only accept one line for now.
	    } else
	      pic16_addpCode2pBlock(pcwb->pb, pc);
	  } else
	    error++;
	}
	j += c;
      }
      k++;
    }
    while(j<=lparsedPatIdx && k<ALTPATTERNS);

/*
    DFPRINTF((stderr,"\nConverting parsed line to pCode:\n\n"));

    j = 0;
    do {
      if(parsedPatArr[j].pcp && parsedPatArr[j].pcp->f )
	parsedPatArr[j].pcp->f(&parsedPatArr[j]);
      DFPRINTF((stderr,"  %d",parsedPatArr[j].pcp->pt));
      j++;
    }
    while(j<lparsedPatIdx);
*/
    DFPRINTF((stderr,"\n"));

  }

  return error;
}

/*-----------------------------------------------------------------*/
/*                                                                 */
/*-----------------------------------------------------------------*/
static void  peepRuleBlock2pCodeBlock(  lineNode *ln, pCodeWildBlock *pcwb)
{

  if(!ln)
    return;

  for( ; ln; ln = ln->next) {

    //DFPRINTF((stderr,"%s\n",ln->line));

    tokenizeLineNode(ln->line);
    
    if(parseTokens(pcwb,NULL)) {
      int i;
      fprintf(stderr,"ERROR assembling line:\n%s\n",ln->line);
      fprintf(stderr,"Tokens:\n");
      for(i=0; i<5; i++)
	dump1Token(tokArr[i].tt);
      fputc('\n',stderr);
      exit (1);
    }
  }
}

/*-----------------------------------------------------------------*/
/*                                                                 */
/*-----------------------------------------------------------------*/
pCode *pic16_AssembleLine(char *line)
{
  pCode *pc=NULL;

  if(!line || !*line) {
    fprintf(stderr,"WARNING returning NULL in AssembleLine\n");
    return NULL;
  }

  tokenizeLineNode(line);
    
  if(parseTokens(NULL,&pc))
    fprintf(stderr, "WARNING: unable to assemble line:\n%s\n",line);

  return pc;

}

/*-----------------------------------------------------------------*/
/* peepRuleCondition                                               */
/*-----------------------------------------------------------------*/
static void   peepRuleCondition(char *cond, pCodePeep *pcp)
{
  if(!cond || !pcp)
    return;

  //DFPRINTF((stderr,"\nCondition:  %s\n",cond));
  /* brute force compares for now */

  if(STRCASECMP(cond, "NZ") == 0) {
    //DFPRINTF((stderr,"found NZ\n"));
    pcp->postFalseCond = PCC_Z;

  }

}


static void initpCodeWildBlock(pCodeWildBlock *pcwb)
{
  
  //  pcwb = Safe_calloc(1,sizeof(pCodeWildBlock));

  if(!pcwb)
    return;

  pcwb->vars = NULL; 
  pcwb->wildpCodes = NULL;
  pcwb->wildpCodeOps = NULL;

  pcwb->nvars = 0; 
  pcwb->nwildpCodes = 0;
  pcwb->nops = 0;

}

static void postinit_pCodeWildBlock(pCodeWildBlock *pcwb)
{
  
  if(!pcwb)
    return;

  pcwb->nvars+=2;
  pcwb->nops = pcwb->nvars;

  pcwb->vars = Safe_calloc(pcwb->nvars, sizeof(char *));
  pcwb->wildpCodeOps = Safe_calloc(pcwb->nvars, sizeof(pCodeOp *));

  pcwb->nwildpCodes+=2;
  pcwb->wildpCodes = Safe_calloc(pcwb->nwildpCodes, sizeof(pCode *));

}

static void initpCodePeep(pCodePeep *pcp)
{
  
  //  pcwb = Safe_calloc(1,sizeof(pCodeWildBlock));

  if(!pcp)
    return;

  initpCodeWildBlock(&pcp->target);
  pcp->target.pb = pic16_newpCodeChain(NULL, 'W', NULL);

  initpCodeWildBlock(&pcp->replace);
  pcp->replace.pb = pic16_newpCodeChain(NULL, 'W', NULL);

}

/*-----------------------------------------------------------------*/
/* peepRules2pCode - parse the "parsed" peep hole rules to generate*/
/*                   pCode.                                        */
/*                                                                 */
/* SDCCpeeph parses the peep rules file and extracts variables,    */
/* removes white space, and checks the syntax. This function       */
/* extends that processing to produce pCode objects. You can kind  */
/* think of this function as an "assembler", though instead of     */
/* taking raw text to produce machine code, it produces pCode.     */
/*                                                                 */
/*-----------------------------------------------------------------*/
extern void pic16initpCodePeepCommands(void);

void  pic16_peepRules2pCode(peepRule *rules)
{
  peepRule *pr;

  pCodePeep *currentRule;
  pCodePeepSnippets *pcps;

  pic16initpCodePeepCommands();

  /* The rules are in a linked-list. Each rule has two portions */
  /* There's the `target' and there's the `replace'. The target */
  /* is compared against the SDCC generated code and if it      */
  /* matches, it gets replaced by the `replace' block of code.  */
  /*                                                            */
  /* Here we loop through each rule and convert the target's and*/
  /* replace's into pCode target and replace blocks             */

  for (pr = rules; pr; pr = pr->next) {

    //DFPRINTF((stderr,"\nRule:\n\n"));

    pcps = Safe_calloc(1,sizeof(pCodePeepSnippets));
    peepSnippets = DLL_append((_DLL*)peepSnippets,(_DLL*)pcps);

    currentRule = pcps->peep  = Safe_calloc(1,sizeof(pCodePeep));
    initpCodePeep(currentRule);

    /* Convert the target block */
    peepRuleBlock2pCodeBlock(pr->match, &currentRule->target);

    //DFPRINTF((stderr,"finished target, here it is in pcode form:\n"));
    //pic16_printpBlock(stderr, currentRule->target.pb);

    //DFPRINTF((stderr,"target with labels merged:\n"));
    //pic16_pBlockMergeLabels(curBlock);
    pic16_pBlockMergeLabels(currentRule->target.pb);
    //pic16_printpBlock(stderr, currentRule->replace.pb);

    //#ifdef PCODE_DEBUG
    //    pic16_printpBlock(stderr, curBlock);
    //#endif
    //DFPRINTF((stderr,"\nReplaced by:\n"));


    /* Convert the replace block */
    peepRuleBlock2pCodeBlock(pr->replace, &currentRule->replace);

    //DFPRINTF((stderr,"finished replace block, here it is in pcode form:\n"));
    //pic16_printpBlock(stderr, curBlock);

    //DFPRINTF((stderr,"replace with labels merged:\n"));

    pic16_pBlockMergeLabels(currentRule->replace.pb);
    //pic16_printpBlock(stderr, currentRule->replace.pb);

    peepRuleCondition(pr->cond,currentRule);

    /* The rule has been converted to pCode. Now allocate
     * space for the wildcards */
    
    postinit_pCodeWildBlock(&currentRule->target);
    postinit_pCodeWildBlock(&currentRule->replace);

    //return; // debug ... don't want to go through all the rules yet
  }

  {
    pCodePeep *peepBlock;
    _DLL *peeprules;

    peeprules = (_DLL *)peepSnippets;
    //fprintf(stderr,"target rules\n");
    while(peeprules) {
      //fprintf(stderr,"   rule:\n");
      peepBlock = ((pCodePeepSnippets*)peeprules)->peep;
      //pic16_printpBlock(stderr, peepBlock->target.pb);
      peeprules = peeprules->next;
    }
    //fprintf(stderr," ... done\n");
  }

}
#if 0
static void printpCodeString(FILE *of, pCode *pc, int max)
{
  int i=0;

  while(pc && (i++<max)) {
    pc->print(of,pc);
    pc = pc->next;
  }
}
#endif
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



/*-----------------------------------------------------------------*/
/* pCodeSearchCondition - Search a pCode chain for a 'condition'   */
/*                                                                 */
/* return conditions                                               */
/*  1 - The Condition was found for a pCode's input                */
/*  0 - No matching condition was found for the whole chain        */
/* -1 - The Condition was found for a pCode's output               */
/*                                                                 */
/*-----------------------------------------------------------------*/
int pic16_pCodeSearchCondition(pCode *pc, unsigned int cond)
{
  //fprintf(stderr,"Checking conditions %d\n",cond);
  while(pc) {

    /* If we reach a function end (presumably an end since we most
       probably began the search in the middle of a function), then
       the condition was not found. */
    if(pc->type == PC_FUNCTION)
      return 0;

    if(pc->type == PC_OPCODE) {
      //fprintf(stderr," checking conditions of: ");
      //pc->print(stderr,pc);
      //fprintf(stderr,"\t\tinCond=%d\toutCond=%d\n",PCI(pc)->inCond,PCI(pc)->outCond);
      if(PCI(pc)->inCond & cond)
	return 1;
      if(PCI(pc)->outCond & cond)
	return -1;
    }

    pc = pc->next;
  }

  return 0;
}

/*-----------------------------------------------------------------
 * int pCodeOpCompare(pCodeOp *pcops, pCodeOp *pcopd)
 *
 * Compare two pCodeOp's and return 1 if they're the same
 *-----------------------------------------------------------------*/
static int pCodeOpCompare(pCodeOp *pcops, pCodeOp *pcopd)
{
  char b[50], *n2;

  if(!pcops || !pcopd)
    return 0;
/*
  fprintf(stderr," Comparing operands %s",
	  pic16_get_op( pcops,NULL,0));

  fprintf(stderr," to %s\n",
	  pic16_get_op( pcopd,NULL,0));
*/

  if(pcops->type != pcopd->type) {
    //fprintf(stderr,"  - fail - diff types\n");
    return 0;  // different types
  }

  if(pcops->type == PO_LITERAL) {

    if((PCOL(pcops)->lit >= 0) && (PCOL(pcops)->lit == PCOL(pcopd)->lit))
      return 1;

    return 0;
  }

  b[0]=0;
  pic16_get_op(pcops,b,50);

  n2 = pic16_get_op(pcopd,NULL,0);

  if( !n2 || strcmp(b,n2)) {
    //fprintf(stderr,"  - fail - diff names: %s, len=%d,  %s, len=%d\n",b,strlen(b), n2, strlen(n2) );
    return 0;  // different names
  }

  switch(pcops->type) {
  case PO_DIR:
    if( PCOR(pcops)->instance != PCOR(pcopd)->instance) {
      //fprintf(stderr, "  - fail different instances\n");
      return 0;
    }
    break;
  default:
    break;
  }

  //fprintf(stderr,"  - pass\n");

  return 1;
}

static int pCodePeepMatchLabels(pCodePeep *peepBlock, pCode *pcs, pCode *pcd)
{
  int labindex;

  /* Check for a label associated with this wild pCode */
  // If the wild card has a label, make sure the source code does too.
  if(PCI(pcd)->label) {
    pCode *pcl = PCI(pcd)->label->pc;

#ifdef PCODE_DEBUG
    int li = -PCL(pcl)->key;

    if(peepBlock->target.vars[li] == NULL) {
      if(PCI(pcs)->label) {
	DFPRINTF((stderr,"first time for a label: %d %s\n",li,PCL(PCI(pcs)->label->pc)->label));
      }
    } else {
      // DFPRINTF((stderr,"label id = %d \n",PCL(PCI(pcd)->label->pc)->key));
      DFPRINTF((stderr," label id: %d %s\n",li,peepBlock->target.vars[li]));
      if(PCI(pcs)->label) {
	DFPRINTF((stderr," src %s\n",PCL(PCI(pcs)->label->pc)->label));
      }
    }
#endif


    if(!PCI(pcs)->label)
      return 0;

    labindex = -PCL(pcl)->key;
    if(peepBlock->target.vars[labindex] == NULL) {
      // First time to encounter this label
      peepBlock->target.vars[labindex] = PCL(PCI(pcs)->label->pc)->label;
      DFPRINTF((stderr,"first time for a label: %d %s\n",labindex,PCL(PCI(pcs)->label->pc)->label));

    } else {
      if(strcmp(peepBlock->target.vars[labindex],PCL(PCI(pcs)->label->pc)->label) != 0) {
	DFPRINTF((stderr,"labels don't match dest %s != src %s\n",peepBlock->target.vars[labindex],PCL(PCI(pcs)->label->pc)->label));
	return 0;
      }
      DFPRINTF((stderr,"matched a label %d %s -hey\n",labindex,peepBlock->target.vars[labindex]));
    }
  } else {
    //DFPRINTF((stderr,"destination doesn't have a label\n"));

    if(PCI(pcs)->label)
      return 0;

    //DFPRINTF((stderr,"neither src nor dest have labels\n"));

  }

  return 1;
    
}

/*-----------------------------------------------------------------*/
/* pCodePeepMatchLine - Compare source and destination pCodes to   */
/*                      see they're the same.                      */
/*                                                                 */
/* In this context, "source" refers to the coded generated by gen.c*/
/* and "destination" refers to a pcode in a peep rule. If the dest-*/
/* ination has no wild cards, then MatchLine will compare the two  */
/* pcodes (src and dest) for a one-to-one match. If the destination*/
/* has wildcards, then those get expanded. When a wild card is     */
/* encountered for the first time it autmatically is considered a  */
/* match and the object that matches it is referenced in the       */
/* variables or opcodes array (depending on the type of match).    */
/*                                                                 */
/*                                                                 */
/* Inputs:                                                         */
/*  *peepBlock - A pointer to the peepBlock that contains the      */
/*               entire rule to which the destination pcode belongs*/
/*  *pcs - a pointer to the source pcode                           */
/*  *pcd - a pointer to the destination pcode                      */
/*                                                                 */
/* Returns:                                                        */
/*  1 - pcodes match                                               */
/*  0 - pcodes don't match                                         */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

static int pCodePeepMatchLine(pCodePeep *peepBlock, pCode *pcs, pCode *pcd)
{
  int index;   // index into wild card arrays

  /* one-for-one match. Here the source and destination opcodes 
   * are not wild. However, there may be a label or a wild operand */

  if(pcs) {
    if(PCI(pcs)->label) {
      DFPRINTF((stderr,"Match line source label: %s\n",PCL(PCI(pcs)->label->pc)->label));
    }
  }

  if(pcs->type == pcd->type) {

    if(pcs->type == PC_OPCODE) {

      /* If the opcodes don't match then the line doesn't match */
      if(PCI(pcs)->op != PCI(pcd)->op)
	return 0;

#ifdef PCODE_DEBUG
      DFPRINTF((stderr,"%s comparing\n",__FUNCTION__));
      pcs->print(stderr,pcs);
      pcd->print(stderr,pcd);
#endif

      if(!pCodePeepMatchLabels(peepBlock, pcs, pcd))
	return 0;

      /* Compare the operands */
      if(PCI(pcd)->pcop) {
	if (PCI(pcd)->pcop->type == PO_WILD) {
	  index = PCOW(PCI(pcd)->pcop)->id;
	  //DFPRINTF((stderr,"destination is wild\n"));
#ifdef DEBUG_PCODEPEEP
	  if (index > peepBlock->nops) {
	    DFPRINTF((stderr,"%s - variables exceeded\n",__FUNCTION__));
	    exit(1);
	  }
#endif

	  PCOW(PCI(pcd)->pcop)->matched = PCI(pcs)->pcop;
	  if(!peepBlock->target.wildpCodeOps[index]) {
	    peepBlock->target.wildpCodeOps[index] = PCI(pcs)->pcop;

	    //if(PCI(pcs)->pcop->type == PO_GPR_TEMP) 

	  } else {
	    /*
	      pcs->print(stderr,pcs);
	      pcd->print(stderr,pcd);

	      fprintf(stderr, "comparing operands of these instructions, result %d\n",
	      pCodeOpCompare(PCI(pcs)->pcop, peepBlock->target.wildpCodeOps[index])
	      );
	    */

	    return pCodeOpCompare(PCI(pcs)->pcop, peepBlock->target.wildpCodeOps[index]);
	  }

	  {
	    char *n;

	    switch(PCI(pcs)->pcop->type) {
	    case PO_GPR_TEMP:
	    case PO_FSR0:
	      //case PO_INDF0:
	      n = PCOR(PCI(pcs)->pcop)->r->name;

	      break;
	    default:
	      n = PCI(pcs)->pcop->name;
	    }

	    if(peepBlock->target.vars[index])
	      return  (strcmp(peepBlock->target.vars[index],n) == 0);
	    else {
	      DFPRINTF((stderr,"first time for a variable: %d, %s\n",index,n));
	      peepBlock->target.vars[index] = n;
	      return 1;
	    }
	  }

	} else if (PCI(pcd)->pcop->type == PO_LITERAL) {
	  return pCodeOpCompare(PCI(pcs)->pcop, PCI(pcd)->pcop);

	}
	/* FIXME - need an else to check the case when the destination 
	 * isn't a wild card */
      } else
	/* The pcd has no operand. Lines match if pcs has no operand either*/
	return (PCI(pcs)->pcop == NULL);
    }
  }

  /* Compare a wild instruction to a regular one. */

  if((pcd->type == PC_WILD) && (pcs->type == PC_OPCODE)) {

    index = PCW(pcd)->id;
#ifdef PCODE_DEBUG
    DFPRINTF((stderr,"%s comparing wild cards\n",__FUNCTION__));
    pcs->print(stderr,pcs);
    pcd->print(stderr,pcd);
#endif
    peepBlock->target.wildpCodes[PCW(pcd)->id] = pcs;

    if(!pCodePeepMatchLabels(peepBlock, pcs, pcd)) {
      DFPRINTF((stderr," Failing because labels don't match\n"));
      return 0;
    }

    if(PCW(pcd)->mustBeBitSkipInst & !(PCI(pcs)->isBitInst && PCI(pcs)->isSkip)) {
      // doesn't match because the wild pcode must be a bit skip
      DFPRINTF((stderr," Failing match because bit skip is req\n"));
      //pcd->print(stderr,pcd);
      //pcs->print(stderr,pcs);
      return 0;
    } 

    if(PCW(pcd)->mustNotBeBitSkipInst & (PCI(pcs)->isBitInst && PCI(pcs)->isSkip)) {
      // doesn't match because the wild pcode must *not* be a bit skip
      DFPRINTF((stderr," Failing match because shouldn't be bit skip\n"));
      //pcd->print(stderr,pcd);
      //pcs->print(stderr,pcs);
      return 0;
    } 

    if(PCW(pcd)->operand) {
      PCOW(PCI(pcd)->pcop)->matched = PCI(pcs)->pcop;
      if(peepBlock->target.vars[index]) {
	int i = (strcmp(peepBlock->target.vars[index],PCI(pcs)->pcop->name) == 0);
#ifdef PCODE_DEBUG

	if(i)
	  DFPRINTF((stderr," (matched)\n"));
	else {
	  DFPRINTF((stderr," (no match: wild card operand mismatch\n"));
	  DFPRINTF((stderr,"  peepblock= %s,  pcodeop= %s\n",
		  peepBlock->target.vars[index],
		  PCI(pcs)->pcop->name));
	}
#endif
	return i;
      } else {
	DFPRINTF((stderr," (matched %s\n",PCI(pcs)->pcop->name));
	peepBlock->target.vars[index] = PCI(pcs)->pcop->name;
	return 1;
      }
    }

    pcs = pic16_findNextInstruction(pcs->next); 
    if(pcs) {
      //DFPRINTF((stderr," (next to match)\n"));
      //pcs->print(stderr,pcs);
    } else if(pcd->next) {
      /* oops, we ran out of code, but there's more to the rule */
      return 0;
    }

    return 1; /*  wild card matches */
  }

  return 0;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pCodePeepClrVars(pCodePeep *pcp)
{

  int i;
  if(!pcp)
    return;
/*
  DFPRINTF((stderr," Clearing peep rule vars\n"));
  DFPRINTF((stderr," %d %d %d  %d %d %d\n",
	    pcp->target.nvars,pcp->target.nops,pcp->target.nwildpCodes,
	    pcp->replace.nvars,pcp->replace.nops,pcp->replace.nwildpCodes));
*/
  for(i=0;i<pcp->target.nvars; i++)
    pcp->target.vars[i] = NULL;
  for(i=0;i<pcp->target.nops; i++)
    pcp->target.wildpCodeOps[i] = NULL;
  for(i=0;i<pcp->target.nwildpCodes; i++)
    pcp->target.wildpCodes[i] = NULL;

  for(i=0;i<pcp->replace.nvars; i++)
    pcp->replace.vars[i] = NULL;
  for(i=0;i<pcp->replace.nops; i++)
    pcp->replace.wildpCodeOps[i] = NULL;
  for(i=0;i<pcp->replace.nwildpCodes; i++)
    pcp->replace.wildpCodes[i] = NULL;



}

/*-----------------------------------------------------------------*/
/*  pCodeInsertAfter - splice in the pCode chain starting with pc2 */
/*                     into the pCode chain containing pc1         */
/*-----------------------------------------------------------------*/
void pic16_pCodeInsertAfter(pCode *pc1, pCode *pc2)
{

  if(!pc1 || !pc2)
    return;

  pc2->next = pc1->next;
  if(pc1->next)
    pc1->next->prev = pc2;

  pc2->pb = pc1->pb;
  pc2->prev = pc1;
  pc1->next = pc2;

}

/*-----------------------------------------------------------------*/
/* pic16_pCodeOpCopy - copy a pcode operator                       */
/*-----------------------------------------------------------------*/
pCodeOp *pic16_pCodeOpCopy(pCodeOp *pcop)
{
  pCodeOp *pcopnew=NULL;

  if(!pcop)
    return NULL;

  switch(pcop->type) { 
  case PO_CRY:
  case PO_BIT:
    //DFPRINTF((stderr,"pCodeOpCopy bit\n"));
    pcopnew = Safe_calloc(1,sizeof(pCodeOpRegBit) );
    PCORB(pcopnew)->bit = PCORB(pcop)->bit;
    PCORB(pcopnew)->inBitSpace = PCORB(pcop)->inBitSpace;

    break;

  case PO_WILD:
    /* Here we expand the wild card into the appropriate type: */
    /* By recursively calling pCodeOpCopy */
    //DFPRINTF((stderr,"pCodeOpCopy wild\n"));
    if(PCOW(pcop)->matched)
      pcopnew = pic16_pCodeOpCopy(PCOW(pcop)->matched);
    else {
      // Probably a label
      pcopnew = pic16_pCodeOpCopy(PCOW(pcop)->subtype);
      pcopnew->name = Safe_strdup(PCOW(pcop)->pcwb->vars[PCOW(pcop)->id]);
      //DFPRINTF((stderr,"copied a wild op named %s\n",pcopnew->name));
    }

    return pcopnew;
    break;

  case PO_LABEL:
    //DFPRINTF((stderr,"pCodeOpCopy label\n"));
    pcopnew = Safe_calloc(1,sizeof(pCodeOpLabel) );
    PCOLAB(pcopnew)->key =  PCOLAB(pcop)->key;
    break;

  case PO_IMMEDIATE:
    pcopnew = Safe_calloc(1,sizeof(pCodeOpImmd) );
    PCOI(pcopnew)->index = PCOI(pcop)->index;
    PCOI(pcopnew)->offset = PCOI(pcop)->offset;
    PCOI(pcopnew)->_const = PCOI(pcop)->_const;
    break;

  case PO_LITERAL:
    //DFPRINTF((stderr,"pCodeOpCopy lit\n"));
    pcopnew = Safe_calloc(1,sizeof(pCodeOpLit) );
    PCOL(pcopnew)->lit = PCOL(pcop)->lit;
    break;

#if 0 // mdubuc - To add
  case PO_REL_ADDR:
    break;
#endif

  case PO_GPR_BIT:

    pcopnew = pic16_newpCodeOpBit(pcop->name, PCORB(pcop)->bit,PCORB(pcop)->inBitSpace);
    PCOR(pcopnew)->r = PCOR(pcop)->r;
    PCOR(pcopnew)->rIdx = PCOR(pcop)->rIdx;
    DFPRINTF((stderr," pCodeOpCopy Bit -register index\n"));
    return pcopnew;
    break;

  case PO_GPR_REGISTER:
  case PO_GPR_TEMP:
  case PO_FSR0:
  case PO_INDF0:
    //DFPRINTF((stderr,"pCodeOpCopy GPR register\n"));
    pcopnew = Safe_calloc(1,sizeof(pCodeOpReg) );
    PCOR(pcopnew)->r = PCOR(pcop)->r;
    PCOR(pcopnew)->rIdx = PCOR(pcop)->rIdx;
    PCOR(pcopnew)->instance = PCOR(pcop)->instance;
    DFPRINTF((stderr," register index %d\n", PCOR(pcop)->r->rIdx));
    break;

  case PO_DIR:
    //fprintf(stderr,"pCodeOpCopy PO_DIR\n");
    pcopnew = Safe_calloc(1,sizeof(pCodeOpReg) );
    PCOR(pcopnew)->r = PCOR(pcop)->r;
    PCOR(pcopnew)->rIdx = PCOR(pcop)->rIdx;
    PCOR(pcopnew)->instance = PCOR(pcop)->instance;
    break;
  case PO_STATUS:
    DFPRINTF((stderr,"pCodeOpCopy PO_STATUS\n"));
  case PO_BSR:
    DFPRINTF((stderr,"pCodeOpCopy PO_BSR\n"));
  case PO_SFR_REGISTER:
  case PO_STR:
  case PO_NONE:
  case PO_W:
  case PO_WREG:
  case PO_INTCON:
  case PO_PCL:
  case PO_PCLATH:
  case PO_REL_ADDR:
    //DFPRINTF((stderr,"pCodeOpCopy register type %d\n", pcop->type));
    pcopnew = Safe_calloc(1,sizeof(pCodeOp) );

  }

  pcopnew->type = pcop->type;
  if(pcop->name)
    pcopnew->name = Safe_strdup(pcop->name);
  else
    pcopnew->name = NULL;

  return pcopnew;
}


/*-----------------------------------------------------------------*/
/* pCodeCopy - copy a pcode                                        */
/*-----------------------------------------------------------------*/
static pCode *pCodeInstructionCopy(pCodeInstruction *pci,int invert)
{
  pCodeInstruction *new_pci;

  if(invert)
    new_pci = PCI(pic16_newpCode(pci->inverted_op,pci->pcop));
  else
    new_pci = PCI(pic16_newpCode(pci->op,pci->pcop));

  new_pci->pc.pb = pci->pc.pb;
  new_pci->from = pci->from;
  new_pci->to   = pci->to;
  new_pci->label = pci->label;
  new_pci->pcflow = pci->pcflow;

  return PCODE(new_pci);
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pCodeDeleteChain(pCode *f,pCode *t)
{
  pCode *pc;


  while(f && f!=t) {
    DFPRINTF((stderr,"delete pCode:\n"));
    pc = f->next;
    //f->print(stderr,f);
    //f->delete(f);  this dumps core...

    f = pc;

  }

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
int pic16_pCodePeepMatchRule(pCode *pc)
{
  pCodePeep *peepBlock;
  pCode *pct, *pcin;
  pCodeCSource *pc_cline=NULL;
  _DLL *peeprules;
  int matched;

  peeprules = (_DLL *)peepSnippets;

  while(peeprules) {
    peepBlock = ((pCodePeepSnippets*)peeprules)->peep;

    if(!peepBlock || /*!peepBlock->target ||*/ !peepBlock->target.pb->pcHead) {
      fprintf(stderr, "skipping rule because target pb is NULL\n");
      goto next_rule;
    }

    pCodePeepClrVars(peepBlock);
/*
    pcin = pc;
    if(IS_PCCOMMENT(pcin))
      pc = pcin = pic16_findNextInstruction(pcin->next);
*/
    pcin = pc = pic16_findNextInstruction(pc);

    pct = peepBlock->target.pb->pcHead;
#ifdef PCODE_DEBUG
    {
      pCode *pcr = peepBlock->replace.pb->pcHead;
      if(pcr) pct->print(stderr,pcr);
    }
#endif
    matched = 0;
    while(pct && pcin) {

      if(! (matched = pCodePeepMatchLine(peepBlock, pcin,pct)))
	break;

      pcin = pic16_findNextInstruction(pcin->next);
      pct = pct->next;
      //debug:
      //DFPRINTF((stderr,"    matched\n"));

      if(!pcin && pct) {
	DFPRINTF((stderr," partial match... no more code\n"));
	fprintf(stderr," partial match... no more code\n");
	matched = 0; 
      }
      if(!pct) {
	DFPRINTF((stderr," end of rule\n"));
      }
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
      DFPRINTF((stderr,"    matched rule so far, now checking conditions\n"));
      //pcin->print(stderr,pcin);
      
      if (pcin && peepBlock->postFalseCond && 
	  (pic16_pCodeSearchCondition(pcin,peepBlock->postFalseCond) > 0) )
	matched = 0;

      //fprintf(stderr," condition results = %d\n",pic16_pCodeSearchCondition(pcin,peepBlock->postFalseCond));


      //if(!matched) fprintf(stderr,"failed on conditions\n");
    }

    if(matched) {

      pCode *pcprev;
      pCode *pcr;


      /* We matched a rule! Now we have to go through and remove the
	 inefficient code with the optimized version */
#ifdef PCODE_DEBUG
      DFPRINTF((stderr, "Found a pcode peep match:\nRule:\n"));
      printpCodeString(stderr,peepBlock->target.pb->pcHead,10);
      DFPRINTF((stderr,"first thing matched\n"));
      pc->print(stderr,pc);
      if(pcin) {
	DFPRINTF((stderr,"last thing matched\n"));
	pcin->print(stderr,pcin);
      }
#endif


      /* Unlink the original code */
      pcprev = pc->prev;
      pcprev->next = pcin;
      if(pcin) 
	pcin->prev = pc->prev;


#if 0
      {
	/*     DEBUG    */
	/* Converted the deleted pCodes into comments */

	char buf[256];
	pCodeCSource *pc_cline2=NULL;

	buf[0] = ';';
	buf[1] = '#';

	while(pc &&  pc!=pcin) {

	  if(pc->type == PC_OPCODE && PCI(pc)->cline) {
	    if(pc_cline) {
	      pc_cline2->pc.next = PCODE(PCI(pc)->cline);
	      pc_cline2 = PCCS(pc_cline2->pc.next);
	    } else {
	      pc_cline = pc_cline2 = PCI(pc)->cline;
	      pc_cline->pc.seq = pc->seq;
	    }
	  }

	  pCode2str(&buf[2], 254, pc);
	  pic16_pCodeInsertAfter(pcprev, pic16_newpCodeCharP(buf));
	  pcprev = pcprev->next;
	  pc = pc->next;

	}
	if(pc_cline2)
	  pc_cline2->pc.next = NULL;
      }
#endif

      if(pcin)
	pCodeDeleteChain(pc,pcin);

      /* Generate the replacement code */
      pc = pcprev;
      pcr = peepBlock->replace.pb->pcHead;  // This is the replacement code
      while (pcr) {
	pCodeOp *pcop=NULL;
	
	/* If the replace pcode is an instruction with an operand, */
	/* then duplicate the operand (and expand wild cards in the process). */
	if(pcr->type == PC_OPCODE) {
	  if(PCI(pcr)->pcop) {
	    /* The replacing instruction has an operand.
	     * Is it wild? */
	    if(PCI(pcr)->pcop->type == PO_WILD) {
	      int index = PCOW(PCI(pcr)->pcop)->id;
	      //DFPRINTF((stderr,"copying wildopcode\n"));
	      if(peepBlock->target.wildpCodeOps[index])
		pcop = pic16_pCodeOpCopy(peepBlock->target.wildpCodeOps[index]);
	      else
		DFPRINTF((stderr,"error, wildopcode in replace but not source?\n"));
	    } else
	      pcop = pic16_pCodeOpCopy(PCI(pcr)->pcop);
	  }
	  //DFPRINTF((stderr,"inserting pCode\n"));
	  pic16_pCodeInsertAfter(pc, pic16_newpCode(PCI(pcr)->op,pcop));
	} else if (pcr->type == PC_WILD) {
	  if(PCW(pcr)->invertBitSkipInst)
	    DFPRINTF((stderr,"We need to invert the bit skip instruction\n"));
	  pic16_pCodeInsertAfter(pc,
			   pCodeInstructionCopy(PCI(peepBlock->target.wildpCodes[PCW(pcr)->id]),
						PCW(pcr)->invertBitSkipInst));
	} else if (pcr->type == PC_COMMENT) {
	  pic16_pCodeInsertAfter(pc, pic16_newpCodeCharP( ((pCodeComment *)(pcr))->comment));
	}


	pc = pc->next;
#ifdef PCODE_DEBUG
	DFPRINTF((stderr,"  NEW Code:"));
	if(pc) pc->print(stderr,pc);
#endif
	pcr = pcr->next;
      }

      /* We have just replaced the inefficient code with the rule.
       * Now, we need to re-add the C-source symbols if there are any */
      pc = pcprev;
      while(pc_cline ) {
       
	pc =  pic16_findNextInstruction(pc->next);
	PCI(pc)->cline = pc_cline;
	pc_cline = PCCS(pc_cline->pc.next);
	
      }

      return 1;
    }
  next_rule:
    peeprules = peeprules->next;
  }
  DFPRINTF((stderr," no rule matched\n"));

  return 0;
}
