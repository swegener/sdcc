#define SIZE8 0
#define SIZE16 1
#define SIZE32 2
#define UNKNOWN -1
#define WORD_REG 16384
#define BYTE_REG 32768

/* max # of bytes in db directive */
#define MAX_DB          2500

/* max # char in symbol name */
#define MAX_SYMBOL      1024

/* max # of bytes per line */
#define MAX_LINE	4096

/* REL() computes branch operand from dest and memory */
/* location of the jump instruction itself */
/* this is later adjusted by one for jcu, of course */
#define BRANCH_SPACING 2
#define REL(dest, mem) (((dest)-((((mem)+1)/(\
BRANCH_SPACING))*(BRANCH_SPACING)))/(BRANCH_SPACING))

#define NOP_OPCODE 0	/* opcode for NOP */

/* a linked list of all the symbols */

struct symbol {
        char *name;
        int value;
        int istarget;   /* 1 if a branch target, 0 otherwise */
        int isdef;      /* 1 if defined, 0 if no value yet */
        int line_def;   /* line in which is was defined */
        int isbit;      /* 1 if a bit address, 0 otherwise */
	int isreg;	/* 1 if a register, 0 otehrwise */
        struct symbol *next; };

/* a list of all the symbols that are branch targets */
/* (and will need to get aligned on 4 byte boundries) */

struct target {
        char *name;
        struct target *next; };

struct area_struct {
	int alloc_position;
};

extern int current_area;

#define MEM_POS (area[current_area].alloc_position)

#define AREA_CSEG	0
#define AREA_DSEG	1
#define AREA_OSEG	2
#define AREA_ISEG	3
#define AREA_BSEG	4
#define AREA_XSEG	5
#define AREA_XISEG	6
#define AREA_GSINIT	7
#define AREA_GSFINAL	8
#define AREA_HOME	9
#define NUM_AREAS	10

extern struct area_struct area[NUM_AREAS];

extern FILE *yyin;
extern char *yytext;
extern int lineno;
extern int p1, p2, p3, mem, m_len;

extern struct symbol * build_sym_list(char *thename);
extern int assign_value(char *thename, int thevalue);
extern int mk_bit(char *thename);
extern int mk_reg(char *thename);
extern void out(int *byte_list, int num);
extern int is_target(char *thename);
extern void pad_with_nop();
extern int binary2int(char *str);
extern int is_def(char *thename);
extern int get_value(char *thename);

