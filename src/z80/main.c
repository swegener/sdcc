#include "common.h"
#include "main.h"

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

    optimize.global_cse = 0;    
    optimize.label1 = 0;
    optimize.label2 = 0;
    optimize.label3 = 0;
    optimize.label4 = 0;    
    optimize.loopInvariant = 0;
    optimize.loopInduction = 0;
}

/* Globals */
PORT z80_port = {
    "z80",
    "Zilog Z80",		/* Target name */
    {	
	"as-z80",		/* Assembler executable name */
	"-plosgff",		/* Options with debug */
	"-plosgff",		/* Options without debug */
	TRUE			/* TRUE if the assembler requires an output name */
    },
    {
	"link-z80",		/* Linker executable name */
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 2, 2, 2, 1, 4, 4
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
};

