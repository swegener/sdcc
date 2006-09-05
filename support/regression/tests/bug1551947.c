/*
   bug1551947.c
*/

#include <testfwk.h>

xdata float z;
xdata float x = 1.0;

void
testBug(void)
{
    z = x * x;
    ASSERT( z == 1.0 );
}
