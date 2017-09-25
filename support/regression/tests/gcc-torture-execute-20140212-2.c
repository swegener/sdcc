/*
   20140212-2.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

/* This used to fail as we would convert f into just return (unsigned int)usVlanID
   which is wrong. */

int f(unsigned short usVlanID);
int f(unsigned short usVlanID)
{
  unsigned int uiVlanID = 0xffffffff;
  int i;
  if ((unsigned short)0xffff != usVlanID)
    uiVlanID = (unsigned int)usVlanID;
  return uiVlanID;
}

void
testTortureExecute (void)
{
  if (f(1) != 1)
    ASSERT (0);
  if (f(0xffff) != -1)
    ASSERT (0);
}
