/* bug-221168.c
 */
#include <testfwk.h>

#define XDATA

XDATA static char x[10][20];

XDATA char *
getAddrOfCell(unsigned char y, unsigned char z)
{
    return &x[y][z];
}

static void
testMultiDimensionalAddress(void)
{
    ASSERT(getAddrOfCell(5, 6) == (char *)x + 106);
}
