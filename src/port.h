/** @file port.h
    Definitions for what a port must provide.
    All ports are referenced in SDCCmain.c.
*/
#ifndef PORT_INCLUDE
#define PORT_INCLUDE

#include "SDCCicode.h"
#include "SDCCargs.h"

#define TARGET_ID_MCS51    1
#define TARGET_ID_GBZ80    2
#define TARGET_ID_Z80      3
#define TARGET_ID_AVR      4
#define TARGET_ID_DS390    5
#define TARGET_ID_PIC      6
#define TARGET_ID_PIC16	   7
#define TARGET_ID_XA51     9
#define TARGET_ID_DS400	   10

/* Macro to test the target we are compiling for.
   Can only be used after SDCCmain has defined the port
*/
#define TARGET_IS_MCS51 (port->id==TARGET_ID_MCS51)
#define TARGET_IS_GBZ80 (port->id==TARGET_ID_GBZ80)
#define TARGET_IS_Z80 (port->id==TARGET_ID_Z80)
#define TARGET_IS_AVR (port->id==TARGET_ID_AVR)
#define TARGET_IS_DS390 (port->id==TARGET_ID_DS390)
#define TARGET_IS_DS400 (port->id==TARGET_ID_DS400)
#define TARGET_IS_PIC   (port->id==TARGET_ID_PIC)
#define TARGET_IS_PIC16	(port->id==TARGET_ID_PIC16)
#define TARGET_IS_XA51 (port->id==TARGET_ID_XA51)

#define MAX_BUILTIN_ARGS	16
/* definition of builtin functions */
typedef struct builtins
{
    char *name ;		/* name of builtin function */
    char *rtype;		/* return type as string : see typefromStr */
    int  nParms;		/* number of parms : max 8 */
    char *parm_types[MAX_BUILTIN_ARGS]; /* each parm type as string : see typeFromStr */
} builtins ;

/* Processor specific names */
typedef struct
  {
/** Unique id for this target */
    const int id;
/** Target name used for -m */
    const char *target;

/** Target name string, used for --help */
    const char *target_name;

/** Specific processor for the given target family. specified by -p */
    char *processor;

    struct
      {
	/** TRUE if all types of glue functions should be inserted into
	    the file that also defines main.
	    We dont want this in cases like the z80 where the startup
	    code is provided by a seperate module.
	*/
	bool glue_up_main;
	/* OR of MODEL_* */
	int supported_models;
	int default_model;
      }
    general;

    /* assembler related information */
    struct
      {
        /** Command to run and arguments (eg as-z80) */
        const char **cmd;
        /** Alternate macro based form. */
        const char *mcmd;
        /** Arguments for debug mode. */
	const char *debug_opts;
        /** Arguments for normal assembly mode. */
	const char *plain_opts;
	/* print externs as global */
	int externGlobal;
	/* assembler file extension */
	const char *file_ext;
        /** If non-null will be used to execute the assembler. */
	void (*do_assemble) (set *);	
      }
    assembler;

    /* linker related info */
    struct
      {
        /** Command to run (eg link-z80) */
	const char **cmd;
        /** Alternate macro based form. */
        const char *mcmd;
        /** If non-null will be used to execute the link. */
	void (*do_link) (void);
        /** Extension for object files (.rel, .obj, ...) */
	const char *rel_ext;
      }
    linker;

    struct
      {
/** Default peephole rules */
	char *default_rules;
      }
    peep;

/** Basic type sizes */
    struct
      {
	int char_size;
	int short_size;
	int int_size;
	int long_size;
	int ptr_size;
	int fptr_size;
	int gptr_size;
	int bit_size;
	int float_size;
	int max_base_size;
      }
    s;

/** memory regions related stuff */
    struct
      {
	const char *xstack_name;
	const char *istack_name;
	const char *code_name;
	const char *data_name;
	const char *idata_name;
	const char *xdata_name;
	const char *bit_name;
	const char *reg_name;
	const char *static_name;
	const char *overlay_name;
	const char *post_static_name;
	const char *home_name;
	const char *xidata_name; // initialized xdata
	const char *xinit_name; // a code copy of xidata
	struct memmap *default_local_map; // default location for auto vars
	struct memmap *default_globl_map; // default location for globl vars
	int code_ro;		/* code space read-only 1=yes */
      }
    mem;

    struct
      {
	  void (*genExtraAreaDeclaration)(FILE *, bool);
	  void (*genExtraAreaLinkOptions)(FILE *);
      }
    extraAreas;
      
    /* stack related information */
    struct
      {
/** -1 for grows down (z80), +1 for grows up (mcs51) */
	int direction;
/** Extra overhead when calling between banks */
	int bank_overhead;
/** Extra overhead when the function is an ISR */
	int isr_overhead;
/** Standard overhead for a function call */
	int call_overhead;
/** Re-enterant space */
	int reent_overhead;
	/** 'banked' call overhead.
	    Mild overlap with bank_overhead */
	int banked_overhead;
      }
    stack;

    struct
      {
	/** One more than the smallest 
	    mul/div operation the processor can do nativley 
	    Eg if the processor has an 8 bit mul, nativebelow is 2 */
	unsigned muldiv;
        unsigned shift;
      }
    support;

/** Prefix to add to a C function (eg "_") */
    const char *fun_prefix;

    /** Called once the processor target has been selected.
	First chance to initalise and set any port specific variables.
	'port' is set before calling this.  May be NULL.
    */
    void (*init) (void);
/** Parses one option + its arguments */
      bool (*parseOption) (int *pargc, char **argv, int *i);
/** Optional list of automatically parsed options.  Should be
    implemented to at least show the help text correctly. */
    OPTION *poptions;
/** Called after all the options have been parsed. */
    void (*finaliseOptions) (void);
    /** Called after the port has been selected but before any
	options are parsed. */
    void (*setDefaultOptions) (void);
/** Does the dirty work. */
    void (*assignRegisters) (struct eBBlock **, int);

    /** Returns the register name of a symbol.
	Used so that 'regs' can be an incomplete type. */
    const char *(*getRegName) (struct regs * reg);

    /* list of keywords that are used by this
       target (used by lexer) */
    char **keywords;

    /* Write any port specific assembler output. */
    void (*genAssemblerPreamble) (FILE * of);
      /* invoked at end assembler file */  
    void (*genAssemblerEnd) (FILE * of);

    /* Write the port specific IVT. If genIVT is NULL or if
     * it returns zero, default (8051) IVT generation code
     * will be used. 
     */
    int (*genIVT) (FILE * of, symbol ** intTable, int intCount);

    void (*genXINIT) (FILE * of);

    /* parameter passing in register related functions */
    void (*reset_regparms) ();	/* reset the register count */
    int (*reg_parm) (struct sym_link *);	/* will return 1 if can be passed in register */

    /** Process the pragma string 'sz'.  Returns 0 if recognised and
	processed, 1 otherwise.  May be NULL.
    */
    int (*process_pragma) (const char *sz);

    /** Mangles a support function name to reflect the calling model. 
     */
    char *(*getMangledFunctionName) (char *szOrginial);

    /** Returns true if the port can multiply the two types nativly
        without using support functions.
    */
    bool (*hasNativeMulFor) (iCode *ic, sym_link *left, sym_link *right);

    /** If TRUE, then tprintf and !dw will be used for some initalisers
     */
    bool use_dw_for_init;

    /* condition transformations */
    bool lt_nge;		/* transform (a < b)  to !(a >= b)  */
    bool gt_nle;		/* transform (a > b)  to !(a <= b)  */
    bool le_ngt;		/* transform (a <= b) to !(a > b)   */
    bool ge_nlt;		/* transform (a >= b) to !(a < b)   */
    bool ne_neq;		/* transform a != b --> ! (a == b)  */
    bool eq_nne;		/* transform a == b --> ! (a != b)  */

    bool arrayInitializerSuppported;  
    bool (*cseOk) (iCode *ic, iCode *pdic);
    builtins *builtintable;     /* table of builtin functions */
    int unqualified_pointer;	/* unqualified pointers type is  */    
    int reset_labelKey  ;	/* reset Label no 1 at the start of a function */
    int globals_allowed ;       /* global & static locals not allowed ?  0 ONLY TININative*/
#define PORT_MAGIC 0xAC32
/** Used at runtime to detect if this structure has been completly filled in. */
    int magic;
  }
PORT;

extern PORT *port;

#if !OPT_DISABLE_MCS51
extern PORT mcs51_port;
#endif
#if !OPT_DISABLE_GBZ80
extern PORT gbz80_port;
#endif
#if !OPT_DISABLE_Z80
extern PORT z80_port;
#endif
#if !OPT_DISABLE_AVR
extern PORT avr_port;
#endif
#if !OPT_DISABLE_DS390
extern PORT ds390_port;
#endif
#if !OPT_DISABLE_PIC
extern PORT pic_port;
#endif
#if !OPT_DISABLE_PIC16
extern PORT pic16_port;
#endif
#if !OPT_DISABLE_TININative
extern PORT tininative_port;
#endif
#if !OPT_DISABLE_XA51
extern PORT xa51_port;
#endif
#if !OPT_DISABLE_DS400
extern PORT ds400_port;
#endif

#endif /* PORT_INCLUDE*/
