#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE

bool x_parseOptions (char **argv, int *pargc);
void x_setDefaultOptions (void);
void x_finaliseOptions (void);


typedef struct {
	char *at_udata;
} pic16_sectioninfo_t;

typedef struct absSym {
	char *name;
	unsigned int address;
} absSym;


extern set *absSymSet;


#endif
