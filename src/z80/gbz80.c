/** @file z80/gbz80.c
    Extra bits ontop of the z80 port to target the Gameboy's Z80 like
    processor.

    Michael Hope <michaelh@earthling.net> 2000
*/

#include "z80.h"

static char _defaultRules[] =
{
#include "peeph.rul"
};

static char *_gbz80_keywords[] = { NULL };

static bool _gbz80_parseOptions(int *pargc, char **argv, int *i)
{
    return FALSE;
}

static void _gbz80_init(void)
{
    z80_opts.sub = SUB_GBZ80;
}

static void _gbz80_finaliseOptions(void)
{
}

static void _gbz80_setDefaultOptions(void)
{    
    options.genericPtr = 1;   /* default on */
    options.nopeep    = 0;
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

static const char *_gbz80_getRegName(struct regs *reg)
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
    "link-gb", "-nf", "$1", NULL
};

static const char *_asmCmd[] = {
    "as-gb", "-plosgff", "$1.o", "$1.asm", NULL
};

/* Globals */
PORT gbz80_port = {
    "gbz80",
    "Gameboy Z80-like",		/* Target name */
    {	
	_asmCmd,
	"-plosgff",		/* Options with debug */
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
	-1, 0, 0, 4, 0
    },
    /* Z80 has no native mul/div commands */
    {  
	0
    },
    _gbz80_init,
    _gbz80_parseOptions,
    _gbz80_finaliseOptions,
    _gbz80_setDefaultOptions,
    z80_assignRegisters,
    _gbz80_getRegName,
    _gbz80_keywords,
    0,	/* no assembler preamble */
    0,	/* no local IVT generation code */
};
