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


#ifdef WORDS_BIGENDIAN
#define _ENDIAN(x)  (3-x)
#else
#define _ENDIAN(x)  (x)
#endif

#define BYTE_IN_LONG(x,b) ((x>>(8*_ENDIAN(b)))&0xff)

extern symbol *interrupts[256];
static void printIval (symbol * sym, sym_link * type, initList * ilist, pBlock *pb);
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
set *pic14_localFunctions = NULL;

extern DEFSETFUNC (closeTmpFiles);
extern DEFSETFUNC (rmTmpFiles);

extern void AnalyzeBanking (void);
extern void copyFile (FILE * dest, FILE * src);
extern void ReuseReg(void);
extern void InlinepCode(void);
extern void writeUsedRegs(FILE *);

extern void initialComments (FILE * afile);
extern void printPublics (FILE * afile);

extern void printChar (FILE * ofile, char *s, int plen);
void  pCodeInitRegisters(void);
int getConfigWord(int address);
int getHasSecondConfigReg(void);
int pic14_getSharebankSize(void);
int pic14_getSharebankAddress(void);

char *udata_section_name=0;		// FIXME Temporary fix to change udata section name -- VR
int pic14_hasInterrupt = 0;		// Indicates whether to emit interrupt handler or not

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

/* Check whether the given reg is shared amongst all .o files of a project.
 * This is true for the pseudo stack and WSAVE, SSAVE and PSAVE. */
static int
is_shared_address (int addr)
{
  return ((addr > Gstack_base_addr - 18)
  	&& (addr <= Gstack_base_addr));
}

int
pic14_is_shared (regs *reg)
{
	if (!reg) return 0;
	return is_shared_address (reg->address);
}

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

/* set of already emitted symbols; we store only pointers to the emitted
 * symbol names so these MUST NO BE CHANGED afterwards... */
static set *symbolsEmitted = NULL;

/*-------------------------------------------------------------------*/
/* emitSymbolToFile - write a symbol definition only if it is not    */
/*                    already present                                */
/*-------------------------------------------------------------------*/
void
emitSymbolToFile (FILE *of, const char *name, const char *section_type, int size, int addr, int useEQU, int globalize)
{
	const char *sym;
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
	for (sym = (const char *) setFirstItem (symbolsEmitted);
		sym;
		sym = (const char *) setNextItem (symbolsEmitted))
	{
		if (!strcmp (sym, name))
		{
			//fprintf (stderr, "%s: already emitted: %s\n", __FUNCTION__, name);
			return;
		}
	} // for
	
	/* new symbol -- define it */
	//fprintf (stderr, "%s: emitting %s (%d)\n", __FUNCTION__, name, size);
	if (useEQU)
	  fprintf (of, "%s\tEQU\t0x%04x\n", name, addr);
	else
	{
	  /* we place each symbol into a section of its own to allow the linker
	   * to distribute the data into all available memory banks */
	  if (!section_type) section_type = "udata";
	  if (addr != -1)
	  {
	    /* absolute symbols are handled in pic14_constructAbsMap */
	    /* do nothing */
#if 0
	    /* workaround gpasm bug with symbols being EQUated and placed in absolute sections */
	    if (1 || !is_shared_address (addr))
	    {
	      if (globalize) fprintf (of, "\tglobal\t%s\n", name);
	      fprintf (of, "udata_%s_%u\t%s\t0x%04x\n", moduleName,
	          sec_idx++, "udata_ovr", addr);
	      fprintf (of, "%s\tres\t%d\n", name, size);
	    }
	    else
	    {
	      /* EQUs cannot be exported... */
	      fprintf (of, "%s\tEQU\t0x%04x\n", name, addr);
	    }
#endif
	  } else {
	    if (globalize) fprintf (of, "\tglobal\t%s\n", name);
	    fprintf (of, "udata_%s_%u\t%s\n", moduleName,
	          sec_idx++, section_type);
	    fprintf (of, "%s\tres\t%d\n", name, size);
	  }
	}

	//if (options.verbose) fprintf (stderr, "%s: emitted %s\n", __FUNCTION__, name);
	addSet (&symbolsEmitted, (void *) name);
}

#define IS_DEFINED_HERE(sym)	(!IS_EXTERN(sym->etype))
extern int IS_CONFIG_ADDRESS( int addr );
static void
pic14_constructAbsMap (FILE *ofile)
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
  fprintf (ofile, "%s", iComments2);
  fprintf (ofile, "; absolute symbol definitions\n");
  fprintf (ofile, "%s", iComments2);
  for (addr=min; addr <= max; addr++)
  {
    size = 1;
    aliases = hTabItemWithKey (ht, addr);
    if (aliases && elementsInSet(aliases)) {
      fprintf (ofile, "udata_abs_%s_%x\tudata_ovr\t0x%04x",
	  moduleName, addr, addr);
      for (sym = setFirstItem (aliases); sym;
	  sym = setNextItem (aliases))
      {
        /* emit STATUS as well as _STATUS, required for SFRs only */
	fprintf (ofile, "\n%s", sym->name);
	fprintf (ofile, "\n%s", sym->rname);
	if (getSize(sym->type) > size) {
	  size = getSize(sym->type);
	}
      } // for
      fprintf (ofile, "\tres\t%d\n", size);
    } // if
  } // for i

  /* also emit STK symbols
   * XXX: This is ugly and fails as soon as devices start to get
   *      differently sized sharebanks, since STK12 will be
   *      required by larger devices but only up to STK03 might
   *      be defined using smaller devices. */
  fprintf (ofile, "\n");
  if (!pic14_options.isLibrarySource)
  {
    fprintf (ofile, "\tglobal PSAVE\n");
    fprintf (ofile, "\tglobal SSAVE\n");
    fprintf (ofile, "\tglobal WSAVE\n");
    for (i=pic14_getSharebankSize()-4; i >= 0; i--) {
      fprintf (ofile, "\tglobal STK%02d\n", i);
    } // for i
    fprintf (ofile, "sharebank udata_ovr 0x%04x\n",
	  pic14_getSharebankAddress() - pic14_getSharebankSize() + 1);
    fprintf (ofile, "PSAVE\tres 1\n");
    fprintf (ofile, "SSAVE\tres 1\n");
    fprintf (ofile, "WSAVE\tres 1\n");
    /* fill rest of sharebank with stack STKxx .. STK00 */
    for (i=pic14_getSharebankSize()-4; i >= 0; i--) {
      fprintf (ofile, "STK%02d\tres 1\n", i);
    } // for i
  } else {
    /* declare STKxx as extern for all files
     * except the one containing main() */
    fprintf (ofile, "\textern PSAVE\n");
    fprintf (ofile, "\textern SSAVE\n");
    fprintf (ofile, "\textern WSAVE\n");
    for (i=pic14_getSharebankSize()-4; i >= 0; i--) {
      fprintf (ofile, "\textern STK%02d\n", i);
    } // for i
  }
}

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
		fprintf (map->oFile, ";\t.area\t%s\n", map->sname);
	
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
					fprintf (map->oFile, "F%s_", moduleName);		/* scope is file */
				else
					fprintf (map->oFile, "G_");	/* scope is global */
				else
					/* symbol is local */
					fprintf (map->oFile, "L%s_", (sym->localof ? sym->localof->name : "-null-"));
				fprintf (map->oFile, "%s_%d_%d", sym->name, sym->level, sym->block);
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
			//fprintf (map->oFile,"; == 0x%04x\n",SPEC_ADDR (sym->etype));
			
			fprintf (map->oFile, "%s\tEQU\t0x%04x\n",
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
				emitSymbolToFile (map->oFile,
					sym->rname, 
					NULL,
					getSize (sym->type) & 0xffff,
					SPEC_ABSA(sym->etype)
						? SPEC_ADDR(sym->etype)
						: -1,
					0,
					0);
				/*
				{
				int i, size;
				
				  if ((size = (unsigned int) getSize (sym->type) & 0xffff) > 1)
				  {
				  for (i = 1; i < size; i++)
				  fprintf (map->oFile, "\t%s_%d\n", sym->rname, i);
				  }
				  }
				*/
			}
			//fprintf (map->oFile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
		}
		
		/* if it has a initial value then do it only if
		it is a global variable */
		if (sym->ival && sym->level == 0) {
			ast *ival = NULL;
		
			if (IS_AGGREGATE (sym->type))
				ival = initAggregates (sym, sym->ival, NULL);
			else
				ival = newNode ('=', newAst_VALUE(symbolVal (sym)),
				decorateType (resolveSymbols (list2expr (sym->ival)), RESULT_TYPE_NONE));
			codeOutFile = statsg->oFile;
			GcurMemmap = statsg;
			eBBlockFromiCode (iCodeFromAst (ival));
			sym->ival = NULL;
		}
	}
}


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

extern void pCodeConstString(char *name, char *value);
/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
static void
pic14emitStaticSeg (memmap * map)
{
	symbol *sym;
	
	fprintf (map->oFile, ";\t.area\t%s\n", map->sname);
	
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
					fprintf (code->oFile, "F%s_", moduleName);	/* scope is file */
				else
					fprintf (code->oFile, "G_");	/* scope is global */
			}
			else
				/* symbol is local */
				fprintf (code->oFile, "L%s_",
				(sym->localof ? sym->localof->name : "-null-"));
			fprintf (code->oFile, "%s_%d_%d", sym->name, sym->level, sym->block);
			
		}
		
		/* if it has an absolute address */
		if (SPEC_ABSA (sym->etype))
		{
			if (options.debug || sym->level == 0)
				fprintf (code->oFile, " == 0x%04x\n", SPEC_ADDR (sym->etype));
			
			fprintf (code->oFile, "%s\t=\t0x%04x\n",
				sym->rname,
				SPEC_ADDR (sym->etype));
		}
		else
		{
			if (options.debug || sym->level == 0)
				fprintf (code->oFile, " == .\n");
			
			/* if it has an initial value */
			if (sym->ival)
			{
				pBlock *pb;
				
				fprintf (code->oFile, "%s:\n", sym->rname);
				noAlloc++;
				resolveIvalSym (sym->ival, sym->type);
				//printIval (sym, sym->type, sym->ival, code->oFile);
				pb = newpCodeChain(NULL, 'P',newpCodeCharP("; Starting pCode block for Ival"));
				addpBlock(pb);
				addpCode2pBlock(pb,newpCodeLabel(sym->rname,-1));
				
				printIval (sym, sym->type, sym->ival, pb);
				noAlloc--;
			}
			else
			{
				
				/* allocate space */
				fprintf (code->oFile, "%s:\n", sym->rname);
				/* special case for character strings */
				if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
					SPEC_CVAL (sym->etype).v_char)
					pCodeConstString(sym->rname , SPEC_CVAL (sym->etype).v_char);
					/*printChar (code->oFile,
					SPEC_CVAL (sym->etype).v_char,
				strlen (SPEC_CVAL (sym->etype).v_char) + 1);*/
				else
					fprintf (code->oFile, "\t.ds\t0x%04x\n", (unsigned int) getSize (sym->type) & 0xffff);
			}
		}
	}
	
}


/*-----------------------------------------------------------------*/
/* emitMaps - emits the code for the data portion the code         */
/*-----------------------------------------------------------------*/
static void
pic14emitMaps ()
{
	pic14_constructAbsMap (sfr->oFile);
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
}

/*-----------------------------------------------------------------*/
/* createInterruptVect - creates the interrupt vector              */
/*-----------------------------------------------------------------*/
static void
pic14createInterruptVect (FILE * vFile)
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
	
	fprintf (vFile, "%s", iComments2);
	fprintf (vFile, "; reset vector \n");
	fprintf (vFile, "%s", iComments2);
	fprintf (vFile, "STARTUP\t%s\n", CODE_NAME); // Lkr file should place section STARTUP at address 0x0
	fprintf (vFile, "\tnop\n"); /* first location for used by incircuit debugger */
	fprintf( vFile, "\tpagesel __sdcc_gsinit_startup\n");
	fprintf (vFile, "\tgoto\t__sdcc_gsinit_startup\n");
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

static int
pic14_emitSymbolIfNew(FILE *file, const char *fmt, const char *sym, int checkLocals)
{
  static set *emitted = NULL;

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
  fprintf (afile, "; publics variables in this module\n");
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

/*-----------------------------------------------------------------*/
/* emitOverlay - will emit code for the overlay stuff              */
/*-----------------------------------------------------------------*/
static void
pic14emitOverlay (FILE * afile)
{
	set *ovrset;
	
	/*  if (!elementsInSet (ovrSetSets))*/
	
	/* the hack below, fixes translates for devices which
	* only have udata_shr memory */
	fprintf (afile, "%s\t%s\n",
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
	char udata_name[80];
	FILE *vFile;
	FILE *asmFile;
	FILE *ovrFile = tempfile();
	
	addSetHead(&tmpfileSet,ovrFile);
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
	
	vFile = tempfile();
	
	addSetHead(&tmpfileSet,vFile);
	
	/* emit code for the all the variables declared */
	pic14emitMaps ();
	/* do the overlay segments */
	pic14emitOverlay(ovrFile);
	
	/* PENDING: this isnt the best place but it will do */
	if (port->general.glue_up_main) {
		/* create the interrupt vector table */
		pic14createInterruptVect (vFile);
	}
	
	AnalyzepCode('*');
	
	ReuseReg(); // ReuseReg where call tree permits
	
	InlinepCode();
	
	AnalyzepCode('*');
	
	pcode_test();
	
	
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

	/* Emit the __config directive */
	pic14_emitConfigWord (asmFile);
	
	/* print the global variables in this module */
	pic14printPublics (asmFile);
	
	/* print the extern variables in this module */
	pic14printExterns (asmFile);
	
	/* copy the sfr segment */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; special function registers\n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, sfr->oFile);
	
	
	if (udata_section_name) {
		sprintf(udata_name,"%s",udata_section_name);
	} else {
		sprintf(udata_name,"data_%s",moduleName);
	}
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; udata\n");
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "%s\tudata\n", udata_name);
	copyFile (asmFile, data->oFile);
	
	/* Put all variables into a cblock */
	AnalyzeBanking();
	writeUsedRegs(asmFile);
	
	/* create the overlay segments */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; overlayable items in internal ram \n");
	fprintf (asmFile, "%s", iComments2);    
	copyFile (asmFile, ovrFile);
	
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
	copyFile (asmFile, idata->oFile);
	
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
	copyFile (asmFile, xdata->oFile);
	
#endif
	
	/* copy the bit segment */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; bit data\n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, bit->oFile);

	/* copy the interrupt vector table */
	if (mainf && IFFUNC_HASBODY(mainf->type)) {
		copyFile (asmFile, vFile);
	}
	
	/* create interupt ventor handler */
	pic14_emitInterruptHandler (asmFile);
	
	if (mainf && IFFUNC_HASBODY(mainf->type)) {
		/* initialize data memory */
		fprintf (asmFile, "code_init\t%s\n", CODE_NAME); // Note - for mplink may have to enlarge section vectors in .lnk file
		fprintf (asmFile,"__sdcc_gsinit_startup\n");
		/* FIXME: This is temporary.  The idata section should be used.  If 
		not, we could add a special feature to the linker.  This will 
		work in the mean time.  Put all initalized data in main.c */
		copypCode(asmFile, statsg->dbName);
		fprintf (asmFile,"\tpagesel _main\n");
		fprintf (asmFile,"\tgoto _main\n");
	}
	
#if 0    
	
	/* copy global & static initialisations */
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; global & static initialisations\n");
	fprintf (asmFile, "%s", iComments2);
	copypCode(asmFile, statsg->dbName);
	
#endif
	
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
	
	rm_tmpfiles();
}
