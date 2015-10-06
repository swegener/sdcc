/* Tests character constants.
 */
#include <testfwk.h>

#ifdef __SDCC
#pragma std_c11
#endif

#include <stddef.h> // For wchar_t
#include <uchar.h> // For char16_t, char32_t

void
testCharConst(void)
{
#ifndef PORT_HOST // Too many old host compilers out there
  ASSERT (_Generic('c', default: 1, int: 0) == 0);
  ASSERT (_Generic(L'c', default: 1, wchar_t: 0) == 0);
  ASSERT (_Generic(u'c', default: 1, char16_t: 0) == 0);
  ASSERT (_Generic(U'c', default: 1, char32_t: 0) == 0);
#endif
}

