/** Test new octal and escape syntax (C2y) */

#include <testfwk.h>

#ifdef __SDCC
_Pragma("std_c2y")
#endif

void testOctal(void)
{
#ifdef __SDCC
  ASSERT(0o377 == 255);
  ASSERT(0o10000 == 4096);
  ASSERT('\o{377}' == '\377');
  ASSERT('\x{ff}' == '\xff');
  ASSERT(u'\u2122' == u'\u{2122}');
#endif
}
