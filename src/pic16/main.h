#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE

#include "ralloc.h"

bool x_parseOptions (char **argv, int *pargc);
void x_setDefaultOptions (void);
void x_finaliseOptions (void);


typedef struct {
	char *at_udata;
} pic16_sectioninfo_t;

typedef struct absSym {
	char name[SDCC_SYMNAME_MAX+1];
	unsigned int address;
} absSym;

typedef struct sectName {
	char *name;
	set *regsSet;
} sectName;

typedef struct sectSym {
	sectName *section;
	char *name;
	regs *reg;
} sectSym;

extern set *absSymSet;
extern set *sectNames;
extern set *sectSyms;

extern int pic16_mplab_comp;
extern int pic16_fstack;
extern int pic16_nl;

#endif
