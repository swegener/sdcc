

#include "common.h"


/*************************************************************
 *
 *
 *
 *
 *************************************************************/

int cdbOpenFile(char *file);
int cdbCloseFile(void);
int cdbWriteFunction(symbol *pSym);
int cdbWriteSymbol(symbol *pSym);
int cdbWriteType(structdef *sdef, int block, int inStruct, char *tag);
int cdbWriteModule(char *name);
int cdbWriteCLine(char *module, int Line, int Level, int Block);
int cdbWriteALine(char *module, int Line);
int cdbWriteBasicSymbol(symbol *sym, int isStructSym, int isFunc);
void cdbTypeInfo (sym_link * type);
     

DEBUGFILE cdbDebugFile = 
  {
    &cdbOpenFile,
    &cdbCloseFile,
    &cdbWriteModule,
    &cdbWriteFunction,
    &cdbWriteSymbol,
    &cdbWriteType,
    &cdbWriteCLine,
    &cdbWriteALine
  };

FILE *cdbFilePtr = NULL;
char *cdbModuleName = NULL;

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbOpenFile(char *file)
{
  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbOpenFile(%s)\n", file);

  return (cdbFilePtr = fopen(file, "w")) ? 1 : 0; 
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/
int cdbCloseFile(void)
{
  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbCloseFile()\n");

  if(!cdbFilePtr) return 0;
 
  fclose(cdbFilePtr);
  cdbFilePtr = NULL;
  cdbModuleName = NULL;

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteFunction(symbol *pSym)
{
  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbWriteFunction()\n");


  if(!cdbFilePtr) return 0;

  return cdbWriteBasicSymbol(pSym, FALSE, TRUE);
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteSymbol(symbol *pSym)
{
  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbWriteSymbol()\n");

  if(!cdbFilePtr) return 0;

  return cdbWriteBasicSymbol(pSym, FALSE, FALSE);
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteType(structdef *sdef, int block, int inStruct, char *tag)
{
  symbol *sym;

  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbWriteType()\n");

  if(!cdbFilePtr) return 0;

  fprintf (cdbFilePtr, "T:");

  /* if block # then must have function scope */
  fprintf (cdbFilePtr, "F%s$", moduleName);

  fprintf (cdbFilePtr, "%s[", (tag ? tag : sdef->tag));

  for (sym = sdef->fields; sym; sym = sym->next)
    {
      fprintf (cdbFilePtr, "({%d}", sym->offset);
      cdbWriteBasicSymbol(sym, TRUE, FALSE);
      fprintf(cdbFilePtr, ")");
    }

  fprintf (cdbFilePtr, "]");

  if (!inStruct)
    fprintf (cdbFilePtr, "\n");

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteModule(char *name)
{
  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbWriteModule()\n");

  if(!cdbFilePtr) return 0;
  cdbModuleName = name;

  fprintf(cdbFilePtr, "M:%s\n", cdbModuleName);

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/
int cdbWriteCLine(char *module, int Line, int Level, int Block)
{
  if(!cdbFilePtr) return 0;

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteALine(char *module, int Line)
{
  if(!cdbFilePtr) return 0;

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

int cdbWriteBasicSymbol(symbol *sym, int isStructSym, int isFunc)
{
  memmap *map;

  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "cdbFile.c:cdbWriteBasicSymbol()\n");

  if(!cdbFilePtr) return 0;

  if (!sym) return 0;

  /* WRITE HEADER, Function or Symbol */
  if (isFunc)
    fprintf (cdbFilePtr, "F:");   
  else
    fprintf (cdbFilePtr, "S:");

  /* STRUCTS do not have scope info.. */

  if (!isStructSym)
    {
      if (!sym->level)
	{
	  /* global */
	  if (IS_STATIC (sym->etype))
	    fprintf (cdbFilePtr, "F%s$", moduleName);	/* scope is file */
	  else
	    fprintf (cdbFilePtr, "G$");	/* scope is global */
	}
      else
	/* symbol is local */
	fprintf (cdbFilePtr, "L%s$", (sym->localof ? sym->localof->name : "-null-"));
    }
  else
    fprintf (cdbFilePtr, "S$");		/* scope is structure */

  /* print the name, & mangled name */
  fprintf (cdbFilePtr, "%s$%d$%d(", sym->name,
	   sym->level, sym->block);

  cdbTypeInfo (sym->type);

  fprintf (cdbFilePtr, "),");

  /* CHECK FOR REGISTER SYMBOL... */ 
  if(sym->reqv)
  {
    int a;
    symbol *TempSym = OP_SYMBOL (sym->reqv);

    fprintf(cdbFilePtr, "R,0,0,[");

    for(a = 0; a < 4; a++)
      if(TempSym->regs[a])     
	fprintf(cdbFilePtr, "%s%s", port->getRegName(TempSym->regs[a]),
		((a < 3) && (TempSym->regs[a+1])) ? "," : "");

    fprintf(cdbFilePtr, "]");
  }
  else
  {
    /* print the address space */
    map = SPEC_OCLS (sym->etype);

    fprintf (cdbFilePtr, "%c,%d,%d",
	     (map ? map->dbName : 'Z'), sym->onStack, SPEC_STAK (sym->etype));
  }

  /* if assigned to registers then output register names */
  /* if this is a function then print
     if is it an interrupt routine & interrupt number
     and the register bank it is using */
  if (isFunc)
    fprintf (cdbFilePtr, ",%d,%d,%d", FUNC_ISISR (sym->type),
	     FUNC_INTNO (sym->type), FUNC_REGBANK (sym->type));
  

/* alternate location to find this symbol @ : eg registers
     or spillication */

  if (!isStructSym)
    fprintf (cdbFilePtr, "\n");

  return 1;
}

/******************************************************************
 *
 *
 *
 *
 *****************************************************************/

/*-----------------------------------------------------------------*/
/* cdbTypeInfo - print the type information for debugger           */
/*-----------------------------------------------------------------*/
void cdbTypeInfo (sym_link * type)
{
  fprintf (cdbFilePtr, "{%d}", getSize (type));

  while (type)
    {
      if (IS_DECL (type))
	{
	  switch (DCL_TYPE (type))
	    {
	    case FUNCTION: fprintf (cdbFilePtr, "DF,"); break;
	    case GPOINTER: fprintf (cdbFilePtr, "DG,"); break;
	    case CPOINTER: fprintf (cdbFilePtr, "DC,"); break;
	    case FPOINTER: fprintf (cdbFilePtr, "DX,"); break;
	    case POINTER:  fprintf (cdbFilePtr, "DD,"); break;
	    case IPOINTER: fprintf (cdbFilePtr, "DI,"); break;
	    case PPOINTER: fprintf (cdbFilePtr, "DP,"); break;
	    case EEPPOINTER: fprintf (cdbFilePtr, "DA,"); break;
	    case ARRAY: fprintf (cdbFilePtr, "DA%d,", DCL_ELEM (type)); break;
	    default:
	      break;
	    }
	}
      else
	{
	  switch (SPEC_NOUN (type))
	    {
	    case V_INT:
	      if (IS_LONG (type))
		fprintf (cdbFilePtr, "SL");
	      else
		fprintf (cdbFilePtr, "SI");
	      break;

	    case V_CHAR: fprintf (cdbFilePtr, "SC"); break;
	    case V_VOID: fprintf (cdbFilePtr, "SV"); break;
	    case V_FLOAT: fprintf (cdbFilePtr, "SF"); break;
	    case V_STRUCT: 
	      fprintf (cdbFilePtr, "ST%s", SPEC_STRUCT (type)->tag); 
	      break;
	    
	    case V_SBIT: fprintf (cdbFilePtr, "SX"); break;
	    case V_BIT: 
	      fprintf (cdbFilePtr, "SB%d$%d", SPEC_BSTR (type), 
		       SPEC_BLEN (type));
	      break;

	    default:
	      break;
	    }
	  fputs (":", cdbFilePtr);
	  if (SPEC_USIGN (type))
	    fputs ("U", cdbFilePtr);
	  else
	    fputs ("S", cdbFilePtr);
	}
      type = type->next;
    }
}



















