#include "SDCCglobl.h"
#include "main.h"

/* Globals */
PROCESSOR_CONSTANTS port = {
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
    }
};

