/** Simple test for the mul/div/mod operations.

    type: int, signed char, short
    storage: static,
    attr: volatile,
*/
#include <testfwk.h>

static void
testMul(void)
{
    {attr} {storage} {type} i;
    {type} result;

    i = 5;

    LOG(("i*5 == 25 = %u\n", (int)i*5));
    result = i*5;
    ASSERT(result == 25);
    LOG(("i*-4 == -20 = %u\n", (int)i*-4));
    ASSERT(i*-4 == -20);

    i = -10;
    LOG(("i*12 == -120 = %u\n", (int)i*12));
    ASSERT(i*12 == -120);
    LOG(("i*-3 == 30 = %u\n", (int)i*-3));
    ASSERT(i*-3 == 30);

    LOG(("30 == %u\n", (int)i*-3));
}

static void
testDiv(void)
{
    {attr} {storage} {type} i;

    i = 100;
    LOG(("i/5 == 20 = %u\n", (int)i/5));
    ASSERT(i/5 == 20);
    LOG(("i/-4 == -25 = %u\n", (int)i/-4));
    ASSERT(i/-4 == -25);

    i = -50;
    LOG(("i/25 == -2 = %u\n", (int)i/25));
    ASSERT(i/25 == -2);
    LOG(("i/-12 == 4 = %u\n", (int)i/-12));
    ASSERT(i/-12 == 4);
}

static void
testMod(void)
{
    {attr} {storage} {type} i;

    // Disabled the LOG functions due to a bug in sdcc involving
    // vaargs.
    i = 100;
    //    LOG(("i%%17 == 15 = %u\n", (int)(i%9)));
    ASSERT(i%17 == 15);

#if MOD_SIGN_FOLLOWS_DIVIDEND
    //    LOG(("i%%-7 == 2 = %u\n", (int)i%-7));
    ASSERT(i%-7 == 2);

    i = -49;
    //    LOG(("i%%3 == -1 = %u\n", (int)i%3));
    ASSERT(i%3 == -1);
    //    LOG(("i%%-5 == -4 = %u\n", (int)i%-5));
    ASSERT(i%-5 == -4);
#endif
}
