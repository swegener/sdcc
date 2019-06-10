#include "common.h"
#include "SDCCgen.h"

#include "peep.h"

#define NOTUSEDERROR() do {werror(E_INTERNAL_ERROR, __FILE__, __LINE__, "error in notUsed()");} while(0)

// #define D(_s) { printf _s; fflush(stdout); }
#define D(_s)

#define ISINST(l, i) (!STRNCASECMP((l), (i), sizeof(i) - 1) && (!(l)[sizeof(i) - 1] || isspace((unsigned char)((l)[sizeof(i) - 1]))))

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

  /* In each jump the label is at the end */
  p = strlen (pl->line) - 1 + pl->line;

  /* Skip trailing whitespace */
  while(isspace(*p))
    p--;

  /* scan backward until space or ',' */
  for (; p > pl->line; p--)
    if (isspace(*p) || *p == ',')
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

/* Check if reading arg implies reading what. */
static bool argCont(const char *arg, const char *what)
{
  if (arg == NULL || strlen (arg) == 0)
    return false;

  while (isblank ((unsigned char)(arg[0])))
    arg++;

  if (arg[0] == ',')
    arg++;

  while (isblank ((unsigned char)(arg[0])))
    arg++;

  return (!strcmp(arg, what));
}

static bool
stm8MightReadFlag(const lineNode *pl, const char *what)
{
  if (ISINST (pl->line, "push") && argCont (pl->line + 4, "af") || ISINST (pl->line, "pushaf"))
    return true;

  if (ISINST (pl->line, "t0sn") || ISINST (pl->line, "t1sn"))
    return argCont(strchr (pl->line, ','), what);

  if(ISINST (pl->line, "addc") ||
    ISINST (pl->line, "subc"))
    return !strcmp(what, "c");

  return false;
}

static bool
pdkMightRead(const lineNode *pl, const char *what)
{
  if (!strcmp(what, "z") || !strcmp(what, "c") || !strcmp(what, "ac") || !strcmp(what, "ov"))
    return (stm8MightReadFlag(pl, what));
  else if (strcmp(what, "a") && strcmp(what, "p"))
    return true; 

  return true;
}

static bool
stm8SurelyWritesFlag(const lineNode *pl, const char *what)
{
  if (ISINST (pl->line, "pop") && argCont (pl->line + 4, "af") || ISINST (pl->line, "popaf"))
    return true;

  // Instructions that write all flags
  if (ISINST (pl->line, "add") ||
    ISINST (pl->line, "addc") ||
    ISINST (pl->line, "ceqsn") ||
    ISINST (pl->line, "cneqsn") ||
    ISINST (pl->line, "dec") ||
    ISINST (pl->line, "dzsn") ||
    ISINST (pl->line, "inc") ||
    ISINST (pl->line, "izsn") ||
    ISINST (pl->line, "sub") ||
    ISINST (pl->line, "subc"))
      return true;

  // Instructions that write c only
  if (ISINST (pl->line, "sl") ||
    ISINST (pl->line, "slc") ||
    ISINST (pl->line, "sr") ||
    ISINST (pl->line, "src"))
      return !strcmp(what, "c");

  // Instructions that write z only
  if (ISINST (pl->line, "and") ||
    ISINST (pl->line, "neg") ||
    ISINST (pl->line, "not") ||
    ISINST (pl->line, "or") ||
    ISINST (pl->line, "xor"))
      return !strcmp(what, "z");

  // mov writes z when the destination is a only.
  if (ISINST (pl->line, "mov") && !strcmp(what, "z") && pl->line[4] == 'a' && pl->line[5] == ',')
    return true;

  return false;
}

static bool
pdkSurelyWrites(const lineNode *pl, const char *what)
{
  if (!strcmp(what, "z") || !strcmp(what, "c") || !strcmp(what, "ac") || !strcmp(what, "ov"))
    return (stm8SurelyWritesFlag(pl, what));

  return false;
}


static bool
pdkUncondJump(const lineNode *pl)
{
  return (ISINST(pl->line, "goto"));
}

static bool
pdkCondJump(const lineNode *pl)
{
  return (ISINST(pl->line, "ceqsn") || ISINST(pl->line, "cneqsn") ||
    ISINST(pl->line, "t0sn") || ISINST(pl->line, "t1sn") ||
    ISINST(pl->line, "izsn") || ISINST(pl->line, "dzsn"));
}

static bool
pdkSurelyReturns(const lineNode *pl)
{
  return(ISINST(pl->line, "ret"));
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
        {
          D(("S4O_ABORT at inline asm\n"));
          return S4O_ABORT;
        }

      if ((*pl)->visited)
        {
          D(("S4O_VISITED\n"));
          return S4O_VISITED;
        }

      (*pl)->visited = TRUE;

      if(pdkMightRead(*pl, what))
        {
          D(("S4O_RD_OP\n"));
          return S4O_RD_OP;
        }

      // Check writes before conditional jumps, some jumps (btjf, btjt) write 'c'
      if(pdkSurelyWrites(*pl, what))
        {
          D(("S4O_WR_OP\n"));
          return S4O_WR_OP;
        }

      if(pdkUncondJump(*pl))
        {
          *pl = findLabel (*pl);
            if (!*pl)
              {
                D(("S4O_ABORT at unconditional jump\n"));
                return S4O_ABORT;
              }
        }
      if(pdkCondJump(*pl))
        {
          *plCond = (*pl)->next->next;
          if (!*plCond)
            {
              D(("S4O_ABORT at conditional jump\n"));
              return S4O_ABORT;
            }
          D(("S4O_CONDJMP\n"));
          return S4O_CONDJMP;
        }

      /* Don't need to check for de, hl since stm8MightRead() does that */
      if(pdkSurelyReturns(*pl))
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
            /* all these are terminating conditions */
            return true;
          case S4O_CONDJMP:
            /* two possible destinations: recurse */
              {
                lineNode *pl2 = plConditional;
                D(("CONDJMP trying other branch first\n"));
                if (!doTermScan (&pl2, what))
                  return false;
                D(("Other branch OK.\n"));
              }
            continue;
          case S4O_RD_OP:
          default:
            /* no go */
            return false;
            return false;
        }
    }
}

/*-----------------------------------------------------------------*/
/* univisitLines - clear "visited" flag in all lines               */
/*-----------------------------------------------------------------*/
static void
unvisitLines (lineNode *pl)
{
  for (; pl; pl = pl->next)
    pl->visited = false;
}

bool pdknotUsed(const char *what, lineNode *endPl, lineNode *head)
{
  lineNode *pl;

  _G.head = head;

  unvisitLines (_G.head);

  pl = endPl->next;
  return (doTermScan (&pl, what));
}

bool pdknotUsedFrom(const char *what, const char *label, lineNode *head)
{
  lineNode *cpl;

  for (cpl = head; cpl; cpl = cpl->next)
    if (cpl->isLabel && !strncmp (label, cpl->line, strlen(label)))
      return (pdknotUsed (what, cpl, head));

  return false;
}

int
pdkinstructionSize(lineNode *pl)
{
  return 1;
}

