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
#include "asm.h"
#include <time.h>
#include "newalloc.h"
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

symbol *interrupts[INTNO_MAX+1];

void printIval (symbol *, sym_link *, initList *, FILE *);
set *publics = NULL;            /* public variables */
set *externs = NULL;            /* Varibles that are declared as extern */

/* TODO: this should be configurable (DS803C90 uses more than 6) */
unsigned maxInterrupts = 6;
int allocInfo = 1;
symbol *mainf;
set *pipeSet = NULL;            /* set of pipes */
set *tmpfileSet = NULL;         /* set of tmp file created by the compiler */
set *tmpfileNameSet = NULL;     /* All are unlinked at close. */

/*-----------------------------------------------------------------*/
/* closePipes - closes all pipes created by the compiler           */
/*-----------------------------------------------------------------*/
DEFSETFUNC (closePipes)
{
  FILE *pfile = item;
  int ret;

  if (pfile) {
    ret = pclose (pfile);
    assert(ret != -1);
  }

  return 0;
}

/*-----------------------------------------------------------------*/
/* closeTmpFiles - closes all tmp files created by the compiler    */
/*                 because of BRAIN DEAD MS/DOS & CYGNUS Libraries */
/*-----------------------------------------------------------------*/
DEFSETFUNC (closeTmpFiles)
{
  FILE *tfile = item;
  int ret;

  if (tfile) {
    ret = fclose (tfile);
    assert(ret == 0);
  }

  return 0;
}

/*-----------------------------------------------------------------*/
/* rmTmpFiles - unlinks all tmp files created by the compiler      */
/*                 because of BRAIN DEAD MS/DOS & CYGNUS Libraries */
/*-----------------------------------------------------------------*/
DEFSETFUNC (rmTmpFiles)
{
  char *name = item;
  int ret;

  if (name) {
      ret = unlink (name);
      assert(ret == 0);
      Safe_free (name);
  }

  return 0;
}

/*-----------------------------------------------------------------*/
/* rm_tmpfiles - close and remove temporary files and delete sets  */
/*-----------------------------------------------------------------*/
void
rm_tmpfiles (void)
{
  /* close temporary files */
  applyToSet (pipeSet, closePipes);
  /* close temporary files */
  deleteSet (&pipeSet);

  applyToSet (tmpfileSet, closeTmpFiles);
  /* remove temporary files */
  applyToSet (tmpfileNameSet, rmTmpFiles);
  /* delete temorary file sets */
  deleteSet (&tmpfileSet);
  deleteSet (&tmpfileNameSet);
}

/*-----------------------------------------------------------------*/
/* copyFile - copies source file to destination file               */
/*-----------------------------------------------------------------*/
void 
copyFile (FILE * dest, FILE * src)
{
  int ch;

  rewind (src);
  while (!feof (src))
    if ((ch = fgetc (src)) != EOF)
      fputc (ch, dest);
}

char *
aopLiteralLong (value * val, int offset, int size)
{
  union {
    float f;
    unsigned char c[4];
  }
  fl;

  if (!val) {
    // assuming we have been warned before
    val=constVal("0");
  }

  /* if it is a float then it gets tricky */
  /* otherwise it is fairly simple */
  if (!IS_FLOAT (val->type)) {
    unsigned long v = (unsigned long) floatFromVal (val);

    v >>= (offset * 8);
    switch (size) {
    case 1:
      tsprintf (buffer, sizeof(buffer), 
          "!immedbyte", (unsigned int) v & 0xff);
      break;
    case 2:
      tsprintf (buffer, sizeof(buffer), 
          "!immedword", (unsigned int) v & 0xffff);
      break;
    default:
      /* Hmm.  Too big for now. */
      assert (0);
    }
    return Safe_strdup (buffer);
  }

  /* PENDING: For now size must be 1 */
  assert (size == 1);

  /* it is type float */
  fl.f = (float) floatFromVal (val);
#ifdef WORDS_BIGENDIAN
  tsprintf (buffer, sizeof(buffer), 
      "!immedbyte", fl.c[3 - offset]);
#else
  tsprintf (buffer, sizeof(buffer), 
      "!immedbyte", fl.c[offset]);
#endif
  return Safe_strdup (buffer);
}

/*-----------------------------------------------------------------*/
/* aopLiteral - string from a literal value                        */
/*-----------------------------------------------------------------*/
char *
aopLiteral (value * val, int offset)
{
  return aopLiteralLong (val, offset, 1);
}

/*-----------------------------------------------------------------*/
/* emitRegularMap - emit code for maps with no special cases       */
/*-----------------------------------------------------------------*/
static void 
emitRegularMap (memmap * map, bool addPublics, bool arFlag)
{
  symbol *sym;
  ast *ival = NULL;

  if (!map)
    return;

  if (addPublics)
    {
      /* PENDING: special case here - should remove */
      if (!strcmp (map->sname, CODE_NAME))
        tfprintf (map->oFile, "\t!areacode\n", map->sname);
      else if (!strcmp (map->sname, DATA_NAME))
        tfprintf (map->oFile, "\t!areadata\n", map->sname);
      else if (!strcmp (map->sname, HOME_NAME))
        tfprintf (map->oFile, "\t!areahome\n", map->sname);
      else
        tfprintf (map->oFile, "\t!area\n", map->sname);
    }
 
  for (sym = setFirstItem (map->syms); sym;
       sym = setNextItem (map->syms))
    {
      symbol *newSym=NULL;

      /* if extern then add it into the extern list */
      if (IS_EXTERN (sym->etype))
        {
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

      /* for bitvar locals and parameters */
      if (!arFlag && !sym->allocreq && sym->level 
          && !SPEC_ABSA (sym->etype)) {
        continue;
      }

      /* if global variable & not static or extern
         and addPublics allowed then add it to the public set */
      if ((sym->level == 0 ||
           (sym->_isparm && !IS_REGPARM (sym->etype))) &&
          addPublics &&
          !IS_STATIC (sym->etype) &&
          (IS_FUNC(sym->type) ? (sym->used || IFFUNC_HASBODY(sym->type)) : 1))
        {
          addSetHead (&publics, sym);
        }

      /* if extern then do nothing or is a function
         then do nothing */
      if (IS_FUNC (sym->type) && !(sym->isitmp))
        continue;

      /* print extra debug info if required */
      if (options.debug)
        {
          if (!sym->level) /* global */
            {
              if (IS_STATIC (sym->etype))
                fprintf (map->oFile, "F%s$", moduleName); /* scope is file */
              else
                fprintf (map->oFile, "G$");     /* scope is global */
            }
          else
            {
              /* symbol is local */
              fprintf (map->oFile, "L%s$", (sym->localof ? sym->localof->name : "-null-"));
            }
          fprintf (map->oFile, "%s$%d$%d", sym->name, sym->level, sym->block);
        }
      
      /* if it has an initial value then do it only if
         it is a global variable */
      if (sym->ival && sym->level == 0) {
        if (SPEC_OCLS(sym->etype)==xidata) {
          /* create a new "XINIT (CODE)" symbol, that will be emitted later
             in the static seg */
          newSym=copySymbol (sym);
          SPEC_OCLS(newSym->etype)=xinit;
          SNPRINTF (newSym->name, sizeof(newSym->name), "__xinit_%s", sym->name);
          SNPRINTF (newSym->rname, sizeof(newSym->rname), "__xinit_%s", sym->rname);
          if (IS_SPEC (newSym->type))
            SPEC_CONST (newSym->type) = 1;
          else
            DCL_PTR_CONST (newSym->type) = 1;
          SPEC_STAT(newSym->etype)=1;
          resolveIvalSym(newSym->ival, newSym->type);

          // add it to the "XINIT (CODE)" segment
          addSet(&xinit->syms, newSym);

          if (!SPEC_ABSA (sym->etype))
            {
              FILE *tmpFile = tempfile ();
              addSetHead (&tmpfileSet, tmpFile);
              // before allocation we must parse the sym->ival tree
              // but without actually generating initialization code
              noAlloc++;
              resolveIvalSym (sym->ival, sym->type);
              printIval (sym, sym->type, sym->ival, tmpFile);
              noAlloc--;
            }

          sym->ival=NULL;
        } else {
          if (IS_AGGREGATE (sym->type)) {
            ival = initAggregates (sym, sym->ival, NULL);
          } else {
            if (getNelements(sym->type, sym->ival)>1) {
              werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "scalar", 
                      sym->name);
            }
            ival = newNode ('=', newAst_VALUE (symbolVal (sym)),
                            decorateType (resolveSymbols (list2expr (sym->ival)), RESULT_CHECK));
          }
          codeOutFile = statsg->oFile;

          if (ival) {
            // set ival's lineno to where the symbol was defined
            setAstLineno (ival, lineno=sym->lineDef);
            // check if this is not a constant expression
            if (!constExprTree(ival)) {
              werror (E_CONST_EXPECTED, "found expression");
              // but try to do it anyway
            }
            allocInfo = 0;
            if (!astErrors(ival))
              eBBlockFromiCode (iCodeFromAst (ival));
            allocInfo = 1;
          }
        }         
        sym->ival = NULL;
      }

      /* if it has an absolute address then generate
         an equate for this no need to allocate space */
      if (SPEC_ABSA (sym->etype))
        {
          char *equ="=";
          if (options.debug) {
            fprintf (map->oFile, " == 0x%04x\n", SPEC_ADDR (sym->etype));
          }
          if (TARGET_IS_XA51) {
            if (map==sfr) {
              equ="sfr";
            } else if (map==bit || map==sfrbit) {
              equ="bit";
            }
          }
          fprintf (map->oFile, "%s\t%s\t0x%04x\n",
                   sym->rname, equ,
                   SPEC_ADDR (sym->etype));
        }
      else {
        int size = getAllocSize (sym->type);
        if (size==0) {
          werrorfl (sym->fileDef, sym->lineDef, E_UNKNOWN_SIZE, sym->name);
        }
        /* allocate space */
        if (options.debug) {
          fprintf (map->oFile, "==.\n");
        }
        if (IS_STATIC (sym->etype))
          tfprintf (map->oFile, "!slabeldef\n", sym->rname);
        else
          tfprintf (map->oFile, "!labeldef\n", sym->rname);           
        tfprintf (map->oFile, "\t!ds\n", 
                  (unsigned int)  size & 0xffff);
      }
    }
}

/*-----------------------------------------------------------------*/
/* initPointer - pointer initialization code massaging             */
/*-----------------------------------------------------------------*/
value *
initPointer (initList * ilist, sym_link *toType)
{
  value *val;
  ast *expr;

  if (!ilist) {
      return valCastLiteral(toType, 0.0);
  }

  expr = list2expr (ilist);
  
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
      if (SPEC_SCLS (expr->left->etype) == S_CODE) {
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
/* printChar - formats and prints a characater string with DB      */
/*-----------------------------------------------------------------*/
void 
printChar (FILE * ofile, char *s, int plen)
{
  int i;
  int len = plen;
  int pplen = 0;
  char buf[100];
  char *p = buf;

  while (len && pplen < plen)
    {
      i = 60;
      while (i && pplen < plen)
        {
          if (*s < ' ' || *s == '\"' || *s=='\\')
            {
              *p = '\0';
              if (p != buf)
                tfprintf (ofile, "\t!ascii\n", buf);
              tfprintf (ofile, "\t!db !constbyte\n", (unsigned char)*s);
              p = buf;
            }
          else
            {
              *p = *s;
              p++;
            }
          s++;
          pplen++;
          i--;
        }
      if (p != buf)
        {
          *p = '\0';
          tfprintf (ofile, "\t!ascii\n", buf);
          p = buf;
        }

      if (len > 60)
        len -= 60;
      else
        len = 0;
    }
  while (pplen < plen)
    {
      tfprintf (ofile, "\t!db !constbyte\n", 0);
      pplen++;
    }
}

/*-----------------------------------------------------------------*/
/* return the generic pointer high byte for a given pointer type.  */
/*-----------------------------------------------------------------*/
int 
pointerTypeToGPByte (const int p_type, const char *iname, const char *oname)
{
  switch (p_type)
    {
    case IPOINTER:
    case POINTER:
      return GPTYPE_NEAR;
    case GPOINTER:
      werror (E_CANNOT_USE_GENERIC_POINTER, 
              iname ? iname : "<null>", 
              oname ? oname : "<null>");
      exit (1);
    case FPOINTER:
      return GPTYPE_FAR;
    case CPOINTER:
      return GPTYPE_CODE;
    case PPOINTER:
      return GPTYPE_XSTACK;
    default:
      fprintf (stderr, "*** internal error: unknown pointer type %d in GPByte.\n",
               p_type);
      break;
    }
  return -1;
}


/*-----------------------------------------------------------------*/
/* printPointerType - generates ival for pointer type              */
/*-----------------------------------------------------------------*/
void 
_printPointerType (FILE * oFile, const char *name)
{
  if (options.model == MODEL_FLAT24)
    {
      if (port->little_endian)
        fprintf (oFile, "\t.byte %s,(%s >> 8),(%s >> 16)", name, name, name);
      else
        fprintf (oFile, "\t.byte (%s >> 16),(%s >> 8),%s", name, name, name);
    }
  else
    {
      if (port->little_endian)
        fprintf (oFile, "\t.byte %s,(%s >> 8)", name, name);
      else
        fprintf (oFile, "\t.byte (%s >> 8),%s", name, name);
    }
}

/*-----------------------------------------------------------------*/
/* printPointerType - generates ival for pointer type              */
/*-----------------------------------------------------------------*/
void 
printPointerType (FILE * oFile, const char *name)
{
  _printPointerType (oFile, name);
  fprintf (oFile, "\n");
}

/*-----------------------------------------------------------------*/
/* printGPointerType - generates ival for generic pointer type     */
/*-----------------------------------------------------------------*/
void 
printGPointerType (FILE * oFile, const char *iname, const char *oname,
                   const unsigned int type)
{
  _printPointerType (oFile, iname);
  fprintf (oFile, ",#0x%02x\n", pointerTypeToGPByte (type, iname, oname));
}

/*-----------------------------------------------------------------*/
/* printIvalType - generates ival for int/char                     */
/*-----------------------------------------------------------------*/
void 
printIvalType (symbol *sym, sym_link * type, initList * ilist, FILE * oFile)
{
  value *val;

  /* if initList is deep */
  if (ilist && (ilist->type == INIT_DEEP))
    ilist = ilist->init.deep;

  if (!(val = list2val (ilist))) {
    // assuming a warning has been thrown
    val=constVal("0");
  }

  if (val->type != type) {
    val = valCastLiteral(type, floatFromVal(val));
  }
  
  switch (getSize (type)) {
  case 1:
    if (!val)
      tfprintf (oFile, "\t!db !constbyte\n", 0);
    else
      tfprintf (oFile, "\t!dbs\n",
                aopLiteral (val, 0));
    break;

  case 2:
    if (port->use_dw_for_init)
      tfprintf (oFile, "\t!dws\n", aopLiteralLong (val, 0, 2));
    else if (port->little_endian)
      fprintf (oFile, "\t.byte %s,%s\n", aopLiteral (val, 0), aopLiteral (val, 1));
    else
      fprintf (oFile, "\t.byte %s,%s\n", aopLiteral (val, 1), aopLiteral (val, 0));
    break;
  case 4:
    if (!val) {
      tfprintf (oFile, "\t!dw !constword\n", 0);
      tfprintf (oFile, "\t!dw !constword\n", 0);
    }
    else if (port->little_endian) {
      fprintf (oFile, "\t.byte %s,%s,%s,%s\n",
               aopLiteral (val, 0), aopLiteral (val, 1),
               aopLiteral (val, 2), aopLiteral (val, 3));
    }
    else {
      fprintf (oFile, "\t.byte %s,%s,%s,%s\n",
               aopLiteral (val, 3), aopLiteral (val, 2),
               aopLiteral (val, 1), aopLiteral (val, 0));
    }
    break;
  }
}

/*-----------------------------------------------------------------*/
/* printIvalBitFields - generate initializer for bitfields         */
/*-----------------------------------------------------------------*/
void printIvalBitFields(symbol **sym, initList **ilist, FILE * oFile)
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
    tfprintf (oFile, "\t!db !constbyte\n",ival);
    break;

  case 2:
    tfprintf (oFile, "\t!dw !constword\n",ival);
    break;
  case 4: /* EEP: why is this db and not dw? */
    tfprintf (oFile, "\t!db  !constword,!constword\n",
             (ival >> 8) & 0xffff, (ival & 0xffff));
    break;
  }
  *sym = lsym;
  *ilist = lilist;
}

/*-----------------------------------------------------------------*/
/* printIvalStruct - generates initial value for structures        */
/*-----------------------------------------------------------------*/
void 
printIvalStruct (symbol * sym, sym_link * type,
                 initList * ilist, FILE * oFile)
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
      printIvalBitFields(&sflds,&iloop,oFile);
    } else {
      printIval (sym, sflds->type, iloop, oFile);
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
int 
printIvalChar (sym_link * type, initList * ilist, FILE * oFile, char *s)
{
  value *val;

  if (!s)
    {

      val = list2val (ilist);
      /* if the value is a character string  */
      if (IS_ARRAY (val->type) && IS_CHAR (val->etype))
        {
          if (!DCL_ELEM (type))
            DCL_ELEM (type) = strlen (SPEC_CVAL (val->etype).v_char) + 1;

          printChar (oFile, SPEC_CVAL (val->etype).v_char, DCL_ELEM (type));

          return 1;
        }
      else
        return 0;
    }
  else
    printChar (oFile, s, strlen (s) + 1);
  return 1;
}

/*-----------------------------------------------------------------*/
/* printIvalArray - generates code for array initialization        */
/*-----------------------------------------------------------------*/
void
printIvalArray (symbol * sym, sym_link * type, initList * ilist,
                FILE * oFile)
{
  initList *iloop;
  int size = 0;

  if (ilist) {
    /* take care of the special   case  */
    /* array of characters can be init  */
    /* by a string                      */
    if (IS_CHAR (type->next)) {
      if (!IS_LITERAL(list2val(ilist)->etype)) {
        werrorfl (ilist->filename, ilist->lineno, E_CONST_EXPECTED);
        return;
      }
      if (printIvalChar (type,
                         (ilist->type == INIT_DEEP ? ilist->init.deep : ilist),
                         oFile, SPEC_CVAL (sym->etype).v_char))
        return;
    }
    /* not the special case             */
    if (ilist->type != INIT_DEEP) {
      werrorfl (ilist->filename, ilist->lineno, E_INIT_STRUCT, sym->name);
      return;
    }

    for (iloop=ilist->init.deep; iloop; iloop=iloop->next) {
      printIval (sym, type->next, iloop, oFile);
      
      if ((++size > DCL_ELEM(type)) && DCL_ELEM(type)) {
        werrorfl (sym->fileDef, sym->lineDef, W_EXCESS_INITIALIZERS, "array", sym->name);
        break;
      }
    }
  }
  
  if (DCL_ELEM(type)) {
    // pad with zeros if needed
    if (size<DCL_ELEM(type)) {
      size = (DCL_ELEM(type) - size) * getSize(type->next);
      while (size--) {
        tfprintf (oFile, "\t!db !constbyte\n", 0);
      }
    }
  } else {
    // we have not been given a size, but we now know it
    DCL_ELEM (type) = size;
  }

  return;
}

/*-----------------------------------------------------------------*/
/* printIvalFuncPtr - generate initial value for function pointers */
/*-----------------------------------------------------------------*/
void 
printIvalFuncPtr (sym_link * type, initList * ilist, FILE * oFile)
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
    printIvalCharPtr (NULL, type, val, oFile);
    return;
  }

  /* check the types   */
  if ((dLvl = compareType (val->type, type->next)) <= 0)
    {
      tfprintf (oFile, "\t!dw !constword\n", 0);
      return;
    }

  /* now generate the name */
  if (!val->sym)
    {
      if (port->use_dw_for_init)
        {
          tfprintf (oFile, "\t!dws\n", val->name);
        }
      else
        {
          printPointerType (oFile, val->name);
        }
    }
  else if (port->use_dw_for_init)
    {
      tfprintf (oFile, "\t!dws\n", val->sym->rname);
    }
  else
    {
      printPointerType (oFile, val->sym->rname);
    }

  return;
}

/*-----------------------------------------------------------------*/
/* printIvalCharPtr - generates initial values for character pointers */
/*-----------------------------------------------------------------*/
int 
printIvalCharPtr (symbol * sym, sym_link * type, value * val, FILE * oFile)
{
  int size = 0;

  /* PENDING: this is _very_ mcs51 specific, including a magic
     number...
     It's also endin specific.
   */
  size = getSize (type);

  if (val->name && strlen (val->name))
    {
      if (size == 1)            /* This appears to be Z80 specific?? */
        {
          tfprintf (oFile,
                    "\t!dbs\n", val->name);
        }
      else if (size == FPTRSIZE)
        {
          if (port->use_dw_for_init)
            {
              tfprintf (oFile, "\t!dws\n", val->name);
            }
          else
            {
              printPointerType (oFile, val->name);
            }
        }
      else if (size == GPTRSIZE)
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
          printGPointerType (oFile, val->name, sym->name, type);
        }
      else
        {
          fprintf (stderr, "*** internal error: unknown size in "
                   "printIvalCharPtr.\n");
        }
    }
  else
    {
      // these are literals assigned to pointers
      switch (size)
        {
        case 1:
          tfprintf (oFile, "\t!dbs\n", aopLiteral (val, 0));
          break;
        case 2:
          if (port->use_dw_for_init)
            tfprintf (oFile, "\t!dws\n", aopLiteralLong (val, 0, size));
          else if (port->little_endian)
            tfprintf (oFile, "\t.byte %s,%s\n",
                      aopLiteral (val, 0), aopLiteral (val, 1));
          else
            tfprintf (oFile, "\t.byte %s,%s\n",
                      aopLiteral (val, 1), aopLiteral (val, 0));
          break;
        case 3:
          if (IS_GENPTR(type) && floatFromVal(val)!=0) {
            // non-zero mcs51 generic pointer
            werrorfl (sym->fileDef, sym->lineDef, E_LITERAL_GENERIC);
          }
          if (port->little_endian) {
            fprintf (oFile, "\t.byte %s,%s,%s\n",
                     aopLiteral (val, 0), 
                     aopLiteral (val, 1),
                     aopLiteral (val, 2));
          } else {
            fprintf (oFile, "\t.byte %s,%s,%s\n",
                     aopLiteral (val, 2), 
                     aopLiteral (val, 1),
                     aopLiteral (val, 0));
          }
          break;
        case 4:
          if (IS_GENPTR(type) && floatFromVal(val)!=0) {
            // non-zero ds390 generic pointer
            werrorfl (sym->fileDef, sym->lineDef, E_LITERAL_GENERIC);
          }
          if (port->little_endian) {
            fprintf (oFile, "\t.byte %s,%s,%s,%s\n",
                     aopLiteral (val, 0), 
                     aopLiteral (val, 1), 
                     aopLiteral (val, 2),
                     aopLiteral (val, 3));
          } else {
            fprintf (oFile, "\t.byte %s,%s,%s,%s\n",
                     aopLiteral (val, 3), 
                     aopLiteral (val, 2), 
                     aopLiteral (val, 1),
                     aopLiteral (val, 0));
          }
          break;
        default:
          assert (0);
        }
    }

  if (val->sym && val->sym->isstrlit && !isinSet(statsg->syms, val->sym)) {
    addSet (&statsg->syms, val->sym);
  }

  return 1;
}

/*-----------------------------------------------------------------*/
/* printIvalPtr - generates initial value for pointers             */
/*-----------------------------------------------------------------*/
void 
printIvalPtr (symbol * sym, sym_link * type, initList * ilist, FILE * oFile)
{
  value *val;
  int size;

  /* if deep then   */
  if (ilist && (ilist->type == INIT_DEEP))
    ilist = ilist->init.deep;

  /* function pointer     */
  if (IS_FUNC (type->next))
    {
      printIvalFuncPtr (type, ilist, oFile);
      return;
    }

  if (!(val = initPointer (ilist, type)))
    return;

  /* if character pointer */
  if (IS_CHAR (type->next))
    if (printIvalCharPtr (sym, type, val, oFile))
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
          tfprintf (oFile, "\t!db !constbyte\n", (unsigned int) floatFromVal (val) & 0xff);
          break;
        case 2:
          if (port->use_dw_for_init)
            tfprintf (oFile, "\t!dws\n", aopLiteralLong (val, 0, 2));
          else if (port->little_endian)
            tfprintf (oFile, "\t.byte %s,%s\n", aopLiteral (val, 0), aopLiteral (val, 1));
          else
            tfprintf (oFile, "\t.byte %s,%s\n", aopLiteral (val, 1), aopLiteral (val, 0));
          break;
        case 3: // how about '390??
          fprintf (oFile, "; generic printIvalPtr\n");
          if (port->little_endian)
            {
              fprintf (oFile, "\t.byte %s,%s",
                       aopLiteral (val, 0), aopLiteral (val, 1));
            }
          else
            {
              fprintf (oFile, "\t.byte %s,%s",
                       aopLiteral (val, 1), aopLiteral (val, 0));
            }
          if (IS_GENPTR (val->type))
            fprintf (oFile, ",%s\n", aopLiteral (val, 2));
          else if (IS_PTR (val->type))
            fprintf (oFile, ",#%x\n", pointerTypeToGPByte (DCL_TYPE (val->type), NULL, NULL));
          else
            fprintf (oFile, ",%s\n", aopLiteral (val, 2));
        }
      return;
    }


  size = getSize (type);

  if (size == 1)                /* Z80 specific?? */
    {
      tfprintf (oFile, "\t!dbs\n", val->name);
    }
  else if (size == FPTRSIZE)
    {
      if (port->use_dw_for_init) {
        tfprintf (oFile, "\t!dws\n", val->name);
      } else {
        printPointerType (oFile, val->name);
      }
    }
  else if (size == GPTRSIZE)
    {
      printGPointerType (oFile, val->name, sym->name,
                         (IS_PTR (val->type) ? DCL_TYPE (val->type) :
                          PTR_TYPE (SPEC_OCLS (val->etype))));
    }
  return;
}

/*-----------------------------------------------------------------*/
/* printIval - generates code for initial value                    */
/*-----------------------------------------------------------------*/
void 
printIval (symbol * sym, sym_link * type, initList * ilist, FILE * oFile)
{
  sym_link *itype;
  
  /* if structure then    */
  if (IS_STRUCT (type))
    {
      printIvalStruct (sym, type, ilist, oFile);
      return;
    }

  /* if this is an array   */
  if (IS_ARRAY (type))
    {
      printIvalArray (sym, type, ilist, oFile);
      return;
    }

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

      // and the type must match
      itype=ilist->init.node->ftype;

      if (compareType(type, itype)==0) {
        // special case for literal strings
        if (IS_ARRAY (itype) && IS_CHAR (getSpec(itype)) &&
            // which are really code pointers
            IS_PTR(type) && DCL_TYPE(type)==CPOINTER) {
          // no sweat
        } else {
          werrorfl (ilist->filename, ilist->lineno, E_TYPE_MISMATCH, "assignment", " ");
          printFromToType(itype, type);
        }
      }
    }

  /* if this is a pointer */
  if (IS_PTR (type))
    {
      printIvalPtr (sym, type, ilist, oFile);
      return;
    }

  /* if type is SPECIFIER */
  if (IS_SPEC (type))
    {
      printIvalType (sym, type, ilist, oFile);
      return;
    }
}

/*-----------------------------------------------------------------*/
/* emitStaticSeg - emitcode for the static segment                 */
/*-----------------------------------------------------------------*/
void 
emitStaticSeg (memmap * map, FILE * out)
{
  symbol *sym;

  /* fprintf(out, "\t.area\t%s\n", map->sname); */

  /* for all variables in this segment do */
  for (sym = setFirstItem (map->syms); sym;
       sym = setNextItem (map->syms))
    {

      /* if it is "extern" then do nothing */
      if (IS_EXTERN (sym->etype))
        continue;

      /* if it is not static add it to the public
         table */
      if (!IS_STATIC (sym->etype))
        {
          addSetHead (&publics, sym);
        }

      /* print extra debug info if required */
      if (options.debug) {

        if (!sym->level)
          {                     /* global */
            if (IS_STATIC (sym->etype))
              fprintf (out, "F%s$", moduleName);        /* scope is file */
            else
              fprintf (out, "G$");      /* scope is global */
          }
        else
          /* symbol is local */
          fprintf (out, "L%s$",
                   (sym->localof ? sym->localof->name : "-null-"));
        fprintf (out, "%s$%d$%d", sym->name, sym->level, sym->block);
      }
      
      /* if it has an absolute address */
      if (SPEC_ABSA (sym->etype))
        {
          if (options.debug)
            fprintf (out, " == 0x%04x\n", SPEC_ADDR (sym->etype));
          
          fprintf (out, "%s\t=\t0x%04x\n",
                   sym->rname,
                   SPEC_ADDR (sym->etype));
        }
      else
        {
          if (options.debug)
            fprintf (out, " == .\n");
          
          /* if it has an initial value */
          if (sym->ival)
            {
              fprintf (out, "%s:\n", sym->rname);
              noAlloc++;
              resolveIvalSym (sym->ival, sym->type);
              printIval (sym, sym->type, sym->ival, out);
              noAlloc--;
              /* if sym is a simple string and sym->ival is a string, 
                 WE don't need it anymore */
              if (IS_ARRAY(sym->type) && IS_CHAR(sym->type->next) &&
                  IS_AST_SYM_VALUE(list2expr(sym->ival)) &&
                  list2val(sym->ival)->sym->isstrlit) {
                freeStringSymbol(list2val(sym->ival)->sym);
              }
            }
          else {
              /* allocate space */
              int size = getSize (sym->type);
              
              if (size==0) {
                  werrorfl (sym->fileDef, sym->lineDef, E_UNKNOWN_SIZE,sym->name);
              }
              fprintf (out, "%s:\n", sym->rname);
              /* special case for character strings */
              if (IS_ARRAY (sym->type) && IS_CHAR (sym->type->next) &&
                  SPEC_CVAL (sym->etype).v_char)
                  printChar (out,
                             SPEC_CVAL (sym->etype).v_char,
                             size);
              else
                  tfprintf (out, "\t!ds\n", (unsigned int) size & 0xffff);
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* emitMaps - emits the code for the data portion the code         */
/*-----------------------------------------------------------------*/
void 
emitMaps (void)
{
  int publicsfr = TARGET_IS_MCS51; /* Ideally, this should be true for all  */
                                   /* ports but let's be conservative - EEP */
  
  inInitMode++;
  /* no special considerations for the following
     data, idata & bit & xdata */
  emitRegularMap (data, TRUE, TRUE);
  emitRegularMap (idata, TRUE, TRUE);
  emitRegularMap (bit, TRUE, FALSE);
  emitRegularMap (xdata, TRUE, TRUE);
  if (port->genXINIT) {
    emitRegularMap (xidata, TRUE, TRUE);
  }
  emitRegularMap (sfr, publicsfr, FALSE);
  emitRegularMap (sfrbit, publicsfr, FALSE);
  emitRegularMap (home, TRUE, FALSE);
  emitRegularMap (code, TRUE, FALSE);

  emitStaticSeg (statsg, code->oFile);
  if (port->genXINIT) {
    tfprintf (code->oFile, "\t!area\n", xinit->sname);
    emitStaticSeg (xinit, code->oFile);
  }
  inInitMode--;
}

/*-----------------------------------------------------------------*/
/* flushStatics - flush all currently defined statics out to file  */
/*  and delete.  Temporary function                                */
/*-----------------------------------------------------------------*/
void 
flushStatics (void)
{
  emitStaticSeg (statsg, codeOutFile);
  statsg->syms = NULL;
}

/*-----------------------------------------------------------------*/
/* createInterruptVect - creates the interrupt vector              */
/*-----------------------------------------------------------------*/
void 
createInterruptVect (FILE * vFile)
{
  unsigned i = 0;
  mainf = newSymbol ("main", 0);
  mainf->block = 0;

  /* only if the main function exists */
  if (!(mainf = findSymWithLevel (SymbolTab, mainf)))
    {
      if (!options.cc_only && !noAssemble && !options.c1mode)
        werror (E_NO_MAIN);
      return;
    }

  /* if the main is only a prototype ie. no body then do nothing */
  if (!IFFUNC_HASBODY(mainf->type))
    {
      /* if ! compile only then main function should be present */
      if (!options.cc_only && !noAssemble)
        werror (E_NO_MAIN);
      return;
    }

  tfprintf (vFile, "\t!areacode\n", CODE_NAME);
  fprintf (vFile, "__interrupt_vect:\n");


  if (!port->genIVT || !(port->genIVT (vFile, interrupts, maxInterrupts)))
    {
      /* "generic" interrupt table header (if port doesn't specify one).
       * Look suspiciously like 8051 code to me...
       */

      fprintf (vFile, "\tljmp\t__sdcc_gsinit_startup\n");

      /* now for the other interrupts */
      for (; i < maxInterrupts; i++)
        {
          if (interrupts[i])
            {
              fprintf (vFile, "\tljmp\t%s\n", interrupts[i]->rname);
              if ( i != maxInterrupts - 1 )
                fprintf (vFile, "\t.ds\t5\n");
            }
          else
            {
              fprintf (vFile, "\treti\n");
              if ( i != maxInterrupts - 1 )
                fprintf (vFile, "\t.ds\t7\n");
            }
        }
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
void 
initialComments (FILE * afile)
{
  time_t t;
  time (&t);
  fprintf (afile, "%s", iComments1);
  fprintf (afile, "; Version " SDCC_VERSION_STR " (%s)\n", __DATE__);
  fprintf (afile, "; This file generated %s", asctime (localtime (&t)));
  fprintf (afile, "%s", iComments2);
}

/*-----------------------------------------------------------------*/
/* printPublics - generates .global for publics                    */
/*-----------------------------------------------------------------*/
void 
printPublics (FILE * afile)
{
  symbol *sym;

  fprintf (afile, "%s", iComments2);
  fprintf (afile, "; Public variables in this module\n");
  fprintf (afile, "%s", iComments2);

  for (sym = setFirstItem (publics); sym;
       sym = setNextItem (publics))
    tfprintf (afile, "\t!global\n", sym->rname);
}

/*-----------------------------------------------------------------*/
/* printExterns - generates .global for externs                    */
/*-----------------------------------------------------------------*/
void 
printExterns (FILE * afile)
{
  symbol *sym;

  fprintf (afile, "%s", iComments2);
  fprintf (afile, "; Externals used\n");
  fprintf (afile, "%s", iComments2);

  for (sym = setFirstItem (externs); sym;
       sym = setNextItem (externs))
    tfprintf (afile, "\t!extern\n", sym->rname);
}

/*-----------------------------------------------------------------*/
/* emitOverlay - will emit code for the overlay stuff              */
/*-----------------------------------------------------------------*/
static void 
emitOverlay (FILE * afile)
{
  set *ovrset;

  if (!elementsInSet (ovrSetSets))
    tfprintf (afile, "\t!area\n", port->mem.overlay_name);

  /* for each of the sets in the overlay segment do */
  for (ovrset = setFirstItem (ovrSetSets); ovrset;
       ovrset = setNextItem (ovrSetSets))
    {

      symbol *sym;

      if (elementsInSet (ovrset))
        {
          /* output the area informtion */
          fprintf (afile, "\t.area\t%s\n", port->mem.overlay_name);     /* MOF */
        }

      for (sym = setFirstItem (ovrset); sym;
           sym = setNextItem (ovrset))
        {
          /* if extern then it is in the publics table: do nothing */
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
            {
              addSetHead (&publics, sym);
            }

          /* if extern then do nothing or is a function
             then do nothing */
          if (IS_FUNC (sym->type))
            continue;

          /* print extra debug info if required */
          if (options.debug)
            {
              if (!sym->level)
                {               /* global */
                  if (IS_STATIC (sym->etype))
                    fprintf (afile, "F%s$", moduleName);        /* scope is file */
                  else
                    fprintf (afile, "G$");      /* scope is global */
                }
              else
                /* symbol is local */
                fprintf (afile, "L%s$",
                         (sym->localof ? sym->localof->name : "-null-"));
              fprintf (afile, "%s$%d$%d", sym->name, sym->level, sym->block);
            }

          /* if is has an absolute address then generate
             an equate for this no need to allocate space */
          if (SPEC_ABSA (sym->etype))
            {

              if (options.debug)
                fprintf (afile, " == 0x%04x\n", SPEC_ADDR (sym->etype));

              fprintf (afile, "%s\t=\t0x%04x\n",
                       sym->rname,
                       SPEC_ADDR (sym->etype));
            }
          else {
              int size = getSize(sym->type);

              if (size==0) {
                  werrorfl (sym->fileDef, sym->lineDef, E_UNKNOWN_SIZE);
              }       
              if (options.debug)
                  fprintf (afile, "==.\n");
              
              /* allocate space */
              tfprintf (afile, "!labeldef\n", sym->rname);
              tfprintf (afile, "\t!ds\n", (unsigned int) getSize (sym->type) & 0xffff);
          }
          
        }
    }
}


/*-----------------------------------------------------------------*/
/* spacesToUnderscores - replace spaces with underscores        */
/*-----------------------------------------------------------------*/
static char *
spacesToUnderscores (char *dest, const char *src, size_t len)
{
  int i;
  char *p;

  assert(dest != NULL);
  assert(src != NULL);
  assert(len > 0);

  --len;
  for (p = dest, i = 0; *src != '\0' && i < len; ++src, ++i) {
    *p++ = isspace(*src) ? '_' : *src;
  }
  *p = '\0';

  return dest;
}


/*-----------------------------------------------------------------*/
/* glue - the final glue that hold the whole thing together        */
/*-----------------------------------------------------------------*/
void 
glue (void)
{
  FILE *vFile;
  FILE *asmFile;
  FILE *ovrFile = tempfile ();
  char moduleBuf[PATH_MAX];
  int mcs51_like;

  if(port->general.glue_up_main &&
    (TARGET_IS_MCS51 || TARGET_IS_DS390 || TARGET_IS_XA51 || TARGET_IS_DS400))
  {
      mcs51_like=1; /*So it has bits, sfr, sbits, data, idata, etc...*/
  }
  else
  {
      mcs51_like=0;
  }

  addSetHead (&tmpfileSet, ovrFile);
  /* print the global struct definitions */
  if (options.debug)
    cdbStructBlock (0);

  vFile = tempfile ();
  /* PENDING: this isnt the best place but it will do */
  if (port->general.glue_up_main)
    {
      /* create the interrupt vector table */
      createInterruptVect (vFile);
    }

  addSetHead (&tmpfileSet, vFile);

  /* emit code for the all the variables declared */
  emitMaps ();
  /* do the overlay segments */
  emitOverlay (ovrFile);

  outputDebugSymbols();

  /* now put it all together into the assembler file */
  /* create the assembler file name */

  /* -o option overrides default name? */
  if ((noAssemble || options.c1mode) && fullDstFileName)
    {
      strncpyz (scratchFileName, fullDstFileName, PATH_MAX);
    }
  else
    {
      strncpyz (scratchFileName, dstFileName, PATH_MAX);
      strncatz (scratchFileName, port->assembler.file_ext, PATH_MAX);
    }

  if (!(asmFile = fopen (scratchFileName, "w")))
    {
      werror (E_FILE_OPEN_ERR, scratchFileName);
      exit (1);
    }

  /* initial comments */
  initialComments (asmFile);

  /* print module name */
  tfprintf (asmFile, "\t!module\n",
    spacesToUnderscores (moduleBuf, moduleName, sizeof moduleBuf));
  if(mcs51_like)
  {
    fprintf (asmFile, "\t.optsdcc -m%s", port->target);

    switch(options.model)
    {
        case MODEL_SMALL:   fprintf (asmFile, " --model-small");   break;
        case MODEL_COMPACT: fprintf (asmFile, " --model-compact"); break;
        case MODEL_MEDIUM:  fprintf (asmFile, " --model-medium");  break;
        case MODEL_LARGE:   fprintf (asmFile, " --model-large");   break;
        case MODEL_FLAT24:  fprintf (asmFile, " --model-flat24");  break;
        case MODEL_PAGE0:   fprintf (asmFile, " --model-page0");   break;
        default: break;
    }
    /*if(options.stackAuto)      fprintf (asmFile, " --stack-auto");*/
    if(options.useXstack)      fprintf (asmFile, " --xstack");
    /*if(options.intlong_rent)   fprintf (asmFile, " --int-long-rent");*/
    /*if(options.float_rent)     fprintf (asmFile, " --float-rent");*/
    if(options.noRegParams)    fprintf (asmFile, " --no-reg-params");
    if(options.parms_in_bank1) fprintf (asmFile, " --parms-in-bank1");
    fprintf (asmFile, "\n");
  }
  else if(TARGET_IS_Z80 || TARGET_IS_GBZ80 || TARGET_IS_HC08)
  {
    fprintf (asmFile, "\t.optsdcc -m%s\n", port->target);
  }

  tfprintf (asmFile, "\t!fileprelude\n");

  /* Let the port generate any global directives, etc. */
  if (port->genAssemblerPreamble)
    {
      port->genAssemblerPreamble (asmFile);
    }

  /* print the global variables in this module */
  printPublics (asmFile);
  if (port->assembler.externGlobal)
    printExterns (asmFile);

  if(( mcs51_like )
   ||( TARGET_IS_Z80 )) /*.p.t.20030924 need to output SFR table for Z80 as well */
  {
      /* copy the sfr segment */
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "; special function registers\n");
      fprintf (asmFile, "%s", iComments2);
      copyFile (asmFile, sfr->oFile);
  }
  
  if(mcs51_like)
    {
      /* copy the sbit segment */
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "; special function bits \n");
      fprintf (asmFile, "%s", iComments2);
      copyFile (asmFile, sfrbit->oFile);
  
      /*JCF: Create the areas for the register banks*/
      if(RegBankUsed[0]||RegBankUsed[1]||RegBankUsed[2]||RegBankUsed[3])
      {
         fprintf (asmFile, "%s", iComments2);
         fprintf (asmFile, "; overlayable register banks \n");
         fprintf (asmFile, "%s", iComments2);
         if(RegBankUsed[0])
            fprintf (asmFile, "\t.area REG_BANK_0\t(REL,OVR,DATA)\n\t.ds 8\n");
         if(RegBankUsed[1]||options.parms_in_bank1)
            fprintf (asmFile, "\t.area REG_BANK_1\t(REL,OVR,DATA)\n\t.ds 8\n");
         if(RegBankUsed[2])
            fprintf (asmFile, "\t.area REG_BANK_2\t(REL,OVR,DATA)\n\t.ds 8\n");
         if(RegBankUsed[3])
            fprintf (asmFile, "\t.area REG_BANK_3\t(REL,OVR,DATA)\n\t.ds 8\n");
      }
    }

  /* copy the data segment */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; %s ram data\n", mcs51_like?"internal":"");
  fprintf (asmFile, "%s", iComments2);
  copyFile (asmFile, data->oFile);


  /* create the overlay segments */
  if (overlay) {
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; overlayable items in %s ram \n", mcs51_like?"internal":"");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, ovrFile);
  }

  /* create the stack segment MOF */
  if (mainf && IFFUNC_HASBODY(mainf->type))
    {
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "; Stack segment in internal ram \n");
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "\t.area\tSSEG\t(DATA)\n"
               "__start__stack:\n\t.ds\t1\n\n");
    }

  /* create the idata segment */
  if ( (idata) && (mcs51_like) ) {
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; indirectly addressable internal ram data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, idata->oFile);
  }

  /* copy the bit segment */
  if (mcs51_like) {
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; bit data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, bit->oFile);
  }

  /* if external stack then reserve space of it */
  if (mainf && IFFUNC_HASBODY(mainf->type) && options.useXstack)
    {
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "; external stack \n");
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "\t.area XSEG (XDATA)\n");      /* MOF */
      fprintf (asmFile, "\t.ds 256\n");
    }


  /* copy xtern ram data */
  if (mcs51_like) {
    fprintf (asmFile, "%s", iComments2);
    fprintf (asmFile, "; external ram data\n");
    fprintf (asmFile, "%s", iComments2);
    copyFile (asmFile, xdata->oFile);
  }

  /* copy xternal initialized ram data */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; external initialized ram data\n");
  fprintf (asmFile, "%s", iComments2);
  copyFile (asmFile, xidata->oFile);

  /* If the port wants to generate any extra areas, let it do so. */
  if (port->extraAreas.genExtraAreaDeclaration)
  {
      port->extraAreas.genExtraAreaDeclaration(asmFile, 
                                               mainf && IFFUNC_HASBODY(mainf->type));
  }
    
  /* copy the interrupt vector table */
  if (mainf && IFFUNC_HASBODY(mainf->type))
    {
      fprintf (asmFile, "%s", iComments2);
      fprintf (asmFile, "; interrupt vector \n");
      fprintf (asmFile, "%s", iComments2);
      copyFile (asmFile, vFile);
    }

  /* copy global & static initialisations */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; global & static initialisations\n");
  fprintf (asmFile, "%s", iComments2);

  /* Everywhere we generate a reference to the static_name area,
   * (which is currently only here), we immediately follow it with a
   * definition of the post_static_name area. This guarantees that
   * the post_static_name area will immediately follow the static_name
   * area.
   */
  tfprintf (asmFile, "\t!area\n", port->mem.static_name);       /* MOF */
  tfprintf (asmFile, "\t!area\n", port->mem.post_static_name);
  tfprintf (asmFile, "\t!area\n", port->mem.static_name);

  if (mainf && IFFUNC_HASBODY(mainf->type))
    {
      if (port->genInitStartup)
        {
           port->genInitStartup(asmFile);
        }
      else
        {
          fprintf (asmFile, "__sdcc_gsinit_startup:\n");
          /* if external stack is specified then the
             higher order byte of the xdatalocation is
             going into P2 and the lower order going into
             spx */
          if (options.useXstack)
            {
              fprintf (asmFile, "\tmov\tP2,#0x%02x\n",
                       (((unsigned int) options.xdata_loc) >> 8) & 0xff);
              fprintf (asmFile, "\tmov\t_spx,#0x%02x\n",
                       (unsigned int) options.xdata_loc & 0xff);
            }

          // This should probably be a port option, but I'm being lazy.
          // on the 400, the firmware boot loader gives us a valid stack
          // (see '400 data sheet pg. 85 (TINI400 ROM Initialization code)
          if (!TARGET_IS_DS400)
            {
              /* initialise the stack pointer.  JCF: aslink takes care of the location */
              fprintf (asmFile, "\tmov\tsp,#__start__stack - 1\n");     /* MOF */
            }

          fprintf (asmFile, "\tlcall\t__sdcc_external_startup\n");
          fprintf (asmFile, "\tmov\ta,dpl\n");
          fprintf (asmFile, "\tjz\t__sdcc_init_data\n");
          fprintf (asmFile, "\tljmp\t__sdcc_program_startup\n");
          fprintf (asmFile, "__sdcc_init_data:\n");

          // if the port can copy the XINIT segment to XISEG
          if (port->genXINIT)
            {
              port->genXINIT(asmFile);
            }
        }

    }
  copyFile (asmFile, statsg->oFile);

  if (port->general.glue_up_main && mainf && IFFUNC_HASBODY(mainf->type))
    {
      /* This code is generated in the post-static area.
       * This area is guaranteed to follow the static area
       * by the ugly shucking and jiving about 20 lines ago.
       */
      tfprintf (asmFile, "\t!area\n", port->mem.post_static_name);
      fprintf (asmFile, "\tljmp\t__sdcc_program_startup\n");
    }

  fprintf (asmFile,
           "%s"
           "; Home\n"
           "%s", iComments2, iComments2);
  tfprintf (asmFile, "\t!areahome\n", HOME_NAME);
  copyFile (asmFile, home->oFile);

  /* copy over code */
  fprintf (asmFile, "%s", iComments2);
  fprintf (asmFile, "; code\n");
  fprintf (asmFile, "%s", iComments2);
  tfprintf (asmFile, "\t!areacode\n", CODE_NAME);
  if (mainf && IFFUNC_HASBODY(mainf->type))
    {

      /* entry point @ start of CSEG */
      fprintf (asmFile, "__sdcc_program_startup:\n");

      /* put in jump or call to main */
      if (options.mainreturn)
        {
          fprintf (asmFile, "\tljmp\t_main\n");   /* needed? */
          fprintf (asmFile, ";\treturn from main will return to caller\n");
        }
      else
        {
          fprintf (asmFile, "\tlcall\t_main\n");
          fprintf (asmFile, ";\treturn from main will lock up\n");
          fprintf (asmFile, "\tsjmp .\n");
        }
    }
  copyFile (asmFile, code->oFile);

  if (port->genAssemblerEnd) {
      port->genAssemblerEnd(asmFile);
  }
  fclose (asmFile);

  rm_tmpfiles ();
}


/** Creates a temporary file with unique file name
    Scans, in order:
    - TMP, TEMP, TMPDIR env. varibles
    - if Un*x system: /usr/tmp and /tmp
    - root directory using mkstemp() if avaliable
    - default location using tempnam()
*/
static int
tempfileandname(char *fname, size_t len)
{
#define TEMPLATE      "sdccXXXXXX"
#define TEMPLATE_LEN  ((sizeof TEMPLATE) - 1)

  const char *tmpdir = NULL;
  int fd;

  if ((tmpdir = getenv ("TMP")) == NULL)
    if ((tmpdir = getenv ("TEMP")) == NULL)
      tmpdir = getenv ("TMPDIR");

#if defined(_WIN32)
  {
    static int warning_emitted;

    if (tmpdir == NULL)
      {
        tmpdir = "c:\\";
        if (!warning_emitted)
          {
            fprintf (stderr, "TMP not defined in environment, using %s for temporary files\n.", tmpdir);
            warning_emitted = 1;
          }
      }
  }
#else
  {
    /* try with /usr/tmp and /tmp on Un*x systems */
    struct stat statbuf;

    if (tmpdir == NULL) {
      if (stat("/usr/tmp", &statbuf) != -1)
        tmpdir = "/usr/tmp";
      else if (stat("/tmp", &statbuf) != -1)
        tmpdir = "/tmp";
    }
  }
#endif

#ifdef HAVE_MKSTEMP
  {
    char fnamebuf[PATH_MAX];
    size_t name_len;

    if (fname == NULL || len == 0) {
      fname = fnamebuf;
      len = sizeof fnamebuf;
    }

    if (tmpdir) {
      name_len = strlen(tmpdir) + 1 + TEMPLATE_LEN;

      assert(name_len < len);
      if (!(name_len < len))  /* in NDEBUG is defined */
        return -1;            /* buffer too small, temporary file can not be created */

      sprintf(fname, "%s" DIR_SEPARATOR_STRING TEMPLATE, tmpdir);
    }
    else {
      name_len = TEMPLATE_LEN;

      assert(name_len < len);
      if (!(name_len < len))  /* in NDEBUG is defined */
        return -1;            /* buffer too small, temporary file can not be created */

      strcpy(fname, TEMPLATE);
    }

    fd = mkstemp(fname);
  }
#else
  {
    char *name = tempnam(tmpdir, "sdcc");

    if (name == NULL) {
      perror("Can't create temporary file name");
      exit(1);
    }

    assert(strlen(name) < len);
    if (!(strlen(name) < len))  /* in NDEBUG is defined */
      return -1;                /* buffer too small, temporary file can not be created */

    strcpy(fname, name);
#ifdef _WIN32
    fd = open(name, O_CREAT | O_EXCL | O_RDWR, S_IREAD | S_IWRITE);
#else
    fd = open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
#endif
  }
#endif

  if (fd == -1) {
    perror("Can't create temporary file");
    exit(1);
  }

  return fd;
}


/** Create a temporary file name
*/
char *
tempfilename(void)
{
  int fd;
  static char fnamebuf[PATH_MAX];

  if ((fd = tempfileandname(fnamebuf, sizeof fnamebuf)) == -1) {
    fprintf(stderr, "Can't create temporary file name!");
    exit(1);
  }

  fd = close(fd);
  assert(fd != -1);

  return fnamebuf;
}


/** Create a temporary file and add it to tmpfileNameSet,
    so that it is removed explicitly by rm_tmpfiles()
    or implicitly at program extit.
*/
FILE *
tempfile(void)
{
  int fd;
  char *tmp;
  FILE *fp;
  char fnamebuf[PATH_MAX];

  if ((fd = tempfileandname(fnamebuf, sizeof fnamebuf)) == -1) {
    fprintf(stderr, "Can't create temporary file!");
    exit(1);
  }

  tmp = Safe_strdup(fnamebuf);
  if (tmp)
    addSetHead(&tmpfileNameSet, tmp);

  if ((fp = fdopen(fd, "w+b")) == NULL) {
      perror("Can't create temporary file!");
      exit(1);
  }

  return fp;
}
