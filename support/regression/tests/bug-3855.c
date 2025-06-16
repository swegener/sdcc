/* bug-3855.c
   Looks like an issue in generalized constant propagation.
 */
 
#include <testfwk.h>

long g(long t) {
    if(t>=2000 && t<4250)
        t=(24*t-38580)/1000;
    return t;
}

void testBug(void) {
#ifndef __SDCC_pdk14 // Lack of RAM (just by a few B - otherwise, ww'd have to disable compilation of g above, too; let's hope future optimization makes this test possible for pdk14) */
    long r = g(4000);
#if 0 // Bug not yet fixed.
    ASSERT( r == 57 ); // 24*4000-38580=57420
#endif
#endif
}

