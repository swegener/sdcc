#include "z80.h"

static char _z80_defaultRules[] =
{
#include "peeph.rul"
#include "peeph-z80.rul"
};

static char _gbz80_defaultRules[] =
{
#include "peeph.rul"
#include "peeph-gbz80.rul"
};

Z80_OPTS z80_opts;

static struct {
    bool fsetAsmType;
} _G;

static char *_keywords[] = {
    "sfr",
    NULL 
};

extern PORT gbz80_port;
extern PORT z80_port;

#include "mappings.i"

static void _z80_init(void)
{
    z80_opts.sub = SUB_Z80;
    asm_addTree(&_asxxxx_z80);
}

static void _gbz80_init(void)
{
    z80_opts.sub = SUB_GBZ80;
}

static int regParmFlg = 0; /* determine if we can register a parameter */

static void _reset_regparm()
{
    regParmFlg = 0;
}

static int _reg_parm(link *l)
{
        /* for this processor it is simple
       can pass only the first parameter in a register */
    if (regParmFlg)
	return 0;

    regParmFlg = 1;
    return 1;

}

static bool _startsWith(const char *sz, const char *key)
{
    return !strncmp(sz, key, strlen(key));
}

static void _chomp(char *sz)
{
    char *nl;
    while ((nl = strrchr(sz, '\n'))) 
	*nl = '\0';
}

static int _process_pragma(const char *sz)
{
    if (_startsWith(sz, "bank=")) {
	char buffer[128];
	sprintf(buffer, "%s", sz+5);
	_chomp(buffer);
	gbz80_port.mem.code_name = gc_strdup(buffer);
	code->sname = gbz80_port.mem.code_name;
	return 0;
    }
    return 1;
}

static bool _parseOptions(int *pargc, char **argv, int *i)
{
    if (argv[*i][0] == '-') {
	if (argv[*i][1] == 'b' && IS_GB) {
	    int bank = atoi(argv[*i] + 3);
	    char buffer[128];
	    switch (argv[*i][2]) {
	    case 'o':
		/* ROM bank */
		sprintf(buffer, "CODE_%u", bank);
		gbz80_port.mem.code_name = gc_strdup(buffer);
		return TRUE;
	    case 'a':
		/* RAM bank */
		sprintf(buffer, "DATA_%u", bank);
		gbz80_port.mem.data_name = gc_strdup(buffer);
		return TRUE;
	    }
	}
	else if (!strncmp(argv[*i], "--asm=", 6)) {
	    if (!strcmp(argv[*i], "--asm=rgbds")) {
		asm_addTree(&_rgbds_gb);
		_G.fsetAsmType = TRUE;
		return TRUE;
	    }
	    else if (!strcmp(argv[*i], "--asm=asxxxx")) {
		asm_addTree(&_asxxxx_gb);
		_G.fsetAsmType = TRUE;
		return TRUE;
	    }
	    else if (!strcmp(argv[*i], "--asm=isas")) {
		asm_addTree(&_isas_gb);
		/* Munge the function prefix */
		gbz80_port.fun_prefix = "";
		_G.fsetAsmType = TRUE;
		return TRUE;
	    }
	}
    }
    return FALSE;
}

static void _finaliseOptions(void)
{
    port->mem.default_local_map = data;
    port->mem.default_globl_map = data;
    if (!_G.fsetAsmType && IS_GB)
	asm_addTree(&_asxxxx_gb);
}

static void _setDefaultOptions(void)
{    
    options.genericPtr = 1;   /* default on */
    options.nopeep    = 0;
    options.stackAuto = 1;
    options.mainreturn = 1;
    options.noregparms = 1;
    options.nodebug = 1;
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

static const char *_getRegName(struct regs *reg)
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
static const char *_z80_linkCmd[] = {
    "link-z80", "-nf", "$1", NULL
};

static const char *_z80_asmCmd[] = {
    "as-z80", "-plosgff", "$1.o", "$1.asm", NULL
};

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_gbz80_linkCmd[] = {
    "link-gbz80", "-nf", "$1", NULL
};

static const char *_gbz80_asmCmd[] = {
    "as-gbz80", "-plosgff", "$1.o", "$1.asm", NULL
};

/* Globals */
PORT z80_port = {
    "z80",
    "Zilog Z80",		/* Target name */
    {
	FALSE,
    },
    {	
	_z80_asmCmd,
	"-plosgff",		/* Options with debug */
	"-plosgff",		/* Options without debug */
    },
    {
	_z80_linkCmd
    },
    {
	_z80_defaultRules
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 2, 2, 2, 1, 4, 4
    },
    {
	"XSEG",
	"STACK",
	"CODE",
	"DATA",
	"ISEG",
	"XSEG",
	"BSEG",
	"RSEG",
	"GSINIT",
	"OVERLAY",
	"GSFINAL",
	NULL,
	NULL,
	1
    },
    { 
	-1, 0, 0, 8, 0
    },
    /* Z80 has no native mul/div commands */
    {  
	0
    },
    "_",
    _z80_init,
    _parseOptions,
    _finaliseOptions,
    _setDefaultOptions,
    z80_assignRegisters,
    _getRegName,
    _keywords,
    0,	/* no assembler preamble */
    0,	/* no local IVT generation code */
    _reset_regparm,
    _reg_parm,
    TRUE
};

/* Globals */
PORT gbz80_port = {
    "gbz80",
    "Gameboy Z80-like",		/* Target name */
    {
	FALSE,
    },
    {	
	_gbz80_asmCmd,
	"-plosgff",		/* Options with debug */
	"-plosgff",		/* Options without debug */
    },
    {
	_gbz80_linkCmd
    },
    {
	_gbz80_defaultRules
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 1, 2, 4, 2, 2, 2, 1, 4, 4
    },
    {
	"XSEG",
	"STACK",
	"CODE",
	"DATA",
	"ISEG",
	"XSEG",
	"BSEG",
	"RSEG",
	"GSINIT",
	"OVERLAY",
	"GSFINAL",
	NULL,
	NULL,
	1
    },
    { 
	-1, 0, 0, 4, 0
    },
    /* gbZ80 has no native mul/div commands */
    {  
	0
    },
    "_",
    _gbz80_init,
    _parseOptions,
    _finaliseOptions,
    _setDefaultOptions,
    z80_assignRegisters,
    _getRegName,
    _keywords,
    0,	/* no assembler preamble */
    0,	/* no local IVT generation code */
    _reset_regparm,
    _reg_parm,
    _process_pragma,
    TRUE
};
