/*-------------------------------------------------------------------------

  glue.c - glues everything we have done together into one file.
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

#include "../common.h"
#include <time.h>
#include "ralloc.h"
#include "pcode.h"
#include "newalloc.h"
#include "gen.h"
#include "device.h"
#include "main.h"
#include <string.h>

#include <string.h>


#ifdef WORDS_BIGENDIAN
  #define _ENDIAN(x)  (3-x)
#else
  #define _ENDIAN(x)  (x)
#endif

#define BYTE_IN_LONG(x,b) ((x>>(8*_ENDIAN(b)))&0xff)

extern symbol *interrupts[256];
void pic16_printIval (symbol * sym, sym_link * type, initList * ilist, char ptype, void *p);
extern int noAlloc;
extern set *publics;
extern set *externs;
extern unsigned maxInterrupts;
extern int maxRegBank;
extern symbol *mainf;
extern char *VersionString;
extern FILE *codeOutFile;
extern set *tmpfileSet;
extern set *tmpfileNameSet;
extern char *iComments1;
extern char *iComments2;
//extern void emitStaticSeg (memmap * map);

extern int initsfpnt;

set *rel_idataSymSet=NULL;
set *fix_idataSymSet=NULL;

extern DEFSETFUNC (closeTmpFiles);
extern DEFSETFUNC (rmTmpFiles);

extern void pic16_AnalyzeBanking (void);
extern void copyFile (FILE * dest, FILE * src);
extern void pic16_InlinepCode(void);
extern void pic16_writeUsedRegs(FILE *);

extern void initialComments (FILE * afile);
extern void printPublics (FILE * afile);

void  pic16_pCodeInitRegisters(void);
pCodeOp *pic16_popGetLit(unsigned int lit);
pCodeOp *pic16_popGetLit2(unsigned int lit, pCodeOp *arg2);
pCodeOp *pic16_popCopyReg(pCodeOpReg *pc);
extern void pic16_pCodeConstString(char *name, char *value);

/*-----------------------------------------------------------------*/
/* aopLiteral - string from a literal value                        */
/*-----------------------------------------------------------------*/
int pic16aopLiteral (value *val, int offset)
{
  union {
    float f;
    unsigned char c[4];
  } fl;

  /* if it is a float then it gets tricky */
  /* otherwise it is fairly simple */
  if (!IS_FLOAT(val->type)) {
    unsigned long v = (unsigned long) floatFromVal(val);

    return ( (v >> (offset * 8)) & 0xff);
  }

  /* it is type float */
  fl.f = (float) floatFromVal(val);
#ifdef WORDS_BIGENDIAN
  return fl.c[3-offset];
#else
  return fl.c[offset];
#endif

}

iCode *tic;
symbol *nsym;
char tbuffer[512], *tbuf=tbuffer;;


/*-----------------------------------------------------------------*/
/* emitRegularMap - emit code for maps with no special cases       */
/*-----------------------------------------------------------------*/
static void
pic16emitRegularMap (memmap * map, bool addPublics, bool arFlag)
{
  symbol *sym;
//  int i, size, bitvars = 0;;

//	fprintf(stderr, "%s:%d map name= %s\n", __FUNCTION__, __LINE__, map->sname);
	
	if(addPublics)
		fprintf (map->oFile, ";\t.area\t%s\n", map->sname);
		/* print the area name */

	for (sym = setFirstItem (map->syms); sym; sym = setNextItem (map->syms)) {

#if 0
		fprintf(stderr, "%s\t%s: sym: %s\tused: %d\textern: %d\tstatic: %d\taggregate: %d\tregister: 0x%x\tfunction: %d\n",
			__FUNCTION__,
			map->sname, sym->name, sym->used, IS_EXTERN(sym->etype), IS_STATIC(sym->etype),
			IS_AGGREGATE(sym->type), SPEC_SCLS(sym->etype), IS_FUNC(sym->type));
		printTypeChain( sym->type, stderr );
		fprintf(stderr, "\n");
#endif

		/* if extern then add to externs */
		if (IS_EXTERN (sym->etype)) {

			/* reduce overhead while linking by not declaring
			 * extern unused external functions (usually declared
			 * in header files) */
			if(IS_FUNC(sym->type) && !sym->used)continue;
			
			checkAddSym(&externs, sym);
			continue;
		}
		
		/* if allocation required check is needed
		 *  then check if the symbol really requires
		 * allocation only for local variables */
		 if (arFlag && !IS_AGGREGATE (sym->type) &&
			!(sym->_isparm && !IS_REGPARM (sym->etype)) &&
			!sym->allocreq && sym->level) {

//			fprintf(stderr, "%s:%d special case, continuing...\n", __FILE__, __LINE__);

			continue;
		}

		/* if global variable & not static or extern
		 * and addPublics allowed then add it to the public set */
		if ((sym->used) && (sym->level == 0 ||
			(sym->_isparm && !IS_REGPARM (sym->etype))) &&
			addPublics &&
			!IS_STATIC (sym->etype) && !IS_FUNC(sym->type)) {
		  
			checkAddSym(&publics, sym);
		} else
			if(IS_STATIC(sym->etype)
				&& !(sym->ival && !sym->level)
			) {
			  regs *reg;
				/* add it to udata list */

//				fprintf(stderr, "%s:%d adding %s (%s) remat=%d\n", __FILE__, __LINE__,
//					sym->name, sym->rname, sym->remat);
					
						//, OP_SYMBOL(operandFromSymbol(sym))->name);
#define SET_IMPLICIT	1

#if SET_IMPLICIT
				if(IS_STRUCT(sym->type))
					sym->implicit = 1;
#endif

				reg = pic16_allocDirReg( operandFromSymbol( sym ));
				
				{
				  sectSym *ssym;
				  int found=0;
				  
#if 1
				  	for(ssym=setFirstItem(sectSyms); ssym; ssym=setNextItem(sectSyms)) {
				  		if(!strcmp(ssym->name, reg->name))found=1;
					}
#endif

					if(!found)checkAddReg(&pic16_rel_udata, reg);
				}
			}

		/* if extern then do nothing or is a function
		 * then do nothing */
		if (IS_FUNC (sym->type) && !IS_STATIC(sym->etype)) {
			if(SPEC_OCLS(sym->etype) == code) {
//				fprintf(stderr, "%s:%d: symbol added: %s\n", __FILE__, __LINE__, sym->rname);
				checkAddSym(&publics, sym);
			}
			continue;
		}

#if 0
		/* print extra debug info if required */
		if (options.debug || sym->level == 0) {
			cdbWriteSymbol (sym);	//, cdbFile, FALSE, FALSE);

			if (!sym->level)	/* global */
				if (IS_STATIC (sym->etype))
					fprintf (map->oFile, "F%s_", moduleName);		/* scope is file */
				else
					fprintf (map->oFile, "G_");	/* scope is global */
			else
				/* symbol is local */
				fprintf (map->oFile, "L%s_", (sym->localof ? sym->localof->name : "-null-"));
			fprintf (map->oFile, "%s_%d_%d", sym->name, sym->level, sym->block);
		}
#endif


		/* if is has an absolute address then generate
		an equate for this no need to allocate space */
		if (SPEC_ABSA (sym->etype)) {
//			if (options.debug || sym->level == 0)
//				fprintf (stderr,"; %s == 0x%04x\t\treqv= %p nRegs= %d\n",
//					sym->name, SPEC_ADDR (sym->etype), sym->reqv, sym->regType);

			fprintf (map->oFile, "%s\tEQU\t0x%04x\n",
				sym->rname,
				SPEC_ADDR (sym->etype));

			/* emit only if it is global */
			if(sym->level == 0) {
			  regs *reg;

				reg = pic16_dirregWithName( sym->name );
				if(!reg) {
					/* here */
//					fprintf(stderr, "%s:%d: implicit add of symbol = %s\n",
//							__FUNCTION__, __LINE__, sym->name);

					/* if IS_STRUCT is omitted the following
					 * fixes structures but break char/int etc */
#if SET_IMPLICIT
					if(IS_STRUCT(sym->type))
						sym->implicit = 1;		// mark as implicit
#endif
					if(!sym->ival) {
						reg = pic16_allocDirReg( operandFromSymbol(sym) );
						if(reg) {
							if(checkAddReg(&pic16_fix_udata, reg)) {
								/* and add to globals list if not exist */
								addSet(&publics, sym);
							}
						}
					} else
						addSet(&publics, sym);
				}
			}
		} else {
			if(!sym->used && (sym->level == 0)) {
			  regs *reg;

				/* symbol not used, just declared probably, but its in
				 * level 0, so we must declare it fine as global */
				
//				fprintf(stderr, "EXTRA symbol declaration sym= %s\n", sym->name);

#if SET_IMPLICIT
				if(IS_STRUCT(sym->type))
					sym->implicit = 1;		// mark as implicit
#endif
				if(!sym->ival) {
					if(IS_AGGREGATE(sym->type)) {
						reg=pic16_allocRegByName(sym->rname, getSize( sym->type ), NULL);
					} else {
						reg = pic16_allocDirReg( operandFromSymbol( sym ) );
					}

					{
					  sectSym *ssym;
					  int found=0;
				  
					  	fprintf(stderr, "%s:%d sym->rname: %s reg: %p reg->name: %s\n", __FILE__, __LINE__,
					  		sym->rname, reg, (reg?reg->name:"<<NULL>>"));
#if 1
					  	for(ssym=setFirstItem(sectSyms); ssym; ssym=setNextItem(sectSyms)) {
					  		if(!strcmp(ssym->name, reg->name))found=1;
						}
#endif

						if(!found)
							if(checkAddReg(&pic16_rel_udata, reg)) {
								addSetHead(&publics, sym);
							}

					}


		
				} else

					addSetHead(&publics, sym);
			}

#if 0
			/* allocate space */
			/* If this is a bit variable, then allocate storage after 8 bits have been declared */
			/* unlike the 8051, the pic does not have a separate bit area. So we emulate bit ram */
			/* by grouping the bits together into groups of 8 and storing them in the normal ram. */
			if (IS_BITVAR (sym->etype)) {
				bitvars++;
			} else {
				fprintf (map->oFile, "\t%s\n", sym->rname);
				if ((size = (unsigned int) getSize (sym->type) & 0xffff) > 1) {
					for (i = 1; i < size; i++)
						fprintf (map->oFile, "\t%s_%d\n", sym->rname, i);
				}
			}
			fprintf (map->oFile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
#endif
		}
	
		/* FIXME -- VR Fix the following, so that syms to be placed
		 * in the idata section and let linker decide about their fate */

		/* if it has an initial value then do it only if
			it is a global variable */

		if (sym->ival && sym->level == 0) {
		  ast *ival = NULL;

#if 0
			if(SPEC_OCLS(sym->etype)==data) {
				fprintf(stderr, "%s: sym %s placed in data segment\n", map->sname, sym->name);
			}

			if(SPEC_OCLS(sym->etype)==code) {
				fprintf(stderr, "%s: sym %s placed in code segment\n", map->sname, sym->name);
			}
#endif

#if 0
			fprintf(stderr, "'%s': sym '%s' has initial value SPEC_ABSA: %d, IS_AGGREGATE: %d\n",
				map->sname, sym->name, SPEC_ABSA(sym->etype), IS_AGGREGATE(sym->type));
#endif

			if (IS_AGGREGATE (sym->type)) {
				if(SPEC_ABSA(sym->etype))
					addSet(&fix_idataSymSet, copySymbol(sym));
				else
					addSet(&rel_idataSymSet, copySymbol(sym));
//				ival = initAggregates (sym, sym->ival, NULL);
			} else {
				if(SPEC_ABSA(sym->etype))
					addSet(&fix_idataSymSet, copySymbol(sym));
				else
					addSet(&rel_idataSymSet, copySymbol(sym));

//					ival = newNode ('=', newAst_VALUE(symbolVal (sym)),
//						decorateType (resolveSymbols (list2expr (sym->ival)), RESULT_CHECK));
			}

			if(ival) {
				setAstLineno(ival, sym->lineDef);
				codeOutFile = statsg->oFile;
				GcurMemmap = statsg;
				eBBlockFromiCode (iCodeFromAst (ival));
				sym->ival = NULL;
			}
		}
	}
}


/*-----------------------------------------------------------------*/
/* pic16_initPointer - pointer initialization code massaging       */
/*-----------------------------------------------------------------*/
value *pic16_initPointer (initList * ilist, sym_link *toType)
{
  value *val;
  ast *expr;

  if (!ilist) {
      return valCastLiteral(toType, 0.0);
  }

  expr = decorateType(resolveSymbols( list2expr (ilist) ), FALSE);
//  expr = list2expr( ilist );
  
  if (!expr)
    goto wrong;
  
  /* try it the old way first */
  if ((val = constExprValue (expr, FALSE)))
    return val;
  
  /* ( ptr + constant ) */
  if (IS_AST_OP (expr) &&
      (expr->opval.op == '+' || expr->opval.op == '-') &&
      IS_AST_SYM_VALUE (expr->left) &&
      (IS_ARRAY(expr->left->ftype) || IS_PTR(expr->left->ftype)) &&
      compareType(toType, expr->left->ftype) &&
      IS_AST_LIT_VALUE (expr->right)) {
    return valForCastAggr (expr->left, expr->left->ftype,
                           expr->right,
                           expr->opval.op);
  }
  
  /* (char *)&a */
  if (IS_AST_OP(expr) && expr->opval.op==CAST &&
      IS_AST_OP(expr->right) && expr->right->opval.op=='&') {
    if (compareType(toType, expr->left->ftype)!=1) {
      werror (W_INIT_WRONG);
      printFromToType(expr->left->ftype, toType);
    }
    // skip the cast ???
    expr=expr->right;
  }

  /* no then we have to do these cludgy checks */
  /* pointers can be initialized with address of
     a variable or address of an array element */
  if (IS_AST_OP (expr) && expr->opval.op == '&') {
    /* address of symbol */
    if (IS_AST_SYM_VALUE (expr->left)) {
      val = copyValue (AST_VALUE (expr->left));
      val->type = newLink (DECLARATOR);
      if(SPEC_SCLS (expr->left->etype) == S_CODE) {
        DCL_TYPE (val->type) = CPOINTER;
        DCL_PTR_CONST (val->type) = port->mem.code_ro;
      }
      else if (SPEC_SCLS (expr->left->etype) == S_XDATA)
        DCL_TYPE (val->type) = FPOINTER;
      else if (SPEC_SCLS (expr->left->etype) == S_XSTACK)
        DCL_TYPE (val->type) = PPOINTER;
      else if (SPEC_SCLS (expr->left->etype) == S_IDATA)
        DCL_TYPE (val->type) = IPOINTER;
      else if (SPEC_SCLS (expr->left->etype) == S_EEPROM)
        DCL_TYPE (val->type) = EEPPOINTER;
      else
        DCL_TYPE (val->type) = POINTER;

      val->type->next = expr->left->ftype;
      val->etype = getSpec (val->type);
      return val;
    }

    /* if address of indexed array */
    if (IS_AST_OP (expr->left) && expr->left->opval.op == '[')
      return valForArray (expr->left);

    /* if address of structure element then
       case 1. a.b ; */
    if (IS_AST_OP (expr->left) &&
        expr->left->opval.op == '.') {
      return valForStructElem (expr->left->left,
                               expr->left->right);
    }

    /* case 2. (&a)->b ;
       (&some_struct)->element */
    if (IS_AST_OP (expr->left) &&
        expr->left->opval.op == PTR_OP &&
        IS_ADDRESS_OF_OP (expr->left->left)) {
      return valForStructElem (expr->left->left->left,
                               expr->left->right);
    }
  }
  /* case 3. (((char *) &a) +/- constant) */
  if (IS_AST_OP (expr) &&
      (expr->opval.op == '+' || expr->opval.op == '-') &&
      IS_AST_OP (expr->left) && expr->left->opval.op == CAST &&
      IS_AST_OP (expr->left->right) &&
      expr->left->right->opval.op == '&' &&
      IS_AST_LIT_VALUE (expr->right)) {

    return valForCastAggr (expr->left->right->left,
                           expr->left->left->opval.lnk,
                           expr->right, expr->opval.op);

  }
  /* case 4. (char *)(array type) */
  if (IS_CAST_OP(expr) && IS_AST_SYM_VALUE (expr->right) &&
      IS_ARRAY(expr->right->ftype)) {

    val = copyValue (AST_VALUE (expr->right));
    val->type = newLink (DECLARATOR);
    if (SPEC_SCLS (expr->right->etype) == S_CODE) {
      DCL_TYPE (val->type) = CPOINTER;
      DCL_PTR_CONST (val->type) = port->mem.code_ro;
    }
    else if (SPEC_SCLS (expr->right->etype) == S_XDATA)
      DCL_TYPE (val->type) = FPOINTER;
    else if (SPEC_SCLS (expr->right->etype) == S_XSTACK)
      DCL_TYPE (val->type) = PPOINTER;
    else if (SPEC_SCLS (expr->right->etype) == S_IDATA)
      DCL_TYPE (val->type) = IPOINTER;
    else if (SPEC_SCLS (expr->right->etype) == S_EEPROM)
      DCL_TYPE (val->type) = EEPPOINTER;
    else
      DCL_TYPE (val->type) = POINTER;
    val->type->next = expr->right->ftype->next;
    val->etype = getSpec (val->type);
    return val;
  }

 wrong:
  if (expr)
    werrorfl (expr->filename, expr->lineno, E_INCOMPAT_PTYPES);
  else
    werror (E_INCOMPAT_PTYPES);
  return NULL;

}


/*-----------------------------------------------------------------*/
/* printPointerType - generates ival for pointer type              */
/*-----------------------------------------------------------------*/
void _pic16_printPointerType (const char *name, char ptype, void *p)
{
  char buf[256];

	sprintf(buf, "LOW(%s)", name);
	pic16_emitDS(buf, ptype, p);
	sprintf(buf, "HIGH(%s)", name);
	pic16_emitDS(buf, ptype, p);
}

/*-----------------------------------------------------------------*/
/* printPointerType - generates ival for pointer type              */
/*-----------------------------------------------------------------*/
void pic16_printPointerType (const char *name, char ptype, void *p)
{
  _pic16_printPointerType (name, ptype, p);
  pic16_flushDB(ptype, p);
}

/*-----------------------------------------------------------------*/
/* printGPointerType - generates ival for generic pointer type     */
/*-----------------------------------------------------------------*/
void pic16_printGPointerType (const char *iname, const char *oname, const unsigned int itype,
                   const unsigned int type, char ptype, void *p)
{
  _pic16_printPointerType (iname, ptype, p);

  if(itype == FPOINTER || itype == CPOINTER) {	// || itype == GPOINTER) {
    char buf[256];

	sprintf(buf, "UPPER(%s)", iname);
	pic16_emitDS(buf, ptype, p);
  }

  pic16_flushDB(ptype, p);
}


/* set to 0 to disable debug messages */
#define DEBUG_PRINTIVAL	1

/*-----------------------------------------------------------------*/
/* pic16_printIvalType - generates ival for int/char               */
/*-----------------------------------------------------------------*/
static void 
pic16_printIvalType (symbol *sym, sym_link * type, initList * ilist, char ptype, void *p)
{
  value *val;
  unsigned long ulval;

//  fprintf(stderr, "%s for symbol %s\n",__FUNCTION__, sym->rname);

#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif


  /* if initList is deep */
  if (ilist && ilist->type == INIT_DEEP)
    ilist = ilist->init.deep;

  if (!IS_AGGREGATE(sym->type) && getNelements(type, ilist)>1) {
    werror (W_EXCESS_INITIALIZERS, "scalar", sym->name, sym->lineDef);
  }

  if (!(val = list2val (ilist))) {
    // assuming a warning has been thrown
    val=constVal("0");
  }

  if (val->type != type) {
    val = valCastLiteral(type, floatFromVal(val));
  }

  if(val) 
    ulval = (unsigned long) floatFromVal (val);
  else
    ulval =0;

  switch (getSize (type)) {
  case 1:
    pic16_emitDB(BYTE_IN_LONG(ulval,0), ptype, p);
    break;

  case 2:
    pic16_emitDB(BYTE_IN_LONG(ulval,0), ptype, p);
    pic16_emitDB(BYTE_IN_LONG(ulval,1), ptype, p);
    break;

  case 4:
    pic16_emitDB(BYTE_IN_LONG(ulval,0), ptype, p);
    pic16_emitDB(BYTE_IN_LONG(ulval,1), ptype, p);
    pic16_emitDB(BYTE_IN_LONG(ulval,2), ptype, p);
    pic16_emitDB(BYTE_IN_LONG(ulval,3), ptype, p);
    break;
  }
}

/*--------------------------------------------------------------------*/
/* pic16_printIvalChar - generates initital value for character array */
/*--------------------------------------------------------------------*/
static int 
pic16_printIvalChar (sym_link * type, initList * ilist, char *s, char ptype, void *p)
{
  value *val;
  int remain;

  if(!p)
    return 0;

#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif

  if (!s)
    {

      val = list2val (ilist);
      /* if the value is a character string  */
      if (IS_ARRAY (val->type) && IS_CHAR (val->etype))
	{
	  if (!DCL_ELEM (type))
	    DCL_ELEM (type) = strlen (SPEC_CVAL (val->etype).v_char) + 1;

 	    for(remain=0; remain<DCL_ELEM(type); remain++)
		pic16_emitDB(SPEC_CVAL(val->etype).v_char[ remain ], ptype, p);
			
	  if ((remain = (DCL_ELEM (type) - strlen (SPEC_CVAL (val->etype).v_char) - 1)) > 0) {
	      while (remain--) {
                  pic16_emitDB(0x00, ptype, p);
              }
          }
	  return 1;
	}
      else
	return 0;
    }
  else {
    for(remain=0; remain<strlen(s); remain++) {
        pic16_emitDB(s[remain], ptype, p);
    }
  }
  return 1;
}

/*-----------------------------------------------------------------*/
/* pic16_printIvalArray - generates code for array initialization        */
/*-----------------------------------------------------------------*/
static void 
pic16_printIvalArray (symbol * sym, sym_link * type, initList * ilist,
		char ptype, void *p)
{
  initList *iloop;
  int lcnt = 0, size = 0;

  if(!p)
    return;


#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif
  /* take care of the special   case  */
  /* array of characters can be init  */
  /* by a string                      */
  if (IS_CHAR (type->next)) {
    if (!IS_LITERAL(list2val(ilist)->etype)) {
      werror (W_INIT_WRONG);
      return;
    }

    if(pic16_printIvalChar (type,
		       (ilist->type == INIT_DEEP ? ilist->init.deep : ilist),
		       SPEC_CVAL (sym->etype).v_char, ptype, p))
      return;
  }
  /* not the special case             */
  if (ilist && ilist->type != INIT_DEEP)
    {
      werror (E_INIT_STRUCT, sym->name);
      return;
    }

  iloop = ilist->init.deep;
  lcnt = DCL_ELEM (type);

  for (;;)
    {
      size++;
      pic16_printIval (sym, type->next, iloop, ptype, p);
      iloop = (iloop ? iloop->next : NULL);


      /* if not array limits given & we */
      /* are out of initialisers then   */
      if (!DCL_ELEM (type) && !iloop)
	break;

      /* no of elements given and we    */
      /* have generated for all of them */
      if (!--lcnt) {
	/* if initializers left */
	if (iloop) {
	  werror (W_EXCESS_INITIALIZERS, "array", sym->name, sym->lineDef);
	}
	break;
      }
    }

  /* if we have not been given a size  */
  if (!DCL_ELEM (type))
    DCL_ELEM (type) = size;

  return;
}

/*-----------------------------------------------------------------*/
/* pic16_printIvalBitFields - generate initializer for bitfields   */
/*-----------------------------------------------------------------*/
void pic16_printIvalBitFields(symbol **sym, initList **ilist, char ptype, void *p)
{
  value *val ;
  symbol *lsym = *sym;
  initList *lilist = *ilist ;
  unsigned long ival = 0;
  int size =0;


#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif


  do {
    unsigned long i;
    val = list2val(lilist);
    if (size) {
      if (SPEC_BLEN(lsym->etype) > 8) {
        size += ((SPEC_BLEN (lsym->etype) / 8) +
                 (SPEC_BLEN (lsym->etype) % 8 ? 1 : 0));
      }
    } else {
      size = ((SPEC_BLEN (lsym->etype) / 8) +
              (SPEC_BLEN (lsym->etype) % 8 ? 1 : 0));
    }
    i = (unsigned long)floatFromVal(val);
    i <<= SPEC_BSTR (lsym->etype);
    ival |= i;
    if (! ( lsym->next &&
          (IS_BITFIELD(lsym->next->type)) &&
          (SPEC_BSTR(lsym->next->etype)))) break;
    lsym = lsym->next;
    lilist = lilist->next;
  } while (1);
  switch (size) {
  case 1:
	pic16_emitDB(BYTE_IN_LONG(ival, 0), ptype, p);
        break;

  case 2:
	pic16_emitDB(BYTE_IN_LONG(ival, 0), ptype, p);
	pic16_emitDB(BYTE_IN_LONG(ival, 1), ptype, p);
        break;

  case 4: /* EEP: why is this db and not dw? */
	pic16_emitDB(BYTE_IN_LONG(ival, 0), ptype, p);
	pic16_emitDB(BYTE_IN_LONG(ival, 1), ptype, p);
	pic16_emitDB(BYTE_IN_LONG(ival, 2), ptype, p);
	pic16_emitDB(BYTE_IN_LONG(ival, 3), ptype, p);
        break;
  default:
	/* VR - only 1,2,4 size long can be handled???? Why? */
  	fprintf(stderr, "%s:%d: unhandled case. Contact author.\n", __FILE__, __LINE__);
  	assert(0);
  }
  *sym = lsym;
  *ilist = lilist;
}
 

/*-----------------------------------------------------------------*/
/* printIvalStruct - generates initial value for structures        */
/*-----------------------------------------------------------------*/
void pic16_printIvalStruct (symbol * sym, sym_link * type,
                 initList * ilist, char ptype, void *p)
{
  symbol *sflds;
  initList *iloop = NULL;


#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif

  sflds = SPEC_STRUCT (type)->fields;

  if (ilist) {
    if (ilist->type != INIT_DEEP) {
      werrorfl (sym->fileDef, sym->lineDef, E_INIT_STRUCT, sym->name);
      return;
    }

    iloop = ilist->init.deep;
  }

  for (; sflds; sflds = sflds->next, iloop = (iloop ? iloop->next : NULL)) {
//    fprintf(stderr, "%s:%d sflds: %p\tiloop = %p\n", __FILE__, __LINE__, sflds, iloop);
    if (IS_BITFIELD(sflds->type)) {
      pic16_printIvalBitFields(&sflds, &iloop, ptype, p);
    } else {
      pic16_printIval (sym, sflds->type, iloop, ptype, p);
    }
  }
  if (iloop) {
    werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "struct", sym->name);
  }
  return;
}

/*--------------------------------------------------------------------------*/
/* pic16_printIvalCharPtr - generates initial values for character pointers */
/*--------------------------------------------------------------------------*/
int pic16_printIvalCharPtr (symbol * sym, sym_link * type, value * val, char ptype, void *p)
{
  int size = 0;

  /* PENDING: this is _very_ mcs51 specific, including a magic
     number...
     It's also endin specific.

     VR - Attempting to port this function to pic16 port - 8-Jun-2004
   */


#if DEBUG_PRINTIVAL
  fprintf(stderr, "%s\n",__FUNCTION__);
#endif

  size = getSize (type);

  if (val->name && strlen (val->name))
    {
      if (size == 1)            /* This appears to be Z80 specific?? */
        {
	  pic16_emitDS(val->name, ptype, p);
        }
      else if (size == 2)
        {
          pic16_printPointerType (val->name, ptype, p);
        }
      else if (size == 3)
        {
          int type;
          if (IS_PTR (val->type)) {
            type = DCL_TYPE (val->type);
          } else {
            type = PTR_TYPE (SPEC_OCLS (val->etype));
          }
          if (val->sym && val->sym->isstrlit) {
            // this is a literal string
            type=CPOINTER;
          }
          pic16_printGPointerType(val->name, sym->name, type, type, ptype, p);
        }
      else
        {
          fprintf (stderr, "*** internal error: unknown size in "
                   "printIvalCharPtr.\n");
	  assert(0);
        }
    }
  else
    {
      // these are literals assigned to pointers
      switch (size)
        {
        case 1:
	  pic16_emitDB(pic16aopLiteral(val, 0), ptype, p);
          break;
        case 2:
	  pic16_emitDB(pic16aopLiteral(val, 0), ptype, p);
	  pic16_emitDB(pic16aopLiteral(val, 1), ptype, p);
          break;
        case 3:
	  pic16_emitDB(pic16aopLiteral(val, 0), ptype, p);
	  pic16_emitDB(pic16aopLiteral(val, 1), ptype, p);
	  pic16_emitDB(pic16aopLiteral(val, 2), ptype, p);
          break;

        default:
          assert (0);
        }
    }

  if (val->sym && val->sym->isstrlit) {	// && !isinSet(statsg->syms, val->sym)) {
  	if(ptype == 'p' && !isinSet(statsg->syms, val->sym))addSet (&statsg->syms, val->sym);
  	else if(ptype == 'f' /*&& !isinSet(rel_idataSymSet, val->sym)*/)addSet(&rel_idataSymSet, val->sym);
  }

  return 1;
}

/*-----------------------------------------------------------------------*/
/* pic16_printIvalFuncPtr - generate initial value for function pointers */
/*-----------------------------------------------------------------------*/
void pic16_printIvalFuncPtr (sym_link * type, initList * ilist, char ptype, void *p)
{
  value *val;
  int dLvl = 0;

  if (ilist)
    val = list2val (ilist);
  else
    val = valCastLiteral(type, 0.0);

  if (!val) {
    // an error has been thrown already
    val=constVal("0");
  }

  if (IS_LITERAL(val->etype)) {
    if (compareType(type, val->etype) == 0) {
      werrorfl (ilist->filename, ilist->lineno, E_INCOMPAT_TYPES);
      printFromToType (val->type, type);
    }
    pic16_printIvalCharPtr (NULL, type, val, ptype, p);
    return;
  }

  /* check the types   */
  if ((dLvl = compareType (val->type, type->next)) <= 0)
    {
      pic16_emitDB(0x00, ptype, p);
      return;
    }

  /* now generate the name */
  if (!val->sym) {
      pic16_printPointerType (val->name, ptype, p);
  } else {
      pic16_printPointerType (val->sym->rname, ptype, p);
  }

  return;
}


/*-----------------------------------------------------------------*/
/* pic16_printIvalPtr - generates initial value for pointers       */
/*-----------------------------------------------------------------*/
void pic16_printIvalPtr (symbol * sym, sym_link * type, initList * ilist, char ptype, void *p)
{
  value *val;
  int size;

#if 0
	fprintf(stderr, "%s:%d initialising pointer: %s size: %d\n", __FILE__, __LINE__,
		sym->rname, getSize(sym->type));
#endif

  /* if deep then   */
  if (ilist && (ilist->type == INIT_DEEP))
    ilist = ilist->init.deep;

  /* function pointer     */
  if (IS_FUNC (type->next))
    {
      pic16_printIvalFuncPtr (type, ilist, ptype, p);
      return;
    }

  if (!(val = pic16_initPointer (ilist, type)))
    return;

  /* if character pointer */
  if (IS_CHAR (type->next))
    if (pic16_printIvalCharPtr (sym, type, val, ptype, p))
      return;

  /* check the type      */
  if (compareType (type, val->type) == 0) {
    werrorfl (ilist->filename, ilist->lineno, W_INIT_WRONG);
    printFromToType (val->type, type);
  }

  /* if val is literal */
  if (IS_LITERAL (val->etype))
    {
      switch (getSize (type))
        {
        case 1:
            pic16_emitDB((unsigned int)floatFromVal(val) & 0xff, ptype, p);
            break;
        case 2:
            pic16_emitDB(pic16aopLiteral(val, 0), ptype, p);
            pic16_emitDB(pic16aopLiteral(val, 1), ptype, p);
            break;
        case 3:
            pic16_emitDB(pic16aopLiteral(val, 0), ptype, p);
            pic16_emitDB(pic16aopLiteral(val, 1), ptype, p);
            pic16_emitDB(pic16aopLiteral(val, 2), ptype, p);
        default:
        	assert(0);
        }
      return;
    }


  size = getSize (type);

  if (size == 1)                /* Z80 specific?? */
    {
      pic16_emitDS(val->name, ptype, p);
    }
  else if (size == 2)
    {
        pic16_printPointerType (val->name, ptype, p);
    }
  else if (size == 3)
    {
      pic16_printGPointerType (val->name, sym->name, (IS_PTR(type)?DCL_TYPE(type):PTR_TYPE(SPEC_OCLS(sym->etype))),
                         (IS_PTR (val->type) ? DCL_TYPE (val->type) :
                          PTR_TYPE (SPEC_OCLS (val->etype))), ptype, p);
    } else
    	assert(0);
  return;
}



/*-----------------------------------------------------------------*/
/* pic16_printIval - generates code for initial value                    */
/*-----------------------------------------------------------------*/
void pic16_printIval (symbol * sym, sym_link * type, initList * ilist, char ptype, void *p)
{
//  sym_link *itype;
  
  if (!p)
    return;

#if 0
	fprintf(stderr, "%s:%d generating init for %s\n", __FILE__, __LINE__, sym->name);
	fprintf(stderr, "%s: IS_STRUCT: %d  IS_ARRAY: %d  IS_PTR: %d  IS_SPEC: %d\n", sym->name,
		IS_STRUCT(type), IS_ARRAY(type), IS_PTR(type), IS_SPEC(type));
#endif
	
  /* if structure then */
  if (IS_STRUCT (type))
    {
//      fprintf(stderr,"%s struct\n",__FUNCTION__);
      pic16_printIvalStruct (sym, type, ilist, ptype, p);
      return;
    }

  /* if this is an array */
  if (IS_ARRAY (type))
    {
//	fprintf(stderr,"%s array\n",__FUNCTION__);
      pic16_printIvalArray (sym, type, ilist, ptype, p);
      return;
    }

#if 0
  if (ilist)
    {
      // not an aggregate, ilist must be a node
      if (ilist->type!=INIT_NODE) {
          // or a 1-element list
        if (ilist->init.deep->next) {
          werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "scalar",
                  sym->name);
        } else {
          ilist=ilist->init.deep;
        }
      }

#if 0
      // and the type must match
      itype=ilist->init.node->ftype;

      if (compareType(type, itype)==0) {
        // special case for literal strings
        if (IS_ARRAY (itype) && IS_CHAR (getSpec(itype)) &&
            // which are really code pointers
            IS_PTR(type) && DCL_TYPE(type)==CPOINTER) {
          // no sweat
        } else {
//          werrorfl (ilist->filename, ilist->lineno, E_TYPE_MISMATCH, "assignment", " ");
//          printFromToType(itype, type);
        }
      }
#endif
    }
#endif

  /* if this is a pointer */
  if (IS_PTR (type))
    {
//      fprintf(stderr,"%s pointer\n",__FUNCTION__);
      pic16_printIvalPtr (sym, type, ilist, ptype, p);
      return;
    }


  /* if type is SPECIFIER */
  if (IS_SPEC (type))
    {
//	fprintf(stderr,"%s spec\n",__FUNCTION__);
      pic16_printIvalType (sym, type, ilist, ptype, p);
      return;
    }
}

int PIC16_IS_CONFIG_ADDRESS(int address)
{
  return ((address >= pic16->cwInfo.confAddrStart && address <= pic16->cwInfo.confAddrEnd));
}

int PIC16_IS_IDLOC_ADDRESS(int address)
{
   return ((address >= pic16->idInfo.idAddrStart && address <= pic16->idInfo.idAddrEnd));
}

/* wrapper function for the above */
int PIC16_IS_HWREG_ADDRESS(int address)
{
  return (PIC16_IS_CONFIG_ADDRESS(address) || PIC16_IS_IDLOC_ADDRESS(address));
}


/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
static void
pic16emitStaticSeg (memmap * map)
{
  symbol *sym;

  //fprintf(stderr, "%s\n",__FUNCTION__);

  pic16_initDB();

  /* for all variables in this segment do */
  for (sym = setFirstItem (map->syms); sym;
       sym = setNextItem (map->syms))
    {

#if 0
	fprintf(stderr, "%s\t%s: sym: %s\tused: %d\tSPEC_ABSA: %d\tSPEC_AGGREGATE: %d\tCODE: %d\n\
CODESPACE: %d\tCONST: %d\tPTRCONST: %d\tSPEC_CONST: %d\n", __FUNCTION__,
		map->sname, sym->name, sym->used, SPEC_ABSA(sym->etype), IS_AGGREGATE(sym->type),
		IS_CODE(sym->etype), IN_CODESPACE( SPEC_OCLS(sym->etype)), IS_CONSTANT(sym->etype),
		IS_PTR_CONST(sym->etype), SPEC_CONST(sym->etype));
	printTypeChain( sym->type, stderr );
	fprintf(stderr, "\n");
#endif

        if(SPEC_ABSA(sym->etype) && PIC16_IS_CONFIG_ADDRESS(SPEC_ADDR(sym->etype))) {
		pic16_assignConfigWordValue(SPEC_ADDR(sym->etype),
			(int) floatFromVal(list2val(sym->ival)));

		continue;
        }

        if(SPEC_ABSA(sym->etype) && PIC16_IS_IDLOC_ADDRESS(SPEC_ADDR(sym->etype))) {
		pic16_assignIdByteValue(SPEC_ADDR(sym->etype),
			(char) floatFromVal(list2val(sym->ival)));

		continue;
        }

	/* if it is "extern" then do nothing */
	if (IS_EXTERN (sym->etype) && !SPEC_ABSA(sym->etype)) {
		checkAddSym(&externs, sym);
	  continue;
	}

	/* if it is not static add it to the public
	   table */
	if (!IS_STATIC (sym->etype)) {
		/* do not emit if it is a config word declaration */
		checkAddSym(&publics, sym);
	}

      /* print extra debug info if required */
      if (options.debug || sym->level == 0) {
	  /* NOTE to me - cdbFile may be null in which case,
	   * the sym name will be printed to stdout. oh well */
	   debugFile->writeSymbol(sym);
      }
	 
      /* if it has an absolute address */
      if (SPEC_ABSA (sym->etype)) {
//		fprintf(stderr, "%s:%d spec_absa is true for symbol: %s\n",
//			__FILE__, __LINE__, sym->name);
			
	  /* if it has an initial value */
	  if (sym->ival)
	    {
	      pBlock *pb;
              symbol *asym;
              absSym *abSym;
              pCode *pcf;
              
              /* symbol has absolute address and initial value */
	      noAlloc++;
	      resolveIvalSym (sym->ival, sym->type);
	      asym = newSymbol(sym->rname, 0);
	      abSym = Safe_calloc(1, sizeof(absSym));
	      abSym->name = Safe_strdup( sym->rname );
	      abSym->address = SPEC_ADDR( sym->etype );
	      addSet(&absSymSet, abSym);
	      
	      pb = pic16_newpCodeChain(NULL, 'A',pic16_newpCodeCharP("; Starting pCode block for absolute Ival"));
	      pic16_addpBlock(pb);

	      pcf = pic16_newpCodeFunction(moduleName, asym->name);
	      PCF(pcf)->absblock = 1;
	      
	      pic16_addpCode2pBlock(pb,pcf);
	      pic16_addpCode2pBlock(pb,pic16_newpCodeLabel(sym->rname,-1));
//	      fprintf(stderr, "%s:%d [1] generating init for label: %s\n", __FILE__, __LINE__, sym->rname);
	      pic16_printIval(sym, sym->type, sym->ival, 'p', (void *)pb);
              pic16_flushDB('p', (void *)pb);

	      pic16_addpCode2pBlock(pb, pic16_newpCodeFunction(NULL, NULL));
	      noAlloc--;
	    }
	  else
	    {

	      /* symbol has absolute address but no initial value */
	      
	      /* allocate space */
	      fprintf (code->oFile, "%s:\n", sym->rname);

	      /* special case for character strings */
	      if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
		  SPEC_CVAL (sym->etype).v_char) {

//		fprintf(stderr, "%s:%d printing code string from %s\n", __FILE__, __LINE__, sym->rname);

		pic16_pCodeConstString(sym->rname , SPEC_CVAL (sym->etype).v_char);
	      } else {
	      	assert(0);
	      }
	    }
  
	} else {
//		fprintf(stderr, "%s:%d spec_absa is false for symbol: %s\n",
//			__FILE__, __LINE__, sym->name);

		
	  /* if it has an initial value */
	  if (sym->ival) {
	      pBlock *pb;

	      /* symbol doesn't have absolute address but has initial value */
	      fprintf (code->oFile, "%s:\n", sym->rname);
	      noAlloc++;
	      resolveIvalSym (sym->ival, sym->type);

	      pb = pic16_newpCodeChain(NULL, 'P',pic16_newpCodeCharP("; Starting pCode block for Ival"));
	      pic16_addpBlock(pb);

              /* make sure that 'code' directive is emitted before */
              pic16_addpCode2pBlock(pb, pic16_newpCodeAsmDir("code", NULL));
                            
//	      fprintf(stderr, "%s:%d [2] generating init for label: %s\n", __FILE__, __LINE__, sym->rname);

	      pic16_addpCode2pBlock(pb,pic16_newpCodeLabel(sym->rname,-1));
	      pic16_printIval(sym, sym->type, sym->ival, 'p', (void *)pb);
              pic16_flushDB('p', (void *)pb);
	      noAlloc--;
	    } else {

	      /* symbol doesn't have absolute address and no initial value */
	      /* allocate space */
//	      fprintf(stderr, "%s:%d [3] generating init for label: %s\n", __FILE__, __LINE__, sym->rname);
	      fprintf (code->oFile, "%s:\n", sym->rname);
	      /* special case for character strings */
	      if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
		  SPEC_CVAL (sym->etype).v_char) {
		
//		fprintf(stderr, "%s:%d printing code string from %s\n", __FILE__, __LINE__, sym->rname);

		pic16_pCodeConstString(sym->rname , SPEC_CVAL (sym->etype).v_char);
	      } else {
	        assert(0);
	      }
	    }
	}
    }

}


/*-----------------------------------------------------------------*/
/* pic16_emitConfigRegs - emits the configuration registers              */
/*-----------------------------------------------------------------*/
void pic16_emitConfigRegs(FILE *of)
{
  int i;

	for(i=0;i<pic16->cwInfo.confAddrEnd-pic16->cwInfo.confAddrStart;i++)
		if(pic16->cwInfo.crInfo[i].emit)	//mask != -1)
			fprintf (of, "\t__config 0x%x, 0x%hhx\n",
				pic16->cwInfo.confAddrStart+i,
				pic16->cwInfo.crInfo[i].value);
}

void pic16_emitIDRegs(FILE *of)
{
  int i;

	for(i=0;i<pic16->idInfo.idAddrEnd-pic16->idInfo.idAddrStart;i++)
		if(pic16->idInfo.irInfo[i].emit)
			fprintf (of, "\t__idlocs 0x%06x, 0x%hhx\n",
				pic16->idInfo.idAddrStart+i,
				pic16->idInfo.irInfo[i].value);
}


static void
pic16emitMaps ()
{
  /* no special considerations for the following
     data, idata & bit & xdata */
  pic16emitRegularMap (data, TRUE, TRUE);
  pic16emitRegularMap (idata, TRUE, TRUE);
  pic16emitRegularMap (bit, TRUE, FALSE);
  pic16emitRegularMap (xdata, TRUE, TRUE);
  pic16emitRegularMap (sfr, FALSE, FALSE);
  pic16emitRegularMap (sfrbit, FALSE, FALSE);
  pic16emitRegularMap (code, TRUE, FALSE);
  pic16emitStaticSeg (statsg);
}

/*-----------------------------------------------------------------*/
/* createInterruptVect - creates the interrupt vector              */
/*-----------------------------------------------------------------*/
static void
pic16createInterruptVect (FILE * vFile)
{
	/* if the main is only a prototype ie. no body then do nothing */
#if 0
	if (!IFFUNC_HASBODY(mainf->type)) {
		/* if ! compile only then main function should be present */
		if (!options.cc_only)
			werror (E_NO_MAIN);
		return;
	}
#endif

	if((!pic16_options.omit_ivt) || (pic16_options.omit_ivt && pic16_options.leave_reset)) {
		fprintf (vFile, ";\t.area\t%s\n", CODE_NAME);
		fprintf(vFile, ".intvecs\tcode\t0x%06x\n", pic16_options.ivt_loc);

		/* this is an overkill since WE are the port,
		 * and we know if we have a genIVT function! */
		if(port->genIVT) {
			port->genIVT(vFile, interrupts, maxInterrupts);
		}
	}
	
}


/*-----------------------------------------------------------------*/
/* pic16initialComments - puts in some initial comments            */
/*-----------------------------------------------------------------*/
static void
pic16initialComments (FILE * afile)
{
	initialComments (afile);
	fprintf (afile, "; PIC16 port for the Microchip 16-bit core micros\n");
	if(pic16_mplab_comp)
		fprintf(afile, "; MPLAB/MPASM/MPASMWIN/MPLINK compatibility mode enabled\n");
	fprintf (afile, iComments2);
}

/*-----------------------------------------------------------------*/
/* printPublics - generates global declarations for publics        */
/*-----------------------------------------------------------------*/
static void
pic16printPublics (FILE *afile)
{
  symbol *sym;

	fprintf (afile, "\n%s", iComments2);
	fprintf (afile, "; public variables in this module\n");
	fprintf (afile, "%s", iComments2);

	for(sym = setFirstItem (publics); sym; sym = setNextItem (publics))
		fprintf(afile, "\tglobal %s\n", sym->rname);
}

/*-----------------------------------------------------------------*/
/* printExterns - generates extern declarations for externs        */
/*-----------------------------------------------------------------*/
static void
pic16_printExterns(FILE *afile)
{
  symbol *sym;

  	/* print nothing if no externs to declare */
	if(!elementsInSet(externs) && !elementsInSet(pic16_builtin_functions))
		return;

	fprintf(afile, "\n%s", iComments2);
	fprintf(afile, "; extern variables in this module\n");
	fprintf(afile, "%s", iComments2);
	
	for(sym = setFirstItem(externs); sym; sym = setNextItem(externs))
		fprintf(afile, "\textern %s\n", sym->rname);

	for(sym = setFirstItem(pic16_builtin_functions); sym; sym = setNextItem(pic16_builtin_functions))
		fprintf(afile, "\textern _%s\n", sym->name);
}

/*-----------------------------------------------------------------*/
/* emitOverlay - will emit code for the overlay stuff              */
/*-----------------------------------------------------------------*/
static void
pic16emitOverlay (FILE * afile)
{
  set *ovrset;

  if (!elementsInSet (ovrSetSets))
    fprintf (afile, ";\t.area\t%s\n", port->mem.overlay_name);

  /* for each of the sets in the overlay segment do */
  for (ovrset = setFirstItem (ovrSetSets); ovrset;
       ovrset = setNextItem (ovrSetSets))
    {

      symbol *sym;

      if (elementsInSet (ovrset))
	{
	  /* this dummy area is used to fool the assembler
	     otherwise the assembler will append each of these
	     declarations into one chunk and will not overlay
	     sad but true */
	  fprintf (afile, ";\t.area _DUMMY\n");
	  /* output the area informtion */
	  fprintf (afile, ";\t.area\t%s\n", port->mem.overlay_name);	/* MOF */
	}

      for (sym = setFirstItem (ovrset); sym;
	   sym = setNextItem (ovrset))
	{

	  /* if extern then do nothing */
	  if (IS_EXTERN (sym->etype))
	    continue;

	  /* if allocation required check is needed
	     then check if the symbol really requires
	     allocation only for local variables */
	  if (!IS_AGGREGATE (sym->type) &&
	      !(sym->_isparm && !IS_REGPARM (sym->etype))
	      && !sym->allocreq && sym->level)
	    continue;

	  /* if global variable & not static or extern
	     and addPublics allowed then add it to the public set */
	  if ((sym->_isparm && !IS_REGPARM (sym->etype))
	      && !IS_STATIC (sym->etype)) {
//	      fprintf(stderr, "%s:%d %s accessed\n", __FILE__, __LINE__, __FUNCTION__);
	      checkAddSym(&publics, sym);
//	    addSetHead (&publics, sym);
	  }

	  /* if extern then do nothing or is a function
	     then do nothing */
	  if (IS_FUNC (sym->type))
	    continue;

#if 0
	  /* print extra debug info if required */
	  if (options.debug || sym->level == 0)
	    {

	      cdbSymbol (sym, cdbFile, FALSE, FALSE);

	      if (!sym->level)
		{		/* global */
		  if (IS_STATIC (sym->etype))
		    fprintf (afile, "F%s_", moduleName);	/* scope is file */
		  else
		    fprintf (afile, "G_");	/* scope is global */
		}
	      else
		/* symbol is local */
		fprintf (afile, "L%s_",
			 (sym->localof ? sym->localof->name : "-null-"));
	      fprintf (afile, "%s_%d_%d", sym->name, sym->level, sym->block);
	    }
#endif

	  /* if is has an absolute address then generate
	     an equate for this no need to allocate space */
	  if (SPEC_ABSA (sym->etype))
	    {

	      if (options.debug || sym->level == 0)
		fprintf (afile, " == 0x%04x\n", SPEC_ADDR (sym->etype));

	      fprintf (afile, "%s\t=\t0x%04x\n",
		       sym->rname,
		       SPEC_ADDR (sym->etype));
	    }
	  else
	    {
	      if (options.debug || sym->level == 0)
		fprintf (afile, "==.\n");

	      /* allocate space */
	      fprintf (afile, "%s:\n", sym->rname);
	      fprintf (afile, "\t.ds\t0x%04x\n", (unsigned int) getSize (sym->type) & 0xffff);
	    }

	}
    }
}


/*-----------------------------------------------------------------*/
/* glue - the final glue that hold the whole thing together        */
/*-----------------------------------------------------------------*/
void
pic16glue ()
{

  FILE *vFile;
  FILE *asmFile;
  FILE *ovrFile = tempfile();


	mainf = newSymbol ("main", 0);
	mainf->block = 0;

	mainf = findSymWithLevel(SymbolTab, mainf);
#if 0
	/* only if the main function exists */
	if (!(mainf = findSymWithLevel (SymbolTab, mainf))) {
		if (!options.cc_only)
			werror (E_NO_MAIN);
		return;
	}
#endif

// 	fprintf(stderr, "main function= %p (%s)\thas body= %d\n", mainf, (mainf?mainf->name:NULL), mainf?IFFUNC_HASBODY(mainf->type):-1);

	addSetHead(&tmpfileSet,ovrFile);
	pic16_pCodeInitRegisters();

	if (mainf && IFFUNC_HASBODY(mainf->type)) {
	  pBlock *pb = pic16_newpCodeChain(NULL,'X',pic16_newpCodeCharP("; Starting pCode block"));

		pic16_addpBlock(pb);

		/* entry point @ start of CSEG */
		pic16_addpCode2pBlock(pb,pic16_newpCodeLabel("__sdcc_program_startup",-1));

		if(initsfpnt) {
			pic16_addpCode2pBlock(pb, pic16_newpCode(POC_LFSR,
				pic16_popGetLit2(1, pic16_newpCodeOpRegFromStr("_stack_end"))));
			pic16_addpCode2pBlock(pb, pic16_newpCode(POC_LFSR,
				pic16_popGetLit2(2, pic16_newpCodeOpRegFromStr("_stack_end"))));
		}

		/* put in the call to main */
		pic16_addpCode2pBlock(pb,pic16_newpCode(POC_CALL,pic16_newpCodeOp("_main",PO_STR)));

		if (options.mainreturn) {
			pic16_addpCode2pBlock(pb,pic16_newpCodeCharP(";\treturn from main will return to caller\n"));
			pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETURN,NULL));
		} else {
			pic16_addpCode2pBlock(pb,pic16_newpCodeCharP(";\treturn from main will lock up\n"));
			pic16_addpCode2pBlock(pb,pic16_newpCode(POC_GOTO,pic16_newpCodeOp("$",PO_STR)));
		}
	}

	/* At this point we've got all the code in the form of pCode structures */
	/* Now it needs to be rearranged into the order it should be placed in the */
	/* code space */

	pic16_movepBlock2Head('P');              // Last
	pic16_movepBlock2Head(code->dbName);
	pic16_movepBlock2Head('X');
	pic16_movepBlock2Head(statsg->dbName);   // First

	/* print the global struct definitions */
//	if (options.debug)
//		cdbStructBlock (0);	//,cdbFile);

	vFile = tempfile();
	/* PENDING: this isnt the best place but it will do */
	if (port->general.glue_up_main) {
		/* create the interrupt vector table */
		pic16createInterruptVect (vFile);
	}

	addSetHead(&tmpfileSet,vFile);
    
	/* emit code for the all the variables declared */
	pic16emitMaps ();
	/* do the overlay segments */
	pic16emitOverlay(ovrFile);
	pic16_AnalyzepCode('*');

#if 1
	if(pic16_options.dumpcalltree) {
	  FILE *cFile;
		sprintf(buffer, dstFileName);
		strcat(buffer, ".calltree");
		cFile = fopen(buffer, "w");
		pic16_printCallTree( cFile );
		fclose(cFile);
	}
#endif

	pic16_InlinepCode();
	pic16_AnalyzepCode('*');

	if(pic16_debug_verbose)
		pic16_pcode_test();

	/* now put it all together into the assembler file */
	/* create the assembler file name */
	if ((noAssemble || options.c1mode)  && fullDstFileName) {
		sprintf (buffer, fullDstFileName);
	} else {
		sprintf (buffer, dstFileName);
		strcat (buffer, ".asm");
	}

	if (!(asmFile = fopen (buffer, "w"))) {
		werror (E_FILE_OPEN_ERR, buffer);
		exit (1);
	}
    
	/* initial comments */
	pic16initialComments (asmFile);

	/* print module name */
	fprintf(asmFile, "#FILE\t\"%s\"\n", fullSrcFileName);

	/* Let the port generate any global directives, etc. */
	if (port->genAssemblerPreamble) {
		port->genAssemblerPreamble(asmFile);
	}
	
	/* print the extern variables to this module */
	pic16_printExterns(asmFile);
	
	/* print the global variables in this module */
	pic16printPublics (asmFile);

#if 0
	/* copy the sfr segment */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; special function registers\n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, sfr->oFile);
#endif

	/* Put all variables into a cblock */
	pic16_AnalyzeBanking();
	pic16_writeUsedRegs(asmFile);

#if 0
	/* no xdata in pic */
	/* if external stack then reserve space of it */
	if (mainf && IFFUNC_HASBODY(mainf->type) && options.useXstack ) {
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile, "; external stack \n");
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile,";\t.area XSEG (XDATA)\n"); /* MOF */
		fprintf (asmFile,";\t.ds 256\n");
	}
#endif

#if 0	
	/* no xdata in pic */
	/* copy xtern ram data */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; external ram data\n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, xdata->oFile);
#endif

#if 0
	/* copy the bit segment */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; bit data\n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, bit->oFile);
#endif

	/* copy the interrupt vector table */
	if(mainf && IFFUNC_HASBODY(mainf->type)) {
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile, "; interrupt vector \n");
		fprintf (asmFile, "%s", iComments2);
		copyFile (asmFile, vFile);
	}
    
	/* copy global & static initialisations */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; global & static initialisations\n");
	fprintf (asmFile, "%s", iComments2);
    
#if 0
	/* copy over code */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "\tcode\n");
	fprintf (asmFile, "%s", iComments2);
#endif

	if(pic16_debug_verbose)
		fprintf(asmFile, "; A code from now on!\n");
	pic16_copypCode(asmFile, 'A');


	if(mainf && IFFUNC_HASBODY(mainf->type)) {
		fprintf(asmFile, "\tcode\n");
		fprintf(asmFile,"__sdcc_gsinit_startup:\n");

#if 0
		/* FIXME 8051 legacy (?!) - VR 20-Jun-2003 */
		/* if external stack is specified then the
		 * higher order byte of the xdatalocation is
		 * going into P2 and the lower order going into */
	
		if (options.useXstack) {
			fprintf(asmFile,";\tmov\tP2,#0x%02x\n",
					(((unsigned int)options.xdata_loc) >> 8) & 0xff);
			fprintf(asmFile,";\tmov\t_spx,#0x%02x\n",
					(unsigned int)options.xdata_loc & 0xff);
		}
#endif
	}

//	copyFile (stderr, code->oFile);

	fprintf(asmFile, "; I code from now on!\n");
	pic16_copypCode(asmFile, 'I');

	if(pic16_debug_verbose)
		fprintf(asmFile, "; dbName from now on!\n");
	pic16_copypCode(asmFile, statsg->dbName);


	if (port->general.glue_up_main && mainf && IFFUNC_HASBODY(mainf->type)) {
		fprintf (asmFile,"\tgoto\t__sdcc_program_startup\n");
	}
	

	if(pic16_debug_verbose)
		fprintf(asmFile, "; X code from now on!\n");
	pic16_copypCode(asmFile, 'X');

	if(pic16_debug_verbose)
		fprintf(asmFile, "; M code from now on!\n");
	pic16_copypCode(asmFile, 'M');


	pic16_copypCode(asmFile, code->dbName);

	pic16_copypCode(asmFile, 'P');

	fprintf (asmFile,"\tend\n");
	fclose (asmFile);

	rm_tmpfiles();
}
