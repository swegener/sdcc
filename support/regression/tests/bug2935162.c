/*
   bug2935162.c
*/

#pragma disable_warning 184

#include <testfwk.h>
#include <limits.h>

const float a[] =
{
    LONG_MAX,       // .byte #0x00,#0x00,#0x00,#0x4F ; +2.147484e+09
    LONG_MIN,       // .byte #0x00,#0x00,#0x00,#0x4F ; +2.147484e+09 ?

    12345678901,    // .byte #0x00,#0x00,#0x80,#0x4F ; +4.294967e+09
    -12345678901,   // .byte #0x00,#0x00,#0x80,#0x3F ; +1.000000e+00 ?

    12345678901L,   // .byte #0x00,#0x00,#0x00,#0x4F ; +2.147484e+09
    -12345678901L,  // .byte #0x00,#0x00,#0x00,#0xCF ; -2.147484e+09

    2147483647L,    // .byte #0x00,#0x00,#0x00,#0x4F ; +2.147484e+09
    -2147483648L,   // .byte #0x00,#0x00,#0x00,#0xCF ; -2.147484e+09

    ULONG_MAX,      // .byte #0x00,#0x00,#0x80,#0x4F ; +4.294967e+09
    1.0,            // .byte #0x00,#0x00,#0x80,#0x3F ; +1.000000e+00

    0.0,            // .byte #0x00,#0x00,#0x00,#0x00 ; +0.000000e+00
    -0.0            // .byte #0x00,#0x00,#0x00,#0x80 ; -0.000000e+00
};

void testBug (void)
{
    volatile int right;

    right = -120;

    ASSERT(a[0] > +2.1e9 && a[0] < +2.2e9);
    ASSERT(a[1] < -2.1e9 && a[1] > -2.2e9);
    ASSERT(a[2] > +2.1e9);
    ASSERT(a[3] < -2.1e9);
    ASSERT(a[4] > +2.1e9);
    ASSERT(a[5] < -2.1e9);
    ASSERT(a[6] > +2.1e9);
    ASSERT(a[7] < -2.1e9);
    ASSERT(a[8] > +4.2e9);
}
