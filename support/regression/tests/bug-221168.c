/* bug-221168.c
 */
#include <testfwk.h>

#ifndef __SDCC_pdk14 // Not enough RAM
__xdata static char x[10][20];

__xdata char *
getAddrOfCell (unsigned char y, unsigned char z)
{
  return &x[y][z];
}
#endif

static void
testMultiDimensionalAddress (void)
{
#ifndef __SDCC_pdk14 // Not enough RAM
  ASSERT (getAddrOfCell (5, 6) == (char __xdata *)x + 106);
#endif
}

