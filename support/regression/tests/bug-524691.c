/* Division by powers of two.
 */
#include <testfwk.h>

typedef unsigned int UINT;

typedef struct _HeapEntryState
{
  void *pBase;
  UINT uFlags;
} HeapEntryState;

static HeapEntryState *_getHeapEntryState(void *p, HeapEntryState *pStates, UINT nStateEntries)
{
  int uLeft = -1, uRight = nStateEntries, uMiddle;

  while (uRight - uLeft > 1)
    {
      int iDiff;

      uMiddle = (uLeft + uRight)/2;
      iDiff = pStates[uMiddle].pBase - p;

      if (iDiff > 0)
	{
	  uRight = uMiddle;
	}
      else if (iDiff < 0)
	{
	  uLeft = uMiddle;
	}
      else
	{
	  return pStates + uMiddle;
	}
    }

  return NULL;
}
