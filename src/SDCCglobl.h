/* SDCCglobl.h - global macros etc required by all files */
#ifndef SDCCGLOBL_H
#define SDCCGLOBL_H
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#include "sdccconf.h"
#include "SDCCerr.h"

#ifdef _NO_GC

#define GC_malloc malloc
#define GC_free free
#define GC_realloc realloc
#define GC_malloc_atomic malloc
#else

#include "./gc/gc.h" 

#endif

#define SPACE ' '
#define ZERO  0

#define  MAX_FNAME_LEN  128
#define  MAX_REG_PARMS  1
typedef int bool;

#ifndef max
#define max(a,b) (a > b ? a : b)
#endif

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif

#ifndef THROWS
#define THROWS
#define THROW_NONE  0 
#define THROW_SRC   1 
#define THROW_DEST  2
#define THROW_BOTH  3
#endif

/* size's in bytes  */
#define CHARSIZE    1
#define SHORTSIZE   1
#define INTSIZE     2
#define LONGSIZE    4
#define PTRSIZE     1
#define FPTRSIZE    2
#define GPTRSIZE    3
#define BITSIZE     1
#define FLOATSIZE   4
#define MAXBASESIZE 4


#define PRAGMA_SAVE        "SAVE"
#define PRAGMA_RESTORE     "RESTORE"
#define PRAGMA_NOINDUCTION "NOINDUCTION"
#define PRAGMA_NOINVARIANT "NOINVARIANT"
#define PRAGMA_NOLOOPREV   "NOLOOPREVERSE"
#define PRAGMA_INDUCTION   "INDUCTION"
#define PRAGMA_STACKAUTO   "STACKAUTO"
#define PRAGMA_NOJTBOUND   "NOJTBOUND"
#define PRAGMA_NOGCSE      "NOGCSE"
#define PRAGMA_NOOVERLAY   "NOOVERLAY"
#define PRAGMA_CALLEESAVES "CALLEE-SAVES"
#define PRAGMA_EXCLUDE     "EXCLUDE"
#define PRAGMA_NOREGPARMS  "NOREGPARMS"
#define  SMALL_MODEL 0
#define  LARGE_MODEL 1
#define  TRUE 1
#define  FALSE 0

#define MAX_TVAR 6
#define MAX_INLINEASM 4*1024
#define DEFPOOLSTACK(type,size)     \
    type       *type##Pool        ; \
    type *type##FreeStack [size]  ; \
    int   type##StackPtr = 0      ;

#define ALLOCTYPE(type,size) if (!(type##Pool = GC_malloc(size*sizeof(type))))   \
         {                                                            \
            werror(E_OUT_OF_MEM,__FILE__,(long) size);				 \
            exit (1);                                                \
         }

#define PUSH(x,y)   x##FreeStack[x##StackPtr++] = y
#define PEEK(x)     x##FreeStack[x##StackPtr-1]
#define POP(type)   type##FreeStack[--type##StackPtr]   
/* #define POP(x)    (x##StackPtr ? x##FreeStack[--x##StackPtr] :       \
                  (assert(x##StackPtr),0)) */
#ifdef UNIX
#define EMPTY(x)        (x##StackPtr <= 1 ? 1 : 0)
#else
#define EMPTY(x)	(x##StackPtr == 0 ? 1 : 0)
#endif

#define  ALLOC(x,sz) if (!(x = GC_malloc(sz)))      \
         {                                          \
            werror(E_OUT_OF_MEM,__FILE__,(long) sz);\
            exit (1);                               \
         }

#define ALLOC_ATOMIC(x,sz)   if (!(x = GC_malloc_atomic(sz)))   \
         {                                               \
            werror(E_OUT_OF_MEM,__FILE__,(long) sz);     \
            exit (1);                                    \
         }

#define COPYTYPE(start,end,from) (end = getSpec (start = from))


/* generalpurpose stack related macros */
#define  STACK_DCL(stack,type,size)                   \
         typedef  type  t_##stack   ;                 \
         t_##stack   stack[size]    ;                 \
         t_##stack   (*p_##stack) = stack + (size);   \

/* define extern stack */
#define EXTERN_STACK_DCL(stack,type,size)             \
        typedef type t_##stack     ;                  \
        extern t_##stack stack[size] ;                \
        extern t_##stack *p_##stack; 

#define  STACK_FULL(stack)    ((p_##stack) <= stack )
#define  STACK_EMPTY(stack)   ((p_##stack) >= (stack +      \
                              sizeof(stack)/sizeof(*stack)) )  

#define  STACK_PUSH_(stack,x) (*--p_##stack = (x))
#define  STACK_POP_(stack)    (*p_##stack++)

#define  STACK_PUSH(stack,x)  (STACK_FULL(stack)                  \
                              ?((t_##stack)(long)(STACK_ERR(1)))  \
                              : STACK_PUSH_(stack,x)              )

#define  STACK_POP(stack)     (STACK_EMPTY(stack)                 \
                              ?((t_##stack)(long)(STACK_ERR(0)))  \
                              : STACK_POP_(stack)                 )

#define  STACK_PEEK(stack)    (STACK_EMPTY(stack)                 \
                              ?((t_##stack) NULL)                 \
                              : *p_##stack                        )

#define  STACK_ERR(o)         ( o                                 \
                              ? fprintf(stderr,"stack Overflow\n")\
                              : fprintf(stderr,"stack underflow\n"))

/* optimization options */
struct optimize {
    unsigned    global_cse : 1  ;
    unsigned    ptrArithmetic :1;
    unsigned    label1 : 1      ;
    unsigned    label2 : 1     ;
    unsigned    label3 : 1     ;
    unsigned    label4 : 1     ;   
    unsigned    loopInvariant: 1;
    unsigned    loopInduction: 1;
    unsigned    noJTabBoundary:1;
    unsigned    noLoopReverse :1;
} ;

/* other command line options */
struct options {
    int model  : 1     ; /* LARGE == 1 */
    int stackAuto : 3  ; /* Stack Automatic  */
    int useXstack : 3  ; /* use Xternal Stack */
    int genericPtr: 1  ; /* use generic pointers */
    int regExtend : 1  ; /* don't use extended registers */
    int dump_raw  : 1  ; /* dump after intermediate code generation */
    int dump_gcse : 1  ; /* dump after gcse */
    int dump_loop : 1  ; /* dump after loop optimizations */
    int dump_kill : 1  ; /* dump after dead code elimination */
    int dump_range: 1  ; /* dump after live range analysis */
    int dump_pack : 1  ; /* dump after register packing */
    int dump_rassgn:1  ; /* dump after register assignment */
    int cc_only   : 1  ; /* compile only flag              */
    int intlong_rent:1 ; /* integer & long support routines reentrant */
    int float_rent: 1  ; /* floating point routines are reentrant */
    int out_fmt   : 1  ; /* 1 = motorola S19 format 0 = intel Hex format */
    int cyclomatic: 1  ; /* print cyclomatic information */
    int noOverlay : 1  ; /* don't overlay local variables & parameters */
    int mainreturn: 1  ; /* issue a return after main */
    int nopeep    : 1  ; /* no peep hole optimization */
    int asmpeep   : 1  ; /* pass inline assembler thru peep hole */
    int debug     : 1  ; /* generate extra debug info */
    int stackOnData:1  ; /* stack after data segment  */
    int noregparms: 1  ; /* do not pass parameters in registers */
    char *peep_file    ; /* additional rules for peep hole */    

    char *calleeSaves[128]; /* list of functions using callee save */
    char *excludeRegs[32] ; /* registers excluded from saving */

    /* starting address of the segments */
    int xstack_loc     ; /* initial location of external stack */
    int stack_loc      ; /* initial value of internal stack pointer */
    int xdata_loc      ; /* xternal ram starts at address */
    int data_loc       ; /* interram start location       */
    int idata_loc      ; /* indirect address space        */
    int code_loc       ; /* code location start           */    
    int iram_size      ; /* internal ram size (used only for error checking) */
} ;

/* forward definition for variables accessed globally */
extern char *currFname ;
extern char *srcFileName; /* source file name without the extenstion */
extern char *moduleName ; /* source file name without path & extension */
extern int currLineno ;	  /* current line number    */
extern int yylineno  ;    /* line number of the current file SDCC.lex */
extern FILE *yyin    ;	  /* */
extern FILE *asmFile ;    /* assembly output file */
extern FILE *cdbFile ;    /* debugger symbol file */
extern int NestLevel ;    /* NestLevel		       SDCC.y	*/
extern int stackPtr  ;    /* stack pointer             SDCC.y   */
extern int xstackPtr ;    /* external stack pointer    SDCC.y   */
extern int reentrant ;    /* /X flag has been sent     SDCC.y */
extern char	buffer[]    ;/* general buffer	    SDCCgen.c	*/
extern int currRegBank;   /* register bank being used   SDCCgens.c   */
extern struct symbol  *currFunc;   /* current function    SDCCgens.c */
extern int	cNestLevel;	 /* block nest level  SDCCval.c	     */
extern int      currBlockno;     /* sequentail block number */
extern struct optimize optimize ;
extern struct options options;
extern int maxInterrupts;
void parseWithComma (char **,char *) ;

#endif
