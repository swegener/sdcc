/*-------------------------------------------------------------------------
  SDCCast.c - source file for parser support & all ast related routines

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

#define DEBUG_CF(x) /* puts(x); */

#include "common.h"

int currLineno = 0;
set *astList = NULL;
set *operKeyReset = NULL;
ast *staticAutos = NULL;
int labelKey = 1;

#define LRVAL(x) x->left->rvalue
#define RRVAL(x) x->right->rvalue
#define TRVAL(x) x->rvalue
#define LLVAL(x) x->left->lvalue
#define RLVAL(x) x->right->lvalue
#define TLVAL(x) x->lvalue
#define RTYPE(x) x->right->ftype
#define RETYPE(x) x->right->etype
#define LTYPE(x) x->left->ftype
#define LETYPE(x) x->left->etype
#define TTYPE(x) x->ftype
#define TETYPE(x) x->etype
#define ALLOCATE 1
#define DEALLOCATE 2

int noLineno = 0;
int noAlloc = 0;
symbol *currFunc=NULL;
static ast *createIval (ast *, sym_link *, initList *, ast *);
static ast *createIvalCharPtr (ast *, sym_link *, ast *);
static ast *optimizeCompare (ast *);
ast *optimizeRRCRLC (ast *);
ast *optimizeSWAP (ast *);
ast *optimizeGetHbit (ast *);
ast *backPatchLabels (ast *, symbol *, symbol *);
void PA(ast *t);
int inInitMode = 0;
memmap *GcurMemmap=NULL;  /* points to the memmap that's currently active */
FILE *codeOutFile;
int 
ptt (ast * tree)
{
  printTypeChain (tree->ftype, stdout);
  return 0;
}


/*-----------------------------------------------------------------*/
/* newAst - creates a fresh node for an expression tree            */
/*-----------------------------------------------------------------*/
static ast *
newAst_ (unsigned type)
{
  ast *ex;
  static int oldLineno = 0;

  ex = Safe_alloc ( sizeof (ast));

  ex->type = type;
  ex->lineno = (noLineno ? oldLineno : mylineno);
  ex->filename = currFname;
  ex->level = NestLevel;
  ex->block = currBlockno;
  ex->initMode = inInitMode;
  ex->seqPoint = seqPointNo;
  return ex;
}

ast *
newAst_VALUE (value * val)
{
  ast *ex = newAst_ (EX_VALUE);
  ex->opval.val = val;
  return ex;
}

ast *
newAst_OP (unsigned op)
{
  ast *ex = newAst_ (EX_OP);
  ex->opval.op = op;
  return ex;
}

ast *
newAst_LINK (sym_link * val)
{
  ast *ex = newAst_ (EX_LINK);
  ex->opval.lnk = val;
  return ex;
}

/*-----------------------------------------------------------------*/
/* newNode - creates a new node                                    */
/*-----------------------------------------------------------------*/
ast *
newNode (long op, ast * left, ast * right)
{
  ast *ex;

  ex = newAst_OP (op);
  ex->left = left;
  ex->right = right;

  return ex;
}

/*-----------------------------------------------------------------*/
/* newIfxNode - creates a new Ifx Node                             */
/*-----------------------------------------------------------------*/
ast *
newIfxNode (ast * condAst, symbol * trueLabel, symbol * falseLabel)
{
  ast *ifxNode;

  /* if this is a literal then we already know the result */
  if (condAst->etype && IS_LITERAL (condAst->etype))
    {
      /* then depending on the expression value */
      if (floatFromVal (condAst->opval.val))
        ifxNode = newNode (GOTO,
                           newAst_VALUE (symbolVal (trueLabel)),
                           NULL);
      else
        ifxNode = newNode (GOTO,
                           newAst_VALUE (symbolVal (falseLabel)),
                           NULL);
    }
  else
    {
      ifxNode = newNode (IFX, condAst, NULL);
      ifxNode->trueLabel = trueLabel;
      ifxNode->falseLabel = falseLabel;
    }

  return ifxNode;
}

/*-----------------------------------------------------------------*/
/* copyAstValues - copies value portion of ast if needed     */
/*-----------------------------------------------------------------*/
void
copyAstValues (ast * dest, ast * src)
{
  switch (src->opval.op)
    {
    case BLOCK:
      dest->values.sym = copySymbolChain (src->values.sym);
      break;

    case SWITCH:
      dest->values.switchVals.swVals =
        copyValue (src->values.switchVals.swVals);
      dest->values.switchVals.swDefault =
        src->values.switchVals.swDefault;
      dest->values.switchVals.swNum =
        src->values.switchVals.swNum;
      break;

    case INLINEASM:
      dest->values.inlineasm =  Safe_strdup(src->values.inlineasm);
      break;

    case ARRAYINIT:
        dest->values.constlist = copyLiteralList(src->values.constlist);
        break;

    case FOR:
      AST_FOR (dest, trueLabel) = copySymbol (AST_FOR (src, trueLabel));
      AST_FOR (dest, continueLabel) = copySymbol (AST_FOR (src, continueLabel));
      AST_FOR (dest, falseLabel) = copySymbol (AST_FOR (src, falseLabel));
      AST_FOR (dest, condLabel) = copySymbol (AST_FOR (src, condLabel));
      AST_FOR (dest, initExpr) = copyAst (AST_FOR (src, initExpr));
      AST_FOR (dest, condExpr) = copyAst (AST_FOR (src, condExpr));
      AST_FOR (dest, loopExpr) = copyAst (AST_FOR (src, loopExpr));
    }

}

/*-----------------------------------------------------------------*/
/* copyAst - makes a copy of a given astession                     */
/*-----------------------------------------------------------------*/
ast *
copyAst (ast * src)
{
  ast *dest;

  if (!src)
    return NULL;

  dest = Safe_alloc ( sizeof (ast));

  dest->type = src->type;
  dest->lineno = src->lineno;
  dest->level = src->level;
  dest->funcName = src->funcName;

  if (src->ftype)
    dest->etype = getSpec (dest->ftype = copyLinkChain (src->ftype));

  /* if this is a leaf */
  /* if value */
  if (src->type == EX_VALUE)
    {
      dest->opval.val = copyValue (src->opval.val);
      goto exit;
    }

  /* if link */
  if (src->type == EX_LINK)
    {
      dest->opval.lnk = copyLinkChain (src->opval.lnk);
      goto exit;
    }

  dest->opval.op = src->opval.op;

  /* if this is a node that has special values */
  copyAstValues (dest, src);

  dest->trueLabel = copySymbol (src->trueLabel);
  dest->falseLabel = copySymbol (src->falseLabel);
  dest->left = copyAst (src->left);
  dest->right = copyAst (src->right);
exit:
  return dest;

}

/*-----------------------------------------------------------------*/
/* removeIncDecOps: remove for side effects in *_ASSIGN's          */
/*                  "*s++ += 3" -> "*s++ = *s++ + 3"               */
/*-----------------------------------------------------------------*/
ast *removeIncDecOps (ast * tree) {

  // traverse the tree and remove inc/dec ops

  if (!tree)
    return NULL;

  if (tree->type == EX_OP &&
      (tree->opval.op == INC_OP || tree->opval.op == DEC_OP)) {
    if (tree->left)
      tree=tree->left;
    else
      tree=tree->right;
  }

  tree->left=removeIncDecOps(tree->left);
  tree->right=removeIncDecOps(tree->right);

 return tree;
}

/*-----------------------------------------------------------------*/
/* removePreIncDecOps: remove for side effects in *_ASSIGN's       */
/*                  "*++s += 3" -> "*++s = *++s + 3"               */
/*-----------------------------------------------------------------*/
ast *removePreIncDecOps (ast * tree) {

  // traverse the tree and remove pre-inc/dec ops

  if (!tree)
    return NULL;

  if (tree->type == EX_OP &&
      (tree->opval.op == INC_OP || tree->opval.op == DEC_OP)) {
    if (tree->right)
      tree=tree->right;
  }

  tree->left=removePreIncDecOps(tree->left);
  tree->right=removePreIncDecOps(tree->right);

 return tree;
}

/*-----------------------------------------------------------------*/
/* removePostIncDecOps: remove for side effects in *_ASSIGN's      */
/*                  "*s++ += 3" -> "*s++ = *s++ + 3"               */
/*-----------------------------------------------------------------*/
ast *removePostIncDecOps (ast * tree) {

  // traverse the tree and remove pre-inc/dec ops

  if (!tree)
    return NULL;

  if (tree->type == EX_OP &&
      (tree->opval.op == INC_OP || tree->opval.op == DEC_OP)) {
    if (tree->left)
      tree=tree->left;
  }

  tree->left=removePostIncDecOps(tree->left);
  tree->right=removePostIncDecOps(tree->right);

 return tree;
}

/*-----------------------------------------------------------------*/
/* hasSEFcalls - returns TRUE if tree has a function call          */
/*-----------------------------------------------------------------*/
bool
hasSEFcalls (ast * tree)
{
  if (!tree)
    return FALSE;

  if (tree->type == EX_OP &&
      (tree->opval.op == CALL ||
       tree->opval.op == PCALL ||
       tree->opval.op == '=' ||
       tree->opval.op == INC_OP ||
       tree->opval.op == DEC_OP))
    return TRUE;

  return (hasSEFcalls (tree->left) |
          hasSEFcalls (tree->right));
}

/*-----------------------------------------------------------------*/
/* isAstEqual - compares two asts & returns 1 if they are equal    */
/*-----------------------------------------------------------------*/
static int
isAstEqual (ast * t1, ast * t2)
{
  if (!t1 && !t2)
    return 1;

  if (!t1 || !t2)
    return 0;

  /* match type */
  if (t1->type != t2->type)
    return 0;

  switch (t1->type)
    {
    case EX_OP:
      if (t1->opval.op != t2->opval.op)
        return 0;
      return (isAstEqual (t1->left, t2->left) &&
              isAstEqual (t1->right, t2->right));
      break;

    case EX_VALUE:
      if (t1->opval.val->sym)
        {
          if (!t2->opval.val->sym)
            return 0;
          else
            return isSymbolEqual (t1->opval.val->sym,
                                  t2->opval.val->sym);
        }
      else
        {
          if (t2->opval.val->sym)
            return 0;
          else
            return (floatFromVal (t1->opval.val) ==
                    floatFromVal (t2->opval.val));
        }
      break;

      /* only compare these two types */
    default:
      return 0;
    }

  return 0;
}

/*-----------------------------------------------------------------*/
/* resolveSymbols - resolve symbols from the symbol table          */
/*-----------------------------------------------------------------*/
ast *
resolveSymbols (ast * tree)
{
  /* walk the entire tree and check for values */
  /* with symbols if we find one then replace  */
  /* symbol with that from the symbol table    */

  if (tree == NULL)
    return tree;

#if 0
  /* print the line          */
  /* if not block & function */
  if (tree->type == EX_OP &&
      (tree->opval.op != FUNCTION &&
       tree->opval.op != BLOCK &&
       tree->opval.op != NULLOP))
    {
      filename = tree->filename;
      lineno = tree->lineno;
    }
#endif

  /* make sure we resolve the true & false labels for ifx */
  if (tree->type == EX_OP && tree->opval.op == IFX)
    {
      symbol *csym;

      if (tree->trueLabel)
        {
          if ((csym = findSym (LabelTab, tree->trueLabel,
                               tree->trueLabel->name)))
            tree->trueLabel = csym;
          else
            werrorfl (tree->filename, tree->lineno, E_LABEL_UNDEF,
                      tree->trueLabel->name);
        }

      if (tree->falseLabel)
        {
          if ((csym = findSym (LabelTab,
                               tree->falseLabel,
                               tree->falseLabel->name)))
            tree->falseLabel = csym;
          else
            werrorfl (tree->filename, tree->lineno, E_LABEL_UNDEF,
                      tree->falseLabel->name);
        }

    }

  /* if this is a label resolve it from the labelTab */
  if (IS_AST_VALUE (tree) &&
      tree->opval.val->sym &&
      tree->opval.val->sym->islbl)
    {

      symbol *csym = findSym (LabelTab, tree->opval.val->sym,
                              tree->opval.val->sym->name);

      if (!csym)
        werrorfl (tree->filename, tree->lineno, E_LABEL_UNDEF,
                  tree->opval.val->sym->name);
      else
        tree->opval.val->sym = csym;

      goto resolveChildren;
    }

  /* do only for leafs */
  if (IS_AST_VALUE (tree) &&
      tree->opval.val->sym &&
      !tree->opval.val->sym->implicit)
    {

      symbol *csym = findSymWithLevel (SymbolTab, tree->opval.val->sym);

      /* if found in the symbol table & they r not the same */
      if (csym && tree->opval.val->sym != csym)
        {
          tree->opval.val->sym = csym;
          tree->opval.val->type = csym->type;
          tree->opval.val->etype = csym->etype;
        }

      /* if not found in the symbol table */
      /* mark it as undefined assume it is */
      /* an integer in data space         */
      if (!csym && !tree->opval.val->sym->implicit)
        {

          /* if this is a function name then */
          /* mark it as returning an int     */
          if (tree->funcName)
            {
              tree->opval.val->sym->type = newLink (DECLARATOR);
              DCL_TYPE (tree->opval.val->sym->type) = FUNCTION;
              tree->opval.val->sym->type->next =
                tree->opval.val->sym->etype = newIntLink ();
              tree->opval.val->etype = tree->opval.val->etype;
              tree->opval.val->type = tree->opval.val->sym->type;
              werrorfl (tree->filename, tree->lineno, W_IMPLICIT_FUNC,
                        tree->opval.val->sym->name);
              //tree->opval.val->sym->undefined = 1;
              allocVariables (tree->opval.val->sym);
            }
          else
            {
              tree->opval.val->sym->undefined = 1;
              tree->opval.val->type =
                tree->opval.val->etype = newIntLink ();
              tree->opval.val->sym->type =
                tree->opval.val->sym->etype = newIntLink ();
            }
        }
    }

resolveChildren:
  resolveSymbols (tree->left);
  resolveSymbols (tree->right);

  return tree;
}

/*-----------------------------------------------------------------*/
/* setAstLineno - walks a ast tree & sets the line number          */
/*-----------------------------------------------------------------*/
int setAstLineno (ast * tree, int lineno)
{
  if (!tree)
    return 0;

  tree->lineno = lineno;
  setAstLineno (tree->left, lineno);
  setAstLineno (tree->right, lineno);
  return 0;
}

/*-----------------------------------------------------------------*/
/* funcOfType :- function of type with name                        */
/*-----------------------------------------------------------------*/
symbol *
funcOfType (char *name, sym_link * type, sym_link * argType,
            int nArgs, int rent)
{
  symbol *sym;
  /* create the symbol */
  sym = newSymbol (name, 0);

  /* setup return value */
  sym->type = newLink (DECLARATOR);
  DCL_TYPE (sym->type) = FUNCTION;
  sym->type->next = copyLinkChain (type);
  sym->etype = getSpec (sym->type);
  FUNC_ISREENT(sym->type) = rent ? 1 : 0;

  /* if arguments required */
  if (nArgs)
    {
      value *args;
      args = FUNC_ARGS(sym->type) = newValue ();

      while (nArgs--)
        {
          args->type = copyLinkChain (argType);
          args->etype = getSpec (args->type);
          SPEC_EXTR(args->etype)=1;
          if (!nArgs)
            break;
          args = args->next = newValue ();
        }
    }

  /* save it */
  addSymChain (sym);
  sym->cdef = 1;
  allocVariables (sym);
  return sym;

}

/*-----------------------------------------------------------------*/
/* funcOfTypeVarg :- function of type with name and argtype        */
/*-----------------------------------------------------------------*/
symbol *
funcOfTypeVarg (char *name, char * rtype, int nArgs , char **atypes)
{
  
    symbol *sym;
    int i ;
    /* create the symbol */
    sym = newSymbol (name, 0);
    
    /* setup return value */
    sym->type = newLink (DECLARATOR);
    DCL_TYPE (sym->type) = FUNCTION;
    sym->type->next = typeFromStr(rtype);
    sym->etype = getSpec (sym->type);
    
    /* if arguments required */
    if (nArgs) {
        value *args;
        args = FUNC_ARGS(sym->type) = newValue ();
        
        for ( i = 0 ; i < nArgs ; i++ ) {
            args->type = typeFromStr(atypes[i]);
            args->etype = getSpec (args->type);
            SPEC_EXTR(args->etype)=1;
            if ((i + 1) == nArgs) break;
            args = args->next = newValue ();
        }
    }

    /* save it */
    addSymChain (sym);
    sym->cdef = 1;
    allocVariables (sym);
    return sym;

}

/*-----------------------------------------------------------------*/
/* reverseParms - will reverse a parameter tree                    */
/*-----------------------------------------------------------------*/
static void
reverseParms (ast * ptree)
{
  ast *ttree;
  if (!ptree)
    return;

  /* top down if we find a nonParm tree then quit */
  if (ptree->type == EX_OP && ptree->opval.op == PARAM)
    {
      ttree = ptree->left;
      ptree->left = ptree->right;
      ptree->right = ttree;
      reverseParms (ptree->left);
      reverseParms (ptree->right);
    }

  return;
}

/*-----------------------------------------------------------------*/
/* processParms  - makes sure the parameters are okay and do some  */
/*                 processing with them                            */
/*-----------------------------------------------------------------*/
static int
processParms (ast *func,
              value *defParm,
              ast **actParm,
              int *parmNumber, /* unused, although updated */
              bool rightmost)
{
  RESULT_TYPE resultType;
  sym_link *functype;
  
  /* if none of them exist */
  if (!defParm && !*actParm)
    return 0;

  if (defParm)
    {
      if (getenv("DEBUG_SANITY"))
        {
          fprintf (stderr, "processParms: %s ", defParm->name);
        }
      /* make sure the type is complete and sane */
      checkTypeSanity(defParm->etype, defParm->name);
    }

  if (IS_CODEPTR (func->ftype))
    functype = func->ftype->next;
  else
    functype = func->ftype;
    
  /* if the function is being called via a pointer &   */
  /* it has not been defined a reentrant then we cannot */
  /* have parameters                                   */
  if (func->type != EX_VALUE && !IFFUNC_ISREENT (functype) && !options.stackAuto)
    {
      werror (W_NONRENT_ARGS);
      fatalError++;
      return 1;
    }

  /* if defined parameters ended but actual parameters */
  /* exist and this is not defined as a variable arg   */
  if (!defParm && *actParm && !IFFUNC_HASVARARGS(functype))
    {
      werror (E_TOO_MANY_PARMS);
      return 1;
    }

  /* if defined parameters present but no actual parameters */
  if (defParm && !*actParm)
    {
      werror (E_TOO_FEW_PARMS);
      return 1;
    }

  /* if this is a PARAM node then match left & right */
  if ((*actParm)->type == EX_OP && (*actParm)->opval.op == PARAM)
    {
      (*actParm)->decorated = 1;
      return (processParms (func, defParm,
                            &(*actParm)->left,  parmNumber, FALSE) ||
              processParms (func, defParm ? defParm->next : NULL,
                            &(*actParm)->right, parmNumber, rightmost));
    }
  else if (defParm) /* not vararg */
    {
      /* If we have found a value node by following only right-hand links,
       * then we know that there are no more values after us.
       *
       * Therefore, if there are more defined parameters, the caller didn't
       * supply enough.
       */
      if (rightmost && defParm->next)
        {
          werror (E_TOO_FEW_PARMS);
          return 1;
        }
    }

  /* decorate parameter */
  resultType = defParm ? getResultTypeFromType (defParm->etype) :
                         RESULT_TYPE_NONE;
  *actParm = decorateType (*actParm, resultType);

  if (IS_VOID((*actParm)->ftype))
    {
      werror (E_VOID_VALUE_USED);
      return 1;
    }

  /* If this is a varargs function... */
  if (!defParm && *actParm && IFFUNC_HASVARARGS(functype))
    {
      ast *newType = NULL;
      sym_link *ftype;

      if (IS_CAST_OP (*actParm)
          || (IS_AST_LIT_VALUE (*actParm) && (*actParm)->values.literalFromCast))
        {
          /* Parameter was explicitly typecast; don't touch it. */
          return 0;
        }

      ftype = (*actParm)->ftype;

      /* If it's a char, upcast to int. */
      if (IS_INTEGRAL (ftype)
          && (getSize (ftype) < (unsigned) INTSIZE))
        {
          newType = newAst_LINK(INTTYPE);  
        }

      if (IS_PTR(ftype) && !IS_GENPTR(ftype))
        {
          newType = newAst_LINK (copyLinkChain(ftype));
          DCL_TYPE (newType->opval.lnk) = port->unqualified_pointer;
        }

      if (IS_AGGREGATE (ftype))
        {
          newType = newAst_LINK (copyLinkChain (ftype));
          DCL_TYPE (newType->opval.lnk) = port->unqualified_pointer;
        }
      
      if (newType)
        {
          /* cast required; change this op to a cast. */
          (*actParm)->decorated = 0;
           *actParm = newNode (CAST, newType, *actParm);
          (*actParm)->lineno = (*actParm)->right->lineno;
          
          decorateType (*actParm, RESULT_TYPE_NONE);
        }
      return 0;
    } /* vararg */

  /* if defined parameters ended but actual has not & */
  /* reentrant */
  if (!defParm && *actParm &&
      (options.stackAuto || IFFUNC_ISREENT (functype)))
    return 0;

  resolveSymbols (*actParm);
  
  /* the parameter type must be at least castable */
  if (compareType (defParm->type, (*actParm)->ftype) == 0)
    {
      werror (E_INCOMPAT_TYPES);
      printFromToType ((*actParm)->ftype, defParm->type);
      return 1;
    }

  /* if the parameter is castable then add the cast */
  if (compareType (defParm->type, (*actParm)->ftype) < 0)
    {
      ast *pTree;

      resultType = getResultTypeFromType (defParm->etype);
      pTree = resolveSymbols (copyAst (*actParm));
      
      /* now change the current one to a cast */
      (*actParm)->type = EX_OP;
      (*actParm)->opval.op = CAST;
      (*actParm)->left = newAst_LINK (defParm->type);
      (*actParm)->right = pTree;
      (*actParm)->decorated = 0; /* force typechecking */
      decorateType (*actParm, resultType);
    }

  /* make a copy and change the regparm type to the defined parm */
  (*actParm)->etype = getSpec ((*actParm)->ftype = copyLinkChain ((*actParm)->ftype));
  SPEC_REGPARM ((*actParm)->etype) = SPEC_REGPARM (defParm->etype);
  SPEC_ARGREG  ((*actParm)->etype) = SPEC_ARGREG (defParm->etype);
  (*parmNumber)++;
  return 0;
}

/*-----------------------------------------------------------------*/
/* createIvalType - generates ival for basic types                 */
/*-----------------------------------------------------------------*/
static ast *
createIvalType (ast * sym, sym_link * type, initList * ilist)
{
  ast *iExpr;

  /* if initList is deep */
  if (ilist->type == INIT_DEEP)
    ilist = ilist->init.deep;

  iExpr = decorateType (resolveSymbols (list2expr (ilist)), RESULT_CHECK);
  return decorateType (newNode ('=', sym, iExpr), RESULT_CHECK);
}

/*-----------------------------------------------------------------*/
/* createIvalStruct - generates initial value for structures       */
/*-----------------------------------------------------------------*/
static ast *
createIvalStruct (ast * sym, sym_link * type, initList * ilist)
{
  ast *rast = NULL;
  ast *lAst;
  symbol *sflds;
  initList *iloop;

  sflds = SPEC_STRUCT (type)->fields;
  if (ilist->type != INIT_DEEP)
    {
      werror (E_INIT_STRUCT, "");
      return NULL;
    }

  iloop = ilist->init.deep;

  for (; sflds; sflds = sflds->next, iloop = (iloop ? iloop->next : NULL))
    {
      /* if we have come to end */
      if (!iloop)
        break;
      sflds->implicit = 1;
      lAst = newNode (PTR_OP, newNode ('&', sym, NULL), newAst_VALUE (symbolVal (sflds)));
      lAst = decorateType (resolveSymbols (lAst), RESULT_CHECK);
      rast = decorateType (resolveSymbols (createIval (lAst, sflds->type, iloop, rast)), RESULT_CHECK);
    }

  if (iloop) {
    werrorfl (sym->opval.val->sym->fileDef, sym->opval.val->sym->lineDef,
              W_EXCESS_INITIALIZERS, "struct", 
              sym->opval.val->sym->name);
  }

  return rast;
}


/*-----------------------------------------------------------------*/
/* createIvalArray - generates code for array initialization       */
/*-----------------------------------------------------------------*/
static ast *
createIvalArray (ast * sym, sym_link * type, initList * ilist)
{
  ast *rast = NULL;
  initList *iloop;
  int lcnt = 0, size = 0;
  literalList *literalL;

  /* take care of the special   case  */
  /* array of characters can be init  */
  /* by a string                      */
  if (IS_CHAR (type->next))
    if ((rast = createIvalCharPtr (sym,
                                   type,
                        decorateType (resolveSymbols (list2expr (ilist)), RESULT_CHECK))))

      return decorateType (resolveSymbols (rast), RESULT_CHECK);

    /* not the special case             */
    if (ilist->type != INIT_DEEP)
    {
        werror (E_INIT_STRUCT, "");
        return NULL;
    }

    iloop = ilist->init.deep;
    lcnt = DCL_ELEM (type);

    if (port->arrayInitializerSuppported && convertIListToConstList(ilist, &literalL))
    {
        ast *aSym;

        aSym = decorateType (resolveSymbols(sym), RESULT_CHECK);
        
        rast = newNode(ARRAYINIT, aSym, NULL);
        rast->values.constlist = literalL;
        
        // Make sure size is set to length of initializer list.
        while (iloop)
        {
            size++;
            iloop = iloop->next;
        }
        
        if (lcnt && size > lcnt)
        {
            // Array size was specified, and we have more initializers than needed.
            char *name=sym->opval.val->sym->name;
            int lineno=sym->opval.val->sym->lineDef;
            char *filename=sym->opval.val->sym->fileDef;
            
            werrorfl (filename, lineno, W_EXCESS_INITIALIZERS, "array", name);
        }
    }
    else
    {
        for (;;)
        {
            ast *aSym;
            
            aSym = newNode ('[', sym, newAst_VALUE (valueFromLit ((float) (size++))));
            aSym = decorateType (resolveSymbols (aSym), RESULT_CHECK);
            rast = createIval (aSym, type->next, iloop, rast);
            iloop = (iloop ? iloop->next : NULL);
            if (!iloop)
            {
                break;
            }
            
            /* no of elements given and we    */
            /* have generated for all of them */
            if (!--lcnt) 
            {
                // is this a better way? at least it won't crash
                char *name = (IS_AST_SYM_VALUE(sym)) ? AST_SYMBOL(sym)->name : "";
                int lineno = iloop->lineno;
                char *filename = iloop->filename;
                werrorfl (filename, lineno, W_EXCESS_INITIALIZERS, "array", name);
                
                break;
            }
        }
    }

    /* if we have not been given a size  */
    if (!DCL_ELEM (type))
    {
        /* but this still updates the typedef instead of the instance ! see bug 770487 */
        DCL_ELEM (type) = size;
    }

    return decorateType (resolveSymbols (rast), RESULT_CHECK);
}


/*-----------------------------------------------------------------*/
/* createIvalCharPtr - generates initial values for char pointers  */
/*-----------------------------------------------------------------*/
static ast *
createIvalCharPtr (ast * sym, sym_link * type, ast * iexpr)
{
  ast *rast = NULL;

  /* if this is a pointer & right is a literal array then */
  /* just assignment will do                              */
  if (IS_PTR (type) && ((IS_LITERAL (iexpr->etype) ||
                         SPEC_SCLS (iexpr->etype) == S_CODE)
                        && IS_ARRAY (iexpr->ftype)))
    return newNode ('=', sym, iexpr);

  /* left side is an array so we have to assign each */
  /* element                                         */
  if ((IS_LITERAL (iexpr->etype) ||
       SPEC_SCLS (iexpr->etype) == S_CODE)
      && IS_ARRAY (iexpr->ftype))
    {
      /* for each character generate an assignment */
      /* to the array element */
      char *s = SPEC_CVAL (iexpr->etype).v_char;
      int i = 0;
      int size = getSize (iexpr->ftype);
      int symsize = getSize (type);
      
      if (size>symsize)
        {
          if (size>(symsize+1))
            werrorfl (iexpr->filename, iexpr->lineno, W_EXCESS_INITIALIZERS,
                      "string", sym->opval.val->sym->name);
          size = symsize;
        }

      for (i=0;i<size;i++)
        {
          rast = newNode (NULLOP,
                          rast,
                          newNode ('=',
                                   newNode ('[', sym,
                                   newAst_VALUE (valueFromLit ((float) i))),
                                   newAst_VALUE (valueFromLit (*s))));
          s++;
        }

      // now WE don't need iexpr's symbol anymore
      freeStringSymbol(AST_SYMBOL(iexpr));

      return decorateType (resolveSymbols (rast), RESULT_CHECK);
    }

  return NULL;
}

/*-----------------------------------------------------------------*/
/* createIvalPtr - generates initial value for pointers            */
/*-----------------------------------------------------------------*/
static ast *
createIvalPtr (ast * sym, sym_link * type, initList * ilist)
{
  ast *rast;
  ast *iexpr;

  /* if deep then   */
  if (ilist->type == INIT_DEEP)
    ilist = ilist->init.deep;

  iexpr = decorateType (resolveSymbols (list2expr (ilist)), RESULT_CHECK);

  /* if character pointer */
  if (IS_CHAR (type->next))
    if ((rast = createIvalCharPtr (sym, type, iexpr)))
      return rast;

  return newNode ('=', sym, iexpr);
}

/*-----------------------------------------------------------------*/
/* createIval - generates code for initial value                   */
/*-----------------------------------------------------------------*/
static ast *
createIval (ast * sym, sym_link * type, initList * ilist, ast * wid)
{
  ast *rast = NULL;

  if (!ilist)
    return NULL;

  /* if structure then    */
  if (IS_STRUCT (type))
    rast = createIvalStruct (sym, type, ilist);
  else
    /* if this is a pointer */
  if (IS_PTR (type))
    rast = createIvalPtr (sym, type, ilist);
  else
    /* if this is an array   */
  if (IS_ARRAY (type))
    rast = createIvalArray (sym, type, ilist);
  else
    /* if type is SPECIFIER */
  if (IS_SPEC (type))
    rast = createIvalType (sym, type, ilist);

  if (wid)
    return decorateType (resolveSymbols (newNode (NULLOP, wid, rast)), RESULT_CHECK);
  else
    return decorateType (resolveSymbols (rast), RESULT_CHECK);
}

/*-----------------------------------------------------------------*/
/* initAggregates - initialises aggregate variables with initv     */
/*-----------------------------------------------------------------*/
ast * initAggregates (symbol * sym, initList * ival, ast * wid) {
  return createIval (newAst_VALUE (symbolVal (sym)), sym->type, ival, wid);
}

/*-----------------------------------------------------------------*/
/* gatherAutoInit - creates assignment expressions for initial     */
/*    values                 */
/*-----------------------------------------------------------------*/
static ast *
gatherAutoInit (symbol * autoChain)
{
  ast *init = NULL;
  ast *work;
  symbol *sym;

  inInitMode = 1;
  for (sym = autoChain; sym; sym = sym->next)
    {

      /* resolve the symbols in the ival */
      if (sym->ival)
        resolveIvalSym (sym->ival, sym->type);

      /* if this is a static variable & has an */
      /* initial value the code needs to be lifted */
      /* here to the main portion since they can be */
      /* initialised only once at the start    */
      if (IS_STATIC (sym->etype) && sym->ival &&
          SPEC_SCLS (sym->etype) != S_CODE)
        {
          symbol *newSym;
          
          /* insert the symbol into the symbol table */
          /* with level = 0 & name = rname       */
          newSym = copySymbol (sym);
          addSym (SymbolTab, newSym, newSym->rname, 0, 0, 1);

          /* now lift the code to main */
          if (IS_AGGREGATE (sym->type)) {
            work = initAggregates (sym, sym->ival, NULL);
          } else {
            if (getNelements(sym->type, sym->ival)>1) {
              werrorfl (sym->fileDef, sym->lineDef,
                        W_EXCESS_INITIALIZERS, "scalar", 
                        sym->name);
            }
            work = newNode ('=', newAst_VALUE (symbolVal (newSym)),
                            list2expr (sym->ival));
          }

          setAstLineno (work, sym->lineDef);

          sym->ival = NULL;
          if (staticAutos)
            staticAutos = newNode (NULLOP, staticAutos, work);
          else
            staticAutos = work;

          continue;
        }

      /* if there is an initial value */
      if (sym->ival && SPEC_SCLS (sym->etype) != S_CODE)
        {
          initList *ilist=sym->ival;
          
          while (ilist->type == INIT_DEEP) {
            ilist = ilist->init.deep;
          }

          /* update lineno for error msg */
          lineno=sym->lineDef;
          setAstLineno (ilist->init.node, lineno);
          
          if (IS_AGGREGATE (sym->type)) {
            work = initAggregates (sym, sym->ival, NULL);
          } else {
            if (getNelements(sym->type, sym->ival)>1) {
              werrorfl (sym->fileDef, sym->lineDef,
                        W_EXCESS_INITIALIZERS, "scalar", 
                        sym->name);
            }
            work = newNode ('=', newAst_VALUE (symbolVal (sym)),
                            list2expr (sym->ival));
          }
          
          // just to be sure
          setAstLineno (work, sym->lineDef);

          sym->ival = NULL;
          if (init)
            init = newNode (NULLOP, init, work);
          else
            init = work;
        }
    }
  inInitMode = 0;
  return init;
}

/*-----------------------------------------------------------------*/
/* freeStringSymbol - delete a literal string if no more usage     */
/*-----------------------------------------------------------------*/
void freeStringSymbol(symbol *sym) {
  /* make sure this is a literal string */
  assert (sym->isstrlit);
  if (--sym->isstrlit == 0) { // lower the usage count
    memmap *segment=SPEC_OCLS(sym->etype);
    if (segment) {
      deleteSetItem(&segment->syms, sym);
    }
  }
}
  
/*-----------------------------------------------------------------*/
/* stringToSymbol - creates a symbol from a literal string         */
/*-----------------------------------------------------------------*/
static value *
stringToSymbol (value * val)
{
  char name[SDCC_NAME_MAX + 1];
  static int charLbl = 0;
  symbol *sym;
  set *sp;
  int size;

  // have we heard this before?
  for (sp=statsg->syms; sp; sp=sp->next) {
    sym=sp->item;
    size = getSize (sym->type);
    if (sym->isstrlit && size == getSize (val->type) &&
        !memcmp(SPEC_CVAL(sym->etype).v_char, SPEC_CVAL(val->etype).v_char, size)) {
      // yes, this is old news. Don't publish it again.
      sym->isstrlit++; // but raise the usage count
      return symbolVal(sym);
    }
  }

  SNPRINTF (name, sizeof(name), "_str_%d", charLbl++);
  sym = newSymbol (name, 0);    /* make it @ level 0 */
  strncpyz (sym->rname, name, SDCC_NAME_MAX);

  /* copy the type from the value passed */
  sym->type = copyLinkChain (val->type);
  sym->etype = getSpec (sym->type);
  /* change to storage class & output class */
  SPEC_SCLS (sym->etype) = S_CODE;
  SPEC_CVAL (sym->etype).v_char = SPEC_CVAL (val->etype).v_char;
  SPEC_STAT (sym->etype) = 1;
  /* make the level & block = 0 */
  sym->block = sym->level = 0;
  sym->isstrlit = 1;
  /* create an ival */
  sym->ival = newiList (INIT_NODE, newAst_VALUE (val));
  if (noAlloc == 0)
    {
      /* allocate it */
      addSymChain (sym);
      allocVariables (sym);
    }
  sym->ival = NULL;
  return symbolVal (sym);

}

/*-----------------------------------------------------------------*/
/* processBlockVars - will go thru the ast looking for block if    */
/*                    a block is found then will allocate the syms */
/*                    will also gather the auto inits present      */
/*-----------------------------------------------------------------*/
ast *
processBlockVars (ast * tree, int *stack, int action)
{
  if (!tree)
    return NULL;

  /* if this is a block */
  if (tree->type == EX_OP && tree->opval.op == BLOCK)
    {
      ast *autoInit;

      if (action == ALLOCATE)
        {
          *stack += allocVariables (tree->values.sym);
          autoInit = gatherAutoInit (tree->values.sym);
        
          /* if there are auto inits then do them */
          if (autoInit)
            tree->left = newNode (NULLOP, autoInit, tree->left);
        }
      else                      /* action is deallocate */
        deallocLocal (tree->values.sym);
    }

  processBlockVars (tree->left, stack, action);
  processBlockVars (tree->right, stack, action);
  return tree;
}


/*-------------------------------------------------------------*/
/* constExprTree - returns TRUE if this tree is a constant     */
/*                 expression                                  */
/*-------------------------------------------------------------*/
bool constExprTree (ast *cexpr) {

  if (!cexpr) {
    return TRUE;
  }

  cexpr = decorateType (resolveSymbols (cexpr), RESULT_CHECK);

  switch (cexpr->type) 
    {
    case EX_VALUE:
      if (IS_AST_LIT_VALUE(cexpr)) {
        // this is a literal
        return TRUE;
      }
      if (IS_AST_SYM_VALUE(cexpr) && IS_FUNC(AST_SYMBOL(cexpr)->type)) {
        // a function's address will never change
        return TRUE;
      }
      if (IS_AST_SYM_VALUE(cexpr) && IS_ARRAY(AST_SYMBOL(cexpr)->type)) {
        // an array's address will never change
        return TRUE;
      }
      if (IS_AST_SYM_VALUE(cexpr) && 
          IN_CODESPACE(SPEC_OCLS(AST_SYMBOL(cexpr)->etype))) {
        // a symbol in code space will never change
        // This is only for the 'char *s="hallo"' case and will have to leave
        //printf(" code space symbol");
        return TRUE;
      }
      return FALSE;
    case EX_LINK:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "unexpected link in expression tree\n");
      return FALSE;
    case EX_OP:
      if (cexpr->opval.op==ARRAYINIT) {
        // this is a list of literals
        return TRUE;
      }
      if (cexpr->opval.op=='=') {
        return constExprTree(cexpr->right);
      }
      if (cexpr->opval.op==CAST) {
        // cast ignored, maybe we should throw a warning here?
        return constExprTree(cexpr->right);
      }
      if (cexpr->opval.op=='&') { 
        return TRUE;
      }
      if (cexpr->opval.op==CALL || cexpr->opval.op==PCALL) {
        return FALSE;
      }
      if (constExprTree(cexpr->left) && constExprTree(cexpr->right)) {
        return TRUE;
      }
      return FALSE;
    case EX_OPERAND:
      return IS_CONSTANT(operandType(cexpr->opval.oprnd));
    }
  return FALSE;
}
    
/*-----------------------------------------------------------------*/
/* constExprValue - returns the value of a constant expression     */
/*                  or NULL if it is not a constant expression     */
/*-----------------------------------------------------------------*/
value *
constExprValue (ast * cexpr, int check)
{
  cexpr = decorateType (resolveSymbols (cexpr), RESULT_CHECK);

  /* if this is not a constant then */
  if (!IS_LITERAL (cexpr->ftype))
    {
      /* then check if this is a literal array
         in code segment */
      if (SPEC_SCLS (cexpr->etype) == S_CODE &&
          SPEC_CVAL (cexpr->etype).v_char &&
          IS_ARRAY (cexpr->ftype))
        {
          value *val = valFromType (cexpr->ftype);
          SPEC_SCLS (val->etype) = S_LITERAL;
          val->sym = cexpr->opval.val->sym;
          val->sym->type = copyLinkChain (cexpr->ftype);
          val->sym->etype = getSpec (val->sym->type);
          strncpyz (val->name, cexpr->opval.val->sym->rname, SDCC_NAME_MAX);
          return val;
        }

      /* if we are casting a literal value then */
      if (IS_AST_OP (cexpr) &&
          cexpr->opval.op == CAST &&
          IS_LITERAL (cexpr->right->ftype))
        {
        return valCastLiteral (cexpr->ftype,
                               floatFromVal (cexpr->right->opval.val));
        }

      if (IS_AST_VALUE (cexpr))
        {
        return cexpr->opval.val;
        }

      if (check)
        werror (E_CONST_EXPECTED, "found expression");

      return NULL;
    }

  /* return the value */
  return cexpr->opval.val;

}

/*-----------------------------------------------------------------*/
/* isLabelInAst - will return true if a given label is found       */
/*-----------------------------------------------------------------*/
bool 
isLabelInAst (symbol * label, ast * tree)
{
  if (!tree || IS_AST_VALUE (tree) || IS_AST_LINK (tree))
    return FALSE;

  if (IS_AST_OP (tree) &&
      tree->opval.op == LABEL &&
      isSymbolEqual (AST_SYMBOL (tree->left), label))
    return TRUE;

  return isLabelInAst (label, tree->right) &&
    isLabelInAst (label, tree->left);

}

/*-----------------------------------------------------------------*/
/* isLoopCountable - return true if the loop count can be determi- */
/* -ned at compile time .                                          */
/*-----------------------------------------------------------------*/
bool 
isLoopCountable (ast * initExpr, ast * condExpr, ast * loopExpr,
                 symbol ** sym, ast ** init, ast ** end)
{

  /* the loop is considered countable if the following
     conditions are true :-

     a) initExpr :- <sym> = <const>
     b) condExpr :- <sym> < <const1>
     c) loopExpr :- <sym> ++
   */

  /* first check the initExpr */
  if (IS_AST_OP (initExpr) &&
      initExpr->opval.op == '=' &&      /* is assignment */
      IS_AST_SYM_VALUE (initExpr->left))
    {                           /* left is a symbol */

      *sym = AST_SYMBOL (initExpr->left);
      *init = initExpr->right;
    }
  else
    return FALSE;

  /* for now the symbol has to be of
     integral type */
  if (!IS_INTEGRAL ((*sym)->type))
    return FALSE;

  /* now check condExpr */
  if (IS_AST_OP (condExpr))
    {

      switch (condExpr->opval.op)
        {
        case '<':
          if (IS_AST_SYM_VALUE (condExpr->left) &&
              isSymbolEqual (*sym, AST_SYMBOL (condExpr->left)) &&
              IS_AST_LIT_VALUE (condExpr->right))
            {
              *end = condExpr->right;
              break;
            }
          return FALSE;

        case '!':
          if (IS_AST_OP (condExpr->left) &&
              condExpr->left->opval.op == '>' &&
              IS_AST_LIT_VALUE (condExpr->left->right) &&
              IS_AST_SYM_VALUE (condExpr->left->left) &&
              isSymbolEqual (*sym, AST_SYMBOL (condExpr->left->left)))
            {

              *end = newNode ('+', condExpr->left->right,
                              newAst_VALUE (constVal ("1")));
              break;
            }
          return FALSE;

        default:
          return FALSE;
        }

    }

  /* check loop expression is of the form <sym>++ */
  if (!IS_AST_OP (loopExpr))
    return FALSE;

  /* check if <sym> ++ */
  if (loopExpr->opval.op == INC_OP)
    {

      if (loopExpr->left)
        {
          /* pre */
          if (IS_AST_SYM_VALUE (loopExpr->left) &&
              isSymbolEqual (*sym, AST_SYMBOL (loopExpr->left)))
            return TRUE;

        }
      else
        {
          /* post */
          if (IS_AST_SYM_VALUE (loopExpr->right) &&
              isSymbolEqual (*sym, AST_SYMBOL (loopExpr->right)))
            return TRUE;
        }

    }
  else
    {
      /* check for += */
      if (loopExpr->opval.op == ADD_ASSIGN)
        {

          if (IS_AST_SYM_VALUE (loopExpr->left) &&
              isSymbolEqual (*sym, AST_SYMBOL (loopExpr->left)) &&
              IS_AST_LIT_VALUE (loopExpr->right) &&
              (int) AST_LIT_VALUE (loopExpr->right) != 1)
            return TRUE;
        }
    }

  return FALSE;
}

/*-----------------------------------------------------------------*/
/* astHasVolatile - returns true if ast contains any volatile      */
/*-----------------------------------------------------------------*/
bool 
astHasVolatile (ast * tree)
{
  if (!tree)
    return FALSE;

  if (TETYPE (tree) && IS_VOLATILE (TETYPE (tree)))
    return TRUE;

  if (IS_AST_OP (tree))
    return astHasVolatile (tree->left) ||
      astHasVolatile (tree->right);
  else
    return FALSE;
}

/*-----------------------------------------------------------------*/
/* astHasPointer - return true if the ast contains any ptr variable */
/*-----------------------------------------------------------------*/
bool 
astHasPointer (ast * tree)
{
  if (!tree)
    return FALSE;

  if (IS_AST_LINK (tree))
    return TRUE;

  /* if we hit an array expression then check
     only the left side */
  if (IS_AST_OP (tree) && tree->opval.op == '[')
    return astHasPointer (tree->left);

  if (IS_AST_VALUE (tree))
    return IS_PTR (tree->ftype) || IS_ARRAY (tree->ftype);

  return astHasPointer (tree->left) ||
    astHasPointer (tree->right);

}

/*-----------------------------------------------------------------*/
/* astHasSymbol - return true if the ast has the given symbol      */
/*-----------------------------------------------------------------*/
bool 
astHasSymbol (ast * tree, symbol * sym)
{
  if (!tree || IS_AST_LINK (tree))
    return FALSE;

  if (IS_AST_VALUE (tree))
    {
      if (IS_AST_SYM_VALUE (tree))
        return isSymbolEqual (AST_SYMBOL (tree), sym);
      else
        return FALSE;
    }

  return astHasSymbol (tree->left, sym) ||
    astHasSymbol (tree->right, sym);
}

/*-----------------------------------------------------------------*/
/* astHasDeref - return true if the ast has an indirect access     */
/*-----------------------------------------------------------------*/
static bool 
astHasDeref (ast * tree)
{
  if (!tree || IS_AST_LINK (tree) || IS_AST_VALUE(tree))
    return FALSE;

  if (tree->opval.op == '*' && tree->right == NULL) return TRUE;
  
  return astHasDeref (tree->left) || astHasDeref (tree->right);
}

/*-----------------------------------------------------------------*/
/* isConformingBody - the loop body has to conform to a set of rules */
/* for the loop to be considered reversible read on for rules      */
/*-----------------------------------------------------------------*/
bool 
isConformingBody (ast * pbody, symbol * sym, ast * body)
{

  /* we are going to do a pre-order traversal of the
     tree && check for the following conditions. (essentially
     a set of very shallow tests )
     a) the sym passed does not participate in
     any arithmetic operation
     b) There are no function calls
     c) all jumps are within the body
     d) address of loop control variable not taken
     e) if an assignment has a pointer on the
     left hand side make sure right does not have
     loop control variable */

  /* if we reach the end or a leaf then true */
  if (!pbody || IS_AST_LINK (pbody) || IS_AST_VALUE (pbody))
    return TRUE;
  
  /* if anything else is "volatile" */
  if (IS_VOLATILE (TETYPE (pbody)))
    return FALSE;

  /* we will walk the body in a pre-order traversal for
     efficiency sake */
  switch (pbody->opval.op)
    {
/*------------------------------------------------------------------*/
    case '[':
      // if the loopvar is used as an index
      /* array op is commutative -- must check both left & right */
      if (astHasSymbol(pbody->right, sym) || astHasSymbol(pbody->left, sym)) {
        return FALSE;
      }
      return isConformingBody (pbody->right, sym, body)
              && isConformingBody (pbody->left, sym, body);

/*------------------------------------------------------------------*/
    case PTR_OP:
    case '.':
      return TRUE;

/*------------------------------------------------------------------*/
    case INC_OP:
    case DEC_OP:

      /* sure we are not sym is not modified */
      if (pbody->left &&
          IS_AST_SYM_VALUE (pbody->left) &&
          isSymbolEqual (AST_SYMBOL (pbody->left), sym))
        return FALSE;

      if (pbody->right &&
          IS_AST_SYM_VALUE (pbody->right) &&
          isSymbolEqual (AST_SYMBOL (pbody->right), sym))
        return FALSE;

      return TRUE;

/*------------------------------------------------------------------*/

    case '*':                   /* can be unary  : if right is null then unary operation */
    case '+':
    case '-':
    case '&':

      /* if right is NULL then unary operation  */
/*------------------------------------------------------------------*/
/*----------------------------*/
      /*  address of                */
/*----------------------------*/
      if (!pbody->right)
        {
          if (IS_AST_SYM_VALUE (pbody->left) &&
              isSymbolEqual (AST_SYMBOL (pbody->left), sym))
            return FALSE;
          else
            return isConformingBody (pbody->left, sym, body);
        }
      else
        {
          if (astHasSymbol (pbody->left, sym) ||
              astHasSymbol (pbody->right, sym))
            return FALSE;
        }


/*------------------------------------------------------------------*/
    case '|':
    case '^':
    case '/':
    case '%':
    case LEFT_OP:
    case RIGHT_OP:

      if (IS_AST_SYM_VALUE (pbody->left) &&
          isSymbolEqual (AST_SYMBOL (pbody->left), sym))
        return FALSE;

      if (IS_AST_SYM_VALUE (pbody->right) &&
          isSymbolEqual (AST_SYMBOL (pbody->right), sym))
        return FALSE;

      return isConformingBody (pbody->left, sym, body) &&
        isConformingBody (pbody->right, sym, body);

    case '~':
    case '!':
    case RRC:
    case RLC:
    case GETHBIT:
    case SWAP:
      if (IS_AST_SYM_VALUE (pbody->left) &&
          isSymbolEqual (AST_SYMBOL (pbody->left), sym))
        return FALSE;
      return isConformingBody (pbody->left, sym, body);

/*------------------------------------------------------------------*/

    case AND_OP:
    case OR_OP:
    case '>':
    case '<':
    case LE_OP:
    case GE_OP:
    case EQ_OP:
    case NE_OP:
    case '?':
    case ':':
    case SIZEOF:                /* evaluate wihout code generation */

      if (IS_AST_SYM_VALUE (pbody->left) &&
          isSymbolEqual (AST_SYMBOL (pbody->left), sym))
        return FALSE;

      if (IS_AST_SYM_VALUE (pbody->right) &&
          isSymbolEqual (AST_SYMBOL (pbody->right), sym))
        return FALSE;

      return isConformingBody (pbody->left, sym, body) &&
        isConformingBody (pbody->right, sym, body);

/*------------------------------------------------------------------*/
    case '=':

      /* if left has a pointer & right has loop
         control variable then we cannot */
      if (astHasPointer (pbody->left) &&
          astHasSymbol (pbody->right, sym))
        return FALSE;
      if (astHasVolatile (pbody->left))
        return FALSE;

      if (IS_AST_SYM_VALUE (pbody->left)) {
        // if the loopvar has an assignment
        if (isSymbolEqual (AST_SYMBOL (pbody->left), sym))
          return FALSE;
        // if the loopvar is used in another (maybe conditional) block
        if (astHasSymbol (pbody->right, sym) &&
            (pbody->level >= body->level)) {
          return FALSE;
        }
      }

      if (astHasVolatile (pbody->left))
        return FALSE;

      if (astHasDeref(pbody->right)) return FALSE;

      return isConformingBody (pbody->left, sym, body) &&
        isConformingBody (pbody->right, sym, body);

    case MUL_ASSIGN:
    case DIV_ASSIGN:
    case AND_ASSIGN:
    case OR_ASSIGN:
    case XOR_ASSIGN:
    case RIGHT_ASSIGN:
    case LEFT_ASSIGN:
    case SUB_ASSIGN:
    case ADD_ASSIGN:
      assert ("Parser should not have generated this\n");

/*------------------------------------------------------------------*/
/*----------------------------*/
      /*      comma operator        */
/*----------------------------*/
    case ',':
      return isConformingBody (pbody->left, sym, body) &&
        isConformingBody (pbody->right, sym, body);

/*------------------------------------------------------------------*/
/*----------------------------*/
      /*       function call        */
/*----------------------------*/
    case CALL:
        /* if local & not passed as paramater then ok */
        if (sym->level && !astHasSymbol(pbody->right,sym)) 
            return TRUE;
      return FALSE;

/*------------------------------------------------------------------*/
/*----------------------------*/
      /*     return statement       */
/*----------------------------*/
    case RETURN:
      return FALSE;

    case GOTO:
      if (isLabelInAst (AST_SYMBOL (pbody->left), body))
        return TRUE;
      else
        return FALSE;
    case SWITCH:
      if (astHasSymbol (pbody->left, sym))
        return FALSE;

    default:
      break;
    }

  return isConformingBody (pbody->left, sym, body) &&
    isConformingBody (pbody->right, sym, body);



}

/*-----------------------------------------------------------------*/
/* isLoopReversible - takes a for loop as input && returns true    */
/* if the for loop is reversible. If yes will set the value of     */
/* the loop control var & init value & termination value           */
/*-----------------------------------------------------------------*/
bool
isLoopReversible (ast * loop, symbol ** loopCntrl,
                  ast ** init, ast ** end)
{
  /* if option says don't do it then don't */
  if (optimize.noLoopReverse)
    return 0;
  /* there are several tests to determine this */

  /* for loop has to be of the form
     for ( <sym> = <const1> ;
     [<sym> < <const2>]  ;
     [<sym>++] | [<sym> += 1] | [<sym> = <sym> + 1] )
     forBody */
  if (!isLoopCountable (AST_FOR (loop, initExpr),
                        AST_FOR (loop, condExpr),
                        AST_FOR (loop, loopExpr),
                        loopCntrl, init, end))
    return 0;

  /* now do some serious checking on the body of the loop
   */

  return isConformingBody (loop->left, *loopCntrl, loop->left);

}

/*-----------------------------------------------------------------*/
/* replLoopSym - replace the loop sym by loop sym -1               */
/*-----------------------------------------------------------------*/
static void 
replLoopSym (ast * body, symbol * sym)
{
  /* reached end */
  if (!body || IS_AST_LINK (body))
    return;

  if (IS_AST_SYM_VALUE (body))
    {

      if (isSymbolEqual (AST_SYMBOL (body), sym))
        {

          body->type = EX_OP;
          body->opval.op = '-';
          body->left = newAst_VALUE (symbolVal (sym));
          body->right = newAst_VALUE (constVal ("1"));

        }

      return;

    }

  replLoopSym (body->left, sym);
  replLoopSym (body->right, sym);

}

/*-----------------------------------------------------------------*/
/* reverseLoop - do the actual loop reversal                       */
/*-----------------------------------------------------------------*/
ast *
reverseLoop (ast * loop, symbol * sym, ast * init, ast * end)
{
  ast *rloop;

  /* create the following tree
     <sym> = loopCount ;
     for_continue:
     forbody
     <sym> -= 1;
     if (sym) goto for_continue ;
     <sym> = end */

  /* put it together piece by piece */
  rloop = newNode (NULLOP,
                   createIf (newAst_VALUE (symbolVal (sym)),
                             newNode (GOTO,
                                      newAst_VALUE (symbolVal (AST_FOR (loop, continueLabel))),
                                      NULL), NULL),
                   newNode ('=',
                            newAst_VALUE (symbolVal (sym)),
                            end));

  replLoopSym (loop->left, sym);
  setAstLineno (rloop, init->lineno);

  rloop = newNode (NULLOP,
                   newNode ('=',
                            newAst_VALUE (symbolVal (sym)),
                            newNode ('-', end, init)),
                   createLabel (AST_FOR (loop, continueLabel),
                                newNode (NULLOP,
                                         loop->left,
                                         newNode (NULLOP,
                                                  newNode (SUB_ASSIGN,
                                                           newAst_VALUE (symbolVal (sym)),
                                                           newAst_VALUE (constVal ("1"))),
                                                  rloop))));

  rloop->lineno=init->lineno;
  return decorateType (rloop, RESULT_CHECK);

}

/*-----------------------------------------------------------------*/
/* searchLitOp - search tree (*ops only) for an ast with literal */
/*-----------------------------------------------------------------*/
static ast *
searchLitOp (ast *tree, ast **parent, const char *ops)
{
  ast *ret;

  if (tree && optimize.global_cse)
    {
      /* is there a literal operand? */
      if (tree->right &&
          IS_AST_OP(tree->right) &&
          tree->right->right &&
          (tree->right->opval.op == ops[0] || tree->right->opval.op == ops[1]))
        {
          if (IS_LITERAL (RTYPE (tree->right)) !=
              IS_LITERAL (LTYPE (tree->right)))
            {
              tree->right->decorated = 0;
              tree->decorated = 0;
              *parent = tree;
              return tree->right;
            }
          ret = searchLitOp (tree->right, parent, ops);
          if (ret)
            return ret;
        }
      if (tree->left &&
          IS_AST_OP(tree->left) &&
          tree->left->right &&
          (tree->left->opval.op == ops[0] || tree->left->opval.op == ops[1]))
        {
          if (IS_LITERAL (RTYPE (tree->left)) !=
              IS_LITERAL (LTYPE (tree->left)))
            {
              tree->left->decorated = 0;
              tree->decorated = 0;
              *parent = tree;
              return tree->left;
            }
          ret = searchLitOp (tree->left, parent, ops);
          if (ret)
            return ret;
        }
    }
  return NULL;
}

/*-----------------------------------------------------------------*/
/* getResultFromType                                               */
/*-----------------------------------------------------------------*/
RESULT_TYPE
getResultTypeFromType (sym_link *type)
{
  /* type = getSpec (type); */
  if (IS_BIT (type))
    return RESULT_TYPE_BIT;
  if (IS_BITFIELD (type))
    {
      int blen = SPEC_BLEN (type);
      
      if (blen <= 1)
        return RESULT_TYPE_BIT;
      if (blen <= 8)
        return RESULT_TYPE_CHAR;
      return RESULT_TYPE_INT;
    }
  if (IS_CHAR (type))
    return RESULT_TYPE_CHAR;
  if (   IS_INT (type)
      && !IS_LONG (type))
    return RESULT_TYPE_INT;
  return RESULT_TYPE_OTHER;
}

/*-----------------------------------------------------------------*/
/* addCast - adds casts to a type specified by RESULT_TYPE         */
/*-----------------------------------------------------------------*/
static ast *
addCast (ast *tree, RESULT_TYPE resultType, bool upcast)
{
  sym_link *newLink;
  bool upCasted = FALSE;
  
  switch (resultType)
    {
      case RESULT_TYPE_NONE:
        /* char: promote to int */
        if (!upcast ||
            getSize (tree->etype) >= INTSIZE)
          return tree;
        newLink = newIntLink();
        upCasted = TRUE;
        break;
      case RESULT_TYPE_CHAR:
        if (IS_CHAR (tree->etype) ||
            IS_FLOAT(tree->etype))
          return tree;
        newLink = newCharLink();
        break;
      case RESULT_TYPE_INT:
#if 0
        if (getSize (tree->etype) > INTSIZE)
          {
            /* warn ("Loosing significant digits"); */
            return;
          }
#endif
        /* char: promote to int */
        if (!upcast ||
            getSize (tree->etype) >= INTSIZE)
          return tree;
        newLink = newIntLink();
        upCasted = TRUE;
        break;
      case RESULT_TYPE_OTHER:
        if (!upcast)
          return tree;
        /* return type is long, float: promote char to int */
        if (getSize (tree->etype) >= INTSIZE)
          return tree;
        newLink = newIntLink();
        upCasted = TRUE;
        break;
      default:
        return tree;
    }
  tree->decorated = 0;
  tree = newNode (CAST, newAst_LINK (newLink), tree);
  tree->lineno = tree->right->lineno;
  /* keep unsigned type during cast to smaller type,
     but not when promoting from char to int */
  if (!upCasted)
    SPEC_USIGN (tree->left->opval.lnk) = IS_UNSIGNED (tree->right->etype) ? 1 : 0;
  return decorateType (tree, resultType);
}

/*-----------------------------------------------------------------*/
/* resultTypePropagate - decides if resultType can be propagated   */
/*-----------------------------------------------------------------*/
static RESULT_TYPE
resultTypePropagate (ast *tree, RESULT_TYPE resultType)
{
  switch (tree->opval.op)
    {
      case '=':
      case '?':
      case ':':
      case '|':
      case '^':
      case '~':
      case '*':
      case '+':
      case '-':
      case LABEL:
        return resultType;
      case '&':
        if (!tree->right)
          /* can be unary */
          return RESULT_TYPE_NONE;
        else
          return resultType;
      case IFX:
        return RESULT_TYPE_IFX;
      default:
        return RESULT_TYPE_NONE;
    }
}

/*-----------------------------------------------------------------*/
/* getLeftResultType - gets type from left branch for propagation  */
/*-----------------------------------------------------------------*/
static RESULT_TYPE
getLeftResultType (ast *tree, RESULT_TYPE resultType)
{
  switch (tree->opval.op)
    {
      case '=':
      case CAST:
        if (IS_PTR (LTYPE (tree)))
          return RESULT_TYPE_NONE;
        else
          return getResultTypeFromType (LETYPE (tree));
      case RETURN:
        if (IS_PTR (currFunc->type->next))
          return RESULT_TYPE_NONE;
        else
          return getResultTypeFromType (currFunc->type->next);
      case '[':
        if (!IS_ARRAY (LTYPE (tree)))
          return resultType;
        if (DCL_ELEM (LTYPE (tree)) > 0 && DCL_ELEM (LTYPE (tree)) <= 256)
          return RESULT_TYPE_CHAR;
        return resultType;
      default:
        return resultType;
    }
}

/*--------------------------------------------------------------------*/
/* decorateType - compute type for this tree, also does type checking.*/
/* This is done bottom up, since type has to flow upwards.            */
/* resultType flows top-down and forces e.g. char-arithmetik, if the  */
/* result is a char and the operand(s) are int's.                     */
/* It also does constant folding, and parameter checking.             */
/*--------------------------------------------------------------------*/
ast *
decorateType (ast * tree, RESULT_TYPE resultType)
{
  int parmNumber;
  sym_link *p;
  RESULT_TYPE resultTypeProp;

  if (!tree)
    return tree;

  /* if already has type then do nothing */
  if (tree->decorated)
    return tree;

  tree->decorated = 1;

#if 0
  /* print the line          */
  /* if not block & function */
  if (tree->type == EX_OP &&
      (tree->opval.op != FUNCTION &&
       tree->opval.op != BLOCK &&
       tree->opval.op != NULLOP))
    {
      filename = tree->filename;
      lineno = tree->lineno;
    }
#endif

  /* if any child is an error | this one is an error do nothing */
  if (tree->isError ||
      (tree->left && tree->left->isError) ||
      (tree->right && tree->right->isError))
    return tree;

/*------------------------------------------------------------------*/
/*----------------------------*/
/*   leaf has been reached    */
/*----------------------------*/
  lineno=tree->lineno;
  /* if this is of type value */
  /* just get the type        */
  if (tree->type == EX_VALUE)
    {

      if (IS_LITERAL (tree->opval.val->etype))
        {

          /* if this is a character array then declare it */
          if (IS_ARRAY (tree->opval.val->type))
            tree->opval.val = stringToSymbol (tree->opval.val);

          /* otherwise just copy the type information */
          COPYTYPE (TTYPE (tree), TETYPE (tree), tree->opval.val->type);
          return tree;
        }

      if (tree->opval.val->sym)
        {
          /* if the undefined flag is set then give error message */
          if (tree->opval.val->sym->undefined)
            {
              werror (E_ID_UNDEF, tree->opval.val->sym->name);
              /* assume int */
              TTYPE (tree) = TETYPE (tree) =
                tree->opval.val->type = tree->opval.val->sym->type =
                tree->opval.val->etype = tree->opval.val->sym->etype =
                copyLinkChain (INTTYPE);
            }
          else
            {

              /* if impilicit i.e. struct/union member then no type */
              if (tree->opval.val->sym->implicit)
                TTYPE (tree) = TETYPE (tree) = NULL;

              else
                {

                  /* else copy the type */
                  COPYTYPE (TTYPE (tree), TETYPE (tree), tree->opval.val->type);

                  /* and mark it as referenced */
                  tree->opval.val->sym->isref = 1;
                }
            }
        }

      return tree;
    }

  /* if type link for the case of cast */
  if (tree->type == EX_LINK)
    {
      COPYTYPE (TTYPE (tree), TETYPE (tree), tree->opval.lnk);
      return tree;
    }

  {
    ast *dtl, *dtr;

    #if 0
    if (tree->opval.op == NULLOP || tree->opval.op == BLOCK)
      {
        if (tree->left && tree->left->type == EX_OPERAND
            && (tree->left->opval.op == INC_OP
                || tree->left->opval.op == DEC_OP)
            && tree->left->left)
          {
            tree->left->right = tree->left->left;
            tree->left->left = NULL;
          }
        if (tree->right && tree->right->type == EX_OPERAND
            && (tree->right->opval.op == INC_OP
                || tree->right->opval.op == DEC_OP)
            && tree->right->left)
          {
            tree->right->right = tree->right->left;
            tree->right->left = NULL;
          }
      }
    #endif

    /* Before decorating the left branch we've to decide in dependence
       upon tree->opval.op, if resultType can be propagated */
    resultTypeProp = resultTypePropagate (tree, resultType);

    if (tree->opval.op == '?')
      dtl = decorateType (tree->left, RESULT_TYPE_IFX);
    else
      dtl = decorateType (tree->left, resultTypeProp);

    /* if an array node, we may need to swap branches */
    if (tree->opval.op == '[')
      {
        /* determine which is the array & which the index */
        if ((IS_ARRAY (RTYPE (tree)) || IS_PTR (RTYPE (tree))) &&
            IS_INTEGRAL (LTYPE (tree)))
          {
            ast *tempTree = tree->left;
            tree->left = tree->right;
            tree->right = tempTree;
          }
      }

    /* After decorating the left branch there's type information available
       in tree->left->?type. If the op is e.g. '=' we extract the type
       information from there and propagate it to the right branch. */
    resultTypeProp = getLeftResultType (tree, resultTypeProp);
    
    switch (tree->opval.op)
      {
        case '?':
          /* delay right side for '?' operator since conditional macro
             expansions might rely on this */
          dtr = tree->right;
          break;
        case CALL: 
          /* decorate right side for CALL (parameter list) in processParms();
             there is resultType available */
          dtr = tree->right;
          break;
        default:     
          dtr = decorateType (tree->right, resultTypeProp);
          break;
      }

    /* this is to take care of situations
       when the tree gets rewritten */
    if (dtl != tree->left)
      tree->left = dtl;
    if (dtr != tree->right)
      tree->right = dtr;
    if ((dtl && dtl->isError) || (dtr && dtr->isError))
      return tree;
  }

  /* depending on type of operator do */

  switch (tree->opval.op)
    {
        /*------------------------------------------------------------------*/
        /*----------------------------*/
        /*        array node          */
        /*----------------------------*/
    case '[':

      /* first check if this is a array or a pointer */
      if ((!IS_ARRAY (LTYPE (tree))) && (!IS_PTR (LTYPE (tree))))
        {
          werror (E_NEED_ARRAY_PTR, "[]");
          goto errorTreeReturn;
        }

      /* check if the type of the idx */
      if (!IS_INTEGRAL (RTYPE (tree)))
        {
          werror (E_IDX_NOT_INT);
          goto errorTreeReturn;
        }

      /* if the left is an rvalue then error */
      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "array access");
          goto errorTreeReturn;
        }

      if (IS_LITERAL (RTYPE (tree)))
        {
          int arrayIndex = (int) floatFromVal (valFromType (RETYPE (tree)));
          int arraySize = DCL_ELEM (LTYPE (tree));
          if (arraySize && arrayIndex >= arraySize)
            {
              werror (W_IDX_OUT_OF_BOUNDS, arrayIndex, arraySize);
            }
        }

      RRVAL (tree) = 1;
      COPYTYPE (TTYPE (tree), TETYPE (tree), LTYPE (tree)->next);
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      struct/union          */
      /*----------------------------*/
    case '.':
      /* if this is not a structure */
      if (!IS_STRUCT (LTYPE (tree)))
        {
          werror (E_STRUCT_UNION, ".");
          goto errorTreeReturn;
        }
      TTYPE (tree) = structElemType (LTYPE (tree),
                                     (tree->right->type == EX_VALUE ?
                               tree->right->opval.val : NULL));
      TETYPE (tree) = getSpec (TTYPE (tree));
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    struct/union pointer    */
      /*----------------------------*/
    case PTR_OP:
      /* if not pointer to a structure */
      if (!IS_PTR (LTYPE (tree)) && !IS_ARRAY (LTYPE(tree)))
        {
          werror (E_PTR_REQD);
          goto errorTreeReturn;
        }

      if (!IS_STRUCT (LTYPE (tree)->next))
        {
          werror (E_STRUCT_UNION, "->");
          goto errorTreeReturn;
        }

      TTYPE (tree) = structElemType (LTYPE (tree)->next,
                                     (tree->right->type == EX_VALUE ?
                               tree->right->opval.val : NULL));
      TETYPE (tree) = getSpec (TTYPE (tree));

      /* adjust the storage class */
      switch (DCL_TYPE(tree->left->ftype)) {
      case POINTER:
        SPEC_SCLS(TETYPE(tree)) = S_DATA; 
        break;
      case FPOINTER:
        SPEC_SCLS(TETYPE(tree)) = S_XDATA; 
        break;
      case CPOINTER:
        SPEC_SCLS(TETYPE(tree)) = S_CODE; 
        break;
      case GPOINTER:
        SPEC_SCLS (TETYPE (tree)) = 0;
        break;
      case PPOINTER:
        SPEC_SCLS(TETYPE(tree)) = S_XSTACK; 
        break;
      case IPOINTER:
        SPEC_SCLS(TETYPE(tree)) = S_IDATA;
        break;
      case EEPPOINTER:
        SPEC_SCLS(TETYPE(tree)) = S_EEPROM;
        break;
      case UPOINTER:
        SPEC_SCLS (TETYPE (tree)) = 0;
        break;
      case ARRAY:
      case FUNCTION:
        break;
      }
      
      /* This breaks with extern declarations, bitfields, and perhaps other */
      /* cases (gcse). Let's leave this optimization disabled for now and   */
      /* ponder if there's a safe way to do this. -- EEP                    */
      #if 0
      if (IS_ADDRESS_OF_OP (tree->left) && IS_AST_SYM_VALUE(tree->left->left)
          && SPEC_ABSA (AST_SYMBOL (tree->left->left)->etype))
        {
            /* If defined    struct type at addr var
               then rewrite  (&struct var)->member
               as            temp
               and define    membertype at (addr+offsetof(struct var,member)) temp
            */
            symbol *sym;
            symbol *element = getStructElement (SPEC_STRUCT (LETYPE(tree)),
                                                AST_SYMBOL(tree->right));

            sym = newSymbol(genSymName (0), 0);
            sym->type = TTYPE (tree);
            sym->etype = getSpec(sym->type);
            sym->lineDef = tree->lineno;
            sym->cdef = 1;
            sym->isref = 1;
            SPEC_STAT (sym->etype) = 1;
            SPEC_ADDR (sym->etype) = SPEC_ADDR (AST_SYMBOL (tree->left->left)->etype)
                                     + element->offset;
            SPEC_ABSA(sym->etype) = 1;
            addSym (SymbolTab, sym, sym->name, 0, 0, 0);
            allocGlobal (sym);
            
            AST_VALUE (tree) = symbolVal(sym);
            TLVAL (tree) = 1;
            TRVAL (tree) = 0;
            tree->type = EX_VALUE;
            tree->left = NULL;
            tree->right = NULL;
        }
      #endif
      
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  ++/-- operation           */
      /*----------------------------*/
    case INC_OP:
    case DEC_OP:
      {
        sym_link *ltc = (tree->right ? RTYPE (tree) : LTYPE (tree));
        COPYTYPE (TTYPE (tree), TETYPE (tree), ltc);
        if (!tree->initMode && IS_CONSTANT(TTYPE(tree)))
          werror (E_CODE_WRITE, tree->opval.op==INC_OP ? "++" : "--");

        if (tree->right)
          RLVAL (tree) = 1;
        else
          LLVAL (tree) = 1;
        return tree;
      }

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  bitwise and               */
      /*----------------------------*/
    case '&':                   /* can be unary   */
      /* if right is NULL then unary operation  */
      if (tree->right)          /* not an unary operation */
        {

          if (!IS_INTEGRAL (LTYPE (tree)) || !IS_INTEGRAL (RTYPE (tree)))
            {
              werror (E_BITWISE_OP);
              werror (W_CONTINUE, "left & right types are ");
              printTypeChain (LTYPE (tree), stderr);
              fprintf (stderr, ",");
              printTypeChain (RTYPE (tree), stderr);
              fprintf (stderr, "\n");
              goto errorTreeReturn;
            }

          /* if they are both literal */
          if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
            {
              tree->type = EX_VALUE;
              tree->opval.val = valBitwise (valFromType (LETYPE (tree)),
                                          valFromType (RETYPE (tree)), '&');

              tree->right = tree->left = NULL;
              TETYPE (tree) = tree->opval.val->etype;
              TTYPE (tree) = tree->opval.val->type;
              return tree;
            }

          /* see if this is a GETHBIT operation if yes
             then return that */
          {
            ast *otree = optimizeGetHbit (tree);

            if (otree != tree)
              return decorateType (otree, RESULT_CHECK);
          }

          /* if left is a literal exchange left & right */
          if (IS_LITERAL (LTYPE (tree)))
            {
              ast *tTree = tree->left;
              tree->left = tree->right;
              tree->right = tTree;
            }

          /* if right is a literal and */
          /* we can find a 2nd literal in an and-tree then */
          /* rearrange the tree */
          if (IS_LITERAL (RTYPE (tree)))
            {
              ast *parent;
              ast *litTree = searchLitOp (tree, &parent, "&");
              if (litTree)
                {
                  DEBUG_CF("&")
                  ast *tTree = litTree->left;
                  litTree->left = tree->right;
                  tree->right = tTree;
                  /* both operands in litTree are literal now */
                  decorateType (parent, resultType);
                }
            }

          LRVAL (tree) = RRVAL (tree) = 1;
          
          /* AND is signless so make signedness of literal equal */
          /* to signedness of left for better optimized code */
          if (IS_LITERAL (RTYPE (tree)) &&
              (getSize(LTYPE(tree)) == getSize(RTYPE(tree))) &&
              (SPEC_USIGN(LTYPE(tree)) != SPEC_USIGN(RTYPE(tree))) )
            {
              SPEC_USIGN(RTYPE(tree)) = SPEC_USIGN(LTYPE(tree));
            }

          TTYPE (tree) = computeType (LTYPE (tree),
                                      RTYPE (tree),
                                      resultType,
                                      tree->opval.op);
          TETYPE (tree) = getSpec (TTYPE (tree));

          return tree;
        }

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  address of                */
      /*----------------------------*/
      p = newLink (DECLARATOR);
      /* if bit field then error */
      if (IS_BITVAR (tree->left->etype))
        {
          werror (E_ILLEGAL_ADDR, "address of bit variable");
          goto errorTreeReturn;
        }

      if (LETYPE(tree) && SPEC_SCLS (tree->left->etype) == S_REGISTER)
        {
          werror (E_ILLEGAL_ADDR, "address of register variable");
          goto errorTreeReturn;
        }

      if (IS_FUNC (LTYPE (tree)))
        {
          // this ought to be ignored
          return (tree->left);
        }

      if (IS_LITERAL(LTYPE(tree)))
        {
          werror (E_ILLEGAL_ADDR, "address of literal");
          goto errorTreeReturn;
        }

     if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "address of");
          goto errorTreeReturn;
        }
      if (!LETYPE (tree))
        DCL_TYPE (p) = POINTER;
      else if (SPEC_SCLS (tree->left->etype) == S_CODE)
        DCL_TYPE (p) = CPOINTER;
      else if (SPEC_SCLS (tree->left->etype) == S_XDATA)
        DCL_TYPE (p) = FPOINTER;
      else if (SPEC_SCLS (tree->left->etype) == S_XSTACK)
        DCL_TYPE (p) = PPOINTER;
      else if (SPEC_SCLS (tree->left->etype) == S_IDATA)
        DCL_TYPE (p) = IPOINTER;
      else if (SPEC_SCLS (tree->left->etype) == S_EEPROM)
        DCL_TYPE (p) = EEPPOINTER;
      else if (SPEC_OCLS(tree->left->etype))
          DCL_TYPE (p) = PTR_TYPE(SPEC_OCLS(tree->left->etype));
      else
          DCL_TYPE (p) = POINTER;

      if (IS_AST_SYM_VALUE (tree->left))
        {
          AST_SYMBOL (tree->left)->addrtaken = 1;
          AST_SYMBOL (tree->left)->allocreq = 1;
        }

      p->next = LTYPE (tree);
      TTYPE (tree) = p;
      TETYPE (tree) = getSpec (TTYPE (tree));
      LLVAL (tree) = 1;
      TLVAL (tree) = 1;

      #if 0
      if (IS_AST_OP (tree->left) && tree->left->opval.op == PTR_OP
          && IS_AST_VALUE (tree->left->left) && !IS_AST_SYM_VALUE (tree->left->left))
        {
          symbol *element = getStructElement (SPEC_STRUCT (LETYPE(tree->left)),
                                      AST_SYMBOL(tree->left->right));
          AST_VALUE(tree) = valPlus(AST_VALUE(tree->left->left),
                                    valueFromLit(element->offset));
          tree->left = NULL;
          tree->right = NULL;
          tree->type = EX_VALUE;
          tree->values.literalFromCast = 1;
        }
      #endif

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  bitwise or                */
      /*----------------------------*/
    case '|':
      /* if the rewrite succeeds then don't go any furthur */
      {
        ast *wtree = optimizeRRCRLC (tree);
        if (wtree != tree)
          return decorateType (wtree, RESULT_CHECK);
        
        wtree = optimizeSWAP (tree);
        if (wtree != tree)
          return decorateType (wtree, RESULT_CHECK);
      }

      /* if left is a literal exchange left & right */
      if (IS_LITERAL (LTYPE (tree)))
        {
          ast *tTree = tree->left;
          tree->left = tree->right;
          tree->right = tTree;
        }

      /* if right is a literal and */
      /* we can find a 2nd literal in an or-tree then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree)))
        {
          ast *parent;
          ast *litTree = searchLitOp (tree, &parent, "|");
          if (litTree)
            {
              DEBUG_CF("|")
              ast *tTree = litTree->left;
              litTree->left = tree->right;
              tree->right = tTree;
              /* both operands in tTree are literal now */
              decorateType (parent, resultType);
            }
        }
      /* fall through */

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  bitwise xor               */
      /*----------------------------*/
    case '^':
      if (!IS_INTEGRAL (LTYPE (tree)) || !IS_INTEGRAL (RTYPE (tree)))
        {
          werror (E_BITWISE_OP);
          werror (W_CONTINUE, "left & right types are ");
          printTypeChain (LTYPE (tree), stderr);
          fprintf (stderr, ",");
          printTypeChain (RTYPE (tree), stderr);
          fprintf (stderr, "\n");
          goto errorTreeReturn;
        }

      /* if they are both literal then rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valBitwise (valFromType (LETYPE (tree)),
                                        valFromType (RETYPE (tree)),
                                        tree->opval.op);
          tree->right = tree->left = NULL;
          TETYPE (tree) = tree->opval.val->etype;
          TTYPE (tree) = tree->opval.val->type;
          return tree;
        }

      /* if left is a literal exchange left & right */
      if (IS_LITERAL (LTYPE (tree)))
        {
          ast *tTree = tree->left;
          tree->left = tree->right;
          tree->right = tTree;
        }

      /* if right is a literal and */
      /* we can find a 2nd literal in a xor-tree then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree)) &&
          tree->opval.op == '^') /* the same source is used by 'bitwise or' */
        {
          ast *parent;
          ast *litTree = searchLitOp (tree, &parent, "^");
          if (litTree)
            {
              DEBUG_CF("^")
              ast *tTree = litTree->left;
              litTree->left = tree->right;
              tree->right = tTree;
              /* both operands in litTree are literal now */
              decorateType (parent, resultType);
            }
        }

      /* OR/XOR are signless so make signedness of literal equal */
      /* to signedness of left for better optimized code */
      if (IS_LITERAL (RTYPE (tree)) &&
          (getSize(LTYPE(tree)) == getSize(RTYPE(tree))) &&
          (SPEC_USIGN(LTYPE(tree)) != SPEC_USIGN(RTYPE(tree))) )
        {
          SPEC_USIGN(RTYPE(tree)) = SPEC_USIGN(LTYPE(tree));
        }

      LRVAL (tree) = RRVAL (tree) = 1;

      TTYPE (tree) = computeType (LTYPE (tree),
                                  RTYPE (tree),
                                  resultType,
                                  tree->opval.op);
      TETYPE (tree) = getSpec (TTYPE (tree));

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  division                  */
      /*----------------------------*/
    case '/':
      if (!IS_ARITHMETIC (LTYPE (tree)) || !IS_ARITHMETIC (RTYPE (tree)))
        {
          werror (E_INVALID_OP, "divide");
          goto errorTreeReturn;
        }
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valDiv (valFromType (LETYPE (tree)),
                                    valFromType (RETYPE (tree)));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }

      LRVAL (tree) = RRVAL (tree) = 1;

      TETYPE (tree) = getSpec (TTYPE (tree) =
                               computeType (LTYPE (tree),
                                            RTYPE (tree),
                                            resultType,
                                            tree->opval.op));

      /* if right is a literal and */
      /* left is also a division by a literal then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree))
          /* avoid infinite loop */
          && (TYPE_UDWORD) floatFromVal (tree->right->opval.val) != 1)
        {
          ast *parent;
          ast *litTree = searchLitOp (tree, &parent, "/");
          if (litTree)
            {
              if (IS_LITERAL (RTYPE (litTree)))
                {
                  /* foo_div */
                  DEBUG_CF("div r")
                  litTree->right = newNode ('*',
                                            litTree->right,
                                            copyAst (tree->right));
                  litTree->right->lineno = tree->lineno;

                  tree->right->opval.val = constVal ("1");
                  decorateType (parent, resultType);
                }
              else
                {
                  /* litTree->left is literal: no gcse possible.
                     We can't call decorateType(parent, RESULT_CHECK), because
                     this would cause an infinit loop. */
                  parent->decorated = 1;
                  decorateType (litTree, resultType);
                }
            }
        }

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*            modulus         */
      /*----------------------------*/
    case '%':
      if (!IS_INTEGRAL (LTYPE (tree)) || !IS_INTEGRAL (RTYPE (tree)))
        {
          werror (E_BITWISE_OP);
          werror (W_CONTINUE, "left & right types are ");
          printTypeChain (LTYPE (tree), stderr);
          fprintf (stderr, ",");
          printTypeChain (RTYPE (tree), stderr);
          fprintf (stderr, "\n");
          goto errorTreeReturn;
        }
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valMod (valFromType (LETYPE (tree)),
                                    valFromType (RETYPE (tree)));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }
      LRVAL (tree) = RRVAL (tree) = 1;
      TETYPE (tree) = getSpec (TTYPE (tree) =
                               computeType (LTYPE (tree),
                                            RTYPE (tree),
                                            resultType,
                                            tree->opval.op));
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*  address dereference       */
      /*----------------------------*/
    case '*':                   /* can be unary  : if right is null then unary operation */
      if (!tree->right)
        {
          if (!IS_PTR (LTYPE (tree)) && !IS_ARRAY (LTYPE (tree)))
            {
              werror (E_PTR_REQD);
              goto errorTreeReturn;
            }

          if (LRVAL (tree))
            {
              werror (E_LVALUE_REQUIRED, "pointer deref");
              goto errorTreeReturn;
            }
          if (IS_ADDRESS_OF_OP(tree->left))
            {
              /* replace *&obj with obj */
              return tree->left->left;
            }
          TTYPE (tree) = copyLinkChain (LTYPE (tree)->next);
          TETYPE (tree) = getSpec (TTYPE (tree));
          /* adjust the storage class */
          switch (DCL_TYPE(tree->left->ftype)) {
            case POINTER:
              SPEC_SCLS(TETYPE(tree)) = S_DATA;
              break;
            case FPOINTER:
              SPEC_SCLS(TETYPE(tree)) = S_XDATA; 
              break;
            case CPOINTER:
              SPEC_SCLS(TETYPE(tree)) = S_CODE; 
              break;
            case GPOINTER:
              SPEC_SCLS (TETYPE (tree)) = 0;
              break;
            case PPOINTER:
              SPEC_SCLS(TETYPE(tree)) = S_XSTACK; 
              break;
            case IPOINTER:
              SPEC_SCLS(TETYPE(tree)) = S_IDATA;
              break;
            case EEPPOINTER:
              SPEC_SCLS(TETYPE(tree)) = S_EEPROM;
              break;
            case UPOINTER:
              SPEC_SCLS (TETYPE (tree)) = 0;
              break;
            case ARRAY:
            case FUNCTION:
              break;
          }
          return tree;
        }

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      multiplication        */
      /*----------------------------*/
      if (!IS_ARITHMETIC (LTYPE (tree)) || !IS_ARITHMETIC (RTYPE (tree)))
        {
          werror (E_INVALID_OP, "multiplication");
          goto errorTreeReturn;
        }

      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valMult (valFromType (LETYPE (tree)),
                                     valFromType (RETYPE (tree)));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }

      /* if left is a literal exchange left & right */
      if (IS_LITERAL (LTYPE (tree)))
        {
          ast *tTree = tree->left;
          tree->left = tree->right;
          tree->right = tTree;
        }

      /* if right is a literal and */
      /* we can find a 2nd literal in a mul-tree then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree)))
        {
          ast *parent;
          ast *litTree = searchLitOp (tree, &parent, "*");
          if (litTree)
            {
              DEBUG_CF("mul")
              ast *tTree = litTree->left;
              litTree->left = tree->right;
              tree->right = tTree;
              /* both operands in litTree are literal now */
              decorateType (parent, resultType);
            }
        }

      LRVAL (tree) = RRVAL (tree) = 1;
      tree->left  = addCast (tree->left,  resultType, FALSE);
      tree->right = addCast (tree->right, resultType, FALSE);
      TETYPE (tree) = getSpec (TTYPE (tree) =
                                   computeType (LTYPE (tree),
                                                RTYPE (tree),
                                                resultType,
                                                tree->opval.op));

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    unary '+' operator      */
      /*----------------------------*/
    case '+':
      /* if unary plus */
      if (!tree->right)
        {
          if (!IS_ARITHMETIC (LTYPE (tree)))
            {
              werror (E_UNARY_OP, '+');
              goto errorTreeReturn;
            }

          /* if left is a literal then do it */
          if (IS_LITERAL (LTYPE (tree)))
            {
              tree->type = EX_VALUE;
              tree->opval.val = valFromType (LETYPE (tree));
              tree->left = NULL;
              TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
              return tree;
            }
          LRVAL (tree) = 1;
          COPYTYPE (TTYPE (tree), TETYPE (tree), LTYPE (tree));
          return tree;
        }

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      addition              */
      /*----------------------------*/

      /* this is not a unary operation */
      /* if both pointers then problem */
      if ((IS_PTR (LTYPE (tree)) || IS_ARRAY (LTYPE (tree))) &&
          (IS_PTR (RTYPE (tree)) || IS_ARRAY (RTYPE (tree))))
        {
          werror (E_PTR_PLUS_PTR);
          goto errorTreeReturn;
        }

      if (!IS_ARITHMETIC (LTYPE (tree)) &&
          !IS_PTR (LTYPE (tree)) && !IS_ARRAY (LTYPE (tree)))
        {
          werror (E_PLUS_INVALID, "+");
          goto errorTreeReturn;
        }

      if (!IS_ARITHMETIC (RTYPE (tree)) &&
          !IS_PTR (RTYPE (tree)) && !IS_ARRAY (RTYPE (tree)))
        {
          werror (E_PLUS_INVALID, "+");
          goto errorTreeReturn;
        }
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->left  = addCast (tree->left,  resultType, TRUE);
          tree->right = addCast (tree->right, resultType, TRUE);
          tree->opval.val = valPlus (valFromType (LETYPE (tree)),
                                     valFromType (RETYPE (tree)));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }

      /* if the right is a pointer or left is a literal
         xchange left & right */
      if (IS_ARRAY (RTYPE (tree)) ||
          IS_PTR (RTYPE (tree)) ||
          IS_LITERAL (LTYPE (tree)))
        {
          ast *tTree = tree->left;
          tree->left = tree->right;
          tree->right = tTree;
        }

      /* if right is a literal and */
      /* left is also an addition/subtraction with a literal then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree)))
        {
          ast *litTree, *parent;
          litTree = searchLitOp (tree, &parent, "+-");
          if (litTree)
            {
              if (litTree->opval.op == '+')
                {
                  /* foo_aa */
                  DEBUG_CF("+ 1 AA")
                  ast *tTree = litTree->left;
                  litTree->left = tree->right;
                  tree->right = tree->left;
                  tree->left = tTree;
                }
              else if (litTree->opval.op == '-')
                {
                  if (IS_LITERAL (RTYPE (litTree)))
                    {
                      DEBUG_CF("+ 2 ASR")
                      /* foo_asr */
                      ast *tTree = litTree->left;
                      litTree->left = tree->right;
                      tree->right = tTree;
                    }
                  else
                    {
                      DEBUG_CF("+ 3 ASL")
                      /* foo_asl */
                      ast *tTree = litTree->right;
                      litTree->right = tree->right;
                      tree->right = tTree;
                      litTree->opval.op = '+';
                      tree->opval.op = '-';
                    }
                }
              decorateType (parent, resultType);
            }
        }

      LRVAL (tree) = RRVAL (tree) = 1;
      /* if the left is a pointer */
      if (IS_PTR (LTYPE (tree)) || IS_AGGREGATE (LTYPE (tree)) )
        TETYPE (tree) = getSpec (TTYPE (tree) =
                                 LTYPE (tree));
      else
        {
          tree->left  = addCast (tree->left,  resultType, TRUE);
          tree->right = addCast (tree->right, resultType, TRUE);
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                     computeType (LTYPE (tree),
                                                  RTYPE (tree),
                                                  resultType,
                                                  tree->opval.op));
        }
        
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      unary '-'             */
      /*----------------------------*/
    case '-':                   /* can be unary   */
      /* if right is null then unary */
      if (!tree->right)
        {

          if (!IS_ARITHMETIC (LTYPE (tree)))
            {
              werror (E_UNARY_OP, tree->opval.op);
              goto errorTreeReturn;
            }

          /* if left is a literal then do it */
          if (IS_LITERAL (LTYPE (tree)))
            {
              tree->type = EX_VALUE;
              tree->opval.val = valUnaryPM (valFromType (LETYPE (tree)));
              tree->left = NULL;
              TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
              SPEC_USIGN(TETYPE(tree)) = 0;
              return tree;
            }
          LRVAL (tree) = 1;
          TETYPE(tree) = getSpec (TTYPE (tree) = LTYPE (tree));
          return tree;
        }

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    subtraction             */
      /*----------------------------*/

      if (!(IS_PTR (LTYPE (tree)) ||
            IS_ARRAY (LTYPE (tree)) ||
            IS_ARITHMETIC (LTYPE (tree))))
        {
          werror (E_PLUS_INVALID, "-");
          goto errorTreeReturn;
        }

      if (!(IS_PTR (RTYPE (tree)) ||
            IS_ARRAY (RTYPE (tree)) ||
            IS_ARITHMETIC (RTYPE (tree))))
        {
          werror (E_PLUS_INVALID, "-");
          goto errorTreeReturn;
        }

      if ((IS_PTR (LTYPE (tree)) || IS_ARRAY (LTYPE (tree))) &&
          !(IS_PTR (RTYPE (tree)) || IS_ARRAY (RTYPE (tree)) ||
            IS_INTEGRAL (RTYPE (tree))))
        {
          werror (E_PLUS_INVALID, "-");
          goto errorTreeReturn;
        }

      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->left  = addCast (tree->left,  resultType, TRUE);
          tree->right = addCast (tree->right, resultType, TRUE);
          tree->opval.val = valMinus (valFromType (LETYPE (tree)),
                                      valFromType (RETYPE (tree)));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }

      /* if the left & right are equal then zero */
      if (isAstEqual (tree->left, tree->right))
        {
          tree->type = EX_VALUE;
          tree->left = tree->right = NULL;
          tree->opval.val = constVal ("0");
          TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
          return tree;
        }

      /* if both of them are pointers or arrays then */
      /* the result is going to be an integer        */
      if ((IS_ARRAY (LTYPE (tree)) || IS_PTR (LTYPE (tree))) &&
          (IS_ARRAY (RTYPE (tree)) || IS_PTR (RTYPE (tree))))
        TETYPE (tree) = TTYPE (tree) = newIntLink ();
      else
        /* if only the left is a pointer */
        /* then result is a pointer      */
      if (IS_PTR (LTYPE (tree)) || IS_ARRAY (LTYPE (tree)))
        TETYPE (tree) = getSpec (TTYPE (tree) =
                                 LTYPE (tree));
      else
        {
          tree->left  = addCast (tree->left,  resultType, TRUE);
          tree->right = addCast (tree->right, resultType, TRUE);

          TETYPE (tree) = getSpec (TTYPE (tree) =
                                     computeType (LTYPE (tree),
                                                  RTYPE (tree),
                                                  resultType,
                                                  tree->opval.op));
        }

      LRVAL (tree) = RRVAL (tree) = 1;

      /* if right is a literal and */
      /* left is also an addition/subtraction with a literal then */
      /* rearrange the tree */
      if (IS_LITERAL (RTYPE (tree))
          /* avoid infinite loop */
          && (TYPE_UDWORD) floatFromVal (tree->right->opval.val) != 0)
        {
          ast *litTree, *litParent;
          litTree = searchLitOp (tree, &litParent, "+-");
          if (litTree)
            {
              if (litTree->opval.op == '+')
                {
                  /* foo_sa */
                  DEBUG_CF("- 1 SA")
                  ast *tTree = litTree->left;
                  litTree->left = litTree->right;
                  litTree->right = tree->right;
                  tree->right = tTree;
                  tree->opval.op = '+';
                  litTree->opval.op = '-';
                }
              else if (litTree->opval.op == '-')
                {
                  if (IS_LITERAL (RTYPE (litTree)))
                    {
                      /* foo_ssr */
                      DEBUG_CF("- 2 SSR")
                      ast *tTree = litTree->left;
                      litTree->left = tree->right;
                      tree->right = litParent->left;
                      litParent->left = tTree;
                      litTree->opval.op = '+';
                      
                      tree->decorated = 0;
                      decorateType (tree, resultType);
                    }
                  else
                    {
                      /* foo_ssl */
                      DEBUG_CF("- 3 SSL")
                      ast *tTree = litTree->right;
                      litTree->right = tree->right;
                      tree->right = tTree;
                    }
                }
              decorateType (litParent, resultType);
            }
        }
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    complement              */
      /*----------------------------*/
    case '~':
      /* can be only integral type */
      if (!IS_INTEGRAL (LTYPE (tree)))
        {
          werror (E_UNARY_OP, tree->opval.op);
          goto errorTreeReturn;
        }

      /* if left is a literal then do it */
      if (IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valComplement (valFromType (LETYPE (tree)));
          tree->left = NULL;
          TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
          return addCast (tree, resultType, TRUE);
        }
      tree->left = addCast (tree->left, resultType, TRUE);
      LRVAL (tree) = 1;
      COPYTYPE (TTYPE (tree), TETYPE (tree), LTYPE (tree));
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*           not              */
      /*----------------------------*/
    case '!':
      /* can be pointer */
      if (!IS_ARITHMETIC (LTYPE (tree)) &&
          !IS_PTR (LTYPE (tree)) &&
          !IS_ARRAY (LTYPE (tree)))
        {
          werror (E_UNARY_OP, tree->opval.op);
          goto errorTreeReturn;
        }

      /* if left is a literal then do it */
      if (IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valNot (valFromType (LETYPE (tree)));
          tree->left = NULL;
          TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
          return tree;
        }
      LRVAL (tree) = 1;
      TTYPE (tree) = TETYPE (tree) = newCharLink ();
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*           shift            */
      /*----------------------------*/
    case RRC:
    case RLC:
    case SWAP:
      TTYPE (tree) = LTYPE (tree);
      TETYPE (tree) = LETYPE (tree);
      return tree;

    case GETHBIT:
      TTYPE (tree) = TETYPE (tree) = newCharLink ();
      return tree;

    case LEFT_OP:
    case RIGHT_OP:
      if (!IS_INTEGRAL (LTYPE (tree)) || !IS_INTEGRAL (tree->left->etype))
        {
          werror (E_SHIFT_OP_INVALID);
          werror (W_CONTINUE, "left & right types are ");
          printTypeChain (LTYPE (tree), stderr);
          fprintf (stderr, ",");
          printTypeChain (RTYPE (tree), stderr);
          fprintf (stderr, "\n");
          goto errorTreeReturn;
        }

      /* make smaller type only if it's a LEFT_OP */
      if (tree->opval.op == LEFT_OP)
        tree->left = addCast (tree->left, resultType, TRUE);
      
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valShift (valFromType (LETYPE (tree)),
                                      valFromType (RETYPE (tree)),
                                      (tree->opval.op == LEFT_OP ? 1 : 0));
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }

      LRVAL (tree) = RRVAL (tree) = 1;
      if (tree->opval.op == LEFT_OP)
        {
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                       computeType (LTYPE (tree),
                                                    NULL,
                                                    resultType,
                                                    tree->opval.op));
        }
      else /* RIGHT_OP */
        {
          /* no promotion necessary */
          TTYPE (tree) = TETYPE (tree) = copyLinkChain (LTYPE (tree));
          if (IS_LITERAL (TTYPE (tree)))
            SPEC_SCLS (TTYPE (tree)) &= ~S_LITERAL;
        }

      /* if only the right side is a literal & we are
         shifting more than size of the left operand then zero */
      if (IS_LITERAL (RTYPE (tree)) &&
          ((TYPE_UDWORD) floatFromVal (valFromType (RETYPE (tree)))) >=
          (getSize (TETYPE (tree)) * 8))
        {
          if (tree->opval.op==LEFT_OP ||
              (tree->opval.op==RIGHT_OP && SPEC_USIGN(LETYPE(tree))))
            {
              lineno=tree->lineno;
              werror (W_SHIFT_CHANGED,
                      (tree->opval.op == LEFT_OP ? "left" : "right"));
              tree->type = EX_VALUE;
              tree->left = tree->right = NULL;
              tree->opval.val = constVal ("0");
              TETYPE (tree) = TTYPE (tree) = tree->opval.val->type;
              return tree;
            }
        }

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*         casting            */
      /*----------------------------*/
    case CAST:                  /* change the type   */
      /* cannot cast to an aggregate type */
      if (IS_AGGREGATE (LTYPE (tree)))
        {
          werror (E_CAST_ILLEGAL);
          goto errorTreeReturn;
        }

      /* make sure the type is complete and sane */
      checkTypeSanity(LETYPE(tree), "(cast)");

      /* If code memory is read only, then pointers to code memory */
      /* implicitly point to constants -- make this explicit       */
      {
        sym_link *t = LTYPE(tree);
        while (t && t->next)
          {
            if (IS_CODEPTR(t) && port->mem.code_ro)
              {
                if (IS_SPEC(t->next))
                  SPEC_CONST (t->next) = 1;
                else
                  DCL_PTR_CONST (t->next) = 1;
              }
            t = t->next;
          }
      }

#if 0
      /* if the right is a literal replace the tree */
      if (IS_LITERAL (RETYPE (tree))) {
              if (!IS_PTR (LTYPE (tree))) {
                      tree->type = EX_VALUE;
                      tree->opval.val =
                              valCastLiteral (LTYPE (tree),
                                              floatFromVal (valFromType (RETYPE (tree))));
                      tree->left = NULL;
                      tree->right = NULL;
                      TTYPE (tree) = tree->opval.val->type;
                      tree->values.literalFromCast = 1;
              } else if (IS_GENPTR(LTYPE(tree)) && !IS_PTR(RTYPE(tree)) &&
                         ((int)floatFromVal(valFromType(RETYPE(tree)))) !=0 ) /* special case of NULL */  {
                      sym_link *rest = LTYPE(tree)->next;
                      werror(W_LITERAL_GENERIC);
                      TTYPE(tree) = newLink(DECLARATOR);
                      DCL_TYPE(TTYPE(tree)) = FPOINTER;
                      TTYPE(tree)->next = rest;
                      tree->left->opval.lnk = TTYPE(tree);
                      LRVAL (tree) = 1;
              } else {
                      TTYPE (tree) = LTYPE (tree);
                      LRVAL (tree) = 1;
              }
      } else {
              TTYPE (tree) = LTYPE (tree);
              LRVAL (tree) = 1;
      }
#else
#if 0 // this is already checked, now this could be explicit
      /* if pointer to struct then check names */
      if (IS_PTR(LTYPE(tree)) && IS_STRUCT(LTYPE(tree)->next) &&
          IS_PTR(RTYPE(tree)) && IS_STRUCT(RTYPE(tree)->next) &&
          strcmp(SPEC_STRUCT(LETYPE(tree))->tag,SPEC_STRUCT(RETYPE(tree))->tag))
        {
          werror(W_CAST_STRUCT_PTR,SPEC_STRUCT(RETYPE(tree))->tag,
                 SPEC_STRUCT(LETYPE(tree))->tag);
        }
#endif
      if (IS_ADDRESS_OF_OP(tree->right)
          && IS_AST_SYM_VALUE (tree->right->left)
          && SPEC_ABSA (AST_SYMBOL (tree->right->left)->etype)) {

        tree->type = EX_VALUE;
        tree->opval.val =
          valCastLiteral (LTYPE (tree),
                          SPEC_ADDR (AST_SYMBOL (tree->right->left)->etype));
        TTYPE (tree) = tree->opval.val->type;
        TETYPE (tree) = getSpec (TTYPE (tree));
        tree->left = NULL;
        tree->right = NULL;
        tree->values.literalFromCast = 1;
        return tree;
      }

      /* handle offsetof macro:            */
      /* #define offsetof(TYPE, MEMBER) \  */
      /* ((unsigned) &((TYPE *)0)->MEMBER) */
      if (IS_ADDRESS_OF_OP(tree->right)
          && IS_AST_OP (tree->right->left)
          && tree->right->left->opval.op == PTR_OP
          && IS_AST_OP (tree->right->left->left)
          && tree->right->left->left->opval.op == CAST
          && IS_AST_LIT_VALUE(tree->right->left->left->right)) {

        symbol *element = getStructElement (
          SPEC_STRUCT (LETYPE(tree->right->left)),
          AST_SYMBOL(tree->right->left->right)
        );

        if (element) {
          tree->type = EX_VALUE;
          tree->opval.val = valCastLiteral (
            LTYPE (tree),
            element->offset
            + floatFromVal (valFromType (RTYPE (tree->right->left->left)))
          );

          TTYPE (tree) = tree->opval.val->type;
          TETYPE (tree) = getSpec (TTYPE (tree));
          tree->left = NULL;
          tree->right = NULL;
          return tree;
        }
      }

      /* if the right is a literal replace the tree */
      if (IS_LITERAL (RETYPE (tree))) {
        #if 0
        if (IS_PTR (LTYPE (tree)) && !IS_GENPTR (LTYPE (tree)) ) {
          /* rewrite      (type *)litaddr
             as           &temp
             and define   type at litaddr temp
             (but only if type's storage class is not generic)
          */
          ast *newTree = newNode ('&', NULL, NULL);
          symbol *sym;

          TTYPE (newTree) = LTYPE (tree);
          TETYPE (newTree) = getSpec(LTYPE (tree));

          /* define a global symbol at the casted address*/
          sym = newSymbol(genSymName (0), 0);
          sym->type = LTYPE (tree)->next;
          if (!sym->type)
            sym->type = newLink (V_VOID);
          sym->etype = getSpec(sym->type);
          SPEC_SCLS (sym->etype) = sclsFromPtr (LTYPE (tree));
          sym->lineDef = tree->lineno;
          sym->cdef = 1;
          sym->isref = 1;
          SPEC_STAT (sym->etype) = 1;
          SPEC_ADDR(sym->etype) = floatFromVal (valFromType (RTYPE (tree)));
          SPEC_ABSA(sym->etype) = 1;
          addSym (SymbolTab, sym, sym->name, 0, 0, 0);
          allocGlobal (sym);

          newTree->left = newAst_VALUE(symbolVal(sym));
          newTree->left->lineno = tree->lineno;
          LTYPE (newTree) = sym->type;
          LETYPE (newTree) = sym->etype;
          LLVAL (newTree) = 1;
          LRVAL (newTree) = 0;
          TLVAL (newTree) = 1;
          return newTree;
        }
        #endif
        if (!IS_PTR (LTYPE (tree))) {
          tree->type = EX_VALUE;
          tree->opval.val =
          valCastLiteral (LTYPE (tree),
                          floatFromVal (valFromType (RTYPE (tree))));
          TTYPE (tree) = tree->opval.val->type;
          tree->left = NULL;
          tree->right = NULL;
          tree->values.literalFromCast = 1;
          TETYPE (tree) = getSpec (TTYPE (tree));
          return tree;
        }
      }
      TTYPE (tree) = LTYPE (tree);
      LRVAL (tree) = 1;

#endif
      TETYPE (tree) = getSpec (TTYPE (tree));

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*       logical &&, ||       */
      /*----------------------------*/
    case AND_OP:
    case OR_OP:
      /* each must be arithmetic type or be a pointer */
      if (!IS_PTR (LTYPE (tree)) &&
          !IS_ARRAY (LTYPE (tree)) &&
          !IS_INTEGRAL (LTYPE (tree)))
        {
          werror (E_COMPARE_OP);
          goto errorTreeReturn;
        }

      if (!IS_PTR (RTYPE (tree)) &&
          !IS_ARRAY (RTYPE (tree)) &&
          !IS_INTEGRAL (RTYPE (tree)))
        {
          werror (E_COMPARE_OP);
          goto errorTreeReturn;
        }
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) &&
          IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valLogicAndOr (valFromType (LTYPE (tree)),
                                           valFromType (RTYPE (tree)),
                                           tree->opval.op);
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }
      LRVAL (tree) = RRVAL (tree) = 1;
      TTYPE (tree) = TETYPE (tree) = newCharLink ();
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*     comparison operators   */
      /*----------------------------*/
    case '>':
    case '<':
    case LE_OP:
    case GE_OP:
    case EQ_OP:
    case NE_OP:
      {
        ast *lt = optimizeCompare (tree);

        if (tree != lt)
          return lt;
      }

      /* if they are pointers they must be castable */
      if (IS_PTR (LTYPE (tree)) && IS_PTR (RTYPE (tree)))
        {
          if (tree->opval.op==EQ_OP &&
              !IS_GENPTR(LTYPE(tree)) && IS_GENPTR(RTYPE(tree))) {
            // we cannot cast a gptr to a !gptr: switch the leaves
            struct ast *s=tree->left;
            tree->left=tree->right;
            tree->right=s;
          }
          if (compareType (LTYPE (tree), RTYPE (tree)) == 0)
            {
              werror (E_COMPARE_OP);
              fprintf (stderr, "comparing type ");
              printTypeChain (LTYPE (tree), stderr);
              fprintf (stderr, "to type ");
              printTypeChain (RTYPE (tree), stderr);
              fprintf (stderr, "\n");
              goto errorTreeReturn;
            }
        }
      /* else they should be promotable to one another */
      else
        {
          if (!((IS_PTR (LTYPE (tree)) && IS_LITERAL (RTYPE (tree))) ||
                (IS_PTR (RTYPE (tree)) && IS_LITERAL (LTYPE (tree)))))

            if (compareType (LTYPE (tree), RTYPE (tree)) == 0)
              {
                werror (E_COMPARE_OP);
                fprintf (stderr, "comparing type ");
                printTypeChain (LTYPE (tree), stderr);
                fprintf (stderr, "to type ");
                printTypeChain (RTYPE (tree), stderr);
                fprintf (stderr, "\n");
                goto errorTreeReturn;
              }
        }
      /* if unsigned value < 0  then always false */
      /* if (unsigned value) > 0 then '(unsigned value) ? 1 : 0' */
      if (SPEC_USIGN(LETYPE(tree)) &&
          !IS_CHAR(LETYPE(tree)) && /* promotion to signed int */
          IS_LITERAL(RTYPE(tree))  &&
          ((int) floatFromVal (valFromType (RETYPE (tree)))) == 0)
        {
          if (tree->opval.op == '<')
            {
              return tree->right;
            }
          if (tree->opval.op == '>')
            {
              if (resultType == RESULT_TYPE_IFX)
                {
                  /* the parent is an ifx: */
                  /* if (unsigned value) */
                  return tree->left;
                }
              
              /* (unsigned value) ? 1 : 0 */
              tree->opval.op = '?';
              tree->right = newNode (':',
                                     newAst_VALUE (constVal ("1")),
                                     tree->right); /* val 0 */
              tree->right->lineno = tree->lineno;
              tree->right->left->lineno = tree->lineno;
              decorateType (tree->right, RESULT_CHECK);
            }
        }
      /* if they are both literal then */
      /* rewrite the tree */
      if (IS_LITERAL (RTYPE (tree)) &&
          IS_LITERAL (LTYPE (tree)))
        {
          tree->type = EX_VALUE;
          tree->opval.val = valCompare (valFromType (LETYPE (tree)),
                                        valFromType (RETYPE (tree)),
                                        tree->opval.op);
          tree->right = tree->left = NULL;
          TETYPE (tree) = getSpec (TTYPE (tree) =
                                   tree->opval.val->type);
          return tree;
        }
      LRVAL (tree) = RRVAL (tree) = 1;
      TTYPE (tree) = TETYPE (tree) = newCharLink ();
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*             sizeof         */
      /*----------------------------*/
    case SIZEOF:                /* evaluate wihout code generation */
      /* change the type to a integer */
      {
        int size = getSize (tree->right->ftype);
        SNPRINTF(buffer, sizeof(buffer), "%d", size);
        if (!size && !IS_VOID(tree->right->ftype))
          werrorfl (tree->filename, tree->lineno, E_SIZEOF_INCOMPLETE_TYPE);
      }
      tree->type = EX_VALUE;
      tree->opval.val = constVal (buffer);
      tree->right = tree->left = NULL;
      TETYPE (tree) = getSpec (TTYPE (tree) =
                               tree->opval.val->type);
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*             typeof         */
      /*----------------------------*/
    case TYPEOF:
        /* return typeof enum value */
        tree->type = EX_VALUE;
        {
            int typeofv = 0;
            if (IS_SPEC(tree->right->ftype)) {
                switch (SPEC_NOUN(tree->right->ftype)) {
                case V_INT:
                    if (SPEC_LONG(tree->right->ftype)) typeofv = TYPEOF_LONG;
                    else typeofv = TYPEOF_INT;
                    break;
                case V_FLOAT:
                    typeofv = TYPEOF_FLOAT;
                    break;
                case V_CHAR:
                    typeofv = TYPEOF_CHAR;
                    break;
                case V_VOID:
                    typeofv = TYPEOF_VOID;
                    break;
                case V_STRUCT:
                    typeofv = TYPEOF_STRUCT;
                    break;
                case V_BITFIELD:
                    typeofv = TYPEOF_BITFIELD;
                    break;
                case V_BIT:
                    typeofv = TYPEOF_BIT;
                    break;
                case V_SBIT:
                    typeofv = TYPEOF_SBIT;
                    break;
                default:
                    break;
                }
            } else {
                switch (DCL_TYPE(tree->right->ftype)) {
                case POINTER:
                    typeofv = TYPEOF_POINTER;
                    break;
                case FPOINTER:
                    typeofv = TYPEOF_FPOINTER;
                    break;
                case CPOINTER:
                    typeofv = TYPEOF_CPOINTER;
                    break;
                case GPOINTER:
                    typeofv = TYPEOF_GPOINTER;
                    break;
                case PPOINTER:
                    typeofv = TYPEOF_PPOINTER;
                    break;
                case IPOINTER:
                    typeofv = TYPEOF_IPOINTER;
                    break;
                case ARRAY:
                    typeofv = TYPEOF_ARRAY;
                    break;
                case FUNCTION:
                    typeofv = TYPEOF_FUNCTION;
                    break;
                default:
                    break;
                }
            }
            SNPRINTF (buffer, sizeof(buffer), "%d", typeofv);
            tree->opval.val = constVal (buffer);
            tree->right = tree->left = NULL;
            TETYPE (tree) = getSpec (TTYPE (tree) =
                                     tree->opval.val->type);
        }
        return tree;
      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /* conditional operator  '?'  */
      /*----------------------------*/
    case '?':
      /* the type is value of the colon operator (on the right) */
      assert (IS_COLON_OP (tree->right));
      /* if already known then replace the tree : optimizer will do it
         but faster to do it here */
      if (IS_LITERAL (LTYPE (tree)))
        {
          if (((int) floatFromVal (valFromType (LETYPE (tree)))) != 0)
            return decorateType (tree->right->left, resultTypeProp);
          else
            return decorateType (tree->right->right, resultTypeProp);
        }
      else
        {
          tree->right = decorateType (tree->right, resultTypeProp);
          TTYPE (tree) = RTYPE (tree);
          TETYPE (tree) = getSpec (TTYPE (tree));
        }
      return tree;

    case ':':
      /* if they don't match we have a problem */
      if (compareType (LTYPE (tree), RTYPE (tree)) == 0)
        {
          werror (E_TYPE_MISMATCH, "conditional operator", " ");
          goto errorTreeReturn;
        }

      TTYPE (tree) = computeType (LTYPE (tree), RTYPE (tree),
                                  resultType, tree->opval.op);
      TETYPE (tree) = getSpec (TTYPE (tree));
      return tree;


#if 0 // assignment operators are converted by the parser
      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    assignment operators    */
      /*----------------------------*/
    case MUL_ASSIGN:
    case DIV_ASSIGN:
      /* for these it must be both must be integral */
      if (!IS_ARITHMETIC (LTYPE (tree)) ||
          !IS_ARITHMETIC (RTYPE (tree)))
        {
          werror (E_OPS_INTEGRAL);
          goto errorTreeReturn;
        }
      RRVAL (tree) = 1;
      TETYPE (tree) = getSpec (TTYPE (tree) = LTYPE (tree));

      if (!tree->initMode && IS_CONSTANT (LTYPE (tree)))
        werror (E_CODE_WRITE, tree->opval.op==MUL_ASSIGN ? "*=" : "/=");

      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, tree->opval.op==MUL_ASSIGN ? "*=" : "/=");
          goto errorTreeReturn;
        }
      LLVAL (tree) = 1;

      return tree;

    case AND_ASSIGN:
    case OR_ASSIGN:
    case XOR_ASSIGN:
    case RIGHT_ASSIGN:
    case LEFT_ASSIGN:
      /* for these it must be both must be integral */
      if (!IS_INTEGRAL (LTYPE (tree)) ||
          !IS_INTEGRAL (RTYPE (tree)))
        {
          werror (E_OPS_INTEGRAL);
          goto errorTreeReturn;
        }
      RRVAL (tree) = 1;
      TETYPE (tree) = getSpec (TTYPE (tree) = LTYPE (tree));

      if (!tree->initMode && IS_CONSTANT (LETYPE (tree)))
        werror (E_CODE_WRITE, "&= or |= or ^= or >>= or <<=");

      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "&= or |= or ^= or >>= or <<=");
          goto errorTreeReturn;
        }
      LLVAL (tree) = 1;

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*    -= operator             */
      /*----------------------------*/
    case SUB_ASSIGN:
      if (!(IS_PTR (LTYPE (tree)) ||
            IS_ARITHMETIC (LTYPE (tree))))
        {
          werror (E_PLUS_INVALID, "-=");
          goto errorTreeReturn;
        }

      if (!(IS_PTR (RTYPE (tree)) ||
            IS_ARITHMETIC (RTYPE (tree))))
        {
          werror (E_PLUS_INVALID, "-=");
          goto errorTreeReturn;
        }
      RRVAL (tree) = 1;
      TETYPE (tree) = getSpec (TTYPE (tree) =
                               computeType (LTYPE (tree),
                                            RTYPE (tree),
                                            RESULT_TYPE_NOPROM,
                                            tree->opval.op));

      if (!tree->initMode && IS_CONSTANT (LETYPE (tree)))
        werror (E_CODE_WRITE, "-=");

      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "-=");
          goto errorTreeReturn;
        }
      LLVAL (tree) = 1;

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*          += operator       */
      /*----------------------------*/
    case ADD_ASSIGN:
      /* this is not a unary operation */
      /* if both pointers then problem */
      if (IS_PTR (LTYPE (tree)) && IS_PTR (RTYPE (tree)))
        {
          werror (E_PTR_PLUS_PTR);
          goto errorTreeReturn;
        }

      if (!IS_ARITHMETIC (LTYPE (tree)) && !IS_PTR (LTYPE (tree)))
        {
          werror (E_PLUS_INVALID, "+=");
          goto errorTreeReturn;
        }

      if (!IS_ARITHMETIC (RTYPE (tree)) && !IS_PTR (RTYPE (tree)))
        {
          werror (E_PLUS_INVALID, "+=");
          goto errorTreeReturn;
        }
      RRVAL (tree) = 1;
      TETYPE (tree) = getSpec (TTYPE (tree) =
                               computeType (LTYPE (tree),
                                            RTYPE (tree),
                                            RESULT_TYPE_NOPROM,
                                            tree->opval.op));

      if (!tree->initMode && IS_CONSTANT (LETYPE (tree)))
        werror (E_CODE_WRITE, "+=");

      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "+=");
          goto errorTreeReturn;
        }

      tree->right = decorateType (newNode ('+', copyAst (tree->left), tree->right), RESULT_CHECK);
      tree->opval.op = '=';

      return tree;
#endif

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      straight assignemnt   */
      /*----------------------------*/
    case '=':
      /* cannot be an aggregate */
      if (IS_AGGREGATE (LTYPE (tree)))
        {
          werror (E_AGGR_ASSIGN);
          goto errorTreeReturn;
        }

      /* they should either match or be castable */
      if (compareType (LTYPE (tree), RTYPE (tree)) == 0)
        {
          werror (E_TYPE_MISMATCH, "assignment", " ");
          printFromToType(RTYPE(tree),LTYPE(tree));
        }

      /* if the left side of the tree is of type void
         then report error */
      if (IS_VOID (LTYPE (tree)))
        {
          werror (E_CAST_ZERO);
          printFromToType(RTYPE(tree), LTYPE(tree));
        }

      TETYPE (tree) = getSpec (TTYPE (tree) =
                               LTYPE (tree));
      RRVAL (tree) = 1;
      LLVAL (tree) = 1;
      if (!tree->initMode ) {
        if (IS_CONSTANT(LTYPE(tree)))
          werror (E_CODE_WRITE, "=");
      }
      if (LRVAL (tree))
        {
          werror (E_LVALUE_REQUIRED, "=");
          goto errorTreeReturn;
        }

      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*      comma operator        */
      /*----------------------------*/
    case ',':
      TETYPE (tree) = getSpec (TTYPE (tree) = RTYPE (tree));
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*       function call        */
      /*----------------------------*/
    case CALL:
      
      /* undo any explicit pointer derefernce; PCALL will handle it instead */
      if (IS_FUNC (LTYPE (tree)) && tree->left->type == EX_OP)
        {
          if (tree->left->opval.op == '*' && !tree->left->right)
            tree->left = tree->left->left;
        }

      /* require a function or pointer to function */
      if (!IS_FUNC (LTYPE (tree))
          && !(IS_CODEPTR (LTYPE (tree)) && IS_FUNC (LTYPE (tree)->next)))
        {
          werrorfl (tree->filename, tree->lineno, E_FUNCTION_EXPECTED);
          goto errorTreeReturn;
        }

      /* if there are parms, make sure that
         parms are decorate / process / reverse only once */
      if (!tree->right ||
          !tree->right->decorated)
        {
          sym_link *functype;      
          parmNumber = 1;

          if (IS_CODEPTR(LTYPE(tree)))
            functype = LTYPE (tree)->next;
          else
            functype = LTYPE (tree);

          if (processParms (tree->left, FUNC_ARGS(functype),
                            &tree->right, &parmNumber, TRUE))
            {
              goto errorTreeReturn;
            }
        
          if ((options.stackAuto || IFFUNC_ISREENT (functype)) && 
             !IFFUNC_ISBUILTIN(functype))
            {
              reverseParms (tree->right);
            }

           TTYPE (tree) = functype->next;
           TETYPE (tree) = getSpec (TTYPE (tree));
        }
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*     return statement       */
      /*----------------------------*/
    case RETURN:
      if (!tree->right)
        goto voidcheck;

      if (compareType (currFunc->type->next, RTYPE (tree)) == 0)
        {
          werrorfl (tree->filename, tree->lineno, W_RETURN_MISMATCH);
          printFromToType (RTYPE(tree), currFunc->type->next);
          goto errorTreeReturn;
        }

      if (IS_VOID (currFunc->type->next)
          && tree->right &&
          !IS_VOID (RTYPE (tree)))
        {
          werrorfl (tree->filename, tree->lineno, E_FUNC_VOID);
          goto errorTreeReturn;
        }

      /* if there is going to be a casting required then add it */
      if (compareType (currFunc->type->next, RTYPE (tree)) < 0)
        {
          tree->right =
            decorateType (newNode (CAST,
                          newAst_LINK (copyLinkChain (currFunc->type->next)),
                                        tree->right),
                          RESULT_CHECK);
        }

      RRVAL (tree) = 1;
      return tree;

    voidcheck:

      if (!IS_VOID (currFunc->type->next) && tree->right == NULL)
        {
          werror (W_VOID_FUNC, currFunc->name);
          goto errorTreeReturn;
        }

      TTYPE (tree) = TETYPE (tree) = NULL;
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /*     switch statement       */
      /*----------------------------*/
    case SWITCH:
      /* the switch value must be an integer */
      if (!IS_INTEGRAL (LTYPE (tree)))
        {
          werrorfl (tree->filename, tree->lineno, E_SWITCH_NON_INTEGER);
          goto errorTreeReturn;
        }
      LRVAL (tree) = 1;
      TTYPE (tree) = TETYPE (tree) = NULL;
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /* ifx Statement              */
      /*----------------------------*/
    case IFX:
      tree->left = backPatchLabels (tree->left,
                                    tree->trueLabel,
                                    tree->falseLabel);
      TTYPE (tree) = TETYPE (tree) = NULL;
      return tree;

      /*------------------------------------------------------------------*/
      /*----------------------------*/
      /* for Statement              */
      /*----------------------------*/
    case FOR:

      decorateType (resolveSymbols (AST_FOR (tree, initExpr)), RESULT_CHECK);
      decorateType (resolveSymbols (AST_FOR (tree, condExpr)), RESULT_CHECK);
      decorateType (resolveSymbols (AST_FOR (tree, loopExpr)), RESULT_CHECK);

      /* if the for loop is reversible then
         reverse it otherwise do what we normally
         do */
      {
        symbol *sym;
        ast *init, *end;

        if (isLoopReversible (tree, &sym, &init, &end))
          return reverseLoop (tree, sym, init, end);
        else
          return decorateType (createFor (AST_FOR (tree, trueLabel),
                                          AST_FOR (tree, continueLabel),
                                          AST_FOR (tree, falseLabel),
                                          AST_FOR (tree, condLabel),
                                          AST_FOR (tree, initExpr),
                                          AST_FOR (tree, condExpr),
                                          AST_FOR (tree, loopExpr),
                                          tree->left), RESULT_CHECK);
      }
    case PARAM:
      werror (E_INTERNAL_ERROR, __FILE__, __LINE__,
              "node PARAM shouldn't be processed here");
              /* but in processParams() */
      return tree;
    default:
      TTYPE (tree) = TETYPE (tree) = NULL;
      return tree;
    }

  /* some error found this tree will be killed */
errorTreeReturn:
  TTYPE (tree) = TETYPE (tree) = newCharLink ();
  tree->opval.op = NULLOP;
  tree->isError = 1;

  return tree;
}

/*-----------------------------------------------------------------*/
/* sizeofOp - processes size of operation                          */
/*-----------------------------------------------------------------*/
value *
sizeofOp (sym_link * type)
{
  char buff[10];
  int size;

  /* make sure the type is complete and sane */
  checkTypeSanity(type, "(sizeof)");

  /* get the size and convert it to character  */
  SNPRINTF (buff, sizeof(buff), "%d", size = getSize (type));
  if (!size && !IS_VOID(type))
    werror (E_SIZEOF_INCOMPLETE_TYPE);

  /* now convert into value  */
  return constVal (buff);
}


#define IS_AND(ex) (ex->type == EX_OP && ex->opval.op == AND_OP )
#define IS_OR(ex)  (ex->type == EX_OP && ex->opval.op == OR_OP )
#define IS_NOT(ex) (ex->type == EX_OP && ex->opval.op == '!' )
#define IS_ANDORNOT(ex) (IS_AND(ex) || IS_OR(ex) || IS_NOT(ex))
#define IS_IFX(ex) (ex->type == EX_OP && ex->opval.op == IFX )
#define IS_LT(ex)  (ex->type == EX_OP && ex->opval.op == '<' )
#define IS_GT(ex)  (ex->type == EX_OP && ex->opval.op == '>')

/*-----------------------------------------------------------------*/
/* backPatchLabels - change and or not operators to flow control    */
/*-----------------------------------------------------------------*/
ast *
backPatchLabels (ast * tree, symbol * trueLabel, symbol * falseLabel)
{

  if (!tree)
    return NULL;

  if (!(IS_ANDORNOT (tree)))
    return tree;

  /* if this an and */
  if (IS_AND (tree))
    {
      static int localLbl = 0;
      symbol *localLabel;

      SNPRINTF(buffer, sizeof(buffer), "_andif_%d", localLbl++);
      localLabel = newSymbol (buffer, NestLevel);

      tree->left = backPatchLabels (tree->left, localLabel, falseLabel);

      /* if left is already a IFX then just change the if true label in that */
      if (!IS_IFX (tree->left))
        tree->left = newIfxNode (tree->left, localLabel, falseLabel);

      tree->right = backPatchLabels (tree->right, trueLabel, falseLabel);
      /* right is a IFX then just join */
      if (IS_IFX (tree->right))
        return newNode (NULLOP, tree->left, createLabel (localLabel, tree->right));

      tree->right = createLabel (localLabel, tree->right);
      tree->right = newIfxNode (tree->right, trueLabel, falseLabel);

      return newNode (NULLOP, tree->left, tree->right);
    }

  /* if this is an or operation */
  if (IS_OR (tree))
    {
      static int localLbl = 0;
      symbol *localLabel;

      SNPRINTF(buffer, sizeof(buffer), "_orif_%d", localLbl++);
      localLabel = newSymbol (buffer, NestLevel);

      tree->left = backPatchLabels (tree->left, trueLabel, localLabel);

      /* if left is already a IFX then just change the if true label in that */
      if (!IS_IFX (tree->left))
        tree->left = newIfxNode (tree->left, trueLabel, localLabel);

      tree->right = backPatchLabels (tree->right, trueLabel, falseLabel);
      /* right is a IFX then just join */
      if (IS_IFX (tree->right))
        return newNode (NULLOP, tree->left, createLabel (localLabel, tree->right));

      tree->right = createLabel (localLabel, tree->right);
      tree->right = newIfxNode (tree->right, trueLabel, falseLabel);

      return newNode (NULLOP, tree->left, tree->right);
    }

  /* change not */
  if (IS_NOT (tree))
    {
      int wasnot = IS_NOT (tree->left);
      tree->left = backPatchLabels (tree->left, falseLabel, trueLabel);

      /* if the left is already a IFX */
      if (!IS_IFX (tree->left))
        tree->left = newNode (IFX, tree->left, NULL);

      if (wasnot)
        {
          tree->left->trueLabel = trueLabel;
          tree->left->falseLabel = falseLabel;
        }
      else
        {
          tree->left->trueLabel = falseLabel;
          tree->left->falseLabel = trueLabel;
        }
      return tree->left;
    }

  if (IS_IFX (tree))
    {
      tree->trueLabel = trueLabel;
      tree->falseLabel = falseLabel;
    }

  return tree;
}


/*-----------------------------------------------------------------*/
/* createBlock - create expression tree for block                  */
/*-----------------------------------------------------------------*/
ast *
createBlock (symbol * decl, ast * body)
{
  ast *ex;

  /* if the block has nothing */
  if (!body && !decl)
    return NULL;

  ex = newNode (BLOCK, NULL, body);
  ex->values.sym = decl;
  
  ex->right = ex->right;
  ex->level++;
  ex->lineno = 0;
  return ex;
}

/*-----------------------------------------------------------------*/
/* createLabel - creates the expression tree for labels            */
/*-----------------------------------------------------------------*/
ast *
createLabel (symbol * label, ast * stmnt)
{
  symbol *csym;
  char name[SDCC_NAME_MAX + 1];
  ast *rValue;

  /* must create fresh symbol if the symbol name  */
  /* exists in the symbol table, since there can  */
  /* be a variable with the same name as the labl */
  if ((csym = findSym (SymbolTab, NULL, label->name)) &&
      (csym->level == label->level))
    label = newSymbol (label->name, label->level);

  /* change the name before putting it in add _ */
  SNPRINTF(name, sizeof(name), "%s", label->name);

  /* put the label in the LabelSymbol table    */
  /* but first check if a label of the same    */
  /* name exists                               */
  if ((csym = findSym (LabelTab, NULL, name)))
    werror (E_DUPLICATE_LABEL, label->name);
  else
    addSym (LabelTab, label, name, label->level, 0, 0);

  label->isitmp = 1;
  label->islbl = 1;
  label->key = labelKey++;
  rValue = newNode (LABEL, newAst_VALUE (symbolVal (label)), stmnt);
  rValue->lineno = 0;

  return rValue;
}

/*-----------------------------------------------------------------*/
/* createCase - generates the parsetree for a case statement       */
/*-----------------------------------------------------------------*/
ast *
createCase (ast * swStat, ast * caseVal, ast * stmnt)
{
  char caseLbl[SDCC_NAME_MAX + 1];
  ast *rexpr;
  value *val;

  /* if the switch statement does not exist */
  /* then case is out of context            */
  if (!swStat)
    {
      werrorfl (caseVal->filename, caseVal->lineno, E_CASE_CONTEXT);
      return NULL;
    }

  caseVal = decorateType (resolveSymbols (caseVal), RESULT_CHECK);
  /* if not a constant then error  */
  if (!IS_LITERAL (caseVal->ftype))
    {
      werrorfl (caseVal->filename, caseVal->lineno, E_CASE_CONSTANT);
      return NULL;
    }

  /* if not a integer than error */
  if (!IS_INTEGRAL (caseVal->ftype))
    {
      werrorfl (caseVal->filename, caseVal->lineno, E_CASE_NON_INTEGER);
      return NULL;
    }

  /* find the end of the switch values chain   */
  if (!(val = swStat->values.switchVals.swVals))
    swStat->values.switchVals.swVals = caseVal->opval.val;
  else
    {
      /* also order the cases according to value */
      value *pval = NULL;
      int cVal = (int) floatFromVal (caseVal->opval.val);
      while (val && (int) floatFromVal (val) < cVal)
        {
          pval = val;
          val = val->next;
        }

      /* if we reached the end then */
      if (!val)
        {
          pval->next = caseVal->opval.val;
        }
      else if ((int) floatFromVal (val) == cVal)
        {
          werrorfl (caseVal->filename, caseVal->lineno, E_DUPLICATE_LABEL,
                    "case");
          return NULL;
        }
      else
        {
          /* we found a value greater than */
          /* the current value we must add this */
          /* before the value */
          caseVal->opval.val->next = val;

          /* if this was the first in chain */
          if (swStat->values.switchVals.swVals == val)
            swStat->values.switchVals.swVals =
              caseVal->opval.val;
          else
            pval->next = caseVal->opval.val;
        }

    }

  /* create the case label   */
  SNPRINTF(caseLbl, sizeof(caseLbl), 
           "_case_%d_%d",
           swStat->values.switchVals.swNum,
           (int) floatFromVal (caseVal->opval.val));

  rexpr = createLabel (newSymbol (caseLbl, 0), stmnt);
  rexpr->lineno = 0;
  return rexpr;
}

/*-----------------------------------------------------------------*/
/* createDefault - creates the parse tree for the default statement */
/*-----------------------------------------------------------------*/
ast *
createDefault (ast * swStat, ast * defaultVal, ast * stmnt)
{
  char defLbl[SDCC_NAME_MAX + 1];

  /* if the switch statement does not exist */
  /* then case is out of context            */
  if (!swStat)
    {
      werrorfl (defaultVal->filename, defaultVal->lineno, E_CASE_CONTEXT);
      return NULL;
    }

  if (swStat->values.switchVals.swDefault)
    {
      werrorfl (defaultVal->filename, defaultVal->lineno, E_DUPLICATE_LABEL,
                "default");
      return NULL;
    }

  /* turn on the default flag   */
  swStat->values.switchVals.swDefault = 1;

  /* create the label  */
  SNPRINTF (defLbl, sizeof(defLbl),
            "_default_%d", swStat->values.switchVals.swNum);
  return createLabel (newSymbol (defLbl, 0), stmnt);
}

/*-----------------------------------------------------------------*/
/* createIf - creates the parsetree for the if statement           */
/*-----------------------------------------------------------------*/
ast *
createIf (ast * condAst, ast * ifBody, ast * elseBody)
{
  static int Lblnum = 0;
  ast *ifTree;
  symbol *ifTrue, *ifFalse, *ifEnd;

  /* if neither exists */
  if (!elseBody && !ifBody) {
    // if there are no side effects (i++, j() etc)
    if (!hasSEFcalls(condAst)) {
      return condAst;
    }
  }

  /* create the labels */
  SNPRINTF (buffer, sizeof(buffer), "_iffalse_%d", Lblnum);
  ifFalse = newSymbol (buffer, NestLevel);
  /* if no else body then end == false */
  if (!elseBody)
    ifEnd = ifFalse;
  else
    {
      SNPRINTF(buffer, sizeof(buffer), "_ifend_%d", Lblnum);
      ifEnd = newSymbol (buffer, NestLevel);
    }

  SNPRINTF (buffer, sizeof(buffer), "_iftrue_%d", Lblnum);
  ifTrue = newSymbol (buffer, NestLevel);

  Lblnum++;

  /* attach the ifTrue label to the top of it body */
  ifBody = createLabel (ifTrue, ifBody);
  /* attach a goto end to the ifBody if else is present */
  if (elseBody)
    {
      ifBody = newNode (NULLOP, ifBody,
                        newNode (GOTO,
                                 newAst_VALUE (symbolVal (ifEnd)),
                                 NULL));
      /* put the elseLabel on the else body */
      elseBody = createLabel (ifFalse, elseBody);
      /* out the end at the end of the body */
      elseBody = newNode (NULLOP,
                          elseBody,
                          createLabel (ifEnd, NULL));
    }
  else
    {
      ifBody = newNode (NULLOP, ifBody,
                        createLabel (ifFalse, NULL));
    }
  condAst = backPatchLabels (condAst, ifTrue, ifFalse);
  if (IS_IFX (condAst))
    ifTree = condAst;
  else
    ifTree = newIfxNode (condAst, ifTrue, ifFalse);

  return newNode (NULLOP, ifTree,
                  newNode (NULLOP, ifBody, elseBody));

}

/*-----------------------------------------------------------------*/
/* createDo - creates parse tree for do                            */
/*        _dobody_n:                                               */
/*            statements                                           */
/*        _docontinue_n:                                           */
/*            condition_expression +-> trueLabel -> _dobody_n      */
/*                                 |                               */
/*                                 +-> falseLabel-> _dobreak_n     */
/*        _dobreak_n:                                              */
/*-----------------------------------------------------------------*/
ast *
createDo (symbol * trueLabel, symbol * continueLabel,
          symbol * falseLabel, ast * condAst, ast * doBody)
{
  ast *doTree;


  /* if the body does not exist then it is simple */
  if (!doBody)
    {
      condAst = backPatchLabels (condAst, continueLabel, NULL);
      doTree = (IS_IFX (condAst) ? createLabel (continueLabel, condAst)
                : newNode (IFX, createLabel (continueLabel, condAst), NULL));
      doTree->trueLabel = continueLabel;
      doTree->falseLabel = NULL;
      return doTree;
    }

  /* otherwise we have a body */
  condAst = backPatchLabels (condAst, trueLabel, falseLabel);

  /* attach the body label to the top */
  doBody = createLabel (trueLabel, doBody);
  /* attach the continue label to end of body */
  doBody = newNode (NULLOP, doBody,
                    createLabel (continueLabel, NULL));

  /* now put the break label at the end */
  if (IS_IFX (condAst))
    doTree = condAst;
  else
    doTree = newIfxNode (condAst, trueLabel, falseLabel);

  doTree = newNode (NULLOP, doTree, createLabel (falseLabel, NULL));

  /* putting it together */
  return newNode (NULLOP, doBody, doTree);
}

/*-----------------------------------------------------------------*/
/* createFor - creates parse tree for 'for' statement              */
/*        initExpr                                                 */
/*   _forcond_n:                                                   */
/*        condExpr  +-> trueLabel -> _forbody_n                    */
/*                  |                                              */
/*                  +-> falseLabel-> _forbreak_n                   */
/*   _forbody_n:                                                   */
/*        statements                                               */
/*   _forcontinue_n:                                               */
/*        loopExpr                                                 */
/*        goto _forcond_n ;                                        */
/*   _forbreak_n:                                                  */
/*-----------------------------------------------------------------*/
ast *
createFor (symbol * trueLabel, symbol * continueLabel,
           symbol * falseLabel, symbol * condLabel,
           ast * initExpr, ast * condExpr, ast * loopExpr,
           ast * forBody)
{
  ast *forTree;

  /* if loopexpression not present then we can generate it */
  /* the same way as a while */
  if (!loopExpr)
    return newNode (NULLOP, initExpr,
                    createWhile (trueLabel, continueLabel,
                                 falseLabel, condExpr, forBody));
  /* vanilla for statement */
  condExpr = backPatchLabels (condExpr, trueLabel, falseLabel);

  if (condExpr && !IS_IFX (condExpr))
    condExpr = newIfxNode (condExpr, trueLabel, falseLabel);


  /* attach condition label to condition */
  condExpr = createLabel (condLabel, condExpr);

  /* attach body label to body */
  forBody = createLabel (trueLabel, forBody);

  /* attach continue to forLoop expression & attach */
  /* goto the forcond @ and of loopExpression       */
  loopExpr = createLabel (continueLabel,
                          newNode (NULLOP,
                                   loopExpr,
                                   newNode (GOTO,
                                       newAst_VALUE (symbolVal (condLabel)),
                                            NULL)));
  /* now start putting them together */
  forTree = newNode (NULLOP, initExpr, condExpr);
  forTree = newNode (NULLOP, forTree, forBody);
  forTree = newNode (NULLOP, forTree, loopExpr);
  /* finally add the break label */
  forTree = newNode (NULLOP, forTree,
                     createLabel (falseLabel, NULL));
  return forTree;
}

/*-----------------------------------------------------------------*/
/* createWhile - creates parse tree for while statement            */
/*               the while statement will be created as follows    */
/*                                                                 */
/*      _while_continue_n:                                         */
/*            condition_expression +-> trueLabel -> _while_boby_n  */
/*                                 |                               */
/*                                 +-> falseLabel -> _while_break_n */
/*      _while_body_n:                                             */
/*            statements                                           */
/*            goto _while_continue_n                               */
/*      _while_break_n:                                            */
/*-----------------------------------------------------------------*/
ast *
createWhile (symbol * trueLabel, symbol * continueLabel,
             symbol * falseLabel, ast * condExpr, ast * whileBody)
{
  ast *whileTree;

  /* put the continue label */
  condExpr = backPatchLabels (condExpr, trueLabel, falseLabel);
  condExpr = createLabel (continueLabel, condExpr);
  condExpr->lineno = 0;

  /* put the body label in front of the body */
  whileBody = createLabel (trueLabel, whileBody);
  whileBody->lineno = 0;
  /* put a jump to continue at the end of the body */
  /* and put break label at the end of the body */
  whileBody = newNode (NULLOP,
                       whileBody,
                       newNode (GOTO,
                                newAst_VALUE (symbolVal (continueLabel)),
                                createLabel (falseLabel, NULL)));

  /* put it all together */
  if (IS_IFX (condExpr))
    whileTree = condExpr;
  else
    {
      whileTree = newNode (IFX, condExpr, NULL);
      /* put the true & false labels in place */
      whileTree->trueLabel = trueLabel;
      whileTree->falseLabel = falseLabel;
    }

  return newNode (NULLOP, whileTree, whileBody);
}

/*-----------------------------------------------------------------*/
/* optimizeGetHbit - get highest order bit of the expression       */
/*-----------------------------------------------------------------*/
ast *
optimizeGetHbit (ast * tree)
{
  int i, j;
  /* if this is not a bit and */
  if (!IS_BITAND (tree))
    return tree;

  /* will look for tree of the form
     ( expr >> ((sizeof expr) -1) ) & 1 */
  if (!IS_AST_LIT_VALUE (tree->right))
    return tree;

  if (AST_LIT_VALUE (tree->right) != 1)
    return tree;

  if (!IS_RIGHT_OP (tree->left))
    return tree;

  if (!IS_AST_LIT_VALUE (tree->left->right))
    return tree;

  if ((i = (int) AST_LIT_VALUE (tree->left->right)) !=
      (j = (getSize (TTYPE (tree->left->left)) * 8 - 1)))
    return tree;
      
  /* make sure the port supports GETHBIT */
  if (port->hasExtBitOp
      && !port->hasExtBitOp(GETHBIT, getSize (TTYPE (tree->left->left))))
    return tree;

  return decorateType (newNode (GETHBIT, tree->left->left, NULL), RESULT_CHECK);

}

/*-----------------------------------------------------------------*/
/* optimizeRRCRLC :- optimize for Rotate Left/Right with carry     */
/*-----------------------------------------------------------------*/
ast *
optimizeRRCRLC (ast * root)
{
  /* will look for trees of the form
     (?expr << 1) | (?expr >> 7) or
     (?expr >> 7) | (?expr << 1) will make that
     into a RLC : operation ..
     Will also look for
     (?expr >> 1) | (?expr << 7) or
     (?expr << 7) | (?expr >> 1) will make that
     into a RRC operation
     note : by 7 I mean (number of bits required to hold the
     variable -1 ) */
  /* if the root operations is not a | operation the not */
  if (!IS_BITOR (root))
    return root;

  /* I have to think of a better way to match patterns this sucks */
  /* that aside let start looking for the first case : I use a the
     negative check a lot to improve the efficiency */
  /* (?expr << 1) | (?expr >> 7) */
  if (IS_LEFT_OP (root->left) &&
      IS_RIGHT_OP (root->right))
    {

      if (!SPEC_USIGN (TETYPE (root->left->left)))
        return root;

      if (!IS_AST_LIT_VALUE (root->left->right) ||
          !IS_AST_LIT_VALUE (root->right->right))
        goto tryNext0;

      /* make sure it is the same expression */
      if (!isAstEqual (root->left->left,
                       root->right->left))
        goto tryNext0;

      if (AST_LIT_VALUE (root->left->right) != 1)
        goto tryNext0;

      if (AST_LIT_VALUE (root->right->right) !=
          (getSize (TTYPE (root->left->left)) * 8 - 1))
        goto tryNext0;

      /* make sure the port supports RLC */
      if (port->hasExtBitOp
          && !port->hasExtBitOp(RLC, getSize (TTYPE (root->left->left))))
        return root;

      /* whew got the first case : create the AST */
      return newNode (RLC, root->left->left, NULL);
    }

tryNext0:
  /* check for second case */
  /* (?expr >> 7) | (?expr << 1) */
  if (IS_LEFT_OP (root->right) &&
      IS_RIGHT_OP (root->left))
    {

      if (!SPEC_USIGN (TETYPE (root->left->left)))
        return root;

      if (!IS_AST_LIT_VALUE (root->left->right) ||
          !IS_AST_LIT_VALUE (root->right->right))
        goto tryNext1;

      /* make sure it is the same symbol */
      if (!isAstEqual (root->left->left,
                       root->right->left))
        goto tryNext1;

      if (AST_LIT_VALUE (root->right->right) != 1)
        goto tryNext1;

      if (AST_LIT_VALUE (root->left->right) !=
          (getSize (TTYPE (root->left->left)) * 8 - 1))
        goto tryNext1;

      /* make sure the port supports RLC */
      if (port->hasExtBitOp
          && !port->hasExtBitOp(RLC, getSize (TTYPE (root->left->left))))
        return root;

      /* whew got the first case : create the AST */
      return newNode (RLC, root->left->left, NULL);

    }

tryNext1:
  /* third case for RRC */
  /*  (?symbol >> 1) | (?symbol << 7) */
  if (IS_LEFT_OP (root->right) &&
      IS_RIGHT_OP (root->left))
    {

      if (!SPEC_USIGN (TETYPE (root->left->left)))
        return root;

      if (!IS_AST_LIT_VALUE (root->left->right) ||
          !IS_AST_LIT_VALUE (root->right->right))
        goto tryNext2;

      /* make sure it is the same symbol */
      if (!isAstEqual (root->left->left,
                       root->right->left))
        goto tryNext2;

      if (AST_LIT_VALUE (root->left->right) != 1)
        goto tryNext2;

      if (AST_LIT_VALUE (root->right->right) !=
          (getSize (TTYPE (root->left->left)) * 8 - 1))
        goto tryNext2;

      /* make sure the port supports RRC */
      if (port->hasExtBitOp
          && !port->hasExtBitOp(RRC, getSize (TTYPE (root->left->left))))
        return root;

      /* whew got the first case : create the AST */
      return newNode (RRC, root->left->left, NULL);

    }
tryNext2:
  /* fourth and last case for now */
  /* (?symbol << 7) | (?symbol >> 1) */
  if (IS_RIGHT_OP (root->right) &&
      IS_LEFT_OP (root->left))
    {

      if (!SPEC_USIGN (TETYPE (root->left->left)))
        return root;

      if (!IS_AST_LIT_VALUE (root->left->right) ||
          !IS_AST_LIT_VALUE (root->right->right))
        return root;

      /* make sure it is the same symbol */
      if (!isAstEqual (root->left->left,
                       root->right->left))
        return root;

      if (AST_LIT_VALUE (root->right->right) != 1)
        return root;

      if (AST_LIT_VALUE (root->left->right) !=
          (getSize (TTYPE (root->left->left)) * 8 - 1))
        return root;

      /* make sure the port supports RRC */
      if (port->hasExtBitOp
          && !port->hasExtBitOp(RRC, getSize (TTYPE (root->left->left))))
        return root;

      /* whew got the first case : create the AST */
      return newNode (RRC, root->left->left, NULL);

    }

  /* not found return root */
  return root;
}

/*-----------------------------------------------------------------*/
/* optimizeSWAP :- optimize for nibble/byte/word swaps             */
/*-----------------------------------------------------------------*/
ast *
optimizeSWAP (ast * root)
{
  /* will look for trees of the form
     (?expr << 4) | (?expr >> 4) or
     (?expr >> 4) | (?expr << 4) will make that
     into a SWAP : operation ..
     note : by 4 I mean (number of bits required to hold the
     variable /2 ) */
  /* if the root operations is not a | operation the not */
  if (!IS_BITOR (root))
    return root;

  /* (?expr << 4) | (?expr >> 4) */
  if ((IS_LEFT_OP (root->left) && IS_RIGHT_OP (root->right))
      || (IS_RIGHT_OP (root->left) && IS_LEFT_OP (root->right)))
    {

      if (!SPEC_USIGN (TETYPE (root->left->left)))
        return root;

      if (!IS_AST_LIT_VALUE (root->left->right) ||
          !IS_AST_LIT_VALUE (root->right->right))
        return root;

      /* make sure it is the same expression */
      if (!isAstEqual (root->left->left,
                       root->right->left))
        return root;

      if (AST_LIT_VALUE (root->left->right) !=
          (getSize (TTYPE (root->left->left)) * 4))
        return root;

      if (AST_LIT_VALUE (root->right->right) !=
          (getSize (TTYPE (root->left->left)) * 4))
        return root;

      /* make sure the port supports SWAP */
      if (port->hasExtBitOp
          && !port->hasExtBitOp(SWAP, getSize (TTYPE (root->left->left))))
        return root;

      /* found it : create the AST */
      return newNode (SWAP, root->left->left, NULL);
    }


  /* not found return root */
  return root;
}

/*-----------------------------------------------------------------*/
/* optimizeCompare - otimizes compares for bit variables     */
/*-----------------------------------------------------------------*/
static ast *
optimizeCompare (ast * root)
{
  ast *optExpr = NULL;
  value *vleft;
  value *vright;
  unsigned int litValue;

  /* if nothing then return nothing */
  if (!root)
    return NULL;

  /* if not a compare op then do leaves */
  if (!IS_COMPARE_OP (root))
    {
      root->left = optimizeCompare (root->left);
      root->right = optimizeCompare (root->right);
      return root;
    }

  /* if left & right are the same then depending
     of the operation do */
  if (isAstEqual (root->left, root->right))
    {
      switch (root->opval.op)
        {
        case '>':
        case '<':
        case NE_OP:
          optExpr = newAst_VALUE (constVal ("0"));
          break;
        case GE_OP:
        case LE_OP:
        case EQ_OP:
          optExpr = newAst_VALUE (constVal ("1"));
          break;
        }

      return decorateType (optExpr, RESULT_CHECK);
    }

  vleft = (root->left->type == EX_VALUE ?
           root->left->opval.val : NULL);

  vright = (root->right->type == EX_VALUE ?
            root->right->opval.val : NULL);

  /* if left is a BITVAR in BITSPACE */
  /* and right is a LITERAL then opt- */
  /* imize else do nothing       */
  if (vleft && vright &&
      IS_BITVAR (vleft->etype) &&
      IN_BITSPACE (SPEC_OCLS (vleft->etype)) &&
      IS_LITERAL (vright->etype))
    {

      /* if right side > 1 then comparison may never succeed */
      if ((litValue = (int) floatFromVal (vright)) > 1)
        {
          werror (W_BAD_COMPARE);
          goto noOptimize;
        }

      if (litValue)
        {
          switch (root->opval.op)
            {
            case '>':           /* bit value greater than 1 cannot be */
              werror (W_BAD_COMPARE);
              goto noOptimize;
              break;

            case '<':           /* bit value < 1 means 0 */
            case NE_OP:
              optExpr = newNode ('!', newAst_VALUE (vleft), NULL);
              break;

            case LE_OP: /* bit value <= 1 means no check */
              optExpr = newAst_VALUE (vright);
              break;

            case GE_OP: /* bit value >= 1 means only check for = */
            case EQ_OP:
              optExpr = newAst_VALUE (vleft);
              break;
            }
        }
      else
        {                       /* literal is zero */
          switch (root->opval.op)
            {
            case '<':           /* bit value < 0 cannot be */
              werror (W_BAD_COMPARE);
              goto noOptimize;
              break;

            case '>':           /* bit value > 0 means 1 */
            case NE_OP:
              optExpr = newAst_VALUE (vleft);
              break;

            case LE_OP: /* bit value <= 0 means no check */
            case GE_OP: /* bit value >= 0 means no check */
              werror (W_BAD_COMPARE);
              goto noOptimize;
              break;

            case EQ_OP: /* bit == 0 means ! of bit */
              optExpr = newNode ('!', newAst_VALUE (vleft), NULL);
              break;
            }
        }
      return decorateType (resolveSymbols (optExpr), RESULT_CHECK);
    }                           /* end-of-if of BITVAR */

noOptimize:
  return root;
}
/*-----------------------------------------------------------------*/
/* addSymToBlock : adds the symbol to the first block we find      */
/*-----------------------------------------------------------------*/
void 
addSymToBlock (symbol * sym, ast * tree)
{
  /* reached end of tree or a leaf */
  if (!tree || IS_AST_LINK (tree) || IS_AST_VALUE (tree))
    return;

  /* found a block */
  if (IS_AST_OP (tree) &&
      tree->opval.op == BLOCK)
    {

      symbol *lsym = copySymbol (sym);

      lsym->next = AST_VALUES (tree, sym);
      AST_VALUES (tree, sym) = lsym;
      return;
    }

  addSymToBlock (sym, tree->left);
  addSymToBlock (sym, tree->right);
}

/*-----------------------------------------------------------------*/
/* processRegParms - do processing for register parameters         */
/*-----------------------------------------------------------------*/
static void 
processRegParms (value * args, ast * body)
{
  while (args)
    {
      if (IS_REGPARM (args->etype))
        addSymToBlock (args->sym, body);
      args = args->next;
    }
}

/*-----------------------------------------------------------------*/
/* resetParmKey - resets the operandkeys for the symbols           */
/*-----------------------------------------------------------------*/
DEFSETFUNC (resetParmKey)
{
  symbol *sym = item;

  sym->key = 0;
  sym->defs = NULL;
  sym->uses = NULL;
  sym->remat = 0;
  return 1;
}

/*-----------------------------------------------------------------*/
/* createFunction - This is the key node that calls the iCode for  */
/*                  generating the code for a function. Note code  */
/*                  is generated function by function, later when  */
/*                  add inter-procedural analysis this will change */
/*-----------------------------------------------------------------*/
ast *
createFunction (symbol * name, ast * body)
{
  ast *ex;
  symbol *csym;
  int stack = 0;
  sym_link *fetype;
  iCode *piCode = NULL;

  if (getenv("SDCC_DEBUG_FUNCTION_POINTERS"))
    fprintf (stderr, "SDCCast.c:createFunction(%s)\n", name->name);

  /* if check function return 0 then some problem */
  if (checkFunction (name, NULL) == 0)
    return NULL;

  /* create a dummy block if none exists */
  if (!body)
    body = newNode (BLOCK, NULL, NULL);

  noLineno++;

  /* check if the function name already in the symbol table */
  if ((csym = findSym (SymbolTab, NULL, name->name)))
    {
      name = csym;
      /* special case for compiler defined functions
         we need to add the name to the publics list : this
         actually means we are now compiling the compiler
         support routine */
      if (name->cdef)
        {
          addSet (&publics, name);
        }
    }
  else
    {
      addSymChain (name);
      allocVariables (name);
    }
  name->lastLine = mylineno;
  currFunc = name;

  /* set the stack pointer */
  /* PENDING: check this for the mcs51 */
  stackPtr = -port->stack.direction * port->stack.call_overhead;
  if (IFFUNC_ISISR (name->type))
    stackPtr -= port->stack.direction * port->stack.isr_overhead;
  if (IFFUNC_ISREENT (name->type) || options.stackAuto)
    stackPtr -= port->stack.direction * port->stack.reent_overhead;

  xstackPtr = -port->stack.direction * port->stack.call_overhead;

  fetype = getSpec (name->type);        /* get the specifier for the function */
  /* if this is a reentrant function then */
  if (IFFUNC_ISREENT (name->type))
    reentrant++;

  allocParms (FUNC_ARGS(name->type));   /* allocate the parameters */

  /* do processing for parameters that are passed in registers */
  processRegParms (FUNC_ARGS(name->type), body);

  /* set the stack pointer */
  stackPtr = 0;
  xstackPtr = -1;

  /* allocate & autoinit the block variables */
  processBlockVars (body, &stack, ALLOCATE);

  /* save the stack information */
  if (options.useXstack)
    name->xstack = SPEC_STAK (fetype) = stack;
  else
    name->stack = SPEC_STAK (fetype) = stack;

  /* name needs to be mangled */
  SNPRINTF (name->rname, sizeof(name->rname), "%s%s", port->fun_prefix, name->name);

  body = resolveSymbols (body); /* resolve the symbols */
  body = decorateType (body, RESULT_TYPE_NONE); /* propagateType & do semantic checks */
                                        

  ex = newAst_VALUE (symbolVal (name)); /* create name */
  ex = newNode (FUNCTION, ex, body);
  ex->values.args = FUNC_ARGS(name->type);
  ex->decorated=1;
  if (options.dump_tree) PA(ex);
  if (fatalError)
    {
      werror (E_FUNC_NO_CODE, name->name);
      goto skipall;
    }

  /* create the node & generate intermediate code */
  GcurMemmap = code;
  codeOutFile = code->oFile;
  piCode = iCodeFromAst (ex);

  if (fatalError)
    {
      werror (E_FUNC_NO_CODE, name->name);
      goto skipall;
    }

  eBBlockFromiCode (piCode);

  /* if there are any statics then do them */
  if (staticAutos)
    {
      GcurMemmap = statsg;
      codeOutFile = statsg->oFile;
      eBBlockFromiCode (iCodeFromAst (decorateType (resolveSymbols (staticAutos), RESULT_CHECK)));
      staticAutos = NULL;
    }

skipall:

  /* dealloc the block variables */
  processBlockVars (body, &stack, DEALLOCATE);
  outputDebugStackSymbols();
  /* deallocate paramaters */
  deallocParms (FUNC_ARGS(name->type));

  if (IFFUNC_ISREENT (name->type))
    reentrant--;

  /* we are done freeup memory & cleanup */
  noLineno--;
  if (port->reset_labelKey) labelKey = 1;
  name->key = 0;
  FUNC_HASBODY(name->type) = 1;
  addSet (&operKeyReset, name);
  applyToSet (operKeyReset, resetParmKey);

  if (options.debug)
    cdbStructBlock(1);

  cleanUpLevel (LabelTab, 0);
  cleanUpBlock (StructTab, 1);
  cleanUpBlock (TypedefTab, 1);

  xstack->syms = NULL;
  istack->syms = NULL;
  return NULL;
}


#define INDENT(x,f) { int i ; fprintf (f, "%d:", tree->lineno); for (i=0;i < x; i++) fprintf(f," "); }
/*-----------------------------------------------------------------*/
/* ast_print : prints the ast (for debugging purposes)             */
/*-----------------------------------------------------------------*/

void ast_print (ast * tree, FILE *outfile, int indent)
{

        if (!tree) return ;

        /* can print only decorated trees */
        if (!tree->decorated) return;

        /* if any child is an error | this one is an error do nothing */
        if (tree->isError ||
            (tree->left && tree->left->isError) ||
            (tree->right && tree->right->isError)) {
                fprintf(outfile,"ERROR_NODE(%p)\n",tree);
        }


        /* print the line          */
        /* if not block & function */
        if (tree->type == EX_OP &&
            (tree->opval.op != FUNCTION &&
             tree->opval.op != BLOCK &&
             tree->opval.op != NULLOP)) {
        }

        if (tree->opval.op == FUNCTION) {
                int arg=0;
                value *args=FUNC_ARGS(tree->left->opval.val->type);
                fprintf(outfile,"FUNCTION (%s=%p) type (",
                        tree->left->opval.val->name, tree);
                printTypeChain (tree->left->opval.val->type->next,outfile);
                fprintf(outfile,") args (");
                do {
                  if (arg) {
                    fprintf (outfile, ", ");
                  }
                  printTypeChain (args ? args->type : NULL, outfile);
                  arg++;
                  args= args ? args->next : NULL;
                } while (args);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent);
                ast_print(tree->right,outfile,indent);
                return ;
        }
        if (tree->opval.op == BLOCK) {
                symbol *decls = tree->values.sym;
                INDENT(indent,outfile);
                fprintf(outfile,"{\n");
                while (decls) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"DECLARE SYMBOL (%s=%p) type (",
                                decls->name, decls);
                        printTypeChain(decls->type,outfile);
                        fprintf(outfile,")\n");

                        decls = decls->next;
                }
                ast_print(tree->right,outfile,indent+2);
                INDENT(indent,outfile);
                fprintf(outfile,"}\n");
                return;
        }
        if (tree->opval.op == NULLOP) {
                ast_print(tree->left,outfile,indent);
                ast_print(tree->right,outfile,indent);
                return ;
        }
        INDENT(indent,outfile);

        /*------------------------------------------------------------------*/
        /*----------------------------*/
        /*   leaf has been reached    */
        /*----------------------------*/
        /* if this is of type value */
        /* just get the type        */
        if (tree->type == EX_VALUE) {

                if (IS_LITERAL (tree->opval.val->etype)) {
                        fprintf(outfile,"CONSTANT (%p) value = ", tree);
                        if (SPEC_USIGN (tree->opval.val->etype))
                                fprintf(outfile,"%u", (TYPE_UDWORD) floatFromVal(tree->opval.val));
                        else
                                fprintf(outfile,"%d", (TYPE_DWORD) floatFromVal(tree->opval.val));
                        fprintf(outfile,", 0x%x, %f", (TYPE_UDWORD) floatFromVal(tree->opval.val),
                                                      floatFromVal(tree->opval.val));
                } else if (tree->opval.val->sym) {
                        /* if the undefined flag is set then give error message */
                        if (tree->opval.val->sym->undefined) {
                                fprintf(outfile,"UNDEFINED SYMBOL ");
                        } else {
                                fprintf(outfile,"SYMBOL ");
                        }
                        fprintf(outfile,"(%s=%p)",
                                tree->opval.val->sym->name,tree);
                }
                if (tree->ftype) {
                        fprintf(outfile," type (");
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                } else {
                        fprintf(outfile,"\n");
                }
                return ;
        }

        /* if type link for the case of cast */
        if (tree->type == EX_LINK) {
                fprintf(outfile,"TYPENODE (%p) type = (",tree);
                printTypeChain(tree->opval.lnk,outfile);
                fprintf(outfile,")\n");
                return ;
        }


        /* depending on type of operator do */

        switch (tree->opval.op) {
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*        array node          */
                /*----------------------------*/
        case '[':
                fprintf(outfile,"ARRAY_OP (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*      struct/union          */
                /*----------------------------*/
        case '.':
                fprintf(outfile,"STRUCT_ACCESS (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*    struct/union pointer    */
                /*----------------------------*/
        case PTR_OP:
                fprintf(outfile,"PTR_ACCESS (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*  ++/-- operation           */
                /*----------------------------*/
        case INC_OP:
                if (tree->left)
                  fprintf(outfile,"post-");
                else
                  fprintf(outfile,"pre-");
                fprintf(outfile,"INC_OP (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2); /* postincrement case */
                ast_print(tree->right,outfile,indent+2); /* preincrement case */
                return ;

        case DEC_OP:
                if (tree->left)
                  fprintf(outfile,"post-");
                else
                  fprintf(outfile,"pre-");
                fprintf(outfile,"DEC_OP (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2); /* postdecrement case */
                ast_print(tree->right,outfile,indent+2); /* predecrement case */
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*  bitwise and               */
                /*----------------------------*/
        case '&':
                if (tree->right) {
                        fprintf(outfile,"& (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                        ast_print(tree->right,outfile,indent+2);
                } else {
                        fprintf(outfile,"ADDRESS_OF (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                        ast_print(tree->right,outfile,indent+2);
                }
                return ;
                /*----------------------------*/
                /*  bitwise or                */
                /*----------------------------*/
        case '|':
                fprintf(outfile,"OR (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*  bitwise xor               */
                /*----------------------------*/
        case '^':
                fprintf(outfile,"XOR (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*  division                  */
                /*----------------------------*/
        case '/':
                fprintf(outfile,"DIV (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*            modulus         */
                /*----------------------------*/
        case '%':
                fprintf(outfile,"MOD (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*  address dereference       */
                /*----------------------------*/
        case '*':                       /* can be unary  : if right is null then unary operation */
                if (!tree->right) {
                        fprintf(outfile,"DEREF (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                        return ;
                }                       
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*      multiplication        */
                /*----------------------------*/                
                fprintf(outfile,"MULT (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;


                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*    unary '+' operator      */
                /*----------------------------*/
        case '+':
                /* if unary plus */
                if (!tree->right) {
                        fprintf(outfile,"UPLUS (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                } else {
                        /*------------------------------------------------------------------*/
                        /*----------------------------*/
                        /*      addition              */
                        /*----------------------------*/
                        fprintf(outfile,"ADD (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                        ast_print(tree->right,outfile,indent+2);
                }
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*      unary '-'             */
                /*----------------------------*/
        case '-':                       /* can be unary   */
                if (!tree->right) {
                        fprintf(outfile,"UMINUS (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                } else {
                        /*------------------------------------------------------------------*/
                        /*----------------------------*/
                        /*      subtraction           */
                        /*----------------------------*/
                        fprintf(outfile,"SUB (%p) type (",tree);
                        printTypeChain(tree->ftype,outfile);
                        fprintf(outfile,")\n");
                        ast_print(tree->left,outfile,indent+2);
                        ast_print(tree->right,outfile,indent+2);
                }
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*    compliment              */
                /*----------------------------*/
        case '~':
                fprintf(outfile,"COMPL (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*           not              */
                /*----------------------------*/
        case '!':
                fprintf(outfile,"NOT (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*           shift            */
                /*----------------------------*/
        case RRC:
                fprintf(outfile,"RRC (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;

        case RLC:
                fprintf(outfile,"RLC (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;
        case SWAP:
                fprintf(outfile,"SWAP (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;
        case GETHBIT:
                fprintf(outfile,"GETHBIT (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                return ;
        case LEFT_OP:
                fprintf(outfile,"LEFT_SHIFT (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case RIGHT_OP:
                fprintf(outfile,"RIGHT_SHIFT (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*         casting            */
                /*----------------------------*/
        case CAST:                      /* change the type   */
                fprintf(outfile,"CAST (%p) from type (",tree);
                printTypeChain(tree->right->ftype,outfile);
                fprintf(outfile,") to type (");
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->right,outfile,indent+2);
                return ;
                
        case AND_OP:
                fprintf(outfile,"ANDAND (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case OR_OP:
                fprintf(outfile,"OROR (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
                
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*     comparison operators   */
                /*----------------------------*/
        case '>':
                fprintf(outfile,"GT(>) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case '<':
                fprintf(outfile,"LT(<) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case LE_OP:
                fprintf(outfile,"LE(<=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case GE_OP:
                fprintf(outfile,"GE(>=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case EQ_OP:
                fprintf(outfile,"EQ(==) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;
        case NE_OP:
                fprintf(outfile,"NE(!=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*             sizeof         */
                /*----------------------------*/
        case SIZEOF:            /* evaluate wihout code generation */
                fprintf(outfile,"SIZEOF %d\n",(getSize (tree->right->ftype)));
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /* conditional operator  '?'  */
                /*----------------------------*/
        case '?':
                fprintf(outfile,"QUEST(?) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;

        case ':':
                fprintf(outfile,"COLON(:) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return ;

                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*    assignment operators    */
                /*----------------------------*/
        case MUL_ASSIGN:
                fprintf(outfile,"MULASS(*=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case DIV_ASSIGN:
                fprintf(outfile,"DIVASS(/=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case AND_ASSIGN:
                fprintf(outfile,"ANDASS(&=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case OR_ASSIGN:
                fprintf(outfile,"ORASS(|=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case XOR_ASSIGN:
                fprintf(outfile,"XORASS(^=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case RIGHT_ASSIGN:
                fprintf(outfile,"RSHFTASS(>>=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case LEFT_ASSIGN:
                fprintf(outfile,"LSHFTASS(<<=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*    -= operator             */
                /*----------------------------*/
        case SUB_ASSIGN:
                fprintf(outfile,"SUBASS(-=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*          += operator       */
                /*----------------------------*/
        case ADD_ASSIGN:
                fprintf(outfile,"ADDASS(+=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*      straight assignemnt   */
                /*----------------------------*/
        case '=':
                fprintf(outfile,"ASSIGN(=) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;     
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*      comma operator        */
                /*----------------------------*/
        case ',':
                fprintf(outfile,"COMMA(,) (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*       function call        */
                /*----------------------------*/
        case CALL:
        case PCALL:
                fprintf(outfile,"CALL (%p) type (",tree);
                printTypeChain(tree->ftype,outfile);
                fprintf(outfile,")\n");
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent+2);
                return;
        case PARAM:
                fprintf(outfile,"PARMS\n");
                ast_print(tree->left,outfile,indent+2);
                if (tree->right /*&& !IS_AST_PARAM(tree->right)*/) {
                        ast_print(tree->right,outfile,indent+2);
                }
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*     return statement       */
                /*----------------------------*/
        case RETURN:
                fprintf(outfile,"RETURN (%p) type (",tree);
                if (tree->right) {
                    printTypeChain(tree->right->ftype,outfile);
                }
                fprintf(outfile,")\n");
                ast_print(tree->right,outfile,indent+2);
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*     label statement        */
                /*----------------------------*/
        case LABEL :
                fprintf(outfile,"LABEL (%p)\n",tree);
                ast_print(tree->left,outfile,indent+2);
                ast_print(tree->right,outfile,indent);
                return;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /*     switch statement       */
                /*----------------------------*/
        case SWITCH:
                {
                        value *val;
                        fprintf(outfile,"SWITCH (%p) ",tree);
                        ast_print(tree->left,outfile,0);
                        for (val = tree->values.switchVals.swVals; val ; val = val->next) {
                                INDENT(indent+2,outfile);
                                fprintf(outfile,"CASE 0x%x GOTO _case_%d_%d\n",
                                        (int) floatFromVal(val),
                                        tree->values.switchVals.swNum,
                                        (int) floatFromVal(val));
                        }
                        ast_print(tree->right,outfile,indent);
                }
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /* ifx Statement              */
                /*----------------------------*/
        case IFX:
                fprintf(outfile,"IF (%p) \n",tree);
                ast_print(tree->left,outfile,indent+2);
                if (tree->trueLabel) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"NE(!=) 0 goto %s\n",tree->trueLabel->name);
                }
                if (tree->falseLabel) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"EQ(==) 0 goto %s\n",tree->falseLabel->name);
                }
                ast_print(tree->right,outfile,indent+2);
                return ;
                /*----------------------------*/
                /* goto Statement              */
                /*----------------------------*/
        case GOTO:
                fprintf(outfile,"GOTO (%p) \n",tree);
                ast_print(tree->left,outfile,indent+2);
                fprintf(outfile,"\n");
                return ;
                /*------------------------------------------------------------------*/
                /*----------------------------*/
                /* for Statement              */
                /*----------------------------*/
        case FOR:
                fprintf(outfile,"FOR (%p) \n",tree);
                if (AST_FOR( tree, initExpr)) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"INIT EXPR ");
                        ast_print(AST_FOR(tree, initExpr),outfile,indent+2);
                }
                if (AST_FOR( tree, condExpr)) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"COND EXPR ");
                        ast_print(AST_FOR(tree, condExpr),outfile,indent+2);
                }
                if (AST_FOR( tree, loopExpr)) {
                        INDENT(indent+2,outfile);
                        fprintf(outfile,"LOOP EXPR ");
                        ast_print(AST_FOR(tree, loopExpr),outfile,indent+2);
                }
                fprintf(outfile,"FOR LOOP BODY \n");
                ast_print(tree->left,outfile,indent+2);
                return ;
        case CRITICAL:
                fprintf(outfile,"CRITICAL (%p) \n",tree);
                ast_print(tree->left,outfile,indent+2);
        default:
            return ;
        }
}

void PA(ast *t)
{
        ast_print(t,stdout,0);
}



/*-----------------------------------------------------------------*/
/* astErrors : returns non-zero if errors present in tree          */
/*-----------------------------------------------------------------*/
int astErrors(ast *t)
{
  int errors=0;
  
  if (t)
    {
      if (t->isError)
        errors++;
  
      if (t->type == EX_VALUE
          && t->opval.val->sym
          && t->opval.val->sym->undefined)
        errors++;

      errors += astErrors(t->left);
      errors += astErrors(t->right);
    }
    
  return errors;
}
