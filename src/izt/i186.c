/** @file izt/i186.c
    i186 specific general functions.
*/
#include "izt.h"

static REG _i186_regs[] = {
    { 1, REG_ID_CL, "cl", 0, { REG_ID_CX, REG_ID_NONE, REG_ID_NONE } },
    { 1, REG_ID_CH, "ch", 0, { REG_ID_CX, REG_ID_NONE, REG_ID_NONE } },
    { 1, REG_ID_DL, "dl", 0, { REG_ID_DX, REG_ID_NONE, REG_ID_NONE } },
    { 1, REG_ID_DH, "dh", 0, { REG_ID_DX, REG_ID_NONE, REG_ID_NONE } },
    { 2, REG_ID_CX, "cx", 0, { REG_ID_CL, REG_ID_CH, REG_ID_NONE } },
    { 2, REG_ID_DX, "dx", 0, { REG_ID_DL, REG_ID_DH, REG_ID_NONE } },
    { 0, REG_ID_NONE,"??",  0, { REG_ID_NONE, REG_ID_NONE, REG_ID_NONE } }
};

static IZT_PORT _i186_port = {
    _i186_regs
};

static char _defaultRules[] =
{
    //#include "peeph.rul"
};

/* list of key words used by i186 */
static char *_i186_keywords[] =     {
    NULL
};

// PENDING: A default set of mappings to make asm.c happy.
static const ASM_MAPPING _asxxxx_z80_mapping[] = {
    /* We want to prepend the _ */
    { "area", ".area _%s" },
    { "areacode", ".area _%s" },
    { "areadata", ".area _%s" },
    { "areahome", ".area _%s" },
    { "*ixx", "%d(ix)" },
    { "*iyx", "%d(iy)" },
    { "*hl", "(hl)" },
    { "di", "di" },
    { "ldahli", 
		"ld a,(hl)\n"
		"\tinc\thl" },
    { "ldahlsp", 
		"ld hl,#%d\n"
		"\tadd\thl,sp" },
    { "ldaspsp", 
		"ld hl,#%d\n"
		"\tadd\thl,sp\n"
		"\tld\tsp,hl" },
    { "*pair", "(%s)" },
    { "shortjp", "jp" },
    { "enter", 
		"push\tix\n"
		"\tld\tix,#0\n"
		"\tadd\tix,sp" },
    { "enterx", 
		"push\tix\n"
		"\tld\tix,#0\n"
		"\tadd\tix,sp\n"
		"\tld\thl,#-%d\n"
		"\tadd\thl,sp\n"
		"\tld\tsp,hl" },
    { "leave", 
		"pop\tix\n"
    },
    { "leavex", 
		"ld sp,ix\n"
		"\tpop\tix\n"
    },
    { "pusha", 
      		"push af\n"
      		"\tpush\tbc\n"
      		"\tpush\tde\n"
      		"\tpush\thl"
    },
    { "adjustsp", "lda sp,-%d(sp)" },
    { NULL, NULL }
};

static const ASM_MAPPINGS _asxxxx_z80 = {
    &asm_asxxxx_mapping,
    _asxxxx_z80_mapping
};

static void _i186_init(void)
{
    asm_addTree(&asm_asxxxx_mapping);
    asm_addTree(&_asxxxx_z80);
    izt_init(&_i186_port);
}

static void _i186_reset_regparm()
{
}

static int _i186_regparm( sym_link *l)
{
    // PENDING: No register parameters.
    return 0;
}

static bool _i186_parseOptions(int *pargc, char **argv, int *i)
{
    /* TODO: allow port-specific command line options to specify
     * segment names here.
     */
    return FALSE;
}

static void _i186_finaliseOptions(void)
{
    // No options
}

static void _i186_setDefaultOptions(void)
{
    // No options
}

static const char *_i186_getRegName(struct regs *reg)
{
    if (reg)
	return reg->name;
    wassert(0);
    return "err";
}

static void _i186_genAssemblerPreamble(FILE *of)
{
    // PENDING
}

/* Generate interrupt vector table. */
static int _i186_genIVT(FILE *of, symbol **interrupts, int maxInterrupts)
{
    // PENDING
    return 0;
}

/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
// PENDING
static const char *_linkCmd[] = {
    "aslink", "-nf", "$1", NULL
};

// PENDING
static const char *_asmCmd[] = {
 "gpasm", NULL, NULL, NULL
};

void i186_assignRegisters (eBBlock **ebbs, int count)
{
}

/* Globals */
PORT i186_port = {
    "i186",
    "Intel 8086/80186",		/* Target name */
    {
	FALSE,			/* Emit glue around main */
	MODEL_SMALL,
	MODEL_SMALL
    },
    {	
	_asmCmd,
	NULL,
	NULL,
	0
    },
    {
	_linkCmd,
	NULL,
	".o"
    },
    {
	_defaultRules
    },
    {
    	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
	1, 2, 2, 4, 2, 2, 2, 1, 4, 4
    },
    {
	"XSEG    (XDATA)",
	"STACK   (DATA)",
	"CSEG    (CODE)",
	"DSEG    (DATA)",
	"ISEG    (DATA)",
	"XSEG    (XDATA)",
	"BSEG    (BIT)",
	"RSEG    (DATA)",
	"GSINIT  (CODE)",
	"OSEG    (OVR,DATA)",
	"GSFINAL (CODE)",
	"HOME	 (CODE)",
	NULL,
	NULL,
	1
    },
    { 
	+1, 1, 4, 1, 1, 0
    },
    /* i186 has an 16 bit mul */
    {
	2, 0
    },
    "_",
    _i186_init,
    _i186_parseOptions,
    _i186_finaliseOptions,
    _i186_setDefaultOptions,
    izt_assignRegisters,
    _i186_getRegName ,
    _i186_keywords,
    _i186_genAssemblerPreamble,
    _i186_genIVT ,
    _i186_reset_regparm,
    _i186_regparm,
    NULL,
    FALSE,
    0,  /* leave lt */
    0,  /* leave gt */
    1,  /* transform <= to ! > */
    1,  /* transform >= to ! < */
    1,  /* transform != to !(a == b) */
    0,  /* leave == */
    PORT_MAGIC
};
