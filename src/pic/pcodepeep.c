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
#include <stdlib.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "pcode.h"
#include "ralloc.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

pCodeOp *popCopyGPR2Bit(pCodeOpReg *pc, int bitval);


pCodeOp *newpCodeOpWild(int id, pCodePeep *pcp, pCodeOp *subtype);
pCode *newpCodeWild(int pCodeID, pCodeOp *optional_operand, pCodeOp *optional_label);
pCode * findNextInstruction(pCode *pc);
char *Safe_strdup(char *str);
int getpCode(char *mnem,int dest);
extern pCodeInstruction *pic14Mnemonics[];


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

static pCodePeep          *curPeep=NULL;

/****************************************************************/
/*                                                              */
/* curBlock                                                     */
/*                                                              */
/****************************************************************/

static pBlock             *curBlock=NULL;


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
unsigned   tokIdx=0;


typedef enum  {
  PCT_SPACE=1,
  PCT_PERCENT,
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
unsigned int parsedPatIdx=0;


typedef enum {
  PCP_LABEL=1,
  PCP_NUMBER,
  PCP_STR,
  PCP_WILDVAR,
  PCP_WILDSTR,
  PCP_COMMA
} pCodePatterns;

static char pcpat_label[]      = {PCT_PERCENT, PCT_NUMBER, PCT_COLON, 0};
static char pcpat_number[]     = {PCT_NUMBER, 0};
static char pcpat_string[]     = {PCT_STRING, 0};
static char pcpat_wildString[] = {PCT_PERCENT, PCT_STRING, 0};
static char pcpat_wildVar[]    = {PCT_PERCENT, PCT_NUMBER, 0};
static char pcpat_comma[]      = {PCT_COMMA, 0};


typedef struct pcPattern {
  char pt;                 // Pattern type
  char *tokens;           // list of tokens that describe the pattern
  void * (*f) (void *);
} pcPattern;

pcPattern pcpArr[] = {
  {PCP_LABEL,     pcpat_label,      NULL},
  {PCP_WILDSTR,   pcpat_wildString, NULL},
  {PCP_STR,       pcpat_string,     NULL},
  {PCP_WILDVAR,   pcpat_wildVar,    NULL},
  {PCP_COMMA,     pcpat_comma,      NULL},
  {PCP_NUMBER,    pcpat_number,     NULL}
};

#define PCPATTERNS (sizeof(pcpArr)/sizeof(pcPattern))

// Assembly Line Token
typedef enum {
  ALT_LABEL=1,
  ALT_MNEM0,
  ALT_MNEM0A,
  ALT_MNEM1,
  ALT_MNEM1A,
  ALT_MNEM1B,
  ALT_MNEM2,
  ALT_MNEM2A
} altPatterns;

static char alt_label[]     = { PCP_LABEL, 0};
static char alt_mnem0[]     = { PCP_STR, 0};
static char alt_mnem0a[]    = { PCP_WILDVAR, 0};
static char alt_mnem1[]     = { PCP_STR, PCP_STR, 0};
static char alt_mnem1a[]    = { PCP_STR, PCP_WILDVAR, 0};
static char alt_mnem1b[]    = { PCP_STR, PCP_NUMBER, 0};
static char alt_mnem2[]     = { PCP_STR, PCP_STR, PCP_COMMA, PCP_STR, 0};
static char alt_mnem2a[]    = { PCP_STR, PCP_WILDVAR, PCP_COMMA, PCP_STR, 0};

static void * cvt_altpat_label(void *pp);
static void * cvt_altpat_mnem0(void *pp);
static void * cvt_altpat_mnem0a(void *pp);
static void * cvt_altpat_mnem1(void *pp);
static void * cvt_altpat_mnem1a(void *pp);
static void * cvt_altpat_mnem1b(void *pp);
static void * cvt_altpat_mnem2(void *pp);
static void * cvt_altpat_mnem2a(void *pp);

pcPattern altArr[] = {
  {ALT_LABEL,        alt_label,  cvt_altpat_label},
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

  if(STRCASECMP(reg, pc_status.pcop.name))
    return NULL;

  len = strlen(bit);

  if(len == 1) {
    // check C,Z
    if(toupper(*bit) == 'C')
      return PCOP(popCopyGPR2Bit(&pc_status,PIC_C_BIT));
    if(toupper(*bit) == 'Z')
      return PCOP(popCopyGPR2Bit(&pc_status,PIC_Z_BIT));
  }

  // Check DC
  if(len ==2 && toupper(bit[0]) == 'D' && toupper(bit[1]) == 'C')
    return PCOP(popCopyGPR2Bit(&pc_status,PIC_DC_BIT));

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
static void * cvt_altpat_label(void *pp)
{
  parsedPattern *p = pp;

  fprintf(stderr,"altpat_label with ID = %d\n",p->pct[1].tok.n);
  return newpCodeOpLabel(-p->pct[1].tok.n);

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem0(void *pp)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;

  fprintf(stderr,"altpat_mnem0 %s\n",  p->pct[0].tok.s);

  opcode = getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  pci = PCI(newpCode(opcode, NULL));

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
static void * cvt_altpat_mnem0a(void *pp)
{
  parsedPattern *p = pp;

  fprintf(stderr,"altpat_mnem0a wild mnem # %d\n",  p[0].pct[1].tok.n);

  return newpCodeWild(p[0].pct[1].tok.n,NULL,NULL);

}

/*-----------------------------------------------------------------*/
/* cvt_altpat_mem1 - convert assembly line type to a pCode         */
/*                   instruction with 1 operand.                   */
/*                                                                 */
/*  pp[0] - mnem                                                   */
/*  pp[1] - Operand                                                */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem1(void *pp)
{

  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  fprintf(stderr,"altpat_mnem1 %s var %s\n",  p->pct[0].tok.s,p[1].pct[0].tok.s);

  opcode = getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic14Mnemonics[opcode]->bit_inst)
    pcosubtype = newpCodeOp(p[1].pct[0].tok.s,PO_BIT);
  else
    pcosubtype = newpCodeOp(p[1].pct[0].tok.s,PO_GPR_REGISTER);


  pci = PCI(newpCode(opcode, pcosubtype));

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
static void * cvt_altpat_mnem1a(void *pp)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  fprintf(stderr,"altpat_mnem1a %s var %d\n",  p->pct[0].tok.s,p[1].pct[1].tok.n);

  opcode = getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic14Mnemonics[opcode]->bit_inst)
    pcosubtype = newpCodeOp(NULL,PO_BIT);
  else
    pcosubtype = newpCodeOp(NULL,PO_GPR_REGISTER);


  pci = PCI(newpCode(opcode,
		     newpCodeOpWild(p[1].pct[1].tok.n, curPeep, pcosubtype)));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem1b(void *pp)
{
  parsedPattern *p = pp;
  int opcode;

  pCodeInstruction *pci=NULL;

  fprintf(stderr,"altpat_mnem1b %s var %d\n",  p->pct[0].tok.s,p[1].pct[0].tok.n);

  opcode = getpCode(p->pct[0].tok.s,0);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  pci = PCI(newpCode(opcode, newpCodeOpLit(p[1].pct[0].tok.n) ));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");


  return pci;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void * cvt_altpat_mnem2(void *pp)
{
  parsedPattern *p = pp;
  int opcode;
  int dest;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  dest = cvt_extract_destination(&p[3]);

  fprintf(stderr,"altpat_mnem2 %s var %s destination %s(%d)\n",
	  p->pct[0].tok.s,
	  p[1].pct[0].tok.s,
	  p[3].pct[0].tok.s,
	  dest);


  opcode = getpCode(p->pct[0].tok.s,dest);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic14Mnemonics[opcode]->bit_inst) {
    pcosubtype = cvt_extract_status(p[1].pct[0].tok.s, p[3].pct[0].tok.s);
    if(pcosubtype == NULL) {
      fprintf(stderr, "bad operand?\n");
      return NULL;
    }
      
  } else
    pcosubtype = newpCodeOp(p[1].pct[0].tok.s,PO_GPR_REGISTER);


  pci = PCI(newpCode(opcode,pcosubtype));

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
static void * cvt_altpat_mnem2a(void *pp)
{
  parsedPattern *p = pp;
  int opcode;
  int dest;

  pCodeInstruction *pci=NULL;
  pCodeOp *pcosubtype;

  dest = cvt_extract_destination(&p[3]);

  fprintf(stderr,"altpat_mnem2a %s var %d destination %s(%d)\n",
	  p->pct[0].tok.s,
	  p[1].pct[1].tok.n,
	  p[3].pct[0].tok.s,
	  dest);


  opcode = getpCode(p->pct[0].tok.s,dest);
  if(opcode < 0) {
    fprintf(stderr, "Bad mnemonic\n");
    return NULL;
  }

  if(pic14Mnemonics[opcode]->bit_inst)
    pcosubtype = newpCodeOp(NULL,PO_BIT);
  else
    pcosubtype = newpCodeOp(NULL,PO_GPR_REGISTER);


  pci = PCI(newpCode(opcode,
		     newpCodeOpWild(p[1].pct[1].tok.n, curPeep, pcosubtype)));

  if(!pci)
    fprintf(stderr,"couldn't find mnemonic\n");

  return pci;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/


static void parseLineNode(char *ln)
{

  tokIdx = 0;

  if(!ln || !*ln)
    return;

  while(*ln) {

    if(isspace(*ln)) {
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
    case ':':
      tokArr[tokIdx++].tt = PCT_COLON;
      break;
    case ';':
      tokArr[tokIdx].tok.s = Safe_strdup(ln);
      tokArr[tokIdx++].tt = PCT_COMMENT;
      return;
    case ',':
      tokArr[tokIdx++].tt = PCT_COMMA;
      break;


    default:
      if(isalpha(*ln)) {
	char buffer[50];
	int i=0;

	while( (isalpha(*ln)  ||  isdigit(*ln)) && i<49)
	  buffer[i++] = *ln++;

	ln--;
	buffer[i] = 0;

	tokArr[tokIdx].tok.s = Safe_strdup(buffer);
	//fprintf(stderr," string %s",tokArr[tokIdx].tok.s);

	tokArr[tokIdx++].tt = PCT_STRING;

      }
    }
    ln++;
  }

  tokArr[tokIdx].tt = 0;
}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/



void dump1Token(pCodeTokens tt)
{

  switch(tt) {
  case PCT_SPACE:
    fprintf(stderr, " space ");
    break;
  case PCT_PERCENT:
    fprintf(stderr, " pct ");
    fputc('%', stderr);
    break;
  case PCT_COLON:
    fprintf(stderr, " col ");
    fputc(':',stderr);
    break;
  case PCT_COMMA:
    fprintf(stderr, " com ");
    fputc(',',stderr);
    break;
  case PCT_COMMENT:
  case PCT_STRING:
    fprintf(stderr, " str ");
    //fprintf(stderr,"%s",tokArr[i].tok.s);
    break;
  case PCT_NUMBER:
    fprintf(stderr, " num ");
    //fprintf(stderr,"%d",tokArr[i].tok.n);


  }
}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

int pcComparePattern(pCodeToken *pct, char *pat, int max_tokens)
{
  int i=0;

  if(!pct || !pat || !*pat)
    return 0;

  //fprintf(stderr,"comparing against:\n");

  while(i < max_tokens) {

    if(*pat == 0){
      //fprintf(stderr,"matched\n");
      return (i+1);
    }

    //dump1Token(*pat); fprintf(stderr,"\n");

    if(pct->tt != *pat) 
      return 0;


    pct++;
    pat++;
  }

  return 0;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

int altComparePattern( char *pct, parsedPattern *pat, int max_tokens)
{
  int i=0;
  
  if(!pct || !pat || !*pct)
    return 0;


  while(i < max_tokens) {

    if(*pct == 0) {
      //fprintf(stderr,"matched\n");
      return i;
    }

    //dump1Token(*pat); fprintf(stderr,"\n");

    if( !pat || !pat->pcp )
      return 0;

    if (pat->pcp->pt != *pct)  
      return 0;

    //fprintf(stderr," pct=%d\n",*pct);
    pct++;
    pat++;
    i++;
  }

  return 0;

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

int advTokIdx(int *v, int amt)
{

  if((unsigned) (*v + amt) > tokIdx)
    return 1;

  *v += amt;
  return 0;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

void dumpTokens(void)
{
  unsigned i;
  pCode *pc;

  if(!tokIdx)
    return;

  for(i=0; i<=tokIdx; i++)
    dump1Token(tokArr[i].tt);


  fputc('\n',stderr);

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
      //fprintf(stderr,"ltokIdx = %d\n",ltokIdx);

      if(  ((tokArr[ltokIdx].tt == PCT_SPACE) )
	   && (advTokIdx(&ltokIdx, 1)) ) // eat space
	break;

      do {
	j = pcComparePattern(&tokArr[ltokIdx], pcpArr[lpcpIdx].tokens, tokIdx +1);
	if( j ) {
	  //fprintf(stderr,"found token pattern match\n");
	  switch(pcpArr[lpcpIdx].pt) {
	  case  PCP_LABEL:
	    if(state == PS_START){
	      fprintf(stderr,"  label\n");
	      state = PS_HAVE_LABEL;
	    } else 
	      fprintf(stderr,"  bad state (%d) for label\n",state);
	    break;

	  case  PCP_STR:
	    fprintf(stderr,"  %s is",tokArr[ltokIdx].tok.s);
	    switch(state) {
	    case PS_START:
	    case PS_HAVE_LABEL:
	      fprintf(stderr,"  mnem\n");
	      cPmnem = tokArr[ltokIdx].tok.s;
	      state = PS_HAVE_MNEM;
	      break;
	    case PS_HAVE_MNEM:
	      fprintf(stderr,"  1st operand\n");
	      cP1stop = tokArr[ltokIdx].tok.s;
	      //pco1 = newpCodeOp(NULL,PO_GPR_REGISTER);
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      fprintf(stderr,"  error expecting comma\n");
	      break;
	    case PS_HAVE_COMMA:
	      fprintf(stderr,"  2 operands\n");
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
	      fprintf(stderr,"  wild mnem\n");
	      state = PS_HAVE_MNEM;
	      break;
	    case PS_HAVE_MNEM:
	      fprintf(stderr,"  1st operand is wild\n");
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      fprintf(stderr,"  error expecting comma\n");
	      break;
	    case PS_HAVE_COMMA:
	      fprintf(stderr,"  2nd operand is wild\n");
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
	      fprintf(stderr,"  1st operand is a number\n");
	      state = PS_HAVE_1OPERAND;
	      break;
	    case PS_HAVE_1OPERAND:
	      fprintf(stderr,"  error expecting comma\n");
	      break;
	    case PS_HAVE_COMMA:
	      fprintf(stderr,"  2nd operand is a number\n");
	      break;
	    case PS_HAVE_2OPERANDS:
	      break;
	    }
	    break;

	  case  PCP_WILDSTR:
	    break;
	  case  PCP_COMMA:
	    if(state == PS_HAVE_1OPERAND){
	      fprintf(stderr,"  got a comma\n");
	      state = PS_HAVE_COMMA;
	    } else
	      fprintf(stderr,"  unexpected comma\n");
	  }

	  matching = 1;
	  parsedPatArr[lparsedPatIdx].pcp = &pcpArr[lpcpIdx];
	  parsedPatArr[lparsedPatIdx].pct = &tokArr[ltokIdx];
	  lparsedPatIdx++;

	  //dump1Token(tokArr[ltokIdx].tt);

	  if(advTokIdx(&ltokIdx, strlen(pcpArr[lpcpIdx].tokens) ) ) {
	    fprintf(stderr," reached end \n");
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
	  pc = altArr[k].f(&parsedPatArr[j]);
	  if(pc && pc->print)
	    pc->print(stderr,pc);
	  //if(pc && pc->destruct) pc->destruct(pc); dumps core?
	}
	j += c;
      }
      k++;
    }
    while(j<=lparsedPatIdx && k<ALTPATTERNS);

/*
    fprintf(stderr,"\nConverting parsed line to pCode:\n\n");

    j = 0;
    do {
      if(parsedPatArr[j].pcp && parsedPatArr[j].pcp->f )
	parsedPatArr[j].pcp->f(&parsedPatArr[j]);
      fprintf(stderr,"  %d",parsedPatArr[j].pcp->pt);
      j++;
    }
    while(j<lparsedPatIdx);
*/
    fprintf(stderr,"\n");

  }


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

void  peepRules2pCode(peepRule *rules)
{
  peepRule *pr;
  lineNode *ln;

  pCodePeepSnippets *pcps;

  for (pr = rules; pr; pr = pr->next) {
    fprintf(stderr,"\nRule:\n\n");

    pcps = Safe_calloc(1,sizeof(pCodePeepSnippets));
    curPeep = pcps->peep  = Safe_calloc(1,sizeof(pCodePeep));
    //curPeep->target = curPeep->replace = NULL;
    curPeep->vars = NULL; curPeep->wildpCodes = NULL;
    peepSnippets = DLL_append((_DLL*)peepSnippets,(_DLL*)pcps);

    curPeep->target = curBlock = newpCodeChain(NULL, 'W', NULL);

    for(ln = pr->match; ln; ln = ln->next) {

      fprintf(stderr,"%s\n",ln->line);

      parseLineNode(ln->line);
      dumpTokens();

    }

    fprintf(stderr,"\nReplaced by:\n");

    curPeep->replace = curBlock = newpCodeChain(NULL, 'W', NULL);

    for(ln = pr->replace; ln; ln = ln->next)
      fprintf(stderr,"%s\n",ln->line);

    if(pr->cond)
      fprintf(stderr,"\nCondition:  %s\n",pr->cond);

    return;
  }

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
#if 0
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
    /*
      target:

          btfsc  %0
           goto  %1
          %3
      %1: %4

      replace:
          btfss  %0
      %1:  %4
          %3

	  The %3 and %4 are wild opcodes. Since the opcodes
	  are stored in a different array than the wild operands,
	  they can have the same indices and not conflict. So
	  below, the %3 is really a %0, %4 is a %1.

     */
    pCodeOp *pcl;
    pCodeOp *pcw;
    pCodeOp *pcwb;

    // Create a new wild operand subtyped as a bit
    pcwb =  newpCodeOpWild(0,pcp,newpCodeOpBit(NULL,-1));

    // Create a 
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



#endif
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
  if(!pcp)
    return;

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
    if(!peepBlock || !peepBlock->target || !peepBlock->target->pcHead)
      goto next_rule;

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
	  fprintf(stderr,"inserting pCode\n");
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
  next_rule:
    peeprules = peeprules->next;
  }

  return 0;
}










#if 0
/*******************/
pCode *parseLineNode(char *ln)
{
  char buffer[50], *s;
  int state=0;          //0 label, 1 mnemonic, 2 operand, 3 operand, 4 comment
  int var;
  pCode *pc = NULL;
  //  pCodeLabel *pcl = NULL;

  if(!ln || !*ln)
    return pc;

  s = buffer;
  *s = 0;

  while(*ln) {

    /* skip white space */
    while (isspace (*ln))
      ln++;

    switch(state) {

    case 0:   // look for a label
    case 1:   // look for mnemonic

      if(*ln == '%') {

	// Wild

	ln++;
	if(!isdigit(*ln) )
	  break;
	  //goto next_state;

	var = strtol(ln, &ln, 10);
	if(*ln  == ':') {
	  // valid wild card label
	  fprintf(stderr, " wildcard label: %d\n",var);
	  ln++;
	} else
	  fprintf(stderr, " wild opcode: %d\n",var), state++;

      } else {
	// not wild
	// Extract the label/mnemonic from the line

	s = buffer;
	while(*ln && !(isspace(*ln) || *ln == ':'))
	  *s++ = *ln++;

	*s = 0;
	if(*ln == ':')
	  fprintf(stderr," regular label: %s\n",buffer), ln++;
	else
	  fprintf(stderr," regular mnem: %s\n",buffer), state++;
      }
      state++;
      break;

    default:
      ln++;

    }
  }

  return pc;
  
}
#endif
