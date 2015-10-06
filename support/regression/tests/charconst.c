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
  ASSERT (_Generic('c', default: 4, int: 0, wchar_t: 1, char16_t: 2, char32_t: 3) == 0);
  ASSERT (_Generic(L'c', default: 4, int: 0, wchar_t: 1, char16_t: 2, char32_t: 3) == 1);
  ASSERT (_Generic(u'c', default: 4, int: 0, wchar_t: 1, char16_t: 2, char32_t: 3) == 2);
  ASSERT (_Generic(U'c', default: 4, int: 0, wchar_t: 1, char16_t: 2, char32_t: 3) == 3);
}

