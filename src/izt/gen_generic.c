#include "izt.h"
#include "gen.h"

static void
_genLabel (iCode * ic)
{
  iemit ("!tlabeldef", IC_LABEL (ic)->key + 100);
}

static void
_genGoto (iCode * ic)
{
  iemit ("jp !tlabel", IC_LABEL (ic)->key + 100);
}

static void
_genFunction (iCode * ic)
{
  symbol *sym = OP_SYMBOL (IC_LEFT (ic));

  // Create the function header
  iemit ("!functionheader", sym->name);
  iemit ("!functionlabeldef", sym->rname);

  if (sym->stack)
    {
      iemit ("!enterx", sym->stack);
    }
  else
    {
      iemit ("!enter");
    }
}

static void
_genReturn (iCode * ic)
{
  if (IC_LEFT (ic))
    {
      // Has a return value.  Load it up.
      iemit ("; PENDING: call the generic loader to setup the return value.");
    }

  if (ic->next && ic->next->op == LABEL && IC_LABEL (ic->next) == returnLabel)
    {
      // The next label is the return label.  Dont bother emitting a jump.
    }
  else
    {
      iemit ("jp !tlabel", returnLabel->key + 100);
    }
}

static EMITTER _base_emitters[] =
{
  {LABEL, _genLabel},
  {GOTO, _genGoto},
  {FUNCTION, _genFunction},
  {RETURN, _genReturn},
  {0, NULL}
};

void
izt_initBaseEmitters (hTab ** into)
{
  izt_addEmittersToHTab (into, _base_emitters);
}
