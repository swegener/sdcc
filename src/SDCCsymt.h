/*-------------------------------------------------------------------------
  SDCCsymt.h - Header file for Symbols table related structures and MACRO's.              
	      Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#ifndef  SDCCSYMT_H
#define  SDCCSYMT_H

#define MAX_NEST_LEVEL  256
#define SDCC_NAME_MAX    64
#include "SDCChasht.h"


#define HASHTAB_SIZE 256

/* hash table bucket */
typedef struct bucket
  {
    void *sym;			/* pointer to the object   */
    char name[SDCC_NAME_MAX + 1];	/* name of this symbol          */
    int level;			/* nest level for this symbol   */
    int block;			/* belongs to which block */
    struct bucket *prev;	/* ptr 2 previous bucket   */
    struct bucket *next;	/* ptr 2 next bucket       */
  }
bucket;

typedef struct structdef
  {
    char tag[SDCC_NAME_MAX + 1];	/* tag part of structure      */
    unsigned char level;	/* Nesting level         */
    struct symbol *fields;	/* pointer to fields     */
    unsigned size;		/* sizeof the table in bytes  */
  }
structdef;

/* noun definitions */
typedef enum
  {
    V_INT = 1,
    V_FLOAT,
    V_CHAR,
    V_VOID,
    V_STRUCT,
    V_LABEL,
    V_BIT,
    V_SBIT,
    V_DOUBLE
  }
NOUN;

/* storage class    */
typedef enum
  {
    S_FIXED = 0,
    S_AUTO,
    S_REGISTER,
    S_SFR,
    S_SBIT,
    S_CODE,
    S_XDATA,
    S_DATA,
    S_IDATA,
    S_PDATA,
    S_LITERAL,
    S_STACK,
    S_XSTACK,
    S_BIT,
    S_EEPROM
  }
STORAGE_CLASS;

/* specifier is the last in the type-chain */
typedef struct specifier
  {
    NOUN noun;			/* CHAR INT STRUCTURE LABEL   */
    STORAGE_CLASS sclass;	/* REGISTER,AUTO,FIX,CONSTANT */
    struct memmap *oclass;	/* output storage class       */
    unsigned _long:1;		/* 1=long            */
    unsigned _short:1;		/* 1=short int    */
    unsigned _unsigned:1;	/* 1=unsigned, 0=signed       */
    unsigned _signed:1;         /* just for sanity checks only*/
    unsigned _static:1;		/* 1=static keyword found     */
    unsigned _extern:1;		/* 1=extern found             */
    unsigned _absadr:1;		/* absolute address specfied  */
    unsigned _reent:1;		/* function is reentrant      */
    unsigned _intrtn:1;		/* this is an interrupt routin */
    unsigned _rbank:1;		/* seperate register bank     */
    unsigned _volatile:1;	/* is marked as volatile      */
    unsigned _const:1;		/* is a constant              */
    unsigned _critical:1;	/* critical function          */
    unsigned _naked:1;		/* naked function	      */
    unsigned _typedef:1;	/* is typedefed               */
    unsigned _isregparm:1;	/* is the first parameter     */
    unsigned _isenum:1;		/* is an enumerated type      */
    unsigned nonbanked:1;	/* function has the nonbanked attribute */
    unsigned banked:1;		/* function has the banked attribute */
    unsigned _IntNo;		/* 1=Interrupt svc routine    */
    short _regbank;		/* register bank 2b used      */
    unsigned _addr;		/* address of symbol          */
    unsigned _stack;		/* stack offset for stacked v */
    unsigned _bitStart;		/* bit start position         */
    int _bitLength;		/* bit length                 */

    union
      {				/* Values if constant or enum */
	int v_int;		/* int and char values        */
	char *v_char;		/* character string           */
	unsigned v_uint;	/* unsigned int const value   */
	long v_long;		/* long constant value        */
	unsigned long v_ulong;	/* unsigned long constant val */
	double v_float;		/* floating point constant value */
	struct symbol *v_enum;	/* ptr 2 enum_list if enum==1 */
      }
    const_val;
    struct structdef *v_struct;	/* structure pointer      */
  }
specifier;

/* types of declarators */
typedef enum
  {
    POINTER = 0,		/* pointer to near data */
    FPOINTER,			/* pointer to far data  */
    CPOINTER,			/* pointer to code space */
    GPOINTER,			/* _generic pointer     */
    PPOINTER,			/* paged area pointer   */
    IPOINTER,			/* pointer to upper 128 bytes */
    UPOINTER,			/* unknown pointer used only when parsing */
    EEPPOINTER,			/* pointer to eeprom     */
    ARRAY,
    FUNCTION
  }
DECLARATOR_TYPE;

typedef struct declarator
  {
    DECLARATOR_TYPE dcl_type;	/* POINTER,ARRAY or FUNCTION  */
    unsigned int num_elem;	/* # of elems if type==array  */
    short ptr_const:1;		/* pointer is constant        */
    short ptr_volatile:1;	/* pointer is volatile        */
    struct sym_link *tspec;	/* pointer type specifier      */
  }
declarator;

#define DECLARATOR   0
#define SPECIFIER    1

typedef struct sym_link
  {
    unsigned class:1;		/* DECLARATOR or SPECIFIER    */
    unsigned tdef:1;		/* current link created by    */
    /* typedef if this flag is set */
    union
      {
	specifier s;		/* if CLASS == SPECIFIER      */
	declarator d;		/* if CLASS == DECLARATOR     */
      }
    select;

    struct sym_link *next;	/* next element on the chain  */
  }
sym_link;

typedef struct symbol
  {
    char name[SDCC_NAME_MAX + 1];	/* Input Variable Name     */
    char rname[SDCC_NAME_MAX + 1];	/* internal name           */

    short level;		/* declration lev,fld offset */
    short block;		/* sequential block # of defintion */
    int key;
    unsigned fbody:1;		/* function body defined             */
    unsigned implicit:1;	/* implicit flag                     */
    unsigned undefined:1;	/* undefined variable                */
    unsigned ret:1;		/* return statement for a function   */
    unsigned hasVargs:1;	/* has a variable argument list      */
    unsigned _isparm:1;		/* is a parameter          */
    unsigned ismyparm:1;	/* is parameter of the function being generated */
    unsigned isitmp:1;		/* is an intermediate temp */
    unsigned islbl:1;		/* is a temporary label */
    unsigned isref:1;		/* has been referenced  */
    unsigned isind:1;		/* is a induction variable */
    unsigned isinvariant:1;	/* is a loop invariant  */
    unsigned isstrlit:1;	/* is a string literal  */
    unsigned cdef:1;		/* compiler defined symbol */
    unsigned allocreq:1;	/* allocation is required for this variable */
    unsigned addrtaken:1;	/* address of the symbol was taken */
    unsigned isreqv:1;		/* is the register quivalent of a symbol */
    unsigned hasFcall:1;	/* for functions does it call other functions */
    unsigned calleeSave:1;	/* for functions uses callee save paradigm */
    unsigned udChked:1;		/* use def checking has been already done */

    /* following flags are used by the backend
       for code generation and can be changed
       if a better scheme for backend is thought of */
    unsigned isLiveFcall:1;	/* is live at or across a function call */
    unsigned isspilt:1;		/* has to be spilt */
    unsigned remat:1;		/* can be remateriazed */
    unsigned isptr:1;		/* is a pointer */
    unsigned uptr:1;		/* used as a pointer */
    unsigned isFree:1;		/* used by register allocator */
    unsigned islocal:1;		/* is a local variable        */
    unsigned blockSpil:1;	/* spilt at block level       */
    unsigned remainSpil:1;	/* spilt because not used in remainder */
    unsigned stackSpil:1;	/* has been spilt on temp stack location */
    unsigned onStack:1;		/* this symbol allocated on the stack */
    unsigned iaccess:1;		/* indirect access      */
    unsigned ruonly:1;		/* used in return statement only */
    unsigned spildir:1;		/* spilt in direct space */
    unsigned ptrreg:1;		/* this symbol assigned to a ptr reg */
    unsigned noSpilLoc:1;	/* cannot be assigned a spil location */
    unsigned accuse;		/* can be left in the accumulator
				   On the Z80 accuse is devided into
				   ACCUSE_A and ACCUSE_HL as the idea
				   is quite similar.
				 */

    int stack;			/* offset on stack      */
    int xstack;			/* offset on xternal stack */
    short nRegs;		/* number of registers required */
    short regType;		/* type of register required    */

    struct regs *regs[4];	/* can have at the most 4 registers */
    struct asmop *aop;		/* asmoperand for this symbol */
    struct iCode *fuse;		/* furthest use */
    struct iCode *rematiCode;	/* rematerialse with which instruction */
    struct operand *reqv;	/* register equivalent of a local variable */
    union
      {
	struct symbol *spillLoc;	/* register spil location */
	struct set *itmpStack;	/* symbols spilt @ this stack location */
      }
    usl;
    short bitVar;		/* this is a bit variable    */
    unsigned offset;		/* offset from top if struct */

    int lineDef;		/* defined line number        */
    int lastLine;		/* for functions the last line */
    struct sym_link *type;	/* 1st link to declator chain */
    struct sym_link *etype;	/* last link to declarator chn */
    struct value *args;		/* arguments if function      */
    struct symbol *next;	/* crosslink to next symbol   */
    struct symbol *localof;	/* local variable of which function */
    struct initList *ival;	/* ptr to initializer if any  */
    struct bitVect *defs;	/* bit vector for definitions */
    struct bitVect *uses;	/* bit vector for uses        */
    struct bitVect *regsUsed;	/* for functions registers used */
    int liveFrom;		/* live from iCode sequence number */
    int liveTo;			/* live to sequence number */
    int used;			/* no. of times this was used */
    int recvSize;		/* size of first argument  */
  }
symbol;

/* Easy Access Macros */
#define DCL_TYPE(l)  l->select.d.dcl_type
#define DCL_ELEM(l)  l->select.d.num_elem
#define DCL_PTR_CONST(l) l->select.d.ptr_const
#define DCL_PTR_VOLATILE(l) l->select.d.ptr_volatile
#define DCL_TSPEC(l) l->select.d.tspec
#define SPEC_NOUN(x) x->select.s.noun
#define SPEC_LONG(x) x->select.s._long
#define SPEC_USIGN(x) x->select.s._unsigned
#define SPEC_SCLS(x) x->select.s.sclass
#define SPEC_ENUM(x) x->select.s._isenum
#define SPEC_OCLS(x) x->select.s.oclass
#define SPEC_STAT(x) x->select.s._static
#define SPEC_EXTR(x) x->select.s._extern
#define SPEC_CODE(x) x->select.s._codesg
#define SPEC_RENT(x) x->select.s._reent
#define SPEC_INTN(x) x->select.s._IntNo
#define SPEC_ABSA(x) x->select.s._absadr
#define SPEC_BANK(x) x->select.s._regbank
#define SPEC_ADDR(x) x->select.s._addr
#define SPEC_STAK(x) x->select.s._stack
#define SPEC_CVAL(x) x->select.s.const_val
#define SPEC_BSTR(x) x->select.s._bitStart
#define SPEC_BLEN(x) x->select.s._bitLength

/* Sleaze: SPEC_ISR_SAVED_BANKS is only used on 
 * function type symbols, which obviously cannot
 * be of BIT type. Therefore, we recycle the 
 * _bitStart field instead of defining a new field.
 */
#define SPEC_ISR_SAVED_BANKS(x) x->select.s._bitStart
#define SPEC_BNKF(x) x->select.s._rbank
#define SPEC_INTRTN(x) x->select.s._intrtn
#define SPEC_CRTCL(x) x->select.s._critical
#define SPEC_NAKED(x) x->select.s._naked
#define SPEC_VOLATILE(x) x->select.s._volatile
#define SPEC_CONST(x) x->select.s._const
#define SPEC_STRUCT(x) x->select.s.v_struct
#define SPEC_TYPEDEF(x) x->select.s._typedef
#define SPEC_REGPARM(x) x->select.s._isregparm
#define SPEC_NONBANKED(x) x->select.s.nonbanked
#define SPEC_BANKED(x) x->select.s.banked

/* type check macros */
#define IS_DECL(x)   ( x && x->class == DECLARATOR	)
#define IS_SPEC(x)   ( x && x->class == SPECIFIER  )
#define IS_ARRAY(x)  (IS_DECL(x) && DCL_TYPE(x) == ARRAY)
#define IS_DATA_PTR(x) (IS_DECL(x) && DCL_TYPE(x) == POINTER)
#define IS_PTR(x)    (IS_DECL(x) && (DCL_TYPE(x) == POINTER    ||    \
                                     DCL_TYPE(x) == FPOINTER   ||    \
			             DCL_TYPE(x) == GPOINTER   ||    \
			             DCL_TYPE(x) == IPOINTER   ||    \
			             DCL_TYPE(x) == PPOINTER   ||    \
			             DCL_TYPE(x) == EEPPOINTER ||    \
                                     DCL_TYPE(x) == CPOINTER   ||    \
                                     DCL_TYPE(x) == UPOINTER  ))
#define IS_PTR_CONST(x) (IS_PTR(x) && DCL_PTR_CONST(x))
#define IS_FARPTR(x) (IS_DECL(x) && DCL_TYPE(x) == FPOINTER)
#define IS_GENPTR(x) (IS_DECL(x) && DCL_TYPE(x) == GPOINTER)
#define IS_FUNC(x)   (IS_DECL(x) && DCL_TYPE(x) == FUNCTION)
#define IS_LONG(x)   (IS_SPEC(x) && x->select.s._long)
#define IS_TYPEDEF(x)(IS_SPEC(x) && x->select.s._typedef)
#define IS_CONSTANT(x)  (IS_SPEC(x) && ( x->select.s._const == 1))
#define IS_STRUCT(x) (IS_SPEC(x) && x->select.s.noun == V_STRUCT)
#define IS_ABSOLUTE(x)  (IS_SPEC(x) && x->select.s._absadr )
#define IS_REGISTER(x)  (IS_SPEC(x) && SPEC_SCLS(x) == S_REGISTER)
#define IS_RENT(x)   (IS_SPEC(x) && x->select.s._reent )
#define IS_STATIC(x) (IS_SPEC(x) && SPEC_STAT(x))
#define IS_INT(x)    (IS_SPEC(x) && x->select.s.noun == V_INT)
#define IS_VOID(x)   (IS_SPEC(x) && x->select.s.noun == V_VOID)
#define IS_CHAR(x)   (IS_SPEC(x) && x->select.s.noun == V_CHAR)
#define IS_EXTERN(x)	(IS_SPEC(x) && x->select.s._extern)
#define IS_VOLATILE(x)  (IS_SPEC(x) && x->select.s._volatile )
#define IS_INTEGRAL(x) (IS_SPEC(x) && (x->select.s.noun == V_INT ||  \
                                       x->select.s.noun == V_CHAR || \
                                       x->select.s.noun == V_BIT ||  \
                                       x->select.s.noun == V_SBIT ))
#define IS_BITFIELD(x) (IS_SPEC(x) && (x->select.s.noun == V_BIT))
#define IS_BITVAR(x) (IS_SPEC(x) && (x->select.s.noun  == V_BIT ||   \
                                     x->select.s.noun == V_SBIT ))
#define IS_FLOAT(x)  (IS_SPEC(x) && x->select.s.noun == V_FLOAT)
#define IS_ARITHMETIC(x) (IS_INTEGRAL(x) || IS_FLOAT(x))
#define IS_AGGREGATE(x) (IS_ARRAY(x) || IS_STRUCT(x))
#define IS_LITERAL(x)   (IS_SPEC(x)  && x->select.s.sclass == S_LITERAL)
#define IS_ISR(x)	(IS_SPEC(x)  && SPEC_INTRTN(x))
#define IS_REGPARM(x)   (IS_SPEC(x) && SPEC_REGPARM(x))
#define IS_NONBANKED(x) (IS_SPEC(x) && SPEC_NONBANKED(x))
#define IS_BANKED(x)	(IS_SPEC(x) && SPEC_BANKED(x))
#define IS_BANKEDCALL(x) (IS_SPEC(x) && !SPEC_NONBANKED(x) && !SPEC_STAT(x) && (options.model == MODEL_LARGE || options.model == MODEL_MEDIUM || SPEC_BANKED(x)))

/* forward declaration for the global vars */
extern bucket *SymbolTab[];
extern bucket *StructTab[];
extern bucket *TypedefTab[];
extern bucket *LabelTab[];
extern bucket *enumTab[];
extern symbol *__fsadd;
extern symbol *__fssub;
extern symbol *__fsmul;
extern symbol *__fsdiv;
extern symbol *__fseq;
extern symbol *__fsneq;
extern symbol *__fslt;
extern symbol *__fslteq;
extern symbol *__fsgt;
extern symbol *__fsgteq;

/* Dims: mul/div/mod, BYTE/WORD/DWORD, SIGNED/UNSIGNED */
extern symbol *__muldiv[3][3][2];
/* Dims: BYTE/WORD/DWORD SIGNED/UNSIGNED */
extern sym_link *__multypes[3][2];
/* Dims: to/from float, BYTE/WORD/DWORD, SIGNED/USIGNED */
extern symbol *__conv[2][3][2];
/* Dims: shift left/shift right, BYTE/WORD/DWORD, SIGNED/UNSIGNED */
symbol *__rlrr[2][3][2];

#define CHARTYPE	__multypes[0][0]
#define UCHARTYPE	__multypes[0][1]
#define INTTYPE		__multypes[1][0]
#define UINTTYPE	__multypes[1][1]
#define LONGTYPE	__multypes[2][0]
#define ULONGTYPE	__multypes[2][1]


extern sym_link *floatType;

#include "SDCCval.h"

/* forward definitions for the symbol table related functions */
void initSymt ();
symbol *newSymbol (char *, int);
sym_link *newLink ();
sym_link *newFloatLink ();
structdef *newStruct (char *);
void addDecl (symbol *, int, sym_link *);
sym_link *mergeSpec (sym_link *, sym_link *, char *name);
sym_link *cloneSpec (sym_link *);
symbol *reverseSyms (symbol *);
sym_link *reverseLink (sym_link *);
symbol *copySymbol (symbol *);
symbol *copySymbolChain (symbol *);
void printSymChain (symbol *, int);
void printStruct (structdef *, int);
char *genSymName (int);
sym_link *getSpec (sym_link *);
char *genSymName (int);
int compStructSize (int, structdef *);
sym_link *copyLinkChain (sym_link *);
int checkDecl (symbol *, int);
void checkBasic (sym_link *, sym_link *);
value *checkPointerIval (sym_link *, value *);
value *checkStructIval (symbol *, value *);
value *checkArrayIval (sym_link *, value *);
value *checkIval (sym_link *, value *);
unsigned int getSize (sym_link *);
unsigned int bitsForType (sym_link *);
sym_link *newIntLink ();
sym_link *newCharLink ();
sym_link *newLongLink ();
int compareType (sym_link *, sym_link *);
int checkFunction (symbol *);
void cleanUpLevel (bucket **, int);
void cleanUpBlock (bucket **, int);
int funcInChain (sym_link *);
void addSymChain (symbol *);
sym_link *structElemType (sym_link *, value *, value **);
symbol *getStructElement (structdef *, symbol *);
sym_link *computeType (sym_link *, sym_link *);
void processFuncArgs (symbol *, int);
int isSymbolEqual (symbol *, symbol *);
int powof2 (unsigned long);
void printTypeChain (sym_link *, FILE *);
void initCSupport ();
void pointerTypes (sym_link *, sym_link *);
void cdbTypeInfo (sym_link *, FILE *);
void cdbSymbol (symbol *, FILE *, int, int);
void cdbStructBlock (int, FILE *);
void initHashT ();
bucket *newBucket ();
void addSym (bucket **, void *, char *, int, int, int checkType);
void deleteSym (bucket **, void *, char *);
void *findSym (bucket **, void *, const char *);
void *findSymWithLevel (bucket **, struct symbol *);
void *findSymWithBlock (bucket **, struct symbol *, int);
void changePointer (symbol * sym);
void checkTypeSanity(sym_link *etype, char *name);
/* noun strings */
extern char *nounName(sym_link *);

#endif
