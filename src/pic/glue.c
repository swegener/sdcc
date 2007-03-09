/*-------------------------------------------------------------------------

  SDCCglue.c - glues everything we have done together into one file.
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
#include "main.h"
#include "device.h"
#include "dbuf_string.h"


#ifdef WORDS_BIGENDIAN
#define _ENDIAN(x)  (3-x)
#else
#define _ENDIAN(x)  (x)
#endif

#define BYTE_IN_LONG(x,b) ((x>>(8*_ENDIAN(b)))&0xff)
#define IS_GLOBAL(sym)	((sym)->level == 0)

extern symbol *interrupts[256];
static void showAllMemmaps(FILE *of); // XXX: emits initialized symbols
extern int noAlloc;
extern set *publics;
extern set *externs;
extern unsigned maxInterrupts;
extern int maxRegBank;
extern symbol *mainf;
extern char *VersionString;
extern struct dbuf_s *codeOutBuf;
extern char *iComments1;
extern char *iComments2;
//extern void emitStaticSeg (memmap * map);
set *pic14_localFunctions = NULL;

extern DEFSETFUNC (closeTmpFiles);
extern DEFSETFUNC (rmTmpFiles);

extern void AnalyzeBanking (void);
extern void ReuseReg(void);
extern void InlinepCode(void);
extern void writeUsedRegs(FILE *);

extern void initialComments (FILE * afile);
extern void printPublics (FILE * afile);

extern void printChar (FILE * ofile, char *s, int plen);
void  pCodeInitRegisters(void);
int getConfigWord(int address);
int getHasSecondConfigReg(void);
void pic14_debugLogClose(void); // from ralloc.c

char *udata_section_name=0;		// FIXME Temporary fix to change udata section name -- VR
int pic14_hasInterrupt = 0;		// Indicates whether to emit interrupt handler or not

/* dbufs for initialized data (idata and code sections),
 * extern, and global declarations */
struct dbuf_s *ivalBuf, *extBuf, *gloBuf, *gloDefBuf;

static set *emitted = NULL;
int pic14_stringInSet(const char *str, set **world, int autoAdd);

/*-----------------------------------------------------------------*/
/* aopLiteral - string from a literal value                        */
/*-----------------------------------------------------------------*/
unsigned int pic14aopLiteral (value *val, int offset)
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

#if 0
static int
is_valid_identifier( const char *name )
{
  char a;
  if (!name) return 0;
  a = *name;
  
  /* only accept [a-zA-Z_][a-zA-Z0-9_] */
  if (!((a >= 'a' && a <= 'z')
  	|| (a >= 'A' && a <= 'z')
	|| (a == '_')))
    return 0;

  name++;
  while ((a = *name++))
  {
    if (!((a >= 'a' && a <= 'z')
    	|| (a >= 'A' && a <= 'Z')
	|| (a >= '0' && a <= '9')
	|| (a == '_')))
      return 0;
  } // while

  /* valid identifier */
  return 1;
}
#endif

static void
emitPseudoStack(struct dbuf_s *oBuf, struct dbuf_s *oBufExt)
{
    int shared, low, high, size, i;
    PIC_device *pic;

    /* also emit STK symbols
     * XXX: This is ugly and fails as soon as devices start to get
     *      differently sized sharebanks, since STK12 will be
     *      required by larger devices but only up to STK03 might
     *      be defined using smaller devices. */
    shared = pic14_getSharedStack(&low, &high, &size);
    if (!pic14_options.isLibrarySource)
    {
	pic = pic14_getPIC();

	dbuf_printf (oBuf, "\n");
	dbuf_printf (oBuf, "\tglobal PSAVE\n");
	dbuf_printf (oBuf, "\tglobal SSAVE\n");
	dbuf_printf (oBuf, "\tglobal WSAVE\n");
	for (i = size - 4; i >= 0; i--) {
	    dbuf_printf (oBuf, "\tglobal STK%02d\n", i);
	} // for i
	dbuf_printf (oBuf, "\n");

	// 16f84 has no SHAREBANK (in linkerscript) but memory aliased in two
	// banks, sigh...
	if (1 || !shared) {
	    // for single banked devices: use normal, "banked" RAM
	    dbuf_printf (oBuf, "sharebank udata_ovr 0x%04X\n", low);
	} else {
	    // for devices with at least two banks, require a sharebank section
	    dbuf_printf (oBuf, "sharebank udata_shr\n");
	}
	dbuf_printf (oBuf, "PSAVE\tres 1\n");
	dbuf_printf (oBuf, "SSAVE\tres 1\n");
	dbuf_printf (oBuf, "WSAVE\tres 1\n"); // WSAVE *must* be in sharebank (IRQ handlers)
	/* fill rest of sharebank with stack STKxx .. STK00 */
	for (i = size - 4; i >= 0; i--) {
	    dbuf_printf (oBuf, "STK%02d\tres 1\n", i);
	} // for i
    } else {
	/* declare STKxx as extern for all files
	 * except the one containing main() */
	dbuf_printf (oBufExt, "\n");
	dbuf_printf (oBufExt, "\textern PSAVE\n");
	dbuf_printf (oBufExt, "\textern SSAVE\n");
	dbuf_printf (oBufExt, "\textern WSAVE\n");
	for (i = size - 4; i >= 0; i--) {
	    char buffer[128];
	    SNPRINTF(&buffer[0], 127, "STK%02d", i);
	    dbuf_printf (oBufExt, "\textern %s\n", &buffer[0]);
	    pic14_stringInSet(&buffer[0], &emitted, 1);
	} // for i
    }
    dbuf_printf (oBuf, "\n");
}

static int
emitIfNew(struct dbuf_s *oBuf, set **emitted, const char *fmt,
	const char *name)
{
    int wasPresent = pic14_stringInSet(name, emitted, 1);

    if (!wasPresent) {
	dbuf_printf (oBuf, fmt, name);
    } // if
    return (!wasPresent);
}

#define IS_DEFINED_HERE(sym)	(!IS_EXTERN(sym->etype))
extern int IS_CONFIG_ADDRESS( int addr );
static void
pic14_constructAbsMap (struct dbuf_s *oBuf, struct dbuf_s *gloBuf)
{
  memmap *maps[] = { data, sfr, NULL };
  int i;
  hTab *ht = NULL;
  symbol *sym;
  set *aliases;
  int addr, min=-1, max=-1;
  int size;

  for (i=0; maps[i] != NULL; i++)
  {
    for (sym = (symbol *)setFirstItem (maps[i]->syms);
	sym; sym = setNextItem (maps[i]->syms))
    {
      if (IS_DEFINED_HERE(sym) && SPEC_ABSA(sym->etype))
      {
	addr = SPEC_ADDR(sym->etype);

	/* handle CONFIG words here */
	if (IS_CONFIG_ADDRESS( addr ))
	{
	  //fprintf( stderr, "%s: assignment to CONFIG@0x%x found\n", __FUNCTION__, addr );
	  //fprintf( stderr, "ival: %p (0x%x)\n", sym->ival, (int)list2int( sym->ival ) );
	  if (sym->ival) {
	    pic14_assignConfigWordValue( addr, (int)list2int( sym->ival ) );
	  } else {
	    fprintf( stderr, "ERROR: Symbol %s, which is covering a __CONFIG word must be initialized!\n", sym->name );
	  }
	  continue;
	}
	
	if (max == -1 || addr > max) max = addr;
	if (min == -1 || addr < min) min = addr;
	//fprintf (stderr, "%s: sym %s @ 0x%x\n", __FUNCTION__, sym->name, addr);
	aliases = hTabItemWithKey (ht, addr);
	if (aliases) {
	  /* May not use addSetHead, as we cannot update the
	   * list's head in the hastable `ht'. */
	  addSet (&aliases, sym);
#if 0
	  fprintf( stderr, "%s: now %d aliases for %s @ 0x%x\n",
	      __FUNCTION__, elementsInSet(aliases), sym->name, addr);
#endif
	} else {
	  addSet (&aliases, sym);
	  hTabAddItem (&ht, addr, aliases);
	} // if
      } // if
    } // for sym
  } // for i

  /* now emit definitions for all absolute symbols */
  dbuf_printf (oBuf, "%s", iComments2);
  dbuf_printf (oBuf, "; absolute symbol definitions\n");
  dbuf_printf (oBuf, "%s", iComments2);
  for (addr=min; addr <= max; addr++)
  {
    size = 1;
    aliases = hTabItemWithKey (ht, addr);
    if (aliases && elementsInSet(aliases)) {
      /* Make sure there is no initialized value at this location! */
      for (sym = setFirstItem(aliases); sym; sym = setNextItem(aliases)) {
	  if (sym->ival) break;
      } // for
      if (sym) continue;
      
      dbuf_printf (oBuf, "UD_abs_%s_%x\tudata_ovr\t0x%04x\n",
	  moduleName, addr, addr);
      for (sym = setFirstItem (aliases); sym;
	  sym = setNextItem (aliases))
      {
	if (getSize(sym->type) > size) {
	  size = getSize(sym->type);
	}
	
	/* initialized values are handled somewhere else */
	if (sym->ival) continue;
	
        /* emit STATUS as well as _STATUS, required for SFRs only */
	//dbuf_printf (oBuf, "%s\tres\t0\n", sym->name);
	dbuf_printf (oBuf, "%s\n", sym->rname);

	if (IS_GLOBAL(sym) && !IS_STATIC(sym->etype)) {
	    //emitIfNew(gloBuf, &emitted, "\tglobal\t%s\n", sym->name);
	    emitIfNew(gloBuf, &emitted, "\tglobal\t%s\n", sym->rname);
	} // if
      } // for
      dbuf_printf (oBuf, "\tres\t%d\n", size);
    } // if
  } // for i
}

#if 0
/*-----------------------------------------------------------------*/
/* emitRegularMap - emit code for maps with no special cases       */
/*-----------------------------------------------------------------*/
static void
pic14emitRegularMap (memmap * map, bool addPublics, bool arFlag)
{
	symbol *sym;
	int bitvars = 0;;
	
	/* print the area name */
	if (addPublics)
		dbuf_printf (&map->oBuf, ";\t.area\t%s\n", map->sname);
	
	for (sym = setFirstItem (map->syms); sym;
	sym = setNextItem (map->syms)) {
		
		//printf("%s\n",sym->name);

		/* ignore if config word */
		if (SPEC_ABSA(sym->etype)
		    && IS_CONFIG_ADDRESS(SPEC_ADDR(sym->etype)))
			continue;
		
		/* if extern then add it into the extern list */
		if (IS_EXTERN (sym->etype)) {
			addSetHead (&externs, sym);
			continue;
		}
		
		/* if allocation required check is needed
		then check if the symbol really requires
		allocation only for local variables */
		if (arFlag && !IS_AGGREGATE (sym->type) &&
			!(sym->_isparm && !IS_REGPARM (sym->etype)) &&
			!sym->allocreq && sym->level)
			continue;
		
		/* if global variable & not static or extern
		and addPublics allowed then add it to the public set */
		if ((sym->level == 0 ||
			(sym->_isparm && !IS_REGPARM (sym->etype))) &&
			addPublics &&
			!IS_STATIC (sym->etype))
		{
			//fprintf( stderr, "%s: made public %s\n", __FUNCTION__, sym->name );
			addSetHead (&publics, sym);
		}
		
		// PIC code allocates its own registers - so ignore parameter variable generated by processFuncArgs()
		if (sym->_isparm)
			continue;
			/* if extern then do nothing or is a function
		then do nothing */
		if (IS_FUNC (sym->type))
			continue;
#if 0
		/* print extra debug info if required */
		if (options.debug || sym->level == 0)
		{
			if (!sym->level)	/* global */
				if (IS_STATIC (sym->etype))
					dbuf_printf (&map->oBuf, "F%s_", moduleName);		/* scope is file */
				else
					dbuf_printf (&map->oBuf, "G_");	/* scope is global */
				else
					/* symbol is local */
					dbuf_printf (&map->oBuf, "L%s_", (sym->localof ? sym->localof->name : "-null-"));
				dbuf_printf (&map->oBuf, "%s_%d_%d", sym->name, sym->level, sym->block);
		}
#endif
		/* absolute symbols are handled in pic14_constructAbsMap */
		if (SPEC_ABSA(sym->etype) && IS_DEFINED_HERE(sym))
			continue;
		
		/* if it has an absolute address then generate
		an equate for this no need to allocate space */
		if (0 && SPEC_ABSA (sym->etype))
		{
			//if (options.debug || sym->level == 0)
			//dbuf_printf (&map->oBuf,"; == 0x%04x\n",SPEC_ADDR (sym->etype));
			
			dbuf_printf (&map->oBuf, "%s\tEQU\t0x%04x\n",
				sym->rname,
				SPEC_ADDR (sym->etype));
		}
		else
		{
			/* allocate space */
			
			/* If this is a bit variable, then allocate storage after 8 bits have been declared */
			/* unlike the 8051, the pic does not have a separate bit area. So we emulate bit ram */
			/* by grouping the bits together into groups of 8 and storing them in the normal ram. */
			if (IS_BITVAR (sym->etype))
			{
				bitvars++;
			}
			else
			{
			    if (!sym->ival) {
				emitSymbol (&map->oBuf,
					sym->rname, 
					NULL,
					getSize (sym->type) & 0xffff,
					SPEC_ABSA(sym->etype)
						? SPEC_ADDR(sym->etype)
						: -1,
					0,
					0);
			    }
				/*
				{
				int i, size;
				
				  if ((size = (unsigned int) getSize (sym->type) & 0xffff) > 1)
				  {
				  for (i = 1; i < size; i++)
				  dbuf_printf (&map->oBuf, "\t%s_%d\n", sym->rname, i);
				  }
				  }
				*/
			}
			//dbuf_printf (&map->oBuf, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
		}

		/* if it has a initial value then do it only if
		it is a global variable */
		if (sym->ival) {
		    /* mark symbol as already defined */
		    pic14_stringInSet(sym->name, &emitted, 1);
		    pic14_stringInSet(sym->rname, &emitted, 1);
		}
#if 0		
		/* if it has a initial value then do it only if
		it is a global variable */
		if (sym->ival && sym->level == 0) {
			ast *ival = NULL;
		
			if (IS_AGGREGATE (sym->type))
				ival = initAggregates (sym, sym->ival, NULL);
			else
				ival = newNode ('=', newAst_VALUE(symbolVal (sym)),
				decorateType (resolveSymbols (list2expr (sym->ival)), RESULT_TYPE_NONE));
			codeOutBuf = &statsg->oBuf;
			GcurMemmap = statsg;
			eBBlockFromiCode (iCodeFromAst (ival));
			sym->ival = NULL;
		}
#endif
	}
}
#endif


#if 0
/*-----------------------------------------------------------------*/
/* printIvalType - generates ival for int/char                     */
/*-----------------------------------------------------------------*/
static void 
printIvalType (symbol *sym, sym_link * type, initList * ilist, pBlock *pb)
{
	value *val;
	unsigned long ulval;
	
	//fprintf(stderr, "%s\n",__FUNCTION__);
	
	/* if initList is deep */
	if (ilist->type == INIT_DEEP)
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
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
		break;
		
	case 2:
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,1))));
		break;
		
	case 4:
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,1))));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,2))));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(BYTE_IN_LONG(ulval,3))));
		break;
	}
}

/*-----------------------------------------------------------------*/
/* printIvalBitFields - generate initializer for bitfields         */
/*-----------------------------------------------------------------*/
static void printIvalBitFields(symbol **sym, initList **ilist, pBlock *pb ) 
{
	value *val ;
	symbol *lsym = *sym;
	initList *lilist = *ilist ;
	unsigned long ival = 0;
	int size =0;
	
	
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
		//tfprintf (oFile, "\t!db !constbyte\n",ival);
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival)));
		break;
		
	case 2:
		//tfprintf (oFile, "\t!dw !constword\n",ival);
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival>>8)));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival)));
		break;
	case 4:
		//tfprintf (oFile, "\t!db  !constword,!constword\n",(ival >> 8) & 0xffff, (ival & 0xffff));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival>>24)));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival>>16)));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival>>8)));
		addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(ival)));
		break;
	}
	*sym = lsym;
	*ilist = lilist;
}

/*-----------------------------------------------------------------*/
/* printIvalStruct - generates initial value for structures        */
/*-----------------------------------------------------------------*/
static void printIvalStruct (symbol * sym, sym_link * type, initList * ilist, pBlock *pb)
{
	symbol *sflds;
	initList *iloop = NULL;
	
	sflds = SPEC_STRUCT (type)->fields;
	
	if (ilist) {
		if (ilist->type != INIT_DEEP) {
			werrorfl (sym->fileDef, sym->lineDef, E_INIT_STRUCT, sym->name);
			return;
		}
		
		iloop = ilist->init.deep;
	}
	
	for (; sflds; sflds = sflds->next, iloop = (iloop ? iloop->next : NULL)) {
		if (IS_BITFIELD(sflds->type)) {
			printIvalBitFields(&sflds,&iloop,pb);
		} else {
			printIval (sym, sflds->type, iloop, pb);
		}
	}
	if (iloop) {
		werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "struct", sym->name);
	}
	return;
}

/*-----------------------------------------------------------------*/
/* printIvalChar - generates initital value for character array    */
/*-----------------------------------------------------------------*/
static int 
printIvalChar (sym_link * type, initList * ilist, pBlock *pb, char *s)
{
	value *val;
	int remain, ilen;
	
	if(!pb)
		return 0;
	
	//fprintf(stderr, "%s\n",__FUNCTION__);
	if (!s)
	{
		
		val = list2val (ilist);

		/* if the value is a character string  */
		if (IS_ARRAY (val->type) && IS_CHAR (val->etype))
		{
			ilen = DCL_ELEM(val->type);

			if (!DCL_ELEM (type))
				DCL_ELEM (type) = ilen;
		
			/* emit string constant */
			for (remain = 0; remain < ilen; remain++) {
				addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(SPEC_CVAL(val->etype).v_char[remain])));
			}
			
			/* fill array up to desired size */
			if ((remain = (DCL_ELEM (type) - ilen)) > 0)
				while (remain--)
					//tfprintf (oFile, "\t!db !constbyte\n", 0);
					addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(0)));
				return 1;
		}
		else
			return 0;
	}
	else {
		//printChar (oFile, s, strlen (s) + 1);
		
		for(remain=0; remain<(int)strlen(s); remain++) {
			addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(s[remain])));
			//fprintf(stderr,"0x%02x ",s[remain]);
		}
		//fprintf(stderr,"\n");
	}
	return 1;
}

/*-----------------------------------------------------------------*/
/* printIvalArray - generates code for array initialization        */
/*-----------------------------------------------------------------*/
static void 
printIvalArray (symbol * sym, sym_link * type, initList * ilist,
				pBlock *pb)
{
	initList *iloop;
	unsigned size = 0;

	if(!pb)
		return;
	if (ilist) {
		/* take care of the special   case  */
		/* array of characters can be init  */
		/* by a string                      */
		if (IS_CHAR (type->next)) {
			//fprintf(stderr,"%s:%d - is_char\n",__FUNCTION__,__LINE__);
			if (!IS_LITERAL(list2val(ilist)->etype)) {
				werror (W_INIT_WRONG);
				return;
			}
			if (printIvalChar (type,
				(ilist->type == INIT_DEEP ? ilist->init.deep : ilist),
				pb, SPEC_CVAL (sym->etype).v_char))
				return;
		}
		/* not the special case */
		if (ilist->type != INIT_DEEP) {
			werrorfl (ilist->filename, ilist->lineno, E_INIT_STRUCT, sym->name);
			return;
		}

		for (iloop=ilist->init.deep; iloop; iloop=iloop->next) {
			if ((++size > DCL_ELEM(type)) && DCL_ELEM(type)) {
				werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "array", sym->name);
				break;
			}
			printIval (sym, type->next, iloop, pb);
		}
	}

	if (DCL_ELEM(type)) {
		// pad with zeros if needed
		if (size<DCL_ELEM(type)) {
			size = (DCL_ELEM(type) - size) * getSize(type->next);
			while (size--) {
				//tfprintf (oFile, "\t!db !constbyte\n", 0);
				addpCode2pBlock(pb,newpCode(POC_RETLW,newpCodeOpLit(0)));
			}
		}
	} else {
		// we have not been given a size, but we now know it
		DCL_ELEM (type) = size;
	}

	return;
}

/*-----------------------------------------------------------------*/
/* printIvalPtr - generates code for initial value of pointers     */
/*-----------------------------------------------------------------*/
extern value *initPointer (initList *, sym_link *toType);

static void 
printIvalPtr (symbol * sym, sym_link * type, initList * ilist, pBlock *pb)
{
	value *val;
	
	if (!ilist || !pb)
		return;
	
	fprintf (stderr, "FIXME: initializers for pointers...\n");
	printTypeChain (type, stderr);
	
	fprintf (stderr, "symbol: %s, DCL_TYPE():%d, DCL_ELEM():%d, IS_ARRAY():%d", sym->rname, DCL_TYPE(type), DCL_ELEM(type), IS_ARRAY(type));
	fprintf (stderr, "ilist: type=%d (INIT_DEEP=%d, INIT_NODE=%d)\n", ilist->type, INIT_DEEP, INIT_NODE);

	if (ilist && (ilist->type == INIT_DEEP))
	  ilist = ilist->init.deep;
	
	/* function pointers */
	if (IS_FUNC (type->next))
	{
		assert ( !"function pointers not yet handled" );
		//printIvalFuncPtr (type, ilist, pb);
	}

	if (!(val = initPointer (ilist, type)))
		return;
	
	if (IS_CHAR (type->next))
	{
		if (printIvalChar (type, ilist, pb, NULL)) return;
	}

	/* check the type */
	if (compareType (type, val->type) == 0)
	{
		werrorfl (ilist->filename, ilist->lineno, W_INIT_WRONG);
		printFromToType (val->type, type);
	}

	if (IS_LITERAL (val->etype))
	{
		switch (getSize (type))
		{
		case 1:
			fprintf (stderr, "BYTE: %i\n", (unsigned char)floatFromVal (val) & 0x00FF);
			break;
		case 2:
			fprintf (stderr, "WORD: %i\n", (unsigned int)floatFromVal (val) & 0x00FFFF);
			break;
		case 3: /* gneric pointers */
			assert ( !"generic pointers not yet handled" );
		case 4:
			fprintf (stderr, "LONG: %i\n", (unsigned int)floatFromVal (val) & 0x0000FFFFFFFF);
			break;
		default:
			assert ( !"invaild size of value -- aborting" );
		} // switch

		return;
	} // if (IS_LITERAL)

	/* now handle symbolic values */
	switch (getSize (type))
	{
	case 1:
		fprintf (stderr, "BYTE: %s", val->name);
		break;
	case 2:
		fprintf (stderr, "WORD: %s", val->name);
		break;
	case 4:
		fprintf (stderr, "LONG: %s", val->name);
		break;
	default:
		assert ( !"invalid size of (symbolic) value -- aborting" );
	} // switch
}

/*-----------------------------------------------------------------*/
/* printIval - generates code for initial value                    */
/*-----------------------------------------------------------------*/
static void 
printIval (symbol * sym, sym_link * type, initList * ilist, pBlock *pb)
{
	if (!ilist || !pb)
		return;
	
	/* if structure then    */
	if (IS_STRUCT (type))
	{
		//fprintf(stderr,"%s struct: %s\n",__FUNCTION__, sym->rname);
		printIvalStruct (sym, type, ilist, pb);
		return;
	}
	
	/* if this is an array   */
	if (IS_ARRAY (type))
	{
		//fprintf(stderr,"%s array: %s\n",__FUNCTION__, sym->rname);
		printIvalArray (sym, type, ilist, pb);
		return;
	}
	
	/* if this is a pointer */
	if (IS_PTR (type))
	{
		//fprintf(stderr,"%s pointer: %s\n",__FUNCTION__, sym->rname);
		printIvalPtr (sym, type, ilist, pb);
		return;
	}
	
	/* if type is SPECIFIER */
	if (IS_SPEC (type))
	{
		//fprintf(stderr,"%s spec %s\n",__FUNCTION__, sym->rname);
		printIvalType (sym, type, ilist, pb);
		return;
	}
}
#endif

#if 0
extern void pCodeConstString(char *name, char *value);
/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
static void
pic14emitStaticSeg (memmap * map)
{
	symbol *sym;
	
	dbuf_printf (&map->oBuf, ";\t.area\t%s\n", map->sname);
	
	//fprintf(stderr, "%s\n",__FUNCTION__);
	
	/* for all variables in this segment do */
	for (sym = setFirstItem (map->syms); sym;
	sym = setNextItem (map->syms))
	{
		/* if extern then add it into the extern list */
		if (IS_EXTERN (sym->etype)) {
			addSetHead (&externs, sym);
			continue;
		}
		
		/* if it is not static add it to the public
		table */
		if (!IS_STATIC (sym->etype))
			addSetHead (&publics, sym);
		
		/* print extra debug info if required */
		if (options.debug || sym->level == 0)
		{
			if (!sym->level)
			{			/* global */
				if (IS_STATIC (sym->etype))
					dbuf_printf (&code->oBuf, "F%s_", moduleName);	/* scope is file */
				else
					dbuf_printf (&code->oBuf, "G_");	/* scope is global */
			}
			else
				/* symbol is local */
				dbuf_printf (&code->oBuf, "L%s_",
				(sym->localof ? sym->localof->name : "-null-"));
			dbuf_printf (&code->oBuf, "%s_%d_%d", sym->name, sym->level, sym->block);
			
		}
		
		/* if it has an absolute address */
		if (SPEC_ABSA (sym->etype))
		{
			if (options.debug || sym->level == 0)
				dbuf_printf (&code->oBuf, " == 0x%04x\n", SPEC_ADDR (sym->etype));
			
			dbuf_printf (&code->oBuf, "%s\t=\t0x%04x\n",
				sym->rname,
				SPEC_ADDR (sym->etype));
		}
		else
		{
			if (options.debug || sym->level == 0)
				dbuf_printf (&code->oBuf, " == .\n");
			
			/* if it has an initial value */
			if (sym->ival)
			{
#if 0
				pBlock *pb;
				
				dbuf_printf (&code->oBuf, "%s:\n", sym->rname);
				noAlloc++;
				resolveIvalSym (sym->ival, sym->type);
				//printIval (sym, sym->type, sym->ival, &code->oBuf);
				pb = newpCodeChain(NULL, 'P',newpCodeCharP("; Starting pCode block for Ival"));
				addpBlock(pb);
				addpCode2pBlock(pb,newpCodeLabel(sym->rname,-1));
				
				printIval (sym, sym->type, sym->ival, pb);
				noAlloc--;
#endif
			}
			else
			{
				
				/* allocate space */
				dbuf_printf (&code->oBuf, "%s:\n", sym->rname);
				/* special case for character strings */
				if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
					SPEC_CVAL (sym->etype).v_char)
					pCodeConstString(sym->rname , SPEC_CVAL (sym->etype).v_char);
					/*printChar (code->oFile,
					SPEC_CVAL (sym->etype).v_char,
				strlen (SPEC_CVAL (sym->etype).v_char) + 1);*/
				else
					dbuf_printf (&code->oBuf, "\t.ds\t0x%04x\n", (unsigned int) getSize (sym->type) & 0xffff);
			}
		}
	}
	
}
#endif


#if 0
/*-----------------------------------------------------------------*/
/* emitMaps - emits the code for the data portion the code         */
/*-----------------------------------------------------------------*/
static void
pic14emitMaps ()
{
	pic14_constructAbsMap (&sfr->oBuf);
	emitPseudoStack(&sfr->oBuf, &sfr->oBuf);
/* no special considerations for the following
	data, idata & bit & xdata */
	pic14emitRegularMap (data, TRUE, TRUE);
	pic14emitRegularMap (idata, TRUE, TRUE);
	pic14emitRegularMap (bit, TRUE, FALSE);
	pic14emitRegularMap (xdata, TRUE, TRUE);
	pic14emitRegularMap (sfr, TRUE, FALSE);
	pic14emitRegularMap (sfrbit, FALSE, FALSE);
	pic14emitRegularMap (code, TRUE, FALSE);
	pic14emitStaticSeg (statsg);
	pic14emitStaticSeg (c_abs);
}
#endif

/*-----------------------------------------------------------------*/
/* createInterruptVect - creates the interrupt vector              */
/*-----------------------------------------------------------------*/
pCodeOp *popGetExternal (char *str);
static void
pic14createInterruptVect (struct dbuf_s * vBuf)
{
	mainf = newSymbol ("main", 0);
	mainf->block = 0;
	
	/* only if the main function exists */
	if (!(mainf = findSymWithLevel (SymbolTab, mainf)))
	{
		struct options *op = &options;
		if (!(op->cc_only || noAssemble))
			//	werror (E_NO_MAIN);
			fprintf(stderr,"WARNING: function 'main' undefined\n");
		return;
	}
	
	/* if the main is only a prototype ie. no body then do nothing */
	if (!IFFUNC_HASBODY(mainf->type))
	{
		/* if ! compile only then main function should be present */
		if (!(options.cc_only || noAssemble))
			//	werror (E_NO_MAIN);
			fprintf(stderr,"WARNING: function 'main' undefined\n");
		return;
	}
	
	dbuf_printf (vBuf, "%s", iComments2);
	dbuf_printf (vBuf, "; reset vector \n");
	dbuf_printf (vBuf, "%s", iComments2);
	dbuf_printf (vBuf, "STARTUP\t%s\n", CODE_NAME); // Lkr file should place section STARTUP at address 0x0
	dbuf_printf (vBuf, "\tnop\n"); /* first location for used by incircuit debugger */
	dbuf_printf (vBuf, "\tpagesel __sdcc_gsinit_startup\n");
	dbuf_printf (vBuf, "\tgoto\t__sdcc_gsinit_startup\n");
	popGetExternal("__sdcc_gsinit_startup");
}


/*-----------------------------------------------------------------*/
/* initialComments - puts in some initial comments                 */
/*-----------------------------------------------------------------*/
static void
pic14initialComments (FILE * afile)
{
	initialComments (afile);
	fprintf (afile, "; PIC port for the 14-bit core\n");
	fprintf (afile, iComments2);
	
}

int
pic14_stringInSet(const char *str, set **world, int autoAdd)
{
  char *s;

  if (!str) return 1;
  assert(world);

  for (s = setFirstItem(*world); s; s = setNextItem(*world))
  {
    /* found in set */
    if (0 == strcmp(s, str)) return 1;
  }

  /* not found */
  if (autoAdd) addSet(world, Safe_strdup(str));
  return 0;
}

#if 0
static int
pic14_emitSymbolIfNew(FILE *file, const char *fmt, const char *sym, int checkLocals)
{
  if (!pic14_stringInSet(sym, &emitted, 1)) {
    /* sym was not in emittedSymbols */
    if (!checkLocals || !pic14_stringInSet(sym, &pic14_localFunctions, 0)) {
      /* sym is not a locally defined function---avoid bug #1443651 */
      fprintf( file, fmt, sym );
      return 0;
    }
  }
  return 1;
}
#endif

#if 0
/*-------------------------------------------------------------------*/
/* emitSymbol - write a symbol definition only if it is not    */
/*                    already present                                */
/*-------------------------------------------------------------------*/
static void
emitSymbol (struct dbuf_s *oBuf, const char *name, const char *section_type, int size, int addr, int useEQU, int globalize)
{
    static unsigned int sec_idx = 0;

    /* workaround: variables declared via `sbit' result in a numeric
     * identifier (0xHH), EQU'ing them is invalid, so just ignore it.
     * sbit is heavily used in the inc2h-generated header files!
     */
    if (!is_valid_identifier(name))
    {
	//fprintf( stderr, "%s:%s:%u: ignored symbol: %s\n", __FILE__, __FUNCTION__, __LINE__, name );
	return;
    }

    /* check whether the symbol is already defined */
    if (pic14_stringInSet(name, &emitted, 1)) return;

    /* new symbol -- define it */
    //fprintf (stderr, "%s: emitting %s (%d)\n", __FUNCTION__, name, size);
    if (useEQU) {
	dbuf_printf (oBuf, "%s\tEQU\t0x%04x\n", name, addr);
    } else {
	/* we place each symbol into a section of its own to allow the linker
	 * to distribute the data into all available memory banks */
	if (!section_type) section_type = "udata";
	if (addr != -1)
	{
	    /* absolute symbols are handled in pic14_constructAbsMap */
	    /* do nothing */
	} else {
	    if (globalize) dbuf_printf (oBuf, "\tglobal\t%s\n", name);
	    dbuf_printf (oBuf, "udata_%s_%u\t%s\n", moduleName,
		    sec_idx++, section_type);
	    dbuf_printf (oBuf, "%s\tres\t%d\n", name, size);
	}
    }
}
#endif


#if 0
/*-----------------------------------------------------------------*/
/* printExterns - generates extern for external variables          */
/*-----------------------------------------------------------------*/
static void
pic14printExterns (FILE * afile)
{
	symbol *sym;
	
	fprintf (afile, "%s", iComments2);
	fprintf (afile, "; extern variables in this module\n");
	fprintf (afile, "%s", iComments2);
	
	for (sym = setFirstItem (externs); sym; sym = setNextItem (externs))
		pic14_emitSymbolIfNew(afile, "\textern %s\n", sym->rname, 1);
}

/*-----------------------------------------------------------------*/
/* printPublics - generates .global for publics                    */
/*-----------------------------------------------------------------*/
static void
pic14printPublics (FILE * afile)
{
  symbol *sym;

  fprintf (afile, "%s", iComments2);
  fprintf (afile, "; public variables in this module\n");
  fprintf (afile, "%s", iComments2);

  for (sym = setFirstItem (publics); sym;
      sym = setNextItem (publics)) {

    if(!IS_BITFIELD(sym->type) && ((IS_FUNC(sym->type) || sym->allocreq))) {
      if (!IS_BITVAR(sym->type))
	pic14_emitSymbolIfNew(afile, "\tglobal %s\n", sym->rname, 0);
    } else {
      /* Absolute variables are defines in the asm file as equates and thus can not be made global. */
      /* Not any longer! */
      //if (!SPEC_ABSA (sym->etype))
      pic14_emitSymbolIfNew(afile, "\tglobal %s\n", sym->rname, 0);
    }
  }
}
#endif

/*
 * Interface to BANKSEL generation.
 * This function should return != 0 iff str1 and str2 denote operands that
 * are known to be allocated into the same bank. Consequently, there will
 * be no BANKSEL emitted if str2 is accessed while str1 has been used to
 * select the current bank just previously.
 *
 * If in doubt, return 0.
 */
int
pic14_operandsAllocatedInSameBank(const char *str1, const char *str2) {
    // see pic14printLocals
    
    if (getenv("SDCC_PIC14_SPLIT_LOCALS")) {
	// no clustering applied, each register resides in its own bank
    } else {
	// check whether BOTH names are local registers
	// XXX: This is some kind of shortcut, should be safe...
	// In this model, all r0xXXXX are allocated into a single section
	// per file, so no BANKSEL required if accessing a r0xXXXX after a
	// (different) r0xXXXX. Works great for multi-byte operands.
	if (str1 && str2 && str1[0] == 'r' && str2[0] == 'r') return (1);
    } // if

    // assume operands in different banks
    return (0);
}

static void
pic14printLocals (struct dbuf_s *oBuf)
{
    set *allregs[6] = { dynAllocRegs/*, dynStackRegs, dynProcessorRegs*/,
	dynDirectRegs, dynDirectBitRegs/*, dynInternalRegs */ };
    regs *reg;
    int i, is_first = 1;
    static unsigned sectionNr = 0;

    /* emit all registers from all possible sets */
    for (i = 0; i < 6; i++) {
	if (allregs[i] == NULL) continue;

	for (reg = setFirstItem(allregs[i]); reg; reg = setNextItem(allregs[i])) {
	    if (reg->isEmitted) continue;

	    if (reg->wasUsed && !reg->isExtern) {
		if (!pic14_stringInSet(reg->name, &emitted, 1)) {
		    if (reg->isFixed) {
			// Should not happen, really...
			assert ( !"Compiler-assigned variables should not be pinned... This is a bug." );
			dbuf_printf(oBuf, "UDL_%s_%u\tudata\t0x%04X\n%s\tres\t%d\n",
				moduleName, sectionNr++, reg->address, reg->name, reg->size);
		    } else {
			if (getenv("SDCC_PIC14_SPLIT_LOCALS")) {
			    // assign each local register into its own section
			    dbuf_printf(oBuf, "UDL_%s_%u\tudata\n%s\tres\t%d\n",
				    moduleName, sectionNr++, reg->name, reg->size);
			} else {
			    // group all local registers into a single section
			    // This should greatly improve BANKSEL generation...
			    if (is_first) {
				dbuf_printf(oBuf, "UDL_%s_%u\tudata\n", moduleName, sectionNr++);
				is_first = 0;
			    }
			    dbuf_printf(oBuf, "%s\tres\t%d\n", reg->name, reg->size);
			}
		    }
		}
	    }
	    reg->isEmitted = 1;
	} // for
    } // for
}

/*-----------------------------------------------------------------*/
/* emitOverlay - will emit code for the overlay stuff              */
/*-----------------------------------------------------------------*/
static void
pic14emitOverlay (struct dbuf_s * aBuf)
{
	set *ovrset;
	
	/*  if (!elementsInSet (ovrSetSets))*/
	
	/* the hack below, fixes translates for devices which
	* only have udata_shr memory */
	dbuf_printf (aBuf, "%s\t%s\n",
		(elementsInSet(ovrSetSets)?"":";"),
		port->mem.overlay_name);
	
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
			
			/* I don't think this applies to us. We are using gpasm.  CRF */
			
			dbuf_printf (aBuf, ";\t.area _DUMMY\n");
			/* output the area informtion */
			dbuf_printf (aBuf, ";\t.area\t%s\n", port->mem.overlay_name);	/* MOF */
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
				&& !IS_STATIC (sym->etype))
				addSetHead (&publics, sym);
			
				/* if extern then do nothing or is a function
			then do nothing */
			if (IS_FUNC (sym->type))
				continue;
			
			/* print extra debug info if required */
			if (options.debug || sym->level == 0)
			{
				if (!sym->level)
				{		/* global */
					if (IS_STATIC (sym->etype))
						dbuf_printf (aBuf, "F%s_", moduleName);	/* scope is file */
					else
						dbuf_printf (aBuf, "G_");	/* scope is global */
				}
				else
					/* symbol is local */
					dbuf_printf (aBuf, "L%s_",
					(sym->localof ? sym->localof->name : "-null-"));
				dbuf_printf (aBuf, "%s_%d_%d", sym->name, sym->level, sym->block);
			}
			
			/* if is has an absolute address then generate
			an equate for this no need to allocate space */
			if (SPEC_ABSA (sym->etype))
			{
				
				if (options.debug || sym->level == 0)
					dbuf_printf (aBuf, " == 0x%04x\n", SPEC_ADDR (sym->etype));
				
				dbuf_printf (aBuf, "%s\t=\t0x%04x\n",
					sym->rname,
					SPEC_ADDR (sym->etype));
			}
			else
			{
				if (options.debug || sym->level == 0)
					dbuf_printf (aBuf, "==.\n");
				
				/* allocate space */
				dbuf_printf (aBuf, "%s:\n", sym->rname);
				dbuf_printf (aBuf, "\t.ds\t0x%04x\n", (unsigned int) getSize (sym->type) & 0xffff);
			}
			
		}
	}
}


void
pic14_emitInterruptHandler (FILE * asmFile)
{
	if (pic14_hasInterrupt)
	{

		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile, "; interrupt and initialization code\n");
		fprintf (asmFile, "%s", iComments2);
		// Note - for mplink may have to enlarge section vectors in .lnk file
		// Note: Do NOT name this code_interrupt to avoid nameclashes with
		//       source files's code segment (interrupt.c -> code_interrupt)
		fprintf (asmFile, "c_interrupt\t%s\t0x4\n", CODE_NAME);
		
		/* interrupt service routine */
		fprintf (asmFile, "__sdcc_interrupt\n");
		copypCode(asmFile, 'I');
	}	
}

/*-----------------------------------------------------------------*/
/* glue - the final glue that hold the whole thing together        */
/*-----------------------------------------------------------------*/
void
picglue ()
{
	FILE *asmFile;
        struct dbuf_s ovrBuf;
        struct dbuf_s vBuf;

        dbuf_init(&ovrBuf, 4096);
        dbuf_init(&vBuf, 4096);

	pCodeInitRegisters();

	/* check for main() */
	mainf = newSymbol ("main", 0);
	mainf->block = 0;
	mainf = findSymWithLevel (SymbolTab, mainf);

	if (!mainf || !IFFUNC_HASBODY(mainf->type))
	{
		/* main missing -- import stack from main module */
		//fprintf (stderr, "main() missing -- assuming we are NOT the main module\n");
		pic14_options.isLibrarySource = 1;
	}

#if 0
	if (mainf && IFFUNC_HASBODY(mainf->type)) {
		
		pBlock *pb = newpCodeChain(NULL,'X',newpCodeCharP("; Starting pCode block"));
		addpBlock(pb);
		
		/* entry point @ start of CSEG */
		addpCode2pBlock(pb,newpCodeLabel("__sdcc_program_startup",-1));
		/* put in the call to main */
		addpCode2pBlock(pb,newpCode(POC_CALL,newpCodeOp("_main",PO_STR)));
		
		if (options.mainreturn) {
			
			addpCode2pBlock(pb,newpCodeCharP(";\treturn from main will return to caller\n"));
			addpCode2pBlock(pb,newpCode(POC_RETURN,NULL));
			
		} else {
			
			addpCode2pBlock(pb,newpCodeCharP(";\treturn from main will lock up\n"));
			addpCode2pBlock(pb,newpCode(POC_GOTO,newpCodeOp("$",PO_STR)));
			
		}
	}
#endif	
	
	/* At this point we've got all the code in the form of pCode structures */
	/* Now it needs to be rearranged into the order it should be placed in the */
	/* code space */
	
	movepBlock2Head('P');              // Last
	movepBlock2Head(code->dbName);
	movepBlock2Head('X');
	movepBlock2Head(statsg->dbName);   // First
	
	
	/* print the global struct definitions */
	if (options.debug)
		cdbStructBlock (0);

	/* emit code for the all the variables declared */
	//pic14emitMaps ();
	/* do the overlay segments */
	pic14emitOverlay(&ovrBuf);
	
	/* PENDING: this isnt the best place but it will do */
	if (port->general.glue_up_main) {
		/* create the interrupt vector table */
		pic14createInterruptVect (&vBuf);
	}
	
	AnalyzepCode('*');
	
	ReuseReg(); // ReuseReg where call tree permits
	
	InlinepCode();
	
	AnalyzepCode('*');
	
	if (options.debug) pcode_test();
	
	
	/* now put it all together into the assembler file */
	/* create the assembler file name */
	
	if ((noAssemble || options.c1mode) && fullDstFileName)
	{
		sprintf (buffer, fullDstFileName);
	}
	else
	{
		sprintf (buffer, dstFileName);
		strcat (buffer, ".asm");
	}
	
	if (!(asmFile = fopen (buffer, "w"))) {
		werror (E_FILE_OPEN_ERR, buffer);
		exit (1);
	}

	/* prepare statistics */
	resetpCodeStatistics ();
	
	/* initial comments */
	pic14initialComments (asmFile);
	
	/* print module name */
	fprintf (asmFile, ";\t.module %s\n", moduleName);
	
	/* Let the port generate any global directives, etc. */
	if (port->genAssemblerPreamble)
	{
		port->genAssemblerPreamble(asmFile);
	}

	/* print the global variables in this module */
	//pic14printPublics (asmFile);
	
	/* print the extern variables in this module */
	//pic14printExterns (asmFile);
	
	/* copy the sfr segment */
#if 0
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; special function registers\n");
	fprintf (asmFile, "%s", iComments2);
        dbuf_write_and_destroy (&sfr->oBuf, asmFile);
	
	
	if (udata_section_name) {
		sprintf(udata_name,"%s",udata_section_name);
	} else {
		sprintf(udata_name,"data_%s",moduleName);
	}
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; udata\n");
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "%s\tudata\n", udata_name);
        dbuf_write_and_destroy(&data->oBuf, asmFile);
#endif

	/* Put all variables into a cblock */
	AnalyzeBanking();

	/* emit initialized data */
	showAllMemmaps(asmFile);

	/* print the locally defined variables in this module */
	writeUsedRegs(asmFile);
	
	/* create the overlay segments */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; overlayable items in internal ram \n");
	fprintf (asmFile, "%s", iComments2);    
        dbuf_write_and_destroy (&ovrBuf, asmFile);
	
#if 0
	
	/* create the stack segment MOF */
	if (mainf && IFFUNC_HASBODY(mainf->type)) {
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile, "; Stack segment in internal ram \n");
		fprintf (asmFile, "%s", iComments2);    
		fprintf (asmFile, ";\t.area\tSSEG\t(DATA)\n"
			";__start__stack:\n;\t.ds\t1\n\n");
	}
	
	/* create the idata segment */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; indirectly addressable internal ram data\n");
	fprintf (asmFile, "%s", iComments2);
        dbuf_write_and_destroy (&idata->oBuf, asmFile);

	/* if external stack then reserve space of it */
	if (mainf && IFFUNC_HASBODY(mainf->type) && options.useXstack ) {
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile, "; external stack \n");
		fprintf (asmFile, "%s", iComments2);
		fprintf (asmFile,";\t.area XSEG (XDATA)\n"); /* MOF */
		fprintf (asmFile,";\t.ds 256\n");
	}
	
	/* copy xtern ram data */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; external ram data\n");
	fprintf (asmFile, "%s", iComments2);
        dbuf_write_and_destroy (&xdata->oBuf, asmFile);

#endif
	
	/* copy the bit segment */
#if 0
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; bit data\n");
	fprintf (asmFile, "%s", iComments2);
        dbuf_write_and_destroy (&bit->oBuf, asmFile);
#endif

	/* copy the interrupt vector table */
	if (mainf && IFFUNC_HASBODY(mainf->type))
	  dbuf_write_and_destroy (&vBuf, asmFile);
        else
	  dbuf_destroy(&vBuf);

	/* create interupt ventor handler */
	pic14_emitInterruptHandler (asmFile);
	
	/* copy over code */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; code\n");
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "code_%s\t%s\n", moduleName, port->mem.code_name);
	
	/* unknown */
	copypCode(asmFile, 'X');
	
	/* _main function */
	copypCode(asmFile, 'M');
	
	/* other functions */
	copypCode(asmFile, code->dbName);
	
	/* unknown */
	copypCode(asmFile, 'P');

	dumppCodeStatistics (asmFile);
	
	fprintf (asmFile,"\tend\n");
	
	fclose (asmFile);
	pic14_debugLogClose();
}

/*
 * Deal with initializers.
 */
#undef DEBUGprintf
#if 0
// debugging output
#define DEBUGprintf printf
#else
// be quiet
#define DEBUGprintf 1 ? (void)0 : (void)printf
#endif


void ast_print (ast * tree, FILE *outfile, int indent);

#if 0
/*
 * Emit all memmaps.
 */
static void
showInitList(initList *list, int level)
{
    static const char *list_type[] = { "INIT_NODE", "INIT_DEEP", "INIT_HOLE" };
    static const char *ast_type[] = { "EX_OP", "EX_VALUE", "EX_LINK", "EX_OPERAND" };
    struct ast *ast;
    while (list) {
	printf ("      %d: type %u (%s), init %p, next %p\n", level, list->type, list_type[list->type], list->init.node, list->next);
	if (list->type == INIT_DEEP) {
	    showInitList(list->init.deep, level + 1);
	} else if (list->type == INIT_NODE) {
	    ast = list->init.node;
	    printf ("        type %u (%s), level %d, block %d, seqPoint %d\n",
		    ast->type, ast_type[ast->type], ast->level, ast->block, ast->seqPoint);
	    if (ast->type == EX_VALUE) {
		printf ("          VAL %lf\n", floatFromVal(ast->opval.val));
	    } else if (ast->type == EX_LINK) {
		printTypeChain(ast->opval.lnk, NULL);
	    } else if (ast->type == EX_OP) {
		printf ("          OP %u\n", ast->opval.op);
	    }
	} // if
	list = list->next;
    } // while
}
#endif

/*
 * DEBUG: Print a value.
 */
void
printVal(value *val)
{
    printf ("value %p: name %s, type %p, etype %p, sym %s, vArgs %d, lit 0x%lx/%ld\n",
	    val, val->name, val->type, val->etype,
	    val->sym ? val->sym->name : NULL, val->vArgs,
	    (long)floatFromVal(val), (long)floatFromVal(val));
    printTypeChain(val->type, stdout);
    printf ("\n");
    printTypeChain(val->etype, stdout);
    printf ("\n");
}

//prototype from ../SDCCicode.c
operand *operandFromAst (ast * tree,int lvl);

char *
parseIvalAst (ast *node, int *inCodeSpace) {
#define LEN 4096
    char *buffer = NULL;
    char *left, *right;
    
    if (IS_AST_VALUE(node)) {
	value *val = AST_VALUE(node);
	symbol *sym = IS_AST_SYM_VALUE(node) ? AST_SYMBOL(node) : NULL;
	if (inCodeSpace && val->type
		&& (IS_FUNC(val->type) || IS_CODE(getSpec(val->type))))
	{
	    *inCodeSpace = 1;
	}
	if (inCodeSpace && sym
		&& (IS_FUNC(sym->type)
		    || IS_CODE(getSpec(sym->type))))
	{
	    *inCodeSpace = 1;
	}
	
	DEBUGprintf ("%s: AST_VALUE\n", __FUNCTION__);
	if (IS_AST_LIT_VALUE(node)) {
	    buffer = Safe_alloc(LEN);
	    SNPRINTF(buffer, LEN, "0x%lx", (long)AST_LIT_VALUE(node));
	} else if (IS_AST_SYM_VALUE(node)) {
	    assert ( AST_SYMBOL(node) );
	    /*
	    printf ("sym %s: ", AST_SYMBOL(node)->rname);
	    printTypeChain(AST_SYMBOL(node)->type, stdout);
	    printTypeChain(AST_SYMBOL(node)->etype, stdout);
	    printf ("\n---sym %s: done\n", AST_SYMBOL(node)->rname);
	    */
	    buffer = Safe_strdup(AST_SYMBOL(node)->rname);
	} else {
	    assert ( !"Invalid values type for initializers in AST." );
	}
    } else if (IS_AST_OP(node)) {
	DEBUGprintf ("%s: AST_OP\n", __FUNCTION__);
	switch (node->opval.op) {
	case CAST:
	    assert (node->right);
	    buffer = parseIvalAst(node->right, inCodeSpace);
	    DEBUGprintf ("%s: %s\n", __FUNCTION__, buffer);
	    break;
	case '&':
	    assert ( node->left && !node->right );
	    buffer = parseIvalAst(node->left, inCodeSpace);
	    DEBUGprintf ("%s: %s\n", __FUNCTION__, buffer);
	    break;
	case '+':
	    assert (node->left && node->right );
	    left = parseIvalAst(node->left, inCodeSpace);
	    right = parseIvalAst(node->right, inCodeSpace);
	    buffer = Safe_alloc(LEN);
	    SNPRINTF(buffer, LEN, "(%s + %s)", left, right);
	    DEBUGprintf ("%s: %s\n", __FUNCTION__, buffer);
	    Safe_free(left);
	    Safe_free(right);
	    break;
	case '[':
	    assert ( node->left && node->right );
	    assert ( IS_AST_VALUE(node->left) && AST_VALUE(node->left)->sym );
	    right = parseIvalAst(node->right, inCodeSpace);
	    buffer = Safe_alloc(LEN);
	    SNPRINTF(buffer, LEN, "(%s + %u * %s)",
		    AST_VALUE(node->left)->sym->rname, getSize(AST_VALUE(node->left)->type), right);
	    Safe_free(right);
	    DEBUGprintf ("%s: %s\n", __FUNCTION__, &buffer[0]);
	    break;
	default:
	    assert ( !"Unhandled operation in initializer." );
	    break;
	}
    } else {
	assert ( !"Invalid construct in initializer." );
    }
    
    return (buffer);
}

/*
 * Emit the section preamble, absolute location (if any) and 
 * symbol name(s) for intialized data.
 */
static int
emitIvalLabel(struct dbuf_s *oBuf, symbol *sym)
{
    char *segname;
    static int in_code = 0;
    static int sectionNr = 0;
    
    if (sym) {
	// code or data space?
	if (IS_CODE(getSpec(sym->type))) {
	    segname = "code";
	    in_code = 1;
	} else {
	    segname = "idata";
	    in_code  = 0;
	}
	dbuf_printf(oBuf, "\nID_%s_%d\t%s", moduleName, sectionNr++, segname);
	if (SPEC_ABSA(getSpec(sym->type))) {
	    // specify address for absolute symbols
	    dbuf_printf(oBuf, "\t0x%04X", SPEC_ADDR(getSpec(sym->type)));
	} // if
	dbuf_printf(oBuf, "\n%s\n", sym->rname);

	addSet(&emitted, sym->rname);
    }
    return (in_code);
}

char *get_op(pCodeOp *pcop,char *buffer, size_t size);
/*
 * Actually emit the initial values in .asm format.
 */
static void
emitIvals(struct dbuf_s *oBuf, symbol *sym, initList *list, long lit, int size)
{
    int i;
    ast *node;
    operand *op;
    value *val = NULL;
    int inCodeSpace = 0;
    char *str = NULL;
    int in_code;
    
    assert (size <= sizeof(long));
    assert (!list || (list->type == INIT_NODE));
    node = list ? list->init.node : NULL;
    
    in_code = emitIvalLabel(oBuf, sym);
    if (!in_code) dbuf_printf (oBuf, "\tdb\t");

    if (!node) {
	// initialize as zero
	for (i=0; i < size; i++) {
	    if (in_code) {
		dbuf_printf (oBuf, "\tretlw 0x00");
	    } else {
		dbuf_printf (oBuf, "%s0x00", (i == 0) ? "" : ", ");
	    }
	} // for
	dbuf_printf (oBuf, "\n");
	return;
    } // if
    
    op = NULL;
    if (constExprTree(node) && (val = constExprValue(node, 0))) {
	op = operandFromValue(val);
	DEBUGprintf ("%s: constExpr ", __FUNCTION__);
	//printVal(val);
    } else if (IS_AST_VALUE(node)) {
	op = operandFromAst(node, 0);
    } else if (IS_AST_OP(node)) {
	str = parseIvalAst(node, &inCodeSpace);
	DEBUGprintf("%s: AST_OP: %s\n", __FUNCTION__, str);
	op = NULL;
    } else {
	assert ( !"Unhandled construct in intializer." );
    }

    if (op) { 
	aopOp(op, NULL, 1);
	assert(AOP(op));
	//printOperand(op, of);
    }
    
    for (i=0; i < size; i++) {
	char *text = op ? aopGet(AOP(op), i, 0, 0)
	    : get_op(newpCodeOpImmd(str, i, 0, inCodeSpace, 0), NULL, 0);
	if (in_code) {
	    dbuf_printf (oBuf, "\tretlw %s\n", text);
	} else {
	    dbuf_printf (oBuf, "%s%s", (i == 0) ? "" : ", ", text);
	}
    } // for
    dbuf_printf (oBuf, "\n");
}

/*
 * For UNIONs, we first have to find the correct alternative to map the
 * initializer to. This function maps the structure of the initializer to
 * the UNION members recursively.
 * Returns the type of the first `fitting' member.
 */
static sym_link *
matchIvalToUnion (initList *list, sym_link *type, int size)
{
    symbol *sym;
    
    assert (type);

    if (IS_PTR(type) || IS_CHAR(type) || IS_INT(type) || IS_LONG(type)
	    || IS_FLOAT(type)) 
    {
	if (!list || (list->type == INIT_NODE)) {
	    DEBUGprintf ("OK, simple type\n");
	    return (type);
	} else {
	    DEBUGprintf ("ERROR, simple type\n");
	    return (NULL);
	}
    } else if (IS_BITFIELD(type)) {
	if (!list || (list->type == INIT_NODE)) {
	    DEBUGprintf ("OK, bitfield\n");
	    return (type);
	} else {
	    DEBUGprintf ("ERROR, bitfield\n");
	    return (NULL);
	}
    } else if (IS_STRUCT(type) && SPEC_STRUCT(getSpec(type))->type == STRUCT) {
	if (!list || (list->type == INIT_DEEP)) {
	    if (list) list = list->init.deep;
	    sym = SPEC_STRUCT(type)->fields;
	    while (sym) {
		DEBUGprintf ("Checking STRUCT member %s\n", sym->name);
		if (!matchIvalToUnion(list, sym->type, 0)) {
		    DEBUGprintf ("ERROR, STRUCT member %s\n", sym->name);
		    return (NULL);
		}
		if (list) list = list->next;
		sym = sym->next;
	    } // while
	    
	    // excess initializers?
	    if (list) {
		DEBUGprintf ("ERROR, excess initializers\n");
		return (NULL);
	    }
	    
	    DEBUGprintf ("OK, struct\n");
	    return (type);
	}
	return (NULL);
    } else if (IS_STRUCT(type) && SPEC_STRUCT(getSpec(type))->type == UNION) {
	if (!list || (list->type == INIT_DEEP)) {
	    if (list) list = list->init.deep;
	    sym = SPEC_STRUCT(type)->fields;
	    while (sym) {
		DEBUGprintf ("Checking UNION member %s.\n", sym->name);
		if (((IS_STRUCT(sym->type) || getSize(sym->type) == size))
			&& matchIvalToUnion(list, sym->type, size))
		{
		    DEBUGprintf ("Matched UNION member %s.\n", sym->name);
		    return (sym->type);
		}
		sym = sym->next;
	    } // while
	} // if
	// no match found
	DEBUGprintf ("ERROR, no match found.\n");
	return (NULL);
    } else {
	assert ( !"Unhandled type in UNION." );
    }

    assert ( !"No match found in UNION for the given initializer structure." );
    return (NULL);
}

/*
 * Parse the type and its initializer and emit it (recursively).
 */
static void
emitInitVal(struct dbuf_s *oBuf, symbol *topsym, sym_link *my_type, initList *list)
{
    symbol *sym;
    int size, i;
    long lit;
    unsigned char *str;

    size = getSize(my_type);

    if (IS_PTR(my_type)) {
	DEBUGprintf ("(pointer, %d byte) %p\n", size, list ? (void *)(long)list2int(list) : NULL);
	emitIvals(oBuf, topsym, list, 0, size);
	return;
    }

    if (IS_ARRAY(my_type) && topsym && topsym->isstrlit) {
	str = (unsigned char *)SPEC_CVAL(topsym->etype).v_char;
	emitIvalLabel(oBuf, topsym);
	do {
	    dbuf_printf (oBuf, "\tretlw 0x%02x ; '%c'\n", str[0], (str[0] >= 0x20 && str[0] < 128) ? str[0] : '.');
	} while (*(str++));
	return;
    }

    if (IS_ARRAY(my_type) && list && list->type == INIT_NODE) {
	fprintf (stderr, "Unhandled initialized symbol: %s\n", topsym->name);
	assert ( !"Initialized char-arrays are not yet supported, assign at runtime instead." );
	return;
    }

    if (IS_ARRAY(my_type)) {
	DEBUGprintf ("(array, %d items, %d byte) below\n", DCL_ELEM(my_type), size);
	assert (!list || list->type == INIT_DEEP);
	if (list) list = list->init.deep;
	for (i = 0; i < DCL_ELEM(my_type); i++) {
	    emitInitVal(oBuf, topsym, my_type->next, list);
	    topsym = NULL;
	    if (list) list = list->next;
	} // for i
	return;
    }
    
    if (IS_FLOAT(my_type)) {
	// float, 32 bit
	DEBUGprintf ("(float, %d byte) %lf\n", size, list ? list2int(list) : 0.0);
	emitIvals(oBuf, topsym, list, 0, size);
	return;
    }
    
    if (IS_CHAR(my_type) || IS_INT(my_type) || IS_LONG(my_type)) {
	// integral type, 8, 16, or 32 bit
	DEBUGprintf ("(integral, %d byte) 0x%lx/%ld\n", size, list ? (long)list2int(list) : 0, list ? (long)list2int(list) : 0);
	emitIvals(oBuf, topsym, list, 0, size);
	return;
	
    } else if (IS_STRUCT(my_type) && SPEC_STRUCT(my_type)->type == STRUCT) {
	// struct
	DEBUGprintf ("(struct, %d byte) handled below\n", size);
	assert (!list || (list->type == INIT_DEEP));

	// iterate over struct members and initList
	if (list) list = list->init.deep;
	sym = SPEC_STRUCT(my_type)->fields;
	while (sym) {
	    long bitfield = 0;
	    int len = 0;
	    if (IS_BITFIELD(sym->type)) {
		while (sym && IS_BITFIELD(sym->type)) {
		    assert (!list || ((list->type == INIT_NODE) 
				&& IS_AST_LIT_VALUE(list->init.node)));
		    lit = list ? list2int(list) : 0;
		    DEBUGprintf ( "(bitfield member) %02lx (%d bit, starting at %d, bitfield %02lx)\n",
			    lit, SPEC_BLEN(getSpec(sym->type)),
			    SPEC_BSTR(getSpec(sym->type)), bitfield);
		    bitfield |= (lit & ((1ul << SPEC_BLEN(getSpec(sym->type))) - 1)) << SPEC_BSTR(getSpec(sym->type));
		    len += SPEC_BLEN(getSpec(sym->type));

		    sym = sym->next;
		    if (list) list = list->next;
		} // while
		assert (len < sizeof (long) * 8); // did we overflow our initializer?!?
		len = (len + 7) & ~0x07; // round up to full bytes
		emitIvals(oBuf, topsym, NULL, bitfield, len / 8);
		topsym = NULL;
	    } // if
	    
	    if (sym) {
		emitInitVal(oBuf, topsym, sym->type, list);
		topsym = NULL;
		sym = sym->next;
		if (list) list = list->next;
	    } // if
	} // while
	if (list) {
	    assert ( !"Excess initializers." );
	} // if
	return;
	
    } else if (IS_STRUCT(my_type) && SPEC_STRUCT(my_type)->type == UNION) {
	// union
	DEBUGprintf ("(union, %d byte) handled below\n", size);
	assert (list && list->type == INIT_DEEP);

	// iterate over union members and initList, try to map number and type of fields and initializers
	my_type = matchIvalToUnion(list, my_type, size);
	if (my_type) {
	    emitInitVal(oBuf, topsym, my_type, list->init.deep);
	    topsym = NULL;
	    size -= getSize(my_type);
	    if (size > 0) {
		// pad with (leading) zeros
		emitIvals(oBuf, NULL, NULL, 0, size);
	    }
	    return;
	} // if
	
	assert ( !"No UNION member matches the initializer structure.");
    } else if (IS_BITFIELD(my_type)) {
	assert ( !"bitfields should only occur in structs..." );
	
    } else {
	printf ("SPEC_NOUN: %d\n", SPEC_NOUN(my_type));
	assert( !"Unhandled initialized type.");
    }
}

/*
 * Emit a set of symbols.
 * type - 0: have symbol tell whether it is local, extern or global
 *        1: assume all symbols in set to be global
 *        2: assume all symbols in set to be extern
 */
static void
emitSymbolSet(set *s, int type)
{
    symbol *sym;
    initList *list;
    unsigned sectionNr = 0;

    for (sym = setFirstItem(s); sym; sym = setNextItem(s)) {
#if 0
	fprintf (stdout, ";    name %s, rname %s, level %d, block %d, key %d, local %d, ival %p, static %d, cdef %d, used %d\n",
		sym->name, sym->rname, sym->level, sym->block, sym->key, sym->islocal, sym->ival, IS_STATIC(sym->etype), sym->cdef, sym->used);
#endif

	if (sym->etype && SPEC_ABSA(sym->etype)
		&& IS_CONFIG_ADDRESS(SPEC_ADDR(sym->etype))
		&& sym->ival)
	{
	    // handle config words
	    pic14_assignConfigWordValue(SPEC_ADDR(sym->etype),
		    (int)list2int(sym->ival));
	    pic14_stringInSet(sym->rname, &emitted, 1);
	    continue;
	}

	if (sym->isstrlit) {
	    // special case: string literals
	    emitInitVal(ivalBuf, sym, sym->type, NULL);
	    continue;
	}

	if (type != 0 || sym->cdef
		|| (!IS_STATIC(sym->etype)
		    && IS_GLOBAL(sym)))
	{
	    // bail out for ___fsadd and friends
	    if (sym->cdef && !sym->used) continue;

	    /* export or import non-static globals */
	    if (!pic14_stringInSet(sym->rname, &emitted, 0)) {

		if (type == 2 || IS_EXTERN(sym->etype) || sym->cdef)
		{
		    /* do not add to emitted set, it might occur again! */
		    //if (!sym->used) continue;
		    // declare symbol
		    emitIfNew (extBuf, &emitted, "\textern\t%s\n", sym->rname);
		} else {
		    // declare symbol
		    emitIfNew (gloBuf, &emitted, "\tglobal\t%s\n", sym->rname);
		    if (!sym->ival && !IS_FUNC(sym->type)) {
			// also define symbol
			if (IS_ABSOLUTE(sym->etype)) {
			    // absolute location?
			    //dbuf_printf (gloDefBuf, "UD_%s_%u\tudata\t0x%04X\n", moduleName, sectionNr++, SPEC_ADDR(sym->etype));
			    // deferred to pic14_constructAbsMap
			} else {
			    dbuf_printf (gloDefBuf, "UD_%s_%u\tudata\n", moduleName, sectionNr++);
			    dbuf_printf (gloDefBuf, "%s\tres\t%d\n\n", sym->rname, getSize(sym->type));
			}
		    } // if
		} // if
		pic14_stringInSet(sym->rname, &emitted, 1);
	    } // if
	} // if
	list = sym->ival;
	//if (list) showInitList(list, 0);
	if (list) {
	    resolveIvalSym( list, sym->type );
	    emitInitVal(ivalBuf, sym, sym->type, sym->ival);
	    dbuf_printf (ivalBuf, "\n");
	}
    } // for sym
}

/*
 * Iterate over all memmaps and emit their contents (attributes, symbols).
 */
static void
showAllMemmaps(FILE *of)
{
    struct dbuf_s locBuf;
    memmap *maps[] = {
	xstack, istack, code, data, pdata, xdata, xidata, xinit,
	idata, bit, statsg, c_abs, x_abs, i_abs, d_abs,
	sfr, sfrbit, reg, generic, overlay, eeprom, home };
    memmap * map;
    int i;

    DEBUGprintf ("---begin memmaps---\n");
    if (!extBuf) extBuf = dbuf_new(1024);
    if (!gloBuf) gloBuf = dbuf_new(1024);
    if (!gloDefBuf) gloDefBuf = dbuf_new(1024);
    if (!ivalBuf) ivalBuf = dbuf_new(1024);
    dbuf_init(&locBuf, 1024);

    dbuf_printf (extBuf, "%s; external declarations\n%s", iComments2, iComments2);
    dbuf_printf (gloBuf, "%s; global declarations\n%s", iComments2, iComments2);
    dbuf_printf (gloDefBuf, "%s; global definitions\n%s", iComments2, iComments2);
    dbuf_printf (ivalBuf, "%s; initialized data\n%s", iComments2, iComments2);
    dbuf_printf (&locBuf, "%s; compiler-defined variables\n%s", iComments2, iComments2);

    for (i = 0; i < sizeof(maps) / sizeof (memmap *); i++) {
	map = maps[i];
	//DEBUGprintf ("memmap %i: %p\n", i, map);
	if (map) {
#if 0
	    fprintf (stdout, ";  pageno %c, sname %s, dbName %c, ptrType %d, slbl %d, sloc %u, fmap %u, paged %u, direct %u, bitsp %u, codesp %u, regsp %u, syms %p\n", 
		    map->pageno, map->sname, map->dbName, map->ptrType, map->slbl,
		    map->sloc, map->fmap, map->paged, map->direct, map->bitsp,
		    map->codesp, map->regsp, map->syms);
#endif
	    emitSymbolSet(map->syms, 0);
	} // if (map)
    } // for i
    DEBUGprintf ("---end of memmaps---\n");

    emitSymbolSet(publics, 1);
    emitSymbolSet(externs, 2);

    emitPseudoStack(gloBuf, extBuf);
    pic14_constructAbsMap(gloDefBuf, gloBuf);
    pic14printLocals (&locBuf);
    pic14_emitConfigWord(of); // must be done after all the rest

    dbuf_write_and_destroy(extBuf, of);
    dbuf_write_and_destroy(gloBuf, of);
    dbuf_write_and_destroy(gloDefBuf, of);
    dbuf_write_and_destroy(&locBuf, of);
    dbuf_write_and_destroy(ivalBuf, of);

    extBuf = gloBuf = gloDefBuf = ivalBuf = NULL;
}

