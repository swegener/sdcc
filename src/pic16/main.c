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

static void
_pic16_init (void)
{
	asm_addTree (&asm_asxxxx_mapping);
	pic16_pCodeInitRegisters();
	maxInterrupts = 2;

	/* set pic16 port options to defaults */
	pic16_options.gen_banksel = 0;
	pic16_options.opt_banksel = 0;
	pic16_options.omit_configw = 0;
	pic16_options.omit_ivt = 0;
	pic16_options.leave_reset = 0;
	pic16_options.stack_model = 0;			/* 0 for 'small', 1 for 'large' */
}

static void
_pic16_reset_regparm ()
{
  regParmFlg = 0;
}

static int
_pic16_regparm (sym_link * l)
{
  /* for this processor it is simple
     can pass only the first parameter in a register */
  //if (regParmFlg)
  //  return 0;

  regParmFlg++;// = 1;
  return 1;
}


int initsfpnt=0;		/* set to 1 if source provides a pragma for stack
				 * so glue() later emits code to initialize stack/frame pointers */
set *absSymSet;

static int
_process_pragma(const char *sz)
{
  static const char *WHITE = " \t";
  
  char	*ptr = strtok((char *)sz, WHITE);

	if (startsWith (ptr, "maxram")) {
	  char *maxRAM = strtok((char *)NULL, WHITE);

		if (maxRAM != (char *)NULL) {
		  int maxRAMaddress;
		  value *maxRAMVal;

			maxRAMVal = constVal(maxRAM);
			maxRAMaddress = (int)floatFromVal(maxRAMVal);
			pic16_setMaxRAM(maxRAMaddress);
		}
	}
	
	if(startsWith(ptr, "stack")) {
	  char *stackPosS = strtok((char *)NULL, WHITE);
	  value *stackPosVal;
	  regs *reg;

//	  	fprintf(stderr, "Initializing stack pointer to 0x%x\n", (int)floatFromVal(constVal(stackPos)));
		stackPosVal = constVal( stackPosS );
		stackPos = (unsigned int)floatFromVal( stackPosVal );

		reg=newReg(REG_SFR, PO_SFR_REGISTER, stackPos, "stack", 1, 0, NULL);
		addSet(&pic16_fix_udata, reg);
		
		initsfpnt = 1;		// force glue() to initialize stack/frame pointers */

	  return 0;
	}
	
	if(startsWith(ptr, "code")) {
	  char *symname = strtok((char *)NULL, WHITE);
	  char *location = strtok((char *)NULL, WHITE);
	  absSym *absS;
	  value *addr;

		absS = Safe_calloc(1, sizeof(absSym));
		absS->name = Safe_strdup( symname );
		addr = constVal( location );
		absS->address = (unsigned int)floatFromVal( addr );

		addSet(&absSymSet, absS);
		fprintf(stderr, "%s:%d symbol %s will be placed in location 0x%06x in code memory\n",
			__FILE__, __LINE__, symname, absS->address);

	  return 0;
	}	  

  return 1;
}

#define REP_UDATA	"--preplace-udata-with="
#define REP_UDATAACS	"--preplace-udata-acs-with="
#define REP_UDATAOVR	"--preplace-udata-ovr-with="
#define REP_UDATASHR	"--preplace-udata-sht-with="

#define NAME_CODE	"--psection-code-name="
#define NAME_IDATA	"--psection-idata-name="
#define NAME_UDATA	"--psection-udata-name="
#define NAME_UDATAACS	"--psection-udata-acs-name="
#define NAME_UDATAOVR	"--psection-udata-ovr-name="
#define NAME_UDATASHR	"--psection-udata-shr-name="

#define ADDR_CODE	"--psection-code-addr="
#define ADDR_IDATA	"--psection-idata-addr="
#define ADDR_UDATA	"--psection-udata-addr="
#define ADDR_UDATAACS	"--psection-udata-acs-addr="
#define ADDR_UDATAOVR	"--psection-udata-ovr-addr="
#define ADDR_UDATASHR	"--psection-udata-shr-addr="

#define STACK_MODEL	"--pstack-model="
#define OPT_BANKSEL	"--obanksel="


extern int pic16_debug_verbose;
extern int pic16_ralloc_debug;
extern int pic16_pcode_verbose;

OPTION pic16_optionsTable[]= {
	{ 0,	"--pgen-banksel",	&pic16_options.gen_banksel,	"generate BANKSEL assembler directives"},
	{ 0,	OPT_BANKSEL,		NULL,				"set banksel optimization level (default=0 no)"},
	{ 0,	"--pomit-config-words",	&pic16_options.omit_configw,	"omit the generation of configuration words"},
	{ 0,	"--pomit-ivt",		&pic16_options.omit_ivt,	"omit the generation of the Interrupt Vector Table"},
	{ 0,	"--pleave-reset-vector",&pic16_options.leave_reset,	"when omitting IVT leave RESET vector"},
	{ 0,	STACK_MODEL,	NULL,	"use stack model 'small' (default) or 'large'"},

	{ 0,	"--debug-xtra",		&pic16_debug_verbose,	"show more debug info in assembly output"},
	{ 0,	"--debug-ralloc",	&pic16_ralloc_debug,	"dump register allocator debug file *.d"},
	{ 0,	"--pcode-verbose",	&pic16_pcode_verbose,	"dump pcode related info"},
		
	{ 0,	REP_UDATA,	NULL,	"Place udata variables at another section: udata_acs, udata_ovr, udata_shr"},

#if 0
	/* these may not be in any use -- VR */
	{ 0,	AT_UDATAACS,	NULL,	"Emit udata_acs variables at another section"},
	{ 0,	AT_UDATAOVR,	NULL,	"Emit udata_ovr variables at another section"},
	{ 0,	AT_UDATASHR,	NULL,	"Emit udata_shr variables at another section"},
#endif

#if 0
	/* commented out for the time being -- VR */
	{ 0,	NAME_CODE,	NULL,	"Set code section name[,address]"},
	{ 0,	NAME_IDATA,	NULL,	"Set idata section name[,address]"},
	{ 0,	NAME_UDATA,	NULL,	"Set udata section name[,address]"},
	{ 0,	NAME_UDATAACS,	NULL,	"Set udata_acs section name[,address]"},
	{ 0,	NAME_UDATAOVR,	NULL,	"Set udata_ovr section name[,address]"},
	{ 0,	NAME_UDATASHR,	NULL,	"Set udata_shr section name[,address]"},
	
	{ 0,	ADDR_CODE,	NULL,	"Set code section address"},
	{ 0,	ADDR_IDATA,	NULL,	"Set idata section address"},
	{ 0,	ADDR_UDATA,	NULL,	"Set udata section address"},
	{ 0,	ADDR_UDATAACS,	NULL,	"Set udata_acs section address"},
	{ 0,	ADDR_UDATAOVR,	NULL,	"Set udata_ovr section address"},
	{ 0,	ADDR_UDATASHR,	NULL,	"Set udata_shr section address"},
#endif

	{ 0,	NULL,		NULL,	NULL}
	};


#define ISOPT(str)	!strncmp(argv[ *i ], str, strlen(str) )

extern char *getStringArg(const char *,  char **, int *, int);
extern int getIntArg(const char *, char **, int *, int);

static bool
_pic16_parseOptions (int *pargc, char **argv, int *i)
{
  int j=0;
//  set *tset;
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
		if(STRCASECMP(stkmodel, "small"))pic16_options.stack_model = 0;
		else if(STRCASECMP(stkmodel, "large"))pic16_options.stack_model = 1;
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
	
/*
	if(ISOPT(AT_UDATAACS)) {
		pic16_sectioninfo.at_udataacs = Safe_strdup( getStringArg(AT_UDATAACS, argv, i, *pargc));
		return TRUE;
	}

	if(ISOPT(AT_UDATAOVR)) {
		pic16_sectioninfo.at_udataovr = Safe_strdup( getStringArg(AT_UDATAOVR, argv, i, *pargc));
		return TRUE;
	}
	
	if(ISOPT(AT_UDATASHR)) {
		pic16_sectioninfo.at_udatashr = Safe_strdup( getStringArg(AT_UDATASHR, argv, i, *pargc));
		return TRUE;
	}
*/

#if 0
	if(ISOPT(ADDR_CODE)) {
		pic16_sectioninfo.addr_code = getIntArg(ADDR_CODE, argv, i, *pargc);
		return TRUE;
	}
	
	if(ISOPT(ADDR_IDATA)) {
		pic16_sectioninfo.addr_idata = getIntArg(ADDR_IDATA, argv, i, *pargc);
		return TRUE;
	}
	
	if(ISOPT(ADDR_UDATA)) {
		pic16_sectioninfo.addr_udata = getIntArg(ADDR_UDATA, argv, i, *pargc);
		return TRUE;
	}
	
	if(ISOPT(ADDR_UDATAACS)) {
		pic16_sectioninfo.addr_udataacs = getIntArg(ADDR_UDATAACS, argv, i, *pargc);
		return TRUE;
	}
	
	if(ISOPT(ADDR_UDATAOVR)) {
		pic16_sectioninfo.addr_udataovr = getIntArg(ADDR_UDATAOVR, argv, i, *pargc);
		return TRUE;
	}
	
	if(ISOPT(ADDR_UDATASHR)) {
		pic16_sectioninfo.addr_udatashr = getIntArg(ADDR_UDATASHR, argv, i, *pargc);
		return TRUE;
	}

	tset = newSet();
	
	if(ISOPT(NAME_CODE)) {
		setParseWithComma(&tset, getStringArg(NAME_CODE, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_code = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_code = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	if(ISOPT(NAME_IDATA)) {
		setParseWithComma(&tset, getStringArg(NAME_IDATA, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_idata = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_idata = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	if(ISOPT(NAME_UDATA)) {
		setParseWithComma(&tset, getStringArg(NAME_UDATA, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_udata = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_udata = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	if(ISOPT(NAME_UDATAACS)) {
		setParseWithComma(&tset, getStringArg(NAME_UDATAACS, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_udataacs = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_udataacs = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	if(ISOPT(NAME_UDATAOVR)) {
		setParseWithComma(&tset, getStringArg(NAME_UDATAOVR, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_udataovr = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_udataovr = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	if(ISOPT(NAME_UDATASHR)) {
		setParseWithComma(&tset, getStringArg(NAME_UDATASHR, argv, i, *pargc));
		if(elementsInSet(tset) > 0)
			pic16_sectioninfo.name_udatashr = Safe_strdup( (char *)indexSet(tset, 0));
		if(elementsInSet(tset) > 1)
			pic16_sectioninfo.addr_udatashr = atoi( (char *)indexSet(tset, 1));
		return TRUE;
	}

	deleteSet( &tset );
#endif

  return FALSE;
}

extern set *includeDirsSet;
extern set *dataDirsSet;
extern set *libFilesSet;

static void
_pic16_finaliseOptions (void)
{
  char pic16incDir[512];
  char pic16libDir[512];
  set *pic16incDirsSet;
  set *pic16libDirsSet;
  char devlib[512];

	port->mem.default_local_map = data;
	port->mem.default_globl_map = data;

	options.all_callee_saves = 1;		// always callee saves

	setMainValue("mcu", pic16->name[2] );
	addSet(&preArgvSet, Safe_strdup("-D{mcu}"));

	sprintf(pic16incDir, "%s/pic16", INCLUDE_DIR_SUFFIX);
	sprintf(pic16libDir, "%s/pic16", LIB_DIR_SUFFIX);

	if(!options.nostdinc) {
		/* setup pic16 include directory */
		pic16incDirsSet = appendStrSet(dataDirsSet, NULL, pic16incDir);
		mergeSets(&includeDirsSet, pic16incDirsSet);
	}
	
	if(!options.nostdlib) {
		/* setup pic16 library directory */
		pic16libDirsSet = appendStrSet(dataDirsSet, NULL, pic16libDir);
		mergeSets(&libDirsSet, pic16libDirsSet);
	
		/* now add the library for the device */
		sprintf(devlib, "%s.lib", pic16->name[2]);
		addSet(&libFilesSet, Safe_strdup(devlib));
	}
}


/* all the rest is commented ifdef'd out */
#if 0
  /* Hack-o-matic: if we are using the flat24 model,
   * adjust pointer sizes.
   */
  if (options.model == MODEL_FLAT24)
    {

      fprintf (stderr, "*** WARNING: you should use the '-mds390' option "
	       "for DS80C390 support. This code generator is "
	       "badly out of date and probably broken.\n");

      port->s.fptr_size = 3;
      port->s.gptr_size = 4;
      port->stack.isr_overhead++;	/* Will save dpx on ISR entry. */
#if 1
      port->stack.call_overhead++;	/* This acounts for the extra byte 
					 * of return addres on the stack.
					 * but is ugly. There must be a 
					 * better way.
					 */
#endif
      fReturn = fReturn390;
      fReturnSize = 5;
    }

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
	/* initialize to defaults section locations, names and addresses */
	pic16_sectioninfo.at_udata	= "udata";
	pic16_sectioninfo.at_udataacs	= "udata_acs";
	pic16_sectioninfo.at_udataovr	= "udata_ovr";
	pic16_sectioninfo.at_udatashr	= "udata_shr";

	/* initialize to nothing, so let linker decide about their names */
	pic16_sectioninfo.name_code	=
	pic16_sectioninfo.name_idata	=
	pic16_sectioninfo.name_udata	=
	pic16_sectioninfo.name_udataacs =
	pic16_sectioninfo.name_udataovr =
	pic16_sectioninfo.name_udatashr = "";

	/* initialize to -1, so let linker decide about their address */
	pic16_sectioninfo.addr_code	=
	pic16_sectioninfo.addr_idata	=
	pic16_sectioninfo.addr_udata	=
	pic16_sectioninfo.addr_udataacs =
	pic16_sectioninfo.addr_udataovr =
	pic16_sectioninfo.addr_udatashr = -1;
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
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300001, pic16_getConfigWord(0x300001));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300002, pic16_getConfigWord(0x300002));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300003, pic16_getConfigWord(0x300003));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300005, pic16_getConfigWord(0x300005));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300006, pic16_getConfigWord(0x300006));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300008, pic16_getConfigWord(0x300008));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x300009, pic16_getConfigWord(0x300009));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x30000a, pic16_getConfigWord(0x30000a));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x30000b, pic16_getConfigWord(0x30000b));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x30000c, pic16_getConfigWord(0x30000c));
		fprintf (of, "\t__config 0x%x,0x%hhx\n", 0x30000d, pic16_getConfigWord(0x30000d));
	}
	
  fprintf (of, "\tradix dec\n");
}

/* Generate interrupt vector table. */
static int
_pic16_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
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

  return TRUE;
}

static bool
_hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
  //  sym_link *test = NULL;
  //  value *val;

//  fprintf(stderr,"checking for native mult\n");

  if ( ic->op != '*')
    {
      return FALSE;
    }

  return TRUE;
/*
  if ( IS_LITERAL (left))
    {
      fprintf(stderr,"left is lit\n");
      test = left;
      val = OP_VALUE (IC_LEFT (ic));
    }
  else if ( IS_LITERAL (right))
    {
      fprintf(stderr,"right is lit\n");
      test = left;
      val = OP_VALUE (IC_RIGHT (ic));
    }
  else
    {
      fprintf(stderr,"oops, neither is lit so no\n");
      return FALSE;
    }

  if ( getSize (test) <= 2)
    {
      fprintf(stderr,"yep\n");
      return TRUE;
    }
  fprintf(stderr,"nope\n");

  return FALSE;
*/
}

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

/** $1 is always the basename.
    $2 is always the output file.
    $3 -L path and -l libraries
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
  "gplink", "$3", "\"$1.o\"", "-o $1", "$l", NULL
};



/** $1 is always the basename.
    $2 is always the output file.
    $3 varies (nothing currently)
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_asmCmd[] =
{
  "gpasm", "$l", "$3", "-c", "\"$1.asm\"", NULL

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
    _asmCmd,			/* assembler command and arguments */
    NULL,			/* alternate macro based form */
    NULL,			/* arguments for debug mode */
    NULL,			/* arguments for normal mode */
    0,				/* print externs as global */
    ".asm",			/* assembler file extension */
    NULL			/* no do_assemble function */
  },
  {
    _linkCmd,			/* linker command and arguments */
    NULL,			/* alternate macro based form */
    NULL,			/* no do_link function */
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
    2,		/* gptr */
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
    1, -1
  },
  "_",
  _pic16_init,
  _pic16_parseOptions,
  pic16_optionsTable,
  _pic16_finaliseOptions,
  _pic16_setDefaultOptions,
  pic16_assignRegisters,
  _pic16_getRegName,
  _pic16_keywords,
  _pic16_genAssemblerPreamble,
  NULL,				/* no genAssemblerEnd */
  _pic16_genIVT,
  NULL, // _pic16_genXINIT
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
  0,                            /* no CSE cost estimation yet */
  NULL, 			/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};
