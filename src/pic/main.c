/** @file main.c
    pic14 specific general functions.

    Note that mlh prepended _pic14_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "device.h"
#include "SDCCutil.h"
#include "glue.h"
//#include "gen.h"


static char _defaultRules[] =
{
#include "peeph.rul"
	""
};

/* list of key words used by msc51 */
static char *_pic14_keywords[] =
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
		NULL
};

void  pCodeInitRegisters(void);

void pic14_assignRegisters (eBBlock ** ebbs, int count);

static int regParmFlg = 0;	/* determine if we can register a parameter */

static void
_pic14_init (void)
{
	asm_addTree (&asm_asxxxx_mapping);
	pCodeInitRegisters();
}

static void
_pic14_reset_regparm ()
{
	regParmFlg = 0;
}

static int
_pic14_regparm (sym_link * l)
{
/* for this processor it is simple
	can pass only the first parameter in a register */
	//if (regParmFlg)
	//  return 0;
	
	regParmFlg++;// = 1;
	return 1;
}

static int
_process_pragma(const char *sz)
{
	static const char *WHITE = " \t";
	char	*ptr = strtok((char *)sz, WHITE);
	
	if (startsWith (ptr, "memmap"))
	{
		char	*start;
		char	*end;
		char	*type;
		char	*alias;
		
		start = strtok((char *)NULL, WHITE);
		end = strtok((char *)NULL, WHITE);
		type = strtok((char *)NULL, WHITE);
		alias = strtok((char *)NULL, WHITE);
		
		if (start != (char *)NULL
			&& end != (char *)NULL
			&& type != (char *)NULL) {
			value		*startVal = constVal(start);
			value		*endVal = constVal(end);
			value		*aliasVal;
			memRange	r;
			
			if (alias == (char *)NULL) {
				aliasVal = constVal(0);
			} else {
				aliasVal = constVal(alias);
			}
			
			r.start_address = (int)floatFromVal(startVal);
			r.end_address = (int)floatFromVal(endVal);
			r.alias = (int)floatFromVal(aliasVal);
			r.bank = (r.start_address >> 7) & 3;
			
			if (strcmp(type, "RAM") == 0) {
				addMemRange(&r, 0);
			} else if (strcmp(type, "SFR") == 0) {
				addMemRange(&r, 1);
			} else {
				return 1;
			}
		}
		
		return 0;
	} else if (startsWith (ptr, "maxram")) {
		char *maxRAM = strtok((char *)NULL, WHITE);
		
		if (maxRAM != (char *)NULL) {
			int	maxRAMaddress;
			value 	*maxRAMVal;
			
			maxRAMVal = constVal(maxRAM);
			maxRAMaddress = (int)floatFromVal(maxRAMVal);
			setMaxRAM(maxRAMaddress);
		}
		
		return 0;
	}
	return 1;
}

extern char *udata_section_name;

static bool
_pic14_parseOptions (int *pargc, char **argv, int *i)
{
	char buf[128];
	
	/* TODO: allow port-specific command line options to specify
	* segment names here.
	*/
	
	/* This is a temporary hack, to solve problems with some processors
	* that do not have udata section. It will be changed when a more
	* robust solution is figured out -- VR 27-11-2003 FIXME
	*/
	strcpy(buf, "--udata-section-name");
	if(!strncmp(buf, argv[ *i ], strlen(buf))) {
		if(strlen(argv[ *i ]) <= strlen(buf)+1) {
			fprintf(stderr, "WARNING: no `%s' entered\n", buf+2);
			exit(-1);
		} else {
			udata_section_name = strdup( strchr(argv[*i], '=') + 1 );
		}
		return 1;
	}
	
	return FALSE;
}

static void
_pic14_finaliseOptions (void)
{
	
	port->mem.default_local_map = data;
	port->mem.default_globl_map = data;
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
}

static void
_pic14_setDefaultOptions (void)
{
}

static const char *
_pic14_getRegName (struct regs *reg)
{
	if (reg)
		return reg->name;
	return "err";
}

extern char *processor_base_name(void);

static void
_pic14_genAssemblerPreamble (FILE * of)
{
	char * name = processor_base_name();
	
	if(!name) {
		
		name = "p16f877";
		fprintf(stderr,"WARNING: No Pic has been selected, defaulting to %s\n",name);
	}
	
	fprintf (of, "\tlist\tp=%s\n",&name[1]);
	fprintf (of, "\tradix dec\n");
	fprintf (of, "\tinclude \"%s.inc\"\n",name);
}

/* Generate interrupt vector table. */
static int
_pic14_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
	int i;
	
	if (options.model != MODEL_FLAT24)
	{
		/* Let the default code handle it. */
		return FALSE;
	}
	
	fprintf (of, "\t;ajmp\t__sdcc_gsinit_startup\n");
	
	/* now for the other interrupts */
	for (i = 0; i < maxInterrupts; i++)
	{
		if (interrupts[i])
		{
			fprintf (of, "\t;ljmp\t%s\n\t.ds\t4\n", interrupts[i]->rname);
		}
		else
		{
			fprintf (of, "\t;reti\n\t.ds\t7\n");
		}
	}
	
	return TRUE;
}

static bool
_hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
/*
sym_link *test = NULL;
value *val;
	*/
	
	fprintf(stderr,"checking for native mult\n");
	
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
$3 varies
$l is the list of extra options that should be there somewhere...
MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
	"gplink", "-o $2", "\"$1.o\"", "$l", NULL
};

static const char *_asmCmd[] =
{
	"gpasm", "$l", "-c", "\"$1.asm\"", NULL
		
};

/* Globals */
PORT pic_port =
{
	TARGET_ID_PIC,
	"pic14",
	"MCU pic",			/* Target name */
	"",                    /* Processor */
	{
		picglue,
		TRUE,			/* Emit glue around main */
		MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
		MODEL_SMALL
	},
	{
		_asmCmd,
		NULL,
		NULL,
		NULL,
		//"-plosgffc",          /* Options with debug */
		//"-plosgff",           /* Options without debug */
		0,
		".asm",
		NULL			/* no do_assemble function */
	},
	{
		_linkCmd,
		NULL,
		NULL,
		".o",
		0
	},
	{
		_defaultRules
	},
	{
		/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
		1, 2, 2, 4, 2, 2, 2, 1, 4, 4
		/* TSD - I changed the size of gptr from 3 to 1. However, it should be
		   2 so that we can accomodate the PIC's with 4 register banks (like the
		   16f877)
		 */
	},
	{
		"XSEG    (XDATA)",
		"STACK   (DATA)",
		"code",
		"DSEG    (DATA)",
		"ISEG    (DATA)",
		"XSEG    (XDATA)",
		"BSEG    (BIT)",
		"RSEG    (DATA)",
		"GSINIT  (CODE)",
		"udata_ovr",
		"GSFINAL (CODE)",
		"HOME	 (CODE)",
		NULL, // xidata
		NULL, // xinit
		NULL,
		NULL,
		1        // code is read only
	},
	{ NULL, NULL },
	{
		+1, 1, 4, 1, 1, 0
	},
		/* pic14 has an 8 bit mul */
	{
		1, -1
	},
	"_",
	_pic14_init,
	_pic14_parseOptions,
	NULL,
	_pic14_finaliseOptions,
	_pic14_setDefaultOptions,
	pic14_assignRegisters,
	_pic14_getRegName,
	_pic14_keywords,
	_pic14_genAssemblerPreamble,
	NULL,				/* no genAssemblerEnd */
	_pic14_genIVT,
	NULL, // _pic14_genXINIT
	NULL, 				/* genInitStartup */
	_pic14_reset_regparm,
	_pic14_regparm,
	_process_pragma,				/* process a pragma */
	NULL,
	_hasNativeMulFor,
	hasExtBitOp,			/* hasExtBitOp */
	oclsExpense,			/* oclsExpense */
	FALSE,
//	TRUE,				/* little endian */
	FALSE,				/* little endian - PIC code enumlates big endian */
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
