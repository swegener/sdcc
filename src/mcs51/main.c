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
	"GSFINAL (CODE)"
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
    _mcs51_genIVT
};

