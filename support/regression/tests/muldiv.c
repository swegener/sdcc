/** Simple test for the mul/div/mod operations.

    type: int
    storage: , static
*/
#include <testfwk.h>

static void
testMul(void)
{
#if SDCC
    // Disabled as the z80 port is broken
#else
    volatile {storage} {type} i;

    i = 5;
    ASSERT(i*5 == 25);
    ASSERT(i*-4 == -20);

    i = -10;
    ASSERT(i*12 == -120);
    ASSERT(i*-3 == 30);
#endif
}

static void
testDiv(void)
{
#if SDCC
    // Disabled as the z80 port is broken
#else
    volatile {storage} {type} i;

    i = 100;
    ASSERT(i/5 == 20);
    ASSERT(i/-4 == -25);

    i = -50;
    ASSERT(i/25 == -2);
    ASSERT(i/-12 == 4);
#endif
}
