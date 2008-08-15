/** @file main.c
    pic14 specific general functions.

    Note that mlh prepended _pic14_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "MySystem.h"
#include "SDCCmacro.h"

#include "device.h"
#include "gen.h"
#include "glue.h"
#include "main.h"
#include "pcode.h"
#include "ralloc.h"

/*
 * Imports
 */
extern set *dataDirsSet;
extern set *includeDirsSet;
extern set *libDirsSet;
extern set *libPathsSet;
extern set *linkOptionsSet;


pic14_options_t pic14_options;
int debug_verbose = 0;


#define OPTION_STACK_SIZE         "--stack-size"

static char _defaultRules[] =
{
#include "peeph.rul"
};

static OPTION _pic14_poptions[] =
  {
    { 0, "--debug-xtra",   &debug_verbose, "show more debug info in assembly output" },
    { 0, "--no-pcode-opt", &pic14_options.disable_df, "disable (slightly faulty) optimization on pCode" },
    { 0, OPTION_STACK_SIZE, &options.stack_size, "sets the size if the argument passing stack (default: 16, minimum: 4)", CLAT_INTEGER },
    { 0, NULL, NULL, NULL }
  };

/* list of key words used by pic14 */
static char *_pic14_keywords[] =
{
  "at",
  //"bit",
  "code",
  "critical",
  "data",
  "far",
  "idata",
  "interrupt",
  "near",
  "pdata",
  "reentrant",
  "sfr",
  //"sbit",
  "using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  "_pdata",
  "_idata",
  NULL
};

static int regParmFlg = 0;  /* determine if we can register a parameter */


/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
  "gplink", "$l", "-w", "-r", "-o \"$2\"", "\"$1\"", "$3", NULL
};

static const char *_asmCmd[] =
{
  "gpasm", "$l", "$3", "-c", "\"$1.asm\"", NULL
};

static void
_pic14_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
  memset (&pic14_options, 0, sizeof (pic14_options));
}

static void
_pic14_reset_regparm (void)
{
  regParmFlg = 0;
}

static int
_pic14_regparm (sym_link * l, bool reentrant)
{
/* for this processor it is simple
  can pass only the first parameter in a register */
  //if (regParmFlg)
  //  return 0;
  
  regParmFlg++;// = 1;
  return 1;
}

static bool
_pic14_parseOptions (int *pargc, char **argv, int *i)
{
    /* TODO: allow port-specific command line options to specify
    * segment names here.
    */
    return FALSE;
}

/* pic14 port uses include/pic and lib/pic instead of
 * include/pic14 and lib/pic14 as indicated by SDCCmain.c's
 * setIncludePaths routine. */
static void
_pic14_initPaths (void)
{
  char *p;
  char *p2=NULL;
  set *tempSet=NULL;

  if (options.nostdinc)
      return;

  tempSet = appendStrSet(dataDirsSet, NULL, INCLUDE_DIR_SUFFIX DIR_SEPARATOR_STRING "pic");
  mergeSets(&includeDirsSet, tempSet);

  if ((p = getenv(SDCC_INCLUDE_NAME)) != NULL)
  {
    addSetHead(&includeDirsSet, p);
    p2=Safe_alloc(strlen(p)+strlen(DIR_SEPARATOR_STRING)+strlen("pic")+1);
    if(p2!=NULL)
    {
        strcpy(p2, p);
        strcat(p2, DIR_SEPARATOR_STRING);
        strcat(p2, "pic");
        addSetHead(&includeDirsSet, p2);
    }
  }
}

static void
_pic14_finaliseOptions (void)
{
  pCodeInitRegisters();
  
  port->mem.default_local_map = data;
  port->mem.default_globl_map = data;
}

static void
_pic14_setDefaultOptions (void)
{
}

static const char *
_pic14_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}

static void
_pic14_genAssemblerPreamble (FILE * of)
{
  char * name = processor_base_name();
  
  if(!name) {
    
    name = "16f877";
    fprintf(stderr,"WARNING: No Pic has been selected, defaulting to %s\n",name);
  }
  
  fprintf (of, "\tlist\tp=%s\n",name);
  fprintf (of, "\tradix dec\n");
  fprintf (of, "\tinclude \"p%s.inc\"\n",name);
}

/* Generate interrupt vector table. */
static int
_pic14_genIVT (struct dbuf_s * oBuf, symbol ** interrupts, int maxInterrupts)
{
  /* Let the default code handle it. */
  return FALSE;
}

static bool
_hasNativeMulFor (iCode *ic, sym_link *left, sym_link *right)
{
  if ( ic->op != '*')
  {
    return FALSE;
  }
  
  /* multiply chars in-place */
  if (getSize(left) == 1 && getSize(right) == 1)
    return TRUE;
  
  /* use library functions for more complex maths */
  return FALSE;
}

/* Indicate which extended bit operations this port supports */
static bool
hasExtBitOp (int op, int size)
{
  if (op == RRC
    || op == RLC
    /* || op == GETHBIT */ /* GETHBIT doesn't look complete for PIC */
    )
    return TRUE;
  else
    return FALSE;
}

/* Indicate the expense of an access to an output storage class */
static int
oclsExpense (struct memmap *oclass)
{
  /* The IN_FARSPACE test is compatible with historical behaviour, */
  /* but I don't think it is applicable to PIC. If so, please feel */
  /* free to remove this test -- EEP */
  if (IN_FARSPACE(oclass))
    return 1;

  return 0;
}

static void _pic14_do_link (void)
{
  hTab *linkValues=NULL;
  char lfrm[256];
  char *lcmd;
  char temp[128];
  set *tSet=NULL;
  int ret;
  char * procName;
  
  /*
   * link command format:
   * {linker} {incdirs} {lflags} -o {outfile} {spec_ofiles} {ofiles} {libs}
   *
   */

  sprintf(lfrm, "{linker} {incdirs} {sysincdirs} {lflags} -w -r -o {outfile} {user_ofile} {spec_ofiles} {ofiles} {libs}");

  shash_add(&linkValues, "linker", "gplink");

  /* LIBRARY SEARCH DIRS */
  mergeSets(&tSet, libPathsSet);
  mergeSets(&tSet, libDirsSet);
  shash_add(&linkValues, "incdirs", joinStrSet(appendStrSet(tSet, "-I\"", "\"")));

  SNPRINTF (&temp[0], 128, "%cpic\"", DIR_SEPARATOR_CHAR);
  joinStrSet(appendStrSet(libDirsSet, "-I\"", &temp[0]));
  shash_add(&linkValues, "sysincdirs", joinStrSet(appendStrSet(libDirsSet, "-I\"", &temp[0])));
  
  shash_add(&linkValues, "lflags", joinStrSet(linkOptionsSet));

  shash_add(&linkValues, "outfile", fullDstFileName ? fullDstFileName : dstFileName);

  if(fullSrcFileName) {
    sprintf(temp, "%s.o", fullDstFileName ? fullDstFileName : dstFileName );
    shash_add(&linkValues, "user_ofile", temp);
  }

  shash_add(&linkValues, "ofiles", joinStrSet(relFilesSet));

  /* LIBRARIES */
  procName = processor_base_name();
  if (!procName) {
     procName = "16f877";
  }

  addSet(&libFilesSet, "libsdcc.lib");
  SNPRINTF(&temp[0], 128, "pic%s.lib", procName);
  addSet(&libFilesSet, temp);
  shash_add(&linkValues, "libs", joinStrSet(libFilesSet));

  lcmd = msprintf(linkValues, lfrm);

  ret = my_system( lcmd );

  Safe_free( lcmd );

  if(ret)
    exit(1);
}

/* Globals */
PORT pic_port =
{
  TARGET_ID_PIC,
  "pic14",
  "MCU pic",        /* Target name */
  "",               /* Processor */
  {
    picglue,
    TRUE,     /* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
    MODEL_SMALL
  },
  {
    _asmCmd,
    NULL,
    "-g",	    /* options with --debug */
    NULL,	    /* options without --debug */
    //"-plosgffc",  /* Options with debug */
    //"-plosgff",   /* Options without debug */
    0,
    ".asm",
    NULL            /* no do_assemble function */
  },
  {
    _linkCmd,
    NULL,
    _pic14_do_link, /* own do link function */
    ".o",
    0
  },
  {
    _defaultRules
  },
  {
    /* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 2, 2, 3, 1, 4, 4
    /* TSD - I changed the size of gptr from 3 to 1. However, it should be
       2 so that we can accomodate the PIC's with 4 register banks (like the
       16f877)
     */
  },
  /* tags for generic pointers */
  { 0x00, 0x00, 0x00, 0x80 },   /* far, near, xstack, code */
  {
    "XSEG    (XDATA)",
    "STACK   (DATA)",
    "code",
    "DSEG    (DATA)",
    "ISEG    (DATA)",
    NULL, /* pdata */
    "XSEG    (XDATA)",
    "BSEG    (BIT)",
    "RSEG    (DATA)",
    "GSINIT  (CODE)",
    "udata_ovr",
    "GSFINAL (CODE)",
    "HOME	 (CODE)",
    NULL, // xidata
    NULL, // xinit
    "CONST   (CODE)",   // const_name - const data (code or not)
    "CABS    (ABS,CODE)", // cabs_name - const absolute data (code or not)
    "XABS    (ABS,XDATA)",  // xabs_name - absolute xdata
    "IABS    (ABS,DATA)", // iabs_name - absolute data
    NULL,
    NULL,
    1        // code is read only
  },
  { NULL, NULL },
  {
    +1, 1, 4, 1, 1, 0
  },
    /* pic14 has an 8 bit mul */
  {
    1, -1
  },
  {
    pic14_emitDebuggerSymbol
  },
  {
    255/3,      /* maxCount */
    3,          /* sizeofElement */
    /* The rest of these costs are bogus. They approximate */
    /* the behavior of src/SDCCicode.c 1.207 and earlier.  */
    {4,4,4},    /* sizeofMatchJump[] */
    {0,0,0},    /* sizeofRangeCompare[] */
    0,          /* sizeofSubtract */
    3,          /* sizeofDispatch */
  },
  "_",
  _pic14_init,
  _pic14_parseOptions,
  _pic14_poptions,
  _pic14_initPaths,
  _pic14_finaliseOptions,
  _pic14_setDefaultOptions,
  pic14_assignRegisters,
  _pic14_getRegName,
  _pic14_keywords,
  _pic14_genAssemblerPreamble,
  NULL,         /* no genAssemblerEnd */
  _pic14_genIVT,
  NULL, // _pic14_genXINIT
  NULL,         /* genInitStartup */
  _pic14_reset_regparm,
  _pic14_regparm,
  NULL,         /* process a pragma */
  NULL,
  _hasNativeMulFor,
  hasExtBitOp,  /* hasExtBitOp */
  oclsExpense,  /* oclsExpense */
  FALSE,
//  TRUE,       /* little endian */
  FALSE,        /* little endian - PIC code enumlates big endian */
  0,            /* leave lt */
  0,            /* leave gt */
  1,            /* transform <= to ! > */
  1,            /* transform >= to ! < */
  1,            /* transform != to !(a == b) */
  0,            /* leave == */
  FALSE,        /* No array initializer support. */
  0,            /* no CSE cost estimation yet */
  NULL,         /* no builtin functions */
  GPOINTER,     /* treat unqualified pointers as "generic" pointers */
  1,            /* reset labelKey to 1 */
  1,            /* globals & local static allowed */
  PORT_MAGIC
};

