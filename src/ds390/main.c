/** @file main.c
    ds390 specific general functions.

    Note that mlh prepended _ds390_ on the static functions.  Makes
    it easier to set a breakpoint using the debugger.
*/
#include "common.h"
#include "main.h"
#include "ralloc.h"
#include "gen.h"
#include "BuildCmd.h"
#include "MySystem.h"
#include "../SDCCutil.h"
extern const char *preArgv[128];	/* pre-processor arguments  */
static char _defaultRules[] =
{
#include "peeph.rul"
};

/* list of key words used by msc51 */
static char *_ds390_keywords[] =
{
  "at",
  "bit",
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
  "sbit",
  "using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  "_pdata",
  "_idata",
  "_naked",
  NULL
};

static builtins __ds390_builtins[] = {
    { "__builtin_memcpy_x2x","v",3,{"cx*","cx*","i"}}, /* void __builtin_memcpy_x2x (xdata char *,xdata char *,int) */
    { "__builtin_memcpy_c2x","v",3,{"cx*","cp*","i"}}, /* void __builtin_memcpy_c2x (xdata char *,code  char *,int) */
    { "__builtin_memset_x","v",3,{"cx*","c","i"}},     /* void __builtin_memset     (xdata char *,char,int) 	    */
    /* __builtin_inp - used to read from a memory mapped port, increment first pointer */
    { "__builtin_inp","v",3,{"cx*","cx*","i"}},        /* void __builtin_inp        (xdata char *,xdata char *,int) */
    /* __builtin_inp - used to write to a memory mapped port, increment first pointer */
    { "__builtin_outp","v",3,{"cx*","cx*","i"}},       /* void __builtin_outp       (xdata char *,xdata char *,int) */
    { "__builtin_swapw","us",1,{"us"}},		       /* unsigned short __builtin_swapw (unsigned short) */
    { "__builtin_memcmp_x2x","c",3,{"cx*","cx*","i"}}, /* void __builtin_memcmp_x2x (xdata char *,xdata char *,int) */
    { "__builtin_memcmp_c2x","c",3,{"cx*","cp*","i"}}, /* void __builtin_memcmp_c2x (xdata char *,code  char *,int) */
    { NULL , NULL,0, {NULL}} 			   /* mark end of table */
};    
void ds390_assignRegisters (eBBlock ** ebbs, int count);

static int regParmFlg = 0;	/* determine if we can register a parameter */

static void
_ds390_init (void)
{
  asm_addTree (&asm_asxxxx_mapping);
}

static void
_ds390_reset_regparm ()
{
  regParmFlg = 0;
}

static int
_ds390_regparm (sym_link * l)
{

    if (options.parms_in_bank1 == 0) {
	/* simple can pass only the first parameter in a register */
	if (regParmFlg)
	    return 0;

	regParmFlg = 1;
	return 1;
    } else {
	int size = getSize(l);
	int remain ;

	/* first one goes the usual way to DPTR */
	if (regParmFlg == 0) {
	    regParmFlg += 4 ;
	    return 1;
	}
	/* second one onwards goes to RB1_0 thru RB1_7 */
        remain = regParmFlg - 4;
	if (size > (8 - remain)) {
	    regParmFlg = 12 ;
	    return 0;
	}
	regParmFlg += size ;
	return regParmFlg - size + 1;	
    }
}

static bool
_ds390_parseOptions (int *pargc, char **argv, int *i)
{
  /* TODO: allow port-specific command line options to specify
   * segment names here.
   */
  return FALSE;
}

static void
_ds390_finaliseOptions (void)
{
  if (options.noXinitOpt) {
    port->genXINIT=0;
  }

  /* Hack-o-matic: if we are using the flat24 model,
   * adjust pointer sizes.
   */
  if (options.model != MODEL_FLAT24)  {
      fprintf (stderr,
	       "*** warning: ds390 port small and large model experimental.\n");
      if (options.model == MODEL_LARGE)
      {
        port->mem.default_local_map = xdata;
        port->mem.default_globl_map = xdata;
      }
      else
      {
        port->mem.default_local_map = data;
        port->mem.default_globl_map = data;
      }
  }
  else {
    port->s.fptr_size = 3;
    port->s.gptr_size = 4;

    port->stack.isr_overhead += 2;	/* Will save dpx on ISR entry. */

    port->stack.call_overhead += 2;	/* This acounts for the extra byte 
				 * of return addres on the stack.
				 * but is ugly. There must be a 
				 * better way.
				 */

    port->mem.default_local_map = xdata;
    port->mem.default_globl_map = xdata;

    if (!options.stack10bit)
    {
    fprintf (stderr,
	     "*** error: ds390 port only supports the 10 bit stack mode.\n");
    } else {
	if (!options.stack_loc) options.stack_loc = 0x400008;
    }
    
    /* generate native code 16*16 mul/div */
    if (options.useAccelerator) 
	    port->support.muldiv=2;
    else 
	    port->support.muldiv=1;

     /* Fixup the memory map for the stack; it is now in
     * far space and requires a FPOINTER to access it.
     */
    istack->fmap = 1;
    istack->ptrType = FPOINTER;

    if (options.parms_in_bank1) {
	addToList (preArgv, "-DSDCC_PARMS_IN_BANK1");
    }
  }  /* MODEL_FLAT24 */
}

static void
_ds390_setDefaultOptions (void)
{
  options.model=MODEL_FLAT24;
  options.stack10bit=1;
}

static const char *
_ds390_getRegName (struct regs *reg)
{
  if (reg)
    return reg->name;
  return "err";
}

static void
_ds390_genAssemblerPreamble (FILE * of)
{
      if (options.model == MODEL_FLAT24)
        fputs (".flat24 on\t\t; 24 bit flat addressing\n", of);

      fputs ("dpx = 0x93\t\t; dpx register unknown to assembler\n", of);
      fputs ("dps = 0x86\t\t; dps register unknown to assembler\n", of);
      fputs ("dpl1 = 0x84\t\t; dpl1 register unknown to assembler\n", of);
      fputs ("dph1 = 0x85\t\t; dph1 register unknown to assembler\n", of);
      fputs ("dpx1 = 0x95\t\t; dpx1 register unknown to assembler\n", of);
      fputs ("ap = 0x9C\t\t; ap register unknown to assembler\n", of);
      fputs ("_ap = 0x9C\t\t; _ap register unknown to assembler\n", of);
      fputs ("mcnt0 = 0xD1\t\t; mcnt0 register unknown to assembler\n", of);
      fputs ("mcnt1 = 0xD2\t\t; mcnt1 register unknown to assembler\n", of);
      fputs ("ma = 0xD3\t\t; ma register unknown to assembler\n", of);
      fputs ("mb = 0xD4\t\t; mb register unknown to assembler\n", of);
      fputs ("mc = 0xD5\t\t; mc register unknown to assembler\n", of);
      fputs ("F1 = 0xD1\t\t; F1 user flag unknown to assembler\n", of);
      fputs ("esp = 0x9B\t\t; ESP user flag unknown to assembler\n", of);
      if (options.parms_in_bank1) {
	  int i ;
	  for (i=0; i < 8 ; i++ )
	      fprintf (of,"b1_%d = 0x%x \n",i,8+i);
      }
}

/* Generate interrupt vector table. */
static int
_ds390_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts)
{
  int i;

  if (options.model != MODEL_FLAT24)
    {
      /* Let the default code handle it. */
      return FALSE;
    }

  fprintf (of, "\tajmp\t__reset_vect\n");

  /* now for the other interrupts */
  for (i = 0; i < maxInterrupts; i++)
    {
      if (interrupts[i])
	{
	  fprintf (of, "\tljmp\t%s\n\t.ds\t4\n", interrupts[i]->rname);
	}
      else
	{
	  fprintf (of, "\treti\n\t.ds\t7\n");
	}
    }

  fprintf (of, "__reset_vect:\n\tljmp\t__sdcc_gsinit_startup\n");

  return TRUE;
}

/* Generate code to copy XINIT to XISEG */
static void _ds390_genXINIT (FILE * of) {
  fprintf (of, ";	_ds390_genXINIT() start\n");
  fprintf (of, "	mov	a,#l_XINIT\n");
  fprintf (of, "	orl	a,#l_XINIT>>8\n");
  fprintf (of, "	jz	00003$\n");
  fprintf (of, "	mov	a,#s_XINIT\n");
  fprintf (of, "	add	a,#l_XINIT\n");
  fprintf (of, "	mov	r1,a\n");
  fprintf (of, "	mov	a,#s_XINIT>>8\n");
  fprintf (of, "	addc	a,#l_XINIT>>8\n");
  fprintf (of, "	mov	r2,a\n");
  fprintf (of, "	mov	dptr,#s_XINIT\n");
  fprintf (of, "	mov	dps,#0x21\n");
  fprintf (of, "	mov	dptr,#s_XISEG\n");
  fprintf (of, "00001$:	clr	a\n");
  fprintf (of, "	movc	a,@a+dptr\n");
  fprintf (of, "	movx	@dptr,a\n");
  fprintf (of, "	inc	dptr\n");
  fprintf (of, "	inc	dptr\n");
  fprintf (of, "00002$:	mov	a,dpl\n");
  fprintf (of, "	cjne	a,ar1,00001$\n");
  fprintf (of, "	mov	a,dph\n");
  fprintf (of, "	cjne	a,ar2,00001$\n");
  fprintf (of, "	mov	dps,#0\n");
  fprintf (of, "00003$:\n");
  fprintf (of, ";	_ds390_genXINIT() end\n");
}

/* Do CSE estimation */
static bool cseCostEstimation (iCode *ic, iCode *pdic)
{
    operand *result = IC_RESULT(ic);
    //operand *right  = IC_RIGHT(ic);
    //operand *left   = IC_LEFT(ic);
    sym_link *result_type = operandType(result);
    //sym_link *right_type  = (right ? operandType(right) : 0);
    //sym_link *left_type   = (left  ? operandType(left)  : 0);
    
    /* if it is a pointer then return ok for now */
    if (IC_RESULT(ic) && IS_PTR(result_type)) return 1;
    
    /* if bitwise | add & subtract then no since mcs51 is pretty good at it 
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
/** $1 is always the basename.
    $2 is always the output file.
    $3 varies
    $l is the list of extra options that should be there somewhere...
    MUST be terminated with a NULL.
*/
static const char *_linkCmd[] =
{
  "aslink", "-nf", "$1", NULL
};

/* $3 is replaced by assembler.debug_opts resp. port->assembler.plain_opts */   static const char *_asmCmd[] =
{
  "asx8051", "$l", "$3", "$1.asm", NULL
};

/* Globals */
PORT ds390_port =
{
  TARGET_ID_DS390,
  "ds390",
  "DS80C390",			/* Target name */
  NULL,
  {
    TRUE,			/* Emit glue around main */
    MODEL_SMALL | MODEL_LARGE | MODEL_FLAT24,
    MODEL_SMALL
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
    ".rel"
  },
  {
    _defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 1, 2, 3, 1, 4, 4
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
    "HOME    (CODE)",
    "XISEG   (XDATA)", // initialized xdata
    "XINIT   (CODE)", // a code copy of xiseg
    NULL,
    NULL,
    1
  },
  {
    +1, 1, 4, 1, 1, 0
  },
    /* ds390 has an 16 bit mul & div */
  {
    2, -1
  },
  "_",
  _ds390_init,
  _ds390_parseOptions,
  NULL,
  _ds390_finaliseOptions,
  _ds390_setDefaultOptions,
  ds390_assignRegisters,
  _ds390_getRegName,
  _ds390_keywords,
  _ds390_genAssemblerPreamble,
  NULL,				/* no genAssemblerEnd */
  _ds390_genIVT,
  _ds390_genXINIT,
  _ds390_reset_regparm,
  _ds390_regparm,
  NULL,
  NULL,
  NULL,
  FALSE,
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  TRUE,                         /* we support array initializers. */
  cseCostEstimation,
  __ds390_builtins,             /* table of builtin functions */
  GPOINTER,			/* treat unqualified pointers as "generic" pointers */
  1,				/* reset labelKey to 1 */
  1,				/* globals & local static allowed */
  PORT_MAGIC
};

/*---------------------------------------------------------------------------------*/
/*  				 TININative specific 			   	   */
/*---------------------------------------------------------------------------------*/
/* Globals */
static void _tininative_init (void)
{
    asm_addTree (&asm_a390_mapping);
}

static void _tininative_setDefaultOptions (void)
{
    options.model=MODEL_FLAT24;
    options.stack10bit=1;
    options.stackAuto = 1;
}

static void _tininative_finaliseOptions (void)
{
    /* Hack-o-matic: if we are using the flat24 model,
     * adjust pointer sizes.
     */
    if (options.model != MODEL_FLAT24)  {
	options.model = MODEL_FLAT24 ;
	fprintf(stderr,"TININative supports only MODEL FLAT24\n");
    }
    port->s.fptr_size = 3;
    port->s.gptr_size = 4;
    
    port->stack.isr_overhead += 2;	/* Will save dpx on ISR entry. */
    
    port->stack.call_overhead += 2;	/* This acounts for the extra byte 
					 * of return addres on the stack.
					 * but is ugly. There must be a 
					 * better way.
					 */
    
    port->mem.default_local_map = xdata;
    port->mem.default_globl_map = xdata;
    
    if (!options.stack10bit) {
	options.stack10bit = 1;
	fprintf(stderr,"TININative supports only stack10bit \n");
    }
    
    if (!options.stack_loc) options.stack_loc = 0x400008;
    
    /* generate native code 16*16 mul/div */
    if (options.useAccelerator) 
	port->support.muldiv=2;
    else 
	port->support.muldiv=1;
    
    /* Fixup the memory map for the stack; it is now in
     * far space and requires a FPOINTER to access it.
     */
    istack->fmap = 1;
    istack->ptrType = FPOINTER;
    options.cc_only =1;
}

static int _tininative_genIVT (FILE * of, symbol ** interrupts, int maxInterrupts) 
{
    return 1;
}
static void _tininative_genAssemblerPreamble (FILE * of)
{
    fputs("$include(tini.inc)\n", of);
    fputs("$include(ds80c390.inc)\n", of);
    fputs("$include(tinimacro.inc)\n", of);
    fputs("$include(apiequ.inc)\n", of);
    fputs("_bpx EQU 01Eh \t\t; _bpx (frame pointer) mapped to R8_B3:R7_B3\n", of);
    fputs("_ap  EQU 01Dh \t\t; _ap mapped to R6_B3\n", of);
    /* Must be first and return 0 */
    fputs("Lib_Native_Init:\n",of);
    fputs("\tclr\ta\n",of);
    fputs("\tret\n",of);
    fputs("LibraryID:\n",of);
    fputs("\tdb \"DS\"\n",of);
    if (options.tini_libid) {
	fprintf(of,"\tdb 0,0,0%02xh,0%02xh,0%02xh,0%02xh\n",
		(options.tini_libid>>24 & 0xff),
		(options.tini_libid>>16 & 0xff),
		(options.tini_libid>>8 & 0xff),
		(options.tini_libid  & 0xff));
    } else {
	fprintf(of,"\tdb 0,0,0,0,0,1\n");
    }

}
static void _tininative_genAssemblerEnd (FILE * of)
{
    fputs("\tend\n",of);
}
/* tininative assembler , calls "macro", if it succeeds calls "a390" */
static void _tininative_do_assemble (set *asmOptions)
{
    static const char *macroCmd[] = {
	"macro","$1.a51",NULL
    };
    static const char *a390Cmd[] = {
	"a390","$1.mpp",NULL
    };
    char buffer[100];

    buildCmdLine(buffer,macroCmd,dstFileName,NULL,NULL,NULL);
    if (my_system(buffer)) {
	exit(1);
    }
    buildCmdLine(buffer,a390Cmd,dstFileName,NULL,NULL,asmOptions);
    if (my_system(buffer)) {
	exit(1);
    }    
}

/* list of key words used by TININative */
static char *_tininative_keywords[] =
{
  "at",
  "bit",
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
  "sbit",
  "using",
  "xdata",
  "_data",
  "_code",
  "_generic",
  "_near",
  "_xdata",
  "_pdata",
  "_idata",
  "_naked",
  "_JavaNative",
  NULL
};

static builtins __tininative_builtins[] = {
    { "__builtin_memcpy_x2x","v",3,{"cx*","cx*","i"}}, /* void __builtin_memcpy_x2x (xdata char *,xdata char *,int) */
    { "__builtin_memcpy_c2x","v",3,{"cx*","cp*","i"}}, /* void __builtin_memcpy_c2x (xdata char *,code  char *,int) */
    { "__builtin_memset_x","v",3,{"cx*","c","i"}},     /* void __builtin_memset     (xdata char *,char,int) 	    */
    /* TINI NatLib */
    { "NatLib_LoadByte","c",1,{"c"}},                  /* char  Natlib_LoadByte  (0 based parameter number)         */
    { "NatLib_LoadShort","s",1,{"c"}},                 /* short Natlib_LoadShort (0 based parameter number)         */
    { "NatLib_LoadInt","l",1,{"c"}},                   /* long  Natlib_LoadLong  (0 based parameter number)         */
    { "NatLib_LoadPointer","cx*",1,{"c"}},             /* long  Natlib_LoadPointer  (0 based parameter number)      */
    /* TINI StateBlock related */
    { "NatLib_InstallImmutableStateBlock","c",2,{"vx*","us"}},/* char NatLib_InstallImmutableStateBlock(state block *,int handle) */
    { "NatLib_InstallEphemeralStateBlock","c",2,{"vx*","us"}},/* char NatLib_InstallEphemeralStateBlock(state block *,int handle) */
    { "NatLib_RemoveImmutableStateBlock","v",0,{NULL}},/* void NatLib_RemoveImmutableStateBlock() */
    { "NatLib_RemoveEphemeralStateBlock","v",0,{NULL}},/* void NatLib_RemoveEphemeralStateBlock() */
    { "NatLib_GetImmutableStateBlock","i",0,{NULL}},   /* int  NatLib_GetImmutableStateBlock () */
    { "NatLib_GetEphemeralStateBlock","i",0,{NULL}},   /* int  NatLib_GetEphemeralStateBlock () */
    /* Memory manager */
    { "MM_XMalloc","i",1,{"l"}},                       /* int  MM_XMalloc (long)                */
    { "MM_Malloc","i",1,{"i"}},                        /* int  MM_Malloc  (int)                 */
    { "MM_ApplicationMalloc","i",1,{"i"}},             /* int  MM_ApplicationMalloc  (int)      */
    { "MM_Free","i",1,{"i"}},                          /* int  MM_Free  (int)      		*/
    { "MM_Deref","cx*",1,{"i"}},                       /* char *MM_Free  (int)      		*/
    { "MM_UnrestrictedPersist","c",1,{"i"}},           /* char  MM_UnrestrictedPersist  (int)   */
    /* System functions */
    { "System_ExecJavaProcess","c",2,{"cx*","i"}},     /* char System_ExecJavaProcess (char *,int) */
    { "System_GetRTCRegisters","v",1,{"cx*"}},	       /* void System_GetRTCRegisters (char *) */
    { "System_SetRTCRegisters","v",1,{"cx*"}},	       /* void System_SetRTCRegisters (char *) */
    { "System_ThreadSleep","v",2,{"l","c"}},           /* void System_ThreadSleep (long,char)  */
    { "System_ThreadSleep_ExitCriticalSection","v",2,{"l","c"}},/* void System_ThreadSleep_ExitCriticalSection (long,char)  */
    { "System_ProcessSleep","v",2,{"l","c"}},           /* void System_ProcessSleep (long,char)  */
    { "System_ProcessSleep_ExitCriticalSection","v",2,{"l","c"}},/* void System_ProcessSleep_ExitCriticalSection (long,char)  */
    { "System_ThreadResume","c",2,{"c","c"}},          /* char System_ThreadResume(char,char)  */
    { "System_SaveJavaThreadState","v",0,{NULL}},      /* void System_SaveJavaThreadState()    */
    { "System_RestoreJavaThreadState","v",0,{NULL}},   /* void System_RestoreJavaThreadState() */
    { "System_ProcessYield","v",0,{NULL}},             /* void System_ProcessYield() */
    { "System_ProcessSuspend","v",0,{NULL}},           /* void System_ProcessSuspend() */
    { "System_ProcessResume","v",1,{"c"}},             /* void System_ProcessResume(char) */
    { "System_RegisterPoll","c",1,{"vF*"}},            /* char System_RegisterPoll ((void *func pointer)()) */
    { "System_RemovePoll","c",1,{"vF*"}},              /* char System_RemovePoll ((void *func pointer)()) */
    { "System_GetCurrentProcessId","c",0,{NULL}},      /* char System_GetCurrentProcessId() */
    { "System_GetCurrentThreadId","c",0,{NULL}},       /* char System_GetCurrentThreadId() */
    { NULL , NULL,0, {NULL}} 			   /* mark end of table */
};    

static const char *_a390Cmd[] =
{
  "macro", "$l", "$3", "$1.a51", NULL
};
PORT tininative_port =
{
  TARGET_ID_DS390,
  "TININative",
  "DS80C390",			/* Target name */
	NULL,			/* processor */
  {
    FALSE,			/* Emit glue around main */
    MODEL_FLAT24,
    MODEL_FLAT24
  },
  {
    _a390Cmd,
    NULL,
    "-l",		/* Options with debug */
    "-l",		/* Options without debug */
    0,
    ".a51",
    _tininative_do_assemble
  },
  {
    NULL,
    NULL,
    NULL,
    ".tlib",
  },
  {
    _defaultRules
  },
  {
	/* Sizes: char, short, int, long, ptr, fptr, gptr, bit, float, max */
    1, 2, 2, 4, 1, 3, 3, 1, 4, 4
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
    NULL,
    NULL,
    1
  },
  {
    +1, 1, 4, 1, 1, 0
  },
    /* ds390 has an 16 bit mul & div */
  {
    2, -1
  },
  "",
  _tininative_init,
  _ds390_parseOptions,
  NULL,
  _tininative_finaliseOptions,
  _tininative_setDefaultOptions,
  ds390_assignRegisters,
  _ds390_getRegName,
  _tininative_keywords,
  _tininative_genAssemblerPreamble,
  _tininative_genAssemblerEnd,
  _tininative_genIVT,
  NULL,
  _ds390_reset_regparm,
  _ds390_regparm,
  NULL,
  NULL,
  NULL,
  FALSE,
  0,				/* leave lt */
  0,				/* leave gt */
  1,				/* transform <= to ! > */
  1,				/* transform >= to ! < */
  1,				/* transform != to !(a == b) */
  0,				/* leave == */
  TRUE,                         /* we support array initializers. */
  cseCostEstimation,
  __tininative_builtins,        /* table of builtin functions */
  FPOINTER,			/* treat unqualified pointers as far pointers */
  0,				/* DONOT reset labelKey */
  0,				/* globals & local static NOT allowed */
  PORT_MAGIC
};
