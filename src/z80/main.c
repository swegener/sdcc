#include "z80.h"

static char _defaultRules[] =
{
#include "peeph.rul"
#include "peeph-z80.rul"
};

Z80_OPTS z80_opts;

static char *_z80_keywords[] = { NULL };

static void _z80_init(void)
{
    z80_opts.sub = SUB_Z80;
}

static int regParmFlg = 0; /* determine if we can register a parameter */

static void _z80_reset_regparm()
{
    regParmFlg = 0;
}

static int _z80_reg_parm(link *l)
{
        /* for this processor it is simple
       can pass only the first parameter in a register */
    if (regParmFlg)
	return 0;

    regParmFlg = 1;
    return 1;

}

static bool _z80_parseOptions(int *pargc, char **argv, int *i)
{
    return FALSE;
}

static void _z80_finaliseOptions(void)
{
    port->mem.default_local_map = data;
    port->mem.default_globl_map = data;
}

static void _z80_setDefaultOptions(void)
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
	FALSE,
    },
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
	"_OVERLAY",
	"_GSFINAL",
	NULL,
	NULL
    },
    { 
	-1, 0, 0, 8, 0
    },
    /* Z80 has no native mul/div commands */
    {  
	0
    },
    _z80_init,
    _z80_parseOptions,
    _z80_finaliseOptions,
    _z80_setDefaultOptions,
    z80_assignRegisters,
    _z80_getRegName,
    _z80_keywords,
    0,	/* no assembler preamble */
    0,	/* no local IVT generation code */
    _z80_reset_regparm,
    _z80_reg_parm
};

