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
	/** Command to run (eg as-z80) */
	const char *exec_name;
	/** Arguments for debug mode */
	const char *debug_opts;
	/** Arguments for normal assembly mode */
	const char *plain_opts;
	/** TRUE if the output file name should be pre-pended to the args */
	bool requires_output_name;
    } assembler;
    struct {
	/** Command to run (eg link-z80) */
	const char *exec_name;
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
    struct {
	/** -1 for grows down (z80), +1 for grows up (mcs51) */
	int direction;
	/** Extra overhead when calling between banks */
	int bank_overhead;
	/** Extra overhead when the function is an ISR */
	int isr_overhead;
	/** Standard overhead for a function call */
	int call_overhead;
	/** Initial SP offset */
	int start_sp;
    } stack;
    struct {
	/** One more than the smallest mul/div operation the processor can do nativley 
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
} PORT;

extern PORT *port;

#endif

