/** @file main.c
    mcs51 specific general functions.

    Note that mlh prepended _mcs51_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "gen.h"

static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by msc51 */
static char *_mcs51_keywords[] =     {
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


void mcs51_assignRegisters (eBBlock **ebbs, int count);

static int regParmFlg = 0; /* determine if we can register a parameter */

static void _mcs51_reset_regparm()
{
    regParmFlg = 0;
}

static int _mcs51_regparm( link *l)
{
    /* for this processor it is simple
       can pass only the first parameter in a register */
    if (regParmFlg)
	return 0;

    regParmFlg = 1;
    return 1;
}

static bool _mcs51_parseOptions(int *pargc, char **argv, int *i)
{
    /* TODO: allow port-specific command line options to specify
     * segment names here.
     */
    return FALSE;
}

static void _mcs51_finaliseOptions(void)
{
    /* Hack-o-matic: if we are using the flat24 model,
     * adjust pointer sizes.
     */
    if (options.model == MODEL_FLAT24)
    {
        port->s.fptr_size = 3;
        port->s.gptr_size = 4;
        port->stack.isr_overhead++;   /* Will save dpx on ISR entry. */
        #if 1
        port->stack.call_overhead++; 	   /* This acounts for the extra byte 
        				    * of return addres on the stack.
        				    * but is ugly. There must be a 
        				    * better way.
        				    */
	#endif
        fReturn = fReturn390;
        fReturnSize = 5;
    } 

    if (options.model) {
	port->mem.default_local_map = xdata;
	port->mem.default_globl_map = xdata;
    } else {
	port->mem.default_local_map = data;
	port->mem.default_globl_map = data;
    }
    
    if (options.stack10bit)
    {
    	if (options.model != MODEL_FLAT24)
    	{
            fprintf(stderr, 
        	    "*** warning: 10 bit stack mode is only supported in flat24 model.\n");
            fprintf(stderr, "\t10 bit stack mode disabled.\n");
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
}

static void _mcs51_setDefaultOptions(void)
{
}

static const char *_mcs51_getRegName(struct regs *reg)
{
    if (reg)
	return reg->name;
    return "err";
}

static void _mcs51_genAssemblerPreamble(FILE *of)
{
   if (options.model == MODEL_FLAT24)
   {
       fputs(".flat24 on\t\t; 24 bit flat addressing\n", of);
       fputs("dpx = 0x93\t\t; dpx register unknown to assembler\n", of);
       fputs("dps = 0x86\t\t; dps register unknown to assembler\n", of);
       fputs("dpl1 = 0x84\t\t; dpl1 register unknown to assembler\n", of);
       fputs("dph1 = 0x85\t\t; dph1 register unknown to assembler\n", of);
       fputs("dpx1 = 0x95\t\t; dpx1 register unknown to assembler\n", of);
   }
}

/* Generate interrupt vector table. */
static int _mcs51_genIVT(FILE *of, symbol **interrupts, int maxInterrupts)
{
    int i;
    
    if (options.model != MODEL_FLAT24)
    {
        /* Let the default code handle it. */
    	return FALSE;
    }
    
    fprintf (of, "\tajmp\t__sdcc_gsinit_startup\n");
    
    /* now for the other interrupts */
    for (i = 0; i < maxInterrupts; i++) 
    {
	if (interrupts[i])
	{
	    fprintf(of, "\tljmp\t%s\n\t.ds\t4\n", interrupts[i]->rname);
	}
	else
	{
	    fprintf(of, "\treti\n\t.ds\t7\n");
	}
    }
    
    return TRUE;
}

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] = {
    "aslink", "-nf", "$1", NULL
};

static const char *_asmCmd[] = {
    "asx8051", "-plosgffc", "$1.asm", NULL
};

/* Globals */
PORT mcs51_port = {
    "mcs51",
    "MCU 8051",			/* Target name */
    {
	TRUE,			/* Emit glue around main */
    },
    {	
	_asmCmd,
	"-plosgffc",		/* Options with debug */
	"-plosgff",		/* Options without debug */
    },
    {
	_linkCmd
    },
    {
	_defaultRules
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 1, 2, 3, 1, 4, 4
    },
    {
	"XSEG    (XDATA)",
	"STACK   (DATA)",
	"CSEG    (CODE)",
	"DSEG    (DATA)",
	"ISEG    (DATA)",
	"XSEG    (XDATA)",
	"BSEG    (BIT)",
	"RSEG    (DATA)",
	"GSINIT  (CODE)",
	"OSEG    (OVR,DATA)",
	"GSFINAL (CODE)",
	NULL,
	NULL
    },
    { 
	+1, 1, 4, 1, 1
    },
    /* mcs51 has an 8 bit mul */
    {
	1
    },
    NULL,
    _mcs51_parseOptions,
    _mcs51_finaliseOptions,
    _mcs51_setDefaultOptions,
    mcs51_assignRegisters,
    _mcs51_getRegName ,
    _mcs51_keywords,
    _mcs51_genAssemblerPreamble,
    _mcs51_genIVT ,
    _mcs51_reset_regparm,
    _mcs51_regparm
};

