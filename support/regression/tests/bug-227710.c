/* bug-227710.c
 */
#include <testfwk.h>

static unsigned char _data[] = {
    1, 2, 3, 4
};

unsigned char *p; 

struct { 
    unsigned char index; 
} s; 

static unsigned char 
foo(void) 
{ 
    // BUG, there will be a PRE-increment 
    return p[s.index++];
} 

static void
testPostIncrement(void)
{
    p = _data;
    ASSERT(foo() == 1);
    ASSERT(foo() == 2);
    ASSERT(foo() == 3);
    ASSERT(foo() == 4);
}
