/*-----------------------------------------------------------------*/
/* SDCCmem.h - header file for memory management                    */
/*-----------------------------------------------------------------*/

#ifndef SDCCMEM_H
#define SDCCMEM_H

struct set ;
struct value ;
typedef  struct memmap{
    unsigned char  pageno;/* page no for this variable  */
    char     *sname;    /*   character prefix for map */
    char     dbName ;   /* debugger address space name */
    int      slbl ;    /* label counter for space    */
    unsigned sloc    ;    /* starting location          */
    unsigned fmap : 1;    /* 1 = 16bit addressing reqd  */
    unsigned paged : 1;    /* this is a paged mem space  */
    unsigned direct: 1;    /* 1= indirect access only    */
    unsigned bitsp: 1;    /* 1 = bit addressable space  */
    unsigned codesp:1;    /* 1 = code space             */
    unsigned regsp: 1;    /* 1= sfr space               */
    FILE    *oFile   ;    /* object file associated     */
    struct  set *syms;   /* symbols defined in this segment */
} memmap ;

extern FILE	*junkFile ;	

/* memory map prefixes  MOF added the DATA,CODE,XDATA,BIT */
#define  XSTACK_NAME   "XSEG    (XDATA)"
#define  ISTACK_NAME   "STACK   (DATA)"
#define  CODE_NAME     "CSEG    (CODE)"
#define  DATA_NAME     "DSEG    (DATA)"
#define  IDATA_NAME    "ISEG    (DATA)"
#define  XDATA_NAME    "XSEG    (XDATA)"
#define  BIT_NAME      "BSEG    (BIT)"
#define  REG_NAME      "RSEG    (DATA)"
#define  STATIC_NAME   "GSINIT  (CODE)"
 
/* forward definition for variables */
extern   memmap  *xstack;    /* xternal stack data         */
extern   memmap  *istack;    /* internal stack             */
extern   memmap  *code  ;    /* code segment               */
extern   memmap  *data  ;    /* internal data upto 128     */
extern   memmap  *xdata ;    /* external data		   */
extern   memmap  *idata ;    /* internal data upto 256     */
extern   memmap  *bit   ;    /* bit addressable space      */
extern   memmap  *statsg;    /* static code segment        */
extern   memmap  *sfr   ;    /* register space             */
extern   memmap  *sfrbit;    /* sfr bit space              */
extern   memmap  *reg   ;    /* register space		   */
extern   memmap  *_const;    /* constant segment           */
extern   memmap  *generic;   /* unknown                    */
extern   memmap  *overlay;   /* the overlay segment        */

extern   struct  set     *ovrSetSets;

/* easy access macros */
#define IN_BITSPACE(map)	(map && map->bitsp)
#define IN_STACK(x)  (IS_SPEC(x) && (SPEC_OCLS(x) == xstack || SPEC_OCLS(x) == istack ))
#define IN_FARSPACE(map)        (map && map->fmap)
#define IN_DIRSPACE(map)        (map && map->direct)
#define IN_PAGEDSPACE(map)      (map && map->paged )
#define IN_CODESPACE(map)       (map && map->codesp)

/* forward decls for functions    */
memmap     *allocMap       (char,char,char,char,char,char,unsigned,char *,char );
void        initMem        (                                );
void        allocGlobal    (struct symbol  *                );
void        allocLocal     (struct symbol  *                );
void        allocParms     (struct value   *                );
void        deallocParms   (struct value   *                );
void        deallocLocal   (struct symbol  *                );
int         allocVariables (struct symbol  *                );
void        overlay2Set    (                                );
void        overlay2data   (                                );
void        redoStackOffsets(                               );

#endif
