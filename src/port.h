/** @file port.h
    Definitions for what a port must provide.
    All ports are referenced in SDCCmain.c.
*/
#ifndef PORT_INCLUDE
#define PORT_INCLUDE

/* Processor specific names */
typedef struct {
    /** Target name used for -m */
    const char *target;

    /** Target name string, used for --help */
    const char *target_name;

    struct {
	/** TRUE if all types of glue functions should be inseted into
	    the file that also defines main.
	    We dont want this in cases like the z80 where the startup
	    code is provided by a seperate module.
	*/
	bool glue_up_main;
	/* OR of MODEL_* */
	int supported_models;
	int default_model;
    } general;

    /* assembler related information */
    struct {
	/** Command to run and arguments (eg as-z80) */
	const char **cmd;
	/** Arguments for debug mode.  PENDING: ignored */
	const char *debug_opts;
	/** Arguments for normal assembly mode.  PENDING: ignored */
	const char *plain_opts;
	/* print externs as global */
	int externGlobal;
    } assembler;

    /* linker related info */
    struct {
	/** Command to run (eg link-z80) */
	const char **cmd;
	/** If non-null will be used to execute the link. */
	void (*do_link)(void);
	/** Extention for object files (.rel, .obj, ...) */
	const char *rel_ext;
    } linker;

    struct {
	/** Default peephole rules */
	char *default_rules;
    } peep;

    /** Basic type sizes */
    struct {
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
    } s;

    /** memory regions related stuff */
    struct {
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
	struct memmap *default_local_map ; /* default location for auto vars */
	struct memmap *default_globl_map ; /* default location for globl vars*/
	int         code_ro;               /* code space read-only 1=yes */
    } mem;
    
    /* stack related information */
    struct {
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
    } stack;

    struct {
	/** One more than the smallest 
	    mul/div operation the processor can do nativley 
	    Eg if the processor has an 8 bit mul, nativebelow is 2 */
	int native_below;
	/** The mul/div/mod functions will be made to use regparams
	    for sizeof(param) < log2(force_reg)
	    i.e. Use 2 for WORD and BYTE, 0 for none. */
	int force_reg_param_below;
    } muldiv;

    /** Prefix to add to a C function (eg "_") */
    const char *fun_prefix;

    /** Called once the processor target has been selected.
	First chance to initalise and set any port specific varibles.
	'port' is set before calling this.  May be NULL.
    */
    void (*init)(void);
    /** Parses one option + its arguments */
    bool (*parseOption)(int *pargc, char **argv, int *i);
    /** Called after all the options have been parsed. */
    void (*finaliseOptions)(void);
    /** Called after the port has been selected but before any
	options are parsed. */
    void (*setDefaultOptions)(void);
    /** Does the dirty work. */
    void (*assignRegisters)(eBBlock **, int);
    
    /** Returns the register name of a symbol.
	Used so that 'regs' can be an incomplete type. */
    const char *(*getRegName)(struct regs *reg);

    /* list of keywords that are used by this
       target (used by lexer) */
    char **keywords; 
    
    /* Write any port specific assembler output. */
    void (*genAssemblerPreamble)(FILE *of);
    
    /* Write the port specific IVT. If genIVT is NULL or if
     * it returns zero, default (8051) IVT generation code
     * will be used. 
     */
    int (*genIVT)(FILE *of, symbol **intTable, int intCount); 


    /* parameter passing in register related functions */
    void (*reset_regparms)();          /* reset the register count */
    int  (*reg_parm)(struct link *);   /* will return 1 if can be passed in register */

    /** Process the pragma string 'sz'.  Returns 0 if recognised and
	processed, 1 otherwise.  May be NULL.
    */
    int (*process_pragma)(const char *sz);

    /** If TRUE, then tprintf and !dw will be used for some initalisers
     */
    bool use_dw_for_init;
} PORT;

extern PORT *port;

#endif
