/** Tests covering the shift operators.

    type: char, int
    storage: static, 
    attr: volatile

    vals: 3

    pending - 1792, 851968, 1560281088, -3, -1792, -851968, -1560000000
*/
#include <testfwk.h>

static void
testShiftClasses(void)
{
    {attr} {storage} {type} i, result;

    i = 30;
    ASSERT(i>>3 == 3);
    ASSERT(i<<2 == 120);
    
    result = i;
    result >>= 2;
    ASSERT(result == 7);

    result = i;
    result <<= 2;
    ASSERT(result == 120);
}

/** PENDING: Disabled. */
static void
testShiftByteMultiples(void)
{
#if 0
    {attr} {storage} {type} i;

    i = ({type}){vals};
    ASSERT(i>>8  == (({type}){vals} >> 8));
    ASSERT(i>>16 == (({type}){vals} >> 16));
    ASSERT(i>>24 == (({type}){vals} >> 24));

    i = ({type}){vals};
    ASSERT(i<<8  == (({type}){vals} << 8));;
    ASSERT(i<<16 == (({type}){vals} << 16));
    ASSERT(i<<24 == (({type}){vals} << 24));
#endif
}

static void
testShiftOne(void)
{
    {attr} {storage} {type} i;
    {type} result;

    i = ({type}){vals};

    result = i >> 1;
    ASSERT(result == ({type})(({type}){vals} >> 1));

    result = i;
    result >>= 1;
    ASSERT(result == ({type})(({type}){vals} >> 1));

    result = i << 1;
    ASSERT(result == ({type})(({type}){vals} << 1));

    result = i;
    result <<= 1;
    ASSERT(result == ({type})(({type}){vals} << 1));
}
