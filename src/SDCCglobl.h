/* SDCCglobl.h - global macros etc required by all files */

#ifndef SDCCGLOBL_H
#define SDCCGLOBL_H

#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>

#include "SDCCset.h"


/*
 * Define host port dependant constants etc.
 */

#define UNIX_DIR_SEPARATOR_CHAR    '/'

#ifdef _WIN32       /* WIN32 native */

#  define NATIVE_WIN32          1
#  ifdef __MINGW32__  /* GCC MINGW32 depends on configure */
#    include "sdccconf.h"
#  else
#    include "sdcc_vc.h"
#    define PATH_MAX  _MAX_PATH
#  endif

#else               /* Assume Un*x style system */
#  include "sdccconf.h"
#endif

#include "SDCCerr.h"

#define SPACE ' '
#define ZERO  0

#include <limits.h>             /* PATH_MAX                  */
#if !defined(PATH_MAX) || (PATH_MAX < 2048)
#  undef  PATH_MAX
#  define PATH_MAX 2048         /* define a reasonable value */
#endif

#define  MAX_REG_PARMS  1
typedef int bool;

#ifndef max
#  define max(a,b) (a > b ? a : b)
#endif

#ifndef min
#  define min(a,b) (a < b ? a : b)
#endif

#ifndef THROWS
#define THROWS
#define THROW_NONE  0
#define THROW_SRC   1
#define THROW_DEST  2
#define THROW_BOTH  3
#endif

/* size's in bytes  */
#define CHARSIZE    port->s.char_size
#define SHORTSIZE   port->s.short_size
#define INTSIZE     port->s.int_size
#define LONGSIZE    port->s.long_size
#define PTRSIZE     port->s.ptr_size
#define FPTRSIZE    port->s.fptr_size
#define GPTRSIZE    port->s.gptr_size
#define BITSIZE     port->s.bit_size
#define FLOATSIZE   port->s.float_size
#define MAXBASESIZE port->s.max_base_size

#define  SMALL_MODEL 0
#define  LARGE_MODEL 1
#define  TRUE 1
#define  FALSE 0

#define MAX_TVAR 6
#define INITIAL_INLINEASM 4*1024
#define DEFPOOLSTACK(type,size)     \
    type       *type##Pool        ; \
    type *type##FreeStack [size]  ; \
    int   type##StackPtr = 0      ;

#define PUSH(x,y)   x##FreeStack[x##StackPtr++] = y
#define PEEK(x)     x##FreeStack[x##StackPtr-1]
#define POP(type)   type##FreeStack[--type##StackPtr]
/* #define POP(x)    (x##StackPtr ? x##FreeStack[--x##StackPtr] :       \
   (assert(x##StackPtr),0)) */
#ifdef UNIX
#define EMPTY(x)        (x##StackPtr <= 1 ? 1 : 0)
#else
#define EMPTY(x)        (x##StackPtr == 0 ? 1 : 0)
#endif


#define COPYTYPE(start,end,from) (end = getSpec (start = from))


/* general purpose stack related macros */
#define  STACK_DCL(stack,type,size)                                   \
         typedef  type  t_##stack   ;                                 \
         t_##stack   stack[size]    ;                                 \
         t_##stack   (*p_##stack) = stack;

/* define extern stack */
#define EXTERN_STACK_DCL(stack,type,size)                             \
        typedef type t_##stack     ;                                  \
        extern t_##stack stack[size] ;                                \
        extern t_##stack *p_##stack;

#define  STACK_EMPTY(stack)     ((p_##stack) <= stack )
#define  STACK_FULL(stack)      ((p_##stack) >= (stack +              \
                                sizeof(stack)/sizeof(*stack))         )

#define  STACK_PUSH_(stack, x)  (*++p_##stack = (x))
#define  STACK_POP_(stack)      (*p_##stack--)

#define  STACK_PUSH(stack, x)   (STACK_FULL(stack)                    \
                                ? (STACK_ERR(1, stack), *p_##stack)   \
                                : STACK_PUSH_(stack,x)                )

#define  STACK_POP(stack)       (STACK_EMPTY(stack)                   \
                                ? (STACK_ERR(-1, stack), *p_##stack)  \
                                : STACK_POP_(stack)                   )

#define  STACK_PEEK(stack)      (STACK_EMPTY(stack)                   \
                                ? (STACK_ERR(0, stack), *p_##stack)   \
                                : *p_##stack                          )

#define  STACK_ERR(o, stack)    (fatal(1, E_STACK_VIOLATION, #stack,  \
                                        (o < 0)                       \
                                        ? "underflow"                 \
                                        : (o > 0)                     \
                                          ? "overflow"                \
                                          : "empty"))

/* optimization options */
/*
 * cloneOptimize function in SDCC.lex should be updated every time
 * a new set is added to the optimize structure!
 */
struct optimize
  {
    int global_cse;
    int ptrArithmetic;
    int label1;
    int label2;
    int label3;
    int label4;
    int loopInvariant;
    int loopInduction;
    int noJTabBoundary;
    int noLoopReverse;
    int codeSpeed;
    int codeSize;
  };

/** Build model.
    Used in options.model.A bit each as port.supported_models is an OR
    of these.
*/
enum
  {
    MODEL_SMALL = 1,
    MODEL_COMPACT = 2,
    MODEL_MEDIUM = 4,
    MODEL_LARGE = 8,
    MODEL_FLAT24 = 16,
    MODEL_PAGE0 = 32, /* for the xa51 port */
    MODEL_HUGE = 64 /* for banked support */
  };

/* overlay segment name and the functions
   that belong to it. used by pragma overlay */
typedef struct {
    char *osname;       /* overlay segment name */
    int  nfuncs;        /* number of functions in this overlay */
    char *funcs[128];   /* function name that belong to this */
} olay;

/* other command line options */
/*
 * cloneOptions function in SDCC.lex should be updated every time
 * a new set is added to the options structure!
 */
struct options
  {
    int model;                  /* see MODEL_* defines above */
    int stackAuto;              /* Stack Automatic  */
    int useXstack;              /* use Xternal Stack */
    int stack10bit;             /* use 10 bit stack (flat24 model only) */
    int dump_raw;               /* dump after intermediate code generation */
    int dump_gcse;              /* dump after gcse */
    int dump_loop;              /* dump after loop optimizations */
    int dump_kill;              /* dump after dead code elimination */
    int dump_range;             /* dump after live range analysis */
    int dump_pack;              /* dump after register packing */
    int dump_rassgn;            /* dump after register assignment */
    int dump_tree;              /* dump front-end tree before lowering to iCode */
    int cc_only;                /* compile only flag              */
    int intlong_rent;           /* integer & long support routines reentrant */
    int float_rent;             /* floating point routines are reentrant */
    int out_fmt;                /* 1 = motorola S19 format 0 = intel Hex format */
    int cyclomatic;             /* print cyclomatic information */
    int noOverlay;              /* don't overlay local variables & parameters */
    int mainreturn;             /* issue a return after main */
    int xram_movc;              /* use movc instead of movx to read xram (mcs51) */
    int nopeep;                 /* no peep hole optimization */
    int asmpeep;                /* pass inline assembler thru peep hole */
    int debug;                  /* generate extra debug info */
    int c1mode;                 /* Act like c1 - no pre-proc, asm or link */
    char *peep_file;            /* additional rules for peep hole */
    int nostdlib;               /* Don't use standard lib files */
    int nostdinc;               /* Don't use standard include files */
    int noRegParams;            /* Disable passing some parameters in registers */
    int verbose;                /* Show what the compiler is doing */
    int shortis8bits;           /* treat short like int or char */
    int lessPedantic;           /* disable some warnings */
    int profile;                /* Turn on extra profiling information */
    int ommitFramePtr;          /* Turn off the frame pointer. */
    int useAccelerator;         /* use ds390 Arithmetic Accelerator */
    int noiv;                   /* do not generate irq vector table entries */
    int all_callee_saves;       /* callee saves for all functions */
    int stack_probe;            /* insert call to function __stack_probe */
    int tini_libid;             /* library ID for TINI */
    int protect_sp_update;      /* DS390 - will disable interrupts during ESP:SP updates */
    int parms_in_bank1;         /* DS390 - use reg bank1 to pass parameters */
    int stack_size;             /* MCS51/DS390 - Tells the linker to allocate this space for stack */
    int no_pack_iram;           /* MCS51/DS390 - Tells the linker not to pack variables in internal ram */
    /* starting address of the segments */
    int xstack_loc;             /* initial location of external stack */
    int stack_loc;              /* initial value of internal stack pointer */
    int xdata_loc;              /* xternal ram starts at address */
    int data_loc;               /* interram start location       */
    int idata_loc;              /* indirect address space        */
    int code_loc;               /* code location start           */
    int iram_size;              /* internal ram size (used only for error checking) */
    int xram_size;              /* external ram size (used only for error checking) */
    bool xram_size_set;         /* since xram_size=0 is a possibility */
    int code_size;              /* code size (used only for error checking) */
    int verboseExec;            /* show what we are doing */
    int noXinitOpt;             /* don't optimize initialized xdata */
    int noCcodeInAsm;           /* hide c-code from asm */
    int iCodeInAsm;             /* show i-code in asm */
    int noPeepComments;         /* hide peephole optimizer comments */
    int printSearchDirs;        /* display the directories in the compiler's search path */
    int vc_err_style;           /* errors and warnings are compatible with Micro$oft visual studio */
    int use_stdout;             /* send errors to stdout instead of stderr */
    int no_std_crt0;            /* for the z80/gbz80 do not link default crt0.o*/
    int std_c99;                /* enable C99 keywords/constructs */
    int std_sdcc;               /* enable SDCC extensions to C */
    const char *code_seg;       /* segment name to use instead of CSEG */
    const char *const_seg;      /* segment name to use instead of CONST */
    /* sets */
    set *calleeSavesSet;        /* list of functions using callee save */
    set *excludeRegsSet;        /* registers excluded from saving */
/*  set *olaysSet;               * not implemented yet: overlay segments used in #pragma OVERLAY */
  };

/* forward definition for variables accessed globally */
extern int noAssemble;          /* no assembly, stop after code generation */
extern char *yytext;
extern char *currFname;
extern char *fullSrcFileName;   /* full name for the source file; */
                                /* can be NULL while linking without compiling */
extern char *fullDstFileName;   /* full name for the output file; */
                                /* only given by -o, otherwise NULL */
extern char *dstFileName;       /* destination file name without extension */
extern char *dstPath;           /* path for the output files; */
                                /* "" is equivalent with cwd */
extern char *moduleName;        /* module name is source file without path and extension */
                                /* can be NULL while linking without compiling */
extern int seqPointNo;          /* current sequence point */
extern int currLineno;          /* current line number    */
extern int mylineno;            /* line number of the current file SDCC.lex */
extern FILE *yyin;              /* */
extern FILE *asmFile;           /* assembly output file */
extern FILE *cdbFile;           /* debugger symbol file */
extern int NestLevel;           /* NestLevel                 SDCC.y   */
extern int stackPtr;            /* stack pointer             SDCC.y   */
extern int xstackPtr;           /* external stack pointer    SDCC.y   */
extern int reentrant;           /* /X flag has been sent     SDCC.y */
extern char buffer[PATH_MAX * 2];/* general buffer           SDCCmain.c   */
extern int currRegBank;         /* register bank being used  SDCCgens.c   */
extern int RegBankUsed[4];      /* JCF: register banks used  SDCCmain.c   */
extern int BitBankUsed;         /* MB: overlayable bit bank  SDCCmain.c   */
extern struct symbol *currFunc; /* current function    SDCCgens.c */
extern int cNestLevel;          /* block nest level  SDCCval.c      */
extern int currBlockno;         /* sequentail block number */
extern struct optimize optimize;
extern struct options options;
extern unsigned maxInterrupts;
extern int ignoreTypedefType;

/* Visible from SDCCmain.c */
extern set *preArgvSet;
extern set *relFilesSet;
extern set *libFilesSet;
extern set *libPathsSet;
extern set *libDirsSet;         /* list of lib search directories */

void setParseWithComma (set **, char *);

/** Creates a temporary file a'la tmpfile which avoids the bugs
    in cygwin wrt c:\tmp.
    Scans, in order: TMP, TEMP, TMPDIR, else uses tmpfile().
*/
FILE *tempfile (void);

/** Creates a temporary file name a'la tmpnam which avoids the bugs
    in cygwin wrt c:\tmp.
    Scans, in order: TMP, TEMP, TMPDIR, else uses tmpfile().
*/
char *tempfilename (void);

/** An assert() macro that will go out through sdcc's error
    system.
*/
#define wassertl(a,s)   ((a) ? 0 : \
        (werror (E_INTERNAL_ERROR,__FILE__,__LINE__, s), 0))

#define wassert(a)    wassertl(a,"code generator internal error")

#define DUMP_RAW0 1
#define DUMP_RAW1 DUMP_RAW0+1
#define DUMP_CSE DUMP_RAW1+1
#define DUMP_DFLOW DUMP_CSE+1
#define DUMP_GCSE DUMP_DFLOW+1
#define DUMP_DEADCODE DUMP_GCSE+1
#define DUMP_LOOP DUMP_DEADCODE+1
#define DUMP_LOOPG DUMP_LOOP+1
#define DUMP_LOOPD DUMP_LOOPG+1
#define DUMP_RANGE DUMP_LOOPD+1
#define DUMP_PACK DUMP_RANGE+1
#define DUMP_RASSGN DUMP_PACK+1
#define DUMP_LRANGE DUMP_RASSGN+1

struct _dumpFiles {
  int id;
  char *ext;
  FILE *filePtr;
};

extern struct _dumpFiles dumpFiles[];

/* Buffer which can be used to hold a file name; assume it will
 * be trashed by any function call within SDCC.
 */
extern char scratchFileName[PATH_MAX];

/* Define well known filenos if the system does not define them.  */
#ifndef STDIN_FILENO
# define STDIN_FILENO   0
#endif
#ifndef STDOUT_FILENO
# define STDOUT_FILENO  1
#endif
#ifndef STDERR_FILENO
# define STDERR_FILENO  2
#endif

#endif
