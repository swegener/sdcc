/*-------------------------------------------------------------------------

  main.c - pic16 specific general functions.

   Written by - Scott Dattalo scott@dattalo.com
   Ported to PIC16 by - Martin Dubuc m.debuc@rogers.com
    
   Note that mlh prepended _pic16_ on the static functions.  Makes
   it easier to set a breakpoint using the debugger.


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

#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "device.h"
#include "SDCCutil.h"
#include "glue.h"
#include "pcode.h"
//#include "gen.h"


static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by pic16 */
static char *_pic16_keywords[] =
{
  "at",
  "bit",
  "code",
  "critical",
  "register",
  "data",
  "far",
  "idata",
  "interrupt",
  "near",
  "pdata",
  "reentrant",
  "sfr",
  "sbit",
  "using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  "_pdata",
  "_idata",
  "_naked",
  NULL
};


pic16_sectioninfo_t pic16_sectioninfo;


extern char *pic16_processor_base_name(void);

void  pic16_pCodeInitRegisters(void);

void pic16_assignRegisters (eBBlock ** ebbs, int count);

static int regParmFlg = 0;	/* determine if we can register a parameter */

pic16_options_t pic16_options;

extern set *includeDirsSet;
extern set *dataDirsSet;
extern set *libFilesSet;

/* Also defined in gen.h, but the #include is commented out */
/* for an unknowned reason. - EEP */
void pic16_emitDebuggerSymbol (char *);
 
extern regs* newReg(short type, short pc_type, int rIdx, char *name, int size, int alias, operand *refop);
extern void pic16_emitConfigRegs(FILE *of);
extern void pic16_emitIDRegs(FILE *of);



static void
_pic16_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
  pic16_pCodeInitRegisters();
  maxInterrupts = 2;
}

static void
_pic16_reset_regparm (void)
{
  regParmFlg = 0;
}

static int
_pic16_regparm (sym_link * l)
{
  /* force all parameters via SEND/RECEIVE */
  if(0 /*pic16_options.ip_stack*/) {
    /* for this processor it is simple
     * can pass only the first parameter in a register */
    if(regParmFlg)return 0;
      regParmFlg++;
      return 1;	//regParmFlg;
  } else {
    /* otherwise pass all arguments in registers via SEND/RECEIVE */
    regParmFlg++;// = 1;
    return regParmFlg;
  }
}


int initsfpnt=0;		/* set to 1 if source provides a pragma for stack
				 * so glue() later emits code to initialize stack/frame pointers */
set *absSymSet;

set *sectNames=NULL;			/* list of section listed in pragma directives */
set *sectSyms=NULL;			/* list of symbols set in a specific section */
set *wparamList=NULL;

static int
_process_pragma(const char *sz)
{
  static const char *WHITE = " \t\n";
  static const char *WHITECOMMA = " \t\n,";
  
  char	*ptr = strtok((char *)sz, WHITE);

  	/* #pragma maxram [maxram] */
	if (startsWith (ptr, "maxram")) {
	  char *maxRAM = strtok((char *)NULL, WHITE);

		if (maxRAM != (char *)NULL) {
		  int maxRAMaddress;
		  value *maxRAMVal;

			maxRAMVal = constVal(maxRAM);
			maxRAMaddress = (int)floatFromVal(maxRAMVal);
			pic16_setMaxRAM(maxRAMaddress);
		}

          return 0;
	}
	
	/* #pragma stack [stack-position] [stack-len] */
	if(startsWith(ptr, "stack")) {
	  char *stackPosS = strtok((char *)NULL, WHITE);
	  char *stackLenS = strtok((char *)NULL, WHITE);
	  value *stackPosVal;
	  value *stackLenVal;
	  regs *reg;
	  symbol *sym;

		stackPosVal = constVal( stackPosS );
		stackPos = (unsigned int)floatFromVal( stackPosVal );

		
		if(stackLenS) {
			stackLenVal = constVal( stackLenS );
			stackLen = (unsigned int)floatFromVal( stackLenVal );
		}

		if(stackLen < 1) {
			stackLen = 64;
			fprintf(stderr, "%s:%d: warning: setting stack to default size %d (0x%04x)\n",
		                      filename, lineno-1, stackLen, stackLen);
                        
//			fprintf(stderr, "%s:%d setting stack to default size %d\n", __FILE__, __LINE__, stackLen);
		}

//	  	fprintf(stderr, "Initializing stack pointer at 0x%x len 0x%x\n", stackPos, stackLen);
				
		reg=newReg(REG_SFR, PO_SFR_REGISTER, stackPos, "_stack", stackLen-1, 0, NULL);
		addSet(&pic16_fix_udata, reg);
		
		reg = newReg(REG_SFR, PO_SFR_REGISTER, stackPos + stackLen-1, "_stack_end", 1, 0, NULL);
		addSet(&pic16_fix_udata, reg);

		sym = newSymbol("stack", 0);
		sprintf(sym->rname, "_%s", sym->name);
		addSet(&publics, sym);

		sym = newSymbol("stack_end", 0);
		sprintf(sym->rname, "_%s", sym->name);
		addSet(&publics, sym);
		
		initsfpnt = 1;		// force glue() to initialize stack/frame pointers */

	  return 0;
	}
	
	/* #pragma code [symbol] [location] */
	if(startsWith(ptr, "code")) {
	  char *symname = strtok((char *)NULL, WHITE);
	  char *location = strtok((char *)NULL, WHITE);
	  absSym *absS;
	  value *addr;

		absS = Safe_calloc(1, sizeof(absSym));
		sprintf(absS->name, "_%s", symname);
		
		addr = constVal( location );
		absS->address = (unsigned int)floatFromVal( addr );

		if((absS->address % 2) != 0) {
		  absS->address--;
		  fprintf(stderr, "%s:%d: warning: code memory locations should be word aligned, will locate to 0x%06x instead\n",
		                      filename, lineno-1, absS->address);
                }

		addSet(&absSymSet, absS);
//		fprintf(stderr, "%s:%d symbol %s will be placed in location 0x%06x in code memory\n",
//			__FILE__, __LINE__, symname, absS->address);

	  return 0;
	}

	/* #pragma udata [section-name] [symbol] */
	if(startsWith(ptr, "udata")) {
	  char *sectname = strtok((char *)NULL, WHITE);
	  char *symname = strtok((char *)NULL, WHITE);
	  symbol *nsym;
	  sectSym *ssym;
	  sectName *snam;
	  int found=0;
	  
	  	while(symname) {
	  		ssym = Safe_calloc(1, sizeof(sectSyms));
			ssym->name = Safe_calloc(1, strlen(symname)+2);
			sprintf(ssym->name, "_%s", symname);
	  		ssym->reg = NULL;

			addSet(&sectSyms, ssym);

                        nsym = newSymbol(symname, 0);
                        strcpy(nsym->rname, ssym->name);
			checkAddSym(&publics, nsym);

			found = 0;
			for(snam=setFirstItem(sectNames);snam;snam=setNextItem(sectNames)) {
				if(!strcmp(sectname, snam->name)){ found=1; break; }
			}
			
			if(!found) {
				snam = Safe_calloc(1, sizeof(sectNames));
				snam->name = Safe_strdup( sectname );
				snam->regsSet = NULL;
				
				addSet(&sectNames, snam);
			}
			
			ssym->section = snam;
				
//	  		fprintf(stderr, "%s:%d placing symbol %s at section %s (%p)\n", __FILE__, __LINE__,
//	  		 	ssym->name, snam->name, snam);

	  		symname = strtok((char *)NULL, WHITE);
		}

	  return 0;
	}
	
	if(startsWith(ptr, "wparam")) {
	  char *fname = strtok((char *)NULL, WHITECOMMA);
	  
            while(fname) {
              addSet(&wparamList, Safe_strdup(fname));
              
//              debugf("passing with WREG to %s\n", fname);
              fname = strtok((char *)NULL, WHITECOMMA);
            }
            
          return 0;
        }
	
  return 1;
}

#define REP_UDATA	"--preplace-udata-with="

#define STACK_MODEL	"--pstack-model="
#define OPT_BANKSEL	"--obanksel="

#define ALT_ASM		"--asm="
#define ALT_LINK	"--link="

#define IVT_LOC		"--ivt-loc"
#define NO_DEFLIBS	"--nodefaultlibs"
#define MPLAB_COMPAT	"--mplab-comp"

#define NL_OPT		"--nl="
#define USE_CRT		"--use-crt="

#define	OFMSG_LRSUPPORT	"--flr-support"

#define OPTIMIZE_GOTO   "--optimize-goto"

char *alt_asm=NULL;
char *alt_link=NULL;

int pic16_mplab_comp=0;
extern int pic16_debug_verbose;
extern int pic16_ralloc_debug;
extern int pic16_pcode_verbose;

int pic16_fstack=0;
int pic16_enable_peeps=0;
int pic16_nl=0;			/* 0 for LF, 1 for CRLF */

OPTION pic16_optionsTable[]= {
	{ 0,	NO_DEFLIBS,		&pic16_options.nodefaultlibs,	"do not link default libraries when linking"},
	{ 0,	"--pno-banksel",	&pic16_options.no_banksel,	"do not generate BANKSEL assembler directives"},
	{ 0,	OPT_BANKSEL,		NULL,				"set banksel optimization level (default=0 no)"},
	{ 0,	"--pomit-config-words",	&pic16_options.omit_configw,	"omit the generation of configuration words"},
	{ 0,	"--pomit-ivt",		&pic16_options.omit_ivt,	"omit the generation of the Interrupt Vector Table"},
	{ 0,	"--pleave-reset-vector",&pic16_options.leave_reset,	"when omitting IVT leave RESET vector"},
	{ 0,	STACK_MODEL,		NULL,				"use stack model 'small' (default) or 'large'"},

	{ 0,	"--debug-xtra",		&pic16_debug_verbose,	"show more debug info in assembly output"},
	{ 0,	"--debug-ralloc",	&pic16_ralloc_debug,	"dump register allocator debug file *.d"},
	{ 0,	"--pcode-verbose",	&pic16_pcode_verbose,	"dump pcode related info"},
		
	{ 0,	REP_UDATA,	NULL,	"Place udata variables at another section: udata_acs, udata_ovr, udata_shr"},

	{ 0,	ALT_ASM,	NULL,	"Use alternative assembler"},
	{ 0,	ALT_LINK,	NULL,	"Use alternative linker"},

	{ 0,	"--denable-peeps",	&pic16_enable_peeps,	"explicit enable of peepholes"},
	{ 0,	IVT_LOC,	NULL,	"<nnnn> interrupt vector table location"},
	{ 0,	"--calltree",		&pic16_options.dumpcalltree,	"dump call tree in .calltree file"},
	{ 0,	MPLAB_COMPAT,		&pic16_mplab_comp,	"enable compatibility mode for MPLAB utilities (MPASM/MPLINK)"},
	{ 0,	"--fstack",		&pic16_fstack,		"enable stack optimizations"},
	{ 0,	NL_OPT,		NULL,				"new line, \"lf\" or \"crlf\""},
	{ 0,	USE_CRT,	NULL,	"use <crt-o> run-time initialization module"},
	{ 0,	"--no-crt",	&pic16_options.no_crt,	"do not link any default run-time initialization module"},
	{ 0,	"--gstack",	&pic16_options.gstack,	"trace stack pointer push/pop to overflow"},
//	{ 0,	OFMSG_LRSUPPORT,	NULL,		"use support functions for local register store/restore"},
	{ 0,    OPTIMIZE_GOTO,  NULL,			"try to use (conditional) BRA instead of GOTO"},
	{ 0,	NULL,		NULL,	NULL}
	};


#define ISOPT(str)	!strncmp(argv[ *i ], str, strlen(str) )

extern char *getStringArg(const char *,  char **, int *, int);
extern int getIntArg(const char *, char **, int *, int);

static bool
_pic16_parseOptions (int *pargc, char **argv, int *i)
{
  int j=0;
  char *stkmodel;
  
  /* TODO: allow port-specific command line options to specify
   * segment names here.
   */
  
    /* check for arguments that have associated an integer variable */
    while(pic16_optionsTable[j].pparameter) {
      if(ISOPT( pic16_optionsTable[j].longOpt )) {
        (*pic16_optionsTable[j].pparameter)++;
        return TRUE;
      }
      j++;
    }

    if(ISOPT(STACK_MODEL)) {
      stkmodel = getStringArg(STACK_MODEL, argv, i, *pargc);
      if(!STRCASECMP(stkmodel, "small"))pic16_options.stack_model = 0;
      else if(!STRCASECMP(stkmodel, "large"))pic16_options.stack_model = 1;
      else {
        fprintf(stderr, "Unknown stack model: %s", stkmodel);
        exit(-1);
      }
      return TRUE;
    }

    if(ISOPT(OPT_BANKSEL)) {
      pic16_options.opt_banksel = getIntArg(OPT_BANKSEL, argv, i, *pargc);
      return TRUE;
    }

    if(ISOPT(REP_UDATA)) {
      pic16_sectioninfo.at_udata = Safe_strdup(getStringArg(REP_UDATA, argv, i, *pargc));
      return TRUE;
    }
	
    if(ISOPT(ALT_ASM)) {
      alt_asm = Safe_strdup(getStringArg(ALT_ASM, argv, i, *pargc));
      return TRUE;
    }
	
    if(ISOPT(ALT_LINK)) {
      alt_link = Safe_strdup(getStringArg(ALT_LINK, argv, i, *pargc));
      return TRUE;
    }

    if(ISOPT(IVT_LOC)) {
      pic16_options.ivt_loc = getIntArg(IVT_LOC, argv, i, *pargc);
      return TRUE;
    }
	
    if(ISOPT(NL_OPT)) {
      char *tmp;
            
        tmp = Safe_strdup( getStringArg(NL_OPT, argv, i, *pargc) );
        if(!STRCASECMP(tmp, "lf"))pic16_nl = 0;
        else if(!STRCASECMP(tmp, "crlf"))pic16_nl = 1;
        else {
          fprintf(stderr, "invalid termination character id\n");
          exit(-1);
        }
        return TRUE;
    }

    if(ISOPT(USE_CRT)) {
      pic16_options.no_crt = 0;
      pic16_options.crt_name = Safe_strdup( getStringArg(USE_CRT, argv, i, *pargc) );

      return TRUE;
    }

#if 0
    if(ISOPT(OFMSG_LRSUPPORT)) {
      pic16_options.opt_flags |= OF_LR_SUPPORT;
      return TRUE;
    }
#endif

#if 1
    if (ISOPT(OPTIMIZE_GOTO)) {
      pic16_options.opt_flags |= OF_OPTIMIZE_GOTO;
      return TRUE;
    }
#endif

  return FALSE;
}

extern set *userIncDirsSet;

static void _pic16_initPaths(void)
{
  char pic16incDir[512];
  char pic16libDir[512];
  set *pic16incDirsSet=NULL;
  set *pic16libDirsSet=NULL;
  char devlib[512];

    setMainValue("mcu", pic16->name[2] );
    addSet(&preArgvSet, Safe_strdup("-D{mcu}"));

    sprintf(pic16incDir, "%s%cpic16", INCLUDE_DIR_SUFFIX, DIR_SEPARATOR_CHAR);
    sprintf(pic16libDir, "%s%cpic16", LIB_DIR_SUFFIX, DIR_SEPARATOR_CHAR);


    if(!options.nostdinc) {
      /* setup pic16 include directory */
      pic16incDirsSet = appendStrSet(dataDirsSet, NULL, pic16incDir);
      includeDirsSet = pic16incDirsSet;
//      mergeSets(&includeDirsSet, pic16incDirsSet);
    }
    /* pic16 port should not search to the SDCC standard include directories,
     * so add here the deleted include dirs that user has issued in command line */
    mergeSets(&pic16incDirsSet, userIncDirsSet);

    if(!options.nostdlib) {
      /* setup pic16 library directory */
      pic16libDirsSet = appendStrSet(dataDirsSet, NULL, pic16libDir);
      libDirsSet = pic16libDirsSet;
//      mergeSets(&libDirsSet, pic16libDirsSet);
    }

    if(!pic16_options.nodefaultlibs) {
      /* now add the library for the device */
      sprintf(devlib, "%s.lib", pic16->name[2]);
      addSet(&libFilesSet, Safe_strdup(devlib));

      /* add the internal SDCC library */
      addSet(&libFilesSet, Safe_strdup( "libsdcc.lib" ));
    }
}

extern set *linkOptionsSet;
char *msprintf(hTab *pvals, const char *pformat, ...);
int my_system(const char *cmd);

/* custom function to link objects */
static void _pic16_linkEdit(void)
{
  hTab *linkValues=NULL;
  char lfrm[256];
  char *lcmd;
  char temp[128];
  set *tSet=NULL;
  int ret;
  
  	/*
  	 * link command format:
  	 * {linker} {incdirs} {lflags} -o {outfile} {spec_ofiles} {ofiles} {libs}
  	 *
  	 */
  	 
  	sprintf(lfrm, "{linker} {incdirs} {lflags} -o {outfile} {user_ofile} {spec_ofiles} {ofiles} {libs}");
  	   	 
  	shash_add(&linkValues, "linker", "gplink");

  	mergeSets(&tSet, libDirsSet);
  	mergeSets(&tSet, libPathsSet);
  	
  	shash_add(&linkValues, "incdirs", joinStrSet( appendStrSet(tSet, "-I\"", "\"")));
  	shash_add(&linkValues, "lflags", joinStrSet(linkOptionsSet));
  
	shash_add(&linkValues, "outfile", dstFileName);

  	if(fullSrcFileName) {
		sprintf(temp, "%s.o", dstFileName);
//		addSetHead(&relFilesSet, Safe_strdup(temp));
                shash_add(&linkValues, "user_ofile", temp);
	}

	if(!pic16_options.no_crt)
          shash_add(&linkValues, "spec_ofiles", pic16_options.crt_name);

  	shash_add(&linkValues, "ofiles", joinStrSet(relFilesSet));
  	shash_add(&linkValues, "libs", joinStrSet(libFilesSet));
  	
  	lcmd = msprintf(linkValues, lfrm);
  	 
  	ret = my_system( lcmd );
  	 
  	Safe_free( lcmd );
  	 
  	if(ret)
  		exit(1);
}


/* forward declarations */
extern const char *pic16_linkCmd[];
extern const char *pic16_asmCmd[];

static void
_pic16_finaliseOptions (void)
{
    port->mem.default_local_map = data;
    port->mem.default_globl_map = data;

    /* peepholes are disabled for the time being */
    options.nopeep = 1;

    /* explicit enable peepholes for testing */
    if(pic16_enable_peeps)
      options.nopeep = 0;

    options.all_callee_saves = 1;		// always callee saves

#if 0
    options.float_rent = 1;
    options.intlong_rent = 1;
#endif
	

    if(alt_asm && strlen(alt_asm))
      pic16_asmCmd[0] = alt_asm;
	
    if(alt_link && strlen(alt_link))
      pic16_linkCmd[0] = alt_link;
        
    if(!pic16_options.no_crt) {
      pic16_options.omit_ivt = 1;
      pic16_options.leave_reset = 0;
    }
}


#if 0
  if (options.model == MODEL_LARGE)
    {
      port->mem.default_local_map = xdata;
      port->mem.default_globl_map = xdata;
    }
  else
    {
      port->mem.default_local_map = data;
      port->mem.default_globl_map = data;
    }

  if (options.stack10bit)
    {
      if (options.model != MODEL_FLAT24)
	{
	  fprintf (stderr,
		   "*** warning: 10 bit stack mode is only supported in flat24 model.\n");
	  fprintf (stderr, "\t10 bit stack mode disabled.\n");
	  options.stack10bit = 0;
	}
      else
	{
	  /* Fixup the memory map for the stack; it is now in
	   * far space and requires a FPOINTER to access it.
	   */
	  istack->fmap = 1;
	  istack->ptrType = FPOINTER;
	}
    }
#endif


static void
_pic16_setDefaultOptions (void)
{
  options.stackAuto = 0;		/* implicit declaration */
  /* port is not capable yet to allocate separate registers 
   * dedicated for passing certain parameters */
  
  /* initialize to defaults section locations, names and addresses */
  pic16_sectioninfo.at_udata	= "udata";

  /* set pic16 port options to defaults */
  pic16_options.no_banksel = 0;
  pic16_options.opt_banksel = 0;
  pic16_options.omit_configw = 0;
  pic16_options.omit_ivt = 0;
  pic16_options.leave_reset = 0;
  pic16_options.stack_model = 0;			/* 0 for 'small', 1 for 'large' */
  pic16_options.ivt_loc = 0x000000;
  pic16_options.nodefaultlibs = 0;
  pic16_options.dumpcalltree = 0;
  pic16_options.crt_name = "crt0i.o";		/* the default crt to link */
  pic16_options.no_crt = 0;			/* use crt by default */
  pic16_options.ip_stack = 1;		/* set to 1 to enable ipop/ipush for stack */
  pic16_options.gstack = 0;
  pic16_options.debgen = 0;
}

static const char *
_pic16_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}


#if 1
static  char *_pic16_mangleFunctionName(char *sz)
{
//	fprintf(stderr, "mangled function name: %s\n", sz);

  return sz;
}
#endif


static void
_pic16_genAssemblerPreamble (FILE * of)
{
  char *name = pic16_processor_base_name();

	if(!name) {
		name = "p18f452";
		fprintf(stderr,"WARNING: No Pic has been selected, defaulting to %s\n",name);
	}

	fprintf (of, "\tlist\tp=%s\n",&name[1]);

	if(!pic16_options.omit_configw) {
		pic16_emitConfigRegs(of);
		fprintf(of, "\n");
		pic16_emitIDRegs(of);
	}
	
  fprintf (of, "\tradix dec\n");
}

/* Generate interrupt vector table. */
static int
_pic16_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
#if 1
	/* PIC18F family has only two interrupts, the high and the low
	 * priority interrupts, which reside at 0x0008 and 0x0018 respectively - VR */

	if((!pic16_options.omit_ivt) || (pic16_options.omit_ivt && pic16_options.leave_reset)) {
		fprintf(of, "; RESET vector\n");
		fprintf(of, "\tgoto\t__sdcc_gsinit_startup\n");
	}
	
	if(!pic16_options.omit_ivt) {
		fprintf(of, "\tres 4\n");


		fprintf(of, "; High priority interrupt vector 0x0008\n");
		if(interrupts[1]) {
			fprintf(of, "\tgoto\t%s\n", interrupts[1]->rname);
			fprintf(of, "\tres\t12\n"); 
		} else {
			fprintf(of, "\tretfie\n");
			fprintf(of, "\tres\t14\n");
		}

		fprintf(of, "; Low priority interrupt vector 0x0018\n");
		if(interrupts[2]) {
			fprintf(of, "\tgoto\t%s\n", interrupts[2]->rname);
		} else {
			fprintf(of, "\tretfie\n");
		}
	}
#endif
  return TRUE;
}

/* return True if the port can handle the type,
 * False to convert it to function call */
static bool _hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
//	fprintf(stderr,"checking for native mult for %c (size: %d)\n", ic->op, getSize(OP_SYMBOL(IC_RESULT(ic))->type));

#if 1
	/* multiplication is fixed */
	/* support mul for char/int/long */
	if((ic->op == '*')
	  && (getSize(OP_SYMBOL(IC_LEFT(ic))->type ) < 2))return TRUE;
#endif

#if 0
	/* support div for char/int/long */
	if((getSize(OP_SYMBOL(IC_LEFT(ic))->type ) < 0)
		&& (ic->op == '/'))return TRUE;
#endif
	
  return FALSE;
}


#if 0
/* Do CSE estimation */
static bool cseCostEstimation (iCode *ic, iCode *pdic)
{
//    operand *result = IC_RESULT(ic);
//    sym_link *result_type = operandType(result);


	/* VR -- this is an adhoc. Put here after conversation
	 * with Erik Epetrich */

	if(ic->op == '<'
		|| ic->op == '>'
		|| ic->op == EQ_OP) {

		fprintf(stderr, "%d %s\n", __LINE__, __FUNCTION__);
	  return 0;
	}

#if 0
    /* if it is a pointer then return ok for now */
    if (IC_RESULT(ic) && IS_PTR(result_type)) return 1;

    /* if bitwise | add & subtract then no since mcs51 is pretty good at it
       so we will cse only if they are local (i.e. both ic & pdic belong to
       the same basic block */
    if (IS_BITWISE_OP(ic) || ic->op == '+' || ic->op == '-') {
        /* then if they are the same Basic block then ok */
        if (ic->eBBlockNum == pdic->eBBlockNum) return 1;
        else return 0;
    }
#endif

    /* for others it is cheaper to do the cse */
    return 1;
}
#endif


/* Indicate which extended bit operations this port supports */
static bool
hasExtBitOp (int op, int size)
{
  if (op == RRC
      || op == RLC
      /* || op == GETHBIT */ /* GETHBIT doesn't look complete for PIC */
     )
    return TRUE;
  else
    return FALSE;
}

/* Indicate the expense of an access to an output storage class */
static int
oclsExpense (struct memmap *oclass)
{
  /* The IN_FARSPACE test is compatible with historical behaviour, */
  /* but I don't think it is applicable to PIC. If so, please feel */
  /* free to remove this test -- EEP */
  if (IN_FARSPACE(oclass))
    return 1;
    
  return 0;
}

/** $1 is the input object file (PIC16 specific)	// >>always the basename<<.
    $2 is always the output file.
    $3 -L path and -l libraries
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
const char *pic16_linkCmd[] =
{
  "gplink", "$l", "-o \"$2\"", "\"$1\"","$3", NULL
};



/** $1 is always the basename.
    $2 is always the output file.
    $3 varies (nothing currently)
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
const char *pic16_asmCmd[] =
{
  "gpasm", "$l", "$3", "-c", "\"$1.asm\"", "-o \"$2\"", NULL

};

/* Globals */
PORT pic16_port =
{
  TARGET_ID_PIC16,
  "pic16",
  "MCU PIC16",			/* Target name */
  "p18f452",                    /* Processor */
  {
    pic16glue,
    TRUE,			/* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
    MODEL_SMALL
  },
  {
    pic16_asmCmd,		/* assembler command and arguments */
    NULL,			/* alternate macro based form */
    "-g",			/* arguments for debug mode */
    NULL,			/* arguments for normal mode */
    0,				/* print externs as global */
    ".asm",			/* assembler file extension */
    NULL			/* no do_assemble function */
  },
  {
    NULL,			//    pic16_linkCmd,		/* linker command and arguments */
    NULL,			/* alternate macro based form */
    _pic16_linkEdit,		//NULL,			/* no do_link function */
    ".o",			/* extension for object files */
    0				/* no need for linker file */
  },
  {
    _defaultRules
  },
  {
	/* Sizes */
    1,		/* char */
    2,		/* short */
    2,		/* int */
    4,		/* long */
    2,		/* ptr */
    3,		/* fptr, far pointers (see Microchip) */
    3,		/* gptr */
    1,		/* bit */
    4,		/* float */
    4		/* max */
  },
  {
    "XSEG    (XDATA)",		// xstack
    "STACK   (DATA)",		// istack
    "CSEG    (CODE)",		// code
    "DSEG    (DATA)",		// data
    "ISEG    (DATA)",		// idata
    NULL,					// pdata
    "XSEG    (XDATA)",		// xdata
    "BSEG    (BIT)",		// bit
    "RSEG    (DATA)",		// reg
    "GSINIT  (CODE)",		// static
    "OSEG    (OVR,DATA)",	// overlay
    "GSFINAL (CODE)",		// post static
    "HOME	 (CODE)",	// home
    NULL,			// xidata
    NULL,			// xinit
    NULL,			// default location for auto vars
    NULL,			// default location for global vars
    1				// code is read only 1=yes
  },
  {
    NULL,		/* genExtraAreaDeclaration */
    NULL		/* genExatrAreaLinkOptions */
  },
  {
	/* stack related information */
    -1,			/* -1 stack grows downwards, +1 upwards */
    1,			/* extra overhead when calling between banks */
    4,			/* extra overhead when the function is an ISR */
    1,			/* extra overhead for a function call */
    1,			/* re-entrant space */
    0			/* 'banked' call overhead, mild overlap with bank_overhead */
  },
    /* pic16 has an 8 bit mul */
  {
     0, -1
  },
  {
    pic16_emitDebuggerSymbol
  },
  {
    255/3,      /* maxCount */
    3,          /* sizeofElement */
    /* The rest of these costs are bogus. They approximate */
    /* the behavior of src/SDCCicode.c 1.207 and earlier.  */
    {4,4,4},    /* sizeofMatchJump[] */
    {0,0,0},    /* sizeofRangeCompare[] */
    0,          /* sizeofSubtract */
    3,          /* sizeofDispatch */
  },
  "_",
  _pic16_init,
  _pic16_parseOptions,
  pic16_optionsTable,
  _pic16_initPaths,
  _pic16_finaliseOptions,
  _pic16_setDefaultOptions,
  pic16_assignRegisters,
  _pic16_getRegName,
  _pic16_keywords,
  _pic16_genAssemblerPreamble,
  NULL,				/* no genAssemblerEnd */
  _pic16_genIVT,
  NULL, // _pic16_genXINIT
  NULL, 			/* genInitStartup */
  _pic16_reset_regparm,
  _pic16_regparm,
  _process_pragma,				/* process a pragma */
  _pic16_mangleFunctionName,				/* mangles function name */
  _hasNativeMulFor,
  hasExtBitOp,			/* hasExtBitOp */
  oclsExpense,			/* oclsExpense */
  FALSE,			
  TRUE,				/* little endian */
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  FALSE,                        /* No array initializer support. */
  0,	//cseCostEstimation,            /* !!!no CSE cost estimation yet */
  NULL, 			/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};
