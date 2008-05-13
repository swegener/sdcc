/*-------------------------------------------------------------------------
  peep.c - source file for peephole optimizer helper functions

  Written By -  Philipp Klaus Krause

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
#include "SDCCicode.h"
#include "z80.h"
#include "SDCCglobl.h"
#include "SDCCpeeph.h"
#include "gen.h"

#define NOTUSEDERROR() do {werror(E_INTERNAL_ERROR, __FILE__, __LINE__, "error in notUsed()");} while(0)

/*#define D(_s) { printf _s; fflush(stdout); }*/
#define D(_s)

typedef enum
{
  S4O_CONDJMP,
  S4O_WR_OP,
  S4O_RD_OP,
  S4O_TERM,
  S4O_VISITED,
  S4O_ABORT,
  S4O_CONTINUE
} S4O_RET;

static struct
{
  lineNode *head;
} _G;

/*-----------------------------------------------------------------*/
/* univisitLines - clear "visited" flag in all lines               */
/*-----------------------------------------------------------------*/
static void
unvisitLines (lineNode *pl)
{
  for (; pl; pl = pl->next)
    pl->visited = FALSE;
}

#define AOP(op) op->aop
#define AOP_SIZE(op) AOP(op)->size

static bool
isReturned(const char *what)
{
  symbol *sym;
  sym_link *sym_lnk;
  int size;
  lineNode *l;

  if(strncmp(what, "iy", 2) == 0)
    return FALSE;
  if(strlen(what) != 1)
    return TRUE;

  l = _G.head;
  do
  {
    l = l->next;
  } while(l->ic->op != FUNCTION);

  sym = OP_SYMBOL(IC_LEFT(_G.head->next->next->ic));

  if(sym && IS_DECL(sym->type))
    {
      // Find size of return value.
      specifier *spec;
      if(sym->type->select.d.dcl_type != FUNCTION)
        NOTUSEDERROR();
      spec = &(sym->etype->select.s);
      if(spec->noun == V_VOID)
         size = 0;
      else if(spec->noun == V_CHAR)
         size = 1;
      else if(spec->noun == V_INT && !(spec->b_long))
         size = 2;
      else
        size = 4;

      // Check for returned pointer.
      sym_lnk = sym->type;
      while (sym_lnk && !IS_PTR (sym_lnk))
        sym_lnk = sym_lnk->next;
      if(IS_PTR(sym_lnk))
        size = 2;
    }
  else
    {
      NOTUSEDERROR();
      size = 4;
    }

  switch(*what)
    {
    case 'd':
      return(size >= 4);
    case 'e':
      return(size >= 3);
    case 'h':
      return(size >= 2);
    case 'l':
      return(size >= 1);
    default:
      return FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* incLabelJmpToCount - increment counter "jmpToCount" in entry    */
/* of the list labelHash                                           */
/*-----------------------------------------------------------------*/
static bool
incLabelJmpToCount (const char *label)
{
  labelHashEntry *entry;

  entry = getLabelRef (label, _G.head);
  if (!entry)
    return FALSE;
  entry->jmpToCount++;
  return TRUE;
}

/*-----------------------------------------------------------------*/
/* findLabel -                                                     */
/* 1. extracts label in the opcode pl                              */
/* 2. increment "label jump-to count" in labelHash                 */
/* 3. search lineNode with label definition and return it          */
/*-----------------------------------------------------------------*/
static lineNode *
findLabel (const lineNode *pl)
{
  char *p;
  lineNode *cpl;

  /* 1. extract label in opcode */

  /* In each mcs51 jumping opcode the label is at the end of the opcode */
  p = strlen (pl->line) - 1 + pl->line;

  /* scan backward until ',' or '\t' */
  for (; p > pl->line; p--)
    if (*p == ',' || *p == '\t')
      break;

  /* sanity check */
  if (p == pl->line)
    {
      NOTUSEDERROR();
      return NULL;
    }

  /* skip ',' resp. '\t' */
  ++p;

  /* 2. increment "label jump-to count" */
  if (!incLabelJmpToCount (p))
    return NULL;

  /* 3. search lineNode with label definition and return it */
  for (cpl = _G.head; cpl; cpl = cpl->next)
    {
      if (   cpl->isLabel
          && strncmp (p, cpl->line, strlen(p)) == 0)
        {
          return cpl;
        }
    }
  return NULL;
}

static bool
z80MightRead(const lineNode *pl, const char *what)
{
  if(strcmp(pl->line, "call\t__initrleblock") == 0)
    return TRUE;

  if(strcmp(what, "iyl") == 0 || strcmp(what, "iyh") == 0)
    what = "iy";

  if(strncmp(pl->line, "call\t", 5) == 0 && strchr(pl->line, ',') == 0)
    return FALSE;

  if(strncmp(pl->line, "ret", 3) == 0 && !isReturned(what))
    return FALSE;

  if(strcmp(pl->line, "ex\tde,hl") == 0 && strchr(what, 'h') == 0 && strchr(what, 'l') == 0 && strchr(what, 'd') == 0&& strchr(what, 'e') == 0)
    return FALSE;
  if(strncmp(pl->line, "ld\t", 3) == 0)
    {
      if(strstr(strchr(pl->line, ','), what) && strchr(pl->line, ',')[1] != '#')
        return TRUE;
      if(*(strchr(pl->line, ',') - 1) == ')' && strstr(pl->line + 3, what) && (strchr(pl->line, '#') == 0 || strchr(pl->line, '#') > strchr(pl->line, ',')))
        return TRUE;
      return FALSE;
    }

  if(strcmp(pl->line, "xor\ta,a") == 0)
    return FALSE;

  if(strncmp(pl->line, "adc\t", 4) == 0 ||
    strncmp(pl->line, "add\t", 4) == 0 ||
    strncmp(pl->line, "and\t", 4) == 0 ||
    strncmp(pl->line, "or\t", 3) == 0 ||
    strncmp(pl->line, "sbc\t", 4) == 0 ||
    strncmp(pl->line, "sub\t", 4) == 0 ||
    strncmp(pl->line, "xor\t", 4) == 0)
    {
      if( strstr(pl->line + 3, what) == 0 && strcmp("a", what))
        return FALSE;
    }

  if(strncmp(pl->line, "pop\t", 4) == 0)
    return FALSE;

  if(strncmp(pl->line, "push\t", 5) == 0)
    return(strstr(pl->line + 5, what) != 0);

  if(
    strncmp(pl->line, "dec\t", 4) == 0 ||
    strncmp(pl->line, "inc\t", 4) == 0 ||
    strncmp(pl->line, "rl\t", 3) == 0 ||
    strncmp(pl->line, "rr\t", 3) == 0 ||  
    strncmp(pl->line, "sla\t", 4) == 0 ||
    strncmp(pl->line, "srl\t", 4) == 0)
    {
       return (strstr(pl->line + 3, what) != 0);
    }

  if(strncmp(pl->line, "jp\t", 3) == 0 ||
    (bool)(strncmp(pl->line, "jr\t", 3)) == 0)
    return FALSE;

  if(strncmp(pl->line, "rla", 3) == 0 ||
    strncmp(pl->line, "rlca", 4) == 0)
    return(strcmp(what, "a") == 0);

  return TRUE;
}

static bool
z80UncondJump(const lineNode *pl)
{
  if((strncmp(pl->line, "jp\t", 3) == 0 ||
    strncmp(pl->line, "jr\t", 3) == 0) && strchr(pl->line, ',') == 0)
    return TRUE;
  return FALSE;
}

static bool
z80CondJump(const lineNode *pl)
{
  if((strncmp(pl->line, "jp\t", 3) == 0 ||
    strncmp(pl->line, "jr\t", 3) == 0) && strchr(pl->line, ',') != 0)
    return TRUE;
  return FALSE;
}

static bool
z80SurelyWrites(const lineNode *pl, const char *what)
{
  if(strcmp(pl->line, "xor\ta,a") == 0 && strcmp(what, "a") == 0)
    return TRUE;
  if(strncmp(pl->line, "ld\t", 3) == 0 && strncmp(pl->line + 3, "hl", 2) == 0 && (what[0] == 'h' || what[0] == 'l'))
    return TRUE;
  if(strncmp(pl->line, "ld\t", 3) == 0 && strncmp(pl->line + 3, "de", 2) == 0 && (what[0] == 'd' || what[0] == 'e'))
    return TRUE;
  if(strncmp(pl->line, "ld\t", 3) == 0 && strncmp(pl->line + 3, "bc", 2) == 0 && (what[0] == 'b' || what[0] == 'c'))
    return TRUE;
  if(strncmp(pl->line, "ld\t", 3) == 0 && strncmp(pl->line + 3, what, strlen(what)) == 0 && pl->line[3 + strlen(what)] == ',')
    return TRUE;
  if(strncmp(pl->line, "pop\t", 4) == 0 && strstr(pl->line + 4, what))
    return TRUE;
  if(strncmp(pl->line, "call\t", 5) == 0 && strchr(pl->line, ',') == 0)
    return TRUE;
  if(strcmp(pl->line, "ret") == 0)
    return TRUE;
  if(strncmp(pl->line, "ld\tiy", 5) == 0 && strncmp(what, "iy", 2) == 0)
    return TRUE;
  return FALSE;
}

static bool
z80SurelyReturns(const lineNode *pl)
{
  if(strcmp(pl->line, "\tret") == 0)
    return TRUE;
  return FALSE;
}

/*-----------------------------------------------------------------*/
/* scan4op - "executes" and examines the assembler opcodes,        */
/* follows conditional and un-conditional jumps.                   */
/* Moreover it registers all passed labels.                        */
/*                                                                 */
/* Parameter:                                                      */
/*    lineNode **pl                                                */
/*       scanning starts from pl;                                  */
/*       pl also returns the last scanned line                     */
/*    const char *pReg                                             */
/*       points to a register (e.g. "ar0"). scan4op() tests for    */
/*       read or write operations with this register               */
/*    const char *untilOp                                          */
/*       points to NULL or a opcode (e.g. "push").                 */
/*       scan4op() returns if it hits this opcode.                 */
/*    lineNode **plCond                                            */
/*       If a conditional branch is met plCond points to the       */
/*       lineNode of the conditional branch                        */
/*                                                                 */
/* Returns:                                                        */
/*    S4O_ABORT                                                    */
/*       on error                                                  */
/*    S4O_VISITED                                                  */
/*       hit lineNode with "visited" flag set: scan4op() already   */
/*       scanned this opcode.                                      */
/*    S4O_FOUNDOPCODE                                              */
/*       found opcode and operand, to which untilOp and pReg are   */
/*       pointing to.                                              */
/*    S4O_RD_OP, S4O_WR_OP                                         */
/*       hit an opcode reading or writing from pReg                */
/*    S4O_CONDJMP                                                  */
/*       hit a conditional jump opcode. pl and plCond return the   */
/*       two possible branches.                                    */
/*    S4O_TERM                                                     */
/*       acall, lcall, ret and reti "terminate" a scan.            */
/*-----------------------------------------------------------------*/
static S4O_RET
scan4op (lineNode **pl, const char *what, const char *untilOp,
         lineNode **plCond)
{
  for (; *pl; *pl = (*pl)->next)
    {
      if (!(*pl)->line || (*pl)->isDebug || (*pl)->isComment || (*pl)->isLabel)
        continue;
      D(("Scanning %s for %s\n", (*pl)->line, what));
      /* don't optimize across inline assembler,
         e.g. isLabel doesn't work there */
      if ((*pl)->isInline)
        return S4O_ABORT;

      if ((*pl)->visited)
        return S4O_VISITED;
      (*pl)->visited = TRUE;

      if(z80MightRead(*pl, what))
        {
          D(("S4O_RD_OP\n"));
          return S4O_RD_OP;
        }

      if(z80UncondJump(*pl))
        {
          *pl = findLabel (*pl);
            if (!*pl)
              {
                D(("S4O_ABORT\n"));
                return S4O_ABORT;
              }
        }
      if(z80CondJump(*pl))
        {
          *plCond = findLabel (*pl);
          if (!*plCond)
            {
              D(("S4O_ABORT\n"));
              return S4O_ABORT;
            }
          D(("S4O_CONDJMP\n"));
          return S4O_CONDJMP;
        }

      if(z80SurelyWrites(*pl, what))
        {
          D(("S4O_WR_OP\n"));
          return S4O_WR_OP;
        }

      /* Don't need to check for de, hl since z80MightRead() does that */
      if(z80SurelyReturns(*pl))
        {
          D(("S4O_TERM\n"));
          return S4O_TERM;
        }
    }
  D(("S4O_ABORT\n"));
  return S4O_ABORT;
}

/*-----------------------------------------------------------------*/
/* doTermScan - scan through area 2. This small wrapper handles:   */
/* - action required on different return values                    */
/* - recursion in case of conditional branches                     */
/*-----------------------------------------------------------------*/
static bool
doTermScan (lineNode **pl, const char *what)
{
  lineNode *plConditional;

  for (;; *pl = (*pl)->next)
    {
      switch (scan4op (pl, what, NULL, &plConditional))
        {
          case S4O_TERM:
          case S4O_VISITED:
          case S4O_WR_OP:
            /* all these are terminating condtions */
            return TRUE;
          case S4O_CONDJMP:
            /* two possible destinations: recurse */
              {
                lineNode *pl2 = plConditional;
                D(("CONDJMP trying other branch first\n"));
                if (!doTermScan (&pl2, what))
                  return FALSE;
                D(("Other branch OK.\n"));
              }
            continue;
          case S4O_RD_OP:
          default:
            /* no go */
            return FALSE;
        }
    }
}

static bool
isReg(const char *what)
{
  if(strcmp(what, "iyl") == 0 || strcmp(what, "iyh") == 0)
    return TRUE;
  if(strlen(what) != 1)
    return FALSE;
  switch(*what)
    {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'h':
    case 'l':
      return TRUE;
    }
  return FALSE;
}

static bool
isRegPair(const char *what)
{
  if(strlen(what) != 2)
    return FALSE;
  if(strcmp(what, "bc") == 0)
    return TRUE;
  if(strcmp(what, "de") == 0)
    return TRUE;
  if(strcmp(what, "hl") == 0)
    return TRUE;
  if(strcmp(what, "iy") == 0)
    return TRUE;
  return FALSE;
}

/* Check that what is never read after endPl. */

bool
z80notUsed (const char *what, lineNode *endPl, lineNode *head)
{
  lineNode *pl;
  D(("Checking for %s\n", what));
  if(isRegPair(what))
    {
      char low[2], high[2];
      low[0] = what[1];
      high[0] = what[0];
      low[1] = 0;
      high[1] = 0;
      if(strcmp(what, "iy") == 0)
        return(z80notUsed("iyl", endPl, head) && z80notUsed("iyh", endPl, head));
      return(z80notUsed(low, endPl, head) && z80notUsed(high, endPl, head));
    }

  if(!isReg(what))
    return FALSE;

  _G.head = head;

  unvisitLines (_G.head);

  pl = endPl->next;
  if (!doTermScan (&pl, what))
    return FALSE;

  return TRUE;
}

