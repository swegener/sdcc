#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE

bool x_parseOptions (char **argv, int *pargc);
void x_setDefaultOptions (void);
void x_finaliseOptions (void);


typedef struct {
	char *at_udata;
	char *at_udataacs;
	char *at_udataovr;
	char *at_udatashr;
	
	char *name_code;
	char *name_idata;
	char *name_udata;
	char *name_udataacs;
	char *name_udataovr;
	char *name_udatashr;
	
	unsigned int addr_code;
	unsigned int addr_idata;
	unsigned int addr_udata;
	unsigned int addr_udataacs;
	unsigned int addr_udataovr;
	unsigned int addr_udatashr;
} pic16_sectioninfo_t;

typedef struct absSym {
	char *name;
	unsigned int address;
} absSym;

extern set *absSymSet;


#endif
