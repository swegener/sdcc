#include "common.h"
#include "main.h"
#include "ralloc.h"

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

/* Globals */
PORT mcs51_port = {
    "mcs51",
    "MCU 8051",			/* Target name */
    {	
	"asx8051",		/* Assembler executable name */
	"-plosgffc",		/* Options with debug */
	"-plosgff",		/* Options without debug */
	FALSE			/* TRUE if the assembler requires an output name */
    },
    {
	"aslink",		/* Linker executable name */
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 1, 2, 3, 1, 4, 4
    },
    { 
	+1, 1, 4, 0, 0
    },
    /* mcs51 has an 8 bit mul */
    {
	1
    },
    _mcs51_parseOptions,
    _mcs51_finaliseOptions,
    _mcs51_setDefaultOptions,
    mcs51_assignRegisters,
    _mcs51_getRegName
};

