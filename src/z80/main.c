#include "common.h"
#include "ralloc.h"

static char *_z80_keywords[] = { NULL };

void z80_assignRegisters (eBBlock **ebbs, int count);

static bool _z80_parseOptions(int *pargc, char **argv)
{
    return FALSE;
}

static void _z80_finaliseOptions(void)
{
}

static void _z80_setDefaultOptions(void)
{    
    options.genericPtr = 1;   /* default on */
    options.nopeep    = 1;
    options.stackAuto = 1;
    options.mainreturn = 1;
    options.noregparms = 1;
    /* first the options part */
    options.intlong_rent = 1;

    optimize.global_cse = 1;    
    optimize.label1 = 1;
    optimize.label2 = 1;
    optimize.label3 = 1;
    optimize.label4 = 1;    
    optimize.loopInvariant = 1;
    optimize.loopInduction = 0;
}

static const char *_z80_getRegName(struct regs *reg)
{
    if (reg)
	return reg->name;
    assert(0);
    return "err";
}

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] = {
    "link-z80", "-nf", "$1", NULL
};

static const char *_asmCmd[] = {
    "as-z80", "-plosgff", "$1.o", "$1.asm", NULL
};

/* Globals */
PORT z80_port = {
    "z80",
    "Zilog Z80",		/* Target name */
    {	
	_asmCmd,
	"-plosgff",		/* Options with debug */
	"-plosgff",		/* Options without debug */
    },
    {
	_linkCmd
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 2, 2, 2, 1, 4, 4
    },
    {
	"_XSEG",
	"_STACK",
	"_CODE",
	"_DATA",
	"_ISEG",
	"_XSEG",
	"_BSEG",
	"_RSEG",
	"_GSINIT",
	"_OVERLAY"
    },
    { 
	-1, 0, 0, 8, 0
    },
    /* Z80 has no native mul/div commands */
    {  
	0
    },
    _z80_parseOptions,
    _z80_finaliseOptions,
    _z80_setDefaultOptions,
    z80_assignRegisters,
    _z80_getRegName,
    _z80_keywords
};

