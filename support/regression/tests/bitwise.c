/** Test the bitwise operators.

    type: char, short, long
    attr: volatile,
    storage: static,
 */
#include <testfwk.h>

static void
testTwoOpBitwise(void)
{
    {storage} {attr} {type} left, right;

    left = ({type})0x3df7;
    right = ({type})0xc1ec;

    ASSERT(({type})(left & right) == ({type})0x1E4);
    ASSERT(({type})(right & left) == ({type})0x1E4);
    ASSERT(({type})(left & 0xc1ec) == ({type})0x1E4);
    ASSERT(({type})(0x3df7 & right) == ({type})0x1E4);

    ASSERT(({type})(left | right) == ({type})0xFDFF);
    ASSERT(({type})(right | left) == ({type})0xFDFF);
    ASSERT(({type})(left | 0xc1ec) == ({type})0xFDFF);
    ASSERT(({type})(0x3df7 | right) == ({type})0xFDFF);

    ASSERT(({type})(left ^ right) == ({type})0xFC1B);
    ASSERT(({type})(right ^ left) == ({type})0xFC1B);
    ASSERT(({type})(left ^ 0xc1ec) == ({type})0xFC1B);
    ASSERT(({type})(0x3df7 ^ right) == ({type})0xFC1B);

#if defined (__GNUC__) && (__GNUC__ < 3)
    // long is 64 bits on GCC 2.95.4 on alpha and I don't know how to detect alpha...
    ASSERT(({type})(~left) == ({type})0xFFFFFFFFFFFFC208);
#else
    ASSERT(({type})(~left) == ({type})0xFFFFC208);
#endif
}
