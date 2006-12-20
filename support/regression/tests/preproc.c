/*
   preproc.c
*/

#include <testfwk.h>

/*
 * test for bug 135170
 */
const char *
const_str(void)
{
return "A"



/* just for testing, actually nop */
#pragma save





"B";
#pragma restore
}

/*
 * test for bug 982435
 */
#if !defined (__GNUC__) && !defined (_MSC_VER)
/* since this fails on GCC cpp and MSVC cl -E... */
#define LO_B(x) ((x) & 0xff)

unsigned char
hexeminus(void)
{
unsigned char a=0x\
fe\
-\
LO_B(3);
return a;
}
#endif
