/** Simple test for increment

    type: signed char, int, long
    storage: static, 
*/
#include <testfwk.h>

static void
testIncrement(void)
{
    volatile {storage} {type} i;
    i = 0;
    i--;
    ASSERT(i == -1);
}
