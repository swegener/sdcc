/*----------------------------------------------------------------------
    SDCCval.c :- has routine to do all kinds of fun stuff with the
                value wrapper & with initialiser lists.

    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1998)

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
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "newalloc.h"

int cNestLevel;

/*-----------------------------------------------------------------*/
/* newValue - allocates and returns a new value        */
/*-----------------------------------------------------------------*/
value *
newValue ()
{
  value *val;

  val = Safe_alloc (sizeof (value));

  return val;
}

/*-----------------------------------------------------------------*/
/* newiList - new initializer list                                 */
/*-----------------------------------------------------------------*/
initList *
newiList (int type, void *ilist)
{
  initList *nilist;


  nilist = Safe_alloc (sizeof (initList));

  nilist->type = type;
  nilist->lineno = lexLineno;
  nilist->filename = lexFilename;

  switch (type)
    {
    case INIT_NODE:
      nilist->init.node = (struct ast *) ilist;
      break;

    case INIT_DEEP:
      nilist->init.deep = (struct initList *) ilist;
      break;
    }

  return nilist;
}

/*------------------------------------------------------------------*/
/* revinit   - reverses the initial values for a value  chain        */
/*------------------------------------------------------------------*/
initList *
revinit (initList * val)
{
  initList *prev, *curr, *next;

  if (!val)
    return NULL;

  prev = val;
  curr = val->next;

  while (curr)
    {
      next = curr->next;
      curr->next = prev;
      prev = curr;
      curr = next;
    }
  val->next = (void *) NULL;
  return prev;
}

bool
convertIListToConstList(initList *src, literalList **lList)
{
    initList    *iLoop;
    literalList *head, *last, *newL;

    head = last = NULL;

    if (!src || src->type != INIT_DEEP)
    {
        return FALSE;
    }

    iLoop =  src->init.deep;

    while (iLoop)
    {
        if (iLoop->type != INIT_NODE)
        {
            return FALSE;
        }

        if (!IS_AST_LIT_VALUE(decorateType(resolveSymbols(iLoop->init.node), RESULT_TYPE_NONE)))
        {
            return FALSE;
        }
        iLoop = iLoop->next;
    }

    /* We've now established that the initializer list contains only literal values. */

    iLoop = src->init.deep;
    while (iLoop)
    {
        double val = AST_LIT_VALUE(iLoop->init.node);

        if (last && last->literalValue == val)
        {
            last->count++;
        }
        else
        {
            newL = Safe_alloc(sizeof(literalList));
            newL->literalValue = val;
            newL->count = 1;
            newL->next = NULL;

            if (last)
            {
                last->next = newL;
            }
            else
            {
                head = newL;
            }
            last = newL;
        }
        iLoop = iLoop->next;
    }

    if (!head)
    {
        return FALSE;
    }

    *lList = head;
    return TRUE;
}

literalList *
copyLiteralList(literalList *src)
{
    literalList *head, *prev, *newL;

    head = prev = NULL;

    while (src)
    {
        newL = Safe_alloc(sizeof(literalList));

        newL->literalValue = src->literalValue;
        newL->count = src->count;
        newL->next = NULL;

        if (prev)
        {
            prev->next = newL;
        }
        else
        {
            head = newL;
        }
        prev = newL;
        src = src->next;
    }

    return head;
}



/*------------------------------------------------------------------*/
/* copyIlist - copy initializer list            */
/*------------------------------------------------------------------*/
initList *
copyIlist (initList * src)
{
  initList *dest = NULL;

  if (!src)
    return NULL;

  switch (src->type)
    {
    case INIT_DEEP:
      dest = newiList (INIT_DEEP, copyIlist (src->init.deep));
      break;
    case INIT_NODE:
      dest = newiList (INIT_NODE, copyAst (src->init.node));
      break;
    }

  if (src->next)
    dest->next = copyIlist (src->next);

  return dest;
}

/*------------------------------------------------------------------*/
/* list2int - converts the first element of the list to value       */
/*------------------------------------------------------------------*/
double
list2int (initList * val)
{
  initList *i = val;

  if (i->type == INIT_DEEP)
    return list2int (val->init.deep);

  return floatFromVal (constExprValue (val->init.node, TRUE));
}

/*------------------------------------------------------------------*/
/* list2val - converts the first element of the list to value       */
/*------------------------------------------------------------------*/
value *
list2val (initList * val)
{
  if (!val)
    return NULL;

  if (val->type == INIT_DEEP)
    return list2val (val->init.deep);

  return constExprValue (val->init.node, TRUE);
}

/*------------------------------------------------------------------*/
/* list2expr - returns the first expression in the initializer list */
/*------------------------------------------------------------------*/
ast *
list2expr (initList * ilist)
{
  if (ilist->type == INIT_DEEP)
    return list2expr (ilist->init.deep);
  return ilist->init.node;
}

/*------------------------------------------------------------------*/
/* resolveIvalSym - resolve symbols in initial values               */
/*------------------------------------------------------------------*/
void
resolveIvalSym (initList * ilist, sym_link * type)
{
  int is_ptr = IS_PTR (type);
  RESULT_TYPE resultType = getResultTypeFromType (getSpec (type));

  while (ilist)
    {
      if (ilist->type == INIT_NODE)
        {
          ilist->init.node = decorateType (resolveSymbols (ilist->init.node),
            is_ptr ? RESULT_TYPE_INT : resultType);
        }
      else if (ilist->type == INIT_DEEP)
        {
          resolveIvalSym (ilist->init.deep, type);
        }

      ilist = ilist->next;
   }
}

/*------------------------------------------------------------------*/
/* symbolVal - creates a value for a symbol                         */
/*------------------------------------------------------------------*/
value *
symbolVal (symbol * sym)
{
  value *val;

  if (!sym)
    return NULL;

  val = newValue ();
  val->sym = sym;

  if (sym->type)
    {
      val->type = sym->type;
      val->etype = getSpec (val->type);
    }

  if (*sym->rname)
    {
        SNPRINTF (val->name, sizeof(val->name), "%s", sym->rname);
    }
  else
    {
        SNPRINTF (val->name, sizeof(val->name), "_%s", sym->name);
    }

  return val;
}

/*--------------------------------------------------------------------*/
/* cheapestVal - try to reduce 'signed int' to 'char'                 */
/*--------------------------------------------------------------------*/
static value *
cheapestVal (value *val)
{
  /* only int can be reduced */
  if (!IS_INT(val->type))
    return val;

  /* long must not be changed */
  if (SPEC_LONG(val->type))
    return val;

  /* unsigned must not be changed */
  if (SPEC_USIGN(val->type))
    return val;

  /* the only possible reduction is from signed int to (un)signed char,
     because it's automatically promoted back to signed int.

     a reduction from unsigned int to unsigned char is a bug,
     because an _unsigned_ char is promoted to _signed_ int! */
  if (SPEC_CVAL(val->type).v_int < -128 ||
      SPEC_CVAL(val->type).v_int >  255)
    {
      /* not in the range of (un)signed char */
      return val;
    }

  SPEC_NOUN(val->type) = V_CHAR;

  /* 'unsigned char' promotes to 'signed int', so that we can
     reduce it the other way */
  if (SPEC_CVAL(val->type).v_int >= 0)
    {
      SPEC_USIGN(val->type) = 1;
    }
  return (val);
}

/*--------------------------------------------------------------------*/
/* checkConstantRange - check if constant fits in numeric range of    */
/* var type in comparisons and assignments                            */
/*--------------------------------------------------------------------*/
CCR_RESULT
checkConstantRange (sym_link *var, sym_link *lit, int op, bool exchangeLeftRight)
{
  sym_link *reType;
  double litVal;
  int varBits;

  litVal = floatFromVal (valFromType (lit));
  varBits = bitsForType (var);

  /* sanity checks */
  if (   IS_FLOAT (var)
      || IS_FIXED (var))
    return CCR_OK;
  if (varBits < 1)
    return CCR_ALWAYS_FALSE;
  if (varBits > 32)
    return CCR_ALWAYS_TRUE;

  /* special: assignment */
  if (op == '=')
    {
      if (IS_BIT (var))
        return CCR_OK;

      if (getenv ("SDCC_VERY_PEDANTIC"))
        {
          if (SPEC_USIGN (var))
            {
              TYPE_TARGET_ULONG maxVal = 0xffffffffu >> (32 - varBits);

              if (   litVal < 0
                  || litVal > maxVal)
                return CCR_OVL;
              return CCR_OK;
            }
          else
            {
              TYPE_TARGET_LONG minVal = 0xffffffff << (varBits - 1);
              TYPE_TARGET_LONG maxVal = 0x7fffffff >> (32 - varBits);

              if (   litVal < minVal
                  || litVal > maxVal)
                return CCR_OVL;
              return CCR_OK;
            }
        }
      else
        {
          /* ignore signedness, e.g. allow everything
             from -127...+255 for (unsigned) char */
          TYPE_TARGET_LONG  minVal = 0xffffffff  << (varBits - 1);
          TYPE_TARGET_ULONG maxVal = 0xffffffffu >> (32 - varBits);

          if (   litVal < minVal
              || litVal > maxVal)
            return CCR_OVL;
          return CCR_OK;
        }
    }

  if (exchangeLeftRight)
    switch (op)
      {
        case EQ_OP:             break;
        case NE_OP:             break;
        case '>':   op = '<';   break;
        case GE_OP: op = LE_OP; break;
        case '<':   op = '>';   break;
        case LE_OP: op = GE_OP; break;
        default:                return CCR_ALWAYS_FALSE;
      }

  reType = computeType (var, lit, RESULT_TYPE_NONE, op);

  if (SPEC_USIGN (reType))
    {
      /* unsigned operation */
      TYPE_TARGET_ULONG minValP, maxValP, minValM, maxValM;
      TYPE_TARGET_ULONG opBitsMask = 0xffffffffu >> (32 - bitsForType (reType));

      if (SPEC_USIGN (lit) && SPEC_USIGN (var))
        {
          /* both operands are unsigned, this is easy */
          minValP = 0;
          maxValP = 0xffffffffu >> (32 - varBits);
          /* there's only range, just copy it to 2nd set */
          minValM = minValP;
          maxValM = maxValP;
        }
      else if (SPEC_USIGN (var))
        {
          /* lit is casted from signed to unsigned, e.g.:
               unsigned u;
                 u == (char) -17
              -> u == 0xffef'
          */
          minValP = 0;
          maxValP = 0xffffffffu >> (32 - varBits);
          /* there's only one range, just copy it to 2nd set */
          minValM = minValP;
          maxValM = maxValP;

          /* it's an unsigned operation */
          if (   IS_CHAR (reType)
              || IS_INT (reType))
            {
              /* make signed literal unsigned and
                 limit no of bits to size of return type */
              litVal = (TYPE_TARGET_ULONG) double2ul (litVal) & opBitsMask;
            }
        }
      else /* SPEC_USIGN (lit) */
        {
          /* var is casted from signed to unsigned, e.g.:
               signed char c;
                 c == (unsigned) -17
              -> c == 0xffef'

             The possible values after casting var
             split up in two, nonconsecutive ranges:

             minValP =      0;  positive range: 0...127
             maxValP =   0x7f;
             minValM = 0xff80;  negative range: -128...-1
             maxValM = 0xffff;
          */

          /* positive range */
          minValP = 0;
          maxValP = 0x7fffffffu >> (32 - varBits);

          /* negative range */
          minValM = 0xffffffff  << (varBits - 1);
          maxValM = 0xffffffffu; /* -1 */
          /* limit no of bits to size of return type */
          minValM &= opBitsMask;
          maxValM &= opBitsMask;
        }

      switch (op)
        {
          case EQ_OP:                   /* var == lit */
            if (   litVal <= maxValP
                && litVal >= minValP) /* 0 */
              return CCR_OK;
            if (   litVal <= maxValM
                && litVal >= minValM)
              return CCR_OK;
            return CCR_ALWAYS_FALSE;
          case NE_OP:                   /* var != lit */
            if (   litVal <= maxValP
                && litVal >= minValP) /* 0 */
              return CCR_OK;
            if (   litVal <= maxValM
                && litVal >= minValM)
              return CCR_OK;
            return CCR_ALWAYS_TRUE;
          case '>':                     /* var >  lit */
            if (litVal >= maxValM)
              return CCR_ALWAYS_FALSE;
            if (litVal <  minValP) /* 0 */
              return CCR_ALWAYS_TRUE;
            return CCR_OK;
          case GE_OP:                   /* var >= lit */
            if (litVal >  maxValM)
              return CCR_ALWAYS_FALSE;
            if (litVal <= minValP) /* 0 */
              return CCR_ALWAYS_TRUE;
            return CCR_OK;
          case '<':                     /* var <  lit */
            if (litVal >  maxValM)
              return CCR_ALWAYS_TRUE;
            if (litVal <= minValP) /* 0 */
              return CCR_ALWAYS_FALSE;
            return CCR_OK;
          case LE_OP:                   /* var <= lit */
            if (litVal >= maxValM)
              return CCR_ALWAYS_TRUE;
            if (litVal <  minValP) /* 0 */
              return CCR_ALWAYS_FALSE;
            return CCR_OK;
          default:
            return CCR_ALWAYS_FALSE;
        }
    }
  else
    {
      /* signed operation */
      TYPE_TARGET_LONG minVal, maxVal;

      if (SPEC_USIGN (var))
        {
          /* unsigned var, but signed operation. This happens
             when var is promoted to signed int.
             Set actual min/max values of var. */
          minVal = 0;
          maxVal = 0xffffffff >> (32 - varBits);
        }
      else
        {
          /* signed var */
          minVal = 0xffffffff << (varBits - 1);
          maxVal = 0x7fffffff >> (32 - varBits);
        }

      switch (op)
        {
          case EQ_OP:                   /* var == lit */
            if (   litVal > maxVal
                || litVal < minVal)
              return CCR_ALWAYS_FALSE;
            return CCR_OK;
          case NE_OP:                   /* var != lit */
            if (   litVal > maxVal
                || litVal < minVal)
              return CCR_ALWAYS_TRUE;
            return CCR_OK;
          case '>':                     /* var >  lit */
            if (litVal >= maxVal)
              return CCR_ALWAYS_FALSE;
            if (litVal <  minVal)
              return CCR_ALWAYS_TRUE;
            return CCR_OK;
          case GE_OP:                   /* var >= lit */
            if (litVal >  maxVal)
              return CCR_ALWAYS_FALSE;
            if (litVal <= minVal)
              return CCR_ALWAYS_TRUE;
            return CCR_OK;
          case '<':                     /* var <  lit */
            if (litVal >  maxVal)
              return CCR_ALWAYS_TRUE;
            if (litVal <= minVal)
              return CCR_ALWAYS_FALSE;
            return CCR_OK;
          case LE_OP:                   /* var <= lit */
            if (litVal >= maxVal)
              return CCR_ALWAYS_TRUE;
            if (litVal <  minVal)
              return CCR_ALWAYS_FALSE;
            return CCR_OK;
          default:
            return CCR_ALWAYS_FALSE;
        }
    }
}

/*-----------------------------------------------------------------*/
/* valueFromLit - creates a value from a literal                   */
/*-----------------------------------------------------------------*/
value *
valueFromLit (double lit)
{
  char buffer[50];

  if ((((TYPE_TARGET_LONG) lit) - lit) == 0)
    {
      SNPRINTF (buffer, sizeof(buffer), "%d", (TYPE_TARGET_LONG) lit);
      return constVal (buffer);
    }

  SNPRINTF (buffer, sizeof(buffer), "%f", lit);
  return constFloatVal (buffer);
}

/*-----------------------------------------------------------------*/
/* constFloatVal - converts a FLOAT constant to value              */
/*-----------------------------------------------------------------*/
value *
constFloatVal (char *s)
{
  value *val = newValue ();
  double sval;
  char *p;

  sval = strtod(s, &p);
  if (p == s)
    {
      werror (E_INVALID_FLOAT_CONST, s);
      return constVal ("0");
    }

  val->type = val->etype = newLink (SPECIFIER);
  SPEC_NOUN (val->type) = V_FLOAT;
  SPEC_SCLS (val->type) = S_LITERAL;
  SPEC_CVAL (val->type).v_float = sval;

  return val;
}

/*-----------------------------------------------------------------*/
/* constFixed16x16Val - converts a FIXED16X16 constant to value    */
/*-----------------------------------------------------------------*/
value *
constFixed16x16Val (char *s)
{
  value *val = newValue ();
  double sval;
  char *p;

  sval = strtod(s, &p);
  if (p == s)
    {
      werror (E_INVALID_FLOAT_CONST, s);
      return constVal ("0");
    }

  val->type = val->etype = newLink (SPECIFIER);
  SPEC_NOUN (val->type) = V_FLOAT;
  SPEC_SCLS (val->type) = S_LITERAL;
  SPEC_CVAL (val->type).v_fixed16x16 = fixed16x16FromDouble ( sval );

  return val;
}

/*-----------------------------------------------------------------*/
/* constVal - converts an INTEGER constant into a cheapest value   */
/*-----------------------------------------------------------------*/
value *constVal (const char *s)
{
  value *val;
  short hex = 0, octal = 0;
  double dval;

  val = newValue ();            /* alloc space for value   */

  val->type = val->etype = newLink (SPECIFIER); /* create the spcifier */
  SPEC_SCLS (val->type) = S_LITERAL;
  /* let's start with a signed char */
  SPEC_NOUN (val->type) = V_CHAR;
  SPEC_USIGN (val->type) = 0;

  hex = ((strchr (s, 'x') || strchr (s, 'X')) ? 1 : 0);

  /* set the octal flag   */
  if (!hex && *s == '0' && *(s + 1))
    octal = 1;

  errno = 0;
  if (hex || octal) {
    unsigned long sval;
    sval = strtoul (s, NULL, 0);
    dval = sval;
    if (errno) {
      dval = 4294967295.0;
      werror (W_INVALID_INT_CONST, s, dval);
    }
  } else {
    dval = strtod(s, NULL);
  }

  /* Setup the flags first */
  /* set the unsigned flag if 'uU' is found */
  if (strchr (s, 'u') || strchr (s, 'U')) {
    SPEC_USIGN (val->type) = 1;
  }

  /* set the b_long flag if 'lL' is found */
  if (strchr (s, 'l') || strchr (s, 'L')) {
    SPEC_NOUN (val->type) = V_INT;
    SPEC_LONG (val->type) = 1;
  } else {
    if (dval<0) { /* "-28u" will still be signed and negative */
      if (dval<-128) { /* check if we have to promote to int */
        SPEC_NOUN (val->type) = V_INT;
      }
      if (dval<-32768) { /* check if we have to promote to long int */
        SPEC_LONG (val->type) = 1;
      }
    } else { /* >=0 */
      if (dval>0xff ||  /* check if we have to promote to int */
          SPEC_USIGN (val->type)) { /* if it's unsigned, we can't use unsigned
                                     char. After an integral promotion it will
                                     be a signed int; this certainly isn't what
                                     the programer wants */
        SPEC_NOUN (val->type) = V_INT;
      }
      else { /* store char's always as unsigned; this helps other optimizations */
        SPEC_USIGN (val->type) = 1;
      }
      if (dval>0xffff && SPEC_USIGN (val->type)) { /* check if we have to promote to long */
        SPEC_LONG (val->type) = 1;
      }
      else if (dval>0x7fff && !SPEC_USIGN (val->type)) { /* check if we have to promote to long int */
        if ((hex || octal) && /* hex or octal constants may be stored in unsigned type */
            dval<=0xffff) {
          SPEC_USIGN (val->type) = 1;
        } else {
          SPEC_LONG (val->type) = 1;
          if (dval>0x7fffffff) {
            SPEC_USIGN (val->type) = 1;
          }
        }
      }
    }
  }

  /* check for out of range */
  if (dval<-2147483648.0) {
    dval = -2147483648.0;
    werror (W_INVALID_INT_CONST, s, dval);
  }
  if (dval>2147483647.0 && !SPEC_USIGN (val->type)) {
    dval = 2147483647.0;
    werror (W_INVALID_INT_CONST, s, dval);
  }
  if (dval>4294967295.0) {
    dval = 4294967295.0;
    werror (W_INVALID_INT_CONST, s, dval);
  }

  if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        {
          SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) double2ul (dval);
        }
      else
        {
          SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) double2ul (dval);
        }
    }
  else
    {
      if (SPEC_USIGN (val->type))
        {
          SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) double2ul (dval);
        }
      else
        {
          SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) double2ul (dval);
        }
    }

  return val;
}

/*------------------------------------------------------------------*/
/* strVal - converts a string constant to a value       */
/*------------------------------------------------------------------*/
value *
strVal (const char *s)
{
  value *val;

  val = newValue ();            /* get a new one */

  /* get a declarator */
  val->type = newLink (DECLARATOR);
  DCL_TYPE (val->type) = ARRAY;
  val->type->next = val->etype = newLink (SPECIFIER);
  SPEC_NOUN (val->etype) = V_CHAR;
  SPEC_SCLS (val->etype) = S_LITERAL;

  SPEC_CVAL (val->etype).v_char = Safe_alloc (strlen (s) + 1);
  DCL_ELEM (val->type) = copyStr (SPEC_CVAL (val->etype).v_char, s);

  return val;
}


/*------------------------------------------------------------------*/
/* reverseValWithType - reverses value chain with type & etype      */
/*------------------------------------------------------------------*/
value *
reverseValWithType (value * val)
{
  sym_link *type;
  sym_link *etype;

  if (!val)
    return NULL;

  /* save the type * etype chains */
  type = val->type;
  etype = val->etype;

  /* set the current one 2b null */
  val->type = val->etype = NULL;
  val = reverseVal (val);

  /* restore type & etype */
  val->type = type;
  val->etype = etype;

  return val;
}

/*------------------------------------------------------------------*/
/* reverseVal - reverses the values for a value  chain        */
/*------------------------------------------------------------------*/
value *
reverseVal (value * val)
{
  value *prev, *curr, *next;

  if (!val)
    return NULL;

  prev = val;
  curr = val->next;

  while (curr)
    {
      next = curr->next;
      curr->next = prev;
      prev = curr;
      curr = next;
    }
  val->next = (void *) NULL;
  return prev;
}

/*------------------------------------------------------------------*/
/* copyValueChain - will copy a chain of values                     */
/*------------------------------------------------------------------*/
value *
copyValueChain (value * src)
{
  value *dest;

  if (!src)
    return NULL;

  dest = copyValue (src);
  dest->next = copyValueChain (src->next);

  return dest;
}

/*------------------------------------------------------------------*/
/* copyValue - copies contents of a value to a fresh one            */
/*------------------------------------------------------------------*/
value *
copyValue (value * src)
{
  value *dest;

  dest = newValue ();
  dest->sym = copySymbol (src->sym);
  strncpyz (dest->name, src->name, SDCC_NAME_MAX);
  dest->type = (src->type ? copyLinkChain (src->type) : NULL);
  dest->etype = (src->type ? getSpec (dest->type) : NULL);

  return dest;
}

/*------------------------------------------------------------------*/
/* charVal - converts a character constant to a value       */
/*------------------------------------------------------------------*/
value *
charVal (const char *s)
{
  value *val;

  val = newValue ();

  val->type = val->etype = newLink (SPECIFIER);
  SPEC_NOUN (val->type) = V_CHAR;
  SPEC_USIGN(val->type) = 1;
  SPEC_SCLS (val->type) = S_LITERAL;

  s++;                          /* get rid of quotation */
  /* if \ then special processing */
  if (*s == '\\')
    {
      s++;                      /* go beyond the backslash  */
      switch (*s)
        {
        case 'n':
          SPEC_CVAL (val->type).v_uint = '\n';
          break;
        case 't':
          SPEC_CVAL (val->type).v_uint = '\t';
          break;
        case 'v':
          SPEC_CVAL (val->type).v_uint = '\v';
          break;
        case 'b':
          SPEC_CVAL (val->type).v_uint = '\b';
          break;
        case 'r':
          SPEC_CVAL (val->type).v_uint = '\r';
          break;
        case 'f':
          SPEC_CVAL (val->type).v_uint = '\f';
          break;
        case 'a':
          SPEC_CVAL (val->type).v_uint = '\a';
          break;
        case '\\':
          SPEC_CVAL (val->type).v_uint = '\\';
          break;
        case '\?':
          SPEC_CVAL (val->type).v_uint = '\?';
          break;
        case '\'':
          SPEC_CVAL (val->type).v_uint = '\'';
          break;
        case '\"':
          SPEC_CVAL (val->type).v_uint = '\"';
          break;

        case '0' :
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
          SPEC_CVAL (val->type).v_uint = octalEscape(&s);
          break;

        case 'x':
          SPEC_CVAL (val->type).v_uint = hexEscape(&s) ;
          break;

        default:
          SPEC_CVAL (val->type).v_uint = (unsigned char)*s;
          break;
        }
    }
  else                          /* not a backslash */
    SPEC_CVAL (val->type).v_uint = (unsigned char)*s;

  return val;
}

/*------------------------------------------------------------------*/
/* valFromType - creates a value from type given                    */
/*------------------------------------------------------------------*/
value *
valFromType (sym_link * type)
{
  value *val = newValue ();
  val->type = copyLinkChain (type);
  val->etype = getSpec (val->type);
  return val;
}

/*------------------------------------------------------------------*/
/* floatFromVal - value to double float conversion                  */
/*------------------------------------------------------------------*/
double
floatFromVal (value * val)
{
  if (!val)
    return 0;

  if (val->etype && SPEC_SCLS (val->etype) != S_LITERAL)
    {
      werror (E_CONST_EXPECTED, val->name);
      return 0;
    }

  /* if it is not a specifier then we can assume that */
  /* it will be an unsigned long                      */
  if (!IS_SPEC (val->type))
    return SPEC_CVAL (val->etype).v_ulong;

  if (SPEC_NOUN (val->etype) == V_FLOAT)
    return SPEC_CVAL (val->etype).v_float;

  if (SPEC_NOUN (val->etype) == V_FIXED16X16)
    return doubleFromFixed16x16 (SPEC_CVAL (val->etype).v_fixed16x16);

  if (SPEC_LONG (val->etype))
    {
      if (SPEC_USIGN (val->etype))
        return SPEC_CVAL (val->etype).v_ulong;
      else
        return SPEC_CVAL (val->etype).v_long;
    }

  if (SPEC_NOUN (val->etype) == V_INT)
    {
      if (SPEC_USIGN (val->etype))
        return SPEC_CVAL (val->etype).v_uint;
      else
        return SPEC_CVAL (val->etype).v_int;
    }

  if (SPEC_NOUN (val->etype) == V_CHAR)
    {
      if (SPEC_USIGN (val->etype))
        return (unsigned char) SPEC_CVAL (val->etype).v_uint;
      else
        return (signed char) SPEC_CVAL (val->etype).v_int;
    }

  if (IS_BITVAR(val->etype))
    return SPEC_CVAL (val->etype).v_uint;

  if (SPEC_NOUN (val->etype) == V_VOID)
    return SPEC_CVAL (val->etype).v_ulong;

  /* we are lost ! */
  werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "floatFromVal: unknown value");
  return 0;
}

/*------------------------------------------------------------------*/
/* ulFromVal - value to unsigned long conversion                    */
/*------------------------------------------------------------------*/
unsigned long
ulFromVal (value * val)
{
  if (!val)
    return 0;

  if (val->etype && SPEC_SCLS (val->etype) != S_LITERAL)
    {
      werror (E_CONST_EXPECTED, val->name);
      return 0;
    }

  /* if it is not a specifier then we can assume that */
  /* it will be an unsigned long                      */
  if (!IS_SPEC (val->type))
    return SPEC_CVAL (val->etype).v_ulong;

  if (SPEC_NOUN (val->etype) == V_FLOAT)
    return double2ul (SPEC_CVAL (val->etype).v_float);

  if (SPEC_NOUN (val->etype) == V_FIXED16X16)
    return double2ul (doubleFromFixed16x16 (SPEC_CVAL (val->etype).v_fixed16x16));

  if (SPEC_LONG (val->etype))
    {
      if (SPEC_USIGN (val->etype))
        return SPEC_CVAL (val->etype).v_ulong;
      else
        return SPEC_CVAL (val->etype).v_long;
    }

  if (SPEC_NOUN (val->etype) == V_INT)
    {
      if (SPEC_USIGN (val->etype))
        return SPEC_CVAL (val->etype).v_uint;
      else
        return SPEC_CVAL (val->etype).v_int;
    }

  if (SPEC_NOUN (val->etype) == V_CHAR)
    {
      if (SPEC_USIGN (val->etype))
        return (unsigned char) SPEC_CVAL (val->etype).v_uint;
      else
        return (signed char) SPEC_CVAL (val->etype).v_int;
    }

  if (IS_BITVAR(val->etype))
    return SPEC_CVAL (val->etype).v_uint;

  if (SPEC_NOUN (val->etype) == V_VOID)
    return SPEC_CVAL (val->etype).v_ulong;

  /* we are lost ! */
  werror (E_INTERNAL_ERROR, __FILE__, __LINE__, "ulFromVal: unknown value");
  return 0;
}

/*-----------------------------------------------------------------*/
/* doubleFromFixed16x16 - convert a fixed16x16 to double           */
/*-----------------------------------------------------------------*/
double doubleFromFixed16x16(TYPE_TARGET_ULONG value)
{
#if 0
  /* This version is incorrect negative values. */
  double tmp=0, exp=2;

    tmp = (value & 0xffff0000) >> 16;
    
    while(value) {
      value &= 0xffff;
      if(value & 0x8000)tmp += 1/exp;
      exp *= 2;
      value <<= 1;
    }
  
  return (tmp);
#else
  return ((double)(value * 1.0) / (double)(1UL << 16));
#endif
}

TYPE_TARGET_ULONG fixed16x16FromDouble(double value)
{
#if 0
  /* This version is incorrect negative values. */
  unsigned int tmp=0, pos=16;
  TYPE_TARGET_ULONG res;

    tmp = floor( value );
    res = tmp << 16;
    value -= tmp;
    
    tmp = 0;
    while(pos--) {
      value *= 2;
      if(value >= 1.0)tmp |= (1 << pos);
      value -= floor( value );
    }
  
    res |= tmp;

  return (res);
#else
  return  double2ul (value * (double)(1UL << 16));
#endif
}

/*------------------------------------------------------------------*/
/* valUnaryPM - does the unary +/- operation on a constant          */
/*------------------------------------------------------------------*/
value *
valUnaryPM (value * val)
{
  /* depending on type */
  if (SPEC_NOUN (val->etype) == V_FLOAT)
    SPEC_CVAL (val->etype).v_float = -1.0 * SPEC_CVAL (val->etype).v_float;
  else if (SPEC_NOUN (val->etype) == V_FIXED16X16)
    SPEC_CVAL (val->etype).v_fixed16x16 = (TYPE_TARGET_ULONG) -((long) SPEC_CVAL (val->etype).v_fixed16x16);
  else
    {
      if (SPEC_LONG (val->etype))
        {
          if (SPEC_USIGN (val->etype))
            SPEC_CVAL (val->etype).v_ulong = 0-SPEC_CVAL (val->etype).v_ulong;
          else
            SPEC_CVAL (val->etype).v_long = -SPEC_CVAL (val->etype).v_long;
        }
      else
        {
          if (SPEC_USIGN (val->etype))
            SPEC_CVAL (val->etype).v_uint = 0-SPEC_CVAL (val->etype).v_uint;
          else
            SPEC_CVAL (val->etype).v_int = -SPEC_CVAL (val->etype).v_int;

          if (SPEC_NOUN(val->etype) == V_CHAR)
            {
              /* promote to 'signed int', cheapestVal() might reduce it again */
              SPEC_USIGN(val->etype) = 0;
              SPEC_NOUN(val->etype) = V_INT;
            }
          return cheapestVal (val);
        }
    }
  return val;
}

/*------------------------------------------------------------------*/
/* valueComplement - complements a constant                         */
/*------------------------------------------------------------------*/
value *
valComplement (value * val)
{
  /* depending on type */
  if (SPEC_LONG (val->etype))
    {
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_ulong = ~SPEC_CVAL (val->etype).v_ulong;
      else
        SPEC_CVAL (val->etype).v_long = ~SPEC_CVAL (val->etype).v_long;
    }
  else
    {
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_uint = ~SPEC_CVAL (val->etype).v_uint;
      else
        SPEC_CVAL (val->etype).v_int = ~SPEC_CVAL (val->etype).v_int;

      if (SPEC_NOUN(val->etype) == V_CHAR)
        {
          /* promote to 'signed int', cheapestVal() might reduce it again */
          SPEC_USIGN(val->etype) = 0;
          SPEC_NOUN(val->etype) = V_INT;
        }
      return cheapestVal (val);
    }
  return val;
}

/*------------------------------------------------------------------*/
/* valueNot - complements a constant                                */
/*------------------------------------------------------------------*/
value *
valNot (value * val)
{
  /* depending on type */
  if (SPEC_LONG (val->etype))
    {
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_int = !SPEC_CVAL (val->etype).v_ulong;
      else
        SPEC_CVAL (val->etype).v_int = !SPEC_CVAL (val->etype).v_long;
    }
  else
    {
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_int = !SPEC_CVAL (val->etype).v_uint;
      else
        SPEC_CVAL (val->etype).v_int = !SPEC_CVAL (val->etype).v_int;

    }
  /* ANSI: result type is int, value is 0 or 1 */
  /* sdcc will hold this in an 'unsigned char' */
  SPEC_USIGN(val->etype) = 1;
  SPEC_LONG (val->etype) = 0;
  SPEC_NOUN(val->etype) = V_CHAR;
  return val;
}

/*------------------------------------------------------------------*/
/* valMult - multiply constants                                     */
/*------------------------------------------------------------------*/
value *
valMult (value * lval, value * rval)
{
  value *val;

  /* create a new value */
  val = newValue ();
  val->type = val->etype = computeType (lval->etype,
                                        rval->etype,
                                        RESULT_TYPE_INT,
                                        '*');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (IS_FLOAT (val->type))
    SPEC_CVAL (val->type).v_float = floatFromVal (lval) * floatFromVal (rval);
  else
  if (IS_FIXED16X16 (val->type))
    SPEC_CVAL (val->type).v_fixed16x16 = fixed16x16FromDouble(floatFromVal (lval) * floatFromVal (rval));
      /* signed and unsigned mul are the same, as long as the precision of the
         result isn't bigger than the precision of the operands. */
  else if (SPEC_LONG (val->type))
    SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) *
                                    (TYPE_TARGET_ULONG) ulFromVal (rval);
  else if (SPEC_USIGN (val->type)) /* unsigned int */
    {
      TYPE_TARGET_ULONG ul = (TYPE_TARGET_UINT) ulFromVal (lval) *
                             (TYPE_TARGET_UINT) ulFromVal (rval);

      SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ul;
      if (ul != (TYPE_TARGET_UINT) ul)
        werror (W_INT_OVL);
    }
  else /* signed int */
    {
      TYPE_TARGET_LONG l = (TYPE_TARGET_INT) floatFromVal (lval) *
                           (TYPE_TARGET_INT) floatFromVal (rval);

      SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) l;
      if (l != (TYPE_TARGET_INT) l)
        werror (W_INT_OVL);
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valDiv  - Divide   constants                                     */
/*------------------------------------------------------------------*/
value *
valDiv (value * lval, value * rval)
{
  value *val;

  if (floatFromVal (rval) == 0)
    {
      werror (E_DIVIDE_BY_ZERO);
      return rval;
    }

  /* create a new value */
  val = newValue ();
  val->type = val->etype = computeType (lval->etype,
                                        rval->etype,
                                        RESULT_TYPE_INT,
                                        '/');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (IS_FLOAT (val->type))
    SPEC_CVAL (val->type).v_float = floatFromVal (lval) / floatFromVal (rval);
  else
  if (IS_FIXED16X16 (val->type))
    SPEC_CVAL (val->type).v_fixed16x16 = fixed16x16FromDouble( floatFromVal (lval) / floatFromVal (rval) );
  else if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) /
          (TYPE_TARGET_ULONG) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) /
          (TYPE_TARGET_LONG) ulFromVal (rval);
    }
  else
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) /
          (TYPE_TARGET_UINT) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) ulFromVal (lval) /
          (TYPE_TARGET_INT) ulFromVal (rval);
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valMod  - Modulus  constants                                     */
/*------------------------------------------------------------------*/
value *
valMod (value * lval, value * rval)
{
  value *val;

  /* create a new value */
  val = newValue();
  val->type = val->etype = computeType (lval->etype,
                                        rval->etype,
                                        RESULT_TYPE_INT,
                                        '%');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) %
          (TYPE_TARGET_ULONG) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) %
          (TYPE_TARGET_LONG) ulFromVal (rval);
    }
  else
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) %
          (TYPE_TARGET_UINT) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) ulFromVal (lval) %
          (TYPE_TARGET_INT) ulFromVal (rval);
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valPlus - Addition constants                                     */
/*------------------------------------------------------------------*/
value *
valPlus (value * lval, value * rval)
{
  value *val;

  /* create a new value */
  val = newValue();
  val->type = val->etype = computeType (lval->etype,
                                        rval->etype,
                                        RESULT_TYPE_INT,
                                        '+');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (IS_FLOAT (val->type))
    SPEC_CVAL (val->type).v_float = floatFromVal (lval) + floatFromVal (rval);
  else
  if (IS_FIXED16X16 (val->type))
    SPEC_CVAL (val->type).v_fixed16x16 = fixed16x16FromDouble( floatFromVal (lval) + floatFromVal (rval) );
  else  if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) +
         (TYPE_TARGET_ULONG) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) +
          (TYPE_TARGET_LONG) ulFromVal (rval);
    }
  else
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) +
          (TYPE_TARGET_UINT) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) ulFromVal (lval) +
          (TYPE_TARGET_INT) ulFromVal (rval);
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valMinus - Addition constants                                    */
/*------------------------------------------------------------------*/
value *
valMinus (value * lval, value * rval)
{
  value *val;

  /* create a new value */
  val = newValue();
  val->type = val->etype = computeType (lval->etype,
                                        rval->etype,
                                        RESULT_TYPE_INT,
                                        '-');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (IS_FLOAT (val->type))
    SPEC_CVAL (val->type).v_float = floatFromVal (lval) - floatFromVal (rval);
  else
  if (IS_FIXED16X16 (val->type))
    SPEC_CVAL (val->type).v_fixed16x16 = fixed16x16FromDouble( floatFromVal (lval) - floatFromVal (rval) );
  else  if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) -
          (TYPE_TARGET_ULONG) ulFromVal (rval);
      else
        SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) -
          (TYPE_TARGET_LONG) ulFromVal (rval);
    }
  else
   {
     if (SPEC_USIGN (val->type))
       SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) -
         (TYPE_TARGET_UINT) ulFromVal (rval);
     else
       SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) ulFromVal (lval) -
         (TYPE_TARGET_INT) ulFromVal (rval);
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valShift - Shift left or right                                   */
/*------------------------------------------------------------------*/
value *
valShift (value * lval, value * rval, int lr)
{
  value *val;

  /* create a new value */
  val = newValue();
  val->type = val->etype = computeType (lval->etype,
                                        NULL,
                                        RESULT_TYPE_INT,
                                        'S');
  SPEC_SCLS (val->etype) = S_LITERAL; /* will remain literal */

  if (getSize (val->type) * 8 <= (TYPE_TARGET_ULONG) ulFromVal (rval) &&
       /* left shift */
      (lr ||
        /* right shift and unsigned */
       (!lr && SPEC_USIGN (rval->type))))
    {
      werror (W_SHIFT_CHANGED, (lr ? "left" : "right"));
    }

  if (SPEC_LONG (val->type))
    {
      if (SPEC_USIGN (val->type))
        {
          SPEC_CVAL (val->type).v_ulong = lr ?
            (TYPE_TARGET_ULONG) ulFromVal (lval) << (TYPE_TARGET_ULONG) ulFromVal (rval) : \
            (TYPE_TARGET_ULONG) ulFromVal (lval) >> (TYPE_TARGET_ULONG) ulFromVal (rval);
        }
      else
        {
          SPEC_CVAL (val->type).v_long = lr ?
            (TYPE_TARGET_LONG) ulFromVal (lval) << (TYPE_TARGET_ULONG) ulFromVal (rval) : \
            (TYPE_TARGET_LONG) ulFromVal (lval) >> (TYPE_TARGET_ULONG) ulFromVal (rval);
        }
    }
  else
    {
      if (SPEC_USIGN (val->type))
        {
          SPEC_CVAL (val->type).v_uint = lr ?
            (TYPE_TARGET_UINT) ulFromVal (lval) << (TYPE_TARGET_ULONG) ulFromVal (rval) : \
            (TYPE_TARGET_UINT) ulFromVal (lval) >> (TYPE_TARGET_ULONG) ulFromVal (rval);
        }
      else
        {
          SPEC_CVAL (val->type).v_int = lr ?
            (TYPE_TARGET_INT) ulFromVal (lval) << (TYPE_TARGET_ULONG) ulFromVal (rval) : \
            (TYPE_TARGET_INT) ulFromVal (lval) >> (TYPE_TARGET_ULONG) ulFromVal (rval);
        }
    }
  return cheapestVal (val);
}

/*------------------------------------------------------------------*/
/* valCompare- Compares two literal                                 */
/*------------------------------------------------------------------*/
value *
valCompare (value * lval, value * rval, int ctype)
{
  value *val;

  /* create a new value */
  val = newValue ();
  val->type = val->etype = newCharLink ();
  val->type->class = SPECIFIER;
  SPEC_NOUN (val->type) = V_CHAR;       /* type is char */
  SPEC_USIGN (val->type) = 1;
  SPEC_SCLS (val->type) = S_LITERAL;    /* will remain literal */

  switch (ctype)
    {
    case '<':
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) < floatFromVal (rval);
      break;

    case '>':
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) > floatFromVal (rval);
      break;

    case LE_OP:
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) <= floatFromVal (rval);
      break;

    case GE_OP:
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) >= floatFromVal (rval);
      break;

    case EQ_OP:
      if (SPEC_NOUN(lval->type) == V_FLOAT ||
          SPEC_NOUN(rval->type) == V_FLOAT)
        {
          SPEC_CVAL (val->type).v_int = floatFromVal (lval) == floatFromVal (rval);
        }
      else
      if (SPEC_NOUN(lval->type) == V_FIXED16X16 ||
          SPEC_NOUN(rval->type) == V_FIXED16X16)
        {
          SPEC_CVAL (val->type).v_int = floatFromVal (lval) == floatFromVal (rval);
        }
      else
        {
          /* integrals: ignore signedness */
          TYPE_TARGET_ULONG l, r;

          l = (TYPE_TARGET_ULONG) ulFromVal (lval);
          r = (TYPE_TARGET_ULONG) ulFromVal (rval);
          /* In order to correctly compare 'signed int' and 'unsigned int' it's
             neccessary to strip them to 16 bit.
             Literals are reduced to their cheapest type, therefore left and
             right might have different types. It's neccessary to find a
             common type: int (used for char too) or long */
          if (!IS_LONG (lval->etype) &&
              !IS_LONG (rval->etype))
            {
              r = (TYPE_TARGET_UINT) r;
              l = (TYPE_TARGET_UINT) l;
            }
          SPEC_CVAL (val->type).v_int = l == r;
        }
      break;
    case NE_OP:
      if (SPEC_NOUN(lval->type) == V_FLOAT ||
          SPEC_NOUN(rval->type) == V_FLOAT)
        {
          SPEC_CVAL (val->type).v_int = floatFromVal (lval) != floatFromVal (rval);
        }
      else
      if (SPEC_NOUN(lval->type) == V_FIXED16X16 ||
          SPEC_NOUN(rval->type) == V_FIXED16X16)
        {
          SPEC_CVAL (val->type).v_int = floatFromVal (lval) != floatFromVal (rval);
        }
      else
        {
          /* integrals: ignore signedness */
          TYPE_TARGET_ULONG l, r;

          l = (TYPE_TARGET_ULONG) ulFromVal (lval);
          r = (TYPE_TARGET_ULONG) ulFromVal (rval);
          /* In order to correctly compare 'signed int' and 'unsigned int' it's
             neccessary to strip them to 16 bit.
             Literals are reduced to their cheapest type, therefore left and
             right might have different types. It's neccessary to find a
             common type: int (used for char too) or long */
          if (!IS_LONG (lval->etype) &&
              !IS_LONG (rval->etype))
            {
              r = (TYPE_TARGET_UINT) r;
              l = (TYPE_TARGET_UINT) l;
            }
          SPEC_CVAL (val->type).v_int = l != r;
        }
      break;

    }

  return val;
}

/*------------------------------------------------------------------*/
/* valBitwise - Bitwise operation                                   */
/*------------------------------------------------------------------*/
value *
valBitwise (value * lval, value * rval, int op)
{
  value *val;

  /* create a new value */
  val = newValue ();
  val->type = computeType (lval->etype, rval->etype, RESULT_TYPE_CHAR, op);
  val->etype = getSpec (val->type);
  SPEC_SCLS (val->etype) = S_LITERAL;

  switch (op)
    {
    case '&':
      if (SPEC_LONG (val->type))
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) &
              (TYPE_TARGET_ULONG) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) &
              (TYPE_TARGET_LONG) ulFromVal (rval);
        }
      else
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) &
              (TYPE_TARGET_UINT) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_int = (TYPE_TARGET_INT) ulFromVal (lval) &
              (TYPE_TARGET_INT) ulFromVal (rval);
        }
      break;

    case '|':
      if (SPEC_LONG (val->type))
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) |
              (TYPE_TARGET_ULONG) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) |
              (TYPE_TARGET_LONG) ulFromVal (rval);
        }
      else
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) |
              (TYPE_TARGET_UINT) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_int =
              (TYPE_TARGET_INT) ulFromVal (lval) | (TYPE_TARGET_INT) ulFromVal (rval);
        }

      break;

    case '^':
      if (SPEC_LONG (val->type))
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_ulong = (TYPE_TARGET_ULONG) ulFromVal (lval) ^
              (TYPE_TARGET_ULONG) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_long = (TYPE_TARGET_LONG) ulFromVal (lval) ^
              (TYPE_TARGET_LONG) ulFromVal (rval);
        }
      else
        {
          if (SPEC_USIGN (val->type))
            SPEC_CVAL (val->type).v_uint = (TYPE_TARGET_UINT) ulFromVal (lval) ^
              (TYPE_TARGET_UINT) ulFromVal (rval);
          else
            SPEC_CVAL (val->type).v_int =
              (TYPE_TARGET_INT) ulFromVal (lval) ^ (TYPE_TARGET_INT) ulFromVal (rval);
        }
      break;
    }

  return cheapestVal(val);
}

/*------------------------------------------------------------------*/
/* valAndOr   - Generates code for and / or operation               */
/*------------------------------------------------------------------*/
value *
valLogicAndOr (value * lval, value * rval, int op)
{
  value *val;

  /* create a new value */
  val = newValue ();
  val->type = val->etype = newCharLink ();
  val->type->class = SPECIFIER;
  SPEC_SCLS (val->type) = S_LITERAL;    /* will remain literal */
  SPEC_USIGN (val->type) = 1;

  switch (op)
    {
    case AND_OP:
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) && floatFromVal (rval);
      break;

    case OR_OP:
      SPEC_CVAL (val->type).v_int = floatFromVal (lval) || floatFromVal (rval);
      break;
    }


  return val;
}

/*------------------------------------------------------------------*/
/* valCastLiteral - casts a literal value to another type           */
/*------------------------------------------------------------------*/
value *
valCastLiteral (sym_link * dtype, double fval)
{
  value *val;
  unsigned long l = double2ul (fval);

  if (!dtype)
    return NULL;

  val = newValue ();
  if (dtype)
    val->etype = getSpec (val->type = copyLinkChain (dtype));
  else
    {
      val->etype = val->type = newLink (SPECIFIER);
      SPEC_NOUN (val->etype) = V_VOID;
    }
  SPEC_SCLS (val->etype) = S_LITERAL;

  /* if it is not a specifier then we can assume that */
  /* it will be an unsigned long                      */
  if (!IS_SPEC (val->type))
    {
      SPEC_CVAL (val->etype).v_ulong = (TYPE_TARGET_ULONG) l;
      return val;
    }

  switch (SPEC_NOUN (val->etype))
    {
    case V_FLOAT:
      SPEC_CVAL (val->etype).v_float = fval;
      break;

    case V_FIXED16X16:
      SPEC_CVAL (val->etype).v_fixed16x16 = fixed16x16FromDouble (fval);
      break;

    case V_BIT:
    case V_SBIT:
      SPEC_CVAL (val->etype).v_uint = fval ? 1 : 0;
      break;

    case V_BITFIELD:
      l &= (0xffffffffu >> (32 - SPEC_BLEN (val->etype)));
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_uint = (TYPE_TARGET_UINT) l;
      else
        SPEC_CVAL (val->etype).v_int = (TYPE_TARGET_INT) l;
      break;

    case V_CHAR:
      if (SPEC_USIGN (val->etype))
        SPEC_CVAL (val->etype).v_uint = (TYPE_TARGET_UCHAR) l;
      else
        SPEC_CVAL (val->etype).v_int = (TYPE_TARGET_CHAR) l;
      break;

    default:
      if (SPEC_LONG (val->etype))
        {
          if (SPEC_USIGN (val->etype))
            SPEC_CVAL (val->etype).v_ulong = (TYPE_TARGET_ULONG) l;
          else
            SPEC_CVAL (val->etype).v_long = (TYPE_TARGET_LONG) l;
        }
      else
        {
          if (SPEC_USIGN (val->etype))
            SPEC_CVAL (val->etype).v_uint = (TYPE_TARGET_UINT) l;
          else
            SPEC_CVAL (val->etype).v_int = (TYPE_TARGET_INT) l;
        }
    }

  return val;
}

/*------------------------------------------------------------------*/
/* getNelements - determines # of elements from init list           */
/*------------------------------------------------------------------*/
int
getNelements (sym_link * type, initList * ilist)
{
  int i;

  if (!ilist)
    return 0;

  if (ilist->type == INIT_DEEP)
    ilist = ilist->init.deep;

  /* if type is a character array and there is only one
     (string) initialiser then get the length of the string */
  if (IS_ARRAY (type) && IS_CHAR (type->next) && !ilist->next)
    {
      ast *iast = ilist->init.node;
      value *v = (iast->type == EX_VALUE ? iast->opval.val : NULL);
      if (!v)
        {
          werror (E_CONST_EXPECTED);
          return 0;
        }

      if (IS_ARRAY (v->type) && IS_CHAR (v->etype))
        /* yep, it's a string */
        {
          return DCL_ELEM (v->type);
        }
    }

  i = 0;
  while (ilist)
    {
      i++;
      ilist = ilist->next;
    }
  return i;
}

/*-----------------------------------------------------------------*/
/* valForArray - returns a value with name of array index          */
/*-----------------------------------------------------------------*/
value *
valForArray (ast * arrExpr)
{
  value *val, *lval = NULL;
  char buffer[128];
  int size = getSize (arrExpr->left->ftype->next);
  /* if the right or left is an array
     resolve it first */
  if (IS_AST_OP (arrExpr->left))
    {
      if (arrExpr->left->opval.op == '[')
        lval = valForArray (arrExpr->left);
      else if (arrExpr->left->opval.op == '.')
        lval = valForStructElem (arrExpr->left->left,
                                 arrExpr->left->right);
      else if (arrExpr->left->opval.op == PTR_OP &&
               IS_ADDRESS_OF_OP (arrExpr->left->left))
        lval = valForStructElem (arrExpr->left->left->left,
                                 arrExpr->left->right);
      else
        return NULL;

    }
  else if (!IS_AST_SYM_VALUE (arrExpr->left))
    return NULL;

  if (!IS_AST_LIT_VALUE (arrExpr->right))
    return NULL;

  val = newValue ();
  if (!lval)
    {
        SNPRINTF (buffer, sizeof(buffer), "%s", AST_SYMBOL (arrExpr->left)->rname);
    }
  else
    {
        SNPRINTF (buffer, sizeof(buffer), "%s", lval->name);
    }

  SNPRINTF (val->name, sizeof(val->name), "(%s + %d)", buffer,
           (int) AST_LIT_VALUE (arrExpr->right) * size);

  val->type = newLink (DECLARATOR);
  if (SPEC_SCLS (arrExpr->left->etype) == S_CODE)
    DCL_TYPE (val->type) = CPOINTER;
  else if (SPEC_SCLS (arrExpr->left->etype) == S_XDATA)
    DCL_TYPE (val->type) = FPOINTER;
  else if (SPEC_SCLS (arrExpr->left->etype) == S_XSTACK)
    DCL_TYPE (val->type) = PPOINTER;
  else if (SPEC_SCLS (arrExpr->left->etype) == S_IDATA)
    DCL_TYPE (val->type) = IPOINTER;
  else if (SPEC_SCLS (arrExpr->left->etype) == S_EEPROM)
    DCL_TYPE (val->type) = EEPPOINTER;
  else
    DCL_TYPE (val->type) = POINTER;
  val->type->next = arrExpr->left->ftype->next;
  val->etype = getSpec (val->type);
  return val;
}

/*-----------------------------------------------------------------*/
/* valForStructElem - returns value with name of struct element    */
/*-----------------------------------------------------------------*/
value *
valForStructElem (ast * structT, ast * elemT)
{
  value *val, *lval = NULL;
  char buffer[128];
  symbol *sym;

  /* left could be furthur derefed */
  if (IS_AST_OP (structT))
    {
      if (structT->opval.op == '[')
        lval = valForArray (structT);
      else if (structT->opval.op == '.')
        lval = valForStructElem (structT->left, structT->right);
      else if (structT->opval.op == PTR_OP &&
               IS_ADDRESS_OF_OP (structT->left))
        lval = valForStructElem (structT->left->left,
                                 structT->right);
      else
        return NULL;
    }

  if (!IS_AST_SYM_VALUE (elemT))
    return NULL;

  if (!IS_STRUCT (structT->etype))
    return NULL;

  if ((sym = getStructElement (SPEC_STRUCT (structT->etype),
                               AST_SYMBOL (elemT))) == NULL)
    {
      return NULL;
    }

  val = newValue ();
  if (!lval)
    {
        SNPRINTF(buffer, sizeof(buffer), "%s", AST_SYMBOL (structT)->rname);
    }
  else
    {
        SNPRINTF (buffer, sizeof(buffer), "%s", lval->name);
    }

  SNPRINTF (val->name, sizeof(val->name), "(%s + %d)", buffer,
           (int) sym->offset);

  val->type = newLink (DECLARATOR);
  if (SPEC_SCLS (structT->etype) == S_CODE)
    DCL_TYPE (val->type) = CPOINTER;
  else if (SPEC_SCLS (structT->etype) == S_XDATA)
    DCL_TYPE (val->type) = FPOINTER;
  else if (SPEC_SCLS (structT->etype) == S_XSTACK)
    DCL_TYPE (val->type) = PPOINTER;
  else if (SPEC_SCLS (structT->etype) == S_IDATA)
    DCL_TYPE (val->type) = IPOINTER;
  else if (SPEC_SCLS (structT->etype) == S_EEPROM)
    DCL_TYPE (val->type) = EEPPOINTER;
  else
    DCL_TYPE (val->type) = POINTER;
  val->type->next = sym->type;
  val->etype = getSpec (val->type);
  return val;
}

/*-----------------------------------------------------------------*/
/* valForCastAggr - will return value for a cast of an aggregate   */
/*                  plus minus a constant                          */
/*-----------------------------------------------------------------*/
value *
valForCastAggr (ast * aexpr, sym_link * type, ast * cnst, int op)
{
  value *val;

  if (!IS_AST_SYM_VALUE (aexpr))
    return NULL;
  if (!IS_AST_LIT_VALUE (cnst))
    return NULL;

  val = newValue ();

  SNPRINTF (val->name, sizeof(val->name), "(%s %c %d)",
           AST_SYMBOL (aexpr)->rname, op,
           getSize (type->next) * (int) AST_LIT_VALUE (cnst));

  val->type = type;
  val->etype = getSpec (val->type);
  return val;
}

/*-----------------------------------------------------------------*/
/* valForCastAggr - will return value for a cast of an aggregate   */
/*                  with no constant                               */
/*-----------------------------------------------------------------*/
value *
valForCastArr (ast * aexpr, sym_link * type)
{
  value *val;

  if (!IS_AST_SYM_VALUE (aexpr))
    return NULL;

  val = newValue ();

  SNPRINTF (val->name, sizeof(val->name), "(%s)",
           AST_SYMBOL (aexpr)->rname);

  val->type = type;
  val->etype = getSpec (val->type);
  return val;
}
