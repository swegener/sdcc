/** @file main.c
    avr specific general functions.

    Note that mlh prepended _avr_ on the static functions.  Makes
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
static char *_avr_keywords[] =     {
    "at",
    "code",
    "critical",
    "eeprom"
    "interrupt",
    "sfr",
    "sbit",
    "xdata",
    "_code",
    "_eeprom"
    "_generic",
    "_xdata",
    NULL
};


void avr_assignRegisters (eBBlock **ebbs, int count);

static bool _avr_parseOptions(int *pargc, char **argv, int *i)
{
    /* TODO: allow port-specific command line options to specify
     * segment names here.
     */
    return FALSE;
}

static void _avr_finaliseOptions(void)
{
    port->default_local_map =
	port->default_globl_map = xdata;
}

static void _avr_setDefaultOptions(void)
{
}

static const char *_avr_getRegName(struct regs *reg)
{
    if (reg)
	return reg->name;
    return "err";
}

static void _avr_genAssemblerPreamble(FILE *of)
{

}

/* Generate interrupt vector table. */
static int _avr_genIVT(FILE *of, symbol **interrupts, int maxInterrupts)
{
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
PORT avr_port = {
    "avr",
    "ATMEL AVR",		/* Target name */
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
	NULL,
    },
    { 
	+1, 1, 4, 1, 1
    },
    /* avr has an 8 bit mul */
    {
	1
    },
    NULL,
    _avr_parseOptions,
    _avr_finaliseOptions,
    _avr_setDefaultOptions,
    avr_assignRegisters,
    _avr_getRegName ,
    _avr_keywords,
    _avr_genAssemblerPreamble,
    _avr_genIVT
};

