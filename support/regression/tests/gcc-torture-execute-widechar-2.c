/*
widechar-2.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

#include <stddef.h>

#if 0 // SDCC bug in handling wide string literals in initialization
const wchar_t ws[] = L"foo";
#endif

void
testTortureExecute (void)
{
#if 0
  if (ws[0] != L'f' || ws[1] != L'o' || ws[2] != L'o' || ws[3] != L'\0')
    ASSERT(0);
#endif
  return;
}
