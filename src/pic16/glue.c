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
#include "device.h"
#include "main.h"


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

extern DEFSETFUNC (closeTmpFiles);
extern DEFSETFUNC (rmTmpFiles);

extern void pic16_AnalyzeBanking (void);
extern void copyFile (FILE * dest, FILE * src);
extern void pic16_InlinepCode(void);
extern void pic16_writeUsedRegs(FILE *);

extern void initialComments (FILE * afile);
extern void printPublics (FILE * afile);

extern void printChar (FILE * ofile, char *s, int plen);
void  pic16_pCodeInitRegisters(void);
pCodeOp *pic16_popGetLit(unsigned int lit);
pCodeOp *pic16_popGetLit2(unsigned int lit, unsigned int lit2);
pCodeOp *pic16_popCopyReg(pCodeOpReg *pc);
pCodeOp *pic16_popCombine2(pCodeOpReg *src, pCodeOpReg *dst);

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

/*-----------------------------------------------------------------*/
/* emitRegularMap - emit code for maps with no special cases       */
/*-----------------------------------------------------------------*/
static void
pic16emitRegularMap (memmap * map, bool addPublics, bool arFlag)
{
  symbol *sym;
  int i, size, bitvars = 0;;

//	fprintf(stderr, "%s:%d map name= %s\n", __FUNCTION__, __LINE__, map->sname);
	
  if (addPublics)

    fprintf (map->oFile, ";\t.area\t%s\n", map->sname);

  /* print the area name */
  for (sym = setFirstItem (map->syms); sym;
       sym = setNextItem (map->syms))
    {

#if 0
	fprintf(stderr, "\t%s: sym: %s\tused: %d\n", map->sname, sym->name, sym->used);
	printTypeChain( sym->type, stderr );
	printf("\n");
#endif

      /* if extern then add to externs */
      if (IS_EXTERN (sym->etype)) {
	addSetHead(&externs, sym);
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
      if ((sym->used) && (sym->level == 0 ||
	   (sym->_isparm && !IS_REGPARM (sym->etype))) &&
	  addPublics &&
	  !IS_STATIC (sym->etype))
	addSetHead (&publics, sym);

      /* if extern then do nothing or is a function
         then do nothing */
      if (IS_FUNC (sym->type))
	continue;

#if 0
      /* print extra debug info if required */
      if (options.debug || sym->level == 0)
	{

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

	/* FIXME -- VR
	 * The equates are nice, but do not allow relocatable objects to
	 * be created in the form that I (VR) want to make SDCC to work */

      /* if is has an absolute address then generate
         an equate for this no need to allocate space */
      if (SPEC_ABSA (sym->etype))
	{
	  //if (options.debug || sym->level == 0)
//	  fprintf (stderr,"; %s == 0x%04x\t\treqv= %p nRegs= %d\n",
//	  	sym->name, SPEC_ADDR (sym->etype), sym->reqv, sym->regType);

	  fprintf (map->oFile, "%s\tEQU\t0x%04x\n",
		   sym->rname,
		   SPEC_ADDR (sym->etype));

#if 1
	  /* emit only if it is global */
	  if(sym->level == 0) {
	    regs *reg;
	    operand *op;

//		fprintf(stderr, "%s: implicit add of symbol = %s\n", __FUNCTION__, sym->name);
		op = operandFromSymbol( sym );
		reg = pic16_allocDirReg( op );
		if(reg) {
			//continue;

			checkAddReg(&pic16_fix_udata, reg);
			/* and add to globals list */
			addSetHead(&publics, sym);
		}
	  }
#endif
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
	      fprintf (map->oFile, "\t%s\n", sym->rname);
	      if ((size = (unsigned int) getSize (sym->type) & 0xffff) > 1)
		{
		  for (i = 1; i < size; i++)
		    fprintf (map->oFile, "\t%s_%d\n", sym->rname, i);
		}
	    }
	  //fprintf (map->oFile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
	}
	

	/* FIXME -- VR Fix the following, so that syms to be placed
	 * in the idata section and let linker decide about their fate */

	/* if it has an initial value then do it only if
	   it is a global variable */
//	if(sym->ival && sym->level == 0) 
		

#if 1
	if (sym->ival && sym->level == 0) {
	    ast *ival = NULL;

//		if(SPEC_OCLS(sym->etype)==data) {
//			fprintf(stderr, "%s: sym %s placed in data\n", map->sname, sym->name);
//		}

//		fprintf(stderr, "'%s': sym '%s' has initial value\n", map->sname, sym->name);

	    if (IS_AGGREGATE (sym->type))
		ival = initAggregates (sym, sym->ival, NULL);
	    else
		ival = newNode ('=', newAst_VALUE(symbolVal (sym)),
				decorateType (resolveSymbols (list2expr (sym->ival))));
	    codeOutFile = statsg->oFile;
	    GcurMemmap = statsg;
	    eBBlockFromiCode (iCodeFromAst (ival));
	    sym->ival = NULL;
	}
#endif
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
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
    break;

  case 2:
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,1))));
    break;

  case 4:
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,1))));
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,2))));
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,3))));
    break;
  }
}

/*-----------------------------------------------------------------*/
/* printIvalChar - generates initital value for character array    */
/*-----------------------------------------------------------------*/
static int 
printIvalChar (sym_link * type, initList * ilist, pBlock *pb, char *s)
{
  value *val;
  int remain;

  if(!pb)
    return 0;

  fprintf(stderr, "%s\n",__FUNCTION__);
  if (!s)
    {

      val = list2val (ilist);
      /* if the value is a character string  */
      if (IS_ARRAY (val->type) && IS_CHAR (val->etype))
	{
	  if (!DCL_ELEM (type))
	    DCL_ELEM (type) = strlen (SPEC_CVAL (val->etype).v_char) + 1;

	  //printChar (oFile, SPEC_CVAL (val->etype).v_char, DCL_ELEM (type));
	  //fprintf(stderr, "%s omitting call to printChar\n",__FUNCTION__);
	  pic16_addpCode2pBlock(pb,pic16_newpCodeCharP(";omitting call to printChar"));

	  if ((remain = (DCL_ELEM (type) - strlen (SPEC_CVAL (val->etype).v_char) - 1)) > 0)
	    while (remain--)
	      //tfprintf (oFile, "\t!db !constbyte\n", 0);
	      pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(0)));
	  return 1;
	}
      else
	return 0;
    }
  else {
    //printChar (oFile, s, strlen (s) + 1);

    for(remain=0; remain<strlen(s); remain++) {
      pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(s[remain])));
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
  int lcnt = 0, size = 0;

  if(!pb)
    return;

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
  /* not the special case             */
  if (ilist->type != INIT_DEEP)
    {
      werror (E_INIT_STRUCT, sym->name);
      return;
    }

  iloop = ilist->init.deep;
  lcnt = DCL_ELEM (type);

  for (;;)
    {
      //fprintf(stderr,"%s:%d - is_char\n",__FUNCTION__,__LINE__);
      size++;
      printIval (sym, type->next, iloop, pb);
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
      //fprintf(stderr,"%s struct\n",__FUNCTION__);
      //printIvalStruct (sym, type, ilist, oFile);
      return;
    }

  /* if this is a pointer */
  if (IS_PTR (type))
    {
      //fprintf(stderr,"%s pointer\n",__FUNCTION__);
      //printIvalPtr (sym, type, ilist, oFile);
      return;
    }

  /* if this is an array   */
  if (IS_ARRAY (type))
    {
      //fprintf(stderr,"%s array\n",__FUNCTION__);
      printIvalArray (sym, type, ilist, pb);
      return;
    }

  /* if type is SPECIFIER */
  if (IS_SPEC (type))
    {
      //fprintf(stderr,"%s spec\n",__FUNCTION__);
      printIvalType (sym, type, ilist, pb);
      return;
    }
}

extern void pic16_pCodeConstString(char *name, char *value);
/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
static void
pic16emitStaticSeg (memmap * map)
{
  symbol *sym;

  fprintf (map->oFile, ";\t.area\t%s\n", map->sname);

  //fprintf(stderr, "%s\n",__FUNCTION__);

  /* for all variables in this segment do */
  for (sym = setFirstItem (map->syms); sym;
       sym = setNextItem (map->syms))
    {

#if 1
	fprintf(stderr, "\t%s: sym: %s\tused: %d\n", map->sname, sym->name, sym->used);
	printTypeChain( sym->type, stderr );
	printf("\n");
#endif


      /* if it is "extern" then do nothing */
      if (IS_EXTERN (sym->etype))
	continue;

      /* if it is not static add it to the public
         table */
      if (!IS_STATIC (sym->etype))
	addSetHead (&publics, sym);

#if 0
      /* print extra debug info if required */
      if (options.debug || sym->level == 0)
	{
	  /* NOTE to me - cdbFile may be null in which case,
	   * the sym name will be printed to stdout. oh well */
	  if(cdbFile)
	    cdbSymbol (sym, cdbFile, FALSE, FALSE);

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
#endif

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
	      resolveIvalSym (sym->ival);
	      //printIval (sym, sym->type, sym->ival, code->oFile);
	      pb = pic16_newpCodeChain(NULL, 'P',pic16_newpCodeCharP("; Starting pCode block for Ival"));
	      pic16_addpBlock(pb);
	      pic16_addpCode2pBlock(pb,pic16_newpCodeLabel(sym->rname,-1));

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
		pic16_pCodeConstString(sym->rname , SPEC_CVAL (sym->etype).v_char);
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
  mainf = newSymbol ("main", 0);
  mainf->block = 0;

  /* only if the main function exists */
  if (!(mainf = findSymWithLevel (SymbolTab, mainf)))
    {
      if (!options.cc_only)
	werror (E_NO_MAIN);
      return;
    }

  /* if the main is only a prototype ie. no body then do nothing */
  if (!IFFUNC_HASBODY(mainf->type))
    {
      /* if ! compile only then main function should be present */
      if (!options.cc_only)
	werror (E_NO_MAIN);
      return;
    }

	if((!pic16_options.omit_ivt) || (pic16_options.omit_ivt && pic16_options.leave_reset)) {
		fprintf (vFile, ";\t.area\t%s\n", CODE_NAME);
		fprintf(vFile, ".intvecs\tcode\t0x0000\n");
		fprintf (vFile, "__interrupt_vect:\n");

		/* this is an overkill since WE are the port,
		 * and we know if we have a genIVT function! */
		if(port->genIVT) {
			port->genIVT(vFile, interrupts, maxInterrupts);
		}
	}
	
}


/*-----------------------------------------------------------------*/
/* initialComments - puts in some initial comments                 */
/*-----------------------------------------------------------------*/
static void
pic16initialComments (FILE * afile)
{
  initialComments (afile);
  fprintf (afile, "; PIC port for the 16-bit core\n");
  fprintf (afile, iComments2);

}

/*-----------------------------------------------------------------*/
/* printPublics - generates global declarations for publics        */
/*-----------------------------------------------------------------*/
static void
pic16printPublics (FILE * afile)
{
  symbol *sym;

	fprintf (afile, "%s", iComments2);
	fprintf (afile, "; publics variables in this module\n");
	fprintf (afile, "%s", iComments2);

	for (sym = setFirstItem (publics); sym; sym = setNextItem (publics))
		fprintf(afile, "\tglobal %s\n", sym->rname);
}

/*-----------------------------------------------------------------*/
/* printExterns - generates extern declarations for externs        */
/*-----------------------------------------------------------------*/
static void
pic16_printExterns(FILE *afile)
{
  symbol *sym;

	fprintf(afile, "%s", iComments2);
	fprintf(afile, "; extern variables to this module\n");
	fprintf(afile, "%s", iComments2);
	
	for(sym = setFirstItem(externs); sym; sym = setNextItem(externs))
		fprintf(afile, "\textern %s\n", sym->rname);
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
	      && !IS_STATIC (sym->etype))
	    addSetHead (&publics, sym);

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

  addSetHead(&tmpfileSet,ovrFile);
  pic16_pCodeInitRegisters();

  if (mainf && IFFUNC_HASBODY(mainf->type)) {

    pBlock *pb = pic16_newpCodeChain(NULL,'X',pic16_newpCodeCharP("; Starting pCode block"));
    pic16_addpBlock(pb);

    /* entry point @ start of CSEG */
    pic16_addpCode2pBlock(pb,pic16_newpCodeLabel("__sdcc_program_startup",-1));
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

#if STACK_SUPPORT
	if(USE_STACK) {
	  pBlock *pb = pic16_newpCodeChain(NULL, 'X', pic16_newpCodeCharP("; Setup stack & frame register"));

		pic16_addpBlock(pb);
		pic16_addpCode2pBlock(pb, pic16_newpCode(POC_LFSR, pic16_popGetLit2(1, stackPos)));
		pic16_addpCode2pBlock(pb, pic16_newpCode(POC_LFSR, pic16_popGetLit2(2, stackPos)));

	}
#endif


  /* At this point we've got all the code in the form of pCode structures */
  /* Now it needs to be rearranged into the order it should be placed in the */
  /* code space */

  pic16_movepBlock2Head('P');              // Last
  pic16_movepBlock2Head(code->dbName);
  pic16_movepBlock2Head('X');
  pic16_movepBlock2Head(statsg->dbName);   // First


  /* print the global struct definitions */
  if (options.debug)
    cdbStructBlock (0);	//,cdbFile);

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

#if 0
	{
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
  pic16_pcode_test();

  /* now put it all together into the assembler file */
  /* create the assembler file name */
    
  if ((noAssemble || options.c1mode)  && fullDstFileName)
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
    
  /* initial comments */
  pic16initialComments (asmFile);
    
  /* print module name */
  fprintf (asmFile, ";\t.module %s\n", moduleName);
    
  /* Let the port generate any global directives, etc. */
  if (port->genAssemblerPreamble)
    {
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

  /* create the overlay segments */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; overlayable items in internal ram \n");
  fprintf (asmFile, "%s", iComments2);    
  copyFile (asmFile, ovrFile);

  /* create the stack segment MOF */
  if (mainf && IFFUNC_HASBODY(mainf->type)) {
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; Stack segment in internal ram \n");
    fprintf (asmFile, "%s", iComments2);    
    fprintf (asmFile, ";\t.area\tSSEG\t(DATA)\n"
    	";__start__stack:\n;\t.ds\t1\n\n");
  }

#if 0
	/* no indirect data in pic */
  /* create the idata segment */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; indirectly addressable internal ram data\n");
  fprintf (asmFile, "%s", iComments2);
  copyFile (asmFile, idata->oFile);
#endif


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

  /* copy the bit segment */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; bit data\n");
  fprintf (asmFile, "%s", iComments2);
  copyFile (asmFile, bit->oFile);


/* the following is commented out. the CODE directive will be
   used instead before code */
//  fprintf (asmFile, "\tORG 0\n");

  /* copy the interrupt vector table */
  if (mainf && IFFUNC_HASBODY(mainf->type)) {
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
  /* FIXME 8051 Legacy -- VR */
  /* Everywhere we generate a reference to the static_name area, 
   * (which is currently only here), we immediately follow it with a 
   * definition of the post_static_name area. This guarantees that
   * the post_static_name area will immediately follow the static_name
   * area.
   */
  fprintf (asmFile, ";\t.area %s\n", port->mem.static_name); /* MOF */
  fprintf (asmFile, ";\t.area %s\n", port->mem.post_static_name);
  fprintf (asmFile, ";\t.area %s\n", port->mem.static_name);
#endif

  if (mainf && IFFUNC_HASBODY(mainf->type)) {
    fprintf (asmFile,"__sdcc_gsinit_startup:\n");

#if 0
	/* FIXME 8051 legacy (?!) - VR 20-Jun-2003 */
    /* if external stack is specified then the
       higher order byte of the xdatalocation is
       going into P2 and the lower order going into
       spx */
    if (options.useXstack) {
      fprintf(asmFile,";\tmov\tP2,#0x%02x\n",
	      (((unsigned int)options.xdata_loc) >> 8) & 0xff);
      fprintf(asmFile,";\tmov\t_spx,#0x%02x\n",
	      (unsigned int)options.xdata_loc & 0xff);
    }
#endif

  }

  /* copy over code */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "\tcode\n");
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, ";\t.area %s\n", port->mem.code_name);

  if (port->general.glue_up_main && mainf && IFFUNC_HASBODY(mainf->type))
    {
      /* This code is generated in the post-static area.
       * This area is guaranteed to follow the static area
       * by the ugly shucking and jiving about 20 lines ago.
       */
//      fprintf(asmFile, ";\t.area %s\n", port->mem.post_static_name);
      fprintf (asmFile,"\tgoto\t__sdcc_program_startup\n");
    }
	

  //copyFile (stderr, code->oFile);

		fprintf(asmFile, "; I code from now on!\n");
	pic16_copypCode(asmFile, 'I');

	
//		fprintf(asmFile, "; dbName from now on!\n");
	fprintf(asmFile, "__sdcc_program_startup:\n");
	pic16_copypCode(asmFile, statsg->dbName);

		fprintf(asmFile, "; X code from now on!\n");
	pic16_copypCode(asmFile, 'X');

		fprintf(asmFile, "; M code from now on!\n");
	pic16_copypCode(asmFile, 'M');

	pic16_copypCode(asmFile, code->dbName);

	pic16_copypCode(asmFile, 'P');

	fprintf (asmFile,"\tend\n");

	fclose (asmFile);

	rm_tmpfiles();
}
