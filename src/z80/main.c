#include "SDCCglobl.h"
#include "main.h"

/* Globals */
PROCESSOR_CONSTANTS port = {
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
    }
};

bool x_parseOptions(char **argv, int *pargc)
{
    int i = *pargc;
    bool fRecognised = FALSE;

    *pargc = i;
    return fRecognised;
}

void x_finaliseOptions(void)
{
}

void x_setDefaultOptions(void)
{    
}

