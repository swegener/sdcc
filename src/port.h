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

    /* assembler related information */
    struct {
	/** Command to run and arguments (eg as-z80) */
	const char **cmd;
	/** Arguments for debug mode.  PENDING: ignored */
	const char *debug_opts;
	/** Arguments for normal assembly mode.  PENDING: ignored */
	const char *plain_opts;
    } assembler;

    /* linker related info */
    struct {
	/** Command to run (eg link-z80) */
	const char **cmd;
    } linker;

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

    /** Names for all the memory regions */
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
	
    } stack;
    struct {
	/** One more than the smallest 
	    mul/div operation the processor can do nativley 
	    Eg if the processor has an 8 bit mul, nativebelow is 2 */
	int nativebelow;
    } muldiv;

    /** Parses one option + its arguments */
    bool (*parseOption)(int *pargc, char **argv);
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
} PORT;

extern PORT *port;

#endif

