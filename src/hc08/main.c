/** @file main.c
    hc08 specific general functions.

    Note that mlh prepended _hc08_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "gen.h"
#include "../SDCCutil.h"

void copyFile(FILE *dest, FILE *src);
extern char * iComments2;
extern DEBUGFILE dwarf2DebugFile;
extern int dwarf2FinalizeFile(FILE *);

static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by msc51 */
static char *_hc08_keywords[] =
{
  "at",
  //"bit",
  "code",
  "critical",
  "data",
  "far",
  //"idata",
  "interrupt",
  "near",
  //"pdata",
  "reentrant",
  //"sfr",
  //"sbit",
  //"using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  //"_pdata",
  //"_idata",
  "_naked",
  "_overlay",
  NULL
};


void hc08_assignRegisters (eBBlock ** ebbs, int count);

static int regParmFlg = 0;	/* determine if we can register a parameter */

static void
_hc08_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
}

static void
_hc08_reset_regparm (void)
{
  regParmFlg = 0;
}

static int
_hc08_regparm (sym_link * l)
{
  int size = getSize(l);
    
  /* If they fit completely, the first two bytes of parameters can go */
  /* into A and X, otherwise, they go on the stack. Examples:         */
  /*   foo(char p1)                    A <- p1                        */
  /*   foo(char p1, char p2)           A <- p1, X <- p2               */
  /*   foo(char p1, char p2, char p3)  A <- p1, X <- p2, stack <- p3  */
  /*   foo(int p1)                     XA <- p1                       */
  /*   foo(long p1)                    stack <- p1                    */
  /*   foo(char p1, int p2)            A <- p1, stack <- p2           */
  /*   foo(int p1, char p2)            XA <- p1, stack <- p2          */

  if (regParmFlg>=2)
    return 0;

  if ((regParmFlg+size)>2) 
    {
      regParmFlg = 2;
      return 0;
    }

  regParmFlg += size;
  return 1+regParmFlg-size;
}

static bool
_hc08_parseOptions (int *pargc, char **argv, int *i)
{
  if (!strcmp (argv[*i], "--out-fmt-elf"))
    {
      options.out_fmt = 2;
      debugFile = &dwarf2DebugFile;
      return TRUE;
    }
    
  return FALSE;
}

static OPTION _hc08_options[] = 
  {
    {  0,   "--out-fmt-elf", NULL, "Output executable in ELF format" },
    {  0, NULL }
  };

static void
_hc08_finaliseOptions (void)
{
  if (options.noXinitOpt) {
    port->genXINIT=0;
  }

  if (options.model == MODEL_LARGE) {
      port->mem.default_local_map = xdata;
      port->mem.default_globl_map = xdata;
    }
  else
    {
      port->mem.default_local_map = data;
      port->mem.default_globl_map = data;
    }

  istack->ptrType = FPOINTER;
}

static void
_hc08_setDefaultOptions (void)
{
  options.code_loc = 0x8000;
  options.data_loc = 0x80;
  options.xdata_loc = 0;	/* 0 means immediately following data */
  options.stack_loc = 0x7fff;
  options.out_fmt = 1;		/* use motorola S19 output */

  options.ommitFramePtr = 1;	/* no frame pointer (we use SP */
                                /* offsets instead)            */
  
}

static const char *
_hc08_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}

static void
_hc08_genAssemblerPreamble (FILE * of)
{
  int i;
  int needOrg = 1;
  symbol *mainExists=newSymbol("main", 0);
  mainExists->block=0;

  fprintf (of, "\t.area %s\n",port->mem.code_name);
  fprintf (of, "\t.area GSINIT0 (CODE)\n");
  fprintf (of, "\t.area %s\n",port->mem.static_name);
  fprintf (of, "\t.area %s\n",port->mem.post_static_name);
  fprintf (of, "\t.area %s\n",port->mem.xinit_name);
  fprintf (of, "\t.area %s\n",port->mem.data_name);
  fprintf (of, "\t.area %s\n",port->mem.overlay_name);
  fprintf (of, "\t.area %s\n",port->mem.bit_name);
  fprintf (of, "\t.area %s\n",port->mem.xdata_name);
  fprintf (of, "\t.area %s\n",port->mem.xidata_name);

  if ((mainExists=findSymWithLevel(SymbolTab, mainExists)))
    {
      // generate interrupt vector table
      fprintf (of, "\t.area\tCODEIVT (ABS)\n");
      
      for (i=maxInterrupts;i>0;i--)
        {
          if (interrupts[i])
	    {
	      if (needOrg)
		{
		  fprintf (of, "\t.org\t0x%04x\n", (0xfffe - (i * 2)));
		  needOrg = 0;
		}
	      fprintf (of, "\t.dw\t%s\n", interrupts[i]->rname);
	    }
          else
	    needOrg = 1;
        }
      if (needOrg)
	fprintf (of, "\t.org\t0xfffe\n");
      fprintf (of, "\t.dw\t%s", "__sdcc_gs_init_startup\n\n");
        
      fprintf (of, "\t.area GSINIT0\n");
      fprintf (of, "__sdcc_gs_init_startup:\n");
      if (options.stack_loc)
        {
          fprintf (of, "\tldhx\t#0x%04x\n", options.stack_loc+1);
          fprintf (of, "\ttxs\n");
        }
      else
        fprintf (of, "\trsp\n");
      fprintf (of, "\tjsr\t__sdcc_external_startup\n");
      fprintf (of, "\tbeq\t__sdcc_init_data\n");
      fprintf (of, "\tjmp\t__sdcc_program_startup\n");
      fprintf (of, "__sdcc_init_data:\n");

      fprintf (of, "; _hc08_genXINIT() start\n");
      fprintf (of, "        ldhx #0\n");
      fprintf (of, "00001$:\n");
      fprintf (of, "        cphx #l_XINIT\n");
      fprintf (of, "        beq  00002$\n");
      fprintf (of, "        lda  s_XINIT,x\n");
      fprintf (of, "        sta  s_XISEG,x\n");
      fprintf (of, "        aix  #1\n");
      fprintf (of, "        bra  00001$\n");
      fprintf (of, "00002$:\n");
      fprintf (of, "; _hc08_genXINIT() end\n");

      fprintf (of, "\t.area GSFINAL\n");
      fprintf (of, "\tjmp\t__sdcc_program_startup\n\n");

      fprintf (of, "\t.area CSEG\n");
      fprintf (of, "__sdcc_program_startup:\n");
      fprintf (of, "\tjsr\t_main\n");
      fprintf (of, "\tbra\t.\n");
      
    }
}

static void
_hc08_genAssemblerEnd (FILE * of)
{
  if (options.out_fmt == 2 && options.debug)
    {
      dwarf2FinalizeFile (of);
    }
}

static void
_hc08_genExtraAreas (FILE * asmFile, bool mainExists)
{
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; extended address mode data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, xdata->oFile);
}


/* Generate interrupt vector table. */
static int
_hc08_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
  int i;
  
  fprintf (of, "\t.area\tCODEIVT (ABS)\n");
  fprintf (of, "\t.org\t0x%04x\n",
    (0xfffe - (maxInterrupts * 2)));
  
  for (i=maxInterrupts;i>0;i--)
    {
      if (interrupts[i])
        fprintf (of, "\t.dw\t%s\n", interrupts[i]->rname);
      else
        fprintf (of, "\t.dw\t0xffff\n");
    }
  fprintf (of, "\t.dw\t%s", "__sdcc_gs_init_startup\n");
        
  return TRUE;
}

/* Generate code to copy XINIT to XISEG */
static void _hc08_genXINIT (FILE * of) {
  fprintf (of, ";	_hc08_genXINIT() start\n");
  fprintf (of, ";	_hc08_genXINIT() end\n");
}


/* Do CSE estimation */
static bool cseCostEstimation (iCode *ic, iCode *pdic)
{
    operand *result = IC_RESULT(ic);
    sym_link *result_type = operandType(result);

    return 0; /* disable CSE */
    
    /* if it is a pointer then return ok for now */
    if (IC_RESULT(ic) && IS_PTR(result_type)) return 1;
    
    if (ic->op == ADDRESS_OF)
      return 0;
    
    /* if bitwise | add & subtract then no since hc08 is pretty good at it 
       so we will cse only if they are local (i.e. both ic & pdic belong to
       the same basic block */
    if (IS_BITWISE_OP(ic) || ic->op == '+' || ic->op == '-') {
	/* then if they are the same Basic block then ok */
	if (ic->eBBlockNum == pdic->eBBlockNum) return 1;
	else return 0;
    }
	
    /* for others it is cheaper to do the cse */
    return 1;
}


/* Indicate which extended bit operations this port supports */
static bool
hasExtBitOp (int op, int size)
{
  if (op == RRC
      || op == RLC
      || op == GETHBIT
      || (op == SWAP && size <= 2)
     )
    return TRUE;
  else
    return FALSE;
}

/* Indicate the expense of an access to an output storage class */
static int
oclsExpense (struct memmap *oclass)
{
  /* The hc08's addressing modes allow access to all storage classes */
  /* inexpensively (<=0) */
  
  if (IN_DIRSPACE (oclass))	/* direct addressing mode is fastest */
    return -2;
  if (IN_FARSPACE (oclass))	/* extended addressing mode is almost at fast */
    return -1;
  if (oclass == istack) /* stack is the slowest, but still faster than */
    return 0;		/* trying to copy to a temp location elsewhere */
  
  return 0; /* anything we missed */
}


/*----------------------------------------------------------------------*/
/* hc08_dwarfRegNum - return the DWARF register number for a register.  */
/*   These are defined for the HC08 in "Motorola 8- and 16-bit Embedded */
/*   Application Binary Interface (M8/16EABI)"                          */
/*----------------------------------------------------------------------*/
static int
hc08_dwarfRegNum (regs * reg)
{
  switch (reg->rIdx)
    {
    case A_IDX: return 0;
    case H_IDX: return 1;
    case X_IDX: return 2;
    case CND_IDX: return 17;
    case SP_IDX: return 15;
    }
  return -1;
}



/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
  "link-hc08", "-nf", "\"$1\"", NULL
};

/* $3 is replaced by assembler.debug_opts resp. port->assembler.plain_opts */
static const char *_asmCmd[] =
{
  "as-hc08", "$l", "$3", "\"$1.asm\"", NULL
};

/* Globals */
PORT hc08_port =
{
  TARGET_ID_HC08,
  "hc08",
  "HC08",			/* Target name */
  NULL,				/* Processor name */
  {
    glue,
    FALSE,			/* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE,
    MODEL_LARGE
  },
  {
    _asmCmd,
    NULL,
    "-plosgffc",		/* Options with debug */
    "-plosgff",			/* Options without debug */
    0,
    ".asm",
    NULL			/* no do_assemble function */
  },
  {
    _linkCmd,
    NULL,
    NULL,
    ".rel",
    1
  },
  {
    _defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 2, 2, 2, 1, 4, 4
  },
  {
    "XSEG",
    "STACK",
    "CSEG (CODE)",
    "DSEG",
    NULL, /* "ISEG" */
    NULL, /* "PSEG" */
    "XSEG",
    "BSEG",
    "RSEG",
    "GSINIT (CODE)",
    "OSEG    (OVR)",
    "GSFINAL (CODE)",
    "HOME (CODE)",
    "XISEG", // initialized xdata
    "XINIT", // a code copy of xiseg
    NULL,
    NULL,
    1
  },
  { _hc08_genExtraAreas,
    NULL },
  {
    -1,		/* direction (-1 = stack grows down) */
    0,		/* bank_overhead (switch between register banks) */
    4,		/* isr_overhead */
    2,		/* call_overhead */
    0,		/* reent_overhead */
    0		/* banked_overhead (switch between code banks) */
  },
    /* hc08 has an 8 bit mul */
  {
    1, -1
  },
  {
    hc08_emitDebuggerSymbol,
    {
      hc08_dwarfRegNum,
      NULL,
      NULL,
      4,				/* addressSize */
      14,			/* regNumRet */
      15,			/* regNumSP */
      -1,			/* regNumBP */
      1,			/* offsetSP */
    },
  },
  {
    256,        /* maxCount */
    2,          /* sizeofElement */
    {8,16,32},  /* sizeofMatchJump[] */
    {8,16,32},  /* sizeofRangeCompare[] */
    5,          /* sizeofSubtract */
    10,         /* sizeofDispatch */
  },
  "_",
  _hc08_init,
  _hc08_parseOptions,
  _hc08_options,
  NULL,
  _hc08_finaliseOptions,
  _hc08_setDefaultOptions,
  hc08_assignRegisters,
  _hc08_getRegName,
  _hc08_keywords,
  _hc08_genAssemblerPreamble,
  _hc08_genAssemblerEnd,	/* no genAssemblerEnd */
  _hc08_genIVT,
  _hc08_genXINIT,
  NULL, 			/* genInitStartup */
  _hc08_reset_regparm,
  _hc08_regparm,
  NULL,				/* process_pragma */
  NULL,				/* getMangledFunctionName */
  NULL,				/* hasNativeMulFor */
  hasExtBitOp,			/* hasExtBitOp */
  oclsExpense,			/* oclsExpense */
  TRUE,				/* use_dw_for_init */
  FALSE,			/* little endian */
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  FALSE,                        /* No array initializer support. */
  cseCostEstimation,
  NULL, 			/* no builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};
