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

#include "common.h"
#include <time.h>

symbol *interrupts[256];
extern char *aopLiteral (value *, int);
void printIval (symbol *, link *, initList *, FILE *);
extern int noAlloc;
set *publics = NULL;		/* public variables */
int maxInterrupts = 6;
extern int maxRegBank ;
symbol *mainf;
extern char *VersionString;
extern FILE *codeOutFile;
set *tmpfileSet = NULL; /* set of tmp file created by the compiler */
/*-----------------------------------------------------------------*/
/* closeTmpFiles - closes all tmp files created by the compiler    */
/*                 because of BRAIN DEAD MS/DOS & CYGNUS Libraries */
/*-----------------------------------------------------------------*/
DEFSETFUNC(closeTmpFiles)
{
    FILE *tfile = item;

    if (tfile)
	fclose(tfile);
    
    return 0;
}

/*-----------------------------------------------------------------*/
/* copyFile - copies source file to destination file               */
/*-----------------------------------------------------------------*/
void copyFile (FILE * dest, FILE * src)
{
    int ch;
    
    rewind (src);
    while (!feof (src))
	if ((ch = fgetc (src)) != EOF)
	    fputc (ch, dest);
}

/*-----------------------------------------------------------------*/
/* emitRegularMap - emit code for maps with no special cases       */
/*-----------------------------------------------------------------*/
static void emitRegularMap (memmap * map, bool addPublics, bool arFlag)
{
    symbol *sym;
    
    if (addPublics)
	fprintf (map->oFile, "\t.area\t%s\n", map->sname);
    
    /* print the area name */
    for (sym = setFirstItem (map->syms); sym;
	 sym = setNextItem (map->syms))  {
	
	/* if extern then do nothing */
	if (IS_EXTERN (sym->etype))
	    continue;
	
	/* if allocation required check is needed
	   then check if the symbol really requires
	   allocation only for local variables */
	if (arFlag && !IS_AGGREGATE(sym->type) &&
	    !(sym->_isparm && !IS_REGPARM(sym->etype)) && 
	      !sym->allocreq && sym->level)
	    continue ;
	
	/* if global variable & not static or extern 
	   and addPublics allowed then add it to the public set */
	if ((sym->level == 0 || 
	     (sym->_isparm && !IS_REGPARM(sym->etype))) &&
	    addPublics &&
	    !IS_STATIC (sym->etype))
	    addSetHead (&publics, sym);
	
	/* if extern then do nothing or is a function 
	   then do nothing */
	if (IS_FUNC (sym->type))
	    continue;
	
	/* print extra debug info if required */
	if (options.debug || sym->level == 0) {

	    cdbSymbol(sym,cdbFile,FALSE,FALSE);

	    if (!sym->level) /* global */
		if (IS_STATIC(sym->etype))
		    fprintf(map->oFile,"F%s$",moduleName); /* scope is file */
		else
		    fprintf(map->oFile,"G$"); /* scope is global */
	    else
		/* symbol is local */
		fprintf(map->oFile,"L%s$",(sym->localof ? sym->localof->name : "-null-"));
	    fprintf(map->oFile,"%s$%d$%d",sym->name,sym->level,sym->block);
	}

	/* if is has an absolute address then generate
	   an equate for this no need to allocate space */
	if (SPEC_ABSA (sym->etype)) {
	    if (options.debug || sym->level == 0)
		fprintf (map->oFile," == 0x%04x\n",SPEC_ADDR (sym->etype));	    

	    fprintf (map->oFile, "%s\t=\t0x%04x\n",
		     sym->rname,
		     SPEC_ADDR (sym->etype));
	}
	else {
	    /* allocate space */
	    if (options.debug || sym->level == 0)
		fprintf(map->oFile,"==.\n");
	    fprintf (map->oFile, "%s:\n", sym->rname);
	    fprintf (map->oFile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
	}
	
	/* if it has a initial value then do it only if
	   it is a global variable */
	if (sym->ival && sym->level == 0) {
	    ast *ival = NULL;
	    
	    if (IS_AGGREGATE (sym->type))
		ival = initAggregates (sym, sym->ival, NULL);
	    else
		ival = newNode ('=', newAst (EX_VALUE, symbolVal (sym)),
				decorateType (resolveSymbols (list2expr (sym->ival))));
	    codeOutFile = statsg->oFile;
	    eBBlockFromiCode (iCodeFromAst (ival));
	    sym->ival = NULL;
	}
    }
}


/*-----------------------------------------------------------------*/
/* initPointer - pointer initialization code massaging             */
/*-----------------------------------------------------------------*/
value *initPointer (initList *ilist)
{
    value *val;
    ast *expr = list2expr(ilist);

    if (!expr) 
	goto wrong; 	        
	
    /* try it the oldway first */
    if ((val = constExprValue(expr,FALSE)))
	return val;

    /* no then we have to do these cludgy checks */
    /* pointers can be initialized with address of
       a variable or address of an array element */
    if (IS_AST_OP(expr) && expr->opval.op == '&') {
	/* address of symbol */
	if (IS_AST_SYM_VALUE(expr->left)) {
	    val = copyValue(AST_VALUE(expr->left));
	    val->type = newLink();
	    if (SPEC_SCLS(expr->left->etype) == S_CODE) {
		DCL_TYPE(val->type) = CPOINTER ;
		DCL_PTR_CONST(val->type) = 1;
	    }
	    else
		if (SPEC_SCLS(expr->left->etype) == S_XDATA)
		    DCL_TYPE(val->type) = FPOINTER;
		else
		    if (SPEC_SCLS(expr->left->etype) == S_XSTACK )
			DCL_TYPE(val->type) = PPOINTER ;
		    else
			if (SPEC_SCLS(expr->left->etype) == S_IDATA)
			    DCL_TYPE(val->type) = IPOINTER ;
			else
			    DCL_TYPE(val->type) = POINTER ;
	    val->type->next = expr->left->ftype;
	    val->etype = getSpec(val->type);
	    return val;
	}

	/* if address of indexed array */
	if (IS_AST_OP(expr->left) && expr->left->opval.op == '[')
	    return valForArray(expr->left);	

	/* if address of structure element then 
	   case 1. a.b ; */
	if (IS_AST_OP(expr->left) && 
	    expr->left->opval.op == '.' ) {
		return valForStructElem(expr->left->left,
					expr->left->right);
	}

	/* case 2. (&a)->b ; 
	   (&some_struct)->element */
	if (IS_AST_OP(expr->left) &&
	    expr->left->opval.op == PTR_OP &&
	    IS_ADDRESS_OF_OP(expr->left->left))
		return valForStructElem(expr->left->left->left,
					expr->left->right);	
    }

 wrong:    
    werror(E_INIT_WRONG);
    return NULL;
    
}

/*-----------------------------------------------------------------*/
/* printChar - formats and prints a characater string with DB      */
/*-----------------------------------------------------------------*/
void printChar (FILE * ofile, char *s, int plen)
{
    int i;
    int len = strlen (s);
    int pplen = 0;
    
    while (len && pplen < plen) {

	fprintf (ofile, "\t.ascii /");
	i = 60;
	while (i && *s && pplen < plen) {
	    if (*s < ' ' || *s == '/') {	       
		fprintf (ofile, "/\n\t.byte 0x%02x\n\t.ascii /", *s++);
	    }
	    else 
		fprintf (ofile, "%c", *s++);
	    pplen++;
	    i--;
	}
	fprintf (ofile, "/\n");
	
	if (len > 60)
	    len -= 60;
	else
	    len = 0;
    }
    if (pplen < plen)
	fprintf(ofile,"\t.byte\t0\n");
}

/*-----------------------------------------------------------------*/
/* printIvalType - generates ival for int/char                     */
/*-----------------------------------------------------------------*/
void printIvalType (link * type, initList * ilist, FILE * oFile)
{
    value *val;
    
    /* if initList is deep */
    if (ilist->type == INIT_DEEP)
	ilist = ilist->init.deep;
    
    val = list2val (ilist);
    switch (getSize (type)) {
    case 1:
	if (!val)
	    fprintf (oFile, "\t.byte 0\n");
	else
	    fprintf (oFile, "\t.byte %s\n",
		     aopLiteral (val, 0));
	break;

    case 2:
	if (!val)
	    fprintf (oFile, "\t.word 0\n");
	else
	    fprintf (oFile, "\t.byte %s,%s\n",
		     aopLiteral (val, 0), aopLiteral (val, 1));
	break;

    case 4:
	if (!val)
	    fprintf (oFile, "\t.word 0,0\n");
	else
	    fprintf (oFile, "\t.byte %s,%s,%s,%s\n",
		     aopLiteral (val, 0), aopLiteral (val, 1),
		     aopLiteral (val, 2), aopLiteral (val, 3));
	break;
    }
    
    return;
}

/*-----------------------------------------------------------------*/
/* printIvalStruct - generates initial value for structures        */
/*-----------------------------------------------------------------*/
void printIvalStruct (symbol * sym,link * type,
		      initList * ilist, FILE * oFile)
{
    symbol *sflds;
    initList *iloop;
    
    sflds = SPEC_STRUCT (type)->fields;
    if (ilist->type != INIT_DEEP) {
	werror (E_INIT_STRUCT, sym->name);
	return;
    }
    
    iloop = ilist->init.deep;
    
    for (; sflds; sflds = sflds->next, iloop = (iloop ? iloop->next : NULL))
	printIval (sflds, sflds->type, iloop, oFile);
    
    return;
}

/*-----------------------------------------------------------------*/
/* printIvalChar - generates initital value for character array    */
/*-----------------------------------------------------------------*/
int printIvalChar (link * type, initList * ilist, FILE * oFile, char *s)
{
    value *val;
    int remain;
    
    if (!s) {
	
	val = list2val (ilist);
	/* if the value is a character string  */
	if (IS_ARRAY (val->type) && IS_CHAR (val->etype)) {
	    if (!DCL_ELEM (type))
		DCL_ELEM (type) = strlen (SPEC_CVAL (val->etype).v_char) + 1;
	    
	    /* if size mismatch  */
/* 	    if (DCL_ELEM (type) < ((int) strlen (SPEC_CVAL (val->etype).v_char) + 1)) */
/* 		werror (E_ARRAY_BOUND); */
	    
	    printChar (oFile, SPEC_CVAL (val->etype).v_char,DCL_ELEM(type));
	    
	    if ((remain = (DCL_ELEM (type) - strlen (SPEC_CVAL (val->etype).v_char) -1))>0)
		while (remain--)
		    fprintf (oFile, "\t.byte 0\n");
	    
	    return 1;
	}
	else
	    return 0;
    }
    else
	printChar (oFile, s,strlen(s)+1);
    return 1;
}

/*-----------------------------------------------------------------*/
/* printIvalArray - generates code for array initialization        */
/*-----------------------------------------------------------------*/
void printIvalArray (symbol * sym, link * type, initList * ilist,
		     FILE * oFile)
{
    initList *iloop;
    int lcnt = 0, size = 0;
    
    /* take care of the special   case  */
    /* array of characters can be init  */
    /* by a string                      */
    if (IS_CHAR (type->next))
	if (printIvalChar (type,
			   (ilist->type == INIT_DEEP ? ilist->init.deep : ilist),
			   oFile, SPEC_CVAL (sym->etype).v_char))
	    return;
    
    /* not the special case             */
    if (ilist->type != INIT_DEEP) {
	werror (E_INIT_STRUCT, sym->name);
	return;
    }
    
    iloop = ilist->init.deep;
    lcnt = DCL_ELEM (type);
    
    for (;;) {
	size++;
	printIval (sym, type->next, iloop, oFile);
	iloop = (iloop ? iloop->next : NULL);
	
	
	/* if not array limits given & we */
	/* are out of initialisers then   */
	if (!DCL_ELEM (type) && !iloop)
	    break;
	
	/* no of elements given and we    */
	/* have generated for all of them */
	if (!--lcnt)
	    break;
    }
    
    /* if we have not been given a size  */
    if (!DCL_ELEM (type))
	DCL_ELEM (type) = size;
    
    return;
}

/*-----------------------------------------------------------------*/
/* printIvalFuncPtr - generate initial value for function pointers */
/*-----------------------------------------------------------------*/
void printIvalFuncPtr (link * type, initList * ilist, FILE * oFile)
{
    value *val;
    int dLvl = 0;
    
    val = list2val (ilist);
    /* check the types   */
    if ((dLvl = checkType (val->type, type->next)) <= 0) {
	
	fprintf (oFile, "\t.word 0\n");
	return;
    }
    
    /* now generate the name */
    if (!val->sym) {
	if (IS_LITERAL (val->etype))
	    fprintf (oFile, "\t.byte %s,%s\n",
		     aopLiteral (val, 0), aopLiteral (val, 1));
	else
	    fprintf (oFile, "\t.byte %s,(%s >> 8)\n",
		     val->name, val->name);
    }
    else 
	fprintf (oFile, "\t.byte %s,(%s >> 8)\n",
		 val->sym->rname, val->sym->rname);
    
    return;
}

/*-----------------------------------------------------------------*/
/* printIvalCharPtr - generates initial values for character pointers */
/*-----------------------------------------------------------------*/
int printIvalCharPtr (symbol * sym, link * type, value * val, FILE * oFile)
{
    int size = 0;
    
    size = getSize (type);
    
    if (size == 1)
	fprintf(oFile,
	     "\t.byte %s", val->name) ;
    else
	fprintf (oFile,
		 "\t.byte %s,(%s >> 8)",
		 val->name, val->name);
   
    if (size > 2)
	fprintf (oFile, ",#0x02\n");
    else
	fprintf (oFile, "\n");
    
    if (val->sym && val->sym->isstrlit)
	addSet (&statsg->syms, val->sym);
    
    return 1;
}

/*-----------------------------------------------------------------*/
/* printIvalPtr - generates initial value for pointers             */
/*-----------------------------------------------------------------*/
void printIvalPtr (symbol * sym, link * type, initList * ilist, FILE * oFile)
{
    value *val;
    
    /* if deep then   */
    if (ilist->type == INIT_DEEP)
	ilist = ilist->init.deep;
    
    /* function pointer     */
    if (IS_FUNC (type->next)) {
	printIvalFuncPtr (type, ilist, oFile);
	return;
    }
    
    if (!(val = initPointer (ilist)))
	return ;

    /* if character pointer */
    if (IS_CHAR (type->next))
	if (printIvalCharPtr (sym, type, val, oFile))
	    return;
    
    /* check the type      */
    if (checkType (type, val->type) != 1)
	werror (E_INIT_WRONG);
    
    /* if val is literal */
    if (IS_LITERAL (val->etype)) {
	switch (getSize (type)) {
	case 1:
	    fprintf (oFile, "\t.byte 0x%02x\n", ((char) floatFromVal (val)) & 0xff);
	    break;
	case 2:
	    fprintf (oFile, "\t.byte %s,%s\n",
		     aopLiteral (val, 0), aopLiteral (val, 1));
	    
	    break;
	case 3:
	    fprintf (oFile, "\t.byte %s,%s,0x%02x\n",
		     aopLiteral (val, 0), aopLiteral (val, 1), CPOINTER);
	}
	return;
    }
    
    
    switch (getSize (type)) {
    case 1:
	fprintf (oFile, "\t.byte %s\n", val->name);
	break;
    case 2:
	fprintf (oFile, "\t.byte %s,(%s >> 8)\n", val->name, val->name);
	break;
	
    case 3:
	fprintf (oFile, "\t.byte %s,(%s >> 8),0x%02x\n",
		 val->name, val->name, DCL_TYPE(val->type));
    }
    return;
}

/*-----------------------------------------------------------------*/
/* printIval - generates code for initial value                    */
/*-----------------------------------------------------------------*/
void printIval (symbol * sym, link * type, initList * ilist, FILE * oFile)
{
    if (!ilist)
	return;    
    
    /* if structure then    */
    if (IS_STRUCT (type)) {
	printIvalStruct (sym, type, ilist, oFile);
	return;
    }
    
    /* if this is a pointer */
    if (IS_PTR (type)) {
	printIvalPtr (sym, type, ilist, oFile);
	return;
    }
    
    /* if this is an array   */
    if (IS_ARRAY (type)) {
	printIvalArray (sym, type, ilist, oFile);
	return;
    }
    
    /* if type is SPECIFIER */
    if (IS_SPEC (type)) {
	printIvalType (type, ilist, oFile);
	return;
    }
}

/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
void emitStaticSeg (memmap * map)
{
    symbol *sym;
    
    /*     fprintf(map->oFile,"\t.area\t%s\n",map->sname); */
    
    
    /* for all variables in this segment do */
    for (sym = setFirstItem (map->syms); sym;
	 sym = setNextItem (map->syms)) {
	
	/* if it is "extern" then do nothing */
	if (IS_EXTERN (sym->etype))
	    continue;
	
	/* if it is not static add it to the public
	   table */
	if (!IS_STATIC (sym->etype))
	    addSetHead (&publics, sym);

	/* print extra debug info if required */
	if (options.debug || sym->level == 0) {

	    cdbSymbol(sym,cdbFile,FALSE,FALSE);

	    if (!sym->level) { /* global */
		if (IS_STATIC(sym->etype))
		    fprintf(code->oFile,"F%s$",moduleName); /* scope is file */
		else
		    fprintf(code->oFile,"G$"); /* scope is global */
	    }
	    else
		/* symbol is local */
		fprintf(code->oFile,"L%s$",
			(sym->localof ? sym->localof->name : "-null-"));
	    fprintf(code->oFile,"%s$%d$%d",sym->name,sym->level,sym->block);
	}
	
	/* if it has an absolute address */
	if (SPEC_ABSA (sym->etype)) {
	    if (options.debug || sym->level == 0)
		fprintf(code->oFile," == 0x%04x\n", SPEC_ADDR (sym->etype));

	    fprintf (code->oFile, "%s\t=\t0x%04x\n",
		     sym->rname,
		     SPEC_ADDR (sym->etype));
	}
	else {
	    if (options.debug || sym->level == 0)
		fprintf(code->oFile," == .\n");	

	    /* if it has an initial value */
	    if (sym->ival) {
		fprintf (code->oFile, "%s:\n", sym->rname);
		noAlloc++;
		resolveIvalSym (sym->ival);
		printIval (sym, sym->type, sym->ival, code->oFile);
		noAlloc--;
	    }
	    else {
		/* allocate space */
		fprintf (code->oFile, "%s:\n", sym->rname);
		/* special case for character strings */
		if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
		    SPEC_CVAL (sym->etype).v_char)
		    printChar (code->oFile,
			       SPEC_CVAL (sym->etype).v_char,
			       strlen(SPEC_CVAL (sym->etype).v_char)+1);
		else
		    fprintf (code->oFile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type)& 0xffff);
	    }
	}
    }
}

/*-----------------------------------------------------------------*/
/* emitMaps - emits the code for the data portion the code         */
/*-----------------------------------------------------------------*/
void emitMaps ()
{
    /* no special considerations for the following
       data, idata & bit & xdata */
    emitRegularMap (data, TRUE, TRUE);
    emitRegularMap (idata, TRUE,TRUE);
    emitRegularMap (bit, TRUE,FALSE);
    emitRegularMap (xdata, TRUE,TRUE);
    emitRegularMap (sfr, FALSE,FALSE);
    emitRegularMap (sfrbit, FALSE,FALSE);
    emitRegularMap (code, TRUE,FALSE);
    emitStaticSeg (statsg);
}

/*-----------------------------------------------------------------*/
/* createInterruptVect - creates the interrupt vector              */
/*-----------------------------------------------------------------*/
void createInterruptVect (FILE * vFile)
{
    int i = 0;
    mainf = newSymbol ("main", 0);
    mainf->block = 0;
    
    /* only if the main function exists */
    if (!(mainf = findSymWithLevel (SymbolTab, mainf))) {
	if (!options.cc_only)
	    werror(E_NO_MAIN);
	return;
    }
    
    /* if the main is only a prototype ie. no body then do nothing */
    if (!mainf->fbody) {
	/* if ! compile only then main function should be present */
	if (!options.cc_only)
	    werror(E_NO_MAIN);
	return;
    }
    
    fprintf (vFile, "\t.area\tCODE (CODE)\n");
    fprintf (vFile, "__interrupt_vect:\n");
    
    fprintf (vFile, "\tljmp\t__sdcc_gsinit_startup\n");
    
    
    /* now for the other interrupts */
    for (; i < maxInterrupts; i++) {
	if (interrupts[i])
	    fprintf (vFile, "\tljmp\t%s\n\t.ds\t5\n", interrupts[i]->rname);
	else
	    fprintf (vFile, "\treti\n\t.ds\t7\n");
    }
}

char *iComments1 =
{
    ";--------------------------------------------------------\n"
    "; File Created by SDCC : FreeWare ANSI-C Compiler\n"};

char *iComments2 =
{
    ";--------------------------------------------------------\n"};


/*-----------------------------------------------------------------*/
/* initialComments - puts in some initial comments                 */
/*-----------------------------------------------------------------*/
void initialComments (FILE * afile)
{
    time_t t;
    time(&t);
    fprintf (afile, "%s", iComments1);
    fprintf (afile, "; Version %s %s\n", VersionString,asctime(localtime(&t)));
    fprintf (afile, "%s", iComments2);
}

/*-----------------------------------------------------------------*/
/* printPublics - generates .global for publics                    */
/*-----------------------------------------------------------------*/
void printPublics (FILE * afile)
{
    symbol *sym;
    
    fprintf (afile, "%s", iComments2);
    fprintf (afile, "; publics variables in this module\n");
    fprintf (afile, "%s", iComments2);
    
    for (sym = setFirstItem (publics); sym;
	 sym = setNextItem (publics))
	fprintf (afile, "\t.globl %s\n", sym->rname);
}

/*-----------------------------------------------------------------*/
/* emitOverlay - will emit code for the overlay stuff              */
/*-----------------------------------------------------------------*/
static void emitOverlay(FILE *afile)
{
    set *ovrset;
    
    if (!elementsInSet(ovrSetSets))
	fprintf(afile,"\t.area\tOSEG\t(OVR,DATA)\n");

    /* for each of the sets in the overlay segment do */
    for (ovrset = setFirstItem(ovrSetSets); ovrset;
	 ovrset = setNextItem(ovrSetSets)) {

	symbol *sym ;

	if (elementsInSet(ovrset)) {
	    /* this dummy area is used to fool the assembler
	       otherwise the assembler will append each of these
	       declarations into one chunk and will not overlay 
	       sad but true */
	    fprintf(afile,"\t.area _DUMMY\n");
	    /* output the area informtion */
	    fprintf(afile,"\t.area\tOSEG\t(OVR,DATA)\n"); /* MOF */
	}
	
	for (sym = setFirstItem(ovrset); sym;
	     sym = setNextItem(ovrset)) {
	
	    /* if extern then do nothing */
	    if (IS_EXTERN (sym->etype))
		continue;
	    
	    /* if allocation required check is needed
	       then check if the symbol really requires
	       allocation only for local variables */
	    if (!IS_AGGREGATE(sym->type) &&
		!(sym->_isparm && !IS_REGPARM(sym->etype))
		&& !sym->allocreq && sym->level)
		continue ;
	    
	    /* if global variable & not static or extern 
	       and addPublics allowed then add it to the public set */
	    if ((sym->_isparm && !IS_REGPARM(sym->etype))
		&& !IS_STATIC (sym->etype))
		addSetHead (&publics, sym);
	    
	    /* if extern then do nothing or is a function 
	       then do nothing */
	    if (IS_FUNC (sym->type))
		continue;

	    /* print extra debug info if required */
	    if (options.debug || sym->level == 0) {
		
		cdbSymbol(sym,cdbFile,FALSE,FALSE);
		
		if (!sym->level) { /* global */
		    if (IS_STATIC(sym->etype))
			fprintf(afile,"F%s$",moduleName); /* scope is file */
		    else
			fprintf(afile,"G$"); /* scope is global */
		}
		else
		    /* symbol is local */
		    fprintf(afile,"L%s$",
			    (sym->localof ? sym->localof->name : "-null-"));
		fprintf(afile,"%s$%d$%d",sym->name,sym->level,sym->block);
	    }
	    
	    /* if is has an absolute address then generate
	       an equate for this no need to allocate space */
	    if (SPEC_ABSA (sym->etype)) {
		
		if (options.debug || sym->level == 0)
		    fprintf (afile," == 0x%04x\n",SPEC_ADDR (sym->etype));	    

		fprintf (afile, "%s\t=\t0x%04x\n",
			 sym->rname,
			 SPEC_ADDR (sym->etype));
	    }
	    else {
		if (options.debug || sym->level == 0)
		    fprintf(afile,"==.\n");
	
		/* allocate space */
		fprintf (afile, "%s:\n", sym->rname);
		fprintf (afile, "\t.ds\t0x%04x\n", (unsigned int)getSize (sym->type) & 0xffff);
	    }
	    
	}
    }
}

/*-----------------------------------------------------------------*/
/* glue - the final glue that hold the whole thing together        */
/*-----------------------------------------------------------------*/
void glue ()
{
    FILE *vFile;
    FILE *asmFile;
    FILE *ovrFile = tmpfile();
    
    addSetHead(&tmpfileSet,ovrFile);
    /* print the global struct definitions */
    if (options.debug)
	cdbStructBlock (0,cdbFile);

    /* create the interrupt vector table */
    createInterruptVect ((vFile = tmpfile ()));
    addSetHead(&tmpfileSet,vFile);
    
    /* emit code for the all the variables declared */
    emitMaps ();
    /* do the overlay segments */
    emitOverlay(ovrFile);

    /* now put it all together into the assembler file */
    /* create the assembler file name */
    sprintf (buffer, srcFileName);
    strcat (buffer, ".asm");
    if (!(asmFile = fopen (buffer, "w"))) {
	werror (E_FILE_OPEN_ERR, buffer);
	exit (1);
    }
    
    /* initial comments */
    initialComments (asmFile);
    
    /* print module name */
    fprintf (asmFile, "\t.module %s\n", moduleName);
    
    /* print the global variables in this module */
    printPublics (asmFile);

    
    /* copy the sfr segment */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; special function registers\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, sfr->oFile);
    
    /* copy the sbit segment */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; special function bits \n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, sfrbit->oFile);
    
    /* copy the data segment */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; internal ram data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, data->oFile);


    /* create the overlay segments */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; overlayable items in internal ram \n");
    fprintf (asmFile, "%s", iComments2);    
    copyFile (asmFile, ovrFile);

    /* create the stack segment MOF */
    if (mainf && mainf->fbody) {
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; Stack segment in internal ram \n");
	fprintf (asmFile, "%s", iComments2);    
	fprintf (asmFile, "\t.area\tSSEG\t(DATA)\n"
		 "__start__stack:\n\t.ds\t1\n\n");
    }

    /* create the idata segment */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; indirectly addressable internal ram data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, idata->oFile);
    
    /* copy the bit segment */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; bit data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, bit->oFile);

    /* if external stack then reserve space of it */
    if (mainf && mainf->fbody && options.useXstack ) {
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; external stack \n");
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile,"\t.area XSEG (XDATA)\n"); /* MOF */
	fprintf (asmFile,"\t.ds 256\n");
    }
	
	
    /* copy xtern ram data */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; external ram data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, xdata->oFile);
    
    /* copy the interrupt vector table */
    if (mainf && mainf->fbody) {
	fprintf (asmFile, "%s", iComments2);
	fprintf (asmFile, "; interrupt vector \n");
	fprintf (asmFile, "%s", iComments2);
	copyFile (asmFile, vFile);
    }
    
    /* copy global & static initialisations */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; global & static initialisations\n");
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "\t.area GSINIT (CODE)\n"); /* MOF */
    if (mainf && mainf->fbody) {
	fprintf (asmFile,"__sdcc_gsinit_startup:\n");
	/* if external stack is specified then the
	   higher order byte of the xdatalocation is
	   going into P2 and the lower order going into
	   spx */
	if (options.useXstack) {
	    fprintf(asmFile,"\tmov\tP2,#0x%02x\n",
		    (((unsigned int)options.xdata_loc) >> 8) & 0xff);
	    fprintf(asmFile,"\tmov\t_spx,#0x%02x\n",
		    (unsigned int)options.xdata_loc & 0xff);
	}

	/* initialise the stack pointer */
	/* if the user specified a value then use it */
	if (options.stack_loc) 
	    fprintf(asmFile,"\tmov\tsp,#%d\n",options.stack_loc);
	else 
	    /* no: we have to compute it */
	    if (!options.stackOnData && maxRegBank <= 3)
		fprintf(asmFile,"\tmov\tsp,#%d\n",((maxRegBank + 1) * 8) -1); 
	    else
		fprintf(asmFile,"\tmov\tsp,#__start__stack\n"); /* MOF */

	fprintf (asmFile,"\tlcall\t__sdcc_external_startup\n");
	fprintf (asmFile,"\tmov\ta,dpl\n");
	fprintf (asmFile,"\tjz\t__sdcc_init_data\n");
	fprintf (asmFile,"\tljmp\t__sdcc_program_startup\n");
	fprintf (asmFile,"__sdcc_init_data:\n");
	
    }
    copyFile (asmFile, statsg->oFile);
    
    /* copy over code */
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; code\n");
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "\t.area CSEG (CODE)\n");
    if (mainf && mainf->fbody) {
	
	/* entry point @ start of CSEG */
	fprintf (asmFile,"__sdcc_program_startup:\n");
	
	/* put in the call to main */
	fprintf(asmFile,"\tlcall\t_main\n");
	if (options.mainreturn) {

	    fprintf(asmFile,";\treturn from main ; will return to caller\n");
	    fprintf(asmFile,"\tret\n");

	} else {
	           
	    fprintf(asmFile,";\treturn from main will lock up\n");
	    fprintf(asmFile,"\tsjmp	.\n");
	}
    }
    copyFile (asmFile, code->oFile);
    
    fclose (asmFile);
    applyToSet(tmpfileSet,closeTmpFiles);
}
