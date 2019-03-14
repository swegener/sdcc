/*
   bug1551947.c
*/

#include <testfwk.h>

__xdata float z;
__xdata float x = 1.0;

void
testBug (void)
{
#ifndef __SDCC_pdk14 // Lack of memory
  z = x * x;
  ASSERT (z == 1.0);
#endif
}

