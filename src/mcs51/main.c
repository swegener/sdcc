/** @file main.c
    mcs51 specific general functions.

    Note that mlh prepended _mcs51_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "main.h"
#include "ralloc.h"

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

static bool _mcs51_parseOptions(int *pargc, char **argv)
{
    return FALSE;
}

static void _mcs51_finaliseOptions(void)
{
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
	_asmCmd,
	"-plosgffc",		/* Options with debug */
	"-plosgff",		/* Options without debug */
    },
    {
	_linkCmd
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
	"OSEG    (OVR,DATA)"
    },
    { 
	+1, 1, 4, 1, 1
    },
    /* mcs51 has an 8 bit mul */
    {
	1
    },
    _mcs51_parseOptions,
    _mcs51_finaliseOptions,
    _mcs51_setDefaultOptions,
    mcs51_assignRegisters,
    _mcs51_getRegName ,
    _mcs51_keywords

};
