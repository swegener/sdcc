/*-------------------------------------------------------------------------
  gen.c - Z80 specific code generator.

  Benchmarks on dhry.c 2.1 with 32766 loops and a 10ms clock:
						ticks	dhry	size
  Base with asm strcpy / strcmp / memcpy:	23198	141	1A14
  Improved WORD push				22784	144	19AE
  With label1 on				22694	144	197E
  With label2 on				22743	144	198A
  With label3 on				22776	144	1999
  With label4 on				22776	144	1999
  With all 'label' on				22661	144	196F
  With loopInvariant on				20919	156	19AB
  With loopInduction on				Breaks		198B
  With all working on				20796	158	196C
  Slightly better genCmp(signed)		20597	159	195B
  Better reg packing, first peephole		20038	163	1873
  With assign packing				19281	165	1849

  Michael Hope <michaelh@earthling.net>	2000
  Based on the mcs51 generator - Sandeep Dutta . sandeep.dutta@usa.net (1998)
                           and -  Jean-Louis VERN.jlvern@writeme.com (1999)
	 
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
#include <string.h>
#include <ctype.h>

#ifdef HAVE_SYS_ISA_DEFS_H
#include <sys/isa_defs.h>
#endif

#include "z80.h"
#include "SDCCpeeph.h"
#include "gen.h"
#include "SDCCglue.h"

/* this is the down and dirty file with all kinds of kludgy & hacky
   stuff. This is what it is all about CODE GENERATION for a specific MCU.
   Some of the routines may be reusable, will have to see */

static char *zero = "#0x00";
static char *one  = "#0x01";
static char *spname ;
static char *_z80_return[] = {"l", "h", "e", "d" };
static char *_gbz80_return[] = { "e", "d", "l", "h" };
static char **_fReturn;
static char **_fTmp;

static char *accUse[] = {"a" };
short rbank = -1;
short accInUse = 0 ;
short inLine = 0;
short debugLine = 0;
short nregssaved = 0;
extern int ptrRegReq ;
extern int nRegs;
extern FILE *codeOutFile;
set *sendSet = NULL;
const char *_shortJP = "jp";

#define RESULTONSTACK(x) \
                         (IC_RESULT(x) && IC_RESULT(x)->aop && \
                         IC_RESULT(x)->aop->type == AOP_STK )

#define MOVA(x) if (strcmp(x,"a")) emitcode("ld","a,%s",x);
#define CLRC    emitcode("xor","a,a");

#define LABEL_STR 	"%05d$"

lineNode *lineHead = NULL;
lineNode *lineCurr = NULL;

unsigned char   SLMask[] = {0xFF ,0xFE, 0xFC, 0xF8, 0xF0,
0xE0, 0xC0, 0x80, 0x00};
unsigned char   SRMask[] = {0xFF, 0x7F, 0x3F, 0x1F, 0x0F,
0x07, 0x03, 0x01, 0x00};

static int _lastStack = 0;
static int _pushed = 0;
static int _spoffset;
static int _lastHLOff = 0;
static asmop *_lastHL;

#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3

/* Stack frame:

   IX+4		param0	LH
   IX+2		ret	LH
   IX+0		ix	LH
   IX-2		temp0	LH
*/

/*-----------------------------------------------------------------*/
/* emitcode - writes the code into a file : for now it is simple    */
/*-----------------------------------------------------------------*/
void emitcode (const char *inst, const char *fmt, ...)
{
    va_list ap;
    char lb[MAX_INLINEASM];  
    char *lbp = lb;

    va_start(ap,fmt);   

    if (*inst != '\0') {
        sprintf(lb,"%s\t",inst);
        vsprintf(lb+(strlen(lb)),fmt,ap);
    }  else 
        vsprintf(lb,fmt,ap);

    while (isspace(*lbp)) lbp++;

    if (lbp && *lbp) 
        lineCurr = (lineCurr ?
                    connectLine(lineCurr,newLineNode(lb)) :
                    (lineHead = newLineNode(lb)));
    lineCurr->isInline = inLine;
    lineCurr->isDebug  = debugLine;
    va_end(ap);
}

const char *getPairName(asmop *aop)
{
    if (aop->type == AOP_REG) {
	switch (aop->aopu.aop_reg[0]->rIdx) {
	case C_IDX:
	    return "bc";
	    break;
	case E_IDX:
	    return "de";
	    break;
	case L_IDX:
	    return "hl";
	    break;
	}
    }
    else if (aop->type == AOP_STR) {
	switch (*aop->aopu.aop_str[0]) {
	case 'c':
	    return "bc";
	    break;
	case 'e':
	    return "de";
	    break;
	case 'l':
	    return "hl";
	    break;
	}
    }
    assert(0);
    return NULL;
}

/** Returns TRUE if the registers used in aop form a pair (BC, DE, HL) */
bool isPair(asmop *aop)
{
    if (aop->size == 2) {
	if (aop->type == AOP_REG) {
	    if ((aop->aopu.aop_reg[0]->rIdx == C_IDX)&&(aop->aopu.aop_reg[1]->rIdx == B_IDX)) {
		return TRUE;
	    }
	    if ((aop->aopu.aop_reg[0]->rIdx == E_IDX)&&(aop->aopu.aop_reg[1]->rIdx == D_IDX)) {
		return TRUE;
	    }
	    if ((aop->aopu.aop_reg[0]->rIdx == L_IDX)&&(aop->aopu.aop_reg[1]->rIdx == H_IDX)) {
		return TRUE;
	    }
	}
	if (aop->type == AOP_STR) {
	    if (!strcmp(aop->aopu.aop_str[0], "c")&&!strcmp(aop->aopu.aop_str[1], "b")) {
		return TRUE;
	    }
	    if (!strcmp(aop->aopu.aop_str[0], "e")&&!strcmp(aop->aopu.aop_str[1], "d")) {
		return TRUE;
	    }
	    if (!strcmp(aop->aopu.aop_str[0], "l")&&!strcmp(aop->aopu.aop_str[1], "h")) {
		return TRUE;
	    }
	}
    }
    return FALSE;
}

/** Push a register pair onto the stack */
void genPairPush(asmop *aop)
{
    emitcode("push", "%s", getPairName(aop));
}

/*-----------------------------------------------------------------*/
/* newAsmop - creates a new asmOp                                  */
/*-----------------------------------------------------------------*/
static asmop *newAsmop (short type)
{
    asmop *aop;

    ALLOC(aop,sizeof(asmop));
    aop->type = type;
    return aop;
}

/*-----------------------------------------------------------------*/
/* aopForSym - for a true symbol                                   */
/*-----------------------------------------------------------------*/
static asmop *aopForSym (iCode *ic,symbol *sym,bool result)
{
    asmop *aop;
    memmap *space= SPEC_OCLS(sym->etype);

    /* if already has one */
    if (sym->aop)
        return sym->aop;

    /* Assign depending on the storage class */
    if (sym->onStack || sym->iaccess) {
        sym->aop = aop = newAsmop(AOP_STK);
        aop->size = getSize(sym->type);
	_lastHL = NULL;
	aop->aopu.aop_stk = sym->stack;
        return aop;
    }

    /* special case for a function */
    if (IS_FUNC(sym->type)) {   
        sym->aop = aop = newAsmop(AOP_IMMD);    
        ALLOC_ATOMIC(aop->aopu.aop_immd,strlen(sym->rname)+1);
        strcpy(aop->aopu.aop_immd,sym->rname);
        aop->size = 2;
        return aop;
    }

#if 0
    if (IS_GB) {
	/* if it is in direct space */
	if (IN_DIRSPACE(space)) {
	    sym->aop = aop = newAsmop (AOP_DIR);
	    aop->aopu.aop_dir = sym->rname ;
	    aop->size = getSize(sym->type);
	    emitcode("", "; AOP_DIR for %s", sym->rname);
	    return aop;
	}
    }
#endif

    /* only remaining is far space */
    /* in which case DPTR gets the address */
    if (IS_GB) {
	sym->aop = aop = newAsmop(AOP_HL);
	_lastHL = NULL;
    }
    else {
	sym->aop = aop = newAsmop(AOP_IY);
	emitcode ("ld","iy,#%s ; a", sym->rname);
    }
    aop->size = getSize(sym->type);
    aop->aopu.aop_dir = sym->rname;

    /* if it is in code space */
    if (IN_CODESPACE(space))
        aop->code = 1;

    return aop;     
}

/*-----------------------------------------------------------------*/
/* aopForRemat - rematerialzes an object                           */
/*-----------------------------------------------------------------*/
static asmop *aopForRemat (symbol *sym)
{
    char *s = buffer;   
    iCode *ic = sym->rematiCode;
    asmop *aop = newAsmop(AOP_IMMD);

    while (1) {
        /* if plus or minus print the right hand side */
        if (ic->op == '+' || ic->op == '-') {
            sprintf(s,"0x%04x %c ",(int) operandLitValue(IC_RIGHT(ic)),
                    ic->op );
            s += strlen(s);
            ic = OP_SYMBOL(IC_LEFT(ic))->rematiCode;
            continue ;
        }
        /* we reached the end */
        sprintf(s,"%s",OP_SYMBOL(IC_LEFT(ic))->rname);
        break;
    }

    ALLOC_ATOMIC(aop->aopu.aop_immd,strlen(buffer)+1);
    strcpy(aop->aopu.aop_immd,buffer);    
    return aop;        
}

/*-----------------------------------------------------------------*/
/* regsInCommon - two operands have some registers in common       */
/*-----------------------------------------------------------------*/
bool regsInCommon (operand *op1, operand *op2)
{
    symbol *sym1, *sym2;
    int i;

    /* if they have registers in common */
    if (!IS_SYMOP(op1) || !IS_SYMOP(op2))
        return FALSE ;

    sym1 = OP_SYMBOL(op1);
    sym2 = OP_SYMBOL(op2);

    if (sym1->nRegs == 0 || sym2->nRegs == 0)
        return FALSE ;

    for (i = 0 ; i < sym1->nRegs ; i++) {
        int j;
        if (!sym1->regs[i])
            continue ;

        for (j = 0 ; j < sym2->nRegs ;j++ ) {
            if (!sym2->regs[j])
                continue ;

            if (sym2->regs[j] == sym1->regs[i])
                return TRUE ;
        }
    }

    return FALSE ;
}

/*-----------------------------------------------------------------*/
/* operandsEqu - equivalent                                        */
/*-----------------------------------------------------------------*/
bool operandsEqu ( operand *op1, operand *op2)
{
    symbol *sym1, *sym2;

    /* if they not symbols */
    if (!IS_SYMOP(op1) || !IS_SYMOP(op2))
        return FALSE;

    sym1 = OP_SYMBOL(op1);
    sym2 = OP_SYMBOL(op2);

    /* if both are itemps & one is spilt
       and the other is not then false */
    if (IS_ITEMP(op1) && IS_ITEMP(op2) &&
	sym1->isspilt != sym2->isspilt )
	return FALSE ;

    /* if they are the same */
    if (sym1 == sym2)
        return 1;

    if (strcmp(sym1->rname,sym2->rname) == 0)
        return 2;


    /* if left is a tmp & right is not */
    if (IS_ITEMP(op1)  && 
        !IS_ITEMP(op2) &&
        sym1->isspilt  &&
        (sym1->usl.spillLoc == sym2))
        return 3;

    if (IS_ITEMP(op2)  && 
        !IS_ITEMP(op1) &&
        sym2->isspilt  &&
	sym1->level > 0 &&
        (sym2->usl.spillLoc == sym1))
        return 4;

    return FALSE;
}

/*-----------------------------------------------------------------*/
/* sameRegs - two asmops have the same registers                   */
/*-----------------------------------------------------------------*/
bool sameRegs (asmop *aop1, asmop *aop2 )
{
    int i;

    if (aop1 == aop2)
        return TRUE ;

    if (aop1->type != AOP_REG ||
        aop2->type != AOP_REG )
        return FALSE ;

    if (aop1->size != aop2->size)
        return FALSE ;

    for (i = 0 ; i < aop1->size ; i++ )
        if (aop1->aopu.aop_reg[i] !=
            aop2->aopu.aop_reg[i] )
            return FALSE ;

    return TRUE ;
}

/*-----------------------------------------------------------------*/
/* aopOp - allocates an asmop for an operand  :                    */
/*-----------------------------------------------------------------*/
static void aopOp (operand *op, iCode *ic, bool result)
{
    asmop *aop;
    symbol *sym;
    int i;

    if (!op)
        return ;

    /* if this a literal */
    if (IS_OP_LITERAL(op)) {
        op->aop = aop = newAsmop(AOP_LIT);
        aop->aopu.aop_lit = op->operand.valOperand;
        aop->size = getSize(operandType(op));
        return;
    }

    /* if already has a asmop then continue */
    if (op->aop)
        return ;

    /* if the underlying symbol has a aop */
    if (IS_SYMOP(op) && OP_SYMBOL(op)->aop) {
        op->aop = OP_SYMBOL(op)->aop;
        return;
    }

    /* if this is a true symbol */
    if (IS_TRUE_SYMOP(op)) {    
        op->aop = aopForSym(ic,OP_SYMBOL(op),result);
        return ;
    }

    /* this is a temporary : this has
    only four choices :
    a) register
    b) spillocation
    c) rematerialize 
    d) conditional   
    e) can be a return use only */

    sym = OP_SYMBOL(op);

    /* if the type is a conditional */
    if (sym->regType == REG_CND) {
        aop = op->aop = sym->aop = newAsmop(AOP_CRY);
        aop->size = 0;
        return;
    }

    /* if it is spilt then two situations
    a) is rematerialize 
    b) has a spill location */
    if (sym->isspilt || sym->nRegs == 0) {
        /* rematerialize it NOW */
        if (sym->remat) {
            sym->aop = op->aop = aop =
                                      aopForRemat (sym);
            aop->size = getSize(sym->type);
            return;
        }

	if (sym->accuse) {
	    int i;
            aop = op->aop = sym->aop = newAsmop(AOP_ACC);
            aop->size = getSize(sym->type);
            for ( i = 0 ; i < 2 ; i++ )
                aop->aopu.aop_str[i] = accUse[i];
            return;  
	}

        if (sym->ruonly ) {
            int i;
            aop = op->aop = sym->aop = newAsmop(AOP_STR);
            aop->size = getSize(sym->type);
            for ( i = 0 ; i < 4 ; i++ )
                aop->aopu.aop_str[i] = _fReturn[i];
            return;
        }

        /* else spill location  */
        sym->aop = op->aop = aop = 
                                  aopForSym(ic,sym->usl.spillLoc,result);
        aop->size = getSize(sym->type);
        return;
    }

    /* must be in a register */
    sym->aop = op->aop = aop = newAsmop(AOP_REG);
    aop->size = sym->nRegs;
    for ( i = 0 ; i < sym->nRegs ;i++)
        aop->aopu.aop_reg[i] = sym->regs[i];
}

/*-----------------------------------------------------------------*/
/* freeAsmop - free up the asmop given to an operand               */
/*----------------------------------------------------------------*/
static void freeAsmop (operand *op, asmop *aaop, iCode *ic)
{   
    asmop *aop ;

    if (!op)
        aop = aaop;
    else 
        aop = op->aop;

    if (!aop)
        return ;

    if (aop->freed)
        goto dealloc; 

    aop->freed = 1;

    switch (aop->type) {
    case AOP_STK :
	break;
    }

dealloc:
    /* all other cases just dealloc */
    if (op ) {
        op->aop = NULL;
        if (IS_SYMOP(op)) {
            OP_SYMBOL(op)->aop = NULL;    
            /* if the symbol has a spill */
	    if (SPIL_LOC(op))
                SPIL_LOC(op)->aop = NULL;
        }
    }
}

char *aopGetWordLong(asmop *aop, int offset, bool with_hash)
{
    char *s = buffer ;
    char *rs;

    if (aop->size != 2)
	return NULL;
    assert(offset == 0);

    /* depending on type */
    switch (aop->type) {
    case AOP_IMMD:
	sprintf (s,"%s%s",with_hash ? "#" : "", aop->aopu.aop_immd);
	ALLOC_ATOMIC(rs,strlen(s)+1);
	strcpy(rs,s);   
	return rs;
	
    case AOP_LIT: {
	value * val = aop->aopu.aop_lit;
	/* if it is a float then it gets tricky */
	/* otherwise it is fairly simple */
	if (!IS_FLOAT(val->type)) {
	    unsigned long v = floatFromVal(val);

	    sprintf(buffer,"%s0x%04lx", with_hash ? "#" : "", v);
	    ALLOC_ATOMIC(rs,strlen(buffer)+1);
	    return strcpy (rs,buffer);
	}
	assert(0);
	return NULL;
    }
    }
    return NULL;
}

char *aopGetWord(asmop *aop, int offset)
{
    return aopGetWordLong(aop, offset, TRUE);
}

static void setupHL(asmop *aop, int offset)
{
    if (_lastHL != aop) {
	switch (aop->type) {
	case AOP_HL:
	    emitcode("ld", "hl,#%s+%d", aop->aopu.aop_dir, offset);
	    break;
	case AOP_STK:
	    /* In some cases we can still inc or dec hl */
	    emitcode("lda", "hl,%d+%d+%d(sp)", aop->aopu.aop_stk+offset, _pushed, _spoffset);
	    break;
	default:
	    assert(0);
	}
	_lastHL = aop;
	_lastHLOff = offset;
    }
    else {
	while (offset < _lastHLOff) {
	    emitcode("dec", "hl");
	    _lastHLOff--;
	}
	while (offset > _lastHLOff) {
	    emitcode("inc", "hl");
	    _lastHLOff++;
	}
    }
}

/*-----------------------------------------------------------------*/
/* aopGet - for fetching value of the aop                          */
/*-----------------------------------------------------------------*/
static char *aopGet (asmop *aop, int offset, bool bit16)
{
    char *s = buffer ;
    char *rs;

    /* offset is greater than size then zero */
    if (offset > (aop->size - 1) &&
        aop->type != AOP_LIT)
        return zero;

    /* depending on type */
    switch (aop->type) {
    case AOP_IMMD:
	if (bit16) 
	    sprintf (s,"#%s",aop->aopu.aop_immd);
	else
	    if (offset) {
		assert(offset == 1);
		sprintf(s,"#>%s",
			aop->aopu.aop_immd);
	    }
	    else
		sprintf(s,"#<%s",
			aop->aopu.aop_immd);
	ALLOC_ATOMIC(rs,strlen(s)+1);
	strcpy(rs,s);   
	return rs;
	
    case AOP_DIR:
	assert(IS_GB);
	emitcode("ld", "a,(%s+%d) ; x", aop->aopu.aop_dir, offset);
	sprintf(s, "a");
	ALLOC_ATOMIC(rs,strlen(s)+1);
	strcpy(rs,s);   
	return rs;
	
    case AOP_REG:
	return aop->aopu.aop_reg[offset]->name;

    case AOP_HL:
	assert(IS_GB);
	setupHL(aop, offset);
	sprintf(s, "(hl)");
	ALLOC_ATOMIC(rs, strlen(s)+1);
	strcpy(rs,s);
	return rs;

    case AOP_IY:
	sprintf(s,"%d(iy)", offset);
	ALLOC_ATOMIC(rs,strlen(s)+1);
	strcpy(rs,s);   
	return rs;

    case AOP_STK:
	if (IS_GB) {
	    setupHL(aop, offset);
	    sprintf(s, "(hl)");
	}
	else {
	    sprintf(s,"%d(ix) ; %u", aop->aopu.aop_stk+offset, offset);
	}
	ALLOC_ATOMIC(rs,strlen(s)+1);
	strcpy(rs,s);   
	return rs;
	
    case AOP_CRY:
	assert(0);
	
    case AOP_ACC:
	if (!offset) {
	    return "a";
	}
	return "#0x00";

    case AOP_LIT:
	return aopLiteral (aop->aopu.aop_lit,offset);
	
    case AOP_STR:
	aop->coff = offset;
	return aop->aopu.aop_str[offset];
    }

    fprintf(stderr, "Type %u\n", aop->type);

    werror(E_INTERNAL_ERROR,__FILE__,__LINE__,
           "aopget got unsupported aop->type");
    exit(0);
}

bool isRegString(char *s)
{
    if (!strcmp(s, "b") ||
	!strcmp(s, "c") ||
	!strcmp(s, "d") ||
	!strcmp(s, "e") ||
	!strcmp(s, "a") ||
	!strcmp(s, "h") ||
	!strcmp(s, "l"))
	return TRUE;
    return FALSE;
}

bool isConstant(char *s)
{
    return  (*s == '#');
}

bool canAssignToPtr(char *s)
{
    if (isRegString(s))
	return TRUE;
    if (isConstant(s))
	return TRUE;
    return FALSE;
}

/*-----------------------------------------------------------------*/
/* aopPut - puts a string for a aop                                */
/*-----------------------------------------------------------------*/
static void aopPut (asmop *aop, char *s, int offset)
{
    if (aop->size && offset > ( aop->size - 1)) {
        werror(E_INTERNAL_ERROR,__FILE__,__LINE__,
               "aopPut got offset > aop->size");
        exit(0);
    }

    /* will assign value to value */
    /* depending on where it is ofcourse */
    switch (aop->type) {
    case AOP_DIR:
	/* Direct.  Hmmm. */
	assert(IS_GB);
	emitcode("ld", "a,%s", s);
	emitcode("ld", "(%s+%d),a", aop->aopu.aop_dir, offset);
	break;
	
    case AOP_REG:
	/* Dont bother if it's a ld x,x */
	if (strcmp(aop->aopu.aop_reg[offset]->name,s) != 0) {
	    emitcode("ld","%s,%s",
		     aop->aopu.aop_reg[offset]->name,s);
	}
	break;
	
    case AOP_IY:
	assert(!IS_GB);
	if (!canAssignToPtr(s)) {
	    emitcode("ld", "a,%s", s);
	    emitcode("ld", "%d(iy),a", offset);
	}
	else
	    emitcode("ld", "%d(iy),%s", offset, s);
	break;
    
    case AOP_HL:
	assert(IS_GB);
	if (!strcmp(s, "(hl)")) {
	    emitcode("ld", "a,(hl)");
	    s = "a";
	}
	setupHL(aop, offset);
	emitcode("ld", "(hl),%s", s);
	break;

    case AOP_STK:
	if (IS_GB) {
	    if (!strcmp("(hl)", s)) {
		emitcode("ld", "a,(hl)");
		s = "a";
	    }
	    setupHL(aop, offset);
	    if (!canAssignToPtr(s)) {
		emitcode("ld", "a,%s", s);
		emitcode("ld", "(hl),a");
	    }
	    else
		emitcode("ld", "(hl),%s", s);
	}
	else {
	    if (!canAssignToPtr(s)) {
		emitcode("ld", "a,%s", s);
		emitcode("ld", "%d(ix),a", aop->aopu.aop_stk+offset);
	    }
	    else
		emitcode("ld", "%d(ix),%s", aop->aopu.aop_stk+offset, s);
	}
	break;
	
    case AOP_CRY:
	/* if bit variable */
	if (!aop->aopu.aop_dir) {
	    emitcode("ld", "a,#0");
	    emitcode("rla", "");
	} else {
	    /* In bit space but not in C - cant happen */
	    assert(0);
	}
	break;
	
    case AOP_STR:
	aop->coff = offset;
	if (strcmp(aop->aopu.aop_str[offset],s)) {
	    emitcode ("ld","%s,%s",aop->aopu.aop_str[offset],s);
	}
	break;
	
    case AOP_ACC:
	aop->coff = offset;
	if (!offset && (strcmp(s,"acc") == 0))
	    break;
	if (offset>0) {
	    
	    emitcode("", "; Error aopPut AOP_ACC");
	}
	else {
	    if (strcmp(aop->aopu.aop_str[offset],s))
		emitcode ("ld","%s,%s",aop->aopu.aop_str[offset],s);
	}
	break;

    default :
	werror(E_INTERNAL_ERROR,__FILE__,__LINE__,
	       "aopPut got unsupported aop->type");
	exit(0);    
    }    
}

#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define AOP_NEEDSACC(x) (AOP(x) && (AOP_TYPE(x) == AOP_CRY))

/*-----------------------------------------------------------------*/
/* getDataSize - get the operand data size                         */
/*-----------------------------------------------------------------*/
int getDataSize(operand *op)
{
    int size;
    size = AOP_SIZE(op);
    if(size == 3) {
        /* pointer */
	assert(0);
    }
    return size;
}

/*-----------------------------------------------------------------*/
/* movLeft2Result - move byte from left to result                  */
/*-----------------------------------------------------------------*/
static void movLeft2Result (operand *left, int offl,
                            operand *result, int offr, int sign)
{
    char *l;
    if(!sameRegs(AOP(left),AOP(result)) || (offl != offr)){
        l = aopGet(AOP(left),offl,FALSE);

	if (!sign) {
	    aopPut(AOP(result),l,offr);
	}
	else {
	    assert(0);
        }
    }
}


/** Put Acc into a register set 
 */
void outAcc(operand *result)
{
    int size, offset;
    size = getDataSize(result);
    if (size){
        aopPut(AOP(result),"a",0);
        size--;
        offset = 1;
        /* unsigned or positive */
        while (size--){
            aopPut(AOP(result),zero,offset++);
        }
    }
}

/** Take the value in carry and put it into a register
 */
void outBitC(operand *result)
{
    /* if the result is bit */
    if (AOP_TYPE(result) == AOP_CRY) {
	emitcode("", "; Note: outBitC form 1");
        aopPut(AOP(result),"blah",0);
    }
    else {
	emitcode("ld", "a,#0");
	emitcode("rla", "");
        outAcc(result);
    }
}

/*-----------------------------------------------------------------*/
/* toBoolean - emit code for orl a,operator(sizeop)                */
/*-----------------------------------------------------------------*/
void toBoolean(operand *oper)
{
    int size = AOP_SIZE(oper);
    int offset = 0;
    if (size>1) {
	emitcode("ld", "a,%s", aopGet(AOP(oper), offset++, FALSE));
	size--;
	while (size--) 
	    emitcode("or","a,%s",aopGet(AOP(oper),offset++,FALSE));
    }
    else {
	if (AOP(oper)->type != AOP_ACC) {
	    CLRC;
	    emitcode("or","a,%s",aopGet(AOP(oper),0,FALSE));
	}
    }
}

/*-----------------------------------------------------------------*/
/* genNot - generate code for ! operation                          */
/*-----------------------------------------------------------------*/
static void genNot (iCode *ic)
{
    link *optype = operandType(IC_LEFT(ic));

    /* assign asmOps to operand & result */
    aopOp (IC_LEFT(ic),ic,FALSE);
    aopOp (IC_RESULT(ic),ic,TRUE);

    /* if in bit space then a special case */
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY) {
	assert(0);
    }

    /* if type float then do float */
    if (IS_FLOAT(optype)) {
	assert(0);
    }

    toBoolean(IC_LEFT(ic));

    /* Not of A:
       If A == 0, !A = 1
       else A = 0
       So if A = 0, A-1 = 0xFF and C is set, rotate C into reg. */
    emitcode("sub", "a,#0x01");
    outBitC(IC_RESULT(ic));

    /* release the aops */
    freeAsmop(IC_LEFT(ic),NULL,ic);
    freeAsmop(IC_RESULT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genCpl - generate code for complement                           */
/*-----------------------------------------------------------------*/
static void genCpl (iCode *ic)
{
    int offset = 0;
    int size ;


    /* assign asmOps to operand & result */
    aopOp (IC_LEFT(ic),ic,FALSE);
    aopOp (IC_RESULT(ic),ic,TRUE);

    /* if both are in bit space then 
    a special case */
    if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY &&
        AOP_TYPE(IC_LEFT(ic)) == AOP_CRY ) { 
	assert(0);
    } 

    size = AOP_SIZE(IC_RESULT(ic));
    while (size--) {
        char *l = aopGet(AOP(IC_LEFT(ic)),offset,FALSE);
        MOVA(l);       
        emitcode("cpl","");
        aopPut(AOP(IC_RESULT(ic)),"a",offset++);
    }

    /* release the aops */
    freeAsmop(IC_LEFT(ic),NULL,ic);
    freeAsmop(IC_RESULT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genUminus - unary minus code generation                         */
/*-----------------------------------------------------------------*/
static void genUminus (iCode *ic)
{
    int offset ,size ;
    link *optype, *rtype;

    /* assign asmops */
    aopOp(IC_LEFT(ic),ic,FALSE);
    aopOp(IC_RESULT(ic),ic,TRUE);

    /* if both in bit space then special
    case */
    if (AOP_TYPE(IC_RESULT(ic)) == AOP_CRY &&
        AOP_TYPE(IC_LEFT(ic)) == AOP_CRY ) { 
	assert(0);
        goto release;
    } 

    optype = operandType(IC_LEFT(ic));
    rtype = operandType(IC_RESULT(ic));

    /* if float then do float stuff */
    if (IS_FLOAT(optype)) {
	assert(0);
        goto release;
    }

    /* otherwise subtract from zero */
    size = AOP_SIZE(IC_LEFT(ic));
    offset = 0 ;
    CLRC ;
    while(size--) {
        char *l = aopGet(AOP(IC_LEFT(ic)),offset,FALSE);
	emitcode("ld", "a,#0");
	emitcode("sbc","a,%s",l);
        aopPut(AOP(IC_RESULT(ic)),"a",offset++);
    }

    /* if any remaining bytes in the result */
    /* we just need to propagate the sign   */
    if ((size = (AOP_SIZE(IC_RESULT(ic)) - AOP_SIZE(IC_LEFT(ic))))) {
        emitcode("rlc","a");
        emitcode("sbc","a,a");
        while (size--) 
            aopPut(AOP(IC_RESULT(ic)),"a",offset++);
    }       

release:
    /* release the aops */
    freeAsmop(IC_LEFT(ic),NULL,ic);
    freeAsmop(IC_RESULT(ic),NULL,ic);
}

static bool requiresHL(asmop *aop)
{
    switch (aop->type) {
    case AOP_HL:
    case AOP_STK:
	return TRUE;
    default:
	return FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* assignResultValue -						   */
/*-----------------------------------------------------------------*/
void assignResultValue(operand * oper)
{
    int offset = 0;
    int size = AOP_SIZE(oper);

    assert(size <= 2);

    while (size--) {
	aopPut(AOP(oper),_fReturn[offset],offset);
	offset++;
    }
}

static void fetchHL(asmop *aop)
{
    if (IS_GB && requiresHL(aop)) {
	aopGet(aop, 0, FALSE);
	emitcode("ld", "a,(hl+)");
	emitcode("ld", "h,(hl)");
	emitcode("ld", "l,a");
    }
    else {
	emitcode("ld", "l,%s", aopGet(aop, 0, FALSE));
	emitcode("ld", "h,%s", aopGet(aop, 1, FALSE));
    }
}

/*-----------------------------------------------------------------*/
/* genIpush - genrate code for pushing this gets a little complex  */
/*-----------------------------------------------------------------*/
static void genIpush (iCode *ic)
{
    int size, offset = 0 ;
    char *l;


    /* if this is not a parm push : ie. it is spill push 
       and spill push is always done on the local stack */
    if (!ic->parmPush) {
        /* and the item is spilt then do nothing */
        if (OP_SYMBOL(IC_LEFT(ic))->isspilt)
            return ;

        aopOp(IC_LEFT(ic),ic,FALSE);
        size = AOP_SIZE(IC_LEFT(ic));
        /* push it on the stack */
	if (isPair(AOP(IC_LEFT(ic)))) {
	    emitcode("push", getPairName(AOP(IC_LEFT(ic))));
	    _pushed += 2;
	}
	else {
	    offset = size;
	    while (size--) {
		l = aopGet(AOP(IC_LEFT(ic)), --offset, FALSE);
		/* Simple for now - load into A and PUSH AF */
		emitcode("ld", "a,%s", l);
		emitcode("push", "af");
		emitcode("inc", "sp");
		_pushed++;
	    }
	}
	return ;        
    }

    /* Hmmm... what about saving the currently used registers
       at this point? */

    /* then do the push */
    aopOp(IC_LEFT(ic),ic,FALSE);

    size = AOP_SIZE(IC_LEFT(ic));

    if (isPair(AOP(IC_LEFT(ic)))) {
	_pushed+=2;
	emitcode("push", "%s", getPairName(AOP(IC_LEFT(ic))));
    }
    else {
	if (size == 2) {
	    char *s = aopGetWord(AOP(IC_LEFT(ic)), 0);
	    if (s) {
		emitcode("ld", "hl,%s", s);
		emitcode("push", "hl");
		_pushed+=2;
	    }
	    else {
		/* Optimise here - load into HL then push HL */
		fetchHL(AOP(IC_LEFT(ic)));
		emitcode("push", "hl");
		_pushed += 2;
	    }
	    goto release;
	}
	offset = size;
	while (size--) {
	    l = aopGet(AOP(IC_LEFT(ic)), --offset, FALSE);
	    emitcode("ld", "a,%s", l);
	    emitcode("push", "af");
	    emitcode("inc", "sp");
	    _pushed++;
	}       
    }
 release:
    freeAsmop(IC_LEFT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genIpop - recover the registers: can happen only for spilling   */
/*-----------------------------------------------------------------*/
static void genIpop (iCode *ic)
{
    int size,offset ;


    /* if the temp was not pushed then */
    if (OP_SYMBOL(IC_LEFT(ic))->isspilt)
        return ;

    aopOp(IC_LEFT(ic),ic,FALSE);
    size = AOP_SIZE(IC_LEFT(ic));
    offset = (size-1);
    if (isPair(AOP(IC_LEFT(ic)))) {
	emitcode("pop", getPairName(AOP(IC_LEFT(ic))));
    }
    else {
	while (size--) {
	    emitcode("dec", "sp");
	    emitcode("pop", "hl");
	    aopPut(AOP(IC_LEFT(ic)), "l", offset--);
	}
    }

    freeAsmop(IC_LEFT(ic),NULL,ic);
}

/** Emit the code for a call statement 
 */
static void emitCall (iCode *ic, bool ispcall)
{
    /* if caller saves & we have not saved then */
    if (!ic->regsSaved) {
	/* PENDING */
    }

    /* if send set is not empty then assign */
    if (sendSet) {
	iCode *sic ;
	for (sic = setFirstItem(sendSet) ; sic ; 
	     sic = setNextItem(sendSet)) {
	    int size, offset = 0;
	    aopOp(IC_LEFT(sic),sic,FALSE);
	    size = AOP_SIZE(IC_LEFT(sic));
	    while (size--) {
		char *l = aopGet(AOP(IC_LEFT(sic)),offset,
				FALSE);
		if (strcmp(l, _fReturn[offset]))
		    emitcode("ld","%s,%s",
			     _fReturn[offset],
			     l);
		offset++;
	    }
	    freeAsmop (IC_LEFT(sic),NULL,sic);
	}
	sendSet = NULL;
    }

    if (ispcall) {
	symbol *rlbl = newiTempLabel(NULL);

	emitcode("ld", "hl,#" LABEL_STR, (rlbl->key+100));
	emitcode("push", "hl");
	_pushed += 2;

	aopOp(IC_LEFT(ic),ic,FALSE);
	fetchHL(AOP(IC_LEFT(ic)));
	freeAsmop(IC_LEFT(ic),NULL,ic); 
	
	emitcode("jp", "(hl)");
	emitcode("","%05d$:",(rlbl->key+100));
	_pushed -= 2;
    }
    else {
	/* make the call */
	char *name = OP_SYMBOL(IC_LEFT(ic))->rname[0] ?
	    OP_SYMBOL(IC_LEFT(ic))->rname :
	    OP_SYMBOL(IC_LEFT(ic))->name;
	emitcode("call", "%s", name);
    }

    /* if we need assign a result value */
    if ((IS_ITEMP(IC_RESULT(ic)) && 
         (OP_SYMBOL(IC_RESULT(ic))->nRegs ||
          OP_SYMBOL(IC_RESULT(ic))->spildir )) ||
        IS_TRUE_SYMOP(IC_RESULT(ic)) ) {

        accInUse++;
        aopOp(IC_RESULT(ic),ic,FALSE);
        accInUse--;

	assignResultValue(IC_RESULT(ic));
		
        freeAsmop(IC_RESULT(ic),NULL, ic);
    }

    /* adjust the stack for parameters if required */
    if (IC_LEFT(ic)->parmBytes) {
	int i = IC_LEFT(ic)->parmBytes;
	_pushed -= i;
	if (IS_GB) {
	    emitcode("lda", "sp,%d(sp)", i);
	}
	else {
	    if (i>6) {
		emitcode("ld", "hl,#%d", i);
		emitcode("add", "hl,sp");
		emitcode("ld", "sp,hl");
	    }
	    else {
		while (i>1) {
		    emitcode("pop", "hl");
		    i-=2;
		}
		if (i) 
		    emitcode("inc", "sp");
	    }
	}
    }

}

/*-----------------------------------------------------------------*/
/* genCall - generates a call statement                            */
/*-----------------------------------------------------------------*/
static void genCall (iCode *ic)
{
    emitCall(ic, FALSE);
}

/*-----------------------------------------------------------------*/
/* genPcall - generates a call by pointer statement                */
/*-----------------------------------------------------------------*/
static void genPcall (iCode *ic)
{
    emitCall(ic, TRUE);
}

/*-----------------------------------------------------------------*/
/* resultRemat - result  is rematerializable                       */
/*-----------------------------------------------------------------*/
static int resultRemat (iCode *ic)
{
    if (SKIP_IC(ic) || ic->op == IFX)
        return 0;

    if (IC_RESULT(ic) && IS_ITEMP(IC_RESULT(ic))) {
        symbol *sym = OP_SYMBOL(IC_RESULT(ic));
        if (sym->remat && !POINTER_SET(ic)) 
            return 1;
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/* genFunction - generated code for function entry                 */
/*-----------------------------------------------------------------*/
static void genFunction (iCode *ic)
{
    symbol *sym;
    link *fetype;

    nregssaved = 0;
    /* create the function header */
    emitcode(";","-----------------------------------------");
    emitcode(";"," function %s",(sym = OP_SYMBOL(IC_LEFT(ic)))->name);
    emitcode(";","-----------------------------------------");

    emitcode("","%s:",sym->rname);
    fetype = getSpec(operandType(IC_LEFT(ic)));

    /* if critical function then turn interrupts off */
    if (SPEC_CRTCL(fetype))
        emitcode("di","");

    /* if this is an interrupt service routine then
    save acc, b, dpl, dph  */
    if (IS_ISR(sym->etype)) {
	emitcode("push", "af");
	emitcode("push", "bc");
	emitcode("push", "de");
	emitcode("push", "hl");
    }
    /* PENDING: callee-save etc */

    /* adjust the stack for the function */
    emitcode("push", "bc");
    if (!IS_GB) {
	emitcode("push", "de");
	emitcode("push", "ix");
	emitcode("ld", "ix,#0");
	emitcode("add", "ix,sp");
    }

    _lastStack = sym->stack;

    if (sym->stack) {
	if (IS_GB) {
	    emitcode("lda", "sp,-%d(sp)", sym->stack);
	}
	else {
	    emitcode("ld", "hl,#-%d", sym->stack);
	    emitcode("add", "hl,sp");
	    emitcode("ld", "sp,hl");
	}
    }
    _spoffset = sym->stack;
}

/*-----------------------------------------------------------------*/
/* genEndFunction - generates epilogue for functions               */
/*-----------------------------------------------------------------*/
static void genEndFunction (iCode *ic)
{
    symbol *sym = OP_SYMBOL(IC_LEFT(ic));

    if (IS_ISR(sym->etype)) {
	assert(0);
    }
    else {
        if (SPEC_CRTCL(sym->etype))
            emitcode("ei", "");
	
	/* PENDING: calleeSave */

	/* if debug then send end of function */
 	if (options.debug && currFunc) { 
	    debugLine = 1;
	    emitcode("","C$%s$%d$%d$%d ==.",
		     ic->filename,currFunc->lastLine,
		     ic->level,ic->block); 
	    if (IS_STATIC(currFunc->etype))	    
		emitcode("","XF%s$%s$0$0 ==.",moduleName,currFunc->name); 
	    else
		emitcode("","XG$%s$0$0 ==.",currFunc->name);
	    debugLine = 0;
	}
	if (!IS_GB) {
	    emitcode("ld", "sp,ix");
	    emitcode("pop", "ix");
	    emitcode("pop", "de");
	}
	else {
	    if (_spoffset) {
		emitcode("ld", "hl,#%d", _spoffset);
		emitcode("add", "hl,sp");
		emitcode("ld", "sp,hl");
	    }
	}
	emitcode("pop", "bc");
	emitcode("ret", "");
    }
    _pushed = 0;
    _spoffset = 0;
}

/*-----------------------------------------------------------------*/
/* genRet - generate code for return statement                     */
/*-----------------------------------------------------------------*/
static void genRet (iCode *ic)
{
    char *l;
    /* Errk.  This is a hack until I can figure out how
       to cause dehl to spill on a call */
    int size,offset = 0;
    
    /* if we have no return value then
       just generate the "ret" */
    if (!IC_LEFT(ic)) 
	goto jumpret;       
    
    /* we have something to return then
       move the return value into place */
    aopOp(IC_LEFT(ic),ic,FALSE);
    size = AOP_SIZE(IC_LEFT(ic));
    
    if ((size == 2) && ((l = aopGetWord(AOP(IC_LEFT(ic)), 0)))) {
	if (IS_GB) {
	    emitcode("ld", "de,%s", l);
	}
	else {
	    emitcode("ld", "hl,%s", l);
	}
    }
    else {
	while (size--) {
	    l = aopGet(AOP(IC_LEFT(ic)),offset,
		       FALSE);
	    if (strcmp(_fReturn[offset],l))
		emitcode("ld","%s,%s", _fReturn[offset++],l);
	}
    }
    freeAsmop (IC_LEFT(ic),NULL,ic);
    
 jumpret:
	/* generate a jump to the return label
	   if the next is not the return statement */
    if (!(ic->next && ic->next->op == LABEL &&
	  IC_LABEL(ic->next) == returnLabel))
	
	emitcode("jp",  LABEL_STR ,(returnLabel->key+100));
}

/*-----------------------------------------------------------------*/
/* genLabel - generates a label                                    */
/*-----------------------------------------------------------------*/
static void genLabel (iCode *ic)
{
    /* special case never generate */
    if (IC_LABEL(ic) == entryLabel)
        return ;

    emitcode("", LABEL_STR ":",(IC_LABEL(ic)->key+100));
}

/*-----------------------------------------------------------------*/
/* genGoto - generates a ljmp                                      */
/*-----------------------------------------------------------------*/
static void genGoto (iCode *ic)
{
    emitcode ("jp", LABEL_STR ,(IC_LABEL(ic)->key+100));
}

/*-----------------------------------------------------------------*/
/* genPlusIncr :- does addition with increment if possible         */
/*-----------------------------------------------------------------*/
static bool genPlusIncr (iCode *ic)
{
    unsigned int icount ;
    unsigned int size = getDataSize(IC_RESULT(ic));
    
    /* will try to generate an increment */
    /* if the right side is not a literal 
       we cannot */
    if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT)
        return FALSE;
    
    emitcode("", "; genPlusIncr");

    icount = floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);

    /* If result is a pair */
    if (isPair(AOP(IC_RESULT(ic)))) {
	char *left = aopGetWordLong(AOP(IC_LEFT(ic)), 0, FALSE);
	if (left) {
	    /* Both a lit on the right and a true symbol on the left */
	    emitcode("ld", "%s,#%s + %d", getPairName(AOP(IC_RESULT(ic))), left, icount);
	    return TRUE;
	}
    }

    /* if the literal value of the right hand side
       is greater than 4 then it is not worth it */
    if (icount > 4)
        return FALSE ;

    /* Inc a pair */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
	isPair(AOP(IC_RESULT(ic)))) {
	while (icount--) {
	    emitcode("inc", "%s", getPairName(AOP(IC_RESULT(ic))));
	}
	return TRUE;
    }
    /* if increment 16 bits in register */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
        (size > 1) &&
        (icount == 1)) {
        symbol *tlbl = newiTempLabel(NULL);
	emitcode("inc","%s",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE));
	emitcode(_shortJP, "nz," LABEL_STR ,tlbl->key+100);
    
	emitcode("inc","%s",aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE));
	if(size == 4) {
	    assert(0);
	}
	emitcode("", LABEL_STR ":",tlbl->key+100);
        return TRUE;
    }

    /* If result is a pair */
    if (isPair(AOP(IC_RESULT(ic)))) {
	movLeft2Result(IC_LEFT(ic), 0, IC_RESULT(ic), 0, 0);
	movLeft2Result(IC_LEFT(ic), 1, IC_RESULT(ic), 1, 0);
	while (icount--)
	    emitcode("inc", "%s", getPairName(AOP(IC_RESULT(ic))));
	return TRUE;
    }

    /* if the sizes are greater than 1 then we cannot */
    if (AOP_SIZE(IC_RESULT(ic)) > 1 ||
        AOP_SIZE(IC_LEFT(ic)) > 1   )
        return FALSE ;
    
    /* we can if the aops of the left & result match or
       if they are in registers and the registers are the
       same */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) ) {
	while (icount--)
	    emitcode ("inc","%s",aopGet(AOP(IC_LEFT(ic)),0, FALSE));
	
        return TRUE ;
    }
    
    return FALSE ;
}

/*-----------------------------------------------------------------*/
/* outBitAcc - output a bit in acc                                 */
/*-----------------------------------------------------------------*/
void outBitAcc(operand *result)
{
    symbol *tlbl = newiTempLabel(NULL);
    /* if the result is a bit */
    if (AOP_TYPE(result) == AOP_CRY){
	assert(0);
    }
    else {
        emitcode(_shortJP,"z," LABEL_STR ,tlbl->key+100);
        emitcode("ld","a,%s",one);
        emitcode("", LABEL_STR ":",tlbl->key+100);
        outAcc(result);
    }
}

/*-----------------------------------------------------------------*/
/* genPlus - generates code for addition                           */
/*-----------------------------------------------------------------*/
static void genPlus (iCode *ic)
{
    int size, offset = 0;

    /* special cases :- */

    aopOp (IC_LEFT(ic),ic,FALSE);
    aopOp (IC_RIGHT(ic),ic,FALSE);
    aopOp (IC_RESULT(ic),ic,TRUE);

    /* Swap the left and right operands if:

       if literal, literal on the right or
       if left requires ACC or right is already
       in ACC */

    if ((AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) ||
	(AOP_NEEDSACC(IC_LEFT(ic))) ||
	AOP_TYPE(IC_RIGHT(ic)) == AOP_ACC ){
        operand *t = IC_RIGHT(ic);
        IC_RIGHT(ic) = IC_LEFT(ic);
        IC_LEFT(ic) = t;
    }

    /* if both left & right are in bit
    space */
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
        AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
	/* Cant happen */
	assert(0);
    }

    /* if left in bit space & right literal */
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
        AOP_TYPE(IC_RIGHT(ic)) == AOP_LIT) {
	/* Can happen I guess */
	assert(0);
    }

    /* if I can do an increment instead
    of add then GOOD for ME */
    if (genPlusIncr (ic) == TRUE)
        goto release;   

    size = getDataSize(IC_RESULT(ic));

    /* Special case when left and right are constant */
    if (isPair(AOP(IC_RESULT(ic)))) {
	char *left, *right;
	
	left = aopGetWordLong(AOP(IC_LEFT(ic)), 0, FALSE);
	right = aopGetWordLong(AOP(IC_RIGHT(ic)), 0, FALSE);
	if (left && right) {
	    /* It's a pair */
	    /* PENDING: fix */
	    char buffer[100];
	    sprintf(buffer, "#(%s + %s)", left, right);
	    emitcode("ld", "%s,%s", getPairName(AOP(IC_RESULT(ic))), buffer);
	    goto release;
	}
    }

    while(size--) {
	if (AOP_TYPE(IC_LEFT(ic)) == AOP_ACC) {
	    MOVA(aopGet(AOP(IC_LEFT(ic)),offset,FALSE));
	    if(offset == 0)
		emitcode("add","a,%s",
			 aopGet(AOP(IC_RIGHT(ic)),offset,FALSE));
	    else
		emitcode("adc","a,%s",
			 aopGet(AOP(IC_RIGHT(ic)),offset,FALSE));
	} else {
	    MOVA(aopGet(AOP(IC_RIGHT(ic)),offset,FALSE));
	    if(offset == 0)
		emitcode("add","a,%s",
			 aopGet(AOP(IC_LEFT(ic)),offset,FALSE));
	    else
		emitcode("adc","a,%s",
			 aopGet(AOP(IC_LEFT(ic)),offset,FALSE));
	}
        aopPut(AOP(IC_RESULT(ic)),"a",offset++);      
    }

    /* Some kind of pointer arith. */
    if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
	AOP_SIZE(IC_LEFT(ic)) == 3   &&
	!sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
	assert(0);

     if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
	AOP_SIZE(IC_RIGHT(ic)) == 3   &&
	!sameRegs(AOP(IC_RESULT(ic)),AOP(IC_RIGHT(ic))))
	 assert(0);

   
release:
    freeAsmop(IC_LEFT(ic),NULL,ic);
    freeAsmop(IC_RIGHT(ic),NULL,ic);
    freeAsmop(IC_RESULT(ic),NULL,ic);
    
}

/*-----------------------------------------------------------------*/
/* genMinusDec :- does subtraction with deccrement if possible     */
/*-----------------------------------------------------------------*/
static bool genMinusDec (iCode *ic)
{
    unsigned int icount ;
    unsigned int size = getDataSize(IC_RESULT(ic));

    /* will try to generate an increment */
    /* if the right side is not a literal we cannot */
    if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT)
        return FALSE ;

    /* if the literal value of the right hand side
    is greater than 4 then it is not worth it */
    if ((icount = (unsigned int) floatFromVal (AOP(IC_RIGHT(ic))->aopu.aop_lit)) > 2)
        return FALSE;

    size = getDataSize(IC_RESULT(ic));

#if 0
    /* if increment 16 bits in register */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
        (size > 1) &&
        (icount == 1)) {
        symbol *tlbl = newiTempLabel(NULL);
	emitcode("dec","%s",aopGet(AOP(IC_RESULT(ic)),LSB,FALSE));
	emitcode("jp", "np," LABEL_STR ,tlbl->key+100);
    
	emitcode("dec","%s",aopGet(AOP(IC_RESULT(ic)),MSB16,FALSE));
	if(size == 4) {
	    assert(0);
	}
	emitcode("", LABEL_STR ":",tlbl->key+100);
        return TRUE;
    }
#endif

    /* if decrement 16 bits in register */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic))) &&
        (size > 1) && isPair(AOP(IC_RESULT(ic)))) {
	while (icount--)
	    emitcode("dec", "%s", getPairName(AOP(IC_RESULT(ic))));
        return TRUE;
    }

    /* If result is a pair */
    if (isPair(AOP(IC_RESULT(ic)))) {
	movLeft2Result(IC_LEFT(ic), 0, IC_RESULT(ic), 0, 0);
	movLeft2Result(IC_LEFT(ic), 1, IC_RESULT(ic), 1, 0);
	while (icount--)
	    emitcode("dec", "%s", getPairName(AOP(IC_RESULT(ic))));
	return TRUE;
    }

    /* if the sizes are greater than 1 then we cannot */
    if (AOP_SIZE(IC_RESULT(ic)) > 1 ||
        AOP_SIZE(IC_LEFT(ic)) > 1   )
        return FALSE ;

    /* we can if the aops of the left & result match or if they are in
       registers and the registers are the same */
    if (sameRegs(AOP(IC_LEFT(ic)), AOP(IC_RESULT(ic)))) {
        while (icount--) 
            emitcode ("dec","%s",aopGet(AOP(IC_RESULT(ic)),0,FALSE));
        return TRUE ;
    }

    return FALSE ;
}

/*-----------------------------------------------------------------*/
/* genMinus - generates code for subtraction                       */
/*-----------------------------------------------------------------*/
static void genMinus (iCode *ic)
{
    int size, offset = 0;
    unsigned long lit = 0L;

    aopOp (IC_LEFT(ic),ic,FALSE);
    aopOp (IC_RIGHT(ic),ic,FALSE);
    aopOp (IC_RESULT(ic),ic,TRUE);

    /* special cases :- */
    /* if both left & right are in bit space */
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_CRY &&
        AOP_TYPE(IC_RIGHT(ic)) == AOP_CRY) {
	assert(0);
        goto release ;
    }

    /* if I can do an decrement instead of subtract then GOOD for ME */
    if (genMinusDec (ic) == TRUE)
        goto release;   

    size = getDataSize(IC_RESULT(ic));   

    if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT){
    }
    else{
        lit = (unsigned long)floatFromVal(AOP(IC_RIGHT(ic))->aopu.aop_lit);
        lit = - (long)lit;
    }


    /* if literal, add a,#-lit, else normal subb */
    while (size--) {
	MOVA(aopGet(AOP(IC_LEFT(ic)),offset,FALSE));    
	if (AOP_TYPE(IC_RIGHT(ic)) != AOP_LIT) {
	    if (!offset)
		emitcode("sub","a,%s",
			 aopGet(AOP(IC_RIGHT(ic)),offset,FALSE));
	    else
		emitcode("sbc","a,%s",
			 aopGet(AOP(IC_RIGHT(ic)),offset,FALSE));
	}
	else{
	    /* first add without previous c */
	    if (!offset)
		emitcode("add","a,#0x%02x",
			 (unsigned int)(lit & 0x0FFL));
	    else
		emitcode("adc","a,#0x%02x",
			 (unsigned int)((lit >> (offset*8)) & 0x0FFL));
	}
	aopPut(AOP(IC_RESULT(ic)),"a",offset++);      
    }
    
    if (AOP_SIZE(IC_RESULT(ic)) == 3 && 
	AOP_SIZE(IC_LEFT(ic)) == 3   &&
	!sameRegs(AOP(IC_RESULT(ic)),AOP(IC_LEFT(ic))))
	assert(0);

release:
    freeAsmop(IC_LEFT(ic),NULL,ic);
    freeAsmop(IC_RIGHT(ic),NULL,ic);
    freeAsmop(IC_RESULT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genMult - generates code for multiplication                     */
/*-----------------------------------------------------------------*/
static void genMult (iCode *ic)
{
    /* Shouldn't occur - all done through function calls */
    assert(0);
}

/*-----------------------------------------------------------------*/
/* genDiv - generates code for division                            */
/*-----------------------------------------------------------------*/
static void genDiv (iCode *ic)
{
    /* Shouldn't occur - all done through function calls */
    assert(0);
}

/*-----------------------------------------------------------------*/
/* genMod - generates code for division                            */
/*-----------------------------------------------------------------*/
static void genMod (iCode *ic)
{
    /* Shouldn't occur - all done through function calls */
    assert(0);
}

/*-----------------------------------------------------------------*/
/* genIfxJump :- will create a jump depending on the ifx           */
/*-----------------------------------------------------------------*/
static void genIfxJump (iCode *ic, char *jval)
{
    symbol *jlbl ;
    const char *inst;

    /* if true label then we jump if condition
    supplied is true */
    if ( IC_TRUE(ic) ) {
        jlbl = IC_TRUE(ic);
	if (!strcmp(jval, "a")) {
	    inst = "nz";
	}
	else if (!strcmp(jval, "c")) {
	    inst = "c";
	}
	else {
	    /* The buffer contains the bit on A that we should test */
	    inst = "nz";
	}
    }
    else {
        /* false label is present */
        jlbl = IC_FALSE(ic) ;
	if (!strcmp(jval, "a")) {
	    inst = "z";
	}
	else if (!strcmp(jval, "c")) {
	    inst = "nc";
	}
	else {
	    /* The buffer contains the bit on A that we should test */
	    inst = "z";
	}
    }
    /* Z80 can do a conditional long jump */
    if (!strcmp(jval, "a")) {
	emitcode("or", "a,a");
    }
    else if (!strcmp(jval, "c")) {
    }
    else {
	emitcode("bit", "%s,a", jval);
    }
    emitcode("jp", "%s," LABEL_STR , inst, jlbl->key+100);

    /* mark the icode as generated */
    ic->generated = 1;
}

/** Generic compare for > or <
 */
static void genCmp (operand *left,operand *right,
                    operand *result, iCode *ifx, int sign)
{
    int size, offset = 0 ;
    unsigned long lit = 0L;

    /* if left & right are bit variables */
    if (AOP_TYPE(left) == AOP_CRY &&
        AOP_TYPE(right) == AOP_CRY ) {
	/* Cant happen on the Z80 */
	assert(0);
    } else {
        /* subtract right from left if at the
        end the carry flag is set then we know that
        left is greater than right */
        size = max(AOP_SIZE(left),AOP_SIZE(right));

        /* if unsigned char cmp with lit, just compare */
        if((size == 1) && 
           (AOP_TYPE(right) == AOP_LIT && AOP_TYPE(left) != AOP_DIR )){
	    emitcode("ld", "a,%s", aopGet(AOP(left), offset, FALSE));
	    if (sign) {
		emitcode("xor", "a,#0x80");
		emitcode("cp", "%s^0x80", aopGet(AOP(right), offset, FALSE));
	    }
	    else 
		emitcode("cp", "%s ; 7", aopGet(AOP(right), offset, FALSE));
        } 
	else {
            if(AOP_TYPE(right) == AOP_LIT) {
                lit = (unsigned long)floatFromVal(AOP(right)->aopu.aop_lit);
                /* optimize if(x < 0) or if(x >= 0) */
                if (lit == 0L){
                    if (!sign) {
			/* No sign so it's always false */
                        CLRC;
                    }
                    else{
			/* Just load in the top most bit */
                        MOVA(aopGet(AOP(left),AOP_SIZE(left)-1,FALSE));
                        if(!(AOP_TYPE(result) == AOP_CRY && AOP_SIZE(result)) && ifx) {
                            genIfxJump (ifx,"7");
                            return;
                        }
                        else    
                            emitcode("rlc","a");
                    }
                    goto release;
                }
            }
	    if (sign) {
		/* First setup h and l contaning the top most bytes XORed */
		bool fDidXor = FALSE;
		if (AOP_TYPE(left) == AOP_LIT){
		    unsigned long lit = (unsigned long)
			floatFromVal(AOP(left)->aopu.aop_lit);
		    emitcode("ld", "%s,#0x%02x", _fTmp[0],
			     0x80 ^ (unsigned int)((lit >> ((size-1)*8)) & 0x0FFL));
		}
		else {
		    emitcode("ld", "a,%s", aopGet(AOP(left), size-1, FALSE));
		    emitcode("xor", "a,#0x80");
		    emitcode("ld", "%s,a", _fTmp[0]);
		    fDidXor = TRUE;
		}
		if (AOP_TYPE(right) == AOP_LIT) {
		    unsigned long lit = (unsigned long)
			floatFromVal(AOP(right)->aopu.aop_lit);
		    emitcode("ld", "%s,#0x%02x", _fTmp[1],
			     0x80 ^ (unsigned int)((lit >> ((size-1)*8)) & 0x0FFL));
		}
		else {
		    emitcode("ld", "a,%s", aopGet(AOP(right), size-1, FALSE));
		    emitcode("xor", "a,#0x80");
		    emitcode("ld", "%s,a", _fTmp[1]);
		    fDidXor = TRUE;
		}
		if (!fDidXor)
		    CLRC;
	    }
	    else {
		CLRC;
	    }
            while (size--) {
		/* Do a long subtract */
		if (!sign || size) 
		    MOVA(aopGet(AOP(left),offset,FALSE));
                if (sign && size == 0) {
		    emitcode("ld", "a,%s", _fTmp[0]);
		    emitcode("sbc", "a,%s", _fTmp[1]);
		}
		else {
		    /* Subtract through, propagating the carry */
		    emitcode("sbc","a,%s",aopGet(AOP(right),offset++,FALSE));
		}
            }
        }
    }

release:
    if (AOP_TYPE(result) == AOP_CRY && AOP_SIZE(result)) {
        outBitC(result);
    } else {
        /* if the result is used in the next
        ifx conditional branch then generate
        code a little differently */
        if (ifx )
            genIfxJump (ifx,"c");
        else
            outBitC(result);
        /* leave the result in acc */
    }
}

/*-----------------------------------------------------------------*/
/* genCmpGt :- greater than comparison                             */
/*-----------------------------------------------------------------*/
static void genCmpGt (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;
    link *letype , *retype;
    int sign ;

    left = IC_LEFT(ic);
    right= IC_RIGHT(ic);
    result = IC_RESULT(ic);

    letype = getSpec(operandType(left));
    retype =getSpec(operandType(right));
    sign =  !(SPEC_USIGN(letype) | SPEC_USIGN(retype));
    /* assign the amsops */
    aopOp (left,ic,FALSE);
    aopOp (right,ic,FALSE);
    aopOp (result,ic,TRUE);

    genCmp(right, left, result, ifx, sign);

    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genCmpLt - less than comparisons                                */
/*-----------------------------------------------------------------*/
static void genCmpLt (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;
    link *letype , *retype;
    int sign ;

    left = IC_LEFT(ic);
    right= IC_RIGHT(ic);
    result = IC_RESULT(ic);

    letype = getSpec(operandType(left));
    retype =getSpec(operandType(right));
    sign =  !(SPEC_USIGN(letype) | SPEC_USIGN(retype));

    /* assign the amsops */
    aopOp (left,ic,FALSE);
    aopOp (right,ic,FALSE);
    aopOp (result,ic,TRUE);

    genCmp(left, right, result, ifx, sign);

    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* gencjneshort - compare and jump if not equal                    */
/*-----------------------------------------------------------------*/
static void gencjneshort(operand *left, operand *right, symbol *lbl)
{
    int size = max(AOP_SIZE(left),AOP_SIZE(right));
    int offset = 0;
    unsigned long lit = 0L;

    /* Swap the left and right if it makes the computation easier */
    if (AOP_TYPE(left) == AOP_LIT) {
        operand *t = right;
        right = left;
        left = t;
    }

    if(AOP_TYPE(right) == AOP_LIT)
        lit = (unsigned long)floatFromVal(AOP(right)->aopu.aop_lit);

    /* if the right side is a literal then anything goes */
    if (AOP_TYPE(right) == AOP_LIT &&
        AOP_TYPE(left) != AOP_DIR ) {
        while (size--) {
	    emitcode("ld", "a,%s ; 2", aopGet(AOP(left),offset,FALSE));
	    if ((AOP_TYPE(right) == AOP_LIT) && lit == 0)
		emitcode("or", "a,a");
	    else 
		emitcode("cp", "a,%s", aopGet(AOP(right),offset,FALSE));
            emitcode("jp", "nz," LABEL_STR , lbl->key+100);
            offset++;
        }
    }
    /* if the right side is in a register or in direct space or
    if the left is a pointer register & right is not */    
    else if (AOP_TYPE(right) == AOP_REG ||
             AOP_TYPE(right) == AOP_DIR || 
             (AOP_TYPE(left) == AOP_DIR && AOP_TYPE(right) == AOP_LIT)) {
        while (size--) {
            MOVA(aopGet(AOP(left),offset,FALSE));
            if((AOP_TYPE(left) == AOP_DIR && AOP_TYPE(right) == AOP_LIT) &&
               ((unsigned int)((lit >> (offset*8)) & 0x0FFL) == 0))
		/* PENDING */
                emitcode("jp","nz," LABEL_STR ,lbl->key+100);
            else {
		emitcode("cp", "%s ; 4", aopGet(AOP(right),offset,FALSE));
		emitcode("jp", "nz," LABEL_STR , lbl->key+100);
	    }
            offset++;
        }
    } else {
        /* right is a pointer reg need both a & b */
	/* PENDING: is this required? */
        while(size--) {
            char *l = aopGet(AOP(left),offset,FALSE);
            MOVA(aopGet(AOP(right),offset,FALSE));
	    emitcode("cp", "%s ; 5", l);
	    emitcode("jr", "nz," LABEL_STR, lbl->key+100);
            offset++;
        }
    }
}

/*-----------------------------------------------------------------*/
/* gencjne - compare and jump if not equal                         */
/*-----------------------------------------------------------------*/
static void gencjne(operand *left, operand *right, symbol *lbl)
{
    symbol *tlbl  = newiTempLabel(NULL);

    gencjneshort(left, right, lbl);

    /* PENDING: ?? */
    emitcode("ld","a,%s",one);
    emitcode(_shortJP, LABEL_STR ,tlbl->key+100);
    emitcode("", LABEL_STR ":",lbl->key+100);
    emitcode("xor","a,a");
    emitcode("", LABEL_STR ":",tlbl->key+100);
}

/*-----------------------------------------------------------------*/
/* genCmpEq - generates code for equal to                          */
/*-----------------------------------------------------------------*/
static void genCmpEq (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;

    aopOp((left=IC_LEFT(ic)),ic,FALSE);
    aopOp((right=IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,TRUE);

    /* Swap operands if it makes the operation easier. ie if:
       1.  Left is a literal.
    */
    if (AOP_TYPE(IC_LEFT(ic)) == AOP_LIT) {
        operand *t = IC_RIGHT(ic);
        IC_RIGHT(ic) = IC_LEFT(ic);
        IC_LEFT(ic) = t;
    }

    if (ifx && !AOP_SIZE(result)){
        symbol *tlbl;
        /* if they are both bit variables */
        if (AOP_TYPE(left) == AOP_CRY &&
            ((AOP_TYPE(right) == AOP_CRY) || (AOP_TYPE(right) == AOP_LIT))) {
	    assert(0);
        } else {
            tlbl = newiTempLabel(NULL);
            gencjneshort(left, right, tlbl);
            if ( IC_TRUE(ifx) ) {
                emitcode("jp", LABEL_STR ,IC_TRUE(ifx)->key+100);
                emitcode("", LABEL_STR ":",tlbl->key+100);                
            } else {
		/* PENDING: do this better */
                symbol *lbl = newiTempLabel(NULL);
                emitcode(_shortJP, LABEL_STR ,lbl->key+100);
                emitcode("", LABEL_STR ":",tlbl->key+100);                
                emitcode("jp", LABEL_STR ,IC_FALSE(ifx)->key+100);
                emitcode("", LABEL_STR ":",lbl->key+100);             
            }
        }
        /* mark the icode as generated */
        ifx->generated = 1;
        goto release ;
    }

    /* if they are both bit variables */
    if (AOP_TYPE(left) == AOP_CRY &&
        ((AOP_TYPE(right) == AOP_CRY) || (AOP_TYPE(right) == AOP_LIT))) {
	assert(0);
    } else {
        gencjne(left,right,newiTempLabel(NULL));    
        if (AOP_TYPE(result) == AOP_CRY && AOP_SIZE(result)) {
	    assert(0);
        }
        if (ifx) {
	    genIfxJump(ifx,"a");
	    goto release;
        }
        /* if the result is used in an arithmetic operation
        then put the result in place */
        if (AOP_TYPE(result) != AOP_CRY) {
            outAcc(result);
	}
        /* leave the result in acc */
    }

release:
    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* ifxForOp - returns the icode containing the ifx for operand     */
/*-----------------------------------------------------------------*/
static iCode *ifxForOp ( operand *op, iCode *ic )
{
    /* if true symbol then needs to be assigned */
    if (IS_TRUE_SYMOP(op))
        return NULL ;

    /* if this has register type condition and
    the next instruction is ifx with the same operand
    and live to of the operand is upto the ifx only then */
    if (ic->next &&
        ic->next->op == IFX &&
        IC_COND(ic->next)->key == op->key &&
        OP_SYMBOL(op)->liveTo <= ic->next->seq )
        return ic->next;

    return NULL;
}

/*-----------------------------------------------------------------*/
/* genAndOp - for && operation                                     */
/*-----------------------------------------------------------------*/
static void genAndOp (iCode *ic)
{
    operand *left,*right, *result;
    symbol *tlbl;

    /* note here that && operations that are in an if statement are
       taken away by backPatchLabels only those used in arthmetic
       operations remain */
    aopOp((left=IC_LEFT(ic)),ic,FALSE);
    aopOp((right=IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,FALSE);

    /* if both are bit variables */
    if (AOP_TYPE(left) == AOP_CRY &&
        AOP_TYPE(right) == AOP_CRY ) {
	assert(0);
    } else {
        tlbl = newiTempLabel(NULL);
        toBoolean(left);    
        emitcode(_shortJP, "z," LABEL_STR ,tlbl->key+100);
        toBoolean(right);
        emitcode("", LABEL_STR ":",tlbl->key+100);
        outBitAcc(result);
    }

    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genOrOp - for || operation                                      */
/*-----------------------------------------------------------------*/
static void genOrOp (iCode *ic)
{
    operand *left,*right, *result;
    symbol *tlbl;

    /* note here that || operations that are in an
       if statement are taken away by backPatchLabels
       only those used in arthmetic operations remain */
    aopOp((left=IC_LEFT(ic)),ic,FALSE);
    aopOp((right=IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,FALSE);

    /* if both are bit variables */
    if (AOP_TYPE(left) == AOP_CRY &&
        AOP_TYPE(right) == AOP_CRY ) {
	assert(0);
    } else {
        tlbl = newiTempLabel(NULL);
        toBoolean(left);
        emitcode(_shortJP, "nz," LABEL_STR,tlbl->key+100);
        toBoolean(right);
        emitcode("", LABEL_STR ":",tlbl->key+100);
        outBitAcc(result);
    }

    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* isLiteralBit - test if lit == 2^n                               */
/*-----------------------------------------------------------------*/
int isLiteralBit(unsigned long lit)
{
    unsigned long pw[32] = {1L,2L,4L,8L,16L,32L,64L,128L,
    0x100L,0x200L,0x400L,0x800L,
    0x1000L,0x2000L,0x4000L,0x8000L,
    0x10000L,0x20000L,0x40000L,0x80000L,
    0x100000L,0x200000L,0x400000L,0x800000L,
    0x1000000L,0x2000000L,0x4000000L,0x8000000L,
    0x10000000L,0x20000000L,0x40000000L,0x80000000L};
    int idx;
    
    for(idx = 0; idx < 32; idx++)
        if(lit == pw[idx])
            return idx+1;
    return 0;
}

/*-----------------------------------------------------------------*/
/* jmpTrueOrFalse -                                                */
/*-----------------------------------------------------------------*/
static void jmpTrueOrFalse (iCode *ic, symbol *tlbl)
{
    // ugly but optimized by peephole
    if(IC_TRUE(ic)){
        symbol *nlbl = newiTempLabel(NULL);
        emitcode("jp", LABEL_STR, nlbl->key+100);                 
        emitcode("", LABEL_STR ":",tlbl->key+100);
        emitcode("jp",LABEL_STR,IC_TRUE(ic)->key+100);
        emitcode("", LABEL_STR ":",nlbl->key+100);
    }
    else{
        emitcode("jp", LABEL_STR, IC_FALSE(ic)->key+100);
        emitcode("", LABEL_STR ":",tlbl->key+100);
    }
    ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genAnd  - code for and                                          */
/*-----------------------------------------------------------------*/
static void genAnd (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;
    int size, offset=0;  
    unsigned long lit = 0L;
    int bytelit = 0;

    aopOp((left = IC_LEFT(ic)),ic,FALSE);
    aopOp((right= IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,TRUE);

#ifdef DEBUG_TYPE
    emitcode("","; Type res[%d] = l[%d]&r[%d]",
             AOP_TYPE(result),
             AOP_TYPE(left), AOP_TYPE(right));
    emitcode("","; Size res[%d] = l[%d]&r[%d]",
             AOP_SIZE(result),
             AOP_SIZE(left), AOP_SIZE(right));
#endif

    /* if left is a literal & right is not then exchange them */
    if ((AOP_TYPE(left) == AOP_LIT && AOP_TYPE(right) != AOP_LIT) ||
	AOP_NEEDSACC(left)) {
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if result = right then exchange them */
    if(sameRegs(AOP(result),AOP(right))){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if right is bit then exchange them */
    if (AOP_TYPE(right) == AOP_CRY &&
        AOP_TYPE(left) != AOP_CRY){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }
    if(AOP_TYPE(right) == AOP_LIT)
        lit = (unsigned long)floatFromVal (AOP(right)->aopu.aop_lit);

    size = AOP_SIZE(result);

    if (AOP_TYPE(left) == AOP_CRY){
	assert(0);
        goto release ;
    }

    // if(val & 0xZZ)       - size = 0, ifx != FALSE  -
    // bit = val & 0xZZ     - size = 1, ifx = FALSE -
    if((AOP_TYPE(right) == AOP_LIT) &&
       (AOP_TYPE(result) == AOP_CRY) &&
       (AOP_TYPE(left) != AOP_CRY)) {
        int posbit = isLiteralBit(lit);
        /* left &  2^n */
        if(posbit){
            posbit--;
            MOVA(aopGet(AOP(left),posbit>>3,FALSE));
            // bit = left & 2^n
            if(size) {
		assert(0);
                emitcode("mov","c,acc.%d",posbit&0x07);
	    }
            // if(left &  2^n)
            else{
                if (ifx) {
                    sprintf(buffer, "%d", posbit&0x07);
                    genIfxJump(ifx, buffer);
                }
		else {
		    assert(0);
		}
                goto release;
            }
        } else {
            symbol *tlbl = newiTempLabel(NULL);
            int sizel = AOP_SIZE(left);
            if(size) {
		assert(0);
                emitcode("setb","c");
	    }
            while(sizel--){
                if((bytelit = ((lit >> (offset*8)) & 0x0FFL)) != 0x0L){
                    MOVA( aopGet(AOP(left),offset,FALSE));
                    // byte ==  2^n ?
                    if((posbit = isLiteralBit(bytelit)) != 0) {
			assert(0);
                        emitcode("jb","acc.%d,%05d$",(posbit-1)&0x07,tlbl->key+100);
		    }
                    else{
                        if(bytelit != 0x0FFL)
                            emitcode("and","a,%s",
                                     aopGet(AOP(right),offset,FALSE));
                        emitcode("jr","nz, %05d$",tlbl->key+100);
                    }
                }
                offset++;
            }
            // bit = left & literal
            if (size){
                emitcode("clr","c");
                emitcode("","%05d$:",tlbl->key+100);
            }
            // if(left & literal)
            else{
                if(ifx)
                    jmpTrueOrFalse(ifx, tlbl);
                goto release ;
            }
        }
        outBitC(result);
        goto release ;
    }

    /* if left is same as result */
    if(sameRegs(AOP(result),AOP(left))){
        for(;size--; offset++) {
            if(AOP_TYPE(right) == AOP_LIT){
                if((bytelit = (int)((lit >> (offset*8)) & 0x0FFL)) == 0x0FF)
                    continue;
                else {
		    if (bytelit == 0)
			aopPut(AOP(result),zero,offset);
		    else {
			MOVA(aopGet(AOP(left),offset,FALSE));
			emitcode("and","a,%s",
				 aopGet(AOP(right),offset,FALSE));
			emitcode("ld", "%s,a", aopGet(AOP(left),offset,FALSE));
		    }
		}

            } else {
		if (AOP_TYPE(left) == AOP_ACC) {
		    assert(0);
		}
		else {
		    MOVA(aopGet(AOP(right),offset,FALSE));
		    emitcode("and","%s,a",
			     aopGet(AOP(left),offset,FALSE));
		}
            }
        }
    } else {
        // left & result in different registers
        if(AOP_TYPE(result) == AOP_CRY){
	    assert(0);
        } else {
	    for(;(size--);offset++) {
		// normal case
		// result = left & right
		if(AOP_TYPE(right) == AOP_LIT){
		    if((bytelit = (int)((lit >> (offset*8)) & 0x0FFL)) == 0x0FF){
			aopPut(AOP(result),
			       aopGet(AOP(left),offset,FALSE),
			       offset);
			continue;
		    } else if(bytelit == 0){
			aopPut(AOP(result),zero,offset);
			continue;
		    }
		}
		// faster than result <- left, anl result,right
		// and better if result is SFR
		if (AOP_TYPE(left) == AOP_ACC) 
		    emitcode("and","a,%s",aopGet(AOP(right),offset,FALSE));
		else {
		    MOVA(aopGet(AOP(right),offset,FALSE));
		    emitcode("and","a,%s",
			     aopGet(AOP(left),offset,FALSE));
		}
		aopPut(AOP(result),"a",offset);
	    }
	}

    }

release :
    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genOr  - code for or                                            */
/*-----------------------------------------------------------------*/
static void genOr (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;
    int size, offset=0;
    unsigned long lit = 0L;

    aopOp((left = IC_LEFT(ic)),ic,FALSE);
    aopOp((right= IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,TRUE);

#if 1
    emitcode("","; Type res[%d] = l[%d]&r[%d]",
             AOP_TYPE(result),
             AOP_TYPE(left), AOP_TYPE(right));
    emitcode("","; Size res[%d] = l[%d]&r[%d]",
             AOP_SIZE(result),
             AOP_SIZE(left), AOP_SIZE(right));
#endif

    /* if left is a literal & right is not then exchange them */
    if ((AOP_TYPE(left) == AOP_LIT && AOP_TYPE(right) != AOP_LIT) ||
	AOP_NEEDSACC(left)) {
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if result = right then exchange them */
    if(sameRegs(AOP(result),AOP(right))){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if right is bit then exchange them */
    if (AOP_TYPE(right) == AOP_CRY &&
        AOP_TYPE(left) != AOP_CRY){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }
    if(AOP_TYPE(right) == AOP_LIT)
        lit = (unsigned long)floatFromVal (AOP(right)->aopu.aop_lit);

    size = AOP_SIZE(result);

    if (AOP_TYPE(left) == AOP_CRY){
	assert(0);
        goto release ;
    }

    if((AOP_TYPE(right) == AOP_LIT) &&
       (AOP_TYPE(result) == AOP_CRY) &&
       (AOP_TYPE(left) != AOP_CRY)){
	assert(0);
        goto release ;
    }

    /* if left is same as result */
    if(sameRegs(AOP(result),AOP(left))){
        for(;size--; offset++) {
            if(AOP_TYPE(right) == AOP_LIT){
                if(((lit >> (offset*8)) & 0x0FFL) == 0x00L)
                    continue;
                else 
		    emitcode("or","%s,%s; 5",
			     aopGet(AOP(left),offset,FALSE),
			     aopGet(AOP(right),offset,FALSE));
            } else {
		if (AOP_TYPE(left) == AOP_ACC) 
		    emitcode("or","a,%s ; 6",aopGet(AOP(right),offset,FALSE));
		else {		    
		    MOVA(aopGet(AOP(right),offset,FALSE));
		    emitcode("or","a,%s ; 7",
			     aopGet(AOP(left),offset,FALSE));
		    aopPut(AOP(result),"a ; 8", offset);
		}
            }
        }
    } else {
        // left & result in different registers
        if(AOP_TYPE(result) == AOP_CRY){
	    assert(0);
        } else for(;(size--);offset++){
            // normal case
            // result = left & right
            if(AOP_TYPE(right) == AOP_LIT){
                if(((lit >> (offset*8)) & 0x0FFL) == 0x00L){
                    aopPut(AOP(result),
                           aopGet(AOP(left),offset,FALSE),
                           offset);
                    continue;
                }
            }
            // faster than result <- left, anl result,right
            // and better if result is SFR
	    if (AOP_TYPE(left) == AOP_ACC) 
		emitcode("or","a,%s",aopGet(AOP(right),offset,FALSE));
	    else {
		MOVA(aopGet(AOP(right),offset,FALSE));
		emitcode("or","a,%s",
			 aopGet(AOP(left),offset,FALSE));
	    }
	    aopPut(AOP(result),"a",offset);			
	    /* PENDING: something weird is going on here.  Add exception. */
	    if (AOP_TYPE(result) == AOP_ACC)
		break;
        }
    }

release :
    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genXor - code for xclusive or                                   */
/*-----------------------------------------------------------------*/
static void genXor (iCode *ic, iCode *ifx)
{
    operand *left, *right, *result;
    int size, offset=0;
    unsigned long lit = 0L;

    aopOp((left = IC_LEFT(ic)),ic,FALSE);
    aopOp((right= IC_RIGHT(ic)),ic,FALSE);
    aopOp((result=IC_RESULT(ic)),ic,TRUE);

    /* if left is a literal & right is not then exchange them */
    if ((AOP_TYPE(left) == AOP_LIT && AOP_TYPE(right) != AOP_LIT) ||
	AOP_NEEDSACC(left)) {
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if result = right then exchange them */
    if(sameRegs(AOP(result),AOP(right))){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }

    /* if right is bit then exchange them */
    if (AOP_TYPE(right) == AOP_CRY &&
        AOP_TYPE(left) != AOP_CRY){
        operand *tmp = right ;
        right = left;
        left = tmp;
    }
    if(AOP_TYPE(right) == AOP_LIT)
        lit = (unsigned long)floatFromVal (AOP(right)->aopu.aop_lit);

    size = AOP_SIZE(result);

    if (AOP_TYPE(left) == AOP_CRY){
	assert(0);
        goto release ;
    }

    if((AOP_TYPE(right) == AOP_LIT) &&
       (AOP_TYPE(result) == AOP_CRY) &&
       (AOP_TYPE(left) != AOP_CRY)){
	assert(0);
        goto release ;
    }

    /* if left is same as result */
    if(sameRegs(AOP(result),AOP(left))){
        for(;size--; offset++) {
            if(AOP_TYPE(right) == AOP_LIT){
                if(((lit >> (offset*8)) & 0x0FFL) == 0x00L)
                    continue;
                else {
		    MOVA(aopGet(AOP(right),offset,FALSE));
		    emitcode("xor","a,%s",
			     aopGet(AOP(left),offset,FALSE));
		    aopPut(AOP(result),"a",0);
		}
            } else {
		if (AOP_TYPE(left) == AOP_ACC) 
		    emitcode("xor","a,%s",aopGet(AOP(right),offset,FALSE));
		else {		    
		    MOVA(aopGet(AOP(right),offset,FALSE));
		    emitcode("xor","a,%s",
			     aopGet(AOP(left),offset,FALSE));
		    aopPut(AOP(result),"a",0);
		}
            }
        }
    } else {
        // left & result in different registers
        if(AOP_TYPE(result) == AOP_CRY){
	    assert(0);
        } else for(;(size--);offset++){
            // normal case
            // result = left & right
            if(AOP_TYPE(right) == AOP_LIT){
                if(((lit >> (offset*8)) & 0x0FFL) == 0x00L){
                    aopPut(AOP(result),
                           aopGet(AOP(left),offset,FALSE),
                           offset);
                    continue;
                }
            }
            // faster than result <- left, anl result,right
            // and better if result is SFR
	    if (AOP_TYPE(left) == AOP_ACC) 
		emitcode("xor","a,%s",aopGet(AOP(right),offset,FALSE));
	    else {
		MOVA(aopGet(AOP(right),offset,FALSE));
		emitcode("xor","a,%s",
			 aopGet(AOP(left),offset,FALSE));
		aopPut(AOP(result),"a",0);
	    }
	    aopPut(AOP(result),"a",offset);			
        }
    }

release :
    freeAsmop(left,NULL,ic);
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genInline - write the inline code out                           */
/*-----------------------------------------------------------------*/
static void genInline (iCode *ic)
{
    char buffer[MAX_INLINEASM];
    char *bp = buffer;
    char *bp1= buffer;
    
    inLine += (!options.asmpeep);
    strcpy(buffer,IC_INLINE(ic));

    /* emit each line as a code */
    while (*bp) {
        if (*bp == '\n') {
            *bp++ = '\0';
            emitcode(bp1,"");
            bp1 = bp;
        } else {
            if (*bp == ':') {
                bp++;
                *bp = '\0';
                bp++;
                emitcode(bp1,"");
                bp1 = bp;
            } else
                bp++;
        }
    }
    if (bp1 != bp)
        emitcode(bp1,"");
    /*     emitcode("",buffer); */
    inLine -= (!options.asmpeep);
}

/*-----------------------------------------------------------------*/
/* genRRC - rotate right with carry                                */
/*-----------------------------------------------------------------*/
static void genRRC (iCode *ic)
{
    assert(0);
}

/*-----------------------------------------------------------------*/
/* genRLC - generate code for rotate left with carry               */
/*-----------------------------------------------------------------*/
static void genRLC (iCode *ic)
{    
    assert(0);
}

/*-----------------------------------------------------------------*/
/* shiftR2Left2Result - shift right two bytes from left to result  */
/*-----------------------------------------------------------------*/
static void shiftR2Left2Result (operand *left, int offl,
                                operand *result, int offr,
                                int shCount, int sign)
{
    if(sameRegs(AOP(result), AOP(left)) &&
       ((offl + MSB16) == offr)){
	assert(0);
    } else {
	movLeft2Result(left, offl, result, offr, 0);
	movLeft2Result(left, offl+1, result, offr+1, 0);
    }

    if (sign) {
	assert(0);
    }
    else {
	/*	if (AOP(result)->type == AOP_REG) {*/
	    int size = 2;
	    int offset = 0;
	    symbol *tlbl , *tlbl1;
	    char *l;

	    /* Left is already in result - so now do the shift */
	    if (shCount>1) {
		emitcode("ld","a,#%u+1", shCount);
		tlbl = newiTempLabel(NULL);
		tlbl1 = newiTempLabel(NULL);
		emitcode(_shortJP, LABEL_STR ,tlbl1->key+100); 
		emitcode("", LABEL_STR ":",tlbl->key+100);    
	    }

	    emitcode("or", "a,a");
	    offset = size;
	    while (size--) {
		l = aopGet(AOP(result), --offset, FALSE);
		emitcode("rr","%s", l);         
	    }
	    if (shCount>1) {
		emitcode("", LABEL_STR ":",tlbl1->key+100);
		emitcode("dec", "a");
		emitcode(_shortJP,"nz," LABEL_STR ,tlbl->key+100);
	    }
    }
}

/*-----------------------------------------------------------------*/
/* shiftL2Left2Result - shift left two bytes from left to result   */
/*-----------------------------------------------------------------*/
static void shiftL2Left2Result (operand *left, int offl,
                                operand *result, int offr, int shCount)
{
    if(sameRegs(AOP(result), AOP(left)) &&
       ((offl + MSB16) == offr)){
	assert(0);
    } else {
	/* Copy left into result */
	movLeft2Result(left, offl, result, offr, 0);
	movLeft2Result(left, offl+1, result, offr+1, 0);
    }
    /* PENDING: for now just see if it'll work. */
    /*if (AOP(result)->type == AOP_REG) { */
    {
	int size = 2;
	int offset = 0;
	symbol *tlbl , *tlbl1;
	char *l;

	/* Left is already in result - so now do the shift */
	if (shCount>1) {
	    emitcode("ld","a,#%u+1", shCount);
	    tlbl = newiTempLabel(NULL);
	    tlbl1 = newiTempLabel(NULL);
	    emitcode(_shortJP, LABEL_STR ,tlbl1->key+100); 
	    emitcode("", LABEL_STR ":",tlbl->key+100);    
	}

	emitcode("or", "a,a");
	while (size--) {
	    l = aopGet(AOP(result),offset++,FALSE);
	    emitcode("rl","%s", l);         
	}
	if (shCount>1) {
	    emitcode("", LABEL_STR ":",tlbl1->key+100);
	    emitcode("dec", "a");
	    emitcode(_shortJP,"nz," LABEL_STR ,tlbl->key+100);
	}
    }
}

/*-----------------------------------------------------------------*/
/* AccRol - rotate left accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void AccRol (int shCount)
{
    shCount &= 0x0007;              // shCount : 0..7
    switch(shCount){
        case 0 :
            break;
        case 1 :
            emitcode("rl","a");
            break;
        case 2 :
            emitcode("rl","a");
            emitcode("rl","a");
            break;
        case 3 :
            emitcode("rl","a");
            emitcode("rl","a");
            emitcode("rl","a");
            break;
        case 4 :
            emitcode("rl","a");
            emitcode("rl","a");
            emitcode("rl","a");
            emitcode("rl","a");
            break;
        case 5 :
            emitcode("rr","a");
            emitcode("rr","a");
            emitcode("rr","a");
            break;
        case 6 :
            emitcode("rr","a");
            emitcode("rr","a");
            break;
        case 7 :
            emitcode("rr","a");
            break;
    }
}

/*-----------------------------------------------------------------*/
/* AccLsh - left shift accumulator by known count                  */
/*-----------------------------------------------------------------*/
static void AccLsh (int shCount)
{
    if(shCount != 0){
        if(shCount == 1)
            emitcode("add","a,a");
        else 
	    if(shCount == 2) {
            emitcode("add","a,a");
            emitcode("add","a,a");
        } else {
            /* rotate left accumulator */
            AccRol(shCount);
            /* and kill the lower order bits */
            emitcode("and","a,#0x%02x", SLMask[shCount]);
        }
    }
}

/*-----------------------------------------------------------------*/
/* shiftL1Left2Result - shift left one byte from left to result    */
/*-----------------------------------------------------------------*/
static void shiftL1Left2Result (operand *left, int offl,
                                operand *result, int offr, int shCount)
{
    char *l;
    l = aopGet(AOP(left),offl,FALSE);
    MOVA(l);
    /* shift left accumulator */
    AccLsh(shCount);
    aopPut(AOP(result),"a",offr);
}


/*-----------------------------------------------------------------*/
/* genlshTwo - left shift two bytes by known amount != 0           */
/*-----------------------------------------------------------------*/
static void genlshTwo (operand *result,operand *left, int shCount)
{
    int size = AOP_SIZE(result);

    assert(size==2);

    /* if shCount >= 8 */
    if (shCount >= 8) {
        shCount -= 8 ;

        if (size > 1){
            if (shCount) {
                movLeft2Result(left, LSB, result, MSB16, 0);
		aopPut(AOP(result),zero, 0);   
		shiftL1Left2Result(left, MSB16, result, MSB16, shCount-8);
	    }
            else {
                movLeft2Result(left, LSB, result, MSB16, 0);
		aopPut(AOP(result),zero, 0);   
	    }
        }
        aopPut(AOP(result),zero,LSB);   
    }
    /*  1 <= shCount <= 7 */
    else {  
        if(size == 1) {
	    assert(0);
	}
        else {
            shiftL2Left2Result(left, LSB, result, LSB, shCount);
	}
    }
}

/*-----------------------------------------------------------------*/
/* genlshOne - left shift a one byte quantity by known count       */
/*-----------------------------------------------------------------*/
static void genlshOne (operand *result, operand *left, int shCount)
{       
    shiftL1Left2Result(left, LSB, result, LSB, shCount);
}

/*-----------------------------------------------------------------*/
/* genLeftShiftLiteral - left shifting by known count              */
/*-----------------------------------------------------------------*/
static void genLeftShiftLiteral (operand *left,
                                 operand *right,
                                 operand *result,
                                 iCode *ic)
{    
    int shCount = (int) floatFromVal (AOP(right)->aopu.aop_lit);
    int size;

    freeAsmop(right,NULL,ic);

    aopOp(left,ic,FALSE);
    aopOp(result,ic,FALSE);

    size = getSize(operandType(result));

#if VIEW_SIZE
    emitcode("; shift left ","result %d, left %d",size,
             AOP_SIZE(left));
#endif

    /* I suppose that the left size >= result size */
    if (shCount == 0) {
	assert(0);
    }

    else if(shCount >= (size * 8))
        while(size--)
            aopPut(AOP(result),zero,size);
    else{
        switch (size) {
	case 1:
	    genlshOne (result,left,shCount);
	    break;
	case 2:
	    genlshTwo (result,left,shCount);
	    break;
	case 4:
	    assert(0);
	    break;
	default:
	    assert(0);
        }
    }
    freeAsmop(left,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genLeftShift - generates code for left shifting                 */
/*-----------------------------------------------------------------*/
static void genLeftShift (iCode *ic)
{
    int size, offset;
    char *l;
    symbol *tlbl , *tlbl1;
    operand *left,*right, *result;

    right = IC_RIGHT(ic);
    left  = IC_LEFT(ic);
    result = IC_RESULT(ic);

    aopOp(right,ic,FALSE);

    /* if the shift count is known then do it 
    as efficiently as possible */
    if (AOP_TYPE(right) == AOP_LIT) {
        genLeftShiftLiteral (left,right,result,ic);
        return ;
    }

    /* shift count is unknown then we have to form a loop get the loop
       count in B : Note: we take only the lower order byte since
       shifting more that 32 bits make no sense anyway, ( the largest
       size of an object can be only 32 bits ) */
    emitcode("ld","a,%s",aopGet(AOP(right),0,FALSE));
    emitcode("inc","a");
    freeAsmop (right,NULL,ic);
    aopOp(left,ic,FALSE);
    aopOp(result,ic,FALSE);

    /* now move the left to the result if they are not the
       same */
#if 1
    if (!sameRegs(AOP(left),AOP(result))) {

        size = AOP_SIZE(result);
        offset = 0;
        while (size--) {
            l = aopGet(AOP(left),offset,FALSE);
	    aopPut(AOP(result),l,offset);
            offset++;
        }
    }
#else
    size = AOP_SIZE(result);
    offset = 0;
    while (size--) {
	l = aopGet(AOP(left),offset,FALSE);
	aopPut(AOP(result),l,offset);
	offset++;
    }
#endif


    tlbl = newiTempLabel(NULL);
    size = AOP_SIZE(result);
    offset = 0 ;   
    tlbl1 = newiTempLabel(NULL);

    emitcode(_shortJP, LABEL_STR ,tlbl1->key+100); 
    emitcode("", LABEL_STR ":",tlbl->key+100);    
    l = aopGet(AOP(result),offset,FALSE);
    emitcode("or", "a,a");
    while (size--) {
        l = aopGet(AOP(result),offset++,FALSE);
        emitcode("rl","%s", l);         
    }
    emitcode("", LABEL_STR ":",tlbl1->key+100);
    emitcode("dec", "a");
    emitcode(_shortJP,"nz," LABEL_STR ,tlbl->key+100);

    freeAsmop(left,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/* genlshTwo - left shift two bytes by known amount != 0           */
/*-----------------------------------------------------------------*/
static void genrshOne (operand *result,operand *left, int shCount)
{
    /* Errk */
    int size = AOP_SIZE(result);
    char *l;

    assert(size==1);
    assert(shCount<8);

    l = aopGet(AOP(left),0,FALSE);
    if (AOP(result)->type == AOP_REG) {
	aopPut(AOP(result), l, 0);
	l = aopGet(AOP(result), 0, FALSE);
	while (shCount--) 
	    emitcode("srl", "%s", l);
    }
    else {
	MOVA(l);
	while (shCount--) {
	    emitcode("srl", "a");
	}
	aopPut(AOP(result),"a",0);
    }
}

/*-----------------------------------------------------------------*/
/* AccRsh - right shift accumulator by known count                 */
/*-----------------------------------------------------------------*/
static void AccRsh (int shCount)
{
    if(shCount != 0){
        if(shCount == 1){
            CLRC;
            emitcode("rr","a");
        } else {
            /* rotate right accumulator */
            AccRol(8 - shCount);
            /* and kill the higher order bits */
            emitcode("and","a,#0x%02x", SRMask[shCount]);
        }
    }
}

/*-----------------------------------------------------------------*/
/* shiftR1Left2Result - shift right one byte from left to result   */
/*-----------------------------------------------------------------*/
static void shiftR1Left2Result (operand *left, int offl,
                                operand *result, int offr,
                                int shCount, int sign)
{
    MOVA(aopGet(AOP(left),offl,FALSE));
    if (sign) {
	assert(0);
    }
    else {
        AccRsh(shCount);
    }
    aopPut(AOP(result),"a",offr);
}

/*-----------------------------------------------------------------*/
/* genrshTwo - right shift two bytes by known amount != 0          */
/*-----------------------------------------------------------------*/
static void genrshTwo (operand *result,operand *left,
                       int shCount, int sign)
{
    /* if shCount >= 8 */
    if (shCount >= 8) {
        shCount -= 8 ;
        if (shCount) {
	    assert(0);
            shiftR1Left2Result(left, MSB16, result, LSB,
                               shCount, sign);
	}
        else {
            movLeft2Result(left, MSB16, result, LSB, sign);
	    aopPut(AOP(result),zero,1);
	}
    }
    /*  1 <= shCount <= 7 */
    else {
        shiftR2Left2Result(left, LSB, result, LSB, shCount, sign); 
    }
}

/*-----------------------------------------------------------------*/
/* genRightShiftLiteral - left shifting by known count              */
/*-----------------------------------------------------------------*/
static void genRightShiftLiteral (operand *left,
                                 operand *right,
                                 operand *result,
                                 iCode *ic)
{    
    int shCount = (int) floatFromVal (AOP(right)->aopu.aop_lit);
    int size;

    freeAsmop(right,NULL,ic);

    aopOp(left,ic,FALSE);
    aopOp(result,ic,FALSE);

    size = getSize(operandType(result));

    emitcode("; shift right ","result %d, left %d",size,
             AOP_SIZE(left));

    /* I suppose that the left size >= result size */
    if (shCount == 0) {
	assert(0);
    }

    else if(shCount >= (size * 8))
        while(size--)
            aopPut(AOP(result),zero,size);
    else{
        switch (size) {
	case 1:
	    genrshOne(result, left, shCount);
	    break;
	case 2:
	    /* PENDING: sign support */
	    genrshTwo(result, left, shCount, FALSE);
	    break;
	case 4:
	    assert(0);
	    break;
	default:
	    assert(0);
        }
    }
    freeAsmop(left,NULL,ic);
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genRightShift - generate code for right shifting                */
/*-----------------------------------------------------------------*/
static void genRightShift (iCode *ic)
{
    operand *left,*right, *result;

    right = IC_RIGHT(ic);
    left  = IC_LEFT(ic);
    result = IC_RESULT(ic);

    aopOp(right,ic,FALSE);

    /* if the shift count is known then do it 
    as efficiently as possible */
    if (AOP_TYPE(right) == AOP_LIT) {
        genRightShiftLiteral (left,right,result,ic);
        return ;
    }
    else {
	assert(0);
    }
}

/*-----------------------------------------------------------------*/
/* genGenPointerGet - gget value from generic pointer space        */
/*-----------------------------------------------------------------*/
static void genGenPointerGet (operand *left,
                              operand *result, iCode *ic)
{
    int size, offset ;
    link *retype = getSpec(operandType(result));
    const char *ptr = "hl";

    if (IS_GB)
	ptr = "de";

    aopOp(left,ic,FALSE);
    aopOp(result,ic,FALSE);

    if (isPair(AOP(left)) && (AOP_SIZE(result)==1)) {
	/* Just do it */
	emitcode("ld", "a,(%s)", getPairName(AOP(left)));
	aopPut(AOP(result),"a", 0);
	freeAsmop(left,NULL,ic);
	goto release;
    }

    /* For now we always load into IY */
    /* if this is remateriazable */
    if (AOP_TYPE(left) == AOP_IMMD)
	emitcode("ld","%s,%s", ptr, aopGet(AOP(left),0,TRUE));
    else { /* we need to get it byte by byte */
	if (IS_GB) {
	    emitcode("ld", "e,%s", aopGet(AOP(left), 0, FALSE));
	    emitcode("ld", "d,%s", aopGet(AOP(left), 1, FALSE));
	}
	else
	    fetchHL(AOP(left));
    }
    /* so iy now contains the address */
    freeAsmop(left,NULL,ic);

    /* if bit then unpack */
    if (IS_BITVAR(retype)) {
	assert(0);
    }
    else {
        size = AOP_SIZE(result);
        offset = 0 ;

        while (size--) {
	    /* PENDING: make this better */
	    if (!IS_GB && AOP(result)->type == AOP_REG) {
		aopPut(AOP(result),"(hl)",offset++);
	    }
	    else {
		emitcode("ld", "a,(%s)", ptr, offset);
		aopPut(AOP(result),"a",offset++);
	    }
	    if (size) {
		emitcode("inc", "%s", ptr);
	    }
        }
    }

 release:
    freeAsmop(result,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genPointerGet - generate code for pointer get                   */
/*-----------------------------------------------------------------*/
static void genPointerGet (iCode *ic)
{
    operand *left, *result ;
    link *type, *etype;

    left = IC_LEFT(ic);
    result = IC_RESULT(ic) ;

    /* depending on the type of pointer we need to
    move it to the correct pointer register */
    type = operandType(left);
    etype = getSpec(type);

    genGenPointerGet (left,result,ic);
}

bool isRegOrLit(asmop *aop)
{
    if (aop->type == AOP_REG || aop->type == AOP_LIT || aop->type == AOP_IMMD)
	return TRUE;
    return FALSE;
}

/*-----------------------------------------------------------------*/
/* genGenPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void genGenPointerSet (operand *right,
                              operand *result, iCode *ic)
{    
    int size, offset ;
    link *retype = getSpec(operandType(right));
    const char *ptr = "hl";

    aopOp(result,ic,FALSE);
    aopOp(right,ic,FALSE);

    if (IS_GB)
	ptr = "de";

    /* Handle the exceptions first */
    if (isPair(AOP(result)) && (AOP_SIZE(right)==1)) {
	/* Just do it */
	char *l = aopGet(AOP(right), 0, FALSE);
	MOVA(l);
	emitcode("ld", "(%s),a", getPairName(AOP(result)));
	freeAsmop(result,NULL,ic);
	goto release;
    }
	
    /* if the operand is already in dptr 
       then we do nothing else we move the value to dptr */
    if (AOP_TYPE(result) != AOP_STR) {
        /* if this is remateriazable */
        if (AOP_TYPE(result) == AOP_IMMD) {
	    emitcode("", "; Error 2");
	    emitcode("ld", "%s,%s", ptr, aopGet(AOP(result), 0, TRUE));
        }
        else { /* we need to get it byte by byte */
	    if (IS_GB) {
		emitcode("ld", "e,%s", aopGet(AOP(result), 0, TRUE));
		emitcode("ld", "d,%s", aopGet(AOP(result), 1, TRUE));
	    }
	    else {
		/* PENDING: do this better */
		fetchHL(AOP(result));
	    }
        }
    }
    /* so hl know contains the address */
    freeAsmop(result,NULL,ic);

    /* if bit then unpack */
    if (IS_BITVAR(retype)) {
	assert(0);
    }
    else {
        size = AOP_SIZE(right);
        offset = 0 ;

        while (size--) {
            char *l = aopGet(AOP(right),offset,FALSE);
	    if (isRegOrLit(AOP(right)) && !IS_GB) {
		emitcode("ld", "(%s),%s", ptr, l);
	    }
	    else {
		MOVA(l);
		emitcode("ld", "(%s),a", ptr, offset);
	    }
	    if (size) {
		emitcode("inc", ptr);
	    }
	    offset++;
        }
    }
    release:
    freeAsmop(right,NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genPointerSet - stores the value into a pointer location        */
/*-----------------------------------------------------------------*/
static void genPointerSet (iCode *ic)
{    
    operand *right, *result ;
    link *type, *etype;

    right = IC_RIGHT(ic);
    result = IC_RESULT(ic) ;

    /* depending on the type of pointer we need to
    move it to the correct pointer register */
    type = operandType(result);
    etype = getSpec(type);
    
    genGenPointerSet (right,result,ic);
}

/*-----------------------------------------------------------------*/
/* genIfx - generate code for Ifx statement                        */
/*-----------------------------------------------------------------*/
static void genIfx (iCode *ic, iCode *popIc)
{
    operand *cond = IC_COND(ic);
    int isbit =0;

    aopOp(cond,ic,FALSE);

    /* get the value into acc */
    if (AOP_TYPE(cond) != AOP_CRY)
        toBoolean(cond);
    else
        isbit = 1;
    /* the result is now in the accumulator */
    freeAsmop(cond,NULL,ic);

    /* if there was something to be popped then do it */
    if (popIc)
        genIpop(popIc);

    /* if the condition is  a bit variable */
    if (isbit && IS_ITEMP(cond) && 
	SPIL_LOC(cond))
	genIfxJump(ic,SPIL_LOC(cond)->rname);
    else
	if (isbit && !IS_ITEMP(cond))
	    genIfxJump(ic,OP_SYMBOL(cond)->rname);
	else
	    genIfxJump(ic,"a");

    ic->generated = 1;
}

/*-----------------------------------------------------------------*/
/* genAddrOf - generates code for address of                       */
/*-----------------------------------------------------------------*/
static void genAddrOf (iCode *ic)
{
    symbol *sym = OP_SYMBOL(IC_LEFT(ic));

    aopOp(IC_RESULT(ic),ic,FALSE);

    /* if the operand is on the stack then we 
    need to get the stack offset of this
    variable */
    if (sym->onStack) {
        /* if it has an offset  then we need to compute it */
	if (IS_GB) {
	    emitcode("lda", "hl,%d+%d+%d(sp)", sym->stack, _pushed, _spoffset);
	    emitcode("ld", "d,h");
	    emitcode("ld", "e,l");
	    aopPut(AOP(IC_RESULT(ic)), "e", 0);
	    aopPut(AOP(IC_RESULT(ic)), "d", 1);
	    goto end;
	}
	else {
	    emitcode("push", "de");
	    emitcode("push", "ix");
	    emitcode("pop", "hl");
	    emitcode("ld", "de,#%d", sym->stack);
	    emitcode("add", "hl,de");
	    emitcode("pop", "de");
	}
    }
    else {
	emitcode("ld", "hl,#%s", sym->rname);
    }
    aopPut(AOP(IC_RESULT(ic)), "l", 0);
    aopPut(AOP(IC_RESULT(ic)), "h", 1);
end:
    freeAsmop(IC_RESULT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genAssign - generate code for assignment                        */
/*-----------------------------------------------------------------*/
static void genAssign (iCode *ic)
{
    operand *result, *right;
    int size, offset ;
    unsigned long lit = 0L;

    result = IC_RESULT(ic);
    right  = IC_RIGHT(ic) ;

#if 1
    /* Dont bother assigning if they are the same */
    if (operandsEqu (IC_RESULT(ic),IC_RIGHT(ic))) {
	emitcode("", "; (operands are equal %u)", operandsEqu(IC_RESULT(ic),IC_RIGHT(ic)));
        return;
    }
#endif

    aopOp(right,ic,FALSE);
    aopOp(result,ic,TRUE);

    /* if they are the same registers */
    if (sameRegs(AOP(right),AOP(result))) {
	emitcode("", "; (registers are the same)");
        goto release;
    }

    /* if the result is a bit */
    if (AOP_TYPE(result) == AOP_CRY) {
	assert(0);
    }

    /* general case */
    size = AOP_SIZE(result);
    offset = 0;

    if(AOP_TYPE(right) == AOP_LIT)
	lit = (unsigned long)floatFromVal(AOP(right)->aopu.aop_lit);
    if((size > 1) &&
       (AOP_TYPE(result) != AOP_REG) &&
       (AOP_TYPE(right) == AOP_LIT) &&
       !IS_FLOAT(operandType(right)) &&
       (lit < 256L)) {
	bool fXored = FALSE;
	offset = 0;
	/* Work from the top down.
	   Done this way so that we can use the cached copy of 0
	   in A for a fast clear */
	while (size--) {
	    if((unsigned int)((lit >> (offset*8)) & 0x0FFL)== 0) {
		if (!fXored && size>1) {
		    emitcode("xor", "a,a");
		    fXored = TRUE;
		}
		if (fXored) {
		    aopPut(AOP(result),"a",offset);
		}
		else {
		    aopPut(AOP(result), "#0", offset);
		}
	    }
	    else
		aopPut(AOP(result),
		       aopGet(AOP(right),offset,FALSE),
		       offset);
	    offset++;
	}
    }
    else if (size == 2 && requiresHL(AOP(right)) && requiresHL(AOP(result))) {
	/* Special case.  Load into a and d, then load out. */
	MOVA(aopGet(AOP(right), 0, FALSE));
	emitcode("ld", "e,%s", aopGet(AOP(right), 1, FALSE));
	aopPut(AOP(result), "a", 0);
	aopPut(AOP(result), "e", 1);
    } else {
	while (size--) {
	    aopPut(AOP(result),
		   aopGet(AOP(right),offset,FALSE),
		   offset);
	    offset++;
	}
    }
    
release:
    freeAsmop(right,NULL,ic);
    freeAsmop(result,NULL,ic);
}   

/*-----------------------------------------------------------------*/
/* genJumpTab - genrates code for jump table                       */
/*-----------------------------------------------------------------*/
static void genJumpTab (iCode *ic)
{
    symbol *jtab;
    char *l;

    aopOp(IC_JTCOND(ic),ic,FALSE);
    /* get the condition into accumulator */
    l = aopGet(AOP(IC_JTCOND(ic)),0,FALSE);
    MOVA(l);
    if (!IS_GB)
	emitcode("push", "de");
    emitcode("ld", "e,%s", l);
    emitcode("ld", "d,#0");
    jtab = newiTempLabel(NULL);
    emitcode("ld", "hl,#" LABEL_STR, jtab->key+100);
    emitcode("add", "hl,de");
    emitcode("add", "hl,de");
    emitcode("add", "hl,de");
    freeAsmop(IC_JTCOND(ic),NULL,ic);
    if (!IS_GB)
	emitcode("pop", "de");
    emitcode("jp", "(hl)");
    emitcode("","%05d$:",jtab->key+100);
    /* now generate the jump labels */
    for (jtab = setFirstItem(IC_JTLABELS(ic)) ; jtab;
         jtab = setNextItem(IC_JTLABELS(ic)))
        emitcode("jp", LABEL_STR, jtab->key+100);
}

/*-----------------------------------------------------------------*/
/* genCast - gen code for casting                                  */
/*-----------------------------------------------------------------*/
static void genCast (iCode *ic)
{
    operand *result = IC_RESULT(ic);
    link *ctype = operandType(IC_LEFT(ic));
    operand *right = IC_RIGHT(ic);
    int size, offset ;

    /* if they are equivalent then do nothing */
    if (operandsEqu(IC_RESULT(ic),IC_RIGHT(ic)))
        return ;

    aopOp(right,ic,FALSE) ;
    aopOp(result,ic,FALSE);

    /* if the result is a bit */
    if (AOP_TYPE(result) == AOP_CRY) {
	assert(0);
    }

    /* if they are the same size : or less */
    if (AOP_SIZE(result) <= AOP_SIZE(right)) {

        /* if they are in the same place */
        if (sameRegs(AOP(right),AOP(result)))
            goto release;

        /* if they in different places then copy */
        size = AOP_SIZE(result);
        offset = 0 ;
        while (size--) {
            aopPut(AOP(result),
                   aopGet(AOP(right),offset,FALSE),
                   offset);
            offset++;
        }
        goto release;
    }

    /* PENDING: should be OK. */
#if 0
    /* if the result is of type pointer */
    if (IS_PTR(ctype)) {
	assert(0);
    }
#endif
    
    /* so we now know that the size of destination is greater
    than the size of the source */
    /* we move to result for the size of source */
    size = AOP_SIZE(right);
    offset = 0 ;
    while (size--) {
        aopPut(AOP(result),
               aopGet(AOP(right),offset,FALSE),
               offset);
        offset++;
    }

    /* now depending on the sign of the destination */
    size = AOP_SIZE(result) - AOP_SIZE(right);
    /* Unsigned or not an integral type - right fill with zeros */
    if (SPEC_USIGN(ctype) || !IS_SPEC(ctype)) {
        while (size--)
            aopPut(AOP(result),zero,offset++);
    } else {
        /* we need to extend the sign :{ */
        char *l = aopGet(AOP(right),AOP_SIZE(right) - 1,
                         FALSE);
        MOVA(l);
	emitcode("", "; genCast: sign extend untested.");
        emitcode("rla", "");
        emitcode("sbc", "a,a");
        while (size--)
            aopPut(AOP(result),"a",offset++);   
    }

release:
    freeAsmop(right, NULL, ic);
    freeAsmop(result, NULL, ic);
}

/*-----------------------------------------------------------------*/
/* genReceive - generate code for a receive iCode                  */
/*-----------------------------------------------------------------*/
static void genReceive (iCode *ic)
{    
    if (isOperandInFarSpace(IC_RESULT(ic)) && 
	( OP_SYMBOL(IC_RESULT(ic))->isspilt ||
	  IS_TRUE_SYMOP(IC_RESULT(ic))) ) {
	assert(0);
    } else {
	accInUse++;
	aopOp(IC_RESULT(ic),ic,FALSE);  
	accInUse--;
	assignResultValue(IC_RESULT(ic));	
    }

    freeAsmop(IC_RESULT(ic),NULL,ic);
}

/*-----------------------------------------------------------------*/
/* genZ80Code - generate code for Z80 based controllers            */
/*-----------------------------------------------------------------*/
void genZ80Code (iCode *lic)
{
    iCode *ic;
    int cln = 0;

    /* HACK */
    if (IS_GB) {
	_fReturn = _gbz80_return;
	_fTmp = _gbz80_return;
	_shortJP = "jr";
    }
    else {
	_fReturn = _z80_return;
	_fTmp = _z80_return;
	_shortJP = "jp";
    }

    lineHead = lineCurr = NULL;

    /* if debug information required */
    if (options.debug && currFunc) { 
	cdbSymbol(currFunc,cdbFile,FALSE,TRUE);
	debugLine = 1;
	if (IS_STATIC(currFunc->etype))
	    emitcode("","F%s$%s$0$0 ==.",moduleName,currFunc->name); 
	else
	    emitcode("","G$%s$0$0 ==.",currFunc->name);
	debugLine = 0;
    }
    /* stack pointer name */
    spname = "sp";
    
 
    for (ic = lic ; ic ; ic = ic->next ) {
	
	if ( cln != ic->lineno ) {
	    if ( options.debug ) {
		debugLine = 1;
		emitcode("","C$%s$%d$%d$%d ==.",
			 ic->filename,ic->lineno,
			 ic->level,ic->block);
		debugLine = 0;
	    }
	    emitcode(";","%s %d",ic->filename,ic->lineno);
	    cln = ic->lineno ;
	}
	/* if the result is marked as
	   spilt and rematerializable or code for
	   this has already been generated then
	   do nothing */
	if (resultRemat(ic) || ic->generated ) 
	    continue ;
	
	/* depending on the operation */
	switch (ic->op) {
	case '!' :
	    emitcode("", "; genNot");
	    genNot(ic);
	    break;
	    
	case '~' :
	    emitcode("", "; genCpl");
	    genCpl(ic);
	    break;
	    
	case UNARYMINUS:
	    emitcode("", "; genUminus");
	    genUminus (ic);
	    break;
	    
	case IPUSH:
	    emitcode("", "; genIpush");
	    genIpush (ic);
	    break;
	    
	case IPOP:
	    /* IPOP happens only when trying to restore a 
	       spilt live range, if there is an ifx statement
	       following this pop then the if statement might
	       be using some of the registers being popped which
	       would destory the contents of the register so
	       we need to check for this condition and handle it */
	    if (ic->next            && 
		ic->next->op == IFX &&
		regsInCommon(IC_LEFT(ic),IC_COND(ic->next))) {
		emitcode("", "; genIfx");
		genIfx (ic->next,ic);
	    }
	    else {
		emitcode("", "; genIpop");
		genIpop (ic);
	    }
	    break; 
	    
	case CALL:
	    emitcode("", "; genCall");
	    genCall (ic);
	    break;
	    
	case PCALL:
	    emitcode("", "; genPcall");
	    genPcall (ic);
	    break;
	    
	case FUNCTION:
	    emitcode("", "; genFunction");
	    genFunction (ic);
	    break;
	    
	case ENDFUNCTION:
	    emitcode("", "; genEndFunction");
	    genEndFunction (ic);
	    break;
	    
	case RETURN:
	    emitcode("", "; genRet");
	    genRet (ic);
	    break;
	    
	case LABEL:
	    emitcode("", "; genLabel");
	    genLabel (ic);
	    break;
	    
	case GOTO:
	    emitcode("", "; genGoto");
	    genGoto (ic);
	    break;
	    
	case '+' :
	    emitcode("", "; genPlus");
	    genPlus (ic) ;
	    break;
	    
	case '-' :
	    emitcode("", "; genMinus");
	    genMinus (ic);
	    break;
	    
	case '*' :
	    emitcode("", "; genMult");
	    genMult (ic);
	    break;
	    
	case '/' :
	    emitcode("", "; genDiv");
	    genDiv (ic) ;
	    break;
	    
	case '%' :
	    emitcode("", "; genMod");
	    genMod (ic);
	    break;
	    
	case '>' :
	    emitcode("", "; genCmpGt");
	    genCmpGt (ic,ifxForOp(IC_RESULT(ic),ic));		      
	    break;
	    
	case '<' :
	    emitcode("", "; genCmpLt");
	    genCmpLt (ic,ifxForOp(IC_RESULT(ic),ic));
	    break;
	    
	case LE_OP:
	case GE_OP:
	case NE_OP:
	    
	    /* note these two are xlated by algebraic equivalence
	       during parsing SDCC.y */
	    werror(E_INTERNAL_ERROR,__FILE__,__LINE__,
		   "got '>=' or '<=' shouldn't have come here");
	    break;	
	    
	case EQ_OP:
	    emitcode("", "; genCmpEq");
	    genCmpEq (ic,ifxForOp(IC_RESULT(ic),ic));
	    break;	    
	    
	case AND_OP:
	    emitcode("", "; genAndOp");
	    genAndOp (ic);
	    break;
	    
	case OR_OP:
	    emitcode("", "; genOrOp");
	    genOrOp (ic);
	    break;
	    
	case '^' :
	    emitcode("", "; genXor");
	    genXor (ic,ifxForOp(IC_RESULT(ic),ic));
	    break;
	    
	case '|' :
	    emitcode("", "; genOr");
	    genOr (ic,ifxForOp(IC_RESULT(ic),ic));
	    break;
	    
	case BITWISEAND:
	    emitcode("", "; genAnd");
            genAnd (ic,ifxForOp(IC_RESULT(ic),ic));
	    break;
	    
	case INLINEASM:
	    emitcode("", "; genInline");
	    genInline (ic);
	    break;
	    
	case RRC:
	    emitcode("", "; genRRC");
	    genRRC (ic);
	    break;
	    
	case RLC:
	    emitcode("", "; genRLC");
	    genRLC (ic);
	    break;
	    
	case GETHBIT:
	    emitcode("", "; genHBIT");
	    assert(0);
	    
	case LEFT_OP:
	    emitcode("", "; genLeftShift");
	    genLeftShift (ic);
	    break;
	    
	case RIGHT_OP:
	    emitcode("", "; genRightShift");
	    genRightShift (ic);
	    break;
	    
	case GET_VALUE_AT_ADDRESS:
	    emitcode("", "; genPointerGet");
	    genPointerGet(ic);
	    break;
	    
	case '=' :

	    if (POINTER_SET(ic)) {
		emitcode("", "; genAssign (pointer)");
		genPointerSet(ic);
	    }
	    else {
		emitcode("", "; genAssign");
		genAssign(ic);
	    }
	    break;
	    
	case IFX:
	    emitcode("", "; genIfx");
	    genIfx (ic,NULL);
	    break;
	    
	case ADDRESS_OF:
	    emitcode("", "; genAddrOf");
	    genAddrOf (ic);
	    break;
	    
	case JUMPTABLE:
	    emitcode("", "; genJumpTab");
	    genJumpTab (ic);
	    break;
	    
	case CAST:
	    emitcode("", "; genCast");
	    genCast (ic);
	    break;
	    
	case RECEIVE:
	    emitcode("", "; genReceive");
	    genReceive(ic);
	    break;
	    
	case SEND:
	    emitcode("", "; addSet");
	    addSet(&sendSet,ic);
	    break;

	default :
	    ic = ic;
	    /*      piCode(ic,stdout); */
	    
        }
    }
    

    /* now we are ready to call the 
       peep hole optimizer */
    if (!options.nopeep)
	peepHole (&lineHead);

    /* now do the actual printing */
    printLine (lineHead,codeOutFile);
    return;
}
